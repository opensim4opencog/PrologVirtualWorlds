/***********************************************************************

  OKBC Implemementation for Java

  Copyright (c) 1996, 1997 Stanford University KSL.

  Original code by James Rice (rice@ksl.stanford.edu)


  This code was automatically generated on Tuesday, 21 December 1999.

  KSL Network Services available at:
            http://ontolingua.stanford.edu/


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
public class CachingKb extends StandardDefaultsKb {

  Trie cache = new Trie();
  public boolean allow_caching_p = true;
  public static final Symbol _agressive = Symbol.keyword("AGRESSIVE");
  public static final Symbol _defensive = Symbol.keyword("DEFENSIVE");
  public Node caching_policy = _defensive;

/******************************************************************************


The following methods are the hand-written required client-side Network-OKBC
stub methods.  They are hand written either because they are not part of the
OKBC spec/defined using defokbcop, or have something magical with respect to
the network implementation.

http::login-user-and-create-session
http::login-user-and-connect-to-session
http::network-kill-session
http::network-active-sessions
create-kb-internal
get-all-kbs

******************************************************************************/

  // Handle cache flushing
  public void flush_cache () {
    if (cache.value == null && cache.arcs == null) 
      {} // then do nothing, the cache is already empty
    else { cache = new Trie(); }
  }

  public void flush_cache (Object arg) {
    flush_cache();
  }

  public Node cache_entry_found_p (Node key) {
    if (allow_caching_p)
      {
	TrieResult trie_result = cache.get_trie_returning_node(key);
	if (trie_result.found_p) { return _T; }
	else { return _NIL; }
      }
    else { return _NIL; }
  }

  public void encache (Object values, Node key) {
    if (allow_caching_p)
      {
	TrieResult trie_result = cache.get_trie_returning_node(key);
	trie_result.trie_node.value = values;
      }
  }

/******************************************************************************

  The following methods are likely to be shadowed by KB subclasses.

******************************************************************************/

  static Symbol caching_kb_abstract_name = null;


  // this is an abstract superclass.  We use this
  // just to get the kb type registered.
  private CachingKb (boolean x) {}

  protected CachingKb () {
    if (this.getClass().getName().equals("CachingKb"))
      { throw new RuntimeException("Cannot create a CachingKb."); }
  }

  public static void ensure_registered () {
    if (caching_kb_abstract_name == null)
      { caching_kb_abstract_name = (Symbol)(register_concrete_kb_type
           (Symbol.intern_in_okbc("ABSTRACT-CACHING-KB-KB"),
	    get_kb_type_internal()));
      }
  }

  static CachingKb caching_kb_type = null;

  public static Kb get_kb_type_internal () {
    if (caching_kb_type == null)
      { CachingKb kb = new CachingKb(true);
	kb.kb_type_name = Symbol.intern_in_ok_back("CACHING-KB");
	caching_kb_type = kb;
	return kb;
      }
    else { return caching_kb_type; }
  }

  public static final Symbol _yes = Symbol.keyword("YES");
  public static final Symbol _flush = Symbol.keyword("FLUSH");
  public static final Symbol _testing = Symbol.keyword("TESTING");

  public static final Values2 nil_t_values = new Values2(_NIL, _T);

  public void allow_caching (Object arg) {
    allow_caching_p = true;
  }

  public void disallow_caching (Object arg) {
    allow_caching_p = false;
  }

  // Hooks
  public Values2 cache_miss_hook (Node function, Node key) {
    return nil_t_values;
  }

  // Hooks
  public void cache_hit_hook (Node function, Node key, TrieResult trie_result)
  { // do nothing.
  }

  public void cache_fill_hook (Node function, Node key, Object new_results,
                        boolean multiple_values_p) {
    if (function == get_frame_details_fname)
      { get_frame_details_cache_fill_hook(key, (Values2)new_results); }
  }

  public void get_frame_details_cache_fill_hook(Node key, Values2 new_values)
  { Node new_results = new_values.firstValue();
    Node new_exact_p = new_values.secondValue();
    key = key.cdr(); // pop fname
    key = key.cdr(); // pop kb
    Node frame = key.car();
    key = key.cdr(); // pop
    InferenceLevel inference_level = (InferenceLevel)key.car();
    key = key.cdr(); // pop
    Node number_of_values = key.car();
    key = key.cdr(); // pop
    TruthValue kb_local_only_p = (TruthValue)key.car();

    Node handle = Cons.getf(new_results, _handle);
    Node name = Cons.getf(new_results, _name);
    Node pretty_name = Cons.getf(new_results, _pretty_name);
    Node type = Cons.getf(new_results, _frame_type);
    Node primitive_p = Cons.getf(new_results, _primitive_p);
    Values3 superclasses = new Values3(Cons.getf(new_results, _superclasses),
				       new_exact_p, _NIL);
    Values3 subclasses = new Values3(Cons.getf(new_results, _subclasses),
				     new_exact_p, _NIL);
    Values3 types = new Values3(Cons.getf(new_results, _types), new_exact_p,
				_NIL);
    Node own_slot_specs = Cons.getf(new_results, _own_slots);
    Node template_slot_specs = Cons.getf(new_results, _template_slots);
    Node own_facet_specs = Cons.getf(new_results, _own_facets);
    Node template_facet_specs = Cons.getf(new_results, _template_facets);

    encache_get_frame_handle(handle, frame, kb_local_only_p);
    encache_get_frame_name(name, frame, kb_local_only_p);
    encache_get_frame_pretty_name(pretty_name, frame, kb_local_only_p);
    encache_get_frame_type(type, frame, kb_local_only_p);
    if (type == _class)
      {
	encache_get_class_superclasses(superclasses, frame,
				       inference_level, number_of_values,
				       kb_local_only_p); 
	encache_get_class_subclasses(subclasses, frame, inference_level,
				     number_of_values, kb_local_only_p); 
	encache_primitive_p(primitive_p, frame, kb_local_only_p);
      }
    encache_get_instance_types(types, frame, inference_level, 
			       number_of_values, kb_local_only_p); 
    encache_slot_and_facet_specs
      (own_slot_specs, own_facet_specs, frame, _own, inference_level,
       kb_local_only_p, new_exact_p);
    encache_slot_and_facet_specs
      (template_slot_specs, template_facet_specs, frame, _template, 
       inference_level, kb_local_only_p, new_exact_p);
  }

  void encache_slot_and_facet_specs (Node slots, Node facets, Node frame,
				     SlotType slot_type, 
				     InferenceLevel inference_level,
				     TruthValue kb_local_only_p,
				     Node new_exact_p) {
    Node slot_list = _NIL;
    Enumeration e = slots.elements();
    for (; e.hasMoreElements() ;) {
      Node spec = (Cons)(e.nextElement());
      Node slot = spec.car();
      Node values = spec.cdr();
      if (values == null) { values = _NIL; }
      Enumeration evalues = values.elements();
      Node vals = _NIL;
      Node dvals = _NIL;
      for (; evalues.hasMoreElements() ;) {
	Node value = (Node)(evalues.nextElement());
	if ((value instanceof Cons) && _default == value.car())
	  { dvals = new Cons(value.cadr(), dvals); }
	else { vals = new Cons(value, vals); }
      }
      // HACK ALERT!! We are not handling the other values properly!
      encache_get_slot_values(new Values4(vals, new_exact_p, _NIL, _NIL),
			      frame, slot, inference_level, slot_type, _all,
			      _known_true, kb_local_only_p);
      encache_get_slot_values(new Values4(dvals, new_exact_p, _NIL,_NIL),
			      frame, slot, inference_level, slot_type, _all,
			      _default_only, kb_local_only_p);
      if (!(Cons.member_eql(slot, slot_list, this, kb_local_only_p)))
	{ slot_list = new Cons(slot, slot_list); }
    }
    Node facet_list = _NIL;
    e = facets.elements();
    for (; e.hasMoreElements() ;) {
      Node spec = (Cons)(e.nextElement());
      Node slot = spec.car();
      Node facet_spec = spec.cdr();
      if (facet_spec == null) { facet_spec = _NIL; }
      if (!(Cons.member_eql(slot, slot_list, this, kb_local_only_p)))
	{ slot_list = new Cons(slot, slot_list); }
      Enumeration f = slots.elements();
      for (; f.hasMoreElements() ;) {
	Node facet_and_vals_spec = (Cons)(f.nextElement());
	Node facet = facet_and_vals_spec.car();
	Node values = facet_and_vals_spec.cdr();
	if (values == null) { values = _NIL; }
	Enumeration evalues = values.elements();
	Node vals = _NIL;
	Node dvals = _NIL;
	for (; evalues.hasMoreElements() ;) {
	  Node value = (Node)(evalues.nextElement());
	  if ((value instanceof Cons) && _default == value.car())
	    { dvals = new Cons(value.cadr(), dvals); }
	  else { vals = new Cons(value, vals); }
	}
	encache_get_facet_values
	  (new Values4(values, new_exact_p, _NIL, _NIL),
	   frame, slot, facet, inference_level,
	   slot_type, _all, _known_true, kb_local_only_p);
	encache_get_facet_values
	  (new Values4(values, new_exact_p, _NIL, _NIL),
	   frame, slot, facet, inference_level,
	   slot_type, _all, _default_only, kb_local_only_p);
	if (!(Cons.member_eql(facet, facet_list, this, kb_local_only_p)))
	  { facet_list = new Cons(facet, facet_list); }
      }
      // HACK ALERT.  WE AREN'T HANDLING EXACT-P ETC. PROPERLY HERE.
      encache_get_slot_facets
	(new Values3(facet_list, new_exact_p, _NIL), frame, slot, 
	 inference_level, slot_type, kb_local_only_p);
    }
    encache_get_frame_slots
      (new Values3(slot_list, new_exact_p, _NIL), frame, inference_level,
       slot_type, kb_local_only_p);
  }

/******************************************************************************


The remaiing methods in this file were programmatically generated.

******************************************************************************/


// ---------------------- member-behavior-values-p

/**
 * Returns   when   value  is one of the variants of
 *   behavior  that is currently active for   kb , and returns
 *   otherwise.
 */
  public Node member_behavior_values_p
     (Node behavior,
      Node value) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_behavior_values_p_fname,
                       this,
                       behavior,
                       value);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(member_behavior_values_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(member_behavior_values_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.member_behavior_values_p(behavior,
                                                          value);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.member_behavior_values_p(behavior,
                                                              value);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(member_behavior_values_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("member_behavior_values_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(member_behavior_values_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.member_behavior_values_p(behavior,
                                               value);
       }
    }


  public Node cached_p_member_behavior_values_p
     (Node behavior,
      Node value) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_behavior_values_p_fname,
                       this,
                       behavior,
                       value);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  public void encache_member_behavior_values_p
     (Node _values_,
      Node behavior,
      Node value) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_behavior_values_p_fname,
                       this,
                       behavior,
                       value);
         encache(_values_, key);
       }
  }

// ---------------------- member-facet-value-p

/**
 * Returns   iff   value  is a value in the specified   facet 
 * of   slot  on   frame , as determined by the predicate
 *   test , and returns   otherwise.
 */
  public Values2 member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_facet_value_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       value,
                       inference_level,
                       test,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(member_facet_value_p_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(member_facet_value_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.member_facet_value_p(frame,
                                                      slot,
                                                      facet,
                                                      value,
                                                      inference_level,
                                                      test,
                                                      slot_type,
                                                      value_selector,
                                                      kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.member_facet_value_p(frame,
                                                          slot,
                                                          facet,
                                                          value,
                                                          inference_level,
                                                          test,
                                                          slot_type,
                                                          value_selector,
                                                          kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(member_facet_value_p_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("member_facet_value_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(member_facet_value_p_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.member_facet_value_p(frame,
                                           slot,
                                           facet,
                                           value,
                                           inference_level,
                                           test,
                                           slot_type,
                                           value_selector,
                                           kb_local_only_p);
       }
    }


  public Node cached_p_member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_facet_value_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       value,
                       inference_level,
                       test,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_member_facet_value_p" that defaults kb-local-only-p to nil
  public Node cached_p_member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.cached_p_member_facet_value_p(frame,
                                               slot,
                                               facet,
                                               value,
                                               inference_level,
                                               test,
                                               slot_type,
                                               value_selector,
                                               _NIL);
  }


  // Method for "cached_p_member_facet_value_p" that defaults value-selector to either
  public Node cached_p_member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type) {
     return this.cached_p_member_facet_value_p(frame,
                                               slot,
                                               facet,
                                               value,
                                               inference_level,
                                               test,
                                               slot_type,
                                               _either);
  }


  // Method for "cached_p_member_facet_value_p" that defaults slot-type to own
  public Node cached_p_member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test) {
     return this.cached_p_member_facet_value_p(frame,
                                               slot,
                                               facet,
                                               value,
                                               inference_level,
                                               test,
                                               _own);
  }


  // Method for "cached_p_member_facet_value_p" that defaults test to equal
  public Node cached_p_member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level) {
     return this.cached_p_member_facet_value_p(frame,
                                               slot,
                                               facet,
                                               value,
                                               inference_level,
                                               _equal);
  }


  // Method for "cached_p_member_facet_value_p" that defaults inference-level to taxonomic
  public Node cached_p_member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value) {
     return this.cached_p_member_facet_value_p(frame,
                                               slot,
                                               facet,
                                               value,
                                               _taxonomic);
  }


  public void encache_member_facet_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_facet_value_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       value,
                       inference_level,
                       test,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_member_facet_value_p" that defaults kb-local-only-p to nil
  public void encache_member_facet_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.encache_member_facet_value_p(_values_,
                                       frame,
                                       slot,
                                       facet,
                                       value,
                                       inference_level,
                                       test,
                                       slot_type,
                                       value_selector,
                                       _NIL);
  }


  // Method for "encache_member_facet_value_p" that defaults value-selector to either
  public void encache_member_facet_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type) {
     this.encache_member_facet_value_p(_values_,
                                       frame,
                                       slot,
                                       facet,
                                       value,
                                       inference_level,
                                       test,
                                       slot_type,
                                       _either);
  }


  // Method for "encache_member_facet_value_p" that defaults slot-type to own
  public void encache_member_facet_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test) {
     this.encache_member_facet_value_p(_values_,
                                       frame,
                                       slot,
                                       facet,
                                       value,
                                       inference_level,
                                       test,
                                       _own);
  }


  // Method for "encache_member_facet_value_p" that defaults test to equal
  public void encache_member_facet_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level) {
     this.encache_member_facet_value_p(_values_,
                                       frame,
                                       slot,
                                       facet,
                                       value,
                                       inference_level,
                                       _equal);
  }


  // Method for "encache_member_facet_value_p" that defaults inference-level to taxonomic
  public void encache_member_facet_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      Node value) {
     this.encache_member_facet_value_p(_values_,
                                       frame,
                                       slot,
                                       facet,
                                       value,
                                       _taxonomic);
  }


// ---------------------- member-slot-value-p

/**
 * Returns   iff   value  is a value in   slot  of
 *   frame , as determined by the predicate   test , and returns
 *   otherwise.
 */
  public Values2 member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_slot_value_p_fname,
                       this,
                       frame,
                       slot,
                       value,
                       inference_level,
                       test,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(member_slot_value_p_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(member_slot_value_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.member_slot_value_p(frame,
                                                     slot,
                                                     value,
                                                     inference_level,
                                                     test,
                                                     slot_type,
                                                     value_selector,
                                                     kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.member_slot_value_p(frame,
                                                         slot,
                                                         value,
                                                         inference_level,
                                                         test,
                                                         slot_type,
                                                         value_selector,
                                                         kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(member_slot_value_p_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("member_slot_value_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(member_slot_value_p_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.member_slot_value_p(frame,
                                          slot,
                                          value,
                                          inference_level,
                                          test,
                                          slot_type,
                                          value_selector,
                                          kb_local_only_p);
       }
    }


  public Node cached_p_member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_slot_value_p_fname,
                       this,
                       frame,
                       slot,
                       value,
                       inference_level,
                       test,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_member_slot_value_p" that defaults kb-local-only-p to nil
  public Node cached_p_member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.cached_p_member_slot_value_p(frame,
                                              slot,
                                              value,
                                              inference_level,
                                              test,
                                              slot_type,
                                              value_selector,
                                              _NIL);
  }


  // Method for "cached_p_member_slot_value_p" that defaults value-selector to either
  public Node cached_p_member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type) {
     return this.cached_p_member_slot_value_p(frame,
                                              slot,
                                              value,
                                              inference_level,
                                              test,
                                              slot_type,
                                              _either);
  }


  // Method for "cached_p_member_slot_value_p" that defaults slot-type to own
  public Node cached_p_member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test) {
     return this.cached_p_member_slot_value_p(frame,
                                              slot,
                                              value,
                                              inference_level,
                                              test,
                                              _own);
  }


  // Method for "cached_p_member_slot_value_p" that defaults test to equal
  public Node cached_p_member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level) {
     return this.cached_p_member_slot_value_p(frame,
                                              slot,
                                              value,
                                              inference_level,
                                              _equal);
  }


  // Method for "cached_p_member_slot_value_p" that defaults inference-level to taxonomic
  public Node cached_p_member_slot_value_p
     (Node frame,
      Node slot,
      Node value) {
     return this.cached_p_member_slot_value_p(frame,
                                              slot,
                                              value,
                                              _taxonomic);
  }


  public void encache_member_slot_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(member_slot_value_p_fname,
                       this,
                       frame,
                       slot,
                       value,
                       inference_level,
                       test,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_member_slot_value_p" that defaults kb-local-only-p to nil
  public void encache_member_slot_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.encache_member_slot_value_p(_values_,
                                      frame,
                                      slot,
                                      value,
                                      inference_level,
                                      test,
                                      slot_type,
                                      value_selector,
                                      _NIL);
  }


  // Method for "encache_member_slot_value_p" that defaults value-selector to either
  public void encache_member_slot_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type) {
     this.encache_member_slot_value_p(_values_,
                                      frame,
                                      slot,
                                      value,
                                      inference_level,
                                      test,
                                      slot_type,
                                      _either);
  }


  // Method for "encache_member_slot_value_p" that defaults slot-type to own
  public void encache_member_slot_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test) {
     this.encache_member_slot_value_p(_values_,
                                      frame,
                                      slot,
                                      value,
                                      inference_level,
                                      test,
                                      _own);
  }


  // Method for "encache_member_slot_value_p" that defaults test to equal
  public void encache_member_slot_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level) {
     this.encache_member_slot_value_p(_values_,
                                      frame,
                                      slot,
                                      value,
                                      inference_level,
                                      _equal);
  }


  // Method for "encache_member_slot_value_p" that defaults inference-level to taxonomic
  public void encache_member_slot_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node value) {
     this.encache_member_slot_value_p(_values_,
                                      frame,
                                      slot,
                                      value,
                                      _taxonomic);
  }


// ---------------------- primitive-p

