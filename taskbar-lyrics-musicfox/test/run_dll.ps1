# 测试脚本 - 使用 rundll32 加载 DLL
# 注意：由于这个 DLL 在加载时就会创建窗口（DLL_PROCESS_ATTACH），
# 简单加载就可以启动它

Write-Host "正在启动任务栏歌词显示..."

# DLL 路径
$dllPath = Join-Path $PSScriptRoot "..\build\native.dll"

if (-not (Test-Path $dllPath)) {
    Write-Host "错误: 找不到 native.dll 文件" -ForegroundColor Red
    Write-Host "请先编译项目" -ForegroundColor Yellow
    exit 1
}

Write-Host "找到 DLL: $dllPath" -ForegroundColor Green

# 方法1: 使用 PowerShell 加载 DLL
Write-Host "`n方法1: 直接加载 DLL (窗口会自动启动)" -ForegroundColor Cyan
Write-Host "执行命令: [System.Reflection.Assembly]::LoadFile(`"$dllPath`")" -ForegroundColor Gray

# 使用 .NET 加载（这会触发 DllMain）
try {
    Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;

public class NativeDll {
    [DllImport("$dllPath", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    public static extern void SetLyric(string lyric);
}
"@
    
    Write-Host "DLL 加载成功！" -ForegroundColor Green
    Write-Host "窗口应该已经在任务栏上显示了" -ForegroundColor Green
 
    # 测试设置歌词
    Write-Host "`n开始循环更新歌词..." -ForegroundColor Cyan
    
    $lyrics = @(
"你是我的小呀小苹果",
        "怎么爱你都不嫌多",
    "红红的小脸儿温暖我的心窝",
        "点亮我生命的火 火火火火"
    )
    
    $index = 0
    while ($true) {
    [NativeDll]::SetLyric($lyrics[$index])
        Write-Host "当前歌词: $($lyrics[$index])" -ForegroundColor Yellow
        $index = ($index + 1) % $lyrics.Length
  Start-Sleep -Seconds 3
    }
    
} catch {
    Write-Host "加载失败: $_" -ForegroundColor Red
    
    Write-Host "`n方法2: 使用简单的进程保持 DLL 加载" -ForegroundColor Cyan
    Write-Host "创建一个持续运行的进程来保持 DLL 加载状态..." -ForegroundColor Gray
    
    # 创建一个简单的可执行文件来加载 DLL
    $code = @"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    HMODULE hDll = LoadLibraryW(L"$($dllPath.Replace('\', '\\'))");
    if (!hDll) {
        std::cerr << "Failed to load DLL" << std::endl;
        return -1;
    }
    
    std::cout << "DLL loaded! Window should appear on taskbar." << std::endl;
    std::cout << "Press Ctrl+C to exit..." << std::endl;
    
    // 保持运行
    while (true) {
 std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
"@
    
    Write-Host $code -ForegroundColor Gray
    Write-Host "`n您可以将上述代码保存为 .cpp 文件并编译使用" -ForegroundColor Yellow
}
