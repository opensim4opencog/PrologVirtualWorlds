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
public class Kb extends Node {
  protected static final int typeCode = Kb;
  protected AbstractSymbol kb_type_name = null;
  public Node kbname = null; // why restrict to a symbol?
  public IoSyntax ioSyntax = new IoSyntax();
  public Node readtable_case = _upcase;
  Node unique_id = null;
  static int unique_id_counter = 0;
  public boolean kb_has_been_modified_p = false;
  Node last_locator = null;

  protected void setName (Node name) {
    kbname = name;
  }

  protected Node unique_id() {
    if (unique_id == null)
      { unique_id = new OKBCInteger(unique_id_counter);
	unique_id_counter = unique_id_counter + 1;
	return unique_id;
      }
    else { return unique_id; }
  }

  public String toString() {
    return "#<" + getClass().getName() + " " + kbname 
                + " " 
		+ (kb_has_been_modified_p ? "(unsaved) " : "")
		+ pointerForToString() + ">";
  }

  public static Symbol keyword (String name) {
    return Symbol.keyword(name);
  }
  
  public Symbol intern (String name) {
    return Symbol.intern(name, (Package)(ioSyntax.io_package));
  }

  public Symbol intern (String name, String packagename) {
    return Symbol.intern(name, packagename);
  }

  public boolean equals(Object thing) {
    return this == thing;
  }

  protected void encode(PrintWriter s, boolean client_p) {
    // System.out.println("Kb encode = " + this + " client_p = "+ client_p);
    if (client_p) { super.encode(s, client_p); }
    else
      {
	s.print(typeCode);
	newline(s);
	if (kbname == null) { _kb_type.encode(s, client_p); }
	else { kbname.encode(s, client_p); }
	if (kb_type_name == null)
	  { AbstractSymbol type = abstract_type_from_concrete_kb(this);
	    if (type == null)
	      { throw new RuntimeException
		  ("Cannot compute abstract kb type for " + this + 
		   ".  Its kb type has not been registered with "+
		   "register_concrete_kb_type.");
	      }
	    else { type.encode(s, client_p); }
	  }
	else { abstract_type_from_concrete_type(kb_type_name).encode
	       (s, client_p);}
      }
  }
  
  
// Stub method

  public Connection connection () {
    return null;
  }

  protected Connection connection_arg_default (Connection deflt) {
    if (connection() != null)
      { Connection c = connection();
	if ((c != null) && (c instanceof Connection)) { return c; }
	else if (deflt != null) { return deflt; }
	else { return LocalConnection.local_connection(); }
      }
    else if (deflt != null) { return deflt; }
    else { return LocalConnection.local_connection(); }
  }

  protected Connection connection_arg_default () {
    return connection_arg_default(null);
  }

  public void set_connection (Connection conn) {
    throw new RuntimeException ("Cannot set connection on " + this);
  }

  public boolean has_persistent_connection() {
    return connection().has_persistent_connection();
  }

  public void set_connection_persistence (boolean new_state) {
    connection().set_connection_persistence(new_state);
  }

  public void register_side_effect () {
    connection().register_side_effect();
  }

  public void register_side_effect (Object arg) {
    connection().register_side_effect(arg);
  }

  protected Node makeNetworkCall(Node args) {

       // System.out.println("Call = " + args);

     throw new RuntimeException
       ("Network OKBC Method for makeNetworkCall not implemented for "
        + "makeNetworkCall in class "
	+ getClass().getName());
   }


// ---------- default implementation of add-class-superclass


/**
 * Adds the   new-superclass  to the superclasses of   class .
 * Returns no values.
 */
  protected void add_class_superclass_internal
     (Node the_class, Node new_superclass, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("add_class_superclass"),
			    this).signal();
  }

// ---------- default implementation of add-facet-value


/**
 * If the specified facet does not already contain   value ,
 * then   value  is added to the set of values of the facet.
 * Returns no values.
 */
  protected void add_facet_value_internal
     (Node frame, Node slot, Node facet, Node value, Node test,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("add_facet_value"),
			    this).signal();
  }

// ---------- default implementation of add-instance-type


/**
 * Adds the   new-type  to the types of   frame .
 * Returns no values.
 */
  protected void add_instance_type_internal
     (Node frame, Node new_type, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("add_instance_type"),
			    this).signal();
  }

// ---------- default implementation of add-slot-value


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
  protected void add_slot_value_internal
     (Node frame, Node slot, Node value, Node test, SlotType slot_type,
      Node add_before, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("add_slot_value"),
			    this).signal();
  }

// ---------- default implementation of ask


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
  protected Values3 ask_internal
     (Node query, Node reply_pattern, InferenceLevel inference_level, 
      Node number_of_values, TruthValue error_p, Node where, Node timeout,
      ValueSelector value_selector, TruthValue kb_local_only_p) 
  { throw new MethodMissing(new OKBCString("ask"),
			    this).signal();
  }

  protected Values2 handle_unhandled_query
     (Node query, InferenceLevel inference_level, TruthValue error_p,
      ValueSelector value_selector, TruthValue kb_local_only_p, Node bindings)
  { throw new CannotHandle(query); }


  protected Values2 handle_simple_query
	(Node query, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, 
	 ConsLocation all_binding_lists)
  { Values3 vals = null;
    Node unify_failed_p = null;

    vals = TellAndAsk.handle_evaluable_predicate_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_frame_properties_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_frame_type_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_instance_type_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_subclass_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_primitivity_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_slot_of_attachment_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, __slot_of, _own);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_slot_of_attachment_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, __template_slot_of, _template);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_facet_of_attachment_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, __facet_of, _own);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_facet_of_attachment_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, __template_facet_of, _template);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_slot_value_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, _own);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_slot_value_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, _template);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_facet_value_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, _own);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    vals = TellAndAsk.handle_facet_value_query
      (query, this, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, _template);
    unify_failed_p = vals.thirdValue();
    if (unify_failed_p == _NIL) { return vals; }

    return handle_unhandled_query
      (query, inference_level, error_p, value_selector, kb_local_only_p,
       bindings);
  }

// ---------- default implementation of askable


/**
 * The   askable  operation returns   if the KRS can
 * determine that   ask ing the   sentence  would result in a
 *   cannot-handle  error being signaled, and   otherwise.
 * It may also signal the   syntax-error  condition.  Even if
 *   askable  returns  ,
 *   ask  may still not be able to handle the   sentence .
 */
  protected Node askable_internal
     (Node sentence, ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("askable"),
			    this).signal();
  }

// ---------- default implementation of get-kbs-of-type


/**
 * Returns   list-of-kbs , the list of all the known KBs whose type
 * matches   kb-type , and that are accessible through the
 *   connection .
 */
  protected Node get_kbs_of_type_internal (Connection connection) {
    // THIS is a kb_type (kb instance).
    return get_kbs_of_type_1(connection);
  }

  protected Node get_kbs_of_type_1 (Connection connection) {
    MetaKb meta_kb = (MetaKb)meta_kb(LocalConnection.local_connection());
    return meta_kb.get_kbs_of_type_using_meta_kb(this);
  }

// ---------- default implementation of get-slot-type


/**
 * Returns one of     :own ,    :template ,    to
 * identify the   slot-type  of the slot on question.  If there are both
 * an own and a template slot on   frame  identified by   slot , then
 *    :own  is returned.
 * If no such slot is known, then   is returned.
 */
  protected Node get_slot_type_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
       if (Cons.member_test
	   (slot, get_frame_slots(frame, inference_level, _own,
				  kb_local_only_p).firstValue(),
	    _eql, this, kb_local_only_p))
	 { return _own; }
       else if (Cons.member_test
		(slot, get_frame_slots(frame, inference_level, _template, 
				       kb_local_only_p).firstValue(),
		 _eql, this, kb_local_only_p))
	 { return _template; }
       else { return _NIL; }
    }

// ---------- default implementation of get-slot-value


/**
 * Returns the single member of the set of values
 * of the   slot .  This operation is meaningful only for single-valued
 * slots.  It is an error to call   get-slot-value  on a non-single-valued
 * slot, and implementations should signal a   cardinality-violation  if
 * this occurs.  When there is no value for the slot,   value-or-false 
 * is  .
 */
  protected Values2 get_slot_value_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Values3 $results = get_slot_values
       (frame, slot, inference_level, slot_type, new OKBCInteger(2),
	value_selector, kb_local_only_p);
     Node list = $results.firstValue();
     if (list.length() >= 2)
       { throw new CardinalityViolation
	 (new OKBCString("Single valued slot"), frame, slot, slot_type, _NIL,
	  this).signal();
       }
     return new Values2(list.car(), $results.secondValue());
    }


// ---------- default implementation of put-slot-value


/**
 * Sets the values of   slot  in   frame  to be a singleton set
 * consisting of a single element:    value .
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  protected void put_slot_value_internal
     (Node frame,
      Node slot,
      Node value,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     put_slot_values (frame, slot, Cons.list(value),
		      slot_type, value_selector, kb_local_only_p);
    }


  protected Node get_collection_type
    (Node frame, Node slot, SlotType slot_type, TruthValue kb_local_only_p)  {
       Node facet_val = get_facet_value(frame, slot, _collection_type,
					_all_inferable, slot_type, _either,
					kb_local_only_p).firstValue();
       if (facet_val == _NIL)
	 { Node behav = get_behavior_values(_collection_types).car();
	   if (behav == _NIL)
	     { return _set; }
	   else { return behav; }
	 }
       else { return _NIL; }
     }


// ---------- default implementation of put-facet-value


/**
 * Sets the values of the specified facet to be a singleton set
 * consisting of a single element:   value .  Returns no values.
 */
  protected void put_facet_value_internal
     (Node frame, Node slot, Node facet, Node value, SlotType slot_type,
      ValueSelector value_selector, TruthValue kb_local_only_p) {
       put_facet_values (frame, slot, facet, Cons.list(value), slot_type,
			 value_selector, kb_local_only_p);
    }


// ---------- default implementation of coerce-to-class


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
  protected Values2 coerce_to_class_internal (Node thing, TruthValue error_p,
					   TruthValue kb_local_only_p) {
    if (Cons.member_eql(_class, get_behavior_values(_are_frames), this,
			kb_local_only_p))
      { Values2 framev = coerce_to_frame(thing, _NIL, kb_local_only_p);
	Node the_class = framev.firstValue();
	Node found_p = framev.secondValue();
	if ((found_p != _NIL) && (class_p(the_class, kb_local_only_p) != _NIL))
	  { return new Values2(the_class, found_p); }
	else if (error_p != _NIL)
	  { throw new ClassNotFound(the_class, this).signal(); }
	else { return new Values2(_NIL, _NIL); }
      }
    else { if (class_p(thing, kb_local_only_p) != _NIL)
	     { return new Values2(thing, _T); }
	   else { if (error_p != _NIL)
		    { throw new ClassNotFound(thing, this).signal(); }
		  else { return new Values2(_NIL, _NIL); }
		}
	 }
       
    }

// ---------- default implementation of coerce-to-slot


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
  protected Values2 coerce_to_slot_internal (Node thing, TruthValue error_p,
					  TruthValue kb_local_only_p) {
    if (Cons.member_eql(_slot, get_behavior_values(_are_frames), this,
			kb_local_only_p))
      { Values2 framev = coerce_to_frame(thing, _NIL, kb_local_only_p);
	Node slot = framev.firstValue();
	Node found_p = framev.secondValue();
	if ((found_p != _NIL) && 
	    (slot_p(slot, kb_local_only_p) != _NIL))
	  { return new Values2(slot, found_p); }
	else if (error_p != _NIL)
	  { throw new SlotNotFound(_NIL, thing, _NIL, this).signal(); }
	else { return new Values2(_NIL, _NIL); }
      }
    else { if (slot_p(thing, kb_local_only_p) != _NIL)
	     { return new Values2(thing, _T); }
	   else { if (error_p != _NIL)
		    { throw new SlotNotFound(_NIL, thing, _NIL, 
					     this).signal(); 
		    }
		  else { return new Values2(_NIL, _NIL); }
		}
	 }
       
    }

// ---------- default implementation of coerce-to-facet


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
  protected Values2 coerce_to_facet_internal (Node thing, TruthValue error_p,
					   TruthValue kb_local_only_p) {
    if (Cons.member_eql(_facet, get_behavior_values(_are_frames), this,
			kb_local_only_p))
      { Values2 framev = coerce_to_frame(thing, _NIL, kb_local_only_p);
	Node facet = framev.firstValue();
	Node found_p = framev.secondValue();
	if ((found_p != _NIL) && 
	    (facet_p(facet, kb_local_only_p) != _NIL))
	  { return new Values2(facet, found_p); }
	else if (error_p != _NIL)
	  { throw new FacetNotFound(_NIL, _NIL, _NIL, facet, this).signal(); }
	else { return new Values2(_NIL, _NIL); }
      }
    else { if (facet_p(thing, kb_local_only_p) != _NIL)
	     { return new Values2(thing, _T); }
	   else { if (error_p != _NIL)
		    { throw new FacetNotFound(_NIL, _NIL, _NIL, thing, 
					      this).signal(); 
		    }
		  else { return new Values2(_NIL, _NIL); }
		}
	 }
       
    }

// ---------- default implementation of coerce-to-individual


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
  protected Values2 coerce_to_individual_internal
       (Node thing, TruthValue error_p, TruthValue kb_local_only_p) {
    if (class_p(thing, kb_local_only_p) != _NIL)
      { if (error_p != _NIL)
	  { throw new IndividualNotFound(thing, this).signal(); }
      else { return new Values2(_NIL, _NIL); }
      }
    else if (Cons.member_eql(_individual, get_behavior_values(_are_frames),
			     this, kb_local_only_p))
      { Values2 framev = coerce_to_frame(thing, _NIL, kb_local_only_p);
	Node individual = framev.firstValue();
	Node found_p = framev.secondValue();
	if (found_p != _NIL)
	  { return new Values2(individual, found_p); }
	else if (error_p != _NIL)
	  { throw new IndividualNotFound(individual, this).signal(); }
	else { return new Values2(_NIL, _NIL); }
      }
    else { return new Values2(thing, _T); }
  }

// ---------- default implementation of create-frame


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
  protected Node create_frame_internal
     (Node name, ContextName frame_type, Node direct_types,
      Node direct_superclasses, Node doc, Node template_slots,
      Node template_facets, Node own_slots, Node own_facets, Node primitive_p,
      Node handle, Node pretty_name, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("create_frame"),
			    this).signal();
  }

// ---------- default implementation of create-procedure


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
  protected Node create_procedure_internal
       (Node arguments, Node body, Node environment) {
    return Procedure.intern_procedure(_NIL, arguments, body, environment);
  }

  protected Node create_procedure (String arguments, Node body,
				Node environment) {
    return Procedure.intern_procedure(_NIL, new OKBCString(arguments),
				      body, environment);
  }

  protected Node create_procedure (String arguments, String body,
				Node environment) {
    return Procedure.intern_procedure(_NIL, new OKBCString(arguments),
				      new OKBCString(body), environment);
  }

  protected Node create_procedure (Node arguments, String body,
				Node environment) {
    return Procedure.intern_procedure(_NIL, arguments, new OKBCString(body),
				      environment);
  }

// ---------- default implementation of frs-independent-frame-handle


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
  protected Node frs_independent_frame_handle_internal
      (Node frame, TruthValue kb_local_only_p) {
    Values2 framev = coerce_to_frame(frame, _NIL, kb_local_only_p);
    Node thing_for_handle = frame;
    if (framev.secondValue() != _NIL)
      { thing_for_handle = 
	get_frame_handle(framev.firstValue(), kb_local_only_p); 
      }
    if (thing_for_handle instanceof FrameHandle)
      { return thing_for_handle; }
    else { return FrameHandle.find_or_create_frame_handle
	   (thing_for_handle, this);
	 }
  }

// ---------- default implementation of eql-in-kb


/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * and otherwise returns  .
 *   Arg0  and   arg1  may be any combination of objects coercible
 * to frames.
 */
  protected Node eql_in_kb_internal
         (Node arg0, Node arg1, TruthValue kb_local_only_p) {
    if (arg0 == arg1) { return _T; }
    Values2 v0 = coerce_to_frame(arg0, _NIL, kb_local_only_p);
    Node arg0_found_p = v0.secondValue();
    if (arg0_found_p != _NIL)
      { Node arg0_frame = v0.firstValue();
	if (arg0_frame == arg1) { return _T; }
	Values2 v1 = coerce_to_frame(arg1, _NIL, kb_local_only_p);
	Node arg1_found_p = v1.secondValue();
	if (arg1_found_p != _NIL)
	  { Node arg1_frame = v1.firstValue();
	    if (arg0_frame == arg1_frame) { return _T; }
	  }
      }
    else if (arg0 instanceof OKBCInteger || arg0 instanceof Real)
      { 
	if (arg0.equals(arg1)) { return _T; }
	else { return _NIL; }
      }
    return _NIL;
  }

// ---------- default implementation of equal-in-kb


/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * or they are strings containing the same characters (case sensitively), or
 * both are lists with the same structure, and each of the elements
 * recursively is true according to   equal-in-kb .  Returns   
 * otherwise.
 */
  protected Node equal_in_kb_internal
     (Node arg0, Node arg1, TruthValue kb_local_only_p) {
       if (arg0 == null || arg0 == _NIL)
	 { if (arg1 == null || arg1 == _NIL) { return _T; }
	   else { return _NIL; }
	 }
       else if (arg0 instanceof OKBCString)
	 { if (arg0.equals(arg1)) { return _T; } else { return _NIL; } }
       else if (arg0 instanceof Cons)
	 { if (arg1 instanceof Cons)
	     { if (equal_in_kb(arg0.car(), arg1.car(), kb_local_only_p) !=
		   _NIL)
		 { Node cdr0 = ((Cons)arg0).cdr_or_NIL();
		   Node cdr1 = ((Cons)arg1).cdr_or_NIL();
		   if (equal_in_kb(cdr0, cdr1, kb_local_only_p) != _NIL)
		     { return _T; }
		   else { return _NIL; }
		 }
	       else { return _NIL; }
	     }
	   else { return _NIL; }
	 }
       else { return eql_in_kb(arg0, arg1, kb_local_only_p); }
     }


// ---------- default implementation of equalp-in-kb


/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * or they are strings containing the same characters (case-insensitively), or
 * both are lists with the same structure, and each of the elements
 * recursively is true according to   equalp-in-kb .  Returns
 *   otherwise.
 */
  protected Node equalp_in_kb_internal
     (Node arg0, Node arg1, TruthValue kb_local_only_p) {
       if (arg0 == null || arg0 == _NIL)
	 { if (arg1 == null || arg1 == _NIL) { return _T; }
	   else { return _NIL; }
	 }
       else if (arg0 instanceof OKBCString)
	 { if (arg1 instanceof OKBCString)
	     { if (((OKBCString)arg0).string.equalsIgnoreCase
		   (((OKBCString)arg1).string))
		 { return _T; }
	     }
	   return _NIL;
	 }
       else if (arg0 instanceof Cons)
	 { if (arg1 instanceof Cons)
	     { if (equalp_in_kb(arg0.car(), arg1.car(), kb_local_only_p) !=
		   _NIL)
		 { if (equalp_in_kb(arg0.cdr(), arg1.cdr(),
				    kb_local_only_p) != _NIL)
		     { return _T; }
		   else { return _NIL; }
		 }
	       else { return _NIL; }
	     }
	   else { return _NIL; }
	 }
       else { return eql_in_kb(arg0, arg1, kb_local_only_p); }
     }

// ---------- default implementation of facet-has-value-p


/**
 * Returns   iff the specified facet has a value for the specified slot
 * and frame, and otherwise returns  .
 */
  protected Values2 facet_has_value_p_internal
     (Node frame, Node slot, Node facet, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("facet_has_value_p"),
			    this).signal();
  }

// ---------- default implementation of frame-has-slot-p


/**
 * Returns   iff   slot  is a slot in   frame ,
 * otherwise returns  .
 */
  protected Node frame_has_slot_p_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("frame_has_slot_p"),
			    this).signal();
  }

// ---------- default implementation of get-class-instances


/**
 * Returns a   list-of-instances  for   class .
 */
  protected Values3 get_class_instances_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_class_instances"),
			    this).signal();
  }

// ---------- default implementation of get-class-subclasses


/**
 * Returns the   list-of-subclasses  of   class .
 */

  protected Values3 get_class_subclasses_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values, 
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_class_subclasses"),
			    this).signal();
  }

// ---------- default implementation of get-class-superclasses


/**
 * Returns the   list-of-superclasses  of   class .
 */
  protected Values3 get_class_superclasses_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_class_superclasses"),
			    this).signal();
  }

// ---------- default implementation of get-facet-value


/**
 * Returns the sole member of the set of values
 * of the specified facet.  It is most commonly used when that set is
 * expected to have only one member.  When the facet has no value,
 *   value-or-false  is  .  It is an error to
 * call this operation on a non-single-valued facet; if it is called, a
 *   cardinality-violation  error should be signaled.
 */
  protected Values2 get_facet_value_internal
     (Node frame, Node slot, Node facet, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector, 
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_facet_value"),
			    this).signal();
  }

// ---------- default implementation of get-facet-values


/**
 * Returns the set of values of the specified facet,
 * in no guaranteed order.  It always returns a (possibly empty) list
 * of values.
 */
  protected Values3 get_facet_values_internal
         (Node frame, Node slot, Node facet, InferenceLevel inference_level,
	  SlotType slot_type, Node number_of_values,
	  ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_facet_values"),
			    this).signal();
  }

// ---------- default implementation of get-facet-values-in-detail


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
  protected Values4 get_facet_values_in_detail_internal
         (Node frame, Node slot, Node facet, InferenceLevel inference_level,
	  SlotType slot_type, Node number_of_values,
	  ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_facet_values_in_detail"),
			    this).signal();
  }

// ---------- default implementation of get-frame-handle


/**
 * Returns a   frame-handle  that uniquely identifies the   frame 
 * argument in   kb .
 */
  protected Node get_frame_handle_internal
        (Node frame, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_frame_handle"),
			    this).signal();
  }

// ---------- default implementation of get-frame-sentences


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
  protected Values3 get_frame_sentences_internal
     (Node frame, Node number_of_values, Node okbc_sentences_p,
      ValueSelector value_selector, TruthValue kb_local_only_p) 
  { throw new MethodMissing(new OKBCString("get_frame_sentences"),
			    this).signal();
  }

// ---------- default implementation of get-frame-slots


/**
 * Returns   list-of-slots , a list of all the own, template, or own
 * and template slots that are associated with   frame , depending on the
 * value of   slot-type .
 */
  protected Values2 get_frame_slots_internal
         (Node frame, InferenceLevel inference_level, SlotType slot_type,
	  TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_frame_slots"),
			    this).signal();
  }

// ---------- default implementation of get-frames-with-facet-value


/**
 * Returns the set of frames in which the specified facet value is accessible
 * on the specified slot.
 * If the system is unable to find any frame/slot/facet combinations with the
 * specified value,   is returned.
 * This operation allows user interfaces to take users from a value
 * displayed as a facet value on a particular frame/slot to the place
 * that asserted the value.
 */
  protected Values2 get_frames_with_facet_value_internal
    (Node slot, Node facet, Node value, InferenceLevel inference_level,
     SlotType slot_type, ValueSelector value_selector,
     TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_frames_with_facet_value"),
			    this).signal();
  }

// ---------- default implementation of get-frames-with-slot-value


/**
 * Returns the set of frames in which the specified slot value is accessible.
 * If the system is unable to find any frame/slot combinations with the
 * specified value,   is returned.
 * This operation allows user interfaces to take users from a value
 * displayed as a slot value on a particular frame to the place that
 * asserted the value.
 */
  protected Values2 get_frames_with_slot_value_internal
       (Node slot, Node value, InferenceLevel inference_level,
	SlotType slot_type, ValueSelector value_selector,
	TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_frames_with_slot_value"),
			    this).signal();
  }

// ---------- default implementation of get-frame-type


/**
 * When   thing  identifies a frame, returns either
 *    :slot ,    :facet ,    :class , or    :individual ,
 * depending on the type of the frame.
 * When   thing  does not identify a frame,   frame-type  is
 *  .     :Slot  and    :facet  will be returned only in
 * those systems that support the values    :slot  and    :facet ,
 * respectively, for the    :are-frames  behavior.
 */
  protected Node get_frame_type_internal
     (Node thing, TruthValue kb_local_only_p) {
       Values2 framev = coerce_to_frame(thing, _NIL, kb_local_only_p);
       boolean found_p = framev.secondValue() != _NIL;
       if (found_p)
	 { Node frame = framev.firstValue();
	   if (slot_p(frame, kb_local_only_p) != _NIL)
	     { return _slot; }
	   if (facet_p(frame, kb_local_only_p) != _NIL)
	     { return _facet; }
	   if (class_p(frame, kb_local_only_p) != _NIL)
	     { return _class; }
	   if (individual_p(frame, kb_local_only_p) != _NIL)
	     { return _individual; }
	 }
       if (slot_p(thing, kb_local_only_p) != _NIL)
	 { return _slot; }
       if (facet_p(thing, kb_local_only_p) != _NIL)
	 { return _facet; }
       if (class_p(thing, kb_local_only_p) != _NIL)
	 { return _class; }
       if (individual_p(thing, kb_local_only_p) != _NIL)
	 { return _individual; }
       return _NIL;
    }

// ---------- default implementation of get-instance-types


/**
 * Returns the   list-of-types  of   frame , that is, the list of
 * classes of which   frame  is an instance.
 */
  protected Values3 get_instance_types_internal
     (Node instance, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_instance_types"),
			    this).signal();
  }

// ---------- default implementation of get-kb-classes


/**
 * Returns   list-of-classes , a list of the classes in the KB.
 *   Selector  can be one of the following:
 *  
 *      :all  -- Returns all classes
 *      :frames  -- Returns classes that are represented as frames
 *      :system-default  -- Returns either all classes or
 * only class frames, according to which is the KRS's default
 */
  protected Node get_kb_classes_internal
     (Node selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_kb_classes"),
			    this).signal();
  }

// ---------- default implementation of get-kb-facets


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
  protected Node get_kb_facets_internal
     (Node selector, TruthValue kb_local_only_p) 
  { throw new MethodMissing(new OKBCString("get_kb_facets"),
			    this).signal();
  }

// ---------- default implementation of get-kb-individuals


/**
 * Returns   list-of-individuals , a list of the individual frames in
 *   kb .    Selector  can be one of the following:
 *  
 *      :all  -- Returns all accessible individuals
 *      :frames  -- Returns only individuals that are frames
 *      :system-default  -- Returns either all individuals or
 * only individual frames, according to which is the KRS's default
 */
  protected Node get_kb_individuals_internal
     (Node selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_kb_individuals"),
			    this).signal();
  }

// ---------- default implementation of get-kb-slots


/**
 * Returns the   list-of-slots  that are defined in the KB.
 *   Selector  can be one of the following:
 *  
 *      :all  -- Returns all slots
 *      :frames  -- Returns slots that are represented frames
 *      :system-default  -- Returns either all slots or
 * only slot frames, according to which is the KRS's default
 */
  protected Node get_kb_slots_internal
     (Node selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_kb_slots"),
			    this).signal();
  }

// ---------- default implementation of get-slot-facets


/**
 * Returns the   list-of-facets  associated with   slot  in
 *   frame .
 */
  protected Values2 get_slot_facets_internal (Node frame, Node slot,
					   InferenceLevel inference_level,
					   SlotType slot_type,
					   TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_slot_facets"),
			    this).signal();
  }

// ---------- default implementation of get-slot-values


/**
 * Returns the   list-of-values  of   slot 
 * within   frame .  If the    :collection-type  of the slot is
 *    :list , and only    :direct  own slots have been asserted, then
 * order is preserved; otherwise, the values are returned in no guaranteed
 * order.    Get-slot-values  always returns a list of
 * values.  If the specified slot has no values,   is
 * returned.
 */
  protected Values3 get_slot_values_internal
         (Node frame, Node slot, InferenceLevel inference_level,
	  SlotType slot_type, Node number_of_values,
	  ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_slot_values"),
			    this).signal();
  }

// ---------- default implementation of get-slot-values-in-detail


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
  protected Values4 get_slot_values_in_detail_internal
         (Node frame, Node slot, InferenceLevel inference_level,
	  SlotType slot_type, Node number_of_values,
	  ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("get_slot_values_in_detail"),
			    this).signal();
  }

// ---------- default implementation of individual-p


/**
 * Returns   if   thing  identifies an individual entity, and
 * returns   if   thing  identifies a class.
 */
  protected Node individual_p_internal
     (Node thing, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("individual_p"),
			    this).signal();
  }

// ---------- default implementation of instance-of-p


/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  protected Values2 instance_of_p_internal
     (Node thing, Node the_class, InferenceLevel inference_level,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("instance_of_p"),
			    this).signal();
  }

// ---------- default implementation of member-facet-value-p


/**
 * Returns   iff   value  is a value in the specified   facet 
 * of   slot  on   frame , as determined by the predicate
 *   test , and returns   otherwise.
 */
  protected Values2 member_facet_value_p_internal
     (Node frame, Node slot, Node facet, Node value, 
      InferenceLevel inference_level, Node test, SlotType slot_type, 
      ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("member_facet_value_p"),
			    this).signal();
  }

// ---------- default implementation of member-slot-value-p


/**
 * Returns   iff   value  is a value in   slot  of
 *   frame , as determined by the predicate   test , and returns
 *   otherwise.
 */
  protected Values2 member_slot_value_p_internal
     (Node frame, Node slot, Node value, InferenceLevel inference_level,
      Node test, SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("member_slot_value_p"),
			    this).signal();
  }

// ---------- default implementation of remove-class-superclass


/**
 * Removes   superclass-to-remove  from the superclasses of   class .
 * Returns no values.
 */
  protected void remove_class_superclass_internal
     (Node the_class, Node superclass_to_remove, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("remove_class_superclass"),
			    this).signal();
  }

// ---------- default implementation of remove-facet-value


/**
 * If   value  is currently a member of the set of direct values of
 * the specified facet, then   value  is removed from the values of
 * the facet.  Returns no values.
 */
  protected void remove_facet_value_internal
     (Node frame, Node slot, Node facet, Node value, Node test,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("remove_facet_value"),
			    this).signal();
  }

// ---------- default implementation of remove-instance-type


/**
 * Removes   type-to-remove  from the types of   frame  -- that is,
 * makes   frame  no longer be an instance of   type-to-remove .
 * Returns no values.
 */
  protected void remove_instance_type_internal
     (Node frame, Node type_to_remove,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("remove_instance_type"),
			    this).signal();
  }

// ---------- default implementation of remove-local-facet-values


/**
 * Removes all direct values of   facet  in   slot  of   frame .
 * Returns no values.
 */
  protected void remove_local_facet_values_internal
     (Node frame, Node slot, Node facet, SlotType slot_type,
      ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("remove_local_facet_values"),
			    this).signal();
  }

// ---------- default implementation of remove-local-slot-values


/**
 * Removes all direct values in   slot  of   frame .
 * Returns no values.
 */
  protected void remove_local_slot_values_internal
     (Node frame, Node slot, SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("remove_local_slot_values"),
			    this).signal();
  }

// ---------- default implementation of remove-slot-value


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
  protected void remove_slot_value_internal
     (Node frame, Node slot, Node value, Node test, SlotType slot_type, 
      Node index, ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("remove_slot_values"),
			    this).signal();
  }

// ---------- default implementation of replace-facet-value


/**
 * If   old-value  is currently a member of the set of direct values of
 * the specified facet, then   old-value  is replaced by
 *   new-value  in the facet.  Returns no values.
 */
  protected void replace_facet_value_internal
     (Node frame, Node slot, Node facet, Node old_value, Node new_value,
      Node test, SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("replace_facet_value"),
			    this).signal();
  }

// ---------- default implementation of replace-slot-value


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
  protected void replace_slot_value_internal
     (Node frame, Node slot, Node old_value, Node new_value, Node test,
      SlotType slot_type, Node index, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("replace_slot_value"),
			    this).signal();
  }

// ---------- default implementation of slot-has-facet-p


/**
 * Returns   iff   facet  is a valid facet for   slot  on
 *   frame , and   otherwise.  What constitutes a valid facet is
 * KB-specific, but a facet with a value locally asserted, or with a value
 * that is accessible from a template slot will return   for this
 * operation.
 */
  protected Node slot_has_facet_p_internal
     (Node frame, Node slot, Node facet, InferenceLevel inference_level,
      SlotType slot_type, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("slot_has_facet_p"),
			    this).signal();
  }

// ---------- default implementation of slot-has-value-p


/**
 * Returns   iff   slot  on   frame  has at least one
 * value, otherwise returns  .
 */
  protected Values2 slot_has_value_p_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector, 
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("slot_has_value_p"),
			    this).signal();
  }

// ---------- default implementation of subclass-of-p


/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
   protected Values2 subclass_of_p_internal
     (Node subclass, Node superclass, InferenceLevel inference_level, 
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("subclass_of_p"),
			    this).signal();
  }

// ---------- default implementation of superclass-of-p


/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
  protected Values2 superclass_of_p_internal
     (Node superclass, Node subclass, InferenceLevel inference_level,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("superclass_of_p"),
			    this).signal();
  }

// ---------- default implementation of tell


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
  protected void tell_internal
    (Node sentence, Node frame, ValueSelector value_selector,
     TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("tell"),
			    this).signal();
  }

  protected Node handle_unhandled_sentence
     (Node sentence, Node frame, ValueSelector value_selector,
      TruthValue kb_local_only_p, Node key)
  { if (key == _tell) { throw new CannotHandle(sentence); }
    else { return _NIL; }
  }

// ---------- default implementation of tellable


