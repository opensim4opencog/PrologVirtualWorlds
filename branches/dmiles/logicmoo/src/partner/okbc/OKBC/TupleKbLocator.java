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

public class TupleKbLocator extends AbstractKbLocator {
  OKBCString path;

  public TupleKbLocator(Node the_name, Kb the_kb_type, OKBCString the_path) {
    name = the_name;
    kb_type = the_kb_type;
    path = the_path;
    String broken_cookie = File.separator + File.separator;
    int index = path.string.indexOf(broken_cookie);
    if (index >= 0)
      { path = new OKBCString(path.string.substring
			      (index + broken_cookie.length()));
	System.out.println("Mangled path " + the_path + " trimmed it to "
			   + path + ".");
      }
  }

  public File maybe_defaulted_file(String default_dir) {
    if (path.toString().startsWith(default_dir))
      { return new File(path.toString()); }
    else { return new File(default_dir, path.toString());}
  }
  
  public String toString() {
    return
      "["
      + getClass().getName()
      + " " 
      + name.toString()
      + " of type " 
      + kb_type.toString()
      + " in path \"" 
      + new File(path.toString())
      + "\"]";
  }
  
  public Node pretty_name () {
    return new OKBCString(((MetaKb)Node.meta_kb()).value_as_string
			  (name, Node._user_interface, Node._T).firstValue()
			  + " ("
			  + ((MetaKb)Node.meta_kb()).value_as_string
			  (Node.abstract_type_from_concrete_kb(kb_type),
			   Node._user_interface, Node._T).firstValue()
			  + (path.string.endsWith
			     (TupleKb.portable_kb_file_extension)
			     ? " [portable]"
			     : "")
			  + ")");
  }

}
