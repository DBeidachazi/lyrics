#include <Windows.h>
#include <functional>
#include <string>

export module window.Window;

import window.Renderer;

export class Window {
private:
    HWND hwnd = nullptr;
    Renderer renderer{};
    std::wstring currentLyric = L"等待歌词...";

    static auto CALLBACK WindowProc(const HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
      if (message == WM_CREATE) [[unlikely]] {
   const auto create = reinterpret_cast<LPCREATESTRUCT>(lParam);
            const auto window = static_cast<Window *>(create->lpCreateParams);
       SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        }
        if (const auto that = reinterpret_cast<Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA))) [[likely]] {
            return that->handleMessage(hwnd, message, wParam, lParam);
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    auto handleMessage(const HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
     switch (message) {
            case WM_CREATE: {
  this->hwnd = hwnd;
    this->renderer.initialize(hwnd);
    this->updatePosition();
      break;
            }
            case WM_SIZE: {
         const auto width = LOWORD(lParam);
  const auto height = HIWORD(lParam);
                this->renderer.resize(width, height);
        break;
            }
      case WM_PAINT: {
          this->renderer.render(this->currentLyric);
    ValidateRect(hwnd, nullptr);
  break;
}
            case WM_DESTROY: {
         PostQuitMessage(0);
      break;
      }
      default: return DefWindowProc(hwnd, message, wParam, lParam);
        }
  return 0;
    }

    auto findTaskbarWindow() -> HWND {
        // Find Windows 11 taskbar
        HWND taskbar = FindWindow(L"Shell_TrayWnd", nullptr);
        if (taskbar) {
     return taskbar;
  }
        return nullptr;
    }

    auto updatePosition() -> void {
        if (!this->hwnd) return;

        HWND taskbar = this->findTaskbarWindow();
        if (!taskbar) return;

RECT taskbarRect;
        GetWindowRect(taskbar, &taskbarRect);

      // Calculate position and size
        // Place the window on the left side of the taskbar, avoiding system tray
        int width = 600;
        int height = taskbarRect.bottom - taskbarRect.top;
        int x = taskbarRect.left + 100; // Offset from left edge
        int y = taskbarRect.top;

        SetWindowPos(this->hwnd, HWND_TOPMOST, x, y, width, height,
    SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }

public:
auto create() -> bool {
        const wchar_t* className = L"TaskbarLyricsMusicfox";

        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = Window::WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = className;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

  if (!RegisterClassEx(&wc)) {
            return false;
        }

    HWND taskbar = this->findTaskbarWindow();
 if (!taskbar) {
            return false;
        }

        // Create as a child window of taskbar
    this->hwnd = CreateWindowEx(
         WS_EX_NOPARENTNOTIFY | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_LAYERED,
          className,
            L"Taskbar Lyrics",
    WS_POPUP | WS_VISIBLE,
     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr,
   nullptr,
        GetModuleHandle(nullptr),
            this
        );

        if (!this->hwnd) {
  return false;
        }

        // Make window semi-transparent
  SetLayeredWindowAttributes(this->hwnd, 0, 230, LWA_ALPHA);

     return true;
    }

    auto runner() -> void {
        MSG msg = {};
      while (GetMessage(&msg, nullptr, 0, 0)) {
         TranslateMessage(&msg);
            DispatchMessage(&msg);
}
 }

    auto setLyric(const std::wstring& lyric) -> void {
        this->currentLyric = lyric;
   this->refresh();
    }

    auto refresh() -> void {
    if (this->hwnd) {
    this->updatePosition();
         InvalidateRect(this->hwnd, nullptr, FALSE);
        }
    }
};
