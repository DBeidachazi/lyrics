module;

#include <Windows.h>
#include <thread>
#include <atomic>
#include <chrono>

export module plugin.Plugin;

import plugin.Config;
import window.Window;

export class Plugin {
public:
    HANDLE mutex = nullptr;
    Window *window = nullptr;

private:
    std::atomic_bool refreshPending{false};

    Plugin() {
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
    static auto refresh() -> void {
        auto &inst = Plugin::getInstance();
        if (inst.window && inst.window->isReady()) {
            // 窗口已就绪，立即发送刷新消息
            inst.window->postRefresh();
        } else {
            // 窗口未就绪，标记为待刷新
            inst.refreshPending.store(true, std::memory_order_relaxed);
        }
    }
};
