@echo off
chcp 65001 >nul

echo ========================================
echo Clean Build Files and Temporary Files
echo ========================================
echo.

set ROOT_DIR=%~dp0

echo [1/4] Cleaning main project build directory...
if exist "%ROOT_DIR%build" (
    rmdir /s /q "%ROOT_DIR%build"
    echo Done: Deleted build\
) else (
    echo Skip: build\ does not exist
)

echo.
echo [2/4] Cleaning test project build directory...
if exist "%ROOT_DIR%test\build" (
    rmdir /s /q "%ROOT_DIR%test\build"
    echo Done: Deleted test\build\
) else (
  echo Skip: test\build\ does not exist
)

echo.
echo [3/4] Cleaning Visual Studio temporary files...
if exist "%ROOT_DIR%.vs" (
    rmdir /s /q "%ROOT_DIR%.vs"
    echo Done: Deleted .vs\
) else (
    echo Skip: .vs\ does not exist
)

echo.
echo [4/4] Cleaning other temporary files...
del /q /s "%ROOT_DIR%*.obj" 2>nul
del /q /s "%ROOT_DIR%*.ilk" 2>nul
del /q /s "%ROOT_DIR%*.exp" 2>nul
del /q /s "%ROOT_DIR%*.ifc" 2>nul
del /q /s "%ROOT_DIR%*.pcm" 2>nul
echo Done: Cleaned temporary files

echo.
echo ========================================
echo Clean complete!
echo ========================================
echo.
echo Tip: To rebuild, run:
echo   mkdir build ^&^& cd build
echo   cmake -G Ninja ..
echo   cmake --build .
echo.
pause
