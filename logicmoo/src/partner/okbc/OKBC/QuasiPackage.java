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

public class QuasiPackage extends AbstractPackage {
  static final Hashtable pkg_table = new Hashtable();

  QuasiPackage (String theName) {
    name = theName;
    pkg_table.put(name, this);
  }

  public static QuasiPackage find_or_create_QuasiPackage
        (String theQuasiPackage) {
    QuasiPackage pkg = (QuasiPackage)(pkg_table.get(theQuasiPackage));
    if (pkg == null)
      { pkg = new QuasiPackage(theQuasiPackage);}
    return pkg;
  }

  public static QuasiPackage find_or_create_QuasiPackage
       (Symbol theQuasiPackage) {
    return QuasiPackage.find_or_create_QuasiPackage(theQuasiPackage.symbolName);
  }

  public static QuasiPackage find_or_create_QuasiPackage
       (QuasiSymbol theQuasiPackage) {
    return QuasiPackage.find_or_create_QuasiPackage(theQuasiPackage.symbolName);
  }

  public static QuasiPackage find_or_create_QuasiPackage
       (QuasiPackage theQuasiPackage) {
    return theQuasiPackage;
  }

  public QuasiSymbol find_symbol(String pname) {
    return (QuasiSymbol)(symbol_table.get(pname));
  }

  public QuasiSymbol intern (String name) {
    QuasiSymbol sym;
    sym = this.find_symbol(name);
    if (sym == null)
      { sym = new QuasiSymbol(this, name);
	symbol_table.put(name, sym);
      }
    return sym;
  }

  public QuasiSymbol intern (QuasiSymbol sym) {
    QuasiSymbol existing = this.find_symbol(sym.symbolName);
    if (existing == null)
      { symbol_table.put(sym.symbolName, sym);
	if (sym.symbolPackage == null) 
	  { sym.symbolPackage = this; }
	return sym;
      }
    else { return existing; }
  }

}
