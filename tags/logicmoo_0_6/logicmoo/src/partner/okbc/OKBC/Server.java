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
import java.net.ServerSocket;
import java.net.Socket;

class DisconnectException extends Exception {
  DisconnectException() {}
}

class ConnectionProblem extends RuntimeException {
  ConnectionProblem (String s) { super(s); }
}

public class Server extends NetworkGeneral implements Notifyee {

  int port = -1;
  Node messages = _NIL;
  Node ok_to_cache_p = _NIL;
  Symbol default_kb_class = Symbol.intern_in_ok_back("ABSTRACT-TUPLE-KB-KB");

  public Server (int the_port) {
    if ((the_port < 1024) || the_port >= 65536) // What is the max socket #?
      { throw new RuntimeException("Port number " + the_port +
				   " out of range."); }
    port = the_port;
  }

  public void start_server () {
    start_server(true, false);
  }

  public void start_server (boolean catch_errors_p, boolean backtrace_p) {
    if ((port < 1024) || port >= 65536)
      { throw new RuntimeException("Port number " + port + " out of range."); }
    while (true)
      {
	ServerSocket server_socket = null;
	try 
	  {
	    try { server_socket = new ServerSocket(port); }
	    catch (Exception e1) 
	      { throw new RuntimeException(e1.toString()); }
	    Connection conn = 
	      LocalConnection.local_connection();
	    // Now we have a socket
	    try
	      {
		conn.register_interest_in_side_effects(this);
		while (true)
		  { Socket socket = null;
		    try { socket = server_socket.accept(); }
		    catch (Exception e2)
		      { throw new ConnectionProblem(e2.toString()); }
		    // We should now have a connection
		    PrintWriter write_stream = null;
		    try { write_stream = new PrintWriter
			      (socket.getOutputStream()); }
		    catch (IOException e3)
		      { throw new ConnectionProblem(e3.toString()); }
		    BufferedReader read_stream = null;
		    try { read_stream = new BufferedReader
			    (new InputStreamReader(socket.getInputStream()));
			}
		    catch (IOException e4)
		      { throw new ConnectionProblem(e4.toString()); }
		    // We now have the two streams to the socket.
		    if ((read_stream != null) && (write_stream != null))
		      { handle_connection
			  (server_socket, read_stream, write_stream,
			   catch_errors_p, backtrace_p); }
		    try { socket.close(); }
		    catch (IOException e5)
		      { throw new ConnectionProblem(e5.toString()); }
		  }
	      }
	    finally
	      { conn.register_interest_in_side_effects(this); }
	  }
	catch (ConnectionProblem problem)
	  { System.out.println("Server connection problem:  " + problem); }
      }
  }


  public void handle_connection(ServerSocket server_socket,
				BufferedReader r_s,
				PrintWriter write_stream,
				boolean catch_errors_p, boolean backtrace_p) {
    // System.out.println("Handling connection with " + r_s + " and " +
    // write_stream);
    PushableBufferedReader read_stream = new PushableBufferedReader(r_s);
    Node connect_form = null;
    Exception error = null;
    try { connect_form = decode(read_stream, null, false, 
				LocalConnection.local_connection()); }
    catch (Exception e) { error = e; }
    if (connect_form == null)
      { System.out.println
	  ("Error found whilst reading connection parameters.  " + error);
	handle_okbc_eval_error(write_stream, error, -1);
      }
    else
      {
	Node kb_library_string = Cons.getf(connect_form, _kb_library);
	if (kb_library_string instanceof OKBCString)
	  { kb_library = ((OKBCString)kb_library_string).string; }
	Node tv_sym = Cons.getf(connect_form, _transport_version);
	if (!(tv_sym instanceof AbstractSymbol) ||
	    (((AbstractSymbol)tv_sym).symbolName.compareTo
	     (max_supported_transport_version) > 0) ||
	    (((AbstractSymbol)tv_sym).symbolName.compareTo
	     (min_supported_transport_version) < 0))
	  { throw new RuntimeException("Transport version " + tv_sym + 
				       " not supported.  Supported = " +
				       min_supported_transport_version
				       + "..." +
				       max_supported_transport_version); 
	  }
	Node query_sym = Cons.getf(connect_form, _query_format);
	Symbol supported_query = Symbol.keyword(query_format);
	if (query_sym != supported_query)
	  { throw new RuntimeException("Query format " + query_sym + 
				       " not supported.  Supported = " +
				       supported_query); 
	  }
	Node reply_sym = Cons.getf(connect_form, _reply_format);
	Symbol supported_reply = Symbol.keyword(reply_format);
	if (reply_sym != supported_reply)
	  { throw new RuntimeException("Reply format " + reply_sym + 
				       " not supported.  Supported = " +
				       supported_reply); 
	  }
	Node ok_form = Cons.list(_ok);
	ok_form.encode(write_stream, false); // values
	_NIL.encode(write_stream, false);    // mask
	_NIL.encode(write_stream, false);    // error string
	_ok.encode(write_stream, false);     // status
	_NIL.encode(write_stream, false);	// ok to cache-p
	OKBCString ok_string = new OKBCString
	  ("Persistent Network OKBC connection established.");
	ok_string.encode(write_stream, false);
	newline(write_stream);
	write_stream.flush();
	try 
	  {
	    loop_executing_okbc_requests
	      (read_stream, write_stream, catch_errors_p, backtrace_p);
	  }
	catch (DisconnectException d)
	  { try { server_socket.close(); }
	    catch (IOException e) {}
	  }
      }
  }

