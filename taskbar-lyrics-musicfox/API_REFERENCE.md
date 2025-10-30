# Taskbar Lyrics API 文档

## 概述

Taskbar Lyrics for Musicfox 提供两种方式来控制任务栏歌词显示：

1. **SetLyric** - 简单的歌词设置（向后兼容）
2. **SetConfig** - 灵活的配置系统（推荐使用）

---

## SetLyric API

### 函数签名

```cpp
extern "C" __declspec(dllexport) void SetLyric(const wchar_t* lyric);
```

### 参数

- `lyric` (const wchar_t*): 要显示的歌词文本（Unicode 字符串）

### 说明

- 设置主歌词内容
- 如果 `lyric` 为 NULL，函数不执行任何操作
- 自动转换为内部配置系统
- 线程安全，可从任何线程调用

### 示例

```cpp
// C++
#include <Windows.h>

typedef void (*SetLyricFunc)(const wchar_t*);

int main() {
    HMODULE dll = LoadLibrary(L"native.dll");
    auto setLyric = (SetLyricFunc)GetProcAddress(dll, "SetLyric");
    
    setLyric(L"你的歌词");
    
    // 不要卸载 DLL，否则窗口会关闭
    return 0;
}
```

```python
# Python
import ctypes

dll = ctypes.CDLL('native.dll')
dll.SetLyric.argtypes = [ctypes.c_wchar_p]

dll.SetLyric('你的歌词')
```

```csharp
// C#
[DllImport("native.dll")]
public static extern void SetLyric(string lyric);

SetLyric("你的歌词");
```

---

## SetConfig API

### 函数签名

```cpp
extern "C" __declspec(dllexport) void SetConfig(const char* key, const char* value);
```

### 参数

- `key` (const char*): 配置项名称（UTF-8 字符串）
- `value` (const char*): 配置值（UTF-8 字符串）

### 说明

- 设置或更新任何配置项
- 所有参数都使用 UTF-8 编码
- 线程安全，可从任何线程调用
- 配置立即生效

### 示例

```cpp
// C++
typedef void (*SetConfigFunc)(const char*, const char*);

auto setConfig = (SetConfigFunc)GetProcAddress(dll, "SetConfig");

setConfig("lyric_primary", "主歌词");
setConfig("size_primary", "24");
setConfig("color_primary", "0xFFFFFFFF");
```

```python
# Python
dll.SetConfig.argtypes = [ctypes.c_char_p, ctypes.c_char_p]

dll.SetConfig(b'lyric_primary', b'\xe4\xb8\xbb\xe6\xad\x8c\xe8\xaf\x8d')  # UTF-8 编码
dll.SetConfig(b'size_primary', b'24')
```

---

## 配置项参考

### 歌词内容

#### lyric_primary
- **类型**: 字符串（UTF-8）
- **默认值**: " "（空格）
- **说明**: 主歌词内容
- **示例**: `setConfig("lyric_primary", "欢迎使用任务栏歌词")`

#### lyric_secondary
- **类型**: 字符串（UTF-8）
- **默认值**: " "（空格）
- **说明**: 副歌词内容（如英文翻译）
- **示例**: `setConfig("lyric_secondary", "Welcome to Taskbar Lyrics")`

---

### 通用设置

#### font_family
- **类型**: 字符串（字体名称）
- **默认值**: "Microsoft YaHei UI"
- **说明**: 字体名称
- **可用值**: 系统已安装的任何字体
- **示例**: 
  - `setConfig("font_family", "Microsoft YaHei")`
  - `setConfig("font_family", "SimSun")`
  - `setConfig("font_family", "Arial")`

#### margin_left
- **类型**: 整数（像素）
- **默认值**: 0
- **范围**: >= 0
- **说明**: 左边距
- **示例**: `setConfig("margin_left", "10")`

#### margin_right
- **类型**: 整数（像素）
- **默认值**: 0
- **范围**: >= 0
- **说明**: 右边距
- **示例**: `setConfig("margin_right", "10")`

