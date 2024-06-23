#ifndef AES_256_FRAME_ENCRYPTOR_H
#define AES_256_FRAME_ENCRYPTOR_H

#include <stdint.h>
#include <jni.h>
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_getNativeFrameEncryptor(JNIEnv*, jobject);

JNIEXPORT void JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_cleanupNativeFrameEncryptor(JNIEnv*, jobject);

JNIEXPORT jbyteArray JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_encrypt(JNIEnv*, jobject, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif