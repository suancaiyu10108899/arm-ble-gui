# 检查 ARM-BLE-GUI 开发工具链是否就绪
# 用法: .\scripts\check-toolchain.ps1

$ErrorActionPreference = "Stop"
Write-Host "=== ARM-BLE-GUI 工具链检查 ===" -ForegroundColor Cyan

# 1. CMake
Write-Host "[1/5] CMake..." -NoNewline
try {
    $v = & cmake --version 2>&1 | Select-Object -First 1
    Write-Host " OK ($v)" -ForegroundColor Green
} catch {
    Write-Host " MISSING" -ForegroundColor Red
}

# 2. MSVC (cl.exe)
Write-Host "[2/5] MSVC (cl.exe)..." -NoNewline
try {
    $v = & cl.exe 2>&1 | Select-Object -First 1
    Write-Host " OK" -ForegroundColor Green
} catch {
    Write-Host " 未激活 (请先运行 VsDevShell)" -ForegroundColor Yellow
}

# 3. Ninja
Write-Host "[3/5] Ninja..." -NoNewline
try {
    $v = & ninja --version 2>&1
    Write-Host " OK ($v)" -ForegroundColor Green
} catch {
    Write-Host " 不可用 (可能需激活 VsDevShell)" -ForegroundColor Yellow
}

# 4. Qt 6
Write-Host "[4/5] Qt 6.11.0..." -NoNewline
if (Test-Path "D:\Qt\6.11.0\msvc2022_64") {
    Write-Host " OK" -ForegroundColor Green
} else {
    Write-Host " MISSING" -ForegroundColor Red
}

# 5. Qt Bluetooth
Write-Host "[5/5] Qt6Bluetooth..." -NoNewline
if (Test-Path "D:\Qt\6.11.0\msvc2022_64\lib\cmake\Qt6Bluetooth") {
    Write-Host " OK" -ForegroundColor Green
} else {
    Write-Host " MISSING (请用 Maintenance Tool 安装 Qt Connectivity)" -ForegroundColor Red
}

Write-Host "`n=== 检查完成 ===" -ForegroundColor Cyan
