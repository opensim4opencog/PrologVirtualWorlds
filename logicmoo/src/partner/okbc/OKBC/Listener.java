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

public class Listener {

  public static final Symbol _kb = Symbol.keyword("KB");
  public static final Symbol star = Symbol.keyword("*");
  public static final Symbol starstar = Symbol.keyword("**");
  public static final Symbol starstarstar = Symbol.keyword("***");
  public static final Symbol starstarstarstar = Symbol.keyword("****");
  public static final Symbol _current_kb = Symbol.intern_in_okbc("CURRENT-KB");
  public static final Symbol star_help_star = 
    Symbol.intern_in_ok_back("*HELP*");

  static OKBCString default_help_string = 
  new OKBCString("Type any OKBC expression in Lisp syntax.\n" +
		"For example, you can say things like:\n" +
		"  (setq foo (first (get-kb-root-classes)))\n" +
		"The variable KB is bound to the current KB.");

  public EvalContext environment = new EvalContext();
  public IoSyntax ioSyntax = null;
  public String listener_prompt = "OKBC>";

  public String get_listener_prompt () { return listener_prompt; }

  boolean catch_errors_p = true;

  public Listener (EvalContext env, boolean catch_p) {
    environment = env;
    catch_errors_p = catch_p;
  }

  public Listener (EvalContext env, boolean catch_p, ListenerEvaluator ev) {
    environment = env;
    catch_errors_p = catch_p;
    evaluator = ev;
  }

  public ListenerEvaluator evaluator = new ListenerEvaluator();

  public void propagate_result_to_star_variables(Node primary_result) {
    Cons help_entry  =
      (Cons)environment.get_binding_list_entry(star_help_star);
    Cons star4_entry = 
      (Cons)environment.get_binding_list_entry(starstarstarstar);
    Cons star3_entry = (Cons)environment.get_binding_list_entry(starstarstar);
    Cons star2_entry = (Cons)environment.get_binding_list_entry(starstar);
    Cons star1_entry = (Cons)environment.get_binding_list_entry(star);
    star4_entry.set_nth(1, star3_entry.nth(1));
    star3_entry.set_nth(1, star2_entry.nth(1));
    star2_entry.set_nth(1, star1_entry.nth(1));
    star1_entry.set_nth(1, primary_result);
  }


  static Symbol _upcase = Symbol.keyword("UPCASE");

  public void read_eval_print() {
    read_eval_print(true);
  }

  public void read_eval_print(boolean null_commands_ok_p) {
    Node kb = ListenerEvaluator.getenv(_kb, environment);
    System.out.println();
    //OKBCString line = OKBCApp.prompt_and_read_frame_name("=> ", null, false);
    //Node form = LispReader.readFromString(line.string, ioSyntax);
    LispReader r = new LispReader(System.in, ioSyntax, false, Node._NIL, 10, 
				  _upcase);
    r.null_for_leading_lfs = true;
    r.set_escape_character();
    System.out.print(get_listener_prompt());
    System.out.print(" ");
    Node form = r.read();
    if (form == null)
      { 
	if (!null_commands_ok_p)
	  { System.out.println("Listener warning: form is null!"); }
	throw new NullReturned();
      }
    if (environment == null)
      { System.out.println("Listener warning: environment is null!"); }
    if (evaluator == null)
      { System.out.println
	  ("Listener warning: evaluator is null - setting it!");
	evaluator = new ListenerEvaluator();
      }
    OKBCString string = Prin1ToString.prin1ToString(form, ioSyntax);
    Node interned = ((Kb)kb).coerce_to_kb_value
      (string, Node._value, Node._NIL, Node._NIL, Node._T,
       Node._do_not_coerce_to_frames, Node._NIL).firstValue();
    Object result = eval(interned, kb);
    Node primary_result = Node._NIL;
    if (result instanceof Node)
      { primary_result = (Node)result;
	evaluator.pretty_print((Kb)kb, primary_result); 
      }
    else if (result instanceof Values)
      { Values values = (Values)result;
	for(int i=0; i < values.length(); i++) {
	  if (i > 0) { System.out.println(); }
	  else { primary_result = values.nthValue(i); }
	  evaluator.pretty_print((Kb)kb, values.nthValue(i));
	}
      }
    else { System.out.println(result); }
    propagate_result_to_star_variables(primary_result);
  }

  public Object eval (Node form, Node kb) {
    Object result = evaluator.trivial_eval_expression_maybe_to_values
      (form, environment, 0, (kb != Node._NIL ? (Kb)kb : null));
    return result;
  }

