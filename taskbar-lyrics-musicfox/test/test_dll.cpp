#include <Windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

// Export function pointer type
typedef void (*SetLyricFunc)(const wchar_t*);

// Test lyrics
std::vector<std::wstring> testLyrics = {
 L"First lyric line",
L"Second lyric line",
    L"Third lyric line",
 L"Fourth lyric line"
};

int main() {
    std::wcout << L"========================================" << std::endl;
    std::wcout << L"Taskbar Lyrics Test Program" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << std::endl;

  // Load DLL from current directory first, then try relative paths
    HMODULE hDll = LoadLibrary(L"native.dll");
    if (!hDll) {
        std::wcerr << L"[WARN] Failed to load native.dll from current directory" << std::endl;
        std::wcerr << L"Error code: " << GetLastError() << std::endl;
   
        // Try relative path
        std::wcout << L"Trying relative path: ../../build/native.dll" << std::endl;
        hDll = LoadLibrary(L"../../build/native.dll");
        
if (!hDll) {
            std::wcerr << L"[ERROR] Still failed. Error code: " << GetLastError() << std::endl;
            std::wcerr << L"" << std::endl;
std::wcerr << L"Solutions:" << std::endl;
 std::wcerr << L"1. Build main project first (../build/native.dll)" << std::endl;
     std::wcerr << L"2. Or copy native.dll to current directory" << std::endl;
         std::wcerr << L"3. Or rebuild test project (it will auto-copy DLL)" << std::endl;
 std::wcout << std::endl;
        std::wcout << L"Press Enter to exit..." << std::endl;
   std::wcin.get();
            return -1;
        }
    }

    std::wcout << L"[SUCCESS] DLL loaded successfully!" << std::endl;
    std::wcout << std::endl;

    // Get export function
    SetLyricFunc setLyric = (SetLyricFunc)GetProcAddress(hDll, "SetLyric");
    if (!setLyric) {
        std::wcerr << L"[ERROR] Failed to find SetLyric function" << std::endl;
        std::wcerr << L"The DLL may be corrupted or incompatible" << std::endl;
        FreeLibrary(hDll);
        std::wcout << std::endl;
  std::wcout << L"Press Enter to exit..." << std::endl;
      std::wcin.get();
  return -1;
    }

    std::wcout << L"[SUCCESS] SetLyric function found!" << std::endl;
    std::wcout << L"" << std::endl;
    std::wcout << L"Initializing window..." << std::endl;

    // Wait for window initialization
 std::this_thread::sleep_for(std::chrono::seconds(2));

    std::wcout << L"" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << L"Displaying lyrics on taskbar" << std::endl;
    std::wcout << L"Press Ctrl+C to stop" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << std::endl;

    // Loop to update lyrics
    int index = 0;
    while (true) {
        setLyric(testLyrics[index].c_str());
    std::wcout << L"[" << index + 1 << L"/4] " << testLyrics[index] << std::endl;
        
        index = (index + 1) % testLyrics.size();
   std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    // Note: Don't call FreeLibrary as it will close the window
    // FreeLibrary(hDll);

    return 0;
}
