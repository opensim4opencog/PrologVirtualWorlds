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

public class Package extends AbstractPackage {
  static final Hashtable pkg_table = new Hashtable();

  Package (String theName) {
    name = theName;
    pkg_table.put(name, this);
  }

  public static Package find_package (String thePackage) {
    if (thePackage.equals("LISP") ||
        thePackage.equals("CL"))
      { thePackage = "COMMON-LISP"; }
    Package pkg = (Package)(pkg_table.get(thePackage));
    return pkg;
  }

  public static Package find_or_create_package (String thePackage) {
    if (thePackage.equals("LISP") ||
        thePackage.equals("CL"))
      { thePackage = "COMMON-LISP"; }
    Package pkg = (Package)(pkg_table.get(thePackage));
    if (pkg == null)
      { pkg = new Package(thePackage);}
    return pkg;
  }

  public static Package find_or_create_package (Symbol thePackage) {
    return Package.find_or_create_package(thePackage.symbolName);
  }

  public static Package find_or_create_package (Package thePackage) {
    return thePackage;
  }

  public Symbol find_symbol(String pname) {
    return (Symbol)(symbol_table.get(pname));
  }

  public Symbol intern (String name) {
    Symbol sym;
    sym = this.find_symbol(name);
    if (sym == null)
      { sym = new Symbol(this, name);
	symbol_table.put(name, sym);
      }
    return sym;
  }

  public Symbol intern (Symbol sym) {
    Symbol existing = this.find_symbol(sym.symbolName);
    if (existing == null)
      { symbol_table.put(sym.symbolName, sym);
	if (sym.symbolPackage == null) 
	  { sym.symbolPackage = this; }
	return sym;
      }
    else { return existing; }
  }

  static long gentemp_counter = 0;
  
  public Symbol gentemp (String name) {
    String new_name = name + gentemp_counter;
    gentemp_counter = gentemp_counter + 1;
    return intern(new_name);
  }
}

