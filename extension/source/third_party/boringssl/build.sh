#!/bin/bash
python3 $PWD/src/util/generate_build_files.py android-cmake
cd src
export NDK=~/android-ndk-r26d
cmake \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-32 \
    -GNinja \
    -B build \
    -DCMAKE_BUILD_TYPE=Release
ninja -C build
readelf -h build/libcrypto.a | grep 'Class\|File\|Machine'
cp build/libcrypto.a ../../../../../prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32