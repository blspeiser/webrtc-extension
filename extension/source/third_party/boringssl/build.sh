#!/bin/bash
python3 $PWD/src/util/generate_build_files.py android-cmake
export PROJECT_ROOT=$(dirname $(readlink -e $PWD/../../..))
cd src
export NDK=~/android-ndk-r26d
cmake \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-32 \
    -GNinja \
    -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=1 \
    -DCMAKE_CXX_COMPILER=$PROJECT_ROOT/libwebrtc/src/third_party/llvm-build/Release+Asserts/bin/clang
ninja -C build
readelf -h build/libcrypto.so | grep 'Class\|File\|Machine'
cp build/libcrypto.so ../../../../../prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32
#readelf -h build/libcrypto.a | grep 'Class\|File\|Machine'
#cp build/libcrypto.a ../../../../../prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32