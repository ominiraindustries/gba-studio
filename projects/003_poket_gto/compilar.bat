@echo off
rem Compila este juego con doble clic. Usa Git Bash y el toolchain de tools/README.md.
setlocal
cd /d "%~dp0"
set "GITBASH=%ProgramFiles%\Git\bin\bash.exe"
if not exist "%GITBASH%" (
  echo No encuentro Git Bash en "%GITBASH%". Instala Git for Windows.
  pause
  exit /b 1
)
"%GITBASH%" -c "source ../../tools/env.sh && make -j%NUMBER_OF_PROCESSORS%"
if errorlevel 1 (
  echo.
  echo *** La compilacion ha fallado. Revisa los mensajes de arriba. ***
  echo Si mGBA tiene el juego abierto, cierralo: bloquea el .gba y no se puede reescribir.
  if "%~1"=="" pause
  exit /b 1
)
echo Compilacion correcta.
if "%~1"=="" pause
exit /b 0
