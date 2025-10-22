# 项目结构说明

## ?? 目录结构

```
taskbar-lyrics-musicfox/
├── src/   # 源代码目录
│   ├── DllMain.cpp              # DLL 入口点
│   ├── plugin/
│   │   └── Plugin.cppm          # 插件管理模块（单例）
│   └── window/
│  ├── Window.cppm          # 窗口管理模块
│    └── Renderer.cppm        # Direct2D 渲染模块
│
├── test/         # 测试代码
│   ├── test_dll.cpp  # DLL 测试程序
│   ├── CMakeLists.txt        # 测试项目 CMake 配置
│   ├── build_and_run.bat      # 构建运行脚本 (CMD)
│├── build_and_run.ps1     # 构建运行脚本 (PowerShell)
│   └── run_dll.ps1     # DLL 测试脚本
│
├── build/   # 主项目构建输出 (gitignored)
│   └── native.dll     # 编译生成的 DLL
│
├── CMakeLists.txt       # 主项目 CMake 配置
├── .gitignore          # Git 忽略规则
├── clean.bat # 清理脚本 (CMD)
├── clean.ps1                # 清理脚本 (PowerShell)
│
├── USAGE.md          # 使用说明文档
├── BUILD_SUCCESS.md     # 构建成功指南
└── PROJECT_STRUCTURE.md    # 本文件
```

## ?? 核心文件说明

### 源代码 (src/)

#### `DllMain.cpp`
- **作用**: DLL 的入口点
- **功能**:
  - 处理 DLL 加载/卸载事件
  - 初始化 COM
  - 创建 Plugin 单例
  - 导出 `SetLyric` 函数

#### `plugin/Plugin.cppm`
- **作用**: 插件管理模块
- **功能**:
  - 单例模式，确保只有一个实例
  - 使用全局互斥锁防止多实例
  - 管理 Window 对象的生命周期
  - 提供 `setLyric` 接口

#### `window/Window.cppm`
- **作用**: 窗口管理模块
- **功能**:
  - 创建和管理任务栏窗口
  - 处理 Windows 消息
  - 管理窗口位置和大小
  - 协调 Renderer 进行渲染

#### `window/Renderer.cppm`
- **作用**: 渲染模块
- **功能**:
  - 使用 Direct2D 和 DirectWrite 渲染文字
  - 管理字体、颜色和样式
  - 处理窗口大小变化

### 测试代码 (test/)

#### `test_dll.cpp`
- **作用**: DLL 功能测试程序
- **功能**:
  - 动态加载 native.dll
  - 调用 SetLyric 函数
  - 循环显示测试歌词

## ?? 构建系统

### CMake 配置
- **主项目**: `CMakeLists.txt`
  - 使用 C++20 模块
  - 需要 Ninja 生成器
  - 最低 CMake 版本 3.30

- **测试项目**: `test/CMakeLists.txt`
  - 独立的可执行文件
  - 链接 Windows 系统库

### 构建命令

**主项目**:
```bash
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

**测试项目**:
```bash
cd test
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

## ??? 清理

### 方式 1: 使用脚本
```cmd
clean.bat
```
或
```powershell
.\clean.ps1
```

### 方式 2: 手动清理
删除以下目录:
- `build/`
- `test/build/`
- `.vs/`

## ?? 输出文件

### 主项目输出
- `build/native.dll` - 主 DLL 文件 (~83 KB)
- `build/native.pdb` - 调试符号
- `build/native.lib` - 导入库
- `build/native.exp` - 导出文件

### 测试项目输出
- `test/build/test_dll.exe` - 测试程序 (~87 KB)
- `test/build/test_dll.pdb` - 调试符号

## ?? 开发工作流

### 1. 初次构建
```bash
# 构建主 DLL
mkdir build && cd build
cmake -G Ninja ..
cmake --build .

# 构建测试程序
cd ../test
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

### 2. 修改代码后重新编译
```bash
# 主项目
cd build
cmake --build .

# 测试项目
cd test/build
cmake --build .
```

### 3. 清理重建
```bash
.\clean.bat  # 或 .\clean.ps1
# 然后重复步骤 1
```

### 4. 运行测试
```bash
cd test\build
.\test_dll.exe
```

## ?? 代码架构

### 模块依赖关系
```
DllMain.cpp
    └── import plugin.Plugin
     └── import window.Window
        └── import window.Renderer
```

### 数据流
```
外部调用 SetLyric()
    ↓
Plugin::setLyric()
    ↓
Window::setLyric()
    ↓
Window::refresh()
    ↓
Renderer::render()
    ↓
显示在任务栏
```

## ?? 文档

- **USAGE.md**: 详细的使用说明和 API 文档
- **BUILD_SUCCESS.md**: 构建成功后的快速开始指南
- **PROJECT_STRUCTURE.md**: 本文件，项目结构说明

## ?? 技术栈

- **语言**: C++20
- **构建系统**: CMake 3.30+ with Ninja
- **编译器**: MSVC 19.50+
- **图形 API**: Direct2D, DirectWrite
- **平台**: Windows 10/11

## ?? 单例保护

项目使用全局命名互斥锁实现单例:
```cpp
CreateMutex(nullptr, true, L"Global\\Taskbar-Lyrics-Musicfox");
```

这确保同一时间只能运行一个实例。

## ?? 注意事项

1. **必须使用 Ninja 生成器**: NMake 不支持 C++20 模块
2. **必须使用 MSVC 编译器**: GCC/Clang 的模块支持不完整
3. **不要混用编译器**: 确保整个项目使用同一个编译器
4. **头文件顺序**: 在模块文件中，`#include` 必须在 `export module` 之前
