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
import java.net.*;

public class TupleKb extends AbstractFactStoreKb {
  static Symbol abstract_name = null;
  static TupleKb tuple_kb_type = null;
  static final String portable_kb_file_extension = ".kb";
  static final String nonportable_kb_file_extension = ".tkb";

  boolean save_kb_files_portably_p = false;
  
  public static void ensure_registered () {
    if (abstract_name == null)
      { abstract_name = (Symbol)(register_concrete_kb_type
	  (Symbol.intern_in_ok_back("ABSTRACT-TUPLE-KB-KB"),
	   get_kb_type_internal()));
      }
  }

  public static Kb get_kb_type_internal () {
    if (tuple_kb_type == null)
      { TupleKb kb = new TupleKb(false);
	kb.kb_type_name = Symbol.intern_in_ok_back("TUPLE-KB");
	tuple_kb_type = kb;
	return kb;
      }
    else { return tuple_kb_type; }
  }

  Hashtable frame_name_mapping_table = new Hashtable();
  Hashtable name_to_frame_mapping_table = new Hashtable();
  public boolean read_only_p = false;

  static final TupleStoreTester sf_V = TupleKbTupleStore.sf_V;
  static final TupleStoreTester fa_S_Fv2_F = TupleKbTupleStore.fa_S_Fv2_F;
  static final TupleStoreTester fa_Sf_V2_S = TupleKbTupleStore.fa_Sf_V2_S;
  static final TupleStoreTester _Fa_Sf_V2_S = TupleKbTupleStore._Fa_Sf_V2_S;
  static final TupleStoreTester r_Fa_Sf_V2_S = TupleKbTupleStore.r_Fa_Sf_V2_S;
  static final TupleStoreTester _Fasf_V2_V = TupleKbTupleStore._Fasf_V2_V;
  static final TupleStoreTester r_Fasf_V2_V = TupleKbTupleStore.r_Fasf_V2_V;
  static final TupleStoreTester r_Fasf_V2_Fa = TupleKbTupleStore.r_Fasf_V2_Fa;
  static final TupleStoreTester  fv = TupleKbTupleStore.fv;
  static final TupleStoreTester sfv = TupleKbTupleStore.sfv;
  static final TupleStoreTester _Sfv = TupleKbTupleStore._Sfv;
  static final TupleStoreTester fa_Sfv = TupleKbTupleStore.fa_Sfv;
  static final TupleStoreTester fasfv = TupleKbTupleStore.fasfv;
  static final TupleStoreTester rfasfv = TupleKbTupleStore.rfasfv;
  static final TupleStoreTester s_Fv = TupleKbTupleStore.s_Fv;
  static final TupleStoreTester fas_Fv = TupleKbTupleStore.fas_Fv;
  static final TupleStoreTester rfas_Fv = TupleKbTupleStore.rfas_Fv;
  static final TupleStoreTester     s_F_V2_V = TupleKbTupleStore.s_F_V2_V;
  static final TupleStoreTester     s_F_V2_F = TupleKbTupleStore.s_F_V2_F;
  static final TupleStoreTester  fa_S_F_V2_V = TupleKbTupleStore.fa_S_F_V2_V;
  static final TupleStoreTester rfa_S_F_V2_V = TupleKbTupleStore.rfa_S_F_V2_V;
  static final TupleStoreTester fa_Sf_V2_V = TupleKbTupleStore.fa_Sf_V2_V;
  static final TupleStoreTester fas_F_V2_V = TupleKbTupleStore.fas_F_V2_V;
  static final TupleStoreTester _Fas_F_V2_V = TupleKbTupleStore._Fas_F_V2_V;
  static final TupleStoreTester r_Fas_F_V2_V = TupleKbTupleStore.r_Fas_F_V2_V;
  static final TupleStoreTester fasf_V = TupleKbTupleStore.fasf_V;
  static final TupleStoreTester rfasf_V = TupleKbTupleStore.rfasf_V;
  static final TupleStoreTester f_V = TupleKbTupleStore.f_V;
  static final TupleStoreTester _Fasf_V2_Fa = TupleKbTupleStore._Fasf_V2_Fa;
  static final TupleStoreTester _Sf_V2_S = TupleKbTupleStore._Sf_V2_S;
// This is the actual implementation of the OKBC Tuple KB.  The stuff above
// is just the tuple store.

  static final Symbol _frame_var = Symbol.intern("?FRAME", "VARS");
  static final Symbol _class_var = Symbol.intern("?CLASS", "VARS");
  static final Symbol _facet_var = Symbol.intern("?FACET", "VARS");
  static final Symbol _instance_var = Symbol.intern("?INSTANCE", "VARS");
  static final Symbol _slot_var = Symbol.intern("?SLOT", "VARS");
  static final Symbol _sub_var = Symbol.intern("?SUB", "VARS");
  static final Symbol _super_var = Symbol.intern("?SUPER", "VARS");
  static final Symbol _value_var = Symbol.intern("?VALUE", "VARS");
  static final Symbol __none = Symbol.intern("NONE", "VARS");

  static final Symbol __has_sentence
  = Symbol.intern("__HAS_SENTENCE", "FRAMES");
  static final Symbol __documentation
  = Symbol.intern("__DOCUMENTATION", "FRAMES");
  static final Symbol __deleted = Symbol.intern("__DELETED", "FRAMES");

  static final Node tuplekb_internal_quatlike_relations =
    Cons.list(__template_facet_value, __default_template_facet_value,
	      __default_facet_value);

  static final Node tuplekb_internal_facetlike_relations =
    Cons.list(__template_slot_value, __default_template_slot_value,
	      __default_slot_value, __template_facet_of, __facet_of);

  static final Node tuplekb_internal_slotlike_relations =
    Cons.list(__name, __pretty_name, __subclass_of, __template_slot_of,
	      __slot_of, __has_sentence, __instance_of);

  static final Node tuplekb_internal_classlike_relations =
    Cons.list(__primitive);

  static final Node tuplekb_internal_relations =
    Cons.append_cons(Cons.append_cons(tuplekb_internal_quatlike_relations,
				      tuplekb_internal_facetlike_relations),
		     Cons.append_cons(tuplekb_internal_slotlike_relations,
				      tuplekb_internal_classlike_relations));

  static final Node compute_tuplekb_backtranslation_alist (Node list) {
    Node res = _NIL;
    Enumeration e = list.elements();
    while (e.hasMoreElements()) {
      Symbol sym = (Symbol)(e.nextElement());
      if (sym.symbolName.startsWith("__"))
	{ Cons new_entry = new Cons(sym,
				    Symbol.intern(sym.symbolName.substring(2),
						  "KEYWORD"));
	  res = new Cons(new_entry, res);
	}
    }
    return Cons.reverse(res);
  }

  static final Node tuplekb_backtranslation_alist =
    compute_tuplekb_backtranslation_alist(tuplekb_internal_relations);
    
  static final Node compute_tuplekb_translation_alist (Node list) {
    Node res = _NIL;
    Enumeration e = list.elements();
    while (e.hasMoreElements()) {
      Symbol sym = (Symbol)(e.nextElement());
      if (sym.symbolName.startsWith("__"))
	{ res = new Cons(new Cons(Symbol.intern(sym.symbolName.substring(2),
						"KEYWORD"),
				  sym),
			 res);
	}
    }
    return Cons.reverse(res);
  }

  static final Node tuplekb_translation_alist =
    compute_tuplekb_translation_alist(tuplekb_internal_relations);
    
  static final Node compute_non_okbc_mapping_internal_relations (Node list) {
    Node res = _NIL;
    Enumeration e = list.elements();
    while (e.hasMoreElements()) {
      Symbol sym = (Symbol)(e.nextElement());
      Node entry = Cons.assoc(sym, tuplekb_backtranslation_alist);
      Node key = _NIL;
      if (entry != _NIL && entry != null)
	{ key = entry.cdr(); }
      if (Cons.member_eql(key, okbc_relation_symbols, null, null) ||
	  Cons.member_eql(key, okbc_standard_names, null, null) ||
	  Cons.member_eql(key, kif_operator_symbols, null, null) ||
	  Cons.member_eql(key, evaluable_predicate_symbols, null, null))
	{}
      else
	{ res = new Cons(sym, res); }
    }
    return Cons.reverse(res);
  }

  static final Node non_okbc_mapping_internal_relations =
    compute_non_okbc_mapping_internal_relations(tuplekb_internal_relations);
    
  static final Node compute_defaulting_relations_alist (Node list) {
    Node res = _NIL;
    Enumeration e = list.elements();
    while (e.hasMoreElements()) {
      Symbol sym = (Symbol)(e.nextElement());
      if (sym.symbolName.startsWith("__DEFAULT"))
	{ Symbol match_sym = null;
	  if (sym == __default_facet_value ||
	      sym == __default_slot_value)
	    { match_sym = __none; }
	  else { match_sym = Symbol.intern("__" +
					   sym.symbolName.substring(10),
					   "KEYWORD"); 
	       }
	  res = new Cons(Cons.list(sym,
				   match_sym),
			 res);
	}
    }
    return Cons.reverse(res);
  }

  static final Node tuplekb_defaulting_relations_alist =
    compute_defaulting_relations_alist(tuplekb_internal_relations);
    
  static final Cons facet_slot_class_and_individual_symbols = 
     (Cons)(Cons.list(_facet, _slot, _class, _individual));

  // redundant slot to save the type check.
  protected TupleKbTupleStore tuple_store;

  static boolean default_use_hash_index_p = true;

  public TupleKb () {
    tuple_store = new TupleKbTupleStore(this, default_use_hash_index_p);
    caching_policy = _agressive;
    this.initialize_tuple_kb();
  }

  public TupleKb (boolean initialize_p) {
    super();
    if (initialize_p)
      { tuple_store = new TupleKbTupleStore(this, default_use_hash_index_p);
	caching_policy = _agressive;
	this.initialize_tuple_kb();
      }
  }

  public static final Symbol _indexing_type = Symbol.keyword("INDEXING-TYPE");
  public static final Symbol _hash_index = Symbol.keyword("HASH-INDEX");
  public static final Symbol _full_index = Symbol.keyword("FULL-INDEX");
  public static final Symbol _no_index = Symbol.keyword("NO-INDEX");

  protected void initialize_from_init_plist(Node initargs) {
    super.initialize_from_init_plist(initargs);
    Enumerator e = initargs.enumerate_list();
    Node indexing_type = _full_index;
    if (default_use_hash_index_p) { indexing_type = _hash_index; }
    while (e.has_more_p()) {
      Node key = e.next();
      Node value = e.next();
      if (key == _indexing_type) { indexing_type = value; }
    }
    if (indexing_type == _hash_index)
      { tuple_store = new TupleKbTupleStore(this, true); }
    else if (indexing_type == _full_index)
      { tuple_store = new TupleKbTupleStore(this, false); }
    else if (indexing_type == _full_index)
      {
	tuple_store = new TupleKbTupleStore(this, false); 
	tuple_store.full_index_p = false;
      }
    else { throw new GenericError
	   (_T, new OKBCString("Unknown indexing type: " + 
			       indexing_type)).signal();
	 }
    initialize_tuple_kb();
  }


  public void register_side_effect (Object arg) {
    // System.out.println ("www = " + connection().ok_to_cache_flag);
    if (caching_policy == _agressive) { flush_cache("register_side_effect"); }
    super.register_side_effect(arg);
    // System.out.println ("zzz = " + connection().ok_to_cache_flag);
  }

  public void register_side_effect () {
    // System.out.println ("www = " + connection().ok_to_cache_flag);
    if (caching_policy == _agressive) { flush_cache("register_side_effect"); }
    super.register_side_effect();
    // System.out.println ("zzz = " + connection().ok_to_cache_flag);
  }

  protected static Node frs_name_internal (Kb kb_type, Connection connection) {
    return new OKBCString("Tuple KB");
  }

  protected Node get_kb_roots_internal
    (Node selector, TruthValue kb_local_only_p) {
    Node results = super.get_kb_roots_internal(selector, kb_local_only_p);
    if (selector == _user)
      { Enumerator e = results.enumerate_list();
	Node res = _NIL;
	while (e.has_more_p()) {
	  Node cl = e.next();
	  res = Cons.append_cons(get_user_roots(cl, kb_local_only_p), res);
	}
	return res;
      }
    else { return results; }
  }

  protected Node get_user_roots(Node cl, TruthValue kb_local_only_p) {
    Values2 framev = coerce_to_frame(cl, _NIL, kb_local_only_p);
    Node frame = framev.firstValue();
    if (framev.secondValue() != _NIL)
      {
	Node name = get_frame_name(frame, kb_local_only_p);
	if (Cons.member_eql(name, okbc_standard_names, null,
			    kb_local_only_p))
	  { 
	    Enumerator sube = get_class_subclasses
	      (frame, _direct, _all, kb_local_only_p)
	      .firstValue().enumerate_list();
	    Node results = _NIL;
	    while (sube.has_more_p()) {
	      Node sub = sube.next();
	      results = Cons.append_cons
		(get_user_roots(sub, kb_local_only_p), results);
	    }
	    return results;
	  }
	else { return Cons.list(frame); }
      }
    else { return _NIL; }
  }

  protected Node initialize_tuple_kb_class (Node class_name) {
    Node superclasses = Cons.assoc
      (class_name, okbc_standard_class_direct_superclass_alist);
    if (superclasses == null || superclasses == _NIL)
      { superclasses = Cons.assoc(_otherwise,
				  okbc_standard_class_direct_superclass_alist);
      }
    superclasses = superclasses.cdr();
    if (superclasses == null) { superclasses = _NIL; }
    Node real_supers = _NIL;
    Enumerator e = superclasses.enumerate_list();
    while (e.has_more_p()) {
      Node sup = e.next();
      if (Cons.member_eql(sup, okbc_standard_class_names, null, _NIL))
	{ initialize_tuple_kb_class(sup);
	  real_supers = new Cons(direct_coercible_to_frame_p
				 (sup, _NIL).firstValue(),
				 real_supers);
	}
    }
    if (direct_coercible_to_frame_p(class_name, _NIL).secondValue() != _NIL)
      { return _NIL; }
    else { return create_bootstrap_class(class_name, real_supers, _NIL); }
  }


  protected void initialize_tuple_kb () {
    fact_store = tuple_store;
    Enumerator classe = okbc_class_relation_symbols.enumerate_list();
    while (classe.has_more_p()) {
      Node class_name = classe.next();
      Node new_class = initialize_tuple_kb_class(class_name);
      // System.out.println("Created " + new_class + " for " + class_name + " for " + this);
      if (coerce_standard_name_to_frame
	  (class_name, _NIL, _NIL, false).secondValue() == _NIL)
	{ debug_node(Cons.list(class_name, new_class));
	  throw new RuntimeException("Consistency error.  Cannot find " +
				     class_name);
	}
    }
    classe = okbc_standard_class_names.enumerate_list();
    while (classe.has_more_p()) {
      Node class_name = classe.next();
      Node new_class = initialize_tuple_kb_class(class_name);
      // System.out.println("Created " + new_class + " for " + class_name + " for " + this);
      if (coerce_standard_name_to_frame
	  (class_name, _NIL, _NIL, false).secondValue() == _NIL)
	{ debug_node(Cons.list(class_name, new_class));
	  throw new RuntimeException("Consistency error.  Cannot find " +
				     class_name);
	}
    }
    // Now fix up :class so that it's a class and a thing.
    Node class_frame = class_frame(_NIL);
    Node frame_frame = frame_frame(_NIL);
    Node thing_frame = get_simple_frame_with_slot_value(__name, _thing, _NIL);
    tuple_store.insert(Cons.list(__instance_of, class_frame, class_frame),
		       true, false);
    tuple_store.insert(Cons.list(__instance_of, class_frame, frame_frame),
		       true, false);
    tuple_store.insert(Cons.list(__instance_of, class_frame, thing_frame),
		       true, false);

    tuple_store.insert(Cons.list(__instance_of, frame_frame, class_frame),
		       true, false);
    tuple_store.insert(Cons.list(__instance_of, frame_frame, frame_frame),
		       true, false);
    tuple_store.insert(Cons.list(__instance_of, frame_frame, thing_frame),
		       true, false);

    tuple_store.insert(Cons.list(__instance_of, thing_frame, class_frame),
		       true, false);
    tuple_store.insert(Cons.list(__instance_of, thing_frame, frame_frame),
		       true, false);
    tuple_store.insert(Cons.list(__instance_of, thing_frame, thing_frame),
		       true, false);
    classe = Cons.append_cons
      (okbc_class_relation_symbols,
       okbc_standard_class_names).enumerate_list();
    while (classe.has_more_p()) {
      Node name = classe.next();
      Node frame = get_simple_frame_with_slot_value(__name, name, _NIL);
      tuple_store.insert(Cons.list(__instance_of, frame, frame_frame),
			 true, false);
    }
    Enumerator slote = okbc_standard_slot_names.enumerate_list();
    while (slote.has_more_p()) {
      Node slot_name = slote.next();
      Node new_slot =
	create_reified_standard_non_class ( slot_name,  _slot, _NIL);
      // System.out.println("Created " + new_slot + " for " + slot_name + " for " + this);
      if (coerce_standard_name_to_frame
	  (slot_name, _NIL, _NIL, false).secondValue() == _NIL)
	{ debug_node(Cons.list(slot_name, new_slot));
	  throw new RuntimeException("Consistency error.  Cannot find " +
				     slot_name);
	}
    }
    Enumerator facete = okbc_standard_facet_names.enumerate_list();
    while (facete.has_more_p()) {
      Node facet_name = facete.next();
      Node new_facet =
	create_reified_standard_non_class (facet_name, _facet, _NIL);
      // System.out.println("Created " + new_facet + " for " + facet_name + " for " + this);
      if (coerce_standard_name_to_frame
	  (facet_name, _NIL, _NIL, false).secondValue() == _NIL)
	{ debug_node(Cons.list(facet_name, new_facet));
	  throw new RuntimeException("Consistency error.  Cannot find " +
				     facet_name);
	}
    }
  }

  Node coerce_to_frame_if_you_can (Node frame, 
				   TruthValue kb_local_only_p) {
    if (frame == null)
      { throw new NotCoercibleToFrame(frame, this).signal(); }
    else
      { Node coerced = coerce_to_frame(frame, _NIL, 
				       kb_local_only_p).firstValue();
	if (coerced == _NIL) { return frame; }
	else { return coerced; }
      }
  }

  void err_on_read_only_violation () {
    if (read_only_p)
      { throw new ReadOnlyViolation
	(this, _T,
	 new OKBCString(this + " is read-only.  (Re)Definitions and " +
		       "modifications are not allowed.")).signal();
      }
  }

  Node specific_type_from_frame_type
    (ContextName frame_type, TruthValue kb_local_only_p)
  {
    if (frame_type == _individual) { return individual_frame(kb_local_only_p);}
    else if (frame_type == _facet) { return facet_frame(kb_local_only_p);}
    else if (frame_type == _slot) { return slot_frame(kb_local_only_p);}
    else { throw new RuntimeException("Kernel class " + frame_type + 
				      " is missing.");
	 }
  }

  Node cache_of_frame_frame_local = null;
  Node cache_of_frame_frame_global = null;

  Node frame_frame (TruthValue kb_local_only_p) {
    if (kb_local_only_p == _NIL)
      { if (cache_of_frame_frame_global == null)
          { cache_of_frame_frame_global =
               get_simple_frame_with_slot_value
                   (__name, _frame, kb_local_only_p);
          }
        return cache_of_frame_frame_global;
      }
    else
      { if (cache_of_frame_frame_local == null)
          { cache_of_frame_frame_local =
               get_simple_frame_with_slot_value
                   (__name, _frame, kb_local_only_p);
          }
        return cache_of_frame_frame_local;
      }
  }

