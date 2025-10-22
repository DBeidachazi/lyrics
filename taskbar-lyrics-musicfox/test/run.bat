@echo off
setlocal
chcp 65001 >nul

echo ========================================
echo Run Taskbar Lyrics Test
echo ========================================
echo.

REM Check if DLL exists
if not exist "..\build\native.dll" (
    echo [ERROR] native.dll not found
    echo Please build main project first!
    echo.
    pause
    exit /b 1
)

REM Check if test program exists
if not exist "build\test_dll.exe" (
    echo [ERROR] test_dll.exe not found
    echo Please run build_and_run.bat first!
    echo.
    pause
    exit /b 1
)

echo [SUCCESS] Found required files
echo.
echo Starting test program...
echo You should see lyrics on taskbar
echo Press Ctrl+C to stop
echo ========================================
echo.

cd build
test_dll.exe

endlocal
