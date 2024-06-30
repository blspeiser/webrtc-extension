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
class DecryptorTest {

    @OptIn(ExperimentalStdlibApi::class)
    @Test
    fun testDecryption() {
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

        //Preparation: Encrypt using JCA.
        val cipher = Cipher.getInstance("AES/CTR/NoPadding")
        cipher.init(Cipher.ENCRYPT_MODE, key, ivSpec)
        val encrypted = cipher.doFinal(content)
        Log.d("io.cambium.webrtc.srtp.Aes256FrameDecryptor", "Encrypted length: " + encrypted.size)

        //Baseline: Decrypt using JCA, so we can compare to it.
        cipher.init(Cipher.DECRYPT_MODE, key, ivSpec)
        val baseline = cipher.doFinal(encrypted)
        Log.d("io.cambium.webrtc.srtp.Aes256FrameDecryptor", "Baseline length: " + baseline.size)

        //Now test using our code
        val decryptor = Aes256FrameDecryptor(key, ivHex)

        //Quickly verify that it can return a pointer to the native implementation
        assertNotEquals(0, decryptor.nativeFrameDecryptor);
        //Log the internal pointer so that we can verify in the logs that it correctly matches the native value
        Log.d("io.cambium.webrtc.srtp.Aes256FrameDecryptor",
            "Pointer: 0x" + decryptor.nativeFrameDecryptor.toHexString())

        //Quickly test some obvious use cases around defensive programming
        assertNull(decryptor.decrypt(null))
        val empty = decryptor.decrypt(ByteArray(0))
        assertNotNull(empty)
        assertTrue(empty.isEmpty())

        //Now check that it actually decrypts stuff
        val decrypted = decryptor.decrypt(encrypted)
        assertEquals(baseline.size, decrypted.size)

        Log.d("io.cambium.webrtc.srtp.Aes256FrameDecryptor", "Original:  " + content.toHexString())
        Log.d("io.cambium.webrtc.srtp.Aes256FrameDecryptor", "Encrypted: " + encrypted.toHexString())
        Log.d("io.cambium.webrtc.srtp.Aes256FrameDecryptor", "Expected:  " + baseline.toHexString())
        Log.d("io.cambium.webrtc.srtp.Aes256FrameDecryptor", "Decrypted: " + decrypted.toHexString())

        //Sanity check:
        assertTrue(Arrays.equals(content, baseline))
        //Now compare. First verify that the encryptor actually produced output different than the input.
        assertFalse("Output of decryptor is identical to the encrypted - no decryption occurred!",
            Arrays.equals(encrypted, decrypted))
        assertTrue("Decrypted content doesn't match baseline",
            Arrays.equals(baseline, decrypted))
    }
}