  Node cache_of_class_frame_local = null;
  Node cache_of_class_frame_global = null;

  Node class_frame (TruthValue kb_local_only_p) {
    if (kb_local_only_p == _NIL)
      { if (cache_of_class_frame_global == null)
          { cache_of_class_frame_global =
               get_simple_frame_with_slot_value
                   (__name, _class, kb_local_only_p);
          }
        return cache_of_class_frame_global;
      }
    else
      { if (cache_of_class_frame_local == null)
          { cache_of_class_frame_local =
               get_simple_frame_with_slot_value
                   (__name, _class, kb_local_only_p);
          }
        return cache_of_class_frame_local;
      }
  }

  Node cache_of_slot_frame_local = null;
  Node cache_of_slot_frame_global = null;

  Node slot_frame (TruthValue kb_local_only_p) {
    if (kb_local_only_p == _NIL)
      { if (cache_of_slot_frame_global == null)
          { cache_of_slot_frame_global =
               get_simple_frame_with_slot_value
                   (__name, _slot, kb_local_only_p);
          }
        return cache_of_slot_frame_global;
      }
    else
      { if (cache_of_slot_frame_local == null)
          { cache_of_slot_frame_local =
               get_simple_frame_with_slot_value
                   (__name, _slot, kb_local_only_p);
          }
        return cache_of_slot_frame_local;
      }
  }

  Node cache_of_facet_frame_local = null;
  Node cache_of_facet_frame_global = null;

  Node facet_frame (TruthValue kb_local_only_p) {
    if (kb_local_only_p == _NIL)
      { if (cache_of_facet_frame_global == null)
          { cache_of_facet_frame_global =
               get_simple_frame_with_slot_value
                   (__name, _facet, kb_local_only_p);
          }
        return cache_of_facet_frame_global;
      }
    else
      { if (cache_of_facet_frame_local == null)
          { cache_of_facet_frame_local =
               get_simple_frame_with_slot_value
                   (__name, _facet, kb_local_only_p);
          }
        return cache_of_facet_frame_local;
      }
  }

  Node cache_of_individual_frame_local = null;
  Node cache_of_individual_frame_global = null;

  Node individual_frame (TruthValue kb_local_only_p) {
    if (kb_local_only_p == _NIL)
      { if (cache_of_individual_frame_global == null)
          { cache_of_individual_frame_global =
               get_simple_frame_with_slot_value
                   (__name, _individual, kb_local_only_p);
          }
        return cache_of_individual_frame_global;
      }
    else
      { if (cache_of_individual_frame_local == null)
          { cache_of_individual_frame_local =
               get_simple_frame_with_slot_value
                   (__name, _individual, kb_local_only_p);
          }
        return cache_of_individual_frame_local;
      }
  }

  boolean trace_cache_activity_p = false;
  boolean inside_cache_hook = false;

  public void flush_cache (Object arg) {
    if (trace_cache_activity_p)
      { if (cache.value == null && cache.arcs == null)
	  { System.out.print("X"); }
	else { System.out.println
	       ("-- Flushing for " + arg + " ------------------------");
	     }
      }
    cache_of_frame_frame_local = null;
    cache_of_frame_frame_global = null;
    cache_of_class_frame_local = null;
    cache_of_class_frame_global = null;
    cache_of_slot_frame_local = null;
    cache_of_slot_frame_global = null;
    cache_of_facet_frame_local = null;
    cache_of_facet_frame_global = null;
    cache_of_individual_frame_local = null;
    cache_of_individual_frame_global = null;
    super.flush_cache (arg);
  }

  Node create_reified_standard_non_class
   (Node name, ContextName frame_type, TruthValue kb_local_only_p)
  {
    err_on_read_only_violation();
    Node frame = allocate_frame_handle(name, frame_type, _NIL);
    Node specific_type = specific_type_from_frame_type
      (frame_type,  kb_local_only_p);
    tuple_store.insert(Cons.list(__name, frame, name), true, false);
    tuple_store.insert
      (Cons.list(__instance_of, frame, frame_frame(kb_local_only_p)),
       true, false);
    tuple_store.insert(Cons.list(__instance_of, frame, specific_type), true,
		       false); 
    return frame;
  }


  Node create_reified_non_class
  (Node name, ContextName frame_type, Node direct_types, Node doc, 
   Node own_slots, Node own_facets, Node handle, Node pretty_name,
   TruthValue kb_local_only_p) {
    // System.out.println("Create reified non class: " + Cons.list(name, frame_type, direct_types, doc, own_slots, own_facets, handle, pretty_name));
    if (frame_type == _facet || frame_type == _class || 
	frame_type == _slot  || frame_type == _individual)
      {}
    else { throw new GenericError
	   (_T, new OKBCString("Illegal frame type: " + frame_type)).signal();
	 }
    if (Cons.member_eql(name, okbc_standard_names, null, kb_local_only_p))
      { if (direct_coercible_to_frame_p(name, kb_local_only_p).secondValue()
	    != _NIL)
	  { return create_reified_standard_non_class
	    (name, frame_type, kb_local_only_p); 
	  }
	else { throw new FrameAlreadyExists(name, this).signal(); }
      }
    boolean names_required_p = false;
    if (Cons.member_eql(_NIL, get_behavior_values(_frame_names_required), null,
			kb_local_only_p))
      {}
    else { names_required_p = true; }
    if (name == _NIL && names_required_p)
      { throw new GenericError
	(_T, new OKBCString("Frame names are required")).signal();
      }
    err_on_read_only_violation();
    Node frame = handle;
    if (frame == _NIL)
      { if (names_required_p)
	  { Values2 framev = coerce_to_frame(name, _NIL, kb_local_only_p);
	    Node existing = framev.firstValue();
	    Node found_p = framev.secondValue();
	    if (found_p != _NIL)
	      { frame = get_frame_handle(existing, kb_local_only_p); }
	    else { frame = allocate_frame_handle(name, frame_type, _NIL); }
	  }
	else { frame = allocate_frame_handle(name, frame_type, _NIL); }
      }
    Node specific_type = specific_type_from_frame_type
      (frame_type, kb_local_only_p);
    if (name != _NIL)
      { put_frame_name(frame, name, kb_local_only_p); }
    tuple_store.insert
      (Cons.list(__instance_of, frame, frame_frame(kb_local_only_p)),
       true, false);
    if (direct_types == _NIL || direct_types instanceof Cons)
      {}
    else { direct_types = Cons.list(direct_types); }
    Enumeration e = (Cons.remove_duplicates
		     (new Cons(specific_type, direct_types), _eql, this,
		      kb_local_only_p)).elements();
    while (e.hasMoreElements()) {
      Node type = (Node)(e.nextElement());
      // System.out.println("Add instance type: " + Cons.list(frame, type));
      add_instance_type(frame, type, kb_local_only_p);
    }
    if (doc != _NIL)
      { add_slot_value(frame, __documentation, doc, _equal, _own, _NIL,
		       _known_true, kb_local_only_p);
      }
    initialise_slots_and_facets(frame, own_slots, own_facets, _own,
				kb_local_only_p);
    if (pretty_name != _NIL)
      { put_frame_pretty_name(frame, pretty_name, kb_local_only_p); }
    // System.out.println("Frame is: " + frame);
    return frame;
  }


  Values2 coerce_literal_to_tuple_kb_frame
     (Node thing, TruthValue error_p, TruthValue kb_local_only_p) {
       if (thing == _NIL || thing == null)
	 { if (error_p != _NIL)
	     { throw new NotCoercibleToFrame(thing, this).signal(); }
	   else { return new Values2(_NIL, _NIL); }
	 }
       else if (Cons.member_eql
		(_NIL, get_behavior_values(_frame_names_required), null,
		 kb_local_only_p))
	 // frame names are not unique!
	 { if (error_p != _NIL)
	     { throw new NotCoercibleToFrame(thing, this).signal(); }
	   else { return new Values2(_NIL, _NIL); }
	 }
       else
	 { Node maybe_frame = get_simple_frame_with_slot_value
	   (__name, thing, kb_local_only_p);
	   if (maybe_frame != _NIL && maybe_frame != thing)
	     { return coerce_to_frame
	       (maybe_frame, error_p, kb_local_only_p);
	     }
	   else if (error_p != _NIL)
	     { throw new NotCoercibleToFrame(thing, this).signal(); }
	   else { return new Values2(_NIL, _NIL); }
	 }
     }
	       
  Values2 default_coerce_to_frame
     (Node thing, TruthValue error_p, TruthValue kb_local_only_p) {
       if (primitive_direct_instance_of_p
	   (thing, frame_frame(kb_local_only_p), kb_local_only_p))
	 // (get_simple_own_slot_value (thing, __name, kb_local_only_p)!= _NIL)
	 // It's not really a frame unless it's got a name, 
	 // otherwise it's a disembodied frame handle.
	 { return new Values2(thing, _T); }
       else if (error_p != _NIL)
	 { throw new NotCoercibleToFrame(thing, this).signal(); }
       else { return new Values2(_NIL, _NIL); }
     }

  void issue_warning(String string) {
    System.out.println(string);
  }

  protected Values2 coerce_standard_name_to_frame
     (Node thing, TruthValue error_p, TruthValue kb_local_only_p,
      boolean force_p)
  { Node maybe_frame = get_simple_frame_with_slot_value
    (__name, thing, kb_local_only_p);
    if (maybe_frame != _NIL && maybe_frame != thing)
      { return new Values2(maybe_frame, _T); }
    else if (!force_p) { return new Values2(_NIL, _NIL); }
    else { issue_warning("Consistency error:  Standard frame " + thing +
			 " is missing reasserting kernel ontology.");
	   initialize_tuple_kb();
	   Node maybe_frame2 = get_simple_frame_with_slot_value
	     (__name, thing, kb_local_only_p);
	   if (maybe_frame2 != _NIL && maybe_frame2 != thing)
	     { return coerce_to_frame
	       (maybe_frame2, error_p, kb_local_only_p);
	     }
	   else { throw new GenericError
		  (_NIL, new OKBCString("Consistency error:  Standard frame " +
				       " is missing.")).signal();
		}
	 }
  }
	       
  protected Values2 coerce_to_frame_internal
     (Node thing, TruthValue error_p, TruthValue kb_local_only_p) {
       // if (Cons.member_eql(_NIL, get_behavior_values(_frame_names_required), null, kb_local_only_p)) {} else { System.out.println(Cons.list(new OKBCString("Coerce"), thing, error_p)); }
       if (thing == null)
	 { if (error_p != _NIL)
	     { throw new NotCoercibleToFrame(thing, this).signal(); }
	   else { return new Values2(_NIL, _NIL); }
	 }
       else if (Cons.member_eql
	   (_NIL, get_behavior_values(_frame_names_required), null,
	    kb_local_only_p))
	 { if (Cons.member_eql(thing, okbc_standard_names, null,
			       kb_local_only_p))
	       { return coerce_standard_name_to_frame
		 (thing, error_p, kb_local_only_p, true);
	       }
	     else
	       { return default_coerce_to_frame
		 (thing, error_p, kb_local_only_p);
	       }
	 }
       else if (Cons.member_eql(thing, okbc_standard_names, null,
				kb_local_only_p))
	 { return coerce_standard_name_to_frame
		 (thing, error_p, kb_local_only_p, true);
	 }
       else if (thing instanceof OKBCString || thing instanceof OKBCInteger ||
		thing instanceof AbstractSymbol)
	 { return coerce_literal_to_tuple_kb_frame
	   (thing, error_p, kb_local_only_p);
	 }
       else { return default_coerce_to_frame
	      (thing, error_p, kb_local_only_p);
	    }
     }


  Node get_simple_own_slot_value (Node frame, Node slot, 
				  TruthValue kb_local_only_p){
    Node match = tuple_store.fetch_one_binding
      ((Cons)(Cons.list(slot, frame, _value_var)),
       (kb_local_only_p == _NIL), 1 , sf_V);
// System.out.println("$$Slot=" + slot + " Value=" + match + " Frame=" +frame);
    if (match == __fail) { return _NIL; }
    else { return Cons.assoc(_value_var, match).cdr(); }
  }

  Node get_simple_frame_with_slot_value (Node slot, Node value, 
					 TruthValue kb_local_only_p){
    Node match = tuple_store.fetch_one_binding
      ((Cons)(Cons.list(slot, _frame_var, value)),
       (kb_local_only_p == _NIL), 1 , s_Fv);
// System.out.println("%%Slot=" + slot + " Value=" + value + " Frame=" +match);
    if (match == __fail) { return _NIL; }
    else { return Cons.assoc(_frame_var, match).cdr(); }
  }


  protected Node allocate_frame_handle_internal
         (Node frame_name, ContextName frame_type, Node frame_handle) 
  {
    // ignore the frame type for tuple kbs.
    FrameHandle fh = null;
    if (frame_handle instanceof FrameHandle) 
      { fh = (FrameHandle)frame_handle; }
    else if (Cons.member_eql (frame_name, okbc_standard_names, null, _NIL))
      { fh = FrameHandle.find_or_create_frame_handle(frame_name, this); }
    else if (Cons.member_eql
	     (_NIL, get_behavior_values(_frame_names_required), null, _NIL))
      { fh = FrameHandle.create_frame_handle(frame_handle, this); }
    else
      { // return FrameHandle.find_or_create_frame_handle(frame_handle, this);
	fh = FrameHandle.create_frame_handle(frame_handle, this);
      }
    if (fh.thing == FrameHandle.undefinedValue)
      { fh.thing = fh; }
    /*
    if (frame_type == _slot)
      {
	System.out.println("Allocating for " +  frame_name); 
	// if (frame_name == Symbol.intern("ABBREV", "OKBC-USER"))
	// { throw new RuntimeException ("?"); }
      }
    */
    return fh;
  }

  Node decontextualize_cons(Node value, ContextName from_context) {
    Cons tail = null;
    Cons root = null;
    while (value != _NIL && value != null) {
      if (value instanceof Cons)
	{ Cons new_tail = new Cons(decontextualize_internal
				   (value.car(), from_context));
	  if (root == null)
	    { root = new_tail;
	      tail = (Cons)root;
	    }
	  else { tail.set_cdr(new_tail); 
		 tail = new_tail;
	       }
	  value = value.cdr();
	}
      else { tail.set_cdr(decontextualize_internal (value, from_context)); 
	     value = _NIL;
	   }
    }
    return root;
  }

  Node find_in_list(Node name, Node list) {
    if (list == _NIL || list == null) { return _NIL; }
    else { Node car = list.car();
	   if (car == name) { return car; }
	   else { return find_in_list(name, list.cdr()); }
	 }
  }

  protected Node decontextualize_internal
     (Node value, ContextName from_context) 
  {
    if (value instanceof FrameHandle &&
	(from_context == _slot ||
	 from_context == _facet ||
	 from_context == _frame))
      { Values2 valuev = coerce_to_frame(value, _NIL, _NIL);
	if (valuev.secondValue() == _NIL) { return value; }
	{ Node name = get_frame_name(valuev.firstValue(), _NIL);
	  if (from_context == _slot)
	    { Node match = find_in_list(name, okbc_standard_slot_names);
	      if (match == _NIL) { return value; }
	      else { return match; }
	    }
	  else if (from_context == _facet)
	    { Node match = find_in_list(name, okbc_standard_facet_names);
	      if (match == _NIL) { return value; }
	      else { return match; }
	    }
	  else
	    { Node match = find_in_list(name, okbc_standard_names);
	      if (match == _NIL) { return value; }
	      else { return match; }
	    }
	}
      }
    else if (value instanceof Cons)
      { return decontextualize_cons(value, from_context); }
    else { return value; }
  }

  protected Node get_frame_name_internal
     (Node thing, TruthValue kb_local_only_p) {
       Values2 vals = coerce_to_frame(thing, _NIL, kb_local_only_p);
       Node frame = vals.firstValue();
       Node found_p = vals.secondValue();
       if (found_p != _NIL)
	 { Node name = get_simple_own_slot_value(frame, __name,
						 kb_local_only_p);
	   if (name != _NIL) { return name; }
	   if (Cons.member_eql
	       (_NIL, get_behavior_values(_frame_names_required), null,
		kb_local_only_p))
	     { return _NIL; }
	   else 
	     { FrameHandle.print_frame_names_with_handles_p = false;
	       System.out.println("Frame " + frame + " should have a name:");
	       debug_node(Cons.list(frame));
	       throw new GenericError
	       (_T, new OKBCString("This frame has no name, but the " +
				  ":frame-names-required requires that " +
				  "there be a name.")).signal();
	     }
	 }
       else { throw new NotCoercibleToFrame(thing, this).signal(); }
    }

  protected Node put_frame_name_internal
     (Node frame, Node new_name, TruthValue kb_local_only_p) {
       Node existing = get_simple_own_slot_value(frame, __name,
						 kb_local_only_p);
       if (existing != _NIL)
	 { put_slot_value(frame, __name, new_name, _own, _known_true,
			  kb_local_only_p);
	 }
       else 
	 { add_slot_value(frame, __name, new_name, _equal, _own, _NIL,
			  _known_true, kb_local_only_p);
	 }
       return frame;
    }

  protected Node handle_unhandled_sentence
         (Node sentence, Node frame, ValueSelector value_selector,
	  TruthValue kb_local_only_p, Node key) {
    Node real_frame = _NIL;
    if (frame != _NIL)
      { real_frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();}
    if (key == _tell) 
      { tuple_store.insert(sentence, true, false); 
	if (real_frame != _NIL &&
	    Cons.is_in_tree_p(real_frame, sentence, this))
	  { tuple_store.insert(Cons.list(__has_sentence, real_frame,
					 Cons.list(__quote, sentence)),
			       true, false);
	  }
      }
    else if (key == _untell)
      { if (tuple_store.fetch((Cons)sentence, (kb_local_only_p == _NIL),
			      sentence, 0, 100000000, null).firstValue()
	    != __fail)
	  { tuple_store.drop(sentence, false, (kb_local_only_p == _NIL)); 
	    return _T;
	  }
	else { return _NIL; }
      }
    return _T;
  }

  protected Values2 handle_unhandled_query(Node query, 
				       InferenceLevel inference_level,
				       TruthValue error_p,
				       ValueSelector value_selector,
				       TruthValue kb_local_only_p, 
				       Node bindings) {
    Node vars = Cons.toList(TupleStore.variables_in(query));
    if (query.car() == _holds) { query = query.cdr(); }
    Node results = tuple_store.fetch((Cons)query, (kb_local_only_p == _NIL),
				     vars, 0, 100000000, null).firstValue();
    if (results == __fail)
      { return new Values2(__fail, _T); }
    else { Node res = _NIL;
	   Enumeration e = results.elements();
	   while(e.hasMoreElements()) {
	     Node result = (Node)(e.nextElement());
	     Node this_contribution = _NIL;
	     Enumeration varse = vars.elements();
	     Enumeration resulte = result.elements();
	     while(varse.hasMoreElements()) {
	       this_contribution =
		 new Cons(new Cons((Node)(varse.nextElement()),
				   (Node)(resulte.nextElement())),
			  this_contribution);
	     }
	     res = new Cons(Cons.append_cons(this_contribution, bindings),
			    res);
	   }
	   return new Values2(res, _NIL);
	 }
  }


