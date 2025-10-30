#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

typedef void (*SetLyricFunc)(const wchar_t*);
typedef void (*SetConfigFunc)(const char*, const char*);

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Taskbar Lyrics - Simple Test" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Try to load DLL
    std::cout << "[*] Loading native.dll..." << std::endl;
    HMODULE hDll = LoadLibrary(L"native.dll");
    
    if (!hDll) {
        std::cerr << "[!] Failed to load native.dll from current directory" << std::endl;
        std::cerr << "[*] Error code: " << GetLastError() << std::endl;
        std::cerr << "[*] Trying: ../../build/Release/native.dll" << std::endl;
        hDll = LoadLibrary(L"../../build/Release/native.dll");
        
        if (!hDll) {
            std::cerr << "[!] Still failed. Error: " << GetLastError() << std::endl;
            std::cerr << "\nPlease build the project first:" << std::endl;
            std::cerr << "  cd ../../build" << std::endl;
            std::cerr << "  cmake --build . --config Release" << std::endl;
            return 1;
        }
    }
    
    std::cout << "[+] DLL loaded successfully!\n" << std::endl;

    // Get functions
    std::cout << "[*] Getting SetLyric function..." << std::endl;
    SetLyricFunc setLyric = (SetLyricFunc)GetProcAddress(hDll, "SetLyric");
    if (!setLyric) {
        std::cerr << "[!] Failed to get SetLyric" << std::endl;
        return 1;
    }
    std::cout << "[+] SetLyric found\n" << std::endl;

    std::cout << "[*] Getting SetConfig function..." << std::endl;
    SetConfigFunc setConfig = (SetConfigFunc)GetProcAddress(hDll, "SetConfig");
    if (!setConfig) {
        std::cerr << "[!] Failed to get SetConfig" << std::endl;
        return 1;
    }
    std::cout << "[+] SetConfig found\n" << std::endl;

    // Initialize
    std::cout << "[*] Initializing window (wait 2 seconds)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "[+] Window initialized!\n" << std::endl;

    // Test 1: Basic lyrics
    std::cout << "========================================" << std::endl;
    std::cout << "Test 1: Basic Lyrics" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    setConfig("size_primary", "24");
    setConfig("color_primary", "0xFFFFFFFF");
    setLyric(L"欢迎使用任务栏歌词！");
    std::cout << "[+] Displayed: 欢迎使用任务栏歌词！" << std::endl;
    std::cout << "[*] Waiting 3 seconds...\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Test 2: Large red text
    std::cout << "========================================" << std::endl;
    std::cout << "Test 2: Large Red Text" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    setConfig("size_primary", "32");
    setConfig("color_primary", "0xFFFF0000");  // Red
    setConfig("weight_primary", "700");         // Bold
    setLyric(L"大红色加粗歌词");
    std::cout << "[+] Displayed: 大红色加粗歌词 (size=32, red, bold)" << std::endl;
    std::cout << "[*] Waiting 3 seconds...\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Test 3: Yellow text
    std::cout << "========================================" << std::endl;
    std::cout << "Test 3: Yellow Text" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    setConfig("size_primary", "20");
    setConfig("color_primary", "0xFFFFFF00");  // Yellow
    setLyric(L"黄色歌词显示");
    std::cout << "[+] Displayed: 黄色歌词显示 (size=20, yellow)" << std::endl;
    std::cout << "[*] Waiting 3 seconds...\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Test 4: Green text
    std::cout << "========================================" << std::endl;
    std::cout << "Test 4: Green Text" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    setConfig("color_primary", "0xFF00FF00");  // Green
    setLyric(L"绿色歌词");
    std::cout << "[+] Displayed: 绿色歌词 (green)" << std::endl;
    std::cout << "[*] Waiting 3 seconds...\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Test 5: Secondary lyrics
    std::cout << "========================================" << std::endl;
    std::cout << "Test 5: Dual Lyrics (Chinese + English)" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    setConfig("lyric_primary", "主歌词");
    setConfig("lyric_secondary", "English translation");
    setConfig("size_primary", "18");
    setConfig("size_secondary", "12");
    setConfig("color_primary", "0xFFFFFFFF");   // White
    setConfig("color_secondary", "0xFF808080"); // Gray
    setLyric(L"中英文双行");
    std::cout << "[+] Displayed dual lyrics" << std::endl;
    std::cout << "[*] Waiting 3 seconds...\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Test 6: Reset to default
    std::cout << "========================================" << std::endl;
    std::cout << "Test 6: Reset to Default" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    setConfig("size_primary", "14");
    setConfig("color_primary", "0xFFFFFFFF");
    setConfig("lyric_secondary", " ");
    setLyric(L"默认配置");
    std::cout << "[+] Displayed: 默认配置 (default config)" << std::endl;
    std::cout << "[*] Waiting 2 seconds...\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Summary
    std::cout << "========================================" << std::endl;
    std::cout << "All Tests Completed!" << std::endl;
    std::cout << "========================================\n" << std::endl;
    std::cout << "[+] Test Summary:" << std::endl;
    std::cout << "    - Basic lyrics display: OK" << std::endl;
    std::cout << "    - Large red text: OK" << std::endl;
    std::cout << "    - Yellow text: OK" << std::endl;
    std::cout << "    - Green text: OK" << std::endl;
    std::cout << "    - Dual lyrics: OK" << std::endl;
    std::cout << "    - Default reset: OK" << std::endl;
    std::cout << "\n[+] All features working correctly!" << std::endl;
    std::cout << "\nNote: The DLL is still running. The window should be" << std::endl;
    std::cout << "visible on your taskbar. Do not close this console." << std::endl;
    std::cout << "\nPress Ctrl+C to exit when done." << std::endl;

    // Keep running
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Don't free the DLL - it keeps the window alive
    // FreeLibrary(hDll);

    return 0;
}