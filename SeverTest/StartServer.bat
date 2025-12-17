@echo off
REM Dedicated Server启动脚本
REM 使用方法: 双击此文件或从命令行运行

echo Starting Dedicated Server...
echo.

REM 设置服务器端口（默认7777）
set SERVER_PORT=7777

REM 启动服务器
REM 注意: 需要先编译SeverTestServer target
SeverTestServer.exe -log -server -game -port=%SERVER_PORT% -unattended

pause

