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

public class OKBCInteger extends Node {
  static final int typeCode = INTEGER;
  public long number;
  public OKBCInteger() {}
  
  public OKBCInteger(long i) {
    number = i;
  }
  
  public OKBCInteger(Integer i) {
    number = i.intValue();
  }
  
  public OKBCInteger(Long i) {
    number = i.longValue();
  }
  
  protected void encode(PrintWriter s, boolean client_p) {
    String string = String.valueOf(number);
    s.print(typeCode);
    newline(s);
    s.print(string.length());
    newline(s);
    s.print(string);
    newline(s);
  }
  
  public String toString() {
    return String.valueOf(number);
  }

  public boolean equals(Object thing) {
    if (thing instanceof OKBCInteger)
      { boolean res = number == ((OKBCInteger)thing).number;
	return res; 
      }
    else { return false; }
  }

  public int hashCode() {
    return (int)number + 17;
  }

}

