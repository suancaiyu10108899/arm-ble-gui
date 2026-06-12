# 激活 MSVC 开发环境并切换到项目目录
# 用法: . .\scripts\dev-shell.ps1

Write-Host "=== 激活 MSVC 2022 环境 ===" -ForegroundColor Cyan

& 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1' -Arch amd64 -HostArch amd64

cd D:\Dev\arm-ble-gui

Write-Host "=== 当前目录: $(Get-Location) ===" -ForegroundColor Cyan
Write-Host "开发命令提示:" -ForegroundColor Yellow
Write-Host "  检查工具链: .\scripts\check-toolchain.ps1" -ForegroundColor White
Write-Host "  配置:       cmake --preset win-msvc-debug" -ForegroundColor White
Write-Host "  编译:       cmake --build --preset win-msvc-debug" -ForegroundColor White
Write-Host "  测试:       cd build/win-msvc-debug; ctest --output-on-failure" -ForegroundColor White
Write-Host "  运行:       .\build\win-msvc-debug\src\app\arm-ble-gui.exe" -ForegroundColor White
