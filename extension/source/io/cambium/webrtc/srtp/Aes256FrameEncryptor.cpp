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

JNIEXPORT jlong JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_initialize(
        JNIEnv* env,
        jobject instance)
{
    const jclass clazz = env->FindClass(ENCRYPTOR_CLASS);
    const jfieldID fieldKey = env->GetFieldID(clazz, "key", "[B");
    const jfieldID fieldIV  = env->GetFieldID(clazz, "iv",  "[B");
    const jbyteArray key    = reinterpret_cast<jbyteArray>(env->GetObjectField(instance, fieldKey));
    const jbyteArray iv     = reinterpret_cast<jbyteArray>(env->GetObjectField(instance, fieldIV));
    if(!key) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Error retrieving key data - byte array could not be found");
        return 0;
    }
    if(!iv) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Error retrieving initialization vector data - byte array could not be found");
        return 0;
    }
    const jint sizeKey      = env->GetArrayLength(key);
    const jint sizeIV       = env->GetArrayLength(iv);
    if(sizeKey != 32) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Key is not 32 bytes: %d", sizeKey);
        return 0;
    }
    if(sizeIV != 16) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Initialization Vector is not 16 bytes: %d", sizeIV);
        return 0;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC,
                            "Key size: %d,  Initialization Vector size: %d", sizeKey, sizeIV);
    std::vector<uint8_t> vKey(sizeKey);
    std::vector<uint8_t> vIV(sizeIV);
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, "Reading byte arrays from Java Aes256FrameEncryptor...");
    env->GetByteArrayRegion(key, 0, sizeKey, reinterpret_cast<jbyte*>(vKey.data()));
    env->GetByteArrayRegion(iv,  0, sizeIV,  reinterpret_cast<jbyte*>(vIV.data()));
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, "Creative native Aes256FrameEncryptor...");
    webrtc::Aes256FrameEncryptor* frameEncryptor = new webrtc::Aes256FrameEncryptor(vKey, vIV);
    if(frameEncryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Error while creating native Aes256FrameEncryptor: %s",
                            frameEncryptor->getErrorMessage());
        return 0;
    }
    //otherwise
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TOPIC, 
                        "Created native Aes256FrameEncryptor successfully, pointer: %p", frameEncryptor);
    jlong pointer = reinterpret_cast<jlong>(&frameEncryptor);
    return pointer;
}

JNIEXPORT void JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_destroy(
        JNIEnv* env,
        jobject instance,
        jlong pointer)
{
    webrtc::Aes256FrameEncryptor* frameEncryptor = reinterpret_cast<webrtc::Aes256FrameEncryptor*>(pointer);
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, 
                        "Deleting native Aes256FrameEncryptor at pointer: %p", frameEncryptor);
    delete frameEncryptor;
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TOPIC, 
                        "Deleted native Aes256FrameEncryptor successfully.");
}

JNIEXPORT jbyteArray JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameEncryptor_encrypt(
        JNIEnv* env,
        jobject instance,
        jlong pointer,
        jbyteArray bytes)
{
    if(!bytes) {
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TOPIC,
                            "Asked to encrypt null array; returning null");
        return NULL;                            
    }
    size_t size = env->GetArrayLength(bytes);
    if(!size) {
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TOPIC,
                            "Asked to encrypt empty array; returning new empty array");
        return env->NewByteArray(0);                            
    }
    if(!pointer) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Passed a zero-value pointer, cannot retrieve native Aes256FrameEncryptor");
        return NULL;
    }
    webrtc::Aes256FrameEncryptor* frameEncryptor = reinterpret_cast<webrtc::Aes256FrameEncryptor*>(pointer);
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, 
                            "Retrieved native Aes256FrameEncryptor at pointer: %p, checking for errors...", pointer);
    if(frameEncryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Native Aes256FrameEncryptor had error, cannot encrypt: %s",
                            frameEncryptor->getErrorMessage());
        return NULL;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, 
                            "Calculating encrypted buffer size...");
    size_t padded_size = frameEncryptor->GetMaxCiphertextByteSize(cricket::MediaType::MEDIA_TYPE_DATA, size);
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, 
                            "Declaring buffers; content size: %lu, encrypted size: %lu", size, padded_size);
    std::vector<uint8_t> content(size);
    std::vector<uint8_t> buffer(padded_size);
    rtc::ArrayView<uint8_t> data(content.data(), size);
    rtc::ArrayView<uint8_t> encrypted(buffer.data(), padded_size);
    size_t bytes_written = 0;
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, "Encrypting...");
    frameEncryptor->Encrypt(cricket::MediaType::MEDIA_TYPE_DATA, 0, nullptr, data, encrypted, &bytes_written);
    if(frameEncryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TOPIC,
                            "Native Aes256FrameEncryptor had error while encrypting: %s",
                            frameEncryptor->getErrorMessage());
    }
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, 
                            "Data encrypted successfully (%lu bytes written); preparing byte array for return...",
                            bytes_written);
    jbyteArray result = env->NewByteArray(padded_size);
    env->SetByteArrayRegion(result, 0, padded_size, reinterpret_cast<jbyte*>(encrypted.data()));
    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TOPIC, "Encryption operation completed successfully.");
    return result;
}

#ifdef __cplusplus
}
#endif
#endif
