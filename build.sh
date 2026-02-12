#!/bin/bash

SRC=$(find . -name "*.cpp")

x86_64-w64-mingw32-g++ \
-std=c++17 -Wall -Wextra -O2 \
$SRC \
-o main.exe \
-lglfw3 -lopengl32 -lgdi32 -lwinmm

if [ $? -eq 0 ]; then
    echo "Windows build successful: main.exe created."
else
    echo "Build failed."
fi
