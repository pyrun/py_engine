@echo off
echo             Engine-Skript
echo             =============
echo.
echo   [1] Windows32 Debug
echo   [2] Windows32 Release
echo   [3] Linux Debug
echo   [4] Linux Release
echo   [5] Alle Obj-dateien löschen
echo   [6] Exit
echo.

set asw=0
set /p asw="Bitte Auswahl eingeben: "

Set /p path= < "game"

if %asw%==1 goto WIND
if %asw%==2 goto WINR
if %asw%==3 goto LINUXD
if %asw%==4 goto LINUXR
if %asw%==5 goto OBJDELETING
goto END

:WIND
del %path% /s /q
xcopy "bin/win/debug" %path% /I
xcopy "res" "game" /E /C /Q /I /Y
goto END

:WINR
del %path% /s /q
xcopy "bin/win/release" %path% /I
xcopy "res" "game" /E /C /Q /I /Y
goto END

:END