/**
 * Returns   iff   class  is a class whose definition is
 * primitive, and   otherwise.  For KRSs that do not distinguish
 * primitive from defined classes,   primitive-p 
 * must return   for all classes.
 */
  public Node primitive_p
     (Node the_class,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(primitive_p_fname,
                       this,
                       the_class,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(primitive_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(primitive_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.primitive_p(the_class,
                                             kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.primitive_p(the_class,
                                                 kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(primitive_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("primitive_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(primitive_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.primitive_p(the_class,
                                  kb_local_only_p);
       }
    }


  public Node cached_p_primitive_p
     (Node the_class,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(primitive_p_fname,
                       this,
                       the_class,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_primitive_p" that defaults kb-local-only-p to nil
  public Node cached_p_primitive_p
     (Node the_class) {
     return this.cached_p_primitive_p(the_class,
                                      _NIL);
  }


  public void encache_primitive_p
     (Node _values_,
      Node the_class,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(primitive_p_fname,
                       this,
                       the_class,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_primitive_p" that defaults kb-local-only-p to nil
  public void encache_primitive_p
     (Node _values_,
      Node the_class) {
     this.encache_primitive_p(_values_,
                              the_class,
                              _NIL);
  }


// ---------------------- print-frame

/**
 * Prints a textual representation to   stream  of the
 *   frame .  A warning is printed when   frame  is not
 * coercible to a frame.  Stream objects as values to the   stream 
 * argument are generally system- and implementation-specific, so stream
 * objects will not in general be
 * transmissible in networked implementations.  Two special values for the
 *   stream  argument are also supported.  If   stream  is  ,
 * then   print-frame  prints to a string stream and
 * that string is returned as   false-or-string .
 * If   stream  is  ,   print-frame  attempts to print to the
 * standard output stream    of the server on which    print-frame 
 *    runs.   When   kb  is
 * a network-acessed KB, this latter option is unlikely to have a useful
 * effect.  Unless   stream  is  ,   false-or-string  is
 *  .
 * 
 * The   slots  and   facets  arguments control which slots (facets)
 * are to be displayed.  They can each take on one of the following values:
 *  
 *      :all  -- Shows all applicable slots (facets)
 *      :none  -- Shows no slots (facets)
 *      :filled  -- Shows the subset of slots (facets) that have at
 * least one value for   frame 
 *   list of slots (facets) -- Only the listed slots (facets) are shown,
 * if present
 */
  public Node print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(print_frame_fname,
                       this,
                       frame,
                       slots,
                       facets,
                       stream,
                       inference_level,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(print_frame_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(print_frame_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.print_frame(frame,
                                             slots,
                                             facets,
                                             stream,
                                             inference_level,
                                             number_of_values,
                                             value_selector,
                                             kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.print_frame(frame,
                                                 slots,
                                                 facets,
                                                 stream,
                                                 inference_level,
                                                 number_of_values,
                                                 value_selector,
                                                 kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(print_frame_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("print_frame"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(print_frame_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.print_frame(frame,
                                  slots,
                                  facets,
                                  stream,
                                  inference_level,
                                  number_of_values,
                                  value_selector,
                                  kb_local_only_p);
       }
    }


  public Node cached_p_print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(print_frame_fname,
                       this,
                       frame,
                       slots,
                       facets,
                       stream,
                       inference_level,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_print_frame" that defaults kb-local-only-p to nil
  public Node cached_p_print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.cached_p_print_frame(frame,
                                      slots,
                                      facets,
                                      stream,
                                      inference_level,
                                      number_of_values,
                                      value_selector,
                                      _NIL);
  }


  // Method for "cached_p_print_frame" that defaults value-selector to either
  public Node cached_p_print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.cached_p_print_frame(frame,
                                      slots,
                                      facets,
                                      stream,
                                      inference_level,
                                      number_of_values,
                                      _either);
  }


  // Method for "cached_p_print_frame" that defaults number-of-values to all
  public Node cached_p_print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level) {
     return this.cached_p_print_frame(frame,
                                      slots,
                                      facets,
                                      stream,
                                      inference_level,
                                      _all);
  }


  // Method for "cached_p_print_frame" that defaults inference-level to taxonomic
  public Node cached_p_print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream) {
     return this.cached_p_print_frame(frame,
                                      slots,
                                      facets,
                                      stream,
                                      _taxonomic);
  }


  // Method for "cached_p_print_frame" that defaults stream to t
  public Node cached_p_print_frame
     (Node frame,
      Node slots,
      Node facets) {
     return this.cached_p_print_frame(frame,
                                      slots,
                                      facets,
                                      _T);
  }


  // Method for "cached_p_print_frame" that defaults facets to filled
  public Node cached_p_print_frame
     (Node frame,
      Node slots) {
     return this.cached_p_print_frame(frame,
                                      slots,
                                      _filled);
  }


  // Method for "cached_p_print_frame" that defaults slots to filled
  public Node cached_p_print_frame
     (Node frame) {
     return this.cached_p_print_frame(frame,
                                      _filled);
  }


  public void encache_print_frame
     (Node _values_,
      Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(print_frame_fname,
                       this,
                       frame,
                       slots,
                       facets,
                       stream,
                       inference_level,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_print_frame" that defaults kb-local-only-p to nil
  public void encache_print_frame
     (Node _values_,
      Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values,
      ValueSelector value_selector) {
     this.encache_print_frame(_values_,
                              frame,
                              slots,
                              facets,
                              stream,
                              inference_level,
                              number_of_values,
                              value_selector,
                              _NIL);
  }


  // Method for "encache_print_frame" that defaults value-selector to either
  public void encache_print_frame
     (Node _values_,
      Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values) {
     this.encache_print_frame(_values_,
                              frame,
                              slots,
                              facets,
                              stream,
                              inference_level,
                              number_of_values,
                              _either);
  }


  // Method for "encache_print_frame" that defaults number-of-values to all
  public void encache_print_frame
     (Node _values_,
      Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level) {
     this.encache_print_frame(_values_,
                              frame,
                              slots,
                              facets,
                              stream,
                              inference_level,
                              _all);
  }


  // Method for "encache_print_frame" that defaults inference-level to taxonomic
  public void encache_print_frame
     (Node _values_,
      Node frame,
      Node slots,
      Node facets,
      Node stream) {
     this.encache_print_frame(_values_,
                              frame,
                              slots,
                              facets,
                              stream,
                              _taxonomic);
  }


  // Method for "encache_print_frame" that defaults stream to t
  public void encache_print_frame
     (Node _values_,
      Node frame,
      Node slots,
      Node facets) {
     this.encache_print_frame(_values_,
                              frame,
                              slots,
                              facets,
                              _T);
  }


  // Method for "encache_print_frame" that defaults facets to filled
  public void encache_print_frame
     (Node _values_,
      Node frame,
      Node slots) {
     this.encache_print_frame(_values_,
                              frame,
                              slots,
                              _filled);
  }


  // Method for "encache_print_frame" that defaults slots to filled
  public void encache_print_frame
     (Node _values_,
      Node frame) {
     this.encache_print_frame(_values_,
                              frame,
                              _filled);
  }


// ---------------------- put-behavior-values

/**
 * Sets the list of active   values  of the   behavior  under
 * which the KB is to operate.  The elements in   values  must be a
 * subset of the values returned by a call to
 *   get-behavior-supported-values  for the same   behavior .
 * If they are not, an   illegal-behavior-values  error will be signaled.
 * Note that for some behaviors, the order of values is significant (e.g.,
 *    :collection-type ).  Returns no values.
 */
  public void put_behavior_values
     (Node behavior,
      Node values) {
     super.put_behavior_values(behavior,
                               values);
    }


// ---------------------- put-class-superclasses

/**
 * Changes   class  to be a subclass of all the classes listed in
 *   new-superclasses .  If   frame  was a subclass of any
 * superclasses not mentioned in   new-superclasses , these superclasses
 * are removed.
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  public void put_class_superclasses
     (Node the_class,
      Node new_superclasses,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_class_superclasses");
         flush_cache("put_class_superclasses"); }
     try { connection().ok_to_cache_flag = _testing;
           super.put_class_superclasses(the_class,
                                        new_superclasses,
                                        kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_class_superclasses"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- put-facet-value

/**
 * Sets the values of the specified facet to be a singleton set
 * consisting of a single element:   value .  Returns no values.
 */
  public void put_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_facet_value");
         flush_cache("put_facet_value"); }
     try { connection().ok_to_cache_flag = _testing;
           super.put_facet_value(frame,
                                 slot,
                                 facet,
                                 value,
                                 slot_type,
                                 value_selector,
                                 kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_facet_value"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- put-facet-values

/**
 * Sets the values of the specified facet to be
 *   values , which is assumed to be a set.  Any existing facet values
 * that are not in   values  will be removed.  The order of the elements of
 *   values  will not necessarily be maintained by the KRS.
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  public void put_facet_values
     (Node frame,
      Node slot,
      Node facet,
      Node values,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_facet_values");
         flush_cache("put_facet_values"); }
     try { connection().ok_to_cache_flag = _testing;
           super.put_facet_values(frame,
                                  slot,
                                  facet,
                                  values,
                                  slot_type,
                                  value_selector,
                                  kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_facet_values"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- put-frame-details

/**
 * Redefines   frame  to have the specified   details .
 *   Details  is a property list as specified for
 *   get-frame-details .  This operation is useful for systems that allow
 * transaction-oriented editing of multiple aspects of a frame.
 * The properties    :handle ,    :frame-type , and    :primitive-p 
 * are ignored, since these may not be put.
 * Returns no values.
 */
  public void put_frame_details
     (Node frame,
      Node details,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_frame_details");
         flush_cache("put_frame_details"); }
     try { connection().ok_to_cache_flag = _testing;
           super.put_frame_details(frame,
                                   details,
                                   kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_frame_details"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- put-frame-name

/**
 * Changes the name of   frame  to be   new-name .  All references
 * to   frame  in   kb  (e.g., in slot values) will point to the
 * frame named   new-name .  Returns the frame with the new name,
 *   renamed-frame .  It is not necessary that the frame object identified
 * by   frame  be identical (   == /   EQLness ) to the frame
 * object called   new-name , only that the KB consistently use the
 * new frame instead of the old one.
 * 
 * Implementation note:  KRSs that use frame names as frame handles must
 * replace    all  references to the old name of   frame  with
 *   new-name .  This specification allows for implementations that
 * are forced to replace the representation of the frame with a new, renamed
 * version.
 */
  public Node put_frame_name
     (Node frame,
      Node new_name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_frame_name");
         flush_cache("put_frame_name"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.put_frame_name(frame,
                                       new_name,
                                       kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_frame_name"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- put-frame-pretty-name

/**
 * Stores the   name , a string, as the new pretty-name of the
 *   frame .  OKBC mandates no constraints on the new pretty-  name ,
 * but to maximize the liklihood that applications will interoperate smoothly,
 * implementations are encouraged to make pretty-names be short, and are
 * strongly encouraged to include no whitespace characters other than the space
 * characters, or any display device-specific control characters.  Returns no
 * values.
 */
  public void put_frame_pretty_name
     (Node frame,
      Node name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_frame_pretty_name");
         flush_cache("put_frame_pretty_name"); }
     try { connection().ok_to_cache_flag = _testing;
           super.put_frame_pretty_name(frame,
                                       name,
                                       kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_frame_pretty_name"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- put-instance-types

/**
 * Changes   frame  to be an instance of all the classes listed in
 *   new-types .  If   frame  was an instance of any types not
 * mentioned in   new-types , these types are removed.
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  public void put_instance_types
     (Node frame,
      Node new_types,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_instance_types");
         flush_cache("put_instance_types"); }
     try { connection().ok_to_cache_flag = _testing;
           super.put_instance_types(frame,
                                    new_types,
                                    kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_instance_types"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- put-slot-value

/**
 * Sets the values of   slot  in   frame  to be a singleton set
 * consisting of a single element:    value .
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  public void put_slot_value
     (Node frame,
      Node slot,
      Node value,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_slot_value");
         flush_cache("put_slot_value"); }
     try { connection().ok_to_cache_flag = _testing;
           super.put_slot_value(frame,
                                slot,
                                value,
                                slot_type,
                                value_selector,
                                kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_slot_value"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- put-slot-values

/**
 * Sets the values of   slot  in   frame  to
 * be   values .  Any existing slot values that are not in   values 
 * will be removed.  The order of the elements
 * of   values  will not necessarily be maintained by the KRS, unless the
 *    :collection-type  of the slot is    :list .
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  public void put_slot_values
     (Node frame,
      Node slot,
      Node values,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("put_slot_values");
         flush_cache("put_slot_values"); }
     try { connection().ok_to_cache_flag = _testing;
           super.put_slot_values(frame,
                                 slot,
                                 values,
                                 slot_type,
                                 value_selector,
                                 kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("put_slot_values"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- register-procedure

/**
 * Associates the   procedure  with   name  in   kb .
 * Subsequent calls to   call-procedure  may invoke the procedure merely by
 * using the name.  If   name  is  , then   procedure 
 * must be a    named  procedure, in which case the   name 
 * argument will default to the name of the procedure.
 * Returns no values.
 */
  public void register_procedure
     (Node name,
      Node procedure) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("register_procedure");
         flush_cache("register_procedure"); }
     try { connection().ok_to_cache_flag = _testing;
           super.register_procedure(name,
                                    procedure);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("register_procedure"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- remove-class-superclass

/**
 * Removes   superclass-to-remove  from the superclasses of   class .
 * Returns no values.
 */
  public void remove_class_superclass
     (Node the_class,
      Node superclass_to_remove,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("remove_class_superclass");
         flush_cache("remove_class_superclass"); }
     try { connection().ok_to_cache_flag = _testing;
           super.remove_class_superclass(the_class,
                                         superclass_to_remove,
                                         kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("remove_class_superclass"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- remove-facet-value

/**
 * If   value  is currently a member of the set of direct values of
 * the specified facet, then   value  is removed from the values of
 * the facet.  Returns no values.
 */
  public void remove_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("remove_facet_value");
         flush_cache("remove_facet_value"); }
     try { connection().ok_to_cache_flag = _testing;
           super.remove_facet_value(frame,
                                    slot,
                                    facet,
                                    value,
                                    test,
                                    slot_type,
                                    value_selector,
                                    kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("remove_facet_value"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- remove-instance-type

/**
 * Removes   type-to-remove  from the types of   frame  -- that is,
 * makes   frame  no longer be an instance of   type-to-remove .
 * Returns no values.
 */
  public void remove_instance_type
     (Node frame,
      Node type_to_remove,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("remove_instance_type");
         flush_cache("remove_instance_type"); }
     try { connection().ok_to_cache_flag = _testing;
           super.remove_instance_type(frame,
                                      type_to_remove,
                                      kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("remove_instance_type"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- remove-local-facet-values

/**
 * Removes all direct values of   facet  in   slot  of   frame .
 * Returns no values.
 */
  public void remove_local_facet_values
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("remove_local_facet_values");
         flush_cache("remove_local_facet_values"); }
     try { connection().ok_to_cache_flag = _testing;
           super.remove_local_facet_values(frame,
                                           slot,
                                           facet,
                                           slot_type,
                                           value_selector,
                                           kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("remove_local_facet_values"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- remove-local-slot-values

/**
 * Removes all direct values in   slot  of   frame .
 * Returns no values.
 */
  public void remove_local_slot_values
     (Node frame,
      Node slot,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("remove_local_slot_values");
         flush_cache("remove_local_slot_values"); }
     try { connection().ok_to_cache_flag = _testing;
           super.remove_local_slot_values(frame,
                                          slot,
                                          slot_type,
                                          value_selector,
                                          kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("remove_local_slot_values"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- remove-slot-value

/**
 * If   value  is currently a member of the set of direct values of
 *   slot , then   value  is removed from the values of
 *   slot .  Only values matching the   test  are removed.
 * If   index  is    :all , then all occurrences of
 *   value  will be removed.  Otherwise,   index  should be
 * an integer index into the values list, and only the value at that
 * position, if it matches   value , will be removed (the first
 * value of the slot has index 0).  The index argument is used only by slots
 * whose    :collection-type  is    :list .  Returns no values.
 */
  public void remove_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type,
      Node index,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("remove_slot_value");
         flush_cache("remove_slot_value"); }
     try { connection().ok_to_cache_flag = _testing;
           super.remove_slot_value(frame,
                                   slot,
                                   value,
                                   test,
                                   slot_type,
                                   index,
                                   value_selector,
                                   kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("remove_slot_value"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- rename-facet

/**
 * Renames the facet for all frames containing that facet.
 *  
 *   If the facet identified by   facet  is represented as a frame,
 * that frame is renamed.
 *   If the facet identified by   facet  is not represented as
 * a frame,   facet-p  applied to   facet  will return
 *    and   facet  will not be returned by any of
 * the facet-returning operations, such as   get-kb-facets .
 *   New-name  will now identify the
 * facet, and will be returned by operations such as
 *   get-kb-facets  and   get-frame-facets .
 *  
 * All the facet values and facet values associated with   facet  are
 * preserved under the   new-name .  For example, for any frame in
 * the KB, the values returned by   get-facet-values  for   facet 
 * before the rename are identical to the values returned  for
 *   new-name  after the rename.  In addition, the    attached-to 
 * relationship is preserved -- that is, if   facet  is attached to a
 * frame before the rename,   new-name  is attached to that frame
 * after the rename.  In some implementations, references to   facet 
 * may still remain in the KB after   rename-facet .  Returns the
 *   renamed-facet .
 */
  public Node rename_facet
     (Node facet,
      Node new_name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("rename_facet");
         flush_cache("rename_facet"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.rename_facet(facet,
                                     new_name,
                                     kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("rename_facet"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- rename-slot

/**
 * Renames the slot for all frames containing that slot.
 *  
 *   If the slot identified by   slot  is represented as a frame,
 * that frame is renamed.
 *   If the slot identified by   slot  is not represented as
 * a frame,   slot-p  applied to   slot  will return   
 * and   slot  will not be returned by any of the slot-returning
 * operations, such as   get-kb-slots  and   get-frame-slots .
 *   New-name  will now identify the slot, and will be returned by
 * operations such as   get-kb-slots  and   get-frame-slots .
 *  
 * All the slot values and facet values associated with   slot  are
 * preserved under the   new-name .  For example, for any frame in
 * the KB, the values returned by   get-slot-values  for   slot 
 * before the rename are identical to the values returned  for
 *   new-name  after the rename.  In addition, the    attached-to 
 * relationship is preserved -- that is, if   slot  is attached to a
 * frame before the rename,   new-name  is attached to that frame
 * after the rename.  In some implementations, references to   slot 
 * may still remain in the KB after   rename-slot .  Returns the
 *   renamed-slot .
 */
  public Node rename_slot
     (Node slot,
      Node new_name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("rename_slot");
         flush_cache("rename_slot"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.rename_slot(slot,
                                    new_name,
                                    kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("rename_slot"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- replace-facet-value

/**
 * If   old-value  is currently a member of the set of direct values of
 * the specified facet, then   old-value  is replaced by
 *   new-value  in the facet.  Returns no values.
 */
  public void replace_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("replace_facet_value");
         flush_cache("replace_facet_value"); }
     try { connection().ok_to_cache_flag = _testing;
           super.replace_facet_value(frame,
                                     slot,
                                     facet,
                                     old_value,
                                     new_value,
                                     test,
                                     slot_type,
                                     value_selector,
                                     kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("replace_facet_value"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- replace-slot-value

/**
 * If   old-value  is currently a member of the set of direct values of
 *   slot , then   old-value  is replaced by   new-value  in
 *   slot .  If   index  is    :all  then all occurrences of
 *   old-value  will be replaced.  Otherwise,   index  should be
 * an integer index into the values list, and only the value at that
 * position, if it matches   old-value , will be replaced (the first
 * value of the slot has index 0).
 * This operation may signal constraint violation conditions
 * (see Section  ).  The   index  argument is used only
 * by slots whose    :collection-type  is    :list .
 * Returns no values.
 */
  public void replace_slot_value
     (Node frame,
      Node slot,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type,
      Node index,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("replace_slot_value");
         flush_cache("replace_slot_value"); }
     try { connection().ok_to_cache_flag = _testing;
           super.replace_slot_value(frame,
                                    slot,
                                    old_value,
                                    new_value,
                                    test,
                                    slot_type,
                                    index,
                                    value_selector,
                                    kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("replace_slot_value"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- revert-kb

/**
 * This operation is called when the user wishes to discard any changes
 * made to a KB since it was last saved, and revert to the previously saved
 * state.
 * 
 * Equivalent to successive calls to   close-kb  and then   open-kb .
 * The   reverted-kb  returned has the same content as   kb  had at
 * the time it was last saved (or empty, if the kb had never
 * been saved).  No guarantee is made that the   reverted-kb 
 * will have the same identity (==, EQLness) as   kb ,
 * but some implementations may choose to recycle the existing KB object.
 * After a call to   revert-kb , portable applications must refer only
 * to the   reverted-kb  object rather than   kb  in case it was
 * not recycled.  References to the original   kb  may result in
 * an   object-freed  condition being signaled.
 * If   error-p  is  , tries to catch errors that occur, and
 * attempts to continue with reverting to the extent possible.
 */
  public Node revert_kb
     (TruthValue error_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("revert_kb");
         flush_cache("revert_kb"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.revert_kb(error_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("revert_kb"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- save-kb

/**
 * Saves the contents of the KB to persistent storage.  No commitment is
 * made as to the location of the KB in persistent storage, other than that
 * it will be openable given the name, kb-type and connection first used
 * to access it.  No commitment is made as to whether the save operation
 * results in a complete dump of the KB, or whether it results only in a
 * dump of the changes made since the KB was last saved.  If   error-p 
 * is  , tries to catch errors that occur, and attempts to continue
 * with saving to the extent possible.
 * Returns   iff the KB was saved successfully, and   
 * otherwise.
 */
  public Node save_kb
     (TruthValue error_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("save_kb");
         flush_cache("save_kb"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.save_kb(error_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("save_kb"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- save-kb-as

/**
 * Saves the entire contents of the KB to persistent storage under the
 *   new-name-or-locator .    New-name-or-locator  is either a new
 * name for the KB, or a new kb-locator.  The in-core KB is renamed so that
 *   find-kb-of-type  will return   kb  when passed the new name of
 * the KB.  Returns no values.
 */
  public void save_kb_as
     (Node new_name_or_locator) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("save_kb_as");
         flush_cache("save_kb_as"); }
     try { connection().ok_to_cache_flag = _testing;
           super.save_kb_as(new_name_or_locator);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("save_kb_as"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- slot-has-facet-p

/**
 * Returns   iff   facet  is a valid facet for   slot  on
 *   frame , and   otherwise.  What constitutes a valid facet is
 * KB-specific, but a facet with a value locally asserted, or with a value
 * that is accessible from a template slot will return   for this
 * operation.
 */
  public Node slot_has_facet_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_has_facet_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(slot_has_facet_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(slot_has_facet_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.slot_has_facet_p(frame,
                                                  slot,
                                                  facet,
                                                  inference_level,
                                                  slot_type,
                                                  kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.slot_has_facet_p(frame,
                                                      slot,
                                                      facet,
                                                      inference_level,
                                                      slot_type,
                                                      kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(slot_has_facet_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("slot_has_facet_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(slot_has_facet_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.slot_has_facet_p(frame,
                                       slot,
                                       facet,
                                       inference_level,
                                       slot_type,
                                       kb_local_only_p);
       }
    }


  public Node cached_p_slot_has_facet_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_has_facet_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_slot_has_facet_p" that defaults kb-local-only-p to nil
  public Node cached_p_slot_has_facet_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_slot_has_facet_p(frame,
                                           slot,
                                           facet,
                                           inference_level,
                                           slot_type,
                                           _NIL);
  }


  // Method for "cached_p_slot_has_facet_p" that defaults slot-type to own
  public Node cached_p_slot_has_facet_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.cached_p_slot_has_facet_p(frame,
                                           slot,
                                           facet,
                                           inference_level,
                                           _own);
  }


  // Method for "cached_p_slot_has_facet_p" that defaults inference-level to taxonomic
  public Node cached_p_slot_has_facet_p
     (Node frame,
      Node slot,
      Node facet) {
     return this.cached_p_slot_has_facet_p(frame,
                                           slot,
                                           facet,
                                           _taxonomic);
  }


  public void encache_slot_has_facet_p
     (Node _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_has_facet_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_slot_has_facet_p" that defaults kb-local-only-p to nil
  public void encache_slot_has_facet_p
     (Node _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_slot_has_facet_p(_values_,
                                   frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   slot_type,
                                   _NIL);
  }


  // Method for "encache_slot_has_facet_p" that defaults slot-type to own
  public void encache_slot_has_facet_p
     (Node _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     this.encache_slot_has_facet_p(_values_,
                                   frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   _own);
  }


  // Method for "encache_slot_has_facet_p" that defaults inference-level to taxonomic
  public void encache_slot_has_facet_p
     (Node _values_,
      Node frame,
      Node slot,
      Node facet) {
     this.encache_slot_has_facet_p(_values_,
                                   frame,
                                   slot,
                                   facet,
                                   _taxonomic);
  }


// ---------------------- slot-has-value-p

/**
 * Returns   iff   slot  on   frame  has at least one
 * value, otherwise returns  .
 */
  public Values2 slot_has_value_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_has_value_p_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(slot_has_value_p_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(slot_has_value_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.slot_has_value_p(frame,
                                                  slot,
                                                  inference_level,
                                                  slot_type,
                                                  value_selector,
                                                  kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.slot_has_value_p(frame,
                                                      slot,
                                                      inference_level,
                                                      slot_type,
                                                      value_selector,
                                                      kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(slot_has_value_p_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("slot_has_value_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(slot_has_value_p_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.slot_has_value_p(frame,
                                       slot,
                                       inference_level,
                                       slot_type,
                                       value_selector,
                                       kb_local_only_p);
       }
    }


  public Node cached_p_slot_has_value_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_has_value_p_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_slot_has_value_p" that defaults kb-local-only-p to nil
  public Node cached_p_slot_has_value_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.cached_p_slot_has_value_p(frame,
                                           slot,
                                           inference_level,
                                           slot_type,
                                           value_selector,
                                           _NIL);
  }


  // Method for "cached_p_slot_has_value_p" that defaults value-selector to either
  public Node cached_p_slot_has_value_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_slot_has_value_p(frame,
                                           slot,
                                           inference_level,
                                           slot_type,
                                           _either);
  }


  // Method for "cached_p_slot_has_value_p" that defaults slot-type to own
  public Node cached_p_slot_has_value_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.cached_p_slot_has_value_p(frame,
                                           slot,
                                           inference_level,
                                           _own);
  }


  // Method for "cached_p_slot_has_value_p" that defaults inference-level to taxonomic
  public Node cached_p_slot_has_value_p
     (Node frame,
      Node slot) {
     return this.cached_p_slot_has_value_p(frame,
                                           slot,
                                           _taxonomic);
  }


  public void encache_slot_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_has_value_p_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_slot_has_value_p" that defaults kb-local-only-p to nil
  public void encache_slot_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.encache_slot_has_value_p(_values_,
                                   frame,
                                   slot,
                                   inference_level,
                                   slot_type,
                                   value_selector,
                                   _NIL);
  }


  // Method for "encache_slot_has_value_p" that defaults value-selector to either
  public void encache_slot_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_slot_has_value_p(_values_,
                                   frame,
                                   slot,
                                   inference_level,
                                   slot_type,
                                   _either);
  }


  // Method for "encache_slot_has_value_p" that defaults slot-type to own
  public void encache_slot_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level) {
     this.encache_slot_has_value_p(_values_,
                                   frame,
                                   slot,
                                   inference_level,
                                   _own);
  }


  // Method for "encache_slot_has_value_p" that defaults inference-level to taxonomic
  public void encache_slot_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot) {
     this.encache_slot_has_value_p(_values_,
                                   frame,
                                   slot,
                                   _taxonomic);
  }


// ---------------------- slot-p

/**
 * Returns   iff   thing  is a slot, and
 * otherwise returns  .
 */
  public Node slot_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(slot_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(slot_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.slot_p(thing,
                                        kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.slot_p(thing,
                                            kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(slot_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("slot_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(slot_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.slot_p(thing,
                             kb_local_only_p);
       }
    }


  public Node cached_p_slot_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_slot_p" that defaults kb-local-only-p to nil
  public Node cached_p_slot_p
     (Node thing) {
     return this.cached_p_slot_p(thing,
                                 _NIL);
  }


  public void encache_slot_p
     (Node _values_,
      Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(slot_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_slot_p" that defaults kb-local-only-p to nil
  public void encache_slot_p
     (Node _values_,
      Node thing) {
     this.encache_slot_p(_values_,
                         thing,
                         _NIL);
  }


// ---------------------- subclass-of-p

/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
  public Values2 subclass_of_p
     (Node subclass,
      Node superclass,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(subclass_of_p_fname,
                       this,
                       subclass,
                       superclass,
                       inference_level,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(subclass_of_p_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(subclass_of_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.subclass_of_p(subclass,
                                               superclass,
                                               inference_level,
                                               kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.subclass_of_p(subclass,
                                                   superclass,
                                                   inference_level,
                                                   kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(subclass_of_p_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("subclass_of_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(subclass_of_p_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.subclass_of_p(subclass,
                                    superclass,
                                    inference_level,
                                    kb_local_only_p);
       }
    }


  public Node cached_p_subclass_of_p
     (Node subclass,
      Node superclass,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(subclass_of_p_fname,
                       this,
                       subclass,
                       superclass,
                       inference_level,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_subclass_of_p" that defaults kb-local-only-p to nil
  public Node cached_p_subclass_of_p
     (Node subclass,
      Node superclass,
      InferenceLevel inference_level) {
     return this.cached_p_subclass_of_p(subclass,
                                        superclass,
                                        inference_level,
                                        _NIL);
  }


  // Method for "cached_p_subclass_of_p" that defaults inference-level to taxonomic
  public Node cached_p_subclass_of_p
     (Node subclass,
      Node superclass) {
     return this.cached_p_subclass_of_p(subclass,
                                        superclass,
                                        _taxonomic);
  }


  public void encache_subclass_of_p
     (Values2 _values_,
      Node subclass,
      Node superclass,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(subclass_of_p_fname,
                       this,
                       subclass,
                       superclass,
                       inference_level,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_subclass_of_p" that defaults kb-local-only-p to nil
  public void encache_subclass_of_p
     (Values2 _values_,
      Node subclass,
      Node superclass,
      InferenceLevel inference_level) {
     this.encache_subclass_of_p(_values_,
                                subclass,
                                superclass,
                                inference_level,
                                _NIL);
  }


  // Method for "encache_subclass_of_p" that defaults inference-level to taxonomic
  public void encache_subclass_of_p
     (Values2 _values_,
      Node subclass,
      Node superclass) {
     this.encache_subclass_of_p(_values_,
                                subclass,
                                superclass,
                                _taxonomic);
  }


// ---------------------- superclass-of-p

/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
  public Values2 superclass_of_p
     (Node superclass,
      Node subclass,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(superclass_of_p_fname,
                       this,
                       superclass,
                       subclass,
                       inference_level,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(superclass_of_p_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(superclass_of_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.superclass_of_p(superclass,
                                                 subclass,
                                                 inference_level,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.superclass_of_p(superclass,
                                                     subclass,
                                                     inference_level,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(superclass_of_p_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("superclass_of_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(superclass_of_p_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.superclass_of_p(superclass,
                                      subclass,
                                      inference_level,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_superclass_of_p
     (Node superclass,
      Node subclass,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(superclass_of_p_fname,
                       this,
                       superclass,
                       subclass,
                       inference_level,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_superclass_of_p" that defaults kb-local-only-p to nil
  public Node cached_p_superclass_of_p
     (Node superclass,
      Node subclass,
      InferenceLevel inference_level) {
     return this.cached_p_superclass_of_p(superclass,
                                          subclass,
                                          inference_level,
                                          _NIL);
  }


  // Method for "cached_p_superclass_of_p" that defaults inference-level to taxonomic
  public Node cached_p_superclass_of_p
     (Node superclass,
      Node subclass) {
     return this.cached_p_superclass_of_p(superclass,
                                          subclass,
                                          _taxonomic);
  }


  public void encache_superclass_of_p
     (Values2 _values_,
      Node superclass,
      Node subclass,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(superclass_of_p_fname,
                       this,
                       superclass,
                       subclass,
                       inference_level,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_superclass_of_p" that defaults kb-local-only-p to nil
  public void encache_superclass_of_p
     (Values2 _values_,
      Node superclass,
      Node subclass,
      InferenceLevel inference_level) {
     this.encache_superclass_of_p(_values_,
                                  superclass,
                                  subclass,
                                  inference_level,
                                  _NIL);
  }


  // Method for "encache_superclass_of_p" that defaults inference-level to taxonomic
  public void encache_superclass_of_p
     (Values2 _values_,
      Node superclass,
      Node subclass) {
     this.encache_superclass_of_p(_values_,
                                  superclass,
                                  subclass,
                                  _taxonomic);
  }


// ---------------------- tell

/**
 * The   tell  operation asserts the   sentence  to be true in
 * the knowledge base   kb .  Some KRSs may allow users to attach a
 * sentence to a specific frame in the KB.  If that is possible and desired,
 * the   frame  argument may be supplied.  When   kb-local-only-p 
 * is  , the   sentence  should be asserted in the   kb , even
 * if   kb  includes another KB containing the   sentence .  When the
 *   sentence  argument is syntactically invalid,   tell  signals
 * a   syntax-error  error.  A KRS may not accept all valid sentences
 * of the OKBC Assertion Language, and for such cases,   tell  raises the
 * condition   cannot-handle .  Returns no values.
 */
  public void tell
     (Node sentence,
      Node frame,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     super.tell(sentence,
                frame,
                value_selector,
                kb_local_only_p);
    }


// ---------------------- tellable

/**
 * The   tellable  operation returns   if the KRS can
 * determine that   tell ing the   sentence  would result in a
 *   cannot-handle  error being signaled, and   otherwise.
 * It may raise a   syntax-error  error as specified with the
 * definition of   tell .  Even if   tellable  returns
 *  ,   tell  may still not be able to handle the
 *   sentence .
 */
  public Node tellable
     (Node sentence,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(tellable_fname,
                       this,
                       sentence,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(tellable_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(tellable_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.tellable(sentence,
                                          value_selector,
                                          kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.tellable(sentence,
                                              value_selector,
                                              kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(tellable_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("tellable"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(tellable_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.tellable(sentence,
                               value_selector,
                               kb_local_only_p);
       }
    }


  public Node cached_p_tellable
     (Node sentence,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(tellable_fname,
                       this,
                       sentence,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_tellable" that defaults kb-local-only-p to nil
  public Node cached_p_tellable
     (Node sentence,
      ValueSelector value_selector) {
     return this.cached_p_tellable(sentence,
                                   value_selector,
                                   _NIL);
  }


  // Method for "cached_p_tellable" that defaults value-selector to known-true
  public Node cached_p_tellable
     (Node sentence) {
     return this.cached_p_tellable(sentence,
                                   _known_true);
  }


  public void encache_tellable
     (Node _values_,
      Node sentence,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(tellable_fname,
                       this,
                       sentence,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_tellable" that defaults kb-local-only-p to nil
  public void encache_tellable
     (Node _values_,
      Node sentence,
      ValueSelector value_selector) {
     this.encache_tellable(_values_,
                           sentence,
                           value_selector,
                           _NIL);
  }


  // Method for "encache_tellable" that defaults value-selector to known-true
  public void encache_tellable
     (Node _values_,
      Node sentence) {
     this.encache_tellable(_values_,
                           sentence,
                           _known_true);
  }


// ---------------------- type-of-p

/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  public Values2 type_of_p
     (Node the_class,
      Node thing,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(type_of_p_fname,
                       this,
                       the_class,
                       thing,
                       inference_level,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(type_of_p_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(type_of_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.type_of_p(the_class,
                                           thing,
                                           inference_level,
                                           kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.type_of_p(the_class,
                                               thing,
                                               inference_level,
                                               kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(type_of_p_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("type_of_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(type_of_p_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.type_of_p(the_class,
                                thing,
                                inference_level,
                                kb_local_only_p);
       }
    }


  public Node cached_p_type_of_p
     (Node the_class,
      Node thing,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(type_of_p_fname,
                       this,
                       the_class,
                       thing,
                       inference_level,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_type_of_p" that defaults kb-local-only-p to nil
  public Node cached_p_type_of_p
     (Node the_class,
      Node thing,
      InferenceLevel inference_level) {
     return this.cached_p_type_of_p(the_class,
                                    thing,
                                    inference_level,
                                    _NIL);
  }


  // Method for "cached_p_type_of_p" that defaults inference-level to taxonomic
  public Node cached_p_type_of_p
     (Node the_class,
      Node thing) {
     return this.cached_p_type_of_p(the_class,
                                    thing,
                                    _taxonomic);
  }


  public void encache_type_of_p
     (Values2 _values_,
      Node the_class,
      Node thing,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(type_of_p_fname,
                       this,
                       the_class,
                       thing,
                       inference_level,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_type_of_p" that defaults kb-local-only-p to nil
  public void encache_type_of_p
     (Values2 _values_,
      Node the_class,
      Node thing,
      InferenceLevel inference_level) {
     this.encache_type_of_p(_values_,
                            the_class,
                            thing,
                            inference_level,
                            _NIL);
  }


  // Method for "encache_type_of_p" that defaults inference-level to taxonomic
  public void encache_type_of_p
     (Values2 _values_,
      Node the_class,
      Node thing) {
     this.encache_type_of_p(_values_,
                            the_class,
                            thing,
                            _taxonomic);
  }


// ---------------------- unregister-procedure

/**
 * Removes any procedure association for the   name  in   kb .
 * Returns no values.
 */
  public void unregister_procedure
     (Node name) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("unregister_procedure");
         flush_cache("unregister_procedure"); }
     try { connection().ok_to_cache_flag = _testing;
           super.unregister_procedure(name);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("unregister_procedure"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- untell

/**
 * The   untell  operation can be used to remove assertions from the
 * knowledge base.  Returns   if the sentence was removed, and
 *   otherwise.
 * 
 * The effect of   untell ing a sentence of the OKBC Assertion Language
 * is equivalent to the effect of executing an equivalent OKBC operation.
 * The OKBC operations equivalent to   untell ing a sentence in the
 * Assertion Language are specified in Section  .
 * For example, the operation
 *   
 * (Untell `(slot-name frame-name ,slot-value))
 *  
 * is equivalent to the OKBC operation    (remove-slot-value 'frame-name
 * 'slot-name slot-value :slot-type :own). 
 * 
 * The effect of   untell ing an arbitrary   tellable  sentence
 * is not predictable across implementations.  For a given sentence   
 * s , executing    (untell s :kb kb)  after executing    (tell s :kb
 * kb)  should remove the sentence    s  from the    kb .  That is, a
 * second call to   untell  should return    nil .  This
 * does not mean that    s  is no longer true, as it may be implied by
 * other assertions in the KB.
 * 
 * Some KRSs may allow users to attach an assertion to a frame in the KB.
 * If the   sentence  was attached to a frame, the   frame 
 * argument must be supplied.  The default value for   frame  is
 *  .  When the   sentence  argument is syntactically invalid, it
 * may signal the   syntax-error  error.
 */
  public Node untell
     (Node sentence,
      Node frame,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(untell_fname,
                       this,
                       sentence,
                       frame,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(untell_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(untell_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.untell(sentence,
                                        frame,
                                        value_selector,
                                        kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.untell(sentence,
                                            frame,
                                            value_selector,
                                            kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(untell_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("untell"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(untell_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.untell(sentence,
                             frame,
                             value_selector,
                             kb_local_only_p);
       }
    }


  public Node cached_p_untell
     (Node sentence,
      Node frame,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(untell_fname,
                       this,
                       sentence,
                       frame,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_untell" that defaults kb-local-only-p to nil
  public Node cached_p_untell
     (Node sentence,
      Node frame,
      ValueSelector value_selector) {
     return this.cached_p_untell(sentence,
                                 frame,
                                 value_selector,
                                 _NIL);
  }


  // Method for "cached_p_untell" that defaults value-selector to known-true
  public Node cached_p_untell
     (Node sentence,
      Node frame) {
     return this.cached_p_untell(sentence,
                                 frame,
                                 _known_true);
  }


  // Method for "cached_p_untell" that defaults frame to nil
  public Node cached_p_untell
     (Node sentence) {
     return this.cached_p_untell(sentence,
                                 _NIL);
  }


  public void encache_untell
     (Node _values_,
      Node sentence,
      Node frame,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(untell_fname,
                       this,
                       sentence,
                       frame,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_untell" that defaults kb-local-only-p to nil
  public void encache_untell
     (Node _values_,
      Node sentence,
      Node frame,
      ValueSelector value_selector) {
     this.encache_untell(_values_,
                         sentence,
                         frame,
                         value_selector,
                         _NIL);
  }


  // Method for "encache_untell" that defaults value-selector to known-true
  public void encache_untell
     (Node _values_,
      Node sentence,
      Node frame) {
     this.encache_untell(_values_,
                         sentence,
                         frame,
                         _known_true);
  }


  // Method for "encache_untell" that defaults frame to nil
  public void encache_untell
     (Node _values_,
      Node sentence) {
     this.encache_untell(_values_,
                         sentence,
                         _NIL);
  }


// ---------------------- value-as-string

/**
 * Given some   value , returns two values, a string that is a printed
 * representation of that value, and a list of three-tuples.
 * In the second   location-list  value, one tuple is supplied for each
 * frame reference encountered in   value  in the order they appear
 * in the printed representation.  Each three-tuple is of the form
 *    (index0 index1 frame) , where    index0  is the zero-indexed
 * start index of a printed representation of the
 *    frame  in the printed representation and    index1  is the
 * index one past the end.  For example, printing the list    ( <frame1>) 
 * might deliver the values "(Frame1)" and    ((1 8  <frame1>)) .
 * 
 * This operation is useful for user interfaces that have no way in
 * general to print the arbitrary data structures that might be returned by,
 * for example,   get-slot-values .  The second value allows user
 * interfaces to print the string with mouse-sensitive regions that point to
 * frames. 
 *   Purpose 
 * -- The   purpose  argument can be one of     :file ,
 *    :user-interface  .  When it is    :file , it allows the
 * KB to print data in a manner that will be readable again (e.g.,
 * for dumping to files),
 * preserving object identity where necessary.  The output will
 * therefore typically fully escape strings, and will probably
 * package qualify symbols.  When   purpose  is
 *    :user-interface , the string returned will be as pretty
 * and readable as possible for a user interface, but such
 * output will probably not be readable by OKBC. 
 *   Pretty-p 
 * -- When  , OKBC will attempt to print everything in as pretty a
 * manner as possible.  This includes printing frames by using their
 * pretty-names, and by printing strings without escaping or
 * quotes.
 */
  public Values2 value_as_string
     (Node value,
      Node purpose,
      Node pretty_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(value_as_string_fname,
                       this,
                       value,
                       purpose,
                       pretty_p,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(value_as_string_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(value_as_string_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.value_as_string(value,
                                                 purpose,
                                                 pretty_p,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.value_as_string(value,
                                                     purpose,
                                                     pretty_p,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(value_as_string_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("value_as_string"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(value_as_string_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.value_as_string(value,
                                      purpose,
                                      pretty_p,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_value_as_string
     (Node value,
      Node purpose,
      Node pretty_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(value_as_string_fname,
                       this,
                       value,
                       purpose,
                       pretty_p,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_value_as_string" that defaults kb-local-only-p to nil
  public Node cached_p_value_as_string
     (Node value,
      Node purpose,
      Node pretty_p) {
     return this.cached_p_value_as_string(value,
                                          purpose,
                                          pretty_p,
                                          _NIL);
  }


  public void encache_value_as_string
     (Values2 _values_,
      Node value,
      Node purpose,
      Node pretty_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(value_as_string_fname,
                       this,
                       value,
                       purpose,
                       pretty_p,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_value_as_string" that defaults kb-local-only-p to nil
  public void encache_value_as_string
     (Values2 _values_,
      Node value,
      Node purpose,
      Node pretty_p) {
     this.encache_value_as_string(_values_,
                                  value,
                                  purpose,
                                  pretty_p,
                                  _NIL);
  }


// ---------------------- get-behavior-supported-values

/**
 * Returns a list of the supported values of the   behavior 
 * the KB is capable of supporting.  For example, the KB might support both
 * the   :immediate  and   :never  variants of the behavior
 *   :constraint-checking-time .  These two options would be returned
 * as a list.  The returned value   behavior-values  is always a list,
 * even when no variants are supported -- that is, it is  .
 */
  public Node get_behavior_supported_values
     (Node behavior) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_behavior_supported_values_fname,
                       this,
                       behavior);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_behavior_supported_values_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_behavior_supported_values_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_behavior_supported_values(behavior);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_behavior_supported_values(behavior);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_behavior_supported_values_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_behavior_supported_values"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_behavior_supported_values_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_behavior_supported_values(behavior);
       }
    }


  public Node cached_p_get_behavior_supported_values
     (Node behavior) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_behavior_supported_values_fname,
                       this,
                       behavior);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  public void encache_get_behavior_supported_values
     (Node _values_,
      Node behavior) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_behavior_supported_values_fname,
                       this,
                       behavior);
         encache(_values_, key);
       }
  }

// ---------------------- get-behavior-values

/**
 * Returns a list of active values of the   behavior  under
 * which the KB is currently operating.  For example, the KB might support
 * both the   :immediate  and   :never  variants of the behavior
 *   :constraint-checking-time , but only one of these modes can be
 * enabled at a given time.  A list containing, for example, just
 *   :never  would be returned.
 * The returned value   behavior-values  is always a list, even when
 * no variants are active -- that is, it is the  .
 */
  public Node get_behavior_values
     (Node behavior) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_behavior_values_fname,
                       this,
                       behavior);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_behavior_values_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_behavior_values_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_behavior_values(behavior);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_behavior_values(behavior);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_behavior_values_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_behavior_values"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_behavior_values_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_behavior_values(behavior);
       }
    }


  public Node cached_p_get_behavior_values
     (Node behavior) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_behavior_values_fname,
                       this,
                       behavior);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  public void encache_get_behavior_values
     (Node _values_,
      Node behavior) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_behavior_values_fname,
                       this,
                       behavior);
         encache(_values_, key);
       }
  }

// ---------------------- get-class-instances

/**
 * Returns a   list-of-instances  for   class .
 */
  public Values3 get_class_instances
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_instances_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_class_instances_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_class_instances_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_class_instances(the_class,
                                                     inference_level,
                                                     number_of_values,
                                                     kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_class_instances(the_class,
                                                         inference_level,
                                                         number_of_values,
                                                         kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_class_instances_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_class_instances"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_class_instances_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_class_instances(the_class,
                                          inference_level,
                                          number_of_values,
                                          kb_local_only_p);
       }
    }


  public Node cached_p_get_class_instances
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_instances_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_class_instances" that defaults kb-local-only-p to nil
  public Node cached_p_get_class_instances
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.cached_p_get_class_instances(the_class,
                                              inference_level,
                                              number_of_values,
                                              _NIL);
  }


  // Method for "cached_p_get_class_instances" that defaults number-of-values to all
  public Node cached_p_get_class_instances
     (Node the_class,
      InferenceLevel inference_level) {
     return this.cached_p_get_class_instances(the_class,
                                              inference_level,
                                              _all);
  }


  // Method for "cached_p_get_class_instances" that defaults inference-level to taxonomic
  public Node cached_p_get_class_instances
     (Node the_class) {
     return this.cached_p_get_class_instances(the_class,
                                              _taxonomic);
  }


  public void encache_get_class_instances
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_instances_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_class_instances" that defaults kb-local-only-p to nil
  public void encache_get_class_instances
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     this.encache_get_class_instances(_values_,
                                      the_class,
                                      inference_level,
                                      number_of_values,
                                      _NIL);
  }


  // Method for "encache_get_class_instances" that defaults number-of-values to all
  public void encache_get_class_instances
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level) {
     this.encache_get_class_instances(_values_,
                                      the_class,
                                      inference_level,
                                      _all);
  }


  // Method for "encache_get_class_instances" that defaults inference-level to taxonomic
  public void encache_get_class_instances
     (Values3 _values_,
      Node the_class) {
     this.encache_get_class_instances(_values_,
                                      the_class,
                                      _taxonomic);
  }


// ---------------------- get-class-subclasses

/**
 * Returns the   list-of-subclasses  of   class .
 */
  public Values3 get_class_subclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_subclasses_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_class_subclasses_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_class_subclasses_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_class_subclasses(the_class,
                                                      inference_level,
                                                      number_of_values,
                                                      kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_class_subclasses(the_class,
                                                          inference_level,
                                                          number_of_values,
                                                          kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_class_subclasses_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_class_subclasses"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_class_subclasses_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_class_subclasses(the_class,
                                           inference_level,
                                           number_of_values,
                                           kb_local_only_p);
       }
    }


  public Node cached_p_get_class_subclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_subclasses_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_class_subclasses" that defaults kb-local-only-p to nil
  public Node cached_p_get_class_subclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.cached_p_get_class_subclasses(the_class,
                                               inference_level,
                                               number_of_values,
                                               _NIL);
  }


  // Method for "cached_p_get_class_subclasses" that defaults number-of-values to all
  public Node cached_p_get_class_subclasses
     (Node the_class,
      InferenceLevel inference_level) {
     return this.cached_p_get_class_subclasses(the_class,
                                               inference_level,
                                               _all);
  }


  // Method for "cached_p_get_class_subclasses" that defaults inference-level to taxonomic
  public Node cached_p_get_class_subclasses
     (Node the_class) {
     return this.cached_p_get_class_subclasses(the_class,
                                               _taxonomic);
  }


  public void encache_get_class_subclasses
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_subclasses_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_class_subclasses" that defaults kb-local-only-p to nil
  public void encache_get_class_subclasses
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     this.encache_get_class_subclasses(_values_,
                                       the_class,
                                       inference_level,
                                       number_of_values,
                                       _NIL);
  }


  // Method for "encache_get_class_subclasses" that defaults number-of-values to all
  public void encache_get_class_subclasses
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level) {
     this.encache_get_class_subclasses(_values_,
                                       the_class,
                                       inference_level,
                                       _all);
  }


  // Method for "encache_get_class_subclasses" that defaults inference-level to taxonomic
  public void encache_get_class_subclasses
     (Values3 _values_,
      Node the_class) {
     this.encache_get_class_subclasses(_values_,
                                       the_class,
                                       _taxonomic);
  }


// ---------------------- get-class-superclasses

/**
 * Returns the   list-of-superclasses  of   class .
 */
  public Values3 get_class_superclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_superclasses_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_class_superclasses_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_class_superclasses_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_class_superclasses(the_class,
                                                        inference_level,
                                                        number_of_values,
                                                        kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_class_superclasses(the_class,
                                                            inference_level,
                                                            number_of_values,
                                                            kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_class_superclasses_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_class_superclasses"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_class_superclasses_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_class_superclasses(the_class,
                                             inference_level,
                                             number_of_values,
                                             kb_local_only_p);
       }
    }


  public Node cached_p_get_class_superclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_superclasses_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_class_superclasses" that defaults kb-local-only-p to nil
  public Node cached_p_get_class_superclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.cached_p_get_class_superclasses(the_class,
                                                 inference_level,
                                                 number_of_values,
                                                 _NIL);
  }


  // Method for "cached_p_get_class_superclasses" that defaults number-of-values to all
  public Node cached_p_get_class_superclasses
     (Node the_class,
      InferenceLevel inference_level) {
     return this.cached_p_get_class_superclasses(the_class,
                                                 inference_level,
                                                 _all);
  }


  // Method for "cached_p_get_class_superclasses" that defaults inference-level to taxonomic
  public Node cached_p_get_class_superclasses
     (Node the_class) {
     return this.cached_p_get_class_superclasses(the_class,
                                                 _taxonomic);
  }


  public void encache_get_class_superclasses
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_class_superclasses_fname,
                       this,
                       the_class,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_class_superclasses" that defaults kb-local-only-p to nil
  public void encache_get_class_superclasses
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     this.encache_get_class_superclasses(_values_,
                                         the_class,
                                         inference_level,
                                         number_of_values,
                                         _NIL);
  }


  // Method for "encache_get_class_superclasses" that defaults number-of-values to all
  public void encache_get_class_superclasses
     (Values3 _values_,
      Node the_class,
      InferenceLevel inference_level) {
     this.encache_get_class_superclasses(_values_,
                                         the_class,
                                         inference_level,
                                         _all);
  }


  // Method for "encache_get_class_superclasses" that defaults inference-level to taxonomic
  public void encache_get_class_superclasses
     (Values3 _values_,
      Node the_class) {
     this.encache_get_class_superclasses(_values_,
                                         the_class,
                                         _taxonomic);
  }


// ---------------------- get-classes-in-domain-of

/**
 * Returns a list of   classes  that are known to be in the domain of
 *   slot  with respect to   frame-attachment  (if supplied).  If
 *   frame-attachment  is supplied, it may be used as a hint to the KRS
 * to limit the amount of computation performed by constraining the search
 * for classes to the superclasses or types of   frame-attachment .
 * Each class returned (and any subclass) is guaranteed to be a legal
 *   frame  argument for a slot operation on   slot  with
 *   slot-type     :template , e.g.,   put-slot-values .
 */
  public Node get_classes_in_domain_of
     (Node slot,
      Node frame_attachment,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_classes_in_domain_of_fname,
                       this,
                       slot,
                       frame_attachment,
                       inference_level,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_classes_in_domain_of_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_classes_in_domain_of_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_classes_in_domain_of(slot,
                                                          frame_attachment,
                                                          inference_level,
                                                          kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_classes_in_domain_of(slot,
                                                              frame_attachment,
                                                              inference_level,
                                                              kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_classes_in_domain_of_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_classes_in_domain_of"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_classes_in_domain_of_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_classes_in_domain_of(slot,
                                               frame_attachment,
                                               inference_level,
                                               kb_local_only_p);
       }
    }


  public Node cached_p_get_classes_in_domain_of
     (Node slot,
      Node frame_attachment,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_classes_in_domain_of_fname,
                       this,
                       slot,
                       frame_attachment,
                       inference_level,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_classes_in_domain_of" that defaults kb-local-only-p to nil
  public Node cached_p_get_classes_in_domain_of
     (Node slot,
      Node frame_attachment,
      InferenceLevel inference_level) {
     return this.cached_p_get_classes_in_domain_of(slot,
                                                   frame_attachment,
                                                   inference_level,
                                                   _NIL);
  }


  // Method for "cached_p_get_classes_in_domain_of" that defaults inference-level to taxonomic
  public Node cached_p_get_classes_in_domain_of
     (Node slot,
      Node frame_attachment) {
     return this.cached_p_get_classes_in_domain_of(slot,
                                                   frame_attachment,
                                                   _taxonomic);
  }


  // Method for "cached_p_get_classes_in_domain_of" that defaults frame-attachment to nil
  public Node cached_p_get_classes_in_domain_of
     (Node slot) {
     return this.cached_p_get_classes_in_domain_of(slot,
                                                   _NIL);
  }


  public void encache_get_classes_in_domain_of
     (Node _values_,
      Node slot,
      Node frame_attachment,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_classes_in_domain_of_fname,
                       this,
                       slot,
                       frame_attachment,
                       inference_level,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_classes_in_domain_of" that defaults kb-local-only-p to nil
  public void encache_get_classes_in_domain_of
     (Node _values_,
      Node slot,
      Node frame_attachment,
      InferenceLevel inference_level) {
     this.encache_get_classes_in_domain_of(_values_,
                                           slot,
                                           frame_attachment,
                                           inference_level,
                                           _NIL);
  }


  // Method for "encache_get_classes_in_domain_of" that defaults inference-level to taxonomic
  public void encache_get_classes_in_domain_of
     (Node _values_,
      Node slot,
      Node frame_attachment) {
     this.encache_get_classes_in_domain_of(_values_,
                                           slot,
                                           frame_attachment,
                                           _taxonomic);
  }


  // Method for "encache_get_classes_in_domain_of" that defaults frame-attachment to nil
  public void encache_get_classes_in_domain_of
     (Node _values_,
      Node slot) {
     this.encache_get_classes_in_domain_of(_values_,
                                           slot,
                                           _NIL);
  }


// ---------------------- get-facet-value

/**
 * Returns the sole member of the set of values
 * of the specified facet.  It is most commonly used when that set is
 * expected to have only one member.  When the facet has no value,
 *   value-or-false  is  .  It is an error to
 * call this operation on a non-single-valued facet; if it is called, a
 *   cardinality-violation  error should be signaled.
 */
  public Values2 get_facet_value
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_value_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_facet_value_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_facet_value_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_facet_value(frame,
                                                 slot,
                                                 facet,
                                                 inference_level,
                                                 slot_type,
                                                 value_selector,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_facet_value(frame,
                                                     slot,
                                                     facet,
                                                     inference_level,
                                                     slot_type,
                                                     value_selector,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_facet_value_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_facet_value"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_facet_value_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_facet_value(frame,
                                      slot,
                                      facet,
                                      inference_level,
                                      slot_type,
                                      value_selector,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_get_facet_value
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_value_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_facet_value" that defaults kb-local-only-p to nil
  public Node cached_p_get_facet_value
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.cached_p_get_facet_value(frame,
                                          slot,
                                          facet,
                                          inference_level,
                                          slot_type,
                                          value_selector,
                                          _NIL);
  }


  // Method for "cached_p_get_facet_value" that defaults value-selector to either
  public Node cached_p_get_facet_value
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_facet_value(frame,
                                          slot,
                                          facet,
                                          inference_level,
                                          slot_type,
                                          _either);
  }


  // Method for "cached_p_get_facet_value" that defaults slot-type to own
  public Node cached_p_get_facet_value
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.cached_p_get_facet_value(frame,
                                          slot,
                                          facet,
                                          inference_level,
                                          _own);
  }


  // Method for "cached_p_get_facet_value" that defaults inference-level to taxonomic
  public Node cached_p_get_facet_value
     (Node frame,
      Node slot,
      Node facet) {
     return this.cached_p_get_facet_value(frame,
                                          slot,
                                          facet,
                                          _taxonomic);
  }


  public void encache_get_facet_value
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_value_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_facet_value" that defaults kb-local-only-p to nil
  public void encache_get_facet_value
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.encache_get_facet_value(_values_,
                                  frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  slot_type,
                                  value_selector,
                                  _NIL);
  }


  // Method for "encache_get_facet_value" that defaults value-selector to either
  public void encache_get_facet_value
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_facet_value(_values_,
                                  frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  slot_type,
                                  _either);
  }


  // Method for "encache_get_facet_value" that defaults slot-type to own
  public void encache_get_facet_value
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     this.encache_get_facet_value(_values_,
                                  frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  _own);
  }


  // Method for "encache_get_facet_value" that defaults inference-level to taxonomic
  public void encache_get_facet_value
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet) {
     this.encache_get_facet_value(_values_,
                                  frame,
                                  slot,
                                  facet,
                                  _taxonomic);
  }


// ---------------------- get-facet-values

/**
 * Returns the set of values of the specified facet,
 * in no guaranteed order.  It always returns a (possibly empty) list
 * of values.
 */
  public Values3 get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_values_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_facet_values_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_facet_values_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_facet_values(frame,
                                                  slot,
                                                  facet,
                                                  inference_level,
                                                  slot_type,
                                                  number_of_values,
                                                  value_selector,
                                                  kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_facet_values(frame,
                                                      slot,
                                                      facet,
                                                      inference_level,
                                                      slot_type,
                                                      number_of_values,
                                                      value_selector,
                                                      kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_facet_values_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_facet_values"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_facet_values_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_facet_values(frame,
                                       slot,
                                       facet,
                                       inference_level,
                                       slot_type,
                                       number_of_values,
                                       value_selector,
                                       kb_local_only_p);
       }
    }


  public Node cached_p_get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_values_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_facet_values" that defaults kb-local-only-p to nil
  public Node cached_p_get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.cached_p_get_facet_values(frame,
                                           slot,
                                           facet,
                                           inference_level,
                                           slot_type,
                                           number_of_values,
                                           value_selector,
                                           _NIL);
  }


  // Method for "cached_p_get_facet_values" that defaults value-selector to either
  public Node cached_p_get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.cached_p_get_facet_values(frame,
                                           slot,
                                           facet,
                                           inference_level,
                                           slot_type,
                                           number_of_values,
                                           _either);
  }


  // Method for "cached_p_get_facet_values" that defaults number-of-values to all
  public Node cached_p_get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_facet_values(frame,
                                           slot,
                                           facet,
                                           inference_level,
                                           slot_type,
                                           _all);
  }


  // Method for "cached_p_get_facet_values" that defaults slot-type to own
  public Node cached_p_get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.cached_p_get_facet_values(frame,
                                           slot,
                                           facet,
                                           inference_level,
                                           _own);
  }


  // Method for "cached_p_get_facet_values" that defaults inference-level to taxonomic
  public Node cached_p_get_facet_values
     (Node frame,
      Node slot,
      Node facet) {
     return this.cached_p_get_facet_values(frame,
                                           slot,
                                           facet,
                                           _taxonomic);
  }


  public void encache_get_facet_values
     (Values3 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_values_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_facet_values" that defaults kb-local-only-p to nil
  public void encache_get_facet_values
     (Values3 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     this.encache_get_facet_values(_values_,
                                   frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   slot_type,
                                   number_of_values,
                                   value_selector,
                                   _NIL);
  }


  // Method for "encache_get_facet_values" that defaults value-selector to either
  public void encache_get_facet_values
     (Values3 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     this.encache_get_facet_values(_values_,
                                   frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   slot_type,
                                   number_of_values,
                                   _either);
  }


  // Method for "encache_get_facet_values" that defaults number-of-values to all
  public void encache_get_facet_values
     (Values3 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_facet_values(_values_,
                                   frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   slot_type,
                                   _all);
  }


  // Method for "encache_get_facet_values" that defaults slot-type to own
  public void encache_get_facet_values
     (Values3 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     this.encache_get_facet_values(_values_,
                                   frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   _own);
  }


  // Method for "encache_get_facet_values" that defaults inference-level to taxonomic
  public void encache_get_facet_values
     (Values3 _values_,
      Node frame,
      Node slot,
      Node facet) {
     this.encache_get_facet_values(_values_,
                                   frame,
                                   slot,
                                   facet,
                                   _taxonomic);
  }


// ---------------------- get-facet-values-in-detail

/**
 * Returns the   list-of-specs  describing the values
 * of the   facet  of   slot  within   frame , in no guaranteed
 * order.   It always returns a list of specifications as values.  If the
 * specified slot has no values,   is returned.
 * 
 * Each spec is a 3-tuple of the form (value direct-p default-p).
 *  
 *   value -- a value of the facet
 *   direct-p -- a flag that is   if the value is known to be
 * directly asserted for the facet in the   frame  and
 *   otherwise
 *   default-p -- a flag that is   if the value is known to be
 * a default value, and   otherwise
 *  
 * The fourth returned value (  default-p ) is true if the
 *   list-of-specs  is  , and the fact that there are no values
 * is itself a default.
 */
  public Values4 get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_values_in_detail_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_facet_values_in_detail_fname, key, trie_result);
             return ((Values4)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_facet_values_in_detail_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_facet_values_in_detail(frame,
                                                            slot,
                                                            facet,
                                                            inference_level,
                                                            slot_type,
                                                            number_of_values,
                                                            value_selector,
                                                            kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_facet_values_in_detail(frame,
                                                                slot,
                                                                facet,
                                                                inference_level,
                                                                slot_type,
                                                                number_of_values,
                                                                value_selector,
                                                                kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_facet_values_in_detail_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_facet_values_in_detail"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values4)new_result;
                  }
                else
                  { Values4 new_result = 
                        Values.toValues4(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_facet_values_in_detail_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_facet_values_in_detail(frame,
                                                 slot,
                                                 facet,
                                                 inference_level,
                                                 slot_type,
                                                 number_of_values,
                                                 value_selector,
                                                 kb_local_only_p);
       }
    }


  public Node cached_p_get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_values_in_detail_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_facet_values_in_detail" that defaults kb-local-only-p to nil
  public Node cached_p_get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.cached_p_get_facet_values_in_detail(frame,
                                                     slot,
                                                     facet,
                                                     inference_level,
                                                     slot_type,
                                                     number_of_values,
                                                     value_selector,
                                                     _NIL);
  }


  // Method for "cached_p_get_facet_values_in_detail" that defaults value-selector to either
  public Node cached_p_get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.cached_p_get_facet_values_in_detail(frame,
                                                     slot,
                                                     facet,
                                                     inference_level,
                                                     slot_type,
                                                     number_of_values,
                                                     _either);
  }


  // Method for "cached_p_get_facet_values_in_detail" that defaults number-of-values to all
  public Node cached_p_get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_facet_values_in_detail(frame,
                                                     slot,
                                                     facet,
                                                     inference_level,
                                                     slot_type,
                                                     _all);
  }


  // Method for "cached_p_get_facet_values_in_detail" that defaults slot-type to own
  public Node cached_p_get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.cached_p_get_facet_values_in_detail(frame,
                                                     slot,
                                                     facet,
                                                     inference_level,
                                                     _own);
  }


  // Method for "cached_p_get_facet_values_in_detail" that defaults inference-level to taxonomic
  public Node cached_p_get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet) {
     return this.cached_p_get_facet_values_in_detail(frame,
                                                     slot,
                                                     facet,
                                                     _taxonomic);
  }


  public void encache_get_facet_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_facet_values_in_detail_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_facet_values_in_detail" that defaults kb-local-only-p to nil
  public void encache_get_facet_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     this.encache_get_facet_values_in_detail(_values_,
                                             frame,
                                             slot,
                                             facet,
                                             inference_level,
                                             slot_type,
                                             number_of_values,
                                             value_selector,
                                             _NIL);
  }


  // Method for "encache_get_facet_values_in_detail" that defaults value-selector to either
  public void encache_get_facet_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     this.encache_get_facet_values_in_detail(_values_,
                                             frame,
                                             slot,
                                             facet,
                                             inference_level,
                                             slot_type,
                                             number_of_values,
                                             _either);
  }


  // Method for "encache_get_facet_values_in_detail" that defaults number-of-values to all
  public void encache_get_facet_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_facet_values_in_detail(_values_,
                                             frame,
                                             slot,
                                             facet,
                                             inference_level,
                                             slot_type,
                                             _all);
  }


  // Method for "encache_get_facet_values_in_detail" that defaults slot-type to own
  public void encache_get_facet_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     this.encache_get_facet_values_in_detail(_values_,
                                             frame,
                                             slot,
                                             facet,
                                             inference_level,
                                             _own);
  }


  // Method for "encache_get_facet_values_in_detail" that defaults inference-level to taxonomic
  public void encache_get_facet_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      Node facet) {
     this.encache_get_facet_values_in_detail(_values_,
                                             frame,
                                             slot,
                                             facet,
                                             _taxonomic);
  }


// ---------------------- get-frame-details

/**
 * Returns a property list (list of alternating keywords and values)
 * describing the   frame .  The properties of the frame are computed
 * using the   inference-level ,   number-of-values-p , and
 *   kb-local-only-p  arguments, whenever applicable to the appropriate
 * OKBC operator used to compute any given property.  The set of properties
 * computed is at least the following:
 * 
 *    ll 
 *    Property          &    Operation(s) used to compute property   
 *    :handle           &   get-frame-handle        
 *    :name             &   get-frame-name          
 *    :pretty-name      &   get-frame-pretty-name   
 *    :frame-type       &   get-frame-type          
 *    :primitive-p      &   primitive-p             
 *    :superclasses     &   get-class-superclasses        
 *    :subclasses       &   get-class-subclasses          
 *    :types            &   get-instance-types               
 *    :own-slots        &   get-frame-slots ,   get-slot-values  
 *    :template-slots   &   get-frame-slots ,   get-slot-values  
 *    :own-facets       &   get-frame-slots ,   get-slot-values ,   get-slot-facets ,   get-facet-values   
 *    :template-facets  &   get-frame-slots ,   get-slot-values ,   get-slot-facets ,   get-facet-values   
 *    :sentences        &   get-frame-sentences     
 *  
 * 
 * The    :own-slots ,    :own-facets ,    :template-slots , and
 *    :template-facets  properties returned are slot and facet
 * specifications as defined for   create-frame .  This operation is most
 * useful in low-bandwidth or high-latency applications.  A single call to
 *   get-frame-details  is often sufficient to display all the
 * interesting features of a frame.  The value of the    :sentences 
 * component is a set of non-OKBC sentences, i.e. what would be returned by
 *   get-frame-sentences  with the    :okbc-sentences-p  argument set
 * to
 */
  public Values2 get_frame_details
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_details_fname,
                       this,
                       frame,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frame_details_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frame_details_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frame_details(frame,
                                                   inference_level,
                                                   number_of_values,
                                                   kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frame_details(frame,
                                                       inference_level,
                                                       number_of_values,
                                                       kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frame_details_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_frame_details"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frame_details_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frame_details(frame,
                                        inference_level,
                                        number_of_values,
                                        kb_local_only_p);
       }
    }


  public Node cached_p_get_frame_details
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_details_fname,
                       this,
                       frame,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frame_details" that defaults kb-local-only-p to nil
  public Node cached_p_get_frame_details
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.cached_p_get_frame_details(frame,
                                            inference_level,
                                            number_of_values,
                                            _NIL);
  }


  // Method for "cached_p_get_frame_details" that defaults number-of-values to all
  public Node cached_p_get_frame_details
     (Node frame,
      InferenceLevel inference_level) {
     return this.cached_p_get_frame_details(frame,
                                            inference_level,
                                            _all);
  }


  // Method for "cached_p_get_frame_details" that defaults inference-level to taxonomic
  public Node cached_p_get_frame_details
     (Node frame) {
     return this.cached_p_get_frame_details(frame,
                                            _taxonomic);
  }


  public void encache_get_frame_details
     (Values2 _values_,
      Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_details_fname,
                       this,
                       frame,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frame_details" that defaults kb-local-only-p to nil
  public void encache_get_frame_details
     (Values2 _values_,
      Node frame,
      InferenceLevel inference_level,
      Node number_of_values) {
     this.encache_get_frame_details(_values_,
                                    frame,
                                    inference_level,
                                    number_of_values,
                                    _NIL);
  }


  // Method for "encache_get_frame_details" that defaults number-of-values to all
  public void encache_get_frame_details
     (Values2 _values_,
      Node frame,
      InferenceLevel inference_level) {
     this.encache_get_frame_details(_values_,
                                    frame,
                                    inference_level,
                                    _all);
  }


  // Method for "encache_get_frame_details" that defaults inference-level to taxonomic
  public void encache_get_frame_details
     (Values2 _values_,
      Node frame) {
     this.encache_get_frame_details(_values_,
                                    frame,
                                    _taxonomic);
  }


// ---------------------- get-frame-handle

/**
 * Returns a   frame-handle  that uniquely identifies the   frame 
 * argument in   kb .
 */
  public Node get_frame_handle
     (Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_handle_fname,
                       this,
                       frame,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frame_handle_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frame_handle_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frame_handle(frame,
                                                  kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frame_handle(frame,
                                                      kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frame_handle_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_frame_handle"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frame_handle_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frame_handle(frame,
                                       kb_local_only_p);
       }
    }


  public Node cached_p_get_frame_handle
     (Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_handle_fname,
                       this,
                       frame,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frame_handle" that defaults kb-local-only-p to nil
  public Node cached_p_get_frame_handle
     (Node frame) {
     return this.cached_p_get_frame_handle(frame,
                                           _NIL);
  }


  public void encache_get_frame_handle
     (Node _values_,
      Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_handle_fname,
                       this,
                       frame,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frame_handle" that defaults kb-local-only-p to nil
  public void encache_get_frame_handle
     (Node _values_,
      Node frame) {
     this.encache_get_frame_handle(_values_,
                                   frame,
                                   _NIL);
  }


// ---------------------- get-frame-in-kb

/**
 * Returns two values.  The first value is the   frame 
 * identified by   thing  if such a frame is found, or  .
 * The second value (  frame-found-p ) is   iff   thing  is
 * coercible to a frame, and that frame is resident in   KB .  In
 * all cases it is verified that the frame does, in fact, reside in   kb .
 * Otherwise, the   frame-found-p  value is    nil  (unless
 *   error-p  is  , in which case the operation signals a
 *   not-coercible-to-frame  error because
 *   thing  is not a valid frame in   kb ).
 */
  public Values2 get_frame_in_kb
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_in_kb_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frame_in_kb_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frame_in_kb_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frame_in_kb(thing,
                                                 error_p,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frame_in_kb(thing,
                                                     error_p,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frame_in_kb_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_frame_in_kb"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frame_in_kb_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frame_in_kb(thing,
                                      error_p,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_get_frame_in_kb
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_in_kb_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frame_in_kb" that defaults kb-local-only-p to nil
  public Node cached_p_get_frame_in_kb
     (Node thing,
      TruthValue error_p) {
     return this.cached_p_get_frame_in_kb(thing,
                                          error_p,
                                          _NIL);
  }


  // Method for "cached_p_get_frame_in_kb" that defaults error-p to t
  public Node cached_p_get_frame_in_kb
     (Node thing) {
     return this.cached_p_get_frame_in_kb(thing,
                                          _T);
  }


  public void encache_get_frame_in_kb
     (Values2 _values_,
      Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_in_kb_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frame_in_kb" that defaults kb-local-only-p to nil
  public void encache_get_frame_in_kb
     (Values2 _values_,
      Node thing,
      TruthValue error_p) {
     this.encache_get_frame_in_kb(_values_,
                                  thing,
                                  error_p,
                                  _NIL);
  }


  // Method for "encache_get_frame_in_kb" that defaults error-p to t
  public void encache_get_frame_in_kb
     (Values2 _values_,
      Node thing) {
     this.encache_get_frame_in_kb(_values_,
                                  thing,
                                  _T);
  }


// ---------------------- get-frame-name

/**
 * Returns   frame-name , an entity that is the name of the frame
 * identified by   frame , usually a symbol or string.
 */
  public Node get_frame_name
     (Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_name_fname,
                       this,
                       frame,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frame_name_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frame_name_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frame_name(frame,
                                                kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frame_name(frame,
                                                    kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frame_name_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_frame_name"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frame_name_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frame_name(frame,
                                     kb_local_only_p);
       }
    }


  public Node cached_p_get_frame_name
     (Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_name_fname,
                       this,
                       frame,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frame_name" that defaults kb-local-only-p to nil
  public Node cached_p_get_frame_name
     (Node frame) {
     return this.cached_p_get_frame_name(frame,
                                         _NIL);
  }


  public void encache_get_frame_name
     (Node _values_,
      Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_name_fname,
                       this,
                       frame,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frame_name" that defaults kb-local-only-p to nil
  public void encache_get_frame_name
     (Node _values_,
      Node frame) {
     this.encache_get_frame_name(_values_,
                                 frame,
                                 _NIL);
  }


// ---------------------- get-frame-pretty-name

/**
 * Returns a string that is a pretty, printed representation for   frame 
 * -- that is, the name is suitable for use within a user interface for
 * display purposes.
 * 
 * There is no guarantee that it will be possible to find a unique frame
 * given only its pretty-name, but   get-frames-matching  can be used to
 * find frames matching such strings when possible.
 */
  public Node get_frame_pretty_name
     (Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_pretty_name_fname,
                       this,
                       frame,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frame_pretty_name_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frame_pretty_name_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frame_pretty_name(frame,
                                                       kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frame_pretty_name(frame,
                                                           kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frame_pretty_name_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_frame_pretty_name"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frame_pretty_name_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frame_pretty_name(frame,
                                            kb_local_only_p);
       }
    }


  public Node cached_p_get_frame_pretty_name
     (Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_pretty_name_fname,
                       this,
                       frame,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frame_pretty_name" that defaults kb-local-only-p to nil
  public Node cached_p_get_frame_pretty_name
     (Node frame) {
     return this.cached_p_get_frame_pretty_name(frame,
                                                _NIL);
  }


  public void encache_get_frame_pretty_name
     (Node _values_,
      Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_pretty_name_fname,
                       this,
                       frame,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frame_pretty_name" that defaults kb-local-only-p to nil
  public void encache_get_frame_pretty_name
     (Node _values_,
      Node frame) {
     this.encache_get_frame_pretty_name(_values_,
                                        frame,
                                        _NIL);
  }


// ---------------------- get-frame-sentences

/**
 * Returns a list of all the logical sentences associated with a   frame .
 * The sentences may have been asserted using   tell , or any other
 * OKBC update operation.  If   okbc-sentences-p  is  , then all
 * sentences are returned, including the ones that are equivalent to
 * basic OKBC operations.  The sentences equivalent to OKBC operations are
 * defined in Table  .  If   okbc-sentences-p  is
 *  , sentences that are equivalent to OKBC operations are not
 * returned.  This is very useful for user interface applications that do not
 * want to present redundant information.  If no matching sentences are
 * found,   list-of-sentences  will be  .
 */
  public Values3 get_frame_sentences
     (Node frame,
      Node number_of_values,
      Node okbc_sentences_p,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_sentences_fname,
                       this,
                       frame,
                       number_of_values,
                       okbc_sentences_p,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frame_sentences_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frame_sentences_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frame_sentences(frame,
                                                     number_of_values,
                                                     okbc_sentences_p,
                                                     value_selector,
                                                     kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frame_sentences(frame,
                                                         number_of_values,
                                                         okbc_sentences_p,
                                                         value_selector,
                                                         kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frame_sentences_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_frame_sentences"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frame_sentences_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frame_sentences(frame,
                                          number_of_values,
                                          okbc_sentences_p,
                                          value_selector,
                                          kb_local_only_p);
       }
    }


  public Node cached_p_get_frame_sentences
     (Node frame,
      Node number_of_values,
      Node okbc_sentences_p,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_sentences_fname,
                       this,
                       frame,
                       number_of_values,
                       okbc_sentences_p,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frame_sentences" that defaults kb-local-only-p to nil
  public Node cached_p_get_frame_sentences
     (Node frame,
      Node number_of_values,
      Node okbc_sentences_p,
      ValueSelector value_selector) {
     return this.cached_p_get_frame_sentences(frame,
                                              number_of_values,
                                              okbc_sentences_p,
                                              value_selector,
                                              _NIL);
  }


  // Method for "cached_p_get_frame_sentences" that defaults value-selector to either
  public Node cached_p_get_frame_sentences
     (Node frame,
      Node number_of_values,
      Node okbc_sentences_p) {
     return this.cached_p_get_frame_sentences(frame,
                                              number_of_values,
                                              okbc_sentences_p,
                                              _either);
  }


  // Method for "cached_p_get_frame_sentences" that defaults okbc-sentences-p to t
  public Node cached_p_get_frame_sentences
     (Node frame,
      Node number_of_values) {
     return this.cached_p_get_frame_sentences(frame,
                                              number_of_values,
                                              _T);
  }


  // Method for "cached_p_get_frame_sentences" that defaults number-of-values to all
  public Node cached_p_get_frame_sentences
     (Node frame) {
     return this.cached_p_get_frame_sentences(frame,
                                              _all);
  }


  public void encache_get_frame_sentences
     (Values3 _values_,
      Node frame,
      Node number_of_values,
      Node okbc_sentences_p,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_sentences_fname,
                       this,
                       frame,
                       number_of_values,
                       okbc_sentences_p,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frame_sentences" that defaults kb-local-only-p to nil
  public void encache_get_frame_sentences
     (Values3 _values_,
      Node frame,
      Node number_of_values,
      Node okbc_sentences_p,
      ValueSelector value_selector) {
     this.encache_get_frame_sentences(_values_,
                                      frame,
                                      number_of_values,
                                      okbc_sentences_p,
                                      value_selector,
                                      _NIL);
  }


  // Method for "encache_get_frame_sentences" that defaults value-selector to either
  public void encache_get_frame_sentences
     (Values3 _values_,
      Node frame,
      Node number_of_values,
      Node okbc_sentences_p) {
     this.encache_get_frame_sentences(_values_,
                                      frame,
                                      number_of_values,
                                      okbc_sentences_p,
                                      _either);
  }


  // Method for "encache_get_frame_sentences" that defaults okbc-sentences-p to t
  public void encache_get_frame_sentences
     (Values3 _values_,
      Node frame,
      Node number_of_values) {
     this.encache_get_frame_sentences(_values_,
                                      frame,
                                      number_of_values,
                                      _T);
  }


  // Method for "encache_get_frame_sentences" that defaults number-of-values to all
  public void encache_get_frame_sentences
     (Values3 _values_,
      Node frame) {
     this.encache_get_frame_sentences(_values_,
                                      frame,
                                      _all);
  }


// ---------------------- get-frame-slots

/**
 * Returns   list-of-slots , a list of all the own, template, or own
 * and template slots that are associated with   frame , depending on the
 * value of   slot-type .
 */
  public Values2 get_frame_slots
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_slots_fname,
                       this,
                       frame,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frame_slots_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frame_slots_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frame_slots(frame,
                                                 inference_level,
                                                 slot_type,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frame_slots(frame,
                                                     inference_level,
                                                     slot_type,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frame_slots_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_frame_slots"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frame_slots_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frame_slots(frame,
                                      inference_level,
                                      slot_type,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_get_frame_slots
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_slots_fname,
                       this,
                       frame,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frame_slots" that defaults kb-local-only-p to nil
  public Node cached_p_get_frame_slots
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_frame_slots(frame,
                                          inference_level,
                                          slot_type,
                                          _NIL);
  }


  // Method for "cached_p_get_frame_slots" that defaults slot-type to all
  public Node cached_p_get_frame_slots
     (Node frame,
      InferenceLevel inference_level) {
     return this.cached_p_get_frame_slots(frame,
                                          inference_level,
                                          _all);
  }


  // Method for "cached_p_get_frame_slots" that defaults inference-level to taxonomic
  public Node cached_p_get_frame_slots
     (Node frame) {
     return this.cached_p_get_frame_slots(frame,
                                          _taxonomic);
  }


  public void encache_get_frame_slots
     (Values2 _values_,
      Node frame,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_slots_fname,
                       this,
                       frame,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frame_slots" that defaults kb-local-only-p to nil
  public void encache_get_frame_slots
     (Values2 _values_,
      Node frame,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_frame_slots(_values_,
                                  frame,
                                  inference_level,
                                  slot_type,
                                  _NIL);
  }


  // Method for "encache_get_frame_slots" that defaults slot-type to all
  public void encache_get_frame_slots
     (Values2 _values_,
      Node frame,
      InferenceLevel inference_level) {
     this.encache_get_frame_slots(_values_,
                                  frame,
                                  inference_level,
                                  _all);
  }


  // Method for "encache_get_frame_slots" that defaults inference-level to taxonomic
  public void encache_get_frame_slots
     (Values2 _values_,
      Node frame) {
     this.encache_get_frame_slots(_values_,
                                  frame,
                                  _taxonomic);
  }


// ---------------------- get-frame-type

/**
 * When   thing  identifies a frame, returns either
 *    :slot ,    :facet ,    :class , or    :individual ,
 * depending on the type of the frame.
 * When   thing  does not identify a frame,   frame-type  is
 *  .     :Slot  and    :facet  will be returned only in
 * those systems that support the values    :slot  and    :facet ,
 * respectively, for the    :are-frames  behavior.
 */
  public Node get_frame_type
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_type_fname,
                       this,
                       thing,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frame_type_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frame_type_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frame_type(thing,
                                                kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frame_type(thing,
                                                    kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frame_type_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_frame_type"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frame_type_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frame_type(thing,
                                     kb_local_only_p);
       }
    }


  public Node cached_p_get_frame_type
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_type_fname,
                       this,
                       thing,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frame_type" that defaults kb-local-only-p to nil
  public Node cached_p_get_frame_type
     (Node thing) {
     return this.cached_p_get_frame_type(thing,
                                         _NIL);
  }


  public void encache_get_frame_type
     (Node _values_,
      Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frame_type_fname,
                       this,
                       thing,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frame_type" that defaults kb-local-only-p to nil
  public void encache_get_frame_type
     (Node _values_,
      Node thing) {
     this.encache_get_frame_type(_values_,
                                 thing,
                                 _NIL);
  }


// ---------------------- get-frames-matching

/**
 * Given a   pattern , which is a string or a symbol,
 * finds a set of matching frames for that pattern.
 * The match of a frame to a pattern could take into account the frame's name
 * (if meaningful), printed representation, pretty-name, or any KB-specific
 * feature such as a list of synonyms.
 * 
 * Returns the following two values:
 *   
 *     matching-frames  -- The list of matching frames (which is
 *   if no matches are found).
 *     longest-matching-substring  -- The longest matching initial
 * substring.  This returned value is useful in applications that use
 * get-frames-matching to implement a completion facility, or prompt
 * users for frames (  if no matches are found).
 *  
 *   Wildcards-allowed-p 
 * --- When  , the pattern may contain * (zero or more characters)
 * and ? (exactly one character) wildcards.  Wildcard characters are
 * escaped with the backslash character.  If this argument is  ,
 * the * and ? characters simply denote themselves and need not be
 * escaped.
 *   Selector 
 * --- May be a procedure (see Section  ) of
 * signature
 * (candidate-name, kb, kb-local-only-p)
 * that returns   if the candidate name is to be
 * accepted and   otherwise, or one of the following keywords:
 *  
 *      :all  -- Select all frames
 *      :class  -- Select only class frames
 *      :individual  -- Select only individual frames
 *      :slot  -- Select only slot frames
 *      :facet  -- Select only facet frames
 *  
 *   Force-Case-Insensitive-P 
 * --- When  , cause the comparison is to be case-insensitive,
 * irrespective of the IO syntax of the KB.
 */
  public Values2 get_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_matching_fname,
                       this,
                       pattern,
                       wildcards_allowed_p,
                       selector,
                       force_case_insensitive_p,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frames_matching_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frames_matching_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frames_matching(pattern,
                                                     wildcards_allowed_p,
                                                     selector,
                                                     force_case_insensitive_p,
                                                     kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frames_matching(pattern,
                                                         wildcards_allowed_p,
                                                         selector,
                                                         force_case_insensitive_p,
                                                         kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frames_matching_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_frames_matching"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frames_matching_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frames_matching(pattern,
                                          wildcards_allowed_p,
                                          selector,
                                          force_case_insensitive_p,
                                          kb_local_only_p);
       }
    }


  public Node cached_p_get_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_matching_fname,
                       this,
                       pattern,
                       wildcards_allowed_p,
                       selector,
                       force_case_insensitive_p,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frames_matching" that defaults kb-local-only-p to nil
  public Node cached_p_get_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p) {
     return this.cached_p_get_frames_matching(pattern,
                                              wildcards_allowed_p,
                                              selector,
                                              force_case_insensitive_p,
                                              _NIL);
  }


  // Method for "cached_p_get_frames_matching" that defaults force-case-insensitive-p to nil
  public Node cached_p_get_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector) {
     return this.cached_p_get_frames_matching(pattern,
                                              wildcards_allowed_p,
                                              selector,
                                              _NIL);
  }


  // Method for "cached_p_get_frames_matching" that defaults selector to all
  public Node cached_p_get_frames_matching
     (Node pattern,
      Node wildcards_allowed_p) {
     return this.cached_p_get_frames_matching(pattern,
                                              wildcards_allowed_p,
                                              _all);
  }


  // Method for "cached_p_get_frames_matching" that defaults wildcards-allowed-p to t
  public Node cached_p_get_frames_matching
     (Node pattern) {
     return this.cached_p_get_frames_matching(pattern,
                                              _T);
  }


  public void encache_get_frames_matching
     (Values2 _values_,
      Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_matching_fname,
                       this,
                       pattern,
                       wildcards_allowed_p,
                       selector,
                       force_case_insensitive_p,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frames_matching" that defaults kb-local-only-p to nil
  public void encache_get_frames_matching
     (Values2 _values_,
      Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p) {
     this.encache_get_frames_matching(_values_,
                                      pattern,
                                      wildcards_allowed_p,
                                      selector,
                                      force_case_insensitive_p,
                                      _NIL);
  }


  // Method for "encache_get_frames_matching" that defaults force-case-insensitive-p to nil
  public void encache_get_frames_matching
     (Values2 _values_,
      Node pattern,
      Node wildcards_allowed_p,
      Node selector) {
     this.encache_get_frames_matching(_values_,
                                      pattern,
                                      wildcards_allowed_p,
                                      selector,
                                      _NIL);
  }


  // Method for "encache_get_frames_matching" that defaults selector to all
  public void encache_get_frames_matching
     (Values2 _values_,
      Node pattern,
      Node wildcards_allowed_p) {
     this.encache_get_frames_matching(_values_,
                                      pattern,
                                      wildcards_allowed_p,
                                      _all);
  }


  // Method for "encache_get_frames_matching" that defaults wildcards-allowed-p to t
  public void encache_get_frames_matching
     (Values2 _values_,
      Node pattern) {
     this.encache_get_frames_matching(_values_,
                                      pattern,
                                      _T);
  }


// ---------------------- get-frames-with-facet-value

/**
 * Returns the set of frames in which the specified facet value is accessible
 * on the specified slot.
 * If the system is unable to find any frame/slot/facet combinations with the
 * specified value,   is returned.
 * This operation allows user interfaces to take users from a value
 * displayed as a facet value on a particular frame/slot to the place
 * that asserted the value.
 */
  public Values2 get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_with_facet_value_fname,
                       this,
                       slot,
                       facet,
                       value,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frames_with_facet_value_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frames_with_facet_value_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frames_with_facet_value(slot,
                                                             facet,
                                                             value,
                                                             inference_level,
                                                             slot_type,
                                                             value_selector,
                                                             kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frames_with_facet_value(slot,
                                                                 facet,
                                                                 value,
                                                                 inference_level,
                                                                 slot_type,
                                                                 value_selector,
                                                                 kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frames_with_facet_value_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_frames_with_facet_value"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frames_with_facet_value_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frames_with_facet_value(slot,
                                                  facet,
                                                  value,
                                                  inference_level,
                                                  slot_type,
                                                  value_selector,
                                                  kb_local_only_p);
       }
    }


  public Node cached_p_get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_with_facet_value_fname,
                       this,
                       slot,
                       facet,
                       value,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frames_with_facet_value" that defaults kb-local-only-p to nil
  public Node cached_p_get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.cached_p_get_frames_with_facet_value(slot,
                                                      facet,
                                                      value,
                                                      inference_level,
                                                      slot_type,
                                                      value_selector,
                                                      _NIL);
  }


  // Method for "cached_p_get_frames_with_facet_value" that defaults value-selector to either
  public Node cached_p_get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_frames_with_facet_value(slot,
                                                      facet,
                                                      value,
                                                      inference_level,
                                                      slot_type,
                                                      _either);
  }


  // Method for "cached_p_get_frames_with_facet_value" that defaults slot-type to own
  public Node cached_p_get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level) {
     return this.cached_p_get_frames_with_facet_value(slot,
                                                      facet,
                                                      value,
                                                      inference_level,
                                                      _own);
  }


  // Method for "cached_p_get_frames_with_facet_value" that defaults inference-level to taxonomic
  public Node cached_p_get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value) {
     return this.cached_p_get_frames_with_facet_value(slot,
                                                      facet,
                                                      value,
                                                      _taxonomic);
  }


  public void encache_get_frames_with_facet_value
     (Values2 _values_,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_with_facet_value_fname,
                       this,
                       slot,
                       facet,
                       value,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frames_with_facet_value" that defaults kb-local-only-p to nil
  public void encache_get_frames_with_facet_value
     (Values2 _values_,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.encache_get_frames_with_facet_value(_values_,
                                              slot,
                                              facet,
                                              value,
                                              inference_level,
                                              slot_type,
                                              value_selector,
                                              _NIL);
  }


  // Method for "encache_get_frames_with_facet_value" that defaults value-selector to either
  public void encache_get_frames_with_facet_value
     (Values2 _values_,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_frames_with_facet_value(_values_,
                                              slot,
                                              facet,
                                              value,
                                              inference_level,
                                              slot_type,
                                              _either);
  }


  // Method for "encache_get_frames_with_facet_value" that defaults slot-type to own
  public void encache_get_frames_with_facet_value
     (Values2 _values_,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level) {
     this.encache_get_frames_with_facet_value(_values_,
                                              slot,
                                              facet,
                                              value,
                                              inference_level,
                                              _own);
  }


  // Method for "encache_get_frames_with_facet_value" that defaults inference-level to taxonomic
  public void encache_get_frames_with_facet_value
     (Values2 _values_,
      Node slot,
      Node facet,
      Node value) {
     this.encache_get_frames_with_facet_value(_values_,
                                              slot,
                                              facet,
                                              value,
                                              _taxonomic);
  }


// ---------------------- get-frames-with-slot-value

/**
 * Returns the set of frames in which the specified slot value is accessible.
 * If the system is unable to find any frame/slot combinations with the
 * specified value,   is returned.
 * This operation allows user interfaces to take users from a value
 * displayed as a slot value on a particular frame to the place that
 * asserted the value.
 */
  public Values2 get_frames_with_slot_value
     (Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_with_slot_value_fname,
                       this,
                       slot,
                       value,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_frames_with_slot_value_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_frames_with_slot_value_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_frames_with_slot_value(slot,
                                                            value,
                                                            inference_level,
                                                            slot_type,
                                                            value_selector,
                                                            kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_frames_with_slot_value(slot,
                                                                value,
                                                                inference_level,
                                                                slot_type,
                                                                value_selector,
                                                                kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_frames_with_slot_value_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_frames_with_slot_value"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_frames_with_slot_value_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_frames_with_slot_value(slot,
                                                 value,
                                                 inference_level,
                                                 slot_type,
                                                 value_selector,
                                                 kb_local_only_p);
       }
    }


  public Node cached_p_get_frames_with_slot_value
     (Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_with_slot_value_fname,
                       this,
                       slot,
                       value,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_frames_with_slot_value" that defaults kb-local-only-p to nil
  public Node cached_p_get_frames_with_slot_value
     (Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.cached_p_get_frames_with_slot_value(slot,
                                                     value,
                                                     inference_level,
                                                     slot_type,
                                                     value_selector,
                                                     _NIL);
  }


  // Method for "cached_p_get_frames_with_slot_value" that defaults value-selector to either
  public Node cached_p_get_frames_with_slot_value
     (Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_frames_with_slot_value(slot,
                                                     value,
                                                     inference_level,
                                                     slot_type,
                                                     _either);
  }


  // Method for "cached_p_get_frames_with_slot_value" that defaults slot-type to own
  public Node cached_p_get_frames_with_slot_value
     (Node slot,
      Node value,
      InferenceLevel inference_level) {
     return this.cached_p_get_frames_with_slot_value(slot,
                                                     value,
                                                     inference_level,
                                                     _own);
  }


  // Method for "cached_p_get_frames_with_slot_value" that defaults inference-level to taxonomic
  public Node cached_p_get_frames_with_slot_value
     (Node slot,
      Node value) {
     return this.cached_p_get_frames_with_slot_value(slot,
                                                     value,
                                                     _taxonomic);
  }


  public void encache_get_frames_with_slot_value
     (Values2 _values_,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_frames_with_slot_value_fname,
                       this,
                       slot,
                       value,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_frames_with_slot_value" that defaults kb-local-only-p to nil
  public void encache_get_frames_with_slot_value
     (Values2 _values_,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.encache_get_frames_with_slot_value(_values_,
                                             slot,
                                             value,
                                             inference_level,
                                             slot_type,
                                             value_selector,
                                             _NIL);
  }


  // Method for "encache_get_frames_with_slot_value" that defaults value-selector to either
  public void encache_get_frames_with_slot_value
     (Values2 _values_,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_frames_with_slot_value(_values_,
                                             slot,
                                             value,
                                             inference_level,
                                             slot_type,
                                             _either);
  }


  // Method for "encache_get_frames_with_slot_value" that defaults slot-type to own
  public void encache_get_frames_with_slot_value
     (Values2 _values_,
      Node slot,
      Node value,
      InferenceLevel inference_level) {
     this.encache_get_frames_with_slot_value(_values_,
                                             slot,
                                             value,
                                             inference_level,
                                             _own);
  }


  // Method for "encache_get_frames_with_slot_value" that defaults inference-level to taxonomic
  public void encache_get_frames_with_slot_value
     (Values2 _values_,
      Node slot,
      Node value) {
     this.encache_get_frames_with_slot_value(_values_,
                                             slot,
                                             value,
                                             _taxonomic);
  }


// ---------------------- get-instance-types

/**
 * Returns the   list-of-types  of   frame , that is, the list of
 * classes of which   frame  is an instance.
 */
  public Values3 get_instance_types
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_instance_types_fname,
                       this,
                       frame,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_instance_types_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_instance_types_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_instance_types(frame,
                                                    inference_level,
                                                    number_of_values,
                                                    kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_instance_types(frame,
                                                        inference_level,
                                                        number_of_values,
                                                        kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_instance_types_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_instance_types"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_instance_types_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_instance_types(frame,
                                         inference_level,
                                         number_of_values,
                                         kb_local_only_p);
       }
    }


  public Node cached_p_get_instance_types
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_instance_types_fname,
                       this,
                       frame,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_instance_types" that defaults kb-local-only-p to nil
  public Node cached_p_get_instance_types
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.cached_p_get_instance_types(frame,
                                             inference_level,
                                             number_of_values,
                                             _NIL);
  }


  // Method for "cached_p_get_instance_types" that defaults number-of-values to all
  public Node cached_p_get_instance_types
     (Node frame,
      InferenceLevel inference_level) {
     return this.cached_p_get_instance_types(frame,
                                             inference_level,
                                             _all);
  }


  // Method for "cached_p_get_instance_types" that defaults inference-level to taxonomic
  public Node cached_p_get_instance_types
     (Node frame) {
     return this.cached_p_get_instance_types(frame,
                                             _taxonomic);
  }


  public void encache_get_instance_types
     (Values3 _values_,
      Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_instance_types_fname,
                       this,
                       frame,
                       inference_level,
                       number_of_values,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_instance_types" that defaults kb-local-only-p to nil
  public void encache_get_instance_types
     (Values3 _values_,
      Node frame,
      InferenceLevel inference_level,
      Node number_of_values) {
     this.encache_get_instance_types(_values_,
                                     frame,
                                     inference_level,
                                     number_of_values,
                                     _NIL);
  }


  // Method for "encache_get_instance_types" that defaults number-of-values to all
  public void encache_get_instance_types
     (Values3 _values_,
      Node frame,
      InferenceLevel inference_level) {
     this.encache_get_instance_types(_values_,
                                     frame,
                                     inference_level,
                                     _all);
  }


  // Method for "encache_get_instance_types" that defaults inference-level to taxonomic
  public void encache_get_instance_types
     (Values3 _values_,
      Node frame) {
     this.encache_get_instance_types(_values_,
                                     frame,
                                     _taxonomic);
  }


// ---------------------- get-kb-behaviors

/**
 * When   kb-type-or-kb  is either a KB or a kb-type, returns
 *   list-of-behaviors , which is a list of keywords naming all
 * the behaviors recognized by this KB, or identified by the kb-type,
 * respectively.
 */
  public Node get_kb_behaviors
     (Connection connection) {
     return super.get_kb_behaviors(connection);
    }


// ---------------------- get-kb-classes

/**
 * Returns   list-of-classes , a list of the classes in the KB.
 *   Selector  can be one of the following:
 *  
 *      :all  -- Returns all classes
 *      :frames  -- Returns classes that are represented as frames
 *      :system-default  -- Returns either all classes or
 * only class frames, according to which is the KRS's default
 */
  public Node get_kb_classes
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_classes_fname,
                       this,
                       selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_kb_classes_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_kb_classes_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_kb_classes(selector,
                                                kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_kb_classes(selector,
                                                    kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_kb_classes_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_kb_classes"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_kb_classes_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_kb_classes(selector,
                                     kb_local_only_p);
       }
    }


  public Node cached_p_get_kb_classes
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_classes_fname,
                       this,
                       selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_kb_classes" that defaults kb-local-only-p to nil
  public Node cached_p_get_kb_classes
     (Node selector) {
     return this.cached_p_get_kb_classes(selector,
                                         _NIL);
  }


  // Method for "cached_p_get_kb_classes" that defaults selector to system-default
  public Node cached_p_get_kb_classes
     () {
     return this.cached_p_get_kb_classes(_system_default);
  }


  public void encache_get_kb_classes
     (Node _values_,
      Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_classes_fname,
                       this,
                       selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_kb_classes" that defaults kb-local-only-p to nil
  public void encache_get_kb_classes
     (Node _values_,
      Node selector) {
     this.encache_get_kb_classes(_values_,
                                 selector,
                                 _NIL);
  }


  // Method for "encache_get_kb_classes" that defaults selector to system-default
  public void encache_get_kb_classes
     (Node _values_) {
     this.encache_get_kb_classes(_values_,
                                 _system_default);
  }


// ---------------------- get-kb-direct-children

/**
 * Returns the   list-of-child-kbs  -- that is, the list of KBs that
 * directly include   kb .  Note that certain KB implementations may
 * allow circular inclusion dependencies in KBs.  The semantics of KB
 * inclusion are not specified by OKBC, but where possible, processing can
 * be limited to a particular KB by the use of the   kb-local-only-p 
 * argument.
 */
  public Node get_kb_direct_children
     () {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_direct_children_fname,
                       this);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_kb_direct_children_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_kb_direct_children_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_kb_direct_children();
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_kb_direct_children();
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_kb_direct_children_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_kb_direct_children"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_kb_direct_children_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_kb_direct_children();
       }
    }


  public Node cached_p_get_kb_direct_children
     () {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_direct_children_fname,
                       this);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  public void encache_get_kb_direct_children
     (Node _values_) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_direct_children_fname,
                       this);
         encache(_values_, key);
       }
  }

// ---------------------- get-kb-direct-parents

/**
 * Returns the   list-of-parent-kbs  -- that is, the list of KBs directly
 * included by   kb .  Note that certain KB implementations may allow
 * circular inclusion dependencies in KBs.  The semantics of KB inclusion
 * are not specified by OKBC, but where possible, processing can be limited
 * to a particular KB by the use of the   kb-local-only-p  argument.
 */
  public Node get_kb_direct_parents
     () {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_direct_parents_fname,
                       this);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_kb_direct_parents_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_kb_direct_parents_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_kb_direct_parents();
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_kb_direct_parents();
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_kb_direct_parents_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_kb_direct_parents"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_kb_direct_parents_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_kb_direct_parents();
       }
    }


  public Node cached_p_get_kb_direct_parents
     () {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_direct_parents_fname,
                       this);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  public void encache_get_kb_direct_parents
     (Node _values_) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_direct_parents_fname,
                       this);
         encache(_values_, key);
       }
  }

// ---------------------- get-kb-facets

/**
 * Returns the   list-of-facets  in   kb .
 *   Selector  can be one of the following:
 *  
 *      :all  -- Returns all facets
 *      :frames  -- Returns facets that are represented as frames
 *      :system-default  -- Returns either all facets or
 * only facets represented as frames, according to which is the KRS's
 * default
 */
  public Node get_kb_facets
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_facets_fname,
                       this,
                       selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_kb_facets_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_kb_facets_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_kb_facets(selector,
                                               kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_kb_facets(selector,
                                                   kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_kb_facets_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_kb_facets"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_kb_facets_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_kb_facets(selector,
                                    kb_local_only_p);
       }
    }


  public Node cached_p_get_kb_facets
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_facets_fname,
                       this,
                       selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_kb_facets" that defaults kb-local-only-p to nil
  public Node cached_p_get_kb_facets
     (Node selector) {
     return this.cached_p_get_kb_facets(selector,
                                        _NIL);
  }


  // Method for "cached_p_get_kb_facets" that defaults selector to system-default
  public Node cached_p_get_kb_facets
     () {
     return this.cached_p_get_kb_facets(_system_default);
  }


  public void encache_get_kb_facets
     (Node _values_,
      Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_facets_fname,
                       this,
                       selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_kb_facets" that defaults kb-local-only-p to nil
  public void encache_get_kb_facets
     (Node _values_,
      Node selector) {
     this.encache_get_kb_facets(_values_,
                                selector,
                                _NIL);
  }


  // Method for "encache_get_kb_facets" that defaults selector to system-default
  public void encache_get_kb_facets
     (Node _values_) {
     this.encache_get_kb_facets(_values_,
                                _system_default);
  }


// ---------------------- get-kb-frames

/**
 * Returns the   list-of-frames  in the KB, including class, slot,
 * facets and individual frames, when present.
 */
  public Node get_kb_frames
     (TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_frames_fname,
                       this,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_kb_frames_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_kb_frames_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_kb_frames(kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_kb_frames(kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_kb_frames_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_kb_frames"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_kb_frames_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_kb_frames(kb_local_only_p);
       }
    }


  public Node cached_p_get_kb_frames
     (TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_frames_fname,
                       this,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_kb_frames" that defaults kb-local-only-p to nil
  public Node cached_p_get_kb_frames
     () {
     return this.cached_p_get_kb_frames(_NIL);
  }


  public void encache_get_kb_frames
     (Node _values_,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_frames_fname,
                       this,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_kb_frames" that defaults kb-local-only-p to nil
  public void encache_get_kb_frames
     (Node _values_) {
     this.encache_get_kb_frames(_values_,
                                _NIL);
  }


// ---------------------- get-kb-individuals

/**
 * Returns   list-of-individuals , a list of the individual frames in
 *   kb .    Selector  can be one of the following:
 *  
 *      :all  -- Returns all accessible individuals
 *      :frames  -- Returns only individuals that are frames
 *      :system-default  -- Returns either all individuals or
 * only individual frames, according to which is the KRS's default
 */
  public Node get_kb_individuals
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_individuals_fname,
                       this,
                       selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_kb_individuals_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_kb_individuals_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_kb_individuals(selector,
                                                    kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_kb_individuals(selector,
                                                        kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_kb_individuals_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_kb_individuals"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_kb_individuals_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_kb_individuals(selector,
                                         kb_local_only_p);
       }
    }


  public Node cached_p_get_kb_individuals
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_individuals_fname,
                       this,
                       selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_kb_individuals" that defaults kb-local-only-p to nil
  public Node cached_p_get_kb_individuals
     (Node selector) {
     return this.cached_p_get_kb_individuals(selector,
                                             _NIL);
  }


  // Method for "cached_p_get_kb_individuals" that defaults selector to system-default
  public Node cached_p_get_kb_individuals
     () {
     return this.cached_p_get_kb_individuals(_system_default);
  }


  public void encache_get_kb_individuals
     (Node _values_,
      Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_individuals_fname,
                       this,
                       selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_kb_individuals" that defaults kb-local-only-p to nil
  public void encache_get_kb_individuals
     (Node _values_,
      Node selector) {
     this.encache_get_kb_individuals(_values_,
                                     selector,
                                     _NIL);
  }


  // Method for "encache_get_kb_individuals" that defaults selector to system-default
  public void encache_get_kb_individuals
     (Node _values_) {
     this.encache_get_kb_individuals(_values_,
                                     _system_default);
  }


// ---------------------- get-kb-roots

/**
 * Every KB has one or more frames at the top (root) of the
 * KB.  A frame   is a root of the KB   if there exists no class   such
 * that   is a superclass of   and   is in the KB   and if there
 * exists no class   such that   is a type of   and   is in the
 * KB  , or available in   when   kb-local-only-p  is  .  This
 * operation identifies and returns those roots, the   list-of-roots .
 * Note that this means that unparented individuals, slots and facets will
 * also be returned.
 * 
 * Some KRSs allow    user-defined  classes to be roots of a KB, whereas
 * other KRSs always import certain    system-defined  classes (for example,
 *    thing ) into each KB and force all
 * user classes to be subclasses of    thing.  These system-defined classes
 * may normally be invisible to the user in some KRSs.  The
 *   selector  argument controls which root classes are returned as
 * follows:
 * 
 *  
 *      selector = :all  returns all the true root classes of the
 * KB, regardless of whether they are    system-defined  or
 *    user-defined .
 *      selector = :user  returns the user-defined
 * root classes of the KB, namely all classes   available in the KB such that
 *   was defined by a user application as opposed to being a built-in
 * part of every KB, and such that there exists no class   that is both
 *    user-defined  and a superclass of  .  That is, there may
 * exist    system-defined  superclasses of  .
 *  
 * 
 * If   kb-local-only-p  is  , the list returned may return only
 * the root classes defined in   kb  itself; classes that were
 * inherited from other (included) KBs may be excluded.  This means
 * that a class that has superclasses in some KB included by   kb ,
 * but has no superclasses defined in   kb , may be returned as a
 * root class if   kb-local-only-p  is  .
 */
  public Node get_kb_roots
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_roots_fname,
                       this,
                       selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_kb_roots_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_kb_roots_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_kb_roots(selector,
                                              kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_kb_roots(selector,
                                                  kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_kb_roots_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_kb_roots"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_kb_roots_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_kb_roots(selector,
                                   kb_local_only_p);
       }
    }


  public Node cached_p_get_kb_roots
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_roots_fname,
                       this,
                       selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_kb_roots" that defaults kb-local-only-p to nil
  public Node cached_p_get_kb_roots
     (Node selector) {
     return this.cached_p_get_kb_roots(selector,
                                       _NIL);
  }


  // Method for "cached_p_get_kb_roots" that defaults selector to all
  public Node cached_p_get_kb_roots
     () {
     return this.cached_p_get_kb_roots(_all);
  }


  public void encache_get_kb_roots
     (Node _values_,
      Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_roots_fname,
                       this,
                       selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_kb_roots" that defaults kb-local-only-p to nil
  public void encache_get_kb_roots
     (Node _values_,
      Node selector) {
     this.encache_get_kb_roots(_values_,
                               selector,
                               _NIL);
  }


  // Method for "encache_get_kb_roots" that defaults selector to all
  public void encache_get_kb_roots
     (Node _values_) {
     this.encache_get_kb_roots(_values_,
                               _all);
  }


// ---------------------- get-kb-slots

/**
 * Returns the   list-of-slots  that are defined in the KB.
 *   Selector  can be one of the following:
 *  
 *      :all  -- Returns all slots
 *      :frames  -- Returns slots that are represented frames
 *      :system-default  -- Returns either all slots or
 * only slot frames, according to which is the KRS's default
 */
  public Node get_kb_slots
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_slots_fname,
                       this,
                       selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_kb_slots_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_kb_slots_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_kb_slots(selector,
                                              kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_kb_slots(selector,
                                                  kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_kb_slots_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_kb_slots"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_kb_slots_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_kb_slots(selector,
                                   kb_local_only_p);
       }
    }


  public Node cached_p_get_kb_slots
     (Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_slots_fname,
                       this,
                       selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_kb_slots" that defaults kb-local-only-p to nil
  public Node cached_p_get_kb_slots
     (Node selector) {
     return this.cached_p_get_kb_slots(selector,
                                       _NIL);
  }


  // Method for "cached_p_get_kb_slots" that defaults selector to system-default
  public Node cached_p_get_kb_slots
     () {
     return this.cached_p_get_kb_slots(_system_default);
  }


  public void encache_get_kb_slots
     (Node _values_,
      Node selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_kb_slots_fname,
                       this,
                       selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_kb_slots" that defaults kb-local-only-p to nil
  public void encache_get_kb_slots
     (Node _values_,
      Node selector) {
     this.encache_get_kb_slots(_values_,
                               selector,
                               _NIL);
  }


  // Method for "encache_get_kb_slots" that defaults selector to system-default
  public void encache_get_kb_slots
     (Node _values_) {
     this.encache_get_kb_slots(_values_,
                               _system_default);
  }


// ---------------------- get-procedure

/**
 * Returns the   procedure  that is the procedure association for
 * the   name , or   if there is no such procedure association.
 * See   register-procedure ,   unregister-procedure , and
 *   call-procedure .
 */
  public Node get_procedure
     (Node name) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_procedure_fname,
                       this,
                       name);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_procedure_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_procedure_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_procedure(name);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_procedure(name);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_procedure_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_procedure"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_procedure_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_procedure(name);
       }
    }


  public Node cached_p_get_procedure
     (Node name) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_procedure_fname,
                       this,
                       name);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  public void encache_get_procedure
     (Node _values_,
      Node name) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_procedure_fname,
                       this,
                       name);
         encache(_values_, key);
       }
  }

// ---------------------- get-slot-facets

/**
 * Returns the   list-of-facets  associated with   slot  in
 *   frame .
 */
  public Values2 get_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_facets_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_slot_facets_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_slot_facets_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_slot_facets(frame,
                                                 slot,
                                                 inference_level,
                                                 slot_type,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_slot_facets(frame,
                                                     slot,
                                                     inference_level,
                                                     slot_type,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_slot_facets_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_slot_facets"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_slot_facets_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_slot_facets(frame,
                                      slot,
                                      inference_level,
                                      slot_type,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_get_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_facets_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_slot_facets" that defaults kb-local-only-p to nil
  public Node cached_p_get_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_slot_facets(frame,
                                          slot,
                                          inference_level,
                                          slot_type,
                                          _NIL);
  }


  // Method for "cached_p_get_slot_facets" that defaults slot-type to own
  public Node cached_p_get_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.cached_p_get_slot_facets(frame,
                                          slot,
                                          inference_level,
                                          _own);
  }


  // Method for "cached_p_get_slot_facets" that defaults inference-level to taxonomic
  public Node cached_p_get_slot_facets
     (Node frame,
      Node slot) {
     return this.cached_p_get_slot_facets(frame,
                                          slot,
                                          _taxonomic);
  }


  public void encache_get_slot_facets
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_facets_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_slot_facets" that defaults kb-local-only-p to nil
  public void encache_get_slot_facets
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_slot_facets(_values_,
                                  frame,
                                  slot,
                                  inference_level,
                                  slot_type,
                                  _NIL);
  }


  // Method for "encache_get_slot_facets" that defaults slot-type to own
  public void encache_get_slot_facets
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level) {
     this.encache_get_slot_facets(_values_,
                                  frame,
                                  slot,
                                  inference_level,
                                  _own);
  }


  // Method for "encache_get_slot_facets" that defaults inference-level to taxonomic
  public void encache_get_slot_facets
     (Values2 _values_,
      Node frame,
      Node slot) {
     this.encache_get_slot_facets(_values_,
                                  frame,
                                  slot,
                                  _taxonomic);
  }


// ---------------------- get-slot-type

/**
 * Returns one of     :own ,    :template ,    to
 * identify the   slot-type  of the slot on question.  If there are both
 * an own and a template slot on   frame  identified by   slot , then
 *    :own  is returned.
 * If no such slot is known, then   is returned.
 */
  public Node get_slot_type
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_type_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_slot_type_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_slot_type_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_slot_type(frame,
                                               slot,
                                               inference_level,
                                               kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_slot_type(frame,
                                                   slot,
                                                   inference_level,
                                                   kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_slot_type_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("get_slot_type"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_slot_type_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_slot_type(frame,
                                    slot,
                                    inference_level,
                                    kb_local_only_p);
       }
    }


  public Node cached_p_get_slot_type
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_type_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_slot_type" that defaults kb-local-only-p to nil
  public Node cached_p_get_slot_type
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.cached_p_get_slot_type(frame,
                                        slot,
                                        inference_level,
                                        _NIL);
  }


  // Method for "cached_p_get_slot_type" that defaults inference-level to taxonomic
  public Node cached_p_get_slot_type
     (Node frame,
      Node slot) {
     return this.cached_p_get_slot_type(frame,
                                        slot,
                                        _taxonomic);
  }


  public void encache_get_slot_type
     (Node _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_type_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_slot_type" that defaults kb-local-only-p to nil
  public void encache_get_slot_type
     (Node _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level) {
     this.encache_get_slot_type(_values_,
                                frame,
                                slot,
                                inference_level,
                                _NIL);
  }


  // Method for "encache_get_slot_type" that defaults inference-level to taxonomic
  public void encache_get_slot_type
     (Node _values_,
      Node frame,
      Node slot) {
     this.encache_get_slot_type(_values_,
                                frame,
                                slot,
                                _taxonomic);
  }


// ---------------------- get-slot-value

/**
 * Returns the single member of the set of values
 * of the   slot .  This operation is meaningful only for single-valued
 * slots.  It is an error to call   get-slot-value  on a non-single-valued
 * slot, and implementations should signal a   cardinality-violation  if
 * this occurs.  When there is no value for the slot,   value-or-false 
 * is  .
 */
  public Values2 get_slot_value
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_value_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_slot_value_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_slot_value_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_slot_value(frame,
                                                slot,
                                                inference_level,
                                                slot_type,
                                                value_selector,
                                                kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_slot_value(frame,
                                                    slot,
                                                    inference_level,
                                                    slot_type,
                                                    value_selector,
                                                    kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_slot_value_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_slot_value"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_slot_value_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_slot_value(frame,
                                     slot,
                                     inference_level,
                                     slot_type,
                                     value_selector,
                                     kb_local_only_p);
       }
    }


  public Node cached_p_get_slot_value
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_value_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_slot_value" that defaults kb-local-only-p to nil
  public Node cached_p_get_slot_value
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.cached_p_get_slot_value(frame,
                                         slot,
                                         inference_level,
                                         slot_type,
                                         value_selector,
                                         _NIL);
  }


  // Method for "cached_p_get_slot_value" that defaults value-selector to either
  public Node cached_p_get_slot_value
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_slot_value(frame,
                                         slot,
                                         inference_level,
                                         slot_type,
                                         _either);
  }


  // Method for "cached_p_get_slot_value" that defaults slot-type to own
  public Node cached_p_get_slot_value
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.cached_p_get_slot_value(frame,
                                         slot,
                                         inference_level,
                                         _own);
  }


  // Method for "cached_p_get_slot_value" that defaults inference-level to taxonomic
  public Node cached_p_get_slot_value
     (Node frame,
      Node slot) {
     return this.cached_p_get_slot_value(frame,
                                         slot,
                                         _taxonomic);
  }


  public void encache_get_slot_value
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_value_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_slot_value" that defaults kb-local-only-p to nil
  public void encache_get_slot_value
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.encache_get_slot_value(_values_,
                                 frame,
                                 slot,
                                 inference_level,
                                 slot_type,
                                 value_selector,
                                 _NIL);
  }


  // Method for "encache_get_slot_value" that defaults value-selector to either
  public void encache_get_slot_value
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_slot_value(_values_,
                                 frame,
                                 slot,
                                 inference_level,
                                 slot_type,
                                 _either);
  }


  // Method for "encache_get_slot_value" that defaults slot-type to own
  public void encache_get_slot_value
     (Values2 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level) {
     this.encache_get_slot_value(_values_,
                                 frame,
                                 slot,
                                 inference_level,
                                 _own);
  }


  // Method for "encache_get_slot_value" that defaults inference-level to taxonomic
  public void encache_get_slot_value
     (Values2 _values_,
      Node frame,
      Node slot) {
     this.encache_get_slot_value(_values_,
                                 frame,
                                 slot,
                                 _taxonomic);
  }


// ---------------------- get-slot-values

/**
 * Returns the   list-of-values  of   slot 
 * within   frame .  If the    :collection-type  of the slot is
 *    :list , and only    :direct  own slots have been asserted, then
 * order is preserved; otherwise, the values are returned in no guaranteed
 * order.    Get-slot-values  always returns a list of
 * values.  If the specified slot has no values,   is
 * returned.
 */
  public Values3 get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_values_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_slot_values_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_slot_values_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_slot_values(frame,
                                                 slot,
                                                 inference_level,
                                                 slot_type,
                                                 number_of_values,
                                                 value_selector,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_slot_values(frame,
                                                     slot,
                                                     inference_level,
                                                     slot_type,
                                                     number_of_values,
                                                     value_selector,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_slot_values_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_slot_values"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_slot_values_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_slot_values(frame,
                                      slot,
                                      inference_level,
                                      slot_type,
                                      number_of_values,
                                      value_selector,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_values_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_slot_values" that defaults kb-local-only-p to nil
  public Node cached_p_get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.cached_p_get_slot_values(frame,
                                          slot,
                                          inference_level,
                                          slot_type,
                                          number_of_values,
                                          value_selector,
                                          _NIL);
  }


  // Method for "cached_p_get_slot_values" that defaults value-selector to either
  public Node cached_p_get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.cached_p_get_slot_values(frame,
                                          slot,
                                          inference_level,
                                          slot_type,
                                          number_of_values,
                                          _either);
  }


  // Method for "cached_p_get_slot_values" that defaults number-of-values to all
  public Node cached_p_get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_slot_values(frame,
                                          slot,
                                          inference_level,
                                          slot_type,
                                          _all);
  }


  // Method for "cached_p_get_slot_values" that defaults slot-type to own
  public Node cached_p_get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.cached_p_get_slot_values(frame,
                                          slot,
                                          inference_level,
                                          _own);
  }


  // Method for "cached_p_get_slot_values" that defaults inference-level to taxonomic
  public Node cached_p_get_slot_values
     (Node frame,
      Node slot) {
     return this.cached_p_get_slot_values(frame,
                                          slot,
                                          _taxonomic);
  }


  public void encache_get_slot_values
     (Values3 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_values_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_slot_values" that defaults kb-local-only-p to nil
  public void encache_get_slot_values
     (Values3 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     this.encache_get_slot_values(_values_,
                                  frame,
                                  slot,
                                  inference_level,
                                  slot_type,
                                  number_of_values,
                                  value_selector,
                                  _NIL);
  }


  // Method for "encache_get_slot_values" that defaults value-selector to either
  public void encache_get_slot_values
     (Values3 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     this.encache_get_slot_values(_values_,
                                  frame,
                                  slot,
                                  inference_level,
                                  slot_type,
                                  number_of_values,
                                  _either);
  }


  // Method for "encache_get_slot_values" that defaults number-of-values to all
  public void encache_get_slot_values
     (Values3 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_slot_values(_values_,
                                  frame,
                                  slot,
                                  inference_level,
                                  slot_type,
                                  _all);
  }


  // Method for "encache_get_slot_values" that defaults slot-type to own
  public void encache_get_slot_values
     (Values3 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level) {
     this.encache_get_slot_values(_values_,
                                  frame,
                                  slot,
                                  inference_level,
                                  _own);
  }


  // Method for "encache_get_slot_values" that defaults inference-level to taxonomic
  public void encache_get_slot_values
     (Values3 _values_,
      Node frame,
      Node slot) {
     this.encache_get_slot_values(_values_,
                                  frame,
                                  slot,
                                  _taxonomic);
  }


// ---------------------- get-slot-values-in-detail

/**
 * Returns the   list-of-specs  describing the values
 * of   slot  within   frame .  If the    :collection-type 
 * of the slot is    :list , and only    :direct  own slots have been
 * asserted, then order is preserved; otherwise, the values are returned in
 * no guaranteed order.   Get-slot-values-in-detail  always returns a
 * list of specifications as its   list-of-specs  value.  If the specified
 * slot has no values,   is returned.
 * 
 * Each spec is a 3-tuple of the form (value direct-p default-p).
 *  
 *   value -- A value of the slot
 *   direct-p -- A flag that is   if the value is known to be
 * directly asserted for the slot and   otherwise
 *   default-p -- A flag that is   if the value is known to be
 * a default value, and   otherwise
 *  
 * The   default-p  returned value is true if the   list-of-specs 
 * is  , and the fact that there are no values is itself a default.
 */
  public Values4 get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_values_in_detail_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(get_slot_values_in_detail_fname, key, trie_result);
             return ((Values4)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(get_slot_values_in_detail_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.get_slot_values_in_detail(frame,
                                                           slot,
                                                           inference_level,
                                                           slot_type,
                                                           number_of_values,
                                                           value_selector,
                                                           kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.get_slot_values_in_detail(frame,
                                                               slot,
                                                               inference_level,
                                                               slot_type,
                                                               number_of_values,
                                                               value_selector,
                                                               kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(get_slot_values_in_detail_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("get_slot_values_in_detail"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values4)new_result;
                  }
                else
                  { Values4 new_result = 
                        Values.toValues4(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(get_slot_values_in_detail_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.get_slot_values_in_detail(frame,
                                                slot,
                                                inference_level,
                                                slot_type,
                                                number_of_values,
                                                value_selector,
                                                kb_local_only_p);
       }
    }


  public Node cached_p_get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_values_in_detail_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_get_slot_values_in_detail" that defaults kb-local-only-p to nil
  public Node cached_p_get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.cached_p_get_slot_values_in_detail(frame,
                                                    slot,
                                                    inference_level,
                                                    slot_type,
                                                    number_of_values,
                                                    value_selector,
                                                    _NIL);
  }


  // Method for "cached_p_get_slot_values_in_detail" that defaults value-selector to either
  public Node cached_p_get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.cached_p_get_slot_values_in_detail(frame,
                                                    slot,
                                                    inference_level,
                                                    slot_type,
                                                    number_of_values,
                                                    _either);
  }


  // Method for "cached_p_get_slot_values_in_detail" that defaults number-of-values to all
  public Node cached_p_get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_get_slot_values_in_detail(frame,
                                                    slot,
                                                    inference_level,
                                                    slot_type,
                                                    _all);
  }


  // Method for "cached_p_get_slot_values_in_detail" that defaults slot-type to own
  public Node cached_p_get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.cached_p_get_slot_values_in_detail(frame,
                                                    slot,
                                                    inference_level,
                                                    _own);
  }


  // Method for "cached_p_get_slot_values_in_detail" that defaults inference-level to taxonomic
  public Node cached_p_get_slot_values_in_detail
     (Node frame,
      Node slot) {
     return this.cached_p_get_slot_values_in_detail(frame,
                                                    slot,
                                                    _taxonomic);
  }


  public void encache_get_slot_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(get_slot_values_in_detail_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       number_of_values,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_get_slot_values_in_detail" that defaults kb-local-only-p to nil
  public void encache_get_slot_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     this.encache_get_slot_values_in_detail(_values_,
                                            frame,
                                            slot,
                                            inference_level,
                                            slot_type,
                                            number_of_values,
                                            value_selector,
                                            _NIL);
  }


  // Method for "encache_get_slot_values_in_detail" that defaults value-selector to either
  public void encache_get_slot_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     this.encache_get_slot_values_in_detail(_values_,
                                            frame,
                                            slot,
                                            inference_level,
                                            slot_type,
                                            number_of_values,
                                            _either);
  }


  // Method for "encache_get_slot_values_in_detail" that defaults number-of-values to all
  public void encache_get_slot_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_get_slot_values_in_detail(_values_,
                                            frame,
                                            slot,
                                            inference_level,
                                            slot_type,
                                            _all);
  }


  // Method for "encache_get_slot_values_in_detail" that defaults slot-type to own
  public void encache_get_slot_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level) {
     this.encache_get_slot_values_in_detail(_values_,
                                            frame,
                                            slot,
                                            inference_level,
                                            _own);
  }


  // Method for "encache_get_slot_values_in_detail" that defaults inference-level to taxonomic
  public void encache_get_slot_values_in_detail
     (Values4 _values_,
      Node frame,
      Node slot) {
     this.encache_get_slot_values_in_detail(_values_,
                                            frame,
                                            slot,
                                            _taxonomic);
  }


// ---------------------- individual-p

/**
 * Returns   if   thing  identifies an individual entity, and
 * returns   if   thing  identifies a class.
 */
  public Node individual_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(individual_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(individual_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(individual_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.individual_p(thing,
                                              kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.individual_p(thing,
                                                  kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(individual_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("individual_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(individual_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.individual_p(thing,
                                   kb_local_only_p);
       }
    }


  public Node cached_p_individual_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(individual_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_individual_p" that defaults kb-local-only-p to nil
  public Node cached_p_individual_p
     (Node thing) {
     return this.cached_p_individual_p(thing,
                                       _NIL);
  }


  public void encache_individual_p
     (Node _values_,
      Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(individual_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_individual_p" that defaults kb-local-only-p to nil
  public void encache_individual_p
     (Node _values_,
      Node thing) {
     this.encache_individual_p(_values_,
                               thing,
                               _NIL);
  }


// ---------------------- instance-of-p

/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  public Values2 instance_of_p
     (Node thing,
      Node the_class,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(instance_of_p_fname,
                       this,
                       thing,
                       the_class,
                       inference_level,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(instance_of_p_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(instance_of_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.instance_of_p(thing,
                                               the_class,
                                               inference_level,
                                               kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.instance_of_p(thing,
                                                   the_class,
                                                   inference_level,
                                                   kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(instance_of_p_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("instance_of_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(instance_of_p_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.instance_of_p(thing,
                                    the_class,
                                    inference_level,
                                    kb_local_only_p);
       }
    }


  public Node cached_p_instance_of_p
     (Node thing,
      Node the_class,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(instance_of_p_fname,
                       this,
                       thing,
                       the_class,
                       inference_level,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_instance_of_p" that defaults kb-local-only-p to nil
  public Node cached_p_instance_of_p
     (Node thing,
      Node the_class,
      InferenceLevel inference_level) {
     return this.cached_p_instance_of_p(thing,
                                        the_class,
                                        inference_level,
                                        _NIL);
  }


  // Method for "cached_p_instance_of_p" that defaults inference-level to taxonomic
  public Node cached_p_instance_of_p
     (Node thing,
      Node the_class) {
     return this.cached_p_instance_of_p(thing,
                                        the_class,
                                        _taxonomic);
  }


  public void encache_instance_of_p
     (Values2 _values_,
      Node thing,
      Node the_class,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(instance_of_p_fname,
                       this,
                       thing,
                       the_class,
                       inference_level,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_instance_of_p" that defaults kb-local-only-p to nil
  public void encache_instance_of_p
     (Values2 _values_,
      Node thing,
      Node the_class,
      InferenceLevel inference_level) {
     this.encache_instance_of_p(_values_,
                                thing,
                                the_class,
                                inference_level,
                                _NIL);
  }


  // Method for "encache_instance_of_p" that defaults inference-level to taxonomic
  public void encache_instance_of_p
     (Values2 _values_,
      Node thing,
      Node the_class) {
     this.encache_instance_of_p(_values_,
                                thing,
                                the_class,
                                _taxonomic);
  }


// ---------------------- add-class-superclass

/**
 * Adds the   new-superclass  to the superclasses of   class .
 * Returns no values.
 */
  public void add_class_superclass
     (Node the_class,
      Node new_superclass,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("add_class_superclass");
         flush_cache("add_class_superclass"); }
     try { connection().ok_to_cache_flag = _testing;
           super.add_class_superclass(the_class,
                                      new_superclass,
                                      kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("add_class_superclass"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- add-facet-value

/**
 * If the specified facet does not already contain   value ,
 * then   value  is added to the set of values of the facet.
 * Returns no values.
 */
  public void add_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("add_facet_value");
         flush_cache("add_facet_value"); }
     try { connection().ok_to_cache_flag = _testing;
           super.add_facet_value(frame,
                                 slot,
                                 facet,
                                 value,
                                 test,
                                 slot_type,
                                 value_selector,
                                 kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("add_facet_value"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- add-instance-type

/**
 * Adds the   new-type  to the types of   frame .
 * Returns no values.
 */
  public void add_instance_type
     (Node frame,
      Node new_type,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("add_instance_type");
         flush_cache("add_instance_type"); }
     try { connection().ok_to_cache_flag = _testing;
           super.add_instance_type(frame,
                                   new_type,
                                   kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("add_instance_type"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- add-slot-value

/**
 * Value  is added to the set of values of   slot .  If the
 * collection-type of   slot  is    :set , then   value  is
 * added only if   slot  does not already contain   value .
 *   Add-before , if supplied, should be   or a nonnegative
 * integer.  If the collection-type of
 *   slot  is    :list ,   value  is added immediately before
 * the value whose index is   add-before .  For example, if
 *   add-before  is 1, the new value will be added between the first
 * and second old values.  If   add-before  is greater than or equal
 * to the current number of slot values, or is  , and the
 * collection-type of   slot  is    :list , then   value  is
 * added after all other values.  This operation may signal constraint
 * violation conditions (see Section  ).  It is an error
 * to provide a nonpositive integer as a value for   add-before .
 * Returns no values.
 */
  public void add_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type,
      Node add_before,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("add_slot_value");
         flush_cache("add_slot_value"); }
     try { connection().ok_to_cache_flag = _testing;
           super.add_slot_value(frame,
                                slot,
                                value,
                                test,
                                slot_type,
                                add_before,
                                value_selector,
                                kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("add_slot_value"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- allocate-frame-handle

/**
 * Allocates a frame handle in   kb .  It is not anticipated that this
 * operation will be called by OKBC applications, but rather by OKBC back end
 * implementations.  This operation can be used in two distinct ways:
 *   
 *   Given a frame located in an arbitrary KB, typically different
 * from   kb , passing its   frame-name ,   frame-type , and
 *   frame-handle  will return a frame handle to represent that frame if
 * such a frame were to be created in   kb .  This is useful in OKBC
 * operations such as   copy-frame  and   copy-kb  where it is often
 * necessary to make forward references to frame objects.
 *   Providing just a   frame-name  and a   frame-type 
 * will return a frame handle to represent that frame if such a frame were
 * to be created in   kb .  This is useful when an implementation wants
 * to allocate a frame handle either during the frame creation process, or
 * to create forward references to frames when faulting them in from a
 * lazy persistent store.    Frame-name  may be  .
 *  
 *   Frame-type  is the type of the frame as identified by the operation
 *   get-frame-type ; that is, it must be in the set     :class ,
 *    :individual ,    :slot ,    :facet  .
 * 
 * The rationale for the arguments to this operation is as follows:
 *  
 *     frame-name  -- In some KRSs, the name of a frame cannot be
 * changed after the frame handle has been allocated.  OKBC therefore
 * mandates that the name be supplied.  If the
 *    :frame-names-required  behavior has the value  ,
 * this argument may be  .
 *     frame-type  -- In some KRSs, the type of data structure used
 * to represent the frame handles of (say) classes is completely
 * different from that of (say) individual frames.  OKBC therefore
 * mandates that the frame type be specified.  Implementations that use
 * the same representation for all frame handles will be able to ignore
 * this argument, but it is not portable.
 *     frame-handle  -- Some KRSs may choose to use a frame handle
 * provided as the value of the   frame-handle  argument as the
 * new frame handle.  This allows implementations that do not have a
 * special data structure for frame handles to save memory and to
 * maximize the correspondence between the objects in different KBs.
 *  
 * The contract of   allocate-frame-handle  does not require the callee to
 * return the same frame handle if called multiple times with identical
 * arguments.  Note that this is particularly important in case 2, above,
 * with   frame-name  being  .  It is the responsibility of
 * the caller to remember the
 * correspondence between its frames and the frame handles allocated.  A frame
 * handle allocated using   allocate-frame-handle  can be used as the
 * value of the   handle  argument to   create-frame ,
 *   create-class ,   create-slot ,   create-facet , and
 *   create-individual .  During the execution of these operations, it is
 * the responsibility of the   kb  to preserve any necessary object
 * identity so that, for example,
 *   
 * new-handle = allocate-frame-handle(name, :class, kb, handle);
 * new-frame = create-class(name, .... :handle new-handle);
 * new-handle == get-frame-handle(new-frame) // this identity must hold!
 */
  public Node allocate_frame_handle
     (Node frame_name,
      ContextName frame_type,
      Node frame_handle) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("allocate_frame_handle");
         flush_cache("allocate_frame_handle"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.allocate_frame_handle(frame_name,
                                              frame_type,
                                              frame_handle);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("allocate_frame_handle"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- ask

/**
 * Asks a   query  of the OKBC   kb .    Query  may be any
 * sentence in the OKBC Assertion Language that is   askable .  A
 *   cannot-handle  error
 * may be signaled if it is not   askable .   Reply-pattern  is an
 * expression mentioning KIF variables contained in   query .
 * 
 *   Reply-pattern  is any list structure mentioning the variables
 * in the query, or just the name of a variable.  For example, consider
 * a query that is a sentence of the form,
 *   
 * (subclass-of ?x ?y)
 *  
 * that is, find me the things that are subclasses of other things.  If there
 * is a match in the KB for    ?x = human  and    ?y = animal .
 * -- the class    human  is a subclass of the class    animal  -- then
 * if the   reply-pattern  were to be
 *   
 * (superclass-of ?y ?x)
 *  
 * we would be returned a list of sentences of which one
 * would be    (superclass-of animal human) .  The explicit use of a reply
 * pattern in this manner allows the user to get either sentences
 * that can be conveniently reasserted using   tell , or tuples of
 * matches in a shape that is convenient to the application.
 * 
 * When   error-p  is  , any errors resulting from the execution of
 * the query are signaled.  When error-p is  , all possible attempts are
 * made to continue with the query and deliver as many results as were
 * requested.
 * 
 * If the resources used by   ask  are a concern, the time (in
 * seconds) allowed to answer a query will be limited, if possible, as
 * specified by   timeout .  If the value of   timeout  is
 *  , an unlimited time is allowed for   ask  to complete.
 * 
 * The   where  clause can be used to specify a list of bindings to
 * be used for any variables appearing in the   query .  During query
 * evaluation, such variables are replaced by the values specified by
 *   where .  A valid value of   where  is a list of 2-tuples,
 * with each tuple consisting of a variable and value pair.
 * 
 *   Ask  returns three values.
 *   
 *     reply-pattern-list  -- In this list, each element is an
 * instance of   reply-pattern , with all variables mentioned in
 *   query  substituted.
 *     exact-p  -- This has its normal interpretation.
 *     more-status  -- This has its normal interpretation, except
 * that an additional option    :timeout  may be returned for the
 * more-status value by   ask  if the call terminates because
 * execution time exceeds the limit specified by the   timeout 
 * argument.
 *  
 * When   ask  is given a syntactically invalid   query , it
 * signals the   syntax-error  error.  When   ask  realizes that
 * the   query  cannot be handled by the KRS, it signals a
 *   cannot-handle  error.
 * 
 * The following query matches four channel oscilloscopes with a
 * bandwidth greater than 80MHz.  It returns a list of pairs   
 * (?osc ?bandwidth)  satisfying the query.
 *   
 * (ask '(and (oscilloscope ?osc)
 * (number-of-channels ?osc ?chans)
 * (= ?chans 4)
 * (bandwidth ?osc ?bandwidth)
 * (> ?bandwidth (* 80 mega-hertz)))
 * :reply-pattern '(?osc ?bandwidth)
 * :number-of-values 10 :kb kb)
 *  
 * All KIF operators in the   query  are parsed in a
 * package-insensitive manner.  For example,    (and A B)  and   
 * (:and A B)  have the same effect.  Object, relation, and function constant
 * references in   query  are taken as arguments to
 *   get-frames-matching .  Frame references in the query must uniquely
 * identify frames.  (See   get-frames-matching .)
 */
  public Values3 ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(ask_fname,
                       this,
                       query,
                       reply_pattern,
                       inference_level,
                       number_of_values,
                       error_p,
                       where,
                       timeout,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(ask_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(ask_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.ask(query,
                                     reply_pattern,
                                     inference_level,
                                     number_of_values,
                                     error_p,
                                     where,
                                     timeout,
                                     value_selector,
                                     kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.ask(query,
                                         reply_pattern,
                                         inference_level,
                                         number_of_values,
                                         error_p,
                                         where,
                                         timeout,
                                         value_selector,
                                         kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(ask_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("ask"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(ask_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.ask(query,
                          reply_pattern,
                          inference_level,
                          number_of_values,
                          error_p,
                          where,
                          timeout,
                          value_selector,
                          kb_local_only_p);
       }
    }


  public Node cached_p_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(ask_fname,
                       this,
                       query,
                       reply_pattern,
                       inference_level,
                       number_of_values,
                       error_p,
                       where,
                       timeout,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_ask" that defaults kb-local-only-p to nil
  public Node cached_p_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector) {
     return this.cached_p_ask(query,
                              reply_pattern,
                              inference_level,
                              number_of_values,
                              error_p,
                              where,
                              timeout,
                              value_selector,
                              _NIL);
  }


  // Method for "cached_p_ask" that defaults value-selector to either
  public Node cached_p_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout) {
     return this.cached_p_ask(query,
                              reply_pattern,
                              inference_level,
                              number_of_values,
                              error_p,
                              where,
                              timeout,
                              _either);
  }


  // Method for "cached_p_ask" that defaults timeout to nil
  public Node cached_p_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where) {
     return this.cached_p_ask(query,
                              reply_pattern,
                              inference_level,
                              number_of_values,
                              error_p,
                              where,
                              _NIL);
  }


  // Method for "cached_p_ask" that defaults where to nil
  public Node cached_p_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p) {
     return this.cached_p_ask(query,
                              reply_pattern,
                              inference_level,
                              number_of_values,
                              error_p,
                              _NIL);
  }


  // Method for "cached_p_ask" that defaults error-p to t
  public Node cached_p_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.cached_p_ask(query,
                              reply_pattern,
                              inference_level,
                              number_of_values,
                              _T);
  }


  // Method for "cached_p_ask" that defaults number-of-values to all
  public Node cached_p_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level) {
     return this.cached_p_ask(query,
                              reply_pattern,
                              inference_level,
                              _all);
  }


  // Method for "cached_p_ask" that defaults inference-level to taxonomic
  public Node cached_p_ask
     (Node query,
      Node reply_pattern) {
     return this.cached_p_ask(query,
                              reply_pattern,
                              _taxonomic);
  }


  // Method for "cached_p_ask" that defaults reply-pattern to query
  public Node cached_p_ask
     (Node query) {
     return this.cached_p_ask(query,
                              query);
  }


  public void encache_ask
     (Values3 _values_,
      Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(ask_fname,
                       this,
                       query,
                       reply_pattern,
                       inference_level,
                       number_of_values,
                       error_p,
                       where,
                       timeout,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_ask" that defaults kb-local-only-p to nil
  public void encache_ask
     (Values3 _values_,
      Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector) {
     this.encache_ask(_values_,
                      query,
                      reply_pattern,
                      inference_level,
                      number_of_values,
                      error_p,
                      where,
                      timeout,
                      value_selector,
                      _NIL);
  }


  // Method for "encache_ask" that defaults value-selector to either
  public void encache_ask
     (Values3 _values_,
      Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout) {
     this.encache_ask(_values_,
                      query,
                      reply_pattern,
                      inference_level,
                      number_of_values,
                      error_p,
                      where,
                      timeout,
                      _either);
  }


  // Method for "encache_ask" that defaults timeout to nil
  public void encache_ask
     (Values3 _values_,
      Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where) {
     this.encache_ask(_values_,
                      query,
                      reply_pattern,
                      inference_level,
                      number_of_values,
                      error_p,
                      where,
                      _NIL);
  }


  // Method for "encache_ask" that defaults where to nil
  public void encache_ask
     (Values3 _values_,
      Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p) {
     this.encache_ask(_values_,
                      query,
                      reply_pattern,
                      inference_level,
                      number_of_values,
                      error_p,
                      _NIL);
  }


  // Method for "encache_ask" that defaults error-p to t
  public void encache_ask
     (Values3 _values_,
      Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values) {
     this.encache_ask(_values_,
                      query,
                      reply_pattern,
                      inference_level,
                      number_of_values,
                      _T);
  }


  // Method for "encache_ask" that defaults number-of-values to all
  public void encache_ask
     (Values3 _values_,
      Node query,
      Node reply_pattern,
      InferenceLevel inference_level) {
     this.encache_ask(_values_,
                      query,
                      reply_pattern,
                      inference_level,
                      _all);
  }


  // Method for "encache_ask" that defaults inference-level to taxonomic
  public void encache_ask
     (Values3 _values_,
      Node query,
      Node reply_pattern) {
     this.encache_ask(_values_,
                      query,
                      reply_pattern,
                      _taxonomic);
  }


  // Method for "encache_ask" that defaults reply-pattern to query
  public void encache_ask
     (Values3 _values_,
      Node query) {
     this.encache_ask(_values_,
                      query,
                      query);
  }


// ---------------------- askable

/**
 * The   askable  operation returns   if the KRS can
 * determine that   ask ing the   sentence  would result in a
 *   cannot-handle  error being signaled, and   otherwise.
 * It may also signal the   syntax-error  condition.  Even if
 *   askable  returns  ,
 *   ask  may still not be able to handle the   sentence .
 */
  public Node askable
     (Node sentence,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(askable_fname,
                       this,
                       sentence,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(askable_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(askable_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.askable(sentence,
                                         value_selector,
                                         kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.askable(sentence,
                                             value_selector,
                                             kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(askable_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("askable"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(askable_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.askable(sentence,
                              value_selector,
                              kb_local_only_p);
       }
    }


  public Node cached_p_askable
     (Node sentence,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(askable_fname,
                       this,
                       sentence,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_askable" that defaults kb-local-only-p to nil
  public Node cached_p_askable
     (Node sentence,
      ValueSelector value_selector) {
     return this.cached_p_askable(sentence,
                                  value_selector,
                                  _NIL);
  }


  // Method for "cached_p_askable" that defaults value-selector to either
  public Node cached_p_askable
     (Node sentence) {
     return this.cached_p_askable(sentence,
                                  _either);
  }


  public void encache_askable
     (Node _values_,
      Node sentence,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(askable_fname,
                       this,
                       sentence,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_askable" that defaults kb-local-only-p to nil
  public void encache_askable
     (Node _values_,
      Node sentence,
      ValueSelector value_selector) {
     this.encache_askable(_values_,
                          sentence,
                          value_selector,
                          _NIL);
  }


  // Method for "encache_askable" that defaults value-selector to either
  public void encache_askable
     (Node _values_,
      Node sentence) {
     this.encache_askable(_values_,
                          sentence,
                          _either);
  }


// ---------------------- attach-facet

/**
 * Explicitly associates the   facet  with   slot  on   frame ,
 * in the sense of recording that values of the facet may be asserted with
 *   frame  or with instances of   frame  if   slot-type  is
 *    :template .
 * As a result,   facet  is returned by   get-slot-facets  at the
 *    :direct  inference level, and   slot-has-facet-p  will be
 *   for   facet  in   slot  on   frame .  It is an
 * error to attempt to attach a non-existent facet.  Doing so should signal
 * a   facet-not-found  error.  Returns no values.
 */
  public void attach_facet
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("attach_facet");
         flush_cache("attach_facet"); }
     try { connection().ok_to_cache_flag = _testing;
           super.attach_facet(frame,
                              slot,
                              facet,
                              slot_type,
                              kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("attach_facet"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- attach-slot

/**
 * Explicitly associates the   slot  with   frame , in the sense
 * of recording that values of slot may be asserted with   frame  or
 * with instances of   frame  if   slot-type  is    :template .
 * As a result,   slot  is returned by   get-frame-slots  at the
 *    :direct  inference level, and   frame-has-slot-p  will be
 *   for   slot  on   frame .  It is an error to attempt to
 * attach a non-existent slot.  Doing so should signal a
 *   slot-not-found  error.  Returns no values.
 */
  public void attach_slot
     (Node frame,
      Node slot,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("attach_slot");
         flush_cache("attach_slot"); }
     try { connection().ok_to_cache_flag = _testing;
           super.attach_slot(frame,
                             slot,
                             slot_type,
                             kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("attach_slot"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- call-procedure

/**
 * Returns the   value  resulting from applying
 *   procedure  to
 *   arguments .  See section   for a definition of
 * procedures.
 */
  public Node call_procedure
     (Node procedure,
      Node arguments) {
     return super.call_procedure(procedure,
                                 arguments);
    }


// ---------------------- class-p

/**
 * Returns   if   thing  identifies a class.
 */
  public Node class_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(class_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(class_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(class_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.class_p(thing,
                                         kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.class_p(thing,
                                             kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(class_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("class_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(class_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.class_p(thing,
                              kb_local_only_p);
       }
    }


  public Node cached_p_class_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(class_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_class_p" that defaults kb-local-only-p to nil
  public Node cached_p_class_p
     (Node thing) {
     return this.cached_p_class_p(thing,
                                  _NIL);
  }


  public void encache_class_p
     (Node _values_,
      Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(class_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_class_p" that defaults kb-local-only-p to nil
  public void encache_class_p
     (Node _values_,
      Node thing) {
     this.encache_class_p(_values_,
                          thing,
                          _NIL);
  }


// ---------------------- close-kb

/**
 * Deletes any in-core/accessible representation of   kb , but does not
 * remove it from any persistent store if the persistent version still
 * constitutes a meaningful KB (i.e., temporary disk work files would be
 * deleted).  It is an error ever to use   kb  again for any purpose.  If
 * this occurs, an   object-freed  error should be signaled.
 * Implementations may free any storage allocated for KB.  If   save-p 
 * is  , then any unsaved changes to   kb  will be saved
 * before it is closed.  Note that the default value of   save-p 
 * is  .  Returns no values.
 */
  public void close_kb
     (Node save_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("close_kb");
         flush_cache("close_kb"); }
     try { connection().ok_to_cache_flag = _testing;
           super.close_kb(save_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("close_kb"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- coerce-to-class

/**
 * Coerces   thing  to a class.  This operation returns two values.
 *  
 *     class  -- If   thing  identifies a class for   kb ,
 * then this value is the class so identified, or
 *   otherwise.
 *     class-found-p  -- If the class is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the class is not found, then a
 *   class-not-found  error is signaled.
 * 
 * It is an error to call   coerce-to-class  with   error-p  being
 *  , and with a value of   thing  that does not uniquely identify
 * a class.  If this happens, a   not-unique-error  error should be
 * signaled.
 * 
 * Note that in some KRS,   may be a valid class.  No portable
 * program may assume that a returned value of   for the first
 * (  class ) returned value implies that   class-found-p 
 * is  .
 */
  public Values2 coerce_to_class
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_class_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(coerce_to_class_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(coerce_to_class_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.coerce_to_class(thing,
                                                 error_p,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.coerce_to_class(thing,
                                                     error_p,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(coerce_to_class_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("coerce_to_class"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(coerce_to_class_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.coerce_to_class(thing,
                                      error_p,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_coerce_to_class
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_class_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_coerce_to_class" that defaults kb-local-only-p to nil
  public Node cached_p_coerce_to_class
     (Node thing,
      TruthValue error_p) {
     return this.cached_p_coerce_to_class(thing,
                                          error_p,
                                          _NIL);
  }


  // Method for "cached_p_coerce_to_class" that defaults error-p to t
  public Node cached_p_coerce_to_class
     (Node thing) {
     return this.cached_p_coerce_to_class(thing,
                                          _T);
  }


  public void encache_coerce_to_class
     (Values2 _values_,
      Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_class_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_coerce_to_class" that defaults kb-local-only-p to nil
  public void encache_coerce_to_class
     (Values2 _values_,
      Node thing,
      TruthValue error_p) {
     this.encache_coerce_to_class(_values_,
                                  thing,
                                  error_p,
                                  _NIL);
  }


  // Method for "encache_coerce_to_class" that defaults error-p to t
  public void encache_coerce_to_class
     (Values2 _values_,
      Node thing) {
     this.encache_coerce_to_class(_values_,
                                  thing,
                                  _T);
  }


// ---------------------- coerce-to-facet

/**
 * Coerces   thing  to a facet.  This operation returns two values.
 *  
 *     facet  -- If   thing  identifies a facet for   kb ,
 * then this value is the facet so identified, or
 *   otherwise.
 *     facet-found-p  -- If the facet is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the facet is not found, then a
 *   slot-not-found  error is signaled.
 * 
 * It is an error to call   coerce-to-facet  with   error-p  being
 *  , and with a value of   thing  that does not uniquely identify
 * a facet.  If this happens, a   not-unique-error  error should be
 * signaled.
 * 
 * Note that in some KRS,   may be a valid facet.  No portable
 * program may assume that a returned value of   for the first
 * (  facet ) returned value implies that   facet-found-p  is
 *  .
 */
  public Values2 coerce_to_facet
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_facet_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(coerce_to_facet_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(coerce_to_facet_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.coerce_to_facet(thing,
                                                 error_p,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.coerce_to_facet(thing,
                                                     error_p,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(coerce_to_facet_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("coerce_to_facet"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(coerce_to_facet_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.coerce_to_facet(thing,
                                      error_p,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_coerce_to_facet
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_facet_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_coerce_to_facet" that defaults kb-local-only-p to nil
  public Node cached_p_coerce_to_facet
     (Node thing,
      TruthValue error_p) {
     return this.cached_p_coerce_to_facet(thing,
                                          error_p,
                                          _NIL);
  }


  // Method for "cached_p_coerce_to_facet" that defaults error-p to t
  public Node cached_p_coerce_to_facet
     (Node thing) {
     return this.cached_p_coerce_to_facet(thing,
                                          _T);
  }


  public void encache_coerce_to_facet
     (Values2 _values_,
      Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_facet_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_coerce_to_facet" that defaults kb-local-only-p to nil
  public void encache_coerce_to_facet
     (Values2 _values_,
      Node thing,
      TruthValue error_p) {
     this.encache_coerce_to_facet(_values_,
                                  thing,
                                  error_p,
                                  _NIL);
  }


  // Method for "encache_coerce_to_facet" that defaults error-p to t
  public void encache_coerce_to_facet
     (Values2 _values_,
      Node thing) {
     this.encache_coerce_to_facet(_values_,
                                  thing,
                                  _T);
  }


// ---------------------- coerce-to-frame

/**
 * Coerces   thing  to be a frame object, if such an object exists for
 * the underlying KRS, or a frame handle otherwise.    Thing  can be a
 * frame object or a frame handle.  This operation may be
 * less careful than   get-frame-in-kb  about ensuring that the
 * frame for   thing  is actually in   kb  when the supplied
 *   thing  is a frame object.    Coerce-to-frame  verifies that
 *   thing  is the
 * appropriate    type  of frame object for   kb , but may not
 * actually determine whether the frame resides in   kb .  Therefore, this
 * operation may be faster than   get-frame-in-kb  for some KRSs.
 * 
 * For user convenience, implementors are encouraged to support the coercion
 * into a frame of any data-structure that uniquely identifies a frame in
 * the KRS as well as frame handles and frame objects.  It is not
 * portable to provide any value for   thing  other than a frame
 * object or frame handle;   get-frames-matching  should be used instead.
 * 
 * If the    :frame-names-required  behavior has the value   for
 *   kb , the names of frames are always coercible to frames.
 * If the    :frame-names-required  behavior is  , frame names
 * (if defined) are not guaranteed to be coercible to frames.
 * 
 * This operation returns two values.
 *  
 *     frame  -- If   thing  identifies a frame for   kb ,
 * then this value is the frame so identified, or
 *   otherwise.
 *     frame-found-p  -- If the frame is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the frame is not found, then a
 *   not-coercible-to-frame  error is signaled.
 * 
 * It is an error to call   coerce-to-frame  with   error-p  being
 *  , and with a value of   thing  that does not uniquely identify
 * a frame.  If this happens, a   not-unique-error  error should be
 * signaled.
 * 
 * Note that in some KRS,   may be a valid frame object.  No portable
 * program may assume that a returned value of   for the first
 * (  frame ) returned value implies that   frame-found-p  is
 *  .
 */
  public Values2 coerce_to_frame
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_frame_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(coerce_to_frame_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(coerce_to_frame_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.coerce_to_frame(thing,
                                                 error_p,
                                                 kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.coerce_to_frame(thing,
                                                     error_p,
                                                     kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(coerce_to_frame_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("coerce_to_frame"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(coerce_to_frame_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.coerce_to_frame(thing,
                                      error_p,
                                      kb_local_only_p);
       }
    }


  public Node cached_p_coerce_to_frame
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_frame_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_coerce_to_frame" that defaults kb-local-only-p to nil
  public Node cached_p_coerce_to_frame
     (Node thing,
      TruthValue error_p) {
     return this.cached_p_coerce_to_frame(thing,
                                          error_p,
                                          _NIL);
  }


  // Method for "cached_p_coerce_to_frame" that defaults error-p to t
  public Node cached_p_coerce_to_frame
     (Node thing) {
     return this.cached_p_coerce_to_frame(thing,
                                          _T);
  }


  public void encache_coerce_to_frame
     (Values2 _values_,
      Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_frame_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_coerce_to_frame" that defaults kb-local-only-p to nil
  public void encache_coerce_to_frame
     (Values2 _values_,
      Node thing,
      TruthValue error_p) {
     this.encache_coerce_to_frame(_values_,
                                  thing,
                                  error_p,
                                  _NIL);
  }


  // Method for "encache_coerce_to_frame" that defaults error-p to t
  public void encache_coerce_to_frame
     (Values2 _values_,
      Node thing) {
     this.encache_coerce_to_frame(_values_,
                                  thing,
                                  _T);
  }


// ---------------------- coerce-to-individual

/**
 * Coerces   thing  to an individual.  This operation returns two values.
 *  
 *     individual  -- If   thing  identifies an individual for
 *   kb , then this value is the individual so identified, or
 *   otherwise.
 *     individual-found-p  -- If the individual is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the individual is not found, then a
 *   individual-not-found  error is signaled.
 * 
 * It is an error to call   coerce-to-individual  with   error-p 
 * being  , and with a value of   thing  that does not uniquely
 * identify an individual.  If this happens, a   not-unique-error  error
 * should be signaled.
 * 
 * Note that in most KRS,   is a valid individual.  No portable
 * program may assume that a returned value of   for the first
 * (  individual ) returned value implies that   individual-found-p 
 * is  .
 */
  public Values2 coerce_to_individual
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_individual_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(coerce_to_individual_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(coerce_to_individual_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.coerce_to_individual(thing,
                                                      error_p,
                                                      kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.coerce_to_individual(thing,
                                                          error_p,
                                                          kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(coerce_to_individual_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("coerce_to_individual"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(coerce_to_individual_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.coerce_to_individual(thing,
                                           error_p,
                                           kb_local_only_p);
       }
    }


  public Node cached_p_coerce_to_individual
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_individual_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_coerce_to_individual" that defaults kb-local-only-p to nil
  public Node cached_p_coerce_to_individual
     (Node thing,
      TruthValue error_p) {
     return this.cached_p_coerce_to_individual(thing,
                                               error_p,
                                               _NIL);
  }


  // Method for "cached_p_coerce_to_individual" that defaults error-p to t
  public Node cached_p_coerce_to_individual
     (Node thing) {
     return this.cached_p_coerce_to_individual(thing,
                                               _T);
  }


  public void encache_coerce_to_individual
     (Values2 _values_,
      Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_individual_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_coerce_to_individual" that defaults kb-local-only-p to nil
  public void encache_coerce_to_individual
     (Values2 _values_,
      Node thing,
      TruthValue error_p) {
     this.encache_coerce_to_individual(_values_,
                                       thing,
                                       error_p,
                                       _NIL);
  }


  // Method for "encache_coerce_to_individual" that defaults error-p to t
  public void encache_coerce_to_individual
     (Values2 _values_,
      Node thing) {
     this.encache_coerce_to_individual(_values_,
                                       thing,
                                       _T);
  }


// ---------------------- coerce-to-kb-value

/**
 * This operation is called by applications that receive input, often from a
 * user in the form of typed input, or a value.    Coerce-to-kb-value 
 * takes this input and delivers a value that is meaningful to the KRS.  This
 * allows applications to interact with users and prompt for expressions
 * containing frame references in a manner that will work predictably across
 * implementation languages, and in networked implementations.
 *   Coerce-to-kb-value  implements OKBC's reading model just as
 *   value-as-string  implements OKBC's printing model.
 * 
 *   string-or-value  may be one of the following.
 *  
 *   an arbitrary OKBC value entity -- If this is a list, then the
 * coercion process applies recursively to the elements of the list.
 * For example, if in the KB the symbol    fred  is coercible to the
 * frame     <frame FRED 763736> , the value    (a fred 42) 
 * would be coerced to the KB value
 *    (a  <frame FRED 763736> 42) .
 *   a string -- This must be the printed representation of an OKBC entity,
 * possibly containing wildcards.  For example, the string
 *    "(a fred 42)"  would be coerced to the same KB value as in
 * the example above.
 *  
 * 
 * Note that there is an asymmetry in the way that arguments are handled,
 * in that a string as the value of   string-or-value  will   always 
 * be interpreted as a pattern from which to read a value, i.e. it will not
 * be interpreted as an OKBC value entity.  This is useful in coercing strings
 * into KB values.  However, this means that applications that use
 *   coerce-to-kb-value  as a way to read in values entered by the user
 * must provide string quotes in order to get a string-valued result.  Thus,
 * to get the string "fred" returned by   coerce-to-kb-value  the
 * value of the   string-or-value  argument would have to be
 * " fred ".  This problem arises if a user interface application
 * is using a prompting model (such as the :string presentation type in CLIM)
 * that elicits a string from the user, perhaps because it already knows that
 * the value-type has to be a string.  If it does this, it will have to
 * quote the string before it passes it to   coerce-to-kb-value  or, of
 * course, not pass it to   coerce-to-kb-value  at all.
 * 
 * Given a   string-or-value  and a   target-context , returns three
 * values.
 *   
 *     result-of-read  -- the result of reading from the string or
 * value, interpreting objects in the   target-context  for the
 *   kb 
 *     success-p  --   if an error occurred during the
 * coercion, and   otherwise
 *     completions-alist  -- an association list of possible
 * completions
 *  
 * The first value returned (  result-of-read ) will be an entity such
 * as a string, number, symbol, list (possibly containing other such values),
 * or frame.
 * 
 *   Target-context  is one of     :frame ,    :class ,
 *    :slot ,    :individual ,    :facet ,    :value   and
 * identifies the way in which the value to be extracted from
 *   string-or-value  is to be interpreted.
 *  
 *      :frame  -- It is to be used as a   frame  argument to
 * an OKBC operation.  It will be expected to resolve to a frame.
 *      :slot  -- It is to be used as a   slot  argument to
 * an OKBC operation.  It will be expected to resolve to a slot name if slots
 * are not represented as frames in   kb , or to a slot frame if slots
 * are represented as frames.
 *      :facet  -- It is to be used as a   facet  argument to
 * an OKBC operation.  It will be expected to resolve to a facet name if facets
 * are not represented as frames in   kb , or to a facet frame if facets
 * are represented as frames.
 *      :class  -- It is to be used as a   class  argument to
 * an OKBC operation.  It will be expected to resolve to a class name if classes
 * are not represented as frames in   kb , or to a class frame if classes
 * are represented as frames.
 *      :individual  -- It is to be used as an   individual 
 * argument to an OKBC operation.  It will be expected to resolve to an
 * individual, which may or may not be a frame.
 *      :value  -- it is to be used as a   value  argument to
 * an OKBC operation, such as   put-slot-value .
 *  
 * The   frame-action  argument
 * controls how the reading process interprets entities that can be interpreted
 * as frames.  The   result-of-read  value is   if an
 * error occurs.  The third value returned (  completions-alist ) is
 *   if an error occurs, or otherwise is an association list of
 * the form
 *   
 * ((<<string1>> <<substring1>> <<frame1>> <<frame2>>... <<frameN>>)
 * (<<string2>> ....) ...)
 *  
 * where    <<stringX>>  are strings found in   string-or-value 
 * that match the frames   <<frame1>> ... <<frameN>> 
 * (possibly by using any specified wildcards), and   <<substringX>> 
 * are the corresponding
 * longest matching initial substrings for each   <<stringX>>  (see the
 * specification of   get-frames-matching ).
 *  
 *     Wildcards-allowed-p 
 * --- has the same meaning as in   get-frames-matching .  Wildcards are
 * interpreted piecewise for the components extracted from
 *   string-or-value .  Thus,    "(fr* j*)"  and
 *    ("fr*" "j*")  both denote a list
 * expression with two wildcarded components, and would match
 *   (fred joe) .
 *     Force-Case-Insensitive-P 
 * --- when   causes frame identification comparison to be
 * case-insensitive, irrespective of the preferred case sensitivity of the
 *   KB  itself.
 *     Error-p 
 * --- when   will signal a   kb-value-read-error  error if a
 * problem arises during the reading process, for example, due to
 * mismatched parentheses.
 *    Frame-action 
 * --- is a keyword from the following list:
 *  
 *      :error-if-not-unique 
 * --- If any substring is found that matches more than one frame then
 * signal a   not-unique-error  error.
 *      :do-not-coerce-to-frames 
 * --- Substrings of   string-or-value  (if a string), or strings
 * and symbols in   string-or-value  (if a nonstring) that
 * match frames are not converted into frames, but may be mapped
 * into strings or symbols.
 *      :must-name-frames 
 * --- Any symbol or string value must be coercible to a frame.  If it
 * is not, a   not-coercible-to-frame  error is signaled.
 *      :options-if-not-unique 
 * --- For each ambiguous frame reference in   string-or-value ,
 * give the possible matches in an entry in the
 *   completions-alist  returned value.
 *  
 *  
 * For example, if in a KB there are frames called    "FRED" ,
 *    "FREDDY" , and    "FRESH"  and the call
 *   
 * (coerce-to-kb-value "fr*" :frame-action :options-if-not-unique)
 *  
 * is made, the values returned would be
 *   
 *      --- The coercion could not complete because of
 * the ambiguity.
 *     --- The operation completed without error.
 *      (("FR*" "FRE" FRED FREDDY FRESH))  --- Only one ambiguous
 * reference was found, and for that the longest matching substring for
 * the pattern    "FR*"  is    "FRE" , and the matching frames
 * are  FRED, FREDDY, FRESH .
 *  
 * See also   get-frames-matching , which is called to identify frames.
 */
  public Values3 coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p,
      Node frame_action,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_kb_value_fname,
                       this,
                       string_or_value,
                       target_context,
                       wildcards_allowed_p,
                       force_case_insensitive_p,
                       error_p,
                       frame_action,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(coerce_to_kb_value_fname, key, trie_result);
             return ((Values3)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(coerce_to_kb_value_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.coerce_to_kb_value(string_or_value,
                                                    target_context,
                                                    wildcards_allowed_p,
                                                    force_case_insensitive_p,
                                                    error_p,
                                                    frame_action,
                                                    kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.coerce_to_kb_value(string_or_value,
                                                        target_context,
                                                        wildcards_allowed_p,
                                                        force_case_insensitive_p,
                                                        error_p,
                                                        frame_action,
                                                        kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(coerce_to_kb_value_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("coerce_to_kb_value"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values3)new_result;
                  }
                else
                  { Values3 new_result = 
                        Values.toValues3(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(coerce_to_kb_value_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.coerce_to_kb_value(string_or_value,
                                         target_context,
                                         wildcards_allowed_p,
                                         force_case_insensitive_p,
                                         error_p,
                                         frame_action,
                                         kb_local_only_p);
       }
    }


  public Node cached_p_coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p,
      Node frame_action,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_kb_value_fname,
                       this,
                       string_or_value,
                       target_context,
                       wildcards_allowed_p,
                       force_case_insensitive_p,
                       error_p,
                       frame_action,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_coerce_to_kb_value" that defaults kb-local-only-p to nil
  public Node cached_p_coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p,
      Node frame_action) {
     return this.cached_p_coerce_to_kb_value(string_or_value,
                                             target_context,
                                             wildcards_allowed_p,
                                             force_case_insensitive_p,
                                             error_p,
                                             frame_action,
                                             _NIL);
  }


  // Method for "cached_p_coerce_to_kb_value" that defaults frame-action to error-if-not-unique
  public Node cached_p_coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p) {
     return this.cached_p_coerce_to_kb_value(string_or_value,
                                             target_context,
                                             wildcards_allowed_p,
                                             force_case_insensitive_p,
                                             error_p,
                                             _error_if_not_unique);
  }


  // Method for "cached_p_coerce_to_kb_value" that defaults error-p to t
  public Node cached_p_coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p) {
     return this.cached_p_coerce_to_kb_value(string_or_value,
                                             target_context,
                                             wildcards_allowed_p,
                                             force_case_insensitive_p,
                                             _T);
  }


  // Method for "cached_p_coerce_to_kb_value" that defaults force-case-insensitive-p to nil
  public Node cached_p_coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p) {
     return this.cached_p_coerce_to_kb_value(string_or_value,
                                             target_context,
                                             wildcards_allowed_p,
                                             _NIL);
  }


  // Method for "cached_p_coerce_to_kb_value" that defaults wildcards-allowed-p to nil
  public Node cached_p_coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context) {
     return this.cached_p_coerce_to_kb_value(string_or_value,
                                             target_context,
                                             _NIL);
  }


  public void encache_coerce_to_kb_value
     (Values3 _values_,
      Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p,
      Node frame_action,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_kb_value_fname,
                       this,
                       string_or_value,
                       target_context,
                       wildcards_allowed_p,
                       force_case_insensitive_p,
                       error_p,
                       frame_action,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_coerce_to_kb_value" that defaults kb-local-only-p to nil
  public void encache_coerce_to_kb_value
     (Values3 _values_,
      Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p,
      Node frame_action) {
     this.encache_coerce_to_kb_value(_values_,
                                     string_or_value,
                                     target_context,
                                     wildcards_allowed_p,
                                     force_case_insensitive_p,
                                     error_p,
                                     frame_action,
                                     _NIL);
  }


  // Method for "encache_coerce_to_kb_value" that defaults frame-action to error-if-not-unique
  public void encache_coerce_to_kb_value
     (Values3 _values_,
      Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p) {
     this.encache_coerce_to_kb_value(_values_,
                                     string_or_value,
                                     target_context,
                                     wildcards_allowed_p,
                                     force_case_insensitive_p,
                                     error_p,
                                     _error_if_not_unique);
  }


  // Method for "encache_coerce_to_kb_value" that defaults error-p to t
  public void encache_coerce_to_kb_value
     (Values3 _values_,
      Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p) {
     this.encache_coerce_to_kb_value(_values_,
                                     string_or_value,
                                     target_context,
                                     wildcards_allowed_p,
                                     force_case_insensitive_p,
                                     _T);
  }


  // Method for "encache_coerce_to_kb_value" that defaults force-case-insensitive-p to nil
  public void encache_coerce_to_kb_value
     (Values3 _values_,
      Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p) {
     this.encache_coerce_to_kb_value(_values_,
                                     string_or_value,
                                     target_context,
                                     wildcards_allowed_p,
                                     _NIL);
  }


  // Method for "encache_coerce_to_kb_value" that defaults wildcards-allowed-p to nil
  public void encache_coerce_to_kb_value
     (Values3 _values_,
      Node string_or_value,
      ContextName target_context) {
     this.encache_coerce_to_kb_value(_values_,
                                     string_or_value,
                                     target_context,
                                     _NIL);
  }


// ---------------------- coerce-to-slot

/**
 * Coerces   thing  to a slot.  This operation returns two values.
 *  
 *     slot  -- If   thing  identifies a slot for   kb ,
 * then this value is the slot so identified, or
 *   otherwise.
 *     slot-found-p  -- If the slot is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the slot is not found, then a
 *   slot-not-found  error is signaled.
 * 
 * It is an error to call   coerce-to-slot  with   error-p  being
 *  , and with a value of   thing  that does not uniquely identify
 * a slot.  If this happens, a   not-unique-error  error should be
 * signaled.
 * 
 * Note that in some KRS,   may be a valid slot.  No portable
 * program may assume that a returned value of   for the first
 * (  slot ) returned value implies that   slot-found-p  is  .
 */
  public Values2 coerce_to_slot
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_slot_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(coerce_to_slot_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(coerce_to_slot_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.coerce_to_slot(thing,
                                                error_p,
                                                kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.coerce_to_slot(thing,
                                                    error_p,
                                                    kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(coerce_to_slot_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("coerce_to_slot"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(coerce_to_slot_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.coerce_to_slot(thing,
                                     error_p,
                                     kb_local_only_p);
       }
    }


  public Node cached_p_coerce_to_slot
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_slot_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_coerce_to_slot" that defaults kb-local-only-p to nil
  public Node cached_p_coerce_to_slot
     (Node thing,
      TruthValue error_p) {
     return this.cached_p_coerce_to_slot(thing,
                                         error_p,
                                         _NIL);
  }


  // Method for "cached_p_coerce_to_slot" that defaults error-p to t
  public Node cached_p_coerce_to_slot
     (Node thing) {
     return this.cached_p_coerce_to_slot(thing,
                                         _T);
  }


  public void encache_coerce_to_slot
     (Values2 _values_,
      Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coerce_to_slot_fname,
                       this,
                       thing,
                       error_p,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_coerce_to_slot" that defaults kb-local-only-p to nil
  public void encache_coerce_to_slot
     (Values2 _values_,
      Node thing,
      TruthValue error_p) {
     this.encache_coerce_to_slot(_values_,
                                 thing,
                                 error_p,
                                 _NIL);
  }


  // Method for "encache_coerce_to_slot" that defaults error-p to t
  public void encache_coerce_to_slot
     (Values2 _values_,
      Node thing) {
     this.encache_coerce_to_slot(_values_,
                                 thing,
                                 _T);
  }


// ---------------------- coercible-to-frame-p

/**
 * Returns   when   thing  can be coerced to a frame
 * by using   coerce-to-frame , and otherwise returns  .
 */
  public Node coercible_to_frame_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coercible_to_frame_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(coercible_to_frame_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(coercible_to_frame_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.coercible_to_frame_p(thing,
                                                      kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.coercible_to_frame_p(thing,
                                                          kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(coercible_to_frame_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("coercible_to_frame_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(coercible_to_frame_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.coercible_to_frame_p(thing,
                                           kb_local_only_p);
       }
    }


  public Node cached_p_coercible_to_frame_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coercible_to_frame_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_coercible_to_frame_p" that defaults kb-local-only-p to nil
  public Node cached_p_coercible_to_frame_p
     (Node thing) {
     return this.cached_p_coercible_to_frame_p(thing,
                                               _NIL);
  }


  public void encache_coercible_to_frame_p
     (Node _values_,
      Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(coercible_to_frame_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_coercible_to_frame_p" that defaults kb-local-only-p to nil
  public void encache_coercible_to_frame_p
     (Node _values_,
      Node thing) {
     this.encache_coercible_to_frame_p(_values_,
                                       thing,
                                       _NIL);
  }


// ---------------------- create-class

/**
 * Creates a class called   name  as a direct subclass
 * of the list of classes (or class)   direct-superclasses .  For KRSs
 * that support the distinction between primitive and nonprimitive concepts,
 *   primitive-p  specifies the primitiveness of the created class.
 * The parameters   doc ,   template-slots ,
 *   template-facets ,   own-slots ,   own-facets ,
 *   direct-types ,   handle , and   pretty-name  have the
 * same meaning as for   create-frame .  For KRSs that support metaclasses,
 * the   direct-types  argument specifies the type(s) of the class
 * to be created (i.e., metaclasses).  Returns the   new-class .
 */
  public Node create_class
     (Node name,
      Node direct_types,
      Node direct_superclasses,
      Node primitive_p,
      Node doc,
      Node template_slots,
      Node template_facets,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("create_class");
         flush_cache("create_class"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.create_class(name,
                                     direct_types,
                                     direct_superclasses,
                                     primitive_p,
                                     doc,
                                     template_slots,
                                     template_facets,
                                     own_slots,
                                     own_facets,
                                     handle,
                                     pretty_name,
                                     kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("create_class"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- create-facet

/**
 * Creates a facet called   name  on    slot-or-nil  that is
 * associated with   frame-or-nil .  If   frame-or-nil  is  ,
 * the facet's frame domain is unconstrained (i.e., the facet may apply to
 *   slot-or-nil  in any frame).
 * 
 * If   slot-or-nil  is  , the slot domain of the facet is
 * unconstrained (i.e., the facet may apply to all slots in
 *   frame-or-nil , and if   frame-or-nil  is also  , may
 * apply to all slots in all frames.)
 * If    :facet  is a member of the behavior values for the
 *    :are-frames  behavior,   direct-types ,
 *   doc ,   own-slots ,   own-facets ,   handle  and
 *   pretty-name  have the same interpretation as for   create-frame .
 * If either   frame-or-nil  or   slot-or-nil  is  ,
 *   slot-type  is ignored.  If either of the   frame-or-nil  or
 *   slot-or-nil  arguments is
 *  , and the KRS does not support facets with unconstrained domains,
 * a   domain-required  error will be signaled.
 * If facets must be uniquely named and a facet named   name  already
 * exists, a   facet-already-exists  error will be signalled.
 * Returns the   new-facet .
 */
  public Node create_facet
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("create_facet");
         flush_cache("create_facet"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.create_facet(name,
                                     frame_or_nil,
                                     slot_or_nil,
                                     slot_type,
                                     direct_types,
                                     doc,
                                     own_slots,
                                     own_facets,
                                     handle,
                                     pretty_name,
                                     kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("create_facet"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- create-frame

/**
 * Creates a new frame called   name  of type   frame-type .
 *   Frame-type  is one of     :class ,    :slot ,    :facet ,
 *    :individual  .  A call to   create-frame  is equivalent to
 * a call to one of   create-class ,   create-individual ,
 *   create-slot , or   create-facet  passing through the appropriate
 * arguments, depending on the value of   frame-type .  If
 *   frame-type  is either    :slot  or    :facet , the slot
 * (or facet) created will have unconstrained domains.
 * 
 * If the    :frame-names-required  behavior has the value   for
 *   kb ,   new-name  may be  .  If the
 *    :frame-names-required  behavior is   for   kb ,
 *   new-name  must uniquely name the new frame, and a
 *   frame-already-exists  error will be signaled if   new-name 
 * is coercible to an existing frame.
 * 
 *   Direct-types  is a list of classes (or class) of which this new
 * frame is to be a direct instance.
 *   Direct-superclasses  is a list of classes (or class) of which the
 * new frame is to be a direct subclass.
 *   Doc , if specified, is a string documenting the new frame.
 *   Pretty-name  is the pretty-name of the new frame.  Returns
 *   new-frame , which identifies the newly created frame.
 * 
 *   Template-slots  and   own-slots  each take a list of slot
 * specifications.  A slot specification assigns a set of values to a
 * slot.  The syntax of a slot specification is
 *   
 * slot-spec ::= (slot slot-value-spec*)
 * slot-value-spec ::= default-slot-value   slot-value
 * default-slot-value ::= (:default slot-value)
 *  
 * where    slot  identifies a slot, or names a slot to be created.  If
 *    slot  already exists, it is simply attached to the new frame, if
 * it does not currently exist, it is created and attached to the new frame.
 * Each    slot-value  is an entity suitable as a value of
 * the specified slot.  Default slot values are identified by appearing in
 * a list whose first element is    :default .  Template slots are only
 * allowed for class frames -- that is, when   frame-type  is
 *    :class .
 * 
 *   Template-facets  and   own-facets  each take a list of facet
 * specifications, which can assign a set of facet values.  A facet
 * specification has the form:
 *   
 * facet-spec ::= (slot fspec*)
 * fspec ::= (facet facet-value-spec*)
 * facet-value-spec ::= default-facet-value   facet-value
 * default-facet-value ::= (:default facet-value)
 *  
 * where    slot  identifies a slot, or names a slot to be created.  If
 *    slot  already exists, it is simply attached to the new frame, if
 * it does not currently exist, it is created and attached to the new frame.
 *    Facet  identifies a facet, or names a facet to be created.  If
 *    facet  already exists, it is simply attached to   slot  on
 * the new frame, if it does not currently exist, it is created and attached
 * to   slot  on the new frame.  Each    facet-value  is an object
 * suitable as a value of the specified facet.  Default facet values are
 * identified by appearing in a list whose first element is    :default .
 * Template facets are allowed only for class frames -- that is, when
 *   frame-type  is    :class .
 * 
 * All slot and facet names in slot and facet specs are defined in a unified
 * namespace that operates across all of the    :own-slots ,
 *    :own-facets ,    :template-slots , and    :template-facets 
 * arguments.  Thus, in the following  example, all occurrences of the slot
 *    s1  and the facet    f1  denote the same slot and facet
 * respectively.
 * 
 * The values specified in slot and facet specifications are interpreted
 * conjunctively.  Thus, in the following example, the slot
 *    s1  will have three values; 42, 100 and 2001, rather than just
 * the value 2001.
 *   
 * (create-frame 'foo :class
 * :own-slots '((s1 42 100)
 * (s1 2001))
 * :own-facets '((s1 (:value-type :integer))
 * (s1 (f1 "Own hello")))
 * :template-facets '((s1 (f1 "Template hello"))))
 *  
 * 
 *   Primitive-p  may be used only when creating a class.
 * When   primitive-p  is  , the KRS will make
 * the class nonprimitive, if possible.
 * 
 *   Handle , if supplied, is a previously allocated frame handle for the
 * new frame to be created.  This is used by network applications, and
 * operations such as   copy-frame  and   copy-kb .  (See
 *   allocate-frame-handle .)  It is an error to supply a value for the
 *   handle  argument that is already coercible to a frame.  If this
 * occurs, a   frame-already-exists  error should be signaled.
 * 
 * Note that if   frame-type  is either    :slot  or    :facet ,
 * then a    frame  might not be created because slots (or facets) might not
 * be represented as frames in   kb .  If this is the case, and slots
 * (or facets) with unconstrained domains are not supported, a
 *   domain-required  error will be signaled.
 * 
 * It is an error to supply   own-slots ,   own-facets  if a frame
 * will not be created, according to the    :are-frames  behavior, and a
 *   not-a-frame-type  error should be signaled.
 */
  public Node create_frame
     (Node name,
      ContextName frame_type,
      Node direct_types,
      Node direct_superclasses,
      Node doc,
      Node template_slots,
      Node template_facets,
      Node own_slots,
      Node own_facets,
      Node primitive_p,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("create_frame");
         flush_cache("create_frame"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.create_frame(name,
                                     frame_type,
                                     direct_types,
                                     direct_superclasses,
                                     doc,
                                     template_slots,
                                     template_facets,
                                     own_slots,
                                     own_facets,
                                     primitive_p,
                                     handle,
                                     pretty_name,
                                     kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("create_frame"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- create-individual

/**
 * Creates an individual called   name .  The one or more classes
 * that are the direct types of the instance are given by
 *   direct-types .
 * The parameters   doc ,   own-slots ,   own-facets ,
 *   handle , and   pretty-name  all have the same meaning as for
 *   create-frame .  Returns   new-individual , which identifies the
 * new frame.
 */
  public Node create_individual
     (Node name,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("create_individual");
         flush_cache("create_individual"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.create_individual(name,
                                          direct_types,
                                          doc,
                                          own_slots,
                                          own_facets,
                                          handle,
                                          pretty_name,
                                          kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("create_individual"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- create-procedure

/**
 * Defines and returns a procedure in the OKBC procedure language.  The
 * arguments are defined as follows:
 *  
 *     arguments  -- the argument list for the procedure.  The
 * argument list can be expressed in one of three forms.
 *   
 *   A list of symbols
 *   A string that is the printed representation of a list
 * of symbols
 *     -- the null argument list
 *  
 * For example, the argument lists    (a b c) , and
 *    "(a b c)"  are equivalent, as are    "()"  and  .
 * The string representation is provided for language bindings in
 * which it may be inconvenient to create lists of symbols.
 *     body  -- The body for the procedure expressed in the syntax
 * defined in section  .  The body can be
 * provided in one of two forms:
 *   
 *   A    body-form 
 *   A list of    body-form s
 *   A string that is the printed representation of a sequence
 * of    body-form s
 *  
 * For example, the following procedure bodies are equivalent:
 *   
 * ((put-slot-values frame slot values :slot-type :own)
 * (get-slot-value frame slot :slot-type :own))
 *  
 * and
 *   
 * "(put-slot-values frame slot values :slot-type :own)
 * (get-slot-value frame slot :slot-type :own)"
 *  
 * The string representation is provided for language bindings in
 * which it may be inconvenient to create the complex list structure
 * required in the procedure language.
 *     environment  -- A predefined set of bindings between variables
 * mentioned in the procedure   body  and their associated values.
 * The environment is a list of 2-tuples of the form
 *   
 * ((var1 value1)
 * (var2 value2)
 * ....
 * (varn valuen))
 *  
 * where    varN  are the variables mentioned in   body , and
 *    valueN  are the associated values for the variables.
 *  
 * A procedure is a legal argument to any OKBC operator in a position that
 * expects a procedure.
 * For example,
 *   
 * (call-procedure
 * #'(lambda (frame) (get-frame-pretty-name frame :kb kb))
 * :kb kb :arguments (list my-frame))
 *  
 * and
 *   
 * (call-procedure
 * (create-procedure :arguments '(frame)
 * :body '(get-frame-pretty-name frame :kb kb))
 * :kb my-kb :arguments (list my-frame))
 *  
 * are semantically identical.
 * 
 * The main differences between
 * procedures and lambda expressions in Lisp are as follows:
 *  
 *   All bindings in procedures are dynamic, not lexical.
 *   Only a restricted set of operations is available in procedures.
 *   Lambda defines a    lexical  closure over any free references.
 *   procedure  defines a    dynamic  closure over its free
 * references.  The environment of the procedure is
 * prefilled with bindings for the names of the arguments to
 * the OKBC operator in which it is being executed.  In the above
 * case,   call-procedure  takes arguments   KB ,
 *   Arguments , and   Kb-local-only-p 
 * which will take on the values   my-kb ,   (my-frame) , and
 *   nil  (the default), respectively.
 *   Lambda expressions are meaningful only within the Lisp system
 * in which the OKBC system is running.  procedures are
 * executable on any (possibly network-connected) OKBC KB.
 *   procedures are package-insensitive in all respects
 * other than quoted constants.
 *  
 * 
 * Note that persistent side effects to   <<var1>>  and   <<var2>> 
 * cannot be made from within the procedure.  The arguments and variables
 * mentioned in the procedure exist in a different space from the variables
 * in a user program.  The only ways to establish associations between values
 * in a user program and variables in a procedure are through the use of the
 *   environment  argument to   create-procedure , or by the
 *   arguments  argument to   call-procedure .
 */
  public Node create_procedure
     (Node arguments,
      Node body,
      Node environment) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(create_procedure_fname,
                       this,
                       arguments,
                       body,
                       environment);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(create_procedure_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(create_procedure_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.create_procedure(arguments,
                                                  body,
                                                  environment);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.create_procedure(arguments,
                                                      body,
                                                      environment);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(create_procedure_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("create_procedure"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(create_procedure_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.create_procedure(arguments,
                                       body,
                                       environment);
       }
    }


  public Node cached_p_create_procedure
     (Node arguments,
      Node body,
      Node environment) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(create_procedure_fname,
                       this,
                       arguments,
                       body,
                       environment);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_create_procedure" that defaults environment to nil
  public Node cached_p_create_procedure
     (Node arguments,
      Node body) {
     return this.cached_p_create_procedure(arguments,
                                           body,
                                           _NIL);
  }


  // Method for "cached_p_create_procedure" that defaults body to nil
  public Node cached_p_create_procedure
     (Node arguments) {
     return this.cached_p_create_procedure(arguments,
                                           _NIL);
  }


  // Method for "cached_p_create_procedure" that defaults arguments to nil
  public Node cached_p_create_procedure
     () {
     return this.cached_p_create_procedure(_NIL);
  }


  public void encache_create_procedure
     (Node _values_,
      Node arguments,
      Node body,
      Node environment) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(create_procedure_fname,
                       this,
                       arguments,
                       body,
                       environment);
         encache(_values_, key);
       }
  }

  // Method for "encache_create_procedure" that defaults environment to nil
  public void encache_create_procedure
     (Node _values_,
      Node arguments,
      Node body) {
     this.encache_create_procedure(_values_,
                                   arguments,
                                   body,
                                   _NIL);
  }


  // Method for "encache_create_procedure" that defaults body to nil
  public void encache_create_procedure
     (Node _values_,
      Node arguments) {
     this.encache_create_procedure(_values_,
                                   arguments,
                                   _NIL);
  }


  // Method for "encache_create_procedure" that defaults arguments to nil
  public void encache_create_procedure
     (Node _values_) {
     this.encache_create_procedure(_values_,
                                   _NIL);
  }


// ---------------------- create-slot

/**
 * Creates a slot called   name  in the frame specified by
 *   frame-or-nil .  Returns the   new-slot .  If the slot to be
 * created is to be represented as a frame (   :slot  is a member of the
 *    :are-frames  behavior),   direct-types ,
 *   doc ,   own-slots ,   own-facets ,   handle , and
 *   pretty-name  have the same interpretation as for   create-frame .
 * If   frame-or-nil  is  ,   slot-type  is ignored, and the
 * slot's domain is ignored.  If the
 *   frame  argument is  , and the KRS does not support slots with
 * unconstrained domains, a   domain-required  error will be signaled.
 * If slots must be uniquely named and a slot named   name  already
 * exists, a   slot-already-exists  error will be signalled.
 */
  public Node create_slot
     (Node name,
      Node frame_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("create_slot");
         flush_cache("create_slot"); }
     try { connection().ok_to_cache_flag = _testing;
           return super.create_slot(name,
                                    frame_or_nil,
                                    slot_type,
                                    direct_types,
                                    doc,
                                    own_slots,
                                    own_facets,
                                    handle,
                                    pretty_name,
                                    kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("create_slot"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- decontextualize

/**
 * Given a value from   kb , returns a
 *   decontextualized-value , which contains no KB or KRS-specific
 * data structures.  In particular,
 * any references to frame objects will be replaced with KRS-independent frame
 * handles (produced using   frs-independent-frame-handle ), and all
 * values outside the standard set of OKBC data types that have
 * no interpretation outside   kb  will be
 * replaced with remote-value references.  Any frame references that are
 * the result of an KRS-specific mapping of a canonically named frame will be
 * replaced with the canonical name.  Thus, for example, a facet
 * frame called    cardinality-of-slot  would be mapped back to a frame
 * handle for the canonical facet-reference    :cardinality .
 * 
 *   From-context  is one of     :frame ,    :slot ,
 *    :facet ,    :value  .  It identifies the context of the argument
 * to be decontextualized.  For example, if the
 * decontextualization is to be applied to a slot value, then
 *   from-context  should be    :value .  If the
 * decontextualization is to be applied to a slot (i.e., something that would
 * be used as a   slot  argument to an operation such as
 *   get-slot-values ), then   from-context  should be    :slot .
 * 
 * It is not anticipated that this operation will be called by OKBC
 * applications, but rather by OKBC back end implementations.  It is used to
 * ensure correct operation in networked applications and during communication
 * between KBs of different kb-types.
 */
  public Node decontextualize
     (Node value,
      ContextName from_context) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(decontextualize_fname,
                       this,
                       value,
                       from_context);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(decontextualize_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(decontextualize_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.decontextualize(value,
                                                 from_context);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.decontextualize(value,
                                                     from_context);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(decontextualize_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("decontextualize"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(decontextualize_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.decontextualize(value,
                                      from_context);
       }
    }


  public Node cached_p_decontextualize
     (Node value,
      ContextName from_context) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(decontextualize_fname,
                       this,
                       value,
                       from_context);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  public void encache_decontextualize
     (Node _values_,
      Node value,
      ContextName from_context) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(decontextualize_fname,
                       this,
                       value,
                       from_context);
         encache(_values_, key);
       }
  }

// ---------------------- delete-facet

/**
 * Deletes the facet from all frames containing that facet, and the facet frame
 * itself if the facet is represented as a frame. As a result of
 *   delete-facet ,   facet  will return   for calls to
 *   facet-p , and   facet  is not returned by any of the
 * facet-returning operations, such as   get-kb-facets  and
 *   get-slot-facets .  It is no longer possible to access any values of
 *   facet .  Returns no values.
 * 
 * Many implementations may, in fact, delete the values associated the
 * facet in frames as well as making the facet no longer facet-p.  Other
 * implementations will simply make these values inaccessible.
 */
  public void delete_facet
     (Node facet,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("delete_facet");
         flush_cache("delete_facet"); }
     try { connection().ok_to_cache_flag = _testing;
           super.delete_facet(facet,
                              kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("delete_facet"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- delete-frame

/**
 * Deleting a frame from a KB is difficult to specify in a portable way.
 * After calling   delete-frame , the   frame  argument
 * will no longer be a valid frame reference (  frame-p  will return
 *  ).  As a consequence, the value of   frame  will
 * not be a valid argument to any OKBC operation requiring a frame reference,
 * such as   get-frame-slots .  It will no longer be possible to access
 * any of the properties (e.g., slots, facets) of
 *   frame .  Implementations will delete at least the
 * properties documented as being returned by   get-frame-details 
 * from the   kb .
 * 
 * Note that after a call to   delete-frame , references
 * to   frame  may still remain in the KB.  Returns no values.
 */
  public void delete_frame
     (Node frame,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("delete_frame");
         flush_cache("delete_frame"); }
     try { connection().ok_to_cache_flag = _testing;
           super.delete_frame(frame,
                              kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("delete_frame"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- delete-slot

/**
 * Deletes the slot from all frames containing that slot, and the slot frame
 * itself if the slot is represented as a frame. As a result of
 *   delete-slot ,   slot  will return   for calls to
 *   slot-p , and   slot  is not returned by any of the
 * slot-returning operations, such as   get-kb-slots  and
 *   get-frame-slots .  It is no longer possible to access any values of
 *   slot  or any facets or facet values on   slot .
 * Returns no values.
 * 
 * Many implementations may, in fact, delete the values associated the
 * slot in frames as well as making the slot no longer slot-p.  Other
 * implementations will simply make these values inaccessible.
 */
  public void delete_slot
     (Node slot,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("delete_slot");
         flush_cache("delete_slot"); }
     try { connection().ok_to_cache_flag = _testing;
           super.delete_slot(slot,
                             kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("delete_slot"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- detach-facet

/**
 * Removes any explicit association between the   facet  and
 *   slot  on   frame .  As a result,   facet  is not returned
 * by   get-slot-facets  at inference-level    :direct  unless there are
 * facet values associated with   facet  in   slot  on
 *   frame .
 */
  public void detach_facet
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("detach_facet");
         flush_cache("detach_facet"); }
     try { connection().ok_to_cache_flag = _testing;
           super.detach_facet(frame,
                              slot,
                              facet,
                              slot_type,
                              kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("detach_facet"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- detach-slot

/**
 * Removes any explicit association between the   slot  and
 *   frame .  As a result,   slot  is not returned by
 *   get-frame-slots  at inference-level    :direct  unless there are
 * slot or facet values associated with   slot  in   frame .
 */
  public void detach_slot
     (Node frame,
      Node slot,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     boolean old_allowed_p = allow_caching_p;
     Node old_ok_p = connection().ok_to_cache_flag;
     if (caching_policy == _defensive)
       { disallow_caching("detach_slot");
         flush_cache("detach_slot"); }
     try { connection().ok_to_cache_flag = _testing;
           super.detach_slot(frame,
                             slot,
                             slot_type,
                             kb_local_only_p);
         }
     finally { if (caching_policy == _defensive)
                 { if (old_allowed_p) { allow_caching("detach_slot"); }
                 }
               connection().ok_to_cache_flag = old_ok_p;
             }
    }


// ---------------------- eql-in-kb

/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * and otherwise returns  .
 *   Arg0  and   arg1  may be any combination of objects coercible
 * to frames.
 */
  public Node eql_in_kb
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(eql_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(eql_in_kb_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(eql_in_kb_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.eql_in_kb(arg0,
                                           arg1,
                                           kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.eql_in_kb(arg0,
                                               arg1,
                                               kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(eql_in_kb_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("eql_in_kb"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(eql_in_kb_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.eql_in_kb(arg0,
                                arg1,
                                kb_local_only_p);
       }
    }


  public Node cached_p_eql_in_kb
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(eql_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_eql_in_kb" that defaults kb-local-only-p to nil
  public Node cached_p_eql_in_kb
     (Node arg0,
      Node arg1) {
     return this.cached_p_eql_in_kb(arg0,
                                    arg1,
                                    _NIL);
  }


  public void encache_eql_in_kb
     (Node _values_,
      Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(eql_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_eql_in_kb" that defaults kb-local-only-p to nil
  public void encache_eql_in_kb
     (Node _values_,
      Node arg0,
      Node arg1) {
     this.encache_eql_in_kb(_values_,
                            arg0,
                            arg1,
                            _NIL);
  }


// ---------------------- equal-in-kb

/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * or they are strings containing the same characters (case sensitively), or
 * both are lists with the same structure, and each of the elements
 * recursively is true according to   equal-in-kb .  Returns   
 * otherwise.
 */
  public Node equal_in_kb
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(equal_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(equal_in_kb_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(equal_in_kb_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.equal_in_kb(arg0,
                                             arg1,
                                             kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.equal_in_kb(arg0,
                                                 arg1,
                                                 kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(equal_in_kb_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("equal_in_kb"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(equal_in_kb_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.equal_in_kb(arg0,
                                  arg1,
                                  kb_local_only_p);
       }
    }


  public Node cached_p_equal_in_kb
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(equal_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_equal_in_kb" that defaults kb-local-only-p to nil
  public Node cached_p_equal_in_kb
     (Node arg0,
      Node arg1) {
     return this.cached_p_equal_in_kb(arg0,
                                      arg1,
                                      _NIL);
  }


  public void encache_equal_in_kb
     (Node _values_,
      Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(equal_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_equal_in_kb" that defaults kb-local-only-p to nil
  public void encache_equal_in_kb
     (Node _values_,
      Node arg0,
      Node arg1) {
     this.encache_equal_in_kb(_values_,
                              arg0,
                              arg1,
                              _NIL);
  }


// ---------------------- equalp-in-kb

/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * or they are strings containing the same characters (case-insensitively), or
 * both are lists with the same structure, and each of the elements
 * recursively is true according to   equalp-in-kb .  Returns
 *   otherwise.
 */
  public Node equalp_in_kb
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(equalp_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(equalp_in_kb_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(equalp_in_kb_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.equalp_in_kb(arg0,
                                              arg1,
                                              kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.equalp_in_kb(arg0,
                                                  arg1,
                                                  kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(equalp_in_kb_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("equalp_in_kb"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(equalp_in_kb_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.equalp_in_kb(arg0,
                                   arg1,
                                   kb_local_only_p);
       }
    }


  public Node cached_p_equalp_in_kb
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(equalp_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_equalp_in_kb" that defaults kb-local-only-p to nil
  public Node cached_p_equalp_in_kb
     (Node arg0,
      Node arg1) {
     return this.cached_p_equalp_in_kb(arg0,
                                       arg1,
                                       _NIL);
  }


  public void encache_equalp_in_kb
     (Node _values_,
      Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(equalp_in_kb_fname,
                       this,
                       arg0,
                       arg1,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_equalp_in_kb" that defaults kb-local-only-p to nil
  public void encache_equalp_in_kb
     (Node _values_,
      Node arg0,
      Node arg1) {
     this.encache_equalp_in_kb(_values_,
                               arg0,
                               arg1,
                               _NIL);
  }


// ---------------------- facet-has-value-p

/**
 * Returns   iff the specified facet has a value for the specified slot
 * and frame, and otherwise returns  .
 */
  public Values2 facet_has_value_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(facet_has_value_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(facet_has_value_p_fname, key, trie_result);
             return ((Values2)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(facet_has_value_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.facet_has_value_p(frame,
                                                   slot,
                                                   facet,
                                                   inference_level,
                                                   slot_type,
                                                   value_selector,
                                                   kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.facet_has_value_p(frame,
                                                       slot,
                                                       facet,
                                                       inference_level,
                                                       slot_type,
                                                       value_selector,
                                                       kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(facet_has_value_p_fname, key,
                                                new_result, true);
                              }
                            else { flush_cache("facet_has_value_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Values2)new_result;
                  }
                else
                  { Values2 new_result = 
                        Values.toValues2(cache_miss_hook_result.firstValue());
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(facet_has_value_p_fname, key,
                                    new_result, true);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.facet_has_value_p(frame,
                                        slot,
                                        facet,
                                        inference_level,
                                        slot_type,
                                        value_selector,
                                        kb_local_only_p);
       }
    }


  public Node cached_p_facet_has_value_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(facet_has_value_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_facet_has_value_p" that defaults kb-local-only-p to nil
  public Node cached_p_facet_has_value_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.cached_p_facet_has_value_p(frame,
                                            slot,
                                            facet,
                                            inference_level,
                                            slot_type,
                                            value_selector,
                                            _NIL);
  }


  // Method for "cached_p_facet_has_value_p" that defaults value-selector to either
  public Node cached_p_facet_has_value_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_facet_has_value_p(frame,
                                            slot,
                                            facet,
                                            inference_level,
                                            slot_type,
                                            _either);
  }


  // Method for "cached_p_facet_has_value_p" that defaults slot-type to own
  public Node cached_p_facet_has_value_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.cached_p_facet_has_value_p(frame,
                                            slot,
                                            facet,
                                            inference_level,
                                            _own);
  }


  // Method for "cached_p_facet_has_value_p" that defaults inference-level to taxonomic
  public Node cached_p_facet_has_value_p
     (Node frame,
      Node slot,
      Node facet) {
     return this.cached_p_facet_has_value_p(frame,
                                            slot,
                                            facet,
                                            _taxonomic);
  }


  public void encache_facet_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(facet_has_value_p_fname,
                       this,
                       frame,
                       slot,
                       facet,
                       inference_level,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_facet_has_value_p" that defaults kb-local-only-p to nil
  public void encache_facet_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.encache_facet_has_value_p(_values_,
                                    frame,
                                    slot,
                                    facet,
                                    inference_level,
                                    slot_type,
                                    value_selector,
                                    _NIL);
  }


  // Method for "encache_facet_has_value_p" that defaults value-selector to either
  public void encache_facet_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_facet_has_value_p(_values_,
                                    frame,
                                    slot,
                                    facet,
                                    inference_level,
                                    slot_type,
                                    _either);
  }


  // Method for "encache_facet_has_value_p" that defaults slot-type to own
  public void encache_facet_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     this.encache_facet_has_value_p(_values_,
                                    frame,
                                    slot,
                                    facet,
                                    inference_level,
                                    _own);
  }


  // Method for "encache_facet_has_value_p" that defaults inference-level to taxonomic
  public void encache_facet_has_value_p
     (Values2 _values_,
      Node frame,
      Node slot,
      Node facet) {
     this.encache_facet_has_value_p(_values_,
                                    frame,
                                    slot,
                                    facet,
                                    _taxonomic);
  }


// ---------------------- facet-p

/**
 * Returns   iff   thing  is a facet, and   otherwise.
 */
  public Node facet_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(facet_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(facet_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(facet_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.facet_p(thing,
                                         kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.facet_p(thing,
                                             kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(facet_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("facet_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(facet_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.facet_p(thing,
                              kb_local_only_p);
       }
    }


  public Node cached_p_facet_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(facet_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_facet_p" that defaults kb-local-only-p to nil
  public Node cached_p_facet_p
     (Node thing) {
     return this.cached_p_facet_p(thing,
                                  _NIL);
  }


  public void encache_facet_p
     (Node _values_,
      Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(facet_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_facet_p" that defaults kb-local-only-p to nil
  public void encache_facet_p
     (Node _values_,
      Node thing) {
     this.encache_facet_p(_values_,
                          thing,
                          _NIL);
  }


// ---------------------- follow-slot-chain

/**
 * Allows a program to traverse a chain of slot references,
 * gathering own slot values.  For example, imagine that we wish to determine
 * the sisters of the father of the mother of John.  The following two calls
 * accomplish this goal:
 *   
 * (follow-slot-chain 'john '(mother father sisters))
 * 
 * (get-slot-values
 * (get-slot-value
 * (get-slot-value 'john 'mother)
 * 'father)
 * 'sisters)
 *  
 * This operation is complicated by the fact that slots can have multiple
 * values.  For example, imagine that John has two mothers---adopted and
 * biological.  If    union-multiple-values  is   and a slot
 * has more than one value, a   cardinality-violation  error is signaled;
 * if  , then the slot chain becomes a tree, and the union of
 * all values found at the leaves of the tree is returned.
 */
  public Node follow_slot_chain
     (Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(follow_slot_chain_fname,
                       this,
                       frame,
                       slot_chain,
                       union_multiple_values,
                       inference_level,
                       value_selector,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(follow_slot_chain_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(follow_slot_chain_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.follow_slot_chain(frame,
                                                   slot_chain,
                                                   union_multiple_values,
                                                   inference_level,
                                                   value_selector,
                                                   kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.follow_slot_chain(frame,
                                                       slot_chain,
                                                       union_multiple_values,
                                                       inference_level,
                                                       value_selector,
                                                       kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(follow_slot_chain_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("follow_slot_chain"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(follow_slot_chain_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.follow_slot_chain(frame,
                                        slot_chain,
                                        union_multiple_values,
                                        inference_level,
                                        value_selector,
                                        kb_local_only_p);
       }
    }


  public Node cached_p_follow_slot_chain
     (Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(follow_slot_chain_fname,
                       this,
                       frame,
                       slot_chain,
                       union_multiple_values,
                       inference_level,
                       value_selector,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_follow_slot_chain" that defaults kb-local-only-p to nil
  public Node cached_p_follow_slot_chain
     (Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level,
      ValueSelector value_selector) {
     return this.cached_p_follow_slot_chain(frame,
                                            slot_chain,
                                            union_multiple_values,
                                            inference_level,
                                            value_selector,
                                            _NIL);
  }


  // Method for "cached_p_follow_slot_chain" that defaults value-selector to either
  public Node cached_p_follow_slot_chain
     (Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level) {
     return this.cached_p_follow_slot_chain(frame,
                                            slot_chain,
                                            union_multiple_values,
                                            inference_level,
                                            _either);
  }


  // Method for "cached_p_follow_slot_chain" that defaults inference-level to taxonomic
  public Node cached_p_follow_slot_chain
     (Node frame,
      Node slot_chain,
      Node union_multiple_values) {
     return this.cached_p_follow_slot_chain(frame,
                                            slot_chain,
                                            union_multiple_values,
                                            _taxonomic);
  }


  // Method for "cached_p_follow_slot_chain" that defaults union-multiple-values to t
  public Node cached_p_follow_slot_chain
     (Node frame,
      Node slot_chain) {
     return this.cached_p_follow_slot_chain(frame,
                                            slot_chain,
                                            _T);
  }


  public void encache_follow_slot_chain
     (Node _values_,
      Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(follow_slot_chain_fname,
                       this,
                       frame,
                       slot_chain,
                       union_multiple_values,
                       inference_level,
                       value_selector,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_follow_slot_chain" that defaults kb-local-only-p to nil
  public void encache_follow_slot_chain
     (Node _values_,
      Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level,
      ValueSelector value_selector) {
     this.encache_follow_slot_chain(_values_,
                                    frame,
                                    slot_chain,
                                    union_multiple_values,
                                    inference_level,
                                    value_selector,
                                    _NIL);
  }


  // Method for "encache_follow_slot_chain" that defaults value-selector to either
  public void encache_follow_slot_chain
     (Node _values_,
      Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level) {
     this.encache_follow_slot_chain(_values_,
                                    frame,
                                    slot_chain,
                                    union_multiple_values,
                                    inference_level,
                                    _either);
  }


  // Method for "encache_follow_slot_chain" that defaults inference-level to taxonomic
  public void encache_follow_slot_chain
     (Node _values_,
      Node frame,
      Node slot_chain,
      Node union_multiple_values) {
     this.encache_follow_slot_chain(_values_,
                                    frame,
                                    slot_chain,
                                    union_multiple_values,
                                    _taxonomic);
  }


  // Method for "encache_follow_slot_chain" that defaults union-multiple-values to t
  public void encache_follow_slot_chain
     (Node _values_,
      Node frame,
      Node slot_chain) {
     this.encache_follow_slot_chain(_values_,
                                    frame,
                                    slot_chain,
                                    _T);
  }


// ---------------------- frame-has-slot-p

/**
 * Returns   iff   slot  is a slot in   frame ,
 * otherwise returns  .
 */
  public Node frame_has_slot_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frame_has_slot_p_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(frame_has_slot_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(frame_has_slot_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.frame_has_slot_p(frame,
                                                  slot,
                                                  inference_level,
                                                  slot_type,
                                                  kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.frame_has_slot_p(frame,
                                                      slot,
                                                      inference_level,
                                                      slot_type,
                                                      kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(frame_has_slot_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("frame_has_slot_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(frame_has_slot_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.frame_has_slot_p(frame,
                                       slot,
                                       inference_level,
                                       slot_type,
                                       kb_local_only_p);
       }
    }


  public Node cached_p_frame_has_slot_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frame_has_slot_p_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_frame_has_slot_p" that defaults kb-local-only-p to nil
  public Node cached_p_frame_has_slot_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.cached_p_frame_has_slot_p(frame,
                                           slot,
                                           inference_level,
                                           slot_type,
                                           _NIL);
  }


  // Method for "cached_p_frame_has_slot_p" that defaults slot-type to own
  public Node cached_p_frame_has_slot_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.cached_p_frame_has_slot_p(frame,
                                           slot,
                                           inference_level,
                                           _own);
  }


  // Method for "cached_p_frame_has_slot_p" that defaults inference-level to taxonomic
  public Node cached_p_frame_has_slot_p
     (Node frame,
      Node slot) {
     return this.cached_p_frame_has_slot_p(frame,
                                           slot,
                                           _taxonomic);
  }


  public void encache_frame_has_slot_p
     (Node _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frame_has_slot_p_fname,
                       this,
                       frame,
                       slot,
                       inference_level,
                       slot_type,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_frame_has_slot_p" that defaults kb-local-only-p to nil
  public void encache_frame_has_slot_p
     (Node _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     this.encache_frame_has_slot_p(_values_,
                                   frame,
                                   slot,
                                   inference_level,
                                   slot_type,
                                   _NIL);
  }


  // Method for "encache_frame_has_slot_p" that defaults slot-type to own
  public void encache_frame_has_slot_p
     (Node _values_,
      Node frame,
      Node slot,
      InferenceLevel inference_level) {
     this.encache_frame_has_slot_p(_values_,
                                   frame,
                                   slot,
                                   inference_level,
                                   _own);
  }


  // Method for "encache_frame_has_slot_p" that defaults inference-level to taxonomic
  public void encache_frame_has_slot_p
     (Node _values_,
      Node frame,
      Node slot) {
     this.encache_frame_has_slot_p(_values_,
                                   frame,
                                   slot,
                                   _taxonomic);
  }


// ---------------------- frame-in-kb-p

/**
 * Returns   when   thing  is both coercible to
 * a frame, and that frame is known to be resident in   kb , and otherwise
 * returns  .  See   get-frame-in-kb .
 */
  public Node frame_in_kb_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frame_in_kb_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(frame_in_kb_p_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(frame_in_kb_p_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.frame_in_kb_p(thing,
                                               kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.frame_in_kb_p(thing,
                                                   kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(frame_in_kb_p_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("frame_in_kb_p"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(frame_in_kb_p_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.frame_in_kb_p(thing,
                                    kb_local_only_p);
       }
    }


  public Node cached_p_frame_in_kb_p
     (Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frame_in_kb_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_frame_in_kb_p" that defaults kb-local-only-p to nil
  public Node cached_p_frame_in_kb_p
     (Node thing) {
     return this.cached_p_frame_in_kb_p(thing,
                                        _NIL);
  }


  public void encache_frame_in_kb_p
     (Node _values_,
      Node thing,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frame_in_kb_p_fname,
                       this,
                       thing,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_frame_in_kb_p" that defaults kb-local-only-p to nil
  public void encache_frame_in_kb_p
     (Node _values_,
      Node thing) {
     this.encache_frame_in_kb_p(_values_,
                                thing,
                                _NIL);
  }


// ---------------------- frs-independent-frame-handle

/**
 * Given a frame, returns   frame-handle , which is a KRS-independent
 * OKBC frame handle object.    Frame-handle  may now be used in network
 * applications to refer to   frame  or in communication between KBs.
 * The correspondence between   frame  and   frame-handle  is
 * maintained, so that subsequent calls with the same frame will return the
 * same frame-handle.
 * 
 * It is not anticipated that this operation will ever be called by user
 * applications, but must be used by back ends to implement
 *   decontextualize .
 * 
 * Note:  This operation is named   frs-independent-frame-handle  for
 * historical reasons.  Frame Representation Systems are now uniformly
 * called Knowledge Representation Systems with the exception of in the names
 * of this operator and   frs-name .
 */
  public Node frs_independent_frame_handle
     (Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frs_independent_frame_handle_fname,
                       this,
                       frame,
                       kb_local_only_p);
         TrieResult trie_result = cache.get_trie_returning_node(key);
         if (trie_result.found_p)
           {
             cache_hit_hook(frs_independent_frame_handle_fname, key, trie_result);
             return ((Node)trie_result.result);
           }
         else {
                Values2 cache_miss_hook_result =
                  cache_miss_hook(frs_independent_frame_handle_fname, key);
                Node run_p = cache_miss_hook_result.nthValue(1);
                if (run_p != _NIL)
                  {
                    Object new_result = null;
                    Node old_ok_to_cache_flag = connection().ok_to_cache_flag;
                    if (old_ok_to_cache_flag == _flush)
                      { new_result =
                           super.frs_independent_frame_handle(frame,
                                                              kb_local_only_p);
                      }
                    else if (old_ok_to_cache_flag == _testing ||
                             old_ok_to_cache_flag == _yes)
                      { try 
                        { connection().ok_to_cache_flag = _testing;
                          new_result = 
                               super.frs_independent_frame_handle(frame,
                                                                  kb_local_only_p);
                        }
                        finally
                          { if (connection().ok_to_cache_flag == _testing)
                              { trie_result.trie_node.value = new_result;
                                cache_fill_hook(frs_independent_frame_handle_fname, key,
                                                new_result, false);
                              }
                            else { flush_cache("frs_independent_frame_handle"); }
                            connection().ok_to_cache_flag
                              = old_ok_to_cache_flag;
                          }
                      }
                    else { throw new RuntimeException
                             ("Internal cache error.  ok_to_cache_flag is " +
                              connection().ok_to_cache_flag);
                         }
                    return (Node)new_result;
                  }
                else
                  { Node new_result = cache_miss_hook_result.firstValue();
                    if (connection().ok_to_cache_flag == _testing)
                      { trie_result.trie_node.value = new_result; }
                    cache_fill_hook(frs_independent_frame_handle_fname, key,
                                    new_result, false);
                    return new_result;
                  }
              }
       }
     else
       {
         return super.frs_independent_frame_handle(frame,
                                                   kb_local_only_p);
       }
    }


  public Node cached_p_frs_independent_frame_handle
     (Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frs_independent_frame_handle_fname,
                       this,
                       frame,
                       kb_local_only_p);
	     return cache_entry_found_p(key);
       }
     else { return _NIL; }
  }

  // Method for "cached_p_frs_independent_frame_handle" that defaults kb-local-only-p to nil
  public Node cached_p_frs_independent_frame_handle
     (Node frame) {
     return this.cached_p_frs_independent_frame_handle(frame,
                                                       _NIL);
  }


  public void encache_frs_independent_frame_handle
     (Node _values_,
      Node frame,
      TruthValue kb_local_only_p) {
     if (allow_caching_p)
       {
         Node key = 
             Cons.list(frs_independent_frame_handle_fname,
                       this,
                       frame,
                       kb_local_only_p);
         encache(_values_, key);
       }
  }

  // Method for "encache_frs_independent_frame_handle" that defaults kb-local-only-p to nil
  public void encache_frs_independent_frame_handle
     (Node _values_,
      Node frame) {
     this.encache_frs_independent_frame_handle(_values_,
                                               frame,
                                               _NIL);
  }



/**
 * Logs you in to Host:port under the specified user-id and creates a
 * session called Session-description owned by Group-ID-for-Session with
 * the specified expected duration.  Returns three values:
 *   - Session key
 *   - Session ID
 *   - A string containing any messages that were emitted whilst creating
 *     the session, such as log messages from scripts being run.
 */
  static public SessionResponse login_user_and_create_session
     (String host,
      int    port,
      String user_id,
      String password,
      String group_id_for_session,
      String session_description,
      int expected_duration) {
       return NetworkKb.login_user_and_create_session
	 (host, port, user_id, password, group_id_for_session,
	  session_description, expected_duration);
   }




/**
 * Logs you in to Host:port under the specified user-id and connects
 * you to the session named by Key-Or-Session-ID.  It is important to
 * use this function when reconnecting, otherwise you'll create a new
 * session and will appear to lose all of your work.  It'll still be
 * there, though. Note that unlike other functions, this one accepts
 * either a session ID or a session key as a legal way to identify
 * the session to which you are going to connect.  This is because
 * the function is password protected, and session keys are user and
 * ip address specific.
 * Returns three values:
 *   - Session key
 *   - Session ID
 *   - A string containing any messages that were emitted whilst
 *     creating the session, such as log messages from scripts being run.
 */
  static public SessionResponse login_user_and_connect_to_session
     (String host,
      int    port,
      String user_id,
      String password,
      String key_or_sid) {
       return
	 NetworkKb.login_user_and_connect_to_session
	 (host, port, user_id, password, key_or_sid);
   }




/**
 * Returns a list of lists (session-id group description-string) for each
 *  of the sessions to which you can connect
 */
  static public SessionDescriptor[] active_sessions
     (String host,
      int    port,
      String user_id,
      String password,
      String session_key_to_use_instead_of_password) {
     return NetworkKb.active_sessions
       (host, port, user_id, password,
	session_key_to_use_instead_of_password);
   }




/**
 * Kills the session named by Session-Key-Or-ID.  If you supply a non-null
 * value for key-to-use-instead_of_password, then you can supply a null string
 * for the password, and this is used instead of session-key-or-id.
 */
  static public void kill_session
     (String host,
      int    port,
      String user_id,
      String password,
      String key_or_sid,
      String session_key_to_use_instead_of_password) {
       NetworkKb.kill_session
	 (host, port, user_id, password, key_or_sid, 
	  session_key_to_use_instead_of_password);
   }




/**
 * Returns a list of the group IDs applicable to the current user.
 */
  static public String[] get_groups
     (String host,
      int    port,
      String user_id,
      String password,
      String session_key_to_use_instead_of_password) {
       return NetworkKb.get_groups
	 (host, port, user_id, password, 
	  session_key_to_use_instead_of_password);
   }



}