  public void read_eval_print_loop() {
    Node kb = ListenerEvaluator.getenv(_kb, environment);
    if (ioSyntax == null) { ioSyntax = new IoSyntax(); }
    if (kb != Node._NIL) { ioSyntax = ((Kb)kb).ioSyntax; }
    if (ioSyntax == null) { ioSyntax = new IoSyntax(); }
    try
      {
	while (true) {
	  if (catch_errors_p)
	    { try { read_eval_print(); }
	      catch (ListenerQuit e) { throw e; }
	      catch (NullReturned e) {}
	      catch (Exception e)
		{ System.out.println("Error: " + e); }
	      catch (Error e)
		{ System.out.println("Error: " + e); }
	    }
	  else 
	    { try { read_eval_print(); }
	      catch (ListenerQuit e) { throw e; }
	      catch (NullReturned e) {}
	    }
	}
      }
    catch (ListenerQuit l) { System.out.println(" - resuming"); }
  }

  public static void breakLoop (Kb kb) {
    breakLoop(kb, null, true, false, null);
  }

  public static void breakLoop (Kb kb, EvalContext environment) {
    breakLoop(kb, environment, true, false, null);
  }

  public static void breakLoop (Kb kb, EvalContext environment,
				   boolean catch_errors_p) {
    breakLoop(kb, environment, catch_errors_p, false, null);
  }

  public static EvalContext build_default_listener_environment (Kb kb) {
    return build_default_listener_environment(kb, new EvalContext(kb));
  }

  public static EvalContext build_default_listener_environment
    (Kb kb, EvalContext environment) {
    environment.record_last_kb(kb);
    return environment.augment_multiple
      (Cons.list(Cons.list(_kb, kb),
		 Cons.list(_current_kb, kb),
		 Cons.list(star, Node._NIL),
		 Cons.list(starstar, Node._NIL),
		 Cons.list(starstarstar, Node._NIL),
		 Cons.list(starstarstarstar, Node._NIL),
		 Cons.list(star_help_star, default_help_string)));
  }

  public void startBreakLoop (Kb kb, EvalContext environment,
			      boolean catch_errors_p, boolean trace_p,
			      ListenerEvaluator ev) {
    Kb old_kb = Node.the_current_kb;
    try { Node.the_current_kb = kb;
	  if (trace_p) { evaluator.trace_p = true; }
	  read_eval_print_loop();
        }
    finally { Node.the_current_kb = old_kb; }
  }

  public static void breakLoop (Kb kb, EvalContext environment,
				boolean catch_errors_p, boolean trace_p,
				ListenerEvaluator ev) {
    Listener listener = new Listener (null, catch_errors_p, ev);
    if (environment == null)
      { environment = new EvalContext(kb, listener); }
    EvalContext real_environment 
      = build_default_listener_environment(kb, environment);
    listener.environment = real_environment;
    listener.startBreakLoop(kb, environment, catch_errors_p, trace_p, ev);
  }

  public static void breakLoop (Kb kb, String argv[]) {
    breakLoop(kb, null, !OKBCApp.argv_suppliedp("DONT-CATCH-ERRORS", argv),
	      OKBCApp.argv_suppliedp("TRACE-PROCEDURES", argv), null);
  }

  public static void breakLoop (Kb kb, String argv[], ListenerEvaluator ev) {
    breakLoop(kb, null, !OKBCApp.argv_suppliedp("DONT-CATCH-ERRORS", argv),
	      OKBCApp.argv_suppliedp("TRACE-PROCEDURES", argv), ev);
  }

  public static void main (String argv[]) {
    if (OKBCApp.argv_suppliedp("HELP", argv))
      { System.out.println("Usage: java Listener [-option]*");
	System.out.println("where options include:");
	System.out.println("    -taxa       : run against the TAXA test kb");
	System.out.println("    -cache      : use caching in taxa kb");
	System.out.println("    -trace-procedures   : trace procedures");
	System.out.print  ("    -dont-catch-errors  : ");
	System.out.println("don't catch errors that occur during processing.");
      }
    else
      { 
	Kb kb = null;
	if (OKBCApp.argv_suppliedp("TAXA", argv))
	  { kb = Test.create_taxa_kb (OKBCApp.argv_suppliedp("CACHE", argv));
	    Test.initialize_test_kb(kb);
	  }
	if (kb == null)
	  { throw new RuntimeException("No kb specified.  Try -help"); }
	breakLoop(kb, argv);
      }
  }
}


class NullReturned extends Error {}