#### window_alignment
- **类型**: 整数
- **默认值**: 0
- **可用值**:
  - 0 = AUTO（自动）
  - 1 = LEFT（左对齐）
  - 2 = CENTER（居中）
  - 3 = RIGHT（右对齐）
- **说明**: 窗口对齐方式
- **示例**: 
  - `setConfig("window_alignment", "0")`  // 自动适配
  - `setConfig("window_alignment", "1")`  // 左对齐
  - `setConfig("window_alignment", "2")`  // 居中

---

### 主歌词样式设置

#### size_primary
- **类型**: 整数（像素）
- **默认值**: 14
- **范围**: > 0
- **说明**: 主歌词字体大小
- **示例**: `setConfig("size_primary", "24")`

#### color_primary
- **类型**: 字符串（ARGB 16进制）
- **默认值**: "0xFFFFFFFF"
- **格式**: 0xAARRGGBB
  - AA: 透明度 (00=透明, FF=不透明)
  - RR: 红色通道
  - GG: 绿色通道
  - BB: 蓝色通道
- **说明**: 主歌词颜色
- **示例**:
  - `setConfig("color_primary", "0xFFFFFFFF")`  // 白色
  - `setConfig("color_primary", "0xFFFF0000")`  // 红色
  - `setConfig("color_primary", "0xFF00FF00")`  // 绿色
  - `setConfig("color_primary", "0xFF0000FF")`  // 蓝色
  - `setConfig("color_primary", "0xFFFFFF00")`  // 黄色
  - `setConfig("color_primary", "0xFF00FFFF")`  // 青色
  - `setConfig("color_primary", "0xFFFF00FF")`  // 紫色

#### weight_primary
- **类型**: 整数
- **默认值**: 400
- **可用值**:
  - 100 = Thin（细）
  - 200 = ExtraLight（极细）
  - 300 = Light（轻）
  - 400 = Normal（正常）
  - 500 = Medium（中等）
  - 600 = SemiBold（半粗）
  - 700 = Bold（粗体）
  - 800 = ExtraBold（极粗）
  - 900 = Black（黑体）
- **说明**: 字体粗细
- **示例**: 
  - `setConfig("weight_primary", "400")`  // 正常
  - `setConfig("weight_primary", "700")`  // 粗体

#### slope_primary
- **类型**: 整数
- **默认值**: 0
- **可用值**:
  - 0 = Normal（正常）
  - 1 = Italic（斜体）
  - 2 = Oblique（倾斜）
- **说明**: 字体风格
- **示例**: 
  - `setConfig("slope_primary", "0")`  // 正常
  - `setConfig("slope_primary", "1")`  // 斜体

#### align_primary
- **类型**: 整数
- **默认值**: 0
- **可用值**:
  - 0 = Leading（左对齐）
  - 1 = Center（居中）
  - 2 = Trailing（右对齐）
  - 3 = Justified（两端对齐）
- **说明**: 文本对齐方式
- **示例**: 
  - `setConfig("align_primary", "0")`  // 左对齐
  - `setConfig("align_primary", "1")`  // 居中

#### underline_primary
- **类型**: 字符串（布尔值）
- **默认值**: "false"
- **可用值**: "true" 或 "false"
- **说明**: 是否显示下划线
- **示例**: `setConfig("underline_primary", "true")`

#### strikethrough_primary
- **类型**: 字符串（布尔值）
- **默认值**: "false"
- **可用值**: "true" 或 "false"
- **说明**: 是否显示删除线
- **示例**: `setConfig("strikethrough_primary", "true")`

---

### 副歌词样式设置

副歌词的设置项与主歌词相同，只需将后缀改为 `_secondary`：

- `color_secondary` - 副歌词颜色
- `size_secondary` - 副歌词大小
- `weight_secondary` - 副歌词粗细
- `slope_secondary` - 副歌词样式
- `align_secondary` - 副歌词对齐
- `underline_secondary` - 副歌词下划线
- `strikethrough_secondary` - 副歌词删除线

**示例**:
```cpp
setConfig("lyric_secondary", "English translation");
setConfig("size_secondary", "12");
setConfig("color_secondary", "0xFF808080");  // 灰色
```

