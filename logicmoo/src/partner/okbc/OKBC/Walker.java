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


class InPackageWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons) ||
	expression.length() != 2 ||
	!(Walker._in_package.stringEquals(expression.car())) ||
	!((expression.cadr() instanceof AbstractSymbol) ||
	  (expression.cadr() instanceof OKBCString)))
      { parser_warn(expression + " should be an in-package form.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      { new_expression = Cons.list(get_walker(_lisp_object).walk_subexpression
				   (kb, known_context_type, bound_vars,
				    expression.car(), function_to_apply),
				   get_walker(_lisp_object).walk_subexpression
				   (kb, known_context_type, bound_vars,
				    expression.cadr(), function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class LetWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons) ||
	expression.length() < 2 ||
	!(Walker._let.stringEquals(expression.car())) ||
	!(expression.cadr() instanceof Cons ||
	  expression.cadr() == Node._NIL))
      { parser_warn(expression + " should be a LET form.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node new_forms = Node._NIL;
	Cons tail = null;
	Enumerator e = expression.cdr().cdr().enumerate_list();
	while (e.has_more_p()) {
	  Node form = e.next();
	  Node this_res = get_walker(_okbc).walk_subexpression
	    (kb, known_context_type, bound_vars, form,
	     function_to_apply);
	  if (tail == null)
	    { tail = new Cons(this_res);
	      new_forms = tail;
	    }
	  else { Cons new_tail = new Cons(this_res);
		 tail.set_cdr(new_tail);
		 tail = new_tail;
	       }
	}
	new_expression =
	  new Cons(get_walker(_lisp_object).walk_subexpression
		   (kb, known_context_type, bound_vars,
		    expression.car(), function_to_apply),
		   new Cons(get_walker(_lisp_object).walk_subexpression
			    (kb, known_context_type, bound_vars,
			     expression.cadr(), function_to_apply),
			    new_forms));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class DefineOKBCFrameWalker extends AbstractSubExpressionWalker {

  static final Symbol _define_okbc_frame = Symbol.keyword("DEFINE-OKBC-FRAME");
  static final Symbol _template_slot_specs =
    Symbol.keyword("TEMPLATE-SLOT-SPECS");
  static final Symbol _template_facet_specs =
    Symbol.keyword("TEMPLATE-FACET-SPECS");
  
  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons) ||
	expression.length() < 2 ||
	!(Walker._define_okbc_frame.stringEquals(expression.car())))
      { parser_warn(expression + " should be a define-OKBC-frame form.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node name = expression.cadr();
	Node body = expression.cdr().cdr();
	Node type = Cons.getf(body, Node._frame_type);
	if (type == Node._NIL) { type = Cons.getf(body, Node._type); }
	Node direct_types = Cons.getf(body, Node._direct_types);
	Node direct_superclasses = Cons.getf(body, Node._direct_superclasses);
	Node template_slot_specs = Cons.getf(body, _template_slot_specs);
	Node template_facet_specs = Cons.getf(body, _template_facet_specs);
	Node template_slots = Cons.getf(body, Node._template_slots);
	Node template_facets = Cons.getf(body, Node._template_facets);

	Node key = _object_constant;
	if (type == Node._slot ||
	    type == Node._class ||
	    type == Node._facet ||
	    direct_superclasses != Node._NIL ||
	    template_slot_specs != Node._NIL ||
	    template_slots != Node._NIL ||
	    template_facet_specs != Node._NIL ||
	    template_facets != Node._NIL ||
	    Cons.member_eql(Node._class, direct_types, kb, Node._NIL))
	  { key = _relation_constant; }
	Node tail = walk_key_and_value_plist(kb, known_context_type, 
					     bound_vars, body, 
					     function_to_apply);
	new_expression =
	  Cons.append_cons
	  (Cons.list(get_walker(_lisp_object).walk_subexpression
		     (kb, known_context_type, bound_vars,
		      expression.car(), function_to_apply),
		     get_walker(key).walk_subexpression
		     (kb, type, bound_vars, name, function_to_apply)),
	   tail);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class OKBCSlotSpecWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " is not a legal slot spec.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node slot = expression.car();
	Node values = expression.cdr();
	if (values == null) { values = Node._NIL; }
	Node new_slot = get_walker(_relation_constant).walk_subexpression
	  (kb, Node._slot, bound_vars, slot, function_to_apply);
	Node new_values = walk_maybe_defaulted_values
	  (kb, known_context_type, bound_vars, values, function_to_apply);
	new_expression = new Cons(new_slot, new_values);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class FspecWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " is not a OKBC facet spec.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node facet = expression.car();
	Node values = expression.cdr();
	if (values == null) { values = Node._NIL; }
	if (values == null) { values = Node._NIL; }
	Node new_facet = get_walker(_relation_constant).walk_subexpression
	  (kb, Node._facet, bound_vars, facet, function_to_apply);
	Node new_values = walk_maybe_defaulted_values
	  (kb, known_context_type, bound_vars, values, function_to_apply);
	new_expression = new Cons(new_facet, new_values);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class OKBCFacetSpecWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " is not a legal facet spec.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node slot = expression.car();
	Node fspecs = expression.cdr();
	Node new_slot = get_walker(_relation_constant).walk_subexpression
	  (kb, Node._slot, bound_vars, slot, function_to_apply);
	Node new_fspecs = loop_collecting_walk_results 
	  (walker, kb, Walker._fspec, known_context_type, 
	   bound_vars, fspecs, function_to_apply);
	new_expression = new Cons(new_slot, new_fspecs);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class GenericPairWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply)
  {
    Node new_value = null;
    kif_walker.parser_warn("Unknown key/value pair " + key + " = " + value);
    new_value =
      kif_walker.get(AbstractSubExpressionWalker._lisp_object)
      .walk_subexpression
      (kb, known_context_type, bound_vars, value, function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }
}


class SlotSpecsPairWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply)
  {
    Node new_value = value;
    if (value instanceof Cons)
      {
	new_value = loop_collecting_walk_results 
	  (kb, AbstractKeyValueWalker._okbc_slot_spec, known_context_type, 
	   bound_vars, value, function_to_apply);
      }
    else
      {
	kif_walker.parser_warn("The form " + value +
		    " is not a legal list of OKBC slot specs.");
	new_value =
	  kif_walker.get(AbstractSubExpressionWalker._lisp_object)
	  .walk_subexpression
	  (kb, known_context_type, bound_vars, value, function_to_apply);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class SentencesWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply)
  {
    Node new_value = value;
    new_value =
      walk_list_of_sentences (kb, known_context_type, bound_vars, value,
			      function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

  Node walk_list_of_sentences(Kb kb, Node known_context_type, Node bound_vars,
			      Node value,
			      AbstractWalkFunction function_to_apply) {
    Node new_list = Node._NIL;
    if (value instanceof Cons)
      {
	if (value.car() instanceof Cons)
	  // Then we really have a list of sentences rather than just 
	  // one sentence.
	  {
	    new_list = loop_collecting_walk_results 
	      (kb, AbstractSubExpressionWalker._sentence, known_context_type, 
	       bound_vars, value, function_to_apply);
	  }
	else
	  {
	    new_list =
	      kif_walker.get(AbstractSubExpressionWalker._sentence)
	      .walk_subexpression
	      (kb, known_context_type, bound_vars, value, function_to_apply);
	  }
      }
    else if (value == Node._NIL) 
      { }
    else 
      {
	kif_walker.parser_warn("The form " + value +
		    " is not a legal sentence.");
	new_list =
	  kif_walker.get(AbstractSubExpressionWalker._lisp_object)
	  .walk_subexpression
	  (kb, known_context_type, bound_vars, value, function_to_apply);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_list);
  }

}


class OwnSlotsWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = key_and_value_walker.get
      (AbstractKeyValueWalker._slot_specs).walk_key_and_value_pair
      (kb, Node._own_slots, known_context_type, bound_vars, value,
       function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class TemplateSlotsWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = key_and_value_walker.get
      (AbstractKeyValueWalker._slot_specs).walk_key_and_value_pair
      (kb, Node._template_slots, known_context_type, bound_vars, value,
       function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class FacetSpecsPairWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply)
  {
    Node new_value = value;
    if (value instanceof Cons)
      {
	new_value = loop_collecting_walk_results 
	  (kb, Walker._okbc_facet_spec, known_context_type, 
	   bound_vars, value, function_to_apply);
      }
    else
      {
	kif_walker.parser_warn("The form " + value +
		    " is not a legal list of OKBC facet specs.");
	new_value =
	  kif_walker.get(AbstractSubExpressionWalker._lisp_object)
	  .walk_subexpression
	  (kb, known_context_type, bound_vars, value, function_to_apply);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class OwnFacetsWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = key_and_value_walker.get
      (Walker._facet_specs).walk_key_and_value_pair
      (kb, Node._own_facets, known_context_type, bound_vars, value,
       function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class TemplateFacetsWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = key_and_value_walker.get
      (Walker._facet_specs).walk_key_and_value_pair
      (kb, Node._template_facets, known_context_type, bound_vars, value,
       function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class TypeWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = Node._NIL;
    if (value == Node._class ||
	value == Node._NIL ||
	value == Node._slot ||
	value == Node._facet ||
	value == Node._individual)
      {}
    else 
      {
	kif_walker.parser_warn("The form " + value +
		    " is not a legal OKBC context type.");
      }
    new_value =
      kif_walker.get(AbstractSubExpressionWalker._lisp_object)
      .walk_subexpression
      (kb, known_context_type, bound_vars, value, function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class FrameTypeWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = Node._NIL;
    if (value == Node._class ||
	value == Node._NIL ||
	value == Node._slot ||
	value == Node._facet ||
	value == Node._individual)
      {}
    else 
      {
	kif_walker.parser_warn("The form " + value +
		    " is not a legal OKBC context type.");
      }
    new_value =
      kif_walker.get(AbstractSubExpressionWalker._lisp_object)
      .walk_subexpression
      (kb, known_context_type, bound_vars, value, function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class SimplePairWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = Node._NIL;
    new_value =
      kif_walker.get(AbstractSubExpressionWalker._lisp_object)
      .walk_subexpression
      (kb, known_context_type, bound_vars, value, function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class PrettyNameWalker extends SimplePairWalker { }
class HandleWalker extends SimplePairWalker { }
class PrimitivePWalker extends SimplePairWalker { }


class ClassListWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = Node._NIL;
    if (value == Node._NIL || 
	(value instanceof AbstractSymbol && 
	 AbstractSymbol.stringEquals((AbstractSymbol)value, "NIL")))
      {}
    else if (value instanceof Cons)
      {
	new_value = loop_collecting_walk_results 
	  (kb, AbstractSubExpressionWalker._relation_constant, Node._class, 
	   bound_vars, value, function_to_apply);
      }
    else
      { 
	kif_walker.parser_warn("The form " + value +
			       " should be a list of classes.");
	new_value =
	  kif_walker.get(AbstractSubExpressionWalker._lisp_object)
	  .walk_subexpression
	  (kb, known_context_type, bound_vars, value, function_to_apply);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class DirectTypesWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = key_and_value_walker.get
      (Walker._class_list).walk_key_and_value_pair
      (kb, Node._direct_types, known_context_type, bound_vars, value,
       function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class DirectSuperclassesWalker extends AbstractSubKeyValueWalker {

  public Node walk_key_and_value_pair_1 (Kb kb, Node key,
					 Node known_context_type,
					 Node bound_vars, Node value,
					 AbstractWalkFunction
					 function_to_apply) {
    Node new_value = key_and_value_walker.get
      (Walker._class_list).walk_key_and_value_pair
      (kb, Node._direct_superclasses, known_context_type, bound_vars, value,
       function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_value);
  }

}


class OKBCKeyValueWalker extends AbstractKeyValueWalker {

  OKBCKeyValueWalker (Walker the_walker) {
    kif_walker = the_walker;
  }
  
}



public class Walker extends AbstractKifWalker {

  public static final Symbol _in_package = Symbol.keyword("IN-PACKAGE");
  public static final Symbol _let = Symbol.keyword("LET");
  public static final Symbol _define_okbc_frame = 
    Symbol.keyword("DEFINE-OKBC-FRAME");
  public static final Symbol _okbc_slot_spec =
    Symbol.keyword("OKBC-SLOT-SPEC");
  public static final Symbol _okbc_facet_spec =
    Symbol.keyword("OKBC-FACET-SPEC");
  public static final Symbol _slot_specs = Symbol.keyword("SLOT-SPECS");
  public static final Symbol _facet_specs = Symbol.keyword("FACET-SPECS");
  public static final Symbol _fspec = Symbol.keyword("FSPEC");
  public static final Symbol _sentences = Symbol.keyword("SENTENCES");
  public static final Symbol _type = Symbol.keyword("TYPE");
  public static final Symbol _pretty_name = Symbol.keyword("PRETTY-NAME");
  public static final Symbol _handle = Symbol.keyword("HANDLE");
  public static final Symbol _primitive_p = Symbol.keyword("PRIMITIVE-P");
  public static final Symbol _direct_superclasses = 
    Symbol.keyword("DIRECT-SUPERCLASSES");
  public static final Symbol _direct_types = Symbol.keyword("DIRECT-TYPES");
  public static final Symbol _class_list = Symbol.keyword("CLASS-LIST");

  public void initialize_walker_table() {
    if (key_and_value_walker == null)
      { key_and_value_walker = new OKBCKeyValueWalker(this); }
    super.initialize_walker_table();
    add_walker(_in_package, new InPackageWalker());
    add_walker(_let, new LetWalker());
    add_walker(_define_okbc_frame, new DefineOKBCFrameWalker());
    add_walker(_okbc_slot_spec, new OKBCSlotSpecWalker());
    add_walker(_okbc_facet_spec, new OKBCFacetSpecWalker());
    add_walker(_fspec, new FspecWalker());
  }

  public void initialize_plist_walker_table() {
    super.initialize_plist_walker_table();
    key_and_value_walker.add_walker(Node._T, new GenericPairWalker());
    key_and_value_walker.add_walker(_slot_specs, new SlotSpecsPairWalker());
    key_and_value_walker.add_walker(_facet_specs, new FacetSpecsPairWalker());
    key_and_value_walker.add_walker(Node._own_slots, new OwnSlotsWalker());
    key_and_value_walker.add_walker(Node._own_slot_specs, new OwnSlotsWalker());
    key_and_value_walker.add_walker(Node._template_slots,
				    new TemplateSlotsWalker());
    key_and_value_walker.add_walker(Node._template_slot_specs,
				    new TemplateSlotsWalker());
    key_and_value_walker.add_walker(Node._own_facets, new OwnFacetsWalker());
    key_and_value_walker.add_walker(Node._own_facet_specs,
				    new OwnFacetsWalker());
    key_and_value_walker.add_walker(Node._template_facets,
				    new TemplateFacetsWalker());
    key_and_value_walker.add_walker(Node._template_facet_specs,
				    new TemplateFacetsWalker());
    key_and_value_walker.add_walker(_sentences, new SentencesWalker());
    key_and_value_walker.add_walker(_type, new FrameTypeWalker());
    key_and_value_walker.add_walker(_pretty_name, new PrettyNameWalker());
    key_and_value_walker.add_walker(_handle, new HandleWalker());
    key_and_value_walker.add_walker(_primitive_p, new PrimitivePWalker());
    key_and_value_walker.add_walker(_direct_superclasses,
				    new DirectSuperclassesWalker());
    key_and_value_walker.add_walker(Node.__subclass_of,
				    new DirectSuperclassesWalker());
    key_and_value_walker.add_walker(_direct_types, new DirectTypesWalker());
    key_and_value_walker.add_walker(Node.__instance_of,
				    new DirectTypesWalker());
    key_and_value_walker.add_walker(_class_list, new ClassListWalker());
  }

  public Node walk (Node expression,
		    AbstractWalkFunction function_to_apply,
		    Kb kb)
  {
    return walk(expression, function_to_apply, kb,
		AbstractSubExpressionWalker._kif);
  }
  
  public Node walk (Node expression,
		    AbstractWalkFunction function_to_apply,
		    Kb kb,
		    Node production_key)
  {
    return walk(expression, function_to_apply, kb, production_key,
		Node._NIL);
  }
  
  public Node walk (Node expression,
		    AbstractWalkFunction function_to_apply,
		    Kb kb,
		    Node production_key,
		    Node known_context_type)
  {
    return walk(expression, function_to_apply, kb, production_key,
		known_context_type, Node._NIL);
  }
  
  public Node walk (Node expression,
		    AbstractWalkFunction function_to_apply,
		    Kb kb,
		    Node production_key,
		    Node known_context_type,
		    Node bound_vars)
  {
    AbstractSubExpressionWalker subexpression_walker = get(production_key);
    depth = 0;
    return subexpression_walker.walk_subexpression
      (kb, known_context_type, bound_vars, expression, function_to_apply);
  }
  
  public static Node walk_expression (Node expression,
				      AbstractWalkFunction function_to_apply,
				      Kb kb)
  {
    return Walker.walk_expression(expression, function_to_apply, kb,
				  AbstractSubExpressionWalker._kif);
  }
  
  public static Node walk_expression (Node expression,
				      AbstractWalkFunction function_to_apply,
				      Kb kb,
				      Node production_key)
  {
    return Walker.walk_expression(expression, function_to_apply, kb,
				  production_key, Node._NIL);
  }
  
  public static Node walk_expression (Node expression,
				      AbstractWalkFunction function_to_apply,
				      Kb kb,
				      Node production_key,
				      Node known_context_type)
  {
    return Walker.walk_expression(expression, function_to_apply, kb,
				  production_key, known_context_type,
				  Node._NIL);
  }
  
  public static Node walk_expression (Node expression,
				      AbstractWalkFunction function_to_apply,
				      Kb kb,
				      Node production_key,
				      Node known_context_type,
				      Node bound_vars)
  {
    Walker walker = new Walker();
    return walker.walk(expression, function_to_apply, kb, production_key,
		       known_context_type, bound_vars);
  }

  
}