  protected Values2 handle_simple_query
	(Node query, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, 
	 ConsLocation all_binding_lists)
  {
    Node kif_or_okbc_entry = _NIL;
    Node sym = Cons.first_if_list(query);
    if (sym instanceof AbstractSymbol)
      {
	kif_or_okbc_entry = TellAndAsk.find_sym_in_list
	  ((AbstractSymbol)sym, kif_operator_symbols);
	if (kif_or_okbc_entry == _NIL)
	  { kif_or_okbc_entry = 
	    TellAndAsk.find_sym_in_list((AbstractSymbol)sym,
					evaluable_predicate_symbols); 
	  }
      }
    if (inference_level == _direct && tuple_store.full_index_p &&
	(kif_or_okbc_entry == _NIL) && !(sym instanceof Procedure))
      { Node vars = Cons.toList(TupleStore.variables_in(query));
	Node reformulated = query;
	if (_holds == query.car())
	  { reformulated = (Node)query.cdr(); }
	else if (query.length() == 2)
	  {reformulated = Cons.list(__instance_of, query.cadr(), query.car());}
	else { reformulated = query; }
	if (Cons.member_eql(_NIL, get_behavior_values(_frame_names_required),
			    null, kb_local_only_p))
	  {}
	else { reformulated = coerce_to_kb_value(reformulated,
						 _value, _NIL, _NIL, _T,
						 _error_if_not_unique,
						 kb_local_only_p)
	       .firstValue();
	     }
	Node pattern = Cons.sublis(tuplekb_translation_alist,
				   reformulated, this);
	Node results = tuple_store.fetch((Cons)pattern,
					 (kb_local_only_p == _NIL),
					 vars, 0, 100000000, 
					 null).firstValue();
	//System.out.println("results = " + results + " pattern = " + pattern);
	Values2 res = null;
	if (results == __fail)
	  { res = new Values2(__fail, _T); }
	else
	  { Node filtered = _NIL;
	    Enumerator e = results.enumerate_list();
	    while (e.has_more_p()) {
	      Node result = e.next();
	      Enumerator resulte = result.enumerate_list();
	      boolean found_p = false;
	      while (resulte.has_more_p()) {
		Node x = resulte.next();
		if (Cons.member_eql(x, non_okbc_mapping_internal_relations,
				    null, kb_local_only_p))
		  { found_p = true; break; }
	      }
	      if (!found_p)
		{ filtered = new Cons(Cons.append_cons(Cons.mapcar_cons
						       (vars, result),
						       bindings),
				      filtered);
		}
	    }
	    if (filtered != _NIL || results == _NIL || results == null)
	      { res = new Values2(Cons.sublis(tuplekb_backtranslation_alist,
					      filtered, this),
				  _T);
	      }
	    else { res = new Values2(__fail, _T); }
	  }
//	System.out.println("handle_simple_query(" +
//			   Cons.list(query, inference_level, error_p,
//				     value_selector, kb_local_only_p,
//				     bindings, all_binding_lists.cons) +
//			   " => " + res);
	return res;
      }
    else { return super.handle_simple_query
	   (query, inference_level, error_p, value_selector, kb_local_only_p,
	    bindings, all_binding_lists);
	 }
  }


  protected Values3 get_frame_sentences_internal
     (Node frame, Node number_of_values, Node okbc_sentences_p,
      ValueSelector value_selector, TruthValue kb_local_only_p) {
       frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
       Node facts = tuple_store.facts_full_indexed_under(frame);
       Node filtered_facts = _NIL;
       if (facts == null) { return new Values3(_NIL, _T, _NIL); }
       if (value_selector == _either)
	 { Enumeration factse = facts.elements(); 
	   while (factse.hasMoreElements()) {
	     Node fact = (Node)(factse.nextElement());
	     Node entry = Cons.assoc
	       (fact.car(), tuplekb_defaulting_relations_alist).cadr();
	     Node thing_to_collect = null;
	     if (__has_sentence == fact.car())
	       { thing_to_collect = TellAndAsk.dequote(fact.nth(2)); }
	     else if (entry == _NIL || entry == null)
	       { thing_to_collect = fact; }
	     else if (entry == __none) { thing_to_collect = fact.cdr(); }
	     else { thing_to_collect = new Cons(entry, fact.cdr()); }
	     filtered_facts = new Cons(thing_to_collect, filtered_facts);
	   }
	 }
       else if (value_selector == _default)
	 { Enumeration factse = facts.elements(); 
	   while (factse.hasMoreElements()) {
	     Node fact = (Node)(factse.nextElement());
	     Node entry = Cons.assoc
	       (fact.car(), tuplekb_defaulting_relations_alist).cadr();
	     if (entry != _NIL && entry != null)
	       { Node thing_to_collect = null;
		 if (__has_sentence == fact.car())
		   { thing_to_collect = TellAndAsk.dequote(fact.nth(2)); }
		 else if (entry == __none) { thing_to_collect = fact.cdr(); }
		 else { thing_to_collect = new Cons(entry, fact.cdr()); }
		 filtered_facts = new Cons(thing_to_collect, filtered_facts);
	       }
	   }
	 }
       else if (value_selector == _known_true)
	 { Enumeration factse = facts.elements(); 
	   while (factse.hasMoreElements()) {
	     Node fact = (Node)(factse.nextElement());
	     Node entry = Cons.assoc
	       (fact.car(), tuplekb_defaulting_relations_alist).cadr();
	     if (entry == _NIL || entry == null)
	       { Node thing_to_collect = null;
		 if (__has_sentence == fact.car())
		   { thing_to_collect = TellAndAsk.dequote(fact.nth(2)); }
		 else { thing_to_collect = fact; }
		 filtered_facts = new Cons(thing_to_collect, filtered_facts);
	       }
	   }
	 }
       Node translated = Cons.sublis(tuplekb_backtranslation_alist,
				     filtered_facts, this);
       if (okbc_sentences_p == _NIL)
	 { Enumeration e = translated.elements();
	   Node res = _NIL;
	   while(e.hasMoreElements()) {
	     Node s = (Node)(e.nextElement());
	     if (!(TellAndAsk.okbc_sentence_p(s, this)))
	       { res = new Cons(s, res); }
	   }
	   return new Values3(res, _T, _NIL);
	 }
       else { return new Values3(translated, _T, _NIL); }
     }

  protected Node get_frame_pretty_name_internal
     (Node frame, TruthValue kb_local_only_p) {
       frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
       Node res = get_simple_own_slot_value(frame, _pretty_name,
					    kb_local_only_p);
       if (res == _NIL)
	 { Node name = get_frame_name(frame, kb_local_only_p);
	   if (name instanceof AbstractSymbol)
	     // { res = new OKBCString(((AbstractSymbol)name).symbolName); }
	     { res = OKBCString.capitalize(((AbstractSymbol)name).symbolName);}
	   else { res = new OKBCString(name.toString()); }
	 }
       // System.out.println("Pretty name of " + frame + " is " + res);
       return res;
    }

  protected void put_frame_pretty_name_internal
     (Node frame, Node name, TruthValue kb_local_only_p) {
       OKBCString real_name = null;
       if (name instanceof OKBCString)
	 { real_name = (OKBCString) name; }
       else if (name instanceof AbstractSymbol)
	 { real_name = new OKBCString(((AbstractSymbol)name).symbolName); }
       else { throw new GenericError
	      (_T, new OKBCString(name + " is not coercible to a string."))
	      .signal();
	    }
       put_slot_value(frame, _pretty_name, real_name, _own, _known_true, 
		      kb_local_only_p);
    }

  protected Values2 coerce_to_class_internal (Node thing, TruthValue error_p,
					   TruthValue kb_local_only_p) {
    Node frame = null;
    frame = coerce_to_frame(thing, _NIL, kb_local_only_p).firstValue();
    if (frame == _NIL) { frame = thing; }
    Node class_frame = class_frame(kb_local_only_p);
    if (primitive_direct_instance_of_p(frame, class_frame, kb_local_only_p))
      { return new Values2(frame, _T); }
    else if (error_p != _NIL)
      { System.out.println("Foo " + class_frame + " frame= " + frame);
	debug_node(Cons.list(class_frame, frame, thing));
	throw new ClassNotFound(frame, this).signal(); }
    else { return new Values2(_NIL, _NIL); }
  }

  protected Values2 coerce_to_slot_internal (Node thing, TruthValue error_p,
					  TruthValue kb_local_only_p) {
    Node frame = null;
    frame = coerce_to_frame(thing, _NIL, kb_local_only_p).firstValue();
    if (frame == _NIL) { frame = thing; }
    Node slot_frame = slot_frame(kb_local_only_p);
    if (primitive_direct_instance_of_p(frame, slot_frame, kb_local_only_p))
      { return new Values2(frame, _T); }
    else if (error_p != _NIL)
      { throw new SlotNotFound(_NIL, frame, _NIL, this).signal(); }
    else { return new Values2(_NIL, _NIL); }
  }

  protected Values2 coerce_to_facet_internal (Node thing, TruthValue error_p,
					   TruthValue kb_local_only_p) {
    Node frame = null;
    frame = coerce_to_frame(thing, _NIL, kb_local_only_p).firstValue();
    if (frame == _NIL) { frame = thing; }
    Node facet_frame = facet_frame(kb_local_only_p);
    if (primitive_direct_instance_of_p(frame, facet_frame, kb_local_only_p))
      { return new Values2(frame, _T); }
    else if (error_p != _NIL)
      { throw new FacetNotFound(_NIL, _NIL, frame, _NIL, this).signal(); }
    else { return new Values2(_NIL, _NIL); }
  }

  boolean primitive_direct_instance_of_p 
    (Node instance, Node type, TruthValue kb_local_only_p) {
      if (type instanceof AbstractSymbol)
	{ type = get_simple_frame_with_slot_value
	  (__name, type, kb_local_only_p);
	}
      Node res = tuple_store.fetch_one_binding
	((Cons)(Cons.list(__instance_of, instance, type)),
	 (kb_local_only_p == _NIL), sfv);
      // System.out.println("primitive_direct_instance_of_p " + Cons.list(instance, type, res));
      if (res != __fail)
       { return true; }
      else { return false; }
  }

  Values4 get_direct_own_slot_values_in_detail
  (Node frame, Node slot, Node number_of_values, ValueSelector value_selector,
   TruthValue kb_local_only_p) {
    int numvals = 1000000;
    if (number_of_values != _all)
      { numvals = (int)(((OKBCInteger)number_of_values).number); }
    if (value_selector == _known_true)
      { Values2 v = tuple_store.fetch
	((Cons)(Cons.list(slot, frame, _value_var)),
	 (kb_local_only_p == _NIL), _value_var, 1, 
	 numvals, sf_V);
	Node res = v.firstValue();
	Node more = v.secondValue();
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	Node all_results = _NIL;
	Enumeration e = res.elements();
	while(e.hasMoreElements()) {
	  Node x = (Node)(e.nextElement());
	  if (x != __deleted)
	    { all_results = new Cons(Cons.list(x, _T, _NIL), all_results); }
	}
	return new Values4(all_results, _T, more_status, _NIL);
      }
    else if (value_selector == _default_only)
      { Node match = tuple_store.fetch_one_binding
	    ((Cons)(Cons.list(__default_slot_value, slot, frame, __deleted)),
	     (kb_local_only_p == _NIL), fasfv);
	// System.out.println("@@@@ " + match);
	if (match == __fail)
	  { return new Values4(_NIL, _T, _NIL, _NIL); }
	else { return new Values4(_NIL, _T, _NIL, _T); }
      }
    else // (value_selector == _either)
      { return get_direct_own_slot_values_in_detail
	(frame, slot, number_of_values, _known_true, kb_local_only_p);
      }
  }


  Values4 get_direct_template_slot_values_in_detail
  (Node frame, Node slot, Node number_of_values, ValueSelector value_selector,
   TruthValue kb_local_only_p) {
    int numvals = 1000000;
    if (number_of_values != _all)
      { numvals = (int)(((OKBCInteger)number_of_values).number); }
    if (value_selector == _known_true)
      { Values2 v = tuple_store.fetch
	((Cons)(Cons.list(__template_slot_value, slot, frame, _value_var)),
	 (kb_local_only_p == _NIL), _value_var, 2, 
	 numvals, fasf_V);
	Node res = v.firstValue();
	Node more = v.secondValue();
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	Node all_results = _NIL;
	Enumeration e = res.elements();
	while(e.hasMoreElements()) {
	  Node x = (Node)(e.nextElement());
	  all_results = new Cons(Cons.list(x, _T, _NIL), all_results);
	}
	return new Values4(all_results, _T, more_status, _NIL);
      }
    else if (value_selector == _default_only)
      { Values2 v = tuple_store.fetch
	((Cons)(Cons.list(__default_template_slot_value, slot, frame, 
			  _value_var)),
	 (kb_local_only_p == _NIL), _value_var, 2, 
	 numvals, fasf_V);
	Node res = v.firstValue();
	Node more = v.secondValue();
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	if (res == _NIL || res == null)
	  { return new Values4(_NIL, _T, more_status, _NIL); }
	else if (res.cdr() != _NIL && res.cdr() != null)
	  { throw new GenericError
	      (_T, new OKBCString("Internal consistency error - too many " +
				 "defaults:" + res)).signal();
	  }
	else if (res.car() == __deleted)
	  { return new Values4(_NIL, _T, _NIL, _T); }
	else { return new Values4(Cons.list(Cons.list(res.car(), _T, _T)),
				 _T, _NIL, _T); 
	     }
      }
    else // (value_selector == _either)
      { Values4 v = get_direct_template_slot_values_in_detail
	  (frame, slot, number_of_values, _known_true, kb_local_only_p);
	Node values = v.firstValue();
	Node exact_p = v.secondValue();
	Node more_status = v.thirdValue();
	if (values != _NIL) 
	  { return new Values4(values, exact_p, more_status, _NIL); }
	else
	  { return get_direct_template_slot_values_in_detail
	      (frame, slot, number_of_values, _default_only, kb_local_only_p);
	  }
      }
  }


  protected Values4 get_slot_values_in_detail_internal
         (Node frame, Node slot, InferenceLevel inference_level,
	  SlotType slot_type, Node number_of_values,
	  ValueSelector value_selector, TruthValue kb_local_only_p) {
    if (inference_level == _direct)
      { Node real_frame = coerce_to_frame(frame, _NIL, 
					  kb_local_only_p).firstValue();
	if (real_frame == _NIL) { real_frame = frame; }
	if (real_frame == _NIL)
	  { throw new NotCoercibleToFrame(real_frame, this).signal(); }
	Node real_slot = coerce_to_slot(slot, _NIL, 
					kb_local_only_p).firstValue();
	if (real_slot == _NIL) { real_slot = slot; }
	if (real_slot == _NIL)
	  { throw new SlotNotFound(_NIL, real_slot, _NIL, this).signal(); }
	Values4 result = null;
	if (slot_type == _own)
	  { result = get_direct_own_slot_values_in_detail
	    (real_frame, real_slot, number_of_values, value_selector,
	     kb_local_only_p);
	  }
	else if (slot_type == _template)
	  { result = get_direct_template_slot_values_in_detail
	    (real_frame, real_slot, number_of_values, value_selector, 
	     kb_local_only_p);
	  }
	else
 	  { throw new GenericError
	      (_T, new OKBCString("Illegal slot type " + slot_type)).signal();
	  }
	// System.out.println("***" + Cons.list(real_frame, real_slot, slot_type, value_selector) + " -> " + result);
	return result;
      }
    else { return super.get_slot_values_in_detail_internal
	   (frame, slot, inference_level, slot_type, number_of_values,
	    value_selector, kb_local_only_p);
	 }
  }


  Values4 get_direct_own_facet_values_in_detail
  (Node frame, Node slot, Node facet, Node number_of_values,
   ValueSelector value_selector, TruthValue kb_local_only_p) {
    int numvals = 1000000;
    if (number_of_values != _all)
      { numvals = (int)(((OKBCInteger)number_of_values).number); }
    if (value_selector == _known_true)
      { Values2 v = tuple_store.fetch
	((Cons)(Cons.list(facet, slot, frame, _value_var)),
	 (kb_local_only_p == _NIL), _value_var, 2, 
	 numvals, fasf_V);
	Node res = v.firstValue();
	Node more = v.secondValue();
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	Node all_results = _NIL;
	Enumeration e = res.elements();
	while(e.hasMoreElements()) {
	  Node x = (Node)(e.nextElement());
	  if (x != __deleted)
	    { all_results = new Cons(Cons.list(x, _T, _NIL), all_results); }
	}
	return new Values4(all_results, _T, more_status, _NIL);
      }
    else if (value_selector == _default_only)
      { if (tuple_store.fetch_one_binding
	    ((Cons)(Cons.list(__default_facet_value, facet, slot, frame,
			      __deleted)),
	     (kb_local_only_p == _NIL), rfasfv)
	    == __fail)
	  { return new Values4(_NIL, _T, _NIL, _NIL); }
	else { return new Values4(_NIL, _T, _NIL, _T); }
      }
    else // (value_selector == _either)
      { return get_direct_own_facet_values_in_detail
	(frame, slot, facet, number_of_values, _known_true, kb_local_only_p);
      }
  }


  Values4 get_direct_template_facet_values_in_detail
  (Node frame, Node slot, Node facet, Node number_of_values,
   ValueSelector value_selector, TruthValue kb_local_only_p) {
    int numvals = 1000000;
    if (number_of_values != _all)
      { numvals = (int)(((OKBCInteger)number_of_values).number); }
    if (value_selector == _known_true)
      { Values2 v = tuple_store.fetch
	((Cons)(Cons.list(__template_facet_value, facet, slot, frame,
			  _value_var)),
	 (kb_local_only_p == _NIL), _value_var, 3,
	 numvals, rfasf_V);
	Node res = v.firstValue();
	Node more = v.secondValue();
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	Node all_results = _NIL;
	Enumeration e = res.elements();
	while(e.hasMoreElements()) {
	  Node x = (Node)(e.nextElement());
	  all_results = new Cons(Cons.list(x, _T, _NIL), all_results);
	}
	return new Values4(all_results, _T, more_status, _NIL);
      }
    else if (value_selector == _default_only)
      { Values2 v = tuple_store.fetch
	((Cons)(Cons.list(__default_template_facet_value, facet, slot, frame, 
			  _value_var)),
	 (kb_local_only_p == _NIL), _value_var, 3, 
	 numvals, rfasf_V);
	Node res = v.firstValue();
	Node more = v.secondValue();
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	if (res == _NIL || res == null)
	  { return new Values4(_NIL, _T, more_status, _NIL); }
	else if (res.cdr() != _NIL && res.cdr() != null)
	  { throw new GenericError
	      (_T, new OKBCString("Internal consistency error - too many " +
				 "defaults:" + res)).signal();
	  }
	else if (res.car() == __deleted)
	  { return new Values4(_NIL, _T, _NIL, _T); }
	else { return new Values4(Cons.list(Cons.list(res.car(), _T, _T)),
				 _T, _NIL, _T); 
	     }
      }
    else // (value_selector == _either)
      { Values4 v = get_direct_template_facet_values_in_detail
	  (frame, slot, facet, number_of_values, _known_true, kb_local_only_p);
	Node values = v.firstValue();
	Node exact_p = v.secondValue();
	Node more_status = v.thirdValue();
	if (values != _NIL) 
	  { return new Values4(values, exact_p, more_status, _NIL); }
	else
	  { return get_direct_template_facet_values_in_detail
	      (frame, slot, facet, number_of_values, _default_only,
	       kb_local_only_p);
	  }
      }
  }


