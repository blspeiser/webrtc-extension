#!/bin/bash
export BUILD_PLATFORM="ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32"
mkdir -p $PWD/prebuilt/$BUILD_PLATFORM
cp $PWD/extension/bin/libsrtp.a $PWD/prebuilt/$BUILD_PLATFORM
#cp $PWD/libwebrtc/src/out/Release/obj/libwebrtc.a $PWD/prebuilt/$BUILD_PLATFORM
#cp $PWD/libwebrtc/src/out/Release/libjingle_peerconnection_so.so $PWD/prebuilt/$BUILD_PLATFORM
tree $PWD/prebuilt/$BUILD_PLATFORM