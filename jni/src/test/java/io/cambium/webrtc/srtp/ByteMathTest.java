package io.cambium.webrtc.srtp;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class ByteMathTest {

  public static int adjust(int x) {
    return(x % 16 == 0) ? x : (x+16) & ~15;
  }
  
  @Test
  public void testMath() {
    assertEquals(128, adjust(128));
    assertEquals(128, adjust(127));
    assertEquals(128, adjust(113));
    assertEquals(112, adjust(112));
    assertEquals(112, adjust(111));
  }
  
}
