/***********************************************************************

  OKBC implementation for Java

  Copyright (c) 1996, 1997 Stanford University KSL.

  Original code by Peter S. Kim (pskim@cs.stanford.edu)


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

/*
	BrowserListener.java
	-----------------------
	By Peter S. Kim (pskim@cs.stanford.edu)
*/

package OKBC;
import java.util.*;
import java.io.*;


public class BrowserListener extends Listener
 {
   OKBCString line;	// Actual command
   static String newline =
     System.getProperties().getProperty("line.separator");
   
   BrowserListener (EvalContext env, boolean catch_p, OKBCString _line)
  {
    super(env, catch_p);
    line = _line;
  }

   String evalString() {
     String sResult="";
     System.out.println("\tEvaluating string " + line);
     IoSyntax ioSyntax = new IoSyntax();
     Node kb = ListenerEvaluator.getenv(_kb, environment);
     if (kb != Node._NIL)
       { ioSyntax = ((Kb)kb).ioSyntax; }
     Node form = LispReader.readFromString(line.string, ioSyntax);
     Object result = evaluator.trivial_eval_expression_maybe_to_values
       (form, environment, 0, (kb != Node._NIL ? (Kb)kb : null));
     Node primary_result = Node._NIL;
     if (result instanceof Node)
       { primary_result = (Node)result;
	 sResult += prettyReturn(primary_result, 0); 
       }
     else if (result instanceof Values)
       { Values values = (Values)result;
	 for(int i=0; i < values.length(); i++) {
		if (i > 0) { sResult+=newline; } //System.out.println();
		else { primary_result = values.nthValue(i); }
		sResult+=prettyReturn(values.nthValue(i), 0);
	      }
       }
     else { sResult+=result.toString() + newline; }
     propagate_result_to_star_variables(primary_result);
     
     return sResult;
   }
   
   
   public static String prettyReturn(Node form, int indent) 
  {
    String result="";
    if (form instanceof Cons)
      { 
	result += "("; // System.out.print("(");
	if (((Cons)form).cdr == null || ((Cons)form).cdr == Node._NIL)
	  { 
	    result += prettyReturn(((Cons)form).car, indent + 1);
	    result += ")"; // System.out.print(")");
	  }
	else 
	  {
	    result += prettyReturn(((Cons)form).car, indent + 1);
	    Node e = ((Cons)form).cdr;
	    do 
	      {
		result += newline; //System.out.println();
		for(int i=0; i < indent + 2; i++) {
		  result += " "; // System.out.print(" ");
		}
		result += prettyReturn(((Cons)e).car, indent + 2);
		e = ((Cons)e).cdr;
	      } while (e instanceof Cons);
	    if (e != null)
	      { 
		result += " . "; // System.out.print(" . ");
		result += newline; //System.out.println();
		for(int i=0; i < indent + 4; i++) {
		  result += " "; // System.out.print(" ");
		}
		result += prettyReturn(e, indent);
	      }
	    result += ")"; // System.out.print(")");
	  }
      }
    else { result += form.toString(); } // System.out.print(form); 
    
    return result;
  }
   
   
   
   public static String breakLoop (OKBCString _line, Kb kb) {
     return breakLoop(_line, kb, new EvalContext(kb), true, false);
   }
   
   public static String breakLoop 
      (OKBCString _line, Kb kb, EvalContext environment) {
     return breakLoop(_line, kb, environment, true, false);
   }
   
   public static String breakLoop
     (OKBCString _line, Kb kb, EvalContext environment,
      boolean catch_errors_p) {
     return breakLoop(_line, kb, environment, catch_errors_p, false);
   }
   
   public static String breakLoop
     (OKBCString _line, Kb kb, EvalContext environment,
      boolean catch_errors_p, boolean trace_p) 
  {
    String result;
    EvalContext real_environment = build_default_listener_environment
      (kb, environment);
    BrowserListener listener = new BrowserListener
      (real_environment, catch_errors_p, _line);
    if (trace_p) { listener.evaluator.trace_p = true; }
    if (catch_errors_p)
      {
	try
	  {
	    result=listener.evalString();
	  }
	catch (GenericError e) { result = "Error: " + e.toString(); }
	catch (RuntimeException e) { result="Error: " + e.toString(); }
      }
    else result=listener.evalString();
    return result;
  }
   
}
