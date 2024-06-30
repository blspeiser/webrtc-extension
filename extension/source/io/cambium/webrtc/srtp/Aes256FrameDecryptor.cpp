#ifndef AES_256_FRAME_DECRYPTOR_CPP
#define AES_256_FRAME_DECRYPTOR_CPP

#include <stddef.h>
#include <stdint.h>
#include <jni.h>
#include <vector>
#include <android/log.h>
#include "io/cambium/webrtc/srtp/Aes256FrameDecryptor.h"
#include "aes256_frame_decryptor.h"

#ifdef __cplusplus
extern "C" {
#endif

const char* DECRYPTOR_CLASS = "io/cambium/webrtc/srtp/Aes256FrameDecryptor";
const char* DECRYPTOR_LOG = "io.cambium.webrtc.srtp.Aes256FrameDecryptor";

JNIEXPORT jlong JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameDecryptor_initialize(
        JNIEnv* env,
        jobject instance)
{
    const jclass clazz = env->FindClass(DECRYPTOR_CLASS);
    const jfieldID fieldKey = env->GetFieldID(clazz, "key", "[B");
    const jfieldID fieldIV  = env->GetFieldID(clazz, "iv",  "[B");
    const jbyteArray key    = reinterpret_cast<jbyteArray>(env->GetObjectField(instance, fieldKey));
    const jbyteArray iv     = reinterpret_cast<jbyteArray>(env->GetObjectField(instance, fieldIV));
    if(!key) {
        __android_log_print(ANDROID_LOG_ERROR, DECRYPTOR_LOG,
                            "Error retrieving key data - byte array could not be found");
        return 0;
    }
    if(!iv) {
        __android_log_print(ANDROID_LOG_ERROR, DECRYPTOR_LOG,
                            "Error retrieving initialization vector data - byte array could not be found");
        return 0;
    }
    const jint sizeKey      = env->GetArrayLength(key);
    const jint sizeIV       = env->GetArrayLength(iv);
    if(sizeKey != 32) {
        __android_log_print(ANDROID_LOG_ERROR, DECRYPTOR_LOG,
                            "Key is not 32 bytes: %d", sizeKey);
        return 0;
    }
    if(sizeIV != 16) {
        __android_log_print(ANDROID_LOG_ERROR, DECRYPTOR_LOG,
                            "Initialization Vector is not 16 bytes: %d", sizeIV);
        return 0;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG,
                            "Key size: %d,  Initialization Vector size: %d", sizeKey, sizeIV);
    std::vector<uint8_t> vKey(sizeKey);
    std::vector<uint8_t> vIV(sizeIV);
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, "Reading byte arrays from Java Aes256FrameDecryptor...");
    env->GetByteArrayRegion(key, 0, sizeKey, reinterpret_cast<jbyte*>(vKey.data()));
    env->GetByteArrayRegion(iv,  0, sizeIV,  reinterpret_cast<jbyte*>(vIV.data()));
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, "Creative native Aes256FrameDecryptor...");
    webrtc::Aes256FrameDecryptor* frameDecryptor = new webrtc::Aes256FrameDecryptor(vKey, vIV);
    if(frameDecryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, DECRYPTOR_LOG,
                            "Error while creating native Aes256FrameDecryptor: %s",
                            frameDecryptor->getErrorMessage());
        return 0;
    }
    //otherwise
    __android_log_print(ANDROID_LOG_DEBUG, DECRYPTOR_LOG, 
                        "Created native Aes256FrameDecryptor successfully, pointer: %p", frameDecryptor);
    jlong pointer = reinterpret_cast<jlong>(frameDecryptor);
    return pointer;
}

JNIEXPORT void JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameDecryptor_destroy(
        JNIEnv* env,
        jobject instance,
        jlong pointer)
{
    webrtc::Aes256FrameDecryptor* frameDecryptor = reinterpret_cast<webrtc::Aes256FrameDecryptor*>(pointer);
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, 
                        "Deleting native Aes256FrameDecryptor at pointer: %p", frameDecryptor);
    delete frameDecryptor;
    __android_log_print(ANDROID_LOG_DEBUG, DECRYPTOR_LOG, 
                        "Deleted native Aes256FrameDecryptor successfully.");
}

JNIEXPORT jbyteArray JNICALL Java_io_cambium_webrtc_srtp_Aes256FrameDecryptor_decrypt(
        JNIEnv* env,
        jobject instance,
        jlong pointer,
        jbyteArray bytes)
{
    if(!bytes) {
        __android_log_print(ANDROID_LOG_DEBUG, DECRYPTOR_LOG,
                            "Asked to decrypt null array; returning null");
        return NULL;                            
    }
    size_t size = env->GetArrayLength(bytes);
    if(!size) {
        __android_log_print(ANDROID_LOG_DEBUG, DECRYPTOR_LOG,
                            "Asked to decrypt empty array; returning new empty array");
        return env->NewByteArray(0);                            
    }
    if(!pointer) {
        __android_log_print(ANDROID_LOG_ERROR, DECRYPTOR_LOG,
                            "Passed a zero-value pointer, cannot retrieve native Aes256FrameDecryptor");
        return NULL;
    }
    webrtc::Aes256FrameDecryptor* frameDecryptor = reinterpret_cast<webrtc::Aes256FrameDecryptor*>(pointer);
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, 
                            "Retrieved native Aes256FrameDecryptor at pointer: %p, checking for errors...", 
                            frameDecryptor);
    if(frameDecryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, DECRYPTOR_LOG,
                            "Native Aes256FrameDecryptor had error, cannot decrypt: %s",
                            frameDecryptor->getErrorMessage());
        return NULL;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, 
                            "Calculating encrypted buffer size...");
    size_t plaintext_size = frameDecryptor->GetMaxPlaintextByteSize(cricket::MediaType::MEDIA_TYPE_DATA, size);
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, 
                            "Declaring buffers; content size: %lu, decrypted size: %lu", size, plaintext_size);
    std::vector<uint8_t> content(size);
    std::vector<uint8_t> buffer(plaintext_size);
    env->GetByteArrayRegion(bytes, 0, size, reinterpret_cast<jbyte*>(content.data()));
    rtc::ArrayView<uint8_t> data(content.data(), size);
    rtc::ArrayView<uint8_t> decrypted(buffer.data(), plaintext_size);
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, "Decrypting...");
    const std::vector<uint32_t> csrcs; //unused, but required for the interface
    webrtc::FrameDecryptorInterface::Result status = frameDecryptor->Decrypt(
        cricket::MediaType::MEDIA_TYPE_DATA, csrcs, nullptr, 
        data,
        decrypted);
    if(frameDecryptor->hadError()) {
        __android_log_print(ANDROID_LOG_ERROR, DECRYPTOR_LOG,
                            "Native Aes256FrameDecryptor had error while decrypting: %s",
                            frameDecryptor->getErrorMessage());
        return NULL;
    } 
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, 
                            "Data decrypted successfully (%lu bytes written); preparing byte array for return...",
                            status.bytes_written);
    jbyteArray result = env->NewByteArray(plaintext_size);
    env->SetByteArrayRegion(result, 0, plaintext_size, reinterpret_cast<jbyte*>(decrypted.data()));
    __android_log_print(ANDROID_LOG_VERBOSE, DECRYPTOR_LOG, "Decryption operation completed successfully.");
    return result;
}

#ifdef __cplusplus
}
#endif
#endif
