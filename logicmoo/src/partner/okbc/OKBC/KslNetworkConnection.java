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


public class KslNetworkConnection extends NetworkConnection {


  public static KslNetworkConnection kslNetworkConnection = 
    new KslNetworkConnection();

  KslNetworkConnection () {
    ensure_registered(this);
  }
  

  public static void ensure_registered () {
    if (kslNetworkConnection == null)
      { kslNetworkConnection = new KslNetworkConnection(); }
    ensure_registered(kslNetworkConnection);
  }
  

  static void ensure_registered (KslNetworkConnection conn) {
    kslNetworkConnection = conn;
    Node.register_connection_prototype
      (Symbol.intern("KSL-NETWORK-CONNECTION", "KSL-OKBC"), conn);
  }
  

  public Cons make_connection_key () {
    return (new Cons(host, 
	    new Cons(port, 
            new Cons(connection_method,
            new Cons(user_id,
            new Cons(password,
            new Cons(session_id,
            new Cons(session_key,
	    new Cons(kb_library, "KslNetworkConnection")))))))));
  }

  public static Cons make_connection_key_from_parts
  (String hst, int prt, Symbol conn_meth, String usr_id, String passwd,
   String sess_id, String sess_key, String lib) {
    return (new Cons(hst, 
	    new Cons(prt, 
            new Cons(conn_meth,
            new Cons(usr_id,
            new Cons(passwd,
            new Cons(sess_id,
            new Cons(sess_key,
	    new Cons(lib, "KslNetworkConnection")))))))));
  }

  public static NetworkConnection establish_anonymous_connection
  (String hst, int prt) {
    return establish_NetworkConnection_connection
      (hst, prt, _ephemeral, "ALIEN", "", "ANONYMOUS", "", 
       default_network_okbc_kb_library);
  }

  static final Node _user_id = Symbol.keyword("USER-ID");
  static final Node _password = Symbol.keyword("PASSWORD");
  static final Node _session_id = Symbol.keyword("SESSION-ID");
  static final Node _key = Symbol.keyword("KEY");
  static final Node _kb_library = Symbol.keyword("KB-LIBRARY");

  public Connection establish (Node initargs) {
    String hst = as_string(Cons.getf(initargs, _host));
    int prt = (int)(((OKBCInteger)Cons.getf(initargs, _port)).number);
    Symbol conn_meth = _ephemeral;
    String usr_id = as_string(Cons.getf(initargs, _user_id));
    String passwd = as_string(Cons.getf(initargs, _password));
    String sess_id = as_string(Cons.getf(initargs, _session_id));
    String sess_key = as_string(Cons.getf(initargs, _key));
    String lib = as_string(Cons.getf(initargs, _kb_library));
    return establish_NetworkConnection_connection
      (hst, prt, conn_meth, usr_id, passwd, sess_id, sess_key, lib);
  }


  public static NetworkConnection establish_NetworkConnection_connection
  (String hst, int prt, Symbol conn_meth, String usr_id, String passwd,
   String sess_id, String sess_key, String lib) {
    Cons hash_key = make_connection_key_from_parts
      (hst, prt, conn_meth, usr_id, passwd,
       sess_id, sess_key, lib);
    Connection existing = find_connection(hash_key);
    if (existing == null)
      { KslNetworkConnection conn = new KslNetworkConnection();
	conn.host = hst;
	conn.port = prt;
	conn.connection_method = conn_meth;
	conn.user_id = usr_id;
	conn.password = passwd;
	conn.session_id = sess_id;
	conn.session_key = sess_key;
	conn.kb_library = lib;
	conn.open_p = true;
	existing_Connections.put(hash_key, conn);
	return conn;
      }
    else { return (NetworkConnection)existing; }
  }

  Socket emit_connection_establishing_request() {
    // System.out.println("Making connection persistent- doing okbcSend");
    return okbcSend(Cons.list(_connect));
  }


}