  protected Values4 get_facet_values_in_detail_internal
         (Node frame, Node slot, Node facet, InferenceLevel inference_level,
	  SlotType slot_type, Node number_of_values,
	  ValueSelector value_selector, TruthValue kb_local_only_p) {
    if (inference_level == _direct)
      { Node real_frame = coerce_to_frame(frame, _NIL, 
					  kb_local_only_p).firstValue();
	if (real_frame == _NIL) { real_frame = frame; }
	if (real_frame == _NIL)
	  { throw new NotCoercibleToFrame(real_frame, this).signal(); }
	Node real_slot = coerce_to_slot(slot, _NIL, 
					kb_local_only_p).firstValue();
	if (real_slot == _NIL) { real_slot = slot; }
	if (real_slot == _NIL)
	  { throw new SlotNotFound(_NIL, real_slot, _NIL, this).signal(); }
	Node real_facet = coerce_to_facet(facet, _NIL, 
					kb_local_only_p).firstValue();
	if (real_facet == _NIL) { real_facet = facet; }
	if (real_facet == _NIL)
	  { throw new FacetNotFound(_NIL, _NIL, real_facet, _NIL, 
				    this).signal(); }
	if (slot_type == _own)
	  { return get_direct_own_facet_values_in_detail
	    (real_frame, real_slot, real_facet, number_of_values,
	     value_selector, kb_local_only_p);
	  }
	else if (slot_type == _template)
	  { return get_direct_template_facet_values_in_detail
	    (real_frame, real_slot, real_facet, number_of_values, 
	     value_selector, kb_local_only_p);
	  }
	else
 	  { throw new GenericError
	      (_T, new OKBCString("Illegal slot type " + slot_type)).signal();
	  }
      }
    else { return super.get_facet_values_in_detail_internal
	   (frame, slot, facet, inference_level, slot_type, number_of_values,
	    value_selector, kb_local_only_p);
	 }
  }


  protected Values2 get_frames_with_slot_value_internal
       (Node slot, Node value, InferenceLevel inference_level,
	SlotType slot_type, ValueSelector value_selector,
	TruthValue kb_local_only_p) {
   if (inference_level != _direct)
     { return super.get_frames_with_slot_value_internal
       (slot, value, inference_level, slot_type, value_selector,
	kb_local_only_p);
     }
   if (slot_type == _all)
     { Values2 ownv = get_frames_with_slot_value_internal
         (slot, value, inference_level, _own, value_selector,
	  kb_local_only_p);
       Values2 templatev = get_frames_with_slot_value_internal
         (slot, value, inference_level, _template, value_selector,
	  kb_local_only_p);
       return new Values2(Cons.append_cons(ownv.firstValue(),
					   templatev.firstValue()),
			  ((ownv.secondValue() != _NIL &&
			    templatev.secondValue() != _NIL)
			   ? (Node)_T
			   : (Node)_NIL));
     }
   else if (slot_type == _own)
     { return get_own_direct_frames_with_slot_value
       (slot, value, value_selector, kb_local_only_p);
     }
   else // _template
     { return get_template_direct_frames_with_slot_value
       (slot, value, value_selector, kb_local_only_p);
     }
  }

  Values2 get_own_direct_frames_with_slot_value
       (Node slot, Node value, ValueSelector value_selector,
	TruthValue kb_local_only_p)
  { if (value_selector == _known_true)
      { Values2 v = 
	tuple_store.fetch((Cons)(Cons.list(slot, _frame_var, value)),
			 (kb_local_only_p == _NIL), _frame_var, 0, 
			 100000000, s_Fv);
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	return new Values3(v.firstValue(), _T, more_status);
      }
    else if (value_selector == _default_only)
      { Values2 v = 
	tuple_store.fetch((Cons)(Cons.list(__default_slot_value, slot,
					   _frame_var, value)),
			 (kb_local_only_p == _NIL), _frame_var, 0, 
			 100000000, fas_Fv);
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	return new Values3(v.firstValue(), _T, more_status);
      }
    else // _either
     { Values2 knownv = get_frames_with_slot_value_internal
         (slot, value, _direct, _own, _known_true, kb_local_only_p);
       Values2 defaultv = get_frames_with_slot_value_internal
         (slot, value, _direct, _own, _default_only, kb_local_only_p);
       return new Values2(Cons.append_cons(knownv.firstValue(),
					   defaultv.firstValue()),
			  ((knownv.secondValue() != _NIL &&
			    defaultv.secondValue() != _NIL)
			   ? (Node)_T
			   : (Node)_NIL));
     }
  }

  Values2 get_template_direct_frames_with_slot_value
       (Node slot, Node value, ValueSelector value_selector,
	TruthValue kb_local_only_p)
  { if (value_selector == _known_true)
      { Values2 v = 
	tuple_store.fetch((Cons)(Cons.list(__template_slot_value,
					   slot, _frame_var, value)),
			 (kb_local_only_p == _NIL), _frame_var, 0, 
			 100000000, fas_Fv);
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	return new Values3(v.firstValue(), _T, more_status);
      }
    else if (value_selector == _default_only)
      { Values2 v = 
	tuple_store.fetch((Cons)(Cons.list(__default_template_slot_value,
					   slot, _frame_var, value)),
			 (kb_local_only_p == _NIL), _frame_var, 0, 
			 100000000, fas_Fv);
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	return new Values3(v.firstValue(), _T, more_status);
      }
    else // _either
     { Values2 knownv = get_frames_with_slot_value_internal
         (slot, value, _direct, _template, _known_true, kb_local_only_p);
       Values2 defaultv = get_frames_with_slot_value_internal
         (slot, value, _direct, _template, _default_only, kb_local_only_p);
       return new Values2(Cons.append_cons(knownv.firstValue(),
					   defaultv.firstValue()),
			  ((knownv.secondValue() != _NIL &&
			    defaultv.secondValue() != _NIL)
			   ? (Node)_T
			   : (Node)_NIL));
     }
  }

  protected Values2 get_frames_with_facet_value_internal
       (Node slot, Node facet, Node value, InferenceLevel inference_level,
	SlotType slot_type, ValueSelector value_selector,
	TruthValue kb_local_only_p) {
   if (inference_level != _direct)
     { return super.get_frames_with_facet_value_internal
       (slot, facet, value, inference_level, slot_type, value_selector,
	kb_local_only_p);
     }
   if (slot_type == _all)
     { Values2 ownv = get_frames_with_facet_value_internal
         (slot, facet, value, inference_level, _own, value_selector,
	  kb_local_only_p);
       Values2 templatev = get_frames_with_facet_value_internal
         (slot, facet, value, inference_level, _template, value_selector,
	  kb_local_only_p);
       return new Values2(Cons.append_cons(ownv.firstValue(),
					   templatev.firstValue()),
			  ((ownv.secondValue() != _NIL &&
			    templatev.secondValue() != _NIL)
			   ? (Node)_T
			   : (Node)_NIL));
     }
   else if (slot_type == _own)
     { return get_own_direct_frames_with_facet_value
       (slot, facet, value, value_selector, kb_local_only_p);
     }
   else // _template
     { return get_template_direct_frames_with_facet_value
       (slot, facet, value, value_selector, kb_local_only_p);
     }
  }

  Values2 get_own_direct_frames_with_facet_value
       (Node slot, Node facet, Node value, ValueSelector value_selector,
	TruthValue kb_local_only_p)
  { if (value_selector == _known_true)
      { Values2 v = 
	tuple_store.fetch((Cons)(Cons.list(facet, slot, _frame_var, value)),
			 (kb_local_only_p == _NIL), _frame_var, 0, 
			 100000000, fas_Fv);
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	return new Values3(v.firstValue(), _T, more_status);
      }
    else if (value_selector == _default_only)
      { Values2 v = 
	tuple_store.fetch((Cons)(Cons.list(__default_facet_value, facet, slot,
					   _frame_var, value)),
			 (kb_local_only_p == _NIL), _frame_var, 0, 
			 100000000, rfas_Fv);
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	return new Values3(v.firstValue(), _T, more_status);
      }
    else // _either
     { Values2 knownv = get_frames_with_facet_value_internal
         (slot, facet, value, _direct, _own, _known_true, kb_local_only_p);
       Values2 defaultv = get_frames_with_facet_value_internal
         (slot, facet, value, _direct, _own, _default_only, kb_local_only_p);
       return new Values2(Cons.append_cons(knownv.firstValue(),
					   defaultv.firstValue()),
			  ((knownv.secondValue() != _NIL &&
			    defaultv.secondValue() != _NIL)
			   ? (Node)_T
			   : (Node)_NIL));
     }
  }

  Values2 get_template_direct_frames_with_facet_value
       (Node slot, Node facet, Node value, ValueSelector value_selector,
	TruthValue kb_local_only_p)
  { if (value_selector == _known_true)
      { Values2 v = 
	tuple_store.fetch((Cons)(Cons.list(__template_facet_value,
					   facet, slot, _frame_var, value)),
			 (kb_local_only_p == _NIL), _frame_var, 0, 
			 100000000, rfas_Fv);
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	return new Values3(v.firstValue(), _T, more_status);
      }
    else if (value_selector == _default_only)
      { Values2 v = 
	tuple_store.fetch((Cons)(Cons.list(__default_template_facet_value,
					   facet, slot, _frame_var, value)),
			 (kb_local_only_p == _NIL), _frame_var, 0, 
			 100000000, rfas_Fv);
	Node more_status = _NIL;
	if (more_status != _NIL) { more_status = _more; }
	return new Values3(v.firstValue(), _T, more_status);
      }
    else // _either
     { Values2 knownv = get_frames_with_facet_value_internal
         (slot, facet, value, _direct, _template, _known_true,
	  kb_local_only_p);
       Values2 defaultv = get_frames_with_facet_value_internal
         (slot, facet, value, _direct, _template, _default_only,
	  kb_local_only_p);
       return new Values2(Cons.append_cons(knownv.firstValue(),
					   defaultv.firstValue()),
			  ((knownv.secondValue() != _NIL &&
			    defaultv.secondValue() != _NIL)
			   ? (Node)_T
			   : (Node)_NIL));
     }
  }

  Values2 get_direct_types_from_tuples
    (Node instance, Node number_of_values, TruthValue kb_local_only_p) {
     int numvals = 1000000;
     if (number_of_values != _all)
       { numvals = (int)(((OKBCInteger)number_of_values).number); }
     Values2 fetch_result =
       tuple_store.fetch
       ((Cons)(Cons.list(__instance_of, instance, _class_var)),
	(kb_local_only_p == _NIL), _class_var, 0, 
	numvals, sf_V);
     Node more_status = _NIL;
     if (more_status != _NIL) { more_status = _more; }
     return new Values2(fetch_result.firstValue(), more_status);
  }

  protected Values3 get_instance_types_internal
     (Node instance, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p) {
      if (inference_level == _direct)
	{ Node in = coerce_to_frame(instance, _NIL, 
				    kb_local_only_p).firstValue();
	  if (in == _NIL) { in = instance; }
	  if (in == _NIL) 
	    { throw new NotCoercibleToFrame(instance, this).signal(); }
	  Node types = get_direct_types_from_tuples
	    (in, number_of_values, kb_local_only_p).firstValue();
	  // System.out.println("types= " + Cons.set_difference (types, facet_slot_class_and_individual_symbols, this, kb_local_only_p));
	  if (types == _NIL || types == null)
	    { Values2 cv = coerce_to_frame(_thing, _NIL, kb_local_only_p);
	      if (cv.secondValue() != _NIL)
		{ types = Cons.list(cv.firstValue()); }
	    }
	  return new Values3(types, _T, _NIL);
	}
      else { return super.get_instance_types_internal
	     (instance, inference_level, number_of_values, kb_local_only_p);
	   }
    }

  
  Values3 get_class_instances_implementation
     (Node the_class, Node number_of_values, TruthValue kb_local_only_p) {
     Node cl = coerce_to_frame_if_you_can(the_class, kb_local_only_p);
     if (cl == _NIL) 
       { throw new GenericError
	   (_T, new OKBCString("Cannot have a null class")).signal(); }
     if (cl == _slot || cl == _facet || cl == _class || cl == _individual)
       { throw new GenericError (_T, 
				    new OKBCString("Illegal class " + cl))
	 .signal(); }
     int numvals = 1000000;
     if (number_of_values != _all)
       { numvals = (int)(((OKBCInteger)number_of_values).number); }
     Values2 fetch_result =
       tuple_store.fetch((Cons)(Cons.list(__instance_of, _instance_var, cl)),
			 (kb_local_only_p == _NIL), _instance_var, 0, 
			 numvals, s_Fv);
     Node more_status = _NIL;
     if (more_status != _NIL) { more_status = _more; }
     return new Values3(fetch_result.firstValue(), _T, more_status);
   }


