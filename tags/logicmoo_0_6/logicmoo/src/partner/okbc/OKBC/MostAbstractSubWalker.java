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


public abstract class MostAbstractSubWalker { 

  public Node loop_collecting_walk_results (AbstractKifWalker kif_walker,
					    Kb kb, Node production_key,
					    Node known_context_type,
					    Node bound_vars, Node value,
					    AbstractWalkFunction
					    function_to_apply) {
    Node new_list = Node._NIL;
    Cons tail = null;
    Enumerator e = value.enumerate_list();
    while (e.has_more_p()) {
      Node spec = e.next();
      Node new_spec = kif_walker.get(production_key).walk_subexpression
	(kb, known_context_type, bound_vars, spec, function_to_apply);
      if (tail == null)
	{ tail = new Cons(new_spec);
	new_list = tail;
	}
      else { Cons new_tail = new Cons(new_spec);
      tail.set_cdr(new_tail);
      tail = new_tail;
      }
    }
    return new_list;
  }

  public String production_name () {
    String full_name = getClass().getName();
    int dot_index = full_name.lastIndexOf('.');
    if (dot_index == -1)
      { return full_name; }
    else 
      {
	return full_name.substring(dot_index + 1) ;
      }
  }

}
