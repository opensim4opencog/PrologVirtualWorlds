/***********************************************************************

  OKBC Implemementation for Java

  Copyright (c) 1996, 1997 Stanford University KSL.

  Original code by James Rice (rice@ksl.stanford.edu)


  This code was automatically generated on Tuesday, 14 October 1997.

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

public class StandardDefaultsKb extends Kb {


// ---------- default implementation of add-class-superclass


/**
 * Adds the   new-superclass  to the superclasses of   class .
 * Returns no values.
 */
  protected void add_class_superclass_internal
     (Node the_class, Node new_superclass, TruthValue kb_local_only_p) {
       Node values = get_class_superclasses(the_class, _direct, _all,
					    kb_local_only_p).firstValue();
       // Node new_values = values;
       if (!(Cons.member_test(new_superclass, values, _eql, this,
			      kb_local_only_p)))
	 { Node new_values = new Cons(new_superclass, values);
       put_class_superclasses(the_class, new_values, kb_local_only_p); }
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
      TruthValue kb_local_only_p) {
       Node values = get_facet_values(frame, slot, facet, _direct, slot_type,
				      _all, value_selector,
				      kb_local_only_p).firstValue();
       // Node new_values = values;
       if (!(Cons.member_test(value, values, test, this, kb_local_only_p)))
	 { Node new_values = new Cons(value, values);
       put_facet_values(frame, slot, facet, new_values,
		   slot_type, value_selector, kb_local_only_p); }
    }


// ---------- default implementation of add-instance-type


