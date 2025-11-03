module;

#include <Windows.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

export module plugin.Plugin;

import plugin.Config;
import window.Window;

export class Plugin {
public:
    HANDLE mutex = nullptr;
    Window *window = nullptr;

private:
    std::atomic_bool refreshPending{false};
    std::mutex refreshMutex;
    std::chrono::steady_clock::time_point lastRefreshTime;
    static constexpr int MIN_REFRESH_INTERVAL_MS = 50; // 最小刷新间隔：50ms (20fps)

    Plugin() : lastRefreshTime(std::chrono::steady_clock::now()) {
        this->mutex = CreateMutex(nullptr, true, L"Global\\Taskbar-Lyrics-Musicfox");
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            CloseHandle(this->mutex);
            this->mutex = nullptr;
        } else {
            this->initialize();
        }
    }

    ~Plugin() {
        if (this->mutex) {
            ReleaseMutex(this->mutex);
            CloseHandle(this->mutex);
            this->mutex = nullptr;
        }
        if (this->window) {
            delete this->window;
            this->window = nullptr;
        }
    }

    auto initialize() -> void {
        std::thread([this] {
            this->window = new Window();
            this->window->create();

            // 等待 window 就绪（有 HWND）——短轮询
            for (int i = 0; i < 500 && !this->window->isReady(); ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            // 如果在 window 未就绪时有刷新请求，主动触发一次
            if (this->refreshPending.exchange(false)) {
                this->window->postRefresh();
            }

            this->window->runner();
        }).detach();
    }

public:
    static auto getInstance() -> Plugin & {
        static Plugin instance;
        return instance;
    }

    // 线程安全的刷新请求接口：若 window 就绪则发消息刷新，否则标记为待刷新
    // 使用节流机制防止过度刷新
    static auto refresh() -> void {
        auto &inst = Plugin::getInstance();

        if (inst.window && inst.window->isReady()) {
            // 使用节流：检查距离上次刷新是否已过最小间隔
            std::lock_guard<std::mutex> lock(inst.refreshMutex);
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - inst.lastRefreshTime).count();

            if (elapsed >= MIN_REFRESH_INTERVAL_MS) {
                // 已过最小间隔，立即刷新
                inst.window->postRefresh();
                inst.lastRefreshTime = now;
            } else {
                // 未达到最小间隔，标记为待刷新（会在下次有机会时刷新）
                inst.refreshPending.store(true, std::memory_order_relaxed);
            }
        } else {
            // 窗口未就绪，标记为待刷新
            inst.refreshPending.store(true, std::memory_order_relaxed);
        }
    }
};