/**
 * The   tellable  operation returns   if the KRS can
 * determine that   tell ing the   sentence  would result in a
 *   cannot-handle  error being signaled, and   otherwise.
 * It may raise a   syntax-error  error as specified with the
 * definition of   tell .  Even if   tellable  returns
 *  ,   tell  may still not be able to handle the
 *   sentence .
 */
  protected Node tellable_internal
    (Node sentence, ValueSelector value_selector, TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("tellable"),
			    this).signal();
  }

// ---------- default implementation of type-of-p


/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  protected Values2 type_of_p_internal
     (Node the_class, Node frame, InferenceLevel inference_level,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("type_of_p"),
			    this).signal();
  }

// ---------- default implementation of untell


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
  protected Node untell_internal
     (Node sentence, Node frame, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("untell"),
			    this).signal();
  }

// ---------- default implementation of revert-kb


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
  protected Node revert_kb_internal
     (TruthValue error_p) {
       Connection connection = connection();
       Kb kb_type = get_kb_type();
       Node kb_locator = kb_type.find_kb_locator(this, connection);
       close_kb(_NIL);
       return kb_type.open_kb(kb_locator, connection, error_p);
    }

// ---------- default implementation of frame-in-kb-p


/**
 * Returns   when   thing  is both coercible to
 * a frame, and that frame is known to be resident in   kb , and otherwise
 * returns  .  See   get-frame-in-kb .
 */
  protected Node frame_in_kb_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
       Values2 res = get_frame_in_kb(thing, _NIL, kb_local_only_p);
       return res.nthValue(1);
    }

// ---------- default implementation of coercible-to-frame-p


/**
 * Returns   when   thing  can be coerced to a frame
 * by using   coerce-to-frame , and otherwise returns  .
 */
  protected Node coercible_to_frame_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
       Values2 res = coerce_to_frame(thing, _NIL, kb_local_only_p);
       return res.nthValue(1);
    }

// ---------- default implementation of get-kb-direct-parents


/**
 * Returns the   list-of-parent-kbs  -- that is, the list of KBs directly
 * included by   kb .  Note that certain KB implementations may allow
 * circular inclusion dependencies in KBs.  The semantics of KB inclusion
 * are not specified by OKBC, but where possible, processing can be limited
 * to a particular KB by the use of the   kb-local-only-p  argument.
 */
  protected Node get_kb_direct_parents_internal () 
    { return _NIL; }

// ---------- default implementation of get-kb-direct-children


/**
 * Returns the   list-of-child-kbs  -- that is, the list of KBs that
 * directly include   kb .  Note that certain KB implementations may
 * allow circular inclusion dependencies in KBs.  The semantics of KB
 * inclusion are not specified by OKBC, but where possible, processing can
 * be limited to a particular KB by the use of the   kb-local-only-p 
 * argument.
 */
  protected Node get_kb_direct_children_internal () {
       Vector results = new Vector();
       Enumeration e = get_kbs_of_type(connection()).elements();
       for (; e.hasMoreElements() ;) {
	 Kb some_kb = (Kb)(e.nextElement());
	 if (Cons.member_eql(this, some_kb.get_kb_direct_parents(), this,
			     _NIL))
	   { results.addElement(some_kb); }
       }
       return Cons.toList(results);
    }

// ---------- default implementation of member-behavior-values-p


/**
 * Returns   when   value  is one of the variants of
 *   behavior  that is currently active for   kb , and returns
 *   otherwise.
 */
  protected Node member_behavior_values_p_internal (Node behavior, Node value) {
       if (Cons.member_equal(value, get_behavior_values(behavior), this, _NIL))
	 { return _T; }
       else { return _NIL; }
    }

// ---------- default implementation of get-kb-roots


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
  protected Node get_kb_roots_internal
     (Node selector,
      TruthValue kb_local_only_p) {
       Vector results = new Vector();
       Enumeration e = get_kb_classes(_frames, kb_local_only_p).elements();
       for (; e.hasMoreElements() ;) {
	 Node frame = (Node)(e.nextElement());
	 if ((frame_in_kb_p(frame, kb_local_only_p) != _NIL))
	   { Node superclasses =
	       get_class_superclasses(frame, _direct, _all,
				      kb_local_only_p).firstValue();
	     Enumerator s = superclasses.enumerate_list();
	     boolean found_p = false;
	     while (s.has_more_p()) {
	       Node cl = s.next();
	       if (frame_in_kb_p(cl, kb_local_only_p) != _NIL)
		 { found_p = true; break; }
	     }
	     if (!found_p)
	       { results.addElement(frame); }
	   }
       }
       return Cons.toList(results);
    }




/// Deal with facets.....

// ---------- default implementation of get-classes-in-domain-of


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
  protected Node get_classes_in_domain_of_internal
     (Node slot, Node frame_attachment, InferenceLevel inference_level,
      TruthValue kb_local_only_p) 
  { if (Cons.member_eql(_slot, get_behavior_values(_are_frames), this,
			kb_local_only_p) &&
	slot_p(_domain, kb_local_only_p) != _NIL)
      // We are in the land of reified slots, and :domain actually exists
      // as a slot.
      { Node domains = get_slot_values(slot, _domain, inference_level,
				       _own, _all, _either, 
				       kb_local_only_p).firstValue();
	Node filtered_domains = remove_classes_with_subclasses_in_list
	  (domains, inference_level, kb_local_only_p);
	if (filtered_domains.length() > 1)
	  // We have a list of domains with length > 1.
	  // We have no way to compute the intersection of this list
	  // so as far as we're concerned, there are no classes that are
	  // known to be in the domain of Slot.
	  { return _NIL; }
	else if (filtered_domains != _NIL) 
	  // There is exactly one domain.  We understand this, so it 
	  // is what we return.
	  { return filtered_domains; }
	// There are no known domains, so we are unrestricted. 
	// That means that anyTHING makes sense.
	else if (class_p(_thing, kb_local_only_p) != _NIL)
	  { return Cons.list(coerce_to_class(_thing, _T, 
					     kb_local_only_p).firstValue());
	  }
	else { return _NIL; }
      }
    // We have no :domain slot
    else
      { Values2 framev = coerce_to_frame
	  (frame_attachment, _NIL, kb_local_only_p);
	Node frame = framev.firstValue();
	Node found_p = framev.secondValue();
	Node classes = _NIL;
	if (found_p != _NIL)
	  // We have been given a frame hint, so look only transitively
	  // from there.
	  { classes = search_up_for_slot_attachments
	      (slot, frame, inference_level, kb_local_only_p);
	  }
	else
	  // We have no frame hint, so use all of the known classes to figure
	  // it out.
	  { Enumerator e = enumerate_kb_classes(_all, kb_local_only_p);
	    e.prefetch(_all);
	    while (e.has_more_p()) {
	      Node cl = e.next();
	      if (frame_has_slot_p(cl, slot, inference_level, _template,
				   kb_local_only_p) != _NIL)
		{ classes = new Cons(cl, classes); }
	    }
	    e.free();
	  }
	return remove_classes_with_subclasses_in_list
	  (classes, inference_level, kb_local_only_p);
      }
  }

  Node remove_classes_with_subclasses_in_list 
    (Node classes, InferenceLevel inference_level, TruthValue kb_local_only_p)
  { Node result = _NIL;
    Enumerator e = classes.enumerate_list();
    while (e.has_more_p()) {
      Node x = e.next();
      Enumerator f = classes.enumerate_list();
      boolean found_p = false;
      while (f.has_more_p()) {
	Node y = f.next();
	if (subclass_of_p(x, y, inference_level, kb_local_only_p).firstValue()
	    != _NIL)
	  { found_p = true;
	    break;
	  }
      }
      if (!found_p) { result = new Cons(x, result); }
    }
    return Cons.remove_duplicates(result, _eql, this, kb_local_only_p);
  }


  Node search_up_for_slot_attachments
    (Node slot, Node frame, InferenceLevel inference_level, 
     TruthValue kb_local_only_p)
  { Node classp = class_p(frame, kb_local_only_p);
    Node superclasses = _NIL;
    if (classp != _NIL)
      { superclasses = get_class_superclasses
	  (frame, inference_level, _all, kb_local_only_p).firstValue();
      }
    Node types = get_instance_types
      (frame, inference_level, _all, kb_local_only_p).firstValue();
    Node local_result = _NIL;
    if (classp != _NIL &&
	frame_has_slot_p
	(frame, slot, inference_level, _template, kb_local_only_p) != _NIL)
      { local_result = frame; }
    Node super_result = _NIL;
    // Note that we got all of the specified superclasses above,
    // so we must restrict if we are :direct and prevent recursion.
    if (inference_level == _direct) {}
    else 
      { Enumerator e = superclasses.enumerate_list();
	while (e.has_more_p()) {
	  Node superclass = e.next();
	  super_result = Cons.append_cons(search_up_for_slot_attachments
					  (slot, superclass, inference_level, 
					   kb_local_only_p),
					  super_result);
	}
      }
    Node type_result = _NIL;
    Enumerator typee = types.enumerate_list();
    while (typee.has_more_p()) {
      Node type = typee.next();
      type_result = Cons.append_cons(search_up_for_slot_attachments
				     (slot, type, inference_level, 
				      kb_local_only_p),
				     type_result);
    }
    if (super_result != _NIL)
      // Then we know that any local result must be subsumed.
      { return Cons.append_cons(super_result, type_result); }
    else if (local_result != _NIL)
      { return new Cons(local_result, type_result); }
    else { return type_result; }
  }


// ---------- default implementation of get-frame-details


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
  protected Values2 get_frame_details_internal
    (Node fr, InferenceLevel inference_level, Node number_of_values,
     TruthValue kb_local_only_p) {
      Node frame = coerce_to_frame(fr, _NIL, kb_local_only_p).firstValue();
      if (frame == _NIL)
	{ return new Values2(_NIL, _NIL); }
      else 
	{ boolean inexact_p = false;
	  Node type = get_frame_type(frame, kb_local_only_p);
	  Node name = get_frame_name(frame, kb_local_only_p);
	  Node handle = get_frame_handle(frame, kb_local_only_p);
	  Node pretty_name = get_frame_pretty_name(frame, kb_local_only_p);
	  Values2 own_slots = get_frame_slots
	    (frame, inference_level, _own, kb_local_only_p);
	  if (own_slots.secondValue() == _NIL) { inexact_p = true; }
	  Values2 template_slots = null_values2;
	  Values3 superclasses = null_values3;
	  Values3 subclasses = null_values3;
	  Values3 types = null_values3;
	  Node primitive_p = _T;
	  if (type == _class)
	    { template_slots = get_frame_slots
               (frame, inference_level, _template, kb_local_only_p);
	      if (template_slots.secondValue() == _NIL) { inexact_p = true; }
	      superclasses = get_class_superclasses
		(frame, inference_level, number_of_values,
		 kb_local_only_p); 
	      if (superclasses.secondValue() == _NIL) { inexact_p = true; }
	      subclasses = get_class_subclasses
		(frame, inference_level, number_of_values,
		 kb_local_only_p); 
	      if (subclasses.secondValue() == _NIL) { inexact_p = true; }
	      primitive_p = primitive_p(frame, kb_local_only_p);
	    }
          types = get_instance_types
	    (frame, inference_level, number_of_values, 
	     kb_local_only_p); 
	  if (types.secondValue() == _NIL) { inexact_p = true; }
	  Values2 own_slot_specs = get_slot_specification
	    (frame, own_slots.firstValue(), _own, inference_level,
	     number_of_values,
	     kb_local_only_p);
	  if (own_slot_specs.secondValue() == _NIL) { inexact_p = true; }
	  Values2 template_slot_specs = get_slot_specification
	    (frame, template_slots.firstValue(), _template, inference_level,
	     number_of_values, kb_local_only_p);
	  if (template_slot_specs.secondValue() == _NIL) { inexact_p = true; }
	  Values2 own_facet_specs = get_facet_specification
	    (frame, own_slots.firstValue(), _own, inference_level,
	     number_of_values, kb_local_only_p);
	  if (own_facet_specs.secondValue() == _NIL) { inexact_p = true; }
	  Values2 template_facet_specs = get_facet_specification
	    (frame, template_slots.firstValue(), _template, inference_level,
	     number_of_values, kb_local_only_p);
	  if (template_facet_specs.secondValue() == _NIL) { inexact_p = true; }
	  Values2 sentences = get_frame_sentences
	    (frame, number_of_values, _NIL, _either, kb_local_only_p);
	  if (sentences.secondValue() == _NIL) { inexact_p = true; }
	  Node result = 
	    Cons.append_cons
	    (Cons.append_cons
	     (Cons.list(_handle, handle,
			_name, name,
			_pretty_name, pretty_name,
			_frame_type, type,
			_primitive_p, primitive_p),
	      Cons.list(_superclasses, 
			as_list_of_frame_handles(superclasses,kb_local_only_p),
			_subclasses,
			as_list_of_frame_handles(subclasses,  kb_local_only_p),
			_types,
			as_list_of_frame_handles(types,     kb_local_only_p))),
	     Cons.list(_own_slots,       own_slot_specs.firstValue(),
		       _template_slots,  template_slot_specs.firstValue(),
			   _own_facets,      own_facet_specs.firstValue(),
		       _template_facets, template_facet_specs.firstValue(),
		       _sentences, sentences.firstValue()));
	  if (inexact_p) { return new Values2(result, _NIL); }
	  else  { return new Values2(result, _T); }
	}
    }

  Node as_list_of_frame_handles(Values list, TruthValue kb_local_only_p) {
    Node result = _NIL;
    Enumerator e = list.firstValue().enumerate_list();
    e.prefetch(_all);
    for(; e.has_more_p() ;) {
      Node the_class = e.next();
      Values2 res = coerce_to_frame(the_class, _NIL, kb_local_only_p);
      if (res.secondValue() != _NIL) 
	{ the_class = get_frame_handle(res.firstValue(), kb_local_only_p); }
      result = new Cons(the_class, result);
    }
    e.free();
    return result;
  }

  static final Values2 null_values2 = new Values2(_NIL, _NIL);
  static final Values3 null_values3 = new Values3(_NIL, _NIL, _NIL);

// ---------- default implementation of put-frame-details


/**
 * Redefines   frame  to have the specified   details .
 *   Details  is a property list as specified for
 *   get-frame-details .  This operation is useful for systems that allow
 * transaction-oriented editing of multiple aspects of a frame.
 * The properties    :handle ,    :frame-type , and    :primitive-p 
 * are ignored, since these may not be put.
 * Returns no values.
 */
  protected void put_frame_details_internal (Node frame, Node details, 
					     TruthValue kb_local_only_p) {
    frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
    Node name = Cons.getf(details, _name);
    put_frame_name(frame, name, kb_local_only_p);
    Node pretty_name = Cons.getf(details, _pretty_name);
    put_frame_pretty_name(frame, pretty_name, kb_local_only_p);
    Node types = Cons.getf(details, _types);
    put_instance_types(frame, types, kb_local_only_p);
    Node own_slots = Cons.getf(details, _own_slots);
    Node own_facets = Cons.getf(details, _own_facets);
    Node defined_pair = initialise_slots_and_facets
      (frame, own_slots, own_facets, _own, kb_local_only_p);
    Node template_slots = Cons.getf(details, _template_slots);
    Node template_facets = Cons.getf(details, _template_facets);
    Node subclasses = Cons.getf(details, _subclasses);
    Node superclasses = Cons.getf(details, _superclasses);
    if (class_p(frame, kb_local_only_p) != _NIL)
      { put_class_superclasses(frame, superclasses, kb_local_only_p);
	Node current_subclasses
	  = get_class_subclasses(frame, _direct, _all, 
				 kb_local_only_p).firstValue();
	Node subclasses_to_remove =
	  Cons.set_difference(current_subclasses, subclasses, this,
			      kb_local_only_p);
	Node subclasses_to_add =
	  Cons.set_difference(subclasses, current_subclasses, this,
			      kb_local_only_p);
	Enumeration removee = subclasses_to_remove.elements();
	while (removee.hasMoreElements()) {
	  Node s = (Node)(removee.nextElement());
	  put_class_superclasses(s, 
		  Cons.remove_eql(frame, 
			get_class_superclasses(s, _direct, _all,
					       kb_local_only_p).firstValue(),
				  this),
		  kb_local_only_p);
	  //remove_class_superclass(s, frame, kb_local_only_p);
	}
	Enumeration adde = subclasses_to_add.elements();
	while (adde.hasMoreElements()) {
	  Node s = (Node)(adde.nextElement());
	  add_class_superclass(s, frame, kb_local_only_p);
	}
	initialise_slots_and_facets(frame, template_slots, template_facets, 
				    _template, kb_local_only_p,
				    defined_pair.car(), defined_pair.cadr());
      }
    else if (template_slots == _NIL && template_facets == _NIL &&
	     superclasses == _NIL && subclasses == _NIL)
      {}
    else { throw new GenericError
	     (_T, new OKBCString(frame + " is not a class, so cannot have " +
				"template slots/facets or super/subclasses."))
	     .signal();
	 }
  }


  Node maybe_create_slot(Node frame, Node slot, SlotType slot_type, 
			 TruthValue kb_local_only_p) {
    if (slot_p(slot, kb_local_only_p) == _NIL)
      { if (slot instanceof AbstractSymbol)
	  { slot = create_slot(slot, frame, slot_type, _NIL, _NIL, _NIL, _NIL,
			       _NIL, _NIL, kb_local_only_p);
	  }
	else
	  { Values2 slotv = coerce_to_slot(slot, _NIL, kb_local_only_p);
	    Node sframe = slotv.firstValue();
	    Node found_p = slotv.secondValue();
	    if (found_p != _NIL) { slot = sframe; }
	    else
	      { Node name = _NIL;
		if (found_p != _NIL)
		  { name = get_frame_name(sframe, kb_local_only_p); }
		Node slot_name = name;
		if (Cons.member_eql
		    (_NIL, get_behavior_values(_frame_names_required), this,
		     kb_local_only_p))
		  { slot_name = Symbol.gentemp("TEMP-SLOT-NAME-", 
					       "FRAME-NAMES");
		  }
		Node handle = slot;
		if (found_p != _NIL) { handle = sframe; }
		slot = create_slot(slot_name, frame, slot_type, _NIL, _NIL,
				   _NIL, _NIL, handle, _NIL, kb_local_only_p);
	      }
	  }
      }
    attach_slot(frame, slot, slot_type, kb_local_only_p);
    return slot;
  }

  Node maybe_create_facet(Node frame, Node slot, Node facet, 
			  SlotType slot_type, TruthValue kb_local_only_p) {
    if (facet_p(facet, kb_local_only_p) == _NIL)
      { if (facet instanceof AbstractSymbol)
	  { facet = create_facet(facet, frame, slot, slot_type, _NIL, _NIL,
				 _NIL, _NIL, _NIL, _NIL, kb_local_only_p);
	  }
	else
	  { Values2 facetv = coerce_to_facet(facet, _NIL, kb_local_only_p);
	    Node fframe = facetv.firstValue();
	    Node found_p = facetv.secondValue();
	    if (found_p != _NIL) { facet = fframe; }
	    else
	      { Node name = _NIL;
		if (found_p != _NIL)
		  { name = get_frame_name(fframe, kb_local_only_p); }
		Node facet_name = name;
		if (Cons.member_eql
		    (_NIL, get_behavior_values(_frame_names_required), this,
		     kb_local_only_p))
		  { facet_name = Symbol.gentemp("TEMP-FACET-NAME-", 
						"FRAME-NAMES");
		  }
		Node handle = facet;
		if (found_p != _NIL) { handle = fframe; }
		facet = create_facet(facet_name, frame, slot, slot_type, _NIL,
				     _NIL, _NIL, _NIL, handle, _NIL,
				     kb_local_only_p);
	      }
	  }
      }
    attach_facet(frame, slot, facet, slot_type, kb_local_only_p);
    return facet;
  }


  Node initialise_slots_and_facets(Node frame, Node slot_specs,
				   Node facet_specs, SlotType slot_type, 
				   TruthValue kb_local_only_p) {
    return initialise_slots_and_facets(frame, slot_specs, facet_specs,
				       slot_type, kb_local_only_p, _NIL, _NIL);
  }


  Node initialise_slots_and_facets(Node frame, Node slot_specs,
				   Node facet_specs, SlotType slot_type, 
				   TruthValue kb_local_only_p,
				   Node defined_slot_alist,
				   Node defined_facet_alist) {
    Enumeration slotse = slot_specs.elements();
    while (slotse.hasMoreElements()) {
      Node slot_spec = (Node)(slotse.nextElement());
      Node slot = slot_spec;
      Node values = _NIL;
      if (slot_spec instanceof Cons)
	{ slot = slot_spec.car();
	  values = slot_spec.cdr();
	}
      if (slot == null || slot == _NIL)
	{ throw new SlotNotFound(frame, slot, _NIL, this).signal(); }
      Node known_slot = Cons.assoc(slot, defined_slot_alist).cadr();
      if (known_slot == null || known_slot == _NIL)
	{ Node s = maybe_create_slot(frame, slot, slot_type,
				     kb_local_only_p); 
	  defined_slot_alist = 
	    new Cons(Cons.list(slot, s), defined_slot_alist);
	  slot = s;
	}
      else { slot = known_slot; }
      if (values != _NIL && values != null)
	{ Node monotonic = _NIL;
	  Node dflt = _NIL;
	  Enumeration valuese = slot_spec.cdr().elements(); // discard slot.
	  while (valuese.hasMoreElements()) {
	    Node val = (Node)(valuese.nextElement());
	    if (val instanceof Cons && val.car() == _default)
	      { dflt = new Cons(val.cadr(), dflt); }
	    else { monotonic = new Cons(val, monotonic); }
	  }
	  if (monotonic != _NIL)
	    { put_slot_values(frame, slot, monotonic, slot_type, _known_true,
			      kb_local_only_p);
	    }
	  if (dflt != _NIL)
	    { 
	      if (get_behavior_values(_defaults) != _NIL)
		{ put_slot_values(frame, slot, dflt, slot_type, _default_only,
				  kb_local_only_p);
		}
	      else
		{ throw new GenericError
		    (_T, new OKBCString
		     ("Cannot assert default slot values for " + frame + 
		      " because defaults are not supported in " + this + "."))
		    .signal();
		}
	    }
	}
    }

    slotse = facet_specs.elements();
    while (slotse.hasMoreElements()) {
      Node slot_spec = (Node)(slotse.nextElement());
      Node slot = slot_spec;
      Node facet_specs_list = _NIL;
      if (slot_spec instanceof Cons)
	{ slot = slot_spec.car();
	  facet_specs_list = slot_spec.cdr();
	}
      if (slot == null || slot == _NIL)
	{ throw new SlotNotFound(frame, slot, _NIL, this).signal(); }
      Node known_slot = Cons.assoc(slot, defined_slot_alist).cadr();
      if (known_slot == null || known_slot == _NIL)
	{ Node s = maybe_create_slot(frame, slot, slot_type,
				     kb_local_only_p); 
	  defined_slot_alist = 
	    new Cons(Cons.list(slot, s), defined_slot_alist);
	  slot = s;
	}
      else { slot = known_slot; }

      Enumeration facetse = facet_specs_list.elements();
      while (facetse.hasMoreElements()) {
	Node facet_spec = (Node)(facetse.nextElement());
	Node facet = facet_spec;
	Node values = _NIL;
	if (facet_spec instanceof Cons)
	  { facet = facet_spec.car();
	    values = facet_spec.cdr();
	  }
	if (facet == null || facet == _NIL)
	  { throw new FacetNotFound(frame, slot, _NIL, facet, this).signal(); }
	Node known_facet = Cons.assoc(facet, defined_facet_alist).cadr();
	if (known_facet == null || known_facet == _NIL)
	  { Node fa = maybe_create_facet(frame, slot, facet, slot_type,
					 kb_local_only_p);
	    defined_facet_alist = 
	      new Cons(Cons.list(facet, fa), defined_facet_alist);
	    facet = fa;
	  }
	else { facet = known_facet; }
	if (values != _NIL && values != null)
	  { Node monotonic = _NIL;
	    Node dflt = _NIL;
	    Enumeration valuese = values.elements();
	    while (valuese.hasMoreElements()) {
	      Node val = (Node)(valuese.nextElement());
	      if (val instanceof Cons && val.car() == _default)
		{ dflt = new Cons(val.cadr(), dflt); }
	      else { monotonic = new Cons(val, monotonic); }
	    }
	    if (monotonic != Node._NIL)
	      { put_facet_values(frame, slot, facet, monotonic, slot_type,
				 _known_true, kb_local_only_p);
	      }
	    if (dflt != Node._NIL)
	      { 
		if (get_behavior_values(_defaults) != _NIL)
		  { put_facet_values(frame, slot, facet, dflt, slot_type,
				     _default_only, kb_local_only_p);
		  }
		else
		  { throw new GenericError
		      (_T, new OKBCString
		       ("Cannot assert default slot values for " + frame + 
			" because defaults are not supported in " + 
			this + "."))
		      .signal();
		  }
	      }
	  }
      }
    }
    return Cons.list(defined_slot_alist, defined_facet_alist);
  }


  /********* This is now discredited.  We use initialise_slots_and_facets now.
  void assert_slots_and_facets (Node frame, Node slots, Node facets,
				SlotType slot_type,
				TruthValue kb_local_only_p) {
    Enumeration slotse = slots.elements();
    while (slotse.hasMoreElements()) {
      Node spec = Cons.list_if_not((Node)(slotse.nextElement()));
      Node slot = spec.car();
      Node values = spec.cdr();
      if (values == null) { values = _NIL; }
      Node monotonic = _NIL;
      Node deflt = _NIL;
      Enumeration valuese = values.elements();
      while (valuese.hasMoreElements()) {
	Node val = (Node)(valuese.nextElement());
	if ((val instanceof Cons) &&
	    (val.car() == _default))
	  { deflt = new Cons(val, deflt); }
	else { monotonic = new Cons(val, monotonic); }
      }
      put_slot_values(frame, slot, monotonic, slot_type, _known_true, 
		      kb_local_only_p);
      put_slot_values(frame, slot, deflt, slot_type, _default_only, 
		      kb_local_only_p);
    }

    Enumeration facetse = facets.elements();
    while (facetse.hasMoreElements()) {
      Node spec = Cons.list_if_not((Node)(facetse.nextElement()));
      Node slot = spec.car();
      Node facet_specs = spec.cdr();
      Enumeration fspece = facet_specs.elements();
      while(fspece.hasMoreElements()) {
	Node fspec = Cons.list_if_not((Node)(fspece.nextElement()));
	Node facet = fspec.car();
	Node values = fspec.cdr();
	if (values == null) { values = _NIL; }
	Node monotonic = _NIL;
	Node deflt = _NIL;
	Enumeration valuese = values.elements();
	while (valuese.hasMoreElements()) {
	  Node val = (Node)(valuese.nextElement());
	  if ((val instanceof Cons) &&
	      (val.car() == _default))
	    { deflt = new Cons(val, deflt); }
	  else { monotonic = new Cons(val, monotonic); }
	}
	put_facet_values(frame, slot, facet, monotonic, slot_type,
			 _known_true, kb_local_only_p);
	put_facet_values(frame, slot, facet, deflt, slot_type,
			 _default_only, kb_local_only_p);
      }
    }
  }
  ***********/

  protected Values2 copy_frame_forward
     (Node frame, Node new_name, Kb to_kb, TruthValue error_p,
      Node missing_frame_action, HashtableNode frame_handle_mapping_table,
      TruthValue kb_local_only_p) {
       return to_kb.copy_frame_forward_2
	 (frame, new_name, this, error_p, missing_frame_action,
	  frame_handle_mapping_table, kb_local_only_p);
     }

  protected Values2 copy_frame_forward_2
     (Node frame, Node new_name, Kb from_kb, TruthValue error_p,
      Node missing_frame_action, HashtableNode frame_handle_mapping_table,
      TruthValue kb_local_only_p) {
       return copy_frame(frame, new_name, from_kb, this, error_p,
			 missing_frame_action, frame_handle_mapping_table,
			 kb_local_only_p);
     }

  Node allocate_any_necessary_frame_handles
     (Node form, HashtableNode frame_handle_mapping_table, 
      TruthValue kb_local_only_p, Kb to_kb,
      Node missing_frame_action, TruthValue error_p, Node for_frame,
      ConsLocation allocated_frame_handle_alist) {
    if (form instanceof Cons)
      { Node car_result =
	   allocate_any_necessary_frame_handles
			(form.car(), frame_handle_mapping_table, 
			 kb_local_only_p, to_kb, missing_frame_action, 
			 error_p, for_frame, allocated_frame_handle_alist);
	if (car_result == missing_frame_tag)
	  { return allocate_any_necessary_frame_handles
			(form.cdr(), frame_handle_mapping_table, 
			 kb_local_only_p, to_kb, missing_frame_action, 
			 error_p, for_frame, allocated_frame_handle_alist);
	  }
	else { return new Cons(car_result,
			       allocate_any_necessary_frame_handles
			       (form.cdr(), frame_handle_mapping_table, 
				kb_local_only_p, to_kb, missing_frame_action, 
				error_p, for_frame, 
				allocated_frame_handle_alist));
	     }
      }
    else if (form == _NIL ||
	     form == null ||
	     form instanceof OKBCInteger ||
	     form instanceof Real ||
	     form instanceof Kb ||
	     form instanceof AbstractSymbol)
      { return form; }
    else 
      { Node entry = frame_handle_mapping_table.get(form);
	if (entry != null) { return entry; }
	if (this == to_kb) { return form; }
	Node frame = null;
	if (form instanceof FrameHandle)
	  { frame = form; }
	else { Values2 fv = coerce_to_frame(form, _NIL, kb_local_only_p);
	       if (fv.secondValue() != _NIL)
		 { frame = get_frame_handle(form, kb_local_only_p); }
	     }
	if (frame == _NIL) { return form; }
	Node name = get_frame_name(frame, kb_local_only_p);
	ContextName type = (ContextName)(get_frame_type(frame,
							kb_local_only_p));
	Node key = Cons.list(name, frame, type);
	if (missing_frame_action == _allocate)
	  { Node existing = frame_handle_mapping_table.get(key);
	    if (existing != null) { return existing; }
	    else 
	      { Node handle = to_kb.allocate_frame_handle(name, type, frame);
		frame_handle_mapping_table.put(key, handle);
		frame_handle_mapping_table.put(handle, key);
		allocated_frame_handle_alist.addElement
		  (Cons.list(frame, handle));
		return handle;
	      }
	  }
	else if (missing_frame_action == _stop ||
		 missing_frame_action == _abort)
	  { if (error_p != _NIL)
	      { throw new MissingFrames
		(Cons.list(frame), for_frame, this).signal();
	      }
	    else
	      { throw new StopCopy
		(new Values4(_NIL,
			     allocated_frame_handle_alist.cons,
			     missing_frame_action,
			     Cons.list(frame)));
	      }
	  }
	else { throw new GenericError
	        (_T, new OKBCString("Illegal missing frame action: " +
				   missing_frame_action)).signal();
	     }
      }
  }


  Node find_or_create_frame_handle_for_symbol
    (Node symbol, ContextName frame_type, 
     HashtableNode frame_handle_mapping_table,
     ConsLocation allocated_frame_handle_alist) {
      Node key = Cons.list(symbol, _NIL, frame_type);
      Node existing = (Node)frame_handle_mapping_table.get(key);
      if (existing != null) { return existing; }
      else { Node inst = allocate_frame_handle(symbol, frame_type, _NIL);
	     frame_handle_mapping_table.put(key, inst);
	     frame_handle_mapping_table.put(inst, key);
	     allocated_frame_handle_alist.addElement(Cons.list(symbol, inst));
	     return inst;
	   }
    }

  boolean slots_are_reified_p () {
    return Cons.member_eql(_slot, get_behavior_values(_are_frames), this, _NIL);
  }


  boolean facets_are_reified_p () {
    return Cons.member_eql(_facet, get_behavior_values(_are_frames),
			   this, _NIL);
  }


  Values2 get_slot_specification(Node frame, Node slots, SlotType slot_type,
				InferenceLevel inference_level,
				Node number_of_values,
				TruthValue kb_local_only_p)  {
       Node results = _NIL;
       boolean inexact_p = false;
       Enumeration e = slots.elements();
       for (; e.hasMoreElements() ;) {
	 Node slot = (Node)(e.nextElement());
	 Values3 kv = get_slot_values(frame, slot, inference_level, slot_type,
				      number_of_values, _known_true,
				      kb_local_only_p);
	 if (kv.secondValue() == _NIL) { inexact_p = true; }
	 Values3 dv = get_slot_values(frame, slot, inference_level, slot_type,
				      number_of_values, _default_only,
				      kb_local_only_p);
	 if (dv.secondValue() == _NIL) { inexact_p = true; }
	 Node values = _NIL;
	 Enumerator evalues = kv.firstValue().enumerate_list();
	 for(; evalues.has_more_p() ;) {
	   Node val = evalues.next();
	   values = new Cons(Cons.copy_tree(val), values);
	 }
	 evalues.free();
	 evalues = dv.firstValue().enumerate_list();
	 for(; evalues.has_more_p() ;) {
	   Node val = evalues.next();
	   values = new Cons(Cons.list(_default, Cons.copy_tree(val)),
			     values);
	 }
	 evalues.free();
	 results = new Cons(new Cons(slot, values), results);
       }
       if (inexact_p) { return new Values2(results, _NIL); }
       else { return new Values2(results, _T); }
     }

  /************ Obsolete now !!
  Node validate_facet_spec(Node frame, Node slots, SlotType slot_type,
			   Kb to_kb, TruthValue kb_local_only_p, 
			   NodeLocation loc) {
       Node facet_spec = _NIL;
       if (Cons.member_eql(_user_defined_facets,
			   get_behavior_values(_compliance), this,
			   kb_local_only_p))
	 {
	   facet_spec = get_facet_specification(frame, slots, slot_type,
						_direct, _all, 
						kb_local_only_p).firstValue();
	   if (Cons.member_eql(_user_defined_facets,
			       to_kb.get_behavior_values(_compliance), this,
			       kb_local_only_p))
	     {}
	   else
	     { if (frame != loc.node)
		 { System.out.println
		   ("Warning --  Facets not copied for frame " +
		    get_frame_pretty_name(frame, kb_local_only_p) +
		    " because facets are not supported by " +
		    to_kb);
		   loc.node = frame;
		 }
	       facet_spec = _NIL;
	     }
	 }
       return facet_spec;
     }

     ***********/
 
  Values2 get_facet_specification(Node frame, Node slots, SlotType slot_type,
				  InferenceLevel inference_level,
				  Node number_of_values,
				  TruthValue kb_local_only_p) {
       Node results = _NIL;
       boolean inexact_p = false;
       Enumeration e = slots.elements();
       for (; e.hasMoreElements() ;) {
	 Node slot = (Node)(e.nextElement());
	 Node facet_spec = _NIL;
	 Node facets = get_slot_facets
	   (frame, slot, inference_level, slot_type, 
	    kb_local_only_p).firstValue();
	 Enumeration f = facets.elements();
	 for (; f.hasMoreElements() ;) {
	   Node facet = (Node)(f.nextElement());
	   Values3 kv = get_facet_values (frame, slot, facet, inference_level,
					  slot_type, number_of_values, 
					  _known_true, kb_local_only_p);
	   if (kv.secondValue() == _NIL) { inexact_p = true; }
	   Values3 dv = get_facet_values (frame, slot, facet, inference_level,
					  slot_type, number_of_values, 
					  _default_only, kb_local_only_p);
	   if (dv.secondValue() == _NIL) { inexact_p = true; }
	   Node values = _NIL;
	   Enumerator evalues = kv.firstValue().enumerate_list();
	   for(; evalues.has_more_p() ;) {
	     Node val = evalues.next();
	     values = new Cons(Cons.copy_tree(val), values);
	   }
	   evalues.free();
	   evalues = dv.firstValue().enumerate_list();
	   for(; evalues.has_more_p() ;) {
	     Node val = evalues.next();
	     values = new Cons(Cons.list(_default, Cons.copy_tree(val)),
			       values);
	   }
	   evalues.free();
	   facet_spec = new Cons(new Cons(facet, values), facet_spec);
	 }
	if (facet_spec != _NIL)
	  { results = new Cons(new Cons(slot, facet_spec), results); }
       }
       if (inexact_p) { return new Values2(results, _NIL); }
       else { return new Values2(results, _T); }
     }


