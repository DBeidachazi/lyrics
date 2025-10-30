# 🚀 快速开始指南

## 1. 编译项目

```bash
cd C:\Users\lenovo\Desktop\tmp\lyrics\taskbar-lyrics-musicfox
mkdir build && cd build
cmake -G Ninja ..
cmake --build . --config Release
```

**输出文件**: `build/Release/native.dll`

## 2. 运行测试程序

### 方法 A: 编译测试程序

```bash
cd test
mkdir build && cd build
cmake -G Ninja ..
cmake --build . --config Release
.\test_dll.exe
```

### 方法 B: 手动编译测试

```cpp
// 创建 simple_test.cpp
#include <Windows.h>
#include <iostream>

typedef void (*SetLyricFunc)(const wchar_t*);
typedef void (*SetConfigFunc)(const char*, const char*);

int main() {
    HMODULE dll = LoadLibrary(L"native.dll");
    
    auto setLyric = (SetLyricFunc)GetProcAddress(dll, "SetLyric");
    auto setConfig = (SetConfigFunc)GetProcAddress(dll, "SetConfig");
    
    // 设置配置
    setConfig("size_primary", "24");
    setConfig("color_primary", "0xFFFFFFFF");
    
    // 显示歌词
    setLyric(L"欢迎使用任务栏歌词！");
    
    // 保持运行
    system("pause");
    
    return 0;
}
```

## 3. 新 API 示例

### C++ 示例

```cpp
#include <Windows.h>
#include <thread>
#include <chrono>

typedef void (*SetLyricFunc)(const wchar_t*);
typedef void (*SetConfigFunc)(const char*, const char*);

int main() {
    HMODULE dll = LoadLibrary(L"native.dll");
    auto setLyric = (SetLyricFunc)GetProcAddress(dll, "SetLyric");
    auto setConfig = (SetConfigFunc)GetProcAddress(dll, "SetConfig");
    
    // 配置样式
    setConfig("size_primary", "28");
    setConfig("color_primary", "0xFFFF0000");  // 红色
    setConfig("weight_primary", "700");        // 粗体
    
    // 显示歌词
    setLyric(L"🎵 这是歌词");
    
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    return 0;
}
```

### Python 示例

```python
import ctypes
import time

# 加载 DLL
dll = ctypes.CDLL('build/Release/native.dll')

# 设置函数签名
dll.SetLyric.argtypes = [ctypes.c_wchar_p]
dll.SetConfig.argtypes = [ctypes.c_char_p, ctypes.c_char_p]

# 配置
dll.SetConfig(b'size_primary', b'24')
dll.SetConfig(b'color_primary', b'0xFFFFFFFF')

# 显示歌词
lyrics = ['第一行', '第二行', '第三行']
for lyric in lyrics:
    dll.SetLyric(lyric)
    print(f'显示: {lyric}')
    time.sleep(3)
```

### Go 示例

```go
package main

import (
    "fmt"
    "syscall"
    "time"
    "unsafe"
)

func main() {
    dll := syscall.MustLoadDLL("build/Release/native.dll")
    setLyric := dll.MustFindProc("SetLyric")
    setConfig := dll.MustFindProc("SetConfig")
    
    // 配置
    keyPtr, _ := syscall.BytePtrFromString("size_primary")
    valPtr, _ := syscall.BytePtrFromString("24")
    setConfig.Call(
        uintptr(unsafe.Pointer(keyPtr)),
        uintptr(unsafe.Pointer(valPtr)),
    )
    
    // 显示歌词
    lyrics := []string{"歌词1", "歌词2", "歌词3"}
    for _, lyric := range lyrics {
        lyricPtr, _ := syscall.UTF16PtrFromString(lyric)
        setLyric.Call(uintptr(unsafe.Pointer(lyricPtr)))
        fmt.Printf("显示: %s\n", lyric)
        time.Sleep(3 * time.Second)
    }
}
```

