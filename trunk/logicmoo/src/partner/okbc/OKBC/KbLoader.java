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


public class KbLoader {

  public static final Symbol _load = Symbol.keyword("LOAD");

  public Hashtable load_okbc_kb(File path, Kb kb) {
    return load_okbc_kb(path, kb, _load);
  }


  public Hashtable load_okbc_kb(File path, Kb kb, Node action) {
    return load_okbc_kb(path, kb, action, null);
    
  }


  public Hashtable load_okbc_kb(File path, Kb kb, Node action,
				Hashtable frame_handle_mapping_table) {
    return load_okbc_kb(path, kb, action, frame_handle_mapping_table, 
			Node._NIL);
    
  }


  Node okbc_load_warnings = Node._NIL;

  public Hashtable load_okbc_kb(File path, Kb kb, Node action,
				Hashtable frame_handle_mapping_table,
				TruthValue kb_local_only_p) {
    okbc_load_warnings = Node._NIL;
    Walker walker = new Walker();
    return load_okbc_kb_internal(path, kb, kb_local_only_p,
				 frame_handle_mapping_table, action, walker);
  }

  protected void maybe_okbc_load_warn (String string) {
    OKBCString str = new OKBCString(string);
    maybe_okbc_load_warn(str);
  }


  protected void maybe_okbc_load_warn (OKBCString string) {
    if (Cons.member_equal(string, okbc_load_warnings, null, null))
      {}
    else
      {
	okbc_load_warnings = new Cons(string, okbc_load_warnings);
	System.out.println("Warning: " + string);
      }
  }


  protected Node walk_form_for_interning_given_kb 
      (Node form, Hashtable frame_handle_mapping_table, Kb kb,
       TruthValue kb_local_only_p, Node action, Walker walker) {
    if (form instanceof Cons &&
	form.car() instanceof AbstractSymbol &&
	Walker._define_okbc_frame.stringEquals(form.car()))
      {
	boolean old_switch = walker.symbols_ok_as_non_logical_constants_p;
	walker.symbols_ok_as_non_logical_constants_p = true;
	Node res = walker.walk(form, new InternFramesIfNecessaryWalkFunction
			       (frame_handle_mapping_table, this), 
			       kb, Walker._define_okbc_frame, Node._NIL,
			       Node._NIL);
	walker.symbols_ok_as_non_logical_constants_p = old_switch;
	return res;
      }
    else { return Node._NIL; }
  }


  protected Node assert_form_given_kb (Node form,
				       Hashtable frame_handle_mapping_table, 
				       Hashtable handle_to_name_mapping_table,
				       Kb kb, TruthValue kb_local_only_p,
				       Node action, Walker walker) {
    if (form instanceof Cons &&
	form.car() instanceof AbstractSymbol &&
	Walker._define_okbc_frame.stringEquals(form.car()))
      {
	return assert_frame_definition(form, frame_handle_mapping_table, 
				       handle_to_name_mapping_table, kb,
				       kb_local_only_p, action, walker);
      }
    else { return Node._NIL; }
  }


  protected Node walk_form_for_load(Node form,
				    Hashtable frame_handle_mapping_table,
				    Kb kb, Walker walker) {
    boolean old_switch = walker.symbols_ok_as_non_logical_constants_p;
    walker.symbols_ok_as_non_logical_constants_p = true;
    Node res = walker.walk(form, new WalkFormForLoadWalkFunction
			   (frame_handle_mapping_table, this), 
			   kb, Walker._define_okbc_frame, Node._NIL,
			   Node._NIL);
    walker.symbols_ok_as_non_logical_constants_p = old_switch;
    return res;
  }

