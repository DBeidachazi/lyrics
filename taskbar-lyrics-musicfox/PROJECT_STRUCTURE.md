# ��Ŀ�ṹ˵��

## ?? Ŀ¼�ṹ

```
taskbar-lyrics-musicfox/
������ src/   # Դ����Ŀ¼
��   ������ DllMain.cpp              # DLL ��ڵ�
��   ������ plugin/
��   ��   ������ Plugin.cppm          # �������ģ�飨������
��   ������ window/
��  ������ Window.cppm          # ���ڹ���ģ��
��    ������ Renderer.cppm        # Direct2D ��Ⱦģ��
��
������ test/         # ���Դ���
��   ������ test_dll.cpp  # DLL ���Գ���
��   ������ CMakeLists.txt        # ������Ŀ CMake ����
��   ������ build_and_run.bat      # �������нű� (CMD)
�������� build_and_run.ps1     # �������нű� (PowerShell)
��   ������ run_dll.ps1     # DLL ���Խű�
��
������ build/   # ����Ŀ������� (gitignored)
��   ������ native.dll     # �������ɵ� DLL
��
������ CMakeLists.txt       # ����Ŀ CMake ����
������ .gitignore          # Git ���Թ���
������ clean.bat # ����ű� (CMD)
������ clean.ps1                # ����ű� (PowerShell)
��
������ USAGE.md          # ʹ��˵���ĵ�
������ BUILD_SUCCESS.md     # �����ɹ�ָ��
������ PROJECT_STRUCTURE.md    # ���ļ�
```

## ?? �����ļ�˵��

### Դ���� (src/)

#### `DllMain.cpp`
- **����**: DLL ����ڵ�
- **����**:
  - ���� DLL ����/ж���¼�
  - ��ʼ�� COM
  - ���� Plugin ����
  - ���� `SetLyric` ����

#### `plugin/Plugin.cppm`
- **����**: �������ģ��
- **����**:
  - ����ģʽ��ȷ��ֻ��һ��ʵ��
  - ʹ��ȫ�ֻ�������ֹ��ʵ��
  - ���� Window �������������
  - �ṩ `setLyric` �ӿ�

#### `window/Window.cppm`
- **����**: ���ڹ���ģ��
- **����**:
  - �����͹�������������
  - ���� Windows ��Ϣ
  - ������λ�úʹ�С
  - Э�� Renderer ������Ⱦ

#### `window/Renderer.cppm`
- **����**: ��Ⱦģ��
- **����**:
  - ʹ�� Direct2D �� DirectWrite ��Ⱦ����
  - �������塢��ɫ����ʽ
  - �����ڴ�С�仯

### ���Դ��� (test/)

#### `test_dll.cpp`
- **����**: DLL ���ܲ��Գ���
- **����**:
  - ��̬���� native.dll
  - ���� SetLyric ����
  - ѭ����ʾ���Ը��

## ?? ����ϵͳ

### CMake ����
- **����Ŀ**: `CMakeLists.txt`
  - ʹ�� C++20 ģ��
  - ��Ҫ Ninja ������
  - ��� CMake �汾 3.30

- **������Ŀ**: `test/CMakeLists.txt`
  - �����Ŀ�ִ���ļ�
  - ���� Windows ϵͳ��

### ��������

**����Ŀ**:
```bash
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

**������Ŀ**:
```bash
cd test
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

## ??? ����

### ��ʽ 1: ʹ�ýű�
```cmd
clean.bat
```
��
```powershell
.\clean.ps1
```

### ��ʽ 2: �ֶ�����
ɾ������Ŀ¼:
- `build/`
- `test/build/`
- `.vs/`

## ?? ����ļ�

### ����Ŀ���
- `build/native.dll` - �� DLL �ļ� (~83 KB)
- `build/native.pdb` - ���Է���
- `build/native.lib` - �����
- `build/native.exp` - �����ļ�

### ������Ŀ���
- `test/build/test_dll.exe` - ���Գ��� (~87 KB)
- `test/build/test_dll.pdb` - ���Է���

## ?? ����������

### 1. ���ι���
```bash
# ������ DLL
mkdir build && cd build
cmake -G Ninja ..
cmake --build .

# �������Գ���
cd ../test
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

### 2. �޸Ĵ�������±���
```bash
# ����Ŀ
cd build
cmake --build .

# ������Ŀ
cd test/build
cmake --build .
```

### 3. �����ؽ�
```bash
.\clean.bat  # �� .\clean.ps1
# Ȼ���ظ����� 1
```

### 4. ���в���
```bash
cd test\build
.\test_dll.exe
```

## ?? ����ܹ�

### ģ��������ϵ
```
DllMain.cpp
    ������ import plugin.Plugin
     ������ import window.Window
        ������ import window.Renderer
```

### ������
```
�ⲿ���� SetLyric()
    ��
Plugin::setLyric()
    ��
Window::setLyric()
    ��
Window::refresh()
    ��
Renderer::render()
    ��
��ʾ��������
```

## ?? �ĵ�

- **USAGE.md**: ��ϸ��ʹ��˵���� API �ĵ�
- **BUILD_SUCCESS.md**: �����ɹ���Ŀ��ٿ�ʼָ��
- **PROJECT_STRUCTURE.md**: ���ļ�����Ŀ�ṹ˵��

## ?? ����ջ

- **����**: C++20
- **����ϵͳ**: CMake 3.30+ with Ninja
- **������**: MSVC 19.50+
- **ͼ�� API**: Direct2D, DirectWrite
- **ƽ̨**: Windows 10/11

## ?? ��������

��Ŀʹ��ȫ������������ʵ�ֵ���:
```cpp
CreateMutex(nullptr, true, L"Global\\Taskbar-Lyrics-Musicfox");
```

��ȷ��ͬһʱ��ֻ������һ��ʵ����

## ?? ע������

1. **����ʹ�� Ninja ������**: NMake ��֧�� C++20 ģ��
2. **����ʹ�� MSVC ������**: GCC/Clang ��ģ��֧�ֲ�����
3. **��Ҫ���ñ�����**: ȷ��������Ŀʹ��ͬһ��������
4. **ͷ�ļ�˳��**: ��ģ���ļ��У�`#include` ������ `export module` ֮ǰ