// ---------- default implementation of follow-slot-chain


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
  protected Node follow_slot_chain_internal
     (Node frame, Node slot_chain, Node union_multiple_values, 
      InferenceLevel inference_level, ValueSelector value_selector,
      TruthValue kb_local_only_p) {
       if (slot_chain == _NIL || slot_chain == null)
	 { if (union_multiple_values != _NIL)
	     { return Cons.list(frame); }
	   else { return frame; }
	 }
       Node values = get_slot_values(frame, slot_chain.car(), 
				     inference_level, _own,
				     _all, value_selector,
				     kb_local_only_p).firstValue();
       if (slot_chain.cdr() == _NIL || slot_chain.cdr() == null)
	 { if (union_multiple_values != _NIL)
	     { return values; }
	   else if (values.cdr() == _NIL || values.cdr() == null)
	     { return values.car(); }
	   else 
	     { throw new CardinalityViolation
	       (_NIL, frame, slot_chain.car(),
		_own, _NIL, this, _T, 
		new OKBCString("Frame "+frame+" has too many "+
			      "values for slot "+slot_chain.car())).signal();
	     }
	 }
       else
	 {
	   int values_length = values.length();
	   if (values_length == 1)
	     { return follow_slot_chain(coerce_to_frame
					(values.car(), _T, 
					 kb_local_only_p).firstValue(),
					slot_chain.cdr(),
					union_multiple_values,
					inference_level, value_selector,
					kb_local_only_p);
	     }
	   else if ((values_length > 1) && union_multiple_values != _NIL)
	     { Enumeration e = values.elements();
	       Node results = _NIL;
	       for (; e.hasMoreElements() ;) {
		 Node value = (Node)(e.nextElement());
		 results = Cons.append_cons
		   (results,
		   follow_slot_chain(coerce_to_frame
				      (value, _T,
				       kb_local_only_p).firstValue(),
				      slot_chain.cdr(), _T,
				      inference_level, value_selector,
				      kb_local_only_p));
	       }
	       return Cons.remove_duplicates
		 (results, _eql, this, kb_local_only_p);
	     }
	   else if (values_length > 1)
	     { throw new CardinalityViolation
	       (_NIL, frame, slot_chain.car(),
		_own, _NIL, this, _T, 
		new OKBCString("Frame "+frame+" has too many "+
			      "values for slot "+slot_chain.car())).signal();
	     }
	   else
	     { throw new CardinalityViolation
	       (_NIL, frame, slot_chain.car(),
		_own, _NIL, this, _T, 
		new OKBCString("Frame "+frame+" has no "+
			      "values for slot "+slot_chain.car())).signal();
	     }
	 }
     }

// ---------- default implementation of print-frame


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
  protected Node print_frame_internal
     (Node frame, Node slots, Node facets, Node stream, 
      InferenceLevel inference_level, Node number_of_values,
      ValueSelector value_selector, TruthValue kb_local_only_p) {
       GenericPrinter str = new GenericPrinter(stream);
       print_frame_1(frame, slots, facets, str, inference_level,
		     number_of_values, value_selector, kb_local_only_p);
       if (stream == _NIL)
	 { return str.extract_string(); }
       else { return _NIL; }
     }
       
  protected Node print_frame_1 (Node frame, Node slots, Node facets,
			     GenericPrinter stream,
			     InferenceLevel inference_level,
			     Node number_of_values,
			     ValueSelector value_selector,
			     TruthValue kb_local_only_p) {
       Node print_slots = _NIL;
       Node print_facets = _NIL;
       stream.println();
       stream.println();
       stream.print("___ ");
       if (class_p (frame, kb_local_only_p) == _NIL)
	 { stream.print("Instance "); }
       else { stream.print("Class "); }
       stream.print(get_frame_pretty_name(frame, kb_local_only_p));
       stream.print(" ___");
       stream.println();
       if (class_p(frame, kb_local_only_p) == _NIL)
	 { Node types = get_instance_types 
	   (frame, inference_level, number_of_values,
	    kb_local_only_p).firstValue();
	   output_list("    Types:  ", types, kb_local_only_p, true, stream);
	 }
       else
	 { Node superclasses = get_class_superclasses
	     (frame, inference_level, number_of_values,
	      kb_local_only_p).firstValue();
	   output_list("    Superclasses:  ", superclasses, kb_local_only_p,
		       true, stream);
	   Node subclasses = get_class_subclasses 
	     (frame, inference_level, number_of_values, 
	      kb_local_only_p).firstValue();
	   output_list("    Subclasses:  ", subclasses, kb_local_only_p, true,
		       stream);
	   Node instances = get_class_instances
	     (frame, inference_level, number_of_values,
	      kb_local_only_p).firstValue();
	   output_list("    Instances:  ", instances, kb_local_only_p, true,
		       stream);
	 }

       if (Cons.listp(slots))
	 { print_slots
	     = Cons.intersection_equal(slots, get_frame_slots
				       (frame, inference_level, _all,
					kb_local_only_p).firstValue(), this);
	 }
       else { if (slots == _none) { print_slots = _NIL; }
              else if (slots == _all || slots == _filled)
		{ print_slots = get_frame_slots(frame, inference_level, _all,
						kb_local_only_p).firstValue();
		}
	       else { throw new RuntimeException 
		      ("Illegal print-frame slot type " + slots);
		    }
	      }
       Node slot_specs 
	 = Cons.list
	 (Cons.list(_own, new OKBCString("\nOwn slots:\n")),
	  Cons.list(_template, new OKBCString("\nTemplate slots:\n")));
       Enumeration eslot_type = slot_specs.elements();
       for (; eslot_type.hasMoreElements() ;) {
	 Node pair = (Node)(eslot_type.nextElement());
	 SlotType slot_type = (SlotType)(pair.car());
	 Node string = pair.cdr().car();
	 stream.print(string);
	 print_slots = sort_by_frame_name(print_slots, kb_local_only_p);

	 Enumeration e = print_slots.elements();
	 for (; e.hasMoreElements() ;) {
	   Node slot = (Node)(e.nextElement());
	   Node slot_label_node = get_frame_pretty_name (slot, kb_local_only_p);
	   if (slot_label_node == _NIL) { slot_label_node = slot; }
	   String slot_label = "  " + slot_label_node + ": ";
	   boolean print_slot_p = 
	     slots != _filled ||
	     slot_has_value_p(frame, slot, inference_level, slot_type,
			      value_selector, kb_local_only_p).firstValue()
				!= _NIL;
	   if (print_slot_p)
	     { stream.println();
	       output_list(slot_label,
			   get_slot_values
			   (frame, slot, inference_level, slot_type, 
			    number_of_values, value_selector,
			    kb_local_only_p).firstValue(),
			   kb_local_only_p, stream);
	       stream.println();
	       if (Cons.member_eql(_user_defined_facets,
				   get_behavior_values(_compliance), this,
				   kb_local_only_p))
		 { 
		   if (Cons.listp(facets))
		     { print_facets
			 = Cons.intersection_equal
			 (facets, get_slot_facets
			  (frame, slot, inference_level, slot_type,
			   kb_local_only_p).firstValue(), this);
		     }
		   else
		     { if (facets == _none) { print_facets = _NIL; }
		       else if (facets == _all || facets == _filled)
			 { print_facets = 
			     get_slot_facets(frame, slot, inference_level,
					     slot_type, 
					     kb_local_only_p).firstValue();
			 }
		       else { throw new RuntimeException 
				("Illegal print-frame facet type " + facets);
			    }
		     }
	  
		   print_facets = 
		     sort_by_frame_name(print_facets, kb_local_only_p);
		   Enumeration f = print_facets.elements();
		   for (; f.hasMoreElements() ;) {
		     Node facet = (Node)(f.nextElement());
		     Node facet_label_node = 
		       get_frame_pretty_name (facet, kb_local_only_p);
		     if (facet_label_node == _NIL)
		       { facet_label_node = facet; }
		     String facet_label = "\n    >>" + facet_label_node + ": ";
		     Node fvalues = get_facet_values
		       (frame, slot, facet, inference_level,
			slot_type, number_of_values, value_selector,
			kb_local_only_p).firstValue();
		     if (facets != _filled || fvalues != _NIL)
		       { output_list(facet_label, fvalues, kb_local_only_p,
				     stream); 
		       }
		   }
		 }
	     }
	 }
       }
       stream.println();
       return _NIL;
     }


  String pretty_string_for_sort (Node frame, TruthValue kb_local_only_p) {
    if (coercible_to_frame_p(frame, kb_local_only_p) != _NIL)
	{ Node pretty_name = get_frame_pretty_name (frame, kb_local_only_p);
	  if (pretty_name == _NIL)
	    { return frame.toString(); }
	  else { return ((OKBCString)pretty_name).string; }
	}
    else { return frame.toString(); }
  }


  Node sort_by_frame_name (Node list, TruthValue kb_local_only_p) {
    if (list == _NIL || list == null) { return _NIL; }
    else if (list.cdr() == _NIL || list.cdr() == null) { return list; }
    else { boolean switch_flag = false;
	   for(int i=0; i < list.length(); i++) {
	     if (switch_flag) { break; }
	     switch_flag = true;
	     Node tail = list;
	     for (; (tail.cdr() != _NIL && tail.cdr() != null) ;) {
	       Node car = tail.car();
	       Node cadr = tail.cdr().car();
	       String car_string = pretty_string_for_sort
		 (car, kb_local_only_p);
	       String cadr_string = pretty_string_for_sort
		 (cadr, kb_local_only_p);
	       if (cadr_string.compareTo(car_string) < 0)
		 { ((Cons)tail.cdr()).set_car(car);
		   ((Cons)tail).set_car(cadr);
		   switch_flag = true;
		 }
	       tail = tail.cdr();
	     }
	   }
	   return list;
	 }
  }

    
  void output_list (String label, Node xx, TruthValue kb_local_only_p,
		    GenericPrinter stream) {
       output_list(label, xx, kb_local_only_p, false, stream);
     }


  void output_list (String label, Node xx, TruthValue kb_local_only_p,
		    boolean newline, GenericPrinter stream) {
       int position = label.length();
       String indent = "";
       for(int i=0; i < position; i++) {
	 indent = indent + " ";
       }
       stream.print(label);
       if (!Cons.listp(xx))
	 { stream.print(".");
	   xx = Cons.list(xx);
	 }
       int len = xx.length();
       Enumeration e = xx.elements();
       int i = 0;
       for (; e.hasMoreElements() ;) {
	 Node x = (Node)(e.nextElement());
	 Values2 xv = coerce_to_frame(x, _NIL, kb_local_only_p);
	 if (xv.secondValue() != _NIL)
	   { x = get_frame_pretty_name(xv.firstValue(), kb_local_only_p); }
	 i = i + 1;
	 position =
	   break_output_stream(position,
			       x.toString() + (i == len ? "" :  ", "),
			       indent, i == 1, stream);
       }
       if (newline) { stream.println(); }
     }


  int break_output_stream (int position, String string, String prepend,
			   boolean first_p, GenericPrinter stream) {
    int width_limit = 75;
    int test_position = 0;
    if (position == -1)
      { stream.print(prepend);
	position = 0;
      }
    test_position = compute_position(position, string);
    if (width_limit > test_position || first_p)
      { stream.print(string);
	position = test_position;
      }
    else
      {
	string = prepend + string;
	stream.println();
	stream.print(string);
	position = compute_position(0, string);
      }
    return position;
  }

  int compute_position (int start_position, String string) {
    int position = start_position;
    for (int i = 0; i < string.length(); i++) {
      if (string.charAt(i) == '\t')
	{ position = 8 * ((position + 1) / 8);
	  position = position + 1;
	}
    }
    return position;
  }

// ---------- default implementation of coerce-to-kb-value


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
  protected Values3 coerce_to_kb_value_internal
     (Node string_or_value, ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p,
      Node frame_action,
      TruthValue kb_local_only_p) {
       // HACK ALERT -- WE ARE NOT HANDLING THE TARGET CONTEXT YET.
       Node form = null;
	   if (string_or_value instanceof OKBCString)
       {
         try {
           form = LispReader.readFromString
                  (((OKBCString)string_or_value).string, ioSyntax.io_package);
         }
         catch (Exception e) {}
	   }
       else form = string_or_value;
       if (form == null)
       { 
         if (error_p == _NIL) { return new Values3(_NIL, _NIL, _NIL); }
         else throw new KbValueReadError(string_or_value, this).signal();
	   }
       else if ((form instanceof Cons) && 
         ((target_context==_frame) ||
          (target_context==_slot) ||
          (target_context==_class) ||
          (target_context==_facet)))
       {
         throw new KbValueReadError(string_or_value, this, _T,
			 new OKBCString(
            "Compound value "+string_or_value+" found for " +
            "string-or-value argument, but this is not acceptable " +
            "for target-context = " + target_context)).signal();
       }
       else
       {
         if (!(string_or_value instanceof OKBCString) &&
              (frame_action==_error_if_not_unique) &&
	     ((target_context==_frame) ||
	      (target_context==_slot) ||
	      (target_context==_class) ||
	      (target_context==_facet)))
         {
           Values2 v = ((target_context==_frame) ?
			coerce_to_frame(form, error_p, kb_local_only_p) :
			((target_context==_slot) ?
			 coerce_to_slot(form, error_p, kb_local_only_p) :
			 ((target_context==_facet) ?
			  coerce_to_facet(form, error_p, kb_local_only_p) :
			  (target_context==_class ?
			   coerce_to_class(form, error_p, kb_local_only_p) :
			   (target_context==_individual ?
			    coerce_to_individual(form, error_p,
						 kb_local_only_p) :
			    null)))));
	   return new Values3(v.firstValue(), v.secondValue(), _NIL);
         }
		 // The following is the '(doit)' call.
       if (frame_action == _do_not_coerce_to_frames)
       { return new Values3(form, _T, _NIL); }
       else if (frame_action == _error_if_not_unique ||
                frame_action == _must_name_frames ||
                frame_action == _options_if_not_unique)
	 {  NodeLocation alist_loc = new NodeLocation(_NIL);
	    Node interned = null;
	    if (error_p != _NIL)
	      { interned = intern_frames_in
		  (form, frame_action, wildcards_allowed_p,
		   force_case_insensitive_p, kb_local_only_p, 
		   target_context, alist_loc);
	      }
	    else { try { interned = intern_frames_in
			   (form, frame_action, wildcards_allowed_p,
			    force_case_insensitive_p, kb_local_only_p, 
			    target_context, alist_loc);
		       }
		   catch (Exception e) {}
		 }
	    boolean error_found_p = (interned==null);
	    if (error_found_p)
	      { return new Values3(_NIL, _NIL, _NIL); }
	    else if (alist_loc.node != _NIL)
	      { return new Values3(_NIL, _T, alist_loc.node); }
	    else 
	      { return new Values3(interned, _T, _NIL); }
	  }
       else { throw new RuntimeException 
	      ("Illegal frame action " + frame_action);
	    }
       }
     }


  Node intern_frames_in (Node form, Node frame_action,
			   Node wildcards_allowed_p,
			   Node force_case_insensitive_p,
			   TruthValue kb_local_only_p,
			   ContextName target_context,
			   NodeLocation alist_loc) {
       if (form instanceof Cons)
	 { Cons cform = (Cons) form;
	   return new Cons(intern_frames_in(cform.car(), frame_action,
					    wildcards_allowed_p,
					    force_case_insensitive_p,
					    kb_local_only_p, target_context,
					    alist_loc),
			   intern_frames_in(cform.cdr(), frame_action,
					    wildcards_allowed_p,
					    force_case_insensitive_p,
					    kb_local_only_p, target_context,
					    alist_loc));
	 }
       else if (form == _NIL || form == null) { return _NIL; }
       else if (form instanceof OKBCString || form instanceof AbstractSymbol)
	 { 
	   Node selector = target_context;
	   if (target_context == _value ||
	       target_context == _frame)
	     { selector = _all; }
	   if (frame_action == _must_name_frames)
	     { Values2 res;
		   // ?: operator can't handle different types
		   if (target_context==_value||target_context==_frame)
		   {  res = get_frames_matching
					(form, wildcards_allowed_p, selector, 
					force_case_insensitive_p,
					 kb_local_only_p); }
		   else 
		   {  res = get_frames_matching
			        (form, wildcards_allowed_p, selector, 
				 force_case_insensitive_p,
				 kb_local_only_p); }
	       Node matches = res.firstValue();
	       if (matches.length() == 0) 
		 { throw new NotCoercibleToFrame(form, this).signal(); }
	       else if (matches.length() == 1) { return matches.car(); }
	       else { throw new NotUniqueError
		      (form, matches, target_context, this).signal(); 
		    }
	     }
	   else if (frame_action == _error_if_not_unique ||
		    frame_action == _options_if_not_unique)
	     { Node entry = Cons.assoc(form, alist_loc.node);
	       if (entry == _NIL || entry == null)
		 { Values2 res;
		   if (target_context==_value||target_context==_frame)
		   {  res = get_frames_matching
					(form, wildcards_allowed_p, _all, 
					 force_case_insensitive_p,
					 kb_local_only_p); }
		   else 
		   {  res = get_frames_matching
			        (form, wildcards_allowed_p, target_context, 
				 force_case_insensitive_p, kb_local_only_p); }
		   Node matches = res.firstValue();
		   Node substring = res.nthValue(1);
		   if (matches.length() == 0) { return form; }
		   else if (matches.length() == 1) { return matches.car(); }
		   else 
		     { if (frame_action == _error_if_not_unique)
			 { throw new NotUniqueError
			   (form, matches, target_context, this).signal();
			 }
		       else { alist_loc.node =
			      new Cons(new Cons(form, new Cons(substring,
							       matches)),
				       alist_loc.node);
			    }
		       return _NIL;
		     }
		 }
	       else { return entry.car(); }
	     }
	 }
       else { return form; }
       return form;		// shouldn't be reachable
     }

  boolean pattern_matches_p (Node canonical_pattern,
			     Object string,
			     Node force_case_insensitive_p) {
    String s = null;
    if (string instanceof String) { s = (String)string; }
    else if (string instanceof OKBCString)
      { s = ((OKBCString)string).string; }
    else { s = ((Symbol)string).symbolName; }
    return word_matches_pattern(s, canonical_pattern, 0,
				(force_case_insensitive_p != _NIL
				 ? false
				 : (_upcase == readtable_case)),
				true);
  }

  char single_char_wildcard = '?';
  char multiple_char_wildcard = '*';

  Node canonicalise_pattern (String pattern_string) {
    if (pattern_string.equals("")) { return _NIL; }
    else { String initial_string = "";
	   int wildcard_index = 0;
	   Symbol type = null;
	   for(int index=0; index < pattern_string.length(); index++) {
	     char ch = pattern_string.charAt(index);
	     Symbol entry = null;
	     if (ch == multiple_char_wildcard) { entry = _multiple; }
	     else if (ch == single_char_wildcard) { entry = _single; }
	     if (entry != null)
	       { wildcard_index = index;
		 type = entry;
		 break;
	       }
	     if (ch == '\\')
	       { index = index + 1;
		 if (index >= pattern_string.length()) {}
		 else { initial_string = initial_string + 
			pattern_string.charAt(index); }
	       }
	     else { initial_string = initial_string + ch; }
	   }
	   if (type != null)
	     { if (wildcard_index > 0)
		 { return new Cons(new OKBCString(initial_string),
				   new Cons(type,
					    canonicalise_pattern
					    (pattern_string.substring
					     (wildcard_index + 1))));
		 }
	       else { return new Cons(type,
				      canonicalise_pattern
				      (pattern_string.substring
				       (wildcard_index + 1)));
		    }
	     }
	   else { return Cons.list(new OKBCString(initial_string)); }
	 }
  }

  protected boolean word_matches_pattern
    (String string, Node pattern) {
      return word_matches_pattern(string, pattern, 0, false, true);
    }

  protected boolean word_matches_pattern
    (String string, Node pattern, int start, boolean case_sensitive_p,
     boolean force_match_to_end_p) {
      boolean ret = false;
      if (pattern == _NIL || pattern == null) { ret = true; }
      else { Node car = pattern.car();
	     if (car == _single)
	       { ret = string.length() >= start &&
		 word_matches_pattern(string, pattern.cdr(), start + 1,
				      case_sensitive_p, force_match_to_end_p);
	       }
	     else if (car == _multiple)
	       { if ((string.length() - start) == 0) { ret = true; }
		 else 
		   { ret = false;
		     for(int spread=0; spread < (string.length() - start);
			 spread++) {
		       if (word_matches_pattern
			   (string, pattern.cdr(), start + spread,
			    case_sensitive_p, force_match_to_end_p))
			 { ret = true; break; }
		     }
		   }
	       }
	     else
	       { String pattern_car_string = ((OKBCString)pattern.car()).string;
		 if (force_match_to_end_p && 
		     (pattern.cdr() == _NIL || pattern.cdr() == null))
		   { ret = 
		     ((string.length() - start)
		      == pattern_car_string.length())&&
		     string.regionMatches
		        (!case_sensitive_p, start,
			 pattern_car_string, 0, string.length() - start);
		     // System.out.println("compare(" + string + ", " + pattern_car_string + ") ->" + ret);
		   }
		 else 
		   { int pattern_car_length
		     = pattern_car_string.length();
		     ret = (string.length() >= start + pattern_car_length &&
			     string.regionMatches
			     (!case_sensitive_p, start,
			      pattern_car_string,
			      0, start + pattern_car_length) &&
			     word_matches_pattern
			     (string, pattern.cdr(), 
			      start + pattern_car_length,
			      case_sensitive_p, force_match_to_end_p));
		   }
	       }
	   }
      // System.out.println("word_matches_pattern(" + string + ", " + pattern + ", " + start + ", " + force_match_to_end_p + ") ->" + ret);
   return ret;
    }

  Node possibilities_for_pattern_matching
     (Node thing, TruthValue kb_local_only_p) {
       Values2 vals = coerce_to_frame(thing, _NIL, kb_local_only_p);
       Node frame = vals.firstValue();
       Node found_p = vals.secondValue();
       if (found_p != _NIL)
	 { Node name = get_frame_name(frame, kb_local_only_p);
	   Node pretty = get_frame_pretty_name(frame, kb_local_only_p);
	   Node handle = get_frame_handle(frame, kb_local_only_p);
	   // System.out.println("Possibilities for " + frame + " = " + Cons.remove_eql(_NIL, Cons.list(name, pretty, handle), this));
	   return Cons.remove_eql(_NIL, Cons.list(name, pretty, handle), this);
	 }
       else { return Cons.list(thing); }
     }


  protected boolean frame_matches_pattern_p
    (Node frame, String pattern, Node wildcards_allowed_p, 
     Node force_case_insensitive_p, TruthValue kb_local_only_p) {
       Node reps = possibilities_for_pattern_matching(frame, kb_local_only_p);
       if (wildcards_allowed_p != _NIL)
	 { Node canonical_pattern = canonicalise_pattern(pattern);
	   Enumeration e = reps.elements();
	   boolean ret = false;
	   for (; e.hasMoreElements() ;) {
	     Node rep = (Node)(e.nextElement());
	     if (rep instanceof OKBCString || rep instanceof AbstractSymbol)
	       { if (pattern_matches_p(canonical_pattern, rep,
				       force_case_insensitive_p))
		   { ret = true; break;}
	       }
	     else if (pattern_matches_p
		      (canonical_pattern, rep.toString(),
		       force_case_insensitive_p))
	       { ret = true; break;}
	   }
	   // System.out.println("frame_matches_pattern_p(" + frame + ", " + pattern + ", " + wildcards_allowed_p + ") ->" + ret);
	   return ret;
	 }
       else { Enumeration e = reps.elements();
	      for (; e.hasMoreElements() ;) {
		Node rep = (Node)(e.nextElement());
		String pat = pattern;
		boolean res = false;
		if (rep instanceof AbstractSymbol)
		  { if (force_case_insensitive_p != _NIL)
		      { res = pat.equalsIgnoreCase(((Symbol)rep).symbolName);}
		    else { res = pat.equals(((Symbol)rep).symbolName);}
		  }
		else if (rep instanceof OKBCString)
		  { if (force_case_insensitive_p != _NIL)
		      { res = pat.equalsIgnoreCase(((OKBCString)rep).string); }
		    else { res = pat.equals(((OKBCString)rep).string); }
		  }
		else
		  { if (force_case_insensitive_p != _NIL)
		      { res = pat.equalsIgnoreCase(rep.toString()); }
		    else { res = pat.equals(rep.toString()); }
		  }
		if (res) { return true; }
	      }
	      return false;
	    }
     }

  String longest_common_substring (Node frames, TruthValue kb_local_only_p) {
       // System.out.println("Enter longest common substring with: " + frames);
       Node alist = _NIL;
       Enumeration e = frames.elements();
       if (frames == _NIL || frames == null) { return ""; }
       if (frames.cdr() == _NIL || frames.cdr() == null)
	 { Node n = possibilities_for_pattern_matching
	              (frames.car(), kb_local_only_p).car();
	   if (n instanceof OKBCString) { return ((OKBCString)n).string; }
	   else if (n instanceof AbstractSymbol)
	     { return ((AbstractSymbol)n).symbolName; }
	   else { return n.toString(); }
	 }
       for (; e.hasMoreElements() ;) {
	 Node frame = (Node)(e.nextElement());
	 alist = new Cons(possibilities_for_pattern_matching
			  (frame, kb_local_only_p),
			  alist);
       }
       String chars = "";
       for(int index=0; true; index++) {
	 char ch = 'A';
	 boolean char_found_p = false;
	 boolean found_p = true;
	 Enumeration f = alist.elements();
	 for (; f.hasMoreElements() ;) {
	   Node posses = (Node)(f.nextElement());
	   // System.out.println("Posses = " + posses + ", index = " + index);
	   Node pos = posses;
	   boolean found_p_2 = false;
	   for (; pos != _NIL && pos != null; ) {
	     // System.out.println("Remaining posses = " + pos + ", index = " + index);
	     String string = ensure_string((Cons)pos);
	     if (index < string.length())
	       {
		 if (char_found_p)
		   { if (string.charAt(index) == ch)
		       { found_p_2 = true; break; }
		   }
		 else { ch = string.charAt(index);
			char_found_p = true;
			found_p_2 = true;
			break;
		      }
	       }
	     pos = pos.cdr();
	   }
	   if (!found_p_2) { found_p = false; break; }
	 }
	 if (!found_p) { break; }
	 else { chars = chars + ch; }
       }
       return chars;
     }

  String ensure_string (Cons location) {
    Node thing = location.car();
    if (thing instanceof OKBCString) { return ((OKBCString)thing).string; }
    else if (thing instanceof AbstractSymbol)
      { location.set_car(new OKBCString(((AbstractSymbol)thing).symbolName)); }
    else { location.set_car(new OKBCString(thing.toString())); }
    return ensure_string(location);
  }

// ---------- default implementation of get-frames-matching


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
   protected Values2 get_frames_matching_internal
     (Node pattern, Node wildcards_allowed_p, Node selector,
      Node force_case_insensitive_p, TruthValue kb_local_only_p) {
       if (wildcards_allowed_p == _NIL &&
	   (pattern instanceof OKBCString ||
	    pattern instanceof AbstractSymbol ||
	    pattern instanceof OKBCInteger ||
	    pattern instanceof Real) &&
	   !Cons.member_eql(_NIL, get_behavior_values(_frame_names_required),
			    null, kb_local_only_p))
	 {
	   return get_frames_matching_for_frame_names_required
	     (pattern, wildcards_allowed_p, selector,
	      force_case_insensitive_p, kb_local_only_p);
	 }
       else
	 {
	   return get_frames_matching_the_hard_way
	     (pattern, wildcards_allowed_p, selector,
	      force_case_insensitive_p, kb_local_only_p);
	 }
     }

   protected Values2 get_frames_matching_for_frame_names_required
     (Node pattern, Node wildcards_allowed_p, Node selector,
      Node force_case_insensitive_p, TruthValue kb_local_only_p) {
       Values2 v = null;
       if (selector==_all)
	 { v = coerce_to_frame(pattern, _NIL, kb_local_only_p); }
       else if (selector==_slot)
	 { v = coerce_to_slot(pattern, _NIL, kb_local_only_p); }
       else if (selector==_facet)
	 { v = coerce_to_facet(pattern, _NIL, kb_local_only_p); }
       else if (selector==_class)
	 { v = coerce_to_class(pattern, _NIL, kb_local_only_p); }
       else if (selector==_individual)
	 { v = coerce_to_individual(pattern, _NIL, kb_local_only_p); }
       else
	 { throw new GenericError
	   (_T, new OKBCString("Unknown selector: " + selector))
	   .signal();
	 }
       Node match = v.firstValue();
       if (match != _NIL)
	 { return new Values2(Cons.list(match), pattern); }
       else if (force_case_insensitive_p != _NIL)
	 { return get_frames_matching_the_hard_way
	     (pattern, wildcards_allowed_p, selector,
	      force_case_insensitive_p, kb_local_only_p);
	 }
       else { return new Values2(_NIL, new OKBCString("")); }
     }
  
   protected Values2 get_frames_matching_the_hard_way
     (Node pattern, Node wildcards_allowed_p, Node selector,
      Node force_case_insensitive_p, TruthValue kb_local_only_p) {
       ConsLocation loc = new ConsLocation();
       Node are_frames = get_behavior_values(_are_frames);
       boolean selector_allows_non_frames_p = false;
       if (selector == _all)
	 { if (!(Cons.member_eql(_class, are_frames, this, kb_local_only_p))) 
	     { selector_allows_non_frames_p = true; }
	   else if (!(Cons.member_eql(_slot, are_frames, this,
				      kb_local_only_p))) 
	     { selector_allows_non_frames_p = true; }
	   else if (!(Cons.member_eql(_facet, are_frames, this,
				      kb_local_only_p))) 
	     { selector_allows_non_frames_p = true; }
	   else if (!(Cons.member_eql(_individual, are_frames, this,
				      kb_local_only_p)))
	     { selector_allows_non_frames_p = true; }
	 }
       else if (Cons.member_eql(selector, are_frames, this, kb_local_only_p))
	 { selector_allows_non_frames_p = true; }
       Node enumerand = _NIL; // get_kb_frames(kb_local_only_p);
       String pattern_string = null;
       if (pattern instanceof OKBCString)
	 { pattern_string = ((OKBCString)pattern).string; }
       else if (pattern instanceof AbstractSymbol)
	 { pattern_string = ((AbstractSymbol)pattern).symbolName; }
       else { throw new GenericError
	      (_T, 
	       new OKBCString("Illegal pattern.  Must be a string of symbol: "
			      + pattern)).signal();
	    }
       if (selector == _all)
	 { if (selector_allows_non_frames_p)
	     { enumerand = _NIL; }
	   else { enumerand = get_kb_frames(kb_local_only_p); }
	 }
       else if (selector == _class)
	 { enumerand = get_kb_classes(_all, kb_local_only_p); }
       else if (selector == _slot)
	 { enumerand = get_kb_slots(_all, kb_local_only_p); }
       else if (selector == _facet)
	 { enumerand = get_kb_facets(_all, kb_local_only_p); }
       else if (selector == _individual)
	 { enumerand = get_kb_individuals(_all, kb_local_only_p); }
       else { enumerand = get_kb_frames(kb_local_only_p); }
       Enumerator e = enumerand.enumerate_list();
       while (e.has_more_p()) {
	 Node frame = e.next();
	 get_frames_matching_doit
	   (frame, selector, wildcards_allowed_p, force_case_insensitive_p,
	    kb_local_only_p, pattern_string, selector_allows_non_frames_p,
	    loc);
       }
       if (selector == _all && selector_allows_non_frames_p)
	 { if (!(Cons.member_eql(_slot, are_frames, this, kb_local_only_p)))
	     { Enumerator f = enumerate_kb_slots(_all, kb_local_only_p);
	       f.fetch(_all);
	       while (f.has_more_p()) {
		 Node thing = f.next();
		 get_frames_matching_doit
		   (thing, selector, wildcards_allowed_p, 
		    force_case_insensitive_p, kb_local_only_p, pattern_string,
		    selector_allows_non_frames_p, loc);
	       }
	       f.free();
	     }
	   if (!(Cons.member_eql(_facet, are_frames, this, kb_local_only_p)))
	     { Enumerator f = enumerate_kb_facets(_all, kb_local_only_p);
	       f.fetch(_all);
	       while (f.has_more_p()) {
		 Node thing = f.next();
		 get_frames_matching_doit
		   (thing, selector, wildcards_allowed_p, 
		    force_case_insensitive_p, kb_local_only_p, pattern_string,
		    selector_allows_non_frames_p, loc);
	       }
	       f.free();
	     }
	   if (!(Cons.member_eql(_class, are_frames, this, kb_local_only_p)))
	     { Enumerator f = enumerate_kb_classes(_all, kb_local_only_p);
	       f.fetch(_all);
	       while (f.has_more_p()) {
		 Node thing = f.next();
		 get_frames_matching_doit
		   (thing, selector, wildcards_allowed_p, 
		    force_case_insensitive_p, kb_local_only_p, pattern_string,
		    selector_allows_non_frames_p, loc);
	       }
	       f.free();
	     }
	   if (!(Cons.member_eql(_individual, are_frames, this,
				 kb_local_only_p)))
	     { Enumerator f = enumerate_kb_individuals
	       (_all, kb_local_only_p);
	       f.fetch(_all);
	       while (f.has_more_p()) {
		 Node thing = f.next();
		 get_frames_matching_doit
		   (thing, selector, wildcards_allowed_p, 
		    force_case_insensitive_p, kb_local_only_p, pattern_string,
		    selector_allows_non_frames_p, loc);
	       }
	       f.free();
	     }
	 }
       String longest = pattern_string;
       if (wildcards_allowed_p != _NIL)
	 { longest = longest_common_substring(loc.cons, kb_local_only_p); }
       return new Values2(loc.cons, new OKBCString(longest));
     }

  void get_frames_matching_doit 
    (Node frame, Node selector, Node wildcards_allowed_p,
     Node force_case_insensitive_p, TruthValue kb_local_only_p,
     String pattern_string, boolean selector_allows_non_frames_p,
     ConsLocation loc)
  {
    if (coercible_to_frame_p(frame, kb_local_only_p) != _NIL ||
	selector_allows_non_frames_p)
      {
	if (selector instanceof AbstractSymbol ||
	    (selector instanceof Procedure &&
	     (call_procedure(selector, Cons.list(frame)) != _NIL)))
	  { // System.out.println("*** Testing: " + frame + " for " + pattern);
	    if (frame_matches_pattern_p
		(frame, pattern_string, wildcards_allowed_p,
		 force_case_insensitive_p, kb_local_only_p))
	      { loc.addElement(frame);
		// System.out.println("*** Matches now are: " + matches + " for " + pattern);
	      }
	  }
      }
  }