  protected Node assert_frame_definition 
  (Node form, 
   Hashtable frame_handle_mapping_table, 
   Hashtable handle_to_name_mapping_table,
   Kb kb, TruthValue kb_local_only_p,
   Node action, Walker walker) 
  {
    Node interned = walk_form_for_load(form, frame_handle_mapping_table, kb,
				       walker);
    Node handle_part = Node._NIL;
    Node real_name = (Node)(handle_to_name_mapping_table.get(form.cadr()));
    if (real_name == null) { real_name = form.cadr(); }
    Object existing = frame_handle_mapping_table.get(real_name);
    if (existing != null)
      { handle_part = Cons.list(Node._handle,
				Cons.list(Node.__quote, (Node)existing)); }
    Node create_frame_fname = Symbol.intern_in_okbc("CREATE-FRAME");
    EvalContext environment = Listener.build_default_listener_environment
      (kb, new EvalContext());
    Node filtered = Node._NIL;
    Node type = Cons.getf(form, Node._type);
    if (type == null || type == Node._NIL)
      { type = Node._individual; }
    Cons tail = null;
    Enumerator e = interned.cdr().cdr().enumerate_list();
    while(e.has_more_p()) {
      Node key = e.next();
      Node value = e.next();
      if (key != Node._type && key != Node._kb && key != Node._sentences)
	{
	  Cons new_struct =
	    (Cons)(Cons.list(key, Cons.list(Node.__quote, value)));
	  if (tail == null)
	    { filtered = new_struct;
	      tail = (Cons)(filtered.cdr());
	    }
	  else
	    { tail.set_cdr(new_struct);
	      tail = (Cons)(new_struct.cdr());
	    }
	}
    }
    Evaluator evaluator = new Evaluator();
    Node frame =
      (Node)(evaluator.trivial_eval_expression_maybe_to_values
	     (new Cons(create_frame_fname,
		       new Cons(Cons.list(Node.__quote, real_name),
				new Cons(Cons.list(Node.__quote, type),
					 new Cons(Node._kb,
						  new Cons(Cons.list
							   (Node.__quote, kb),
							   Cons.append_cons
							   (handle_part,
							    filtered)))))),
	      environment, 0, kb));
    Node sentences = Cons.getf(interned, Node._sentences);
    Enumerator sente = sentences.enumerate_list(sentences);
    while (sente.has_more_p()) {
      Node sentence = sente.next();
      kb.tell(sentence, frame, Node._known_true, kb_local_only_p);
    }
    return frame;
  }

  
  Node find_or_create_frame_handle_for_intern (Node form,
					       Node defining_type,
					       Kb kb, Hashtable
					       frame_handle_mapping_table) {
    Object existing = frame_handle_mapping_table.get(form);
    if (existing != null)
      { return (Node)existing; }
    else
      {
	Values2 v = kb.coerce_to_frame(form, Node._NIL, Node._NIL);
	Node frame = v.firstValue();
	Node found_p = v.secondValue();
	if (found_p != Node._NIL)
	  {
	    Node handle = kb.get_frame_handle(frame, Node._NIL);
	    frame_handle_mapping_table.put(form, handle);
	    return handle;
	  }
	else
	  {
	    /*
	    System.out.println
	      ("form = " + form + ", type = " + form.getClass());
	    if (form == Symbol.intern("ABBREV", "OKBC-USER"))
	      { 
		Enumeration e = frame_handle_mapping_table.keys();
		int i = 0;
		while (e.hasMoreElements()) {
		  Object key = e.nextElement();
		  System.out.println
		    (i + ": " +key + " -> " +
		     frame_handle_mapping_table.get(key));
		}
	      }
	    */
	    Node new_handle = null;
	    if (defining_type instanceof ContextName)
	      { new_handle = kb.allocate_frame_handle_internal
		(form, (ContextName)defining_type, Node._NIL);
	      }
	    else
	      { new_handle = kb.allocate_frame_handle_internal
		(form, Node._frame, Node._NIL);
	      }
	    frame_handle_mapping_table.put(form, new_handle);
	    return new_handle;
	  }
      }
  }


