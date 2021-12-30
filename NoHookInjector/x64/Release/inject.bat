@echo off
cd %~dp0
echo RayInjector - the syscalling and hook-defeating dll-injector
echo.
set /p process=Process name: 
set /p dllpath=Dll (full path/relative path): 
echo.
RayInj.exe "%process%" "%dllpath%"
echo.
pause