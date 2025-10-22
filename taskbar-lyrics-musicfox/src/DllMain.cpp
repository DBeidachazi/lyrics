#include <Windows.h>

import plugin.Plugin;

auto WINAPI DllMain(const HINSTANCE hInstance, const DWORD dwReason, const LPVOID lpReserved) -> BOOL {
    if (dwReason == DLL_PROCESS_ATTACH) {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        Plugin::getInstance();
    }
    if (dwReason == DLL_PROCESS_DETACH) {
        CoUninitialize();
    }
    return TRUE;
}

// Export function for setting lyrics from external applications
extern "C" __declspec(dllexport) void SetLyric(const wchar_t* lyric) {
    if (lyric) {
        Plugin::getInstance().setLyric(lyric);
    }
}