  Node intern_frame_handles_if_necessary(Kb kb, MostAbstractSubWalker
					 production,
					 Node known_context_type,
					 Node bound_vars,
					 Node form, Hashtable
					 frame_handle_mapping_table) {
    String p_name = production.production_name();
    // System.out.println("form: " + form);
    // System.out.println("known_context_type: " + known_context_type);
    // System.out.println("production_name: " + p_name);
    if (p_name.equals("ConstantWalker") ||
	p_name.equals("ObjectConstantWalker") ||
	p_name.equals("FunctionConstantWalker") ||
	p_name.equals("RelationConstantWalker"))
      {
	if (form instanceof AbstractSymbol ||
	    form instanceof OKBCString)
	  {
	    Node are_frames = kb.get_behavior_values(Node._are_frames);
	    // System.out.println("are-frames: " + are_frames);
	    // System.out.println("form: " + form);
	    // System.out.println("known_context_type: " + known_context_type);
	    if (known_context_type == Node._NIL ||
		Cons.member_eql(known_context_type, are_frames, kb, Node._NIL))
	      { return find_or_create_frame_handle_for_intern
		(form, known_context_type, kb, frame_handle_mapping_table);
	      }
	    else { return form; }
	  }
	else { return form; }
      }
    else { return form; }
  }
  

  Node substitute_frame_handles_if_necessary(Kb kb,
					     MostAbstractSubWalker production,
					     Node known_context_type,
					     Node bound_vars,
					     Node form, Hashtable
					     frame_handle_mapping_table) {
    Node substituted = form;
    String p_name = production.production_name();
    if (p_name.equals("ConstantWalker") ||
	p_name.equals("ObjectConstantWalker") ||
	p_name.equals("FunctionConstantWalker") ||
	p_name.equals("RelationConstantWalker"))
      {
	Object match = frame_handle_mapping_table.get(form);
	if (match != null) { substituted = (Node)match; }
      }
    return substituted;
  }

  
  static final public int SILENT = 0;
  static final public int BLIP = 1;
  static final public int VERBOSE = 2;

