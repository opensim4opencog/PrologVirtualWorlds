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

public class Evaluator extends AbstractEvaluator {

  public void initialize_evaluator_slots() {
    Enumeration e = evaluators.elements();
    for (; e.hasMoreElements() ;) {
      AbstractEvaluator ev = (AbstractEvaluator)(e.nextElement());
      ev.evaluators = evaluators;
      ev.initialized_p = true;
    }
  }   
 
  public void initialize_evaluators() {
    if (!initialized_p)
      { initialized_p = true;
        evaluators = new Hashtable();
	basic_initialize_evaluators(evaluators);
	initialize_evaluator_slots();
      }
  }

  /**
    * Use {@link #getOkbcEvaluator()} instead.
    * @deprecated
    */
  public static final Evaluator okbc_evaluator = new Evaluator ();

  static Map okbcEvaluatorMap = new HashMap(10);

  /**
    * Returns the appropriate Evaluator based on the ThreadGroup that the 
    * Thread this is running in belongs to.
    */
  public static Evaluator getOkbcEvaluator() {
    Thread thisThread = Thread.currentThread();
    ThreadGroup thisThreadGroup = thisThread.getThreadGroup();
    Evaluator okbcEval = (Evaluator)okbcEvaluatorMap.get(thisThreadGroup);
    
    if (okbcEval == null) {
      okbcEval = new Evaluator();
      okbcEvaluatorMap.put(thisThreadGroup, okbcEval);
    }
    return okbcEval;
  }
}

class SetqEvaluator extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    AbstractSymbol sym = (AbstractSymbol)(args.car());
    Node new_value = trivial_eval_expression
      (args.nth(1), environment, depth + 1, ckb);
    Cons entry = (Cons)(environment.get_binding_list_entry(sym));
    if (entry == null)
      {
	if (allow_interpreted_global_assignmenst_p)
	  { 
	    Node new_entry = Cons.list(sym, new_value);
	    environment.push(new_entry);
	    return new_value;
	  }
	else
	  { throw new GenericError
               (_T, new OKBCString("Cannot setq " + sym + ".  It is not in " +
				   "the environment.  You can only setq " +
				   "bound Symbols. The known bindings are: " +
				   environment.bindings)).signal();
	  }
      }
    else { Cons tail = (Cons)(entry.cdr());
           tail.set_car(new_value);
           return tail.car();
         }
  }
}

