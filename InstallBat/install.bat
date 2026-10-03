@echo off
setlocal
"%~dp0Engine\Extras\Redist\en-us\vc_redist.x64.exe" /install /quiet /norestart /log "%~dp0vcredist_x64.log"
set "InstallExitCode=%ERRORLEVEL%"
if "%InstallExitCode%"=="3010" exit /b 0
if "%InstallExitCode%"=="1638" exit /b 0
exit /b %InstallExitCode%
