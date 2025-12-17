@echo off
REM 客户端启动脚本
REM 使用方法: 双击此文件或从命令行运行

echo Starting Client...
echo.

REM 设置服务器地址（默认本地）
set SERVER_IP=127.0.0.1
set SERVER_PORT=7777

REM 启动客户端并连接到服务器
REM 注意: 需要先编译SeverTest target
SeverTest.exe -game %SERVER_IP%:%SERVER_PORT%

pause

