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

public class NetworkConnection extends NetworkGeneral {

  static final Node _host = Symbol.keyword("HOST");
  static final Node _port = Symbol.keyword("PORT");
  Symbol connection_method = _ephemeral;
  Socket persistent_socket = null;
  PushableBufferedReader persistent_read_stream = null;
  PrintWriter persistent_write_stream = null;
  int request_tag = 0;

  public Cons make_connection_key () {
    throw new RuntimeException
                ("Connection key method not defined for " + this);
  }

  public boolean has_persistent_connection() {
//  System.out.println("Connection " + this + " is " + connection_method + ":"
//		       + (connection_method != _ephemeral));
    return (connection_method != _ephemeral);
  }

  Socket emit_connection_establishing_request() {
    throw new RuntimeException
                ("Connection establishing method not implemented for " + this);
  }

  public void make_connection_persistent () {
    if (!has_persistent_connection())
      {				// not currently persistent, so make it so.
	persistent_socket = emit_connection_establishing_request();
	// System.out.println("Making connection persistent- send performed");
	Values4 result = okbcReceiveValues(persistent_socket, null, true);
	// System.out.println("Making conn persistent- values: " + result);
	if (result.nthValue(3) != _ok)
	  { persistent_socket = null;
	    throw new RuntimeException
	      ("Cannot establish persistent connection to " + host + ":" + 
	       port);
	  }
	else { connection_method = _persistent; }
	// System.out.println("Connection is now persistent");
      }
    else
      { 
	// System.out.println("Connection is already persistent"); 
      }
  }

  public void make_connection_ephemeral () {
    if (has_persistent_connection())
      {				// currently persistent, so disconnect.
	try
	  { emit_network_call_to_persistent_stream 
	      (Cons.list(_disconnect), null);
	    persistent_socket.close();
	  }
	catch (Exception exception)
	  { }			// do nothing
	persistent_socket = null;
	persistent_read_stream = null;
	persistent_write_stream = null;
	connection_method = _ephemeral;
      }
  }

  public void set_connection_persistence (boolean new_state) {
    if (new_state)
      { make_connection_persistent(); }
    else { make_connection_ephemeral(); }
  }

  public static NetworkConnection establish_NetworkConnection_connection
  (String hst, int prt, Symbol conn_meth, String usr_id, String passwd,
   String sess_id, String sess_key, String lib) {
    throw new RuntimeException
      ("Establish Connection method not defined for NetworkConnection");
  }

  public void close_connection () {
    Enumeration e = Enumerator.getEnumeratorIdToEnumeratorTable().keys();
    while (e.hasMoreElements()) {
      Object key = e.nextElement();
      Enumerator this_e =
	(Enumerator)Enumerator.getEnumeratorIdToEnumeratorTable().get(key);
      // System.out.println("Close connection: Enumerator = " + this_e);
      NetworkEnumerator n = null;
      if (this_e instanceof NetworkEnumerator)
	{ n = (NetworkEnumerator)this_e; 
	  if (this == n.connection)
	    { try { n.free(); }
	      catch (Exception ex) {}
	      finally
		{ Enumerator.getEnumeratorIdToEnumeratorTable().remove(key);
		  // System.out.println("Removed enumerator " + this_e);
		}
	    }
	}
    }
    make_connection_ephemeral();
    super.close_connection();
  }

  void skip_until_we_hit_the_right_reply_tag() {
    // System.out.println("Skipping for tag line");
    String line = null;
    try { PushableBufferedReader in = ensure_read_stream();
	  do {
	    do
	      {
		line = in.readLine();
		// System.out.println("Skip line = " + line);
	      }
	    while (!(line.equals(request_cookie)));
	    line = in.readLine(); // now read the tag
	    // System.out.println("Skip tag = " + line);
	    // System.out.println("Looking for tag = " + request_tag);
	    int tag = Integer.parseInt(line);
	    if (tag == request_tag)
	      { break; }
	    else if (tag > request_tag)
	      { throw new IOException
		  ("Couldn't find reply tag " + request_tag + 
		   "  on reply stream.  We have already gone too far " + 
		   "some how.  Tag received was " + tag);
	      }
	    else {}		// do nothing.  Loop round again.
	  }
	  while (true);
	  // System.out.println("Tag line found = " + line);
	}
    catch (IOException e) { throw new RuntimeException(e.toString()); }
  }

