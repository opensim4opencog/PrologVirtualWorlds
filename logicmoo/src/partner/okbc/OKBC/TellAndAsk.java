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

public class TellAndAsk extends AbstractTester {

  static final Symbol _frame_var = Symbol.intern("?FRAME", "VARS");
  static final Symbol _class_var = Symbol.intern("?CLASS", "VARS");
  static final Symbol _facet_var = Symbol.intern("?FACET", "VARS");
  static final Symbol _instance_var = Symbol.intern("?INSTANCE", "VARS");
  static final Symbol _individual_var = Symbol.intern("?INDIVIDUAL", "VARS");
  static final Symbol _slot_var = Symbol.intern("?SLOT", "VARS");
  static final Symbol _sub_var = Symbol.intern("?SUB", "VARS");
  static final Symbol _super_var = Symbol.intern("?SUPER", "VARS");
  static final Symbol _value_var = Symbol.intern("?VALUE", "VARS");
  static final Symbol _x_var = Symbol.intern("?X", "VARS");
  static final Symbol _y_var = Symbol.intern("?Y", "VARS");


  public static boolean okbc_sentence_p (Node sentence, Kb kb) {
    if ((sentence instanceof Cons) &&
	!(sentence.car() instanceof Cons) && // (atom (first sentence))
	!(Cons.member_eql(sentence.car(), kif_operator_symbols, null, null)) &&
	ground_p(sentence, kb))
      { int required_length = -1;
	Node key = sentence.car();
	if (key == __template_facet_value) { required_length = 5; }
	else if (key == __template_slot_value) { required_length = 4; }
	else if (key == __template_facet_of) { required_length = 4; }
	else if (key == __facet_of) { required_length = 4; }
	else if (key == __template_slot_of) { required_length = 3; }
	else if (key == __slot_of) { required_length = 3; }
	else if (key == __subclass_of) { required_length = 3; }
	else if (key == __superclass_of) { required_length = 3; }
	else if (key == __instance_of) { required_length = 3; }
	else if (key == __type_of) { required_length = 3; }
	else if (key == _individual) { required_length = 2; }
	else if (key == _frame) { required_length = 2; }
	else if (key == _class) { required_length = 2; }
	else if (key == _slot) { required_length = 2; }
	else if (key == _facet) { required_length = 2; }
	else if (key == __primitive) { required_length = 2; }
	if (required_length == -1)
	  { return sentence.length() <= 4; }
	else { return sentence.length() == required_length; }
      }
    else { return false; }
  }

  public static boolean ground_p (Node sentence, Kb kb) {
    return ground_p(sentence, kb, _NIL);
  }

  public static boolean ground_p (Node sentence, Kb kb, Node ground_vars) {
    return subsetp(Cons.toList(TupleStore.variables_in(sentence)),
		   ground_vars, kb);
  }

  public static boolean subsetp (Node list1, Node list2, Kb kb) {
    Enumeration e = list1.elements();
    while (e.hasMoreElements()) {
      Node value = (Node)(e.nextElement());
      if (!(Cons.member_eql(value, list2, kb, Node._NIL)))
	{ return false; }
    }
    return true;
  }

  public static Node dequote (Node thing) {
    if (thing instanceof Cons && thing.car() == __quote)
      { return thing.cadr(); }
    else { throw new GenericError
	   (_T, new OKBCString(thing + " is not a quoted structure.")).signal();
	 }
  }


  static Node find_sym_in_list (AbstractSymbol sym, Node list) {
    Enumerator e = list.enumerate_list();
    while (e.has_more_p()) {
      AbstractSymbol elt = (AbstractSymbol)e.next();
      if (elt.symbolName.equalsIgnoreCase(sym.symbolName))
	{ return elt; }
    }
    return _NIL;
  }


  static Node canonicalise_kif_ops_and_okbc_relations(Node thing) {
    if (thing instanceof Cons)
      { Node car = thing.car();
	if (car instanceof AbstractSymbol)
	  { AbstractSymbol sym = (AbstractSymbol)car;
	    if (sym.symbolName.equalsIgnoreCase("QUOTE"))
	      { return Cons.list(__quote, thing.cadr()); }
	    else { Node kif_or_okbc_entry = _NIL;
		   kif_or_okbc_entry = 
		     find_sym_in_list(sym, kif_operator_symbols);
		   if (kif_or_okbc_entry == _NIL)
		     { kif_or_okbc_entry = 
			 find_sym_in_list(sym, evaluable_predicate_symbols); 
		     }
		   if (kif_or_okbc_entry == _NIL)
		     { kif_or_okbc_entry = 
			 find_sym_in_list(sym, okbc_relation_symbols);
		     }
		   if (kif_or_okbc_entry == _NIL) { kif_or_okbc_entry = sym; }
		   Cons root = null;
		   Cons tail = null;
		   Node args = thing.cdr();
		   if (args != null)
		     { Enumerator e = args.enumerate_list();
		       while (e.has_more_p()) {
			 Node elt = 
			   canonicalise_kif_ops_and_okbc_relations(e.next());
			 if (tail == null)
			   { tail = new Cons(elt);
			     root = tail;
			   }
			 else { Cons new_tail = new Cons(elt);
				tail.set_cdr(new_tail);
				tail = new_tail;
			      }
		       }
		     }
		   if (root == null) { return Cons.list(kif_or_okbc_entry); }
		   else {return new Cons(kif_or_okbc_entry, root); }
		 }
	  }
	else { return new 
	       Cons(canonicalise_kif_ops_and_okbc_relations(car),
		    canonicalise_kif_ops_and_okbc_relations(thing.cdr()));
	     }
      }
    else { return thing; }
  }


  static void default_tell (Node sentence, Kb kb, Node frame, 
			    ValueSelector value_selector, 
			    TruthValue kb_local_only_p) {
    Node canonicalised = canonicalise_kif_ops_and_okbc_relations(sentence);
    handle_sentence_handling_conjunctions
      (canonicalised, kb, frame, value_selector, kb_local_only_p, _tell,
       new ConsLocation());
  }

  static Node default_untell (Node sentence, Kb kb, Node frame, 
			      ValueSelector value_selector,
			      TruthValue kb_local_only_p) {
    Node canonicalised = canonicalise_kif_ops_and_okbc_relations(sentence);
    return handle_sentence_handling_conjunctions
      (canonicalised, kb, frame, value_selector, kb_local_only_p, _untell,
       new ConsLocation());
  }

  static Node default_tellable (Node sentence, Kb kb, 
				ValueSelector value_selector,
				TruthValue kb_local_only_p){
    Node canonicalised = canonicalise_kif_ops_and_okbc_relations(sentence);
    return handle_sentence_handling_conjunctions
      (canonicalised, kb, _NIL, value_selector, kb_local_only_p,
       _tellable, new ConsLocation());
  }


  static Node handle_sentence_handling_conjunctions 
    (Node sentence, Kb kb, Node frame, ValueSelector value_selector,
     TruthValue kb_local_only_p, Node key, ConsLocation loc) 
  {
    if (sentence instanceof Cons)
      { Node car = sentence.car();
	if (car == _and)
	  { if (key == _tellable)
	      { loc = new ConsLocation(sentence.cdr());
		while (loc.cons != _NIL && loc.cons != null) {
		  Node next = loc.pop();
		  Node res = handle_sentence_handling_conjunctions
		    (next, kb, frame, value_selector, kb_local_only_p, key,
		     loc);
		  if (res == _NIL) { return _NIL; }
		}
		return _T;
	      }
	    else if (key == _untell)
	      { loc = new ConsLocation(sentence.cdr());
		Node removed_p = _T;
		while (loc.cons != _NIL && loc.cons != null) {
		  Node next = loc.pop();
		  Node res = handle_sentence_handling_conjunctions
		    (next, kb, frame, value_selector, kb_local_only_p, key,
		     loc);
		  if (res == _NIL) { removed_p = _NIL; }
		}
		return removed_p;
	      }
	    else if (key == _tell)
	      { loc = new ConsLocation(sentence.cdr());
		while (loc.cons != _NIL && loc.cons != null) {
		  Node next = loc.pop();
		  Node res = handle_sentence_handling_conjunctions
		    (next, kb, frame, value_selector, kb_local_only_p, key,
		     loc);
		}
		return _T;
	      }
	    return _NIL;
	  }
	else if (ground_p(sentence, kb))
	  { return handle_ground_sentence(sentence, kb, frame, value_selector,
					  kb_local_only_p, key, loc);
	  }
	else
	  { return kb.handle_unhandled_sentence
	    (sentence, frame, value_selector, kb_local_only_p, key);
	  }
      }
    else { return _T; } // drop it on the floor, it's just an atom.
      
  }

  static void substitute_new_instance
    (Node name, Node new_value, ConsLocation c) {
//      System.out.println("Before substitution of " + new_value  + " for " +
//			 name + " = " + c.cons);
      c.cons = subst_handling_quote(name, new_value, c.cons);
//      System.out.println("After substitution of " + new_value  + " for " +
//			 name + " = " + c.cons);
  }


  static Node subst_handling_quote (Node name, Node new_value, Node form) {
    if (name.equals(form)) { return new_value; }
    else { if (form instanceof Cons)
	     { if (form.car() == __quote) { return form; }
	       else { return new
		      Cons(subst_handling_quote(name, new_value, form.car()),
			   subst_handling_quote(name, new_value, form.cdr()));
		    }
	     }
           else { return form; }
	 }
  }


  static Node sublis_handling_quote (Node alist, Node form) {
    if (alist != _NIL && alist != null)
      { return sublis_handling_quote(alist.cdr(),
				     subst_handling_quote(alist.car().car(),
							  alist.car().cdr(),
							  form));
      }
    else { return form; }
  }


  static final Node subclass_of_Sub_Super = 
    Cons.list(__subclass_of, _sub_var, _super_var);
  static final Node superclass_of_Super_Sub = 
    Cons.list(__superclass_of, _super_var, _sub_var);
  static final Node instance_of_Frame_Class = 
    Cons.list(__instance_of, _frame_var, _class_var);
  static final Node type_of_Class_Frame = 
    Cons.list(__type_of, _class_var, _frame_var);

