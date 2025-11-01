module;

#include <Windows.h>
#include <functional>
#include <fstream>
#include <mutex>
#include <string>

export module window.Window;

import plugin.Config;
import taskbar.Taskbar;
import taskbar.Registry;
import window.Renderer;

// 日志功能
static std::mutex g_logMutex;
static const wchar_t* LOG_FILE = L"./dll.log";

static void logWindow(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::ofstream ofs(LOG_FILE, std::ios::app);
    if (ofs.is_open()) {
        ofs << "[Window] " << msg << std::endl;
    }
}

export class Window {
private:
    HWND hwnd = nullptr;
    static constexpr UINT WM_APP_REFRESH = WM_APP + 1;

    static auto CALLBACK WindowProc(const HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
        if (message == WM_CREATE) [[unlikely]] {
            const auto create = reinterpret_cast<LPCREATESTRUCT>(lParam);
            const auto window = static_cast<Window *>(create->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        }
        if (const auto that = reinterpret_cast<Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA))) [[likely]] {
            if (message == WM_APP_REFRESH) {
                logWindow("WindowProc - received WM_APP_REFRESH message");
                that->refresh();
                return 0;
            }
            return that->handleMessage(hwnd, message, wParam, lParam);
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    auto handleMessage(const HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
        switch (message) {
            case WM_CREATE: {
                this->hwnd = hwnd;
                this->taskbar.initialize();
                this->taskbar.setListener(std::bind(&Window::update, this));
                this->renderer.onCreate(hwnd);
                // 主动调用一次 update() 来初始化窗口尺寸和位置
                logWindow("WM_CREATE - calling update() to initialize window size");
                this->update();
                break;
            }
            case WM_SIZE: {
                const auto width = LOWORD(lParam);
                const auto height = HIWORD(lParam);
                const auto dpi = GetDpiForWindow(hwnd);
                logWindow("WM_SIZE - width: " + std::to_string(width) + 
                         ", height: " + std::to_string(height) + 
                         ", dpi: " + std::to_string(dpi));
                this->renderer.onSize(width, height, dpi);
                break;
            }
            case WM_PAINT: {
                this->renderer.onPaint();
                ValidateRect(hwnd, nullptr);
                break;
            }
            default: return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }

public:
    Renderer renderer{};
    Taskbar taskbar{};


    auto create() -> void {
        const auto dll_instance = GetModuleHandle(nullptr);
        const auto class_name = L"taskbar_lyrics_musicfox";
        RegisterClassEx(new WNDCLASSEX{
            .cbSize = sizeof(WNDCLASSEX),
            .lpfnWndProc = Window::WindowProc,
            .hInstance = dll_instance,
            .lpszClassName = class_name,
        });
        CreateWindowEx(
            WS_EX_NOPARENTNOTIFY | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_NOREDIRECTIONBITMAP,
            class_name,
            nullptr,
            WS_CHILD | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            Taskbar::getHWND(),
            nullptr,
            dll_instance,
            this
        );
    }

    auto runner() -> void {
        MSG msg{};
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    auto refresh() -> void {
        if (this->hwnd) {
            logWindow("refresh() called - directly calling onPaint");
            // 直接调用绘制，不依赖 Windows 消息机制
            this->renderer.onPaint();
            logWindow("refresh() completed");
        } else {
            logWindow("refresh() called but hwnd is null!");
        }
    }

    // 新增：窗口是否已就绪（是否有 HWND）
    auto isReady() const -> bool {
        return this->hwnd != nullptr;
    }

    // 新增：以异步消息请求刷新（线程安全）
    auto postRefresh() -> void {
        if (this->hwnd) {
            logWindow("postRefresh() - posting WM_APP_REFRESH message");
            PostMessage(this->hwnd, WM_APP_REFRESH, 0, 0);
        } else {
            logWindow("postRefresh() - hwnd is null!");
        }
    }

    auto update() -> void {
        if (this->hwnd == nullptr) [[unlikely]] {
            logWindow("update() called but hwnd is null!");
            return;
        }

        logWindow("update() - calculating window size and position");

        const auto taskbarFrame = this->taskbar.getRectForTaskbarFrame();
        const auto trayFrameRect = this->taskbar.getRectForTrayFrame();
        const auto widgetsButtonRect = this->taskbar.getRectForWidgetsButton();
        const auto taskListRect = this->taskbar.getRectForTaskList();

        logWindow("update() - taskbarFrame: left=" + std::to_string(taskbarFrame.left) + 
                 ", right=" + std::to_string(taskbarFrame.right) + 
                 ", top=" + std::to_string(taskbarFrame.top) + 
                 ", bottom=" + std::to_string(taskbarFrame.bottom));

        auto offset = 0L;
        auto width = 0L;
        auto height = 0L;

        switch (config.window_alignment) {
            case TASKBAR_WINDOW_ALIGNMENT::TASKBAR_WINDOW_ALIGNMENT_AUTO: {
                if (Registry::isTaskbarCentered()) {
                    width += taskListRect.left;
                    if (Registry::isWidgetsEnabled()) {
                        offset += widgetsButtonRect.right;
                    }
                    break;
                }
                [[fallthrough]];
            }
            case TASKBAR_WINDOW_ALIGNMENT::TASKBAR_WINDOW_ALIGNMENT_RIGHT: {
                offset += taskListRect.right;
                if (Registry::isTaskbarCentered()) {
                    width += trayFrameRect.left;
                } else if (Registry::isWidgetsEnabled()) {
                    width += widgetsButtonRect.left;
                } else {
                    width += trayFrameRect.left;
                }
                break;
            }
            case TASKBAR_WINDOW_ALIGNMENT::TASKBAR_WINDOW_ALIGNMENT_LEFT: {
                // 真正的左对齐：从 Widgets 按钮右侧或任务栏起始位置开始
                if (Registry::isWidgetsEnabled()) {
                    offset += widgetsButtonRect.right;
                    width += taskListRect.left;
                } else {
                    offset += taskbarFrame.left;
                    width += taskListRect.left;
                }
                break;
            }
            case TASKBAR_WINDOW_ALIGNMENT::TASKBAR_WINDOW_ALIGNMENT_CENTER: {
                offset += taskbarFrame.left;
                width += taskbarFrame.right;
                break;
            }
        }

        offset += config.margin_left;
        width -= config.margin_right + offset;
        height += taskbarFrame.bottom - taskbarFrame.top;

        logWindow("update() - final position: offset=" + std::to_string(offset) + 
                 ", width=" + std::to_string(width) + 
                 ", height=" + std::to_string(height));

        BringWindowToTop(this->hwnd);
        MoveWindow(this->hwnd, offset, 0, width, height, false);
        RedrawWindow(this->hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

        logWindow("update() - MoveWindow completed");
    }
};
