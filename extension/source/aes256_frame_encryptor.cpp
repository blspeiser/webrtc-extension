#ifndef AES256_FRAME_ENCRYPTOR_CPP
#define AES256_FRAME_ENCRYPTOR_CPP

#include "aes256_frame_encryptor.h"
#include "rtc_base/checks.h"

#include <string>
#include <vector>

#include <openssl/aes.h>
#include <openssl/evp.h>
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
    ctx = EVP_CIPHER_CTX_new();
    if(ctx == NULL) {
      _error = "Failed to initialize cypher context";
      return;
    }
    int status = EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), nullptr, key.data(), iv.data());
    if(status != 1) {
      _error = "Failed to initialize cipher with key and initialization vector";
      EVP_CIPHER_CTX_free(ctx);
    }
  }

  Aes256FrameEncryptor::~Aes256FrameEncryptor() {
    EVP_CIPHER_CTX_free(ctx);
  }

  int Aes256FrameEncryptor::Encrypt(
    cricket::MediaType media_type,
    uint32_t ssrc,
    rtc::ArrayView<const uint8_t> additional_data,
    rtc::ArrayView<const uint8_t> frame,
    rtc::ArrayView<uint8_t> encrypted_frame,
    size_t* bytes_written) 
  {
    //Now encrypt:
    int outputted = 0;
    int status = EVP_EncryptUpdate(ctx, 
      encrypted_frame.data(), &outputted, 
      frame.data(), frame.size());
    if(status != 1) {
      _error = "Failed to encrypt data!";
    }
    if(bytes_written) *bytes_written = outputted;
    return 0;
  }

  size_t Aes256FrameEncryptor::GetMaxCiphertextByteSize(
    cricket::MediaType media_type,
    size_t frame_size) 
  {
    // AES/CTR256/NoPadding returns the exact same size of the original data, always
    return frame_size;
  }

  bool Aes256FrameEncryptor::hadError() { return _error.length() > 0; }
  const char* Aes256FrameEncryptor::getErrorMessage() { return _error.c_str(); }
}
#endif