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

public class ValueSelector extends Symbol {

  ValueSelector(String name) {
    symbolName = name;
  }

  public static ValueSelector value_selector_keyword (String name) {
    Package symbolPackage = Package.find_or_create_package("KEYWORD");
    Symbol sym = symbolPackage.find_symbol(name);
    if (sym == null)
      { return (ValueSelector)
	(symbolPackage.intern(new ValueSelector(name))); }
    else if (sym instanceof ValueSelector)
      { return (ValueSelector)sym; }
    else { throw new RuntimeException
	   ("Consistency error.  Wrong symbol type.");
	 }
  }
  
}
