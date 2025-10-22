# ?? 编译成功！

## ? 生成的文件

1. **主 DLL**: `build\native.dll` (83 KB)
2. **测试程序**: `test\build\test_dll.exe` (87 KB)

## ?? 如何运行测试

### 方法 1: 直接运行 (最简单)

```powershell
cd test\build
.\test_dll.exe
```

或者双击 `test\build\test_dll.exe`

### 方法 2: 使用脚本

```powershell
# PowerShell
.\test\build_and_run.ps1

# 或 CMD
.\test\build_and_run.bat
```

## ?? 预期效果

运行 `test_dll.exe` 后，你应该看到：

1. **控制台输出**:
   ```
   DLL loaded successfully!
   SetLyric function found!
   Displaying lyrics on taskbar...
   Set lyric: 第一句歌词
   Set lyric: 第二句歌词
   Set lyric: 第三句歌词
   Set lyric: 第四句歌词
   ```

2. **任务栏显示**: 在 Windows 任务栏左侧会出现一个半透明窗口，显示当前歌词

3. **自动循环**: 每 3 秒切换一次歌词

## ?? 如何停止

按 `Ctrl + C` 停止程序

## ?? 自定义歌词

编辑 `test\test_dll.cpp` 文件中的 `testLyrics` 数组：

```cpp
std::vector<std::wstring> testLyrics = {
    L"你的第一句歌词",
    L"你的第二句歌词",
    L"你的第三句歌词",
    // ... 更多歌词
};
```

然后重新编译：

```powershell
cd test\build
cmake --build .
.\test_dll.exe
```

## ?? 集成到音乐播放器

参考 `USAGE.md` 中的详细说明，了解如何：
- 从 C++ 程序调用
- 从 Python 调用
- 从 Node.js 调用
- 从 Musicfox 调用

## ?? 注意事项

1. **必须先编译主项目**: 确保 `build\native.dll` 存在
2. **不要同时运行多个实例**: DLL 使用单例模式
3. **保持程序运行**: 不要关闭 `test_dll.exe`，否则歌词窗口会消失

## ?? 问题排查

### 找不到 native.dll
```
Failed to load native.dll
Error code: 126
```
**解决方案**: 确保 `build\native.dll` 存在，重新编译主项目

### 窗口不显示
- 检查任务栏是否可见
- 确保只有一个实例在运行
- 尝试重启程序

## ?? 更多信息

查看以下文件了解更多：
- `USAGE.md` - 完整的使用文档
- `test\test_dll.cpp` - 测试程序源代码
- `src\plugin\Plugin.cppm` - DLL 主要逻辑
- `src\window\Window.cppm` - 窗口实现