// ---------- default implementation of register-procedure


/**
 * Associates the   procedure  with   name  in   kb .
 * Subsequent calls to   call-procedure  may invoke the procedure merely by
 * using the name.  If   name  is  , then   procedure 
 * must be a    named  procedure, in which case the   name 
 * argument will default to the name of the procedure.
 * Returns no values.
 */
  protected void register_procedure_internal (Node the_name, Node procedure) {
    Procedure funspec = (Procedure) procedure;
    //Symbol name = null;
    //if (the_name != null && the_name != _NIL)
//      { name = (Symbol) the_name; }
    if (the_name!=null && the_name!=_NIL && 
	(the_name instanceof AbstractSymbol))
      {
	AbstractSymbol name = (AbstractSymbol) the_name;
	Procedure.name_to_procedure_table.put
	(name.symbolName.toUpperCase(), funspec);
	// Cache the most recently registered name on the procedure.
	// This has no semantic significance, it just makes things nicer 
	// when we print.
	funspec.name = name;
	register_side_effect("register_procedure");
      }
    else if (funspec.name != _NIL)
      // Then we'll use the name slot of the funspec itself.
      { Procedure.name_to_procedure_table.put
	(((AbstractSymbol)(funspec.name)).symbolName.toUpperCase(), funspec);
	register_side_effect("register_procedure");
      }
    else { throw new RuntimeException 
	   ("Procedure " + funspec + " has no name");
	 }
  }


// ---------- default implementation of unregister-procedure


/**
 * Removes any procedure association for the   name  in   kb .
 * Returns no values.
 */
  protected void unregister_procedure_internal (Node the_name) {
    if (the_name==_NIL)
      { throw new RuntimeException("NIL cannot name a procedure"); }
    if (the_name instanceof AbstractSymbol)
      { 
        AbstractSymbol name = (AbstractSymbol) the_name;
	Procedure.name_to_procedure_table.remove
	(name.symbolName.toUpperCase());
	register_side_effect("unregister_procedure");
      }
    else 
      { throw new RuntimeException
	("Illegal name for a procedure " + the_name); 
      }
  }


// ---------- default implementation of get-procedure


/**
 * Returns the   procedure  that is the procedure association for
 * the   name , or   if there is no such procedure association.
 * See   register-procedure ,   unregister-procedure , and
 *   call-procedure .
 */
  protected Node get_procedure_internal (Node the_name) {
	if (the_name==_NIL)
	  throw new RuntimeException("NIL cannot name a procedure");
    if (the_name instanceof AbstractSymbol)
      {
	AbstractSymbol name = (AbstractSymbol) the_name;
	Procedure funspec =
	  (Procedure)
	  Procedure.name_to_procedure_table.get
	  (name.symbolName);
	if (funspec == null) { return _NIL; }
	else { return funspec; }
      }
    else 
      { throw new RuntimeException
	("Illegal name for a procedure " + the_name); 
      }
  }


// ---------- default implementation of value-as-string


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
  protected Values2 value_as_string_internal
     (Node value, Node purpose, Node pretty_p, TruthValue kb_local_only_p) {
       ByteArrayOutputStream bytes = new ByteArrayOutputStream();
       PrintWriter stringStream = new PrintWriter(bytes);
       Vector location_list = new Vector();
       NodeLocation current_location = new NodeLocation(new OKBCInteger(0)); 
       print_value_to_string(value, stringStream, _T, (pretty_p != _NIL),
			     0, kb_local_only_p, location_list, 
			     current_location);
       stringStream.flush();
       Node list = Cons.vectorToCons(location_list);
       if (list == null) { list = _NIL; }
       return new Values2(new OKBCString(bytes.toString()), list);
    }

  void incf_count(NodeLocation current_location) {
    incf_count(current_location, 1);
  }

  void incf_count(NodeLocation current_location, long amount) {
    long current = ((OKBCInteger)(current_location.node)).number;
    current_location.node = new OKBCInteger(current + amount);
  }

  void print_cons_to_string(Node object, PrintWriter stream,
			    TruthValue error_p, 
			    boolean pretty_p, int indent,
			    TruthValue kb_local_only_p,
			    Vector location_list, 
			    NodeLocation current_location) {
    stream.print("(");
    incf_count(current_location);
    if (((Cons)object).cdr == null || ((Cons)object).cdr == _NIL)
      { Node car = ((Cons)object).car;
	print_value_to_string(car, stream, error_p, pretty_p, indent + 1,
			      kb_local_only_p, location_list, 
			      current_location);
	stream.print(")");
	incf_count(current_location);
      }
    else 
      { Node car = ((Cons)object).car;
	print_value_to_string(car, stream, error_p, pretty_p, indent + 1,
			      kb_local_only_p, location_list, 
			      current_location);
	Node e = ((Cons)object).cdr;
	do 
	  {
	    if (pretty_p)
	      { stream.println();
                incf_count(current_location, 
                           System.getProperties().getProperty
			   ("line.separator").length());
		for(int i=0; i < indent + 2; i++) {
		  stream.print(" ");
		  incf_count(current_location);
		}
	      }
	    else { stream.print(" "); 
                   incf_count(current_location);
                 }
	    print_value_to_string(((Cons)e).car, stream, error_p,
				  pretty_p, 
				  (pretty_p ? indent + 2 : indent),
				  kb_local_only_p, location_list, 
				  current_location);
	    e = ((Cons)e).cdr;
	  }
	while (e instanceof Cons);
	if (e != null)
	  { stream.print(" . ");
            incf_count(current_location, 3);
	    if (pretty_p)
	      { stream.println();
                incf_count(current_location, 
                           System.getProperties().getProperty
			   ("line.separator").length());
		for(int i=0; i < indent + 4; i++) {
		  stream.print(" ");
                  incf_count(current_location);
		}
	      }
	    else { stream.print(" "); 
                   incf_count(current_location);
                 }
	    print_value_to_string(e, stream, error_p, pretty_p, indent,
				  kb_local_only_p, location_list, 
			          current_location);
	  }
	stream.print(")");
        incf_count(current_location);
      }
  }


  void print_value_to_string(Node object, PrintWriter stream,
			     TruthValue error_p, 
			     boolean pretty_p, int indent,
			     TruthValue kb_local_only_p,
			     Vector location_list, 
			     NodeLocation current_location) {
    if (object instanceof Cons)
      { print_cons_to_string(object, stream, error_p, pretty_p, indent, 
			     kb_local_only_p, location_list, 
			     current_location);
      }
    else 
      { Values2 framev = coerce_to_frame(object, _NIL, kb_local_only_p);
	Node frame = framev.firstValue();
	Node found_p = framev.secondValue();
	if (found_p != _NIL)
	  { print_frame_to_stream(frame, stream, error_p, pretty_p, indent,
				  kb_local_only_p, location_list, 
			          current_location);
	  }
	else if (object instanceof OKBCString)
	  { incf_count(current_location, ((OKBCString)object).string.length());
	    if (pretty_p)
	      { stream.print(object); }
	    else { stream.print("\"");
		   stream.print(object);
		   stream.print("\"");
		   incf_count(current_location, 2);
		 }
	  }
	else if (object instanceof AbstractSymbol && pretty_p)
	  { AbstractSymbol sym = (AbstractSymbol)object;
	    incf_count(current_location, sym.symbolName.length());
	    stream.print(sym.symbolName);
	  }
	else { String s = object.toString();
	       stream.print(s); 
	       incf_count(current_location, s.length());
	     }
      }
  }

  void print_frame_to_stream(Node frame, PrintWriter stream, 
			     TruthValue error_p, 
			     boolean pretty_p, int indent,
			     TruthValue kb_local_only_p,
                             Vector location_list, 
			     NodeLocation current_location) {
    Node to_print = null;
    if (error_p != _NIL)
      { if (pretty_p)
	  { to_print = get_frame_pretty_name(frame, kb_local_only_p);}
	else { to_print = get_frame_handle(frame, kb_local_only_p); }
      }
    else
      {
	try
	  {
	    if (pretty_p)
	      { to_print = get_frame_pretty_name(frame, kb_local_only_p); 
	      }
	    else { to_print = get_frame_handle(frame, kb_local_only_p);}
	  }
	catch (Exception e) {}
      }
    if (to_print == null) { to_print = frame; } // error-found-p
    String string_to_print = to_print.toString();
    stream.print(string_to_print);
    long new_loc = ((OKBCInteger)(current_location.node)).number + 
      string_to_print.length();
    location_list.addElement(Cons.list(current_location.node,
				       new OKBCInteger(new_loc), frame));
    incf_count(current_location, string_to_print.length());
  }

// ---------- default implementation of call-procedure


/**
 * Returns the   value  resulting from applying
 *   procedure  to
 *   arguments .  See section   for a definition of
 * procedures.
 */
  protected Node call_procedure_internal
     (Node procedure,
      Node arguments) {
       return static_call_procedure(this, procedure, arguments);
    }

// ---------- default implementation of goto-kb


/**
 * Makes   kb  the current KB.  After a call to   goto-kb , the value
 * of a call to   current-kb  will be   kb .  The newly established
 *   current-kb  will be used as the default value for the   kb 
 * argument by language bindings that support argument defaulting.  Returns
 * no values.
 */
  public void goto_kb () {
    the_current_kb = this;
  }


// ---------- default implementation of kb-modified-p


/**
 * Returns   if   kb  has been modified since it was last saved.
 */
  protected Node kb_modified_p_internal ()
  { return (kb_has_been_modified_p ? (Node)_T : (Node)_NIL);
  }


// ---------- default implementation of create-kb


/**
 * Creates a new KB (see Section  ) called   name  whose
 * implementation type is   kb-type .    Kb-type  identifies the
 * underlying KRS that will be used to manipulate the KB.  Returns the
 *   new-kb .
 * 
 * Note that this operation creates a new    in-memory  KB; it does
 * not necessarily create a persistent version of the knowledge base on
 * external storage until   save-kb  or   save-kb-as  is called.
 * 
 * The   name  of the KB is a symbol.
 * 
 *   kb-locator , if supplied, describes the new KB.  Kb-locators can be
 * created using   create-kb-locator .
 * If   kb-locator  is not supplied, a default kb-locator will be assigned
 * by the KRS for   kb-type  and   connection .
 * 
 *   Initargs  is a list of initializations for the new KB as
 * required by the   kb-type .  The mechanism underlying the
 * implementation of   create-kb  is not specified and the user cannot,
 * therefore, rely on any underlying native object system initialization
 * protocol being invoked.  The format and content of the initialization
 * arguments will be documented with the   kb-type .  For example,
 * if the KB being created allows the specification of parent (included) KBs,
 * a set of initialization arguments might be as follows:
 *   
 * (list :parent-kbs (list my-kb))
 *  
 * Any KB created with   create-kb  can be found by using
 * either   find-kb  or   find-kb-of-type , and it is included in
 * the values returned by   get-kbs .  A KB created with
 *   create-kb  is a frame object in the   meta-kb .
 * 
 * Implementation note:  It is the responsibility of the implementations of
 *   create-kb  to register new KBs in the Meta KB (for example, by using
 *   put-instance-types  to tell the Meta KB that the new KB is an instance
 * of   kb-type .
 */
  protected Kb create_kb_internal
   (Node name, Node kb_locator, Node initargs, Connection connection) {
   if (connection instanceof NetworkConnection)
     { MinimalNetworkKb result;
       Node args =
	 Cons.list(create_kb_fname,
		   name,
		   Cons.list(Symbol.intern_in_okbc("GET-KB-TYPE"),
			     this.kb_type_name),
		   kb_locator,
		   initargs,
		   _establish_local_connection);
       Node values = 
	 ((NetworkConnection)connection).makeNetworkCall(args);
       result = (MinimalNetworkKb) values.car();
       MinimalNetworkKb.add_kb(result, kb_locator);
       return result;
     }
   else
     { Kb new_kb = null;
       Exception exception = null;
       Class kb_class = this.getClass();
       try { new_kb = (Kb)(kb_class.newInstance()); }
       catch (Exception e) { exception = e; }
       if (new_kb == null)
	 { throw new RuntimeException 
	   (error_message_for_illegal_access(kb_class, _NIL, exception));
	 }
       new_kb.setName(name);
       new_kb.set_connection(connection);
       new_kb.initialize_from_init_plist(initargs);
       // Given more MOPish stuff, we could initialise some slots here!
       add_kb(new_kb, kb_locator);
       last_locator = kb_locator;
       return new_kb;
     }
  }


  protected void initialize_from_init_plist(Node initargs) {
    ioSyntax.initialize_from_init_plist(initargs);
  }


  public Kb create_kb
   (Node name, Node kb_locator, Node initargs, Connection connection) {
     return create_kb_internal(name, kb_locator, initargs, connection);
  }


  static final Symbol add_class_superclass_fname =
         Symbol.intern_in_ok_back("ADD-CLASS-SUPERCLASS-INTERNAL");
  static final Symbol add_facet_value_fname =
         Symbol.intern_in_ok_back("ADD-FACET-VALUE-INTERNAL");
  static final Symbol add_instance_type_fname =
         Symbol.intern_in_ok_back("ADD-INSTANCE-TYPE-INTERNAL");
  static final Symbol add_slot_value_fname =
         Symbol.intern_in_ok_back("ADD-SLOT-VALUE-INTERNAL");
  static final Symbol all_connections_fname =
         Symbol.intern_in_ok_back("ALL-CONNECTIONS-INTERNAL");
  static final Symbol allocate_frame_handle_fname =
         Symbol.intern_in_ok_back("ALLOCATE-FRAME-HANDLE-INTERNAL");
  static final Symbol ask_fname =
         Symbol.intern_in_ok_back("ASK-INTERNAL");
  static final Symbol askable_fname =
         Symbol.intern_in_ok_back("ASKABLE-INTERNAL");
  static final Symbol attach_facet_fname =
         Symbol.intern_in_ok_back("ATTACH-FACET-INTERNAL");
  static final Symbol attach_slot_fname =
         Symbol.intern_in_ok_back("ATTACH-SLOT-INTERNAL");
  static final Symbol call_procedure_fname =
         Symbol.intern_in_ok_back("CALL-PROCEDURE-INTERNAL");
  static final Symbol class_p_fname =
         Symbol.intern_in_ok_back("CLASS-P-INTERNAL");
  static final Symbol close_connection_fname =
         Symbol.intern_in_ok_back("CLOSE-CONNECTION-INTERNAL");
  static final Symbol close_kb_fname =
         Symbol.intern_in_ok_back("CLOSE-KB-INTERNAL");
  static final Symbol coerce_to_class_fname =
         Symbol.intern_in_ok_back("COERCE-TO-CLASS-INTERNAL");
  static final Symbol coerce_to_facet_fname =
         Symbol.intern_in_ok_back("COERCE-TO-FACET-INTERNAL");
  static final Symbol coerce_to_frame_fname =
         Symbol.intern_in_ok_back("COERCE-TO-FRAME-INTERNAL");
  static final Symbol coerce_to_individual_fname =
         Symbol.intern_in_ok_back("COERCE-TO-INDIVIDUAL-INTERNAL");
  static final Symbol coerce_to_kb_value_fname =
         Symbol.intern_in_ok_back("COERCE-TO-KB-VALUE-INTERNAL");
  static final Symbol coerce_to_slot_fname =
         Symbol.intern_in_ok_back("COERCE-TO-SLOT-INTERNAL");
  static final Symbol coercible_to_frame_p_fname =
         Symbol.intern_in_ok_back("COERCIBLE-TO-FRAME-P-INTERNAL");
  static final Symbol copy_frame_fname =
         Symbol.intern_in_ok_back("COPY-FRAME-INTERNAL");
  static final Symbol copy_kb_fname =
         Symbol.intern_in_ok_back("COPY-KB-INTERNAL");
  static final Symbol create_class_fname =
         Symbol.intern_in_ok_back("CREATE-CLASS-INTERNAL");
  static final Symbol create_facet_fname =
         Symbol.intern_in_ok_back("CREATE-FACET-INTERNAL");
  static final Symbol create_frame_fname =
         Symbol.intern_in_ok_back("CREATE-FRAME-INTERNAL");
  static final Symbol create_individual_fname =
         Symbol.intern_in_ok_back("CREATE-INDIVIDUAL-INTERNAL");
  static final Symbol create_kb_fname =
         Symbol.intern_in_ok_back("CREATE-KB-INTERNAL");
  static final Symbol create_kb_locator_fname =
         Symbol.intern_in_ok_back("CREATE-KB-LOCATOR-INTERNAL");
  static final Symbol create_procedure_fname =
         Symbol.intern_in_ok_back("CREATE-PROCEDURE-INTERNAL");
  static final Symbol create_slot_fname =
         Symbol.intern_in_ok_back("CREATE-SLOT-INTERNAL");
  static final Symbol decontextualize_fname =
         Symbol.intern_in_ok_back("DECONTEXTUALIZE-INTERNAL");
  static final Symbol delete_facet_fname =
         Symbol.intern_in_ok_back("DELETE-FACET-INTERNAL");
  static final Symbol delete_frame_fname =
         Symbol.intern_in_ok_back("DELETE-FRAME-INTERNAL");
  static final Symbol delete_slot_fname =
         Symbol.intern_in_ok_back("DELETE-SLOT-INTERNAL");
  static final Symbol detach_facet_fname =
         Symbol.intern_in_ok_back("DETACH-FACET-INTERNAL");
  static final Symbol detach_slot_fname =
         Symbol.intern_in_ok_back("DETACH-SLOT-INTERNAL");
  static final Symbol enumerate_all_connections_fname =
         Symbol.intern_in_ok_back("ENUMERATE-ALL-CONNECTIONS-INTERNAL");
  static final Symbol enumerate_ask_fname =
         Symbol.intern_in_ok_back("ENUMERATE-ASK-INTERNAL");
  static final Symbol enumerate_call_procedure_fname =
         Symbol.intern_in_ok_back("ENUMERATE-CALL-PROCEDURE-INTERNAL");
  static final Symbol enumerate_class_instances_fname =
         Symbol.intern_in_ok_back("ENUMERATE-CLASS-INSTANCES-INTERNAL");
  static final Symbol enumerate_class_subclasses_fname =
         Symbol.intern_in_ok_back("ENUMERATE-CLASS-SUBCLASSES-INTERNAL");
  static final Symbol enumerate_class_superclasses_fname =
         Symbol.intern_in_ok_back("ENUMERATE-CLASS-SUPERCLASSES-INTERNAL");
  static final Symbol enumerate_facet_values_fname =
         Symbol.intern_in_ok_back("ENUMERATE-FACET-VALUES-INTERNAL");
  static final Symbol enumerate_facet_values_in_detail_fname =
         Symbol.intern_in_ok_back("ENUMERATE-FACET-VALUES-IN-DETAIL-INTERNAL");
  static final Symbol enumerate_frame_slots_fname =
         Symbol.intern_in_ok_back("ENUMERATE-FRAME-SLOTS-INTERNAL");
  static final Symbol enumerate_frames_matching_fname =
         Symbol.intern_in_ok_back("ENUMERATE-FRAMES-MATCHING-INTERNAL");
  static final Symbol enumerate_instance_types_fname =
         Symbol.intern_in_ok_back("ENUMERATE-INSTANCE-TYPES-INTERNAL");
  static final Symbol enumerate_kb_classes_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-CLASSES-INTERNAL");
  static final Symbol enumerate_kb_direct_children_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-DIRECT-CHILDREN-INTERNAL");
  static final Symbol enumerate_kb_direct_parents_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-DIRECT-PARENTS-INTERNAL");
  static final Symbol enumerate_kb_facets_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-FACETS-INTERNAL");
  static final Symbol enumerate_kb_frames_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-FRAMES-INTERNAL");
  static final Symbol enumerate_kb_individuals_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-INDIVIDUALS-INTERNAL");
  static final Symbol enumerate_kb_roots_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-ROOTS-INTERNAL");
  static final Symbol enumerate_kb_slots_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-SLOTS-INTERNAL");
  static final Symbol enumerate_kb_types_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KB-TYPES-INTERNAL");
  static final Symbol enumerate_kbs_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KBS-INTERNAL");
  static final Symbol enumerate_kbs_of_type_fname =
         Symbol.intern_in_ok_back("ENUMERATE-KBS-OF-TYPE-INTERNAL");
  static final Symbol enumerate_list_fname =
         Symbol.intern_in_ok_back("ENUMERATE-LIST-INTERNAL");
  static final Symbol enumerate_slot_facets_fname =
         Symbol.intern_in_ok_back("ENUMERATE-SLOT-FACETS-INTERNAL");
  static final Symbol enumerate_slot_values_fname =
         Symbol.intern_in_ok_back("ENUMERATE-SLOT-VALUES-INTERNAL");
  static final Symbol enumerate_slot_values_in_detail_fname =
         Symbol.intern_in_ok_back("ENUMERATE-SLOT-VALUES-IN-DETAIL-INTERNAL");
  static final Symbol eql_in_kb_fname =
         Symbol.intern_in_ok_back("EQL-IN-KB-INTERNAL");
  static final Symbol equal_in_kb_fname =
         Symbol.intern_in_ok_back("EQUAL-IN-KB-INTERNAL");
  static final Symbol equalp_in_kb_fname =
         Symbol.intern_in_ok_back("EQUALP-IN-KB-INTERNAL");
  static final Symbol establish_connection_fname =
         Symbol.intern_in_ok_back("ESTABLISH-CONNECTION-INTERNAL");
  static final Symbol expunge_kb_fname =
         Symbol.intern_in_ok_back("EXPUNGE-KB-INTERNAL");
  static final Symbol facet_has_value_p_fname =
         Symbol.intern_in_ok_back("FACET-HAS-VALUE-P-INTERNAL");
  static final Symbol facet_p_fname =
         Symbol.intern_in_ok_back("FACET-P-INTERNAL");
  static final Symbol fetch_fname =
         Symbol.intern_in_ok_back("FETCH-INTERNAL");
  static final Symbol find_kb_fname =
         Symbol.intern_in_ok_back("FIND-KB-INTERNAL");
  static final Symbol find_kb_locator_fname =
         Symbol.intern_in_ok_back("FIND-KB-LOCATOR-INTERNAL");
  static final Symbol find_kb_of_type_fname =
         Symbol.intern_in_ok_back("FIND-KB-OF-TYPE-INTERNAL");
  static final Symbol follow_slot_chain_fname =
         Symbol.intern_in_ok_back("FOLLOW-SLOT-CHAIN-INTERNAL");
  static final Symbol frame_has_slot_p_fname =
         Symbol.intern_in_ok_back("FRAME-HAS-SLOT-P-INTERNAL");
  static final Symbol frame_in_kb_p_fname =
         Symbol.intern_in_ok_back("FRAME-IN-KB-P-INTERNAL");
  static final Symbol free_fname =
         Symbol.intern_in_ok_back("FREE-INTERNAL");
  static final Symbol frs_independent_frame_handle_fname =
         Symbol.intern_in_ok_back("FRS-INDEPENDENT-FRAME-HANDLE-INTERNAL");
  static final Symbol frs_name_fname =
         Symbol.intern_in_ok_back("FRS-NAME-INTERNAL");
  static final Symbol get_behavior_supported_values_fname =
         Symbol.intern_in_ok_back("GET-BEHAVIOR-SUPPORTED-VALUES-INTERNAL");
  static final Symbol get_behavior_values_fname =
         Symbol.intern_in_ok_back("GET-BEHAVIOR-VALUES-INTERNAL");
  static final Symbol get_class_instances_fname =
         Symbol.intern_in_ok_back("GET-CLASS-INSTANCES-INTERNAL");
  static final Symbol get_class_subclasses_fname =
         Symbol.intern_in_ok_back("GET-CLASS-SUBCLASSES-INTERNAL");
  static final Symbol get_class_superclasses_fname =
         Symbol.intern_in_ok_back("GET-CLASS-SUPERCLASSES-INTERNAL");
  static final Symbol get_classes_in_domain_of_fname =
         Symbol.intern_in_ok_back("GET-CLASSES-IN-DOMAIN-OF-INTERNAL");
  static final Symbol get_facet_value_fname =
         Symbol.intern_in_ok_back("GET-FACET-VALUE-INTERNAL");
  static final Symbol get_facet_values_fname =
         Symbol.intern_in_ok_back("GET-FACET-VALUES-INTERNAL");
  static final Symbol get_facet_values_in_detail_fname =
         Symbol.intern_in_ok_back("GET-FACET-VALUES-IN-DETAIL-INTERNAL");
  static final Symbol get_frame_details_fname =
         Symbol.intern_in_ok_back("GET-FRAME-DETAILS-INTERNAL");
  static final Symbol get_frame_handle_fname =
         Symbol.intern_in_ok_back("GET-FRAME-HANDLE-INTERNAL");
  static final Symbol get_frame_in_kb_fname =
         Symbol.intern_in_ok_back("GET-FRAME-IN-KB-INTERNAL");
  static final Symbol get_frame_name_fname =
         Symbol.intern_in_ok_back("GET-FRAME-NAME-INTERNAL");
  static final Symbol get_frame_pretty_name_fname =
         Symbol.intern_in_ok_back("GET-FRAME-PRETTY-NAME-INTERNAL");
  static final Symbol get_frame_sentences_fname =
         Symbol.intern_in_ok_back("GET-FRAME-SENTENCES-INTERNAL");
  static final Symbol get_frame_slots_fname =
         Symbol.intern_in_ok_back("GET-FRAME-SLOTS-INTERNAL");
  static final Symbol get_frame_type_fname =
         Symbol.intern_in_ok_back("GET-FRAME-TYPE-INTERNAL");
  static final Symbol get_frames_matching_fname =
         Symbol.intern_in_ok_back("GET-FRAMES-MATCHING-INTERNAL");
  static final Symbol get_frames_with_facet_value_fname =
         Symbol.intern_in_ok_back("GET-FRAMES-WITH-FACET-VALUE-INTERNAL");
  static final Symbol get_frames_with_slot_value_fname =
         Symbol.intern_in_ok_back("GET-FRAMES-WITH-SLOT-VALUE-INTERNAL");
  static final Symbol get_instance_types_fname =
         Symbol.intern_in_ok_back("GET-INSTANCE-TYPES-INTERNAL");
  static final Symbol get_kb_behaviors_fname =
         Symbol.intern_in_ok_back("GET-KB-BEHAVIORS-INTERNAL");
  static final Symbol get_kb_classes_fname =
         Symbol.intern_in_ok_back("GET-KB-CLASSES-INTERNAL");
  static final Symbol get_kb_direct_children_fname =
         Symbol.intern_in_ok_back("GET-KB-DIRECT-CHILDREN-INTERNAL");
  static final Symbol get_kb_direct_parents_fname =
         Symbol.intern_in_ok_back("GET-KB-DIRECT-PARENTS-INTERNAL");
  static final Symbol get_kb_facets_fname =
         Symbol.intern_in_ok_back("GET-KB-FACETS-INTERNAL");
  static final Symbol get_kb_frames_fname =
         Symbol.intern_in_ok_back("GET-KB-FRAMES-INTERNAL");
  static final Symbol get_kb_individuals_fname =
         Symbol.intern_in_ok_back("GET-KB-INDIVIDUALS-INTERNAL");
  static final Symbol get_kb_roots_fname =
         Symbol.intern_in_ok_back("GET-KB-ROOTS-INTERNAL");
  static final Symbol get_kb_slots_fname =
         Symbol.intern_in_ok_back("GET-KB-SLOTS-INTERNAL");
  static final Symbol get_kb_type_fname =
         Symbol.intern_in_ok_back("GET-KB-TYPE-INTERNAL");
  static final Symbol get_kb_types_fname =
         Symbol.intern_in_ok_back("GET-KB-TYPES-INTERNAL");
  static final Symbol get_kbs_fname =
         Symbol.intern_in_ok_back("GET-KBS-INTERNAL");
  static final Symbol get_kbs_of_type_fname =
         Symbol.intern_in_ok_back("GET-KBS-OF-TYPE-INTERNAL");
  static final Symbol get_procedure_fname =
         Symbol.intern_in_ok_back("GET-PROCEDURE-INTERNAL");
  static final Symbol get_slot_facets_fname =
         Symbol.intern_in_ok_back("GET-SLOT-FACETS-INTERNAL");
  static final Symbol get_slot_type_fname =
         Symbol.intern_in_ok_back("GET-SLOT-TYPE-INTERNAL");
  static final Symbol get_slot_value_fname =
         Symbol.intern_in_ok_back("GET-SLOT-VALUE-INTERNAL");
  static final Symbol get_slot_values_fname =
         Symbol.intern_in_ok_back("GET-SLOT-VALUES-INTERNAL");
  static final Symbol get_slot_values_in_detail_fname =
         Symbol.intern_in_ok_back("GET-SLOT-VALUES-IN-DETAIL-INTERNAL");
  static final Symbol goto_kb_fname =
         Symbol.intern_in_ok_back("GOTO-KB-INTERNAL");
  static final Symbol has_more_fname =
         Symbol.intern_in_ok_back("HAS-MORE-INTERNAL");
  static final Symbol implement_with_kb_io_syntax_fname =
         Symbol.intern_in_ok_back("IMPLEMENT-WITH-KB-IO-SYNTAX-INTERNAL");
  static final Symbol individual_p_fname =
         Symbol.intern_in_ok_back("INDIVIDUAL-P-INTERNAL");
  static final Symbol instance_of_p_fname =
         Symbol.intern_in_ok_back("INSTANCE-OF-P-INTERNAL");
  static final Symbol kb_modified_p_fname =
         Symbol.intern_in_ok_back("KB-MODIFIED-P-INTERNAL");
  static final Symbol kb_p_fname =
         Symbol.intern_in_ok_back("KB-P-INTERNAL");
  static final Symbol member_behavior_values_p_fname =
         Symbol.intern_in_ok_back("MEMBER-BEHAVIOR-VALUES-P-INTERNAL");
  static final Symbol member_facet_value_p_fname =
         Symbol.intern_in_ok_back("MEMBER-FACET-VALUE-P-INTERNAL");
  static final Symbol member_slot_value_p_fname =
         Symbol.intern_in_ok_back("MEMBER-SLOT-VALUE-P-INTERNAL");
  static final Symbol meta_kb_fname =
         Symbol.intern_in_ok_back("META-KB-INTERNAL");
  static final Symbol next_fname =
         Symbol.intern_in_ok_back("NEXT-INTERNAL");
  static final Symbol okbc_condition_p_fname =
         Symbol.intern_in_ok_back("OKBC-CONDITION-P-INTERNAL");
  static final Symbol open_kb_fname =
         Symbol.intern_in_ok_back("OPEN-KB-INTERNAL");
  static final Symbol openable_kbs_fname =
         Symbol.intern_in_ok_back("OPENABLE-KBS-INTERNAL");
  static final Symbol prefetch_fname =
         Symbol.intern_in_ok_back("PREFETCH-INTERNAL");
  static final Symbol primitive_p_fname =
         Symbol.intern_in_ok_back("PRIMITIVE-P-INTERNAL");
  static final Symbol print_frame_fname =
         Symbol.intern_in_ok_back("PRINT-FRAME-INTERNAL");
  static final Symbol put_behavior_values_fname =
         Symbol.intern_in_ok_back("PUT-BEHAVIOR-VALUES-INTERNAL");
  static final Symbol put_class_superclasses_fname =
         Symbol.intern_in_ok_back("PUT-CLASS-SUPERCLASSES-INTERNAL");
  static final Symbol put_facet_value_fname =
         Symbol.intern_in_ok_back("PUT-FACET-VALUE-INTERNAL");
  static final Symbol put_facet_values_fname =
         Symbol.intern_in_ok_back("PUT-FACET-VALUES-INTERNAL");
  static final Symbol put_frame_details_fname =
         Symbol.intern_in_ok_back("PUT-FRAME-DETAILS-INTERNAL");
  static final Symbol put_frame_name_fname =
         Symbol.intern_in_ok_back("PUT-FRAME-NAME-INTERNAL");
  static final Symbol put_frame_pretty_name_fname =
         Symbol.intern_in_ok_back("PUT-FRAME-PRETTY-NAME-INTERNAL");
  static final Symbol put_instance_types_fname =
         Symbol.intern_in_ok_back("PUT-INSTANCE-TYPES-INTERNAL");
  static final Symbol put_slot_value_fname =
         Symbol.intern_in_ok_back("PUT-SLOT-VALUE-INTERNAL");
  static final Symbol put_slot_values_fname =
         Symbol.intern_in_ok_back("PUT-SLOT-VALUES-INTERNAL");
  static final Symbol register_procedure_fname =
         Symbol.intern_in_ok_back("REGISTER-PROCEDURE-INTERNAL");
  static final Symbol remove_class_superclass_fname =
         Symbol.intern_in_ok_back("REMOVE-CLASS-SUPERCLASS-INTERNAL");
  static final Symbol remove_facet_value_fname =
         Symbol.intern_in_ok_back("REMOVE-FACET-VALUE-INTERNAL");
  static final Symbol remove_instance_type_fname =
         Symbol.intern_in_ok_back("REMOVE-INSTANCE-TYPE-INTERNAL");
  static final Symbol remove_local_facet_values_fname =
         Symbol.intern_in_ok_back("REMOVE-LOCAL-FACET-VALUES-INTERNAL");
  static final Symbol remove_local_slot_values_fname =
         Symbol.intern_in_ok_back("REMOVE-LOCAL-SLOT-VALUES-INTERNAL");
  static final Symbol remove_slot_value_fname =
         Symbol.intern_in_ok_back("REMOVE-SLOT-VALUE-INTERNAL");
  static final Symbol rename_facet_fname =
         Symbol.intern_in_ok_back("RENAME-FACET-INTERNAL");
  static final Symbol rename_slot_fname =
         Symbol.intern_in_ok_back("RENAME-SLOT-INTERNAL");
  static final Symbol replace_facet_value_fname =
         Symbol.intern_in_ok_back("REPLACE-FACET-VALUE-INTERNAL");
  static final Symbol replace_slot_value_fname =
         Symbol.intern_in_ok_back("REPLACE-SLOT-VALUE-INTERNAL");
  static final Symbol revert_kb_fname =
         Symbol.intern_in_ok_back("REVERT-KB-INTERNAL");
  static final Symbol save_kb_fname =
         Symbol.intern_in_ok_back("SAVE-KB-INTERNAL");
  static final Symbol save_kb_as_fname =
         Symbol.intern_in_ok_back("SAVE-KB-AS-INTERNAL");
  static final Symbol slot_has_facet_p_fname =
         Symbol.intern_in_ok_back("SLOT-HAS-FACET-P-INTERNAL");
  static final Symbol slot_has_value_p_fname =
         Symbol.intern_in_ok_back("SLOT-HAS-VALUE-P-INTERNAL");
  static final Symbol slot_p_fname =
         Symbol.intern_in_ok_back("SLOT-P-INTERNAL");
  static final Symbol subclass_of_p_fname =
         Symbol.intern_in_ok_back("SUBCLASS-OF-P-INTERNAL");
  static final Symbol superclass_of_p_fname =
         Symbol.intern_in_ok_back("SUPERCLASS-OF-P-INTERNAL");
  static final Symbol tell_fname =
         Symbol.intern_in_ok_back("TELL-INTERNAL");
  static final Symbol tellable_fname =
         Symbol.intern_in_ok_back("TELLABLE-INTERNAL");
  static final Symbol type_of_p_fname =
         Symbol.intern_in_ok_back("TYPE-OF-P-INTERNAL");
  static final Symbol unregister_procedure_fname =
         Symbol.intern_in_ok_back("UNREGISTER-PROCEDURE-INTERNAL");
  static final Symbol untell_fname =
         Symbol.intern_in_ok_back("UNTELL-INTERNAL");
  static final Symbol value_as_string_fname =
         Symbol.intern_in_ok_back("VALUE-AS-STRING-INTERNAL");


