@echo off
setlocal EnableDelayedExpansion
for /d %%i in (C:\Applications\nodejs\node-*) do (
  call .\tests\run-test.bat %%i
  if !errorlevel! NEQ 0 (
    echo "Exit with error!!!"
    exit /b 1
  )
)