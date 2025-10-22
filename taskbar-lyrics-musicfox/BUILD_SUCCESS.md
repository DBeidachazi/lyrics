# ?? ����ɹ���

## ? ���ɵ��ļ�

1. **�� DLL**: `build\native.dll` (83 KB)
2. **���Գ���**: `test\build\test_dll.exe` (87 KB)

## ?? ������в���

### ���� 1: ֱ������ (���)

```powershell
cd test\build
.\test_dll.exe
```

����˫�� `test\build\test_dll.exe`

### ���� 2: ʹ�ýű�

```powershell
# PowerShell
.\test\build_and_run.ps1

# �� CMD
.\test\build_and_run.bat
```

## ?? Ԥ��Ч��

���� `test_dll.exe` ����Ӧ�ÿ�����

1. **����̨���**:
   ```
   DLL loaded successfully!
   SetLyric function found!
   Displaying lyrics on taskbar...
   Set lyric: ��һ����
   Set lyric: �ڶ�����
   Set lyric: ��������
   Set lyric: ���ľ���
   ```

2. **��������ʾ**: �� Windows �������������һ����͸�����ڣ���ʾ��ǰ���

3. **�Զ�ѭ��**: ÿ 3 ���л�һ�θ��

## ?? ���ֹͣ

�� `Ctrl + C` ֹͣ����

## ?? �Զ�����

�༭ `test\test_dll.cpp` �ļ��е� `testLyrics` ���飺

```cpp
std::vector<std::wstring> testLyrics = {
    L"��ĵ�һ����",
    L"��ĵڶ�����",
    L"��ĵ�������",
    // ... ������
};
```

Ȼ�����±��룺

```powershell
cd test\build
cmake --build .
.\test_dll.exe
```

## ?? ���ɵ����ֲ�����

�ο� `USAGE.md` �е���ϸ˵�����˽���Σ�
- �� C++ �������
- �� Python ����
- �� Node.js ����
- �� Musicfox ����

## ?? ע������

1. **�����ȱ�������Ŀ**: ȷ�� `build\native.dll` ����
2. **��Ҫͬʱ���ж��ʵ��**: DLL ʹ�õ���ģʽ
3. **���ֳ�������**: ��Ҫ�ر� `test_dll.exe`�������ʴ��ڻ���ʧ

## ?? �����Ų�

### �Ҳ��� native.dll
```
Failed to load native.dll
Error code: 126
```
**�������**: ȷ�� `build\native.dll` ���ڣ����±�������Ŀ

### ���ڲ���ʾ
- ����������Ƿ�ɼ�
- ȷ��ֻ��һ��ʵ��������
- ������������

## ?? ������Ϣ

�鿴�����ļ��˽���ࣺ
- `USAGE.md` - ������ʹ���ĵ�
- `test\test_dll.cpp` - ���Գ���Դ����
- `src\plugin\Plugin.cppm` - DLL ��Ҫ�߼�
- `src\window\Window.cppm` - ����ʵ��
