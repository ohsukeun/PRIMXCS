package egovframework.com.primx.fun.cmm.service;

import java.nio.charset.StandardCharsets;
import java.util.Random;
import java.util.regex.Pattern;

public class TextUtils {
    
  public static String getRendomString(int n) {
    byte[] array = new byte[256];
    (new Random()).nextBytes(array);
    String rendomString = new String(array, StandardCharsets.UTF_8);
    StringBuffer r = new StringBuffer();
    String AlphaNumericString = rendomString.replaceAll("[^A-Za-z0-9]", "");
    for (int k = 0; k < AlphaNumericString.length(); k++) {
      if ((Character.isLetter(AlphaNumericString.charAt(k)) && n > 0) || (
        
        Character.isDigit(AlphaNumericString.charAt(k)) && n > 0)) {
        r.append(AlphaNumericString.charAt(k));
        n--;
      } 
    } 
    return r.toString();
  }
  
  public static boolean isValidEmail(String email) {
    String regexPattern = "^(?=.{1,64}@)[A-Za-z0-9_-]+(\\.[A-Za-z0-9_-]+)*@[^-][A-Za-z0-9-]+(\\.[A-Za-z0-9-]+)*(\\.[A-Za-z]{2,})$";
    return Pattern.compile(regexPattern).matcher(email).matches();
  }
}
