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

public class MissingFrames extends AbstractError {
  public Node missing_frames = null;
  public Node frame = null;

  public MissingFrames () {}

  public MissingFrames (Node the_missing_frames,
                        Node the_frame,
                        Node the_kb) {
    missing_frames = the_missing_frames;
    frame = the_frame;
    kb = the_kb;
  }

  public MissingFrames (Node the_missing_frames,
                        Node the_frame,
                        Node the_kb,
                        Node the_continuable,
                        Node the_error_message) {
    missing_frames = the_missing_frames;
    frame = the_frame;
    kb = the_kb;
    continuable = the_continuable;
    error_message = the_error_message;
  }

  public void init_from_plist (Node plist) {
    missing_frames = 
      plistGet(Symbol.keyword("MISSING-FRAMES"), plist);
    frame = 
      plistGet(Symbol.keyword("FRAME"), plist);
    kb = 
      plistGet(Symbol.keyword("KB"), plist);
    super.init_from_plist(plist);
  }

  public Node decode_to_plist() {
    return Cons.append_cons(Cons.list(Symbol.keyword("MISSING-FRAMES"),
                                      (missing_frames == null ?
                                       Node._NIL :
                                       missing_frames),
                                      Symbol.keyword("FRAME"),
                                      (frame == null ?
                                       Node._NIL :
                                       frame),
                                      Symbol.keyword("KB"),
                                      (kb == null ?
                                       Node._NIL :
                                       kb)),
                            super.decode_to_plist());
  }

  public String report () {
    return "Missing frames whilst copying [" +
           frame +
           "]";
  }
}