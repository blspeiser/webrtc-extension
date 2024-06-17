#!/bin/bash
export NDK=~/android-ndk-r26d
cmake -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-32 -DANDROID_STL=none -S . -B ./bin
make -C bin