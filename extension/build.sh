#!/bin/bash
export NDK=~/android-ndk-r26d
cmake \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DCMAKE_SYSTEM_NAME=Android \
    -DCMAKE_SYSTEM_VERSION=32 \
    -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
    -DCMAKE_ANDROID_NDK=$NDK \
    -DANDROID_NDK=$NDK \
    -DANDROID_PLATFORM=android-32 \
    -DANDROID_STL=c++_shared \
    -GNinja \
    -S . -B ./bin 
ninja -C bin -v
readelf -h bin/libsrtp.so | grep 'Class\|File\|Machine'
cp bin/libsrtp.so ../prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32