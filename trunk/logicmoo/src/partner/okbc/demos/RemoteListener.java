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

class RemoteListener {

  OKBCApp okbcApp = new OKBCApp();

  public static void main (String argv[]) {
    if (OKBCApp.argv_suppliedp("HELP", argv))
      { System.out.println("Usage: java demos.RemoteListener [-option]*");
	System.out.println("where options include:");
	System.out.println("    -okbchost <hostname>: the host to connect to");
	System.out.println("    -port <port number> : the port to connect to");
	System.out.println("    -trace-procedures   : trace procedures");
	System.out.println("    -trace              : trace network calls");
	System.out.print  ("    -dont-catch-errors  : ");
	System.out.println("don't catch errors that occur during processing.");
	System.out.println("    -ksl                : use KSL connection " +
			                             "method");
	System.out.println("                  If you pick KSL:");
	System.out.println("    -user_id <id>       : your KSL user id");
	System.out.println("    -password <pwd>     : your KSL password");
      }
    else
      { boolean ksl_p = false;
	if (OKBCApp.argv_suppliedp("TRACE", argv)) 
	  { Node.trace_network_interactions_p = true; }
	if (OKBCApp.argv_suppliedp("KSL", argv)) { ksl_p = true; }
	RemoteListener listener = new RemoteListener();
	// Set default host and port
	listener.okbcApp.host = "ontolingua.stanford.edu";
	listener.okbcApp.port = 5915;
	// Get host, port user_id, etc. from argv if supplied.
	listener.okbcApp.initialize_from_argv(argv);
	// Find or create a session.
	if (ksl_p) { listener.okbcApp.connect_to_a_session(); }
	listener.okbcApp.echo_parameters();
	// Now establish the connection given these parameters
	if (ksl_p) { listener.okbcApp.establish_ksl_connection(); }
	else { listener.okbcApp.establish_simple_connection(); }
	// We are now connected, so run the demo.
	Connection connection = listener.okbcApp.connection();
	int count = 0;
	int choice = 0;
	OKBCString choice_string = null;
	OKBCString zero = new OKBCString("0");
	Kb meta_kb = (Kb)NetworkKb.meta_kb(connection);
	meta_kb.register_procedure(get_pretty_kb_types_proc_name,
				   getPrettyKbTypes);
	meta_kb.register_procedure(get_pretty_kbs_of_type_proc_name,
				   getPrettyKbsOfType);
	while (true) {
	  Node kb_type_specs
	    = meta_kb.call_procedure(get_pretty_kb_types_proc_name, Node._NIL);
	  System.out.println("Pick a KB type:");
	  count = 0;
	  Enumerator e = kb_type_specs.enumerate_list();
	  while (e.has_more_p()) {
	    Node spec = e.next();
	    Kb proto = (Kb)(spec.car());
	    Node frs_name = spec.cadr();
	    System.out.println(count + ": " + frs_name);
	    count = count + 1;
	  }
	  choice_string
	    = OKBCApp.prompt_and_read_frame_name("Choose:", zero);
	  choice = (new Integer(choice_string.string)).intValue();
	  Kb kb_type = (Kb)(kb_type_specs.nth(choice).car());
	  Node kb_specs = meta_kb.call_procedure(get_pretty_kbs_of_type_proc_name,
						 Cons.list(kb_type));
	  if (kb_specs.length() == 0)
	    { System.out.println("There are no KBs.  Please try again."); }
	  else if (kb_specs.length() == 1)
	    { System.out.print("There is only one kb of this type: ");
	      System.out.println(kb_specs.car().cadr());
	      Listener.breakLoop((Kb)(kb_specs.car().car()), argv);
	    }
	  else
	    {
	      e = kb_specs.enumerate_list();
	      System.out.println("Pick a KB:");
	      count = 0;
	      while (e.has_more_p()) {
		Node spec = e.next();
		Kb kb = (Kb)(spec.car());
		System.out.print(count + ": ");
		System.out.println(spec.cadr());
		count = count + 1;
	      }
	      choice_string
		= OKBCApp.prompt_and_read_frame_name("Choose:",zero);
	      choice = (new Integer(choice_string.string)).intValue();
	      Kb kb = (Kb)(kb_specs.nth(choice).car());
	      System.out.println("Entering listener.  " +
				 "Type (quit) to exit.");
	      Listener.breakLoop(kb, argv);
	    }
	}
      }
  }

  // Snarfed from the OKBCeditor
  static final Symbol get_pretty_kb_types_proc_name
         = Symbol.intern("RL-GET-PRETTY-KB-TYPES", "OKBC");
  
  static final Procedure getPrettyKbTypes = Procedure.intern_procedure
   (get_pretty_kb_types_proc_name,
    "()",
    "(let ((prototypes (get-kb-types)))" +
    "  (do-list (x prototypes)" +
    "    (list x (frs-name :kb-type x))))"
    );

  static final Symbol get_pretty_kbs_of_type_proc_name
         = Symbol.intern("RL-GET-PRETTY-KBS-OF-TYPE", "OKBC");

  public static final Procedure getPrettyKbsOfType = Procedure.intern_procedure
  (get_pretty_kbs_of_type_proc_name,
   "(type)",
   "(let ((kbs (get-kbs-of-type :kb-type type)))" +
   "  (do-list (x kbs)" +
   "    (list x (get-frame-pretty-name x :kb kb))))"
   );
  
}
