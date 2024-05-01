@REM @echo off
chcp 65001
@REM python mykotlinc.py test.kt -w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6

@REM pyinstaller --onefile mykotlinc.py && "./dist/mykotlinc" test.kt

python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache" && ^
python mykotlinc.py test.kt && ^
python mykotlinc.py test.kt --clear && ^
python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear
python mykotlinc.py --help
@REM "./mykotlinc" test.kt

@REM "./mykotlinc" test.kt -w --no-rebuild --configuration-cache --parallel --daemon --build-cache

@REM "./mykotlinc" test.kt --clear

@REM python D:\学校作业及其他\考研\mykotlinc\mykotlincoutside.py D:\学校作业及其他\考研\mykotlinc\test.kt clear -w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6

@REM python mykotlincoutside.py D:\学校作业及其他\考研\mykotlinc\test.kt

@REM python mykotlincoutside.py D:\学校作业及其他\考研\mykotlinc\test.kt clear