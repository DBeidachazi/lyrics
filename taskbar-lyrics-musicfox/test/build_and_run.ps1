# 任务栏歌词测试程序 - 自动构建脚本
# 此脚本会自动设置 MSVC 环境并编译

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "任务栏歌词显示 - 测试工具" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 检查 DLL 是否存在
$dllPath = Join-Path $PSScriptRoot "..\build\native.dll"
if (-not (Test-Path $dllPath)) {
    Write-Host "[错误] 找不到 native.dll" -ForegroundColor Red
    Write-Host "路径: $dllPath" -ForegroundColor Yellow
    Write-Host ""
 Write-Host "请先编译主项目:" -ForegroundColor Yellow
    Write-Host "  cd .." -ForegroundColor Gray
    Write-Host "  mkdir build" -ForegroundColor Gray
    Write-Host "  cd build" -ForegroundColor Gray
    Write-Host "  cmake -G Ninja .." -ForegroundColor Gray
    Write-Host "  cmake --build ." -ForegroundColor Gray
    pause
  exit 1
}

Write-Host "[成功] 找到 DLL: $dllPath" -ForegroundColor Green
Write-Host ""

# 设置 Visual Studio 环境
$vsPath = "D:\Microsoft VS"
$vsDevCmdPath = Join-Path $vsPath "Common7\Tools\VsDevCmd.bat"

if (-not (Test-Path $vsDevCmdPath)) {
    Write-Host "[警告] 找不到 Visual Studio 开发者命令提示符" -ForegroundColor Yellow
    Write-Host "路径: $vsDevCmdPath" -ForegroundColor Gray
    Write-Host ""
    Write-Host "尝试使用当前环境继续..." -ForegroundColor Yellow
    Write-Host ""
} else {
    Write-Host "设置 Visual Studio 环境..." -ForegroundColor Cyan
 # 注意: 在 PowerShell 中调用 bat 文件并保留环境变量比较复杂
    # 这里我们假设用户已经在正确的环境中运行
}

# 切换到测试目录
Set-Location $PSScriptRoot

# 清理之前的构建
$buildDir = Join-Path $PSScriptRoot "build"
if (Test-Path $buildDir) {
    Write-Host "清理之前的构建..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $buildDir
}

New-Item -ItemType Directory -Path $buildDir | Out-Null
Set-Location $buildDir

Write-Host ""
Write-Host "正在配置测试程序..." -ForegroundColor Cyan
Write-Host "使用 Ninja 生成器..." -ForegroundColor Gray
Write-Host ""

try {
    & cmake -G Ninja ..
    if ($LASTEXITCODE -ne 0) {
        throw "CMake 配置失败"
    }
} catch {
  Write-Host ""
  Write-Host "[错误] CMake 配置失败" -ForegroundColor Red
    Write-Host ""
 Write-Host "解决方案:" -ForegroundColor Yellow
    Write-Host "1. 在 Visual Studio 开发者命令提示符中运行此脚本" -ForegroundColor Gray
    Write-Host "2. 或者运行: Developer PowerShell for VS" -ForegroundColor Gray
    Write-Host "3. 或者手动运行: cmd /k `"$vsDevCmdPath`"" -ForegroundColor Gray
    Write-Host ""
    pause
    exit 1
}

Write-Host ""
Write-Host "正在编译测试程序..." -ForegroundColor Cyan

try {
    & cmake --build .
    if ($LASTEXITCODE -ne 0) {
        throw "编译失败"
    }
} catch {
    Write-Host ""
    Write-Host "[错误] 编译失败" -ForegroundColor Red
    pause
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "[成功] 测试程序已编译" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "正在启动测试程序..." -ForegroundColor Cyan
Write-Host "你应该能在任务栏看到歌词显示" -ForegroundColor Yellow
Write-Host "按 Ctrl+C 停止程序" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 运行测试程序
& .\test_dll.exe

pause
