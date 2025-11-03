@echo off
REM ==============================================
REM 仅构建 native.dll
REM ==============================================

echo ========================================
echo 构建 native.dll
echo ========================================
echo.

if exist "build-dll" (
    echo 清理旧的构建目录...
    rmdir /s /q build-dll
)

mkdir build-dll
cd build-dll

echo 配置项目...
cmake -G "Visual Studio 17 2022" -A x64 ..
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] 配置失败！
    echo 请确保已安装 Visual Studio 2022
    cd ..
    pause
    exit /b 1
)

echo.
echo 编译项目...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] 编译失败！
    cd ..
    pause
    exit /b 1
)

cd ..

echo.
echo ========================================
echo 构建成功！
echo ========================================
echo.
echo 输出文件: build-dll\Release\native.dll
echo.

if not exist "..\bin" mkdir ..\bin
copy build-dll\Release\native.dll ..\bin\ >nul
echo 已复制到: ..\bin\native.dll
echo.
pause

