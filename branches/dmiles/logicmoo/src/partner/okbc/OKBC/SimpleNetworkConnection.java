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


public class SimpleNetworkConnection extends NetworkConnection {


  public static SimpleNetworkConnection simpleNetworkConnection = 
    new SimpleNetworkConnection();

  SimpleNetworkConnection () {
    ensure_registered(this);
  }
  

  public static void ensure_registered () {
    if (simpleNetworkConnection == null)
      { simpleNetworkConnection = new SimpleNetworkConnection(); }
    ensure_registered(simpleNetworkConnection);
  }

  public static void ensure_registered (SimpleNetworkConnection conn) {
    simpleNetworkConnection = conn;
    Node.register_connection_prototype
      (Symbol.intern("SIMPLE-NETWORK-CONNECTION", "OK-BACK"), conn);
  }
  

  public Cons make_connection_key () {
    return (new Cons(host, 
	    new Cons(port, 
	    new Cons(kb_library, "SimpleNetworkConnection"))));
  }

  public static Cons make_connection_key_from_parts
  (String hst, int prt, String lib) {
    return (new Cons(hst, 
	    new Cons((long)prt, 
	    new Cons(lib, "SimpleNetworkConnection"))));
  }

  public Connection establish (Node initargs) {
    String hst = as_string(Cons.getf(initargs, _host));
    int prt = (int)(((OKBCInteger)Cons.getf(initargs, _port)).number);
    String lib = as_string(Cons.getf(initargs, _kb_library));
    return establish_NetworkConnection_connection (hst, prt, lib);
  }


  public static NetworkConnection establish_NetworkConnection_connection
  (String hst, int prt, String lib) {
    Cons hash_key = make_connection_key_from_parts (hst, prt, lib);
    Connection existing = find_connection(hash_key);
    if (existing == null)
      { SimpleNetworkConnection conn = new SimpleNetworkConnection();
	conn.host = hst;
	conn.port = prt;
	conn.kb_library = lib;
	conn.open_p = true;
	existing_Connections.put(hash_key, conn);
	conn.make_connection_persistent();
	return conn;
      }
    else { return (NetworkConnection)existing; }
  }

  Socket emit_connection_establishing_request() {
      Node mask = _NIL;
      Node initargs = Cons.list(_query_format, _portable,
				_reply_format, _portable,
				_transport_version, 
				Symbol.keyword
				(max_supported_transport_version),
				_kb_library,
				Symbol.keyword(kb_library));
      Socket s = null;
      try {
	s = new Socket(host,port); 
	PrintWriter out = ensure_write_stream(s);
	initargs.encode(out, true);
	newline(out);
	out.flush();
      }
      catch (IOException e) { throw new RuntimeException(e.toString()); }
      return s;
  }

}
