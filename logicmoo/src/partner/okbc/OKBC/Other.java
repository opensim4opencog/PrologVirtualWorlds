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

import java.net.Socket;
import java.net.URLEncoder;
import java.net.UnknownHostException;

public class Other extends Node {
  // used for error conditions
  static final int typeCode =  OTHER;
  Object contents;
  protected void encode(PrintWriter s, boolean client_p) {
    String string = contents.toString();
    s.print(typeCode);
    newline(s);
    s.print(string.length());
    newline(s);
    s.print(string);
    newline(s);
  }
  public String toString() {
    return "#<NodeOther" + contents.toString()
                         + " " + pointerForToString() + ">";
  }
}

  
