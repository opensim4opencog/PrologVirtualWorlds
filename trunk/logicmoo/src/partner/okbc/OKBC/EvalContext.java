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

public class EvalContext {

  Kb last_kb = null;
  Node bindings = Node._NIL;
  public Listener listener = null;

  public EvalContext() {}

  public EvalContext(Kb the_kb) {
    last_kb = the_kb;
  }

  public EvalContext(Kb the_kb, Listener the_listener) {
    last_kb = the_kb;
    listener = the_listener;
  }

  public EvalContext(Kb the_kb, Node the_bindings, Listener the_listener) {
    last_kb = the_kb;
    bindings = the_bindings;
    listener = the_listener;
  }

  static Node assoc_string_equal (AbstractSymbol key, Node list) {
    if (list == null) { return null; }
    else { Enumeration e = list.elements();
	   for (; e.hasMoreElements() ;) {
	     Cons elt = (Cons)(e.nextElement());
	     AbstractSymbol car = (AbstractSymbol)(elt.car());
	     if (key.symbolName.equalsIgnoreCase(car.symbolName))
	       { return elt; }
	   }
	   return null;
	 }
  }

  public Node getenv(AbstractSymbol key) {
    Node entry = assoc_string_equal(key, bindings);
    if (entry == null || entry == Node._NIL) 
      { if ((key == Evaluator._current_kb) && (Node.current_kb() != null))
	  { return Node.current_kb();}
	else { return Node._NIL; }
      }
    else { return entry.nth(1); }
  }

  public void putenv(AbstractSymbol key, Node value) {
    // System.out.println("Old bindings: " + bindings);
    Node entry = assoc_string_equal(key, bindings);
    if (entry == null || entry == Node._NIL)
      { augment(Cons.list(key, value)); }
    else { ((Cons)entry).set_nth(1, value); }
    // System.out.println("New bindings: " + bindings);
  }

  public Node get_binding_list_entry(AbstractSymbol key) {
    return assoc_string_equal(key, bindings);
  }

  public EvalContext copy_self() {
    return new EvalContext(last_kb, bindings, listener);
  }

  public EvalContext augment(Node augmentation) {
    return new EvalContext(last_kb,
			   new Cons(augmentation, bindings),
			   listener);
  }

  public void push(Node augmentation) {
    bindings = new Cons(augmentation, bindings);
  }

  public EvalContext augment_multiple(Node augmentation) {
    return new EvalContext(last_kb,
			   Cons.append_cons(augmentation, bindings),
			   listener);
  }

  public void record_last_kb(Kb the_kb) {
    last_kb = the_kb;
  }

  public Kb get_last_kb() {
    return last_kb;
  }

}
