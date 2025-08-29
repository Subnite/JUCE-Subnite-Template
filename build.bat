@echo off
REM build.bat - A script that handles "full" and "update" builds

IF "%1"=="msvc-r" GOTO init-release-msvc
IF "%1"=="msvc-d" GOTO init-debug-msvc
IF "%1"=="clang-r" GOTO init-release-clang
IF "%1"=="clang-d" GOTO init-debug-clang
IF "%1"=="r" GOTO build-release
IF "%1"=="d" GOTO build-debug

ECHO Invalid option: %1
ECHO Usage: build.bat [ir|r|t]
GOTO end

REM Here are the build initialization options

:init-release-msvc
cmake -B bin -G "Visual Studio 17 2022" -DCMAKE_C_COMPILER=MSVC -DCMAKE_CXX_COMPILER=MSVC -DCMAKE_BUILD_TYPE=Release
GOTO build-release

:init-release-clang
cmake -B bin -G "Ninja" -DCMAKE_C_COMPILER=Clang -DCMAKE_CXX_COMPILER=Clang++ -DCMAKE_BUILD_TYPE=Release
GOTO build-release

:init-debug-msvc
cmake -B bin -G "Visual Studio 17 2022" -DCMAKE_C_COMPILER=MSVC -DCMAKE_CXX_COMPILER=MSVC -DCMAKE_BUILD_TYPE=Debug
GOTO build-debug

:init-debug-clang
cmake -B bin -G "Ninja" -DCMAKE_C_COMPILER=Clang -DCMAKE_CXX_COMPILER=Clang++ -DCMAKE_BUILD_TYPE=Debug
GOTO build-debug



REM And of course the actual building steps

:build-release
cmake --build bin --config release
GOTO end

:build-debug
cmake --build bin --config debug
GOTO end

:end