## 4. 配置参数速查

### 常用配置

```cpp
// 颜色配置
setConfig("color_primary", "0xFFFFFFFF");   // 白色
setConfig("color_primary", "0xFFFF0000");   // 红色
setConfig("color_primary", "0xFF00FF00");   // 绿色
setConfig("color_primary", "0xFF0000FF");   // 蓝色
setConfig("color_primary", "0xFFFFFF00");   // 黄色

// 大小配置
setConfig("size_primary", "14");   // 小
setConfig("size_primary", "24");   // 中
setConfig("size_primary", "32");   // 大

// 字重配置
setConfig("weight_primary", "400"); // 正常
setConfig("weight_primary", "700"); // 粗体
setConfig("weight_primary", "900"); // 黑体

// 对齐配置
setConfig("window_alignment", "0"); // AUTO
setConfig("window_alignment", "1"); // LEFT
setConfig("window_alignment", "2"); // CENTER
setConfig("window_alignment", "3"); // RIGHT
```

## 5. 常见问题

### Q: DLL 加载失败？
**A**: 
1. 确保编译了项目：`cmake --build . --config Release`
2. 检查 DLL 路径
3. 检查依赖库（d2d1.dll, dwrite.dll 等）

### Q: 测试时看不到窗口？
**A**: 
1. 请确保 Windows 11
2. 右键检查任务栏，启用原生任务栏
3. 检查是否使用了第三方任务栏软件

### Q: 歌词不显示？
**A**:
1. 检查 SetLyric 参数是否为 NULL
2. 查看 SetConfig 配置是否正确
3. 检查颜色值格式（ARGB 16进制）

### Q: 如何清除之前的配置？
**A**: 重新启动测试程序或应用程序

## 6. 集成到 Musicfox

### Python 集成示例

```python
# 在 musicfox 播放事件中
from ctypes import CDLL, c_char_p, c_wchar_p

class TaskbarLyrics:
    def __init__(self):
        self.dll = CDLL('path/to/native.dll')
        self.dll.SetLyric.argtypes = [c_wchar_p]
        self.dll.SetConfig.argtypes = [c_char_p, c_char_p]
        
        # 初始化配置
        self.dll.SetConfig(b'size_primary', b'24')
    
    def update(self, lyric_cn, lyric_en=None):
        """更新歌词"""
        self.dll.SetLyric(lyric_cn)
        
        if lyric_en:
            self.dll.SetConfig(b'lyric_secondary', lyric_en.encode('utf-8'))

# 使用
lyrics = TaskbarLyrics()
lyrics.update("中文歌词", "English lyric")
```

### Go 集成示例

```go
type TaskbarLyrics struct {
    setLyric   syscall.Proc
    setConfig  syscall.Proc
}

func NewTaskbarLyrics() *TaskbarLyrics {
    dll := syscall.MustLoadDLL("native.dll")
    return &TaskbarLyrics{
        setLyric:  *dll.MustFindProc("SetLyric"),
        setConfig: *dll.MustFindProc("SetConfig"),
    }
}

func (t *TaskbarLyrics) Update(lyric string) error {
    ptr, _ := syscall.UTF16PtrFromString(lyric)
    t.setLyric.Call(uintptr(unsafe.Pointer(ptr)))
    return nil
}
```

## 7. 文档

- 📖 [完整 API 参考](API_REFERENCE.md)
- 🔄 [迁移指南](MIGRATION_GUIDE.md)
- ✅ [转换总结](CONVERSION_SUCCESS.md)

## 8. 下一步

1. ✅ 编译项目：`cmake --build . --config Release`
2. ✅ 运行测试：`test/build/test_dll.exe`
3. ✅ 查看文档：`API_REFERENCE.md`
4. ✅ 集成到 Musicfox
5. ✅ 自定义配置

---

**祝您使用愉快！** 🎉

有问题？查看 `API_REFERENCE.md` 或运行测试程序了解更多！
