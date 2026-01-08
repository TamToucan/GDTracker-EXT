#!/bin/bash
opt=""
target=""

[[ -z "$1" ]] && echo "$0 <core | godot | cute | all>" && exit 1

[[ $1 == "core" ]] &&
opt="-DBUILD_GODOT=OFF -DBUILD_CUTE=OFF" 

[[ $1 == "godot" ]] &&
opt="-DBUILD_GODOT=ON -DBUILD_CUTE=OFF" 

[[ $1 == "cute" ]] &&
opt="-DBUILD_GODOT=OFF -DBUILD_CUTE=ON" 

[[ $1 == "all" ]] &&
opt="-DBUILD_GODOT=ON -DBUILD_CUTE=ON" 

cmake --preset=mingw64 -S . -B build $opt
cmake --build build 

