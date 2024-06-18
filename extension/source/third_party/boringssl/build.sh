#!/bin/bash
python3 $PWD/src/util/generate_build_files.py android-cmake
cd src
export NDK=~/android-ndk-r26d
cmake -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -GNinja -B build -DCMAKE_BUILD_TYPE=Release
ninja -C build