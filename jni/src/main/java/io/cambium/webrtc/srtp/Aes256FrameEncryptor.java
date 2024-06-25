package io.cambium.webrtc.srtp;

import javax.crypto.SecretKey;

import org.webrtc.FrameEncryptor;

public class Aes256FrameEncryptor implements FrameEncryptor {
  private final byte[] key;
  private final byte[] iv;
  private long pointer; 
  
  static {
    System.loadLibrary("srtp");
  }
  
  public Aes256FrameEncryptor(byte[] key, byte[] iv) {
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
      throw new TypeNotPresentException("Native Aes256FrameEncryptor", null); //message is appended with " type not present"
    }
  }
  
  public Aes256FrameEncryptor(SecretKey key, String ivHex) {
    this(key.getEncoded(), decode(ivHex));
  }

  private native long initialize();
  private native void destroy(long pointer);
  
  //These method exists for testing the JNI and native implementation. 
  private native byte[] encrypt(long pointer, byte[] bytes);
  public byte[] encrypt(byte[] bytes) { 
    return encrypt(this.pointer, bytes); 
  }
  
  @Override
  public long getNativeFrameEncryptor() {
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
  
  /*
   * Can't rely on DatatypeConverter or HexFormat,
   * because the execution environment may not have either 
   */
  private static byte[] decode(String s) {
    int len = s.length();
    if(len % 2 != 0) throw new IllegalArgumentException("Hex strings must always be an even number of characters");
    byte[] data = new byte[len / 2];
    for (int i = 0; i < len; i += 2) {
        data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                             + Character.digit(s.charAt(i+1), 16));
    }
    return data;
  }
}
