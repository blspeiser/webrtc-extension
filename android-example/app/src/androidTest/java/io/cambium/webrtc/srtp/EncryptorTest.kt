package io.cambium.webrtc.srtp

import android.util.Log
import androidx.test.ext.junit.runners.AndroidJUnit4

import javax.crypto.*
import javax.crypto.spec.*
import java.security.*
import javax.security.*

import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*
import java.util.Arrays




@RunWith(AndroidJUnit4::class)
class EncryptorTest {

    @OptIn(ExperimentalStdlibApi::class)
    @Test
    fun testEncryption() {
        val rand = SecureRandom.getInstanceStrong();
        val keygen = KeyGenerator.getInstance(
            //"PBKDF2withHmacSHA256" is supposed to be available, but isn't. Just use the generic AES option.
            "AES"
        )
        keygen.init(256);
        val key = keygen.generateKey()
        val iv = ByteArray(16)
        val content = ByteArray(73)
        rand.nextBytes(iv)
        rand.nextBytes(content)
        val ivSpec = IvParameterSpec(iv)
        val ivHex = iv.toHexString()

        //Baseline: Encrypt using JCA, so we can compare to it.
        val cipher = Cipher.getInstance("AES/CTR/NoPadding")
        cipher.init(Cipher.ENCRYPT_MODE, key, ivSpec)
        val baseline = cipher.doFinal(content)
        Log.d("io.cambium.webrtc.srtp.Aes256FrameEncryptor", "Baseline length: " + baseline.size)

        //Now test using our code
        val encryptor = Aes256FrameEncryptor(key, ivHex)

        //Quickly verify that it can return a pointer to the native implementation
        assertNotEquals(0, encryptor.nativeFrameEncryptor);
        //Log the internal pointer so that we can verify in the logs that it correctly matches the native value
        Log.d("io.cambium.webrtc.srtp.Aes256FrameEncryptor",
            "Pointer: 0x" + encryptor.nativeFrameEncryptor.toHexString())

        //Quickly test some obvious use cases around defensive programming
        assertNull(encryptor.encrypt(null))
        val empty = encryptor.encrypt(ByteArray(0))
        assertNotNull(empty)
        assertTrue(empty.isEmpty())

        //Now check that it actually encrypts stuff
        val encrypted = encryptor.encrypt(content)
        assertEquals(baseline.size, encrypted.size)

        Log.d("io.cambium.webrtc.srtp.Aes256FrameEncryptor", "Original:  " + content.toHexString())
        Log.d("io.cambium.webrtc.srtp.Aes256FrameEncryptor", "Expected:  " + baseline.toHexString())
        Log.d("io.cambium.webrtc.srtp.Aes256FrameEncryptor", "Encrypted: " + encrypted.toHexString())

        //Now decrypt the natively encrypted content
        cipher.init(Cipher.DECRYPT_MODE, key, ivSpec)
        val decrypted = cipher.doFinal(encrypted)
        Log.d("io.cambium.webrtc.srtp.Aes256FrameEncryptor", "Decrypted: " + decrypted.toHexString())

        cipher.init(Cipher.DECRYPT_MODE, key, ivSpec)
        val validation = cipher.doFinal(baseline)
        Log.d("io.cambium.webrtc.srtp.Aes256FrameEncryptor", "Validated: " + validation.toHexString())

        //Sanity check:
        assertTrue(Arrays.equals(content, validation))
        //Now compare. First verify that the encryptor actually produced output different than the input.
        assertFalse("Output of encryptor is identical to the content - no encryption occurred!",
            Arrays.equals(content, encrypted))
        assertTrue("Encrypted content doesn't match baseline",
            Arrays.equals(baseline, encrypted))
    }
}