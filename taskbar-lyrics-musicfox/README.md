# Taskbar Lyrics for Musicfox

> 为 Windows 任务栏添加歌词显示功能的 DLL 插件

[![Language](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Build System](https://img.shields.io/badge/CMake-3.30+-green.svg)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%2F11-lightgrey.svg)](https://www.microsoft.com/windows)

## ✨ 特性

- 🎵 在 Windows 任务栏上显示歌词
- 🎨 半透明背景，美观不遮挡
- 🔒 单例模式，防止多实例冲突
- 🚀 使用 C++20 模块，现代化架构
- 🎯 Direct2D 渲染，流畅高效
- 📦 DLL 形式，易于集成

## 🖼️ 效果预览

歌词会显示在任务栏左侧，具有以下特点:
- 半透明黑色背景
- 白色文字，居中对齐
- Microsoft YaHei 字体，24px
- 自动跟随任务栏高度

## 🚀 快速开始

### 环境要求

- Windows 10/11
- CMake 3.30+
- Visual Studio 2022 (MSVC 19.50+)
- Ninja 构建工具

### 编译

```bash
# 1. 克隆或下载项目
cd taskbar-lyrics-musicfox

# 2. 构建主 DLL
mkdir build && cd build
cmake -G Ninja ..
cmake --build .

# 3. 构建测试程序（可选）
cd ../test
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

### 运行测试

```bash
cd test\build
.\test_dll.exe
```

你应该能在任务栏看到歌词循环显示。

## 📖 使用方式

### 方式 1: 在 C++ 中使用

```cpp
#include <Windows.h>

typedef void (*SetLyricFunc)(const wchar_t*);

int main() {
    // 加载 DLL（窗口会自动创建）
    HMODULE dll = LoadLibrary(L"native.dll");
    
    // 获取函数
    auto setLyric = (SetLyricFunc)GetProcAddress(dll, "SetLyric");
    
    // 更新歌词
    setLyric(L"你的歌词");
    
    return 0;
}
```

### 方式 2: 在 Python 中使用

```python
import ctypes

dll = ctypes.CDLL('native.dll')
dll.SetLyric.argtypes = [ctypes.c_wchar_p]

dll.SetLyric('你的歌词')
```

### 方式 3: 在 Node.js 中使用

```javascript
const ffi = require('ffi-napi');

const native = ffi.Library('native.dll', {
  'SetLyric': ['void', ['string']]
});

native.SetLyric('你的歌词');
```

更多使用方式请查看 [USAGE.md](USAGE.md)。

## 📁 项目结构

```
├── src/  # 源代码
│   ├── DllMain.cpp      # DLL 入口
│   ├── plugin/          # 插件模块
│   └── window/                # 窗口和渲染模块
├── test/ # 测试代码
├── build/ # 构建输出
├── CMakeLists.txt     # CMake 配置
└── README.md   # 本文件
```

详细说明请查看 [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)。

## 🔧 API 文档

### SetLyric

```cpp
extern "C" __declspec(dllexport) void SetLyric(const wchar_t* lyric);
```

**参数:**
- `lyric`: 要显示的歌词文本（Unicode 字符串）

**说明:**
- 如果 `lyric` 为 NULL，函数不执行任何操作
- 更新会立即反映在任务栏窗口上
- 线程安全，可从任何线程调用

## 🔄 开发

### 清理构建

```bash
# Windows CMD
clean.bat

# PowerShell
.\clean.ps1
```

### 重新构建

```bash
.\clean.bat
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

### 调试

1. 使用 Visual Studio 打开项目目录
2. 设置 `test_dll.exe` 为启动项目
3. F5 开始调试

## 📦 集成到 Musicfox

### 选项 1: 直接加载 DLL

在 Musicfox 的播放器代码中：

```go
// 使用 syscall 调用 DLL
dll := syscall.MustLoadDLL("native.dll")
setLyric := dll.MustFindProc("SetLyric")

// 更新歌词
lyricPtr, _ := syscall.UTF16PtrFromString("当前歌词")
setLyric.Call(uintptr(unsafe.Pointer(lyricPtr)))
```

### 选项 2: 通过进程通信

创建一个中间服务，监听 Musicfox 的歌词更新，然后调用 DLL。

## ⚠️ 注意事项

1. **单实例**: 同一时间只能运行一个实例
2. **不要卸载 DLL**: 调用 `FreeLibrary` 会关闭窗口
3. **编译器要求**: 必须使用 MSVC，必须使用 Ninja
4. **平台限制**: 仅支持 Windows 10/11

## 🐛 故障排查

### DLL 加载失败
- 检查 DLL 路径
- 确保依赖的 DLL 存在（d2d1.dll, dwrite.dll 等）
- 检查架构是否匹配（x64 vs x86）

### 窗口不显示
- 检查任务栏是否可见
- 查看是否有其他实例正在运行
- 尝试重新加载 DLL

### 编译错误
- 确保使用 Visual Studio 2022
- 确保使用 Ninja 生成器
- 清理后重新构建

详细的故障排查请查看 [USAGE.md](USAGE.md)。

## 📚 文档

- [使用说明](USAGE.md) - 详细的使用文档和 API 参考
- [项目结构](PROJECT_STRUCTURE.md) - 项目结构和架构说明
- [构建指南](BUILD_SUCCESS.md) - 构建成功后的快速指南

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 🙏 致谢

- [Musicfox](https://github.com/go-musicfox/go-musicfox) - 灵感来源
- Direct2D & DirectWrite - 渲染支持
- C++20 Modules - 现代化架构

---

**Made with ❤️ for Musicfox users**

