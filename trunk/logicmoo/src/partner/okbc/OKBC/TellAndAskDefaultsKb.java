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

public class TellAndAskDefaultsKb extends Kb {

  static final Symbol _value_var = Symbol.intern("?VALUE", "VARS");
  static final OKBCInteger one = new OKBCInteger(1);
  InferenceLevel inference_level_for_defaults = _taxonomic;

  protected InferenceLevel inference_level_for_tell_and_ask_defaults() {
    return inference_level_for_defaults;
  }

// ---------------------- add-class-superclass


/**
 * Adds the   new-superclass  to the superclasses of   class .
 * Returns no values.
 */
  protected void add_class_superclass_internal
     (Node the_class, Node new_superclass, TruthValue kb_local_only_p)
  { tell(Cons.list(__subclass_of, the_class, new_superclass),
	 the_class, _known_true, kb_local_only_p);
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
  { Node fact = null;
    if (slot_type == _own) { fact = Cons.list(facet, slot, frame, value); }
    else if (slot_type == _template)
      { fact = Cons.list(__template_facet_value, facet, slot, frame, value); }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    tell(fact, frame, value_selector, kb_local_only_p);
  }

// ---------- default implementation of add-instance-type


/**
 * Adds the   new-type  to the types of   frame .
 * Returns no values.
 */
  protected void add_instance_type_internal
     (Node frame, Node new_type, TruthValue kb_local_only_p)
  { tell(Cons.list(__instance_of, frame, new_type),
	 frame, _known_true, kb_local_only_p);
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
  { Node fact = null;
    if (slot_type == _own) { fact = Cons.list(slot, frame, value); }
    else if (slot_type == _template)
      { fact = Cons.list(__template_slot_value, slot, frame, value); }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    tell(fact, frame, value_selector, kb_local_only_p);
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
     (Node frame, Node slot, Node facet, SlotType slot_type,
      TruthValue kb_local_only_p)
  { Node fact = null;
    if (slot_type == _own) { fact = Cons.list(__facet_of, facet, slot, frame);}
    else if (slot_type == _template)
      { fact = Cons.list(__template_facet_of, facet, slot, frame); }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    tell(fact, frame, _known_true, kb_local_only_p);
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
     (Node frame, Node slot, SlotType slot_type, TruthValue kb_local_only_p)
  { Node fact = null;
    if (slot_type == _own) { fact = Cons.list(__slot_of, slot, frame); }
    else if (slot_type == _template)
      { fact = Cons.list(__template_slot_of, slot, frame); }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    tell(fact, frame, _known_true, kb_local_only_p);
  }

// ---------------------- class-p


/**
 * Returns   if   thing  identifies a class.
 */
  protected Node class_p_internal (Node thing, TruthValue kb_local_only_p)
  { return
    ask(Cons.list(_class, thing), _T, 
	inference_level_for_tell_and_ask_defaults(), one, _T, _NIL, _NIL,
	_known_true, kb_local_only_p).firstValue().car();
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
     (Node name, Node direct_types, Node direct_superclasses, 
      Node primitive_p, Node doc, Node template_slots, Node template_facets,
      Node own_slots, Node own_facets, Node handle, Node pretty_name,
      TruthValue kb_local_only_p) {
       return create_frame(name, _class, direct_types, direct_superclasses, 
			   doc, template_slots, template_facets, own_slots,
			   own_facets, primitive_p, handle, pretty_name,
			   kb_local_only_p);
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
     (Node name, Node frame, Node slot, SlotType slot_type, Node direct_types,
      Node doc, Node own_slots, Node own_facets, Node handle, Node pretty_name,
      TruthValue kb_local_only_p) {
       return create_frame(name, _facet, direct_types, _NIL, doc, _NIL, _NIL,
			   own_slots, own_facets, _T, handle, pretty_name,
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
      Node handle, Node pretty_name, TruthValue kb_local_only_p)
  { Node frame = handle;
    Node clauses = _NIL;
    Node default_clauses = _NIL;
    Enumerator e = null;
    Enumerator specse = null;
    Enumerator slot_typee = null;

    if (frame == _NIL)
      { frame = allocate_frame_handle(name, frame_type, _NIL); }

    clauses = new Cons(Cons.list(_frame, frame), clauses);

    if (handle == _NIL)
      { clauses = new Cons(Cons.list(_handle, frame, handle), clauses); }
    else { clauses = new Cons(Cons.list(_handle, frame, frame), clauses); }

    clauses = new Cons(Cons.list(frame_type, frame), clauses);

    if (name != _NIL)
      { clauses = new Cons(Cons.list(_name, frame, name), clauses); }

    if (pretty_name != _NIL)
      { clauses = new Cons(Cons.list(_pretty_name, frame, name), clauses); }

    if (doc != _NIL)
      { clauses = new Cons(Cons.list(_documentation, frame, doc), clauses); }

    e = Cons.list_if_not(direct_types).enumerate_list();
    while(e.has_more_p()) {
      Node type = e.next();
      clauses = new Cons(Cons.list(__instance_of, frame, type), clauses);
    }

    e = Cons.list_if_not(direct_superclasses).enumerate_list();
    while(e.has_more_p()) {
      Node superclass = e.next();
      clauses = new Cons(Cons.list(__subclass_of, frame, superclass), clauses);
    }

    if (frame_type == _class && primitive_p != _NIL)
      { clauses = new Cons(Cons.list(__primitive, frame), clauses); }

    slot_typee = enumerate_list(Cons.list(_own, _template));
    specse = enumerate_list(Cons.list(own_slots, template_slots));
    while(specse.has_more_p()) {
      Node specs = specse.next();
      SlotType slot_type = (SlotType)(slot_typee.next());
      Node relation_spec = _NIL;

      if (slot_type == _template) 
	{ relation_spec = Cons.list(__template_slot_value); }

      Node slot_of_spec = __slot_of;
      if (slot_type == _template) { slot_of_spec = __template_slot_of; }
      
      e = specs.enumerate_list();
      while(e.has_more_p()) {
	Node spec = e.next();
	Node slot_name = Cons.first_if_list(spec);
	Node values = _NIL;
	if (spec == _NIL || spec instanceof Cons) { values = spec.cdr(); }
	Node slot = ensure_has_slot(slot_name, slot_type, kb_local_only_p);
	if (values == _NIL)
	  { clauses = new Cons(Cons.list(slot_of_spec, slot, frame), clauses);}
	else
	  { Enumerator valuese = values.enumerate_list();
	    while(valuese.has_more_p()) {
	      Node value_spec = valuese.next();
	      Node value = value_spec;
	      if (value_spec instanceof Cons &&
		  value_spec.car() == _default)
		{ Node clause = Cons.append_cons
		    (relation_spec, Cons.list(slot, frame, value.cadr()));
		  default_clauses = new Cons(clause, default_clauses); 
		}
	      else
		{ Node clause = Cons.append_cons
		    (relation_spec, Cons.list(slot, frame, value));
		  clauses = new Cons(clause, clauses); 
		}
	    }
	  }
      }
    }

    slot_typee = enumerate_list(Cons.list(_own, _template));
    specse = enumerate_list(Cons.list(own_facets, template_facets));
    while(specse.has_more_p()) {
      Node specs = specse.next();
      SlotType slot_type = (SlotType)(slot_typee.next());
      Node relation_spec = _NIL;

      if (slot_type == _template) 
	{ relation_spec = Cons.list(__template_facet_value); }

      Node slot_of_spec = __slot_of;
      if (slot_type == _template) { slot_of_spec = __template_slot_of; }
      
      Node facet_of_spec = __facet_of;
      if (slot_type == _template) { facet_of_spec = __template_slot_of; }
      
      e = specs.enumerate_list();
      while(e.has_more_p()) {
	Node spec = e.next();
	Node slot_name = Cons.first_if_list(spec);
	Node slot = ensure_has_slot(slot_name, slot_type, kb_local_only_p);
	Node facet_specs = _NIL;
	if (spec == _NIL || spec instanceof Cons) { facet_specs = spec.cdr(); }
	if (facet_specs == _NIL)
	  { clauses = new Cons(Cons.list(slot_of_spec, slot, frame), clauses);}
	else
	  { Enumerator facet_specse = facet_specs.enumerate_list();
	    while(facet_specse.has_more_p()) {
	      Node fspec = facet_specse.next();
	      Node facet_name = Cons.first_if_list(fspec);
	      Node values = _NIL;
	      if (fspec == _NIL || fspec instanceof Cons)
		{ values = fspec.cdr(); }
	      Node facet = ensure_has_facet(facet_name, slot, slot_type,
					    kb_local_only_p);
	      if (values == _NIL)
		{ clauses = new Cons(Cons.list(facet_of_spec, facet, slot,
					       frame),
				     clauses);
		}
	      else
		{ Enumerator valuese = values.enumerate_list();
		  while(valuese.has_more_p()) {
		    Node value_spec = valuese.next();
		    Node value = value_spec;
		    if (value_spec instanceof Cons &&
			value_spec.car() == _default)
		      { Node clause = Cons.append_cons
			(relation_spec, 
			 Cons.list(facet, slot, frame, value.cadr()));
			default_clauses = new Cons(clause, default_clauses); 
		      }
		    else
		      { Node clause = Cons.append_cons
			(relation_spec, Cons.list(facet, slot, frame, value));
			clauses = new Cons(clause, clauses); 
		      }
		  }
		}
	    }
	  }
      }
    }
    return frame;
  }

  // A tell&ask back end implementor should fill these in.
  protected Node ensure_has_slot(Node slot, SlotType slot_type,
				 TruthValue kb_local_only_p) 
  { throw new MethodMissing(new OKBCString("ensure_has_slot"),
			    this).signal();
  }

  protected Node ensure_has_facet(Node facet, Node slot, SlotType slot_type,
				  TruthValue kb_local_only_p)
  { throw new MethodMissing(new OKBCString("ensure_has_facet"),
			    this).signal();
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
     (Node name, Node direct_types, Node doc, Node own_slots, Node own_facets,
      Node handle, Node pretty_name, TruthValue kb_local_only_p) 
  { return create_frame(name, _individual, direct_types, _NIL, doc, _NIL, _NIL,
			own_slots, own_facets, _T, handle, pretty_name,
			kb_local_only_p);
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
     (Node name, Node frame, SlotType slot_type, Node direct_types, Node doc,
      Node own_slots, Node own_facets, Node handle, Node pretty_name,
      TruthValue kb_local_only_p) 
  { return create_frame(name, _slot, direct_types, _NIL, doc, _NIL, _NIL,
			own_slots, own_facets, _T, handle, pretty_name,
			kb_local_only_p);
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
     (Node facet, TruthValue kb_local_only_p) 
  { Node sentences = null;
    Enumerator e = null;
    sentences = get_frame_sentences
      (facet, _all, _T, _default_only, kb_local_only_p).firstValue();
    e = sentences.enumerate_list();
    while(e.has_more_p()) {
      Node sentence = e.next();
      untell(sentence, facet, _default_only, kb_local_only_p);
    }
    sentences = get_frame_sentences
      (facet, _all, _T, _known_true, kb_local_only_p).firstValue();
    e = sentences.enumerate_list();
    while(e.has_more_p()) {
      Node sentence = e.next();
      untell(sentence, facet, _known_true, kb_local_only_p);
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
  protected void delete_frame_internal
     (Node frame, TruthValue kb_local_only_p) 
  { Node sentences = null;
    Enumerator e = null;
    sentences = get_frame_sentences
      (frame, _all, _T, _default_only, kb_local_only_p).firstValue();
    e = sentences.enumerate_list();
    while(e.has_more_p()) {
      Node sentence = e.next();
      untell(sentence, frame, _default_only, kb_local_only_p);
    }
    sentences = get_frame_sentences
      (frame, _all, _T, _known_true, kb_local_only_p).firstValue();
    e = sentences.enumerate_list();
    while(e.has_more_p()) {
      Node sentence = e.next();
      untell(sentence, frame, _known_true, kb_local_only_p);
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
  protected void delete_slot_internal (Node slot, TruthValue kb_local_only_p)
  { Node sentences = null;
    Enumerator e = null;
    sentences = get_frame_sentences
      (slot, _all, _T, _default_only, kb_local_only_p).firstValue();
    e = sentences.enumerate_list();
    while(e.has_more_p()) {
      Node sentence = e.next();
      untell(sentence, slot, _default_only, kb_local_only_p);
    }
    sentences = get_frame_sentences
      (slot, _all, _T, _known_true, kb_local_only_p).firstValue();
    e = sentences.enumerate_list();
    while(e.has_more_p()) {
      Node sentence = e.next();
      untell(sentence, slot, _known_true, kb_local_only_p);
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
  protected void detach_facet_internal
     (Node frame, Node slot, Node facet, SlotType slot_type,
      TruthValue kb_local_only_p) 
  { Node fact = null;
    if (slot_type == _own) { fact = Cons.list(__facet_of, facet, slot, frame);}
    else if (slot_type == _template)
      { fact = Cons.list(__template_facet_value, facet, slot, frame); }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(fact, frame, _known_true, kb_local_only_p);
  }

// ---------------------- detach-slot


/**
 * Removes any explicit association between the   slot  and
 *   frame .  As a result,   slot  is not returned by
 *   get-frame-slots  at inference-level    :direct  unless there are
 * slot or facet values associated with   slot  in   frame .
 */
  protected void detach_slot_internal
     (Node frame, Node slot, SlotType slot_type, TruthValue kb_local_only_p)
  { Node fact = null;
    if (slot_type == _own) { fact = Cons.list(__slot_of, slot, frame);}
    else if (slot_type == _template)
      { fact = Cons.list(__template_slot_of, slot, frame); }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(fact, frame, _known_true, kb_local_only_p);
  }

// ---------------------- facet-has-value-p


/**
 * Returns   iff the specified facet has a value for the specified slot
 * and frame, and otherwise returns  .
 */
  protected Values2 facet_has_value_p_internal
     (Node frame, Node slot, Node facet, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(facet, slot, frame, _value_var);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_value, facet, slot, frame, 
			    _value_var); 
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    Values2 v = ask(pattern, _T, inference_level, one, _T, _NIL, _NIL,
		    value_selector, kb_local_only_p);
    if (v.firstValue() == _NIL)
      { return v; }
    else { return new Values2(_T, v.secondValue()); }
  }

// ---------------------- facet-p


/**
 * Returns   iff   thing  is a facet, and   otherwise.
 */
  protected Node facet_p_internal (Node thing, TruthValue kb_local_only_p)
  { return
    ask(Cons.list(_facet, thing), _T, 
	inference_level_for_tell_and_ask_defaults(), one, _T, _NIL, _NIL,
	_known_true, kb_local_only_p).firstValue().car();
  }

// ---------------------- frame-has-slot-p


/**
 * Returns   iff   slot  is a slot in   frame ,
 * otherwise returns  .
 */
  protected Node frame_has_slot_p_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, TruthValue kb_local_only_p) 
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(__slot_of, slot, frame);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_of, slot, frame); 
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    return ask(pattern, _T, inference_level, one, _T, _NIL, _NIL,
	       _known_true, kb_local_only_p).firstValue().car();
  }

// ---------- default implementation of get-class-instances


/**
 * Returns a   list-of-instances  for   class .
 */
  protected Values3 get_class_instances_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p)
  { return
    ask(Cons.list(__instance_of, _value_var, the_class), _value_var, 
	inference_level, number_of_values, _T, _NIL, _NIL,
	_known_true, kb_local_only_p);
  }
 
// ---------- default implementation of get-class-subclasses


/**
 * Returns the   list-of-subclasses  of   class .
 */
  protected Values3 get_class_subclasses_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values, 
      TruthValue kb_local_only_p)
  { return
    ask(Cons.list(__subclass_of, _value_var, the_class), _value_var, 
	inference_level, number_of_values, _T, _NIL, _NIL,
	_known_true, kb_local_only_p);
  }

// ---------- default implementation of get-class-superclasses


/**
 * Returns the   list-of-superclasses  of   class .
 */
  protected Values3 get_class_superclasses_internal
     (Node the_class, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p)
  { return
    ask(Cons.list(__subclass_of, the_class, _value_var), _value_var, 
	inference_level, number_of_values, _T, _NIL, _NIL,
	_known_true, kb_local_only_p);
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
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(facet, slot, frame, _value_var);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_value, facet, slot, frame, 
			    _value_var);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    return ask(pattern, _value_var, inference_level, number_of_values, _T,
	       _NIL, _NIL, _known_true, kb_local_only_p);
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
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(facet, slot, frame, _value_var);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_value, facet, slot, frame, 
			    _value_var);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    Values3 v = ask(pattern, Cons.list(_value_var, 
				       (inference_level == _direct
					? (Node)_T
					: (Node)_NIL),
				       _NIL),
		    inference_level, number_of_values, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values4(v.firstValue(), v.secondValue(), v.thirdValue(), _NIL);
  }

// ---------- default implementation of get-frame-handle


/**
 * Returns a   frame-handle  that uniquely identifies the   frame 
 * argument in   kb .
 */
  protected Node get_frame_handle_internal
  (Node frame, TruthValue kb_local_only_p)
  { return
    ask(Cons.list(__handle, frame, _value_var), _value_var, 
	inference_level_for_tell_and_ask_defaults(), _all, _T, _NIL, _NIL,
	_known_true, kb_local_only_p).firstValue().car();
  }

// ---------------------- get-frame-name


/**
 * Returns   frame-name , an entity that is the name of the frame
 * identified by   frame , usually a symbol or string.
 */
  protected Node get_frame_name_internal
     (Node frame, TruthValue kb_local_only_p) 
  { return
    ask(Cons.list(__name, frame, _value_var), _value_var, 
	inference_level_for_tell_and_ask_defaults(), _all, _T, _NIL, _NIL,
	_known_true, kb_local_only_p).firstValue().car();
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
     (Node frame, TruthValue kb_local_only_p) 
  { return
    ask(Cons.list(__pretty_name, frame, _value_var), _value_var, 
	inference_level_for_tell_and_ask_defaults(), _all, _T, _NIL, _NIL,
	_known_true, kb_local_only_p).firstValue().car();
  }

// ---------------------- get-frame-slots


/**
 * Returns   list-of-slots , a list of all the own, template, or own
 * and template slots that are associated with   frame , depending on the
 * value of   slot-type .
 */
  protected Values2 get_frame_slots_internal
     (Node frame, InferenceLevel inference_level, SlotType slot_type,
      TruthValue kb_local_only_p) 
  { if (slot_type == _own)
      { return ask(Cons.list(__slot_of, _value_var, frame), 
		   _value_var, inference_level, _all, _T, _NIL, _NIL,
		   _known_true, kb_local_only_p);
      }
    else if (slot_type == _template)
      { return ask(Cons.list(__template_slot_of, _value_var, frame),
		   _value_var, inference_level, _all, _T, _NIL, _NIL,
		   _known_true, kb_local_only_p);
      }
    else if (slot_type == _all)
      { Values2 ownvs = ask(Cons.list(__slot_of, _value_var, frame), 
			    _value_var, inference_level, _all, _T, _NIL, _NIL,
			    _known_true, kb_local_only_p);
	Values2 tvs   = ask(Cons.list(__template_slot_of, _value_var, frame), 
			    _value_var, inference_level, _all, _T, _NIL, _NIL,
			    _known_true, kb_local_only_p);
	return new Values2(Cons.remove_duplicates
			   (Cons.append_cons(ownvs.firstValue(), 
					     tvs.firstValue()),
			    _equal, this, kb_local_only_p),
			   (ownvs.secondValue() == _NIL ||
			      tvs.secondValue() == _NIL
			    ? (Node)_NIL
			    : (Node)_T));
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
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
  protected Values2 get_frames_with_facet_value_internal
    (Node slot, Node facet, Node value, InferenceLevel inference_level,
     SlotType slot_type, ValueSelector value_selector, 
     TruthValue kb_local_only_p)
  { if (slot_type == _own)
      { return ask(Cons.list(facet, slot, _value_var, value),
		   _value_var, inference_level, _all, _T, _NIL, _NIL,
		   _known_true, kb_local_only_p);
      }
    else if (slot_type == _template)
      { return ask(Cons.list(__template_facet_value,
			     facet, slot, _value_var, value),
		   _value_var, inference_level, _all, _T, _NIL, _NIL,
		   _known_true, kb_local_only_p);
      }
    else if (slot_type == _all)
      { Values2 ownvs = ask(Cons.list(facet, slot, _value_var, value),
			    _value_var, inference_level, _all, _T, _NIL, _NIL,
			    _known_true, kb_local_only_p);
	Values2 tvs   = ask(Cons.list(__template_facet_value,
				      facet, slot, _value_var, value), 
			    _value_var, inference_level, _all, _T, _NIL, _NIL,
			    _known_true, kb_local_only_p);
	return new Values2(Cons.append_cons(ownvs.firstValue(), 
					    tvs.firstValue()),
			   (ownvs.secondValue() == _NIL ||
			      tvs.secondValue() == _NIL
			    ? (Node)_NIL
			    : (Node)_T));
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
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
  protected Values2 get_frames_with_slot_value_internal
     (Node slot, Node value, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { if (slot_type == _own)
      { return ask(Cons.list(slot, _value_var, value),
		   _value_var, inference_level, _all, _T, _NIL, _NIL,
		   _known_true, kb_local_only_p);
      }
    else if (slot_type == _template)
      { return ask(Cons.list(__template_slot_value, slot, _value_var, value),
		   _value_var, inference_level, _all, _T, _NIL, _NIL,
		   _known_true, kb_local_only_p);
      }
    else if (slot_type == _all)
      { Values2 ownvs = ask(Cons.list(slot, _value_var, value),
			    _value_var, inference_level, _all, _T, _NIL, _NIL,
			    _known_true, kb_local_only_p);
	Values2 tvs   = ask(Cons.list(__template_slot_value,
				      slot, _value_var, value), 
			    _value_var, inference_level, _all, _T, _NIL, _NIL,
			    _known_true, kb_local_only_p);
	return new Values2(Cons.append_cons(ownvs.firstValue(), 
					    tvs.firstValue()),
			   (ownvs.secondValue() == _NIL ||
			      tvs.secondValue() == _NIL
			    ? (Node)_NIL
			    : (Node)_T));
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
  }

// ---------------------- get-instance-types


/**
 * Returns the   list-of-types  of   frame , that is, the list of
 * classes of which   frame  is an instance.
 */
  protected Values3 get_instance_types_internal
     (Node frame, InferenceLevel inference_level, Node number_of_values,
      TruthValue kb_local_only_p)
  { return
    ask(Cons.list(__instance_of, frame, _value_var), _value_var, 
	inference_level, number_of_values, _T, _NIL, _NIL,
	_known_true, kb_local_only_p);
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
  protected Node get_kb_classes_internal
  (Node selector, TruthValue kb_local_only_p)
  { Node pattern = null;
    if (selector == _all || selector == _system_default) 
      { pattern = Cons.list(_class, _value_var); }
    else if (selector == _frames)
      { pattern = Cons.list(_and,
			    Cons.list(_class, _value_var),
			    Cons.list(_frame, _value_var));
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal selector: " + selector))
	     .signal();
	 }
    return ask(pattern, _value_var, 
	       inference_level_for_tell_and_ask_defaults(), _all, _T,
	       _NIL, _NIL, _known_true, kb_local_only_p).firstValue();
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
  protected Node get_kb_facets_internal
     (Node selector, TruthValue kb_local_only_p)
  { Node pattern = null;
    if (selector == _all || selector == _system_default) 
      { pattern = Cons.list(_facet, _value_var); }
    else if (selector == _frames)
      { pattern = Cons.list(_and,
			    Cons.list(_facet, _value_var),
			    Cons.list(_frame, _value_var));
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal selector: " + selector))
	     .signal();
	 }
    return ask(pattern, _value_var, 
	       inference_level_for_tell_and_ask_defaults(), _all, _T,
	       _NIL, _NIL, _known_true, kb_local_only_p).firstValue();
  }

// ---------------------- get-kb-frames


/**
 * Returns the   list-of-frames  in the KB, including class, slot,
 * facets and individual frames, when present.
 */
  protected Node get_kb_frames_internal (TruthValue kb_local_only_p) 
  { return ask(Cons.list(_frame, _value_var), _value_var,
	       inference_level_for_tell_and_ask_defaults(), _all, _T,
	       _NIL, _NIL, _known_true, kb_local_only_p).firstValue();
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
  protected Node get_kb_individuals_internal
        (Node selector, TruthValue kb_local_only_p)
  { Node pattern = null;
    if (selector == _all || selector == _system_default) 
      { pattern = Cons.list(_individual, _value_var); }
    else if (selector == _frames)
      { pattern = Cons.list(_and,
			    Cons.list(_individual, _value_var),
			    Cons.list(_frame, _value_var));
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal selector: " + selector))
	     .signal();
	 }
    return ask(pattern, _value_var, 
	       inference_level_for_tell_and_ask_defaults(), _all, _T,
	       _NIL, _NIL, _known_true, kb_local_only_p).firstValue();
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
  protected Node get_kb_slots_internal
     (Node selector, TruthValue kb_local_only_p)
  { Node pattern = null;
    if (selector == _all || selector == _system_default) 
      { pattern = Cons.list(_slot, _value_var); }
    else if (selector == _frames)
      { pattern = Cons.list(_and,
			    Cons.list(_slot, _value_var),
			    Cons.list(_frame, _value_var));
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal selector: " + selector))
	     .signal();
	 }
    return ask(pattern, _value_var, 
	       inference_level_for_tell_and_ask_defaults(), _all, _T,
	       _NIL, _NIL, _known_true, kb_local_only_p).firstValue();
  }

// ---------------------- get-slot-facets


/**
 * Returns the   list-of-facets  associated with   slot  in
 *   frame .
 */
  protected Values2 get_slot_facets_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, TruthValue kb_local_only_p)
  { if (slot_type == _own)
      { return ask(Cons.list(__facet_of, _value_var, slot, frame), 
		   _value_var, inference_level, _all, _T, _NIL, _NIL,
		   _known_true, kb_local_only_p);
      }
    else if (slot_type == _template)
      { return ask(Cons.list(__template_facet_of, _value_var, slot, frame),
		   _value_var, inference_level, _all, _T, _NIL, _NIL,
		   _known_true, kb_local_only_p);
      }
    else if (slot_type == _all)
      { Values2 ownvs = ask(Cons.list(__facet_of, _value_var, slot, frame), 
			    _value_var, inference_level, _all, _T, _NIL, _NIL,
			    _known_true, kb_local_only_p);
	Values2 tvs   = ask(Cons.list(__template_facet_of, _value_var, slot,
				      frame), 
			    _value_var, inference_level, _all, _T, _NIL, _NIL,
			    _known_true, kb_local_only_p);
	return new Values2(Cons.remove_duplicates
			   (Cons.append_cons(ownvs.firstValue(), 
					     tvs.firstValue()),
			    _equal, this, kb_local_only_p),
			   (ownvs.secondValue() == _NIL ||
			      tvs.secondValue() == _NIL
			    ? (Node)_NIL
			    : (Node)_T));
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type)).signal();
	 }
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
  protected Values3 get_slot_values_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, Node number_of_values, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(slot, frame, _value_var);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_value, slot, frame, 
			    _value_var);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    return ask(pattern, _value_var, inference_level, number_of_values, _T,
	       _NIL, _NIL, _known_true, kb_local_only_p);
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
  protected Values4 get_slot_values_in_detail_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, Node number_of_values, ValueSelector value_selector,
      TruthValue kb_local_only_p) 
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(slot, frame, _value_var);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_value, slot, frame, 
			    _value_var);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    Values3 v = ask(pattern, Cons.list(_value_var, 
				       (inference_level == _direct
					? (Node)_T
					: (Node)_NIL),
				       _NIL),
		    inference_level, number_of_values, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values4(v.firstValue(), v.secondValue(), v.thirdValue(), _NIL);
  }

// ---------------------- individual-p


/**
 * Returns   if   thing  identifies an individual entity, and
 * returns   if   thing  identifies a class.
 */
  protected Node individual_p_internal
        (Node thing, TruthValue kb_local_only_p)
  { return
    ask(Cons.list(_individual, thing), _T, 
	inference_level_for_tell_and_ask_defaults(), one, _T, _NIL, _NIL,
	_known_true, kb_local_only_p).firstValue().car();
  }

// ---------------------- instance-of-p


/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  protected Values2 instance_of_p_internal
     (Node thing, Node the_class, InferenceLevel inference_level,
      TruthValue kb_local_only_p)
  { Values2 v = ask(Cons.list(__instance_of, thing, the_class), _T, 
		    inference_level, one, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values2(v.firstValue().car(), v.secondValue());
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
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(facet, slot, frame, value);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_value, facet, slot, frame, value);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    Values2 v = ask(pattern, _T, inference_level, one, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values2(v.firstValue().car(), v.secondValue());
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
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(slot, frame, value);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_value, slot, frame, value);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    Values2 v = ask(pattern, _T, inference_level, one, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values2(v.firstValue().car(), v.secondValue());
  }

// ---------------------- primitive-p


/**
 * Returns   iff   class  is a class whose definition is
 * primitive, and   otherwise.  For KRSs that do not distinguish
 * primitive from defined classes,   primitive-p 
 * must return   for all classes.
 */
  protected Node primitive_p_internal
     (Node the_class, TruthValue kb_local_only_p)
  { return
    ask(Cons.list(__primitive, the_class), _T, 
	inference_level_for_tell_and_ask_defaults(), one, _T, _NIL, _NIL,
	_known_true, kb_local_only_p).firstValue().car();
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
     (Node the_class, Node new_superclasses, TruthValue kb_local_only_p)
  { untell(Cons.list(__subclass_of, the_class, _value_var), the_class, 
	   _known_true, kb_local_only_p);
    Enumerator e = new_superclasses.enumerate_list();
    while(e.has_more_p()) {
      Node s = e.next();
      tell(Cons.list(__subclass_of, the_class, s), the_class, _known_true,
	   kb_local_only_p);
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
  protected void put_facet_values_internal
    (Node frame, Node slot, Node facet, Node values, SlotType slot_type,
     ValueSelector value_selector, TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(facet, slot, frame, _value_var);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_value, facet, slot, frame, 
			    _value_var);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(pattern, frame, value_selector, kb_local_only_p);
    Enumerator e = values.enumerate_list();
    while(e.has_more_p()) {
      Node value = e.next();
      Node fact = null;
      if (slot_type == _own) { fact = Cons.list(facet, slot, frame, value); }
      else if (slot_type == _template)
	{ fact = Cons.list(__template_facet_value, facet, slot, frame, value);}
      else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	   }
      tell(fact, frame, value_selector, kb_local_only_p);
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
  protected Node put_frame_name_internal
     (Node frame, Node new_name, TruthValue kb_local_only_p)
  { untell(Cons.list(_name, frame, _value_var),
	   frame, _known_true, kb_local_only_p);
    tell(Cons.list(_name, frame, new_name),
	 frame, _known_true, kb_local_only_p);
    return frame;
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
     (Node frame, Node name, TruthValue kb_local_only_p)
  { untell(Cons.list(_pretty_name, frame, _value_var),
	   frame, _known_true, kb_local_only_p);
    tell(Cons.list(_pretty_name, frame, name),
	 frame, _known_true, kb_local_only_p);
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
     (Node frame, Node new_types, TruthValue kb_local_only_p)
  { untell(Cons.list(__instance_of, frame, _value_var), frame, 
	   _known_true, kb_local_only_p);
    Enumerator e = new_types.enumerate_list();
    while(e.has_more_p()) {
      Node type = e.next();
      tell(Cons.list(__instance_of, frame, type), frame, _known_true,
	   kb_local_only_p);
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
  protected void put_slot_values_internal
     (Node frame, Node slot, Node values, SlotType slot_type,
      ValueSelector value_selector, TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(slot, frame, _value_var);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_value, slot, frame, 
			    _value_var);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(pattern, frame, value_selector, kb_local_only_p);
    Enumerator e = values.enumerate_list();
    while(e.has_more_p()) {
      Node value = e.next();
      Node fact = null;
      if (slot_type == _own) { fact = Cons.list(slot, frame, value); }
      else if (slot_type == _template)
	{ fact = Cons.list(__template_slot_value, slot, frame, value);}
      else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	   }
      tell(fact, frame, value_selector, kb_local_only_p);
    }
  }

// ---------------------- remove-class-superclass


/**
 * Removes   superclass-to-remove  from the superclasses of   class .
 * Returns no values.
 */
  protected void remove_class_superclass_internal
     (Node the_class, Node superclass_to_remove, TruthValue kb_local_only_p)
  { untell(Cons.list(__subclass_of, the_class, superclass_to_remove),
	   the_class, _known_true, kb_local_only_p);
  }

// ---------------------- remove-facet-value


/**
 * If   value  is currently a member of the set of direct values of
 * the specified facet, then   value  is removed from the values of
 * the facet.  Returns no values.
 */
  protected void remove_facet_value_internal
     (Node frame, Node slot, Node facet, Node value, Node test, 
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(facet, slot, frame, value); }
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_value, facet, slot, frame, value);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(pattern, frame, value_selector, kb_local_only_p);
  }

// ---------------------- remove-instance-type


/**
 * Removes   type-to-remove  from the types of   frame  -- that is,
 * makes   frame  no longer be an instance of   type-to-remove .
 * Returns no values.
 */
  protected void remove_instance_type_internal
     (Node frame, Node type_to_remove, TruthValue kb_local_only_p)
  { untell(Cons.list(__instance_of, frame, type_to_remove),
	   frame, _known_true, kb_local_only_p);
  }

// ---------------------- remove-local-facet-values


/**
 * Removes all direct values of   facet  in   slot  of   frame .
 * Returns no values.
 */
  protected void remove_local_facet_values_internal
     (Node frame, Node slot, Node facet, SlotType slot_type,
      ValueSelector value_selector, TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(facet, slot, frame, _value_var); }
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_value,
			    facet, slot, frame, _value_var);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(pattern, frame, value_selector, kb_local_only_p);
  }

// ---------------------- remove-local-slot-values


/**
 * Removes all direct values in   slot  of   frame .
 * Returns no values.
 */
  protected void remove_local_slot_values_internal
     (Node frame, Node slot, SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p) 
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(slot, frame, _value_var); }
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_value, slot, frame, _value_var);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(pattern, frame, value_selector, kb_local_only_p);
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
  protected void remove_slot_value_internal
     (Node frame, Node slot, Node value, Node test, SlotType slot_type,
      Node index, ValueSelector value_selector, TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(slot, frame, value); }
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_value, slot, frame, value);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(pattern, frame, value_selector, kb_local_only_p);
  }

// ---------------------- replace-facet-value


/**
 * If   old-value  is currently a member of the set of direct values of
 * the specified facet, then   old-value  is replaced by
 *   new-value  in the facet.  Returns no values.
 */
  protected void replace_facet_value_internal
     (Node frame, Node slot, Node facet, Node old_value, Node new_value,
      Node test, SlotType slot_type, ValueSelector value_selector, 
      TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(facet, slot, frame, old_value); }
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_value, facet, slot, frame, 
			    old_value);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(pattern, frame, value_selector, kb_local_only_p);
    Node fact = null;
    if (slot_type == _own) 
      { fact = Cons.list(facet, slot, frame, new_value); }
    else if (slot_type == _template)
      { fact = Cons.list(__template_facet_value, facet, slot, frame,
			 new_value);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    tell(fact, frame, value_selector, kb_local_only_p);
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
  protected void replace_slot_value_internal
     (Node frame, Node slot, Node old_value, Node new_value, Node test,
      SlotType slot_type, Node index, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(slot, frame, old_value); }
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_value, slot, frame, 
			    old_value);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    untell(pattern, frame, value_selector, kb_local_only_p);
    Node fact = null;
    if (slot_type == _own) 
      { fact = Cons.list(slot, frame, new_value); }
    else if (slot_type == _template)
      { fact = Cons.list(__template_slot_value, slot, frame, new_value);
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    tell(fact, frame, value_selector, kb_local_only_p);
  }

// ---------------------- slot-has-facet-p


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
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(__facet_of, facet, slot, frame);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_facet_of, facet, slot, frame); 
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    return ask(pattern, _T, inference_level, one, _T, _NIL, _NIL,
	       _known_true, kb_local_only_p).firstValue().car();
  }

// ---------------------- slot-has-value-p


/**
 * Returns   iff   slot  on   frame  has at least one
 * value, otherwise returns  .
 */
  protected Values2 slot_has_value_p_internal
     (Node frame, Node slot, InferenceLevel inference_level,
      SlotType slot_type, ValueSelector value_selector,
      TruthValue kb_local_only_p)
  { Node pattern = null;
    if (slot_type == _own) 
      { pattern = Cons.list(slot, frame, _value_var);}
    else if (slot_type == _template)
      { pattern = Cons.list(__template_slot_value, slot, frame, _value_var); 
      }
    else { throw new GenericError
	     (_T, new OKBCString("Illegal slot type: " + slot_type))
	     .signal();
	 }
    Values2 v = ask(pattern, _T, inference_level, one, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values2(v.firstValue().car(), v.secondValue());
  }

// ---------------------- slot-p


/**
 * Returns   iff   thing  is a slot, and
 * otherwise returns  .
 */
  protected Node slot_p_internal (Node thing, TruthValue kb_local_only_p)
  { return
    ask(Cons.list(_slot, thing), _T, 
	inference_level_for_tell_and_ask_defaults(), one, _T, _NIL, _NIL,
	_known_true, kb_local_only_p).firstValue().car();
  }

// ---------------------- subclass-of-p


/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
  protected Values2 subclass_of_p_internal
     (Node subclass, Node superclass, InferenceLevel inference_level,
      TruthValue kb_local_only_p)
  { Values2 v = ask(Cons.list(__subclass_of, subclass, superclass), _T, 
		    inference_level, one, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values2(v.firstValue().car(), v.secondValue());
  }

// ---------------------- superclass-of-p


/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
  protected Values2 superclass_of_p_internal
     (Node superclass, Node subclass, InferenceLevel inference_level,
      TruthValue kb_local_only_p)
  { Values2 v = ask(Cons.list(__subclass_of, subclass, superclass), _T, 
		    inference_level, one, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values2(v.firstValue().car(), v.secondValue());
  }

// ---------- default implementation of type-of-p


/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  protected Values2 type_of_p_internal
     (Node the_class, Node frame, InferenceLevel inference_level,
      TruthValue kb_local_only_p)
  { Values2 v = ask(Cons.list(__instance_of, frame, the_class), _T, 
		    inference_level, one, _T, _NIL, _NIL,
		    _known_true, kb_local_only_p);
    return new Values2(v.firstValue().car(), v.secondValue());
  }
}
