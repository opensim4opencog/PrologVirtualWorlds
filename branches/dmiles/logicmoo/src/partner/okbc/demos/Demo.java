/***********************************************************************

  OKBC demos for Java

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

package demos;
import java.util.*;
import java.io.*;
import OKBC.*;

public class Demo {

  static boolean catch_errors_p = false;
  static boolean verbose_p = true;
  OKBCString frame_name = new OKBCString("THING");
  OKBCString slot_name = new OKBCString("SUPERCLASS-OF");
  Node frame = null;
  Node slot = null;
  OKBCApp okbcApp = new OKBCApp();

  void get_frame_and_slot (Kb kb) {
    if (frame_name.string.length() > 0 && slot_name.string.length() > 0)
      { 
	Values2 frames = kb.get_frames_matching
	  (frame_name, Kb._NIL, Kb._all, Kb._NIL, 
	   Kb._NIL);
	if (frames.firstValue().length() != 1)
	  { System.out
	    .println(frame_name + " does not uniquely identify a frame.  " +
		     "Matches: " + frames.firstValue()); 
	    frame = null;
	    frame_name = (OKBCString)frames.nthValue(1);
	  }
	else { frame = frames.firstValue().car(); }

	Values2 slots = kb.get_frames_matching
	  (slot_name, Kb._NIL, Kb._all, Kb._NIL, 
	   Kb._NIL);
	if (slots.firstValue().length() != 1)
	  { System.out
	    .println(slot_name + " does not uniquely identify a slot.  " +
		     "Matches: " + slots.firstValue()); 
	    slot = null;
	    slot_name = (OKBCString)slots.nthValue(1);
	  }
	else { slot = slots.firstValue().car(); }

      }
    else { System.out.println("You must supply both a frame and a slot!"); }
  }

  Object demo_get_slot_values (Kb kb) {
    Values3 res = null;
    Object res_string = null;
    if (frame != null && slot != null)
      { 
	res = kb.get_slot_values (frame, slot,
				  Kb._all_inferable, // inference_level
				  Kb._own, // slot-type
				  Kb._all, // #values
				  Kb._known_true, // value_selector
				  Kb._NIL); // kb_local_only_p
	res_string = kb.value_as_string 
	  (res.firstValue(),
	   Kb._user_interface, // purpose
	   Kb._T,		  // pretty-p
	   Kb._NIL);           // kb_local_only_p
      }
    else { System.out.println("You must supply both a frame and a slot!"); }
    if (verbose_p)
      { System.out.println("res_string = " + res_string);
	Cons.pretty_print(res.firstValue());
	System.out.println();
	System.out.print("value_as_string(res_string) = ");
	System.out.print(kb.value_as_string
			 (res.firstValue(),
			  Kb._user_interface, // purpose
			  Kb._T,              // pretty-p
			  Kb._NIL));          // kb-local-only-p
	System.out.println();
      }
    return res_string;
  }

  void test_copy_of_kb (Kb kb) {
    test_copy_of_kb(kb, verbose_p, false);
  }

  void test_copy_of_kb 
  (Kb kb, boolean verbose, boolean recursive_call_p) {
    Values3 res = null;
    Object res_string = null;
    if (verbose)
      { System.out.println("Testing copy of frame ontology"); }
    if (frame != null && slot != null)
      { 
	res = kb.get_slot_values (frame, slot,
				  Kb._all_inferable, // inference_level
				  Kb._own, // slot-type
				  Kb._all, // #values
				  Kb._known_true, // value_selector
				  Kb._NIL); // kb_local_only_p
	res_string = kb.value_as_string 
	  (res.firstValue(),
	   Kb._user_interface, // purpose
	   Kb._T,		  // pretty-p
	   Kb._NIL);           // kb_local_only_p
      }
    else { System.out.println("You must supply both a frame and a slot!"); }
    if (verbose)
      { System.out.println("res_string = " + res_string);
	Cons.pretty_print(res.firstValue());
	System.out.println();
	System.out.print("value_as_string(res_string) = ");
	System.out.print(kb.value_as_string
			 (res.firstValue(),
			  Kb._user_interface, // purpose
			  Kb._T,              // pretty-p
			  Kb._NIL));          // kb-local-only-p
	System.out.println();
      }
    if (!recursive_call_p)
      { if (verbose)
	  { System.out.println("Testing cache in copy of frame ontology.");
	    System.out.println("Nothing should be printed out before the");
	    System.out.println("next line beginning with ****");
	  }
	test_copy_of_kb(kb, false, true);
	if (verbose) { System.out.println("**** End of cache test"); }
      }
  }

  void test_condition_system (Kb kb) {
	 if (verbose_p)
	   { System.out.println("Testing Condition system"); }
    try { kb.coerce_to_frame (kb.intern("DOESNT-EXIST"),
			      Kb._T,		// error-p
			      Kb._NIL);      // kb_local_only_p
	}
    catch (NotCoercibleToFrame condition)
      { if (verbose_p)
	  { System.out.println("Correct condition found: " + condition); }
      }
  }

  void test_procedure (Kb kb) {
    if (verbose_p)
      { System.out.println("Test fun-spec map op for " + kb); }
    Procedure funspec
      = Procedure.intern_procedure
       ("(defn)",
	"  (let ((pretty-name (get-frame-pretty-name defn :kb kb))\n" +
        "        (superclasses (if (class-p defn :kb kb)\n" +
        "                          (get-class-superclasses defn \n" +
        "                              :inference-level :direct :kb kb)\n" +
        "                          nil)))\n" +
	"      (if (class-p defn :kb kb)\n" +
	"	   (list pretty-name superclasses)\n" +
	"	   pretty-name))");
    if (verbose_p)
      { System.out.println("Procedure is \n" + funspec); }
    Node results = 
      kb.call_procedure
      (funspec,Cons.list(kb.get_kb_roots(Kb._all, Kb._NIL).car()));
    if (verbose_p)
      { System.out.println("Fun-spec result for " + kb + " = ");
	Cons.pretty_print(results);
      }
  }

  Values2 get_target_kb () {
    Kb kb = null;
    Node kb_res;
    Node created_p = Kb._NIL;
    Symbol fo_name = Symbol.intern("LOCAL-KB", "TEST");
    if (verbose_p) { System.out.println("get_target_kb"); }
    Kb proto = Node.get_kb_type(Symbol.intern("DEMO-TUPLE-KB", "OK-BACK"));
    if (verbose_p) { System.out.println("Kb_Type is: " + proto); }
    kb_res = Node.find_kb_of_type
	(fo_name, proto, LocalConnection.local_connection());
    if (verbose_p) { System.out.println("kb_res is: " + kb_res); }
    if (kb_res == null || kb_res == Kb._NIL)
      { 	  	
	if (verbose_p)
	  { System.out.println("kb_res is null, so make a new target kb."); }
	 kb = proto.create_kb (fo_name, Node._NIL, Node._NIL,
			       LocalConnection.local_connection());
	if (verbose_p) { System.out.println("Target kb is: " + kb); }
	created_p = Kb._T;
      }
    else { 
      if (verbose_p)
	{ System.out.println("kb_res is a kb: " + kb_res); }
      kb = (Kb)kb_res; 
    }
    return new Values2 (kb, created_p);
  }

  void run_demo(Symbol kb_name, String kb_type) {
    // This is the kb_type of a kb.
    Kb proto = Node.get_kb_type(Symbol.intern(kb_type, "OK-BACK"),
				okbcApp.connection());
    // Now try to find the KB on the server.  It really should 
    // be there!
    Node result_of_find_kb = Node.find_kb_of_type
      (kb_name, proto, okbcApp.connection());
    // Oops, the KB wan't there.  Something terrible must 
    // have happened.
    if (result_of_find_kb == null || result_of_find_kb == Kb._NIL)
      { throw new RuntimeException ("Couldn't find KB " + kb_name); }
    // Cast the KB into a OKBC KB, now that we know we've got it.
    Kb kb = (Kb)result_of_find_kb;
    // Debug print.
    System.out.println("KB is " + kb);
    // Now prompt for the frame and slot names.
    frame_name = OKBCApp.prompt_and_read_frame_name("Frame", frame_name);
    slot_name  = OKBCApp.prompt_and_read_frame_name("Slot",  slot_name);
    // Given the frame and slot names, try to find unambiguous frame objects
    // for these names.
    get_frame_and_slot(kb);
    // If we've got a frame and a slot, go and get the slot values and 
    // print them.
    Object res_string = null;
    if (frame != null && slot != null) 
      {
	res_string = demo_get_slot_values(kb);
	test_condition_system(kb);
	test_procedure (kb);
	if (kb.connection() instanceof KslNetworkConnection)
	  { if (verbose_p)
	      { System.out.println("Making connection persistent"); }
	    kb.set_connection_persistence(true); 
	  }
	if (verbose_p)
	  { System.out.println("Returning: " + res_string); }
      }
    if (verbose_p) { System.out.println("Exiting getkbResults"); }
    // The following test copies the whole of the remote kb into a local
    // DemoTupleKb.
    // This is only done the first time around this loop.
    Values2 res = get_target_kb ();
    Kb target_kb = (Kb)res.firstValue();
    if (res.nthValue(1) != Kb._NIL)
      { if (verbose_p)
	  { System.out.println("Copying Remote kb into TupleKb"); }
	// If we're copying with kb-local-only-p as true, we need to be in
	// non-error-p mode because of included frames.
	TruthValue just_kb_p = Kb._T;
	TruthValue error_p = (just_kb_p == Kb._T 
			      ? (TruthValue)Kb._NIL
			      : (TruthValue)Kb._T);
	DemoTupleKb.trace_cache_misses_p = false;
	Kb.copy_kb(kb, target_kb, error_p, Kb._allocate, just_kb_p);
	DemoTupleKb.trace_cache_misses_p = true;
      }
    if (verbose_p) { System.out.println("Target_kb = " + target_kb); }
    test_copy_of_kb(target_kb);
  }

  void run_demo_against_ontolingua() {
    // This is the name of the Frame ontology.
    Symbol kb_name = Symbol.intern("FRAME-ONTOLOGY", "ONTOLINGUA");
    String kb_type = "ABSTRACT-ONTOLOGY-KB";
    frame_name = new OKBCString("THING");
    slot_name = new OKBCString("SUPERCLASS-OF");
    run_demo(kb_name, kb_type);
  }

  void run_demo_against_loom() {
    // This is the name of the Naval kb.
    Symbol kb_name = Symbol.intern("NAVAL-THEORY", "USER");
    String kb_type = "ABSTRACT-LOOM-KB-KB";
    frame_name = new OKBCString("ACTIVE-SENSOR");
    slot_name = new OKBCString("USED-BY");
    run_demo(kb_name, kb_type);
  }

  public static void main (String argv[]) {
    if (OKBCApp.argv_suppliedp("HELP", argv))
      { System.out.println("Usage: java demos.Demo [-option]*");
	System.out.println("where options include:");
	System.out.println("    -okbchost <hostname>: the host to connect to");
	System.out.println("    -port <port number> : the port to connect to");
	System.out.println("    -user_id <id>       : your KSL user id");
	System.out.println("    -password <pwd>     : your KSL password");
	System.out.println("    -trace-procedures   : trace procedures");
	System.out.println("    -ontolingua         : use frame ontology");
	System.out.println("    -loom               : loom naval kb");
      }
    else
      { Demo d = new Demo();
	TupleKb.ensure_registered();
	DemoTupleKb.ensure_registered();
	boolean ontolingua_p = true;
	if (OKBCApp.argv_suppliedp("ONTOLINGUA", argv))
	  { ontolingua_p = true; }
	else if (OKBCApp.argv_suppliedp("LOOM", argv))
	  { ontolingua_p = false; }
	else { throw new RuntimeException("Illegal arguments.  " +
					  "Must supply Ontolingua or Loom.  " +
					  "Try -help."); }
	if (OKBCApp.argv_suppliedp("TRACE-PROCEDURES", argv))
	  { Evaluator.getOkbcEvaluator().trace_p = true; }
	d.okbcApp.host = "ontolingua.stanford.edu";
	d.okbcApp.port = 5915;
	d.okbcApp.initialize_from_argv(argv);
	if (ontolingua_p) { d.okbcApp.connect_to_a_session(); }
	d.okbcApp.echo_parameters();
	if (ontolingua_p) { d.okbcApp.establish_ksl_connection(); }
	else { d.okbcApp.establish_simple_connection(); }
	while (true)
	  { if (catch_errors_p)
		{ try 
		  {
		    if (ontolingua_p) { d.run_demo_against_ontolingua(); }
		    else { d.run_demo_against_loom(); }
		  }
		  catch (Exception exception)
		    { System.out.println("Got exception = " + exception); }
		}
	      else
		{ 
		  if (ontolingua_p) { d.run_demo_against_ontolingua(); }
		  else { d.run_demo_against_loom(); }
		}
	    }
      }
  }

}

