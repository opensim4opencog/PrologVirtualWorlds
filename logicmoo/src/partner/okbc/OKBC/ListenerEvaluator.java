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

public class ListenerEvaluator extends Evaluator {

  public void pretty_print (Kb kb, Node thing) {
    Cons.pretty_print(thing);
  }
  
  public void initialize_evaluators() {
    if (!initialized_p)
      { initialized_p = true;
        evaluators = new Hashtable();
	basic_initialize_evaluators(evaluators);
	listener_initialize_evaluators(evaluators);
	initialize_evaluator_slots();
      }
  }

  public void listener_initialize_evaluators(Hashtable ht) {
    allow_interpreted_global_assignmenst_p = true;
    ht.put("HELP", new HelpEvaluator());
    ht.put("QUIT", new QuitEvaluator());
    ht.put("SYMBOL-VALUE", new ListenerSymValEvaluator());
  }

}

class HelpEvaluator extends Evaluator {
  public static final Symbol star_help_star = 
     Symbol.intern_in_ok_back("*HELP*");

  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    return trivial_eval_expression (star_help_star, environment, depth + 1,
				    ckb);
  }
}

class ListenerSymValEvaluator extends Evaluator {
  public Node eval (Node arg, EvalContext environment, int depth, Kb ckb) {
    AbstractSymbol sym = (AbstractSymbol)arg;
    if (sym.getSymbolPackage().name.equals("KEYWORD"))
      { String string = sym.symbolName;
	boolean lowercase_found_p = false;
	for (int i = 0; i < string.length() ; i++) {
	  char c = string.charAt(i);
	  if ((c >= 'a') && (c <= 'z'))
	    { lowercase_found_p = true;
	      break;
	    }
	}
	if (lowercase_found_p)
	  { return QuasiSymbol.intern(string.toUpperCase(),
				      sym.getSymbolPackage()); }
	else { return sym; }
      }
    else 
      { Node entry = environment.get_binding_list_entry(sym);
	if (entry != null) { return entry.nth(1); }
	else if (sym.symbolName.equalsIgnoreCase("T")) { return Kb._T; }
	else if (sym.symbolName.equalsIgnoreCase("NIL"))
	  { return Kb._NIL; }
	else if (sym.symbolValue != null) { return sym.symbolValue; }
	else { Node matches = Node.the_current_kb.get_frames_matching
		 (sym, Node._NIL, Node._all, Node._NIL,
		  Node._NIL).firstValue();
	       if (matches.length() == 1) { return matches.car(); } // DWIM
	       else if (matches.length() > 1)
		    { throw new GenericError
			(_T, new OKBCString
			 (sym + " is unbound.  DWIM matching is not unique." +
			  "  The known bindings are: " +
			  environment.bindings)).signal();
	            }
	       else { throw new GenericError
			(_T, new OKBCString
			 (sym + " is unbound.  The known bindings are: " +
			  environment.bindings)).signal();
	            }
	     }
      }
  }
}

class QuitEvaluator extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    throw new ListenerQuit();
  }
}

