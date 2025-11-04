#include <Windows.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

import plugin.Plugin;
import plugin.Config;

// ✓ DONE: 任务栏图标位置判断 - AUTO模式下自动检测图标位置
//   - 图标居中时：歌词显示在左侧（Widgets按钮右侧到任务列表左侧）
//   - 图标居左时：歌词显示在右侧（任务列表右侧到托盘左侧）
// TODO 做当歌词需要换行时不换行，而是向右滚动，可以添加与下一次的歌词间隔时间自适应判断，etc.
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
        // 删除已有的日志文件，确保每次启动都是新的日志
        DeleteFileW(LOG_FILE);

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
extern "C" __declspec(dllexport) void SetConfig(const char* key, const char* value) {
    if (key && value) {
        std::string skey(key);
        std::string svalue(value);

        logDll("SetConfig(" + skey + ", " + svalue + ")");

        // setConfig 现在返回 bool，表示值是否发生变化
        bool changed = setConfig(skey, svalue);

        // 仅在值实际改变时才请求刷新
        if (changed) {
            Plugin::refresh();
        } else {
            logDll("SetConfig - value unchanged, skipping refresh");
        }
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