// ---------------------- member-behavior-values-p

/**
 * Returns   when   value  is one of the variants of
 *   behavior  that is currently active for   kb , and returns
 *   otherwise.
 */
  public Node member_behavior_values_p
     (Node behavior,
      Node value) {
     Node retval;
     retval = this.member_behavior_values_p_internal(behavior,
                                                     value);
     return retval;
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
     Values2 retval;
     retval = this.member_facet_value_p_internal(frame,
                                                 slot,
                                                 facet,
                                                 value,
                                                 inference_level,
                                                 test,
                                                 slot_type,
                                                 value_selector,
                                                 kb_local_only_p);
     return retval;
    }


  // Method for "member_facet_value_p" that defaults kb-local-only-p to nil
  public Values2 member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.member_facet_value_p(frame,
                                      slot,
                                      facet,
                                      value,
                                      inference_level,
                                      test,
                                      slot_type,
                                      value_selector,
                                      _NIL);
  }


  // Method for "member_facet_value_p" that defaults value-selector to either
  public Values2 member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type) {
     return this.member_facet_value_p(frame,
                                      slot,
                                      facet,
                                      value,
                                      inference_level,
                                      test,
                                      slot_type,
                                      _either);
  }


  // Method for "member_facet_value_p" that defaults slot-type to own
  public Values2 member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test) {
     return this.member_facet_value_p(frame,
                                      slot,
                                      facet,
                                      value,
                                      inference_level,
                                      test,
                                      _own);
  }


  // Method for "member_facet_value_p" that defaults test to equal
  public Values2 member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level) {
     return this.member_facet_value_p(frame,
                                      slot,
                                      facet,
                                      value,
                                      inference_level,
                                      _equal);
  }


  // Method for "member_facet_value_p" that defaults inference-level to taxonomic
  public Values2 member_facet_value_p
     (Node frame,
      Node slot,
      Node facet,
      Node value) {
     return this.member_facet_value_p(frame,
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
     Values2 retval;
     retval = this.member_slot_value_p_internal(frame,
                                                slot,
                                                value,
                                                inference_level,
                                                test,
                                                slot_type,
                                                value_selector,
                                                kb_local_only_p);
     return retval;
    }


  // Method for "member_slot_value_p" that defaults kb-local-only-p to nil
  public Values2 member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.member_slot_value_p(frame,
                                     slot,
                                     value,
                                     inference_level,
                                     test,
                                     slot_type,
                                     value_selector,
                                     _NIL);
  }


  // Method for "member_slot_value_p" that defaults value-selector to either
  public Values2 member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type) {
     return this.member_slot_value_p(frame,
                                     slot,
                                     value,
                                     inference_level,
                                     test,
                                     slot_type,
                                     _either);
  }


  // Method for "member_slot_value_p" that defaults slot-type to own
  public Values2 member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test) {
     return this.member_slot_value_p(frame,
                                     slot,
                                     value,
                                     inference_level,
                                     test,
                                     _own);
  }


  // Method for "member_slot_value_p" that defaults test to equal
  public Values2 member_slot_value_p
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level) {
     return this.member_slot_value_p(frame,
                                     slot,
                                     value,
                                     inference_level,
                                     _equal);
  }


  // Method for "member_slot_value_p" that defaults inference-level to taxonomic
  public Values2 member_slot_value_p
     (Node frame,
      Node slot,
      Node value) {
     return this.member_slot_value_p(frame,
                                     slot,
                                     value,
                                     _taxonomic);
  }


// ---------------------- open-kb

/**
 * Given a   kb-locator , a   kb-type , and a
 *   connection , returns a KB object
 * for that KB locator that will behave as if all the objects in the
 * KB are accessible (the implementation is not actually required to load the
 * whole KB into memory).
 * 
 * Implementations are at liberty to accept other values in place of the
 *   kb-locator , such as a pathname that identifies the location of
 * the KB to the system.  Such usage is convenient, but is not portable.  It
 * is not portable for an OKBC application to use anything other than
 * a KB locator for this argument.  If   error-p  is  , catches
 * errors that occur, and attempts to continue with the opening/loading
 * process.  If the KB could not be successfully opened, returns  .
 */
  public Node open_kb
     (Node kb_locator,
      Connection connection,
      TruthValue error_p) {
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.open_kb_internal(kb_locator,
                                    connection,
                                    error_p);
     return retval;
    }


  // Method for "open_kb" that defaults error-p to t
  public Node open_kb
     (Node kb_locator,
      Connection connection) {
     return this.open_kb(kb_locator,
                         connection,
                         _T);
  }


// ---------------------- open-kb

/**
 * Given a   kb-locator , a   kb-type , and a
 *   connection , returns a KB object
 * for that KB locator that will behave as if all the objects in the
 * KB are accessible (the implementation is not actually required to load the
 * whole KB into memory).
 * 
 * Implementations are at liberty to accept other values in place of the
 *   kb-locator , such as a pathname that identifies the location of
 * the KB to the system.  Such usage is convenient, but is not portable.  It
 * is not portable for an OKBC application to use anything other than
 * a KB locator for this argument.  If   error-p  is  , catches
 * errors that occur, and attempts to continue with the opening/loading
 * process.  If the KB could not be successfully opened, returns  .
 */
  protected Node open_kb_internal
     (Node kb_locator,
      Connection connection,
      TruthValue error_p) {
     throw new MethodMissing
               (new OKBCString("open_kb"), this).signal();
    }


// ---------------------- openable-kbs

/**
 * Given a   kb-type  and a   connection , returns
 *   list-of-kb-locators , a list of frame handles to
 * frames in the   meta-kb  that are instances of the class
 *    kb-locator .
 * Each kb-locator instance describes one openable KB, identified at the
 * time of the call.  Subsequent calls to openable-kbs will refresh the
 * set of kb-locator instances in the meta-kb.  Kb-locators refering to
 * KBs of   kb-type  that are no longer openable will be removed.  KBs
 * of   kb-type  that have become openable since the last call will
 * become represented in the   meta-kb .
 * 
 *   Place  allows the application to communicate to the KRS in an
 * KRS-specific way where to look for the openable KBs (e.g., a directory).
 * The use of the   place  argument is not portable.
 * If a particular   kb-type  does not understand the value of
 * the   place  argument supplied,   openable-kbs  returns
 *  , that is, there are no known openable KBs consistent
 * with the supplied   place .
 */
  public Node openable_kbs
     (Connection connection,
      Node place) {
     Node retval;
     retval = this.openable_kbs_internal(connection,
                                         place);
     return retval;
    }


  // Method for "openable_kbs" that defaults place to nil
  public Node openable_kbs
     (Connection connection) {
     return this.openable_kbs(connection,
                              _NIL);
  }


// ---------------------- openable-kbs

/**
 * Given a   kb-type  and a   connection , returns
 *   list-of-kb-locators , a list of frame handles to
 * frames in the   meta-kb  that are instances of the class
 *    kb-locator .
 * Each kb-locator instance describes one openable KB, identified at the
 * time of the call.  Subsequent calls to openable-kbs will refresh the
 * set of kb-locator instances in the meta-kb.  Kb-locators refering to
 * KBs of   kb-type  that are no longer openable will be removed.  KBs
 * of   kb-type  that have become openable since the last call will
 * become represented in the   meta-kb .
 * 
 *   Place  allows the application to communicate to the KRS in an
 * KRS-specific way where to look for the openable KBs (e.g., a directory).
 * The use of the   place  argument is not portable.
 * If a particular   kb-type  does not understand the value of
 * the   place  argument supplied,   openable-kbs  returns
 *  , that is, there are no known openable KBs consistent
 * with the supplied   place .
 */
  protected Node openable_kbs_internal
     (Connection connection,
      Node place) {
     throw new MethodMissing
               (new OKBCString("openable_kbs"), this).signal();
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
     Node retval;
     retval = this.primitive_p_internal(the_class,
                                        kb_local_only_p);
     return retval;
    }


  // Method for "primitive_p" that defaults kb-local-only-p to nil
  public Node primitive_p
     (Node the_class) {
     return this.primitive_p(the_class,
                             _NIL);
  }


// ---------------------- primitive-p

/**
 * Returns   iff   class  is a class whose definition is
 * primitive, and   otherwise.  For KRSs that do not distinguish
 * primitive from defined classes,   primitive-p 
 * must return   for all classes.
 */
  protected Node primitive_p_internal
     (Node the_class,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("primitive_p"), this).signal();
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
     Node retval;
     retval = this.print_frame_internal(frame,
                                        slots,
                                        facets,
                                        stream,
                                        inference_level,
                                        number_of_values,
                                        value_selector,
                                        kb_local_only_p);
     return retval;
    }


  // Method for "print_frame" that defaults kb-local-only-p to nil
  public Node print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.print_frame(frame,
                             slots,
                             facets,
                             stream,
                             inference_level,
                             number_of_values,
                             value_selector,
                             _NIL);
  }


  // Method for "print_frame" that defaults value-selector to either
  public Node print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.print_frame(frame,
                             slots,
                             facets,
                             stream,
                             inference_level,
                             number_of_values,
                             _either);
  }


  // Method for "print_frame" that defaults number-of-values to all
  public Node print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level) {
     return this.print_frame(frame,
                             slots,
                             facets,
                             stream,
                             inference_level,
                             _all);
  }


  // Method for "print_frame" that defaults inference-level to taxonomic
  public Node print_frame
     (Node frame,
      Node slots,
      Node facets,
      Node stream) {
     return this.print_frame(frame,
                             slots,
                             facets,
                             stream,
                             _taxonomic);
  }


  // Method for "print_frame" that defaults stream to t
  public Node print_frame
     (Node frame,
      Node slots,
      Node facets) {
     return this.print_frame(frame,
                             slots,
                             facets,
                             _T);
  }


  // Method for "print_frame" that defaults facets to filled
  public Node print_frame
     (Node frame,
      Node slots) {
     return this.print_frame(frame,
                             slots,
                             _filled);
  }


  // Method for "print_frame" that defaults slots to filled
  public Node print_frame
     (Node frame) {
     return this.print_frame(frame,
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
     this.put_behavior_values_internal(behavior,
                                       values);
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
  protected void put_behavior_values_internal
     (Node behavior,
      Node values) {
     throw new MethodMissing
               (new OKBCString("put_behavior_values"), this).signal();
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
     kb_has_been_modified_p = true;
     this.put_class_superclasses_internal(the_class,
                                          new_superclasses,
                                          kb_local_only_p);
    }


  // Method for "put_class_superclasses" that defaults kb-local-only-p to nil
  public void put_class_superclasses
     (Node the_class,
      Node new_superclasses) {
     this.put_class_superclasses(the_class,
                                 new_superclasses,
                                 _NIL);
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
  protected void put_class_superclasses_internal
     (Node the_class,
      Node new_superclasses,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("put_class_superclasses"), this).signal();
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
     kb_has_been_modified_p = true;
     this.put_facet_value_internal(frame,
                                   slot,
                                   facet,
                                   value,
                                   slot_type,
                                   value_selector,
                                   kb_local_only_p);
    }


  // Method for "put_facet_value" that defaults kb-local-only-p to nil
  public void put_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.put_facet_value(frame,
                          slot,
                          facet,
                          value,
                          slot_type,
                          value_selector,
                          _NIL);
  }


  // Method for "put_facet_value" that defaults value-selector to known-true
  public void put_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      SlotType slot_type) {
     this.put_facet_value(frame,
                          slot,
                          facet,
                          value,
                          slot_type,
                          _known_true);
  }


  // Method for "put_facet_value" that defaults slot-type to own
  public void put_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value) {
     this.put_facet_value(frame,
                          slot,
                          facet,
                          value,
                          _own);
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
     kb_has_been_modified_p = true;
     this.put_facet_values_internal(frame,
                                    slot,
                                    facet,
                                    values,
                                    slot_type,
                                    value_selector,
                                    kb_local_only_p);
    }


  // Method for "put_facet_values" that defaults kb-local-only-p to nil
  public void put_facet_values
     (Node frame,
      Node slot,
      Node facet,
      Node values,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.put_facet_values(frame,
                           slot,
                           facet,
                           values,
                           slot_type,
                           value_selector,
                           _NIL);
  }


  // Method for "put_facet_values" that defaults value-selector to known-true
  public void put_facet_values
     (Node frame,
      Node slot,
      Node facet,
      Node values,
      SlotType slot_type) {
     this.put_facet_values(frame,
                           slot,
                           facet,
                           values,
                           slot_type,
                           _known_true);
  }


  // Method for "put_facet_values" that defaults slot-type to own
  public void put_facet_values
     (Node frame,
      Node slot,
      Node facet,
      Node values) {
     this.put_facet_values(frame,
                           slot,
                           facet,
                           values,
                           _own);
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
  protected void put_facet_values_internal
     (Node frame,
      Node slot,
      Node facet,
      Node values,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("put_facet_values"), this).signal();
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
     kb_has_been_modified_p = true;
     this.put_frame_details_internal(frame,
                                     details,
                                     kb_local_only_p);
    }


  // Method for "put_frame_details" that defaults kb-local-only-p to nil
  public void put_frame_details
     (Node frame,
      Node details) {
     this.put_frame_details(frame,
                            details,
                            _NIL);
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.put_frame_name_internal(frame,
                                           new_name,
                                           kb_local_only_p);
     return retval;
    }


  // Method for "put_frame_name" that defaults kb-local-only-p to nil
  public Node put_frame_name
     (Node frame,
      Node new_name) {
     return this.put_frame_name(frame,
                                new_name,
                                _NIL);
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
  protected Node put_frame_name_internal
     (Node frame,
      Node new_name,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("put_frame_name"), this).signal();
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
     kb_has_been_modified_p = true;
     this.put_frame_pretty_name_internal(frame,
                                         name,
                                         kb_local_only_p);
    }


  // Method for "put_frame_pretty_name" that defaults kb-local-only-p to nil
  public void put_frame_pretty_name
     (Node frame,
      Node name) {
     this.put_frame_pretty_name(frame,
                                name,
                                _NIL);
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
  protected void put_frame_pretty_name_internal
     (Node frame,
      Node name,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("put_frame_pretty_name"), this).signal();
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
     kb_has_been_modified_p = true;
     this.put_instance_types_internal(frame,
                                      new_types,
                                      kb_local_only_p);
    }


  // Method for "put_instance_types" that defaults kb-local-only-p to nil
  public void put_instance_types
     (Node frame,
      Node new_types) {
     this.put_instance_types(frame,
                             new_types,
                             _NIL);
  }


// ---------------------- put-instance-types

