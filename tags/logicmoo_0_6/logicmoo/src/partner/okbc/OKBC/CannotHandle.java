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

public class CannotHandle extends AbstractError {
  public Node sentence = null;

  public CannotHandle () {}

  public CannotHandle (Node the_sentence) {
    sentence = the_sentence;
  }

  public CannotHandle (Node the_sentence,
                       Node the_continuable,
                       Node the_error_message) {
    sentence = the_sentence;
    continuable = the_continuable;
    error_message = the_error_message;
  }

  public void init_from_plist (Node plist) {
    sentence = 
      plistGet(Symbol.keyword("SENTENCE"), plist);
    super.init_from_plist(plist);
  }

  public Node decode_to_plist() {
    return Cons.append_cons(Cons.list(Symbol.keyword("SENTENCE"),
                                      (sentence == null ?
                                       Node._NIL :
                                       sentence)),
                            super.decode_to_plist());
  }

  public String report () {
    return "Cannot handle sentence [" +
           sentence +
           "]";
  }
}