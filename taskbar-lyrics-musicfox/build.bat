cd C:\Users\lenovo\Desktop\tmp\lyrics\taskbar-lyrics-musicfox
rmdir /s /q build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cd ../test
rmdir /s /q build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cd ../../build/Release
copy native.dll ..\..\test\build\Release