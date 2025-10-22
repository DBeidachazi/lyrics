# �������ļ�����ʱ�ļ�
# PowerShell �汾

$ErrorActionPreference = "Continue"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "�������ļ�����ʱ�ļ�" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$rootDir = $PSScriptRoot

# ��������Ŀ����Ŀ¼
Write-Host "[1/4] ��������Ŀ����Ŀ¼..." -ForegroundColor Yellow
$buildDir = Join-Path $rootDir "build"
if (Test-Path $buildDir) {
    Remove-Item -Recurse -Force $buildDir
    Write-Host "? ��ɾ�� build\" -ForegroundColor Green
} else {
    Write-Host "�� build\ ������" -ForegroundColor Gray
}

Write-Host ""

# ���������Ŀ����Ŀ¼
Write-Host "[2/4] ���������Ŀ����Ŀ¼..." -ForegroundColor Yellow
$testBuildDir = Join-Path $rootDir "test\build"
if (Test-Path $testBuildDir) {
    Remove-Item -Recurse -Force $testBuildDir
    Write-Host "? ��ɾ�� test\build\" -ForegroundColor Green
} else {
    Write-Host "�� test\build\ ������" -ForegroundColor Gray
}

Write-Host ""

# ���� Visual Studio ��ʱ�ļ�
Write-Host "[3/4] ���� Visual Studio ��ʱ�ļ�..." -ForegroundColor Yellow
$vsDir = Join-Path $rootDir ".vs"
if (Test-Path $vsDir) {
    Remove-Item -Recurse -Force $vsDir
    Write-Host "? ��ɾ�� .vs\" -ForegroundColor Green
} else {
    Write-Host "�� .vs\ ������" -ForegroundColor Gray
}

Write-Host ""

# ����������ʱ�ļ�
Write-Host "[4/4] ����������ʱ�ļ�..." -ForegroundColor Yellow
$extensions = @("*.obj", "*.ilk", "*.exp", "*.ifc", "*.pcm", "*.log")
$count = 0
foreach ($ext in $extensions) {
    $files = Get-ChildItem -Path $rootDir -Filter $ext -Recurse -File -ErrorAction SilentlyContinue
  foreach ($file in $files) {
     Remove-Item $file.FullName -Force -ErrorAction SilentlyContinue
   $count++
    }
}
Write-Host "? ������ $count ����ʱ�ļ�" -ForegroundColor Green

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "? ������ɣ�" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "��ʾ: �´ι���ǰ������:" -ForegroundColor Cyan
Write-Host "  mkdir build; cd build" -ForegroundColor Gray
Write-Host "  cmake -G Ninja .." -ForegroundColor Gray
Write-Host "  cmake --build ." -ForegroundColor Gray
Write-Host ""