  public Node emit_network_call_to_persistent_stream
  (Node args, Kb kb_type) {
    // This could be inside a handler-case for file-io-error
    // that reestablishes the connection when you geta broken pipe.
    Node mask = _NIL;
    PrintWriter out = ensure_write_stream();
    // last_request_cachable_p = false;
    out.print(request_cookie);
    out.print("\n");
    request_tag = request_tag + 1;
    out.print(request_tag);
    out.print("\n");
    mask.encode(out, true);
    if (Node.trace_network_interactions_p)
      { System.out.println("Sending network request:");
	Cons.pretty_print(args);
	System.out.println();
      }
    args.encode(out, true);
    out.flush();
    skip_until_we_hit_the_right_reply_tag();
    return (Node)okbcReceive(persistent_socket, kb_type, true);
  }

  public Node emitNetworkCall(Node args, Kb kb_type) {

       // System.out.println("Call = " + args);

     Socket sock = okbcSend(args);
     Node result = (Node)okbcReceive(sock, kb_type, true);
     try { sock.close(); }
     catch (IOException e) { throw new RuntimeException(e.toString()); }
     return result;
   }

  public Node makeNetworkCall(Node args) {
    Node result = null;
     if (has_persistent_connection())
       { result = emit_network_call_to_persistent_stream (args, null);
       }
     else { result = emitNetworkCall(args, null); }
    Node car = result.car();
    if (car instanceof Enumerator)
      { ((Enumerator)car).creating_args = args; }
    return result;
   }

  public Node makeNetworkCall(Node args, Kb kb_type) {
    Node result = null;
     if (has_persistent_connection())
       { result = emit_network_call_to_persistent_stream (args, kb_type);
       }
     else { result = emitNetworkCall(args, kb_type); }
    Node car = result.car();
    if (car instanceof Enumerator)
      { ((Enumerator)car).creating_args = args; }
    return result;
   }

  String lisp_to_java_class_name (String string) {
    int pos = string.indexOf("-");
    if (pos > 0)
      { return
	string.substring(0,1).toUpperCase() +
	string.substring(1,pos).toLowerCase() +
	lisp_to_java_class_name(string.substring(pos + 1));
      } 
    else if (pos == 0)
      { return lisp_to_java_class_name(string.substring(pos + 1)); }
    else { return string.substring(0,1).toUpperCase() +
	          string.substring(1).toLowerCase(); 
	 }
  }

  void maybe_handle_error (Node resultErrp, Node resultStatus) {
    if (! (resultErrp instanceof False)) 
      { 
	// System.out.println("OKBC Error signalled");
	// System.out.println("resultErrp = " + resultErrp);
	// System.out.println("resultStatus = " + resultStatus);

	if (resultStatus instanceof Cons)
	    { Cons list = (Cons)resultStatus;
	      Class condition_class = null;
	      Condition instance = null;
	      String class_name =
		"OKBC."+
		lisp_to_java_class_name (((Symbol)(list.nth(1))).symbolName);
	      // System.out.println("Try to cons error "+ class_name);
	      try
		{ condition_class = Class.forName(class_name);
		  // System.out.println("Class = "+ condition_class);
		  if (condition_class != null)
		    { instance = (Condition)condition_class.newInstance(); }
		  // System.out.println("Instance = "+ instance);
		}
	      catch (Exception exception)
		{ 
		  System.out.println("Error in determining OKBC error class = "
				     + exception);
		}
	      // System.out.println("Instance = "+ instance);
	      if (instance == null)
		{ throw new RuntimeException(resultStatus.toString()); }
	      else { instance.init_from_plist(list);
		     // System.out.println("Throwing Condition = "+ instance);
		     throw instance;
		   }
	    }
	  else 
	    {
	      // status is a OKBCString, so just signal a generic exception.
	      throw new RuntimeException(resultStatus.toString());
	    }
	}
  }

