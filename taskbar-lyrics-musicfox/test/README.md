# 测试程序说明

项目包含两个测试程序：

## 1. simple_test.exe（推荐使用）

**特点**：
- ✅ 自动运行，无需交互
- ✅ 依次测试所有功能
- ✅ 清晰的进度输出
- ✅ 每个测试 2-3 秒

**运行方式**：
```bash
cd Release
.\simple_test.exe
```

**输出示例**：
```
========================================
Taskbar Lyrics - Simple Test
========================================

[*] Loading native.dll...
[+] DLL loaded successfully!

[*] Getting SetLyric function...
[+] SetLyric found

[*] Getting SetConfig function...
[+] SetConfig found

[*] Initializing window (wait 2 seconds)...
[+] Window initialized!

========================================
Test 1: Basic Lyrics
========================================

[+] Displayed: Welcome to Taskbar Lyrics!
[*] Waiting 3 seconds...

... (继续输出其他测试)

========================================
All Tests Completed Successfully!
========================================

[SUCCESS] All features working correctly!
```

**测试内容**：
1. 基础歌词显示
2. 大红色加粗文字
3. 黄色文字
4. 绿色文字
5. 蓝色文字
6. 恢复默认配置

**按 Ctrl+C 退出**

---

## 2. test_dll.exe（交互式菜单）

**特点**：
- 🎮 交互式菜单选择
- 📋 8 个不同的测试场景
- 🔄 可重复测试

**运行方式**：
```bash
cd Release
.\test_dll.exe
```

**使用流程**：
```
========================================
任务栏歌词 - 测试程序
========================================

请选择测试场景：

  1. 默认配置
  2. 大字号显示
  3. 红色显示
  4. 左对齐
  5. 居中对齐
  6. 右对齐
  7. 副歌词显示
  8. 自定义字体
  0. 退出程序

输入选项 (0-8): 
```

**特点**：
- 输入序号选择测试
- 每个测试循环显示 3 行歌词
- 每行显示 3 秒
- 测试完后返回菜单
- 输入 0 退出

---

## 如果程序卡住了

如果运行时卡住，可能的原因：

### 1. DLL 加载失败
- 确保编译了主项目：`cd ../../build && cmake --build . --config Release`
- 检查 native.dll 是否存在于 `build/Release/` 目录

### 2. 窗口初始化失败
- 确保使用 Windows 10 或 Windows 11
- 检查是否使用了第三方任务栏软件
- 尝试重启资源管理器（win+shift+esc）

### 3. 程序响应缓慢
- 这是正常的，因为 DirectComposition 初始化需要时间
- 首次启动可能需要 2-3 秒
- 请耐心等待

---

## 编译测试程序

### 从头编译

```bash
# 确保已编译主项目
cd ../build
cmake --build . --config Release

# 编译测试程序
cd ../test
mkdir build
cd build
cmake -G Ninja ..
cmake --build . --config Release
```

### 快速重新编译

```bash
cd build
cmake --build . --config Release
```

---

## 输出文件位置

```
test/
├── build/
│   └── Release/
│       ├── simple_test.exe      ← 简单测试（推荐）
│       ├── test_dll.exe         ← 交互式测试
│       └── native.dll           ← 自动复制的 DLL
├── test_dll.cpp                 ← 交互式测试源码
├── simple_test.cpp              ← 简单测试源码
└── README.md                    ← 本文件
```

---

## 常见错误信息

### "[!] Failed to load native.dll from current directory"
**原因**: 找不到 native.dll
**解决**: 
1. 编译主项目：`cd ../../build && cmake --build . --config Release`
2. 重新编译测试：`cd ../test/build && cmake --build .`

### "[!] Failed to get SetLyric"
**原因**: SetLyric 函数导出有问题
**解决**: 
1. 检查 DLL 是否正确编译
2. 使用 DependencyWalker 查看导出函数

### 程序显示 DLL 加载成功但没有其他输出
**原因**: 窗口初始化中，需要等待
**解决**: 等待 2-3 秒，或按 Ctrl+C 中断

---

## 如何在任务栏看到歌词

1. 运行 `simple_test.exe` 或 `test_dll.exe`
2. 看任务栏（屏幕最下方）
3. 歌词应该显示在任务栏的左侧或中间区域
4. 根据配置显示不同的颜色、大小、对齐方式

### 如果看不到歌词

- 检查任务栏是否可见（Ctrl+Alt+Delete -> 任务管理器 -> 文件 -> 运行新任务 -> explorer.exe）
- 检查显示器分辨率是否太小
- 尝试在窗口中增加字体大小：`setConfig("size_primary", "32")`

---

## 代码示例

### 在你的程序中使用

```cpp
#include <Windows.h>

typedef void (*SetLyricFunc)(const wchar_t*);
typedef void (*SetConfigFunc)(const char*, const char*);

int main() {
    // 加载 DLL
    HMODULE dll = LoadLibrary(L"native.dll");
    
    auto setLyric = (SetLyricFunc)GetProcAddress(dll, "SetLyric");
    auto setConfig = (SetConfigFunc)GetProcAddress(dll, "SetConfig");
    
    // 配置
    setConfig("size_primary", "24");
    setConfig("color_primary", "0xFFFF0000");  // 红色
    
    // 显示歌词
    setLyric(L"你的歌词");
    
    // 保持运行
    Sleep(5000);
    
    return 0;
}
```

---

## 获取帮助

- 📖 查看 `../API_REFERENCE.md` 了解完整 API
- 🚀 查看 `../QUICK_START.md` 快速开始
- 🔄 查看 `../MIGRATION_GUIDE.md` 迁移指南

---

**祝你测试愉快！** 🎉
