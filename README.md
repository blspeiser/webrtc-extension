# AES256 Frame Encryption/Decryption for WebRTC  in Android

This project builds the Google WebRTC libaries for Android, and includes extensions to supply 256-bit AES CTR encryption to the frame data for end-to-end privacy. While most AES256 encryption uses CBC and not CTR, CBC requires padding and that can impact streaming video performance, so the no-padding CTR option is used instead. 

## How To Build It Yourself
Before you do so, consider just using the prebuilt components, downloadable from here:
https://github.com/blspeiser/webrtc-extension/tree/main/prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32

You can read about how I did this here: 
https://medium.com/@sbaruch/compiling-google-webrtc-extensions-for-android-1669db71e0e6

If you are feeling particularly adventurous, you can follow the steps below in order. 

**Prerequisites:** 
- Ubuntu
- Android NDK 26+
- Git
- Docker
- Python 3
- cmake
- ninja
- readelf
- For Java components, you will need Java and Maven. 
- Android Studio with an emulator are required for running the test cases / sample code, but is not strictly required for building the components. 

## webrtc-build
 Open a bash terminal in webrtc-build folder. 
 1. Run `sh build.sh` to build the Docker container that will be used to build libwebrtc.a 
 2. After completion, run `sh terminal.sh` to run the container and open a terminal.
 3. In the container terminal, you can run `ls` to determine that you have two script files, checkout.sh and compile.sh
 4. Run `sh checkout.sh` to download the code for libwebtrc.  It will take a *very* long time - possibly even overnight, maybe even longer.  Don't expect it to finish any time soon.  
 5. After checkout fully finishes, run `sh compile.sh` and the build should complete successfully. 
 6. You should now have libwebrtc.a available in libwebrtc/src/out/Release/obj/libwebrtc.a

## boringssl
 Open a bash terminal in boringssl folder. 
 1. Run `sh fetch.sh` to get the code. 
 2. Edit the build.sh script, line 5 to set the location of your NDK. The default is `~/android-ndk-r26d`, if your NDK installation is located somewhere else, you will need to adjust this line.
 3. Run `sh build.sh` to compile the code. 
 4. You should now have libcrypt.so available in boringssl/src/build/libcrypto.so

## extension
Assuming you have successfully completed the above components sections, open a bash terminal in extension folder. 

 1. Edit the build.sh script, line 2 to set the location of your NDK. The default is `~/android-ndk-r26d`, if your NDK installation is located somewhere else, you will need to adjust this line.
 2. Run `sh build.sh` to compile the code. 
 3. You should now have libsrtp.so available in extension/bin/libsrtp.so
 4. If you only need C++ implementations of a frame encryptor/decryptor, no further components are required. The relevant header files are located in extension/includes.

## jni
Most likely, you intend to use the Google WebRTC Java libraries, and so you will require the Java-based components. Open a terminal in the jni folder. 
1. Run `mvn clean package` to build the jar file. 
2. You should now have srtp-plugin-1.0.0.jar available in jni/target/srtp-plugin-1.0.0.jar
3. Inside the jar file itself, you will have a folder /arm64-v8a which will contain the libsrtp.so and libcrypto.so libraries. 

## Overview of the Android Example
The android-example folder contains a full Android Studio project showcasing how to use the components, as well as a test case that validates the frame encryptor and decryptor. 

I tested on an emulated Pixel 6 with Android 32. 

### Project Layout
Note the contents of android-example/app/src/main/jar and android-example/app/src/main/jniLibs. You will need these files in your workspace so that they can be referenced by your gradle build. 

### android-example/app/build.gradle.kts
- Note line 8 (android.compileSdk = 32) and 11 (android.defaultConfig.minSdk = 32)
- Lines 21-31 (android.defaultConfig.externalNativeBuild && android.defaultConfig.ndk) indicate this project has native components. 
- Lines 54-55 (dependencies) reference the dependencies that are physically present in your workspace. 

### android-example/app/src/main/java/io/cambium/webrtc/ExamplePeerConnectionSetup.kt
 - Lines 18-40 are not necessarily illustrative; they only exist in order to get a PeerConnection instance for the sake of showing how you would add a frame encryptor/decryptor. 
 - Once you have a PeerConnection instance, the real code starts on line 43:
 
```
  peerConnection?.senders?.filterNotNull()?.forEach {
    it.setFrameEncryptor( Aes256FrameEncryptor(key, ivHex) )
  }
  peerConnection?.receivers?.filterNotNull()?.forEach {
    it.setFrameDecryptor( Aes256FrameDecryptor(key, ivHex) )
  }
```

### android-example/app/src/androidTest/java/io/cambium/webrtc/srtp/*cryptorTest.kt
These two test cases can be executed to test the native frame encryption/decryption.  However, the encrypt and decrypt methods on these classes are not intended for general use; they exist only for the sake of testing. During real usage, the native implementation methods are called directly by the Google WebRTC code. 