---

## 常见用法示例

### 示例 1: 基础歌词显示

```cpp
SetLyricFunc setLyric = /* 获取函数 */;
setLyric(L"当前播放的歌词");
```

### 示例 2: 自定义样式

```cpp
SetConfigFunc setConfig = /* 获取函数 */;

// 设置大红色粗体歌词
setConfig("color_primary", "0xFFFF0000");
setConfig("weight_primary", "700");
setConfig("size_primary", "28");
```

### 示例 3: 双语歌词

```cpp
SetConfigFunc setConfig = /* 获取函数 */;
SetLyricFunc setLyric = /* 获取函数 */;

// 设置主副歌词
setConfig("lyric_primary", "主歌词");
setConfig("lyric_secondary", "英文翻译");
setConfig("size_primary", "18");
setConfig("size_secondary", "14");
setConfig("color_secondary", "0xFF808080");
```

### 示例 4: Python 集成

```python
import ctypes
import os

# 加载 DLL
dll = ctypes.CDLL('native.dll')

# 配置函数
dll.SetLyric.argtypes = [ctypes.c_wchar_p]
dll.SetConfig.argtypes = [ctypes.c_char_p, ctypes.c_char_p]

# 设置歌词
dll.SetLyric('当前歌词')

# 配置样式
dll.SetConfig(b'size_primary', b'24')
dll.SetConfig(b'color_primary', b'0xFFFFFF00')  # 黄色
```

### 示例 5: Go 集成

```go
package main

import (
    "syscall"
    "unsafe"
)

func main() {
    dll := syscall.MustLoadDLL("native.dll")
    setLyric := dll.MustFindProc("SetLyric")
    setConfig := dll.MustFindProc("SetConfig")
    
    // 设置歌词
    lyricPtr, _ := syscall.UTF16PtrFromString("当前歌词")
    setLyric.Call(uintptr(unsafe.Pointer(lyricPtr)))
    
    // 配置大小
    keyPtr, _ := syscall.BytePtrFromString("size_primary")
    valPtr, _ := syscall.BytePtrFromString("24")
    setConfig.Call(uintptr(unsafe.Pointer(keyPtr)), uintptr(unsafe.Pointer(valPtr)))
}
```

---

## 错误处理

### 常见错误

1. **DLL 加载失败**
   - 检查 DLL 路径
   - 确保依赖库已安装（d2d1.dll, dwrite.dll 等）
   - 检查 DLL 架构（x64 vs x86）

2. **函数获取失败**
   - 确保 DLL 已正确编译
   - 检查函数名称拼写
   - 验证 DLL 导出表

3. **配置无效**
   - 检查参数值格式
   - 确保颜色值使用正确的 ARGB 格式
   - 验证整数值在有效范围内

### 调试建议

```cpp
// 检查 DLL 是否加载
HMODULE dll = LoadLibrary(L"native.dll");
if (!dll) {
    DWORD error = GetLastError();
    printf("DLL 加载失败，错误代码: %d\n", error);
    return;
}

// 检查函数是否存在
SetConfigFunc setConfig = (SetConfigFunc)GetProcAddress(dll, "SetConfig");
if (!setConfig) {
    DWORD error = GetLastError();
    printf("SetConfig 获取失败，错误代码: %d\n", error);
    return;
}
```

---

## 线程安全性

所有导出函数都是线程安全的，可以从多个线程同时调用：

```cpp
// 线程 1
std::thread t1([=] {
    setLyric(L"线程 1 的歌词");
});

// 线程 2
std::thread t2([=] {
    setConfig("color_primary", "0xFFFFFFFF");
});

t1.join();
t2.join();
```

---

## 性能考虑

1. **避免频繁更新**: 避免每帧都更新配置
2. **批量配置**: 在初始化时一次性设置所有配置项
3. **异步更新**: 在后台线程中更新歌词，避免阻塞主线程

---

## 更新日志

### v1.0.0
- 初始版本
- SetLyric 函数支持
- SetConfig 函数支持
- 全面的配置选项
- DirectComposition 渲染
- UIAutomation 支持
