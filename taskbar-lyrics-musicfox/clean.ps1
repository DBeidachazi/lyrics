# 清理构建文件和临时文件
# PowerShell 版本

$ErrorActionPreference = "Continue"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "清理构建文件和临时文件" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$rootDir = $PSScriptRoot

# 清理主项目构建目录
Write-Host "[1/4] 清理主项目构建目录..." -ForegroundColor Yellow
$buildDir = Join-Path $rootDir "build"
if (Test-Path $buildDir) {
    Remove-Item -Recurse -Force $buildDir
    Write-Host "? 已删除 build\" -ForegroundColor Green
} else {
    Write-Host "○ build\ 不存在" -ForegroundColor Gray
}

Write-Host ""

# 清理测试项目构建目录
Write-Host "[2/4] 清理测试项目构建目录..." -ForegroundColor Yellow
$testBuildDir = Join-Path $rootDir "test\build"
if (Test-Path $testBuildDir) {
    Remove-Item -Recurse -Force $testBuildDir
    Write-Host "? 已删除 test\build\" -ForegroundColor Green
} else {
    Write-Host "○ test\build\ 不存在" -ForegroundColor Gray
}

Write-Host ""

# 清理 Visual Studio 临时文件
Write-Host "[3/4] 清理 Visual Studio 临时文件..." -ForegroundColor Yellow
$vsDir = Join-Path $rootDir ".vs"
if (Test-Path $vsDir) {
    Remove-Item -Recurse -Force $vsDir
    Write-Host "? 已删除 .vs\" -ForegroundColor Green
} else {
    Write-Host "○ .vs\ 不存在" -ForegroundColor Gray
}

Write-Host ""

# 清理其他临时文件
Write-Host "[4/4] 清理其他临时文件..." -ForegroundColor Yellow
$extensions = @("*.obj", "*.ilk", "*.exp", "*.ifc", "*.pcm", "*.log")
$count = 0
foreach ($ext in $extensions) {
    $files = Get-ChildItem -Path $rootDir -Filter $ext -Recurse -File -ErrorAction SilentlyContinue
  foreach ($file in $files) {
     Remove-Item $file.FullName -Force -ErrorAction SilentlyContinue
   $count++
    }
}
Write-Host "? 已清理 $count 个临时文件" -ForegroundColor Green

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "? 清理完成！" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "提示: 下次构建前请运行:" -ForegroundColor Cyan
Write-Host "  mkdir build; cd build" -ForegroundColor Gray
Write-Host "  cmake -G Ninja .." -ForegroundColor Gray
Write-Host "  cmake --build ." -ForegroundColor Gray
Write-Host ""
