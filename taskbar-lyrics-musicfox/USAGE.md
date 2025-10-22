# �����������ʾ DLL ʹ��˵��

## ?? ���ɵ��ļ�λ��

����ɹ���DLL �ļ�λ�ڣ�
```
C:\Users\lenovo\Desktop\tmp\lyrics\taskbar-lyrics-musicfox\build\native.dll
```

## ?? ʹ�÷�ʽ

### ��ʽ 1: ���ٲ��ԣ��Ƽ���

1. **������Գ���**
   ```bash
   cd test
   build_and_run.bat
   ```

2. **�����ֶ�����**
   ```bash
   cd test
   mkdir build
   cd build
   cmake -G Ninja ..
   cmake --build .
   test_dll.exe
   ```

### ��ʽ 2: ����� C++ ������ʹ��

```cpp
#include <Windows.h>

// ���庯��ָ������
typedef void (*SetLyricFunc)(const wchar_t*);

int main() {
    // 1. ���� DLL
    HMODULE hDll = LoadLibrary(L"native.dll");
    if (!hDll) {
   // ����ʧ��
        return -1;
    }
    
    // ��ʱ�����Ѿ��Զ���������ʾ����������
    
    // 2. ��ȡ SetLyric ����
    SetLyricFunc setLyric = (SetLyricFunc)GetProcAddress(hDll, "SetLyric");
    
    // 3. ���¸��
    if (setLyric) {
        setLyric(L"��ĸ������");
    }
    
// 4. ���ֳ������У���Ҫж�� DLL��
    // ������� FreeLibrary�����ڻ�ر�
    
    return 0;
}
```

### ��ʽ 3: �� Musicfox ���������ֲ���������

#### Node.js ʾ��
```javascript
const ffi = require('ffi-napi');

// ���� DLL
const native = ffi.Library('native.dll', {
  'SetLyric': ['void', ['string']]
});

// ���¸��
function updateLyric(lyric) {
  native.SetLyric(lyric);
}

// ʹ��
updateLyric('��ǰ���ŵĸ��');
```

#### Python ʾ��
```python
import ctypes

# ���� DLL
dll = ctypes.CDLL('native.dll')

# ���ú�����������
dll.SetLyric.argtypes = [ctypes.c_wchar_p]
dll.SetLyric.restype = None

# ���¸��
def update_lyric(lyric):
    dll.SetLyric(lyric)

# ʹ��
update_lyric('��ǰ���ŵĸ��')
```

#### PowerShell ʾ��
```powershell
# ���в��Խű�
.\test\run_dll.ps1
```

## ?? DLL ����ԭ��

1. **�Զ���ʼ��**: �� DLL ������ʱ��`DLL_PROCESS_ATTACH`�������Զ���
   - ����Ƿ�����ʵ�����У�ͨ��ȫ�ֻ�������
   - ���������������ʾ����
   - ������Ϣѭ��

2. **����ģʽ**: ͬһʱ��ֻ����һ��ʵ������

3. **��������**: `SetLyric(const wchar_t* lyric)`
   - ������ʾ�ĸ���ı�
   - �̰߳�ȫ

## ?? API �ο�

### SetLyric
```cpp
extern "C" __declspec(dllexport) void SetLyric(const wchar_t* lyric);
```

**����:**
- `lyric`: Ҫ��ʾ�ĸ���ı���Unicode �ַ�����

**˵��:**
- ��� `lyric` Ϊ NULL����������ִ���κβ���
- ���»�������ӳ��������������

## ?? ��������

- **λ��**: �������������
- **��С**: ��� 600px���߶ȸ����������߶�
- **��ʽ**: 
  - ��͸������
  - ��ɫ�ı�
  - ���ж���
  - ʹ�� Microsoft YaHei ����

## ?? ע������

1. **��Ҫж�� DLL**: ������� `FreeLibrary`�����ڻ�ر�

2. **��ʵ��**: �������ʵ�����У��µļ��ػᱻ�ܾ�

3. **COM ��ʼ��**: DLL �ڲ����Զ���ʼ�� COM��`COINIT_MULTITHREADED`��

4. **�̰߳�ȫ**: ���Դ��κ��̵߳��� `SetLyric`

## ?? �����Ų�

### DLL ����ʧ��
- ��� DLL ·���Ƿ���ȷ
- ȷ������������ DLL ����ϵͳ·����
- ����Ƿ�Ϊ��ȷ�ļܹ���x64 vs x86��

### ���ڲ���ʾ
- ����������Ƿ�ɼ�
- �������¼��� DLL
- �鿴�Ƿ�������ʵ����������

### ��ʲ�����
- ȷ����ȷ��ȡ�� `SetLyric` ����ָ��
- ��鴫����ַ����Ƿ�Ϊ Unicode (wchar_t*)
- ��֤ DLL ʵ���Ƿ���������

## ?? ���ɵ� Musicfox

������뽫�˹��ܼ��ɵ� Musicfox�����ԣ�

1. �޸� Musicfox Դ���룬�ڲ��Ÿ���ʱ���� `SetLyric`
2. ����һ�� Musicfox ��������ش� DLL
3. ʹ�� IPC ���ƣ��������ܵ����� Musicfox ͨ��

## ?? ����ļ�

- `build/native.dll` - �� DLL �ļ�
- `build/native.pdb` - ���Է���
- `test/test_dll.cpp` - ���Գ���Դ����
- `test/build_and_run.bat` - ���ٱ������нű�
- `test/run_dll.ps1` - PowerShell ���Խű�
