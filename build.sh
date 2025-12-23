#!/bin/bash
cmake --preset=mingw64 -S . -B build -DBUILD_GODOT=ON -DBUILD_CUTE=ON
cmake --build build

