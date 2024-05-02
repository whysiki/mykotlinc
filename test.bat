@REM @echo off
chcp 65001

@REM python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache" && ^
@REM python mykotlinc.py test.kt && ^
@REM python mykotlinc.py test.kt --clear && ^
@REM python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear
@REM python mykotlinc.py --help

@REM python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear && ^
@REM python mykotlinc.py D:\KotlinCompiler\mykotlinc\test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear && ^
@REM mykotlinc.exe D:\KotlinCompiler\mykotlinc\test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear && ^
@REM mykotlinc.exe D:\KotlinCompiler\mykotlinc\test.kt --clear --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" && ^
@REM mykotlinc.exe test.kt --clear && ^
@REM mykotlinc.exe -h

@REM @echo off
@REM setlocal

@REM :: get begin time
@REM for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
@REM     set /a "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
@REM )

@REM mykotlinc.exe test.kt --clear

@REM :: get end time
@REM for /f "tokens=1-4 delims=:.," %%a in ("%time%") do (
@REM     set /a "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
@REM )

@REM :: calculate duration
@REM set /a duration=%end%-%start%

@REM echo Cost time：%duration%ms

@REM endlocal

@echo off
setlocal enabledelayedexpansion

set "commands[0]=python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
set "commands[1]=python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
set "commands[2]=mykotlinc.exe test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
set "commands[3]=mykotlinc.exe test.kt --clear --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6""
set "commands[4]=mykotlinc.exe test.kt --clear"

for /L %%i in (0,1,4) do (
    echo executing: !commands[%%i]!

    for /f "tokens=1-4 delims=:.," %%a in ("!time!") do (
        set /a "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
    )

    call !commands[%%i]!

    for /f "tokens=1-4 delims=:.," %%a in ("!time!") do (
        set /a "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
    )

    set /a duration=!end!-!start!
    echo command %%i cost in :  !duration!ms
)

endlocal
