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

public class TupleStoreTester extends Node {
  static Symbol __fail = Symbol.keyword("FAIL");
  static Symbol __subclass_of = Node.__subclass_of;
  static Symbol __instance_of = Node.__instance_of;
  static Symbol __template_slot_value = Node.__template_slot_value;
  static Symbol __template_facet_value = Node.__template_facet_value;
  static Symbol __default_template_slot_value 
    = Node.__default_template_slot_value;
  static Symbol __default_template_facet_value
    = Node.__default_template_facet_value;
  static Symbol __deleted = Symbol.intern("__DELETED", "FRAMES");

  public Node test (Node pattern, Node test_structure, Kb kb) {
    throw new RuntimeException ("Missing test method"); 
  }

  public boolean frames_match_p(Node a, Node b, Kb kb) {
    return (a == b ||
	    (a != null &&
	     (a.equals(b)
	      // Note:  If we allow this test we end up with an
	      // infinite recursion.  We may need to revisit this some time.
	      // I think the plan is that if you're made it this far into the
	      // tuple store enough remapping any uniquification has happened
	      // that eql_in_kb frame references should have been mapped into
	      // == frame references.
	      // ||
	      // (kb != null && (kb.eql_in_kb(a, b, _NIL) != _NIL))
	      )));
  }
  
}

