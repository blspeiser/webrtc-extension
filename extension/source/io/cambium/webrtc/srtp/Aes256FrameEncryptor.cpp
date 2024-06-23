#ifndef AES_256_FRAME_ENCRYPTOR_CPP
#define AES_256_FRAME_ENCRYPTOR_CPP

#include <stddef.h>
#include <stdint.h>
#include <jni.h>
#include <vector>
#include <android/log.h>
#include "io/cambium/webrtc/srtp/Aes256FrameEncryptor.h"
#include "aes256_frame_encryptor.h"

#ifdef __cplusplus
extern "C" {
#endif

const char* ENCRYPTOR_CLASS = "io/cambium/webrtc/srtp/Aes256FrameEncryptor";
const char* LOG_TOPIC = "io.cambium.webrtc.srtp.Aes256FrameEncryptor";

JNIEXPORT jlong JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_getNativeFrameEncryptor(
    JNIEnv* env,
    jobject instance)
{
    const jclass clazz = env->FindClass(ENCRYPTOR_CLASS);
    const jfieldID fieldKey = env->GetFieldID(clazz, "key", "[B");
    const jfieldID fieldIV  = env->GetFieldID(clazz, "iv",  "[B");
    const jbyteArray key    = reinterpret_cast<jbyteArray>(env->GetObjectField(instance, fieldKey));
    const jbyteArray iv     = reinterpret_cast<jbyteArray>(env->GetObjectField(instance, fieldIV));
    const jint sizeKey      = env->GetArrayLength(key);
    const jint sizeIV       = env->GetArrayLength(iv);
    std::vector<uint8_t> vKey(sizeKey);
    std::vector<uint8_t> vIV(sizeIV);
    env->GetByteArrayRegion(key, 0, sizeKey, reinterpret_cast<jbyte*>(vKey.data()));
    env->GetByteArrayRegion(iv,  0, sizeIV,  reinterpret_cast<jbyte*>(vIV.data()));

    webrtc::Aes256FrameEncryptor* frameEncryptor = new webrtc::Aes256FrameEncryptor(vKey, vIV);
    if(frameEncryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Error while creating frame encryptor: %s",
                            frameEncryptor->getErrorMessage());
        return 0;
    }
    //otherwise
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TOPIC, "Created native Aes256FrameEncryptor successfully.");

    jlong pointer = reinterpret_cast<jlong>(&frameEncryptor);
    const jfieldID fieldPointer = env->GetFieldID(clazz, "pointer", "J");
    env->SetLongField(instance, fieldPointer, pointer);
    return pointer;
}

JNIEXPORT void JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_cleanupNativeFrameEncryptor(
        JNIEnv* env,
        jobject instance)
{
    const jclass clazz = env->FindClass(ENCRYPTOR_CLASS);
    const jfieldID fieldPointer = env->GetFieldID(clazz, "pointer", "J");
    const jlong pointer = env->GetLongField(instance, fieldPointer);
    webrtc::Aes256FrameEncryptor* frameEncryptor = reinterpret_cast<webrtc::Aes256FrameEncryptor*>(pointer);
    delete frameEncryptor;
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TOPIC, "Deleted native Aes256FrameEncryptor successfully.");
}

JNIEXPORT jbyteArray JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_encrypt(
        JNIEnv* env,
        jobject instance,
        jbyteArray bytes)
{
    const jclass clazz = env->FindClass(ENCRYPTOR_CLASS);
    const jfieldID fieldPointer = env->GetFieldID(clazz, "pointer", "J");
    const jlong pointer = env->GetLongField(instance, fieldPointer);
    webrtc::Aes256FrameEncryptor* frameEncryptor = reinterpret_cast<webrtc::Aes256FrameEncryptor*>(pointer);
    if(frameEncryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Native Aes256FrameEncryptor had error, cannot encrypt: %s",
                            frameEncryptor->getErrorMessage());
        return NULL;
    }
    size_t size = env->GetArrayLength(bytes);
    size_t padded_size = frameEncryptor->GetMaxCiphertextByteSize(cricket::MediaType::MEDIA_TYPE_DATA, size);
    std::vector<uint8_t> content(size);
    std::vector<uint8_t> buffer(padded_size);
    rtc::ArrayView<uint8_t> data(content.data(), size);
    rtc::ArrayView<uint8_t> encrypted(buffer.data(), padded_size);
    size_t bytes_written = 0;
    frameEncryptor->Encrypt(cricket::MediaType::MEDIA_TYPE_DATA, 0, nullptr, data, encrypted, &bytes_written);
    if(frameEncryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Native Aes256FrameEncryptor had error while encrypting: %s",
                            frameEncryptor->getErrorMessage());
    }
    jbyteArray result = env->NewByteArray(padded_size);
    env->SetByteArrayRegion(result, 0, padded_size, reinterpret_cast<jbyte*>(encrypted.data()));
    return result;
}

#ifdef __cplusplus
}
#endif
#endif