/**
 * Changes   frame  to be an instance of all the classes listed in
 *   new-types .  If   frame  was an instance of any types not
 * mentioned in   new-types , these types are removed.
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  protected void put_instance_types_internal
     (Node frame,
      Node new_types,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("put_instance_types"), this).signal();
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
     kb_has_been_modified_p = true;
     this.put_slot_value_internal(frame,
                                  slot,
                                  value,
                                  slot_type,
                                  value_selector,
                                  kb_local_only_p);
    }


  // Method for "put_slot_value" that defaults kb-local-only-p to nil
  public void put_slot_value
     (Node frame,
      Node slot,
      Node value,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.put_slot_value(frame,
                         slot,
                         value,
                         slot_type,
                         value_selector,
                         _NIL);
  }


  // Method for "put_slot_value" that defaults value-selector to known-true
  public void put_slot_value
     (Node frame,
      Node slot,
      Node value,
      SlotType slot_type) {
     this.put_slot_value(frame,
                         slot,
                         value,
                         slot_type,
                         _known_true);
  }


  // Method for "put_slot_value" that defaults slot-type to own
  public void put_slot_value
     (Node frame,
      Node slot,
      Node value) {
     this.put_slot_value(frame,
                         slot,
                         value,
                         _own);
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
     kb_has_been_modified_p = true;
     this.put_slot_values_internal(frame,
                                   slot,
                                   values,
                                   slot_type,
                                   value_selector,
                                   kb_local_only_p);
    }


  // Method for "put_slot_values" that defaults kb-local-only-p to nil
  public void put_slot_values
     (Node frame,
      Node slot,
      Node values,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.put_slot_values(frame,
                          slot,
                          values,
                          slot_type,
                          value_selector,
                          _NIL);
  }


  // Method for "put_slot_values" that defaults value-selector to known-true
  public void put_slot_values
     (Node frame,
      Node slot,
      Node values,
      SlotType slot_type) {
     this.put_slot_values(frame,
                          slot,
                          values,
                          slot_type,
                          _known_true);
  }


  // Method for "put_slot_values" that defaults slot-type to own
  public void put_slot_values
     (Node frame,
      Node slot,
      Node values) {
     this.put_slot_values(frame,
                          slot,
                          values,
                          _own);
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
  protected void put_slot_values_internal
     (Node frame,
      Node slot,
      Node values,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("put_slot_values"), this).signal();
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
     kb_has_been_modified_p = true;
     this.register_procedure_internal(name,
                                      procedure);
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
     kb_has_been_modified_p = true;
     this.remove_class_superclass_internal(the_class,
                                           superclass_to_remove,
                                           kb_local_only_p);
    }


  // Method for "remove_class_superclass" that defaults kb-local-only-p to nil
  public void remove_class_superclass
     (Node the_class,
      Node superclass_to_remove) {
     this.remove_class_superclass(the_class,
                                  superclass_to_remove,
                                  _NIL);
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
     kb_has_been_modified_p = true;
     this.remove_facet_value_internal(frame,
                                      slot,
                                      facet,
                                      value,
                                      test,
                                      slot_type,
                                      value_selector,
                                      kb_local_only_p);
    }


  // Method for "remove_facet_value" that defaults kb-local-only-p to nil
  public void remove_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.remove_facet_value(frame,
                             slot,
                             facet,
                             value,
                             test,
                             slot_type,
                             value_selector,
                             _NIL);
  }


  // Method for "remove_facet_value" that defaults value-selector to known-true
  public void remove_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test,
      SlotType slot_type) {
     this.remove_facet_value(frame,
                             slot,
                             facet,
                             value,
                             test,
                             slot_type,
                             _known_true);
  }


  // Method for "remove_facet_value" that defaults slot-type to own
  public void remove_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test) {
     this.remove_facet_value(frame,
                             slot,
                             facet,
                             value,
                             test,
                             _own);
  }


  // Method for "remove_facet_value" that defaults test to equal
  public void remove_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value) {
     this.remove_facet_value(frame,
                             slot,
                             facet,
                             value,
                             _equal);
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
     kb_has_been_modified_p = true;
     this.remove_instance_type_internal(frame,
                                        type_to_remove,
                                        kb_local_only_p);
    }


  // Method for "remove_instance_type" that defaults kb-local-only-p to nil
  public void remove_instance_type
     (Node frame,
      Node type_to_remove) {
     this.remove_instance_type(frame,
                               type_to_remove,
                               _NIL);
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
     kb_has_been_modified_p = true;
     this.remove_local_facet_values_internal(frame,
                                             slot,
                                             facet,
                                             slot_type,
                                             value_selector,
                                             kb_local_only_p);
    }


  // Method for "remove_local_facet_values" that defaults kb-local-only-p to nil
  public void remove_local_facet_values
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.remove_local_facet_values(frame,
                                    slot,
                                    facet,
                                    slot_type,
                                    value_selector,
                                    _NIL);
  }


  // Method for "remove_local_facet_values" that defaults value-selector to known-true
  public void remove_local_facet_values
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type) {
     this.remove_local_facet_values(frame,
                                    slot,
                                    facet,
                                    slot_type,
                                    _known_true);
  }


  // Method for "remove_local_facet_values" that defaults slot-type to own
  public void remove_local_facet_values
     (Node frame,
      Node slot,
      Node facet) {
     this.remove_local_facet_values(frame,
                                    slot,
                                    facet,
                                    _own);
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
     kb_has_been_modified_p = true;
     this.remove_local_slot_values_internal(frame,
                                            slot,
                                            slot_type,
                                            value_selector,
                                            kb_local_only_p);
    }


  // Method for "remove_local_slot_values" that defaults kb-local-only-p to nil
  public void remove_local_slot_values
     (Node frame,
      Node slot,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.remove_local_slot_values(frame,
                                   slot,
                                   slot_type,
                                   value_selector,
                                   _NIL);
  }


  // Method for "remove_local_slot_values" that defaults value-selector to known-true
  public void remove_local_slot_values
     (Node frame,
      Node slot,
      SlotType slot_type) {
     this.remove_local_slot_values(frame,
                                   slot,
                                   slot_type,
                                   _known_true);
  }


  // Method for "remove_local_slot_values" that defaults slot-type to own
  public void remove_local_slot_values
     (Node frame,
      Node slot) {
     this.remove_local_slot_values(frame,
                                   slot,
                                   _own);
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
     kb_has_been_modified_p = true;
     this.remove_slot_value_internal(frame,
                                     slot,
                                     value,
                                     test,
                                     slot_type,
                                     index,
                                     value_selector,
                                     kb_local_only_p);
    }


  // Method for "remove_slot_value" that defaults kb-local-only-p to nil
  public void remove_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type,
      Node index,
      ValueSelector value_selector) {
     this.remove_slot_value(frame,
                            slot,
                            value,
                            test,
                            slot_type,
                            index,
                            value_selector,
                            _NIL);
  }


  // Method for "remove_slot_value" that defaults value-selector to known-true
  public void remove_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type,
      Node index) {
     this.remove_slot_value(frame,
                            slot,
                            value,
                            test,
                            slot_type,
                            index,
                            _known_true);
  }


  // Method for "remove_slot_value" that defaults index to all
  public void remove_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type) {
     this.remove_slot_value(frame,
                            slot,
                            value,
                            test,
                            slot_type,
                            _all);
  }


  // Method for "remove_slot_value" that defaults slot-type to own
  public void remove_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test) {
     this.remove_slot_value(frame,
                            slot,
                            value,
                            test,
                            _own);
  }


  // Method for "remove_slot_value" that defaults test to equal
  public void remove_slot_value
     (Node frame,
      Node slot,
      Node value) {
     this.remove_slot_value(frame,
                            slot,
                            value,
                            _equal);
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.rename_facet_internal(facet,
                                         new_name,
                                         kb_local_only_p);
     return retval;
    }


  // Method for "rename_facet" that defaults kb-local-only-p to nil
  public Node rename_facet
     (Node facet,
      Node new_name) {
     return this.rename_facet(facet,
                              new_name,
                              _NIL);
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
  protected Node rename_facet_internal
     (Node facet,
      Node new_name,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("rename_facet"), this).signal();
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.rename_slot_internal(slot,
                                        new_name,
                                        kb_local_only_p);
     return retval;
    }


  // Method for "rename_slot" that defaults kb-local-only-p to nil
  public Node rename_slot
     (Node slot,
      Node new_name) {
     return this.rename_slot(slot,
                             new_name,
                             _NIL);
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
  protected Node rename_slot_internal
     (Node slot,
      Node new_name,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("rename_slot"), this).signal();
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
     kb_has_been_modified_p = true;
     this.replace_facet_value_internal(frame,
                                       slot,
                                       facet,
                                       old_value,
                                       new_value,
                                       test,
                                       slot_type,
                                       value_selector,
                                       kb_local_only_p);
    }


  // Method for "replace_facet_value" that defaults kb-local-only-p to nil
  public void replace_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.replace_facet_value(frame,
                              slot,
                              facet,
                              old_value,
                              new_value,
                              test,
                              slot_type,
                              value_selector,
                              _NIL);
  }


  // Method for "replace_facet_value" that defaults value-selector to known-true
  public void replace_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type) {
     this.replace_facet_value(frame,
                              slot,
                              facet,
                              old_value,
                              new_value,
                              test,
                              slot_type,
                              _known_true);
  }


  // Method for "replace_facet_value" that defaults slot-type to own
  public void replace_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node old_value,
      Node new_value,
      Node test) {
     this.replace_facet_value(frame,
                              slot,
                              facet,
                              old_value,
                              new_value,
                              test,
                              _own);
  }


  // Method for "replace_facet_value" that defaults test to equal
  public void replace_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node old_value,
      Node new_value) {
     this.replace_facet_value(frame,
                              slot,
                              facet,
                              old_value,
                              new_value,
                              _equal);
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
     kb_has_been_modified_p = true;
     this.replace_slot_value_internal(frame,
                                      slot,
                                      old_value,
                                      new_value,
                                      test,
                                      slot_type,
                                      index,
                                      value_selector,
                                      kb_local_only_p);
    }


  // Method for "replace_slot_value" that defaults kb-local-only-p to nil
  public void replace_slot_value
     (Node frame,
      Node slot,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type,
      Node index,
      ValueSelector value_selector) {
     this.replace_slot_value(frame,
                             slot,
                             old_value,
                             new_value,
                             test,
                             slot_type,
                             index,
                             value_selector,
                             _NIL);
  }


  // Method for "replace_slot_value" that defaults value-selector to known-true
  public void replace_slot_value
     (Node frame,
      Node slot,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type,
      Node index) {
     this.replace_slot_value(frame,
                             slot,
                             old_value,
                             new_value,
                             test,
                             slot_type,
                             index,
                             _known_true);
  }


  // Method for "replace_slot_value" that defaults index to all
  public void replace_slot_value
     (Node frame,
      Node slot,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type) {
     this.replace_slot_value(frame,
                             slot,
                             old_value,
                             new_value,
                             test,
                             slot_type,
                             _all);
  }


  // Method for "replace_slot_value" that defaults slot-type to own
  public void replace_slot_value
     (Node frame,
      Node slot,
      Node old_value,
      Node new_value,
      Node test) {
     this.replace_slot_value(frame,
                             slot,
                             old_value,
                             new_value,
                             test,
                             _own);
  }


  // Method for "replace_slot_value" that defaults test to equal
  public void replace_slot_value
     (Node frame,
      Node slot,
      Node old_value,
      Node new_value) {
     this.replace_slot_value(frame,
                             slot,
                             old_value,
                             new_value,
                             _equal);
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.revert_kb_internal(error_p);
     return retval;
    }


  // Method for "revert_kb" that defaults error-p to t
  public Node revert_kb
     () {
     return this.revert_kb(_T);
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
     Node retval;
     retval = this.save_kb_internal(error_p);
     if (retval != _NIL)
       { kb_has_been_modified_p = false; }
     return retval;
    }


  // Method for "save_kb" that defaults error-p to t
  public Node save_kb
     () {
     return this.save_kb(_T);
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
  protected Node save_kb_internal
     (TruthValue error_p) {
     throw new MethodMissing
               (new OKBCString("save_kb"), this).signal();
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
     this.save_kb_as_internal(new_name_or_locator);
     kb_has_been_modified_p = false;
    }


// ---------------------- save-kb-as

/**
 * Saves the entire contents of the KB to persistent storage under the
 *   new-name-or-locator .    New-name-or-locator  is either a new
 * name for the KB, or a new kb-locator.  The in-core KB is renamed so that
 *   find-kb-of-type  will return   kb  when passed the new name of
 * the KB.  Returns no values.
 */
  protected void save_kb_as_internal
     (Node new_name_or_locator) {
     throw new MethodMissing
               (new OKBCString("save_kb_as"), this).signal();
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
     Node retval;
     retval = this.slot_has_facet_p_internal(frame,
                                             slot,
                                             facet,
                                             inference_level,
                                             slot_type,
                                             kb_local_only_p);
     return retval;
    }


  // Method for "slot_has_facet_p" that defaults kb-local-only-p to nil
  public Node slot_has_facet_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.slot_has_facet_p(frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  slot_type,
                                  _NIL);
  }


  // Method for "slot_has_facet_p" that defaults slot-type to own
  public Node slot_has_facet_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.slot_has_facet_p(frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  _own);
  }


  // Method for "slot_has_facet_p" that defaults inference-level to taxonomic
  public Node slot_has_facet_p
     (Node frame,
      Node slot,
      Node facet) {
     return this.slot_has_facet_p(frame,
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
     Values2 retval;
     retval = this.slot_has_value_p_internal(frame,
                                             slot,
                                             inference_level,
                                             slot_type,
                                             value_selector,
                                             kb_local_only_p);
     return retval;
    }


  // Method for "slot_has_value_p" that defaults kb-local-only-p to nil
  public Values2 slot_has_value_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.slot_has_value_p(frame,
                                  slot,
                                  inference_level,
                                  slot_type,
                                  value_selector,
                                  _NIL);
  }


  // Method for "slot_has_value_p" that defaults value-selector to either
  public Values2 slot_has_value_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.slot_has_value_p(frame,
                                  slot,
                                  inference_level,
                                  slot_type,
                                  _either);
  }


  // Method for "slot_has_value_p" that defaults slot-type to own
  public Values2 slot_has_value_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.slot_has_value_p(frame,
                                  slot,
                                  inference_level,
                                  _own);
  }


  // Method for "slot_has_value_p" that defaults inference-level to taxonomic
  public Values2 slot_has_value_p
     (Node frame,
      Node slot) {
     return this.slot_has_value_p(frame,
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
     Node retval;
     retval = this.slot_p_internal(thing,
                                   kb_local_only_p);
     return retval;
    }


  // Method for "slot_p" that defaults kb-local-only-p to nil
  public Node slot_p
     (Node thing) {
     return this.slot_p(thing,
                        _NIL);
  }


// ---------------------- slot-p

/**
 * Returns   iff   thing  is a slot, and
 * otherwise returns  .
 */
  protected Node slot_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("slot_p"), this).signal();
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
     Values2 retval;
     retval = this.subclass_of_p_internal(subclass,
                                          superclass,
                                          inference_level,
                                          kb_local_only_p);
     return retval;
    }


  // Method for "subclass_of_p" that defaults kb-local-only-p to nil
  public Values2 subclass_of_p
     (Node subclass,
      Node superclass,
      InferenceLevel inference_level) {
     return this.subclass_of_p(subclass,
                               superclass,
                               inference_level,
                               _NIL);
  }


  // Method for "subclass_of_p" that defaults inference-level to taxonomic
  public Values2 subclass_of_p
     (Node subclass,
      Node superclass) {
     return this.subclass_of_p(subclass,
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
     Values2 retval;
     retval = this.superclass_of_p_internal(superclass,
                                            subclass,
                                            inference_level,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "superclass_of_p" that defaults kb-local-only-p to nil
  public Values2 superclass_of_p
     (Node superclass,
      Node subclass,
      InferenceLevel inference_level) {
     return this.superclass_of_p(superclass,
                                 subclass,
                                 inference_level,
                                 _NIL);
  }


  // Method for "superclass_of_p" that defaults inference-level to taxonomic
  public Values2 superclass_of_p
     (Node superclass,
      Node subclass) {
     return this.superclass_of_p(superclass,
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
     this.tell_internal(sentence,
                        frame,
                        value_selector,
                        kb_local_only_p);
    }


  // Method for "tell" that defaults kb-local-only-p to nil
  public void tell
     (Node sentence,
      Node frame,
      ValueSelector value_selector) {
     this.tell(sentence,
               frame,
               value_selector,
               _NIL);
  }


  // Method for "tell" that defaults value-selector to known-true
  public void tell
     (Node sentence,
      Node frame) {
     this.tell(sentence,
               frame,
               _known_true);
  }


  // Method for "tell" that defaults frame to nil
  public void tell
     (Node sentence) {
     this.tell(sentence,
               _NIL);
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
     Node retval;
     retval = this.tellable_internal(sentence,
                                     value_selector,
                                     kb_local_only_p);
     return retval;
    }


  // Method for "tellable" that defaults kb-local-only-p to nil
  public Node tellable
     (Node sentence,
      ValueSelector value_selector) {
     return this.tellable(sentence,
                          value_selector,
                          _NIL);
  }


  // Method for "tellable" that defaults value-selector to known-true
  public Node tellable
     (Node sentence) {
     return this.tellable(sentence,
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
     Values2 retval;
     retval = this.type_of_p_internal(the_class,
                                      thing,
                                      inference_level,
                                      kb_local_only_p);
     return retval;
    }


  // Method for "type_of_p" that defaults kb-local-only-p to nil
  public Values2 type_of_p
     (Node the_class,
      Node thing,
      InferenceLevel inference_level) {
     return this.type_of_p(the_class,
                           thing,
                           inference_level,
                           _NIL);
  }


  // Method for "type_of_p" that defaults inference-level to taxonomic
  public Values2 type_of_p
     (Node the_class,
      Node thing) {
     return this.type_of_p(the_class,
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
     kb_has_been_modified_p = true;
     this.unregister_procedure_internal(name);
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
     Node retval;
     retval = this.untell_internal(sentence,
                                   frame,
                                   value_selector,
                                   kb_local_only_p);
     return retval;
    }


  // Method for "untell" that defaults kb-local-only-p to nil
  public Node untell
     (Node sentence,
      Node frame,
      ValueSelector value_selector) {
     return this.untell(sentence,
                        frame,
                        value_selector,
                        _NIL);
  }


  // Method for "untell" that defaults value-selector to known-true
  public Node untell
     (Node sentence,
      Node frame) {
     return this.untell(sentence,
                        frame,
                        _known_true);
  }


  // Method for "untell" that defaults frame to nil
  public Node untell
     (Node sentence) {
     return this.untell(sentence,
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
     Values2 retval;
     retval = this.value_as_string_internal(value,
                                            purpose,
                                            pretty_p,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "value_as_string" that defaults kb-local-only-p to nil
  public Values2 value_as_string
     (Node value,
      Node purpose,
      Node pretty_p) {
     return this.value_as_string(value,
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
     Node retval;
     retval = this.get_behavior_supported_values_internal(behavior);
     return retval;
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
  protected Node get_behavior_supported_values_internal
     (Node behavior) {
     throw new MethodMissing
               (new OKBCString("get_behavior_supported_values"), this).signal();
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
     Node retval;
     retval = this.get_behavior_values_internal(behavior);
     return retval;
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
  protected Node get_behavior_values_internal
     (Node behavior) {
     throw new MethodMissing
               (new OKBCString("get_behavior_values"), this).signal();
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
     Values3 retval;
     retval = this.get_class_instances_internal(the_class,
                                                inference_level,
                                                number_of_values,
                                                kb_local_only_p);
     return retval;
    }


  // Method for "get_class_instances" that defaults kb-local-only-p to nil
  public Values3 get_class_instances
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.get_class_instances(the_class,
                                     inference_level,
                                     number_of_values,
                                     _NIL);
  }


  // Method for "get_class_instances" that defaults number-of-values to all
  public Values3 get_class_instances
     (Node the_class,
      InferenceLevel inference_level) {
     return this.get_class_instances(the_class,
                                     inference_level,
                                     _all);
  }


  // Method for "get_class_instances" that defaults inference-level to taxonomic
  public Values3 get_class_instances
     (Node the_class) {
     return this.get_class_instances(the_class,
                                     _taxonomic);
  }


// ---------------------- enumerate-class-instances

/**
 * Returns an enumerator for the elements returned by
 *   get-class-instances .
 */
  public Enumerator enumerate_class_instances
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_class_instances_internal(the_class,
                                                      inference_level,
                                                      number_of_values,
                                                      kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_class_instances" that defaults kb-local-only-p to nil
  public Enumerator enumerate_class_instances
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.enumerate_class_instances(the_class,
                                           inference_level,
                                           number_of_values,
                                           _NIL);
  }


  // Method for "enumerate_class_instances" that defaults number-of-values to all
  public Enumerator enumerate_class_instances
     (Node the_class,
      InferenceLevel inference_level) {
     return this.enumerate_class_instances(the_class,
                                           inference_level,
                                           _all);
  }


  // Method for "enumerate_class_instances" that defaults inference-level to taxonomic
  public Enumerator enumerate_class_instances
     (Node the_class) {
     return this.enumerate_class_instances(the_class,
                                           _taxonomic);
  }


// ---------------------- enumerate-class-instances

/**
 * Returns an enumerator for the elements returned by
 *   get-class-instances .
 */
  protected Enumerator enumerate_class_instances_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
      Node result_list = get_class_instances(the_class,
                               inference_level,
                               number_of_values,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Values3 retval;
     retval = this.get_class_subclasses_internal(the_class,
                                                 inference_level,
                                                 number_of_values,
                                                 kb_local_only_p);
     return retval;
    }


  // Method for "get_class_subclasses" that defaults kb-local-only-p to nil
  public Values3 get_class_subclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.get_class_subclasses(the_class,
                                      inference_level,
                                      number_of_values,
                                      _NIL);
  }


  // Method for "get_class_subclasses" that defaults number-of-values to all
  public Values3 get_class_subclasses
     (Node the_class,
      InferenceLevel inference_level) {
     return this.get_class_subclasses(the_class,
                                      inference_level,
                                      _all);
  }


  // Method for "get_class_subclasses" that defaults inference-level to taxonomic
  public Values3 get_class_subclasses
     (Node the_class) {
     return this.get_class_subclasses(the_class,
                                      _taxonomic);
  }


// ---------------------- enumerate-class-subclasses

/**
 * Returns an enumerator for the elements returned by
 *   get-class-subclasses .
 */
  public Enumerator enumerate_class_subclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_class_subclasses_internal(the_class,
                                                       inference_level,
                                                       number_of_values,
                                                       kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_class_subclasses" that defaults kb-local-only-p to nil
  public Enumerator enumerate_class_subclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.enumerate_class_subclasses(the_class,
                                            inference_level,
                                            number_of_values,
                                            _NIL);
  }


  // Method for "enumerate_class_subclasses" that defaults number-of-values to all
  public Enumerator enumerate_class_subclasses
     (Node the_class,
      InferenceLevel inference_level) {
     return this.enumerate_class_subclasses(the_class,
                                            inference_level,
                                            _all);
  }


  // Method for "enumerate_class_subclasses" that defaults inference-level to taxonomic
  public Enumerator enumerate_class_subclasses
     (Node the_class) {
     return this.enumerate_class_subclasses(the_class,
                                            _taxonomic);
  }


// ---------------------- enumerate-class-subclasses

/**
 * Returns an enumerator for the elements returned by
 *   get-class-subclasses .
 */
  protected Enumerator enumerate_class_subclasses_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
      Node result_list = get_class_subclasses(the_class,
                               inference_level,
                               number_of_values,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Values3 retval;
     retval = this.get_class_superclasses_internal(the_class,
                                                   inference_level,
                                                   number_of_values,
                                                   kb_local_only_p);
     return retval;
    }


  // Method for "get_class_superclasses" that defaults kb-local-only-p to nil
  public Values3 get_class_superclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.get_class_superclasses(the_class,
                                        inference_level,
                                        number_of_values,
                                        _NIL);
  }


  // Method for "get_class_superclasses" that defaults number-of-values to all
  public Values3 get_class_superclasses
     (Node the_class,
      InferenceLevel inference_level) {
     return this.get_class_superclasses(the_class,
                                        inference_level,
                                        _all);
  }


  // Method for "get_class_superclasses" that defaults inference-level to taxonomic
  public Values3 get_class_superclasses
     (Node the_class) {
     return this.get_class_superclasses(the_class,
                                        _taxonomic);
  }


// ---------------------- enumerate-class-superclasses

/**
 * Returns an enumerator for the elements returned by
 *   get-class-superclasses .
 */
  public Enumerator enumerate_class_superclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_class_superclasses_internal(the_class,
                                                         inference_level,
                                                         number_of_values,
                                                         kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_class_superclasses" that defaults kb-local-only-p to nil
  public Enumerator enumerate_class_superclasses
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.enumerate_class_superclasses(the_class,
                                              inference_level,
                                              number_of_values,
                                              _NIL);
  }


  // Method for "enumerate_class_superclasses" that defaults number-of-values to all
  public Enumerator enumerate_class_superclasses
     (Node the_class,
      InferenceLevel inference_level) {
     return this.enumerate_class_superclasses(the_class,
                                              inference_level,
                                              _all);
  }


  // Method for "enumerate_class_superclasses" that defaults inference-level to taxonomic
  public Enumerator enumerate_class_superclasses
     (Node the_class) {
     return this.enumerate_class_superclasses(the_class,
                                              _taxonomic);
  }


// ---------------------- enumerate-class-superclasses

/**
 * Returns an enumerator for the elements returned by
 *   get-class-superclasses .
 */
  protected Enumerator enumerate_class_superclasses_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
      Node result_list = get_class_superclasses(the_class,
                               inference_level,
                               number_of_values,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_classes_in_domain_of_internal(slot,
                                                     frame_attachment,
                                                     inference_level,
                                                     kb_local_only_p);
     return retval;
    }


  // Method for "get_classes_in_domain_of" that defaults kb-local-only-p to nil
  public Node get_classes_in_domain_of
     (Node slot,
      Node frame_attachment,
      InferenceLevel inference_level) {
     return this.get_classes_in_domain_of(slot,
                                          frame_attachment,
                                          inference_level,
                                          _NIL);
  }


  // Method for "get_classes_in_domain_of" that defaults inference-level to taxonomic
  public Node get_classes_in_domain_of
     (Node slot,
      Node frame_attachment) {
     return this.get_classes_in_domain_of(slot,
                                          frame_attachment,
                                          _taxonomic);
  }


  // Method for "get_classes_in_domain_of" that defaults frame-attachment to nil
  public Node get_classes_in_domain_of
     (Node slot) {
     return this.get_classes_in_domain_of(slot,
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
     Values2 retval;
     retval = this.get_facet_value_internal(frame,
                                            slot,
                                            facet,
                                            inference_level,
                                            slot_type,
                                            value_selector,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "get_facet_value" that defaults kb-local-only-p to nil
  public Values2 get_facet_value
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.get_facet_value(frame,
                                 slot,
                                 facet,
                                 inference_level,
                                 slot_type,
                                 value_selector,
                                 _NIL);
  }


  // Method for "get_facet_value" that defaults value-selector to either
  public Values2 get_facet_value
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_facet_value(frame,
                                 slot,
                                 facet,
                                 inference_level,
                                 slot_type,
                                 _either);
  }


  // Method for "get_facet_value" that defaults slot-type to own
  public Values2 get_facet_value
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.get_facet_value(frame,
                                 slot,
                                 facet,
                                 inference_level,
                                 _own);
  }


  // Method for "get_facet_value" that defaults inference-level to taxonomic
  public Values2 get_facet_value
     (Node frame,
      Node slot,
      Node facet) {
     return this.get_facet_value(frame,
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
     Values3 retval;
     retval = this.get_facet_values_internal(frame,
                                             slot,
                                             facet,
                                             inference_level,
                                             slot_type,
                                             number_of_values,
                                             value_selector,
                                             kb_local_only_p);
     return retval;
    }


  // Method for "get_facet_values" that defaults kb-local-only-p to nil
  public Values3 get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.get_facet_values(frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  slot_type,
                                  number_of_values,
                                  value_selector,
                                  _NIL);
  }


  // Method for "get_facet_values" that defaults value-selector to either
  public Values3 get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.get_facet_values(frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  slot_type,
                                  number_of_values,
                                  _either);
  }


  // Method for "get_facet_values" that defaults number-of-values to all
  public Values3 get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_facet_values(frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  slot_type,
                                  _all);
  }


  // Method for "get_facet_values" that defaults slot-type to own
  public Values3 get_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.get_facet_values(frame,
                                  slot,
                                  facet,
                                  inference_level,
                                  _own);
  }


  // Method for "get_facet_values" that defaults inference-level to taxonomic
  public Values3 get_facet_values
     (Node frame,
      Node slot,
      Node facet) {
     return this.get_facet_values(frame,
                                  slot,
                                  facet,
                                  _taxonomic);
  }


// ---------------------- enumerate-facet-values

/**
 * Returns an enumerator for the elements returned by
 *   get-facet-values .
 */
  public Enumerator enumerate_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_facet_values_internal(frame,
                                                   slot,
                                                   facet,
                                                   inference_level,
                                                   slot_type,
                                                   number_of_values,
                                                   value_selector,
                                                   kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_facet_values" that defaults kb-local-only-p to nil
  public Enumerator enumerate_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.enumerate_facet_values(frame,
                                        slot,
                                        facet,
                                        inference_level,
                                        slot_type,
                                        number_of_values,
                                        value_selector,
                                        _NIL);
  }


  // Method for "enumerate_facet_values" that defaults value-selector to either
  public Enumerator enumerate_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.enumerate_facet_values(frame,
                                        slot,
                                        facet,
                                        inference_level,
                                        slot_type,
                                        number_of_values,
                                        _either);
  }


  // Method for "enumerate_facet_values" that defaults number-of-values to all
  public Enumerator enumerate_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.enumerate_facet_values(frame,
                                        slot,
                                        facet,
                                        inference_level,
                                        slot_type,
                                        _all);
  }


  // Method for "enumerate_facet_values" that defaults slot-type to own
  public Enumerator enumerate_facet_values
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.enumerate_facet_values(frame,
                                        slot,
                                        facet,
                                        inference_level,
                                        _own);
  }


  // Method for "enumerate_facet_values" that defaults inference-level to taxonomic
  public Enumerator enumerate_facet_values
     (Node frame,
      Node slot,
      Node facet) {
     return this.enumerate_facet_values(frame,
                                        slot,
                                        facet,
                                        _taxonomic);
  }


// ---------------------- enumerate-facet-values

/**
 * Returns an enumerator for the elements returned by
 *   get-facet-values .
 */
  protected Enumerator enumerate_facet_values_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_facet_values(frame,
                               slot,
                               facet,
                               inference_level,
                               slot_type,
                               number_of_values,
                               value_selector,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Values4 retval;
     retval = this.get_facet_values_in_detail_internal(frame,
                                                       slot,
                                                       facet,
                                                       inference_level,
                                                       slot_type,
                                                       number_of_values,
                                                       value_selector,
                                                       kb_local_only_p);
     return retval;
    }


  // Method for "get_facet_values_in_detail" that defaults kb-local-only-p to nil
  public Values4 get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.get_facet_values_in_detail(frame,
                                            slot,
                                            facet,
                                            inference_level,
                                            slot_type,
                                            number_of_values,
                                            value_selector,
                                            _NIL);
  }


  // Method for "get_facet_values_in_detail" that defaults value-selector to either
  public Values4 get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.get_facet_values_in_detail(frame,
                                            slot,
                                            facet,
                                            inference_level,
                                            slot_type,
                                            number_of_values,
                                            _either);
  }


  // Method for "get_facet_values_in_detail" that defaults number-of-values to all
  public Values4 get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_facet_values_in_detail(frame,
                                            slot,
                                            facet,
                                            inference_level,
                                            slot_type,
                                            _all);
  }


  // Method for "get_facet_values_in_detail" that defaults slot-type to own
  public Values4 get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.get_facet_values_in_detail(frame,
                                            slot,
                                            facet,
                                            inference_level,
                                            _own);
  }


  // Method for "get_facet_values_in_detail" that defaults inference-level to taxonomic
  public Values4 get_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet) {
     return this.get_facet_values_in_detail(frame,
                                            slot,
                                            facet,
                                            _taxonomic);
  }


// ---------------------- enumerate-facet-values-in-detail

/**
 * Returns an enumerator for the elements returned by
 *   get-facet-values-in-detail .
 */
  public Enumerator enumerate_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_facet_values_in_detail_internal(frame,
                                                             slot,
                                                             facet,
                                                             inference_level,
                                                             slot_type,
                                                             number_of_values,
                                                             value_selector,
                                                             kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_facet_values_in_detail" that defaults kb-local-only-p to nil
  public Enumerator enumerate_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.enumerate_facet_values_in_detail(frame,
                                                  slot,
                                                  facet,
                                                  inference_level,
                                                  slot_type,
                                                  number_of_values,
                                                  value_selector,
                                                  _NIL);
  }


  // Method for "enumerate_facet_values_in_detail" that defaults value-selector to either
  public Enumerator enumerate_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.enumerate_facet_values_in_detail(frame,
                                                  slot,
                                                  facet,
                                                  inference_level,
                                                  slot_type,
                                                  number_of_values,
                                                  _either);
  }


  // Method for "enumerate_facet_values_in_detail" that defaults number-of-values to all
  public Enumerator enumerate_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.enumerate_facet_values_in_detail(frame,
                                                  slot,
                                                  facet,
                                                  inference_level,
                                                  slot_type,
                                                  _all);
  }


  // Method for "enumerate_facet_values_in_detail" that defaults slot-type to own
  public Enumerator enumerate_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.enumerate_facet_values_in_detail(frame,
                                                  slot,
                                                  facet,
                                                  inference_level,
                                                  _own);
  }


  // Method for "enumerate_facet_values_in_detail" that defaults inference-level to taxonomic
  public Enumerator enumerate_facet_values_in_detail
     (Node frame,
      Node slot,
      Node facet) {
     return this.enumerate_facet_values_in_detail(frame,
                                                  slot,
                                                  facet,
                                                  _taxonomic);
  }


// ---------------------- enumerate-facet-values-in-detail

