# Taskbar Lyrics for Musicfox

> 为 Windows 任务栏添加歌词显示功能的 DLL 插件 文档AI生成，未检查

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

> **💡 新功能：一键集成到 go-musicfox！**
> 
> 现在可以自动构建 DLL 和打补丁的 musicfox，详见 [集成指南](#-集成到-go-musicfox)

### 方式 1: 集成构建（推荐）

将 DLL 和 go-musicfox 一起构建：

```bash
# 一键构建脚本
build-integration.bat
# 选择选项 1: Build DLL + Musicfox (Full Build)
```

或使用 CMake：

```bash
mkdir build
cd build
cmake -G Ninja -S .. -B . -DSKIP_BUILD_MUSICFOX=NO
cmake --build .
```

输出文件在 `build/release/` 目录：
- `musicfox.exe` - 打补丁的播放器
- `native.dll` - 任务栏歌词插件

### 方式 2: 仅构建 DLL

如果只需要 DLL：

```bash
cd src
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

### 环境要求

- Windows 10/11
- CMake 3.20+
- Visual Studio 2022 (MSVC 19.50+)
- Ninja 构建工具
- Go 1.21+ (集成构建时需要)
- Git (集成构建时需要)

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

## 📦 集成到 go-musicfox

### 自动集成（推荐）

本项目提供了完整的自动集成方案，类似于 [lyric-for-musicfox](https://github.com/SmileYik/lyric-for-musicfox) 的构建方式，但使用 DLL 进程内调用替代 UDP 网络传输。

#### 🎯 快速集成

```bash
# 方式 1: 使用一键脚本
build-integration.bat
# 选择选项 1: Build DLL + Musicfox (Full Build)

# 方式 2: 使用 CMake
mkdir build-integration && cd build-integration
cmake -G Ninja -S .. -B . -DSKIP_BUILD_MUSICFOX=NO
cmake --build .
```

#### 📂 输出内容

构建完成后，在 `build-integration/release/` 目录下：

```
release/
├── musicfox.exe    # 打补丁的播放器（已集成 DLL 加载）
├── native.dll      # 任务栏歌词插件
└── README.md       # 使用说明
```

#### 🔧 工作原理

集成通过补丁文件 `musicfox-dll.patch` 实现：

1. **自动加载 DLL**：在 `player.go` 中添加 `initLyricsPlugin()` 函数
2. **歌词传输**：通过 `SetConfig(key, value)` 函数发送歌词
3. **实时更新**：在播放进度更新时自动发送当前歌词

补丁修改的文件：`go-musicfox/internal/ui/player.go`

添加的功能：
```go
// DLL 加载（仅 Windows）
func initLyricsPlugin()

// 歌词发送
func sendLyricBySetConfig(content, transContent string)
```

#### 📚 详细文档

- **[完整集成指南](INTEGRATION_GUIDE.md)** - 详细的集成步骤和原理
- **[快速开始](QUICK_START_CN.md)** - 一分钟上手
- **[集成 README](README-INTEGRATION.md)** - 集成版本说明

#### 🆚 对比 lyric-for-musicfox

| 特性 | lyric-for-musicfox | 本项目（DLL 集成） |
|------|-------------------|-------------------|
| 通信方式 | UDP 网络传输 | DLL 函数调用 |
| 显示位置 | 桌面悬浮窗 | Windows 任务栏 |
| 平台支持 | Linux + Windows | 仅 Windows |
| 性能开销 | UDP 网络栈 | 进程内调用（更快） |
| 部署方式 | 两个独立程序 | 集成在一起 |
| 端口配置 | 需要配置端口 | 无需配置 |

### 手动集成

如果需要手动应用补丁：

#### 步骤 1: 构建 DLL

```bash
cd src && mkdir build && cd build
cmake -G Ninja .. && cmake --build .
```

#### 步骤 2: 克隆并打补丁

```bash
git clone https://github.com/go-musicfox/go-musicfox.git
cd go-musicfox
git apply ../musicfox-dll.patch
```

#### 步骤 3: 编译 musicfox

```bash
make
```

#### 步骤 4: 部署

将 `native.dll` 和 `bin/musicfox.exe` 放在同一目录即可运行。

### API 调用示例

如果你想在自己的 Go 程序中使用：

```go
package main

import (
    "syscall"
    "unsafe"
)

var (
    dll           *syscall.DLL
    setConfigProc *syscall.Proc
)

func init() {
    dll = syscall.MustLoadDLL("native.dll")
    setConfigProc = dll.MustFindProc("SetConfig")
}

func setLyric(primary, secondary string) {
    // 设置主歌词
    keyPtr, _ := syscall.UTF16PtrFromString("lyric_primary")
    valPtr, _ := syscall.UTF16PtrFromString(primary)
    setConfigProc.Call(
        uintptr(unsafe.Pointer(keyPtr)),
        uintptr(unsafe.Pointer(valPtr)),
    )
    
    // 设置副歌词（翻译）
    if secondary != "" {
        keyPtr, _ := syscall.UTF16PtrFromString("lyric_secondary")
        valPtr, _ := syscall.UTF16PtrFromString(secondary)
        setConfigProc.Call(
            uintptr(unsafe.Pointer(keyPtr)),
            uintptr(unsafe.Pointer(valPtr)),
        )
    }
}
```

## 🔧 DLL API 文档

### SetConfig

```cpp
extern "C" __declspec(dllexport) void SetConfig(const wchar_t* key, const wchar_t* value);
```

**参数:**
- `key`: 配置键（支持 `lyric_primary` 和 `lyric_secondary`）
- `value`: 配置值（歌词文本）

**支持的配置键:**
- `lyric_primary`: 主歌词
- `lyric_secondary`: 副歌词/翻译

**说明:**
- 线程安全，可从任何线程调用
- 更新会立即反映在任务栏上
- 如果参数为 NULL，函数不执行任何操作

### SetLyric（已废弃，保留兼容）

```cpp
extern "C" __declspec(dllexport) void SetLyric(const wchar_t* lyric);
```

推荐使用 `SetConfig` 以支持翻译歌词。

## 📁 项目结构

```
taskbar-lyrics-musicfox/
├── src/                          # DLL 源代码
│   ├── CMakeLists.txt           # DLL 构建配置
│   ├── DllMain.cpp              # DLL 主文件
│   ├── plugin/                  # 插件接口
│   ├── taskbar/                 # 任务栏渲染
│   └── window/                  # 窗口管理
├── test/                        # 测试代码
│   ├── simple_test.cpp         # 简单测试
│   └── test_dll.cpp            # DLL 功能测试
├── musicfox-dll.patch          # go-musicfox 补丁文件
├── CMakeLists-integration.txt  # 集成构建配置
├── build-integration.bat       # 一键构建脚本
├── test-integration.bat        # 集成测试脚本
├── INTEGRATION_GUIDE.md        # 完整集成指南
├── QUICK_START_CN.md           # 快速开始
├── README-INTEGRATION.md       # 集成版本说明
└── README.md                   # 本文件
```
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

