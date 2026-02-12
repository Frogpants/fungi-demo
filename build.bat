@echo off
REM ----------------------------------------
REM Windows-native build using MSVC (cl.exe)
REM ----------------------------------------

REM Path to your project folder
set SRC=main.cpp
set TARGET=main.exe

REM Compiler flags
set CFLAGS=/EHsc /std:c++17 /W4

REM Linker flags for Win32 + OpenGL
set LDFLAGS=opengl32.lib gdi32.lib user32.lib kernel32.lib glfw3.lib

REM Compile
cl %CFLAGS% %SRC% /Fe%TARGET% %LDFLAGS%

REM Run the program if compilation succeeds
if exist %TARGET% (
    echo Running %TARGET%...
    %TARGET%
)