  static Node handle_subclass_or_instance_sentence 
    (Node sentence, Kb kb, Node frame, ValueSelector value_selector, 
     TruthValue kb_local_only_p, Node key, ConsLocation loc)
  {
    if (value_selector == _known_true)
      { Node schema_result = null;
	Node res = null;
	// Now to (:subclass-of ?sub ?super), (:superclass-of ?super ?sub)
	schema_result = TupleStore.unify_against_literal
	  (subclass_of_Sub_Super, sentence);
	if (schema_result == __fail ||
	    Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			    kb_local_only_p))
	  { schema_result = TupleStore.unify_against_literal
	    (superclass_of_Super_Sub, sentence);
	  }
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node sub = Cons.assoc(_sub_var, schema_result).cdr();
		Node sup = Cons.assoc(_super_var, schema_result).cdr();
		if (key == _tell)
		  { kb.add_class_superclass(sub, sup, kb_local_only_p); }
		else if (key == _untell)
		  { kb.remove_class_superclass(sub, sup, kb_local_only_p); }
		return _T;
	      }
	  }
	// Now to (:type-of ?class ?frame), (:instance-of ?frame ?class)
	schema_result = TupleStore.unify_against_literal
	  (type_of_Class_Frame, sentence);
	if (schema_result == __fail ||
	    Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			    kb_local_only_p))
	  { schema_result = TupleStore.unify_against_literal
	    (instance_of_Frame_Class, sentence);
	  }
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fra = Cons.assoc(_frame_var, schema_result).cdr();
		Node cla = Cons.assoc(_class_var, schema_result).cdr();
		if (key == _tell)
		  { kb.add_instance_type(fra, cla, kb_local_only_p); }
		else if (key == _untell)
		  { kb.remove_instance_type(fra, cla, kb_local_only_p); }
		return _T;
	      }
	  }
	return _NIL;
      }
    else if (key == _tellable) { return _NIL; }
    else { throw new CannotHandle(sentence); }
  }

  static final Node frame_Frame =
    Cons.list(_frame, _frame_var);
  static final Node class_Class =
    Cons.list(_class, _class_var);
  static final Node slot_Slot =
    Cons.list(_slot, _slot_var);
  static final Node facet_Facet =
    Cons.list(_facet, _facet_var);
  static final Node individual_Individual =
    Cons.list(_individual, _individual_var);

  static Node handle_creation_sentence 
    (Node sentence, Kb kb, Node frame , ValueSelector value_selector, 
     TruthValue kb_local_only_p, Node key, ConsLocation loc)
  {
    if (value_selector == _known_true)
      { Node schema_result = null;
	Node res = null;
	// Now to (:frame ?frame)
	schema_result = TupleStore.unify_against_literal
	  (frame_Frame, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fra = Cons.assoc(_frame_var, schema_result).cdr();
		if (key == _tell)
		  { if (kb.coercible_to_frame_p(fra, kb_local_only_p) == _NIL)
		      { substitute_new_instance
			(fra, kb.create_frame(fra, _individual, _NIL, _NIL,
					      _NIL, _NIL, _NIL, _NIL, _NIL,
					      _T, _NIL, _NIL, kb_local_only_p),
			 loc);
		      }
		  }
		else if (key == _untell)
		  { Values2 v = kb.coerce_to_frame(fra, _NIL, 
						   kb_local_only_p);
		    if (v.secondValue() != _NIL)
		      { kb.delete_frame(v.firstValue(), kb_local_only_p); }
		  }
		return _T;
	      }
	  }

	// Now to (:class ?class)
	schema_result = TupleStore.unify_against_literal
	  (class_Class, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node cla = Cons.assoc(_class_var, schema_result).cdr();
		if (key == _tell)
		  { if (kb.class_p(cla, kb_local_only_p) == _NIL)
		      { substitute_new_instance
			(cla, kb.create_class(cla, _NIL, _NIL, _T, _NIL, _NIL,
					      _NIL, _NIL, _NIL, _NIL, _NIL,
					      kb_local_only_p),
			 loc);
		      }
		  }
		else if (key == _untell)
		  { Values2 v = kb.coerce_to_frame(cla, _NIL, 
						   kb_local_only_p);
		    if (v.secondValue() != _NIL)
		      { kb.delete_frame(v.firstValue(), kb_local_only_p); }
		  }
		return _T;
	      }
	  }

	// Now to (:slot ?slot)
	schema_result = TupleStore.unify_against_literal
	  (slot_Slot, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node slo = Cons.assoc(_slot_var, schema_result).cdr();
		if (key == _tell)
		  { if (kb.slot_p(slo, kb_local_only_p)
			== _NIL)
		      { substitute_new_instance
			(slo, kb.create_slot(slo, _NIL, _own, _NIL, 
					     _NIL, _NIL, _NIL, _NIL, _NIL,
					     kb_local_only_p),
			 loc);
		      }
		  }
		else if (key == _untell)
		  { if (kb.slot_p(slo, kb_local_only_p)
			!= _NIL)
		      { kb.delete_slot(slo, kb_local_only_p); }
		  }
		return _T;
	      }
	  }

	// Now to (:facet ?facet)
	schema_result = TupleStore.unify_against_literal
	  (facet_Facet, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fac = Cons.assoc(_facet_var, schema_result).cdr();
		if (key == _tell)
		  { if (kb.facet_p(fac, kb_local_only_p)
			== _NIL)
		      { substitute_new_instance
			(fac, kb.create_facet(fac, _NIL, _NIL, _own, _NIL,
					      _NIL, _NIL, _NIL, _NIL, _NIL,
					      kb_local_only_p),
			 loc);
		      }
		  }
		else if (key == _untell)
		  { if (kb.facet_p(fac, kb_local_only_p)
			!= _NIL)
		      { kb.delete_facet(fac, kb_local_only_p); }
		  }
		return _T;
	      }
	  }

	// Now to (:individual ?individual)
	schema_result = TupleStore.unify_against_literal
	  (individual_Individual, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node ind = Cons.assoc(_individual_var, schema_result).cdr();
		if (key == _tell)
		  { if (kb.coercible_to_frame_p(ind, kb_local_only_p) == _NIL
			|| kb.individual_p(ind, kb_local_only_p) == _NIL)
		      { substitute_new_instance
			(ind, kb.create_individual(ind, _NIL, _NIL, _NIL,
						   _NIL, _NIL, _NIL,
						   kb_local_only_p),
			 loc);
		      }
		  }
		else if (key == _untell)
		  { Values2 v = kb.coerce_to_frame(ind, _NIL, 
						   kb_local_only_p);
		    if (v.secondValue() != _NIL)
		      { kb.delete_frame(v.firstValue(), kb_local_only_p); }
		  }
		return _T;
	      }
	  }
	return _NIL;
      }
    else if (key == _tellable) { return _NIL; }
    else { throw new CannotHandle(sentence); }
  }


  static final Node template_facet_of_Facet_Slot_Class =
    Cons.list(__template_facet_of, _facet_var, _slot_var, _class_var);
  static final Node template_slot_of_Slot_Class =
    Cons.list(__template_slot_of, _slot_var, _class_var);
  static final Node template_facet_value_Facet_Slot_Class_Value =
    Cons.list(__template_facet_value, _facet_var, _slot_var, _class_var,
	      _value_var);
  static final Node template_slot_value_Slot_Class_Value =
    Cons.list(__template_slot_value, _slot_var, _class_var, _value_var);

  static Node handle_template_slot_sentence 
    (Node sentence, Kb kb, Node frame, ValueSelector value_selector, 
     TruthValue kb_local_only_p, Node key, ConsLocation loc)
  {
    if (value_selector == _known_true)
      { Node schema_result = null;
	Node res = null;
	// Now to (:template-facet-of ?facet ?slot ?class)
	schema_result = TupleStore.unify_against_literal
	  (template_facet_of_Facet_Slot_Class, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node cla = Cons.assoc(_class_var, schema_result).cdr();
		Node slo = Cons.assoc(_slot_var,  schema_result).cdr();
		Node fac = Cons.assoc(_facet_var, schema_result).cdr();
		if (key == _tell)
		  { kb.attach_facet(cla, slo, fac, _template, kb_local_only_p);
		  }
		else if (key == _untell)
		  { kb.detach_facet(cla, slo, fac, _template, kb_local_only_p);
		  }
		return _T;
	      }
	  }

	// Now to (:template-slot-of ?slot ?class)
	schema_result = TupleStore.unify_against_literal
	  (template_slot_of_Slot_Class, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node cla = Cons.assoc(_class_var, schema_result).cdr();
		Node slo = Cons.assoc(_slot_var,  schema_result).cdr();
		if (key == _tell)
		  { kb.attach_slot(cla, slo, _template, kb_local_only_p);
		  }
		else if (key == _untell)
		  { kb.detach_slot(cla, slo, _template, kb_local_only_p);
		  }
		return _T;
	      }
	  }

	// Now to (:template-facet-value ?facet ?slot ?class ?value)
	schema_result = TupleStore.unify_against_literal
	  (template_facet_value_Facet_Slot_Class_Value, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node cla = Cons.assoc(_class_var, schema_result).cdr();
		Node slo = Cons.assoc(_slot_var,  schema_result).cdr();
		Node fac = Cons.assoc(_facet_var, schema_result).cdr();
		Node val = Cons.assoc(_value_var, schema_result).cdr();
		if (key == _tell)
		  { kb.add_facet_value(cla, slo, fac, val, _equal, _template,
				       value_selector, kb_local_only_p);
		  }
		else if (key == _untell)
		  { if (TupleStore.variable_p(val))
		      { kb.remove_local_facet_values
			(cla, slo, fac, _template, value_selector, 
			 kb_local_only_p);
		      }
		    else
		      { kb.remove_facet_value
			(cla, slo, fac, val, _equal, _template, value_selector,
			 kb_local_only_p);
		      }
		  }
		return _T;
	      }
	  }

	// Now to (:template-slot-value ?slot ?class ?value)
	schema_result = TupleStore.unify_against_literal
	  (template_slot_value_Slot_Class_Value, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node cla = Cons.assoc(_class_var, schema_result).cdr();
		Node slo = Cons.assoc(_slot_var,  schema_result).cdr();
		Node val = Cons.assoc(_value_var, schema_result).cdr();
		if (key == _tell)
		  { kb.add_slot_value(cla, slo, val, _equal, _template,
				      new OKBCInteger(0), value_selector,
				      kb_local_only_p);
		  }
		else if (key == _untell)
		  { if (TupleStore.variable_p(val))
		      { kb.remove_local_slot_values
			(cla, slo, _template, value_selector, kb_local_only_p);
		      }
		    else
		      { kb.remove_slot_value
			(cla, slo, val, _equal, _template, _all,
			 value_selector, kb_local_only_p);
		      }
		  }
		return _T;
	      }
	  }
	return _NIL;
      }
    else if (key == _tellable) { return _NIL; }
    else { throw new CannotHandle(sentence); }
  }


  static final Node facet_of_Facet_Slot_Frame =
    Cons.list(__facet_of, _facet_var, _slot_var, _frame_var);
  static final Node slot_of_Slot_Frame =
    Cons.list(__slot_of, _slot_var, _frame_var);
  static final Node facet_value_Facet_Slot_Frame_Value =
    Cons.list(_facet_var, _slot_var, _frame_var, _value_var);
  static final Node slot_value_Slot_Frame_Value =
    Cons.list(_slot_var, _frame_var, _value_var);
  static final Node name_Frame_Value =
    Cons.list(__name, _frame_var, _value_var);
  static final Node pretty_name_Frame_Value =
    Cons.list(__pretty_name, _frame_var, _value_var);

  static Node handle_own_slot_sentence 
    (Node sentence, Kb kb, Node frame, ValueSelector value_selector, 
     TruthValue kb_local_only_p, Node key, ConsLocation loc)
  {
    if (value_selector == _known_true)
      { Node schema_result = null;
	Node res = null;
	// Now to (:name ?frame ?value)
	schema_result = TupleStore.unify_against_literal
	  (name_Frame_Value, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fra = Cons.assoc(_frame_var, schema_result).cdr();
		Node val = Cons.assoc(_value_var, schema_result).cdr();
		if (key == _tell)
		  { kb.put_frame_name(fra, val, kb_local_only_p); }
		else if (key == _untell)
		  { if (Cons.member_eql(_NIL, kb.get_behavior_values
					(_frame_names_required), kb,
					kb_local_only_p))
		      { kb.put_frame_name
			(fra, _NIL, kb_local_only_p);
		      }
		    else
		      { throw new CannotHandle(sentence); }
		  }
		return _T;
	      }
	  }

	// Now to (:pretty-name ?frame ?value)
	schema_result = TupleStore.unify_against_literal
	  (pretty_name_Frame_Value, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fra = Cons.assoc(_frame_var, schema_result).cdr();
		Node val = Cons.assoc(_value_var, schema_result).cdr();
		if (key == _tell)
		  { kb.put_frame_pretty_name(fra, val, kb_local_only_p); }
		else if (key == _untell)
		  { throw new CannotHandle(sentence); }
		return _T;
	      }
	  }

	// Now to (:facet-of ?facet ?slot ?frame)
	schema_result = TupleStore.unify_against_literal
	  (facet_of_Facet_Slot_Frame, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fra = Cons.assoc(_frame_var, schema_result).cdr();
		Node slo = Cons.assoc(_slot_var,  schema_result).cdr();
		Node fac = Cons.assoc(_facet_var, schema_result).cdr();
		if (key == _tell)
		  { kb.attach_facet(fra, slo, fac, _own, kb_local_only_p);
		  }
		else if (key == _untell)
		  { kb.detach_facet(fra, slo, fac, _own, kb_local_only_p);
		  }
		return _T;
	      }
	  }

	// Now to (:slot-of ?slot ?frame)
	schema_result = TupleStore.unify_against_literal
	  (slot_of_Slot_Frame, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fra = Cons.assoc(_frame_var, schema_result).cdr();
		Node slo = Cons.assoc(_slot_var,  schema_result).cdr();
		if (key == _tell)
		  { kb.attach_slot(fra, slo, _own, kb_local_only_p);
		  }
		else if (key == _untell)
		  { kb.detach_slot(fra, slo, _own, kb_local_only_p);
		  }
		return _T;
	      }
	  }

	// Now to (?facet ?slot ?frame ?value)
	schema_result = TupleStore.unify_against_literal
	  (facet_value_Facet_Slot_Frame_Value, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fra = Cons.assoc(_frame_var, schema_result).cdr();
		Node slo = Cons.assoc(_slot_var,  schema_result).cdr();
		Node fac = Cons.assoc(_facet_var, schema_result).cdr();
		Node val = Cons.assoc(_value_var, schema_result).cdr();
		if (key == _tell)
		  { kb.add_facet_value(fra, slo, fac, val, _equal, _own,
				       value_selector, kb_local_only_p);
		  }
		else if (key == _untell)
		  { if (TupleStore.variable_p(val))
		      { kb.remove_local_facet_values
			(fra, slo, fac, _own, value_selector, 
			 kb_local_only_p);
		      }
		    else
		      { kb.remove_facet_value
			(fra, slo, fac, val, _equal, _own, value_selector,
			 kb_local_only_p);
		      }
		  }
		return _T;
	      }
	  }

	// Now to (?slot ?frame ?value)
	schema_result = TupleStore.unify_against_literal
	  (slot_value_Slot_Frame_Value, sentence);
	if (schema_result != __fail &&
	    !(Cons.member_eql(sentence.car(), kif_operator_symbols, kb,
			      kb_local_only_p)))
	  { if (key == _tellable) { return _T; }
	    else 
	      { Node fra = Cons.assoc(_frame_var, schema_result).cdr();
		Node slo = Cons.assoc(_slot_var,  schema_result).cdr();
		Node val = Cons.assoc(_value_var, schema_result).cdr();
		if (key == _tell)
		  { kb.add_slot_value(fra, slo, val, _equal, _own,
				      new OKBCInteger(0), value_selector,
				      kb_local_only_p);
		  }
		else if (key == _untell)
		  { if (TupleStore.variable_p(val))
		      { kb.remove_local_slot_values
			(fra, slo, _own, value_selector, kb_local_only_p);
		      }
		    else
		      { kb.remove_slot_value
			(fra, slo, val, _equal, _own, _all,
			 value_selector, kb_local_only_p);
		      }
		  }
		return _T;
	      }
	  }
	return _NIL;
      }
    else if (key == _tellable) { return _NIL; }
    else { throw new CannotHandle(sentence); }
  }


  static Node handle_ground_sentence
     (Node sentence, Kb kb, Node frame, ValueSelector value_selector,
      TruthValue kb_local_only_p, Node key, ConsLocation loc)
  { if (handle_subclass_or_instance_sentence
	(sentence, kb, frame, value_selector, kb_local_only_p, key, loc)!=_NIL)
      { return _T; }
    else if (handle_creation_sentence
	(sentence, kb, frame, value_selector, kb_local_only_p, key, loc)!=_NIL)
      { return _T; }
    else if (handle_template_slot_sentence
	(sentence, kb, frame, value_selector, kb_local_only_p, key, loc)!=_NIL)
      { return _T; }
    else if (handle_own_slot_sentence
	(sentence, kb, frame, value_selector, kb_local_only_p, key, loc)!=_NIL)
      { return _T; }
    else { return kb.handle_unhandled_sentence
	   (sentence, frame, value_selector, kb_local_only_p, key);
	 }
  }


