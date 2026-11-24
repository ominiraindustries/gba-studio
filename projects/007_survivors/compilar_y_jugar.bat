@echo off
rem Compila y abre el juego en mGBA (tools\mgba).
setlocal
cd /d "%~dp0"
call "%~dp0compilar.bat" nopause || (pause & exit /b 1)
for %%I in ("%~dp0.") do set "NAME=%%~nxI"
set "MGBA=%~dp0..\..\tools\mgba\mGBA.exe"
if not exist "%MGBA%" (
  echo No encuentro mGBA en "%MGBA%". Ejecuta: python -I tools/scripts/install_toolchain.py --only mgba
  pause
  exit /b 1
)
start "" "%MGBA%" "%~dp0%NAME%.gba"
