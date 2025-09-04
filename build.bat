@echo off
REM build.bat - A script that handles "full" and "update" builds

IF "%1"=="msvc-r" GOTO init-release-msvc
IF "%1"=="msvc-d" GOTO init-debug-msvc
IF "%1"=="r" GOTO build-release
IF "%1"=="d" GOTO build-debug

ECHO Invalid option: %1
ECHO Usage: build.bat [ir|r|t]
GOTO end

REM Here are the build initialization options

:init-release-msvc
cmake -B bin/windows -G "Visual Studio 17 2022" -DCMAKE_C_COMPILER=MSVC -DCMAKE_CXX_COMPILER=MSVC -DCMAKE_BUILD_TYPE=Release -DJUCE_SOURCE_OVERRIDE_DIR="C:/Program Files/JUCE/8.0.8"
GOTO build-release

:init-debug-msvc
cmake -B bin/windows -G "Visual Studio 17 2022" -DCMAKE_C_COMPILER=MSVC -DCMAKE_CXX_COMPILER=MSVC -DCMAKE_BUILD_TYPE=Debug -DJUCE_SOURCE_OVERRIDE_DIR="C:/Program Files/JUCE/8.0.8"
GOTO build-debug


REM And of course the actual building steps

:build-release
cmake --build bin/windows --config release
GOTO end

:build-debug
cmake --build bin/windows --config debug
GOTO end

:end