// ---------------------------------------------------------------------------
// Now define ASK.


  static Node default_askable
     (Node sentence, Kb kb, ValueSelector value_selector, 
      TruthValue kb_local_only_p)
  { return default_tellable(sentence, kb, value_selector, kb_local_only_p); }


  static Values3 default_ask
     (Node query, Kb kb, Node reply_pattern, InferenceLevel inference_level,
      Node number_of_values, TruthValue error_p, Node where, Node timeout,
      ValueSelector value_selector, TruthValue kb_local_only_p) 
  { Node dotted_where = _NIL;
    Enumerator e = where.enumerate_list();
    while (e.has_more_p()) {
      Node entry = e.next();
      dotted_where = new Cons(new Cons(entry.car(), entry.cadr()), 
			      dotted_where);
    }
    Node canonicalised = canonicalise_kif_ops_and_okbc_relations
      (Cons.sublis(dotted_where, query, kb));
    ConsLocation all_binding_lists = new ConsLocation();
    int numvals = 1000000;
    if (number_of_values != _all)
      { numvals = (int)(((OKBCInteger)number_of_values).number); }
    Values3 vals = handle_ask_handling_conjunctions
      (canonicalised, kb, inference_level, numvals, error_p,
       value_selector, kb_local_only_p, all_binding_lists);
    Node res = _NIL;
    e = all_binding_lists.cons.enumerate_list();
    while (e.has_more_p()) {
      Node b = e.next();
      res = new Cons(Cons.sublis(b, reply_pattern, kb), res);
    }
    return new Values3(res, vals.secondValue(),
		       (vals.firstValue() == _timeout 
			? (Node)_timeout
			: vals.thirdValue()));
  }


  static Values3 handle_ask_handling_conjunctions
      (Node canonicalised, Kb kb, InferenceLevel inference_level,
       int number_of_values, TruthValue error_p,
       ValueSelector value_selector, TruthValue kb_local_only_p,
       ConsLocation all_binding_lists)
  { if (number_of_values != 1000000)
      { Values2 vals = null;
	try { vals = kb.handle_unhandled_query(canonicalised, inference_level, 
					       error_p, value_selector, 
					       kb_local_only_p, _NIL);
	    }
	catch (CannotHandle ex) { vals = new Values2(_NIL, _T); }
	Node binding_lists = vals.firstValue();
	Node exact_p = vals.secondValue();
	if (binding_lists instanceof Cons && 
	    (binding_lists.length() >= number_of_values))
	  { Enumerator bliste = binding_lists.enumerate_list();
	    int count = 0;
	    while (bliste.has_more_p()) {
	      Node blist = bliste.next();
	      all_binding_lists.addElement(blist);
	      count = count + 1;
	      if (count >= number_of_values) { break; }
	    }
	    int remaining = 0;
	    while (bliste.has_more_p()) {
	      bliste.next();
	      remaining = remaining + 1;
	    }
	    return new Values3(_NIL, exact_p, 
			       (remaining == 0
				? (Node)_NIL
				: (Node)new OKBCInteger(remaining)));
	  }
	else { return full_ask_handling_conjunctions
	       (canonicalised, kb, inference_level, number_of_values, error_p,
		value_selector, kb_local_only_p, all_binding_lists);
	     }
      }
    else
      { return full_ask_handling_conjunctions
	(canonicalised, kb, inference_level, number_of_values, error_p,
	 value_selector, kb_local_only_p, all_binding_lists);
      }
  }


  static Values3 full_ask_handling_conjunctions
	(Node canonicalised, Kb kb, InferenceLevel inference_level,
	 int number_of_values, TruthValue error_p,
	 ValueSelector value_selector, TruthValue kb_local_only_p, 
	 ConsLocation all_binding_lists)
  { Values2 vals = handle_ask_handling_conjunctions_internal
    (canonicalised, kb, inference_level, error_p, value_selector,
     kb_local_only_p, _NIL, all_binding_lists);
    Node binding_lists = vals.firstValue();
    Node exact_p = vals.secondValue();
    if (binding_lists == __fail)
      { return new Values3(_NIL, vals.secondValue(), _NIL); }
    else
      { Enumerator bliste = binding_lists.enumerate_list();
	int count = 0;
	while (bliste.has_more_p()) {
	  Node blist = bliste.next();
	  all_binding_lists.addElement(blist);
	  count = count + 1;
	  if (count >= number_of_values) { break; }
	}
	int remaining = 0;
	while (bliste.has_more_p()) {
	  bliste.next();
	  remaining = remaining + 1;
	}
	return new Values3(_NIL, exact_p, 
			   (remaining == 0
			    ? (Node)_NIL
			    : (Node)new OKBCInteger(remaining)));
      }
  }


  static Values2 handle_ask_handling_conjunctions_internal
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, 
	 ConsLocation all_binding_lists)
  { if (query instanceof Cons)
      { Node car = query.car();
	if (car == _and)
	  { return handle_ask_conjunction(query.cdr(), kb, inference_level, 
					  error_p, value_selector,
					  kb_local_only_p, bindings,
					  all_binding_lists);
	  }
	else { return handle_simple_query(Cons.sublis(bindings, query, kb),
					  kb, inference_level, error_p,
					  value_selector, kb_local_only_p,
					  bindings, all_binding_lists);
	     }
      }
    else { return new Values2(bindings, _T); }
  }

  static Values2 handle_ask_conjunction
	(Node args, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, 
	 ConsLocation all_binding_lists)
  { Values2 ret_val = null;
    if (args != _NIL && args != null)
      { boolean inexact_p = false;
	Values2 vals = handle_ask_handling_conjunctions_internal
	  (args.car(), kb, inference_level, error_p, value_selector,
	   kb_local_only_p, bindings, all_binding_lists);
	if (vals.secondValue() == _NIL) { inexact_p = true; }
	Node result = vals.firstValue();
	// System.out.println("Partial result" +
	// Cons.list(args.car(), kb, inference_level, error_p,
	// value_selector, kb_local_only_p, bindings) +
	// " -> " + result);
	if (result != __fail)
	  { Node tail = args.cdr();
	    if (tail != _NIL && tail != null)
	      { Node results = _NIL;
		Enumerator e = result.enumerate_list();
		while (e.has_more_p()) {
		  Node blist = e.next();
		  Values2 blist_vals = handle_ask_conjunction
		    (tail, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, blist, all_binding_lists);
		  if (blist_vals.secondValue() == _NIL) { inexact_p = true; }
		  Node result_from_tail = blist_vals.firstValue();
		  if (result_from_tail != __fail)
		    { results = Cons.append_cons(result_from_tail, results); }
		}
		if (results != _NIL)
		  { ret_val = new Values2
		      (results, (inexact_p ? (Node)_NIL : (Node)_T)); 
		  }
		else 
		  { ret_val = new Values2
		      (__fail, (inexact_p ? (Node)_NIL : (Node)_T)); 
		  }
	      }
	    else { ret_val = new Values2(result,
					 (inexact_p ? (Node)_NIL : (Node)_T)); 
		 }
	  }
	else 
	  { ret_val =
	      new Values2(__fail, (inexact_p ? (Node)_NIL : (Node)_T)); }
	//	    System.out.println("Handle_ask_conjunction" +
	// Cons.list(args, kb, inference_level, error_p,
	// value_selector, kb_local_only_p, bindings) +
	// " -> " + ret_val);
      }
    else { ret_val = new Values2(Cons.list(bindings), _T); }
    return ret_val;
  }



  static final Node is_X_Y = Cons.list(_is, _x_var, _y_var);
  static final Node equal_X_Y = Cons.list(_equals_sign, _x_var, _y_var);
  static final Node not_equal_X_Y = Cons.list(_not_equal, _x_var, _y_var);
  static final Node less_than_X_Y = Cons.list(_less_than, _x_var, _y_var);
  static final Node greater_than_X_Y = 
    Cons.list(_greater_than, _x_var, _y_var);
  static final Node less_than_or_equal_X_Y =
    Cons.list(_less_than_or_equal, _x_var, _y_var);
  static final Node greater_than_or_equal_X_Y = 
    Cons.list(_greater_than_or_equal, _x_var, _y_var);

  static Values3 handle_evaluable_predicate_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings)
  { Values3 vals = null;
    Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;

    if (TupleStore.variable_p(query.car()))
      // Do not allow variable in the CAR.
      { return new Values3(__fail, _T, _T); }

    // ---- Handle `(:is ?x ?y)
    pattern = is_X_Y;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node x = Cons.assoc(_x_var, ask_result).cdr();
	Node y = Cons.assoc(_y_var, ask_result).cdr();
	if (TupleStore.variable_p(y))
	  { throw new GenericError
	   (_T, new OKBCString("The second argument to a :IS sentence " +
			       "cannot be a variable: " + y)).signal();
	  }
	else
	  {
	    Node expression_value = 
	      new Evaluator().trivial_eval_expression
	      (y, new EvalContext(kb), 0, kb);
	    if (TupleStore.variable_p(x))
	      { return new Values3(augment_bindings
				   (x, Cons.list(expression_value), bindings),
				   _T, _NIL); 
	      } // augment x
	    else if (kb.equal_in_kb(x, expression_value, kb_local_only_p)
		     != _NIL)
	      { return new Values3(Cons.list(bindings), _T, _NIL); }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
      }
    
    // ---- Handle `(:= ?x ?y)
    pattern = equal_X_Y;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node x = Cons.assoc(_x_var, ask_result).cdr();
	Node y = Cons.assoc(_y_var, ask_result).cdr();
	if (TupleStore.variable_p(x))
	  { if (TupleStore.variable_p(y))
	      { return new Values3(__fail, _T, _NIL); }
	    else { return new Values3(augment_bindings(x, Cons.list(y),
						       bindings),
				   _T, _NIL); 
		 } // augment x
	  }
	else if (TupleStore.variable_p(y))
	  { return new Values3(augment_bindings(x, Cons.list(y), bindings),
			       _T, _NIL); 
	  } // augment y
	else if (kb.equal_in_kb(x, y, kb_local_only_p) != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(/= ?x ?y)
    pattern = not_equal_X_Y;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node x = Cons.assoc(_x_var, ask_result).cdr();
	Node y = Cons.assoc(_y_var, ask_result).cdr();
	if (TupleStore.variable_p(x) || TupleStore.variable_p(y))
	  { return new Values3(__fail, _T, _NIL); }
	else if (kb.equal_in_kb(x, y, kb_local_only_p) != _NIL)
	  { return new Values3(__fail, _T, _NIL); }
	else { return new Values3(Cons.list(bindings), _T, _NIL); }
      }
    
    // ---- Handle `(:< ?x ?y)
    pattern = less_than_X_Y;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node x = Cons.assoc(_x_var, ask_result).cdr();
	Node y = Cons.assoc(_y_var, ask_result).cdr();
	if (TupleStore.variable_p(x) || TupleStore.variable_p(y))
	  { return new Values3(__fail, _T, _NIL); }
	else if ((x instanceof OKBCInteger || x instanceof Real) &&
		 (y instanceof OKBCInteger || y instanceof Real) &&
		 generalised_less_than(x, y))
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(:> ?x ?y)
    pattern = greater_than_X_Y;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node x = Cons.assoc(_x_var, ask_result).cdr();
	Node y = Cons.assoc(_y_var, ask_result).cdr();
	if (TupleStore.variable_p(x) || TupleStore.variable_p(y))
	  { return new Values3(__fail, _T, _NIL); }
	else if ((x instanceof OKBCInteger || x instanceof Real) &&
		 (y instanceof OKBCInteger || y instanceof Real) &&
		 generalised_less_than(y, x))
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(:>= ?x ?y)
    pattern = greater_than_or_equal_X_Y;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node x = Cons.assoc(_x_var, ask_result).cdr();
	Node y = Cons.assoc(_y_var, ask_result).cdr();
	if (TupleStore.variable_p(x) || TupleStore.variable_p(y))
	  { return new Values3(__fail, _T, _NIL); }
	else if ((x instanceof OKBCInteger || x instanceof Real) &&
		 (y instanceof OKBCInteger || y instanceof Real) &&
		 !(generalised_less_than(x, y)))
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(:<= ?x ?y)
    pattern = less_than_or_equal_X_Y;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node x = Cons.assoc(_x_var, ask_result).cdr();
	Node y = Cons.assoc(_y_var, ask_result).cdr();
	if (TupleStore.variable_p(x) || TupleStore.variable_p(y))
	  { return new Values3(__fail, _T, _NIL); }
	else if ((x instanceof OKBCInteger || x instanceof Real) &&
		 (y instanceof OKBCInteger || y instanceof Real) &&
		 !(generalised_less_than(y, x)))
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(p ?x ?y), where p is a procedure
    if (query.car() instanceof Procedure)
      { Node proc = query.car();
	Node args = query.cdr();
	Node remainder = args;
	boolean var_found_p = false;
	while (remainder != Node._NIL && remainder != null) {
	  Node arg = remainder.car();
	  if (TupleStore.variable_p(arg))
	    { var_found_p = true; break; }
	  remainder = remainder.cdr();
	}
	if (!var_found_p)
	  {
	    // The fully bound case returns just t or nil.
	    if (kb.call_procedure_internal(proc, args) != Node._NIL)
	      { return new Values3(Cons.list(bindings), _T, _NIL); }
	    { return new Values3(__fail, _T, _T); }
	  }
	else
	  {
	    Node results = kb.call_procedure_internal(proc, args);
	    // System.out.println("Procedure called with " + args + " -> " + results);
	    if (results == __fail)
	      { return new Values3(__fail, _T, _NIL); }
	    else
	      {
		Enumerator e = results.enumerate_list();
		Node ret_val = Node._NIL;
		while (e.has_more_p()) {
		  Node binding_set = e.next();
		  Cons consified = null;
		  Cons tail = null;
		  Enumerator b = binding_set.enumerate_list();
		  while (b.has_more_p()) {
		    Node spec = b.next();
		    Node var = spec.car();
		    Node val = spec.cadr();
		    Cons pair = new Cons(var, val);
		    if (tail == null)
		      { consified = (Cons)(Cons.list(pair));
			tail = consified;
		      }
		    else
		      { Cons new_tail = new Cons(pair);
			tail.cdr = new_tail;
			tail = new_tail;
		      }
		    // System.out.println("Current = " + consified);
		  }
		  // tail must be bound!
		  if (bindings != Node._NIL)
		    { tail.cdr = bindings; } // nconc on the existing bindings
		  // System.out.println("Consified = " + consified);
		  // Now collect the results
		  ret_val = new Cons(consified, ret_val);
		}
		// System.out.println("Ret_val = " + ret_val);
		return new Values3(ret_val, _T, _NIL);
	      }
	  }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }


  static Node augment_bindings(Node var, Node list, Node bindings) {
    Enumerator liste = list.enumerate_list();
    Node result = _NIL;
    while (liste.has_more_p()) {
      Node value = liste.next();
      Node spec = new Cons(new Cons(var, value), bindings);
      result = new Cons(spec, result); // collect
    }
    return result;
  }


  static Node add_pair_to_bindings(Node pair, Node list_of_blists) {
    Enumerator bliste = list_of_blists.enumerate_list();
    Node result = _NIL;
    while (bliste.has_more_p()) {
      Node blist = bliste.next();
      Node spec = new Cons(pair, blist);
      result = new Cons(spec, result); // collect
    }
    return result;
  }


  static final Node      _frame_Frame = Cons.list(_frame,      _frame_var);
  static final Node      _class_Frame = Cons.list(_class,      _frame_var);
  static final Node       _slot_Frame = Cons.list(_slot,       _frame_var);
  static final Node      _facet_Frame = Cons.list(_facet,      _frame_var);
  static final Node _individual_Frame = Cons.list(_individual, _frame_var);

  static Values3 handle_frame_type_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings)
  { Values3 vals = null;
    Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;

    if (TupleStore.variable_p(query.car()))
      // Do not allow variable in the CAR.
      { return new Values3(__fail, _T, _T); }

    // ---- Handle `(:frame ?x)
    pattern = _frame_Frame;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node all = kb.get_kb_frames(kb_local_only_p);
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fra, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.coercible_to_frame_p(fra, kb_local_only_p) != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(:class ?x)
    pattern = _class_Frame;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node all = kb.get_kb_classes(_all, kb_local_only_p);
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fra, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.class_p(fra, kb_local_only_p) != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(:slot ?x)
    pattern = _slot_Frame;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node all = kb.get_kb_slots(_all, kb_local_only_p);
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fra, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.slot_p(fra, kb_local_only_p) != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(:facet ?x)
    pattern = _facet_Frame;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node all = kb.get_kb_facets(_all, kb_local_only_p);
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fra, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.facet_p(fra, kb_local_only_p) != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // ---- Handle `(:individual ?x)
    pattern = _individual_Frame;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node all = kb.get_kb_individuals(_all, kb_local_only_p);
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fra, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.individual_p(fra, kb_local_only_p) != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }


  static final Node __subclass_of_Sub_Super 
    = Cons.list(__subclass_of, _sub_var, _super_var);
  static final Node __superclass_of_Super_Sub 
    = Cons.list(__superclass_of, _super_var, _sub_var);

  static Values3 handle_subclass_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings)
  { Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;

    if (TupleStore.variable_p(query.car()))
      // Do not allow variable in the CAR.
      { return new Values3(__fail, _T, _T); }

    // ---- Handle `(:superclass-of ?super ?sub) and (:subclass-of ?sub ?super)
    pattern = __subclass_of_Sub_Super;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      // Didn't unify, so try the othwe way around.
      { pattern = __superclass_of_Super_Sub;
	ask_result = TupleStore.unify_against_literal(pattern, query);
      }
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { 
	Node sub = Cons.assoc(_sub_var,   ask_result).cdr();
	Node sup = Cons.assoc(_super_var, ask_result).cdr();
	if (TupleStore.variable_p(sup) && TupleStore.variable_p(sub))
	  { Node enumerand = kb.get_kb_classes(_all, kb_local_only_p);
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node frame = e.next();
		  Node new_query = Cons.list(__subclass_of, frame, sup);
		  Values3 vals = handle_subclass_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings);
//		  System.out.println("Rec: handle_subclass_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(sub, frame), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(sup))
	  { Node all = kb.get_class_superclasses(sub, inference_level, _all,
						 kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(sup, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(sub))
	  { Node all = kb.get_class_subclasses(sup, inference_level, _all,
					       kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(sub, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.subclass_of_p(sub, sup, inference_level, 
				  kb_local_only_p).firstValue()
		 != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }



  static final Node __instance_of_Frame_Class
    = Cons.list(__instance_of, _frame_var, _class_var);
  static final Node __type_of_Class_Frame 
    = Cons.list(__type_of, _class_var, _frame_var);
  static final Node _Class_Frame 
    = Cons.list(_class_var, _frame_var);

  static Values3 handle_instance_type_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings)
  { Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;

    if (TupleStore.variable_p(query.car()))
      // Do not allow variable in the CAR.
      { return new Values3(__fail, _T, _T); }

    // ---- Handle `(:instance-of ?frame ?class) and (:type-of ?class ?frame)
    pattern = __instance_of_Frame_Class;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      // Didn't unify, so try the other way around.
      { pattern = __type_of_Class_Frame;
	ask_result = TupleStore.unify_against_literal(pattern, query);
      }
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      // Didn't unify, so try the third way.
      { pattern = _Class_Frame;
	ask_result = TupleStore.unify_against_literal(pattern, query);
      }
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { 
	Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	Node cla = Cons.assoc(_class_var, ask_result).cdr();
	if (TupleStore.variable_p(cla) && TupleStore.variable_p(fra))
	  { Node enumerand = kb.get_kb_classes(_all, kb_local_only_p);
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node the_class = e.next();
		  Node new_query = 
		    Cons.list(__instance_of, fra, the_class);
		  Values3 vals = handle_instance_type_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(cla, the_class), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(cla))
	  { Node all = kb.get_instance_types(fra, inference_level, _all,
					     kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(cla, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(fra))
	  { Node all = kb.get_class_instances(cla, inference_level, _all,
					      kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fra, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.instance_of_p(fra, cla, inference_level, 
				  kb_local_only_p).firstValue()
		 != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }


  static final Node __name_Frame_Value
    = Cons.list(__name, _frame_var, _value_var);
  static final Node __pretty_name_Frame_Value
    = Cons.list(__pretty_name, _frame_var, _value_var);
  static final Node __handle_Frame_Value
    = Cons.list(__handle, _frame_var, _value_var);

  static Values3 handle_frame_properties_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings)
  { Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;
    Values3 v = null;

    if (TupleStore.variable_p(query.car()))
      // Do not allow variable in the CAR.
      { return new Values3(__fail, _T, _T); }

    // ---- Handle `(:name ?frame ?class)
    pattern = __name_Frame_Value;
    v = handle_frame_properties_query_internal
      (query, kb, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, pattern);
    if (v.thirdValue() == _NIL) { return v; }

    // ---- Handle `(:pretty-name ?frame ?class)
    pattern = __pretty_name_Frame_Value;
    v = handle_frame_properties_query_internal
      (query, kb, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, pattern);
    if (v.thirdValue() == _NIL) { return v; }

    // ---- Handle `(:handle ?frame ?class)
    pattern = __handle_Frame_Value;
    v = handle_frame_properties_query_internal
      (query, kb, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, pattern);
    if (v.thirdValue() == _NIL) { return v; }

    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }


  static Values3 handle_frame_properties_query_internal
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, Node pattern)
  { Node unify_failed_p = null;
    Node ask_result = null;

    // ---- Handle `(rel ?frame ?value)
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { 
	Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	Node val = Cons.assoc(_value_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node enumerand = kb.get_kb_frames(kb_local_only_p);
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node frame = e.next();
		  Node new_query = 
		    Cons.list(pattern.car(), frame, val);
		  Values3 vals = handle_frame_properties_query_internal
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, pattern);
//           System.out.println("Rec: handle_frame_properties_query_internal" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(fra, frame), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(val))
	  { Node v = null;
	    if (__name_Frame_Value == pattern)
	      { v = kb.get_frame_name(fra, kb_local_only_p); }
	    else if (__pretty_name_Frame_Value == pattern)
	      { v = kb.get_frame_pretty_name(fra, kb_local_only_p); }
	    else if (__handle_Frame_Value == pattern)
	      { v = kb.get_frame_handle(fra, kb_local_only_p); }
	    else { throw new GenericError
		   (_T, new OKBCString("Unhamdled case")).signal(); 
		 }
	    if (v != _NIL) 
	      { return new Values3(augment_bindings(val, Cons.list(v),
						    bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else 
	  { Node v = null;
	    if (__name_Frame_Value == pattern)
	      { v = kb.get_frame_name(fra, kb_local_only_p); }
	    else if (__pretty_name_Frame_Value == pattern)
	      { v = kb.get_frame_pretty_name(fra, kb_local_only_p); }
	    else if (__handle_Frame_Value == pattern)
	      { v = kb.get_frame_handle(fra, kb_local_only_p); }
	    else { throw new GenericError
		   (_T, new OKBCString("Unhamdled case")).signal(); 
		 }
	    if (v.equals(val))
	      { return new Values3(Cons.list(bindings), _T, _NIL); }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }


  static final Node __slot_of_Slot_Frame
    = Cons.list(__slot_of, _slot_var, _frame_var);
  static final Node __template_slot_of_Slot_Frame
    = Cons.list(__template_slot_of, _slot_var, _frame_var);

  static Values3 handle_slot_of_attachment_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, Node relation, 
	 SlotType slot_type)
  { Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;

    if (TupleStore.variable_p(query.car()))
      // Do not allow variable in the CAR.
      { return new Values3(__fail, _T, _T); }

    // --- Handle `(:slot-of ?slot ?frame) and (:template-slot-of ?slot ?frame)
    pattern = ((slot_type == _own)
	       ? __slot_of_Slot_Frame
	       : __template_slot_of_Slot_Frame);
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { 
	Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	Node slo = Cons.assoc(_slot_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node enumerand = ((slot_type == _template)
			      ? kb.get_kb_classes(_all, kb_local_only_p)
			      : kb.get_kb_frames(kb_local_only_p));
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node frame = e.next();
		  Node new_query = 
		    (slot_type == _own 
		     ? Cons.list(__slot_of, slo, frame)
		     : Cons.list(__template_slot_of, slo, frame));
		  Values3 vals = handle_slot_of_attachment_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, relation, slot_type);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(fra, frame), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(slo))
	  { Node all = kb.get_frame_slots(fra, inference_level, slot_type,
					  kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(slo, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.frame_has_slot_p(fra, slo, inference_level, 
				     slot_type, kb_local_only_p)
		 != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }



  static final Node __facet_of_Facet_Slot_Frame
    = Cons.list(__facet_of, _facet_var, _slot_var, _frame_var);
  static final Node __template_facet_of_Facet_Slot_Frame
    = Cons.list(__template_facet_of, _facet_var, _slot_var, _frame_var);

  static Values3 handle_facet_of_attachment_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, Node relation, 
	 SlotType slot_type)
  { Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;

    if (TupleStore.variable_p(query.car()))
      // Do not allow variable in the CAR.
      { return new Values3(__fail, _T, _T); }

    // --- Handle `(:facet-of ?facet ?slot ?frame)
    //     and     (:template-facet-of ?facet ?slot ?frame)
    pattern = ((slot_type == _own)
	       ? __facet_of_Facet_Slot_Frame
	       : __template_facet_of_Facet_Slot_Frame);
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { 
	Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	Node slo = Cons.assoc(_slot_var,  ask_result).cdr();
	Node fac = Cons.assoc(_facet_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node enumerand = ((slot_type == _template)
			      ? kb.get_kb_classes(_all, kb_local_only_p)
			      : kb.get_kb_frames(kb_local_only_p));
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node frame = e.next();
		  Node new_query = 
		    (slot_type == _own 
		     ? Cons.list(__facet_of, fac, slo, frame)
		     : Cons.list(__template_facet_of, fac, slo, frame));
		  Values3 vals = handle_facet_of_attachment_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, relation, slot_type);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(fra, frame), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(slo))
	  { Node enumerand = kb.get_frame_slots(fra, inference_level, 
						slot_type,
						kb_local_only_p).firstValue();
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node slot = e.next();
		  Node new_query = 
		    (slot_type == _own 
		     ? Cons.list(__facet_of, fac, slot, fra)
		     : Cons.list(__template_facet_of, fac, slot, fra));
		  Values3 vals = handle_facet_of_attachment_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, relation, slot_type);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(slo, slot), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(fac))
	  { Node all = kb.get_slot_facets(fra, slo, inference_level, slot_type,
					  kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fac, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.slot_has_facet_p(fra, slo, fac, inference_level, 
				     slot_type, kb_local_only_p)
		 != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }


  static final Node _Slot_Frame_Value
    = Cons.list(_slot_var, _frame_var, _value_var);
  static final Node __template_slot_value_Slot_Frame_Value
    = Cons.list(__template_slot_value, _slot_var, _frame_var, _value_var);

  static Values3 handle_slot_value_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, SlotType slot_type)
  { if (query.car() == _holds)
      { return handle_slot_value_query
	  (query.cdr(), kb, inference_level, error_p, value_selector,
	   kb_local_only_p, bindings, slot_type);
      }
    Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;
    // --- Handle `(?slot ?frame ?value) 
    //     and     (:template-slot-value ?slot ?frame ?value)
    pattern = ((slot_type == _own)
	       ? _Slot_Frame_Value
	       : __template_slot_value_Slot_Frame_Value);
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(query.car()) && slot_type == _template) ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { 
	Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	Node slo = Cons.assoc(_slot_var,  ask_result).cdr();
	Node val = Cons.assoc(_value_var, ask_result).cdr();
//	System.out.println("SLO=" + slo + ", FRA=" + fra + ", VAL=" + val);
	if (TupleStore.variable_p(fra) &&
	    (TupleStore.variable_p(slo) || TupleStore.variable_p(val)))
	  { Node enumerand = ((slot_type == _template)
			      ? kb.get_kb_classes(_all, kb_local_only_p)
			      : kb.get_kb_frames(kb_local_only_p));
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node frame = e.next();
		  Node new_query = 
		    (slot_type == _own 
		     ? Cons.list(slo, frame, val)
		     : Cons.list(__template_slot_value, slo, frame, val));
		  Values3 vals = handle_slot_value_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, slot_type);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(fra, frame), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(slo))
	  { Node enumerand = kb.get_frame_slots
	       (fra, inference_level, slot_type, kb_local_only_p).firstValue();
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node slot = e.next();
		  Node new_query = 
		    (slot_type == _own 
		     ? Cons.list(slot, fra, val)
		     : Cons.list(__template_slot_value, slot, fra, val));
		  Values3 vals = handle_slot_value_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, slot_type);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(slo, slot), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(fra))
	  { Node all = kb.get_frames_with_slot_value
	      (slo, val, inference_level, slot_type, value_selector,
	       kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fra, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(val))
	  { Node all = kb.get_slot_values(fra, slo, inference_level, slot_type,
					  _all, value_selector,
					  kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(val, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.member_slot_value_p(fra, slo, val, inference_level, 
					_equal, slot_type, value_selector,
					kb_local_only_p).firstValue()
		 != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }



  static final Node _Facet_Slot_Frame_Value
    = Cons.list(_facet_var, _slot_var, _frame_var, _value_var);
  static final Node __template_facet_value_Facet_Slot_Frame_Value
    = Cons.list(__template_facet_value, _facet_var, _slot_var, _frame_var,
		_value_var);

  static Values3 handle_facet_value_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, SlotType slot_type)
  { if (query.car() == _holds)
      { return handle_facet_value_query
	  (query.cdr(), kb, inference_level, error_p, value_selector,
	   kb_local_only_p, bindings, slot_type);
      }
    Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;
    // --- Handle `(?facet ?slot ?frame ?value) 
    //     and     (:template-facet-value ?facet ?slot ?frame ?value)
    pattern = ((slot_type == _own)
	       ? _Facet_Slot_Frame_Value
	       : __template_facet_value_Facet_Slot_Frame_Value);
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { 
	Node fac = Cons.assoc(_facet_var, ask_result).cdr();
	Node slo = Cons.assoc(_slot_var,  ask_result).cdr();
	Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	Node val = Cons.assoc(_value_var, ask_result).cdr();
//      System.out.println("FAC="+fac+", SLO="+slo+", FRA="+fra+", VAL="+val);
	if (TupleStore.variable_p(fra) &&
	    (TupleStore.variable_p(fac) || TupleStore.variable_p(slo) || 
	     TupleStore.variable_p(val)))
	  { Node enumerand = ((slot_type == _template)
			      ? kb.get_kb_classes(_all, kb_local_only_p)
			      : kb.get_kb_frames(kb_local_only_p));
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node frame = e.next();
		  Node new_query = 
		    (slot_type == _own 
		     ? Cons.list(fac, slo, frame, val)
		     : Cons.list(__template_facet_value, fac, slo, frame,val));
		  Values3 vals = handle_facet_value_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, slot_type);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(fra, frame), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(slo))
	  { Node enumerand = kb.get_frame_slots
	       (fra, inference_level, slot_type, kb_local_only_p).firstValue();
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node slot = e.next();
		  Node new_query = 
		    (slot_type == _own 
		     ? Cons.list(fac, slot, fra, val)
		     : Cons.list(__template_facet_value, fac, slot, fra, val));
		  Values3 vals = handle_facet_value_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, slot_type);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(slo, slot), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(fac))
	  { Node enumerand = kb.get_slot_facets
	       (fra, slo, inference_level, slot_type,
		kb_local_only_p).firstValue();
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node facet = e.next();
		  Node new_query = 
		    (slot_type == _own 
		     ? Cons.list(facet, slo, fra, val)
		     : Cons.list(__template_facet_value, facet, slo, fra,val));
		  Values3 vals = handle_facet_value_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings, slot_type);
//		  System.out.println("Rec: handle_instance_type_query" +
//		    Cons.list(new_query, kb, inference_level, error_p,
//			      value_selector, kb_local_only_p, bindings) + 
//				     " -> " + vals);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(fac, facet), list_of_binding_lists),
		       results);
//		      System.out.println("Result is now: " + results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(fra))
	  { Node all = kb.get_frames_with_facet_value
	      (slo, fac, val, inference_level, slot_type, value_selector,
	       kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(fra, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (TupleStore.variable_p(val))
	  { Node all = kb.get_facet_values(fra, slo, fac, inference_level,
					   slot_type, _all, value_selector,
					  kb_local_only_p).firstValue();
	    if (all != _NIL) 
	      { return new Values3(augment_bindings(val, all, bindings),
				   _T, _NIL); 
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.member_facet_value_p(fra, slo, fac, val, inference_level, 
					 _equal, slot_type, value_selector,
					 kb_local_only_p).firstValue()
		 != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }


  static final Node __primitive_Frame = Cons.list(__primitive, _frame_var);

  static Values3 handle_primitivity_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings)
  { Node unify_failed_p = null;
    Node pattern = null;
    Node ask_result = null;

    if (TupleStore.variable_p(query.car()))
      // Do not allow variable in the CAR.
      { return new Values3(__fail, _T, _T); }

    // ---- Handle `(:primitive ?x)
    pattern = __primitive_Frame;
    ask_result = TupleStore.unify_against_literal(pattern, query);
    if (ask_result == __fail ||
	(TupleStore.variable_p(pattern.car()) &&
	 Cons.member_eql(query.car(), kif_operator_symbols, kb,
			 kb_local_only_p)))
      {} // Didn't unify, so try something else.
    else 
      { Node fra = Cons.assoc(_frame_var, ask_result).cdr();
	if (TupleStore.variable_p(fra))
	  { Node enumerand = kb.get_kb_classes(_all, kb_local_only_p);
	    if (enumerand != _NIL) 
	      { Enumerator e = enumerand.enumerate_list();
		Node results = _NIL;
		while (e.has_more_p()) {
		  Node frame = e.next();
		  Node new_query = Cons.list(__primitive, frame);
		  Values3 vals = handle_primitivity_query
		    (new_query, kb, inference_level, error_p, value_selector,
		     kb_local_only_p, bindings);
		  Node list_of_binding_lists = vals.firstValue();
		  if (list_of_binding_lists != __fail)
		    { results = Cons.append_cons
		      (add_pair_to_bindings
		         (new Cons(fra, frame), list_of_binding_lists),
		       results);
		    }
		}
		if (results == _NIL)
		  { return new Values3(__fail, _T, _NIL); }
		else { return new Values3(results, _T, _NIL); }
	      }
	    else { return new Values3(__fail, _T, _NIL); }
	  }
	else if (kb.primitive_p(fra, kb_local_only_p) != _NIL)
	  { return new Values3(Cons.list(bindings), _T, _NIL); }
	else { return new Values3(__fail, _T, _NIL); }
      }
    // We completely failed to unify
    return new Values3(__fail, _T, _T);
  }


  static Values2 handle_simple_query
	(Node query, Kb kb, InferenceLevel inference_level,
	 TruthValue error_p, ValueSelector value_selector,
	 TruthValue kb_local_only_p, Node bindings, 
	 ConsLocation all_binding_lists)
  { return kb.handle_simple_query
      (query, inference_level, error_p, value_selector, kb_local_only_p,
       bindings, all_binding_lists);
  }


  // ======================== Test Harness ================================

  static void test_ask_directly_against_pattern
      (String string, Node pattern, Kb kb, ValueSelector value_selector,
       TruthValue kb_local_only_p, boolean verbose_p) {
    Values3 ask_result = kb.ask(pattern, pattern, _direct, _all, _T, _NIL,
				_NIL, value_selector, kb_local_only_p);
    test_equal(kb, string, ask_result, Cons.list(pattern), verbose_p);
  }


  static void test_ask_against_pattern(String string, Node pattern, Kb kb,
				       ValueSelector value_selector,
				       TruthValue kb_local_only_p,
				       boolean verbose_p) {
    Values3 ask_result = kb.ask(pattern, pattern, _taxonomic, _all, _T, _NIL,
				_NIL, value_selector, kb_local_only_p);
    test_equal(kb, string, ask_result, Cons.list(pattern), verbose_p);
  }


  static void test_ask_against_pattern_not
     (String string, Node pattern, Kb kb, ValueSelector value_selector,
      TruthValue kb_local_only_p, boolean verbose_p) {
    Values3 ask_result = kb.ask(pattern, pattern, _taxonomic, _all, _T, _NIL,
				_NIL, value_selector, kb_local_only_p);
    test_equal(kb, string, ask_result, _NIL, verbose_p);
  }


  public static void test_tell_and_ask (Kb kb, boolean verbose_p) {
    TruthValue kb_local_only_p = _NIL;
    ValueSelector value_selector = _known_true;
    Values3 ask_result = null;
    Node pattern = null;

    OKBCInteger one = new OKBCInteger(1);
    Real two = new Real(2.0);
    OKBCInteger three = new OKBCInteger(3);

    Symbol c1 = Symbol.intern("C1", "OKBC");
    Symbol c2 = Symbol.intern("C2", "OKBC");
    Symbol c3 = Symbol.intern("C3", "OKBC");
    Symbol c4 = Symbol.intern("C4", "OKBC");
    Symbol c2_alternate_name = Symbol.intern("C2-ALTERNAME-NAME", "OKBC");
    Node c1f = null;
    Node c2f = null;
    Node c3f = null;
    Node c4f = null;

    // Test creation.
    // Classes
    kb.tell(Cons.list(_class, c1), _NIL, value_selector, kb_local_only_p);
    c1f = get_unique_frame(c1, kb);
    test_true(kb, "class_p(c1)", kb.class_p(c1f, kb_local_only_p), verbose_p);
    test_ask_against_pattern("Ask(:class c1f)", Cons.list(_class, c1f), kb,
			     value_selector, kb_local_only_p, verbose_p);

    kb.tell(Cons.list(_and, Cons.list(_class, c2),
		            Cons.list(__subclass_of, c2, c1f)),
	    _NIL, value_selector, _NIL);
    c2f = get_unique_frame(c2, kb);
    test_true(kb, "class_p(c2f)", kb.class_p(c2f, kb_local_only_p), verbose_p);
    test_true(kb, "subclass_of_p(c2f c1f)", 
	      kb.subclass_of_p(c2f, c1f, _taxonomic, kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern("Ask(:class c2f)", Cons.list(_class, c2f), kb,
			     value_selector, kb_local_only_p, verbose_p);

    kb.tell(Cons.list(_and, Cons.list(_class, c3),
		            Cons.list(__superclass_of, c1f, c3)),
	    _NIL, value_selector, _NIL);
    c3f = get_unique_frame(c3, kb);
    test_true(kb, "class_p(c3f)", kb.class_p(c3f, kb_local_only_p), verbose_p);
    test_true(kb, "(:subclass-of c3f c1f)", 
	      kb.subclass_of_p(c3f, c1f, _taxonomic, kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern("Ask(:class c3f)", Cons.list(_class, c3f), kb,
			     value_selector, kb_local_only_p, verbose_p);

    kb.tell(Cons.list(_and, Cons.list(_class, c4),
		            Cons.list(__superclass_of, c2f, c4)),
	    _NIL, value_selector, _NIL);
    c4f = get_unique_frame(c4, kb);
    test_true(kb, "class_p(c4f)", kb.class_p(c4f, kb_local_only_p), verbose_p);
    test_true(kb, "(:subclass-of c4f c1f)", 
	      kb.subclass_of_p(c4f, c1f, _taxonomic, kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern("Ask(:class c4f)", Cons.list(_class, c4f), kb,
			     value_selector, kb_local_only_p, verbose_p);


    ask_result = kb.ask(Cons.list(__subclass_of, _class_var, c1f), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c2f, c3f, c4f), 
		    "Ask(:subclass-of ?class c1f)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__subclass_of, _class_var, c1f), _class_var,
			_direct, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c2f, c3f), 
		    "Ask(:subclass-of ?class c1f) - direct",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__superclass_of, c1f, _class_var),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c2f, c3f, c4f), 
		    "Ask(:superclass-of c1f ?class)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__subclass_of, c1f, _class_var), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_all_in_list(kb, Cons.list(_thing), 
		     "Ask(:subclass-of c1f ?class)",
		     ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__superclass_of, _class_var, c1f),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_all_in_list(kb, Cons.list(_thing),
		     "Ask(:superclass-of ?class c1f)",
		     ask_result, verbose_p);


    ask_result = kb.ask(Cons.list(__subclass_of, _class_var, c2f), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c4f), 
		    "Ask(:subclass-of ?class c2f)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__superclass_of, c2f, _class_var), 
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c4f),
		    "Ask(:superclass-of c2f ?class)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__subclass_of, c2f, _class_var), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_all_in_list(kb, Cons.list(c1f, _thing),
		     "Ask(:subclass-of c2f ?class)",
		     ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__superclass_of, _class_var, c2f),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_all_in_list(kb, Cons.list(c1f, _thing),
		     "Ask(:superclass-of ?class c2f)",
		     ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__superclass_of, _super_var, _sub_var),
			Cons.list(_sub_var, _super_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(Cons.list(c2f, c1f),
				Cons.list(c3f, c1f),
				Cons.list(c4f, c1f),
				Cons.list(c4f, c2f)),
		  "Ask(:superclass-of ?sub ?super)",
		  ask_result, verbose_p);

    // Now test to get all classes.
    ask_result = kb.ask(Cons.list(_class, _class_var), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(c1f, c2f, c3f, c4f), "Ask(:class ?class)",
		  ask_result, verbose_p);

    Symbol i1 = Symbol.intern("I1", "OKBC");
    Symbol i2 = Symbol.intern("I2", "OKBC");
    Symbol i3 = Symbol.intern("I3", "OKBC");
    Symbol i4 = Symbol.intern("I4", "OKBC");
    Node i1f = null;
    Node i2f = null;
    Node i3f = null;
    Node i4f = null;

    // Individuals
    kb.tell(Cons.list(_and, Cons.list(_individual, i1),
		            Cons.list(__instance_of, i1, c1f)),
	    _NIL, value_selector, _NIL);
    i1f = get_unique_frame(i1, kb);
    test_true(kb, "individual_p(i1f)", kb.individual_p(i1f, kb_local_only_p),
	      verbose_p);
    test_true(kb, "instance_of_p(i1f c1f)", 
	      kb.instance_of_p(i1f, c1f, _taxonomic, kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern
      ("Ask(:individual i1f)", Cons.list(_individual, i1f), kb,
       value_selector, kb_local_only_p, verbose_p);

    kb.tell(Cons.list(_and, Cons.list(_individual, i2),
		            Cons.list(__type_of, c2f, i2)),
	    _NIL, value_selector, _NIL);
    i2f = get_unique_frame(i2, kb);
    test_true(kb, "(individual_p(i2f)", kb.individual_p(i2f, kb_local_only_p),
	      verbose_p);
    test_true(kb, "instance_of_p(i2f c2f)", 
	      kb.instance_of_p(i2f, c2f, _taxonomic, kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern
      ("Ask(:individual i2f)", Cons.list(_individual, i2f), kb,
       value_selector, kb_local_only_p, verbose_p);

    kb.tell(Cons.list(_and, Cons.list(_individual, i3),
		            Cons.list(__instance_of, i3, c3f)),
	    _NIL, value_selector, _NIL);
    i3f = get_unique_frame(i3, kb);
    test_true(kb, "individual_p(i3f)", kb.individual_p(i3f, kb_local_only_p),
	      verbose_p);
    test_true(kb, "instance_of_pi3f c3f)", 
	      kb.instance_of_p(i3f, c3f, _taxonomic, kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern
      ("Ask(:individual i3f)", Cons.list(_individual, i3f), kb,
       value_selector, kb_local_only_p, verbose_p);

    kb.tell(Cons.list(_and, Cons.list(_individual, i4),
		            Cons.list(__type_of, c4f, i4)),
	    _NIL, value_selector, _NIL);
    i4f = get_unique_frame(i4, kb);
    test_true(kb, "individual_p(i4f)", kb.individual_p(i4f, kb_local_only_p),
	      verbose_p);
    test_true(kb, "instance_of_pi4f c4f)", 
	      kb.instance_of_p(i4f, c4f, _taxonomic, kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern
      ("Ask(:individual i4f)", Cons.list(_individual, i4f), kb,
       value_selector, kb_local_only_p, verbose_p);



    
    ask_result = kb.ask(Cons.list(__instance_of, _frame_var, c1f), _frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(i1f, i2f, i3f, i4f), 
		    "Ask(:instance-of ?frame c1f)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(c1f, _frame_var), _frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(i1f, i2f, i3f, i4f), 
		    "Ask(c1f ?frame)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(c1f, _frame_var), _frame_var,
			_direct, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(i1f), 
		    "Ask(c1f ?frame) - direct",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__type_of, c1f, _frame_var), _frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(i1f, i2f, i3f, i4f), 
		    "Ask(:type-of c1f ?frame)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__instance_of, c1f, _class_var), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(_class),
		  "Ask(:instance-of c1f ?class)",
		  ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__type_of, _class_var, c1f), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(_class),
		  "Ask(:type-of ?class c1f)",
		  ask_result, verbose_p);


    ask_result = kb.ask(Cons.list(__instance_of, _frame_var, c2f), _frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(i2f, i4f), 
		    "Ask(:instance-of ?frame c2f)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__type_of, c2f, _frame_var), _frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(i2f, i4f),
		    "Ask(:type-of c2f ?frame)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__instance_of, c2f, _class_var), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(_class),
		  "Ask(:instance-of c2f ?class)",
		  ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__type_of, _class_var, c2f), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(_class),
		  "Ask(:type-of ?class c2f)",
		  ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__type_of, _class_var, _instance_var),
			Cons.list(_instance_var, _class_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(Cons.list(i1f, c1f),
				Cons.list(i2f, c1f),
				Cons.list(i3f, c1f),
				Cons.list(i4f, c1f),

				Cons.list(i2f, c2f),
				Cons.list(i4f, c2f),

				Cons.list(i3f, c3f),
				Cons.list(i4f, c4f)),
		    "Ask(:type-of ?class ?frame)",
		    ask_result, verbose_p);

    // Now test to get all individuals.
    ask_result = kb.ask(Cons.list(_individual, _individual_var), 
			_individual_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(i1f, i2f, i3f, i4f), 
		  "Ask(:individual ?individual)",
		  ask_result, verbose_p);


    // Now test frame properties
    ask_result = kb.ask(Cons.list(__name, c2f, _value_var), _value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal(kb, "Ask(:name c2f ?value)", ask_result, Cons.list(c2),
	       verbose_p);
    ask_result = kb.ask(Cons.list(__name, _class_var, c2), _class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal(kb, "Ask(:name ?frame c2f)", ask_result, Cons.list(c2f),
	       verbose_p);
    ask_result = kb.ask(Cons.list(__name, _class_var, _value_var),
			Cons.list(_class_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(Cons.list(c1f, c1),
				Cons.list(c2f, c2),
				Cons.list(c3f, c3),
				Cons.list(c4f, c4),
				Cons.list(i1f, i1),
				Cons.list(i2f, i2),
				Cons.list(i3f, i3),
				Cons.list(i4f, i4)),
		    "Ask(:name ?frame ?value)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__pretty_name, c2f, _value_var), _value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal(kb, "Ask(:pretty-name c2f ?value)",
	       ask_result,
	       Cons.list(kb.get_frame_pretty_name(c2f, kb_local_only_p)),
	       verbose_p);
    ask_result = kb.ask(Cons.list(__handle, c2f, _value_var), _value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal(kb, "Ask(:handle c2f ?value)",
	       ask_result,
	       Cons.list(kb.get_frame_handle(c2f, kb_local_only_p)),
	       verbose_p);

    kb.tell(Cons.list(__name, c2f, c2_alternate_name), _NIL, value_selector,
	    kb_local_only_p);
    test_equal(kb, "get_frame_name(c2f) = c2_altername_name",
	       c2_alternate_name,
	       kb.get_frame_name(c2f, kb_local_only_p),
	       verbose_p);
    ask_result = kb.ask(Cons.list(__name, c2f, _value_var), _value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal(kb, "Ask(:name c2 ?value)",
	       ask_result,
	       Cons.list(kb.get_frame_name(c2f, kb_local_only_p)),
	       verbose_p);
    kb.tell(Cons.list(_name, c2f, c2), _NIL, value_selector,
	    kb_local_only_p);
    ask_result = kb.ask(Cons.list(__name, c2f, _value_var), _value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal(kb, "get_frame_name(c2f) = c2",
	       c2,
	       kb.get_frame_name(c2f, kb_local_only_p),
	       verbose_p);
    test_equal(kb, "Ask(:name c2f ?value)",
	       ask_result,
	       Cons.list(kb.get_frame_name(c2f, kb_local_only_p)),
	       verbose_p);


    OKBCString c2_pretty_name = new OKBCString("C2's pretty name");
    kb.tell(Cons.list(__pretty_name, c2f, c2_pretty_name), _NIL, 
	    value_selector, kb_local_only_p);
    ask_result = kb.ask(Cons.list(__pretty_name, c2f, _value_var), _value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal(kb, "get_frame_pretty_name(c2f)",
	       c2_pretty_name,
	       kb.get_frame_pretty_name(c2f, kb_local_only_p),
	       verbose_p);
    test_equal(kb, "Ask(:pretty-name c2f ?value)",
	       ask_result,
	       Cons.list(kb.get_frame_pretty_name(c2f, kb_local_only_p)),
	       verbose_p);


    Symbol ts1 = Symbol.intern("TS1", "OKBC");
    Symbol os2 = Symbol.intern("Os2", "OKBC");
    Node ts1f = null;
    Node os2f = null;

    // Slots
    kb.tell(Cons.list(_and, Cons.list(_slot, ts1),
		            Cons.list(__template_slot_of, ts1, c1f)),
	    _NIL, value_selector, _NIL);
    ts1f = get_unique_frame(ts1, kb);
    test_true(kb, "slot_p(ts1f)", kb.slot_p(ts1f, kb_local_only_p),
	      verbose_p);
    test_true(kb, "frame_has_slot_p(c1f, ts1f, _template)", 
	      kb.frame_has_slot_p(c1f, ts1f, _taxonomic, _template,
				  kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern
      ("Ask(:slot ts1)", Cons.list(_slot, ts1f), kb,
       value_selector, kb_local_only_p, verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_of, ts1f, _class_var),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c1f, c2f, c3f, c4f), 
		    "Ask(:template-slot-of ts1f ?class)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_of, _slot_var, c2f),
			_slot_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(ts1f), 
		    "Ask(:template-slot-of ?slot c2f)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_of, _slot_var, _class_var),
			Cons.list(_class_var, _slot_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c1f, ts1f),
				  Cons.list(c2f, ts1f),
				  Cons.list(c3f, ts1f),
				  Cons.list(c4f, ts1f)),
		    "Ask(:template-slot-of ?slot ?class)",
		    ask_result, verbose_p);

    // Own slots
    kb.tell(Cons.list(_and, Cons.list(_slot, os2),
		            Cons.list(__slot_of, os2, c2f)),
	    _NIL, value_selector, _NIL);
    os2f = get_unique_frame(os2, kb);
    test_true(kb, "slot_p(os2f)", kb.slot_p(os2f, kb_local_only_p),
	      verbose_p);
    test_true(kb, "frame_has_slot_p(c2f, os2f, _own)", 
	      kb.frame_has_slot_p(c2f, os2f, _taxonomic, _own,
				  kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern
      ("Ask(:slot os2)", Cons.list(_slot, os2f), kb,
       value_selector, kb_local_only_p, verbose_p);
    ask_result = kb.ask(Cons.list(__slot_of, os2f, _frame_var),
			_frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c2f), 
		    "Ask(:slot-of os2f ?frame)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__slot_of, _slot_var, c2f),
			_slot_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(os2f), 
		    "Ask(:slot-of ?slot c2f)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__slot_of, _slot_var, _frame_var),
			Cons.list(_frame_var, _slot_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c2f, os2f),
				  Cons.list(i1f, ts1f),
				  Cons.list(i2f, ts1f),
				  Cons.list(i3f, ts1f),
				  Cons.list(i4f, ts1f)),
		    "Ask(:slot-of ?slot ?frame)",
		    ask_result, verbose_p);


    Symbol fa1 = Symbol.intern("FA1", "OKBC");
    Symbol fa2 = Symbol.intern("FA2", "OKBC");
    Node fa1f = null;
    Node fa2f = null;

    // Facets
    // Template Facets
    kb.tell(Cons.list(_and, Cons.list(_facet, fa1),
		            Cons.list(__template_facet_of, fa1, ts1f, c1f)),
	    _NIL, value_selector, _NIL);
    fa1f = get_unique_frame(fa1, kb);
    test_true(kb, "facet_p(fa11f)", kb.facet_p(fa1f, kb_local_only_p),
	      verbose_p);
    test_true(kb, "slot_has_facet_p(c1f, ts1f, fa1f, _template)", 
	      kb.slot_has_facet_p(c1f, ts1f, fa1f, _taxonomic, _template,
				  kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern
      ("Ask(:facet fa1f)", Cons.list(_facet, fa1f), kb,
       value_selector, kb_local_only_p, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_of, fa1f, ts1f, _class_var),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c1f, c2f, c3f, c4f), 
		    "Ask(:template-facet-of fa1f ts1f ?class)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_of, fa1f, _slot_var, c2f),
			_slot_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(ts1f), 
		    "Ask(:template-facet-of fa1f ?slot c2f)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_of, _facet_var, ts1f, c2f),
			_facet_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(fa1f), 
		    "Ask(:template-facet-of ?facet ts1f c2f)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__template_facet_of, fa1f, _slot_var, 
				  _class_var),
			Cons.list(_class_var, _slot_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c1f, ts1f),
				  Cons.list(c2f, ts1f),
				  Cons.list(c3f, ts1f),
				  Cons.list(c4f, ts1f)),
		    "Ask(:template-facet-of fa1f ?slot ?class)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_of, _facet_var, ts1f, 
				  _class_var),
			Cons.list(_class_var, _facet_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c1f, fa1f),
				  Cons.list(c2f, fa1f),
				  Cons.list(c3f, fa1f),
				  Cons.list(c4f, fa1f)),
		    "Ask(:template-facet-of ?facet ts1f ?class)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_of, _facet_var, _slot_var, 
				  c1f),
			Cons.list(_slot_var, _facet_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(ts1f, fa1f)),
		    "Ask(:template-facet-of ?facet ?slot c1f)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__template_facet_of, _facet_var, _slot_var,
				  _class_var),
			Cons.list(_class_var, _slot_var, _facet_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c1f, ts1f, fa1f),
				  Cons.list(c2f, ts1f, fa1f),
				  Cons.list(c3f, ts1f, fa1f),
				  Cons.list(c4f, ts1f, fa1f)),
		    "Ask(:template-facet-of ?facet ?slot ?class)",
		    ask_result, verbose_p);

    // Own facets
    kb.tell(Cons.list(_and, Cons.list(_facet, fa2),
		            Cons.list(__facet_of, fa2, os2f, c2f)),
	    _NIL, value_selector, _NIL);
    fa2f = get_unique_frame(fa2, kb);
    test_true(kb, "facet_p(fa21f)", kb.facet_p(fa2f, kb_local_only_p),
	      verbose_p);
    test_true(kb, "slot_has_facet_p(c2f, os2f, fa2f, _own)", 
	      kb.slot_has_facet_p(c2f, os2f, fa2f, _taxonomic, _own,
				  kb_local_only_p),
	      verbose_p);
    test_ask_against_pattern
      ("Ask(:facet fa2f)", Cons.list(_facet, fa2f), kb,
       value_selector, kb_local_only_p, verbose_p);
    ask_result = kb.ask(Cons.list(__facet_of, fa2f, os2f, _frame_var),
			_frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c2f), 
		    "Ask(:facet-of fa2f os2f ?class)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__facet_of, fa2f, _slot_var, c2f),
			_slot_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(os2f), 
		    "Ask(:facet-of fa2f ?slot c2f)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__facet_of, _facet_var, os2f, c2f),
			_facet_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(fa2f), 
		    "Ask(:facet-of ?facet os2f c2f)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__facet_of, fa2f, _slot_var, _frame_var),
			Cons.list(_frame_var, _slot_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c2f, os2f)),
		    "Ask(:facet-of fa2f ?slot ?class)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__facet_of, _facet_var, os2f, _frame_var),
			Cons.list(_frame_var, _facet_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c2f, fa2f)),
		    "Ask(:facet-of ?facet os2f ?class)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__facet_of, _facet_var, _slot_var, c2f),
			Cons.list(_slot_var, _facet_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(os2f, fa2f)),
		    "Ask(:facet-of ?facet ?slot c2f)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__facet_of, _facet_var, _slot_var,
				  _frame_var),
			Cons.list(_frame_var, _slot_var, _facet_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c2f, os2f, fa2f),
				  Cons.list(i1f, ts1f, fa1f),
				  Cons.list(i2f, ts1f, fa1f),
				  Cons.list(i3f, ts1f, fa1f),
				  Cons.list(i4f, ts1f, fa1f)),
		    "Ask(:facet-of ?facet ?slot ?class)",
		    ask_result, verbose_p);


    // Slot values
    // Template slots
    kb.tell(Cons.list(__template_slot_value, ts1f, c1f, one),
	    _NIL, value_selector, _NIL);
    test_true(kb, "member_slot_value_p(c1f, ts1f, 1, _template)",
	      kb.member_slot_value_p(c1f, ts1f, one, _taxonomic, _equal,
				     _template, value_selector, 
				     kb_local_only_p),
	      verbose_p);
    test_equal(kb, "get_slot_values(c1f, ts1f, _template)",
	       kb.get_slot_values(c1f, ts1f, _taxonomic,
				  _template, _all, value_selector, 
				  kb_local_only_p),
	       Cons.list(one), verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_value, ts1f, c1f, one),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_true(kb, "Ask(:template-slot-value ts1f c1f 1)",
	      ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_value, ts1f, c1f,_value_var),
			_value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(one), 
		    "Ask(:template-slot-value ts1f c1f ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_value, ts1f, _class_var,one),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c1f, c2f, c3f, c4f), 
		    "Ask(:template-slot-value ts1f ?class 1)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_value, _slot_var, c1f, one),
			_slot_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(ts1f), 
		    "Ask(:template-slot-value ?slot c1f 1)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__template_slot_value, ts1f, _class_var,
				  _value_var),
			Cons.list(_class_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c1f, one),
				  Cons.list(c2f, one),
				  Cons.list(c3f, one),
				  Cons.list(c4f, one)),
		    "Ask(:template-slot-value ts1f ?class ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_value, _slot_var, c1f,
				  _value_var),
			Cons.list(_slot_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(ts1f, one)),
		    "Ask(:template-slot-value ?slot c1f ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_slot_value, _slot_var, _class_var,
				  one),
			Cons.list(_slot_var, _class_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(ts1f, c1f),
				  Cons.list(ts1f, c2f),
				  Cons.list(ts1f, c3f),
				  Cons.list(ts1f, c4f)),
		    "Ask(:template-slot-value ?slot ?class 1)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__template_slot_value, _slot_var, _class_var,
				  _value_var),
			Cons.list(_slot_var, _class_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(ts1f, c1f, one),
				  Cons.list(ts1f, c2f, one),
				  Cons.list(ts1f, c3f, one),
				  Cons.list(ts1f, c4f, one)),
		    "Ask(:template-slot-value ?slot ?class ?value)",
		    ask_result, verbose_p);


    // Own slots
    kb.tell(Cons.list(os2f, c2f, three),
	    _NIL, value_selector, _NIL);
    test_true(kb, "member_slot_value_p(c2f, os2f, 3, _own)",
	      kb.member_slot_value_p(c2f, os2f, three, _taxonomic, _equal,
				     _own, value_selector, 
				     kb_local_only_p),
	      verbose_p);
    test_equal(kb, "get_slot_values(c2f, os2f, _own)",
	       kb.get_slot_values(c2f, os2f, _taxonomic,
				  _own, _all, value_selector, 
				  kb_local_only_p),
	       Cons.list(three), verbose_p);
    ask_result = kb.ask(Cons.list(os2f, c2f, three),
			_frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_true(kb, "Ask(os2f c2f 3)", ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(os2f, c2f,_value_var),
			_value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(three), 
		    "Ask(os2f c2f ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(os2f, _frame_var, three),
			_frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c2f), 
		    "Ask(os2f ?class 3)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(_slot_var, c2f, three),
			_slot_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(os2f), 
		    "Ask(?slot c2f 3)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(os2f, _frame_var, _value_var),
			Cons.list(_frame_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c2f, three)),
		    "Ask(os2f ?class ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(_slot_var, c2f, _value_var),
			Cons.list(_slot_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(os2f, three)),
		    "Ask(?slot c2f ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(_slot_var, _frame_var, three),
			Cons.list(_slot_var, _frame_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(os2f, c2f)),
		    "Ask(?slot ?class 3)",
		    ask_result, verbose_p);

    // Now all three as vars.
    ask_result = kb.ask(Cons.list(_slot_var, _frame_var, _value_var),
			Cons.list(_slot_var, _frame_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(os2f, c2f, three),
				  Cons.list(ts1f, i1f, one),
				  Cons.list(ts1f, i2f, one),
				  Cons.list(ts1f, i3f, one),
				  Cons.list(ts1f, i4f, one)),
		    "Ask(?slot ?class ?value)",
		    ask_result, verbose_p);

    // Now all three as vars with holds.
    ask_result = kb.ask(Cons.list(_holds, _slot_var, _frame_var, _value_var),
			Cons.list(_slot_var, _frame_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(os2f, c2f, three),
				  Cons.list(ts1f, i1f, one),
				  Cons.list(ts1f, i2f, one),
				  Cons.list(ts1f, i3f, one),
				  Cons.list(ts1f, i4f, one)),
		    "Ask(:holds ?slot ?class ?value)",
		    ask_result, verbose_p);


    // Facet values
    // Template facets
    kb.tell(Cons.list(__template_facet_value, fa1f, ts1f, c1f, one),
	    _NIL, value_selector, _NIL);
    test_true(kb, "member_facet_value_p(c1f, ts1f, fa1f, 1, _template)",
	      kb.member_facet_value_p(c1f, ts1f, fa1f, one, _taxonomic, _equal,
				     _template, value_selector, 
				     kb_local_only_p),
	      verbose_p);
    test_equal(kb, "get_facet_values(c1f, ts1f, fa1f, _template)",
	       kb.get_facet_values(c1f, ts1f, fa1f, _taxonomic,
				  _template, _all, value_selector, 
				  kb_local_only_p),
	       Cons.list(one), verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_value, fa1f, ts1f, c1f,one),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_true(kb, "Ask(:template-facet-value fa1f ts1f c1f 1)",
	      ask_result, verbose_p);
    // Test combinations with one var
    ask_result = kb.ask(Cons.list(__template_facet_value, fa1f, ts1f, c1f,
				  _value_var),
			_value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(one), 
		    "Ask(:template-facet-value fa1f, ts1f c1f ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_value, fa1f, ts1f, 
				  _class_var,one),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c1f, c2f, c3f, c4f), 
		    "Ask(:template-facet-value fa1f ts1f ?class 1)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_value, fa1f, _slot_var, c1f,
				  one),
			_slot_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(ts1f), 
		    "Ask(:template-facet-value fa1f ?slot c1f 1)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_value, _facet_var,
				  ts1f, c1f,
				  one),
			_facet_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(fa1f), 
		    "Ask(:template-facet-value ?facet ts1f c1f 1)",
		    ask_result, verbose_p);

    // Test combinations with two vars
    // Loop over Value
    ask_result = kb.ask(Cons.list(__template_facet_value, fa1f, ts1f,
				  _class_var, _value_var),
			Cons.list(_class_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c1f, one),
				  Cons.list(c2f, one),
				  Cons.list(c3f, one),
				  Cons.list(c4f, one)),
		    "Ask(:template-facet-value fa1f ts1f ?class ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_value, fa1f, _slot_var, c1f,
				  _value_var),
			Cons.list(_slot_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(ts1f, one)),
		    "Ask(:template-facet-value fa1f ?slot c1f ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_value, _facet_var, ts1f,
				  c1f, _value_var),
			Cons.list(_facet_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa1f, one)),
		    "Ask(:template-facet-value ?facet ts1f c1f ?v)",
		    ask_result, verbose_p);
    // Loop over Class
    ask_result = kb.ask(Cons.list(__template_facet_value, fa1f, _slot_var,
				  _class_var, one),
			Cons.list(_slot_var, _class_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(ts1f, c1f),
				  Cons.list(ts1f, c2f),
				  Cons.list(ts1f, c3f),
				  Cons.list(ts1f, c4f)),
		    "Ask(:template-facet-value fa1f, ?slot ?class 1)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(__template_facet_value, _facet_var, ts1f,
				  _class_var, one),
			Cons.list(_facet_var, _class_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa1f, c1f),
				  Cons.list(fa1f, c2f),
				  Cons.list(fa1f, c3f),
				  Cons.list(fa1f, c4f)),
		    "Ask(:template-facet-value ?facet ts1f ?class 1)",
		    ask_result, verbose_p);
    // Loop over slot
    ask_result = kb.ask(Cons.list(__template_facet_value, _facet_var,
				  _slot_var, c1f, one),
			Cons.list(_facet_var, _slot_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa1f, ts1f)),
		    "Ask(:template-facet-value ?facet ?slot c1f 1)",
		    ask_result, verbose_p);

    // Now groups of three vars
    ask_result = kb.ask(Cons.list(__template_facet_value, fa1f, _slot_var,
				  _class_var, _value_var),
			Cons.list(_slot_var, _class_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(ts1f, c1f, one),
				  Cons.list(ts1f, c2f, one),
				  Cons.list(ts1f, c3f, one),
				  Cons.list(ts1f, c4f, one)),
		    "Ask(:template-facet-value fa1f ?slot ?class ?value)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__template_facet_value, _facet_var, ts1f,
				  _class_var, _value_var),
			Cons.list(_facet_var, _class_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa1f, c1f, one),
				  Cons.list(fa1f, c2f, one),
				  Cons.list(fa1f, c3f, one),
				  Cons.list(fa1f, c4f, one)),
		    "Ask(:template-facet-value ?facet ts1f ?class ?value)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__template_facet_value, _facet_var, 
				  _slot_var, c1f, _value_var),
			Cons.list(_facet_var, _slot_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa1f, ts1f, one)),
		    "Ask(:template-facet-value ?facet ?slot c1f ?value)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(__template_facet_value, _facet_var, 
				  _slot_var, _class_var, one),
			Cons.list(_facet_var, _slot_var, _class_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa1f, ts1f, c1f),
				  Cons.list(fa1f, ts1f, c2f),
				  Cons.list(fa1f, ts1f, c3f),
				  Cons.list(fa1f, ts1f, c4f)),
		    "Ask(:template-facet-value ?facet ?slot ?class 1)",
		    ask_result, verbose_p);

    // Now all four as vars.
    ask_result = kb.ask(Cons.list(__template_facet_value, _facet_var, 
				  _slot_var, _class_var, _value_var),
			Cons.list(_facet_var, _slot_var, _class_var,
				  _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa1f, ts1f, c1f, one),
				  Cons.list(fa1f, ts1f, c2f, one),
				  Cons.list(fa1f, ts1f, c3f, one),
				  Cons.list(fa1f, ts1f, c4f, one)),
		    "Ask(:template-facet-value ?facet ?slot ?class ?value)",
		    ask_result, verbose_p);


    // Own facets
    kb.tell(Cons.list(fa2f, os2f, c2f, three),
	    _NIL, value_selector, _NIL);
    test_true(kb, "member_facet_value_p(c2f, os2f, fa2f, 1, _own)",
	      kb.member_facet_value_p(c2f, os2f, fa2f, three, _taxonomic,
					   _equal,_own, value_selector, 
					   kb_local_only_p),
	      verbose_p);
    test_equal(kb, "get_facet_values(c2f, os2f, fa2f, _own)",
	       kb.get_facet_values(c2f, os2f, fa2f, _taxonomic,
				  _own, _all, value_selector, 
				  kb_local_only_p),
	       Cons.list(three), verbose_p);
    ask_result = kb.ask(Cons.list(fa2f, os2f, c2f, three),
			_frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_true(kb, "Ask(fa2f os2f c2f 3)",
	      ask_result, verbose_p);
    // Test combinations with three var
    ask_result = kb.ask(Cons.list(fa2f, os2f, c2f, _value_var),
			_value_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(three), 
		    "Ask(fa2f, os2f c2f ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(fa2f, os2f, _frame_var, three),
			_frame_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(c2f), 
		    "Ask(fa2f os2f ?frame 3)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(fa2f, _slot_var, c2f, three),
			_slot_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(os2f), 
		    "Ask(fa2f ?slot c2f 3)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(_facet_var, os2f, c2f, three),
			_facet_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(fa2f), 
		    "Ask(?facet os2f c2f 3)",
		    ask_result, verbose_p);

    // Test combinations with two vars
    // Loop over Value
    ask_result = kb.ask(Cons.list(fa2f, os2f, _frame_var, _value_var),
			Cons.list(_frame_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(c2f, three)),
		    "Ask(fa2f os2f ?frame ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(fa2f, _slot_var, c2f,
				  _value_var),
			Cons.list(_slot_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(os2f, three)),
		    "Ask(fa2f ?slot c2f ?v)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(_facet_var, os2f, c2f, _value_var),
			Cons.list(_facet_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa2f, three)),
		    "Ask(?facet os2f c2f ?v)",
		    ask_result, verbose_p);
    // Loop over Frame
    ask_result = kb.ask(Cons.list(fa2f, _slot_var, _frame_var, three),
			Cons.list(_slot_var, _frame_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(os2f, c2f)),
		    "Ask(fa2f, ?slot ?frame 3)",
		    ask_result, verbose_p);
    ask_result = kb.ask(Cons.list(_facet_var, os2f, _frame_var, three),
			Cons.list(_facet_var, _frame_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa2f, c2f)),
		    "Ask(?facet os2f ?frame 3)",
		    ask_result, verbose_p);
    // Loop over slot
    ask_result = kb.ask(Cons.list(_facet_var, _slot_var, c2f, three),
			Cons.list(_facet_var, _slot_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa2f, os2f)),
		    "Ask(?facet ?slot c2f 3)",
		    ask_result, verbose_p);

    // Now groups of three vars
    ask_result = kb.ask(Cons.list(fa2f, _slot_var, _frame_var, _value_var),
			Cons.list(_slot_var, _frame_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(os2f, c2f, three)),
		    "Ask(fa2f ?slot ?frame ?value)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(_facet_var, os2f, _frame_var, _value_var),
			Cons.list(_facet_var, _frame_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa2f, c2f, three)),
		    "Ask(?facet os2f ?frame ?value)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(_facet_var, _slot_var, c2f, _value_var),
			Cons.list(_facet_var, _slot_var, _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa2f, os2f, three)),
		    "Ask(?facet ?slot c2f ?value)",
		    ask_result, verbose_p);

    ask_result = kb.ask(Cons.list(_facet_var, _slot_var, _frame_var, three),
			Cons.list(_facet_var, _slot_var, _frame_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa2f, os2f, c2f)),
		    "Ask(?facet ?slot ?frame 3)",
		    ask_result, verbose_p);

    // Now all four as vars.
    ask_result = kb.ask(Cons.list(_facet_var, 
				  _slot_var, _frame_var, _value_var),
			Cons.list(_facet_var, _slot_var, _frame_var,
				  _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa2f, os2f, c2f, three),
				  Cons.list(fa1f, ts1f, i1f, one),
				  Cons.list(fa1f, ts1f, i2f, one),
				  Cons.list(fa1f, ts1f, i3f, one),
				  Cons.list(fa1f, ts1f, i4f, one)),
		    "Ask(?facet ?slot ?frame ?value)",
		    ask_result, verbose_p);

    // Now all four as vars with holds.
    ask_result = kb.ask(Cons.list(_holds, _facet_var, 
				  _slot_var, _frame_var, _value_var),
			Cons.list(_facet_var, _slot_var, _frame_var,
				  _value_var),
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_equal_sets(kb, Cons.list(Cons.list(fa2f, os2f, c2f, three),
				  Cons.list(fa1f, ts1f, i1f, one),
				  Cons.list(fa1f, ts1f, i2f, one),
				  Cons.list(fa1f, ts1f, i3f, one),
				  Cons.list(fa1f, ts1f, i4f, one)),
		    "Ask(:holds ?facet ?slot ?frame ?value)",
		    ask_result, verbose_p);


    Symbol c5 = Symbol.intern("C5", "OKBC");
    Node c5f = null;

    // Test primitivity.
    c5f = kb.create_frame(c5, _class, _NIL, c4f,
			  _NIL, _NIL, _NIL, _NIL, _NIL, _NIL, _NIL, _NIL, 
			  kb_local_only_p);
    test_true(kb, "(:primitive c1f)", kb.primitive_p(c1f, kb_local_only_p),
	      verbose_p);
    test_false(kb, "(:primitive c5f)", kb.primitive_p(c5f, kb_local_only_p),
	       verbose_p);
    test_ask_against_pattern("Ask(:primitive c1f)", 
			     Cons.list(__primitive, c1f),
			     kb, value_selector, kb_local_only_p, verbose_p);
    test_ask_against_pattern_not("Ask(:primitive c5f)",
				 Cons.list(__primitive, c5f),
				 kb, value_selector, kb_local_only_p, 
				 verbose_p);
    ask_result = kb.ask(Cons.list(__primitive, _class_var),
			_class_var,
			_taxonomic, _all, _T, _NIL, _NIL, value_selector,
			kb_local_only_p);
    test_at_least(kb, Cons.list(c1f, c2f, c3f, c4f),
		  "Ask(:primitive ?class)",
		  ask_result, verbose_p);


    // Now test evaluable predicates.
    Node fact = null;
    fact = Cons.list(_equals_sign, one, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(= 1 1)", ask_result, verbose_p);
    fact = Cons.list(_equals_sign, two, two);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(= 2.0 2.0)", ask_result, verbose_p);
    fact = Cons.list(_equals_sign, one, two);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(= 1 2.0)", ask_result, verbose_p);

    fact = Cons.list(_not_equal, one, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(/= 1 1)", ask_result, verbose_p);
    fact = Cons.list(_not_equal, two, two);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(/= 2.0 2.0)", ask_result, verbose_p);
    fact = Cons.list(_not_equal, one, two);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(/= 1 2.0)", ask_result, verbose_p);

    fact = Cons.list(_less_than, one, two);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(< 1 2.0)", ask_result, verbose_p);
    fact = Cons.list(_less_than, one, three);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(< 1 3)", ask_result, verbose_p);

    fact = Cons.list(_less_than, two, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(< 2.0 1)", ask_result, verbose_p);
    fact = Cons.list(_less_than, three, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(< 3 1)", ask_result, verbose_p);

    fact = Cons.list(_greater_than, two, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(> 2.0 1)", ask_result, verbose_p);
    fact = Cons.list(_greater_than, three, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(> 3 1)", ask_result, verbose_p);

    fact = Cons.list(_greater_than, one, two);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(> 1 2.0)", ask_result, verbose_p);
    fact = Cons.list(_greater_than, one, three);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(> 1 3)", ask_result, verbose_p);

    fact = Cons.list(_greater_than_or_equal, one, three);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(>= 1 3)", ask_result, verbose_p);
    fact = Cons.list(_greater_than_or_equal, one, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(>= 1 1)", ask_result, verbose_p);
    fact = Cons.list(_greater_than_or_equal, two, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(>= 2 1)", ask_result, verbose_p);

    fact = Cons.list(_less_than_or_equal, one, three);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(<= 1 3)", ask_result, verbose_p);
    fact = Cons.list(_less_than_or_equal, one, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_true(kb, "(<= 1 1)", ask_result, verbose_p);
    fact = Cons.list(_less_than_or_equal, two, one);
    ask_result = kb.ask(fact, fact, _taxonomic, _all, _T, _NIL, _NIL,
			value_selector, kb_local_only_p);
    test_false(kb, "(<= 2 1)", ask_result, verbose_p);
  }


  static void print_facts(TupleKb kb) {
    System.out.println("Facts:-------------"); 
    Node facts =
    kb.tuple_store.facts_full_indexed_under (TupleKb._frame_var);
    if (facts == null) { facts = _NIL; }
    Enumerator f = facts.enumerate_list();
    while (f.has_more_p()) {
      Cons.pretty_print(f.next()); System.out.println();
    }
  }

  public static void main (String argv[]) {
    boolean break_p = OKBCApp.argv_suppliedp("BREAK", argv);
    boolean facts_p = OKBCApp.argv_suppliedp("FACTS", argv);
    boolean cache_p = OKBCApp.argv_suppliedp("CACHE", argv);
    if (break_p) { Condition.listener_on_error_p = true; }
    boolean verbose_p = !OKBCApp.argv_suppliedp("QUIET", argv);
    violations = 0;
    Symbol test_name = Symbol.intern("TAXA", "OKBC");
    TupleKb kb = (TupleKb)Test.create_taxa_kb(cache_p);
    kb.goto_kb();

    Node proc = kb.create_procedure(new OKBCString("(x y)"),
				    new OKBCString("(> x y)"));
    Node form1 = Cons.list(Node._and,
			   Cons.list(_equals_sign, _y_var,
				     new OKBCInteger(932)),
			   Cons.list(proc, new OKBCInteger(42), _y_var));
    Node form2 = Cons.list(Node._and,
			   Cons.list(_equals_sign, _y_var,
				     new OKBCInteger(32)),
			   Cons.list(proc, new OKBCInteger(42), _y_var));
    System.out.println("Form1 = " + form1);
    System.out.println("Res1  = " + kb.ask(form1, Node._T));
    System.out.println("Form2 = " + form2);
    System.out.println("Res2  = " + kb.ask(form2, Node._T));


    Procedure position_proc =
      (Procedure)kb.create_procedure(new OKBCString("(thing list pos)"),
				     new OKBCString(
"  (if (variablep list) \n" +
"      (error 'generic-error :error-message \"List cannot be a variable.\") \n"+
"      (if (variablep thing) \n" +
"	  (if (variablep pos) \n" +
"	      (let ((index -1)) \n" +
"		(do-list (elt list) \n" +
"		  (setq index (+ 1 index)) \n" +
"		  (list (list pos index) \n" +
"			(list thing elt)))) \n" +
"	      (if (and (>= pos 0) (< pos (length list))) \n" +
"		  (list (list (list thing (nth pos list)))) \n" +
"		  :fail)) \n" +
"	  (let ((index 0) \n" +
"		(found-indices nil)) \n" +
"	    (do-list (elt list) \n" +
"	      (if (equal thing elt) \n" +
"		  (push (list pos index) found-indices) \n" +
"		  nil) \n" +
"	      (setq index (+ index 1))) \n" +
"	    (if found-indices \n" +
"		(list found-indices) \n" +
"		:fail))))"));

    Node form;
    Node one = new OKBCInteger(1);
    Node two = new OKBCInteger(2);
    Node three = new OKBCInteger(3);
    Node four = new OKBCInteger(4);
    Node five = new OKBCInteger(5);
    Node list = Cons.list(two, one, three, four, five, one);

    Node form3 = Cons.list(_equals_sign, one, new OKBCInteger(1));
    System.out.println("Form3 = " + form3);
    System.out.println
      ("Form3 Res  = " + kb.ask(form3, form3).firstValue());


    form = Cons.list(position_proc, _x_var, list, two);
    System.out.println("Args = " + form.cdr());
    System.out.println("Res  = " + kb.ask(form, _x_var).firstValue());
    form = Cons.list(position_proc, five, list, _x_var);
    System.out.println("Args = " + form.cdr());
    System.out.println("Res  = " + kb.ask(form, _x_var).firstValue());
    form = Cons.list(position_proc, _y_var, list, _x_var);
    System.out.println("Args = " + form.cdr());
    System.out.println
      ("Res  = " + kb.ask(form, Cons.list(_x_var, _y_var)).firstValue());

    test_tell_and_ask(kb, verbose_p);

    // Print out the facts in the KB.
    if (facts_p) { print_facts(kb); }
    if (verbose_p)
      { System.out.println();
	System.out.println("Finished testing. Violations: " + violations); 
      } 
  }

}
