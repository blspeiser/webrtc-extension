#!/bin/bash
export DIST=prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32
cp libwebrtc/src/out/Release/libjingle_peerconnection_so.so $DIST
cp boringssl/src/build/libcrypto.so $DIST
cp extension/bin/libsrtp.so $DIST
cp jni/target/srtp-plugin-1.0.0.jar $DIST
