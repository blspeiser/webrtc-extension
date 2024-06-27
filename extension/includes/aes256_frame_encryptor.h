#ifndef AES256_FRAME_ENCRYPTOR_H
#define AES256_FRAME_ENCRYPTOR_H

#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "api/array_view.h"
#include "api/crypto/frame_encryptor_interface.h"
#include "api/media_types.h"
#include "rtc_base/ref_counted_object.h"

#include <openssl/evp.h>

namespace webrtc {
  class Aes256FrameEncryptor : public rtc::RefCountedObject<FrameEncryptorInterface> {
    public:
      //Constructor. @key is expected to be exactly 32 bytes, @iv is expected to be 16 bytes.
      explicit Aes256FrameEncryptor(
        std::vector<uint8_t> key,
        std::vector<uint8_t> iv); 
      
      //Destructor
      ~Aes256FrameEncryptor();

      //Encrypt the frame. 
      int Encrypt(
          cricket::MediaType media_type,
          uint32_t ssrc,
          rtc::ArrayView<const uint8_t> additional_data,
          rtc::ArrayView<const uint8_t> frame,
          rtc::ArrayView<uint8_t> encrypted_frame,
          size_t* bytes_written) override;

      //Returns the maximum potential size of the cipher text, including the AES block padding.
      size_t GetMaxCiphertextByteSize(
          cricket::MediaType media_type,
          size_t frame_size) override;

      bool hadError();
      const char* getErrorMessage();

    private:
        std::vector<uint8_t> _key;
        std::vector<uint8_t> _iv;
        std::string _error;
        EVP_CIPHER_CTX *ctx = NULL;
  };
}

#endif 