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


public abstract class AbstractSubKeyValueWalker extends MostAbstractSubWalker {

  AbstractKeyValueWalker key_and_value_walker = null;
  AbstractKifWalker kif_walker = null;

  void set_key_and_value_walker (AbstractKeyValueWalker the_walker) {
    key_and_value_walker = the_walker;
  }
  
  public Node walk_key_and_value_pair (Kb kb, Node key, 
				       Node known_context_type,
				       Node bound_vars, Node value,
				       AbstractWalkFunction function_to_apply)
  {
    kif_walker.add_depth();
    if (kif_walker.trace_p)
      {
	System.out.println();
	for (int i=1; i < kif_walker.depth ; i++) {
	  System.out.print(" ");
	}
	System.out.print(kif_walker.depth +
			 " Enter walk plist: " + this.getClass().getName() +
			 " key= " + key + " value= " + value);
      }
    Node result = walk_key_and_value_pair_1
      (kb, key, known_context_type, bound_vars, value, function_to_apply);
    if (kif_walker.trace_p)
      {
	System.out.println();
	for (int i=1; i < kif_walker.depth ; i++) {
	  System.out.print(" ");
	}
	System.out.print(kif_walker.depth +
			 " Exit walk plist:  " + this.getClass().getName() +
			 " key= " + key + " value= " + value + " -> " + 
			 result);
      }
    kif_walker.decrement_depth();
    return result;
  }

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply)
  {
    throw new GenericError
      (Node._T, new OKBCString("Missing walk_key_and_value_pair for: " +
			       this)).signal();
  }

  public Node loop_collecting_walk_results (Kb kb, Node production_key,
					    Node known_context_type,
					    Node bound_vars, Node value,
					    AbstractWalkFunction
					    function_to_apply) {
    return loop_collecting_walk_results(kif_walker, kb, production_key,
					known_context_type, bound_vars,
					value, function_to_apply);
  }
    

}  
