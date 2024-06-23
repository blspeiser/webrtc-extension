#!/bin/bash
export NDK=~/android-ndk-r26d
cmake \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-32 \
    -DANDROID_STL=none \
    -GNinja \
    -S . -B ./bin
ninja -C bin
readelf -h bin/libsrtp.a | grep 'Class\|File\|Machine'
cp bin/libsrtp.a ../prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32