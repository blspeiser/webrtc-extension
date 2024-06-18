#ifndef AES256_FRAME_ENCRYPTOR_H
#define AES256_FRAME_ENCRYPTOR_H

#include <stddef.h>
#include <stdint.h>
#include "api/array_view.h"
#include "api/crypto/frame_encryptor_interface.h"
#include "api/media_types.h"
#include "rtc_base/ref_counted_object.h"

namespace webrtc {
  class Aes256FrameEncryptor : public rtc::RefCountedObject<FrameEncryptorInterface> {
    public:
      //Constructor. @key is expected to be exactly 32 bytes.
      explicit Aes256FrameEncryptor(uint8_t key[]); 
      
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

    private:
        uint8_t secretKey[];
  };
}

#endif 