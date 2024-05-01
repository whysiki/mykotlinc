@REM @echo off
chcp 65001

python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache" && ^
python mykotlinc.py test.kt && ^
python mykotlinc.py test.kt --clear && ^
python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" --clear
python mykotlinc.py --help