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

public class AbstractSymbol extends Node {
  static final int typeCode = SYMBOL;
  public String symbolName = null;
  public Node symbolValue = null;

  AbstractPackage getSymbolPackage() {
    throw new RuntimeException("Missing method");
  }
  
  // Encode QuasiSymbols as if they were Symbols!
  protected void encode(PrintWriter s, boolean client_p) {
    s.print(typeCode);
    newline(s);
    if (getSymbolPackage() == null)
      { s.print(3); 
	newline(s);
	s.print("NIL");
      }
    else { s.print(getSymbolPackage().name.length()); 
	   newline(s);
	   s.print(getSymbolPackage().name);
	 }
    newline(s);
    s.print(this.symbolName.length());
    newline(s);
    s.print(this.symbolName);
    newline(s);
  }

  boolean stringEquals (Node sym) {
    if (sym instanceof AbstractSymbol)
      { return stringEquals((AbstractSymbol)sym); }
    else if (sym instanceof OKBCString)
      { return stringEquals((OKBCString)sym); }
    else { throw new GenericError
	   (Node._T, new OKBCString(sym + " is not a generalized string"))
	   .signal();
	 }
  }

  boolean stringEquals (AbstractSymbol sym) {
    return symbolName.equalsIgnoreCase(sym.symbolName);
  }

  boolean stringEquals (String string) {
    return symbolName.equalsIgnoreCase(string);
  }

  boolean stringEquals (OKBCString string) {
    return symbolName.equalsIgnoreCase(string.string);
  }

  static boolean stringEquals (AbstractSymbol sym, String string) {
    return sym.symbolName.equalsIgnoreCase(string);
  }

  public boolean keywordp () {
    return false;
  }
  
}