/**
 * Adds the   new-type  to the types of   frame .
 * Returns no values.
 */
  protected void add_instance_type_internal
     (Node frame, Node new_type, TruthValue kb_local_only_p) {
       Node values = get_instance_types(frame, _direct, _all,
					kb_local_only_p).firstValue();
       // Node new_values = values;
       if (!(Cons.member_test(new_type, values, _eql, this,
			      kb_local_only_p)))
	 { Node new_values = new Cons(new_type, values);
       put_instance_types(frame, new_values, kb_local_only_p); }
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
      TruthValue kb_local_only_p) {
       long after = (add_before instanceof OKBCInteger
		    ? ((OKBCInteger)add_before).number 
		    : 0);
       Node values = get_slot_values(frame, slot, _all_inferable, slot_type, 
				     _all, value_selector, 
				     kb_local_only_p).firstValue();
       Node collection_type = 
	 get_collection_type(frame, slot, slot_type, kb_local_only_p);
       if (collection_type == _none)
	 { put_slot_value(frame, slot, value, slot_type, value_selector,
			  kb_local_only_p); }
       else if ((collection_type == _set) && 
		   Cons.member_test(value, values, test, this,
				    kb_local_only_p)) 
	 { }
       else { Node new_values = 
		Cons.append_cons(Cons.firstn((int)after, values),
				 new Cons(value,
					  Cons.nthcdr((int)after, values)));
	      put_slot_values(frame, slot, new_values, slot_type,
			      value_selector, kb_local_only_p);
	    }
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
  { return TellAndAsk.default_ask(query, this, reply_pattern, inference_level,
				  number_of_values, error_p, where, timeout,
				  value_selector, kb_local_only_p);
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
  { return TellAndAsk.default_askable(sentence, this, value_selector,
				      kb_local_only_p);
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
      Node handle, Node pretty_name, TruthValue kb_local_only_p) {
       if (!(name instanceof OKBCString || name instanceof OKBCInteger ||
	     name instanceof AbstractSymbol))
	 { throw new GenericError
		(_T, new OKBCString("A frame name must be a OKBCString, " +
				    "Symbol or OKBCInteger, not " +
				    name)).signal();
	 }
       if ((own_slots != _NIL || own_facets != _NIL) &&
	   !(Cons.member_eql(frame_type, get_behavior_values(_are_frames),
			     this, kb_local_only_p)))
	 { throw new NotAFrameType(frame_type, this).signal(); }
       Node frame = null;
       if (frame_type == _class)
	 { frame = create_class
	     (name, direct_types, Cons.list_if_not(direct_superclasses),
	      primitive_p, doc, template_slots, template_facets,
	      own_slots, own_facets, handle, pretty_name, kb_local_only_p);
	 }
       else if (frame_type == _individual)
	 { frame = create_individual(name, direct_types, doc, own_slots,
				     own_facets, handle, pretty_name,
				     kb_local_only_p);
	 }
       else if (frame_type == _slot)
	 { frame = create_slot(name, _NIL, _all, direct_types, doc, 
			       own_slots, own_facets, handle, pretty_name,
			       kb_local_only_p);
	 }
       else if (frame_type == _facet)
	 { frame = create_facet(name, _NIL, _NIL, _all, direct_types, doc,
				own_slots, own_facets, handle, pretty_name,
				kb_local_only_p);
	 }
       else { throw new NotAFrameType(frame_type, this).signal(); }
       return frame;
    }

// ---------- default implementation of facet-has-value-p


/**
 * Returns   iff the specified facet has a value for the specified slot
 * and frame, and otherwise returns  .
 */
  protected Values2 facet_has_value_p_internal
     (Node frame, Node slot, Node facet, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Values3 results = get_facet_values
       (frame, slot, facet, inference_level, slot_type, new OKBCInteger(1), 
	value_selector, kb_local_only_p);
     if (results.firstValue() == _NIL)
       { return new Values2(_NIL, results.secondValue()); }
     else { return new Values2(_T, results.secondValue()); }
    }

// ---------- default implementation of frame-has-slot-p


/**
 * Returns   iff   slot  is a slot in   frame ,
 * otherwise returns  .
 */
  protected Node frame_has_slot_p_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, TruthValue kb_local_only_p) {
       return frame_has_slot_or_facet_p (frame, slot, null, inference_level,
					 slot_type, kb_local_only_p, false);
    }


  protected Node direct_frame_has_slot_or_facet_p
     (Node frame, Node slot, Node facet, SlotType slot_type,
      TruthValue kb_local_only_p, boolean facet_op_p) {
       if (facet_op_p)
	 { if (Cons.member_test(facet, 
				get_slot_facets(frame, slot, _direct,
						slot_type, 
						kb_local_only_p).firstValue(),
				_eql, this, kb_local_only_p))
	     { return _T; }
	   else { return _NIL; }
	 }
       else
	 { if (Cons.member_test(slot, 
				get_frame_slots(frame, _direct, slot_type, 
						kb_local_only_p).firstValue(),
				_eql, this, kb_local_only_p))
	     { return _T; }
	   else { return _NIL; }
	 }
     }

  Node frame_has_slot_or_facet_p
     (Node frame, Node slot, Node facet, InferenceLevel inference_level,
      SlotType slot_type, TruthValue kb_local_only_p, boolean facet_op_p) {
       if (inference_level == _direct)
	 { if (facet_op_p)
	   { throw new MethodMissing(new OKBCString("slot_has_facet_p"),
				     this).signal();
           }
	   else
	   { throw new MethodMissing(new OKBCString("frame_has_slot_p"),
				     this).signal();
           }
	 }
       frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
       Node real_slot = coerce_to_slot(slot, _NIL, 
				       kb_local_only_p).firstValue();
       if (real_slot != _NIL) { slot = real_slot; }

       if (facet_op_p)
	 { Node real_facet = coerce_to_facet(facet, _NIL, 
					     kb_local_only_p).firstValue();
	   if (real_facet != _NIL) { facet = real_facet; }
	 }

       Node predicate_value = direct_frame_has_slot_or_facet_p
	 (frame, slot, facet, slot_type, kb_local_only_p, facet_op_p);
       if (predicate_value != _NIL) { return _T; }
       Node class_p = class_p(frame, kb_local_only_p);
       if (class_p != _NIL && 
	   (slot_type == _template || slot_type == _all))
	 { Node superclasses = get_class_superclasses
	     (frame, inference_level, _all, kb_local_only_p).firstValue();
	   Enumeration e = superclasses.elements();
	   while (e.hasMoreElements()) {
	   Node cl = (Node)(e.nextElement());
	   if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
	     { Node val = direct_frame_has_slot_or_facet_p
		 (cl, slot, facet, slot_type,
		  kb_local_only_p, facet_op_p);
	     if (val
		 != _NIL)
	       { predicate_value = _T; 
	         break; 
	       }
	     }
	   }
	 }
       else
	 { if (slot_type == _own || slot_type == _all)
	     { Node types = get_instance_types
		 (frame, inference_level, _all, kb_local_only_p).firstValue();
	       Enumeration e = types.elements();
	       while (e.hasMoreElements()) {
		 Node cl = (Node)(e.nextElement());
		 if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
		   { if (direct_frame_has_slot_or_facet_p
			 (cl, slot, facet, _template,
			  kb_local_only_p, facet_op_p)
			 != _NIL)
		       { predicate_value = _T; 
			 break; 
		       }
		   }
	       }
	     }
	 }
       return predicate_value;
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
      TruthValue kb_local_only_p) {
     Values3 $results = get_facet_values
       (frame, slot, facet, inference_level, slot_type, new OKBCInteger(2),
	value_selector, kb_local_only_p);
     Node list = $results.firstValue();
     if (list.length() >= 2)
       { throw new CardinalityViolation
	 (new OKBCString("Single valued facet"), frame, slot, slot_type, facet,
	  this).signal();
       }
     return new Values2(list.car(), $results.secondValue());
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
	  ValueSelector value_selector, TruthValue kb_local_only_p) {
     Values4 results = get_facet_values_in_detail
       (frame, slot, facet, inference_level, slot_type, number_of_values, 
	value_selector, kb_local_only_p);
     return new Values3(Cons.remove_duplicates
			(Cons.mapcar_first(results.firstValue()),
			 _eql, this, kb_local_only_p),
			results.secondValue(), results.thirdValue());
  }

// ---------- default implementation of get-frame-handle


/**
 * Returns a   frame-handle  that uniquely identifies the   frame 
 * argument in   kb .
 */
  protected Node get_frame_handle_internal
        (Node frame, TruthValue kb_local_only_p) {
       return coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
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
  {
    if (okbc_sentences_p == _NIL)
      { return new Values3(_NIL, _T, _NIL); }
    frame = coerce_to_frame(frame, _T, kb_local_only_p).firstValue();
    Node details = get_frame_details(frame, _direct, number_of_values,
				     kb_local_only_p).firstValue();
    Node sentences = _NIL;
    int count = 0;
    long max = 10000000;
    if (number_of_values instanceof OKBCInteger)
      { max = ((OKBCInteger)number_of_values).number; }
    Node more_status = _NIL;
    Enumerator pliste = details.enumerate_list();
    while (pliste.has_more_p()) {
      Node key = pliste.next();
      Node value = pliste.next();
      if (key == _handle || key == _name || key == _pretty_name)
	{ sentences = new Cons(Cons.list(key, frame, value), sentences);
	  count = count + 1;
	  if (count >= max) { break; }
	}
      else if (key == _frame_type)
	{ sentences = new Cons(Cons.list(value, frame), sentences);
	  count = count + 1;
	  if (count >= max) { break; }
	}
      else if (key == _primitive_p && value != _NIL)
	{ sentences = new Cons(Cons.list(__primitive, frame), sentences);
	  count = count + 1;
	  if (count >= max) { break; }
	}
      else if (key == _superclasses || key == _subclasses || key == _types)
	{ Enumerator se = value.enumerate_list();
	  while (se.has_more_p()) {
	    Node cl = se.next();
	    Node fact = null;
	    if (key == _superclasses)
	      { fact = Cons.list(__subclass_of, frame, cl); }
	    else if (key == _subclasses)
	      { fact = Cons.list(__subclass_of, cl, frame); }
	    if (key == _superclasses)
	      { fact = Cons.list(__instance_of, frame, cl); }
	    sentences = new Cons(fact, sentences);
	    count = count + 1;
	    if (count >= max) { break; }
	  }
	  if (count >= max) { break; }
	}
      else if (key == _own_slots || key == _template_slots)
	{ Enumerator e = value.enumerate_list();
	  while (e.has_more_p()) {
	    Node spec = e.next();
	    Node slot = Cons.first_if_list(spec);
	    Node values = Cons.rest_if_list(spec);
	    Node slot_of_key = 
	      (key == _own_slots ? __slot_of : __template_slot_of);
	    sentences = new Cons(Cons.list(slot_of_key, slot, frame),
				 sentences);
	    count = count + 1;
	    if (count >= max) { break; }
	    Enumerator ev = values.enumerate_list();
	    while (ev.has_more_p()) {
	      Node v = ev.next();
	      boolean default_p = (v instanceof Cons) && (v.car() == _default);
	      if (!default_p ||
		  value_selector == _either ||
		  value_selector == _default_only)
		{ if (default_p) { v = v.cadr(); }
		  Node fact = null;
		  if (key == _own_slots)
		    { fact = Cons.list(slot, frame, value); }
		  else { fact = Cons.list(__template_slot_value, 
					  slot, frame, value); 
		       }
		  sentences = new Cons(fact, sentences);
		  count = count + 1;
		  if (count >= max) { break; }
		}
	    }
	    if (count >= max) { break; }
	  }
	}
      else if (key == _own_facets || key == _template_facets)
	{ Enumerator e = value.enumerate_list();
	  while (e.has_more_p()) {
	    Node spec = e.next();
	    Node slot = Cons.first_if_list(spec);
	    Node fspecs = Cons.rest_if_list(spec);
	    Node slot_of_key = 
	      (key == _own_slots ? __slot_of : __template_slot_of);
	    sentences = new Cons(Cons.list(slot_of_key, slot, frame),
				 sentences);
	    count = count + 1;
	    if (count >= max) { break; }
	    Enumerator ef = fspecs.enumerate_list();
	    while (ef.has_more_p()) {
	      Node fspec = ef.next();
	      Node facet = Cons.first_if_list(fspec);
	      Node values = Cons.rest_if_list(fspec);
	      Node facet_of_key = 
		(key == _own_facets ? __facet_of : __template_facet_of);
	      sentences = new Cons(Cons.list(facet_of_key, facet, slot, frame),
				   sentences);
	      count = count + 1;
	      if (count >= max) { break; }
	      Enumerator ev = values.enumerate_list();
	      while (ev.has_more_p()) {
		Node v = ev.next();
		boolean default_p 
		  = (v instanceof Cons) && (v.car() == _default);
		if (!default_p ||
		    value_selector == _either ||
		    value_selector == _default_only)
		  { if (default_p) { v = v.cadr(); }
		    Node fact = null;
		    if (key == _own_facets)
		      { fact = Cons.list(facet, slot, frame, value); }
		    else { fact = Cons.list(__template_facet_value, 
					    facet, slot, frame, value); 
			 }
		    sentences = new Cons(fact, sentences);
		    count = count + 1;
		    if (count >= max) { break; }
		  }
	      }
	      if (count >= max) { break; }
	    }
	    if (count >= max) { break; }
	  }
	}
    }
    return new Values3(sentences, _NIL, more_status);
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
     TruthValue kb_local_only_p) {
    if (inference_level == _taxonomic || inference_level == _all_inferable)
      { if (value_selector == _default_only || value_selector == _either)
	  { return get_frames_with_slot_or_facet_value_default_or_either
	      (slot, facet, value,inference_level, slot_type, value_selector,
	       kb_local_only_p, true);
	  }
	else // _known_true
	  { return get_inherited_frames_with_slot_or_facet_value_known_true
	      (slot, facet, value,inference_level, slot_type, value_selector,
	       kb_local_only_p, true);
	  }
      }
    else // _direct
      { if (slot_type == _all)
	  { Values2 ownv = default_direct_get_frames_with_facet_value
	      (slot, facet, value, inference_level, _own, value_selector,
	       kb_local_only_p);
	    Values2 templatev = default_direct_get_frames_with_facet_value
	      (slot, facet, value, inference_level, _template, value_selector,
	       kb_local_only_p);
	    return new Values2(Cons.append_cons(ownv.firstValue(),
						templatev.firstValue()),
			       ((ownv.secondValue() != _NIL) &&
				(templatev.secondValue() != _NIL)
				? (Node)_T
				: (Node)_NIL));
	  }
	else
	  { return default_direct_get_frames_with_facet_value
	      (slot, facet, value, inference_level, slot_type, value_selector,
	       kb_local_only_p);
	  }
      }
  }


  protected Values2 default_direct_get_frames_with_facet_value
    (Node slot, Node facet, Node value, InferenceLevel inference_level,
     SlotType slot_type, ValueSelector value_selector,
     TruthValue kb_local_only_p) {
    boolean inexact_p = false;
    Node frames = _NIL;
    Enumerator framese = enumerate_kb_frames(kb_local_only_p);
    framese.prefetch(_all);
    while(framese.has_more_p()) {
     Node frame = framese.next();
     if (frame_has_slot_p(frame, slot, inference_level, slot_type,
			  kb_local_only_p) != _NIL &&
	 slot_has_facet_p(frame, slot, facet, inference_level, slot_type,
			  kb_local_only_p) != _NIL)
       { Values2 resv = member_facet_value_p(frame, slot, facet, value, 
					     inference_level, _equal, 
					     slot_type,
					     value_selector, kb_local_only_p);
	 Node res = resv.firstValue();
	 Node exact_p = resv.secondValue();
	 if (res != _NIL)
	   { frames = new Cons(frame, frames); }
	 if (exact_p == _NIL)
	   { inexact_p = true; }
       }
    }
    framese.free();
	if (inexact_p)
		{ return new Values2(Cons.remove_duplicates
				     (frames, _eql, this, kb_local_only_p),
				     _NIL); 
		}
    else { return new Values2(Cons.remove_duplicates
			      (frames, _eql, this, kb_local_only_p),
			      _T); 
	 }
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
	TruthValue kb_local_only_p) {
    if (inference_level == _taxonomic || inference_level == _all_inferable)
      { if (value_selector == _default_only || value_selector == _either)
	  { return get_frames_with_slot_or_facet_value_default_or_either
	      (slot, null, value,inference_level, slot_type, value_selector,
	       kb_local_only_p, false);
	  }
	else // _known_true
	  { return get_inherited_frames_with_slot_or_facet_value_known_true
	      (slot, null, value,inference_level, slot_type, value_selector,
	       kb_local_only_p, false);
	  }
      }
    else // _direct
      { if (slot_type == _all)
	  { Values2 ownv = default_direct_get_frames_with_slot_value
	      (slot, value, inference_level, _own, value_selector,
	       kb_local_only_p);
	    Values2 templatev = default_direct_get_frames_with_slot_value
	      (slot, value, inference_level, _template, value_selector,
	       kb_local_only_p);
	    return new Values2(Cons.append_cons(ownv.firstValue(),
						templatev.firstValue()),
			       ((ownv.secondValue() != _NIL) &&
				(templatev.secondValue() != _NIL)
				? (Node)_T
				: (Node)_NIL));
	  }
	else { return default_direct_get_frames_with_slot_value
		 (slot, value, inference_level, slot_type, value_selector,
		  kb_local_only_p);
	     }
      }
  }


  Values2 get_inherited_frames_with_slot_or_facet_value_known_true
    (Node slot, Node facet, Node value, InferenceLevel inference_level,
     SlotType slot_type, ValueSelector value_selector, 
     TruthValue kb_local_only_p, boolean facet_op_p) {
      boolean inexact_p = false;
      Values2 localv = null;
      if (facet_op_p)
	{ localv = get_frames_with_facet_value(slot, facet, value, _direct, 
					       slot_type, value_selector,
					       kb_local_only_p);
	}
      else
	{ localv = get_frames_with_slot_value(slot, value, _direct, 
					       slot_type, value_selector,
					       kb_local_only_p);
	}
      Node local_matches = localv.firstValue();
      if (localv.secondValue() == _NIL) { inexact_p = true; }
      Node inherited_values = _NIL;
      Enumerator e = local_matches.enumerate_list();
      while(e.has_more_p()) {
	Node frame = e.next();
	if (class_p(frame, kb_local_only_p) != _NIL)
	  { if (slot_type == _template || slot_type == _all)
	      { if (slot_type == _all)
		  { Values2 v = get_class_instances
		      (frame, inference_level, _all, kb_local_only_p);
		    if (v.secondValue() == _NIL) { inexact_p = true; }
		    inherited_values = Cons.append_cons(v.firstValue(),
							inherited_values);
		  }
		Values2 subclassesv = get_class_subclasses
		  (frame, inference_level, _all, kb_local_only_p);
		if (subclassesv.secondValue() == _NIL) { inexact_p = true; }
		inherited_values = Cons.append_cons(subclassesv.firstValue(),
						    inherited_values);
	      }
	  }
      }
      Node all = Cons.remove_duplicates(Cons.append_cons(local_matches, 
							 inherited_values),
					_eql, this, kb_local_only_p);
      if (slot_type == _own)
	// Step up to classes and look down.
	{ Values2 ownv = null;
	  if (facet_op_p)
	    { ownv = get_frames_with_facet_value(slot, facet, value, _direct,
						 _template, value_selector,
						 kb_local_only_p);
	    }
	  else
	    { ownv = get_frames_with_slot_value(slot, value, _direct, 
						_template, value_selector,
						kb_local_only_p);
	    }
	  Node own_local_matches = ownv.firstValue();
	  if (ownv.secondValue() == _NIL) { inexact_p = true; }
	  Node own_inherited_values = all;
	  Enumerator owne = own_local_matches.enumerate_list();
	  while (owne.has_more_p()) {
	    Node frame = owne.next();
	    // The frames are all classes because they have SLOT as a template
	    // slot
	    Values2 instancesv = get_class_instances(frame, inference_level,
						     _all, kb_local_only_p);
	    if (instancesv.secondValue() == _NIL) { inexact_p = true; }
	    own_inherited_values = Cons.append_cons(instancesv.firstValue(),
						    own_inherited_values);
	  }
	  Node all_own = Cons.remove_duplicates(own_inherited_values,
						_eql, this, kb_local_only_p);
	  return new Values2(all_own, (inexact_p ? (Node)_NIL : (Node)_T));
	}
      else { return new Values2(all, (inexact_p ? (Node)_NIL : (Node)_T)); }
    }


  Values2 member_slot_or_facet_value_p (Node frame, Node slot, Node facet,
					Node value,
					InferenceLevel inference_level,
					SlotType slot_type,
					ValueSelector value_selector,
					TruthValue kb_local_only_p,
					boolean facet_op_p) {
    if (slot_type == _all)
      { Values2 v = member_slot_or_facet_value_p(frame, slot, facet, value, 
						 inference_level, _own,
						 value_selector,
						 kb_local_only_p,
						 facet_op_p);
	if (v.firstValue() != _NIL) { return v; }
	else { if (class_p(frame, kb_local_only_p) != _NIL)
		 { Values2 v2 = member_slot_or_facet_value_p
		     (frame, slot, facet, value, inference_level, _template,
		      value_selector, kb_local_only_p, facet_op_p);
		   return new Values2(v2.firstValue(),
				      (v.secondValue() != _NIL &&
				       v2.secondValue() != _NIL ? (Node)_T
				       : (Node)_NIL));
		 }
	       else { return v; }
	     }
      }
    else if (facet_op_p)
      { return member_facet_value_p(frame, slot, facet, value, inference_level,
				    _equal, slot_type, value_selector, 
				    kb_local_only_p);
      }
    else
      { return member_slot_value_p(frame, slot, value, inference_level, _equal,
				   slot_type, value_selector, 
				   kb_local_only_p);
      }
  }


  Values2 check_down_until_blocked (Node frame, Node slot, Node facet, 
				    Node value, InferenceLevel inference_level,
				    SlotType slot_type,
				    ValueSelector value_selector,
				    TruthValue kb_local_only_p,
				    boolean facet_op_p) {
    Values2 memberv = member_slot_or_facet_value_p
      (frame, slot, facet, value, inference_level, slot_type, value_selector,
       kb_local_only_p, facet_op_p);
    boolean inexact_p = false;
    if (memberv.firstValue() != _NIL)
      { Node all = _NIL;
	if (class_p(frame, kb_local_only_p) != _NIL &&
	    (slot_type == _template || slot_type == _all))
	  { if (slot_type == _all)
	      { Values2 allv = get_class_instances(frame, _direct, _all,
						   kb_local_only_p);
		if (allv.secondValue() == _NIL) { inexact_p = true; }
		Enumerator e = allv.firstValue().enumerate_list();
		while (e.has_more_p()) {
		  Node instance = e.next();
		  Values2 resv = check_down_until_blocked
		    (instance, slot, facet, value, inference_level, _own,
		     value_selector, kb_local_only_p, facet_op_p);
		  if (resv.secondValue() == _NIL) { inexact_p = true; }
		  all = Cons.append_cons(resv.firstValue(), all);
		}
	      }
	    Values2 subv = get_class_subclasses(frame, _direct, _all,
						kb_local_only_p);
	    if (subv.secondValue() == _NIL) { inexact_p = true; }
	    Enumerator sube = subv.firstValue().enumerate_list();
	    while (sube.has_more_p()) {
	      Node instance = sube.next();
	      Values2 resv = check_down_until_blocked
		(instance, slot, facet, value, inference_level, slot_type,
		 value_selector, kb_local_only_p, facet_op_p);
	      if (resv.secondValue() == _NIL) { inexact_p = true; }
	      all = Cons.append_cons(resv.firstValue(), all);
	    }
	  }
	return new Values2(new Cons(frame, all),
			   (memberv.secondValue() != _NIL && !inexact_p
			    ? (Node)_T
			    : (Node)_NIL));
      }
    else { return new Values2(_NIL, memberv.secondValue()); }
  }


  Values2 get_frames_with_slot_or_facet_value_default_or_either
    (Node slot, Node facet, Node value, InferenceLevel inference_level,
     SlotType slot_type, ValueSelector value_selector, 
     TruthValue kb_local_only_p, boolean facet_op_p) {
      boolean inexact_p = false;
      Values2 localv = null;
      if (facet_op_p)
	{ localv = get_frames_with_facet_value
	    (slot, facet, value, _direct, slot_type, value_selector,
	     kb_local_only_p);
	}
      else
	{ localv = get_frames_with_slot_value
	    (slot, value, _direct, slot_type, value_selector,
	     kb_local_only_p);
	}
      if (localv.secondValue() == _NIL) { inexact_p = true; }
      Node local_matches = localv.firstValue();
      Node all = local_matches;
      Enumerator framee = local_matches.enumerate_list();
     while (framee.has_more_p()) {
       Node frame = framee.next();
	Values2 downv = check_down_until_blocked
	  (frame, slot, facet, value, inference_level, slot_type, 
	   value_selector, kb_local_only_p, facet_op_p);
	if (downv.secondValue() == _NIL) { inexact_p = true; }
	all = Cons.append_cons(downv.firstValue(), all);
      }
      if (slot_type == _own)
	// Step up to classes and look down.
	{ Values2 upv = null;
	  if (facet_op_p)
	    { upv = get_frames_with_facet_value
		(slot, facet, value, _direct, _template, value_selector,
		 kb_local_only_p);
	    }
	  else
	    { upv = get_frames_with_slot_value
		(slot, value, _direct, _template, value_selector,
		 kb_local_only_p);
	    }
	  if (upv.secondValue() == _NIL) { inexact_p = true; }
	  Node all_own = all;
	  Enumerator upe = upv.firstValue().enumerate_list();
	  while (upe.has_more_p()) {
	    Node frame = upe.next();
	    Node subset = _NIL;
	    Values2 instancesv = get_class_instances
	      (frame, inference_level, _all, kb_local_only_p);
	    if (instancesv.secondValue() == _NIL) { inexact_p = true; }
	    Enumerator ie = instancesv.firstValue().enumerate_list();
	    while (ie.has_more_p()) {
	      Node i = ie.next();
	      Values2 memberv = member_slot_or_facet_value_p
		(i, slot, facet, value, inference_level, slot_type, 
		 value_selector, kb_local_only_p, facet_op_p);
	      Node member_p = memberv.firstValue();
	      Node exact_p = memberv.secondValue();
	      if (member_p != _NIL && exact_p == _NIL)
		{ inexact_p = true; }
	      if (member_p != _NIL) { subset = new Cons(i, subset); }
	    }
	    all_own = Cons.append_cons(subset, all_own);
	  }
	  return new Values2(Cons.remove_duplicates
			     (all_own, _eql, this, kb_local_only_p),
			     (inexact_p ? (Node)_NIL : (Node)_T));
	}
      else { return new Values2(Cons.remove_duplicates
				(all, _eql, this, kb_local_only_p),
				(inexact_p ? (Node)_NIL : (Node)_T));
	   }
    }


  protected Values2 default_direct_get_frames_with_slot_value
    (Node slot, Node value, InferenceLevel inference_level, SlotType slot_type,
     ValueSelector value_selector, TruthValue kb_local_only_p) {
    boolean inexact_p = false;
    Node frames = _NIL;
    Enumerator framese = enumerate_kb_frames(kb_local_only_p);
    framese.prefetch(_all);
    while(framese.has_more_p()) {
     Node frame = framese.next();
     if (frame_has_slot_p(frame, slot, inference_level, slot_type,
			  kb_local_only_p) != _NIL)
       { Values2 resv = member_slot_value_p(frame, slot, value, 
					    inference_level,
					    _equal, slot_type, value_selector,
					    kb_local_only_p);
	 Node res = resv.firstValue();
	 Node exact_p = resv.secondValue();
	 if (res != _NIL)
	   { frames = new Cons(frame, frames); }
	 if (exact_p == _NIL)
	   { inexact_p = true; }
       }
    }
    framese.free();
    if (inexact_p)
      { return new Values2(Cons.remove_duplicates
			   (frames, _eql, this, kb_local_only_p), _NIL); 
      }
    else { return new Values2(Cons.remove_duplicates
			      (frames, _eql, this, kb_local_only_p), _T); 
	 }
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
     (Node selector, TruthValue kb_local_only_p) {
       Vector results = new Vector();
       Enumeration e = get_kb_frames(kb_local_only_p).elements();
       for (; e.hasMoreElements() ;) {
	 Node frame = (Node)(e.nextElement());
	 if (class_p(frame, kb_local_only_p) != _NIL)
	   { results.addElement(frame); }
       }
       return Cons.toList(results);
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
     (Node selector, TruthValue kb_local_only_p) {
       Vector results = new Vector();
       Enumeration e = get_kb_frames(kb_local_only_p).elements();
       for (; e.hasMoreElements() ;) {
	 Node frame = (Node)(e.nextElement());
	 Enumeration this_frame_facets
	  = get_frame_facets(frame, _all_inferable, _all, 
			     kb_local_only_p).firstValue().elements();
	 for (; this_frame_facets.hasMoreElements() ;) {
	   Node facet = (Node)(this_frame_facets.nextElement());
	   if (!results.contains(facet))
	     { if (selector == _system_default || selector == _all)
		 { results.addElement(facet); }
	       else if (selector == _frames)
		 { if (coercible_to_frame_p(facet, kb_local_only_p) != _NIL)
		     { results.addElement(facet); }
		 }
	       else { throw new GenericError
		      (_T, new OKBCString("Illegal selector: " + selector))
		      .signal();
		    }
	     }
	 }
	 if (!results.contains(frame) && 
		 (facet_p(frame, kb_local_only_p) != _NIL))
		{ results.addElement(frame); }
       }
       return Cons.toList(results);
    }

  Values2 get_frame_facets
     (Node frame, InferenceLevel inference_level, SlotType slot_type,
      TruthValue kb_local_only_p) {
       Vector results = new Vector();
       boolean inexact_p = false;
       Values2 slotsv = get_frame_slots(frame, inference_level, slot_type,
					kb_local_only_p);
       if (slotsv.secondValue() == _NIL) { inexact_p = true; }
       Enumeration e = slotsv.firstValue().elements();
       for (; e.hasMoreElements() ;) {
	 Node slot = (Node)(e.nextElement());
	 Values2 facetsv = get_slot_facets(frame, slot, _all_inferable,
					   slot_type, kb_local_only_p);
	 if (facetsv.secondValue() == _NIL) { inexact_p = true; }
	 Enumeration this_frame_facets = facetsv.firstValue().elements();
	 for (; this_frame_facets.hasMoreElements() ;) {
	   Node facet = (Node)(this_frame_facets.nextElement());
	   if (!results.contains(facet))
	     { results.addElement(facet); }
	 }
       }
       if (inexact_p) { return new Values2(Cons.toList(results), _NIL); }
       else { return new Values2(Cons.toList(results), _T); }
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
     (Node selector, TruthValue kb_local_only_p) {
       Vector results = new Vector();
       Enumeration e = get_kb_frames(kb_local_only_p).elements();
       for (; e.hasMoreElements() ;) {
	 Node frame = (Node)(e.nextElement());
	 if (class_p(frame, kb_local_only_p) == _NIL)
	   { results.addElement(frame); }
       }
       return Cons.toList(results);
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
     (Node selector, TruthValue kb_local_only_p) {
       Vector results = new Vector();
       Enumeration e = get_kb_frames(kb_local_only_p).elements();
       for (; e.hasMoreElements() ;) {
	 Node frame = (Node)(e.nextElement());
	 Enumeration this_frame_slots
	  = get_frame_slots(frame, _all_inferable, _all,
			    kb_local_only_p).elements();
	 for (; this_frame_slots.hasMoreElements() ;) {
	   Node slot = (Node)(this_frame_slots.nextElement());
	   if (!results.contains(slot))
	     { if (selector == _system_default || selector == _all)
		 { results.addElement(slot); }
	       else if (selector == _frames)
		 { if (coercible_to_frame_p(slot, kb_local_only_p) != _NIL)
		     { results.addElement(slot); }
		 }
	       else { throw new GenericError
		      (_T, new OKBCString("Illegal selector: " + selector))
		      .signal();
		    }
	     }
	 }
	 if (!results.contains(frame) &&
		 (slot_p(frame, kb_local_only_p) != _NIL))
		{ results.addElement(frame); }
       }
       return Cons.toList(results);
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
	  ValueSelector value_selector, TruthValue kb_local_only_p) {
     Values4 results = get_slot_values_in_detail
       (frame, slot, inference_level, slot_type, number_of_values, 
	value_selector, kb_local_only_p);
     return new Values3(Cons.remove_duplicates
			(Cons.mapcar_first(results.firstValue()),
			 _eql, this, kb_local_only_p),
		       results.secondValue(), results.thirdValue());
  }

// ---------- default implementation of individual-p


/**
 * Returns   if   thing  identifies an individual entity, and
 * returns   if   thing  identifies a class.
 */
  protected Node individual_p_internal
     (Node thing, TruthValue kb_local_only_p) {
     if (class_p(thing, kb_local_only_p) == _NIL)
       { return _T; }
     else { return _NIL; }
   }


// ---------- default implementation of instance-of-p


/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  protected Values2 instance_of_p_internal
     (Node thing,
      Node the_class,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
       if (inference_level == _direct)
	 { Values3 vals = get_class_instances(the_class, _direct, _all, 
					     kb_local_only_p);
	   if (Cons.member_frame_list(thing, vals.firstValue(), this,
				      kb_local_only_p))
	     { return new Values2(_T, vals.secondValue());}
	   else { return new Values2(_NIL, vals.secondValue());}
	 }
       else 
	 { Values3 vals = get_instance_types(thing, inference_level, _all, 
					     kb_local_only_p);
	   if (Cons.member_frame_list(the_class, vals.firstValue(), this,
				      kb_local_only_p))
	     { return new Values2(_T, vals.secondValue());}
	   else 
	     { boolean inexact_p = false;
	       if (vals.secondValue() == _NIL)
		 { inexact_p = true; }
	       boolean found_p = false;
	       Enumerator types = vals.firstValue().enumerate_list();
	       types.prefetch(_all);
	       for (; types.has_more_p() ;) {
		 Node type = types.next();
		 Values2 sub_found_pV = subclass_of_p(type, the_class, 
						      inference_level,
						      kb_local_only_p);
		 Node sub_found_p = sub_found_pV.firstValue();
		 Node exact_p = sub_found_pV.secondValue();
		 if (exact_p == _NIL) { inexact_p = true; }
		 if (sub_found_p != _NIL) { found_p = true; break; }
	       }
	       types.free();
	       if (found_p) { return new Values2(_T, _T); }
	       else if (inexact_p) { return new Values2(_NIL, _NIL); }
	       else { return new Values2(_NIL, _T); }
	     }
	 }
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
      ValueSelector value_selector, TruthValue kb_local_only_p) {
     Values3 results = get_facet_values
       (frame, slot, facet, inference_level, slot_type, _all, 
	value_selector, kb_local_only_p);
     if (Cons.member_test(value, results.firstValue(), test, this,
			  kb_local_only_p))
       { return new Values2(_T, results.secondValue()); }
     else { return new Values2(_NIL, results.secondValue()); }
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
      TruthValue kb_local_only_p) {
     Values3 results = get_slot_values
       (frame, slot, inference_level, slot_type, _all, value_selector,
	kb_local_only_p);
     if (Cons.member_test(value, results.firstValue(), test, this,
			  kb_local_only_p))
       { return new Values2(_T, results.secondValue()); }
     else { return new Values2(_NIL, results.secondValue()); }
    }


// ---------- default implementation of remove-class-superclass


/**
 * Removes   superclass-to-remove  from the superclasses of   class .
 * Returns no values.
 */
  protected void remove_class_superclass_internal
     (Node the_class, Node superclass_to_remove,
      TruthValue kb_local_only_p) {
       Node values = get_class_superclasses(the_class, _direct, _all, 
					    kb_local_only_p).firstValue();
       if (Cons.member_test(superclass_to_remove, values, _eql, this,
			    kb_local_only_p))
	 { Node trimmed = Cons.remove_test(superclass_to_remove, values, _eql,
					   this, kb_local_only_p);
	   if (trimmed == null) { trimmed = _NIL; }
	   put_class_superclasses(the_class, trimmed, kb_local_only_p);
	 }
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
      TruthValue kb_local_only_p) {
       Node values = get_facet_values(frame, slot, facet, _direct, slot_type,
				      _all, value_selector, 
				      kb_local_only_p).firstValue();
       if (Cons.member_test(value, values, test, this, kb_local_only_p))
	 { Node trimmed = Cons.remove_test(value, values, test, this, 
					   kb_local_only_p);
	   if (trimmed == null) { trimmed = _NIL; }
	   put_facet_values(frame, slot, facet, trimmed, slot_type,
			    value_selector, kb_local_only_p);
	 }
    }


// ---------- default implementation of remove-instance-type


/**
 * Removes   type-to-remove  from the types of   frame  -- that is,
 * makes   frame  no longer be an instance of   type-to-remove .
 * Returns no values.
 */
  protected void remove_instance_type_internal
     (Node frame, Node type_to_remove,
      TruthValue kb_local_only_p) {
       Node values = get_instance_types(frame, _direct, _all, 
					kb_local_only_p).firstValue();
       if (Cons.member_test(type_to_remove, values, _eql, this,
			    kb_local_only_p))
	 { Node trimmed = Cons.remove_test(type_to_remove, values, _eql,
					   this, kb_local_only_p);
	   if (trimmed == null) { trimmed = _NIL; }
	   put_instance_types(frame, trimmed, kb_local_only_p);
	 }
    }


// ---------- default implementation of remove-local-facet-values


/**
 * Removes all direct values of   facet  in   slot  of   frame .
 * Returns no values.
 */
  protected void remove_local_facet_values_internal
     (Node frame, Node slot, Node facet, SlotType slot_type,
      ValueSelector value_selector, TruthValue kb_local_only_p) {
       put_facet_values(frame, slot, facet, _NIL, slot_type, value_selector, 
			kb_local_only_p);
    }


// ---------- default implementation of remove-local-slot-values


/**
 * Removes all direct values in   slot  of   frame .
 * Returns no values.
 */
  protected void remove_local_slot_values_internal
     (Node frame, Node slot, SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p) {
       put_slot_values(frame, slot, _NIL, slot_type, value_selector, 
		       kb_local_only_p);
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
      Node index, ValueSelector value_selector, TruthValue kb_local_only_p) {
       Node values = get_slot_values(frame, slot, _direct, slot_type, _all,
				     value_selector, 
				     kb_local_only_p).firstValue();
       if (Cons.member_test(value, values, test, this, kb_local_only_p))
	 { if ((index == _all) ||
	       (get_collection_type(frame, slot, slot_type, 
				    kb_local_only_p).equals(_set)))
	     { 
	       Node trimmed = Cons.remove_test(value, values, test, this,
					       kb_local_only_p);
	       if (trimmed == null) { trimmed = _NIL; }
               put_slot_values(frame, slot,
			       trimmed, slot_type, value_selector,
			       kb_local_only_p);
	     }
	   else
	     { 
               long idx = (index instanceof OKBCInteger
			  ? ((OKBCInteger)index).number
			  : 0);
	       if (!(call_procedure(test, Cons.list(value, 
						    values.nth((int)idx)))
		     == _NIL))
		 { 
		   Node trimmed = Cons.remove_test(value, values, test,
						   idx, 1, this,
						   kb_local_only_p);
		   if (trimmed == null) { trimmed = _NIL; }
		   put_slot_values
		     (frame, slot, trimmed, slot_type, value_selector,
		      kb_local_only_p);
		 }
	     }
	 }
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
      TruthValue kb_local_only_p) {
       remove_facet_value
	 (frame, slot, facet, old_value, test, slot_type, value_selector,
	  kb_local_only_p);
       add_facet_value
	 (frame, slot, facet, new_value, test, slot_type, value_selector,
	  kb_local_only_p);
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
      TruthValue kb_local_only_p) {
       if (index == _all)
	 { Node values = get_slot_values(frame, slot, _direct, slot_type, _all,
					 value_selector,
					 kb_local_only_p).firstValue();
	   if (Cons.member_test(old_value, values, test, this,
				kb_local_only_p))
	     { put_slot_values(frame, slot,
			       Cons.substitute_test
			       (new_value, old_value,values, test, this),
			       slot_type, value_selector, kb_local_only_p);
	     }
	   //else { }
	 }
       else { remove_slot_value(frame, slot, old_value, test,
				slot_type, index, value_selector,
				kb_local_only_p);
	      add_slot_value(frame, slot, new_value, test,
			     slot_type, index, value_selector,
			     kb_local_only_p);
	    }
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
      SlotType slot_type, TruthValue kb_local_only_p) {
       return frame_has_slot_or_facet_p (frame, slot, facet, inference_level,
					 slot_type, kb_local_only_p, true);
    }

// ---------- default implementation of slot-has-value-p


/**
 * Returns   iff   slot  on   frame  has at least one
 * value, otherwise returns  .
 */
  protected Values2 slot_has_value_p_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector, 
      TruthValue kb_local_only_p) {
     Values3 results = get_slot_values
       (frame, slot, inference_level, slot_type, new OKBCInteger(1), 
	value_selector, kb_local_only_p);
     if (results.firstValue() == _NIL)
       { return new Values2(_NIL, results.secondValue()); }
     else { return new Values2(_T, results.secondValue()); }
    }


// ---------- default implementation of subclass-of-p


/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
   protected Values2 subclass_of_p_internal
     (Node subclass, Node superclass, InferenceLevel inference_level, 
      TruthValue kb_local_only_p) {
       Values3 vals = get_class_subclasses(superclass, inference_level, _all,
					   kb_local_only_p);
       if (Cons.member_frame_list(subclass, vals.firstValue(), this, 
				  kb_local_only_p))
	 { return new Values2(_T, vals.secondValue()); }
       else { return new Values2(_NIL, vals.secondValue()); }
    }


// ---------- default implementation of superclass-of-p


/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
  protected Values2 superclass_of_p_internal
     (Node superclass, Node subclass, InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
       return subclass_of_p(subclass, superclass, inference_level,
			    kb_local_only_p);
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
      TruthValue kb_local_only_p) {
        TellAndAsk.default_tell(sentence, this, frame, value_selector,
				kb_local_only_p);
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
     (Node sentence, ValueSelector value_selector,
      TruthValue kb_local_only_p) {
    return TellAndAsk.default_tellable(sentence, this, value_selector,
				       kb_local_only_p);
  }


// ---------- default implementation of type-of-p


/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  protected Values2 type_of_p_internal
     (Node the_class, Node frame, InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
       return instance_of_p(frame, the_class, inference_level, 
			    kb_local_only_p);
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
      TruthValue kb_local_only_p) {
    return TellAndAsk.default_untell(sentence, this, frame, value_selector,
				     kb_local_only_p);
  }


//-----------------------------------------------------------------------------

// From default inheritance mixin....

// ---------- default implementation of get-instance-types


/**
 * Returns the   list-of-types  of   frame , that is, the list of
 * classes of which   frame  is an instance.
 */
  protected Values3 get_instance_types_internal
     (Node instance, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p) {
       if (inference_level == _direct)
	 { throw new MethodMissing(new OKBCString("get_instance_types"),
				   this).signal();
	 }
       Values2 v = get_instance_types(instance, _direct, number_of_values,
				      kb_local_only_p);
       Node typeclasses = v.firstValue();
       Node exact_p = v.secondValue();
       boolean inexact_p = (exact_p == _NIL ? false : true);
       Node results = typeclasses;
       Enumeration e = typeclasses.elements();
       for (; e.hasMoreElements() ;) {
	 Node typeclass = (Node)e.nextElement();
	 Values2 superv =
	   get_class_superclasses(typeclass, inference_level, _all,
				  kb_local_only_p);
	 Node all_types = superv.firstValue();
	 if (superv.secondValue() == _NIL) { inexact_p = true; }
	 Enumeration e2 = all_types.elements();
	 for (; e2.hasMoreElements() ;) {
	   Node cl = (Node)e2.nextElement();
	   if (!(Cons.member_eql(cl, results, this, kb_local_only_p)))
	     { results = new Cons(cl, results); }
	 }
       }
       // Not handling number-of-values.
       return new Values3(results, 
			  (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
			  _NIL);
    }


  protected Values3 get_inherited_slots (Node frame, InferenceLevel
					 inference_level, SlotType slot_type,
					 TruthValue kb_local_only_p) {
      Node class_p = class_p(frame, kb_local_only_p);
      Node inherited = null;
      boolean inexact_p = false;
      Node ret = null;
      if (class_p != _NIL)
	{ if (slot_type == _template || slot_type == _all)
	    { Values3 v = get_class_superclasses
	      (frame, inference_level, _all, kb_local_only_p);
	      Node superclasses = v.firstValue();
	      Node exact_p = v.secondValue();
	      if (exact_p == _NIL) { inexact_p = true; }
	      Enumeration e = superclasses.elements();
	      for (; e.hasMoreElements() ;) {
		Node cl = (Node)(e.nextElement());
		if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
		  { Values2 slotsv = get_frame_slots (cl, _direct, _template,
						      kb_local_only_p);
		    Node slots = slotsv.firstValue();
		    Node sexact_p = slotsv.secondValue();
		    if (sexact_p == _NIL) { inexact_p = true; }
		    inherited = Cons.append_cons (slots, inherited);
		  }
	      }
	    }
	  }
      if (slot_type == _own || slot_type == _all)
	{ Values3 v = get_instance_types
	    (frame, inference_level, _all, kb_local_only_p);
	  Node types = v.firstValue();
	  Node exact_p = v.secondValue();
	  if (exact_p == _NIL) { inexact_p = true; }
	  Enumeration e = types.elements();
	  for (; e.hasMoreElements() ;) {
	    Node cl = (Node)(e.nextElement());
	    if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
	      { Values2 slotsv = get_frame_slots (cl, _direct, _template,
						  kb_local_only_p);
		Node slots = slotsv.firstValue();
		Node sexact_p = slotsv.secondValue();
		if (sexact_p == _NIL) { inexact_p = true; }
		inherited = Cons.append_cons (slots, inherited);
	      }
	  }
	}
      Values2 localv = get_frame_slots (frame, _direct, slot_type,
				       kb_local_only_p);
      Node local_slots = localv.firstValue();
      Node local_exact_p = localv.secondValue();
      if (local_exact_p == _NIL) { inexact_p = true; }
      return new Values3(Cons.remove_duplicates
			 (Cons.append_cons(inherited, local_slots), _eql,
			  this, kb_local_only_p),
			 (inexact_p ? _NIL : _NIL),
			 // Note: we aren't handling the more-p here.
			 _NIL);
    }


// ---------- default implementation of get-class-superclasses


/**
 * Returns the   list-of-superclasses  of   class .
 */
  protected Values3 get_class_superclasses_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p) {
       if (inference_level == _direct)
	 { throw new MethodMissing(new OKBCString("get_class_superclasses"),
				   this).signal();
	 }
       Vector results = new Vector();
       Values2 v = get_class_superclasses(the_class, _direct,
					  number_of_values,
					  kb_local_only_p);
       Node superclasses = v.firstValue();
       boolean inexact_p = (v.secondValue() == _NIL ? true : false);
       Enumeration e = superclasses.elements();
       for (; e.hasMoreElements() ;) {
	 Node the_super = (Node)(e.nextElement());
	 if (get_class_superclasses_1
	     (results, the_super, number_of_values, kb_local_only_p, 
	      the_class))
	   {}
	 else { inexact_p = true; }
       }
       return new Values3(Cons.toList(results), 
			  (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
			  _NIL);
    }


  boolean get_class_superclasses_1
     (Vector results, Node the_class, Node number_of_values,
      TruthValue kb_local_only_p, Node root_class) {
       if (!results.contains(the_class) &&
	   eql_in_kb(the_class, root_class, _NIL) == _NIL)
	 { results.addElement(the_class);
	   boolean inexact_p = false;
	   Values2 v = get_class_superclasses(the_class, _direct,
					      number_of_values, 
					      kb_local_only_p);
	   Node superclasses = v.firstValue();
	   if (v.secondValue() == _NIL) { inexact_p = true; }
	   Enumeration e = superclasses.elements();
	   for (; e.hasMoreElements() ;) {
	     Node the_super = (Node)(e.nextElement());
	     if (get_class_superclasses_1
		 (results, the_super, number_of_values, kb_local_only_p,
		  root_class))
	       {}
	     else { inexact_p = true; }
	   }
	   return !inexact_p;
       }
       else { return true; }
    }


// ---------- default implementation of get-class-subclasses


/**
 * Returns the   list-of-subclasses  of   class .
 */

  protected Values3 get_class_subclasses_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values, 
      TruthValue kb_local_only_p) {
       if (inference_level == _direct)
	 { throw new MethodMissing(new OKBCString("get_class_subclasses"),
				   this).signal();
	 }
       Vector results = new Vector();
       Values2 v = get_class_subclasses(the_class, _direct, number_of_values,
					kb_local_only_p);
       Node subclasses = v.firstValue();
       boolean inexact_p = (v.secondValue() == _NIL ? true : false);
       Enumeration e = subclasses.elements();
       for (; e.hasMoreElements() ;) {
	 Node the_sub = (Node)(e.nextElement());
	 if (get_class_subclasses_1
	     (results, the_sub, number_of_values, kb_local_only_p, 
	      the_class))
	   {}
	 else { inexact_p = true; }
       }
       return new Values3(Cons.toList(results), 
			  (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
			  _NIL);
    }

  boolean get_class_subclasses_1
     (Vector results, Node the_class, Node number_of_values,
      TruthValue kb_local_only_p, Node root_class) {
       if (!results.contains(the_class) &&
	   eql_in_kb(the_class, root_class, _NIL) == _NIL
	   // the_class != root_class
	   )
	 { results.addElement(the_class);
	   boolean inexact_p = false;
	   Values2 v = get_class_subclasses(the_class, _direct,
					    number_of_values, 
					    kb_local_only_p);
	   Node subclasses = v.firstValue();
	   if (v.secondValue() == _NIL) { inexact_p = true; }
	   Enumeration e = subclasses.elements();
	   for (; e.hasMoreElements() ;) {
	     Node the_sub = (Node)(e.nextElement());
	     if (get_class_subclasses_1
		 (results, the_sub, number_of_values, kb_local_only_p,
		  root_class))
	       {}
	     else { inexact_p = true; }
	   }
	   return !inexact_p;
       }
       else { return true; }
    }


// ---------- default implementation of get-class-instances


/**
 * Returns a   list-of-instances  for   class .
 */
  protected Values3 get_class_instances_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p) {
       if (inference_level == _direct)
	 { throw new MethodMissing(new OKBCString("get_class_instances"),
				   this).signal();
	 }
       Vector results = new Vector();
       Vector classes_visited = new Vector();
       boolean exact_p = get_class_instances_1(results, classes_visited, 
					       the_class, number_of_values,
					       kb_local_only_p);
       return new Values3(Cons.toList(results), 
			  (exact_p ? (TruthValue)_T : (TruthValue)_NIL),
			  _NIL);
    }


  boolean get_class_instances_1
     (Vector results, Vector classes_visited, Node the_class,
      Node number_of_values, TruthValue kb_local_only_p) {
       if (!classes_visited.contains(the_class))
	 { classes_visited.addElement(the_class);
	   boolean inexact_p = false;
	   Values2 v = get_class_instances
	     (the_class, _direct, number_of_values, 
	      kb_local_only_p);
	   Node instances = v.firstValue();
	   if (v.secondValue() == _NIL) { inexact_p = true; }
	   Enumeration e = instances.elements();
	   for (; e.hasMoreElements() ;) {
	     Node instance = (Node)(e.nextElement());
	     if (!results.contains(instance))
	       { results.addElement(instance); }
	   }
	   Values2 subclassesv = get_class_subclasses(the_class, _direct, _all,
						      kb_local_only_p);
	   Node subclasses = subclassesv.firstValue();
	   if (subclassesv.secondValue() == _NIL) { inexact_p = true; }
	   Enumeration e2 = subclasses.elements();
	   for (; e2.hasMoreElements() ;) {
	     Node sub = (Node)(e2.nextElement());
	     if (get_class_instances_1(results, classes_visited, sub,
				       number_of_values, kb_local_only_p))
	       {}
	     else { inexact_p = true; }
	   }
	   return !inexact_p;
	 }
       else { return true; }
    }


// ---------- default implementation of get-frame-slots


/**
 * Returns   list-of-slots , a list of all the own, template, or own
 * and template slots that are associated with   frame , depending on the
 * value of   slot-type .
 */
  protected Values2 get_frame_slots_internal
         (Node frame, InferenceLevel inference_level, SlotType slot_type,
	  TruthValue kb_local_only_p) {
      if (inference_level == _direct)
	{ throw new RuntimeException
	    ("OKBC Method not implemented for get_frame_slots in class "
	     + getClass().getName());
	  }
      else if (slot_type == _all)
	{ Values2 own_slotsv = get_frame_slots(frame, inference_level, _own,
					       kb_local_only_p);
	  Node own_slots = own_slotsv.firstValue();
	  Values2 template_slotsv = get_frame_slots(frame, inference_level,
						   _template, kb_local_only_p);
	  Node template_slots = template_slotsv.firstValue();
	  Node exact_p = _NIL;
	  if ((own_slotsv.secondValue() != _NIL) &&
	      (template_slotsv.secondValue() != _NIL))
	    { exact_p = _T; }
	  return 
	    new Values2(Cons.remove_duplicates
		       (Cons.append_cons(own_slots, template_slots),
			_equal, this, kb_local_only_p),
		       exact_p);
	  }
      else { return get_inherited_slots(frame, inference_level, slot_type,
					kb_local_only_p); }
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
	  ValueSelector value_selector, TruthValue kb_local_only_p) {
      if (inference_level == _direct)
	 { throw new MethodMissing(new OKBCString("get_slot_values_in_detail"),
				   this).signal();
	 }
      if (value_selector == _known_true)
	{ return get_slot_or_facet_values_in_detail_known_true
		   (frame, slot, null, inference_level, slot_type, 
		    number_of_values, kb_local_only_p, false);
	}
      else if (value_selector == _default_only)
	{ return get_slot_or_facet_values_in_detail_default_only
		   (frame, slot, null, inference_level, slot_type, 
		    number_of_values, kb_local_only_p, false, new Hashtable());
	}
      else if (value_selector == _either)
	{ Node class_p = class_p(frame, kb_local_only_p);
	  if (class_p != _NIL && slot_type == _template)
	    { return get_slot_or_facet_values_in_detail_either_class
		   (frame, slot, null, inference_level, slot_type, 
		    number_of_values, kb_local_only_p, false);
	    }
	  else
	    { return get_slot_or_facet_values_in_detail_either_individual
		   (frame, slot, null, inference_level, slot_type, 
		    number_of_values, kb_local_only_p, false);
	    }
	}
      else { return null; } // can never actually get here!
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
	  ValueSelector value_selector, TruthValue kb_local_only_p) {
      if (inference_level == _direct)
	 { throw new MethodMissing(new OKBCString("get_facet_values_in_detail"),
				   this).signal();
	 }
      if (value_selector == _known_true)
	{ return get_slot_or_facet_values_in_detail_known_true
		   (frame, slot, facet, inference_level, slot_type, 
		    number_of_values, kb_local_only_p, true);
	}
      else if (value_selector == _default_only)
	{ return get_slot_or_facet_values_in_detail_default_only
		   (frame, slot, facet, inference_level, slot_type, 
		    number_of_values, kb_local_only_p, true, new Hashtable());
	}
      else if (value_selector == _either)
	{ Node class_p = class_p(frame, kb_local_only_p);
	  if (class_p != _NIL && slot_type == _template)
	    { return get_slot_or_facet_values_in_detail_either_class
		   (frame, slot, facet, inference_level, slot_type, 
		    number_of_values, kb_local_only_p, true);
	    }
	  else
	    { return get_slot_or_facet_values_in_detail_either_individual
		   (frame, slot, facet, inference_level, slot_type, 
		    number_of_values, kb_local_only_p, true);
	    }
	}
      else { return null; } // can never actually get here!
    }


  Node inherited_specs(Node specs) {
    Node res = _NIL;
    Enumeration e = specs.elements();
    while (e.hasMoreElements()) {
      Node spec = (Node)(e.nextElement());
      Node value = spec.car();
      Node direct_p = spec.cadr();
      Node default_p = spec.nth(2);
      if (direct_p != _NIL)
	{ res = new Cons(Cons.list(value, _NIL, default_p), res); }
      else { res = new Cons(spec, res); }
    }
    return res;
  }


  protected Values4 get_slot_or_facet_values_in_detail_either_individual
    (Node frame, Node slot, Node facet, InferenceLevel inference_level,
     SlotType slot_type, Node number_of_values, TruthValue kb_local_only_p,
     boolean facet_op_p) {
      ValueSelector value_selector = _either;
      Node class_p = class_p(frame, kb_local_only_p);
      boolean inexact_p = false;
      boolean default_found_p = false;
      Node defaults = _NIL;
      Values3 localv = null;
      if (facet_op_p)
	{ localv = get_facet_values_in_detail
	    (frame, slot, facet, _direct, slot_type, number_of_values,
	     value_selector, kb_local_only_p);
	}
      else
	{ localv = get_slot_values_in_detail
	    (frame, slot, _direct, slot_type, number_of_values,
	     value_selector, kb_local_only_p);
	}

//      System.out.println("--------");
      Node all_values = localv.firstValue();
      if (localv.secondValue() == _NIL) { inexact_p = true; }
      if (slot_type == _own || slot_type == _all)
	{ Values3 typesv = get_instance_types
	    (frame, _direct, _all, kb_local_only_p);
	  Node types = typesv.firstValue();
	  if (typesv.secondValue() == _NIL) { inexact_p = true; }
	  Enumeration e = types.elements();
	  for (; e.hasMoreElements() ;) {
	    Node cl = (Node)(e.nextElement());
	    if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
	      {
		Values2 found_pv = coerce_to_class_internal
		  (cl, _NIL, kb_local_only_p);
		if (found_pv.secondValue() == _NIL)
		  { inexact_p = true; }
		else
		  {
		    cl = found_pv.firstValue();
		    Values4 valuesv = null;
		    if (all_values != _NIL)
		      { valuesv = get_slot_or_facet_values_in_detail_known_true
			(cl, slot, facet, inference_level, _template,
			 number_of_values, kb_local_only_p, facet_op_p);
//	System.out.println("&&&" + Cons.list(cl, slot, _template, _known_true) + " -> " + valuesv);
		      }
		    else
		      { valuesv = get_slot_or_facet_values_in_detail_either_class
			(cl, slot, facet, inference_level, _template,
			 number_of_values, kb_local_only_p, facet_op_p);
//	System.out.println("###" + Cons.list(cl, slot, _template, _either) + " -> " + valuesv);
		      }
		    Node vals = valuesv.firstValue();
		    if (valuesv.secondValue() == _NIL) { inexact_p = true; }
		    Node default_p = valuesv.fourthValue();
		    if (default_p == _NIL)
		      { Enumeration valse = vals.elements();
			while (valse.hasMoreElements()) {
			  Node spec = (Node)(valse.nextElement());
			  if (spec.nth(2) != _NIL) { default_p = _T; }
			}
		      }
//		System.out.println("default_p = " + default_p);
		    if (default_p != _NIL)
		      { default_found_p = true;
			defaults = inherited_specs(vals);
		      }
		    else { all_values = Cons.append_cons(inherited_specs(vals),
							 all_values);
			 }
		  }
	      }
	  }
//          System.out.println("--------%%%%%");
	  if (all_values == _NIL)
	    { Values4 defaultv = null;
	      if (facet_op_p)
		  { defaultv = get_facet_values_in_detail
		      (frame, slot, facet, _direct, slot_type,
		       number_of_values, _default_only, kb_local_only_p);
		  }
	      else
		  { defaultv = get_slot_values_in_detail
		      (frame, slot, _direct, slot_type,
		       number_of_values, _default_only, kb_local_only_p);
		  }
//	System.out.println("%%%" + Cons.list(frame, slot, _own, _default_only) + " -> " + defaultv);
	      if (defaultv.secondValue() == _NIL) { inexact_p = true; }
	      Node vals = defaultv.firstValue();
	      Node found_this_call_p = defaultv.fourthValue();
	      if (found_this_call_p == _NIL)
		{ Enumeration defsv = vals.elements();
		  while (defsv.hasMoreElements()) {
		    Node spec = (Node)(defsv.nextElement());
		    if (spec.nth(2) != _NIL)
		      { found_this_call_p = _T; break; }
		  }
		}
	      if (found_this_call_p != _NIL)
		{ defaults = vals;
		  default_found_p = true;
		}
	    }
	}
//      System.out.println("--------+++++++");
      if (all_values != _NIL)
	{ Node res =
	    Cons.remove_duplicates
	      (all_values, _equal, this, kb_local_only_p);
	  return new
	    Values4(res, (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
		    _NIL, _NIL);
	}
      else
	{ Node res =
	    Cons.remove_duplicates
	      (defaults, _equal, this, kb_local_only_p);
	  return new
	    Values4(res, (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
		    _NIL, _T);
	}
    }


  protected Values4 get_slot_or_facet_values_in_detail_either_class
    (Node frame, Node slot, Node facet, InferenceLevel inference_level,
     SlotType slot_type, Node number_of_values, TruthValue kb_local_only_p,
     boolean facet_op_p) {
      ValueSelector value_selector = _either;
      Node class_p = class_p(frame, kb_local_only_p);
      boolean inexact_p = false;
      boolean default_found_p = false;
      Node defaults = _NIL;
      Values3 localv = null;
      if (facet_op_p)
	{ localv = get_facet_values_in_detail
	    (frame, slot, facet, _direct, slot_type, number_of_values,
	     _known_true, kb_local_only_p);
	}
      else
	{ localv = get_slot_values_in_detail
	    (frame, slot, _direct, slot_type, number_of_values,
	     _known_true, kb_local_only_p);
	}

      Node all_values = localv.firstValue();
      if (localv.secondValue() == _NIL) { inexact_p = true; }
      if (slot_type == _template || slot_type == _all)
	{ Values3 superclassesv = get_class_superclasses
	    (frame, _direct, _all, kb_local_only_p);
	  Node superclasses = superclassesv.firstValue();
	  if (superclassesv.secondValue() == _NIL) { inexact_p = true; }
	  Enumeration e = superclasses.elements();
	  for (; e.hasMoreElements() ;) {
	    Node cl = (Node)(e.nextElement());
	    if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
	      {
		Values2 found_pv = coerce_to_class_internal
		  (cl, _NIL, kb_local_only_p);
		if (found_pv.secondValue() == _NIL)
		  { inexact_p = true; }
		else
		  {
		    cl = found_pv.firstValue();
		    Values4 valuesv = null;
		    if (all_values != _NIL)
		      { valuesv = get_slot_or_facet_values_in_detail_known_true
			(cl, slot, facet, inference_level, _template,
			 number_of_values, kb_local_only_p, facet_op_p);
		      }
		    else
		      { valuesv = get_slot_or_facet_values_in_detail_either_class
			(cl, slot, facet, inference_level, _template,
			 number_of_values, kb_local_only_p, facet_op_p);
		      }
//System.out.println("$%$%" + Cons.list(cl, slot, _template)+" -> " + valuesv);
		    Node vals = valuesv.firstValue();
		    if (valuesv.secondValue() == _NIL) { inexact_p = true; }
		    Node default_p = valuesv.fourthValue();
		    if (default_p != _NIL)
		      { default_found_p = true;
			defaults = inherited_specs(vals);
		      }
		    else { all_values = Cons.append_cons(inherited_specs(vals),
							 all_values);
			 }
		  }
	      }
	  }
	  if (all_values == _NIL)
	    { Values4 defaultv = null;
	      if (facet_op_p)
		  { defaultv = get_facet_values_in_detail
		      (frame, slot, facet, _direct, slot_type,
		       number_of_values, _default_only, kb_local_only_p);
		  }
	      else
		  { defaultv = get_slot_values_in_detail
		      (frame, slot, _direct, slot_type,
		       number_of_values, _default_only, kb_local_only_p);
		  }
	      if (defaultv.secondValue() == _NIL) { inexact_p = true; }
//System.out.println("^&^&" + Cons.list(frame, slot, _template)+" -> " +defaultv);
	      Node vals = defaultv.firstValue();
	      Node found_this_call_p = defaultv.fourthValue();
	      if (found_this_call_p == _NIL)
		{ Enumeration defsv = vals.elements();
		  while (defsv.hasMoreElements()) {
		    Node spec = (Node)(defsv.nextElement());
		    if (spec.nth(2) != _NIL)
		      { found_this_call_p = _T; break; }
		  }
		}
	      if (found_this_call_p != _NIL)
		{ defaults = vals;
		  default_found_p = true;
		}
	    }
	}
      // The following code copied out of the Individual case.  We need this
      // becase of own slots inherited from template slots on metaclasses

      if (slot_type == _own || slot_type == _all)
	{ Values3 typesv = get_instance_types
	    (frame, _direct, _all, kb_local_only_p);
	  Node types = typesv.firstValue();
	  if (typesv.secondValue() == _NIL) { inexact_p = true; }
	  Enumeration e = types.elements();
	  for (; e.hasMoreElements() ;) {
	    Node cl = (Node)(e.nextElement());
	    if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
	      {
		Values2 found_pv = coerce_to_class_internal
		  (cl, _NIL, kb_local_only_p);
		if (found_pv.secondValue() == _NIL)
		  { inexact_p = true; }
		else
		  {
		    cl = found_pv.firstValue();
		    Values4 valuesv = null;
		    if (all_values != _NIL)
		      { valuesv = get_slot_or_facet_values_in_detail_known_true
			(cl, slot, facet, inference_level, _template,
			 number_of_values, kb_local_only_p, facet_op_p);
//	System.out.println("&&&" + Cons.list(cl, slot, _template, _known_true) + " -> " + valuesv);
		      }
		    else
		      { valuesv = get_slot_or_facet_values_in_detail_either_class
			(cl, slot, facet, inference_level, _template,
			 number_of_values, kb_local_only_p, facet_op_p);
//	System.out.println("###" + Cons.list(cl, slot, _template, _either) + " -> " + valuesv);
		      }
		    Node vals = valuesv.firstValue();
		    if (valuesv.secondValue() == _NIL) { inexact_p = true; }
		    Node default_p = valuesv.fourthValue();
		    if (default_p == _NIL)
		      { Enumeration valse = vals.elements();
			while (valse.hasMoreElements()) {
			  Node spec = (Node)(valse.nextElement());
			  if (spec.nth(2) != _NIL) { default_p = _T; }
			}
		      }
//		System.out.println("default_p = " + default_p);
		    if (default_p != _NIL)
		      { default_found_p = true;
			defaults = inherited_specs(vals);
		      }
		    else { all_values = Cons.append_cons(inherited_specs(vals),
							 all_values);
			 }
		  }
	      }
	  }
//          System.out.println("--------%%%%%");
	  if (all_values == _NIL)
	    { Values4 defaultv = null;
	      if (facet_op_p)
		  { defaultv = get_facet_values_in_detail
		      (frame, slot, facet, _direct, slot_type,
		       number_of_values, _default_only, kb_local_only_p);
		  }
	      else
		  { defaultv = get_slot_values_in_detail
		      (frame, slot, _direct, slot_type,
		       number_of_values, _default_only, kb_local_only_p);
		  }
//	System.out.println("%%%" + Cons.list(frame, slot, _own, _default_only) + " -> " + defaultv);
	      if (defaultv.secondValue() == _NIL) { inexact_p = true; }
	      Node vals = defaultv.firstValue();
	      Node found_this_call_p = defaultv.fourthValue();
	      if (found_this_call_p == _NIL)
		{ Enumeration defsv = vals.elements();
		  while (defsv.hasMoreElements()) {
		    Node spec = (Node)(defsv.nextElement());
		    if (spec.nth(2) != _NIL)
		      { found_this_call_p = _T; break; }
		  }
		}
	      if (found_this_call_p != _NIL)
		{ defaults = vals;
		  default_found_p = true;
		}
	    }
	}

      
      if (all_values != _NIL)
	{ Node res =
	    Cons.remove_duplicates
	      (all_values, _equal, this, kb_local_only_p);
	  return new
	    Values4(res, (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
		    _NIL, _NIL);
	}
      else
	{ Node res =
	    Cons.remove_duplicates
	      (defaults, _equal, this, kb_local_only_p);
	  return new
	    Values4(res, (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
		    _NIL, (res == _NIL ? (TruthValue)_NIL : (TruthValue)_T));
	}
    }


  protected Values4 get_slot_or_facet_values_in_detail_known_true
    (Node frame, Node slot, Node facet, InferenceLevel inference_level,
     SlotType slot_type, Node number_of_values, TruthValue kb_local_only_p,
     boolean facet_op_p) {
      ValueSelector value_selector = _known_true;
      Node class_p = class_p(frame, kb_local_only_p);
      Node inherited = null;
      boolean inexact_p = false;
      Values3 localv = null;
      if (facet_op_p)
	{ localv = get_facet_values_in_detail
	    (frame, slot, facet, _direct, slot_type, number_of_values,
	     value_selector, kb_local_only_p);
	}
      else
	{ localv = get_slot_values_in_detail
	    (frame, slot, _direct, slot_type, number_of_values,
	     value_selector, kb_local_only_p);
	}

      Node local_values = localv.firstValue();
      if (localv.secondValue() == _NIL) { inexact_p = true; }
      if (class_p != _NIL)
	{ if (slot_type == _template || slot_type == _all)
	    { Values3 superclassesv = get_class_superclasses
		(frame, inference_level, _all, kb_local_only_p);
	      Node superclasses = superclassesv.firstValue();
	      if (superclassesv.secondValue() == _NIL) { inexact_p = true; }
	      Enumeration e = superclasses.elements();
	      for (; e.hasMoreElements() ;) {
	        Node cl = (Node)(e.nextElement());
		if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
		  { Values4 valuesv = null;
		    Values2 found_pv = coerce_to_class_internal
		      (cl, _NIL, kb_local_only_p);
		    if (found_pv.secondValue() == _NIL)
		      { inexact_p = true; }
		    else
		      {
			cl = found_pv.firstValue();
			if (facet_op_p)
			  { valuesv = get_facet_values_in_detail
			    (cl, slot, facet, _direct, _template,
			     number_of_values, value_selector, kb_local_only_p);
			  }
			else
			  { valuesv = get_slot_values_in_detail
			    (cl, slot, _direct, _template,
			     number_of_values, value_selector, kb_local_only_p);
			  }
			Node vals = valuesv.firstValue();
			if (valuesv.secondValue() == _NIL) { inexact_p = true; }
			inherited = Cons.append_cons(inherited_specs(vals),
						     inherited);
		      }
		  }
	      }
	    }
	  }
      if (slot_type == _own || slot_type == _all)
	{ Values3 typesv = get_instance_types
	    (frame, inference_level, _all, kb_local_only_p);
	  Node types = typesv.firstValue();
	  if (typesv.secondValue() == _NIL) { inexact_p = true; }
	  Enumeration e = types.elements();
	  for (; e.hasMoreElements() ;) {
	    Node cl = (Node)(e.nextElement());
	    if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
	      { Values4 valuesv = null;
		Values2 found_pv = coerce_to_class_internal
		  (cl, _NIL, kb_local_only_p);
		if (found_pv.secondValue() == _NIL)
		  { inexact_p = true; }
		else
		  {
		    cl = found_pv.firstValue();
		    if (facet_op_p)
		      { valuesv = get_facet_values_in_detail
			  (cl, slot, facet, _direct, _template, 
			   number_of_values, value_selector, kb_local_only_p);
		      }
		    else
		      { valuesv = get_slot_values_in_detail
			  (cl, slot, _direct, _template, 
			   number_of_values, value_selector, kb_local_only_p);
		      }
		    Node vals = valuesv.firstValue();
		    if (valuesv.secondValue() == _NIL) { inexact_p = true; }
		    inherited = Cons.append_cons (inherited_specs(vals),
						  inherited);
		  }
	      }
	  }
	}
  
      Node res =
	Cons.remove_duplicates
	  (Cons.append_cons(local_values, inherited),
	   _equal, this, kb_local_only_p);
      return new Values4(res, (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
			 _NIL, _NIL);
    }


  protected Values4 get_slot_or_facet_values_in_detail_default_only
    (Node frame, Node slot, Node facet, InferenceLevel inference_level,
     SlotType slot_type, Node number_of_values, TruthValue kb_local_only_p,
     boolean facet_op_p, Hashtable closure_table) {
      if (closure_table.get(frame) != null)
	{ return new Values4(_NIL, _T, _NIL, _NIL); }
      ValueSelector value_selector = _default_only;
      Node class_p = class_p(frame, kb_local_only_p);
      Node inherited = null;
      boolean inexact_p = false;
      Values4 localv = null;
      if (facet_op_p)
	{ localv = get_facet_values_in_detail
	    (frame, slot, facet, _direct, slot_type, number_of_values,
	     value_selector, kb_local_only_p);
	}
      else
	{ localv = get_slot_values_in_detail
	    (frame, slot, _direct, slot_type, number_of_values,
	     value_selector, kb_local_only_p);
	}
      Node local_values = localv.firstValue();
      if (localv.secondValue() == _NIL) { inexact_p = true; }
      Node default_found_p = localv.fourthValue();
      boolean go_ahead_p = true;
      if (default_found_p != _NIL) { go_ahead_p = false; }
      Enumeration localse = local_values.elements();
      while (localse.hasMoreElements()) {
	Node spec = (Node)(localse.nextElement());
	if (spec.nth(2) != _NIL) { go_ahead_p = false; break; }
      }
      if (go_ahead_p && (class_p != _NIL))
	{ if (slot_type == _template || slot_type == _all)
	    { Values3 superclassesv = get_class_superclasses
		(frame, _direct, _all, kb_local_only_p);
	      Node superclasses = superclassesv.firstValue();
	      if (superclassesv.secondValue() == _NIL) { inexact_p = true; }
	      Enumeration e = superclasses.elements();
	      for (; e.hasMoreElements() ;) {
	        Node cl = (Node)(e.nextElement());
		if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
		  {
		    Values2 found_pv = coerce_to_class_internal
		      (cl, _NIL, kb_local_only_p);
		    if (found_pv.secondValue() == _NIL)
		      { inexact_p = true; }
		    else
		      {
			cl = found_pv.firstValue();
			Values4 valuesv =
			  get_slot_or_facet_values_in_detail_default_only
			  (cl, slot, facet, inference_level, _template,
			   number_of_values, kb_local_only_p, facet_op_p,
			   closure_table);
			Node vals = valuesv.firstValue();
			if (valuesv.secondValue() == _NIL) { inexact_p = true; }
			inherited = Cons.append_cons(inherited_specs(vals),
						     inherited);
			if (vals != _NIL) { break; }
		      }
		  }
	      }
	    }
	  }
      if (go_ahead_p)
	{ if (slot_type == _own || slot_type == _all)
	    { Values3 typesv = get_instance_types
		(frame, _direct, _all, kb_local_only_p);
	      Node types = typesv.firstValue();
	      if (typesv.secondValue() == _NIL) { inexact_p = true; }
	      Enumeration e = types.elements();
	      for (; e.hasMoreElements() ;) {
	        Node cl = (Node)(e.nextElement());
	        if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
	          {
		    Values2 found_pv = coerce_to_class_internal
		      (cl, _NIL, kb_local_only_p);
		    if (found_pv.secondValue() == _NIL)
		      { inexact_p = true; }
		    else
		      {
			cl = found_pv.firstValue();
			Values4 valuesv = 
			  get_slot_or_facet_values_in_detail_default_only
			  (cl, slot, facet, inference_level, _template, 
			   number_of_values, kb_local_only_p, facet_op_p,
			   closure_table);
			Node vals = valuesv.firstValue();
			if (valuesv.secondValue() == _NIL) { inexact_p = true; }
			inherited = Cons.append_cons (inherited_specs(vals),
						      inherited);
			if (vals != _NIL) { break; }
		      }
		  }
	      }
	    }
	  }
  
      Node res =
	Cons.remove_duplicates
	  (Cons.append_cons(local_values, inherited),
	   _equal, this, kb_local_only_p);
      return new Values4(res, (inexact_p ? (TruthValue)_NIL : (TruthValue)_T),
			 _NIL, _NIL);
    }

// ---------- default implementation of get-slot-facets


/**
 * Returns the   list-of-facets  associated with   slot  in
 *   frame .
 */
  protected Values2 get_slot_facets_internal (Node frame, Node slot,
					   InferenceLevel inference_level,
					   SlotType slot_type,
					   TruthValue kb_local_only_p) {
      if (inference_level == _direct)
	{ throw new RuntimeException
	    ("OKBC Method not implemented for get_slot_facets in class "
	     + getClass().getName());
	  }
      if (slot_type == _all)
	{ Values2 own_facetsv = get_slot_facets(frame, slot, inference_level,
						_own, kb_local_only_p);
	  Node own_facets = own_facetsv.firstValue();
	  Values2 template_facetsv = get_slot_facets(frame, slot,
						     inference_level,
						     _template,
						     kb_local_only_p);
	  Node template_facets = template_facetsv.firstValue();
	  Node exact_p = _NIL;
	  if ((own_facetsv.secondValue() != _NIL) &&
	      (template_facetsv.secondValue() != _NIL))
	    { exact_p = _T; }
	  return 
	    new Values2(Cons.remove_duplicates
		       (Cons.append_cons(own_facets, template_facets),
			_equal, this, kb_local_only_p),
		       exact_p);
	}
      return get_inherited_facets(frame, slot, inference_level, slot_type,
				  kb_local_only_p);
    }


  protected Values3 get_inherited_facets (Node frame, Node slot,
				       InferenceLevel inference_level,
				       SlotType slot_type,
				       TruthValue kb_local_only_p) {
      Node class_p = class_p(frame, kb_local_only_p);
      Node inherited = null;
      boolean inexact_p = false;
      Node ret = null;
      if (class_p != _NIL)
	{ if (slot_type == _template || slot_type == _all)
	    { Values3 v = get_class_superclasses
	      (frame, inference_level, _all, kb_local_only_p);
	      Node superclasses = v.firstValue();
	      Node exact_p = v.secondValue();
	      if (exact_p == _NIL) { inexact_p = true; }
	      Enumeration e = superclasses.elements();
	      for (; e.hasMoreElements() ;) {
		Node cl = (Node)(e.nextElement());
		if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
		  { Values2 facetsv = get_slot_facets (cl, slot, _direct,
						       _template,
						       kb_local_only_p);
		    Node facets = facetsv.firstValue();
		    Node sexact_p = facetsv.secondValue();
		    if (sexact_p == _NIL) { inexact_p = true; }
		    inherited = Cons.append_cons (facets, inherited);
		  }
	      }
	    }
	  }
      if (slot_type == _own || slot_type == _all)
	{ Values3 v = get_instance_types
	    (frame, inference_level, _all, kb_local_only_p);
	  Node types = v.firstValue();
	  Node exact_p = v.secondValue();
	  if (exact_p == _NIL) { inexact_p = true; }
	  Enumeration e = types.elements();
	  for (; e.hasMoreElements() ;) {
	    Node cl = (Node)(e.nextElement());
	    if (eql_in_kb(cl, frame, kb_local_only_p) == _NIL)
	      { Values2 facetsv = get_slot_facets (cl, slot, _direct,
						   _template,
						   kb_local_only_p);
		Node facets = facetsv.firstValue();
		Node sexact_p = facetsv.secondValue();
		if (sexact_p == _NIL) { inexact_p = true; }
		inherited = Cons.append_cons (facets, inherited);
	      }
	  }
	}
      Values2 localv = get_slot_facets (frame, slot, _direct, slot_type,
					kb_local_only_p);
      Node local_facets = localv.firstValue();
      Node local_exact_p = localv.secondValue();
      if (local_exact_p == _NIL) { inexact_p = true; }
      return new Values3(Cons.remove_duplicates
			 (Cons.append_cons(inherited, local_facets), _eql,
			  this, kb_local_only_p),
			 (inexact_p ? _NIL : _NIL),
			 // Note: we aren't handling the more-p here.
			 _NIL);
    }

}