  protected Values3 get_class_instances_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p) {
       if (inference_level == _direct)
	 { return get_class_instances_implementation
	   (the_class, number_of_values, kb_local_only_p);
	 }
       else { return super.get_class_instances_internal
	      (the_class, inference_level, number_of_values, kb_local_only_p);
	    }
   }

  protected Values3 get_class_superclasses_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p) {
     if (inference_level == _direct)
       { Node cl = coerce_to_frame_if_you_can(the_class, kb_local_only_p);
	 if (cl == _NIL) 
	   { throw new GenericError
	     (_T, new OKBCString("Cannot have a null class")).signal(); }
	 int numvals = 1000000;
	 if (number_of_values != _all)
	   { numvals = (int)(((OKBCInteger)number_of_values).number); }
	 Values2 fetch_result =
	   tuple_store.fetch((Cons)(Cons.list(__subclass_of, cl, _super_var)),
			     (kb_local_only_p == _NIL), _super_var, 0, 
			     numvals, sf_V);
	 Node more_status = _NIL;
	 if (more_status != _NIL) { more_status = _more; }
	 // System.out.println("Superclasses are " + fetch_result);
	 return new Values3(fetch_result.firstValue(), _T, more_status);
       }
     else { return super.get_class_superclasses_internal
	    (the_class, inference_level, number_of_values, kb_local_only_p);
	  }
   }

  protected Values3 get_class_subclasses_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p) {
     Node cl = coerce_to_frame_if_you_can(the_class, kb_local_only_p);
     if (inference_level == _direct)
       { if (cl == _NIL) 
	   { throw new GenericError
	     (_T, new OKBCString("Cannot have a null class")).signal(); }
	 int numvals = 1000000;
	 if (number_of_values != _all)
	   { numvals = (int)(((OKBCInteger)number_of_values).number); }
	 Values2 fetch_result =
	   tuple_store.fetch((Cons)(Cons.list(__subclass_of, _sub_var, cl)),
			     (kb_local_only_p == _NIL), _sub_var, 0, 
			     numvals, s_Fv);
	 Node more_status = _NIL;
	 if (more_status != _NIL) { more_status = _more; }
	 return new Values3(fetch_result.firstValue(), _T, more_status);
       }
     else { return super.get_class_subclasses_internal
	    (the_class, inference_level, number_of_values, kb_local_only_p);
	  }
   }


  protected Node class_p_internal (Node thing, TruthValue kb_local_only_p) {
     Node cl = coerce_to_frame_if_you_can(thing, kb_local_only_p);
     Node class_frame = class_frame(kb_local_only_p);
     if (cl == _NIL) { return _NIL; }
     if (tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__instance_of, cl, class_frame)),
	  (kb_local_only_p == _NIL), sfv)
	 != __fail)
       { return _T; }
     if (tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__instance_of, _instance_var, cl)),
	  (kb_local_only_p == _NIL), s_Fv)
	 != __fail)
       { return _T; }
     if (tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__subclass_of, cl, _super_var)),
	  (kb_local_only_p == _NIL), sf_V)
	 != __fail)
       { return _T; }
     if (tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__subclass_of, _sub_var, cl)),
	  (kb_local_only_p == _NIL), s_Fv)
	 != __fail)
       { return _T; }
     return _NIL;
   }


  protected Node facet_p_internal
     (Node thing, TruthValue kb_local_only_p) {
       Node facet = coerce_to_frame_if_you_can(thing, kb_local_only_p);
       if (Cons.member_eql(facet, tuplekb_internal_facetlike_relations, null,
			   kb_local_only_p))
	 { return _NIL; }
       if (facet == _NIL) { return _NIL; }
       Node facet_frame = facet_frame(kb_local_only_p);
       if (primitive_direct_instance_of_p(facet, facet_frame, kb_local_only_p))
	 { return _T; }
       Node match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(facet, _slot_var, _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 0, fa_S_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__facet_of, facet, _slot_var, _frame_var)),
	  (kb_local_only_p == _NIL), 1, fas_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__template_facet_value, facet, _slot_var, 
			   _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 1, rfa_S_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__default_template_facet_value, facet, _slot_var, 
			   _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 1, rfa_S_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__default_facet_value, facet, _slot_var, 
			   _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 1, rfa_S_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__template_facet_of, facet, _slot_var, _frame_var)),
	  (kb_local_only_p == _NIL), 1, fas_F_V2_V);
       if (match != __fail) { return _T; }
       return _NIL;
     }


  protected Node slot_p_internal
     (Node thing, TruthValue kb_local_only_p) {
       Node slot = coerce_to_frame_if_you_can(thing, kb_local_only_p);
       if (Cons.member_eql(slot, tuplekb_internal_slotlike_relations, null,
			   kb_local_only_p))
	 { return _NIL; }
       if (slot == _NIL) { return _NIL; }
       Node slot_frame = slot_frame(kb_local_only_p);
       if (primitive_direct_instance_of_p(slot, slot_frame, kb_local_only_p))
	 { return _T; }
       Node match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(slot, _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 0, s_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__slot_of, slot, _frame_var)),
	  (kb_local_only_p == _NIL), 0, sf_V);
       if (match != __fail) { return _T; }

       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__template_slot_value, slot, _frame_var,
			   _value_var)),
	  (kb_local_only_p == _NIL), 0, fas_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__default_slot_value, slot, _frame_var,
			   _value_var)),
	  (kb_local_only_p == _NIL), 0, fas_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__default_template_slot_value, slot, _frame_var,
			   _value_var)),
	  (kb_local_only_p == _NIL), 0, fas_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__template_slot_of, slot, _frame_var)),
	  (kb_local_only_p == _NIL), 0, sf_V);
       if (match != __fail) { return _T; }

       // Now try using facet axioms.
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(_facet_var, slot, _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 0, _Fas_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__facet_of, _facet_var, slot, _frame_var)),
	  (kb_local_only_p == _NIL), 1, fa_Sf_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__template_facet_value, _facet_var, slot, 
			   _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 1, r_Fas_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__default_template_facet_value, _facet_var, slot, 
			   _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 1, r_Fas_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__default_facet_value, _facet_var, slot, 
			   _frame_var, _value_var)),
	  (kb_local_only_p == _NIL), 1, r_Fas_F_V2_V);
       if (match != __fail) { return _T; }
       match = tuple_store.fetch_one_binding
	 ((Cons)(Cons.list(__template_facet_of, _facet_var, slot, _frame_var)),
	  (kb_local_only_p == _NIL), 1, fa_Sf_V2_V);
       if (match != __fail) { return _T; }
       return _NIL;
    }

  protected Node slot_has_facet_p_internal
     (Node frame, Node slot, Node facet, InferenceLevel inference_level,
      SlotType slot_type, TruthValue kb_local_only_p) {
       if (inference_level != _direct)
	 { return super.slot_has_facet_p_internal
	   (frame, slot, facet, inference_level, slot_type, kb_local_only_p);
	 }
       frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
       Node real_slot = coerce_to_slot(slot, _NIL, 
				       kb_local_only_p).firstValue();
       if (real_slot != _NIL) { slot = real_slot; }
       Node real_facet = coerce_to_facet(facet, _NIL, 
					 kb_local_only_p).firstValue();
       if (real_facet != _NIL) { facet = real_facet; }
       if (Cons.member_eql(slot, tuplekb_internal_slotlike_relations, null,
			   kb_local_only_p))
	 { return _NIL; }
       if (slot == _NIL) { return _NIL; }
       if (slot_type == _own)
	 { Node match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(facet, slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 2, fasf_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__facet_of, facet, slot, frame)),
	    (kb_local_only_p == _NIL), 3, fasfv);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__default_facet_value,
			     facet, slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 3, rfasf_V);
	   if (match != __fail) { return _T; }
	 }
       else if (slot_type == _template)
	 { Node match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__template_facet_value, facet, slot, frame,
			     _value_var)),
	    (kb_local_only_p == _NIL), 3, rfasf_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__default_template_facet_value,
			     facet, slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 3, rfasf_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__template_facet_of, facet, slot, frame)),
	    (kb_local_only_p == _NIL), 3, fasfv);
	   if (match != __fail) { return _T; }
	 }
       else { throw new GenericError
	      (_T, new OKBCString("Illegal slot type")).signal(); 
	    }
       return _NIL;
    }


  protected Node frame_has_slot_p_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, TruthValue kb_local_only_p) {
       // System.out.println(Cons.list(new OKBCString("frame_has_slot_p_internal"), frame, slot, slot_type, kb_local_only_p));
       if (inference_level != _direct)
	 { return super.frame_has_slot_p_internal
	   (frame, slot, inference_level, slot_type, kb_local_only_p);
	 }
       frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
       Node real_slot = coerce_to_slot(slot, _NIL, 
				       kb_local_only_p).firstValue();
       if (real_slot != _NIL) { slot = real_slot; }
       if (Cons.member_eql(slot, tuplekb_internal_slotlike_relations, null,
			   kb_local_only_p))
	 { return _NIL; }
       if (slot == _NIL) { return _NIL; }
       if (slot_type == _own)
	 { Node match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 1, sf_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__slot_of, slot, frame)),
	    (kb_local_only_p == _NIL), 2, sfv);
	   if (match != __fail) { return _T; }
	   // Now try facet axioms.
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(_facet_var, slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 2, _Fasf_V2_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__facet_of, _facet_var, slot, frame)),
	    (kb_local_only_p == _NIL), 3, fa_Sfv);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__default_facet_value,
			     _facet_var, slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 3, r_Fasf_V2_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__default_slot_value, slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 2, fasf_V);
	   if (match != __fail) { return _T; }
	 }
       else if (slot_type == _template)
	 { Node match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__template_slot_value, slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 2, fasf_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__default_template_slot_value, slot, frame,
			     _value_var)),
	    (kb_local_only_p == _NIL), 2, fasf_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__template_slot_of, slot, frame)),
	    (kb_local_only_p == _NIL), 2, sfv);
	   if (match != __fail) { return _T; }
	   // Now try facet axioms.
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__template_facet_value, _facet_var, slot, frame,
			     _value_var)),
	    (kb_local_only_p == _NIL), 3, r_Fasf_V2_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__default_template_facet_value,
			     _facet_var, slot, frame, _value_var)),
	    (kb_local_only_p == _NIL), 3, r_Fasf_V2_V);
	   if (match != __fail) { return _T; }
	   match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__template_facet_of, _facet_var, slot, frame)),
	    (kb_local_only_p == _NIL), 3, fa_Sfv);
	   if (match != __fail) { return _T; }
	 }
       else { throw new GenericError
	      (_T, new OKBCString("Illegal slot type")).signal(); 
	    }
       return _NIL;
    }


  protected Values2 get_slot_facets_internal (Node frame, Node slot,
					   InferenceLevel inference_level,
					   SlotType slot_type,
					   TruthValue kb_local_only_p) {
      Node fr = coerce_to_frame_if_you_can(frame, kb_local_only_p);
      Node sl = coerce_to_frame_if_you_can(slot,  kb_local_only_p);
      if (fr == _NIL) { throw new NotCoercibleToFrame(frame, this).signal(); }
      if (sl == _NIL)
	{ throw new SlotNotFound(frame, slot, slot_type, this).signal(); }
      if (inference_level != _direct)
	{ return super.get_slot_facets_internal(frame, slot, inference_level,
						slot_type, kb_local_only_p);
	}
      Node own_result = _NIL;
      if (slot_type == _all || slot_type == _own)
	{ Node results = tuple_store.fetch((Cons)(Cons.list(_facet_var, sl, fr,
							    _value_var)),
					   (kb_local_only_p == _NIL),
					   _facet_var, 1, 
					   1000000, _Fasf_V2_Fa).firstValue();
	  own_result = Cons.append_cons
	    (Cons.set_difference
	     (results, tuplekb_internal_facetlike_relations, this,
	      kb_local_only_p), own_result);

	  results = tuple_store.fetch
	    ((Cons)(Cons.list(__facet_of, _facet_var, sl, fr)),
	     (kb_local_only_p == _NIL), _facet_var, 2, 
	     1000000, fa_Sfv).firstValue();
	  own_result = Cons.append_cons
	    (Cons.set_difference
	     (results, tuplekb_internal_facetlike_relations, this,
	      kb_local_only_p), own_result);

	  own_result = Cons.append_cons
	    (Cons.set_difference
	     (results, tuplekb_internal_facetlike_relations, this,
	      kb_local_only_p), own_result);
	}
      Node template_result = _NIL;
      if (slot_type == _all || slot_type == _template)
	{ Node results = tuple_store.fetch
	  ((Cons)(Cons.list(__template_facet_value, _facet_var, sl, fr,
			    _value_var)),
	   (kb_local_only_p == _NIL), _facet_var, 2, 
	   1000000, r_Fasf_V2_Fa).firstValue();
	  template_result = Cons.append_cons
	    (Cons.set_difference
	     (results, tuplekb_internal_facetlike_relations, this,
	      kb_local_only_p), template_result);

	  results = tuple_store.fetch
	    ((Cons)(Cons.list(__template_facet_of, _facet_var, sl, fr)),
	     (kb_local_only_p == _NIL), _facet_var, 2, 
	     1000000, fa_Sfv).firstValue();
	  template_result = Cons.append_cons
	    (Cons.set_difference
	     (results, tuplekb_internal_facetlike_relations, this,
	      kb_local_only_p), template_result);

	  template_result = Cons.append_cons
	    (Cons.set_difference
	     (results, tuplekb_internal_facetlike_relations, this,
	      kb_local_only_p), template_result);
	}
      return new Values2(Cons.remove_duplicates
			(Cons.append_cons(own_result, template_result),
			 _equal, this, kb_local_only_p),
			_T);
    }


  protected Values2 get_frame_slots_internal
         (Node frame, InferenceLevel inference_level, SlotType slot_type, 
	  TruthValue kb_local_only_p) {
      Node fr = coerce_to_frame_if_you_can(frame, kb_local_only_p);
      if (fr == _NIL)
	{ throw new GenericError
	  (_T, new OKBCString("Cannot have a null frame")).signal();
	}
      if (inference_level != _direct)
	{ return super.get_frame_slots_internal
	  (frame, inference_level, slot_type, kb_local_only_p);
	}
      Node slots = _NIL;
      if (slot_type == _all || slot_type == _own)
	{ slots = Cons.append_cons
	  (Cons.set_difference
	   (tuple_store.fetch
	    ((Cons)(Cons.list(_slot_var, fr, _value_var)),
	     (kb_local_only_p == _NIL), _slot_var, 0, 
	     1000000, _Sf_V2_S).firstValue(),
	    tuplekb_internal_slotlike_relations, this, kb_local_only_p),
	   slots);

	  slots = Cons.append_cons
	  (Cons.set_difference
	   (tuple_store.fetch
	    ((Cons)(Cons.list(__slot_of, _slot_var, fr)),
	     (kb_local_only_p == _NIL), _slot_var, 1, 
	     1000000, s_Fv).firstValue(),
	    tuplekb_internal_slotlike_relations, this, kb_local_only_p),
	   slots);
	}
      if (slot_type == _all || slot_type == _template)
	{ slots = Cons.append_cons
	  (tuple_store.fetch
	   ((Cons)(Cons.list(__template_slot_value, _slot_var, fr,
			     _value_var)),
	    (kb_local_only_p == _NIL), _slot_var, 0, 
	    1000000, fa_Sf_V2_S).firstValue(),
	   slots);

	  slots = Cons.append_cons
	  (tuple_store.fetch
	   ((Cons)(Cons.list(__template_slot_of, _slot_var, fr)),
	    (kb_local_only_p == _NIL), _slot_var, 0, 
	    1000000, s_Fv).firstValue(),
	   slots);

	}
      // Now try facets....
      if (slot_type == _all || slot_type == _own)
	{ Node all_facts = _NIL;
	  Node facts = tuple_store.fetch
	  ((Cons)(Cons.list(_facet_var, _slot_var, fr, _value_var)),
	   (kb_local_only_p == _NIL), 
	   Cons.list(_facet_var, _slot_var), 0, 
	   1000000, _Fa_Sf_V2_S).firstValue();
	  all_facts = Cons.append_cons(facts, all_facts);

	  facts = tuple_store.fetch
	  ((Cons)(Cons.list(__facet_of, _facet_var, _slot_var, fr)),
	   (kb_local_only_p == _NIL), 
	   Cons.list(_facet_var, _slot_var), 0, 
	   1000000, fa_S_Fv2_F).firstValue();
	  all_facts = Cons.append_cons(facts, all_facts);

	  facts = tuple_store.fetch
	  ((Cons)(Cons.list(__default_facet_value, _facet_var, _slot_var, fr,
			    _value_var)),
	   (kb_local_only_p == _NIL), 
	   Cons.list(_facet_var, _slot_var), 0, 
	   1000000, r_Fa_Sf_V2_S).firstValue();
	  all_facts = Cons.append_cons(facts, all_facts);

	  Node filtered = _NIL;
	  Enumeration e = facts.elements();
	  for (; e.hasMoreElements() ;) {
	    Cons fact = (Cons)(e.nextElement());
	    Node facet = fact.car();
	    Node slot = ((Cons)fact.cdr()).car();
	    if (Cons.member_eql(facet, tuplekb_internal_facetlike_relations,
				null, kb_local_only_p))
	      {}
	    else { filtered = new Cons(slot, filtered); }
	  }
	  slots = Cons.append_cons(filtered, slots);

	}
      if (slot_type == _all || slot_type == _template)
	{ slots = Cons.append_cons
	  (tuple_store.fetch
	   ((Cons)(Cons.list(__template_facet_value, _facet_var, _slot_var, fr,
			     _value_var)), 
	    (kb_local_only_p == _NIL), _slot_var, 0, 
	    1000000, r_Fa_Sf_V2_S).firstValue(),
	   slots);

	  slots = Cons.append_cons
	  (tuple_store.fetch
	   ((Cons)(Cons.list(__default_template_facet_value, _facet_var,
			     _slot_var, fr, _value_var)), 
	    (kb_local_only_p == _NIL), _slot_var, 0, 
	    1000000, r_Fa_Sf_V2_S).firstValue(),
	   slots);

	  slots = Cons.append_cons
	  (tuple_store.fetch
	   ((Cons)(Cons.list(__template_facet_of, _facet_var,
			     _slot_var, fr)), 
	    (kb_local_only_p == _NIL), _slot_var, 0, 
	    1000000, fa_S_Fv2_F).firstValue(),
	   slots);

	}
      return new Values2(Cons.remove_duplicates
			 (slots, _equal, this, kb_local_only_p), _T);
    }

  protected Values2 get_frame_in_kb_internal
     (Node thing, TruthValue error_p, TruthValue kb_local_only_p) {
       return coerce_to_frame(thing, error_p, kb_local_only_p);
    }

  protected Node get_kb_frames_internal (TruthValue kb_local_only_p) {
       Hashtable ht = new Hashtable();
       Node all = _NIL;
       tuple_store.maybe_post_hoc_full_index_kb();
       {
	 Node classes = tuple_store.fetch
	   ((Cons)(Cons.list(__instance_of, _instance_var, _class_var)),
	    (kb_local_only_p == _NIL), _class_var, 0, 
	    1000000, s_F_V2_V).firstValue();
	 Enumeration e = classes.elements();
	 for (; e.hasMoreElements() ;) {
	   Node the_class = (Node)(e.nextElement());
	   if (Cons.member_eql(the_class, tuplekb_internal_relations, null,
			       kb_local_only_p))
	     {}
	   else { Object existing = ht.get(the_class);
		  if (existing == null)
		    { ht.put(the_class, the_class);
		      all = new Cons(the_class, all);
		    }
		}
	 }
       }
       {
	 Node instances = tuple_store.fetch
	   ((Cons)(Cons.list(__instance_of, _instance_var, _class_var)),
	    (kb_local_only_p == _NIL), _instance_var, 0, 
	    1000000, s_F_V2_F).firstValue();
	 Enumeration e = instances.elements();
	 for (; e.hasMoreElements() ;) {
	   Node instance = (Node)(e.nextElement());
	   Object existing = ht.get(instance);
	   if (Cons.member_eql(instance, tuplekb_internal_relations, null,
			       kb_local_only_p))
	     {}
	   else if (existing == null)
	     { ht.put(instance, instance);
	       all = new Cons(instance, all);
	     }
	 }
       }
       return all;
    }

  protected Node get_kb_classes_internal
         (Node selector, TruthValue kb_local_only_p) {
       Hashtable ht = new Hashtable();
       Node all = _NIL;
       Node class_frame = class_frame(kb_local_only_p);
       tuple_store.maybe_post_hoc_full_index_kb();
       {
	 Node classes = tuple_store.fetch
	   ((Cons)(Cons.list(__instance_of, _instance_var, class_frame)),
	    (kb_local_only_p == _NIL), _instance_var, 0, 
	    1000000, s_Fv).firstValue();
	 Enumeration e = classes.elements();
	 for (; e.hasMoreElements() ;) {
	   Node the_class = (Node)(e.nextElement());
	   Object existing = ht.get(the_class);
	   if (existing == null)
	     { ht.put(the_class, the_class);
	       all = new Cons(the_class, all);
	     }
	 }
       }
       {
	 Node classes = tuple_store.fetch
	   ((Cons)(Cons.list(__instance_of, _instance_var, _class_var)),
	    (kb_local_only_p == _NIL), _class_var, 0, 
	    1000000, s_F_V2_V).firstValue();
	 Enumeration e = classes.elements();
	 for (; e.hasMoreElements() ;) {
	   Node the_class = (Node)(e.nextElement());
	   Object existing = ht.get(the_class);
	   if (Cons.member_eql(the_class, tuplekb_internal_relations, null,
			       kb_local_only_p))
	     {}
	   else if (existing == null)
	     { ht.put(the_class, the_class);
	       all = new Cons(the_class, all);
	     }
	 }
       }
       return all;
    }

  protected Node get_kb_individuals_internal
         (Node selector, TruthValue kb_local_only_p) {
       Hashtable ht = new Hashtable();
       Node all = _NIL;
       tuple_store.maybe_post_hoc_full_index_kb();
       {
	 Node frames = tuple_store.fetch
	   ((Cons)(Cons.list(__instance_of, _instance_var, _class_var)),
	    (kb_local_only_p == _NIL), _instance_var, 0, 
	    1000000, s_F_V2_F).firstValue();
	 Enumeration e = frames.elements();
	 for (; e.hasMoreElements() ;) {
	   Node frame = (Node)(e.nextElement());
	   Object existing = ht.get(frame);
	   if (class_p(frame, kb_local_only_p) != _NIL) {}
	   else if (existing == null)
	     { ht.put(frame, frame);
	       all = new Cons(frame, all);
	     }
	 }
       }
       return all;
    }

  protected static Cons tuple_kb_supported_behaviors
  = (Cons)(Cons.list
	   (Cons.list(_constraint_checking_time, _T, _immediate, _never),
	    Cons.list(_constraint_report_time, _NIL, _immediate),
	    Cons.list(_permission, _NIL, _read, _add, _delete),
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
	    Cons.list(_frame_names_required,   _T, _NIL, _T),
	    Cons.list(_are_frames,           _NIL, _class, _facet, _slot,
		      _instance, _individual)));
  
  protected static Cons current_behaviors
  = (Cons)(Cons.list
	   (Cons.list(_constraint_checking_time, _immediate),
	    Cons.list(_constraint_report_time, _immediate),
	    Cons.append_cons
	    (Cons.list(_constraints_checked,
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
	    Cons.list(_defaults,               _override),
	    Cons.list(_compliance,             _facets_supported,
		      _user_defined_facets, _read_only, _monotonic),
	    Cons.list(_class_slot_types,      _template, _own),
	    Cons.list(_collection_types,      _set),
	    Cons.list(_frame_names_required,  _NIL),
	    Cons.list(_are_frames,            _class, _facet, _slot,
		      _instance, _individual)));
  
  Cons get_kb_supported_behaviors () {
    return (Cons)tuple_kb_supported_behaviors;
  }

  protected Node get_kb_behaviors_internal (Connection connection) {
    return new Cons(_permission,
		    Cons.mapcar_first(get_kb_supported_behaviors()));
  }


  protected Node get_behavior_supported_values_internal
     (Node behavior) {
       Cons entry = get_kb_supported_behaviors().assoc(behavior);
       if (entry == null) { return _NIL; }
       else { return entry.cdr().cdr(); }
    }

  protected Node get_behavior_values_internal
     (Node behavior) {
       if (behavior == _permission)
	 { if (read_only_p)
	     { return Cons.list(_read); }
	   else { return Cons.list(_read, _add, _delete); }
	 }
       else
	 {
	   Cons entry = (Cons)current_behaviors.assoc(behavior);
	   if (entry == null) { return _NIL; }
	   else { Cons rest = (Cons)(entry.cdr());
		  if (rest == null) { return _NIL; }
		  else { return rest; }
		}
	 }
    }

  protected void put_behavior_values_internal (Node behavior, Node values) {
    if ((behavior == _constraint_checking_time) &&
	(values.length() != 1))
      { throw new IllegalBehaviorValues(behavior, values).signal(); }
    Cons entry = get_kb_supported_behaviors().assoc(behavior);
    if (entry == null || entry.cadr() == _NIL)
      { throw new IllegalBehaviorValues(behavior, values).signal(); }
    current_behaviors =
      new Cons(new Cons(behavior, values),
	       Cons.remove_eql(current_behaviors.assoc(behavior),
			       current_behaviors, this));
  }


  Node slot_attachment_fact(Node frame, Node slot, SlotType slot_type,
			    TruthValue kb_local_only_p) {
    frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
    slot = coerce_to_slot(slot, _T, kb_local_only_p).firstValue();
    // System.out.println("Class_p = " + class_p(frame, kb_local_only_p));
    if (slot_type != _own && class_p(frame, kb_local_only_p) == _NIL)
      { throw new GenericError
	(_T, new OKBCString("Cannot attach a template slot to a non-class: "
			   + frame)).signal();
      }
    if (slot_type == _own)
      { return Cons.list(__slot_of, slot, frame); }
    else if (slot_type == _template)
      { return Cons.list(__template_slot_of, slot, frame); }
    else { throw new GenericError
	   (_T, new OKBCString("Illegal slot_type: " + slot_type)).signal();
	 }
  }


  protected void attach_slot_internal
     (Node frame, Node slot, SlotType slot_type, TruthValue kb_local_only_p) {
       // System.out.println(Cons.list(new OKBCString("attach-slot"), frame, slot, slot_type, kb_local_only_p));
       ConstraintChecker.enforce_domain_constraint
	 (frame, slot, this, slot_type, _all_inferable, _either,
	  kb_local_only_p);
       Node fact = slot_attachment_fact(frame, slot, slot_type,
					kb_local_only_p);
       tuple_store.insert(fact, true, false);
    }


  protected void detach_slot_internal
     (Node frame, Node slot, SlotType slot_type, TruthValue kb_local_only_p) {
       Node fact = slot_attachment_fact(frame, slot, slot_type,
					kb_local_only_p);
       tuple_store.drop(fact, true, (kb_local_only_p == _NIL));
    }


  Node facet_attachment_fact(Node frame, Node slot, Node facet,
			     SlotType slot_type, TruthValue kb_local_only_p) {
    frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
    slot = coerce_to_slot(slot, _T, kb_local_only_p).firstValue();
    facet = coerce_to_facet(facet, _T, kb_local_only_p).firstValue();
    if (slot_type != _own && class_p(frame, kb_local_only_p) == _NIL)
      { throw new GenericError
	(_T, new OKBCString("Cannot attach a template facet to a non-class: "
			   + frame)).signal();
      }
    if (slot_type == _own)
      { return Cons.list(__facet_of, facet, slot, frame); }
    else if (slot_type == _template)
      { return Cons.list(__template_facet_of, facet, slot, frame); }
    else { throw new GenericError
	   (_T, new OKBCString("Illegal slot_type: " + slot_type)).signal();
	 }
  }


  protected void attach_facet_internal
     (Node frame, Node slot, Node facet, SlotType slot_type,
      TruthValue kb_local_only_p) {
       ConstraintChecker.enforce_domain_constraint
	 (frame, slot, this, slot_type, _all_inferable,_either,
	  kb_local_only_p);
       Node fact = facet_attachment_fact(frame, slot, facet, slot_type,
					 kb_local_only_p);
       tuple_store.insert(fact, true, false);
    }


  protected void detach_facet_internal
     (Node frame, Node slot, Node facet, SlotType slot_type, 
      TruthValue kb_local_only_p) {
       Node fact = facet_attachment_fact(frame, slot, facet, slot_type,
					 kb_local_only_p);
       tuple_store.drop(fact, true, (kb_local_only_p == _NIL));
    }


  Node ensure_has_slot(Node slot, SlotType slot_type,
			 TruthValue kb_local_only_p) {
    if (Cons.member_eql(slot, tuplekb_internal_slotlike_relations, null,
			kb_local_only_p))
      { return slot; }
    Values2 v = coerce_to_slot(slot, _NIL, kb_local_only_p);
    Node real_slot = v.firstValue();
    Node found_p = v.secondValue();
    if (found_p == _NIL) { real_slot = slot; }
    Node slot_frame = slot_frame(kb_local_only_p);
    if (primitive_direct_instance_of_p(real_slot, slot_frame, kb_local_only_p))
      { return real_slot; }
    else if (primitive_direct_instance_of_p(slot, slot_frame, kb_local_only_p))
      { return slot; }
    else
      { Node name = null;
	if (found_p != _NIL)
	  { if (real_slot instanceof AbstractSymbol) { name = real_slot; }
	    else { name = get_frame_name(real_slot, kb_local_only_p); }
	  }
	else if (slot instanceof AbstractSymbol) { name = slot; }
	else { name = _NIL; }
	Node handle = null;
	if (found_p != _NIL)
	  { if (real_slot instanceof AbstractSymbol) { handle = _NIL; }
	    else { handle = real_slot; }
	  }
	else if (slot instanceof AbstractSymbol) { handle = _NIL; }
	else { handle = slot; }
	Node new_slot = create_slot(name, _NIL, slot_type, _NIL, _NIL, _NIL, 
				    _NIL, handle, _NIL, kb_local_only_p);
	return new_slot;
      }
  }

  protected void put_slot_values_internal
     (Node frame, Node slot, Node values, SlotType slot_type,
      ValueSelector value_selector, TruthValue kb_local_only_p) {
       err_on_read_only_violation();
       Node fr = coerce_to_frame_if_you_can(frame, kb_local_only_p);
       if (fr == _NIL)
	 { throw new NotCoercibleToFrame(frame, this).signal(); }
       if (slot == _NIL)
	 { throw new SlotNotFound(fr, slot, slot_type, this).signal(); }
       frame = fr;
       slot = ensure_has_slot(slot, slot_type, kb_local_only_p);
       if (slot == _NIL)
	 { throw new SlotNotFound(fr, slot, slot_type, this).signal(); }
       if (Cons.member_eql(_immediate, 
			   get_behavior_values(_constraint_checking_time),
			   null, kb_local_only_p))
	 { Node current_values = get_slot_values(frame, slot, _all_inferable,
						 slot_type, _all, _either,
						 _NIL).firstValue();
	   ConstraintChecker.enforce_slot_constraints
	     (frame, slot, current_values, values, this, _all_inferable,
	      slot_type, _either, _NIL);
	 }
       if (slot_type == _own)
	 { if (value_selector == _known_true)
	     { tuple_store.drop(Cons.list(slot, frame, _value_var), false,
				(kb_local_only_p == _NIL));
	     }
	   else if (value_selector == _default_only)
	     { tuple_store.drop(Cons.list(__default_slot_value, 
					  slot, frame, _value_var), false,
				(kb_local_only_p == _NIL));
	     }
	   else { throw new GenericError
		  (_T, new OKBCString("Illegal value_selector: " + 
				     value_selector)).signal();
		}
	 }
       else if (slot_type == _template)
	 { if (value_selector == _known_true)
	     { tuple_store.drop(Cons.list(__template_slot_value, 
					  slot, frame, _value_var), false,
				(kb_local_only_p == _NIL));
	     }
	   else if (value_selector == _default_only)
	     { tuple_store.drop(Cons.list(__default_template_slot_value, 
					  slot, frame, _value_var), false,
				(kb_local_only_p == _NIL));
	     }
	   else { throw new GenericError
		  (_T, new OKBCString("Illegal value_selector: " + 
				     value_selector)).signal();
		}
	 }
       else { throw new GenericError
	      (_T, new OKBCString("Illegal slot_type: " + slot_type)).signal();
	    }
       Enumeration e = values.elements();
       for (; e.hasMoreElements() ;) {
	 Node value = (Node)(e.nextElement());
	 Node fact = null;
	 if (slot_type == _own)
	   { if (value_selector == _known_true)
	       {  fact = Cons.list(slot, frame, value); }
	     else if (value_selector == _default_only)
	       { if (value == __deleted)
		   { fact = Cons.list(__default_slot_value,
				      slot, frame, value); }
		 else { throw new GenericError
			(_T, new OKBCString
			 ("Cannot put a default own slot value")).signal();
		      }
	       }
	     else { throw new GenericError
		    (_T, new OKBCString("Illegal value_selector: " + 
				       value_selector)).signal();
		  }
	   }
	 else if (slot_type == _template)
	   { if (value_selector == _known_true)
	       {  fact = Cons.list(__template_slot_value, 
				   slot, frame, value); 
		}
	     else if (value_selector == _default_only)
	       { fact = Cons.list(__default_template_slot_value,
				  slot, frame, value);
	       }
	     else { throw new GenericError
		  (_T, new OKBCString("Illegal value_selector: " + 
				     value_selector)).signal();
		  }
	   }
	 else { throw new GenericError
		(_T, new OKBCString
		 ("Illegal slot_type: " + slot_type)).signal();
	      }
	 tuple_store.insert(fact, true, false);
       }
    }


  protected void remove_slot_value_internal
     (Node frame, Node slot, Node value, Node test, SlotType slot_type, 
      Node index, ValueSelector value_selector, TruthValue kb_local_only_p) {
       if (value_selector != _default_only)
	 { super.remove_slot_value_internal
	   (frame, slot, value, test, slot_type, index, value_selector,
	    kb_local_only_p);
	 }
       else
	 { Node values = get_slot_values(frame, slot, _direct, slot_type, _all,
					 value_selector, 
					 kb_local_only_p).firstValue();
	   if (Cons.member_test(value, values, test, this, kb_local_only_p))
	     { 
	       Node trimmed = Cons.remove_test(value, values, test, this,
					       kb_local_only_p);
	       if (trimmed == null) { trimmed = _NIL; }
	       put_slot_values(frame, slot,
			       trimmed, slot_type, value_selector,
			       kb_local_only_p);
	     }
	   else if (get_slot_values(frame, slot, _all_inferable, slot_type,
				    _all, value_selector,
				    kb_local_only_p).firstValue()
		    != _NIL)
	     { // We dont' have the value locally, but we can inherit it,
	       // so block.
	       put_slot_values
		 (frame, slot, Cons.list(__deleted), slot_type, value_selector,
		  kb_local_only_p);
	     }
	 }
     }


  protected void add_slot_value_internal
     (Node frame, Node slot, Node value, Node test, SlotType slot_type,
      Node add_after, ValueSelector value_selector,
      TruthValue kb_local_only_p) {
       err_on_read_only_violation();
       Node fr = coerce_to_frame_if_you_can(frame, kb_local_only_p);
       if (fr == _NIL) { throw new NotCoercibleToFrame(frame, this).signal(); }
       frame = fr;
       Node sl = ensure_has_slot(slot, slot_type, kb_local_only_p);
       if (sl == _NIL)
	 { throw new SlotNotFound(fr, slot, slot_type, this).signal(); }
       slot = sl;
       if (Cons.member_eql(_immediate, 
			   get_behavior_values(_constraint_checking_time),
			   null, kb_local_only_p))
	 { Node current_values = get_slot_values(frame, slot, _all_inferable,
						 slot_type, _all, _either,
						 _NIL).firstValue();
	   Node new_values = current_values;
	   if (Cons.member_test(value, current_values, _equal, this,
				kb_local_only_p))
	     {}
	   else { new_values = new Cons(value, current_values); }
	   ConstraintChecker.enforce_slot_constraints
	     (frame, slot, current_values, new_values, this, _all_inferable,
	      slot_type, _either, _NIL);
	 }

       Node fact = null;
       if (value_selector == _known_true)
	 { if (slot_type == _own) { fact = Cons.list(slot, frame, value); }
	   else if (slot_type == _template)
	     { fact = Cons.list(__template_slot_value, slot, frame, value); }
	   else { throw new GenericError
		  (_T, new OKBCString
		   ("Illegal slot_type: " + slot_type)).signal();
		}
	 }
       else if (value_selector == _default_only)
	 { // Can only have one default slot value, so delete any 
	   // existing ones.
	   if (slot_type == _template)
	     { tuple_store.drop(Cons.list(__default_template_slot_value, slot,
					  frame, _value_var), false,
				(kb_local_only_p == _NIL));
	       fact = Cons.list(__default_template_slot_value, slot, frame,
				value);
	     }
	   else if (slot_type == _template)
	     { tuple_store.drop(Cons.list(__default_slot_value, slot,
					  frame, _value_var), false,
				(kb_local_only_p == _NIL));
	       fact = Cons.list(__default_slot_value, slot, frame, value);
	     }
	   else { throw new GenericError
		  (_T, new OKBCString
		   ("Illegal slot_type: " + slot_type)).signal();
		}
	 }
       else { throw new GenericError
	      (_T, new OKBCString("Illegal value_selector: " + 
				 value_selector)).signal();
	    }

       tuple_store.insert(fact, true, false);
     }


  Node ensure_has_facet(Node facet, Node slot, SlotType slot_type,
			 TruthValue kb_local_only_p) {
    if (Cons.member_eql(facet, tuplekb_internal_slotlike_relations, null,
			kb_local_only_p))
      { return slot; }
    Values2 v = coerce_to_facet(facet, _NIL, kb_local_only_p);
    Node real_facet = v.firstValue();
    Node found_p = v.secondValue();
    if (found_p == _NIL) { real_facet = facet; }
    Node facet_frame = facet_frame(kb_local_only_p);
    if (primitive_direct_instance_of_p(real_facet, facet_frame,
				       kb_local_only_p))
      { return real_facet; }
    else if (primitive_direct_instance_of_p(facet, facet_frame,
					    kb_local_only_p))
      { return facet; }
    else
      { Node name = null;
	if (found_p != _NIL)
	  { if (real_facet instanceof AbstractSymbol) { name = real_facet; }
	    else { name = get_frame_name(real_facet, kb_local_only_p); }
	  }
	else if (facet instanceof AbstractSymbol) { name = facet; }
	else { name = _NIL; }
	Node handle = null;
	if (found_p != _NIL)
	  { if (real_facet instanceof AbstractSymbol) { handle = _NIL; }
	    else { handle = real_facet; }
	  }
	else if (facet instanceof AbstractSymbol) { handle = _NIL; }
	else { handle = facet; }
	Node new_facet = create_facet(name, _NIL, slot, slot_type, _NIL, _NIL,
				      _NIL, _NIL, handle, _NIL,
				      kb_local_only_p);
	return new_facet;
      }
  }

  protected void put_facet_values_internal
     (Node frame, Node slot, Node facet, Node values, SlotType slot_type,
      ValueSelector value_selector, TruthValue kb_local_only_p) {
       err_on_read_only_violation();
       Node fr = coerce_to_frame_if_you_can(frame, kb_local_only_p);
       if (fr == _NIL)
	 { throw new NotCoercibleToFrame(frame, this).signal(); }
       if (slot == _NIL)
	 { throw new SlotNotFound(fr, slot, slot_type, this).signal(); }
       if (facet == _NIL)
	 { throw new FacetNotFound
	   (fr, slot, facet, slot_type, this).signal(); 
	 }
       frame = fr;
       Node sl = ensure_has_slot(slot, slot_type, kb_local_only_p);
       if (sl == _NIL)
	 { throw new SlotNotFound(fr, slot, slot_type, this).signal(); }
       slot = sl;
       Node fa = ensure_has_facet(facet, slot, slot_type, kb_local_only_p);
       if (fa == _NIL) 
	 { throw new FacetNotFound
	   (fr, slot, facet, slot_type, this).signal(); 
	 }
       facet = fa;

       if (Cons.member_eql(_immediate, 
			   get_behavior_values(_constraint_checking_time), null,
			   kb_local_only_p))
	 { Node current_values = get_facet_values(frame, slot, facet,
						  _all_inferable, slot_type,
						  _all, _either,
						 _NIL).firstValue();
	   ConstraintChecker.check_assertion_of_constraint_facet_values
	     (frame, slot, facet, current_values, values, this, _all_inferable,
	      slot_type, value_selector, kb_local_only_p);
	 }

       if (slot_type == _own)
	 { if (value_selector == _known_true)
	     { tuple_store.drop(Cons.list(facet, slot, frame, _value_var),
				false, (kb_local_only_p == _NIL));
	     }
	   else if (value_selector == _default_only)
	     { tuple_store.drop(Cons.list(__default_facet_value, 
					  facet, slot, frame, _value_var),
				false, (kb_local_only_p == _NIL));
	     }
	   else { throw new GenericError
		  (_T, new OKBCString("Illegal value_selector: " + 
				     value_selector)).signal();
		}
	 }
       else if (slot_type == _template)
	 { if (value_selector == _known_true)
	     { tuple_store.drop(Cons.list(__template_facet_value, 
					  facet, slot, frame, _value_var), 
				false, (kb_local_only_p == _NIL));
	     }
	   else if (value_selector == _default_only)
	     { tuple_store.drop(Cons.list(__default_template_facet_value, 
					  facet, slot, frame, _value_var),
				false, (kb_local_only_p == _NIL));
	     }
	   else { throw new GenericError
		  (_T, new OKBCString("Illegal value_selector: " + 
				     value_selector)).signal();
		}
	 }
       else { throw new GenericError
	      (_T, new OKBCString("Illegal slot_type: " + slot_type)).signal();
	    }

       Enumeration e = values.elements();
       for (; e.hasMoreElements() ;) {
	 Node value = (Node)(e.nextElement());
	 Node fact = null;
	 if (slot_type == _own)
	   { if (value_selector == _known_true)
	       {  fact = Cons.list(facet, slot, frame, value); }
	     else if (value_selector == _default_only)
	       { if (value == __deleted)
		   { fact = Cons.list(__default_facet_value,
				      facet, slot, frame, value); }
		 else { throw new GenericError
			(_T, new OKBCString
			 ("Cannot put a default own facet value")).signal();
		      }
	       }
	     else { throw new GenericError
		    (_T, new OKBCString("Illegal value_selector: " + 
				       value_selector)).signal();
		  }
	   }
	 else if (slot_type == _template)
	   { if (value_selector == _known_true)
	       {  fact = Cons.list(__template_facet_value, 
				   facet, slot, frame, value); 
		}
	     else if (value_selector == _default_only)
	       { fact = Cons.list(__default_template_facet_value,
				  facet, slot, frame, value);
	       }
	     else { throw new GenericError
		  (_T, new OKBCString("Illegal value_selector: " + 
				     value_selector)).signal();
		  }
	   }
	 else { throw new GenericError
		(_T, new OKBCString
		 ("Illegal slot_type: " + slot_type)).signal();
	      }
	 tuple_store.insert(fact, true, false);
       }
    }


  protected void remove_facet_value_internal
     (Node frame, Node slot, Node facet, Node value, Node test,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p) {
       if (value_selector != _default_only)
	 { super.remove_facet_value_internal
	   (frame, slot, facet, value, test, slot_type, value_selector,
	    kb_local_only_p);
	 }
       else
	 { Node values = get_facet_values(frame, slot, facet, _direct, 
					  slot_type, _all, value_selector, 
					  kb_local_only_p).firstValue();
	   if (Cons.member_test(value, values, test, this, kb_local_only_p))
	     { 
	       Node trimmed = Cons.remove_test(value, values, test, this,
					       kb_local_only_p);
	       if (trimmed == null) { trimmed = _NIL; }
	       put_facet_values(frame, slot, facet,
				trimmed, slot_type, value_selector,
				kb_local_only_p);
	     }
	   else if (get_facet_values(frame, slot, facet, _all_inferable,
				     slot_type, _all, value_selector,
				     kb_local_only_p).firstValue()
		    != _NIL)
	     { // We dont' have the value locally, but we can inherit it,
	       // so block.
	       put_facet_values
		 (frame, slot, facet, Cons.list(__deleted), slot_type,
		  value_selector, kb_local_only_p);
	     }
	 }
     }

  protected void put_class_superclasses_internal
     (Node the_class, Node new_superclasses, TruthValue kb_local_only_p) {
       err_on_read_only_violation();
       Node cl = coerce_to_frame_if_you_can(the_class, kb_local_only_p);
       if (cl == _NIL)
	 { throw new NotCoercibleToFrame(the_class, this).signal(); }
       tuple_store.drop(Cons.list(__subclass_of, cl, _super_var), false, 
			(kb_local_only_p == _NIL));
       Enumeration e = new_superclasses.elements();
       for (; e.hasMoreElements() ;) {
	 Node sup = (Node)(e.nextElement());
	 Node coerced_new_super = coerce_to_frame_if_you_can(sup,
							     kb_local_only_p);
	 if (coerced_new_super == _NIL)
	   { throw new NotCoercibleToFrame(coerced_new_super, this).signal(); }
	 Cons fact = (Cons)(Cons.list(__subclass_of, cl, coerced_new_super));
	 tuple_store.insert(fact, true, false);
       }
    }

  protected void add_class_superclass_internal
     (Node the_class, Node new_superclass, TruthValue kb_local_only_p) {
       err_on_read_only_violation();
       Node cl = coerce_to_frame_if_you_can(the_class, kb_local_only_p);
       if (cl == _NIL)
	 { throw new NotCoercibleToFrame(the_class, this).signal(); }
       Node coerced_new_super = coerce_to_frame_if_you_can(new_superclass,
							   kb_local_only_p);
       if (coerced_new_super == _NIL)
	 { throw new NotCoercibleToFrame(coerced_new_super, this).signal(); }
       Cons fact = (Cons)(Cons.list(__subclass_of, cl, coerced_new_super));
       tuple_store.insert(fact, true, false);
    }

  protected void put_instance_types_internal
     (Node instance, Node new_types, TruthValue kb_local_only_p) {
       err_on_read_only_violation();
       Node fr = coerce_to_frame_if_you_can(instance, kb_local_only_p);
       if (fr == _NIL) 
	 { throw new GenericError
	   (_T, new OKBCString("Cannot have a null frame")).signal();
	 }
       {
	 Node existing_types = get_direct_types_from_tuples
	   (fr, _all, kb_local_only_p).firstValue();
	 Enumeration e = existing_types.elements();
	 for (; e.hasMoreElements() ;) {
	   Node type = (Node)(e.nextElement());
	   if (type == _facet || type == _slot ||
	       type == _class || type == _individual) {}
	   else { tuple_store.drop(Cons.list(__instance_of, fr, type), false, 
				   (kb_local_only_p == _NIL));
		}
	 }
       }
       {
	 Enumeration e = Cons.list_if_not(new_types).elements();
	 for (; e.hasMoreElements() ;) {
	   Node type = (Node)(e.nextElement());
	   Node coerced_new_type = coerce_to_frame_if_you_can(type,
							      kb_local_only_p);
	   Cons fact = (Cons)(Cons.list(__instance_of, fr, coerced_new_type));
	   tuple_store.insert(fact, true, false);
	 }
       }
    }

  protected void add_instance_type_internal
     (Node instance, Node new_type, TruthValue kb_local_only_p) {
       err_on_read_only_violation();
       Node fr = coerce_to_frame_if_you_can(instance, kb_local_only_p);
       if (fr == _NIL)
	 { throw new GenericError
	   (_T, new OKBCString("Cannot have a null frame")).signal();
	 }
       Node coerced_new_type = coerce_to_frame_if_you_can(new_type,
							  kb_local_only_p);
       if (coerced_new_type == _NIL)
	 { throw new NotCoercibleToFrame(coerced_new_type, this).signal(); }
       Cons fact = (Cons)(Cons.list(__instance_of, fr, coerced_new_type));
       tuple_store.insert(fact, true, false);
    }


  protected Node create_facet_internal
     (Node name, Node frame_or_nil, Node slot_or_nil, SlotType slot_type,
      Node direct_types, Node doc, Node own_slots, Node own_facets,
      Node handle, Node pretty_name, TruthValue kb_local_only_p) {
       Node facet = create_reified_non_class
	 (name, _facet, direct_types, doc, own_slots, own_facets, handle,
	  pretty_name, kb_local_only_p);
       if (frame_or_nil != _NIL && slot_or_nil != _NIL)
	 { attach_facet(frame_or_nil, slot_or_nil, facet, slot_type,
			kb_local_only_p);
	 }
       return facet;
    }

  protected Node create_slot_internal
     (Node name, Node frame_or_nil, SlotType slot_type, Node direct_types,
      Node doc, Node own_slots, Node own_facets, Node handle, Node pretty_name,
      TruthValue kb_local_only_p) {
       Node slot = create_reified_non_class
	 (name, _slot, direct_types, doc, own_slots, own_facets, handle,
	  pretty_name, kb_local_only_p);
       // System.out.println("Create slot: " + Cons.list(name, _slot, direct_types, doc, own_slots, own_facets, handle, pretty_name, kb_local_only_p) + " -> " + slot);
       if (frame_or_nil != _NIL)
	 { attach_slot(frame_or_nil, slot, slot_type, kb_local_only_p); }
       return slot;
    }

  protected Node create_individual_internal
     (Node name, Node direct_types, Node doc, Node own_slots, Node own_facets,
      Node handle, Node pretty_name, TruthValue kb_local_only_p) {
       return create_reified_non_class
	 (name, _individual, direct_types, doc, own_slots, own_facets, handle,
	  pretty_name, kb_local_only_p);
    }


  protected Values2 direct_coercible_to_frame_p
	    (Node frame_name, TruthValue kb_local_only_p)
  {
    Node hit = get_simple_frame_with_slot_value
      (__name, frame_name, kb_local_only_p);
    if (hit == _NIL || hit == null) { return new Values2(_NIL, _NIL); }
    else { return new Values2(hit, _T); }
  }


  protected Node create_bootstrap_class
  (Node name, Node direct_superclasses, TruthValue kb_local_only_p) {
    err_on_read_only_violation();
    Node frame = allocate_frame_handle(name, _class, _NIL);
    tuple_store.insert(Cons.list(__name, frame, name), true, false);
    if (name != _class && name != _thing && name != _frame)
      { Node class_frame = class_frame(kb_local_only_p);
	tuple_store.insert(Cons.list(__instance_of, frame, class_frame), true,
			   false);
      }
    tuple_store.insert(Cons.list(__primitive, frame), true, false);
    if (name != _thing)
      { Node enumerand = direct_superclasses;
	if (enumerand == _NIL)
	  { enumerand = direct_coercible_to_frame_p
	    (_thing, kb_local_only_p).firstValue();
	  }
        enumerand = Cons.list_if_not(enumerand);
	// System.out.println("Name: " + name + ", enumerand: " + enumerand + ", supers: " + direct_superclasses);
	Enumerator e = enumerand.enumerate_list();
	while (e.has_more_p()) {
	  Node s = e.next();
	  tuple_store.insert(Cons.list(__subclass_of, frame, s), true, false);
        }
      }
    return frame;
  }

  protected Node create_class_internal
     (Node name, Node direct_types, Node direct_superclasses, Node primitive_p,
      Node doc, Node template_slots, Node template_facets, Node own_slots,
      Node own_facets, Node handle, Node pretty_name,
      TruthValue kb_local_only_p) {
       if (Cons.member_eql(name, okbc_standard_names, null, kb_local_only_p))
	 { return coerce_to_frame(name, _T, kb_local_only_p).firstValue(); }
       boolean names_required_p = true;
       if (Cons.member_eql(_NIL, get_behavior_values(_frame_names_required),
			   null, kb_local_only_p))
	 { names_required_p = false; }
       if (names_required_p && name == _NIL)
	 { throw new GenericError
	   (_T, new OKBCString("Frame names are required")).signal();
	 }
       err_on_read_only_violation();

       Node frame = handle;
       if (frame == _NIL)
	 { if (names_required_p)
	     { Values2 v = coerce_to_frame(name, _NIL, kb_local_only_p);
	       Node found_p = v.secondValue();
	       if (found_p != _NIL)
		 { frame = get_frame_handle(v.firstValue(), kb_local_only_p); }
	       else { frame = allocate_frame_handle(name, _class, _NIL); }
	     }
	   else { frame = allocate_frame_handle(name, _class, _NIL); }
	 }

       if (name != _NIL) { put_frame_name(frame, name, kb_local_only_p); }
       tuple_store.insert
	 (Cons.list(__instance_of, frame, frame_frame(kb_local_only_p)),
	  true, false);
       Node class_frame = class_frame(kb_local_only_p);
       if (direct_types != _NIL)
	 { direct_types = new Cons(class_frame,
				   Cons.list_if_not(direct_types));
	   Enumeration e = direct_types.elements();
	   while(e.hasMoreElements()) {
	     Node type = (Node)(e.nextElement());
	     add_instance_type(frame, type, kb_local_only_p);
	   }
	 }
       else if (!(primitive_direct_instance_of_p
		  (frame, class_frame, kb_local_only_p)))
	 { add_instance_type(frame, class_frame, kb_local_only_p); }

       if (doc != _NIL)
	 { add_slot_value(frame, __documentation, doc, _equal, _own, _NIL,
			  _known_true, kb_local_only_p);
	 }
       if (primitive_p != _NIL)
	 { tuple_store.insert(Cons.list(__primitive, frame), true, false); }
       Node enumerand = Cons.list_if_not(direct_superclasses);
       if (enumerand == _NIL)
         { enumerand = direct_coercible_to_frame_p
	   (_thing, kb_local_only_p).firstValue();
	 }
       enumerand = Cons.list_if_not(enumerand);
       Enumeration e = enumerand.elements();
       while(e.hasMoreElements()) {
	 Node superclass = (Node)(e.nextElement());
	 add_class_superclass(frame, superclass, kb_local_only_p);
       }
       Node pair = initialise_slots_and_facets(frame, own_slots, own_facets,
					       _own, kb_local_only_p);
       initialise_slots_and_facets(frame, template_slots, template_facets, 
				   _template, kb_local_only_p, pair.car(),
				   pair.cadr());
       if (pretty_name != _NIL) 
	 { put_frame_pretty_name(frame, pretty_name, kb_local_only_p); }
       //System.out.println("Create_class(" + frame + "," + 
       // direct_superclasses+")");
       return frame;
    }


  protected Node primitive_p_internal
         (Node the_class, TruthValue kb_local_only_p) {
    the_class = coerce_to_frame_if_you_can(the_class, kb_local_only_p);
    if (the_class != _NIL)
      { Node match = tuple_store.fetch_one_binding
	   ((Cons)(Cons.list(__primitive, the_class)),
	    (kb_local_only_p == _NIL), 1, fv);
	if (match != __fail) { return _T; }
	else { return _NIL; }
      }
    else { return _T; }
  }


  protected void delete_slot_internal (Node slot, TruthValue kb_local_only_p) {
    delete_frame(slot, kb_local_only_p);
  }


  protected void delete_facet_internal
            (Node facet, TruthValue kb_local_only_p) {
    delete_frame(facet, kb_local_only_p);
  }


  protected void delete_frame_internal
            (Node frame, TruthValue kb_local_only_p) {
    frame = coerce_to_frame_if_you_can(frame, kb_local_only_p);
    tuple_store.maybe_post_hoc_full_index_kb();
    Node facts = tuple_store.facts_full_indexed_under(frame);
    Enumeration e = facts.elements();
    while (e.hasMoreElements()) {
      Node fact = (Node)(e.nextElement());
      tuple_store.drop(fact, true, (kb_local_only_p == _NIL));
    }
  }


  protected Node rename_slot_internal
    (Node slot, Node new_name, TruthValue kb_local_only_p) {
      return put_frame_name(slot, new_name, kb_local_only_p);
  }


  protected Node rename_facet_internal
     (Node facet, Node new_name, TruthValue kb_local_only_p) {
      return put_frame_name(facet, new_name, kb_local_only_p);
  }


  protected Node get_kb_direct_parents_internal () {
    return Cons.vectorToCons(parent_kbs);
  }


// ============================ Open and Close stuff ===================


  static OKBCString initial_root_path () {
    OKBCString ret = null;
    try
      { ret = new OKBCString(System.getProperties().getProperty("user.home")); }
    catch (Exception e) {}
    return ret;
  }
  
  static Node default_root_path_for_saved_tuple_kbs = initial_root_path();

  Node root_path_for_saved_tuple_kbs () {
    if (default_root_path_for_saved_tuple_kbs == null)
      { throw new GenericError
	(_T, new OKBCString("Cannot compute root for save path.  " +
			    "Perhaps you are running as an applet")).signal();
      }
    else { return default_root_path_for_saved_tuple_kbs; }
  }

  protected Node create_kb_locator_internal
         (Node thing, Connection connection) {
    MetaKb meta_kb = (MetaKb)meta_kb(LocalConnection.local_connection());
    File f = new File(root_path_for_saved_tuple_kbs().toString(),
		      URLEncoder.encode(kbname.toString()) +
		      (save_kb_files_portably_p
		       ? portable_kb_file_extension
		       : nonportable_kb_file_extension));
    TupleKbLocator instance = 
      new TupleKbLocator(kbname, this, new OKBCString(f.getPath()));
    meta_kb.put_instance_types(instance, _kb_locator, _T);
    return instance;
  }

  // NMJ 03/10/98
  /**
   * fix for correct loading of KBs:
   * now handles a 'thing' argument of OKBCString,
   * when the arg. came from an editor menu selection
   */
  protected Node find_kb_locator_internal
            (Node thing, Connection connection) {
    String locname = null;
    if (thing instanceof TupleKbLocator) { return thing; }
    else if (thing instanceof AbstractSymbol)
      {
	MetaKb meta_kb = 
	  (MetaKb)meta_kb(LocalConnection.local_connection());
	Node locators = meta_kb.get_class_instances(_kb_locator, _taxonomic, 
						    _all, _T).firstValue();
	Kb type = Node.concrete_kb_from_abstract_kb((Symbol)thing);
	if (type != null) return find_kb_locator(type, connection);
	locname = ((Symbol)thing).toString();
      }
    
    
    if (thing instanceof TupleKb)
      { MetaKb meta_kb = 
	(MetaKb)meta_kb(LocalConnection.local_connection());
	Node locators = meta_kb.get_class_instances(_kb_locator, _taxonomic, 
						    _all, _T).firstValue();
	Enumeration e = locators.elements();
	while (e.hasMoreElements()) {
	  Node locator = (Node)(e.nextElement());
	  if (locator instanceof TupleKbLocator &&
	      (((TupleKbLocator)locator).name == kbname))
	    { return locator; }
	}
	return create_kb_locator(thing, connection);
      };
    
    if (thing instanceof OKBCString) 
      locname = ((OKBCString) thing).toString();
    MetaKb meta_kb = 
      (MetaKb)meta_kb(LocalConnection.local_connection());
    Node locators = meta_kb.get_class_instances(_kb_locator, _taxonomic, 
						_all, _T).firstValue();
    Enumeration e = locators.elements();
    while (e.hasMoreElements()) {
      Node locator = (Node)(e.nextElement());
      if (locator instanceof TupleKbLocator) {
	if (((TupleKbLocator)locator).name.toString().equals(locname)) {
	  return locator;
	}
      }
    };
    
    throw new GenericError
      (_T, new OKBCString
       ("Cannot find a kb locator for " + thing)).signal();
    
  }

  static String without_file_extension (String s) {
    if (s.endsWith(portable_kb_file_extension))
      { return
	s.substring(0, s.length() - portable_kb_file_extension.length());
      }
    else if (s.endsWith(nonportable_kb_file_extension))
      { return
	s.substring(0, s.length() - nonportable_kb_file_extension.length());
      }
    else { return s; }
  }

  
  protected Node save_kb_internal (TruthValue error_p) {
    Node locator = find_kb_locator(this, connection());
    if (locator instanceof TupleKbLocator) {}
    else { throw new GenericError
	   (_T, new OKBCString("Cannot find the right sort of kb locator for "
			      + this)).signal();
	 }
    TupleKbLocator loc = (TupleKbLocator)locator;
    if (loc.path.toString().endsWith(portable_kb_file_extension))
      { return save_kb_portably (loc, error_p); }
    else if (loc.path.toString().endsWith(nonportable_kb_file_extension))
      { return save_kb_nonportably (loc, error_p); }
    else
      { throw new GenericError
	   (_T, new OKBCString("Unknown KB file extension for locator: " +
			       locator)).signal();
      }
  }


  protected Node save_kb_nonportably (TupleKbLocator loc, TruthValue error_p) {
    Node facts = tuple_store.facts_full_indexed_under(_instance_var);
    boolean ok_p = true;
    boolean err = true;
    if (error_p == _NIL) { err = false; }
    FileOutputStream stream = null;
    File temp_file_name = null;
    Package global_package = Package.find_or_create_package("OKBC-USER");
    if (error_p == _NIL)
      { temp_file_name = new File(root_path_for_saved_tuple_kbs().toString(),
				  "temp.sav");
      }
    try { try { File f = new File(loc.path.toString());
                if (f.exists()) f.delete();
           stream = new FileOutputStream(new File(loc.path.toString())); }
	  catch (IOException e) { ok_p = false; }
	  // Dump out crucial header stuff for KB.
	  EncodeDecoder.encode(stream, kbname, true, global_package);
	  EncodeDecoder.encode(stream, unique_id, true, global_package);
	  EncodeDecoder.encode(stream, current_behaviors, true, global_package);
	  EncodeDecoder.encode(stream,
			       new OKBCString(ioSyntax.io_package.name), true,
			       global_package);
	  // Preallocate pointers for objects.
	  Hashtable ht = new Hashtable();
	  int count = EncodeDecoder.preallocate_unique_ids(facts, ht);
	  try { EncodeDecoder.encode_hashtable(ht, stream, true,
					       ioSyntax.io_package); }
	  catch (IOException e) { throw new RuntimeException(e.toString()); }
	  
	  // Now actually dump out the facts.
	  Enumeration e = facts.elements();
	  while (ok_p && e.hasMoreElements()) {
	    Node fact = (Node)(e.nextElement());
	    if (!ok_p) {}
	    else if (err)
	      { EncodeDecoder.encode(stream, fact, true, ioSyntax.io_package,
				     ht); }
	    else { boolean error_found_p = false;
		   try { FileOutputStream ostream =
			    new FileOutputStream(temp_file_name);
			 EncodeDecoder.encode
			   (ostream, fact, true, ioSyntax.io_package, ht);
			 ostream.flush();
			 ostream.close();
		       }
		   catch (Exception ex) { error_found_p = true; }
		   if (!error_found_p)
		     { try { FileInputStream istream = 
			     new FileInputStream(temp_file_name);
			     long length = temp_file_name.length();
			     for (int i = 0; i < length ; i++) {
			       int ch = istream.read();
			       stream.write(ch);
			     }
			     istream.close();
			   }
		       catch (Exception ex) {}
		     }
		 }
	  }
	  // Write the EOF marker.
	  EncodeDecoder.encode(stream, __EOF, true, ioSyntax.io_package, ht);
	}
    finally
      // Clean up by deleting the temp file wrapped in an ignore errors.
      { if (stream != null)
	  { try { stream.flush();
		  stream.close();
		}
	    catch (Exception ex) {}
	  }
        if (temp_file_name != null && temp_file_name.exists())
	  { try { temp_file_name.delete(); }
	    catch (Exception e) {}
	  }
      }
    if (ok_p) { return _T; }
    else { return _NIL; }
  }


  protected Node save_kb_portably (TupleKbLocator loc, TruthValue error_p) {
    Node facts = tuple_store.facts_full_indexed_under(_instance_var);
    boolean ok_p = true;
    boolean err = true;
    if (error_p == _NIL) { err = false; }
    FileWriter wstream = null;
    PrintWriter stream = null;
    File temp_file_name = null;
    if (error_p == _NIL)
      { temp_file_name = new File(root_path_for_saved_tuple_kbs().toString(),
				  "temp.sav");
      }
    try { try { File f = new File(loc.path.toString()); 
                if (f.exists()) f.delete();
           wstream = new FileWriter(new File(loc.path.toString())); }
	  catch (IOException e) { ok_p = false; }
	  stream = new PrintWriter(wstream);
	  // Dump out crucial header stuff for KB.
	  kbname.encode(stream, true);
	  unique_id.encode(stream, true);
	  current_behaviors.encode(stream, true);
	  (new OKBCString(ioSyntax.io_package.name)).encode(stream, true);
	  // Now actually dump out the facts.
	  Enumeration e = facts.elements();
	  while (ok_p && e.hasMoreElements()) {
	    Node fact = (Node)(e.nextElement());
	    stream.println("FACT");
	    stream.flush();
	    if (!ok_p) {}
	    else if (err)
	      { fact.encode(stream, true); }
	    else { boolean error_found_p = false;
		   try { FileWriter wostream = new FileWriter(temp_file_name);
			 PrintWriter ostream = new PrintWriter(wostream);
			 fact.encode(ostream, true);
			 ostream.flush();
			 ostream.close();
			 wostream.flush();
			 wostream.close();
		       }
		   catch (Exception ex) { error_found_p = true; }
		   if (!error_found_p)
		     { try { FileReader istream = 
			     new FileReader(temp_file_name);
			     long length = temp_file_name.length();
			     for (int i = 0; i < length ; i++) {
			       int ch = istream.read();
			       stream.write(ch);
			     }
			     istream.close();
			   }
		       catch (Exception ex) {}
		     }
		 }
	  }
	}
    finally
      // Clean up by deleting the temp file wrapped in an ignore errors.
      { if (stream != null)
	  { try { stream.flush();
		  stream.close();
		  wstream.flush();
		  wstream.close();
		}
	    catch (Exception ex) {}
	  }
        if (temp_file_name != null && temp_file_name.exists())
	  { try { temp_file_name.delete(); }
	    catch (Exception e) {}
	  }
      }
    if (ok_p) { return _T; }
    else { return _NIL; }
  }


  protected void save_kb_as_internal (Node new_name_or_locator) {
    if (new_name_or_locator instanceof AbstractSymbol)
      { setName(new_name_or_locator); }
    else if (new_name_or_locator instanceof TupleKbLocator)
      { setName(((TupleKbLocator)new_name_or_locator).name); }
    else
      { throw new GenericError
	   (_T, new OKBCString("Name or locator: " + new_name_or_locator))
	   .signal();
      }
    MetaKb meta_kb = (MetaKb)meta_kb(LocalConnection.local_connection());
    meta_kb.put_frame_name(this, new_name_or_locator, _T);
    save_kb(_T);
  }


  protected void close_kb_internal (Node save_p) {
    if (save_p != _NIL) { save_kb(_T); }
    Node locator = find_kb_locator(this, connection());
    MetaKb meta_kb = (MetaKb)meta_kb(LocalConnection.local_connection());
    meta_kb.put_instance_types(locator, _NIL, _T);    
    meta_kb.put_instance_types(this, _NIL, _T);
    tuple_store = null;
    if (current_kb() == this) { the_current_kb = null; }
  }

  protected Node open_kb_internal
     (Node kb_locator, Connection connection, TruthValue error_p) {
    TupleKbLocator locator =
      (TupleKbLocator)find_kb_locator(kb_locator, connection());
    if (locator.path.toString().endsWith(portable_kb_file_extension))
      { return open_portable_tuple_kb(this, locator, connection, error_p); }
    else if (locator.path.toString().endsWith(nonportable_kb_file_extension))
      { return open_nonportable_tuple_kb(this, locator, connection, error_p); }
    else
      { throw new GenericError
	   (_T, new OKBCString("Unknown KB file extension for locator: " +
			       locator)).signal();
      }
  }

  protected static Node open_nonportable_tuple_kb
     (TupleKb kb_type, TupleKbLocator locator, Connection connection,
      TruthValue error_p) {
    Connection localcon = LocalConnection.local_connection();
    Node existing = kb_type.find_kb_of_type(locator.name, kb_type, localcon);
    FileInputStream stream = null;
    TupleKb kb = null;
    boolean ok_p = true;
    Exception exception = null;
    boolean eof_p = false;
    File file = locator.maybe_defaulted_file
      (kb_type.root_path_for_saved_tuple_kbs().toString());
    Node [] lookup_table = null;
    try { try { stream = new FileInputStream (file); }
	  catch (IOException e)
	    {
	      ok_p = false;
	      exception = e;
	    }
	  if (ok_p)
	    { if (existing == _NIL)
		{ kb = (TupleKb)kb_type.create_kb(locator.name, locator, _NIL,
						  localcon);
		}
	      else { kb = (TupleKb)existing; }
	      Node facts = _NIL;
	      String line = null;
	      kb.setName(EncodeDecoder.decode(stream, kb, true, localcon)); 
	      kb.unique_id = EncodeDecoder.decode(stream, kb, true, localcon); 
	      kb.current_behaviors =
		(Cons)(EncodeDecoder.decode(stream, kb, true, localcon));
	      kb.ioSyntax.io_package = Package.find_or_create_package
		(((OKBCString)(EncodeDecoder.decode
			       (stream, kb, true, localcon))).string);
	      Node thing = EncodeDecoder.decode
		(stream, kb, true, localcon);
	      lookup_table = ((NodeVectorNode)thing).nodes;
	    }
	  else if (error_p != _NIL)
	    { throw new GenericError
		(_T, new OKBCString("An error occurred whilst opening file " +
				    file + " for locator " + locator +
				    ".  Exception was " + exception.toString()))
		.signal();
	    }
	  else {}
	  while (ok_p && !eof_p) {
	    if (!eof_p)
	      { Node fact = null;
		if (error_p != _NIL)
		  { fact = EncodeDecoder.decode(stream, kb, true, localcon,
						kb.ioSyntax.io_package,
						lookup_table);
		    if (fact == __EOF)
		      { eof_p = true; }
		    else
		      { ((TupleKb)kb).tuple_store.insert(fact, true, false); }
		  }
		else { try { fact = EncodeDecoder.decode(stream, kb, 
							 true, localcon,
							 kb.ioSyntax.io_package,
							 lookup_table);
			     if (fact == __EOF)
			       { eof_p = true; }
			     else
			       { ((TupleKb)kb).tuple_store.insert(fact, true,
								  false);
			       }
		           }
		       catch (Exception ex) {}
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
    if (kb == null) { return _NIL; }
    else { kb.last_locator = locator;
	   return kb;
	 }
  }


  protected static Node open_portable_tuple_kb
     (TupleKb kb_type, TupleKbLocator locator, Connection connection,
      TruthValue error_p) {
    Connection localcon = LocalConnection.local_connection();
    Node existing = kb_type.find_kb_of_type(locator.name, kb_type, localcon);
    FileReader rstream = null;
    PushableBufferedReader stream = null;
    TupleKb kb = null;
    boolean ok_p = true;
    Exception exception = null;
    File file = locator.maybe_defaulted_file
      (kb_type.root_path_for_saved_tuple_kbs().toString());
    try { try { rstream = new FileReader (file); }
	  catch (IOException e)
	    {
	      ok_p = false;
	      exception = e;
	    }
	  if (ok_p)
	    { stream = new PushableBufferedReader(rstream);
	      if (existing == _NIL)
		{ kb = (TupleKb)kb_type.create_kb(locator.name, locator, _NIL,
						  localcon);
		  kb.setName(NetworkGeneral.decode(stream, kb, true, localcon));
		  kb.unique_id = NetworkGeneral.decode
		    (stream, kb, true, localcon); 
		  kb.current_behaviors =
		    (Cons)(NetworkGeneral.decode(stream, kb, true, localcon));
		  kb.ioSyntax.io_package = Package.find_or_create_package
		    (((OKBCString)(NetworkGeneral.decode
				   (stream, kb, true, localcon))).string);
		}
	      else { kb = (TupleKb)existing; }
	    }
	  else if (error_p != _NIL)
	    { throw new GenericError
		(_T, new OKBCString("An error occurred whilst opening file " +
				    file + " for locator " + locator +
				    ".  Exception was " + exception.toString()))
		.signal();
	    }
	  else {}
	  Node facts = _NIL;
	  String line = null;
	  boolean eof_p = false;
	  while (ok_p && !eof_p) {
	    do { try { line = stream.readLine(); }
		 catch (IOException ex) { line = null; }
	         if (line == null) { eof_p = true; }
	       }
	    while (!eof_p && !("FACT".equals(line)));
	    if (!eof_p)
	      { Node fact = null;
		if (error_p != _NIL)
		  { fact = NetworkGeneral.decode(stream, kb, true,
						 localcon);
		    // System.out.println("Inserted: " + fact);
		    ((TupleKb)kb).tuple_store.insert(fact, true, false);
		  }
		else { try { fact = NetworkGeneral.decode(stream, kb, 
							  true, localcon); 
		             ((TupleKb)kb).tuple_store.insert(fact, true,
							      false);
		           }
		       catch (Exception ex) {}
		     }
	      }
	  }
	}
    finally
      { if (stream != null)
	  { try { stream.close();
		  rstream.close();
		}
	    catch (Exception ex) {}
	  }
      }
    if (kb == null) { return _NIL; }
    else { kb.last_locator = locator;
	   return kb;
	 }
  }


  protected Node openable_kbs_internal (Connection connection, Node place) {
    if (place != _NIL)
      { throw new GenericError
	   (_T, new OKBCString("Place argument not supported")).signal();
      }
    File dirpath = new File(root_path_for_saved_tuple_kbs().toString());
    String[] files = dirpath.list(new TupleKbFilenameFilter());
    Node existing_locators = _NIL;
    MetaKb meta_kb = (MetaKb)meta_kb(LocalConnection.local_connection());
    Node locators = meta_kb.get_class_instances(_kb_locator, _taxonomic, 
						_all, _T).firstValue();
    Enumeration e = locators.elements();
    while (e.hasMoreElements()) {
      Node locator = (Node)(e.nextElement());
      if (locator instanceof TupleKbLocator)
	{ existing_locators = new Cons(locator, existing_locators); }
    }
    Node to_return = _NIL;
    for (int i=0 ; i < files.length ; i++) {
      String path = URLDecoder.decode(files[i]);
      Enumeration locs = existing_locators.elements();
      Node match = _NIL;
      while (locs.hasMoreElements()) {
	AbstractKbLocator loc = (AbstractKbLocator)(locs.nextElement());
	if (loc.name.toString().equals(path)) 
	  { to_return = new Cons(loc, to_return); 
	    match = loc;
	    break;
	  }
      }
      if (match != _NIL)
	{ existing_locators = Cons.remove_eql(match, existing_locators, this);}
      else
	{ TupleKbLocator instance =
	  new TupleKbLocator
	  (LispReader.readFromString(without_file_extension(path)), this,
	   new OKBCString((new File(root_path_for_saved_tuple_kbs().toString(),
				    URLEncoder.encode(path))).getPath()));
	  meta_kb.put_instance_types(instance, _kb_locator, _T);
	  to_return = new Cons(instance, to_return); 
	}
    }
    e = existing_locators.elements();
    while (e.hasMoreElements()) {
      Node loc = (Node)(e.nextElement());
      meta_kb.delete_frame(loc, _T);
    }
    return to_return;
  }


  protected void expunge_kb_internal
       (Node kb_locator, Connection connection, TruthValue error_p) {
    if (kb_locator instanceof TupleKbLocator) {}
    else { throw new GenericError
	   (_T, new OKBCString(kb_locator + 
			      " is not the right sort of kb-locator for " +
			      this)).signal();
	 }
    try { TupleKbLocator loc = (TupleKbLocator) kb_locator;
	  File file_name = new File(loc.path.toString());
	  file_name.delete();
	}
    catch (Exception e) {}
  }


  public Connection connection () {
    return LocalConnection.local_connection();
  }

  public void set_connection (Connection conn) {
    // Do nothing.
  }


  public Node debug_node(Node args) {
    System.out.println("Start of debug data for " + this + ". ==============");
    Enumerator e = (Cons.list_if_not(args)).enumerate_list();
    while (e.has_more_p()) {
      Node arg = e.next();
      System.out.println("Facts about " + arg + ".");
      Node facts = tuple_store.facts_full_indexed_under(arg);
	  if (facts==null) facts = _NIL;
      Enumerator factse = facts.enumerate_list();
      while (factse.has_more_p()) {
	Node fact = factse.next();
	System.out.print("    ");
	Cons.pretty_print(fact, 4);
	System.out.println();
      }
    }
    System.out.println();
    System.out.println("End of debug data for " + this + ". ================");
    return _NIL;
  }


  Node cache_hook_printer (Node key)
  { Node filtered = _NIL;
    Enumerator e = key.enumerate_list();
    while(e.has_more_p()) {
      Node n = e.next();
      if (false) {} // (n instanceof Kb) {}
      else { filtered = new Cons(n, filtered); }
    }
    return Cons.reverse(filtered);
  }

  public void cache_hit_hook (Node function, Node key, TrieResult trie_result)
  { if (trace_cache_activity_p)
      { System.out.print("h");
	if (inside_cache_hook) {}
	else
	  { inside_cache_hook = true;
	    // System.out.println("HHHHHHHHHHH - " + cache_hook_printer(key));
	    inside_cache_hook = false;
	  }
      }
    super.cache_hit_hook (function, key, trie_result);
  }

  public Values2 cache_miss_hook (Node function, Node key) {
    if (trace_cache_activity_p)
      { System.out.print("m");
	// System.out.print(connection().ok_to_cache_flag);
	if (inside_cache_hook) {}
	else { inside_cache_hook = true;
	       // System.out.println(cache_hook_printer(key));
	       inside_cache_hook = false;
	       // System.out.print("m");
	     }
      }
    return super.cache_miss_hook(function, key);
  }

  public void cache_fill_hook (Node function, Node key, Object new_results,
			       boolean multiple_values_p) {
    if (trace_cache_activity_p)
      { // System.out.print("F");
	if (inside_cache_hook) {}
	else { inside_cache_hook = true;
	       // System.out.println(cache_hook_printer(key));
	       inside_cache_hook = false;
	     }
      }
    super.cache_fill_hook (function, key, new_results, multiple_values_p);
  }


  public void allow_caching (Object arg) {
    if (trace_cache_activity_p)
      { System.out.print("+");}
    super.allow_caching(arg);
  }

  public void disallow_caching (Object arg) {
    if (trace_cache_activity_p) { System.out.print("-"); }
    super.disallow_caching(arg);
  }



  // Test harness.
  public static void main (String arg[]) {

    TupleKb kb = new TupleKb();
    System.out.println("KB is: " + kb);
    TupleKb kb2 = new TupleKb();
    System.out.println("KB2 is: " + kb2);

    System.out.println("-----------");
    System.out.println("A = " + kb.class_p(_NIL));
    System.out.println("-----------");
    System.out.println("B = " + kb.class_p(_NIL));
    System.out.println("-----------");
    System.out.println("C = " + kb.class_p(_NIL));
    System.out.println("-----------");
    System.out.println("D = " + kb.class_p(_NIL));
    System.out.println("-----------");

    Node slot1name = Symbol.intern("SLOT1", "OKBC");
    Node slot2name = Symbol.intern("SLOT2", "OKBC");
    Node facet1name = Symbol.intern("FACET1", "OKBC");
    Node facet2name = Symbol.intern("FACET2", "OKBC");
    Node someclassname = Symbol.intern("SOME-CLASS", "OKBC");
    Node slot1 = kb.create_slot(slot1name);
    Node slot2 = kb.create_slot(slot2name);
    Node facet1 = kb.create_facet(facet1name);
    Node facet2 = kb.create_facet(facet2name);
    Node some_class = kb.create_class(someclassname, _NIL, _NIL, _NIL, _NIL,
				      _NIL, _NIL, _NIL, _NIL, _NIL,
				      new OKBCString("Some class"));
    Node forty_two = new OKBCInteger(42);
    Node two = new OKBCInteger(2);
    Node the_class_class = kb.coerce_to_frame(_class).firstValue();

    kb.attach_slot(the_class_class, slot1, _template);
    System.out.println("Slot1 attached to class: " +
		       kb.get_frame_slots(some_class, _taxonomic, _own));

    kb.put_slot_value(the_class_class, slot1, two, _template,
		      _default_only);
    System.out.println("Get-slot-values (known_true): " +
		       kb.get_slot_values(some_class, slot1, _taxonomic,
					  _own, _all, _known_true));
    System.out.println("Get-slot-values (either): " +
		       kb.get_slot_values(some_class, slot1, _taxonomic,
					  _own, _all, _either));
    System.out.println("Get-slot-values (default_only): " +
		       kb.get_slot_values(some_class, slot1, _taxonomic,
					  _own, _all, _default_only));

    kb.put_slot_value(the_class_class, slot1, forty_two, _template,
		      _known_true);
    System.out.println("Get-slot-values (known_true): " +
		       kb.get_slot_values(some_class, slot1, _taxonomic,
					  _own, _all, _known_true));
    System.out.println("Get-slot-values (either): " +
		       kb.get_slot_values(some_class, slot1, _taxonomic,
					  _own, _all, _either));
    System.out.println("Get-slot-values (default_only): " +
		       kb.get_slot_values(some_class, slot1, _taxonomic,
					  _own, _all, _default_only));

    kb.attach_facet(the_class_class, slot2, facet2, _template);
    System.out.println("Facet1 attached to class: " +
		       kb.get_slot_facets(some_class, slot2, _taxonomic, _own));

    kb.put_facet_value(the_class_class, slot2, facet2, two, _template,
		      _default_only);
    System.out.println("Get-facet-values (known_true): " +
		       kb.get_facet_values(some_class, slot2, facet2,
					   _taxonomic, _own, _all,
					   _known_true));
    System.out.println("Get-facet-values (either): " +
		       kb.get_facet_values(some_class, slot2, facet2,
					   _taxonomic, _own, _all,
					   _either));
    System.out.println("Get-facet-values (default_only): " +
		       kb.get_facet_values(some_class, slot2, facet2,
					   _taxonomic, _own, _all,
					   _default_only));

    kb.put_facet_value(the_class_class, slot2, facet2, forty_two, _template,
		      _known_true);
    System.out.println("Get-facet-values (known_true): " +
		       kb.get_facet_values(some_class, slot2, facet2,
					   _taxonomic, _own, _all,
					   _known_true));
    System.out.println("Get-facet-values (either): " +
		       kb.get_facet_values(some_class, slot2, facet2,
					   _taxonomic, _own, _all,
					   _either));
    System.out.println("Get-facet-values (default_only): " +
		       kb.get_facet_values(some_class, slot2, facet2,
					   _taxonomic, _own, _all,
					   _default_only));

  
    Node fact1 = LispReader.readFromString("(foo foo1)");
    Node fact2 = LispReader.readFromString("(foo foo2)");
    Node fact3 = LispReader.readFromString("(foo foo3)");
    System.out.println("Read facts 1, 2, and 3");
    
    kb.tuple_store.insert(fact1);
    kb.tuple_store.insert(fact2);
    kb2.tuple_store.insert(fact3);
    System.out.println("Insert facts 1, 2, and 3");

    Cons pattern1 = (Cons)(LispReader.readFromString("(foo ?x)"));
    System.out.println("Pattern1 is: " + pattern1);

    { Values2 resa = kb.tuple_store.fetch(pattern1, false);
      System.out.println("Bindings are: " + resa); }

    { Node resa = kb.tuple_store.fetch_one_binding(pattern1);
      System.out.println("One binding is: " + resa); }

    { Node resa = kb2.tuple_store.fetch_one_binding(pattern1);
      System.out.println("One binding is: " + resa); }

    kb.include(kb2);
    System.out.println("Kb includes kb2");

    { Values2 resb = kb.tuple_store.fetch(pattern1, false);
      System.out.println("Bindings (should be just 2 facts) are: " + resb); }

    { Values2 resf = kb.tuple_store.fetch(pattern1, true);
      System.out.println("Bindings (should be all 3 facts) are: " + resf); }

    kb2.include(kb);
    System.out.println("Kb2 (circularly) includes kb");

    { Values2 resc = kb.tuple_store.fetch(pattern1, true);
      System.out.println("Bindings (should be all 3 facts) are: " + resc); }

    kb.tuple_store.maybe_post_hoc_full_index_kb();
    System.out.println("Post-hoc full index KBs");

    Cons pattern2 = (Cons)(LispReader.readFromString("(?class ?x)"));
    System.out.println("Pattern2 is: " + pattern2);

    { Values2 resd = kb.tuple_store.fetch(pattern2, true);
      System.out.println("Bindings are: " + resd); }


    Cons pattern3 = (Cons)(LispReader.readFromString("(?class foo3)"));
    { Node resa = kb.tuple_store.fetch_one_binding(pattern3);
      System.out.println("One binding is: " + resa); }

    kb.uninclude(kb2);
    System.out.println("Kb unincludes kb2");

    { Values2 rese = kb.tuple_store.fetch(pattern2, true);
      System.out.println("Bindings are: " + rese); }

    { Node resa = kb.tuple_store.fetch_one_binding(pattern3);
      System.out.println("One binding is (should be :fail): " + resa); }

/***********************************************************************
  ***********************************************************************/


  }


}


class TupleKbFilenameFilter implements FilenameFilter {
  public boolean accept(File f, String s) {
    return s.endsWith(   TupleKb.portable_kb_file_extension) ||
           s.endsWith(TupleKb.nonportable_kb_file_extension);
  }
}
