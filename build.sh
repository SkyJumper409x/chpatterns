#!/bin/bash

if [ -d "./build" ] && [ "$1" = "--clean" ]; then
    rm -r ./build
fi
if [ ! -d "./build" ]; then
    mkdir build
fi
cd build
if [ ! -f "cmake_install.cmake" ]; then
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
fi
cmake --build .
cd ..
if [ ! -e "./compile_commands.json" ]; then
    ln -s build/compile_commands.json .
fi
