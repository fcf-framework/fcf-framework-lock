@echo off
set "DIRVAR=%1%"
set "PATH=%DIRVAR%;%PATH%;%DIRVAR%"
echo %DIRVAR%
node -v
call npm install
if %errorlevel% NEQ 0 (
  echo Install error! %errorlevel%
  exit /b 1
)
echo "Start tests"
call node ./tests/tests.js
if %errorlevel% NEQ 0 (
  echo Test error!
  exit /b 1
)
echo "Tests completed"
