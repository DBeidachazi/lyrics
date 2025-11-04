# go-musicfox 任务栏歌词 (Windows 11)

为 [go-musicfox](https://github.com/go-musicfox/go-musicfox) 添加 Windows 任务栏歌词显示功能。

## ✨ 功能特性

### 核心功能
- 🎵 在 Windows 任务栏显示实时歌词
- 🎨 支持主歌词和翻译歌词双行显示
- 🌓 自动跟随 Windows 主题切换歌词颜色（深色/浅色模式）
- 📍 智能检测任务栏图标位置，自动调整歌词显示区域
  - 图标居中时：歌词显示在左侧
  - 图标居左时：歌词显示在右侧
- ⌨️ 快捷键实时调整歌词字体大小
- ⚙️ 完整的配置文件支持（TOML/INI）

### 技术实现
- 使用 C++20 模块化架构的 DLL
- Direct2D 硬件加速渲染
- 实时监听系统主题和任务栏设置变化
- 低 CPU 占用（< 5%），内存占用小（< 50MB）

## 🚀 快速开始

### 1. 编译 DLL

```bash
cd taskbar-lyrics-musicfox
mkdir build
cd build

# 使用 Visual Studio (推荐)
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# 或使用 Ninja
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Windows 一键脚本
# build-dll-only.bat
```

编译完成后，DLL 位于 `build/Release/native.dll`

### 2. 编译 go-musicfox (可选)

如果需要自己编译 musicfox：

```bash
# 克隆 go-musicfox
git clone https://github.com/go-musicfox/go-musicfox.git
cd go-musicfox

# 应用补丁
git apply ../taskbar-lyrics-musicfox/full.patch

# Windows 下编译
make build

# Linux 交叉编译到 Windows
export GOOS=windows
export GOARCH=amd64
export CGO_ENABLED=1
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
make build
```

### 3. 部署

将 `native.dll` 和 `musicfox.exe` 放在同一目录即可。

**或者直接从 [Releases](https://github.com/你的仓库/releases) 下载已编译的版本。**

### 4. 配置

编辑 `~/.config/go-musicfox/config.toml`（或 `go-musicfox.ini`）：

#### TOML 配置格式

```toml
[main.lyric.taskbar]
# 是否启用任务栏歌词
enable = true

# 窗口对齐方式: 0=AUTO(自动), 1=LEFT(左), 2=CENTER(中), 3=RIGHT(右)
windowAlignment = 0

# 边距设置（像素）
marginLeft = 10
marginRight = 10
lineSpacing = 4

# 字体设置
fontFamily = "Microsoft YaHei UI"

# 主题颜色（推荐开启）
followSystemTheme = true  # true=跟随系统主题, false=使用自定义颜色

# 主歌词配置
sizePrimary = 20                    # 字体大小
colorPrimary = 0xFFFFFFFF          # 颜色（仅 followSystemTheme=false 时生效）
weightPrimary = 500                 # 字重 (100-900)
alignPrimary = 0                    # 对齐: 0=LEFT, 1=RIGHT, 2=CENTER

# 副歌词（翻译）配置
sizeSecondary = 13
colorSecondary = 0xFF999999
weightSecondary = 400
alignSecondary = 0
```

#### INI 配置格式

```ini
[main.taskbarLyric]
enable = true
windowAlignment = 0
marginLeft = 10
marginRight = 10
lineSpacing = 4
fontFamily = Microsoft YaHei UI
followSystemTheme = true
sizePrimary = 20
colorPrimary = 4294967295
weightPrimary = 500
alignPrimary = 0
sizeSecondary = 13
colorSecondary = 4289374617
weightSecondary = 400
alignSecondary = 0
```

**注意：** INI 格式颜色使用十进制数字：
- `0xFFFFFFFF` → `4294967295`（白色）
- `0xFF000000` → `4278190080`（黑色）

## ⌨️ 快捷键

| 功能 | 快捷键 |
|------|-----|
| 增大歌词字体 | `)` |
| 减小歌词字体 | `(` |

字体大小范围：10-48
部分输入法输入`（`会自动补全`）`导致减小歌词字体失效，可以切换英文再输入

## 🎨 主题颜色

### 自动跟随系统主题（推荐）

设置 `followSystemTheme = true`：

- **Windows 深色模式**：主歌词白色，副歌词浅灰
- **Windows 浅色模式**：主歌词黑色，副歌词深灰

### 自定义颜色

设置 `followSystemTheme = false`，然后配置 `colorPrimary` 和 `colorSecondary`。

颜色格式：`0xAARRGGBB`（ARGB）

常用颜色：
- 白色：`0xFFFFFFFF`
- 黑色：`0xFF000000`
- 红色：`0xFFFF0000`
- 网易云红：`0xFFEA403F`

## 📁 项目结构

```
lyrics/
├── README.md                          # 本文件
├── taskbar-lyrics-musicfox/           # 源代码（DLL）
│   ├── src/
│   │   ├── DllMain.cpp               # DLL 入口
│   │   ├── plugin/                   # 插件模块
│   │   │   ├── Config.cppm          # 配置管理
│   │   │   └── Plugin.cppm          # 插件主逻辑
│   │   ├── taskbar/                  # 任务栏模块
│   │   │   ├── Registry.cppm        # 注册表检测
│   │   │   └── Theme.cppm           # 主题检测
│   │   └── window/                   # 窗口渲染
│   │       ├── Window.cppm          # 窗口位置计算
│   │       └── Renderer.cppm        # Direct2D 渲染
│   └── CMakeLists.txt
├── go-musicfox/                       # 修改后的 go-musicfox
│   └── internal/
│       ├── configs/                  # 配置模块
│       │   ├── main.go              # 添加 TaskbarLyricConfig
│       │   ├── registry.go          # INI 读取支持
│       │   └── legacy.go            # INI→TOML 迁移
│       ├── keybindings/
│       │   └── keybindings.go       # 添加快捷键定义
│       └── ui/
│           ├── player.go            # DLL 加载和配置应用
│           └── event_handler.go     # 快捷键处理
└── Taskbar-Lyrics/                    # 参考项目
```

## 🔧 技术细节

### 任务栏图标位置检测

通过读取注册表自动判断：
```
HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced
- TaskbarAl: 1=居中, 0=居左
- TaskbarDa: 1=Widgets启用, 0=禁用
```

### 主题颜色检测

读取注册表检测系统主题：
```
HKCU\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize
- AppsUseLightTheme: 0=深色, 1=浅色
```

### 实时更新

- 监听注册表变化（`RegNotifyChangeKeyValue`）
- 用户切换任务栏设置或主题时自动刷新
- 无需重启程序

## 🐛 故障排查

### 歌词不显示

1. 确认 `native.dll` 在正确位置
2. 检查配置文件 `enable = true`
3. 查看日志文件 `dll.log`

### 歌词位置不对

1. 检查 `windowAlignment` 设置（推荐用 `0` 自动模式）
2. 调整 `marginLeft` 和 `marginRight`
3. 查看日志中的位置计算信息

### 颜色不正确

1. 确认 `followSystemTheme` 设置
2. 如果使用自定义颜色，检查颜色值格式
3. 尝试切换系统主题测试

## 📚 参考项目

- [go-musicfox](https://github.com/go-musicfox/go-musicfox) - 命令行音乐播放器
- [Taskbar-Lyrics](https://github.com/jitwxs/Taskbar-Lyrics) - 任务栏歌词显示参考
- [lyric-for-musicfox](https://github.com/SmileYik/lyric-for-musicfox) - 桌面歌词实现参考

## 📝 更新日志

### v0.1.0 (2025)

- ✅ 实现任务栏歌词显示
- ✅ 自动跟随系统主题颜色
- ✅ 智能检测任务栏图标位置
- ✅ 快捷键调整字体大小
- ✅ 完整的配置文件支持（TOML/INI）
- ✅ 实时响应系统设置变化

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 📄 许可证

本项目基于原项目许可证。

---

**Enjoy your music with taskbar lyrics! 🎵**
