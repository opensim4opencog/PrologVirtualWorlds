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

// We define this class only so that we can get a trace message out for
// every frame that is copied during the copy part of the demo.

public class DemoTupleKb extends TupleKb {

  static Symbol demo_tuple_kb_abstract_name = null;
  static DemoTupleKb demo_tuple_kb_type = null;

  public DemoTupleKb () {
    tuple_store = new TupleKbTupleStore(this, false);
    fact_store = tuple_store;
  }
  
  public static void ensure_registered () {
    if (demo_tuple_kb_abstract_name == null)
      { demo_tuple_kb_abstract_name = (Symbol)(register_concrete_kb_type
	  (Symbol.intern_in_ok_back("ABSTRACT-DEMO-TUPLE-KB-KB"),
	   get_kb_type_internal()));
      }
  }

  public static Kb get_kb_type_internal () {
    if (demo_tuple_kb_type == null)
      { DemoTupleKb kb = new DemoTupleKb();
	kb.kb_type_name = Symbol.intern("DEMO-TUPLE-KB", "OKBC-TEST");
	demo_tuple_kb_type = kb;
	return kb;
      }
    else { return demo_tuple_kb_type; }
  }

  static boolean trace_cache_misses_p = true;

  public Values2 cache_miss_hook (Node function, Node key) {
    if (trace_cache_misses_p) { System.out.println("Cache miss on " + key); }
    return nil_t_values;
  }

  public void cache_hit_hook (Node function, Node key, TrieResult trie_result)
  { 
    if (trace_cache_misses_p) { System.out.print("H"); }
  }

  protected Values2 copy_frame_forward_2
     (Node frame, Node new_name, Kb from_kb, TruthValue error_p,
      Node missing_frame_action, HashtableNode frame_handle_mapping_table,
      TruthValue kb_local_only_p) {
       System.out.println
	 ("Copying " + frame + " [" +
	  from_kb.get_frame_pretty_name(frame, kb_local_only_p) + "]");
       return copy_frame(frame, new_name, from_kb, this, error_p,
			 missing_frame_action, frame_handle_mapping_table,
			 kb_local_only_p);
     }


  protected Values3 get_slot_values_internal
    (Node frame, Node slot, InferenceLevel inference_level,
     SlotType slot_type, Node number_of_values, ValueSelector value_selector,
     TruthValue kb_local_only_p) {
    System.out.println("get_slot_values(" + frame + ", " + slot + ")");
    Values3 result = 
      super.get_slot_values_internal(frame, slot, inference_level, slot_type,
				     number_of_values, value_selector,
				     kb_local_only_p);
    System.out.println(" -> " + result);
    return result;
  }

  // Test harness.
  public static void main (String argv[]) {
    Symbol kb_name = Symbol.intern("MY-DEMO-TUPLE-KB", "OKBC-TEST");
    DemoTupleKb.ensure_registered();
    Connection localcon = LocalConnection.local_connection();
    Kb kb_type = get_kb_type(Symbol.intern("DEMO-TUPLE-KB", "OK-BACK"));
    DemoTupleKb kb = (DemoTupleKb)
      (kb_type.create_kb(kb_name, _NIL, _NIL, localcon));
    Listener.breakLoop(kb, argv);
  }

}

