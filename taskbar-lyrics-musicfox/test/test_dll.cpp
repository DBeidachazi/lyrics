#include <Windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <string>

// Export function pointer types
typedef void (*SetLyricFunc)(const wchar_t*);
typedef void (*SetConfigFunc)(const char*, const char*);

// Helper to convert wide string to UTF-8
std::string wideToUtf8(const wchar_t* wide) {
    if (!wide) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wide, -1, nullptr, 0, nullptr, nullptr);
    std::string result(size - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wide, -1, &result[0], size, nullptr, nullptr);
    return result;
}

// Test data
struct TestScenario {
    std::wstring title;
    std::vector<std::pair<std::string, std::string>> configs;
    std::vector<std::wstring> lyrics;
};

std::vector<TestScenario> scenarios = {
    {
        L"Default Settings",
        {
            {"lyric_primary", "欢迎使用任务栏歌词"},
            {"window_alignment", "0"},  // AUTO
        },
        {L"第一行歌词", L"第二行歌词", L"第三行歌词"}
    },
    {
        L"Large Font Size",
        {
            {"size_primary", "24"},
            {"color_primary", "0xFFFFFF00"},  // Yellow
        },
        {L"大字号显示", L"任务栏歌词"}
    },
    {
        L"Red Color",
        {
            {"color_primary", "0xFFFF0000"},  // Red
            {"size_primary", "18"},
        },
        {L"红色歌词", L"显示效果"}
    },
    {
        L"Left Alignment",
        {
            {"window_alignment", "1"},  // LEFT
            {"margin_left", "10"},
            {"margin_right", "10"},
        },
        {L"左对齐显示", L"带边距显示"}
    },
    {
        L"Center Alignment",
        {
            {"window_alignment", "2"},  // CENTER
        },
        {L"居中显示", L"完全居中对齐"}
    },
    {
        L"Right Alignment",
        {
            {"window_alignment", "3"},  // RIGHT
        },
        {L"右对齐显示", L"靠右显示歌词"}
    },
    {
        L"Secondary Lyrics",
        {
            {"lyric_primary", "主歌词"},
            {"lyric_secondary", "英文翻译"},
            {"size_primary", "16"},
            {"size_secondary", "12"},
            {"color_secondary", "0xFF808080"},  // Gray
        },
        {L"同时显示主副歌词"}
    },
    {
        L"Custom Font",
        {
            {"font_family", "Microsoft YaHei"},
            {"size_primary", "20"},
            {"color_primary", "0xFF00FF00"},  // Green
        },
        {L"自定义字体显示"}
    }
};

void printMenu() {
    std::wcout << L"\n========================================" << std::endl;
    std::wcout << L"任务栏歌词 - 测试程序" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << L"\n请选择测试场景：\n" << std::endl;
    
    for (size_t i = 0; i < scenarios.size(); i++) {
        std::wcout << L"  " << (i + 1) << L". " << scenarios[i].title << std::endl;
    }
    std::wcout << L"  0. 退出程序\n" << std::endl;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    std::wcout << L"========================================" << std::endl;
    std::wcout << L"加载 DLL..." << std::endl;
    std::wcout << L"========================================" << std::endl;

    // Load DLL
    HMODULE hDll = LoadLibrary(L"native.dll");
    if (!hDll) {
        std::wcout << L"[警告] 当前目录找不到 native.dll" << std::endl;
        std::wcout << L"尝试相对路径: ../../build/Release/native.dll" << std::endl;
        hDll = LoadLibrary(L"../../build/Release/native.dll");
        
        if (!hDll) {
            std::wcerr << L"[错误] 加载失败，错误代码: " << GetLastError() << std::endl;
            std::wcerr << L"\n解决方案：" << std::endl;
            std::wcerr << L"  1. 编译主项目 (build/Release/native.dll)" << std::endl;
            std::wcerr << L"  2. 复制 native.dll 到当前目录" << std::endl;
            std::wcerr << L"\n按 Enter 退出..." << std::endl;
            std::wcin.get();
            return -1;
        }
    }

    std::wcout << L"[成功] DLL 加载成功！\n" << std::endl;

    // Get export functions
    SetLyricFunc setLyric = (SetLyricFunc)GetProcAddress(hDll, "SetLyric");
    SetConfigFunc setConfig = (SetConfigFunc)GetProcAddress(hDll, "SetConfig");

    if (!setLyric || !setConfig) {
        std::wcerr << L"[错误] 无法找到导出函数" << std::endl;
        if (!setLyric) std::wcerr << L"  - SetLyric 未找到" << std::endl;
        if (!setConfig) std::wcerr << L"  - SetConfig 未找到" << std::endl;
        FreeLibrary(hDll);
        std::wcout << L"\n按 Enter 退出..." << std::endl;
        std::wcin.get();
        return -1;
    }

    std::wcout << L"[成功] 导出函数已加载！" << std::endl;
    std::wcout << L"  - SetLyric 函数" << std::endl;
    std::wcout << L"  - SetConfig 函数" << std::endl;

    // Wait for window initialization
    std::wcout << L"\n初始化窗口，请稍候 2 秒..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Main loop
    while (true) {
        printMenu();
        std::wcout << L"输入选项 (0-" << scenarios.size() << L"): ";
        
        int choice;
        std::wcin >> choice;
        std::wcin.ignore();  // Clear newline from input buffer

        if (choice == 0) {
            std::wcout << L"\n再见！" << std::endl;
            break;
        }

        if (choice < 1 || choice > static_cast<int>(scenarios.size())) {
            std::wcout << L"\n[错误] 无效的选项！" << std::endl;
            continue;
        }

        TestScenario& scenario = scenarios[choice - 1];

        std::wcout << L"\n========================================" << std::endl;
        std::wcout << L"测试场景: " << scenario.title << std::endl;
        std::wcout << L"========================================\n" << std::endl;

        // Apply configurations
        std::wcout << L"应用配置：" << std::endl;
        for (const auto& cfg : scenario.configs) {
            setConfig(cfg.first.c_str(), cfg.second.c_str());
            std::wcout << L"  ✓ " << std::string(cfg.first.begin(), cfg.first.end()).c_str() 
                      << L" = " << std::string(cfg.second.begin(), cfg.second.end()).c_str() << std::endl;
        }

        std::wcout << L"\n显示歌词 (3 秒/行)，按 Ctrl+C 中断：\n" << std::endl;

        // Display lyrics
        for (const auto& lyric : scenario.lyrics) {
            setLyric(lyric.c_str());
            std::wcout << L"  ♪ " << lyric << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        std::wcout << L"\n[完成] 场景测试结束\n" << std::endl;
        std::wcout << L"按 Enter 返回菜单..." << std::endl;
        std::wcin.get();
    }

    // Note: Don't call FreeLibrary as it will close the window
    // FreeLibrary(hDll);

    return 0;
}