  public void loop_executing_okbc_requests (PushableBufferedReader read_stream,
					    PrintWriter write_stream,
					    boolean catch_errors_p,
					    boolean backtrace_p)
  throws DisconnectException
  {
    while (true)
      { int reply_tag;
	String line = null;
	while (true)
	  {
	    try { do { line = read_stream.readLine();
		       // System.out.println("Skip line = " + line);
		     }
		  while (line != null &&
			 !(line.equals(request_cookie)));
		  line = read_stream.readLine(); // now read the tag
		}
	    catch (IOException e) 
	      { System.out.println("Exception in tag read: " + e); }
	    break;
	  }
	if (line == null)
	  { throw new DisconnectException(); }
	// System.out.println("Skip tag = " + line);
	// System.out.println("Looking for tag = " + request_tag);
	reply_tag = Integer.parseInt(line);
	Node find_args = null;
	Node form      = null;
	try { find_args
		= decode(read_stream, null, false, 
			 LocalConnection.local_connection());
	      form = decode(read_stream, null, false, 
			    LocalConnection.local_connection());
	    }
	catch (Exception e)
	  { System.out.println
	      ("Error found whilst processing command.  " +
	       "reply_tag = " + reply_tag +
	       ", find_args = " + find_args +
	       ", form = " + form +
	       ", exception = " + e); 
	  }
	catch (Error e)
	  { System.out.println
	      ("Error found whilst processing command.  " +
	       "reply_tag = " + reply_tag +
	       ", find_args = " + find_args +
	       ", form = " + form +
	       ", exception = " + e); 
	  }
	if (find_args != null && form != null)
	  {
	    messages = _NIL;	// messages generated in this request.
	    ok_to_cache_p = _T; // until we here different!
	    handle_okbc_request(write_stream, form, find_args,
				reply_tag, catch_errors_p, backtrace_p);
	  }
      }
  }
  
  public static final Symbol _current_kb = Symbol.intern_in_okbc("CURRENT-KB");

  void handle_okbc_request(PrintWriter write_stream, Node form, Node find_args,
			   int reply_tag, boolean catch_errors_p,
			   boolean backtrace_p)
       throws DisconnectException {
	 // System.out.println("Handling request " + form + " for " +
	 //	       reply_tag + " and " + find_args);
    Kb kb = null;
    Enumeration e = form.elements();
    for (; e.hasMoreElements() ;) {
      Object thing = (e.nextElement());
      if (thing instanceof Kb)
	{ kb = (Kb)thing; break; }
    }
    if (form.car() == _disconnect) { disconnect_connection(); }
    Node environment = _NIL;
    if (kb != null)
      { environment = new Cons(Cons.list(_current_kb, kb), environment); }
    eval_network_okbc_request(write_stream, form, environment, reply_tag, 
			     catch_errors_p, backtrace_p);
  }

