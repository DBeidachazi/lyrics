@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul

echo ========================================
echo Build Test Program Only
echo ========================================
echo.

set "SCRIPT_DIR=%~dp0"
set "DLL_PATH=%SCRIPT_DIR%..\build\native.dll"

if not exist "%DLL_PATH%" (
    echo [ERROR] native.dll not found at: %DLL_PATH%
    echo.
    echo Please build main project first:
    echo   cd ..
    echo   cd build (or mkdir build if not exists)
    echo   cmake -G Ninja ..
    echo   cmake --build .
    echo.
    pause
    exit /b 1
)

echo [OK] Found DLL: %DLL_PATH%

cd /d "%SCRIPT_DIR%"

if not exist build mkdir build
cd build

echo.
echo Running CMake...
cmake.exe -G Ninja ..
if errorlevel 1 (
    echo [ERROR] CMake failed
    pause
    exit /b 1
)

echo.
echo Building...
cmake.exe --build .
if errorlevel 1 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo.
echo ========================================
echo [SUCCESS] Build complete!
echo ========================================
echo.
echo Output: %SCRIPT_DIR%build\test_dll.exe
echo.
echo To run: cd build ^&^& test_dll.exe
echo Or use: run.bat
echo.
pause

endlocal
