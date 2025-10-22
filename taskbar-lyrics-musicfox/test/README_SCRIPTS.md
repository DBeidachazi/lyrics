# Test Scripts Usage Guide

## Overview

This directory contains scripts to build and run the test program for the taskbar lyrics DLL.

## Available Scripts

### 1. `build.bat` - Build Only
Compiles the test program without running it.

```cmd
build.bat
```

**What it does:**
- Checks if main DLL exists
- Creates build directory
- Runs CMake configuration
- Builds test_dll.exe

**Output:** `build\test_dll.exe`

### 2. `run.bat` - Run Only  
Runs the already-built test program.

```cmd
run.bat
```

**Requirements:**
- `build\test_dll.exe` must exist
- `..\build\native.dll` must exist

**What it does:**
- Checks if required files exist
- Runs test_dll.exe

### 3. `build_and_run.bat` - Build and Run
Builds the test program and immediately runs it.

```cmd
build_and_run.bat
```

**What it does:**
- All steps from `build.bat`
- Automatically runs test_dll.exe after successful build

## Prerequisites

Before using these scripts:

1. **Build the main DLL first:**
   ```cmd
   cd ..
   mkdir build
   cd build
   cmake -G Ninja ..
   cmake --build .
   ```

2. **Ensure you're in Visual Studio Developer Command Prompt** or have MSVC tools in PATH

## Quick Start

### First Time Setup

```cmd
REM 1. Build main project
cd ..
mkdir build
cd build
cmake -G Ninja ..
cmake --build .

REM 2. Build and run test
cd ..\test
build_and_run.bat
```

### Subsequent Runs

If test program is already built:
```cmd
run.bat
```

If you modified test code:
```cmd
build_and_run.bat
```

## Troubleshooting

### "native.dll not found"
**Solution:** Build the main project first (see Prerequisites)

### "CMake configuration failed"
**Solution:** Run in Visual Studio Developer Command Prompt:
```cmd
"D:\Microsoft VS\Common7\Tools\VsDevCmd.bat"
```

### "Build failed"
**Solution:** 
1. Clean and rebuild: `clean.bat` (in root directory)
2. Rebuild main project
3. Rebuild test project

### Script shows garbled characters
**Solution:** All scripts use English now to avoid encoding issues. If you still see issues, ensure your terminal supports UTF-8.

## What the Test Program Does

When you run `test_dll.exe`:
1. Loads `native.dll`
2. Gets the `SetLyric` function
3. Displays lyrics on the taskbar
4. Updates lyrics every 3 seconds
5. Cycles through test lyrics

**To stop:** Press `Ctrl+C`

## File Structure

```
test/
©À©¤©¤ test_dll.cpp   # Test program source code
©À©¤©¤ CMakeLists.txt        # CMake configuration
©À©¤©¤ build.bat             # Build script
©À©¤©¤ run.bat               # Run script
©À©¤©¤ build_and_run.bat     # Build + Run script
©À©¤©¤ README_SCRIPTS.md     # This file
©¸©¤©¤ build/             # Build output directory
    ©À©¤©¤ test_dll.exe    # Compiled test program
    ©¸©¤©¤ ...    # Other build files
```

## PowerShell Alternative

If you prefer PowerShell, you can use:

```powershell
# Build
cd build
cmake -G Ninja ..
cmake --build .

# Run
cd build
.\test_dll.exe
```

## Notes

- All scripts automatically clean previous builds
- Scripts check for required files before proceeding
- Error messages are clear and actionable
- All output is in English to avoid encoding issues

## Related Files

- `../build/native.dll` - Main DLL (must exist)
- `../clean.bat` - Clean all build files (root directory)
- `../README.md` - Main project documentation
