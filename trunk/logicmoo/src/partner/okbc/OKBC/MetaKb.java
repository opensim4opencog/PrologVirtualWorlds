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

class KbAsClassFrame extends Node {
  Class kbClass = null;
  Kb kb_type = null;

  KbAsClassFrame (Class cl, Kb the_kb_type) {
    if (!(the_kb_type instanceof Kb))
      { throw new GenericError
	(_T, new OKBCString("Illegal kb_type: " + the_kb_type + " for: " +
			   cl)).signal();
      }
    kbClass = cl;
    kb_type = the_kb_type;
  }

  public String toString() {
    return "#<KbAsClassFrame: " + kbClass.getName() + ">";
  }
}

public class MetaKb extends TupleKb {
  static Symbol abstract_name = null; 

  public static void ensure_registered () {
    if (abstract_name == null)
      { abstract_name = (Symbol)(register_concrete_kb_type
           (Symbol.intern_in_ok_back("ABSTRACT-META-KB-KB"),
	    get_kb_type_internal()));
      }
  }

  static MetaKb meta_kb_type = null;

  public static Kb get_kb_type_internal () {
    if (meta_kb_type == null)
      { MetaKb kb = new MetaKb(); // intentionally not == $meta_kb$
	kb.kb_type_name = Symbol.intern("META-KB", "OKBC");
	meta_kb_type = kb;
	return kb;
      }
    else { return meta_kb_type; }
  }

  Hashtable kbClassFrames = new Hashtable();

  MetaKb() {
    setName(Symbol.intern_in_ok_back("$META-KB$"));
    kb_type_name = Symbol.intern_in_ok_back("META-KB");
    // Note:  This is a little dangerous.  We need to make sure that the
    // cache is always flushed when a KB is created or deleted for kbtypes
    // that are not stored directly in the MetaKb.  O if we only had real
    // method combination like in CLOS, this would simply be an :after
    // method.
    caching_policy = _agressive;
    allow_caching_p = true;
    tuple_store = new TupleKbTupleStore(this, false);
    this.initialize_tuple_kb();
  }

  /**
   * Use {@link #getMetaKb()} instead.
   * @deprecated
   */
  static final MetaKb $meta_kb$ = new MetaKb();

  /**
   * A hashtable of MetaKbs.
   * A hashtable of MetaKbs, in which the keys are ThreadGroups
   * and the values are MetaKbs. Essentially allows each thread
   * to have its own globally accessible MetaKb.
   * The {@link #getMetakb()} method uses this variable
   * to return a MetaKb associated with the current thread.
   * @see #getMetaKb()
   */
  private static Hashtable metaKbTable = new Hashtable(10);

  /**
   * Returns the local MetaKb.
   * Uses metaKbTable to return a
   * MetaKb that is valid for the
   * {@link java.lang.Thread#currentThread()}. 
   * Replaces accessing of MetaKb via the static final
   * field $meta_kb$.
   * @return MetaKb
   */
  public static MetaKb getMetaKb () {
    Thread thisThread = Thread.currentThread();
    ThreadGroup thisThreadGroup = thisThread.getThreadGroup();
    MetaKb myMetaKb = (MetaKb)metaKbTable.get(thisThreadGroup);

    if (myMetaKb == null) {
      myMetaKb = new MetaKb();
      metaKbTable.put(thisThreadGroup, myMetaKb);
    }
    return myMetaKb;
  }

  protected Node decontextualize_internal
     (Node value, ContextName from_context) 
  { Node result = null;
    if (value instanceof KbAsClassFrame)
      { Kb kb_type = ((KbAsClassFrame)value).kb_type;
        if (kb_type == null)
	  { result = frs_independent_frame_handle_internal (value, _T);
	  }
	else
	  { AbstractSymbol type = abstract_type_from_concrete_kb(kb_type);
	  // We return a KB object as the decontextualised form of a
	  // KbAsClassZFrame only if we can decontextualize it as an
	  // abstract kb.
	  // Maybe we chould only do this in server mode (?).
	    if (type != null)
	      { result = kb_type; }
	    else
	      { result = frs_independent_frame_handle_internal (value, _T);
	      }
	  }
      }
    else { result = super.decontextualize_internal(value, from_context); }
    // System.out.println("Decontextualized " + value + " to " + result);
    return result;
  }

