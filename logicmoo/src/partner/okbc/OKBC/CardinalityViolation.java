/***********************************************************************

  OKBC Implemementation for Java

  Copyright (c) 1996, 1997 Stanford University KSL.

  Original code by James Rice (rice@ksl.stanford.edu)


  This code was automatically generated on Tuesday, 21 December 1999.

  KSL Network Services available at:
            http://ontolingua.stanford.edu/


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

public class CardinalityViolation extends ConstraintViolation {

  public CardinalityViolation () {}

  public CardinalityViolation (Node the_kb,
                               Node the_facet,
                               Node the_slot_type,
                               Node the_slot,
                               Node the_frame,
                               Node the_constraint) {
    kb = the_kb;
    facet = the_facet;
    slot_type = the_slot_type;
    slot = the_slot;
    frame = the_frame;
    constraint = the_constraint;
  }

  public CardinalityViolation (Node the_kb,
                               Node the_facet,
                               Node the_slot_type,
                               Node the_slot,
                               Node the_frame,
                               Node the_constraint,
                               Node the_continuable,
                               Node the_error_message) {
    kb = the_kb;
    facet = the_facet;
    slot_type = the_slot_type;
    slot = the_slot;
    frame = the_frame;
    constraint = the_constraint;
    continuable = the_continuable;
    error_message = the_error_message;
  }

  public void init_from_plist (Node plist) {
    super.init_from_plist(plist);
  }

  public Node decode_to_plist() {
    return Cons.append_cons(Cons.list(),
                            super.decode_to_plist());
  }

  public String report () {
    return "Violation of cardinality constraint " +
           constraint +
           " in slot " +
           slot +
           " in frame " +
           frame +
           " in " +
           kb +
           (constraint!=null? ".  " + constraint:"");
  }
}