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

public class Symbol extends AbstractSymbol {
  public Package symbolPackage = null;
  
  AbstractPackage getSymbolPackage() { return symbolPackage; }
  
  Symbol() {}

// Now that we have QuasiSymbols we don't allow symbols to get created
// like this.  They have to be interned!
//  Symbol(String thePackage, String name) {
//   if (thePackage != null)
//      { symbolPackage = Package.find_or_create_package(thePackage); }
//    symbolName = name;
//  }

  Symbol(Package thePackage, String name) {
    Symbol existing = thePackage.find_symbol(name);
    if (existing == null)
      { symbolPackage = thePackage;
	symbolName = name;
      }
    else
      { throw new GenericError
	   (_T, new OKBCString("There is already symbol " + existing)).signal();
      }
  }

  Symbol(String name) {
    symbolPackage = null;
    symbolName = name;
  }

  public static Symbol intern (String name, Package thePackage) {
    // Special case for quote, which is always interned in the LISP package.
    if (name.equals("QUOTE") && !thePackage.name.equals("LISP"))
      { return intern(name, "LISP"); }
    else { Package symbolPackage = thePackage;
	   Symbol s = symbolPackage.intern(name);
	   if (s instanceof Symbol)
	     { return s; }
	   else { throw new RuntimeException("Cannot find symbol!"); }
	 }
  }
  
  public static Symbol intern (String name, String thePackage) {
    Package symbolPackage = Package.find_or_create_package(thePackage);
    Symbol s = symbolPackage.intern(name);
    if (s instanceof Symbol)
      { return s; }
    else { throw new RuntimeException("Cannot find symbol!"); }
  }
  
  public static Symbol gentemp (String name, String thePackage) {
    Package symbolPackage = Package.find_or_create_package(thePackage);
    return symbolPackage.gentemp(name);
  }
  
  public static Symbol keyword (String name) {
    return Symbol.intern(name, "KEYWORD");
  }
  
  public static Symbol intern_in_okbc (String name) {
    return Symbol.intern(name, "OKBC");
  }
  
  public static Symbol intern_in_ok_back (String name) {
    return Symbol.intern(name, "OK-BACK");
  }
  
  protected void encode(PrintWriter s, boolean client_p) {
    s.print(typeCode);
    newline(s);
    if (this.symbolPackage == null)
      { s.print(3); 
	newline(s);
	s.print("NIL");
      }
    else { s.print(this.symbolPackage.name.length()); 
	   newline(s);
	   s.print(this.symbolPackage.name);
	 }
    newline(s);
    s.print(this.symbolName.length());
    newline(s);
    s.print(this.symbolName);
    newline(s);
  }

  public boolean keywordp () {
    if (symbolPackage == null) { return false ; }
    else { return symbolPackage.name.equals("KEYWORD"); }
  }
  
  public String toString() {
    if (symbolName == null) 
      { return symbolName; }
    if (symbolPackage == null) 
      { return "#:" + symbolName; }
    else if ( symbolPackage.name.equals("KEYWORD"))
      {return ":" + symbolName; }
    else if ( symbolPackage.name.equals("LISP"))
      {return symbolName; }
    else 
      {return symbolPackage.name + "::" + symbolName; }
  }

  public boolean equals(Object thing) {
    return this==thing;
  }

  public Node nth(Integer n) {
    if (symbolName.equals("NIL"))
      { return _NIL; }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public Node nth(int n) {
    if (symbolName.equals("NIL"))
      { return _NIL; }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public void set_nth(int n, Node to) {
    throw new RuntimeException ("Illegal operation.  Not a list");
  }

  public Node car() {
    if (symbolName.equals("NIL"))
      { return _NIL; }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public int length() {
    if (symbolName.equals("NIL"))
      { return 0; }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public boolean isEmpty() {
    if (symbolName.equals("NIL"))
      { return true; }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public Values toValues() {
    if (symbolName.equals("NIL"))
      { return new Values(); }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public Node member(Node thing) {
    if (symbolName.equals("NIL"))
      { return _NIL; }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public Node member(Node thing, Node test) {
    if (symbolName.equals("NIL"))
      { return _NIL; }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public Enumeration elements() {
    if (symbolName.equals("NIL"))
      { return _NIL.elements(); }
    else
      { throw new RuntimeException ("Illegal operation.  " + this +
				    " is not a list or NIL"); 
      }
  }

  public boolean string_equals_sign(String string) {
    return symbolName.equals(string);
  }

  public boolean string_equals_sign(Node symbol) {
    return symbolName.equals(((Symbol)symbol).symbolName);
  }

}


