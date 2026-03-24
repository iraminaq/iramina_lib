@echo off
setlocal

echo ==========================================
echo     GitHub SSH Auto Push Script
echo ==========================================
echo.

:: コマンドライン引数があればそれをメッセージにする。なければデフォルト。
set MSG=%*
if "%MSG%"=="" set MSG=Add MoRollback, DynamicMo and documentations

echo [1/3] Staging all changes...
git add .
echo.

echo [2/3] Committing with message: "%MSG%"
git commit -m "%MSG%"
echo.

echo [3/3] Pushing to origin main via SSH...
git push origin main
echo.

echo ==========================================
echo Done! 
pause