  public Node okbcReceive(Socket sock, Kb kb_type, boolean client_p) {
    // System.out.println("Start okbcReceive");
    PushableBufferedReader stream = ensure_read_stream(sock);
 
    Node resultList = null;
    Node resultMask = null;
    Node resultErrp = null;
    Node resultStatus = null;
    Node resultCachableP = null;
    Node resultMessages = null;

    try {

      resultList = decode(stream, kb_type, client_p, this);
      if (Node.trace_network_interactions_p)
	{ System.out.println("Received network result:");
	Cons.pretty_print(resultList);
	System.out.println();
	}
      
      // System.out.println("okbcReceive Results = " + resultList.toString());
      resultMask = decode(stream, kb_type, client_p, this);
      // System.out.println("okbcReceive Mask    = " + resultMask.toString());
      resultErrp = decode(stream, kb_type, client_p, this);
      // System.out.println("okbcReceive Errorp  = " + resultErrp.toString());
      resultStatus = decode(stream, kb_type, client_p, this);
      // System.out.println("okbcReceive Status  = " + resultStatus.toString());
      resultCachableP = decode(stream, kb_type, client_p, this);
      // System.out.println("okbcReceive Cachep  = "+resultCachableP.toString());
      resultMessages = decode(stream, kb_type, client_p, this);
      // System.out.println("okbcReceive Messages  = "+resultMessages.toString());

    /**
     * If we get an Exception when trying to read result, we may have an
     * embedded "REQUEST!" tag, indicating an embedded error packet.  
     * Try to read the embedded result.
     */
     } catch (Exception e) {
       System.err.println("okbcReceive:: exception e = \n");
       e.printStackTrace();
       skip_until_we_hit_the_right_reply_tag();
       System.out.println("okbcReceive:: skipped");
       return (Node)okbcReceive(sock, kb_type, true);
     }


    // Note that we are not using the returned coercion mask, which
    // means that we can create KB objects that are not EQ to
    // exising KBs.  Also, we create symbol and number objects that
    // are not EQ to existing ones.

    maybe_handle_error(resultErrp, resultStatus);

    // System.out.println("okbcReceive errors handled ");

    if (resultCachableP == _NIL)
      { if (ok_to_cache_flag == _testing)
	  { ok_to_cache_flag = _flush; }
	// else we aren't trying to cache so don't set the flag!
	else {}
      }

    // System.out.println("okbcReceive about to handle messages ");
    // System.out.println("Connection = " + connection);

    if (callback != null)
      { 
	// System.out.println("Callback = " + callback);
	callback.handleMessages(resultMessages); 
      }
    else { ConnectionCallback.defaultHandleMessages(resultMessages); }

    // System.out.println("okbcReceive messages handled. ");

    return resultList;
  }
  
  PushableBufferedReader ensure_read_stream () {
    return ensure_read_stream(persistent_socket);
  }

  PushableBufferedReader ensure_read_stream (Socket sock) {
    if (persistent_socket != null)
      { if (persistent_read_stream == null)
	  { try { persistent_read_stream = new PushableBufferedReader
		  (new InputStreamReader(sock.getInputStream()));
		}
	    catch (IOException e) { throw new RuntimeException(e.toString()); }
	  }
	return persistent_read_stream;
      }
    else 
	  { PushableBufferedReader stream = null;
	    try { stream = new PushableBufferedReader
		  (new InputStreamReader(sock.getInputStream()));
		}
	    catch (IOException e) { throw new RuntimeException(e.toString()); }
	    return stream;
	  }
  }

  PrintWriter ensure_write_stream () {
    return ensure_write_stream(persistent_socket);
  }

  PrintWriter ensure_write_stream (Socket sock) {
    if (persistent_socket != null)
      { if (persistent_write_stream == null)
	  { try {
	    persistent_write_stream =
	      new PrintWriter(sock.getOutputStream());
		}
	    catch (IOException e) { throw new RuntimeException(e.toString()); }
	  }
	return persistent_write_stream;
      }
    else { PrintWriter stream = null;
	   try { stream = new PrintWriter(sock.getOutputStream()); }
	   catch (IOException e) { throw new RuntimeException(e.toString()); }
	   return stream;
	 }
  }

