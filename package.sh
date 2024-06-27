#!/bin/bash
export DIST=prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32
export ANDROID_STUDIO_PROJECT=android-example/app/src/main
cp libwebrtc/src/out/Release/libjingle_peerconnection_so.so $DIST
cp boringssl/src/build/libcrypto.so $DIST
cp extension/bin/libsrtp.so $DIST
cp jni/target/srtp-plugin-1.0.0.jar $DIST
# make sure not to copy the WebRTC .so, the Android project will get that elsewhere
cp $DIST/libcrypto.so $ANDROID_STUDIO_PROJECT/jniLibs/arm64-v8a
cp $DIST/libsrtp.so $ANDROID_STUDIO_PROJECT/jniLibs/arm64-v8a
cp $DIST/*.jar $ANDROID_STUDIO_PROJECT/jar

