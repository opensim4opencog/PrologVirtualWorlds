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

public class IoSyntax {

  public AbstractPackage io_package =
     Package.find_or_create_package("OKBC-USER");
  public Node readtable_case = Node._upcase;
  public int read_base = 10;
  public int print_base = 10;
  public Node print_case = Node._upcase;
  public boolean print_escape = true;
  public boolean print_pretty = false;
  public boolean print_readably = false;
  public Kb kb = null;

  public IoSyntax copy () {
    IoSyntax res = new IoSyntax();
    res.io_package = io_package;
    res.readtable_case = readtable_case;
    res.read_base = read_base;
    res.print_base = print_base;
    res.print_case = print_case;
    res.print_escape = print_escape;
    res.print_pretty = print_pretty;
    res.print_readably = print_readably;
    res.kb = kb;
    return res;
  }

  public IoSyntax() {
    ensure_package();
  }

  void ensure_package() {
    if (io_package == null)
      { throw new GenericError
	  (Node._T, new OKBCString
	   ("Cannot have a null package in an IoSyntax")).signal();
      }
  }
    
  public IoSyntax(String package_name) {
    io_package = Package.find_or_create_package(package_name);
    ensure_package();
  }

  public IoSyntax(AbstractPackage pack) {
    io_package = pack;
    ensure_package();
  }

  protected void initialize_from_init_plist(Node initargs) {
    // System.out.println("Initargs = " + initargs);
    Enumerator e = initargs.enumerate_list();
    while (e.has_more_p()) {
      Node key = e.next();
      Node value = e.next();
      if (key == Node._package)
	{
	  String package_name = null;
	  if (value instanceof AbstractSymbol)
	    { package_name = ((AbstractSymbol)value).symbolName; }
	  else if (value instanceof OKBCString)
	    { package_name = ((OKBCString)value).string; }
	  if (package_name == null)
	    { throw new GenericError
		(Node._T, new OKBCString("Unknown package designation: " + 
					 value)).signal();
	    }
	  else { io_package = Package.find_or_create_package(package_name); }
	  // System.out.println("io_package is now: " + io_package);
	}
      else if (key == Node._print_case)
	{
	  if (value == Node._upcase ||
	      value == Node._downcase ||
	      value == Node._capitalize)
	    {
	      print_case = value;
	      // System.out.println("print_case is now: " + print_case);
	    }
	  else
	    { throw new GenericError
		(Node._T, new OKBCString("Unknown print case designation: " + 
					 value)).signal();
	    }
	}
    }
  }
  
}
