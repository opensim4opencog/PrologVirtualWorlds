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

public class Condition extends Error {

  public Node plistGet (Symbol key, Node plist) {
    // System.out.println("plistGet " + key + " and " + plist);
    // The CAR of the list is the condition string, the CADR is the condition
    // name, the CDDR is the init plist.
    for(int i = 2; i < plist.length(); i++) {
      Node this_key = plist.nth(i);
      // System.out.println("compare " + key + " and " + this_key);
      if (this_key.equals(key))
	{ return plist.nth(i+1); }
      else { i = i + 1; }
    }
    return null;
  }

  public void init_from_plist (Node list) {
    System.out.println("Should never get here.  " +
		       " Performing null initialization of " + this);
  }

  public Node decode_to_plist() {
    System.out.println("Should never get here.  " +
		       " Cannot decode a generic condition to a plist.");
    return Node._NIL;
  }

  public String report () {
    return "Error!  No condition message found";
  }

  public String getMessage () {
    return report();
  }

  public Node kb = null;
  public static boolean listener_on_error_p = false;

  Node environment_from_plist() {
    Node res = Node._NIL;
    Enumeration e = decode_to_plist().elements();
    while (e.hasMoreElements()) {
      Symbol key = (Symbol)(e.nextElement());
      Node value = (Node)(e.nextElement());
      if (key.symbolName.equals("KB")) {}
      else { res = new Cons(Cons.list(key, value), res); }
    }
    return res;
  }

  void describe_plist() {
    System.out.println("Environment:");
    Enumeration e = decode_to_plist().elements();
    while (e.hasMoreElements()) {
      Symbol key = (Symbol)(e.nextElement());
      Node value = (Node)(e.nextElement());
      System.out.println("  " + key.symbolName + " = " + value);
    }
    System.out.println();
  }

  public Condition signal() {
    if (listener_on_error_p)
      { try { listener_on_error_p = false;
	      if (kb instanceof Kb)
		{ System.out.println("Entering error break loop for: "
				     + this + ".");
		  describe_plist();
		  Listener.breakLoop
		    ((Kb)kb, new EvalContext((Kb)kb,
					     environment_from_plist(),
					     null));
		}
	      else if (Node.the_current_kb instanceof Kb)
		{ System.out.println("Entering error break loop for: "
				     + this + ".");
		  describe_plist();
		  Listener.breakLoop
		    (Node.the_current_kb, 
		     new EvalContext(Node.the_current_kb,
				     environment_from_plist(),
				     null)); 
		}
	      else { throw this; }
	    }
	finally { listener_on_error_p = true; }
      }
    else { throw this; }
    return this;
  }

}