  public int verbosity = SILENT;

  
  protected Hashtable load_okbc_kb_internal(File path, Kb kb,
					    TruthValue kb_local_only_p,
					    Hashtable
					    frame_handle_mapping_table,
					    Node action, Walker walker) {
    if (frame_handle_mapping_table == null)
      { frame_handle_mapping_table = new Hashtable(); }
    
    FileInputStream stream = null;
    Exception exception = null;
    boolean eof_p = false;
    boolean ok_p = true;
    IoSyntax ioSyntax = kb.ioSyntax;
    Node forms = Node._NIL;
    Cons tail = null;
    try { try { stream = new FileInputStream (path); }
	  catch (IOException e)
	    {
	      ok_p = false;
	      exception = e;
	    }
	  LispReader reader = null;
	  if (ok_p)
	    { reader = new LispReader(stream, ioSyntax, false,
				      Node.__EOF, 10,
				      Node._upcase);
	    }
	  while (ok_p && !eof_p) {
	    if (!eof_p)
	      { 
		Node form = reader.read();
		Node processed = form;
		// System.out.println("Form = " + form);
		if (form == Node.__EOF) { break; }
		if (form instanceof Cons)
		  { Node car = form.car();
		    if (car instanceof AbstractSymbol &&
			Walker._in_package.stringEquals(car))
		      { update_reader_package(reader, form); }
		    else
		      { 
			processed = walk_form_for_interning_given_kb
			(form, frame_handle_mapping_table, kb, kb_local_only_p,
			 action, walker);
			if (verbosity == 1)
			  { System.out.print("I"); }
			if (verbosity == 2)
			  { System.out.println();
			    System.out.println("Interning:");
			    Cons.pretty_print(processed);
			  }
		      }
		  }
		else
		  { processed = walk_form_for_interning_given_kb
		    (form, frame_handle_mapping_table, kb, kb_local_only_p,
		     action, walker);
		  }
		if (tail == null)
		  { 
		    forms = Cons.list(processed);
		    tail = (Cons)forms;
		  }
		else
		  { 
		    Cons new_tail = new Cons(processed);
		    tail.set_cdr(new_tail);
		    tail = new_tail;
		  }
	      }
	  }
	}
    finally
      { if (stream != null)
	  { try { stream.close(); }
	    catch (Exception ex) {}
	  }
      }
    // First compute the frame to name backtranslation table.
    Hashtable handle_to_name_mapping_table = new Hashtable
      (frame_handle_mapping_table.size());
    Enumeration he = frame_handle_mapping_table.keys();
    // System.out.println("Mapping table:");
    while (he.hasMoreElements()) {
      Object key = he.nextElement();
      Object value = frame_handle_mapping_table.get(key);
      handle_to_name_mapping_table.put(value, key);
      // System.out.println(key + " -> " + value);
    }
    // Now do the asserting.....
    Enumerator e = forms.enumerate_list();
    while (ok_p && e.has_more_p()) {
      Node form = e.next();
      // System.out.println("Form = " + form);
      if (form == Node.__EOF) { break; }
      if (form instanceof Cons)
	{ Node car = form.car();
	if (car instanceof AbstractSymbol &&
	    Walker._in_package.stringEquals(car))
	  {} // do nothing.
	else
	  { 
	    if (verbosity == 1)
	      { System.out.print("A"); }
	    if (verbosity == 2)
	      { 
		System.out.println();
		System.out.println("Asserting:");
		Cons.pretty_print(form); 
	      }
	    assert_form_given_kb
	      (form, frame_handle_mapping_table, handle_to_name_mapping_table,
	       kb, kb_local_only_p, action, walker);
	  }
	}
      else
	{ assert_form_given_kb
	    (form, frame_handle_mapping_table, handle_to_name_mapping_table,
	     kb, kb_local_only_p, action, walker);
	}
    }
    // We now have the problem that if we're not in frame-names-required
    // mode then we may have stubbed some definitions without having their
    // names available.  Now we rename any frames with broken names.
    boolean names_required_p = false;
    if (Cons.member_eql(Node._NIL,
			kb.get_behavior_values(Node._frame_names_required),
			null, kb_local_only_p))
      {
	Enumeration en = frame_handle_mapping_table.keys();
	// System.out.println("Mapping table 2");
	while (en.hasMoreElements()) {
	  Node name = (Node)(en.nextElement());
	  Node handle = (Node)(frame_handle_mapping_table.get(name));
	  // System.out.println(handle + " -> " + name);
	  if (kb.coercible_to_frame_p(handle, Node._NIL) != Node._NIL)
	    {
	      Node actual_name = kb.get_frame_name(handle);
	      // System.out.println(actual_name + " and " + name + " are " + handle);
	      if (actual_name.equals(name)) {}
	      else
		{
		  kb.put_frame_name(handle, name); 
		  // System.out.println("Renamed " + actual_name + " to " + name);
		}
	    }
	}
      }

    return frame_handle_mapping_table;
  }

  
  void update_reader_package (LispReader reader, Node form) {
    Node cadr = form.cadr();
    if (cadr instanceof AbstractSymbol ||
	cadr instanceof OKBCString)
      {
	String s = null;
	if (cadr instanceof AbstractSymbol)
	  { s = ((AbstractSymbol)cadr).symbolName; }
	else { s = ((OKBCString)cadr).string; }
	AbstractPackage p = Package.find_package(s);
	if (p == null)
	  { p =
	      QuasiPackage.find_or_create_QuasiPackage(s);
	  }
	reader.put_package(p);
      }
    else
      { throw new GenericError
	  (Node._T, new OKBCString
	   ("Illegal in-package form: " + form)).signal();
      }
  }
  
}


class InternFramesIfNecessaryWalkFunction extends AbstractWalkFunction {

  Hashtable frame_handle_mapping_table = null;
  KbLoader loader = null;

  InternFramesIfNecessaryWalkFunction (Hashtable ht, KbLoader l) {
    frame_handle_mapping_table = ht;
    loader = l;
  }

  public Node funcall(Kb kb, MostAbstractSubWalker production,
		      Node known_context_type, Node bound_vars,
		      Node form) {
    return loader.intern_frame_handles_if_necessary
      (kb, production, known_context_type, bound_vars, form,
       frame_handle_mapping_table);
  }
}


class WalkFormForLoadWalkFunction extends AbstractWalkFunction {

  Hashtable frame_handle_mapping_table = null;
  KbLoader loader = null;

  WalkFormForLoadWalkFunction (Hashtable ht, KbLoader l) {
    frame_handle_mapping_table = ht;
    loader = l;
  }

  public Node funcall(Kb kb, MostAbstractSubWalker production,
		      Node known_context_type, Node bound_vars,
		      Node form) {
    return loader.substitute_frame_handles_if_necessary
      (kb, production, known_context_type, bound_vars, form,
       frame_handle_mapping_table);
  }
}