  protected static Node frs_name_internal (Kb kb_type, Connection connection) {
    return new OKBCString("Meta KB");
  }

  protected Node get_frame_name_internal
        (Node frame, TruthValue kb_local_only_p) {
    Node real_frame = coerce_to_frame
      (frame, _NIL, kb_local_only_p).firstValue();
    if (real_frame == _NIL) { real_frame = frame; }
    if (real_frame instanceof Kb) { return ((Kb)real_frame).kbname; }
    if (real_frame instanceof AbstractKbLocator)
      { return ((AbstractKbLocator)real_frame).name; }
    if (real_frame instanceof KbAsClassFrame)
      { return new OKBCString
	(((KbAsClassFrame)real_frame).kbClass.getName()); }
    else { return super.get_frame_name_internal(frame, kb_local_only_p); }
  }

  void ensure_necessary_superclasses_asserted (Class kb_class) {
    ensure_necessary_superclasses_asserted(kb_class, null);
  }

  Node ensure_tuple_kb_class(Node the_class,
				    TruthValue kb_local_only_p) {
    Node real_class = coerce_to_frame_if_you_can(the_class, kb_local_only_p);
    Node class_frame = class_frame(_NIL);    
    if (primitive_direct_instance_of_p(real_class, class_frame, 
				       kb_local_only_p))
      {}
    else 
      { put_instance_types(real_class, Cons.list(class_frame),
			   kb_local_only_p); }
    return real_class;
  }

  void ensure_necessary_superclasses_asserted
    (Class kb_class, KbAsClassFrame subclass) {
    KbAsClassFrame class_frame = 
      (KbAsClassFrame)kbClassFrames.get(kb_class);
    if (class_frame == null)
      { Object instance = null;
	try { instance = kb_class.newInstance(); }
	catch (Exception exception) {}
	if ((instance != null) && (instance instanceof Kb))
	  { class_frame = new KbAsClassFrame(kb_class, (Kb)instance);
	    kbClassFrames.put(kb_class, class_frame);
	  }
      }
    if (class_frame != null)
      {
	ensure_tuple_kb_class(class_frame, _T);
	String name = kb_class.getName();
	if (subclass != null)
	  { put_class_superclasses(subclass, Cons.list(class_frame), _NIL); }
	if (!(name.equals("Kb")))
	  {
	    Class my_super = kb_class.getSuperclass();
	    if (my_super != null)
	      { ensure_necessary_superclasses_asserted(my_super, class_frame);}
	  }
      }
  }
 
  boolean subtypep (Class putative_subclass, Class putative_superclass) {
    if (putative_subclass == putative_superclass) { return true; }
    else { Class my_super = putative_subclass.getSuperclass();
	   if (my_super == null) { return false; }
	   else { return subtypep(my_super, putative_superclass); }
	 }
  }