  public Values4 okbcReceiveValues(Socket sock, Kb kb_type, 
				 boolean client_p) {
    PushableBufferedReader stream = ensure_read_stream(sock);
    Node resultList = decode(stream, kb_type, client_p, this);
    if (Node.trace_network_interactions_p)
      { System.out.println("Received network result:");
	Cons.pretty_print(resultList);
	System.out.println();
      }
    // System.out.println("okbcRecVal Results = " + resultList.toString());
    Node resultMask = decode(stream, kb_type, client_p, this);
    // System.out.println("okbcRecVal Mask    = " + resultMask.toString());
    Node resultErrp = decode(stream, kb_type, client_p, this);
    // System.out.println("okbcRecVal Errorp  = " + resultErrp.toString());
    Node resultStatus = decode(stream, kb_type, client_p, this);
    // System.out.println("okbcRecVal Status  = " + resultStatus.toString());
    Node resultCachableP = decode(stream, kb_type, client_p, this);
    // System.out.println("okbcRecVal Cachep  = " + resultCachableP.toString());
    Node resultMessages = decode(stream, kb_type, client_p, this);
    // System.out.println("okbcRecVal Messages  = "+resultMessages.toString());

    // Note that we are not using the returned coercion mask, which
    // means that we can create KB objects that are not EQ to
    // exising KBs.  Also, we create symbol and number objects that
    // are not EQ to existing ones.


    maybe_handle_error(resultErrp, resultStatus);

    if (resultCachableP == _NIL)
      { if (ok_to_cache_flag == _testing)
	  { ok_to_cache_flag = _flush; }
	// else we aren't trying to cache so don't set the flag!
	else {}
      }

    // System.out.println("Connection = " + connection);

    if (callback != null)
      { 
	// System.out.println("Callback = " + callback);
	callback.handleMessages(resultMessages); 
      }
    else { ConnectionCallback.defaultHandleMessages(resultMessages); }

    return new Values4(resultList, resultMask, resultErrp, resultStatus);
    // resultCachableP, resultMessages);
  }

  public Socket okbcSend(Node args) {
      Node mask = _NIL;
      // last_request_cachable_p = false;
      Socket s = null;
      try {
	s = new Socket(host,port); 
	PrintWriter out = ensure_write_stream(s);
	  
	ByteArrayOutputStream allBytes = new ByteArrayOutputStream();
	PrintWriter allOut = new PrintWriter(allBytes);
      
	ByteArrayOutputStream bytes = new ByteArrayOutputStream();
	PrintWriter stringStream = new PrintWriter(bytes);
	String string;
      
	allOut.print("GET /network-okbc" +
		     "&query-format=" + query_format + 
		     "&reply-format=" + reply_format +
		     "&transport-version=" + max_supported_transport_version +
		     "&kb-library=" + kb_library +
		     "&user-id=");
	allOut.print(this.user_id);
	if (session_key.length() == 0)
	  allOut.print("&sid=" + session_id);
	else
	  allOut.print( "&key=" + session_key );
	allOut.print("&args-to-find-kb-on=");
      
	mask.encode(stringStream, true);
	stringStream.flush();
	string = URLEncoder.encode(bytes.toString());

	// System.out.println("Mask = " + mask);
	// System.out.println("Bytes1 = " + bytes.toString());
	// System.out.println("String1 = " + string);
      
	allOut.print(string);
	allOut.print("&args=");
      
	bytes.reset();
	args.encode(stringStream, true);
	stringStream.flush();
	string = URLEncoder.encode(bytes.toString());
      
	// System.out.println("Args = " + args);
	// System.out.println("Bytes2 = " + bytes.toString());
	// System.out.println("String2 = " + string);

	allOut.print(string);
	allOut.print("\n\n");
	allOut.flush();
      
	//System.out.println("Complete output = " + allBytes.toString());
      
	out.print(allBytes.toString());
	out.flush();
      }
      catch (IOException e) { throw new RuntimeException(e.toString()); }
      return s;
    }

}
