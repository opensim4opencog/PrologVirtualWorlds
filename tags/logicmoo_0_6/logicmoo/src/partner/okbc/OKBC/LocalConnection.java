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

public class LocalConnection extends Connection {

/**
 * Use {@link #local_connection()} instead.
 * @deprecated
 */
public static LocalConnection localConnection = 
  new LocalConnection();

//--------------------
//Private members:
//--------------------

  /**
   * A hashtable of LocalConnections.
   * A hashtable of LocalConnections, in which the keys are ThreadGroups
   * and the values are LocalConnections. Essentially allows each thread
   * to have its own globally accessible LocalConnection.
   * The {@link #local_connection()} method uses this
   * variable to return a LocalConnection associated with the current thread.
   * @see #local_connection()
   */
  private static Hashtable localConnectionTable = new Hashtable(10);

  //--------------------
  //Constructors:
  //--------------------
  LocalConnection () {
    ensure_registered(this);
  }

  
  public static void ensure_registered () {
//    if (localConnection == null) { localConnection = new LocalConnection(); }
    ensure_registered((LocalConnection)local_connection());
  }

  static void ensure_registered (LocalConnection conn) {
//    localConnection = conn;
    Cons hash_key = make_connection_key_from_parts();
    conn.open_p = true;
    existing_Connections.put(hash_key, conn);
    Node.register_connection_prototype
      (Symbol.intern("LOCAL-CONNECTION", "OKBC"), conn);
  }
  

  static public Cons make_connection_key_from_parts () {
    return (new Cons("LocalConnection"));
  }

  public Cons make_connection_key () {
    return make_connection_key_from_parts();
  }

  /**
   * Returns the local Connection.
   * Uses localConnectionTable to return a Connection that is
   * valid for the
   * {@link java.lang.Thread#currentThread()}.
   * Replaces accessing of LocalConnection via the static field
   * localConnection.
   * @return {@link OKBC.Connection}
   */   
  public static Connection local_connection () {
    Thread thisThread = Thread.currentThread();
    ThreadGroup thisThreadGroup = thisThread.getThreadGroup();
    LocalConnection localConn =
      (LocalConnection)localConnectionTable.get(thisThreadGroup);

    if(localConn == null) {
      localConn = new LocalConnection();
      localConnectionTable.put(thisThreadGroup, localConn);
    }
    return localConn;
  }

  public Connection establish (Node initargs) {
    return local_connection();
  }

  public static void main (String arg[]) {
    System.out.println("Connection is " + local_connection());
  }
  

}
