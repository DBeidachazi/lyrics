@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul

echo ========================================
echo Taskbar Lyrics - Test Tool
echo ========================================
echo.

REM Save current directory
set "SCRIPT_DIR=%~dp0"
set "DLL_PATH=%SCRIPT_DIR%..\build\native.dll"

if not exist "%DLL_PATH%" (
    echo [ERROR] native.dll not found
    echo Path: %DLL_PATH%
    echo.
    echo Please build main project first:
    echo   cd ..
    echo mkdir build
    echo   cd build
    echo   cmake -G Ninja ..
    echo   cmake --build .
    pause
    exit /b 1
)

echo [SUCCESS] Found DLL: %DLL_PATH%
echo.

REM Switch to script directory
cd /d "%SCRIPT_DIR%"

REM Clean previous build
if exist build (
    echo Cleaning previous build...
    rmdir /s /q build
)

mkdir build
cd build

echo.
echo Configuring test program...
echo Using Ninja generator with MSVC compiler...
echo.

cmake.exe -G Ninja ..
if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration failed
    echo.
    echo Please run in Visual Studio Developer Command Prompt
    echo Or run: "D:\Microsoft VS\Common7\Tools\VsDevCmd.bat"
    echo.
    pause
    exit /b 1
)

echo.
echo Building test program...
cmake.exe --build .
if errorlevel 1 (
    echo.
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo.
echo ========================================
echo [SUCCESS] Test program compiled
echo ========================================
echo.
echo Starting test program...
echo You should see lyrics on taskbar
echo Press Ctrl+C to stop
echo ========================================
echo.

test_dll.exe

pause
endlocal
