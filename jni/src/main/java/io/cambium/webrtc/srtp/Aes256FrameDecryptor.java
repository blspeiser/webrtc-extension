package io.cambium.webrtc.srtp;

import javax.crypto.SecretKey;

import org.webrtc.FrameDecryptor;

import io.cambium.util.HexUtil;

/**
 * 
 * Aes256FrameDecryptor.
 *
 *  Decrypts frames for WebRTC using AES CTR 256 with no padding. 
 *
 * @author Baruch Speiser, Cambium.
 */
public class Aes256FrameDecryptor implements FrameDecryptor {
  private final byte[] key;
  private final byte[] iv;
  private long pointer; 
  
  static {
    System.loadLibrary("srtp");
  }
  
  public Aes256FrameDecryptor(byte[] key, byte[] iv) {
    if(null == key || key.length != 32) {
      throw new IllegalArgumentException("Key must be exactly 32 bytes!");
    }
    if(null == iv || iv.length != 16) {
      throw new IllegalArgumentException("Initialization vector must be exactly 16 bytes!");
    }
    this.key = key;
    this.iv = iv;
    this.pointer = initialize();
    if(this.pointer == 0) {
      throw new TypeNotPresentException("Native Aes256FrameDecryptor", null); //message is appended with " type not present"
    }
  }
  
  public Aes256FrameDecryptor(SecretKey key, byte[] iv) {
    this(key.getEncoded(), iv);
  }
  
  public Aes256FrameDecryptor(byte[] key, String ivHex) {
    this(key, HexUtil.decode(ivHex));
  }
  
  public Aes256FrameDecryptor(SecretKey key, String ivHex) {
    this(key, HexUtil.decode(ivHex));
  }

  private native long initialize();
  private native void destroy(long pointer);
  
  //These methods exists for testing the JNI and native implementation. 
  private native byte[] decrypt(long pointer, byte[] bytes);
  public byte[] decrypt(byte[] bytes) { 
    return decrypt(this.pointer, bytes); 
  }
  
  @Override
  public long getNativeFrameDecryptor() {
    return this.pointer;
  }
  
  /* 
   * Normally you should never override finalize, but since
   * we are holding on to natively allocated memory, we must
   * be sure to clean it up or otherwise we will have a memory
   * leak. 
   */
  @Override
  protected void finalize() throws Throwable {
    destroy(this.pointer);
    super.finalize();
  }

}
