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

public class Demo3 {

  OKBCString frame_name;
  OKBCString slot_name;
  Node frame = null;
  Node slot = null;
  OKBCApp okbcApp = new OKBCApp();

  // Takes the string representations of the frame and slot names and tries
  // to find unique frames named by those strings.  Barfs if we don't get
  // a single frame.
  void get_frame_and_slot (Kb kb) {
    if (frame_name.string.length() > 0 && slot_name.string.length() > 0)
      { 
	Values frames = kb.get_frames_matching
	  (frame_name, Kb._NIL, Kb._all, Kb._NIL, Kb._NIL);
	// The first value in "frames" is a list of the matching frames.
	if (frames.firstValue().length() != 1)
	  // We expect this to have length 1.  0 would mean no matching frames,
	  // and >1 would mean we're not unique.
	  { System.out.println(frame_name + 
			       " does not uniquely identify a frame.  " +
			       "Matches: " + frames.firstValue()); 
	    // We're barfing out, so null out the frame, and reset the
	    // frame_name string to be the longest unique substring that 
	    // matches frames.
	    frame = null;
	    frame_name = (OKBCString)frames.nthValue(1);
	  }
	else { frame = frames.firstValue().car(); }

	// Do exactly the same for the slot name that we did for the frame name
	Values slots = kb.get_frames_matching
	  (slot_name, Kb._NIL, Kb._all, Kb._NIL, 
	   Kb._NIL);
	if (slots.firstValue().length() != 1)
	  { System.out.println(slot_name +
			       " does not uniquely identify a slot.  " +
			       "Matches: " + slots.firstValue()); 
	    slot = null;
	    slot_name = (OKBCString)slots.nthValue(1);
	  }
	else { slot = slots.firstValue().car(); }

      }
    else { System.out.println("You must supply both a frame and a slot!"); }
  }

  Object demo3_get_slot_values (CachingKb kb) {
    // the instance variables frame and slot should now contain frame handles
    // for frames and slots on the server side.
    Values3 res = null;
    Object res_string = null;
    if (frame != null && slot != null)
      // If we don't actually have a frame and a slot, then barf out
      // (else clause)
      { 
	// This will cause a cache miss first time
	kb.get_frame_details (frame,
			      Kb._all_inferable, // inference_level
			      Kb._all, // number-of-values
			      Kb._NIL); // kb-local-only-p
	// Actually get the slot values for the slot in the frame.
	// This should already be encached.
	System.out.println("cached_p_get_slot_values = " + 
			   kb.cached_p_get_slot_values 
			   (frame, slot,
			    Kb._all_inferable, // inference_level
			    Kb._own, // slot-type
			    Kb._all, // number_of_values
			    Kb._known_true, // value_selector
			    Kb._NIL) + // kb_local_only_p
			   ".  This should be true.");
	res = kb.get_slot_values (frame, slot,
				  Kb._all_inferable, // inference_level
				  Kb._own, // slot-type
				  Kb._all, // number_of_values
				  Kb._known_true, // value_selector
				  Kb._NIL); // kb_local_only_p
	// The result of calling get_slot_values is a Values instance
	// with two values:
	// - the list of values
	// - more status
	// res.firstValue() is therefore the list of slot values themselves.
	// Debug print out the values.
	System.out.print("res.firstValue() = ");
	Cons.pretty_print(res.firstValue());
	System.out.println();
	// The values may well be non-trivial things like FrameHandles.
	// For example, we might get back something like:
	// ([FrameHandle ONTOLINGUA::FRAME-ONTOLOGY:82416488])
	// These don't print out very nicely, so let's ask the OKBC server
	// to turn the values into a string that we can print out.
	res_string = kb.value_as_string 
	  (res.firstValue(),
	   Kb._T,		// error-p
	   Kb._user_interface, // purpose
	   Kb._T);		  // pretty-p
	// res_string is now the printed representation of the values list.
	// The particular string you get will depend on the actual slot
	// values, but is the slot values list was the list with one frame
	// handle like in the example above, the value of res_string would
	// be something like "(Individual-Thing)",
	// where Individual-Thing is the value of the superclass-of
	// slot in the frame called thing.
	System.out.println("res_string = \"" + res_string + "\"");
	System.out.print("value_as_string(res_string) = ");
	System.out.print(kb.value_as_string
			 (res.firstValue(), Kb._user_interface, 
			  Kb._T, Kb._NIL));
	System.out.println();
      }
    else { System.out.println("You must supply both a frame and a slot!"); }
    return res_string;
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
    Demo3NetworkKb kb = (Demo3NetworkKb)result_of_find_kb;
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
    if (frame != null && slot != null) 
      { demo3_get_slot_values(kb); }
  }

  void run_demo_against_ontolingua() {
    // This is the name of the Frame ontology.
    Symbol kb_name = Symbol.intern("FRAME-ONTOLOGY", "ONTOLINGUA");
    String kb_type = "ABSTRACT-ONTOLOGY-KB";
    frame_name = new OKBCString("THING");
    slot_name = new OKBCString("SUPERCLASS-OF");
    run_demo(kb_name, kb_type);
  }

  void run_demo_against_taxa_kb() {
    // This is the name of the Frame ontology.
    Symbol kb_name = Symbol.intern("TAXA", "OKBC-TEST");
    String kb_type = "ABSTRACT-TUPLE-KB-KB";
    frame_name = new OKBCString("BILL");
    slot_name = new OKBCString("FOODS");
    run_demo(kb_name, kb_type);
  }

  public static void main (String argv[]) {
    if (OKBCApp.argv_suppliedp("HELP", argv))
      { System.out.println("Usage: java demos.Demo3 [-option]*");

	// System.out.println("System.out is " + System.out + " and " +
	//                    System.out.getClass().getName());
	
	System.out.println("where options include:");
	System.out.println("    -okbchost <hostname>: the host to connect to");
	System.out.println("    -port <port number> : the port to connect to");
	System.out.println("    -taxa       : run against the TAXA test kb");
	System.out.println("    -ontolingua : run against the frame ontology");
	System.out.println("                  If you pick Ontolingua:");
	System.out.println("    -user_id <id>       : your KSL user id");
	System.out.println("    -password <pwd>     : your KSL password");
      }
    else
      { boolean ontolingua_p = true;
	Demo3NetworkKb.ensure_registered();
	if (OKBCApp.argv_suppliedp("ONTOLINGUA", argv))
	  { ontolingua_p = true; }
	else if (OKBCApp.argv_suppliedp("TAXA", argv))
	  { ontolingua_p = false; }
	else { throw new RuntimeException("Illegal arguments.  Try -help."); }
	Demo3 d = new Demo3();
	// Set default host and port
	d.okbcApp.host = "ontolingua.stanford.edu";
	d.okbcApp.port = 5915;
	// Get host, port user_id, etc. from argv if supplied.
	d.okbcApp.initialize_from_argv(argv);
	// Find or create a session.
	if (ontolingua_p) { d.okbcApp.connect_to_a_session(); }
	d.okbcApp.echo_parameters();
	// Now establish the connection given these parameters
	if (ontolingua_p) { d.okbcApp.establish_ksl_connection(); }
	else { d.okbcApp.establish_simple_connection(); }
	// Install our demo3 kb type in the network hook.
	NetworkGeneral.prototype_of_network_kbs = 
	  (Demo3NetworkKb)(Demo3NetworkKb.get_kb_type_internal());
	// We are now connected, so run the demo.
	while (true) 
	  { if (ontolingua_p)
	      { d.run_demo_against_ontolingua(); }
	    else { d.run_demo_against_taxa_kb(); }
	  }
      }
  }

}
