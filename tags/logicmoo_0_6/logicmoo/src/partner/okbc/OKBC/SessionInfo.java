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

public class SessionInfo extends Node {
// We extend Node so that Connection can be a node and the right thing will
// happen in the evaluator for expressions returning connections.
  static String default_network_okbc_kb_library = "A";
  public String host;
  public int    port;
  public String user_id = "ALIEN";
  public String password = "";
  public String session_id = "*ANONYMOUS";
  public String session_key = "";
  public String kb_library = default_network_okbc_kb_library;


  static final Symbol _query_format = Symbol.keyword("QUERY-FORMAT");
  static final Symbol _reply_format = Symbol.keyword("REPLY-FORMAT");
  static final Symbol _transport_version = Symbol.keyword("TRANSPORT-VERSION");
  static final Symbol _kb_library = Symbol.keyword("KB-LIBRARY");


}