  protected Values2 coerce_to_frame_internal
      (Node thing, TruthValue error_p, TruthValue kb_local_only_p) {
    if (thing instanceof Kb) { return new Values2(thing, _T); }
    else if (thing instanceof KbAsClassFrame) 
      { return new Values2(thing, _T); }
    else if (thing instanceof AbstractKbLocator || thing == _kb_locator)
      { return new Values2(thing, _T); }
    else if (thing instanceof AbstractSymbol)
      { String name = ((AbstractSymbol)thing).symbolName;
        // Special purpose coercion for the root class.
        if (name.equalsIgnoreCase("KB")) { name = "Kb"; }
	Object instance = null;
	Class kb_class = null;
	try { kb_class = Class.forName(name); }
	catch (Exception exception)
	  { // System.out.println("No class found for " + name +
	    //     	          " (" + thing + "), " + exception);
	  }
	if (kb_class == null)
	  // We failed to find the name.  Try in the OKBC. package just in case
	  { name = "OKBC." + name;
	    try { kb_class = Class.forName(name); }
	    catch (Exception exception)
	      { // System.out.println("No class found for " + name +
		// 		    " (" + thing + "), " + exception);
	      }
	  }
	if (kb_class != null)
	  {
	    KbAsClassFrame frame = 
	      (KbAsClassFrame)kbClassFrames.get(kb_class);
	    boolean subtypep = false;
	    if (frame != null) { return new Values2(frame, _T); }
	    instance = get_kb_type_from_class(kb_class);
	    try { if (instance == null)
		    { instance = kb_class.newInstance(); }
		}
	    catch (IllegalAccessException exception)
	      { Class Kb = null;
		try { Kb = Class.forName("OKBC.Kb"); }
		catch (Exception exception2)
		  { throw new RuntimeException(exception2.toString()); }
		if (subtypep(kb_class, Kb))
		  { subtypep = true; }
		String error_string =
		  error_message_for_illegal_access(kb_class, thing, exception);
		System.out.println(error_string);
		throw new RuntimeException(error_string);
	      }
	    catch (Exception exception)
	      { System.out.println("Couldn't instantiate " + kb_class +
				   " (" + thing + "), " + exception);
	      }
	    if (subtypep || 
		((instance != null) && (instance instanceof Kb)))
	      { frame = new KbAsClassFrame(kb_class, (Kb)instance);
		kbClassFrames.put(kb_class, frame);
		ensure_necessary_superclasses_asserted(kb_class);
		return new Values2(frame, _T);
	      }
	  }
	else
	  { Node maybe_frame = get_simple_frame_with_slot_value
	    (__name, thing, kb_local_only_p);
	    if (maybe_frame != _NIL && maybe_frame != thing)
	      { return new Values2(thing, _T); }
	    if (Cons.member_eql(thing, okbc_standard_names, null,
				kb_local_only_p) ||
		Cons.member_eql(thing, tuplekb_internal_relations, null,
				kb_local_only_p) ||
		thing == _kb_locator ||
		thing == _locator)
	      {}
	    else
	      { Node result = find_kb(thing, connection());
		if (result instanceof Kb)
		  { return new Values2(result, _T); }
		Enumerator locators = enumerate_class_instances
		  (_kb_locator, _taxonomic, _all, _T);
		while (locators.has_more_p()) {
		  Node locator = locators.next();
		  if (locator instanceof AbstractKbLocator &&
		      (((AbstractKbLocator)locator).name == kbname))
		    { return new Values2(locator, _T); }
		}
	      }
	  }
	if (error_p != _NIL)
	  { throw new NotCoercibleToFrame(thing, this).signal(); }
	else { return new Values2(_NIL, _NIL); }
      }
    else { Values2 vals = super.coerce_to_frame_internal
	     (thing, error_p, kb_local_only_p);
	   Node result = vals.firstValue();
	   Node found_p = vals.nthValue(1);
	   if (found_p != _NIL)
	     { if (result instanceof Kb) { return vals; } }
	   if (error_p != _NIL)
	     { throw new NotCoercibleToFrame(thing, this).signal(); }
	   else { return new Values2(_NIL, _NIL); }
	 }
  }

  protected Node get_kb_roots_internal
         (Node selector, TruthValue kb_local_only_p) {
    Node Kb_frame = coerce_to_frame(Symbol.intern_in_okbc("KB"),
				       _T, _NIL).firstValue();
    return Cons.list(Kb_frame);
  }

  protected Node get_kb_frames_internal(TruthValue kb_local_only_p) {
    return Cons.remove_duplicates
      (Cons.append_cons
       (get_kb_classes(_frames, kb_local_only_p),
	Cons.append_cons(get_kb_individuals(_frames, kb_local_only_p),
			 super.get_kb_frames_internal(kb_local_only_p))),
       _eql, this, kb_local_only_p);
  }

  protected Node get_kb_classes_internal
         (Node selector, TruthValue kb_local_only_p) {
    // Should be all compliant okbc implementations.
    // We'll run with all classes until we get more MOPish stuff.
    return Cons.remove_duplicates
      (new Cons(_kb_locator,
		Cons.append_cons(super.get_kb_classes_internal
				 (selector, kb_local_only_p),
				 super.get_kb_classes_internal
				 (selector, kb_local_only_p))),
       _eql, this, kb_local_only_p);
  }

