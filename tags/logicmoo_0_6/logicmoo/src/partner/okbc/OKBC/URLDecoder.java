/***********************************************************************

  OKBC implementation for Java

  Copyright (c) 1996, 1997 Stanford University KSL.

  Original code by James Rice (rice@ksl.stanford.edu)


  KSL Network Services available at:
            http://www-ksl-svc.stanford.edu:5915/

  Legal Notice for KSL Network Services

  The software made available through the Stanford Knowledge Systems
  Laboratory's Network Services was written by and is copyright (c) by
  Stanford University, Knowledge Systems Laboratory and others not
  limited to those mentioned below.
  
  NEITHER THE AUTHORS NOR THE PUBLISHER OF THESE SERVICES AND DOCUMENTS
  OFFER ANY WARRANTY OF FITNESS OR MERCHANTABILITY FOR ANY PARTICULAR
  PURPOSE OR ACCEPTS ANY LIABILITY WITH RESPECT TO THE SERVICES,
  PROGRAMS, PROCEDURES, ALGORITHMS, DOCUMENTS OR APPLICATIONS OR
  DOCUMENTS MADE AVAILABLE THROUGH THE KNOWLEDGE SYSTEMS LABORATORY'S
  NETWORK SERVICES.
  
  
  ***********************************************************************/


package OKBC;
import java.util.*;
import java.io.*;
import java.net.*;

public class URLDecoder {

static boolean hex_char_p (int c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

static int hex_nibble_to_int (int d0) {
  if (d0 >= 'A') { return (d0 - 'A') + 10; }
  else { return d0 - '0'; }
}

static int char_from_hex_nibbles (int d1, int d0) {
  return (hex_nibble_to_int(d1) * 16) + hex_nibble_to_int(d0);
}

public static String decode(String s) {
  int maxBytesPerChar = 10;
  ByteArrayOutputStream out = new ByteArrayOutputStream(s.length());
  
  for (int i = 0; i < s.length(); i++) {
    int c = (int)s.charAt(i);
    if (c == '+')
      { out.write(' '); }
    else if (c == '%')
      {
	int d1 = (int)s.charAt(i + 1);
	int d0 = (int)s.charAt(i + 2);
	i = i + 2;
	if (hex_char_p(d0) && hex_char_p(d1))
	  {
	    int real_char = char_from_hex_nibbles(d1, d0);
	    out.write(real_char);
	  }
	else
	  { throw new GenericError
	    (Node._T, new OKBCString("Illegal URL: " + s)).signal();
	  }
      }
    else { out.write(c); }
  }
  
  return out.toString();
}

public static void main (String argv[]) {
  String test_string = "!@#$%^&*()_+|~1234567890-=`qwertyuiop[]asdfghjkl;'" +
    "zxcvbnm,./QWERTYUIOP{}ASDFGHJKL:ZXCVBNM<>?";
  String encoded = URLEncoder.encode(test_string);
  String decoded = URLDecoder.decode(encoded);
  System.out.println("Encoded  = " + encoded);
  System.out.println("Original = " + test_string);
  System.out.println("Decoded  = " + decoded);
  System.out.println("Ok-p = " + test_string.equals(decoded));
}

}
