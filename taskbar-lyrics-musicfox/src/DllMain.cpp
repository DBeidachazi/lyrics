#include <Windows.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

import plugin.Plugin;
import plugin.Config;

// 全局日志锁与文件名
static std::mutex g_logMutex;
static const wchar_t* LOG_FILE = L"./dll.log";

// 线程安全日志函数
static void logDll(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::ofstream ofs(LOG_FILE, std::ios::app);
    if (ofs.is_open()) {
        ofs << "[DLL] " << msg << std::endl;
    }
}

// 可接受宽字符串的重载
static void logDll(const std::wstring& msg) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::ofstream ofs(LOG_FILE, std::ios::app);
    if (ofs.is_open()) {
        // 简单转换为 UTF-8 写入
        int size = WideCharToMultiByte(CP_UTF8, 0, msg.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string utf8(size, '\0');
        WideCharToMultiByte(CP_UTF8, 0, msg.c_str(), -1, utf8.data(), size, nullptr, nullptr);
        ofs << "[DLL] " << utf8.c_str() << std::endl;
    }
}

// -------------------------------------------------------------------
// DllMain
// -------------------------------------------------------------------
auto WINAPI DllMain(const HINSTANCE hInstance, const DWORD dwReason, const LPVOID lpReserved) -> BOOL {
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        logDll("Process attach");
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        break;

    case DLL_PROCESS_DETACH:
        logDll("Process detach");
        CoUninitialize();
        break;
    }
    return TRUE;
}

// -------------------------------------------------------------------
// 导出函数：SetConfig 
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// 导出函数：SetConfig 
// -------------------------------------------------------------------
extern "C" __declspec(dllexport) void SetConfig(const char* key, const char* value) {
    if (key && value) {
        std::string skey(key);
        std::string svalue(value);

        logDll("SetConfig(" + skey + ", " + svalue + ")");
        setConfig(skey, svalue);  // 调用你内部的 C++ 函数

        // 请求刷新（线程安全、如果 window 未就绪会延迟）
        Plugin::refresh();
    }
    else {
        logDll("SetConfig called with null arguments");
    }
}



// -------------------------------------------------------------------
// 导出函数：SetLyric（兼容旧接口）弃用
// -------------------------------------------------------------------
extern "C" __declspec(dllexport) void SetLyric(const wchar_t* lyric) {
    if (lyric) {
        logDll(L"SetLyric called");

        // 转 UTF-8
        int size = WideCharToMultiByte(CP_UTF8, 0, lyric, -1, nullptr, 0, nullptr, nullptr);
        if (size > 0) {
            std::string utf8(size, '\0');
            WideCharToMultiByte(CP_UTF8, 0, lyric, -1, utf8.data(), size, nullptr, nullptr);
            logDll(std::string("Converted lyric: ") + utf8);
            setConfig("lyric_primary", utf8.c_str());

            // 新增：歌词变更后请求刷新
            Plugin::refresh();
        }
        else {
            logDll("SetLyric conversion failed");
        }
    }
    else {
        logDll("SetLyric called with null lyric");
    }
}