  protected Values3 get_class_instances_internal
  (Node the_class, InferenceLevel inference_level, Node number_of_values,
   TruthValue kb_local_only_p) {
    if (inference_level == _direct)
      { Node kb_frame = coerce_to_frame
	(the_class, _NIL, kb_local_only_p).firstValue();
	if ((kb_frame != _NIL) && (kb_frame != the_class))
	  { return get_class_instances(kb_frame, _direct, number_of_values,
				       kb_local_only_p); }
	if (the_class instanceof KbAsClassFrame)
	  { Kb kb_type = ((KbAsClassFrame)the_class).kb_type;
	    Class cl = ((KbAsClassFrame)the_class).kbClass;
	    if (kb_type == null)
	      { throw new NotCoercibleToFrame(the_class, this).signal(); }
	    if (cl.getName().equals("Kb")) 
	      { return new Values3(_NIL, _T, _NIL); } // abstract
	    else { return
		   new Values3(Cons.append_cons
			      (kb_type.get_kbs_of_type
			       (LocalConnection.local_connection()),
			       super.get_class_instances_internal
			       (the_class, inference_level, number_of_values, 
				kb_local_only_p).firstValue()),
			       _T,
			      _NIL);
		 }
	  }
	else { return super.get_class_instances_internal
	       (the_class, inference_level, number_of_values, kb_local_only_p);
	     }
      }
    else { return super.get_class_instances_internal
	   (the_class, inference_level, number_of_values, kb_local_only_p);
	 }
  }

  static protected Node find_kb_of_type_internal
         (Node name_or_kb, Kb kb_type, Connection connection) {
    return meta_kb(connection);
  }

  protected void delete_frame_internal
            (Node frame, TruthValue kb_local_only_p) {
    Node real_frame = coerce_to_frame
      (frame, _NIL, kb_local_only_p).firstValue();
    if (real_frame == _NIL) { real_frame = frame; }
    // Delete facts in meta kb.
    super.delete_frame_internal(real_frame, kb_local_only_p);
    // Now, delete the KB.
    if (real_frame instanceof Kb) { ((Kb)real_frame).close_kb(_NIL); }
  }

  protected Node get_kbs_of_type_internal
         (Connection connection) {
    return Cons.list(meta_kb(connection));
  }

  protected Node get_kb_individuals_internal
         (Node selector, TruthValue kb_local_only_p) {
    Node results = super.get_kb_individuals_internal
      (selector, kb_local_only_p);
    Enumeration e = get_kb_classes(_frames, kb_local_only_p).elements();
    for (; e.hasMoreElements() ;) {
    KbAsClassFrame kb_class = (KbAsClassFrame)(e.nextElement());
      Kb proto = kb_class.kb_type;
      if (proto instanceof Kb)
	{ results = Cons.append_cons(proto.get_kbs_of_type(this.connection()),
				     results);
	}
    }
    return Cons.remove_duplicates(results, _eql, this, kb_local_only_p);
  }

  void get_kbs_of_type_using_meta_kb_1
     (Vector results, Vector kb_classes_visited, Node kb_class) {
       if (!kb_classes_visited.contains(kb_class))
	 { kb_classes_visited.addElement(kb_class);
	   Node instances = get_class_instances_implementation
	     (kb_class, _all, _T).firstValue();
	   Enumeration e = instances.elements();
	   for (; e.hasMoreElements() ;) {
	     Node instance = (Node)(e.nextElement());
	     if (!results.contains(instance))
	       { results.addElement(instance); }
	   }
	   Node subclasses = get_class_subclasses(kb_class, _direct, _all, 
						  _T).firstValue();
	   Enumeration e2 = subclasses.elements();
	   for (; e2.hasMoreElements() ;) {
	     Node subclass = (Node)(e2.nextElement());
	     get_kbs_of_type_using_meta_kb_1
	       (results, kb_classes_visited, subclass);
	   }
       }
    }