/**
 * Returns an enumerator for the elements returned by
 *   get-facet-values-in-detail .
 */
  protected Enumerator enumerate_facet_values_in_detail_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_facet_values_in_detail(frame,
                               slot,
                               facet,
                               inference_level,
                               slot_type,
                               number_of_values,
                               value_selector,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Values2 retval;
     retval = this.get_frame_details_internal(frame,
                                              inference_level,
                                              number_of_values,
                                              kb_local_only_p);
     return retval;
    }


  // Method for "get_frame_details" that defaults kb-local-only-p to nil
  public Values2 get_frame_details
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.get_frame_details(frame,
                                   inference_level,
                                   number_of_values,
                                   _NIL);
  }


  // Method for "get_frame_details" that defaults number-of-values to all
  public Values2 get_frame_details
     (Node frame,
      InferenceLevel inference_level) {
     return this.get_frame_details(frame,
                                   inference_level,
                                   _all);
  }


  // Method for "get_frame_details" that defaults inference-level to taxonomic
  public Values2 get_frame_details
     (Node frame) {
     return this.get_frame_details(frame,
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
     Node retval;
     retval = this.get_frame_handle_internal(frame,
                                             kb_local_only_p);
     return retval;
    }


  // Method for "get_frame_handle" that defaults kb-local-only-p to nil
  public Node get_frame_handle
     (Node frame) {
     return this.get_frame_handle(frame,
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
     Values2 retval;
     retval = this.get_frame_in_kb_internal(thing,
                                            error_p,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "get_frame_in_kb" that defaults kb-local-only-p to nil
  public Values2 get_frame_in_kb
     (Node thing,
      TruthValue error_p) {
     return this.get_frame_in_kb(thing,
                                 error_p,
                                 _NIL);
  }


  // Method for "get_frame_in_kb" that defaults error-p to t
  public Values2 get_frame_in_kb
     (Node thing) {
     return this.get_frame_in_kb(thing,
                                 _T);
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
  protected Values2 get_frame_in_kb_internal
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("get_frame_in_kb"), this).signal();
    }


// ---------------------- get-frame-name

/**
 * Returns   frame-name , an entity that is the name of the frame
 * identified by   frame , usually a symbol or string.
 */
  public Node get_frame_name
     (Node frame,
      TruthValue kb_local_only_p) {
     Node retval;
     retval = this.get_frame_name_internal(frame,
                                           kb_local_only_p);
     return retval;
    }


  // Method for "get_frame_name" that defaults kb-local-only-p to nil
  public Node get_frame_name
     (Node frame) {
     return this.get_frame_name(frame,
                                _NIL);
  }


// ---------------------- get-frame-name

/**
 * Returns   frame-name , an entity that is the name of the frame
 * identified by   frame , usually a symbol or string.
 */
  protected Node get_frame_name_internal
     (Node frame,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("get_frame_name"), this).signal();
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
     Node retval;
     retval = this.get_frame_pretty_name_internal(frame,
                                                  kb_local_only_p);
     return retval;
    }


  // Method for "get_frame_pretty_name" that defaults kb-local-only-p to nil
  public Node get_frame_pretty_name
     (Node frame) {
     return this.get_frame_pretty_name(frame,
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
  protected Node get_frame_pretty_name_internal
     (Node frame,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("get_frame_pretty_name"), this).signal();
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
     Values3 retval;
     retval = this.get_frame_sentences_internal(frame,
                                                number_of_values,
                                                okbc_sentences_p,
                                                value_selector,
                                                kb_local_only_p);
     return retval;
    }


  // Method for "get_frame_sentences" that defaults kb-local-only-p to nil
  public Values3 get_frame_sentences
     (Node frame,
      Node number_of_values,
      Node okbc_sentences_p,
      ValueSelector value_selector) {
     return this.get_frame_sentences(frame,
                                     number_of_values,
                                     okbc_sentences_p,
                                     value_selector,
                                     _NIL);
  }


  // Method for "get_frame_sentences" that defaults value-selector to either
  public Values3 get_frame_sentences
     (Node frame,
      Node number_of_values,
      Node okbc_sentences_p) {
     return this.get_frame_sentences(frame,
                                     number_of_values,
                                     okbc_sentences_p,
                                     _either);
  }


  // Method for "get_frame_sentences" that defaults okbc-sentences-p to t
  public Values3 get_frame_sentences
     (Node frame,
      Node number_of_values) {
     return this.get_frame_sentences(frame,
                                     number_of_values,
                                     _T);
  }


  // Method for "get_frame_sentences" that defaults number-of-values to all
  public Values3 get_frame_sentences
     (Node frame) {
     return this.get_frame_sentences(frame,
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
     Values2 retval;
     retval = this.get_frame_slots_internal(frame,
                                            inference_level,
                                            slot_type,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "get_frame_slots" that defaults kb-local-only-p to nil
  public Values2 get_frame_slots
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_frame_slots(frame,
                                 inference_level,
                                 slot_type,
                                 _NIL);
  }


  // Method for "get_frame_slots" that defaults slot-type to all
  public Values2 get_frame_slots
     (Node frame,
      InferenceLevel inference_level) {
     return this.get_frame_slots(frame,
                                 inference_level,
                                 _all);
  }


  // Method for "get_frame_slots" that defaults inference-level to taxonomic
  public Values2 get_frame_slots
     (Node frame) {
     return this.get_frame_slots(frame,
                                 _taxonomic);
  }


// ---------------------- enumerate-frame-slots

/**
 * Returns an enumerator for the elements returned by
 *   get-frame-slots .
 */
  public Enumerator enumerate_frame_slots
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_frame_slots_internal(frame,
                                                  inference_level,
                                                  slot_type,
                                                  kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_frame_slots" that defaults kb-local-only-p to nil
  public Enumerator enumerate_frame_slots
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.enumerate_frame_slots(frame,
                                       inference_level,
                                       slot_type,
                                       _NIL);
  }


  // Method for "enumerate_frame_slots" that defaults slot-type to all
  public Enumerator enumerate_frame_slots
     (Node frame,
      InferenceLevel inference_level) {
     return this.enumerate_frame_slots(frame,
                                       inference_level,
                                       _all);
  }


  // Method for "enumerate_frame_slots" that defaults inference-level to taxonomic
  public Enumerator enumerate_frame_slots
     (Node frame) {
     return this.enumerate_frame_slots(frame,
                                       _taxonomic);
  }


// ---------------------- enumerate-frame-slots

/**
 * Returns an enumerator for the elements returned by
 *   get-frame-slots .
 */
  protected Enumerator enumerate_frame_slots_internal
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
      Node result_list = get_frame_slots(frame,
                               inference_level,
                               slot_type,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_frame_type_internal(thing,
                                           kb_local_only_p);
     return retval;
    }


  // Method for "get_frame_type" that defaults kb-local-only-p to nil
  public Node get_frame_type
     (Node thing) {
     return this.get_frame_type(thing,
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
     Values2 retval;
     retval = this.get_frames_matching_internal(pattern,
                                                wildcards_allowed_p,
                                                selector,
                                                force_case_insensitive_p,
                                                kb_local_only_p);
     return retval;
    }


  // Method for "get_frames_matching" that defaults kb-local-only-p to nil
  public Values2 get_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p) {
     return this.get_frames_matching(pattern,
                                     wildcards_allowed_p,
                                     selector,
                                     force_case_insensitive_p,
                                     _NIL);
  }


  // Method for "get_frames_matching" that defaults force-case-insensitive-p to nil
  public Values2 get_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector) {
     return this.get_frames_matching(pattern,
                                     wildcards_allowed_p,
                                     selector,
                                     _NIL);
  }


  // Method for "get_frames_matching" that defaults selector to all
  public Values2 get_frames_matching
     (Node pattern,
      Node wildcards_allowed_p) {
     return this.get_frames_matching(pattern,
                                     wildcards_allowed_p,
                                     _all);
  }


  // Method for "get_frames_matching" that defaults wildcards-allowed-p to t
  public Values2 get_frames_matching
     (Node pattern) {
     return this.get_frames_matching(pattern,
                                     _T);
  }


// ---------------------- enumerate-frames-matching

/**
 * Returns an enumerator for the elements returned by
 *   get-frames-matching .
 */
  public Enumerator enumerate_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_frames_matching_internal(pattern,
                                                      wildcards_allowed_p,
                                                      selector,
                                                      force_case_insensitive_p,
                                                      kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_frames_matching" that defaults kb-local-only-p to nil
  public Enumerator enumerate_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p) {
     return this.enumerate_frames_matching(pattern,
                                           wildcards_allowed_p,
                                           selector,
                                           force_case_insensitive_p,
                                           _NIL);
  }


  // Method for "enumerate_frames_matching" that defaults force-case-insensitive-p to nil
  public Enumerator enumerate_frames_matching
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector) {
     return this.enumerate_frames_matching(pattern,
                                           wildcards_allowed_p,
                                           selector,
                                           _NIL);
  }


  // Method for "enumerate_frames_matching" that defaults selector to all
  public Enumerator enumerate_frames_matching
     (Node pattern,
      Node wildcards_allowed_p) {
     return this.enumerate_frames_matching(pattern,
                                           wildcards_allowed_p,
                                           _all);
  }


  // Method for "enumerate_frames_matching" that defaults wildcards-allowed-p to t
  public Enumerator enumerate_frames_matching
     (Node pattern) {
     return this.enumerate_frames_matching(pattern,
                                           _T);
  }


// ---------------------- enumerate-frames-matching

/**
 * Returns an enumerator for the elements returned by
 *   get-frames-matching .
 */
  protected Enumerator enumerate_frames_matching_internal
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p,
      TruthValue kb_local_only_p) {
      Node result_list = get_frames_matching(pattern,
                               wildcards_allowed_p,
                               selector,
                               force_case_insensitive_p,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Values2 retval;
     retval = this.get_frames_with_facet_value_internal(slot,
                                                        facet,
                                                        value,
                                                        inference_level,
                                                        slot_type,
                                                        value_selector,
                                                        kb_local_only_p);
     return retval;
    }


  // Method for "get_frames_with_facet_value" that defaults kb-local-only-p to nil
  public Values2 get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.get_frames_with_facet_value(slot,
                                             facet,
                                             value,
                                             inference_level,
                                             slot_type,
                                             value_selector,
                                             _NIL);
  }


  // Method for "get_frames_with_facet_value" that defaults value-selector to either
  public Values2 get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_frames_with_facet_value(slot,
                                             facet,
                                             value,
                                             inference_level,
                                             slot_type,
                                             _either);
  }


  // Method for "get_frames_with_facet_value" that defaults slot-type to own
  public Values2 get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level) {
     return this.get_frames_with_facet_value(slot,
                                             facet,
                                             value,
                                             inference_level,
                                             _own);
  }


  // Method for "get_frames_with_facet_value" that defaults inference-level to taxonomic
  public Values2 get_frames_with_facet_value
     (Node slot,
      Node facet,
      Node value) {
     return this.get_frames_with_facet_value(slot,
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
     Values2 retval;
     retval = this.get_frames_with_slot_value_internal(slot,
                                                       value,
                                                       inference_level,
                                                       slot_type,
                                                       value_selector,
                                                       kb_local_only_p);
     return retval;
    }


  // Method for "get_frames_with_slot_value" that defaults kb-local-only-p to nil
  public Values2 get_frames_with_slot_value
     (Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.get_frames_with_slot_value(slot,
                                            value,
                                            inference_level,
                                            slot_type,
                                            value_selector,
                                            _NIL);
  }


  // Method for "get_frames_with_slot_value" that defaults value-selector to either
  public Values2 get_frames_with_slot_value
     (Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_frames_with_slot_value(slot,
                                            value,
                                            inference_level,
                                            slot_type,
                                            _either);
  }


  // Method for "get_frames_with_slot_value" that defaults slot-type to own
  public Values2 get_frames_with_slot_value
     (Node slot,
      Node value,
      InferenceLevel inference_level) {
     return this.get_frames_with_slot_value(slot,
                                            value,
                                            inference_level,
                                            _own);
  }


  // Method for "get_frames_with_slot_value" that defaults inference-level to taxonomic
  public Values2 get_frames_with_slot_value
     (Node slot,
      Node value) {
     return this.get_frames_with_slot_value(slot,
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
     Values3 retval;
     retval = this.get_instance_types_internal(frame,
                                               inference_level,
                                               number_of_values,
                                               kb_local_only_p);
     return retval;
    }


  // Method for "get_instance_types" that defaults kb-local-only-p to nil
  public Values3 get_instance_types
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.get_instance_types(frame,
                                    inference_level,
                                    number_of_values,
                                    _NIL);
  }


  // Method for "get_instance_types" that defaults number-of-values to all
  public Values3 get_instance_types
     (Node frame,
      InferenceLevel inference_level) {
     return this.get_instance_types(frame,
                                    inference_level,
                                    _all);
  }


  // Method for "get_instance_types" that defaults inference-level to taxonomic
  public Values3 get_instance_types
     (Node frame) {
     return this.get_instance_types(frame,
                                    _taxonomic);
  }


// ---------------------- enumerate-instance-types

/**
 * Returns an enumerator for the elements returned by
 *   get-instance-types .
 */
  public Enumerator enumerate_instance_types
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_instance_types_internal(frame,
                                                     inference_level,
                                                     number_of_values,
                                                     kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_instance_types" that defaults kb-local-only-p to nil
  public Enumerator enumerate_instance_types
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.enumerate_instance_types(frame,
                                          inference_level,
                                          number_of_values,
                                          _NIL);
  }


  // Method for "enumerate_instance_types" that defaults number-of-values to all
  public Enumerator enumerate_instance_types
     (Node frame,
      InferenceLevel inference_level) {
     return this.enumerate_instance_types(frame,
                                          inference_level,
                                          _all);
  }


  // Method for "enumerate_instance_types" that defaults inference-level to taxonomic
  public Enumerator enumerate_instance_types
     (Node frame) {
     return this.enumerate_instance_types(frame,
                                          _taxonomic);
  }


// ---------------------- enumerate-instance-types

/**
 * Returns an enumerator for the elements returned by
 *   get-instance-types .
 */
  protected Enumerator enumerate_instance_types_internal
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
      Node result_list = get_instance_types(frame,
                               inference_level,
                               number_of_values,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_kb_behaviors_internal(connection);
     return retval;
    }


// ---------------------- get-kb-behaviors

/**
 * When   kb-type-or-kb  is either a KB or a kb-type, returns
 *   list-of-behaviors , which is a list of keywords naming all
 * the behaviors recognized by this KB, or identified by the kb-type,
 * respectively.
 */
  protected Node get_kb_behaviors_internal
     (Connection connection) {
     throw new MethodMissing
               (new OKBCString("get_kb_behaviors"), this).signal();
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
     Node retval;
     retval = this.get_kb_classes_internal(selector,
                                           kb_local_only_p);
     return retval;
    }


  // Method for "get_kb_classes" that defaults kb-local-only-p to nil
  public Node get_kb_classes
     (Node selector) {
     return this.get_kb_classes(selector,
                                _NIL);
  }


  // Method for "get_kb_classes" that defaults selector to system-default
  public Node get_kb_classes
     () {
     return this.get_kb_classes(_system_default);
  }


// ---------------------- enumerate-kb-classes

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-classes .
 */
  public Enumerator enumerate_kb_classes
     (Node selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_kb_classes_internal(selector,
                                                 kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_kb_classes" that defaults kb-local-only-p to nil
  public Enumerator enumerate_kb_classes
     (Node selector) {
     return this.enumerate_kb_classes(selector,
                                      _NIL);
  }


  // Method for "enumerate_kb_classes" that defaults selector to system-default
  public Enumerator enumerate_kb_classes
     () {
     return this.enumerate_kb_classes(_system_default);
  }


// ---------------------- enumerate-kb-classes

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-classes .
 */
  protected Enumerator enumerate_kb_classes_internal
     (Node selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_kb_classes(selector,
                               kb_local_only_p);
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_kb_direct_children_internal();
     return retval;
    }


// ---------------------- enumerate-kb-direct-children

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-direct-children .
 */
  public Enumerator enumerate_kb_direct_children
     () {
     Enumerator retval;
     retval = this.enumerate_kb_direct_children_internal();
     return retval;
    }


// ---------------------- enumerate-kb-direct-children

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-direct-children .
 */
  protected Enumerator enumerate_kb_direct_children_internal
     () {
      Node result_list = get_kb_direct_children();
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_kb_direct_parents_internal();
     return retval;
    }


// ---------------------- enumerate-kb-direct-parents

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-direct-parents .
 */
  public Enumerator enumerate_kb_direct_parents
     () {
     Enumerator retval;
     retval = this.enumerate_kb_direct_parents_internal();
     return retval;
    }


// ---------------------- enumerate-kb-direct-parents

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-direct-parents .
 */
  protected Enumerator enumerate_kb_direct_parents_internal
     () {
      Node result_list = get_kb_direct_parents();
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_kb_facets_internal(selector,
                                          kb_local_only_p);
     return retval;
    }


  // Method for "get_kb_facets" that defaults kb-local-only-p to nil
  public Node get_kb_facets
     (Node selector) {
     return this.get_kb_facets(selector,
                               _NIL);
  }


  // Method for "get_kb_facets" that defaults selector to system-default
  public Node get_kb_facets
     () {
     return this.get_kb_facets(_system_default);
  }


// ---------------------- enumerate-kb-facets

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-facets .
 */
  public Enumerator enumerate_kb_facets
     (Node selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_kb_facets_internal(selector,
                                                kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_kb_facets" that defaults kb-local-only-p to nil
  public Enumerator enumerate_kb_facets
     (Node selector) {
     return this.enumerate_kb_facets(selector,
                                     _NIL);
  }


  // Method for "enumerate_kb_facets" that defaults selector to system-default
  public Enumerator enumerate_kb_facets
     () {
     return this.enumerate_kb_facets(_system_default);
  }


// ---------------------- enumerate-kb-facets

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-facets .
 */
  protected Enumerator enumerate_kb_facets_internal
     (Node selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_kb_facets(selector,
                               kb_local_only_p);
      return result_list.enumerate_list();
    }


// ---------------------- get-kb-frames

/**
 * Returns the   list-of-frames  in the KB, including class, slot,
 * facets and individual frames, when present.
 */
  public Node get_kb_frames
     (TruthValue kb_local_only_p) {
     Node retval;
     retval = this.get_kb_frames_internal(kb_local_only_p);
     return retval;
    }


  // Method for "get_kb_frames" that defaults kb-local-only-p to nil
  public Node get_kb_frames
     () {
     return this.get_kb_frames(_NIL);
  }


// ---------------------- get-kb-frames

/**
 * Returns the   list-of-frames  in the KB, including class, slot,
 * facets and individual frames, when present.
 */
  protected Node get_kb_frames_internal
     (TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("get_kb_frames"), this).signal();
    }


// ---------------------- enumerate-kb-frames

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-frames .
 */
  public Enumerator enumerate_kb_frames
     (TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_kb_frames_internal(kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_kb_frames" that defaults kb-local-only-p to nil
  public Enumerator enumerate_kb_frames
     () {
     return this.enumerate_kb_frames(_NIL);
  }


// ---------------------- enumerate-kb-frames

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-frames .
 */
  protected Enumerator enumerate_kb_frames_internal
     (TruthValue kb_local_only_p) {
      Node result_list = get_kb_frames(kb_local_only_p);
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_kb_individuals_internal(selector,
                                               kb_local_only_p);
     return retval;
    }


  // Method for "get_kb_individuals" that defaults kb-local-only-p to nil
  public Node get_kb_individuals
     (Node selector) {
     return this.get_kb_individuals(selector,
                                    _NIL);
  }


  // Method for "get_kb_individuals" that defaults selector to system-default
  public Node get_kb_individuals
     () {
     return this.get_kb_individuals(_system_default);
  }


// ---------------------- enumerate-kb-individuals

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-individuals .
 */
  public Enumerator enumerate_kb_individuals
     (Node selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_kb_individuals_internal(selector,
                                                     kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_kb_individuals" that defaults kb-local-only-p to nil
  public Enumerator enumerate_kb_individuals
     (Node selector) {
     return this.enumerate_kb_individuals(selector,
                                          _NIL);
  }


  // Method for "enumerate_kb_individuals" that defaults selector to system-default
  public Enumerator enumerate_kb_individuals
     () {
     return this.enumerate_kb_individuals(_system_default);
  }


// ---------------------- enumerate-kb-individuals

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-individuals .
 */
  protected Enumerator enumerate_kb_individuals_internal
     (Node selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_kb_individuals(selector,
                               kb_local_only_p);
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_kb_roots_internal(selector,
                                         kb_local_only_p);
     return retval;
    }


  // Method for "get_kb_roots" that defaults kb-local-only-p to nil
  public Node get_kb_roots
     (Node selector) {
     return this.get_kb_roots(selector,
                              _NIL);
  }


  // Method for "get_kb_roots" that defaults selector to all
  public Node get_kb_roots
     () {
     return this.get_kb_roots(_all);
  }


// ---------------------- enumerate-kb-roots

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-roots .
 */
  public Enumerator enumerate_kb_roots
     (Node selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_kb_roots_internal(selector,
                                               kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_kb_roots" that defaults kb-local-only-p to nil
  public Enumerator enumerate_kb_roots
     (Node selector) {
     return this.enumerate_kb_roots(selector,
                                    _NIL);
  }


  // Method for "enumerate_kb_roots" that defaults selector to all
  public Enumerator enumerate_kb_roots
     () {
     return this.enumerate_kb_roots(_all);
  }


// ---------------------- enumerate-kb-roots

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-roots .
 */
  protected Enumerator enumerate_kb_roots_internal
     (Node selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_kb_roots(selector,
                               kb_local_only_p);
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_kb_slots_internal(selector,
                                         kb_local_only_p);
     return retval;
    }


  // Method for "get_kb_slots" that defaults kb-local-only-p to nil
  public Node get_kb_slots
     (Node selector) {
     return this.get_kb_slots(selector,
                              _NIL);
  }


  // Method for "get_kb_slots" that defaults selector to system-default
  public Node get_kb_slots
     () {
     return this.get_kb_slots(_system_default);
  }


// ---------------------- enumerate-kb-slots

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-slots .
 */
  public Enumerator enumerate_kb_slots
     (Node selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_kb_slots_internal(selector,
                                               kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_kb_slots" that defaults kb-local-only-p to nil
  public Enumerator enumerate_kb_slots
     (Node selector) {
     return this.enumerate_kb_slots(selector,
                                    _NIL);
  }


  // Method for "enumerate_kb_slots" that defaults selector to system-default
  public Enumerator enumerate_kb_slots
     () {
     return this.enumerate_kb_slots(_system_default);
  }


// ---------------------- enumerate-kb-slots

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-slots .
 */
  protected Enumerator enumerate_kb_slots_internal
     (Node selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_kb_slots(selector,
                               kb_local_only_p);
      return result_list.enumerate_list();
    }


// ---------------------- get-kbs-of-type

/**
 * Returns   list-of-kbs , the list of all the known KBs whose type
 * matches   kb-type , and that are accessible through the
 *   connection .
 */
  public Node get_kbs_of_type
     (Connection connection) {
     Node retval;
     retval = this.get_kbs_of_type_internal(connection);
     return retval;
    }


// ---------------------- enumerate-kbs-of-type

/**
 * Returns an enumerator for the elements returned by
 *   get-kbs-of-type .
 */
  public Enumerator enumerate_kbs_of_type
     (Connection connection) {
     Enumerator retval;
     retval = this.enumerate_kbs_of_type_internal(connection);
     return retval;
    }


// ---------------------- enumerate-kbs-of-type

/**
 * Returns an enumerator for the elements returned by
 *   get-kbs-of-type .
 */
  protected Enumerator enumerate_kbs_of_type_internal
     (Connection connection) {
      Node result_list = get_kbs_of_type(connection);
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_procedure_internal(name);
     return retval;
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
     Values2 retval;
     retval = this.get_slot_facets_internal(frame,
                                            slot,
                                            inference_level,
                                            slot_type,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "get_slot_facets" that defaults kb-local-only-p to nil
  public Values2 get_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_slot_facets(frame,
                                 slot,
                                 inference_level,
                                 slot_type,
                                 _NIL);
  }


  // Method for "get_slot_facets" that defaults slot-type to own
  public Values2 get_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.get_slot_facets(frame,
                                 slot,
                                 inference_level,
                                 _own);
  }


  // Method for "get_slot_facets" that defaults inference-level to taxonomic
  public Values2 get_slot_facets
     (Node frame,
      Node slot) {
     return this.get_slot_facets(frame,
                                 slot,
                                 _taxonomic);
  }


// ---------------------- enumerate-slot-facets

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-facets .
 */
  public Enumerator enumerate_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_slot_facets_internal(frame,
                                                  slot,
                                                  inference_level,
                                                  slot_type,
                                                  kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_slot_facets" that defaults kb-local-only-p to nil
  public Enumerator enumerate_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.enumerate_slot_facets(frame,
                                       slot,
                                       inference_level,
                                       slot_type,
                                       _NIL);
  }


  // Method for "enumerate_slot_facets" that defaults slot-type to own
  public Enumerator enumerate_slot_facets
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.enumerate_slot_facets(frame,
                                       slot,
                                       inference_level,
                                       _own);
  }


  // Method for "enumerate_slot_facets" that defaults inference-level to taxonomic
  public Enumerator enumerate_slot_facets
     (Node frame,
      Node slot) {
     return this.enumerate_slot_facets(frame,
                                       slot,
                                       _taxonomic);
  }


// ---------------------- enumerate-slot-facets

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-facets .
 */
  protected Enumerator enumerate_slot_facets_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
      Node result_list = get_slot_facets(frame,
                               slot,
                               inference_level,
                               slot_type,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.get_slot_type_internal(frame,
                                          slot,
                                          inference_level,
                                          kb_local_only_p);
     return retval;
    }


  // Method for "get_slot_type" that defaults kb-local-only-p to nil
  public Node get_slot_type
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.get_slot_type(frame,
                               slot,
                               inference_level,
                               _NIL);
  }


  // Method for "get_slot_type" that defaults inference-level to taxonomic
  public Node get_slot_type
     (Node frame,
      Node slot) {
     return this.get_slot_type(frame,
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
     Values2 retval;
     retval = this.get_slot_value_internal(frame,
                                           slot,
                                           inference_level,
                                           slot_type,
                                           value_selector,
                                           kb_local_only_p);
     return retval;
    }


  // Method for "get_slot_value" that defaults kb-local-only-p to nil
  public Values2 get_slot_value
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.get_slot_value(frame,
                                slot,
                                inference_level,
                                slot_type,
                                value_selector,
                                _NIL);
  }


  // Method for "get_slot_value" that defaults value-selector to either
  public Values2 get_slot_value
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_slot_value(frame,
                                slot,
                                inference_level,
                                slot_type,
                                _either);
  }


  // Method for "get_slot_value" that defaults slot-type to own
  public Values2 get_slot_value
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.get_slot_value(frame,
                                slot,
                                inference_level,
                                _own);
  }


  // Method for "get_slot_value" that defaults inference-level to taxonomic
  public Values2 get_slot_value
     (Node frame,
      Node slot) {
     return this.get_slot_value(frame,
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
     Values3 retval;
     retval = this.get_slot_values_internal(frame,
                                            slot,
                                            inference_level,
                                            slot_type,
                                            number_of_values,
                                            value_selector,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "get_slot_values" that defaults kb-local-only-p to nil
  public Values3 get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.get_slot_values(frame,
                                 slot,
                                 inference_level,
                                 slot_type,
                                 number_of_values,
                                 value_selector,
                                 _NIL);
  }


  // Method for "get_slot_values" that defaults value-selector to either
  public Values3 get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.get_slot_values(frame,
                                 slot,
                                 inference_level,
                                 slot_type,
                                 number_of_values,
                                 _either);
  }


  // Method for "get_slot_values" that defaults number-of-values to all
  public Values3 get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_slot_values(frame,
                                 slot,
                                 inference_level,
                                 slot_type,
                                 _all);
  }


  // Method for "get_slot_values" that defaults slot-type to own
  public Values3 get_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.get_slot_values(frame,
                                 slot,
                                 inference_level,
                                 _own);
  }


  // Method for "get_slot_values" that defaults inference-level to taxonomic
  public Values3 get_slot_values
     (Node frame,
      Node slot) {
     return this.get_slot_values(frame,
                                 slot,
                                 _taxonomic);
  }


// ---------------------- enumerate-slot-values

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-values .
 */
  public Enumerator enumerate_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_slot_values_internal(frame,
                                                  slot,
                                                  inference_level,
                                                  slot_type,
                                                  number_of_values,
                                                  value_selector,
                                                  kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_slot_values" that defaults kb-local-only-p to nil
  public Enumerator enumerate_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.enumerate_slot_values(frame,
                                       slot,
                                       inference_level,
                                       slot_type,
                                       number_of_values,
                                       value_selector,
                                       _NIL);
  }


  // Method for "enumerate_slot_values" that defaults value-selector to either
  public Enumerator enumerate_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.enumerate_slot_values(frame,
                                       slot,
                                       inference_level,
                                       slot_type,
                                       number_of_values,
                                       _either);
  }


  // Method for "enumerate_slot_values" that defaults number-of-values to all
  public Enumerator enumerate_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.enumerate_slot_values(frame,
                                       slot,
                                       inference_level,
                                       slot_type,
                                       _all);
  }


  // Method for "enumerate_slot_values" that defaults slot-type to own
  public Enumerator enumerate_slot_values
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.enumerate_slot_values(frame,
                                       slot,
                                       inference_level,
                                       _own);
  }


  // Method for "enumerate_slot_values" that defaults inference-level to taxonomic
  public Enumerator enumerate_slot_values
     (Node frame,
      Node slot) {
     return this.enumerate_slot_values(frame,
                                       slot,
                                       _taxonomic);
  }


// ---------------------- enumerate-slot-values

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-values .
 */
  protected Enumerator enumerate_slot_values_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_slot_values(frame,
                               slot,
                               inference_level,
                               slot_type,
                               number_of_values,
                               value_selector,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Values4 retval;
     retval = this.get_slot_values_in_detail_internal(frame,
                                                      slot,
                                                      inference_level,
                                                      slot_type,
                                                      number_of_values,
                                                      value_selector,
                                                      kb_local_only_p);
     return retval;
    }


  // Method for "get_slot_values_in_detail" that defaults kb-local-only-p to nil
  public Values4 get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.get_slot_values_in_detail(frame,
                                           slot,
                                           inference_level,
                                           slot_type,
                                           number_of_values,
                                           value_selector,
                                           _NIL);
  }


  // Method for "get_slot_values_in_detail" that defaults value-selector to either
  public Values4 get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.get_slot_values_in_detail(frame,
                                           slot,
                                           inference_level,
                                           slot_type,
                                           number_of_values,
                                           _either);
  }


  // Method for "get_slot_values_in_detail" that defaults number-of-values to all
  public Values4 get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.get_slot_values_in_detail(frame,
                                           slot,
                                           inference_level,
                                           slot_type,
                                           _all);
  }


  // Method for "get_slot_values_in_detail" that defaults slot-type to own
  public Values4 get_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.get_slot_values_in_detail(frame,
                                           slot,
                                           inference_level,
                                           _own);
  }


  // Method for "get_slot_values_in_detail" that defaults inference-level to taxonomic
  public Values4 get_slot_values_in_detail
     (Node frame,
      Node slot) {
     return this.get_slot_values_in_detail(frame,
                                           slot,
                                           _taxonomic);
  }


// ---------------------- enumerate-slot-values-in-detail

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-values-in-detail .
 */
  public Enumerator enumerate_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_slot_values_in_detail_internal(frame,
                                                            slot,
                                                            inference_level,
                                                            slot_type,
                                                            number_of_values,
                                                            value_selector,
                                                            kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_slot_values_in_detail" that defaults kb-local-only-p to nil
  public Enumerator enumerate_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector) {
     return this.enumerate_slot_values_in_detail(frame,
                                                 slot,
                                                 inference_level,
                                                 slot_type,
                                                 number_of_values,
                                                 value_selector,
                                                 _NIL);
  }


  // Method for "enumerate_slot_values_in_detail" that defaults value-selector to either
  public Enumerator enumerate_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values) {
     return this.enumerate_slot_values_in_detail(frame,
                                                 slot,
                                                 inference_level,
                                                 slot_type,
                                                 number_of_values,
                                                 _either);
  }


  // Method for "enumerate_slot_values_in_detail" that defaults number-of-values to all
  public Enumerator enumerate_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.enumerate_slot_values_in_detail(frame,
                                                 slot,
                                                 inference_level,
                                                 slot_type,
                                                 _all);
  }


  // Method for "enumerate_slot_values_in_detail" that defaults slot-type to own
  public Enumerator enumerate_slot_values_in_detail
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.enumerate_slot_values_in_detail(frame,
                                                 slot,
                                                 inference_level,
                                                 _own);
  }


  // Method for "enumerate_slot_values_in_detail" that defaults inference-level to taxonomic
  public Enumerator enumerate_slot_values_in_detail
     (Node frame,
      Node slot) {
     return this.enumerate_slot_values_in_detail(frame,
                                                 slot,
                                                 _taxonomic);
  }


// ---------------------- enumerate-slot-values-in-detail

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-values-in-detail .
 */
  protected Enumerator enumerate_slot_values_in_detail_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
      Node result_list = get_slot_values_in_detail(frame,
                               slot,
                               inference_level,
                               slot_type,
                               number_of_values,
                               value_selector,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
    }


// ---------------------- individual-p

/**
 * Returns   if   thing  identifies an individual entity, and
 * returns   if   thing  identifies a class.
 */
  public Node individual_p
     (Node thing,
      TruthValue kb_local_only_p) {
     Node retval;
     retval = this.individual_p_internal(thing,
                                         kb_local_only_p);
     return retval;
    }


  // Method for "individual_p" that defaults kb-local-only-p to nil
  public Node individual_p
     (Node thing) {
     return this.individual_p(thing,
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
     Values2 retval;
     retval = this.instance_of_p_internal(thing,
                                          the_class,
                                          inference_level,
                                          kb_local_only_p);
     return retval;
    }


  // Method for "instance_of_p" that defaults kb-local-only-p to nil
  public Values2 instance_of_p
     (Node thing,
      Node the_class,
      InferenceLevel inference_level) {
     return this.instance_of_p(thing,
                               the_class,
                               inference_level,
                               _NIL);
  }


  // Method for "instance_of_p" that defaults inference-level to taxonomic
  public Values2 instance_of_p
     (Node thing,
      Node the_class) {
     return this.instance_of_p(thing,
                               the_class,
                               _taxonomic);
  }


// ---------------------- kb-modified-p

/**
 * Returns   if   kb  has been modified since it was last saved.
 */
  public Node kb_modified_p
     () {
     Node retval;
     retval = this.kb_modified_p_internal();
     return retval;
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
     kb_has_been_modified_p = true;
     this.add_class_superclass_internal(the_class,
                                        new_superclass,
                                        kb_local_only_p);
    }


  // Method for "add_class_superclass" that defaults kb-local-only-p to nil
  public void add_class_superclass
     (Node the_class,
      Node new_superclass) {
     this.add_class_superclass(the_class,
                               new_superclass,
                               _NIL);
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
     kb_has_been_modified_p = true;
     this.add_facet_value_internal(frame,
                                   slot,
                                   facet,
                                   value,
                                   test,
                                   slot_type,
                                   value_selector,
                                   kb_local_only_p);
    }


  // Method for "add_facet_value" that defaults kb-local-only-p to nil
  public void add_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector) {
     this.add_facet_value(frame,
                          slot,
                          facet,
                          value,
                          test,
                          slot_type,
                          value_selector,
                          _NIL);
  }


  // Method for "add_facet_value" that defaults value-selector to known-true
  public void add_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test,
      SlotType slot_type) {
     this.add_facet_value(frame,
                          slot,
                          facet,
                          value,
                          test,
                          slot_type,
                          _known_true);
  }


  // Method for "add_facet_value" that defaults slot-type to own
  public void add_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test) {
     this.add_facet_value(frame,
                          slot,
                          facet,
                          value,
                          test,
                          _own);
  }


  // Method for "add_facet_value" that defaults test to equal
  public void add_facet_value
     (Node frame,
      Node slot,
      Node facet,
      Node value) {
     this.add_facet_value(frame,
                          slot,
                          facet,
                          value,
                          _equal);
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
     kb_has_been_modified_p = true;
     this.add_instance_type_internal(frame,
                                     new_type,
                                     kb_local_only_p);
    }


  // Method for "add_instance_type" that defaults kb-local-only-p to nil
  public void add_instance_type
     (Node frame,
      Node new_type) {
     this.add_instance_type(frame,
                            new_type,
                            _NIL);
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
     kb_has_been_modified_p = true;
     this.add_slot_value_internal(frame,
                                  slot,
                                  value,
                                  test,
                                  slot_type,
                                  add_before,
                                  value_selector,
                                  kb_local_only_p);
    }


  // Method for "add_slot_value" that defaults kb-local-only-p to nil
  public void add_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type,
      Node add_before,
      ValueSelector value_selector) {
     this.add_slot_value(frame,
                         slot,
                         value,
                         test,
                         slot_type,
                         add_before,
                         value_selector,
                         _NIL);
  }


  // Method for "add_slot_value" that defaults value-selector to known-true
  public void add_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type,
      Node add_before) {
     this.add_slot_value(frame,
                         slot,
                         value,
                         test,
                         slot_type,
                         add_before,
                         _known_true);
  }


  // Method for "add_slot_value" that defaults add-before to 0
  public void add_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type) {
     this.add_slot_value(frame,
                         slot,
                         value,
                         test,
                         slot_type,
                         new OKBCInteger(0));
  }


  // Method for "add_slot_value" that defaults slot-type to own
  public void add_slot_value
     (Node frame,
      Node slot,
      Node value,
      Node test) {
     this.add_slot_value(frame,
                         slot,
                         value,
                         test,
                         _own);
  }


  // Method for "add_slot_value" that defaults test to equal
  public void add_slot_value
     (Node frame,
      Node slot,
      Node value) {
     this.add_slot_value(frame,
                         slot,
                         value,
                         _equal);
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.allocate_frame_handle_internal(frame_name,
                                                  frame_type,
                                                  frame_handle);
     return retval;
    }


  // Method for "allocate_frame_handle" that defaults frame-handle to nil
  public Node allocate_frame_handle
     (Node frame_name,
      ContextName frame_type) {
     return this.allocate_frame_handle(frame_name,
                                       frame_type,
                                       _NIL);
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
  protected Node allocate_frame_handle_internal
     (Node frame_name,
      ContextName frame_type,
      Node frame_handle) {
     throw new MethodMissing
               (new OKBCString("allocate_frame_handle"), this).signal();
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
     Values3 retval;
     retval = this.ask_internal(query,
                                reply_pattern,
                                inference_level,
                                number_of_values,
                                error_p,
                                where,
                                timeout,
                                value_selector,
                                kb_local_only_p);
     return retval;
    }


  // Method for "ask" that defaults kb-local-only-p to nil
  public Values3 ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector) {
     return this.ask(query,
                     reply_pattern,
                     inference_level,
                     number_of_values,
                     error_p,
                     where,
                     timeout,
                     value_selector,
                     _NIL);
  }


  // Method for "ask" that defaults value-selector to either
  public Values3 ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout) {
     return this.ask(query,
                     reply_pattern,
                     inference_level,
                     number_of_values,
                     error_p,
                     where,
                     timeout,
                     _either);
  }


  // Method for "ask" that defaults timeout to nil
  public Values3 ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where) {
     return this.ask(query,
                     reply_pattern,
                     inference_level,
                     number_of_values,
                     error_p,
                     where,
                     _NIL);
  }


  // Method for "ask" that defaults where to nil
  public Values3 ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p) {
     return this.ask(query,
                     reply_pattern,
                     inference_level,
                     number_of_values,
                     error_p,
                     _NIL);
  }


  // Method for "ask" that defaults error-p to t
  public Values3 ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.ask(query,
                     reply_pattern,
                     inference_level,
                     number_of_values,
                     _T);
  }


  // Method for "ask" that defaults number-of-values to all
  public Values3 ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level) {
     return this.ask(query,
                     reply_pattern,
                     inference_level,
                     _all);
  }


  // Method for "ask" that defaults inference-level to taxonomic
  public Values3 ask
     (Node query,
      Node reply_pattern) {
     return this.ask(query,
                     reply_pattern,
                     _taxonomic);
  }


  // Method for "ask" that defaults reply-pattern to query
  public Values3 ask
     (Node query) {
     return this.ask(query,
                     query);
  }


// ---------------------- enumerate-ask

/**
 * Returns an enumerator for the elements returned by
 *   ask .
 */
  public Enumerator enumerate_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Enumerator retval;
     retval = this.enumerate_ask_internal(query,
                                          reply_pattern,
                                          inference_level,
                                          number_of_values,
                                          error_p,
                                          where,
                                          timeout,
                                          value_selector,
                                          kb_local_only_p);
     return retval;
    }


  // Method for "enumerate_ask" that defaults kb-local-only-p to nil
  public Enumerator enumerate_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector) {
     return this.enumerate_ask(query,
                               reply_pattern,
                               inference_level,
                               number_of_values,
                               error_p,
                               where,
                               timeout,
                               value_selector,
                               _NIL);
  }


  // Method for "enumerate_ask" that defaults value-selector to either
  public Enumerator enumerate_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout) {
     return this.enumerate_ask(query,
                               reply_pattern,
                               inference_level,
                               number_of_values,
                               error_p,
                               where,
                               timeout,
                               _either);
  }


  // Method for "enumerate_ask" that defaults timeout to nil
  public Enumerator enumerate_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where) {
     return this.enumerate_ask(query,
                               reply_pattern,
                               inference_level,
                               number_of_values,
                               error_p,
                               where,
                               _NIL);
  }


  // Method for "enumerate_ask" that defaults where to nil
  public Enumerator enumerate_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p) {
     return this.enumerate_ask(query,
                               reply_pattern,
                               inference_level,
                               number_of_values,
                               error_p,
                               _NIL);
  }


  // Method for "enumerate_ask" that defaults error-p to t
  public Enumerator enumerate_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values) {
     return this.enumerate_ask(query,
                               reply_pattern,
                               inference_level,
                               number_of_values,
                               _T);
  }


  // Method for "enumerate_ask" that defaults number-of-values to all
  public Enumerator enumerate_ask
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level) {
     return this.enumerate_ask(query,
                               reply_pattern,
                               inference_level,
                               _all);
  }


  // Method for "enumerate_ask" that defaults inference-level to taxonomic
  public Enumerator enumerate_ask
     (Node query,
      Node reply_pattern) {
     return this.enumerate_ask(query,
                               reply_pattern,
                               _taxonomic);
  }


  // Method for "enumerate_ask" that defaults reply-pattern to query
  public Enumerator enumerate_ask
     (Node query) {
     return this.enumerate_ask(query,
                               query);
  }


// ---------------------- enumerate-ask

/**
 * Returns an enumerator for the elements returned by
 *   ask .
 */
  protected Enumerator enumerate_ask_internal
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
      Node result_list = ask(query,
                               reply_pattern,
                               inference_level,
                               number_of_values,
                               error_p,
                               where,
                               timeout,
                               value_selector,
                               kb_local_only_p).firstValue();
      return result_list.enumerate_list();
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
     Node retval;
     retval = this.askable_internal(sentence,
                                    value_selector,
                                    kb_local_only_p);
     return retval;
    }


  // Method for "askable" that defaults kb-local-only-p to nil
  public Node askable
     (Node sentence,
      ValueSelector value_selector) {
     return this.askable(sentence,
                         value_selector,
                         _NIL);
  }


  // Method for "askable" that defaults value-selector to either
  public Node askable
     (Node sentence) {
     return this.askable(sentence,
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
     kb_has_been_modified_p = true;
     this.attach_facet_internal(frame,
                                slot,
                                facet,
                                slot_type,
                                kb_local_only_p);
    }


  // Method for "attach_facet" that defaults kb-local-only-p to nil
  public void attach_facet
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type) {
     this.attach_facet(frame,
                       slot,
                       facet,
                       slot_type,
                       _NIL);
  }


  // Method for "attach_facet" that defaults slot-type to own
  public void attach_facet
     (Node frame,
      Node slot,
      Node facet) {
     this.attach_facet(frame,
                       slot,
                       facet,
                       _own);
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
  protected void attach_facet_internal
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("attach_facet"), this).signal();
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
     kb_has_been_modified_p = true;
     this.attach_slot_internal(frame,
                               slot,
                               slot_type,
                               kb_local_only_p);
    }


  // Method for "attach_slot" that defaults kb-local-only-p to nil
  public void attach_slot
     (Node frame,
      Node slot,
      SlotType slot_type) {
     this.attach_slot(frame,
                      slot,
                      slot_type,
                      _NIL);
  }


  // Method for "attach_slot" that defaults slot-type to own
  public void attach_slot
     (Node frame,
      Node slot) {
     this.attach_slot(frame,
                      slot,
                      _own);
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
  protected void attach_slot_internal
     (Node frame,
      Node slot,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("attach_slot"), this).signal();
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
     Node retval;
     retval = this.call_procedure_internal(procedure,
                                           arguments);
     return retval;
    }


  // Method for "call_procedure" that defaults arguments to nil
  public Node call_procedure
     (Node procedure) {
     return this.call_procedure(procedure,
                                _NIL);
  }


// ---------------------- enumerate-call-procedure

/**
 * Returns an enumerator for the elements returned by
 *   call-procedure .
 */
  public Enumerator enumerate_call_procedure
     (Node procedure,
      Node arguments) {
     Enumerator retval;
     retval = this.enumerate_call_procedure_internal(procedure,
                                                     arguments);
     return retval;
    }


  // Method for "enumerate_call_procedure" that defaults arguments to nil
  public Enumerator enumerate_call_procedure
     (Node procedure) {
     return this.enumerate_call_procedure(procedure,
                                          _NIL);
  }


// ---------------------- enumerate-call-procedure

/**
 * Returns an enumerator for the elements returned by
 *   call-procedure .
 */
  protected Enumerator enumerate_call_procedure_internal
     (Node procedure,
      Node arguments) {
      Node result_list = call_procedure(procedure,
                               arguments);
      return result_list.enumerate_list();
    }


// ---------------------- class-p

/**
 * Returns   if   thing  identifies a class.
 */
  public Node class_p
     (Node thing,
      TruthValue kb_local_only_p) {
     Node retval;
     retval = this.class_p_internal(thing,
                                    kb_local_only_p);
     return retval;
    }


  // Method for "class_p" that defaults kb-local-only-p to nil
  public Node class_p
     (Node thing) {
     return this.class_p(thing,
                         _NIL);
  }


// ---------------------- class-p

