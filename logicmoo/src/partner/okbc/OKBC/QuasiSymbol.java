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

public class QuasiSymbol extends AbstractSymbol {
  public QuasiPackage symbolPackage = null;
  
  QuasiSymbol() {}
  
  QuasiSymbol(String thePackage, String name) {
    if (thePackage != null)
      { symbolPackage = QuasiPackage.find_or_create_QuasiPackage(thePackage); }
    symbolName = name;
  }

  QuasiSymbol(QuasiPackage thePackage, String name) {
    symbolPackage = thePackage;
    symbolName = name;
  }

  QuasiSymbol(String name) {
    symbolPackage = null;
    symbolName = name;
  }

  AbstractPackage getSymbolPackage() { return symbolPackage; }
  
  public static AbstractSymbol intern (String name, 
				       AbstractPackage thePackage) {
    if (thePackage instanceof QuasiPackage)
      { return intern(name, (QuasiPackage)thePackage); }
    else { return Symbol.intern(name, (Package)thePackage); }
  }
  
  public static AbstractSymbol intern (String name, QuasiPackage thePackage) {
    Package real_package = Package.find_package(thePackage.name);
    if (real_package == null)
      { QuasiPackage symbolPackage = thePackage;
	QuasiSymbol s = symbolPackage.intern(name);
	if (s instanceof QuasiSymbol)
	  { return s; }
	else { throw new RuntimeException("Cannot find QuasiSymbol!"); }
      }
    else { return real_package.intern(name); }
  }
  
  public static AbstractSymbol intern (String name, String thePackage) {
    Package real_package = Package.find_package(thePackage);
    if (real_package == null)
      { QuasiPackage symbolPackage =
	QuasiPackage.find_or_create_QuasiPackage(thePackage);
	QuasiSymbol s = symbolPackage.intern(name);
	if (s instanceof QuasiSymbol)
	  { return s; }
	else { throw new RuntimeException("Cannot find QuasiSymbol!"); }
      }
    else { return real_package.intern(name); }
  }

  public boolean keywordp () {
    return false;
  }
  
  public String toString() {
    if (symbolName == null) 
      { return symbolName; }
    if (symbolPackage == null) 
      { return "#:Q" + symbolName; }
    else 
      {return symbolPackage.name + "::" + symbolName; }
  }

  public boolean equals(Object thing) {
    return this == thing;
  }

  public boolean string_equals_sign(String string) {
    return symbolName.equals(string);
  }

  public boolean string_equals_sign(Node symbol) {
    return symbolName.equals(((QuasiSymbol)symbol).symbolName);
  }

}
