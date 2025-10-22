# 任务栏歌词显示 DLL 使用说明

## ?? 生成的文件位置

编译成功后，DLL 文件位于：
```
C:\Users\lenovo\Desktop\tmp\lyrics\taskbar-lyrics-musicfox\build\native.dll
```

## ?? 使用方式

### 方式 1: 快速测试（推荐）

1. **编译测试程序**
   ```bash
   cd test
   build_and_run.bat
   ```

2. **或者手动编译**
   ```bash
   cd test
   mkdir build
   cd build
   cmake -G Ninja ..
   cmake --build .
   test_dll.exe
   ```

### 方式 2: 在你的 C++ 程序中使用

```cpp
#include <Windows.h>

// 定义函数指针类型
typedef void (*SetLyricFunc)(const wchar_t*);

int main() {
    // 1. 加载 DLL
    HMODULE hDll = LoadLibrary(L"native.dll");
    if (!hDll) {
   // 加载失败
        return -1;
    }
    
    // 此时窗口已经自动创建并显示在任务栏上
    
    // 2. 获取 SetLyric 函数
    SetLyricFunc setLyric = (SetLyricFunc)GetProcAddress(hDll, "SetLyric");
    
    // 3. 更新歌词
    if (setLyric) {
        setLyric(L"你的歌词内容");
    }
    
// 4. 保持程序运行（不要卸载 DLL）
    // 如果调用 FreeLibrary，窗口会关闭
    
    return 0;
}
```

### 方式 3: 从 Musicfox 或其他音乐播放器调用

#### Node.js 示例
```javascript
const ffi = require('ffi-napi');

// 加载 DLL
const native = ffi.Library('native.dll', {
  'SetLyric': ['void', ['string']]
});

// 更新歌词
function updateLyric(lyric) {
  native.SetLyric(lyric);
}

// 使用
updateLyric('当前播放的歌词');
```

#### Python 示例
```python
import ctypes

# 加载 DLL
dll = ctypes.CDLL('native.dll')

# 设置函数参数类型
dll.SetLyric.argtypes = [ctypes.c_wchar_p]
dll.SetLyric.restype = None

# 更新歌词
def update_lyric(lyric):
    dll.SetLyric(lyric)

# 使用
update_lyric('当前播放的歌词')
```

#### PowerShell 示例
```powershell
# 运行测试脚本
.\test\run_dll.ps1
```

## ?? DLL 工作原理

1. **自动初始化**: 当 DLL 被加载时（`DLL_PROCESS_ATTACH`），会自动：
   - 检查是否已有实例运行（通过全局互斥锁）
   - 创建任务栏歌词显示窗口
   - 启动消息循环

2. **单例模式**: 同一时间只允许一个实例运行

3. **导出函数**: `SetLyric(const wchar_t* lyric)`
   - 更新显示的歌词文本
   - 线程安全

## ?? API 参考

### SetLyric
```cpp
extern "C" __declspec(dllexport) void SetLyric(const wchar_t* lyric);
```

**参数:**
- `lyric`: 要显示的歌词文本（Unicode 字符串）

**说明:**
- 如果 `lyric` 为 NULL，函数不会执行任何操作
- 更新会立即反映在任务栏窗口上

## ?? 窗口特性

- **位置**: 任务栏左侧区域
- **大小**: 宽度 600px，高度跟随任务栏高度
- **样式**: 
  - 半透明背景
  - 白色文本
  - 居中对齐
  - 使用 Microsoft YaHei 字体

## ?? 注意事项

1. **不要卸载 DLL**: 如果调用 `FreeLibrary`，窗口会关闭

2. **单实例**: 如果已有实例运行，新的加载会被拒绝

3. **COM 初始化**: DLL 内部会自动初始化 COM（`COINIT_MULTITHREADED`）

4. **线程安全**: 可以从任何线程调用 `SetLyric`

## ?? 故障排查

### DLL 加载失败
- 检查 DLL 路径是否正确
- 确保所有依赖的 DLL 都在系统路径中
- 检查是否为正确的架构（x64 vs x86）

### 窗口不显示
- 检查任务栏是否可见
- 尝试重新加载 DLL
- 查看是否有其他实例正在运行

### 歌词不更新
- 确保正确获取了 `SetLyric` 函数指针
- 检查传入的字符串是否为 Unicode (wchar_t*)
- 验证 DLL 实例是否正在运行

## ?? 集成到 Musicfox

如果你想将此功能集成到 Musicfox，可以：

1. 修改 Musicfox 源代码，在播放歌曲时调用 `SetLyric`
2. 创建一个 Musicfox 插件来加载此 DLL
3. 使用 IPC 机制（如命名管道）与 Musicfox 通信

## ?? 相关文件

- `build/native.dll` - 主 DLL 文件
- `build/native.pdb` - 调试符号
- `test/test_dll.cpp` - 测试程序源代码
- `test/build_and_run.bat` - 快速编译运行脚本
- `test/run_dll.ps1` - PowerShell 测试脚本
