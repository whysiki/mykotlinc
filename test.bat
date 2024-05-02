@REM @echo off
chcp 65001

python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache" && ^
python mykotlinc.py test.kt && ^
python mykotlinc.py test.kt --clear && ^
python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear && ^
python mykotlinc.py --help

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

@REM @echo off
@REM setlocal enabledelayedexpansion

@REM set "commands[0]=python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
@REM set "commands[1]=python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
@REM set "commands[2]=mykotlinc.exe test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
@REM set "commands[3]=mykotlinc.exe test.kt --clear --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6""
@REM set "commands[4]=mykotlinc.exe test.kt --clear"
@REM set "commands[5]=mykotlinc.exe test2.kt --clear"
@REM set "commands[6]=python mykotlinc.py test2.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
@REM set "commands[7]=python mykotlinc.py test2.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
@REM set "commands[8]=mykotlinc.exe test2.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear"
@REM set "commands[9]=mykotlinc.exe test2.kt --clear --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6""


@REM for /L %%i in (0,1,9) do (
@REM     echo executing: !commands[%%i]!

@REM     for /f "tokens=1-4 delims=:.," %%a in ("!time!") do (
@REM         set /a "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
@REM     )

@REM     call !commands[%%i]!

@REM     for /f "tokens=1-4 delims=:.," %%a in ("!time!") do (
@REM         set /a "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
@REM     )

@REM     set /a duration=!end!-!start!
@REM     echo command %%i cost in :  !duration!ms
@REM )

@REM endlocal


