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


public abstract class AbstractSubExpressionWalker
  extends MostAbstractSubWalker {

  static final Symbol _bound_variable = Symbol.keyword("BOUND-VARIABLE");
  static final Symbol _cons_as_lisp_object =
    Symbol.keyword("CONS-AS-LISP-OBJECT");
  static final Symbol _constant = Symbol.keyword("CONSTANT");
  static final Symbol _constant_list = Symbol.keyword("CONSTANT-LIST");
  static final Symbol _definition = Symbol.keyword("DEFINITION");
  static final Symbol _equation = Symbol.keyword("EQUATION");
  static final Symbol _free_variable = Symbol.keyword("FREE-VARIABLE");
  static final Symbol _function_constant =
    Symbol.keyword("FUNCTION-CONSTANT");
  static final Symbol _function_definition =
    Symbol.keyword("FUNCTION-DEFINITION");
  static final Symbol _functional_term = Symbol.keyword("FUNCTIONAL-TERM");
  static final Symbol _generalized_string =
    Symbol.keyword("GENERALIZED-STRING");
  static final Symbol _inequality = Symbol.keyword("INEQUALITY");
  static final Symbol _kif = Symbol.keyword("KIF");
  static final Symbol _kif_operator = Symbol.keyword("KIF-OPERATOR");
  static final Symbol _lisp_object = Symbol.keyword("LISP-OBJECT");
  static final Symbol _list_term = Symbol.keyword("LIST-TERM");
  static final Symbol _logical_constant = Symbol.keyword("LOGICAL-CONSTANT");
  static final Symbol _logical_sentence = Symbol.keyword("LOGICAL-SENTENCE");
  static final Symbol _logical_term = Symbol.keyword("LOGICAL-TERM");
  static final Symbol _number = Symbol.keyword("NUMBER");
  static final Symbol _object_constant = Symbol.keyword("OBJECT-CONSTANT");
  static final Symbol _object_definition = Symbol.keyword("OBJECT-DEFINITION");
  static final Symbol _okbc = Symbol.keyword("OKBC");
  static final Symbol _optional_term = Symbol.keyword("OPTIONAL-TERM");
  static final Symbol _premise = Symbol.keyword("PREMISE");
  static final Symbol _primitive_lisp_object =
    Symbol.keyword("PRIMITIVE-LISP-OBJECT");
  static final Symbol _quantified_sentence =
    Symbol.keyword("QUANTIFIED-SENTENCE");
  static final Symbol _quantified_term = Symbol.keyword("QUANTIFIED-TERM");
  static final Symbol _quotation_term = Symbol.keyword("QUOTATION-TERM");
  static final Symbol _relation_constant =
    Symbol.keyword("RELATIONAL-CONSTANT");
  static final Symbol _relation_definition =
    Symbol.keyword("RELATION-DEFINITION");
  static final Symbol _relational_sentence =
    Symbol.keyword("RELATIONAL-SENTENCE");
  static final Symbol _rule = Symbol.keyword("RULE");
  static final Symbol _sentence = Symbol.keyword("SENTENCE");
  static final Symbol _set_term = Symbol.keyword("SET-TERM");
  static final Symbol _string = Symbol.keyword("STRING");
  static final Symbol _symbol = Symbol.keyword("SYMBOL");
  static final Symbol _term = Symbol.keyword("TERM");
  static final Symbol _term_list = Symbol.keyword("TERM-LIST");
  static final Symbol _variable = Symbol.keyword("VARIABLE");
  static final Symbol _variable_list = Symbol.keyword("VARIABLE-LIST");

  AbstractKifWalker walker = null;
  void set_walker (AbstractKifWalker the_walker) {
    walker = the_walker;
  }
  
  public AbstractSubExpressionWalker get_walker(Node key) {
    return walker.get(key);
  }
  
  AbstractKeyValueWalker key_and_value_walker = null;

  void set_key_and_value_walker (AbstractKeyValueWalker the_walker) {
    key_and_value_walker = the_walker;
  }
  
  public AbstractSubKeyValueWalker get_key_and_value_walker(Node key) {
    return key_and_value_walker.get(key);
  }
  
  public Node walk_subexpression (Kb kb, Node known_context_type,
				  Node bound_vars, Node expression,
				  AbstractWalkFunction function_to_apply)
  {
    walker.add_depth();
    if (walker.trace_p)
      {
	System.out.println();
	for (int i=1; i < walker.depth ; i++) {
	  System.out.print(" ");
	}
	System.out.print(walker.depth +
			 " Enter walk: " + this.getClass().getName() +
			 " " + expression);
      }
    Node result = walk_subexpression_1
      (kb, known_context_type, bound_vars, expression, function_to_apply);
    if (walker.trace_p)
      {
	System.out.println();
	for (int i=1; i < walker.depth ; i++) {
	  System.out.print(" ");
	}
	System.out.print(walker.depth +
			 " Exit walk:  " + this.getClass().getName() +
			 " " + expression + " -> " + result);
      }
    walker.decrement_depth();
    return result;
  }

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    throw new GenericError
      (Node._T, new OKBCString("Missing walk_subexpression method for: " +
			  this)).signal();
  }

  public Node walk_key_and_value_plist (Kb kb, Node known_context_type,
					Node bound_vars, Node expression,
					AbstractWalkFunction function_to_apply)
  {
    Node root = Node._NIL;
    Cons tail = null;
    Enumerator e = expression.enumerate_list();
    while (e.has_more_p()) {
      Node key = e.next();
      Node value = e.next();
      Node this_key = get_walker(_lisp_object).walk_subexpression
	(kb, known_context_type, bound_vars, key, function_to_apply);
      if (this_key == Node.__subclass_of)
	{ this_key = Node._direct_superclasses; }
      if (this_key == Node.__instance_of)
	{ this_key = Node._direct_types; }
      Node this_value = get_key_and_value_walker(key).walk_key_and_value_pair
	(kb, key, known_context_type, bound_vars, value, function_to_apply);
      if (tail == null)
	{ Node new_tail = Cons.list(this_key, this_value);
	  tail = (Cons)(new_tail.cdr());
	  root = new_tail;
	}
      else { Cons new_tail = (Cons)(Cons.list(this_key, this_value));
	     tail.set_cdr(new_tail);
	     tail = (Cons)(new_tail.cdr());
	     // System.out.println("--- New tail = " + new_tail);
	   }
    }
    return root;
  }

  Node walk_maybe_defaulted_values (Kb kb, Node known_context_type,
				    Node bound_vars, Node value,
				    AbstractWalkFunction function_to_apply) {
    Node new_list = Node._NIL;
    Cons tail = null;
    Enumerator e = value.enumerate_list();
    while (e.has_more_p()) {
      Node val = e.next();
      Node new_val = null;
      if (val instanceof Cons && val.car() == Node._default)
	{ new_val = Cons.list(get_walker(_lisp_object).walk_subexpression
			      (kb, known_context_type, bound_vars, val.car(), 
			       function_to_apply),
			      get_walker(_term).walk_subexpression
			      (kb, known_context_type, bound_vars, val.cadr(), 
			       function_to_apply));
	}
      else 
	{ new_val = get_walker(_term).walk_subexpression
	    (kb, known_context_type, bound_vars, val, function_to_apply);
	}
      if (tail == null)
	{ tail = new Cons(new_val);
	new_list = tail;
	}
      else { Cons new_tail = new Cons(new_val);
      tail.set_cdr(new_tail);
      tail = new_tail;
      }
    }
    return new_list;
  }

  void parser_warn (String string) {
    walker.parser_warn(string);
  }

  boolean listp_or_kif_nil (Node x) {
    return
      x == Node._NIL ||
      x instanceof Cons ||
      (x instanceof AbstractSymbol
       && AbstractSymbol.stringEquals((AbstractSymbol)x, "NIL"));
  }
       
  Node list_or_kif (Node x) {
    if (x == Node._NIL ||
	(x instanceof AbstractSymbol
	 && AbstractSymbol.stringEquals((AbstractSymbol)x, "NIL")))
      { return Node._NIL; }
    else { return x; }
  }
       
  Node term_type_of_operator
  (Node operator, Node expression, Kb kb, boolean barf_p) {
    if (walker.logical_term_operator_p(operator) != Node._NIL)
      { return _logical_term; }
    else if (walker.quotation_operator_p(operator) != Node._NIL)
      { return _quotation_term; }
    else if (walker.quantified_term_operator_p(operator) != Node._NIL)
      { return _quantified_term; }
    else if (walker.list_operator_p(operator) != Node._NIL)
      { return _list_term; }
    else if (walker.set_operator_p(operator) != Node._NIL)
      { return _set_term; }
    else if (walker.function_constant_p(operator, kb))
      { return _functional_term; }
    else if (barf_p)
      { parser_warn("The form " + expression + " is not a legal term.");
	return _functional_term;
      }
    else { return Node._NIL; }
  }

  Node canonical_term_type_of_operator
  (Node operator, Node expression, Kb kb, boolean barf_p) {
    if (walker.logical_term_operator_p(operator) != Node._NIL)
      { return walker.logical_term_operator_p(operator); }
    else if (walker.quotation_operator_p(operator) != Node._NIL)
      { return walker.quotation_operator_p(operator); }
    else if (walker.quantified_term_operator_p(operator) != Node._NIL)
      { return walker.quantified_term_operator_p(operator); }
    else if (walker.list_operator_p(operator) != Node._NIL)
      { return walker.list_operator_p(operator); }
    else if (walker.set_operator_p(operator) != Node._NIL)
      { return walker.set_operator_p(operator); }
    else if (walker.function_constant_p(operator,kb))
      { return operator; }
    else if (barf_p)
      { parser_warn("The form " + expression + " is not a legal term.");
	return _functional_term;
      }
    else { return Node._NIL; }
  }

}
