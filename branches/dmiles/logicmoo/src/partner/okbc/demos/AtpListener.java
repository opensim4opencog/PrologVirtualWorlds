/***********************************************************************

  OKBC demos for Java

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

package demos;
import java.util.*;
import java.io.*;
import OKBC.*;

class AtpListener extends Listener {

  public AtpListener (boolean catch_p, ListenerEvaluator ev) {
    super(null, catch_p, ev);
  }

  static Node timeout = new OKBCInteger(60); //Node._NIL;
  static InferenceLevel inference_level = Node._all_inferable;
  
  static int NONE = 0;
  static int TELLING = 1;
  static int ASKING = 2;
  static int UNTELLING = 3;

  int default_operation = NONE;

  public String get_listener_prompt () {
         if (default_operation ==   TELLING) { return   "Telling>"; }
    else if (default_operation ==    ASKING) { return    "Asking>"; }
    else if (default_operation == UNTELLING) { return "Untelling>"; }
    else { return "OKBC>"; }
  }

  static final Symbol get_pretty_kbs_of_type_proc_name
         = Symbol.intern("GET-PRETTY-KBS-OF-TYPE-PROC", "DEMOS");
  static final Symbol trace_expression_proc
         = Symbol.intern("TRACE-EXPRESSION-PROC", "DEMOS");
  static final Symbol untrace_expression_proc
         = Symbol.intern("UNTRACE-EXPRESSION-PROC", "DEMOS");
  static final Symbol loadable_kb_names_proc
         = Symbol.intern("LOADABLE-KB-NAMES-PROC", "DEMOS");
  static final Symbol download_proc
         = Symbol.intern("DOWNLOAD-PROC", "DEMOS");
  static final Symbol upload_proc
         = Symbol.intern("UPLOAD-PROC", "DEMOS");


  static Node atp_listener_procedures
    = Cons.list(Cons.list(get_pretty_kbs_of_type_proc_name,
			  new OKBCString("(type)"),
			  new OKBCString
			  ("(let ((kbs (get-kbs-of-type :kb-type type)))" +
			   "  (do-list (x kbs)" +
			   "    (list x (get-frame-pretty-name x :kb kb))))")),
		Cons.list(download_proc,
			  new OKBCString("(to-path)"),
			  new OKBCString("(download to-path)")),
		Cons.list(upload_proc,
			  new OKBCString("(string)"),
			  new OKBCString("(upload string)")),
		Cons.list(trace_expression_proc,
			  new OKBCString("()"),
			  new OKBCString("(trace-expression)")),
		Cons.list(untrace_expression_proc,
			  new OKBCString("()"),
			  new OKBCString("(untrace-expression)")),
		Cons.list(loadable_kb_names_proc,
			  new OKBCString("()"),
			  new OKBCString("(loadable-kb-names)")));
		

  static void register_atp_listener_procedures (Kb meta_kb) {
    Enumerator e = atp_listener_procedures.enumerate_list();
    while(e.has_more_p()) {
      Node spec = e.next();
      Node name = spec.car();
      Node args = spec.cadr();
      Node body = spec.nth(2);
      Node procedure = meta_kb.create_procedure(args, body, Node._NIL);
      meta_kb.register_procedure(name, procedure);
    }
  }

  public static void main (String argv[]) {
    if (OKBCApp.argv_suppliedp("HELP", argv))
      { System.out.println("Usage: java demos.AtpListener [-option]*");
	System.out.println("where options include:");
	System.out.println("    -okbchost <hostname>: the host to connect to");
	System.out.println("    -port <port number> : the port to connect to");
	System.out.println
	  ("    -trace-evaluator    : trace client-side evaluator");
 	System.out.println("    -trace              : trace network calls");
	System.out.print  ("    -dont-catch-errors  : ");
	System.out.println("don't catch errors that occur during processing.");
	System.out.println("    -user_id <id>       : your KSL user id");
	System.out.println("    -password <pwd>     : your KSL password");
	System.out.println("Type (help) to get help on commands");
      }
    else
      {
	AtpListenerEvaluator ev = new AtpListenerEvaluator();
	boolean dont_catch_errors_p = OKBCApp.argv_suppliedp
	  ("DONT-CATCH-ERRORS", argv);
	AtpListener listener =
	  new AtpListener(!dont_catch_errors_p, ev);
	ev.listener = listener;
	listener.go(argv, dont_catch_errors_p);
      }
  }
  
  public void go(String []argv, boolean dont_catch_errors_p)
    {
      if (OKBCApp.argv_suppliedp("TRACE", argv)) 
	{ Node.trace_network_interactions_p = true; }
      if (OKBCApp.argv_suppliedp("TRACE-EVALUATOR", argv)) 
	{ AbstractEvaluator.trace_p = true; }
      // Set default host and port
      String host = OKBCApp.argv_get("okbchost", argv);
      if (host == null)
	{ host = OKBCApp.argv_get("host", argv); }
      if (host == null)
	{ 
	  host = "ontolingua.stanford.edu";
	  System.out.println("Defaulting host to " + host + ".");
	}
      String port_string = OKBCApp.argv_get("port", argv);
      if (port_string == null)
	{ 
	  port_string = "5916";
	  System.out.println("Defaulting port to " + port_string + ".");
	}
      Integer port = Integer.decode(port_string);
      String user_id = OKBCApp.argv_get("user_id", argv);
      if (user_id == null)
	{ user_id = OKBCApp.argv_get("user-id", argv); }
      if (user_id == null)
	{ user_id = "Alien";
	System.out.println("Defaulting user_id to " + user_id + ".");
	}
      String password = OKBCApp.argv_get("password", argv);
      if (password == null)
	{ password = "";
	System.out.println("Defaulting password to \"\".");
	}
	
      // Get host, port user_id, etc. from argv if supplied.
      Connection connection = DemoGeneral.get_ksl_connection
	(new OKBCString(host), new OKBCInteger(port), 
	 new OKBCString(user_id),
	 new OKBCString(password));

      int count = 0;
      int choice = 0;
      OKBCString choice_string = null;
      OKBCString zero = new OKBCString("0");
      Kb meta_kb = (Kb)NetworkKb.meta_kb(connection);
      register_atp_listener_procedures(meta_kb);
      while (true) {
	Kb kb_type
	  = meta_kb.get_kb_type(Symbol.intern("ABSTRACT-ATP-OKBC-KB-KB",
					      "OK-BACK"),
				connection);
	Node kb_specs = meta_kb.call_procedure
	  (get_pretty_kbs_of_type_proc_name, Cons.list(kb_type));
	Kb kb = null;
	if (kb_specs.length() == 0)
	  { System.out.println("# There are no KBs.");
	  System.out.println("# Creating one called KB1.");
	  kb = kb_type.create_kb
	    (Symbol.keyword("KB1"), Node._NIL,
	     Cons.list(Symbol.keyword("TIMEOUT-FOR-TELL&ASK-DEFAULTS"),
		       timeout,
		       Symbol.keyword
		       ("INFERENCE-LEVEL-FOR-TELL&ASK-DEFAULTS"),
		       inference_level,
		       Symbol.keyword ("IO-PACKAGE"),
		       new OKBCString("OKBC-USER")),
	     connection);
	  }
	else if (kb_specs.length() == 1)
	  { System.out.print("There is only one kb of this type: ");
	  System.out.println(kb_specs.car().cadr());
	  kb = (Kb)(kb_specs.car().car());
	  }
	else
	  {
	    Enumerator e = kb_specs.enumerate_list();
	    System.out.println("Pick a KB:");
	    count = 0;
	    while (e.has_more_p()) {
	      Node spec = e.next();
	      System.out.print(count + ": ");
	      System.out.println(spec.cadr());
	      count = count + 1;
	    }
	    choice_string
	      = OKBCApp.prompt_and_read_frame_name("Choose:",zero);
	    choice = (new Integer(choice_string.string)).intValue();
	    kb = (Kb)(kb_specs.nth(choice).car());
	  }
	if (kb instanceof Kb)
	  {
	    System.out.println("Entering listener.  " +
			       "Type (quit) to exit.");
	      
	    connection.callback = getCallback(argv);

	    ioSyntax = new IoSyntax("OKBC-USER");
	    EvalContext ec = new EvalContext(kb, this);
	    environment = build_default_listener_environment
	      (kb, ec);

	    register_atp_listener_procedures(kb);
	    if (dont_catch_errors_p)
	      { 
		startBreakLoop(kb, ec,
					!OKBCApp.argv_suppliedp
					("DONT-CATCH-ERRORS", argv),
					OKBCApp.argv_suppliedp
					("TRACE-PROCEDURES", argv),
					evaluator);
	      }
	    else
	      {
		try { 
		  startBreakLoop(kb, ec,
					      !OKBCApp.argv_suppliedp
					      ("DONT-CATCH-ERRORS", argv),
					      OKBCApp.argv_suppliedp
					      ("TRACE-PROCEDURES", argv),
					      evaluator); }
		catch (Error e)
		  { if (connection instanceof Connection)
		    { Node.close_connection(connection); }
		  }
	      }
	  }
      }
    }

  public Object eval (Node expression, Node kb) {
    boolean force_eval = false;
    if (expression instanceof Cons &&
	expression.length() >= 1 &&
	expression.car() instanceof AbstractSymbol)
      {
	AbstractSymbol sym = (AbstractSymbol)(expression.car());
	if (
	    sym.symbolName.equals("ASKING") ||
	    sym.symbolName.equals("DOWNLOAD") ||
	    sym.symbolName.equals("FLUSH") ||
	    sym.symbolName.equals("HELP") ||
	    sym.symbolName.equals("LOAD") ||
	    sym.symbolName.equals("LOADABLE-KB-NAMES") ||
	    sym.symbolName.equals("NEW-KB") ||
	    sym.symbolName.equals("OKBC") ||
	    sym.symbolName.equals("SAVE") ||
	    sym.symbolName.equals("SAVE-AS") ||
	    sym.symbolName.equals("TELLING") ||
	    sym.symbolName.equals("TRACE-EXPRESSION") ||
	    sym.symbolName.equals("UNTELLING")||
	    sym.symbolName.equals("UNTRACE-EXPRESSION") ||
	    sym.symbolName.equals("UPLOAD")
	    )
	  { force_eval = true; }
      }
    if (default_operation == AtpListener.NONE || force_eval)
      { return super.eval(expression, kb); }
    else if (default_operation == AtpListener.TELLING)
      { if (kb == null || kb == Node._NIL)
	  { throw new RuntimeException 
	    ("Cannot find KB in environment.  Environment is "
	     + environment); 
	  }
	((Kb)kb).tell(expression, Node._NIL, Node._known_true, Node._NIL);
	return Node._NIL;
      }
    else if (default_operation == AtpListener.UNTELLING)
      { if (kb == null || kb == Node._NIL)
	  { throw new RuntimeException 
	    ("Cannot find KB in environment.  Environment is "
	     + environment); 
	  }
	return ((Kb)kb).untell(expression, Node._NIL, Node._known_true, 
			       Node._NIL);
      }
    else if (default_operation == AtpListener.ASKING)
      { 
	if (kb == null || kb == Node._NIL)
	  { throw new RuntimeException 
	      ("Cannot find KB in environment.  Environment is "
	       + environment); 
	  }
	Values3 res =((Kb)kb).ask(expression, expression, inference_level,
				  Node._all, Node._T, Node._NIL, timeout,
				  Node._known_true, Node._NIL);
	if (res.thirdValue() == Node._timeout)
	  {
	    System.out.println();
	    System.out.println("[Timeout]");
	  }
	return res.firstValue();
      }
    else
      { throw new GenericError
	(Node._T,
	 new OKBCString("Illegal default operation " + default_operation))
	 .signal();
      }
  }

  public ConnectionCallback getCallback(String [] argv)
    {
      return new ConnectionCallback();
    }

}

class HelpEv extends Evaluator {

  String help_text =
"All commands to the ATP listener are expressed in Lisp syntax.\n"+
"\n"+
"To get a HTML format proof of the results of a query Q, you should\n"+
"ask the query:\n"+
"\n"+
" (:htmlify filename Q)\n"+
"\n"+
"e.g.\n"+
"\n"+
" (:htmlify test1 (human ?x))\n"+
"\n"+
"This will generate an html file called:\n"+
"\n"+
" http://hhh:ppp/doc/atp-proofs/uuu/test1.html\n"+
"\n"+
"where:\n"+
"  hhh - is the host name of the server\n"+
"  ppp - is the port of the server\n"+
"  uuu - is the user ID used for the connection to the server.\n"+
"all substrings are in lowercase.  Since the file space across ATP servers\n"+
"is shared you can always access these files with the URL:\n"+
"\n"+
" http://ontolingua.stanford.edu/doc/atp-proofs/uuu/test1.html\n"+
"The following commands are supported above and beyond the normal OKBC\n"+
"operations:\n"+
"\n"+
" (asking)  - Sets the mode of the listener so that it interprets any\n"+
"             expression that is not an OKBC command as a pattern to ASK.\n"+
"             See also (telling), (untelling), and (okbc).  For example,\n"+
"             saying:\n"+
"\n"+
"      (ask '(dog ?x))\n"+
"      (ask '(cat ?y))\n"+
"\n"+
"    is equivalent to saying:\n"+
"\n"+
"      (asking)\n"+
"      (dog ?x)\n"+
"      (cat ?y)\n"+
"\n"+
" (download <<to-path>>) - Downloads the contents of the current KB to\n"+
"                          client and saves it in the file whose pathname\n"+
"                          is the STRING <<to-path>>.  For example,\n"+
"                          (download \"~~/temp.kb\"). See also\n"+
"                          (upload <<to-path>>)\n"+
"\n"+
" (new-kb <<name>>) - Creates a new KB named <<name>> (e.g. :foo) and\n"+
"                     selects it.\n"+
"\n"+
" (flush) - Flushes the current KB and creates a new KB of the same name.\n"+
"           This is a useful command to call before calling (upload ...).\n"+
"\n"+
" (help) - Prints out this message.\n"+
"\n"+
" (load <<name>>) - Loads the KB of the specified name from the server's KB\n"+
"                   library and makes it be the current KB.  KB names are\n"+
"                   keywords such as \":kb42\".  See also (save),\n"+
"                   (save-as ...), and (loadable-kb-names).\n"+
"\n"+
" (loadable-kb-names) - Returns a list of all of the KBs that can be loaded\n"+
"                       into the current session on the server from the\n"+
"                       server's KB library.  KBs are identified by\n"+
"                       keywords, such as \":kb1\".  See also (load ...),\n"+
"                       (save), and (save-as ...)\n"+
"\n"+
" (okbc)    - Sets the mode of the listener so that it identifies any\n"+
"             expression that is not an OKBC command as an error.\n"+
"             See also (asling), (telling) and (untelling).\n"+
"\n"+
" (save) - Saves the current KB into the server's KB library.\n"+
"          See also (save-as ...), (load ...), and (loadable-kb-names).\n"+
"\n"+
" (save-as <<name>>) - Saves the current KB out to the server's KB library\n"+
"                      under the new name <<name>>.  The current KB in the\n"+
"                      server is also renamed to the new name.  KB names\n"+
"                      are keywords such as \":kb42\".  See also (save),\n"+
"                      (load ...), and (loadable-kb-names).\n"+
"\n"+
" (telling) - Sets the mode of the listener so that it interprets any\n"+
"             expression that is not an OKBC command as a sentence to TELL.\n"+
"             See also (asking), (untelling), and (okbc).  For example,\n"+
"             saying:\n"+
"\n"+
"      (tell '(dog fido))\n"+
"      (tell '(cat felix))\n"+
"\n"+
"    is equivalent to saying:\n"+
"\n"+
"      (telling)\n"+
"      (dog fido)\n"+
"      (cat felix)\n"+
"\n"+
" (trace-expression) - Enables ATP tracing.\n"+
"\n"+
" (untelling) - Sets the mode of the listener so that it interprets any\n"+
"               expression that is not an OKBC command as a sentence to\n"+
"               UNTELL.  See also (asking), (telling), and (okbc).  For\n"+
"               example, saying:\n"+
"\n"+
"      (untell '(dog fido))\n"+
"      (untell '(cat felix))\n"+
"\n"+
"    is equivalent to saying:\n"+
"\n"+
"      (untelling)\n"+
"      (dog fido)\n"+
"      (cat felix)\n"+
"\n"+
" (untrace-expression) - Disables ATP tracing.\n"+
"\n"+
" (upload <<from-path>>) - Uploads the contents of the file identified by\n"+
"                          the STRING <<from-path>> on the client into the\n"+
"                          current KB.  For example,\n"+
"                          (upload \"~~/temp.kb\"). See also\n"+
"                          (download <<to-path>>).\n\n";

  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    System.out.println(help_text);
    return Node._NIL;
  }
}

class OkbcEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    ((AtpListener)environment.listener).default_operation = AtpListener.NONE;
    return Node._NIL;
  }
}

class TellingEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    ((AtpListener)environment.listener).default_operation = 
      AtpListener.TELLING;
    return Node._NIL;
  }
}

class UntellingEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    ((AtpListener)environment.listener).default_operation = 
      AtpListener.UNTELLING;
    return Node._NIL;
  }
}

class AskingEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    ((AtpListener)environment.listener).default_operation = AtpListener.ASKING;
    return Node._NIL;
  }
}

class TraceExpressionEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    return ((Kb)Node.current_kb()).call_procedure
      (AtpListener.trace_expression_proc, Node._NIL);
  }
}

class UntraceExpressionEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    return ((Kb)Node.current_kb()).call_procedure
      (AtpListener.untrace_expression_proc, Node._NIL);
  }
}

class DownloadEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    OKBCString string = (OKBCString)(((Kb)Node.current_kb()).call_procedure
      (AtpListener.download_proc, Cons.list(arg0)));
    File f = new File(((OKBCString) arg0).string);
    try
      { 
	if (f.exists()) { f.delete(); }
	FileOutputStream stream = new FileOutputStream(f); 
	stream.write(string.string.getBytes());
      }
    catch (IOException e) { throw new RuntimeException(e.toString()); }
    return Node._NIL;
  }
}

class UploadEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node path = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    try
      { 
	File f = new File(((OKBCString) path).string);
	FileInputStream stream = new FileInputStream(f); 
	int length = (int)(f.length());
	StringBuffer s = new StringBuffer(length);
	for (int i=0; i < length ; i++) {
	  char c = (char)(stream.read());
	  s.append(c); s.setCharAt(i, c);
	}
	((Kb)Node.current_kb()).call_procedure
	  (AtpListener.upload_proc, Cons.list(new OKBCString(s.toString())));
      }
    catch (IOException e) { throw new RuntimeException(e.toString()); }
    return Node._NIL;
  }
}

class LoadableKbNamesEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    return ((Kb)Node.current_kb()).call_procedure
      (AtpListener.loadable_kb_names_proc, Node._NIL);
  }
}

class SaveEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    return ((Kb)Node.current_kb()).save_kb(Node._T);
  }
}

class SaveAsEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    ((Kb)Node.current_kb()).save_kb_as(arg0);
    return Node._NIL;
  }
}

class LoadEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node results =
      ((Kb)Node.current_kb()).open_kb
      (arg0, ((Kb)Node.current_kb()).connection(), Node._T);
    ((Kb)results).goto_kb();
    environment.putenv(_current_kb, results);
    return results;
  }
}

class FlushEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    Kb current_kb = (Kb)Node.current_kb();
    Node name = current_kb.kbname;
    Node kb_type = Node.get_kb_type(current_kb, current_kb.connection());
    Connection connection = current_kb.connection();
    current_kb.close_kb(Node._NIL);
    Node new_kb = kb_type.create_kb
      (name, Node._NIL,
       Cons.list(Symbol.keyword("TIMEOUT-FOR-TELL&ASK-DEFAULTS"),
		 AtpListener.timeout,
		 Symbol.keyword
		 ("INFERENCE-LEVEL-FOR-TELL&ASK-DEFAULTS"),
		 AtpListener.inference_level,
		 Symbol.keyword ("IO-PACKAGE"),
		 new OKBCString("OKBC-USER")),
       connection);
    ((Kb)new_kb).goto_kb();
    environment.putenv(_current_kb, new_kb);
    return new_kb;
  }
}

class NewKbEv extends Evaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Kb current_kb = (Kb)Node.current_kb();
    Node name = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node kb_type = Node.get_kb_type(current_kb, current_kb.connection());
    Connection connection = current_kb.connection();
    Node new_kb = kb_type.create_kb
      (name, Node._NIL,
       Cons.list(Symbol.keyword("TIMEOUT-FOR-TELL&ASK-DEFAULTS"),
		 AtpListener.timeout,
		 Symbol.keyword
		 ("INFERENCE-LEVEL-FOR-TELL&ASK-DEFAULTS"),
		 AtpListener.inference_level,
		 Symbol.keyword ("IO-PACKAGE"),
		 new OKBCString("OKBC-USER")),
       connection);
    ((Kb)new_kb).goto_kb();
    environment.putenv(_current_kb, new_kb);
    return new_kb;
  }
}

class AtpListenerEvaluator extends ListenerEvaluator {

  AtpListener listener = null;
  
  public void listener_initialize_evaluators(Hashtable ht) {
    super.listener_initialize_evaluators(ht);
    ht.put("ASKING", new AskingEv());
    ht.put("DOWNLOAD", new DownloadEv());
    ht.put("FLUSH", new FlushEv());
    ht.put("HELP", new HelpEv());
    ht.put("LOAD", new LoadEv());
    ht.put("LOADABLE-KB-NAMES", new LoadableKbNamesEv());
    ht.put("NEW-KB", new NewKbEv());
    ht.put("OKBC", new OkbcEv());
    ht.put("SAVE", new SaveEv());
    ht.put("SAVE-AS", new SaveAsEv());
    ht.put("TELLING", new TellingEv());
    ht.put("TRACE-EXPRESSION", new TraceExpressionEv());
    ht.put("UNTELLING", new UntellingEv());
    ht.put("UNTRACE-EXPRESSION", new UntraceExpressionEv());
    ht.put("UPLOAD", new UploadEv());
    Enumeration e = ht.keys();
    while(e.hasMoreElements()) {
      AbstractEvaluator ev = (AbstractEvaluator)ht.get(e.nextElement());
      ev.set_missing_procedure_handler(this);
      // System.out.println("Setting for: " + ev);
    }
  }

  public void pretty_print (Kb kb, Node thing) {
    // System.out.println("KB = " + kb + ", thing = " + thing);
    if (kb == null)
      { Cons.pretty_print(thing); }
    else { System.out.print(kb.value_as_string
			    (thing, Node._user_interface,
			     Node._NIL, Node._NIL).firstValue());
	 }
  }

  public Node err_on_missing_procedure (Node name, Node expression,
					EvalContext environment) {
    //System.out.println("Enter err_on_missing_procedure with: " + expression);
    if (listener.default_operation == AtpListener.NONE)
      { super.err_on_missing_procedure(name, expression, environment); }
    else if (listener.default_operation == AtpListener.TELLING)
      { Node kb = getenv(_current_kb, environment);
	if (kb == null || kb == Node._NIL)
	  { kb = getenv(Node._kb, environment); }
	if (kb == null || kb == Node._NIL)
	  { throw new RuntimeException 
	    ("Cannot find KB in environment.  Environment is "
	     + environment); 
	  }
	((Kb)kb).tell(expression, Node._NIL, Node._known_true, Node._NIL);
	return Node._NIL;
	  
      }
    else if (listener.default_operation == AtpListener.UNTELLING)
      { Node kb = getenv(_current_kb, environment);
	if (kb == null || kb == Node._NIL)
	  { kb = getenv(Node._kb, environment); }
	if (kb == null || kb == Node._NIL)
	  { throw new RuntimeException 
	    ("Cannot find KB in environment.  Environment is "
	     + environment); 
	  }
	((Kb)kb).untell(expression, Node._NIL, Node._known_true, Node._NIL);
	return Node._NIL;
	  
      }
    else if (listener.default_operation == AtpListener.ASKING)
      { Node kb = getenv(_current_kb, environment);
	if (kb == null || kb == Node._NIL)
	  { kb = getenv(Node._kb, environment); }
	if (kb == null || kb == Node._NIL)
	  { throw new RuntimeException 
	    ("Cannot find KB in environment.  Environment is "
	     + environment); 
	  }
	return
	  ((Kb)kb).ask(expression, expression, listener.inference_level,
		       Node._all, Node._T, Node._NIL, listener.timeout,
		       Node._known_true, Node._NIL).firstValue();
      }
    else
      { throw new GenericError
	(Node._T,
	 new OKBCString("Illegal default operation " +
			listener.default_operation))
	 .signal();
      }
    return Node._NIL;
  }

}
