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

public class Connection extends SessionInfo {

  static Hashtable existing_Connections = new Hashtable ();
  public ConnectionCallback callback = 
    new ConnectionCallback(); // trivial default
  boolean open_p = false;

  public static final Symbol _yes = Symbol.keyword("YES");
  public static final Symbol _flush = Symbol.keyword("FLUSH");
  public static final Symbol _testing = Symbol.keyword("TESTING");
  Node ok_to_cache_flag = _yes;
  Vector side_effect_notifyees = new Vector ();

  public void register_interest_in_side_effects (Notifyee object) {
    if (!side_effect_notifyees.contains(object))
      { side_effect_notifyees.addElement(object); }
  }

  public void unregister_interest_in_side_effects (Notifyee object) {
      { side_effect_notifyees.removeElement(object); }
  }

  public void register_side_effect (Object arg) {
    register_side_effect();
  }

  public void register_side_effect () {
    Enumeration e = side_effect_notifyees.elements();
    for (; e.hasMoreElements() ;) {
      Notifyee obj = (Notifyee)(e.nextElement());
      obj.notice_side_effect();
    }
    if (ok_to_cache_flag == _testing)
      { ok_to_cache_flag = _flush; }
  }

  public boolean has_persistent_connection() {
    return false;
  }

  public void set_connection_persistence (boolean new_state) {
    throw new RuntimeException
      ("Cannot establish persistent connection for this connection type.");
  }

  public Connection establish (Node initargs) {
     throw new MethodMissing(new OKBCString("establish_connection"),
			     this).signal();
  }

  public Cons make_connection_key () {
    throw new RuntimeException ("Cannot compute connection key");
  }

  static String as_string (Node n) {
    if (n instanceof OKBCString)
      { return ((OKBCString)n).string; }
    else if (n == Node._NIL) { return ""; }
    else { return ((Symbol)n).symbolName; }
  }
  
  public static Connection find_connection (Cons key) {
    return (Connection)existing_Connections.get (key);
  }

  public void close_connection () {
    Cons hash_key = make_connection_key();
    Connection conn = Connection.find_connection (hash_key);
    conn.open_p = true;
    if (conn == this)
      { existing_Connections.remove (hash_key); }
    for(int i=0; i < all_OKBC_KBs.length; i++) {
      Kb thisKb = all_OKBC_KBs[i]; 
      if (thisKb instanceof NetworkKb
	  && (this == ((NetworkKb)thisKb).connection()))
	{ ((NetworkKb)thisKb).set_connection(null);
	}
    }
  }

  public static Enumeration all_connections_enum () {
    return existing_Connections.elements();
  }

  public static void close_all_connections () {
    Enumeration elements = all_connections_enum();
    while (elements.hasMoreElements()) {
      Connection conn = (Connection)elements.nextElement();
      conn.close_connection();
    }
  }
}
