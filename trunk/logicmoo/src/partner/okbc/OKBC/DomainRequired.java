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

public class DomainRequired extends AbstractError {
  public Node frame = null;
  public Node slot = null;
  public Node facet = null;

  public DomainRequired () {}

  public DomainRequired (Node the_frame,
                         Node the_slot,
                         Node the_facet,
                         Node the_kb) {
    frame = the_frame;
    slot = the_slot;
    facet = the_facet;
    kb = the_kb;
  }

  public DomainRequired (Node the_frame,
                         Node the_slot,
                         Node the_facet,
                         Node the_kb,
                         Node the_continuable,
                         Node the_error_message) {
    frame = the_frame;
    slot = the_slot;
    facet = the_facet;
    kb = the_kb;
    continuable = the_continuable;
    error_message = the_error_message;
  }

  public void init_from_plist (Node plist) {
    frame = 
      plistGet(Symbol.keyword("FRAME"), plist);
    slot = 
      plistGet(Symbol.keyword("SLOT"), plist);
    facet = 
      plistGet(Symbol.keyword("FACET"), plist);
    kb = 
      plistGet(Symbol.keyword("KB"), plist);
    super.init_from_plist(plist);
  }

  public Node decode_to_plist() {
    return Cons.append_cons(Cons.list(Symbol.keyword("FRAME"),
                                      (frame == null ?
                                       Node._NIL :
                                       frame),
                                      Symbol.keyword("SLOT"),
                                      (slot == null ?
                                       Node._NIL :
                                       slot),
                                      Symbol.keyword("FACET"),
                                      (facet == null ?
                                       Node._NIL :
                                       facet),
                                      Symbol.keyword("KB"),
                                      (kb == null ?
                                       Node._NIL :
                                       kb)),
                            super.decode_to_plist());
  }

  public String report () {
    return "Domain required when creating a slot or facet";
  }
}