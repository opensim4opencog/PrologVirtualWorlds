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

public class Demo3NetworkKb extends NetworkKb {

  static Symbol demo3_tuple_kb_abstract_name = null;
  static Demo3NetworkKb demo3_tuple_kb_type = null;

  public Demo3NetworkKb () { }

  static final Symbol get_frame_details_fname =
         Symbol.intern_in_ok_back("GET-FRAME-DETAILS-INTERNAL");

  public static void ensure_registered () {
    if (demo3_tuple_kb_abstract_name == null)
      { demo3_tuple_kb_abstract_name = (Symbol)(register_concrete_kb_type
	  (Symbol.intern_in_ok_back("ABSTRACT-DEMO3-TUPLE-KB-KB"),
	   get_kb_type_internal()));
      }
  }

  public static Kb get_kb_type_internal () {
    if (demo3_tuple_kb_type == null)
      { Demo3NetworkKb kb = new Demo3NetworkKb();
	kb.kb_type_name = Symbol.intern("DEMO3-TUPLE-KB", "OK-BACK");
	demo3_tuple_kb_type = kb;
	return kb;
      }
    else { return demo3_tuple_kb_type; }
  }

  public Values2 cache_miss_hook (Node function, Node key) {
    System.out.println("Cache miss on " + key);
    return nil_t_values;
  }

  public void cache_fill_hook (Node function, Node key, Object new_results,
                        boolean multiple_values_p) {
    System.out.println("Cache fill for " + key + " -> " + new_results);
    if (function == get_frame_details_fname)
      { get_frame_details_cache_fill_hook(key, (Values2)new_results); }
  }

  public void encache (Object values, Node key) {
    if (allow_caching_p)
      {
	System.out.println("Encache fill for " + key + " -> " + values);
	super.encache(values, key);
      }
  }
}

