package io.cambium.util;

/**
 * HexUtil.
 *
 * Can't rely on DatatypeConverter or HexFormat,
 * because the execution environment may not have either 
 *
 * @author Baruch Speiser, Cambium.
 */
public class HexUtil {

  public static byte[] decode(String s) {
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