  void actually_eval_network_okbc_request(PrintWriter write_stream, Node form, 
					 Node environment, int reply_tag) {
    Kb kb = null;
    Node args = form.cdr();
    if (args == null) { args = Node._NIL; }
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Node arg = e.next();
      if (arg instanceof Kb)
	{
	  boolean prototype_p = false;
	  Enumerator aliste = Node.registered_concrete_kbs.enumerate_list();
	  while (aliste.has_more_p()) {
	    Node entry = aliste.next();
	    if (entry.cadr() == arg) { prototype_p = true; break; }
	  }
	  if (!prototype_p) { kb = (Kb)arg; break;}
	}
    }
    if (kb == null) { kb = Node.the_current_kb; }
    Node.the_current_kb = kb;
    // System.out.println("Got KB = " + kb);
    // System.out.println("Got environemnt = " + environment);
    if (kb != null)
      { environment = new Cons(Cons.list(_current_kb, kb), environment); }
    // System.out.println("Got environemnt = " + environment);
    Node results = null;
    EvalContext env = null;
    if (kb == null)
      { env = new EvalContext(null, environment, null); }
    else { env = new EvalContext(kb, environment, null); }
    results = get_network_okbc_results(form, env);
    write_stream.print(request_cookie);
    newline(write_stream);
    write_stream.print(reply_tag);
    newline(write_stream);
    results.encode(write_stream, false);
    _NIL.encode(write_stream, false); // obsolete indices
    _NIL.encode(write_stream, false); // error-p
    _ok.encode(write_stream, false);  // status
    ok_to_cache_p.encode(write_stream, false); // ok-to-cache-p
    messages.encode(write_stream, false);
    write_stream.flush();
  }

  void eval_network_okbc_request(PrintWriter write_stream, Node form, 
				 Node environment, int reply_tag, 
				 boolean catch_errors_p, boolean backtrace_p) {
    if (catch_errors_p)
      { try { actually_eval_network_okbc_request
	      (write_stream, form, environment, reply_tag);
	    }
	catch (Throwable e)
	  { 
	    if (backtrace_p)
	    { e.printStackTrace(); }
	    handle_okbc_eval_error(write_stream, e, reply_tag); 
	  }
      }
    else { actually_eval_network_okbc_request
	   (write_stream, form, environment, reply_tag);
	 }
  }

  String dashify_string (String s, boolean start_p) {
    if (s.equals("")) { return ""; }
    else { char c = s.charAt(0);
	   if ((c >= 'A') && (c <= 'Z'))
	     { return (start_p ? "" : "-") + c + 
	       dashify_string(s.substring(1), false);
	     }
	   else { return c + dashify_string(s.substring(1), false); }
	 }
  }
  
  Symbol lisp_type_for_condition (Condition condition) {
    String class_name = condition.getClass().getName();
    int dotindex = class_name.lastIndexOf('.');
    String real_class_name = class_name;
    if (dotindex >= 0) { real_class_name = class_name.substring(dotindex + 1);}
    // Now we have a class name without the (Java) package name.
    String with_dashes = dashify_string(real_class_name, true).toUpperCase();
    return Symbol.intern_in_okbc(with_dashes);
  }
  
  void handle_okbc_eval_error 
    (PrintWriter write_stream, Throwable e, int reply_tag) {
    if (reply_tag != -1)
      { write_stream.print(request_cookie);
	newline(write_stream);
	write_stream.print(reply_tag);
	newline(write_stream);
      }
    _NIL.encode(write_stream, false);
    _NIL.encode(write_stream, false); // obsolete indices
    _T.encode(write_stream, false);  // error-p
    if (e instanceof Condition)
      { Node spec = new Cons(e.toString(),
			     new Cons(lisp_type_for_condition((Condition)e),
				      ((Condition)e).decode_to_plist()));
	spec.encode(write_stream, false);  // status
      }
    else { OKBCString s = new OKBCString(e.toString());
	   s.encode(write_stream, false);  // status
	 }
    _NIL.encode(write_stream, false); // ok-to-cache-p
    messages.encode(write_stream, false);
    write_stream.flush();
  }

  public static final Symbol _get_kb_type
      = Symbol.intern_in_okbc("GET-KB-TYPE");
  public static final Symbol _meta_kb
      = Symbol.intern_in_okbc("META-KB");
  public static final Symbol _concrete_kb_class_from_abstract_kb_class_name
      = Symbol.intern_in_ok_back
           ("CONCRETE-KB-CLASS-FROM-ABSTRACT-KB-CLASS-NAME");

  Node coerce_kb_types_etc (Node thing, TruthValue error_p) {
    if (!(thing instanceof Cons))
      { if (error_p == _NIL) { return thing; }
	else { throw new KbNotFound(thing).signal(); }
      }
    else if (_get_kb_type == thing.car())
      { Node the_class = extract_class_for_kb_type(thing.nth(1));
	if (the_class == _meta_kb) 
	  { return meta_kb(LocalConnection.local_connection()); }
	else if (the_class == null || the_class == _NIL)
	  { throw new KbNotFound(thing).signal(); }
	else { return the_class; } // this is actually a kb_type
      }
    else if (error_p != _NIL) { throw new KbNotFound(thing).signal(); }
    else if (thing instanceof Cons)
      { return new Cons(coerce_kb_types_etc(thing.car(), error_p),
			coerce_kb_types_etc(thing.cdr(), error_p));
      }
    else { return thing; }
  }


  Node extract_class_for_kb_type (Node spec) {
    if (spec == _default)
      { return concrete_kb_from_abstract_kb(default_kb_class);
      }
    else if ((spec instanceof Cons) &&
	     spec.car() == _concrete_kb_class_from_abstract_kb_class_name)
      { return concrete_kb_from_abstract_kb((Symbol)spec.nth(1)); }
    else if (spec instanceof AbstractSymbol)
      { Node res = concrete_kb_from_abstract_kb((AbstractSymbol)spec);
	if (res == _NIL) { return spec; }
	else { return res; }
      }
    else { return spec; }
  }

  Node get_network_okbc_results(Node form, EvalContext environment) {
    Object results = 
      Evaluator.getOkbcEvaluator().trivial_eval_expression_maybe_to_values
      (new Cons(form.car(), coerce_kb_types_etc(enquote(form.cdr()), _NIL)),
       environment, 0, null);
    if (results instanceof Node) { return Cons.list((Node)results); }
    else if (results instanceof Values) { return ((Values)results).toList(); }
    else { throw new GenericError
	   (_T, new OKBCString(results + 
			      " is an unhandled result type for OKBC eval."))
	   .signal();
	 }
  }

  public void notice_side_effect() {
    // System.out.println("Not ok to cache!");
    ok_to_cache_p = _NIL;
  }

  void disconnect_connection() throws DisconnectException {
    throw new DisconnectException();
  }

  public static void continue_running_okbc_server (String argv[]) {
    TupleKb tkb = new TupleKb(false); // force load of TupleKb
    CachingKb ckb = new CachingKb(); // force load of CachingKb
    NetworkKb nkb = new NetworkKb(); // force load of NetworkKb
    
    String portstring = null;
    if (OKBCApp.argv_suppliedp("PORT", argv)) 
      { portstring = OKBCApp.argv_get("PORT", argv); }
    else { throw new RuntimeException("No port supplied"); }
    boolean catch_errors_p =
      !OKBCApp.argv_suppliedp("DONT-CATCH-ERRORS", argv);
    boolean backtrace_p =
      OKBCApp.argv_suppliedp("PRINT-BACKTRACE", argv);
    boolean test_p = OKBCApp.argv_suppliedp("TEST", argv);
    
    TupleKb.ensure_registered();
    if (test_p)
      { Symbol test_name = Symbol.intern("TAXA", "OKBC-TEST");
	Kb kb_type = get_kb_type(Symbol.intern("TUPLE-KB", "OK-BACK"));
	Kb kb = kb_type.create_kb
	  (test_name, Node._NIL, Node._NIL,
	   LocalConnection.local_connection());
	Test t = new Test();
	boolean verbose_p = OKBCApp.argv_suppliedp("VERBOSE", argv);
	t.initialize_test_kb(kb, verbose_p);
	System.out.println("Finished initializing test KB.");
      }
    
    if (OKBCApp.argv_suppliedp("TRACE", argv)) 
      { Evaluator.getOkbcEvaluator().trace_p = true; }
    int port = (new Integer(portstring)).intValue();
    Server server = new Server(port);
    System.out.println("Starting OKBC server on port " + port + ".");
    server.start_server(catch_errors_p, backtrace_p);
  }

  public static void document_server_options () {
    System.out.println("    -port <port number> : the port to listen on");
    System.out.println("    -trace              : trace all calls");
    System.out.println("    -test               : preload test KB from " +
		                                 "test suite");
    System.out.println("    -verbose            : be verbose");
    System.out.print  ("    -print-backtrace    : ");
    System.out.println("print a backtrace for any errors caught.");
    System.out.print  ("    -dont-catch-errors  : ");
    System.out.println("don't catch errors that occur during processing.");
    System.out.println("                          used for debugging");
  }


  public static void main (String argv[]) {
    if (OKBCApp.argv_suppliedp("HELP", argv))
      { System.out.println("Usage: java Server [-option]*");
	System.out.println("where options include:");
	document_server_options();
      }
    else
      { continue_running_okbc_server(argv); }
  }
}
