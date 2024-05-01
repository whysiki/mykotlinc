@REM @echo off
chcp 65001

@REM python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache" && ^
@REM python mykotlinc.py test.kt && ^
@REM python mykotlinc.py test.kt --clear && ^
@REM python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear
@REM python mykotlinc.py --help

python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear

mykotlinc.exe D:\KotlinCompiler\mykotlinc\test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear

mykotlinc.exe D:\KotlinCompiler\mykotlinc\test.kt --clear --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" 

mykotlinc -h