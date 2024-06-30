#ifndef AES256_FRAME_DECRYPTOR_H
#define AES256_FRAME_DECRYPTOR_H

#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "api/array_view.h"
#include "api/crypto/frame_decryptor_interface.h"
#include "api/media_types.h"
#include "rtc_base/ref_counted_object.h"

#include <openssl/evp.h>

namespace webrtc {
  class Aes256FrameDecryptor : public rtc::RefCountedObject<FrameDecryptorInterface> {
    public:
      //Constructor. @key is expected to be exactly 32 bytes, @iv is expected to be 16 bytesecl.
      explicit Aes256FrameDecryptor(
        std::vector<uint8_t> key,
        std::vector<uint8_t> iv); 
      
      //Destructor
      ~Aes256FrameDecryptor();

      //Decrypt the frame. 
      FrameDecryptorInterface::Result Decrypt(
                        cricket::MediaType media_type,
                        const std::vector<uint32_t>& csrcs,
                        rtc::ArrayView<const uint8_t> additional_data,
                        rtc::ArrayView<const uint8_t> encrypted_frame,
                        rtc::ArrayView<uint8_t> frame) override;

      //Returns the maximum potential size of the plain text, including the AES block padding.
      size_t GetMaxPlaintextByteSize(cricket::MediaType media_type,
                                     size_t encrypted_frame_size) override;

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