  protected Node get_kbs_of_type_using_meta_kb (Kb kb_type) {
    Class kb_class = kb_type.getClass();
    ensure_necessary_superclasses_asserted(kb_class);
    KbAsClassFrame class_frame = 
      (KbAsClassFrame)kbClassFrames.get(kb_class);
    if (class_frame == null)
      { throw new RuntimeException("Consistency error in meta kb!"); }
    Vector results = new Vector();
    Vector kb_classes_visited = new Vector();
    get_kbs_of_type_using_meta_kb_1(results, kb_classes_visited, class_frame);
    return Cons.toList(results);
  }

  protected Node create_kb_locator_internal (Node thing) {
    throw new GenericError
      (_T, new OKBCString("Not supported by MetaKb")).signal();
  }

  protected Node find_kb_locator_internal (Node thing) {
    throw new GenericError
      (_T, new OKBCString("Not supported by MetaKb")).signal();
  }


  protected Node save_kb_internal (TruthValue error_p) {
    throw new GenericError
      (_T, new OKBCString("Not supported by MetaKb")).signal();
  }


  protected void save_kb_as_internal (Node new_name_or_locator) {
    throw new GenericError
      (_T, new OKBCString("Not supported by MetaKb")).signal();
  }


  protected void close_kb_internal (Node save_p) {
    throw new GenericError
      (_T, new OKBCString("Not supported by MetaKb")).signal();
  }

  protected Node open_kb_internal
     (Node kb_locator, Connection connection, TruthValue error_p) {
    throw new GenericError
      (_T, new OKBCString("Not supported by MetaKb")).signal();
  }


  protected Node openable_kbs_internal (Connection connection, Node place) {
    return _NIL;
  }


  protected void expunge_kb_internal
      (Node kb_locator, Connection connection, TruthValue error_p) {
    throw new GenericError
      (_T, new OKBCString("Not supported by MetaKb")).signal();
  }

  protected static Cons meta_kb_supported_behaviors
  = (Cons)(Cons.list
	   (Cons.list(_constraint_checking_time, _NIL, _immediate),
	    Cons.list(_constraint_report_time, _NIL, _immediate),
	    Cons.append_cons
	    (Cons.list(_constraints_checked,    _NIL,
		       _value_type, _inverse, _cardinality, 
		       _minimum_cardinality, _maximum_cardinality, 
		       _same_values, _not_same_values, _subset_of_values,
		       _numeric_minimum, _numeric_maximum, _some_values,
		       _collection_type, _domain),
	     Cons.list(_slot_value_type, _slot_inverse, _slot_cardinality, 
		       _slot_minimum_cardinality, _slot_maximum_cardinality,
		       _slot_same_values, _slot_not_same_values,
		       _slot_subset_of_values, _slot_numeric_minimum,
		       _slot_numeric_maximum, _slot_some_values,
		       _slot_collection_type)),
	    Cons.list(_defaults,              _NIL, _override),
	    Cons.list(_compliance,            _NIL, _facets_supported,
		      _user_defined_facets, _read_only, _monotonic),
	    Cons.list(_class_slot_types,     _NIL, _template, _own),
	    Cons.list(_collection_types,     _NIL, _set),
	    Cons.list(_frame_names_required,  _NIL, _NIL),
	    Cons.list(_are_frames,           _NIL, _class, _facet, _slot,
		      _instance, _individual)));
  

  boolean fast_inhibit_constraint_checking = false;
  
  protected Node get_behavior_values_internal
     (Node behavior) {
       if (behavior == _constraint_checking_time)
	 { if (fast_inhibit_constraint_checking) { return _NIL; }
	   //else { return _T; }
	   else { return super.get_behavior_values_internal(behavior); }
	 }
       else { return super.get_behavior_values_internal(behavior); }
    }

  Cons get_kb_supported_behaviors () {
    return (Cons)meta_kb_supported_behaviors;
  }


  protected void put_behavior_values_internal (Node behavior, Node values) {
    throw new IllegalBehaviorValues(behavior, values).signal();
  }

  protected Node get_frame_pretty_name_internal
     (Node frame, TruthValue kb_local_only_p) {
       frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
       if (frame instanceof AbstractKbLocator)
	 { return ((AbstractKbLocator)frame).pretty_name(); }
       else { return super.get_frame_pretty_name_internal
	      (frame, kb_local_only_p);
	    }
    }

}

