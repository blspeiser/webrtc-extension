#ifndef AES256_FRAME_ENCRYPTOR_CPP
#define AES256_FRAME_ENCRYPTOR_CPP

#include "aes256_frame_encryptor.h"
#include "rtc_base/checks.h"

#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/err.h>


namespace webrtc {

  Aes256FrameEncryptor::Aes256FrameEncryptor(uint8_t key[]) {
    //for now, do nothing
  }

  Aes256FrameEncryptor::~Aes256FrameEncryptor() {
    //for now, do nothing
  }

  int Aes256FrameEncryptor::Encrypt(
    cricket::MediaType media_type,
    uint32_t ssrc,
    rtc::ArrayView<const uint8_t> additional_data,
    rtc::ArrayView<const uint8_t> frame,
    rtc::ArrayView<uint8_t> encrypted_frame,
    size_t* bytes_written) 
  {
    //for now, do nothing
    return 0;
  }

  size_t Aes256FrameEncryptor::GetMaxCiphertextByteSize(
    cricket::MediaType media_type,
    size_t frame_size) 
  {
    // AES256/CBC/PKCS#5 uses 128-bit blocks, so the size will always be a multiple of 16. 
    return (frame_size + 16) & ~15; //fastest way to say size + (size % 16)
  }

}
#endif