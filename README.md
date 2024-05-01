# Kotlin Single-File Command-Line Compilation Tool

This simple Kotlin single-file command-line compilation tool was developed for learning and practicing Kotlin.

It uses Gradle as a wrapper to enhance compilation speed, reducing the compile time to approximately 1 second.

**Note:** The command-line compilation tool provided by the official Kotlin team is relatively slow (perhaps it's just me, but even after tweaking numerous parameters, the speed remains slow??), and it takes more than 6 seconds even for compiling and running very small code snippets.
However, I do not wish to create a separate Gradle project for each small code snippet for compilation and execution.

**You can't execute an infinite loop like a real compiler.You have to finish the output, or you'll wait forever.**
**You're essentially waiting for gradle to execute and then logging it to the console.**

The Gradle configuration files are located in the template folder; modify these parameters to adjust the settings for command-line compilation.

- The program works by first generating the simplest Gradle project from a file in the template.
- When a Kotlin code file is executed, the Kotlin code replaces the contents of Helloworld.kt, and the code is run using `gradle run`.
- When the Kotlin code file is modified, the code in Helloworld.kt is updated and rerun; otherwise, it is not updated.

```
# A minimal gradle project directory structure.
|+-- src/
    |    |+-- main/
    |    |    |+-- kotlin/
    |    |    |    |-- Helloworld.kt
    |    |    |+-- resources/
    |-- build.gradle
    |-- gradlew
    |-- gradlew.bat
    |-- run.bat
    |-- settings.gradle
```

Before use, install Gradle and JDK, and add them to the system PATH environment variable.
The initial run of Gradle may require some time to download dependencies.

```bash
C:\Users\Administrator>where java
D:\KotlinCompiler\jdk-21_windows-x64_bin\jdk-21.0.2\bin\java.exe

C:\Users\Administrator>where gradle
D:\KotlinCompiler\gradle-8.6\bin\gradle
```

## Usage Instructions

To view the `test.bat` file:

```bash
python mykotlinc.py test.kt  # The first argument must be the path to the code file
python mykotlinc.py test.kt --clear  # Clear the contents inside ./build/build
```

Additional Gradle command-line arguments can be appended:

```bash
python mykotlinc.py test.kt --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache"
```

```bash
D:\KotlinCompiler\mykotlinc>python mykotlinc.py --help
usage: mykotlinc.py [-h] [--clear] [--params PARAMS] file_path

Compile Kotlin files.

positional arguments:
  file_path        Path to the Kotlin file.

options:
  -h, --help       show this help message and exit
  --clear          Clear build before compiling.
  --params PARAMS  Additional other gradle parameters for the compiler.
```

## run test.kt

```powershell
PS D:\KotlinCompiler\mykotlinc> cd "d:\KotlinCompiler\mykotlinc\" ; if ($?) { python -u mykotlinc.py test.kt --clear --params "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache" }
After adding 20 elements:
Element 1
Element 2
Element 3
Element 4
Element 5
Element 6
Element 7
Element 8
Element 9
Element 10
Element 11
Element 12
Element 13
Element 14
Element 15
Element 16
Element 17
Element 18
Element 19
Element 20

Removing elements:
Deleted: Element 20
Deleted: Element 19
Deleted: Element 18
Deleted: Element 17
Deleted: Element 16
Deleted: Element 15
Deleted: Element 14
Deleted: Element 13
Deleted: Element 12
Deleted: Element 11
Deleted: Element 10
Deleted: Element 9
Deleted: Element 8
Deleted: Element 7
Deleted: Element 6
Deleted: Element 5
Deleted: Element 4
Deleted: Element 3
Deleted: Element 2
Deleted: Element 1
Deleted: null

After removing 20 elements:
Configuration cache entry stored.

Cleared build in D:\KotlinCompiler\mykotlinc\build\build
Returncode:  0
```
