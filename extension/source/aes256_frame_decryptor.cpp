#ifndef AES256_FRAME_DECRYPTOR_CPP
#define AES256_FRAME_DECRYPTOR_CPP

#include "aes256.h"
#include "aes256_frame_decryptor.h"
#include "rtc_base/checks.h"

#include <string>
#include <vector>

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <openssl/err.h>

namespace webrtc {

  Aes256FrameDecryptor::Aes256FrameDecryptor(
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
    int status = EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), nullptr, key.data(), iv.data());
    if(status != 1) {
      _error = "Failed to initialize cipher with key and initialization vector";
      EVP_CIPHER_CTX_free(ctx);
    }
  }

  Aes256FrameDecryptor::~Aes256FrameDecryptor() {
    EVP_CIPHER_CTX_free(ctx);
  }

  FrameDecryptorInterface::Result Aes256FrameDecryptor::Decrypt(
    cricket::MediaType media_type,
    const std::vector<uint32_t>& csrcs,
    rtc::ArrayView<const uint8_t> additional_data,
    rtc::ArrayView<const uint8_t> encrypted_frame,
    rtc::ArrayView<uint8_t> frame)
  {
    //Now encrypt:
    int outputted = 0;
    int status = EVP_DecryptUpdate(ctx, 
      frame.data(), &outputted, 
      encrypted_frame.data(), encrypted_frame.size());
    if(status != 1) {
      _error = "Failed to encrypt data!";
    }
    return this->hadError() 
        ? FrameDecryptorInterface::Result(FrameDecryptorInterface::Status::kFailedToDecrypt, outputted)
        : FrameDecryptorInterface::Result(FrameDecryptorInterface::Status::kOk, outputted);
  }

  size_t Aes256FrameDecryptor::GetMaxPlaintextByteSize(
    cricket::MediaType media_type,
    size_t encrypted_frame_size) 
  {
    // AES/CTR256/NoPadding returns the exact same size of the original data, always
    return encrypted_frame_size;
  }

  bool Aes256FrameDecryptor::hadError() { return _error.length() > 0; }
  const char* Aes256FrameDecryptor::getErrorMessage() { return _error.c_str(); }
}
#endif