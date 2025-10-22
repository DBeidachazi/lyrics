# ��������ʲ��Գ��� - �Զ������ű�
# �˽ű����Զ����� MSVC ����������

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "�����������ʾ - ���Թ���" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# ��� DLL �Ƿ����
$dllPath = Join-Path $PSScriptRoot "..\build\native.dll"
if (-not (Test-Path $dllPath)) {
    Write-Host "[����] �Ҳ��� native.dll" -ForegroundColor Red
    Write-Host "·��: $dllPath" -ForegroundColor Yellow
    Write-Host ""
 Write-Host "���ȱ�������Ŀ:" -ForegroundColor Yellow
    Write-Host "  cd .." -ForegroundColor Gray
    Write-Host "  mkdir build" -ForegroundColor Gray
    Write-Host "  cd build" -ForegroundColor Gray
    Write-Host "  cmake -G Ninja .." -ForegroundColor Gray
    Write-Host "  cmake --build ." -ForegroundColor Gray
    pause
  exit 1
}

Write-Host "[�ɹ�] �ҵ� DLL: $dllPath" -ForegroundColor Green
Write-Host ""

# ���� Visual Studio ����
$vsPath = "D:\Microsoft VS"
$vsDevCmdPath = Join-Path $vsPath "Common7\Tools\VsDevCmd.bat"

if (-not (Test-Path $vsDevCmdPath)) {
    Write-Host "[����] �Ҳ��� Visual Studio ������������ʾ��" -ForegroundColor Yellow
    Write-Host "·��: $vsDevCmdPath" -ForegroundColor Gray
    Write-Host ""
    Write-Host "����ʹ�õ�ǰ��������..." -ForegroundColor Yellow
    Write-Host ""
} else {
    Write-Host "���� Visual Studio ����..." -ForegroundColor Cyan
 # ע��: �� PowerShell �е��� bat �ļ����������������Ƚϸ���
    # �������Ǽ����û��Ѿ�����ȷ�Ļ���������
}

# �л�������Ŀ¼
Set-Location $PSScriptRoot

# ����֮ǰ�Ĺ���
$buildDir = Join-Path $PSScriptRoot "build"
if (Test-Path $buildDir) {
    Write-Host "����֮ǰ�Ĺ���..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $buildDir
}

New-Item -ItemType Directory -Path $buildDir | Out-Null
Set-Location $buildDir

Write-Host ""
Write-Host "�������ò��Գ���..." -ForegroundColor Cyan
Write-Host "ʹ�� Ninja ������..." -ForegroundColor Gray
Write-Host ""

try {
    & cmake -G Ninja ..
    if ($LASTEXITCODE -ne 0) {
        throw "CMake ����ʧ��"
    }
} catch {
  Write-Host ""
  Write-Host "[����] CMake ����ʧ��" -ForegroundColor Red
    Write-Host ""
 Write-Host "�������:" -ForegroundColor Yellow
    Write-Host "1. �� Visual Studio ������������ʾ�������д˽ű�" -ForegroundColor Gray
    Write-Host "2. ��������: Developer PowerShell for VS" -ForegroundColor Gray
    Write-Host "3. �����ֶ�����: cmd /k `"$vsDevCmdPath`"" -ForegroundColor Gray
    Write-Host ""
    pause
    exit 1
}

Write-Host ""
Write-Host "���ڱ�����Գ���..." -ForegroundColor Cyan

try {
    & cmake --build .
    if ($LASTEXITCODE -ne 0) {
        throw "����ʧ��"
    }
} catch {
    Write-Host ""
    Write-Host "[����] ����ʧ��" -ForegroundColor Red
    pause
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "[�ɹ�] ���Գ����ѱ���" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "�����������Գ���..." -ForegroundColor Cyan
Write-Host "��Ӧ���������������������ʾ" -ForegroundColor Yellow
Write-Host "�� Ctrl+C ֹͣ����" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# ���в��Գ���
& .\test_dll.exe

pause