/**
 * Returns   if   thing  identifies a class.
 */
  protected Node class_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("class_p"), this).signal();
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
     kb_has_been_modified_p = true;
     this.close_kb_internal(save_p);
    }


  // Method for "close_kb" that defaults save-p to nil
  public void close_kb
     () {
     this.close_kb(_NIL);
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
  protected void close_kb_internal
     (Node save_p) {
     throw new MethodMissing
               (new OKBCString("close_kb"), this).signal();
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
     Values2 retval;
     retval = this.coerce_to_class_internal(thing,
                                            error_p,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "coerce_to_class" that defaults kb-local-only-p to nil
  public Values2 coerce_to_class
     (Node thing,
      TruthValue error_p) {
     return this.coerce_to_class(thing,
                                 error_p,
                                 _NIL);
  }


  // Method for "coerce_to_class" that defaults error-p to t
  public Values2 coerce_to_class
     (Node thing) {
     return this.coerce_to_class(thing,
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
     Values2 retval;
     retval = this.coerce_to_facet_internal(thing,
                                            error_p,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "coerce_to_facet" that defaults kb-local-only-p to nil
  public Values2 coerce_to_facet
     (Node thing,
      TruthValue error_p) {
     return this.coerce_to_facet(thing,
                                 error_p,
                                 _NIL);
  }


  // Method for "coerce_to_facet" that defaults error-p to t
  public Values2 coerce_to_facet
     (Node thing) {
     return this.coerce_to_facet(thing,
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
     Values2 retval;
     retval = this.coerce_to_frame_internal(thing,
                                            error_p,
                                            kb_local_only_p);
     return retval;
    }


  // Method for "coerce_to_frame" that defaults kb-local-only-p to nil
  public Values2 coerce_to_frame
     (Node thing,
      TruthValue error_p) {
     return this.coerce_to_frame(thing,
                                 error_p,
                                 _NIL);
  }


  // Method for "coerce_to_frame" that defaults error-p to t
  public Values2 coerce_to_frame
     (Node thing) {
     return this.coerce_to_frame(thing,
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
  protected Values2 coerce_to_frame_internal
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("coerce_to_frame"), this).signal();
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
     Values2 retval;
     retval = this.coerce_to_individual_internal(thing,
                                                 error_p,
                                                 kb_local_only_p);
     return retval;
    }


  // Method for "coerce_to_individual" that defaults kb-local-only-p to nil
  public Values2 coerce_to_individual
     (Node thing,
      TruthValue error_p) {
     return this.coerce_to_individual(thing,
                                      error_p,
                                      _NIL);
  }


  // Method for "coerce_to_individual" that defaults error-p to t
  public Values2 coerce_to_individual
     (Node thing) {
     return this.coerce_to_individual(thing,
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
     Values3 retval;
     retval = this.coerce_to_kb_value_internal(string_or_value,
                                               target_context,
                                               wildcards_allowed_p,
                                               force_case_insensitive_p,
                                               error_p,
                                               frame_action,
                                               kb_local_only_p);
     return retval;
    }


  // Method for "coerce_to_kb_value" that defaults kb-local-only-p to nil
  public Values3 coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p,
      Node frame_action) {
     return this.coerce_to_kb_value(string_or_value,
                                    target_context,
                                    wildcards_allowed_p,
                                    force_case_insensitive_p,
                                    error_p,
                                    frame_action,
                                    _NIL);
  }


  // Method for "coerce_to_kb_value" that defaults frame-action to error-if-not-unique
  public Values3 coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p) {
     return this.coerce_to_kb_value(string_or_value,
                                    target_context,
                                    wildcards_allowed_p,
                                    force_case_insensitive_p,
                                    error_p,
                                    _error_if_not_unique);
  }


  // Method for "coerce_to_kb_value" that defaults error-p to t
  public Values3 coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p) {
     return this.coerce_to_kb_value(string_or_value,
                                    target_context,
                                    wildcards_allowed_p,
                                    force_case_insensitive_p,
                                    _T);
  }


  // Method for "coerce_to_kb_value" that defaults force-case-insensitive-p to nil
  public Values3 coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p) {
     return this.coerce_to_kb_value(string_or_value,
                                    target_context,
                                    wildcards_allowed_p,
                                    _NIL);
  }


  // Method for "coerce_to_kb_value" that defaults wildcards-allowed-p to nil
  public Values3 coerce_to_kb_value
     (Node string_or_value,
      ContextName target_context) {
     return this.coerce_to_kb_value(string_or_value,
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
     Values2 retval;
     retval = this.coerce_to_slot_internal(thing,
                                           error_p,
                                           kb_local_only_p);
     return retval;
    }


  // Method for "coerce_to_slot" that defaults kb-local-only-p to nil
  public Values2 coerce_to_slot
     (Node thing,
      TruthValue error_p) {
     return this.coerce_to_slot(thing,
                                error_p,
                                _NIL);
  }


  // Method for "coerce_to_slot" that defaults error-p to t
  public Values2 coerce_to_slot
     (Node thing) {
     return this.coerce_to_slot(thing,
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
     Node retval;
     retval = this.coercible_to_frame_p_internal(thing,
                                                 kb_local_only_p);
     return retval;
    }


  // Method for "coercible_to_frame_p" that defaults kb-local-only-p to nil
  public Node coercible_to_frame_p
     (Node thing) {
     return this.coercible_to_frame_p(thing,
                                      _NIL);
  }


// ---------------------- copy-frame

/**
 * Copies   frame  from   kb  to   to-kb .  The name of the new
 * frame in   to-kb  will be   new-name .    Kb  and
 *   to-kb  may be the same KB.  If the    :frame-names-required 
 * behavior has the value   for   kb ,   new-name  may be
 *  .
 * 
 * If   error-p  is  , catches errors that occur, and
 * continues copying to the extent possible.
 * 
 * The   frame  may contain references to other frames that do not
 * reside in   to-kb  -- for example,
 * its types, superclasses, or slot values.
 *   Missing-frame-action  controls the behavior of   copy-frame 
 * in this case.  It can take one of the following values:
 * 
 *    :stop  -- Stop copying and signal a   frames-missing  error,
 * depending on the value of   error-p .
 * 
 *    :abort  -- Abort copying   frame , leaving the state of
 *   to-kb  unchanged.  Any side effects of
 *   copy-frame  that have already been performed will
 * be undone.  Signals a   frames-missing  error,
 * depending on the value of   error-p .
 * 
 *    :allocate  -- Allocate frame handles for any frame references that
 * do not yet exist in   to-kb .
 * 
 *    :ignore  - Continue with the copying of the current frame, but
 * ignore and remove any references to missing frames.
 * 
 *   Frame-handle-mapping-table , if supplied, is a hash table that maps
 * the frame handles in the   kb  to frame handles in   to-kb , and
 * is used during compound copy operations, such as   copy-kb .  If
 * copy-frame fails to find a referenced frame in   to-kb , it looks up
 * the reference in the   Frame-handle-mapping-table  before allocating
 * a new frame handle.
 * 
 * It returns two values.
 *   
 *     Copy-of-frame  -- Identifies the newly created frame in
 *   to-kb .  If   copy-frame  terminates because some frames were
 * missing, and   missing-frame-action  was    :abort ,
 *   is returned as a value of   copy-of-frame .
 *     Allocated-frame-handle-alist  -- a list of 2-tuples    
 * (frame-handle-in-kb frame-handle-in-to-kb)  that maps frame handles in
 *   kb  to frame handles in   to-kb  that were allocated during
 * the copying process.  These mappings will also have been entered in
 *   frame-handle-mapping-table  if it was supplied.
 */
  static public Values2 copy_frame
     (Node frame,
      Node new_name,
      Kb kb,
      Kb to_kb,
      TruthValue error_p,
      Node missing_frame_action,
      Node frame_handle_mapping_table,
      TruthValue kb_local_only_p) {
     Values2 retval;
     retval = copy_frame_internal(frame,
                                  new_name,
                                  kb,
                                  to_kb,
                                  error_p,
                                  missing_frame_action,
                                  frame_handle_mapping_table,
                                  kb_local_only_p);
     return retval;
    }


  // Method for "copy_frame" that defaults kb-local-only-p to nil
  static public Values2 copy_frame
     (Node frame,
      Node new_name,
      Kb kb,
      Kb to_kb,
      TruthValue error_p,
      Node missing_frame_action,
      Node frame_handle_mapping_table) {
     return copy_frame(frame,
                       new_name,
                       kb,
                       to_kb,
                       error_p,
                       missing_frame_action,
                       frame_handle_mapping_table,
                       _NIL);
  }


  // Method for "copy_frame" that defaults frame-handle-mapping-table to nil
  static public Values2 copy_frame
     (Node frame,
      Node new_name,
      Kb kb,
      Kb to_kb,
      TruthValue error_p,
      Node missing_frame_action) {
     return copy_frame(frame,
                       new_name,
                       kb,
                       to_kb,
                       error_p,
                       missing_frame_action,
                       _NIL);
  }


  // Method for "copy_frame" that defaults missing-frame-action to stop
  static public Values2 copy_frame
     (Node frame,
      Node new_name,
      Kb kb,
      Kb to_kb,
      TruthValue error_p) {
     return copy_frame(frame,
                       new_name,
                       kb,
                       to_kb,
                       error_p,
                       _stop);
  }


  // Method for "copy_frame" that defaults error-p to t
  static public Values2 copy_frame
     (Node frame,
      Node new_name,
      Kb kb,
      Kb to_kb) {
     return copy_frame(frame,
                       new_name,
                       kb,
                       to_kb,
                       _T);
  }


// ---------------------- copy-kb

/**
 * Copies the frames in   from-kb  into   to-kb .
 * The interpretation of   Missing-frame-action  is the same as
 * for   copy-frame .  If   error-p  is  , catches errors that
 * occur, and attempts to continue with copying.  Returns no values.
 * 
 * Note that the behavior    are-frames  might have different values for
 * the two KBs.  Thus, if slots are represented as frames in
 *   kb , but are not represented as frames in   to-kb , the frames
 * representing slots in   kb  will not be copied.
 */
  static public void copy_kb
     (Kb from_kb,
      Kb to_kb,
      TruthValue error_p,
      Node missing_frame_action,
      TruthValue kb_local_only_p) {
     copy_kb_internal(from_kb,
                      to_kb,
                      error_p,
                      missing_frame_action,
                      kb_local_only_p);
    }


  // Method for "copy_kb" that defaults kb-local-only-p to nil
  static public void copy_kb
     (Kb from_kb,
      Kb to_kb,
      TruthValue error_p,
      Node missing_frame_action) {
     copy_kb(from_kb,
             to_kb,
             error_p,
             missing_frame_action,
             _NIL);
  }


  // Method for "copy_kb" that defaults missing-frame-action to stop
  static public void copy_kb
     (Kb from_kb,
      Kb to_kb,
      TruthValue error_p) {
     copy_kb(from_kb,
             to_kb,
             error_p,
             _stop);
  }


  // Method for "copy_kb" that defaults error-p to t
  static public void copy_kb
     (Kb from_kb,
      Kb to_kb) {
     copy_kb(from_kb,
             to_kb,
             _T);
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.create_class_internal(name,
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
     return retval;
    }


  // Method for "create_class" that defaults kb-local-only-p to nil
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
      Node pretty_name) {
     return this.create_class(name,
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
  protected Node create_class_internal
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
     throw new MethodMissing
               (new OKBCString("create_class"), this).signal();
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.create_facet_internal(name,
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
     return retval;
    }


  // Method for "create_facet" that defaults kb-local-only-p to nil
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
      Node pretty_name) {
     return this.create_facet(name,
                              frame_or_nil,
                              slot_or_nil,
                              slot_type,
                              direct_types,
                              doc,
                              own_slots,
                              own_facets,
                              handle,
                              pretty_name,
                              _NIL);
  }


  // Method for "create_facet" that defaults pretty-name to nil
  public Node create_facet
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle) {
     return this.create_facet(name,
                              frame_or_nil,
                              slot_or_nil,
                              slot_type,
                              direct_types,
                              doc,
                              own_slots,
                              own_facets,
                              handle,
                              _NIL);
  }


  // Method for "create_facet" that defaults handle to nil
  public Node create_facet
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets) {
     return this.create_facet(name,
                              frame_or_nil,
                              slot_or_nil,
                              slot_type,
                              direct_types,
                              doc,
                              own_slots,
                              own_facets,
                              _NIL);
  }


  // Method for "create_facet" that defaults own-facets to nil
  public Node create_facet
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots) {
     return this.create_facet(name,
                              frame_or_nil,
                              slot_or_nil,
                              slot_type,
                              direct_types,
                              doc,
                              own_slots,
                              _NIL);
  }


  // Method for "create_facet" that defaults own-slots to nil
  public Node create_facet
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc) {
     return this.create_facet(name,
                              frame_or_nil,
                              slot_or_nil,
                              slot_type,
                              direct_types,
                              doc,
                              _NIL);
  }


  // Method for "create_facet" that defaults doc to nil
  public Node create_facet
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil,
      SlotType slot_type,
      Node direct_types) {
     return this.create_facet(name,
                              frame_or_nil,
                              slot_or_nil,
                              slot_type,
                              direct_types,
                              _NIL);
  }


  // Method for "create_facet" that defaults direct-types to nil
  public Node create_facet
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil,
      SlotType slot_type) {
     return this.create_facet(name,
                              frame_or_nil,
                              slot_or_nil,
                              slot_type,
                              _NIL);
  }


  // Method for "create_facet" that defaults slot-type to own
  public Node create_facet
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil) {
     return this.create_facet(name,
                              frame_or_nil,
                              slot_or_nil,
                              _own);
  }


  // Method for "create_facet" that defaults slot-or-nil to nil
  public Node create_facet
     (Node name,
      Node frame_or_nil) {
     return this.create_facet(name,
                              frame_or_nil,
                              _NIL);
  }


  // Method for "create_facet" that defaults frame-or-nil to nil
  public Node create_facet
     (Node name) {
     return this.create_facet(name,
                              _NIL);
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
  protected Node create_facet_internal
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
     throw new MethodMissing
               (new OKBCString("create_facet"), this).signal();
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.create_frame_internal(name,
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
     return retval;
    }


  // Method for "create_frame" that defaults kb-local-only-p to nil
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
      Node pretty_name) {
     return this.create_frame(name,
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
                              _NIL);
  }


  // Method for "create_frame" that defaults pretty-name to nil
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
      Node handle) {
     return this.create_frame(name,
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
                              _NIL);
  }


  // Method for "create_frame" that defaults handle to nil
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
      Node primitive_p) {
     return this.create_frame(name,
                              frame_type,
                              direct_types,
                              direct_superclasses,
                              doc,
                              template_slots,
                              template_facets,
                              own_slots,
                              own_facets,
                              primitive_p,
                              _NIL);
  }


  // Method for "create_frame" that defaults primitive-p to t
  public Node create_frame
     (Node name,
      ContextName frame_type,
      Node direct_types,
      Node direct_superclasses,
      Node doc,
      Node template_slots,
      Node template_facets,
      Node own_slots,
      Node own_facets) {
     return this.create_frame(name,
                              frame_type,
                              direct_types,
                              direct_superclasses,
                              doc,
                              template_slots,
                              template_facets,
                              own_slots,
                              own_facets,
                              _T);
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.create_individual_internal(name,
                                              direct_types,
                                              doc,
                                              own_slots,
                                              own_facets,
                                              handle,
                                              pretty_name,
                                              kb_local_only_p);
     return retval;
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
  protected Node create_individual_internal
     (Node name,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("create_individual"), this).signal();
    }


// ---------------------- create-kb-locator

/**
 * Returns a new   kb-locator  associated with   thing  for a kb of
 * type   kb-type .  If   thing  is a KB, the kb-locator created is
 * associated with that KB in the   meta-kb .  It is an error for
 *   thing  to be an incomplete description of a kb-locator.
 * 
 *   Thing  is a   kb-type  and   connection  specific
 * specification of a KB location sufficient to create and fully
 * initialize a KB locator.
 * 
 * For example,   thing  may identify the pathname for a KB that
 * resides in a disk file.  Each back-end implementation must provide
 * documentation for all values of   thing  that the   kb-type 
 * and   connection  will accept other than KBs, which are always
 * accepted.
 * 
 * Implementation note: Back end implementators may use any legal
 * OKBC value entity for the   thing  argument as long as it
 * consists only of the primitive data types: integer, float, string,
 * symbol, true, false, or list.  Values of   thing  of these data
 * types will always be transmitted by networked implementations without
 * substitution of remote references.  For example, the following
 * could be a legal value for for the   thing  argument for some
 *   kb-type 
 *   
 * (:db-file "/projects/foo/my-database.data" :db-type :oracle :name my-kb)
 */
  public Node create_kb_locator
     (Node thing,
      Connection connection) {
     Node retval;
     retval = this.create_kb_locator_internal(thing,
                                              connection);
     return retval;
    }


// ---------------------- create-kb-locator

/**
 * Returns a new   kb-locator  associated with   thing  for a kb of
 * type   kb-type .  If   thing  is a KB, the kb-locator created is
 * associated with that KB in the   meta-kb .  It is an error for
 *   thing  to be an incomplete description of a kb-locator.
 * 
 *   Thing  is a   kb-type  and   connection  specific
 * specification of a KB location sufficient to create and fully
 * initialize a KB locator.
 * 
 * For example,   thing  may identify the pathname for a KB that
 * resides in a disk file.  Each back-end implementation must provide
 * documentation for all values of   thing  that the   kb-type 
 * and   connection  will accept other than KBs, which are always
 * accepted.
 * 
 * Implementation note: Back end implementators may use any legal
 * OKBC value entity for the   thing  argument as long as it
 * consists only of the primitive data types: integer, float, string,
 * symbol, true, false, or list.  Values of   thing  of these data
 * types will always be transmitted by networked implementations without
 * substitution of remote references.  For example, the following
 * could be a legal value for for the   thing  argument for some
 *   kb-type 
 *   
 * (:db-file "/projects/foo/my-database.data" :db-type :oracle :name my-kb)
 */
  protected Node create_kb_locator_internal
     (Node thing,
      Connection connection) {
     throw new MethodMissing
               (new OKBCString("create_kb_locator"), this).signal();
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
     Node retval;
     retval = this.create_procedure_internal(arguments,
                                             body,
                                             environment);
     return retval;
    }


  // Method for "create_procedure" that defaults environment to nil
  public Node create_procedure
     (Node arguments,
      Node body) {
     return this.create_procedure(arguments,
                                  body,
                                  _NIL);
  }


  // Method for "create_procedure" that defaults body to nil
  public Node create_procedure
     (Node arguments) {
     return this.create_procedure(arguments,
                                  _NIL);
  }


  // Method for "create_procedure" that defaults arguments to nil
  public Node create_procedure
     () {
     return this.create_procedure(_NIL);
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
     kb_has_been_modified_p = true;
     Node retval;
     retval = this.create_slot_internal(name,
                                        frame_or_nil,
                                        slot_type,
                                        direct_types,
                                        doc,
                                        own_slots,
                                        own_facets,
                                        handle,
                                        pretty_name,
                                        kb_local_only_p);
     return retval;
    }


  // Method for "create_slot" that defaults kb-local-only-p to nil
  public Node create_slot
     (Node name,
      Node frame_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name) {
     return this.create_slot(name,
                             frame_or_nil,
                             slot_type,
                             direct_types,
                             doc,
                             own_slots,
                             own_facets,
                             handle,
                             pretty_name,
                             _NIL);
  }


  // Method for "create_slot" that defaults pretty-name to nil
  public Node create_slot
     (Node name,
      Node frame_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle) {
     return this.create_slot(name,
                             frame_or_nil,
                             slot_type,
                             direct_types,
                             doc,
                             own_slots,
                             own_facets,
                             handle,
                             _NIL);
  }


  // Method for "create_slot" that defaults handle to nil
  public Node create_slot
     (Node name,
      Node frame_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets) {
     return this.create_slot(name,
                             frame_or_nil,
                             slot_type,
                             direct_types,
                             doc,
                             own_slots,
                             own_facets,
                             _NIL);
  }


  // Method for "create_slot" that defaults own-facets to nil
  public Node create_slot
     (Node name,
      Node frame_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots) {
     return this.create_slot(name,
                             frame_or_nil,
                             slot_type,
                             direct_types,
                             doc,
                             own_slots,
                             _NIL);
  }


  // Method for "create_slot" that defaults own-slots to nil
  public Node create_slot
     (Node name,
      Node frame_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc) {
     return this.create_slot(name,
                             frame_or_nil,
                             slot_type,
                             direct_types,
                             doc,
                             _NIL);
  }


  // Method for "create_slot" that defaults doc to nil
  public Node create_slot
     (Node name,
      Node frame_or_nil,
      SlotType slot_type,
      Node direct_types) {
     return this.create_slot(name,
                             frame_or_nil,
                             slot_type,
                             direct_types,
                             _NIL);
  }


  // Method for "create_slot" that defaults direct-types to nil
  public Node create_slot
     (Node name,
      Node frame_or_nil,
      SlotType slot_type) {
     return this.create_slot(name,
                             frame_or_nil,
                             slot_type,
                             _NIL);
  }


  // Method for "create_slot" that defaults slot-type to all
  public Node create_slot
     (Node name,
      Node frame_or_nil) {
     return this.create_slot(name,
                             frame_or_nil,
                             _all);
  }


  // Method for "create_slot" that defaults frame-or-nil to nil
  public Node create_slot
     (Node name) {
     return this.create_slot(name,
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
  protected Node create_slot_internal
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
     throw new MethodMissing
               (new OKBCString("create_slot"), this).signal();
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
     Node retval;
     retval = this.decontextualize_internal(value,
                                            from_context);
     return retval;
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
  protected Node decontextualize_internal
     (Node value,
      ContextName from_context) {
     throw new MethodMissing
               (new OKBCString("decontextualize"), this).signal();
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
     kb_has_been_modified_p = true;
     this.delete_facet_internal(facet,
                                kb_local_only_p);
    }


  // Method for "delete_facet" that defaults kb-local-only-p to nil
  public void delete_facet
     (Node facet) {
     this.delete_facet(facet,
                       _NIL);
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
  protected void delete_facet_internal
     (Node facet,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("delete_facet"), this).signal();
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
     kb_has_been_modified_p = true;
     this.delete_frame_internal(frame,
                                kb_local_only_p);
    }


  // Method for "delete_frame" that defaults kb-local-only-p to nil
  public void delete_frame
     (Node frame) {
     this.delete_frame(frame,
                       _NIL);
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
  protected void delete_frame_internal
     (Node frame,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("delete_frame"), this).signal();
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
     kb_has_been_modified_p = true;
     this.delete_slot_internal(slot,
                               kb_local_only_p);
    }


  // Method for "delete_slot" that defaults kb-local-only-p to nil
  public void delete_slot
     (Node slot) {
     this.delete_slot(slot,
                      _NIL);
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
  protected void delete_slot_internal
     (Node slot,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("delete_slot"), this).signal();
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
     kb_has_been_modified_p = true;
     this.detach_facet_internal(frame,
                                slot,
                                facet,
                                slot_type,
                                kb_local_only_p);
    }


  // Method for "detach_facet" that defaults kb-local-only-p to nil
  public void detach_facet
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type) {
     this.detach_facet(frame,
                       slot,
                       facet,
                       slot_type,
                       _NIL);
  }


  // Method for "detach_facet" that defaults slot-type to own
  public void detach_facet
     (Node frame,
      Node slot,
      Node facet) {
     this.detach_facet(frame,
                       slot,
                       facet,
                       _own);
  }


// ---------------------- detach-facet

/**
 * Removes any explicit association between the   facet  and
 *   slot  on   frame .  As a result,   facet  is not returned
 * by   get-slot-facets  at inference-level    :direct  unless there are
 * facet values associated with   facet  in   slot  on
 *   frame .
 */
  protected void detach_facet_internal
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("detach_facet"), this).signal();
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
     kb_has_been_modified_p = true;
     this.detach_slot_internal(frame,
                               slot,
                               slot_type,
                               kb_local_only_p);
    }


  // Method for "detach_slot" that defaults kb-local-only-p to nil
  public void detach_slot
     (Node frame,
      Node slot,
      SlotType slot_type) {
     this.detach_slot(frame,
                      slot,
                      slot_type,
                      _NIL);
  }


  // Method for "detach_slot" that defaults slot-type to own
  public void detach_slot
     (Node frame,
      Node slot) {
     this.detach_slot(frame,
                      slot,
                      _own);
  }


// ---------------------- detach-slot

/**
 * Removes any explicit association between the   slot  and
 *   frame .  As a result,   slot  is not returned by
 *   get-frame-slots  at inference-level    :direct  unless there are
 * slot or facet values associated with   slot  in   frame .
 */
  protected void detach_slot_internal
     (Node frame,
      Node slot,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("detach_slot"), this).signal();
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
     Node retval;
     retval = this.eql_in_kb_internal(arg0,
                                      arg1,
                                      kb_local_only_p);
     return retval;
    }


  // Method for "eql_in_kb" that defaults kb-local-only-p to nil
  public Node eql_in_kb
     (Node arg0,
      Node arg1) {
     return this.eql_in_kb(arg0,
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
     Node retval;
     retval = this.equal_in_kb_internal(arg0,
                                        arg1,
                                        kb_local_only_p);
     return retval;
    }


  // Method for "equal_in_kb" that defaults kb-local-only-p to nil
  public Node equal_in_kb
     (Node arg0,
      Node arg1) {
     return this.equal_in_kb(arg0,
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
     Node retval;
     retval = this.equalp_in_kb_internal(arg0,
                                         arg1,
                                         kb_local_only_p);
     return retval;
    }


  // Method for "equalp_in_kb" that defaults kb-local-only-p to nil
  public Node equalp_in_kb
     (Node arg0,
      Node arg1) {
     return this.equalp_in_kb(arg0,
                              arg1,
                              _NIL);
  }


// ---------------------- expunge-kb

/**
 * Given a   kb-locator , removes the KB identified by that locator
 * and any backup copies of it from permanent storage.  Returns no values.
 * Any currently open KB identified by the locator will be unaffected,
 * and may be saved to other locations using   save-kb-as .  If
 *   error-p  is  ,   expunge-kb  catches errors that
 * occur, and attempts to continue with the deletion process.
 */
  public void expunge_kb
     (Node kb_locator,
      Connection connection,
      TruthValue error_p) {
     kb_has_been_modified_p = true;
     this.expunge_kb_internal(kb_locator,
                              connection,
                              error_p);
    }


  // Method for "expunge_kb" that defaults error-p to t
  public void expunge_kb
     (Node kb_locator,
      Connection connection) {
     this.expunge_kb(kb_locator,
                     connection,
                     _T);
  }


// ---------------------- expunge-kb

/**
 * Given a   kb-locator , removes the KB identified by that locator
 * and any backup copies of it from permanent storage.  Returns no values.
 * Any currently open KB identified by the locator will be unaffected,
 * and may be saved to other locations using   save-kb-as .  If
 *   error-p  is  ,   expunge-kb  catches errors that
 * occur, and attempts to continue with the deletion process.
 */
  protected void expunge_kb_internal
     (Node kb_locator,
      Connection connection,
      TruthValue error_p) {
     throw new MethodMissing
               (new OKBCString("expunge_kb"), this).signal();
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
     Values2 retval;
     retval = this.facet_has_value_p_internal(frame,
                                              slot,
                                              facet,
                                              inference_level,
                                              slot_type,
                                              value_selector,
                                              kb_local_only_p);
     return retval;
    }


  // Method for "facet_has_value_p" that defaults kb-local-only-p to nil
  public Values2 facet_has_value_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector) {
     return this.facet_has_value_p(frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   slot_type,
                                   value_selector,
                                   _NIL);
  }


  // Method for "facet_has_value_p" that defaults value-selector to either
  public Values2 facet_has_value_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.facet_has_value_p(frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   slot_type,
                                   _either);
  }


  // Method for "facet_has_value_p" that defaults slot-type to own
  public Values2 facet_has_value_p
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level) {
     return this.facet_has_value_p(frame,
                                   slot,
                                   facet,
                                   inference_level,
                                   _own);
  }


  // Method for "facet_has_value_p" that defaults inference-level to taxonomic
  public Values2 facet_has_value_p
     (Node frame,
      Node slot,
      Node facet) {
     return this.facet_has_value_p(frame,
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
     Node retval;
     retval = this.facet_p_internal(thing,
                                    kb_local_only_p);
     return retval;
    }


  // Method for "facet_p" that defaults kb-local-only-p to nil
  public Node facet_p
     (Node thing) {
     return this.facet_p(thing,
                         _NIL);
  }


// ---------------------- facet-p

/**
 * Returns   iff   thing  is a facet, and   otherwise.
 */
  protected Node facet_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     throw new MethodMissing
               (new OKBCString("facet_p"), this).signal();
    }


// ---------------------- find-kb-locator

/**
 * Returns the   kb-locator  associated with   thing  if such
 * a kb-locator exists for a KB of type   kb-type , and   
 * otherwise.
 * 
 * Always returns a kb-locator if   thing  is a KB.  Implementations
 * are encouraged to accept other values for   thing  such as a pathname
 * that identifies the location of the KB to the system.  Such usage is
 * convenient, but is not portable.  It is not portable for an OKBC application
 * to use anything other than a KB locator, or a KB for this argument.
 */
  public Node find_kb_locator
     (Node thing,
      Connection connection) {
     Node retval;
     retval = this.find_kb_locator_internal(thing,
                                            connection);
     return retval;
    }


// ---------------------- find-kb-locator

/**
 * Returns the   kb-locator  associated with   thing  if such
 * a kb-locator exists for a KB of type   kb-type , and   
 * otherwise.
 * 
 * Always returns a kb-locator if   thing  is a KB.  Implementations
 * are encouraged to accept other values for   thing  such as a pathname
 * that identifies the location of the KB to the system.  Such usage is
 * convenient, but is not portable.  It is not portable for an OKBC application
 * to use anything other than a KB locator, or a KB for this argument.
 */
  protected Node find_kb_locator_internal
     (Node thing,
      Connection connection) {
     throw new MethodMissing
               (new OKBCString("find_kb_locator"), this).signal();
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
     Node retval;
     retval = this.follow_slot_chain_internal(frame,
                                              slot_chain,
                                              union_multiple_values,
                                              inference_level,
                                              value_selector,
                                              kb_local_only_p);
     return retval;
    }


  // Method for "follow_slot_chain" that defaults kb-local-only-p to nil
  public Node follow_slot_chain
     (Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level,
      ValueSelector value_selector) {
     return this.follow_slot_chain(frame,
                                   slot_chain,
                                   union_multiple_values,
                                   inference_level,
                                   value_selector,
                                   _NIL);
  }


  // Method for "follow_slot_chain" that defaults value-selector to either
  public Node follow_slot_chain
     (Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level) {
     return this.follow_slot_chain(frame,
                                   slot_chain,
                                   union_multiple_values,
                                   inference_level,
                                   _either);
  }


  // Method for "follow_slot_chain" that defaults inference-level to taxonomic
  public Node follow_slot_chain
     (Node frame,
      Node slot_chain,
      Node union_multiple_values) {
     return this.follow_slot_chain(frame,
                                   slot_chain,
                                   union_multiple_values,
                                   _taxonomic);
  }


  // Method for "follow_slot_chain" that defaults union-multiple-values to t
  public Node follow_slot_chain
     (Node frame,
      Node slot_chain) {
     return this.follow_slot_chain(frame,
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
     Node retval;
     retval = this.frame_has_slot_p_internal(frame,
                                             slot,
                                             inference_level,
                                             slot_type,
                                             kb_local_only_p);
     return retval;
    }


  // Method for "frame_has_slot_p" that defaults kb-local-only-p to nil
  public Node frame_has_slot_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type) {
     return this.frame_has_slot_p(frame,
                                  slot,
                                  inference_level,
                                  slot_type,
                                  _NIL);
  }


  // Method for "frame_has_slot_p" that defaults slot-type to own
  public Node frame_has_slot_p
     (Node frame,
      Node slot,
      InferenceLevel inference_level) {
     return this.frame_has_slot_p(frame,
                                  slot,
                                  inference_level,
                                  _own);
  }


  // Method for "frame_has_slot_p" that defaults inference-level to taxonomic
  public Node frame_has_slot_p
     (Node frame,
      Node slot) {
     return this.frame_has_slot_p(frame,
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
     Node retval;
     retval = this.frame_in_kb_p_internal(thing,
                                          kb_local_only_p);
     return retval;
    }


  // Method for "frame_in_kb_p" that defaults kb-local-only-p to nil
  public Node frame_in_kb_p
     (Node thing) {
     return this.frame_in_kb_p(thing,
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
     Node retval;
     retval = this.frs_independent_frame_handle_internal(frame,
                                                         kb_local_only_p);
     return retval;
    }


  // Method for "frs_independent_frame_handle" that defaults kb-local-only-p to nil
  public Node frs_independent_frame_handle
     (Node frame) {
     return this.frs_independent_frame_handle(frame,
                                              _NIL);
  }


// ---------------------- frs-name

/**
 * Returns the   krs-name  of the underlying KRS associated with the
 *   kb-type , which is accessed over   connection .
 *   Krs-name  is a string.  For example,
 * given    loom-kb  as the kb-type, it might return the string
 *    "LOOM" .  This operation is used by user interfaces that need to
 * display a printed representation of the underlying KRS for a particular
 * kb-type.
 * 
 * Note:  This operation is named   frs-name  for
 * historical reasons.  Frame Representation Systems are now uniformly
 * called Knowledge Representation Systems with the exception of in the names
 * of this operator and   frs-independent-frame-handle .
 */
  static public Node frs_name
     (Kb kb_type,
      Connection connection) {
     Node retval;
     retval = frs_name_internal(kb_type,
                                connection);
     return retval;
    }


}

