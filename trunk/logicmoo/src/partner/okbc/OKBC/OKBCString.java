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

import java.net.Socket;
import java.net.URLEncoder;
import java.net.UnknownHostException;

public class OKBCString extends Node {
  static final int typeCode = STRING;
  public String string;

  public OKBCString () {
    string = "";
  }

  public OKBCString (String theString) {
    string = theString;
  } 

  protected void encode(PrintWriter s, boolean client_p) {
    s.print(typeCode);
    newline(s);
    s.print(string.length());
    newline(s);
    s.print(string);
    newline(s);
  }
  public String toString() {
    return string;
  }

  public boolean equals(Object thing) {
    if (thing instanceof OKBCString)
      { return string.equals(((OKBCString)thing).string); }
    else { return false; }
  }

  public int hashCode() {
    return string.hashCode() + 43;
  }

  public OKBCString capitalize () {
    return capitalize(false, 0);
  }

  public OKBCString capitalize (boolean replace_dashes_p) {
    return capitalize(replace_dashes_p, 0);
  }

  String capitalize_1 (String s, boolean replace_dashes_p, int index) {
    if (index >= s.length())
      { return ""; }
    else if (s.charAt(index) == '_' || s.charAt(index) == '-')
      { if (replace_dashes_p)
	  { return " " + 
	           capitalize_1(s.substring(index + 1), replace_dashes_p, 0); 
	  }
	else
	  { return s.charAt(index) + 
	           capitalize_1(s.substring(index + 1), replace_dashes_p, 0); 
	  }
      }
    else if (index > 0)
      { return Character.toLowerCase(s.charAt(index)) +
	capitalize_1(s, replace_dashes_p, index + 1); 
      }
    else 
      { return Character.toUpperCase(s.charAt(index)) +
	       capitalize_1(s, replace_dashes_p, index + 1); 
      }
  }

  public OKBCString capitalize (boolean replace_dashes_p, int index) {
    return new OKBCString (capitalize_1(string, replace_dashes_p, index));
  }

  public static OKBCString capitalize (String s) {
    return new OKBCString(s).capitalize();
  }

  public static void main (String arg[]) {
    System.out.println(new OKBCString("hello").capitalize());
    System.out.println(new OKBCString("HELLO").capitalize());
    System.out.println(new OKBCString("hello-bill").capitalize());
    System.out.println(new OKBCString("").capitalize());
    System.out.println(new OKBCString("hello-bill-no-dashes").capitalize(true));
    System.out.println(new OKBCString("hello").capitalize(true));
    System.out.println(new OKBCString("").capitalize(true));
  }

}

