#ifndef AES256_FRAME_ENCRYPTOR_CPP
#define AES256_FRAME_ENCRYPTOR_CPP

#include "aes256_frame_encryptor.h"
#include "rtc_base/checks.h"

#include <string>
#include <vector>

#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/err.h>

#define AES_KEY_LENGTH_BYTES 32
#define AES_KEY_LENGTH_BITS 256

namespace webrtc {

  Aes256FrameEncryptor::Aes256FrameEncryptor(
    std::vector<uint8_t> key,
    std::vector<uint8_t> iv) 
  : _key(key), _iv(iv), _error("") 
  {
    if(key.size() != AES_KEY_LENGTH_BYTES) {
      _error = "Invalid key size: must be exactly 32 bytes";
      return;
    }
    if(iv.size() != AES_BLOCK_SIZE) {
      _error = "Invalid initialization vector size: must be exactly 16 bytes";
      return;
    }
    //otherwise
    int status = AES_set_encrypt_key(key.data(), AES_KEY_LENGTH_BITS, &_aes_key);
    if(status != 0) {
      _error = "Failed to set encryption key";
      return;
    }
  }

  Aes256FrameEncryptor::~Aes256FrameEncryptor() {
    //nothing to do (_aes_key is not a pointer type, so don't need to delete it; and neither are the stl fields)
  }

  int Aes256FrameEncryptor::Encrypt(
    cricket::MediaType media_type,
    uint32_t ssrc,
    rtc::ArrayView<const uint8_t> additional_data,
    rtc::ArrayView<const uint8_t> frame,
    rtc::ArrayView<uint8_t> encrypted_frame,
    size_t* bytes_written) 
  {
    //First, calculate encrypted size:
    const size_t padded_size = GetMaxCiphertextByteSize(cricket::MediaType::MEDIA_TYPE_DATA, frame.size());
    //Now encrypt:
    AES_cbc_encrypt(frame.data(), encrypted_frame.data(), frame.size(), &_aes_key, _iv.data(), AES_ENCRYPT);
    if(bytes_written) *bytes_written = padded_size;
    return 0;
  }

  size_t Aes256FrameEncryptor::GetMaxCiphertextByteSize(
    cricket::MediaType media_type,
    size_t frame_size) 
  {
    // AES256/CBC uses 128-bit blocks, so the size will always be a multiple of 16. 
    // Note that we are NOT using PKCS5 padding, so if it is already a multiple of 16, no need to do anything.
    // If the value is already a multiple of 16, it is supposed to be bumped up to the next multiple. 
    return (frame_size % 16 == 0)
              ? frame_size
              : (frame_size + 16) & ~15;  //fast way to do x + (x%16)
  }

  bool Aes256FrameEncryptor::hadError() { return _error.length() > 0; }
  const char* Aes256FrameEncryptor::getErrorMessage() { return _error.c_str(); }
}
#endif