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


class GenericProductionSubExpressionWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class PrimitiveLispObjectWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class KifWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (expression instanceof Cons)
      {
	Node op = expression.car();
	if (walker.rule_operator_p(op) != Node._NIL)
	  { new_expression =
	  get_walker(_rule).walk_subexpression
	  (kb, known_context_type, bound_vars,
	   new Cons(walker.rule_operator_p(op), expression.cdr()),
	   function_to_apply);
	  }
	else if (walker.definition_operator_p(op) != Node._NIL)
	  { new_expression =
	    get_walker(_rule).walk_subexpression
	    (kb, known_context_type, bound_vars,
	     new Cons(walker.definition_operator_p(op), expression.cdr()),
	     function_to_apply);
	  }
	else
	  {
	    Node key
	      = term_type_of_operator(op, expression, kb, false);
	    if (key != null && key != _functional_term)
	      {
		new_expression =
		  get_walker(_term).walk_subexpression
		  (kb, known_context_type, bound_vars,
		   new Cons(canonical_term_type_of_operator
			    (op, expression, kb, true),
			    expression.cdr()), function_to_apply);
	      }
	    else if (op instanceof AbstractSymbol ||
		     op.frame_reference_p(kb))
	      { new_expression =
		  get_walker(_sentence).walk_subexpression
		  (kb, known_context_type, bound_vars,
		   expression, function_to_apply);
	      }
	    else
	      { parser_warn("The form " + expression +
			    " is not a legal KIF sentence.  ");
		new_expression =
		  get_walker(_lisp_object).walk_subexpression
		  (kb, known_context_type, bound_vars, expression,
		   function_to_apply);
	      }
	  }
      }
    else if (expression instanceof AbstractSymbol ||
	     expression.frame_reference_p(kb))
      { // We know this must be a logical constant, 
	// but it is playing the part of a propositional sentence...
	// (which passes it through to logical constant, 
	// which traps illegal symbols used as sentences.)
	new_expression =
	  get_walker(_sentence).walk_subexpression
	  (kb, known_context_type, bound_vars, expression, function_to_apply);
      }
    else
      { parser_warn("The form " + expression +
		    " is not a legal top-level form for KIF.  " +
		    "It may be intended as a term.");
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression, function_to_apply);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}

class SentenceWalker extends AbstractSubExpressionWalker {

  Node sentence_op_type (Node op, Node expression, Kb kb, boolean warn_p) {
    if (walker.logical_sentence_operator_p(op) != Node._NIL)
      { return _logical_sentence; }
    else if (walker.sentence_quantification_operator_p(op) != Node._NIL)
      { return _quantified_sentence; }
    else if (walker.equality_operator_p(op) != Node._NIL) { return _equation; }
    else if (walker.inequality_operator_p(op) != Node._NIL)
      { return _inequality; }
    else if (walker.relation_constant_p(op, kb))
      { return _relational_sentence; }
    else if (walker.variables_ok_as_relation_expression_functors &&
	     TupleStore.variable_p(op))
      { return _relational_sentence; }
    else 
      { if (warn_p)
	  { parser_warn("The head of a KIF sentence must be an appropriate " +
			"symbol.  " + op + " is not legal.");
	  }
	return Node._NIL;
      }
  }
  
  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (expression instanceof Cons)
      {
	Node op = expression.car();
	Node op_type = sentence_op_type(op, expression, kb, false);
	if (op_type != Node._NIL)
	  { new_expression =
	    get_walker(op_type).walk_subexpression
	    (kb, known_context_type, bound_vars, expression, function_to_apply);
	  }
	else
	  { parser_warn("The head of a KIF sentence must be an appropriate " +
			"symbol.  " + op + " is not legal.");
	    new_expression =
	      get_walker(_lisp_object).walk_subexpression
	      (kb, known_context_type, bound_vars, expression,
	       function_to_apply);
	  }
      }
    else
      { new_expression =
	  get_walker(_logical_constant).walk_subexpression
	  (kb, known_context_type, bound_vars, expression, function_to_apply);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class LogicalConstantWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (!(walker.logical_constant_p(expression, kb)))
      { parser_warn(expression + " should be a logical constant."); }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class RelationalSentenceWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn("This form is not a relational sentence: " + expression);
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node relation_context_type = known_context_type;
	int len = expression.cdr().length();
	if (len == 1) { relation_context_type = Node._class; }
	else if (len == 2) { relation_context_type = Node._slot; }
	Node new_car = null;
	if (walker.variables_ok_as_relation_expression_functors &&
	    TupleStore.variable_p(expression.car()))
	  { new_car = get_walker(_variable).walk_subexpression
	    (kb, relation_context_type, bound_vars, expression.car(),
	     function_to_apply);
	  }
	else
	  { new_car = get_walker(_relation_constant).walk_subexpression
	    (kb, relation_context_type, bound_vars, expression.car(),
	     function_to_apply);
	  }
	new_expression =
	  new Cons(new_car,
		   get_walker(_term_list).walk_subexpression
		   (kb, known_context_type, bound_vars, expression.cdr(),
		    function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class EquationWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons) ||
	expression.length() != 3 ||
	walker.equality_operator_p(expression.car()) != Node._NIL)
      { parser_warn("This form is not a legal equation: " + expression);
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	new_expression =
	  Cons.list(get_walker(_kif_operator).walk_subexpression
		    (kb, known_context_type, bound_vars, expression.car(),
		     function_to_apply),
		    get_walker(_term).walk_subexpression
		    (kb, known_context_type, bound_vars, expression.cadr(),
		     function_to_apply),
		    get_walker(_term).walk_subexpression
		    (kb, known_context_type, bound_vars, expression.nth(2),
		     function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class InequalityWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons) ||
	expression.length() != 3 ||
	walker.inequality_operator_p(expression.car()) != Node._NIL)
      { parser_warn("This form is not a legal inequality: " + expression);
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	new_expression =
	  Cons.list(get_walker(_kif_operator).walk_subexpression
		    (kb, known_context_type, bound_vars, expression.car(),
		     function_to_apply),
		    get_walker(_term).walk_subexpression
		    (kb, known_context_type, bound_vars, expression.cadr(),
		     function_to_apply),
		    get_walker(_term).walk_subexpression
		    (kb, known_context_type, bound_vars, expression.nth(2),
		     function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class LogicalSentenceWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons) ||
	!(expression.car() instanceof AbstractSymbol))
      { parser_warn("This form is not a logical sentence: " + expression);
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	int sentence_count = expression.cdr().length();
	AbstractSymbol op = (AbstractSymbol)(expression.car());
	if ((op.stringEquals(Node._not) && sentence_count != 1) ||
	    (op.stringEquals(Node._iff) && sentence_count != 2) ||
	    (op.stringEquals(Node._implies) && sentence_count < 2) ||
	    (op.stringEquals(Node._backwards_implies) && sentence_count < 2) ||
	    (op.stringEquals(Node._and) && sentence_count < 1) ||
	    (op.stringEquals(Node._or) && sentence_count < 1))
	  { parser_warn("Wrong number of sentences after the " + op + " in "
			+ expression);
	  }
	Node new_car = get_walker(_kif_operator).walk_subexpression
	  (kb, known_context_type, bound_vars, op, function_to_apply);
	Node new_cdr = Node._NIL;
	Cons tail = null;
	Enumerator e = expression.cdr().enumerate_list();
	while (e.has_more_p()) {
	  Node elt = e.next();
	  Node this_res = get_walker(_sentence).walk_subexpression
	    (kb, known_context_type, bound_vars, elt,
	     function_to_apply);
	  if (tail == null)
	    { tail = new Cons(this_res);
	      new_cdr = tail;
	    }
	  else { Cons new_tail = new Cons(this_res);
		 tail.set_cdr(new_tail);
		 tail = new_tail;
	       }
	}
	new_expression = new Cons(new_car, new_cdr);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class VariableListWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn("This form is not a variable list: " + expression);
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	new_expression = Node._NIL;
	Cons tail = null;
	Enumerator e = expression.enumerate_list();
	while (e.has_more_p()) {
	  Node elt = e.next();
	  Node this_res = get_walker(_variable).walk_subexpression
	    (kb, known_context_type, bound_vars, elt,
	     function_to_apply);
	  if (tail == null)
	    { tail = new Cons(this_res);
	      new_expression = tail;
	    }
	  else { Cons new_tail = new Cons(this_res);
		 tail.set_cdr(new_tail);
		 tail = new_tail;
	       }
	}
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class TermListWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!listp_or_kif_nil(expression))
      { parser_warn("This form is not a term list: " + expression);
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	new_expression = Node._NIL;
	Cons tail = null;
	Enumerator e = list_or_kif(expression).enumerate_list();
	while (e.has_more_p()) {
	  Node elt = e.next();
	  Node this_res = get_walker(_term).walk_subexpression
	    (kb, known_context_type, bound_vars, elt,
	     function_to_apply);
	  if (tail == null)
	    { tail = new Cons(this_res);
	      new_expression = tail;
	    }
	  else { Cons new_tail = new Cons(this_res);
		 tail.set_cdr(new_tail);
		 tail = new_tail;
	       }
	}
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class ConstantListWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!listp_or_kif_nil(expression))
      { parser_warn("This form is not a constant list: " + expression);
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	new_expression = Node._NIL;
	Cons tail = null;
	Enumerator e = list_or_kif(expression).enumerate_list();
	while (e.has_more_p()) {
	  Node elt = e.next();
	  Node this_res = get_walker(_constant).walk_subexpression
	    (kb, known_context_type, bound_vars, elt,
	     function_to_apply);
	  if (tail == null)
	    { tail = new Cons(this_res);
	      new_expression = tail;
	    }
	  else { Cons new_tail = new Cons(this_res);
		 tail.set_cdr(new_tail);
		 tail = new_tail;
	       }
	}
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class QuantifiedSentenceWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons) ||
	expression.length() != 3 ||
	walker.sentence_quantification_operator_p(expression.car())
	!= Node._NIL)
      { parser_warn("This form is not a legal quantified sentence: "
		    + expression);
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node sentence = expression.nth(2);
	if (expression.length() > 3)
	  { parser_warn(expression.car() +
			" expressions should contain only a single sentence.  "
			+ "  Multiple sentences in this expression will be " +
			"conjunctively combined:" + expression);
	    sentence = new Cons(Node._and,
				Cons.nthcdr(2, expression));
	  }
	Node arg_list = expression.cadr();
	Node new_vars = arg_list;
	if (arg_list instanceof AbstractSymbol)
	  { new_vars = Cons.list(arg_list); }
	new_expression =
	  Cons.list(get_walker(_kif_operator).walk_subexpression
		    (kb, known_context_type, bound_vars,
		     walker.sentence_quantification_operator_p
		     (expression.car()),
		     function_to_apply),
		    get_walker(_variable_list).walk_subexpression
		    (kb, known_context_type, bound_vars, new_vars,
		     function_to_apply),
		    get_walker(_sentence).walk_subexpression
		    (kb, known_context_type,
		     Cons.append_cons(new_vars, bound_vars), sentence,
		     function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}

class RuleWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " is not a KIF rule");
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node op = walker.rule_operator_p(expression.car());
	Node new_premises = Node._NIL;
	Cons tail = null;
	if (Node._angangeq.stringEquals(op))
	  { Node new_op = get_walker(_kif_operator).walk_subexpression
	      (kb, known_context_type, bound_vars, op,
	       function_to_apply);
	    Node new_sentence = get_walker(_sentence).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.cadr(),
	       function_to_apply);
	    Enumerator e = expression.cdr().cdr().enumerate_list();
	    while (e.has_more_p()) {
	      Node premise = e.next();
	      Node this_res = get_walker(_premise).walk_subexpression
		(kb, known_context_type, bound_vars, premise,
		 function_to_apply);
	      if (tail == null)
		{ tail = new Cons(this_res);
		  new_premises = tail;
		}
	      else { Cons new_tail = new Cons(this_res);
		     tail.set_cdr(new_tail);
		     tail = new_tail;
		   }
	    }
	    new_expression =
	      new Cons(new_op, new Cons(new_sentence, new_premises));
	  }
	else if (Node._eqangang.stringEquals(op))
	  { Node new_op = get_walker(_kif_operator).walk_subexpression
	      (kb, known_context_type, bound_vars, op,
	       function_to_apply);
	    Enumerator e = expression.cdr().enumerate_list();
	    Node loc = expression;
	    while (e.has_more_p()) {
	      Node premise = e.next();
	      Node this_res = get_walker(_premise).walk_subexpression
		(kb, known_context_type, bound_vars, premise,
		 function_to_apply);
	      loc = loc.cdr();
	      if (tail == null)
		{ tail = new Cons(this_res);
		  new_premises = tail;
		}
	      else { Cons new_tail = new Cons(this_res);
		     tail.set_cdr(new_tail);
		     tail = new_tail;
		   }
	      if (!(loc.cdr() instanceof Cons)) { break; }
	    }
	    Node new_sentence = get_walker(_sentence).walk_subexpression
	      (kb, known_context_type, bound_vars, Cons.last(expression).car(),
	       function_to_apply);
	    new_expression =
	      new Cons(new_op, Cons.append_cons(new_premises,
						Cons.list(new_sentence)));
	  }
	else
	  { parser_warn(expression + " is not a KIF rule");
	    new_expression =
	      get_walker(_lisp_object).walk_subexpression
	      (kb, known_context_type, bound_vars, expression,
	       function_to_apply);
	  }
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class PremiseWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (expression instanceof Cons &&
	expression.car() instanceof AbstractSymbol &&
	Node._consis.stringEquals(expression.car()))
      {
	Node new_consis = expression.car();
	if (walker.replace_kif_symbols_with_keywords_p)
	  { new_consis = Node._consis; }
	new_expression =
	  Cons.list(get_walker(_kif_operator).walk_subexpression
		    (kb, known_context_type, bound_vars, new_consis,
		     function_to_apply),
		    get_walker(_sentence).walk_subexpression
		    (kb, known_context_type, bound_vars, expression.cadr(),
		     function_to_apply));
      }
    else
      {
	new_expression =
	  get_walker(_sentence).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class DefinitionWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    Node key = null;
    if (Node._defobject.stringEquals(expression.car()))
      { key = _object_definition; }
    else if (Node._deffunction.stringEquals(expression.car()))
      { key = _function_definition; }
    else if (Node._defrelation.stringEquals(expression.car()))
      { key = _relation_definition; }
    else
      { parser_warn(expression.car() + " is not a legal definition key");
	key = _lisp_object;
      }
    new_expression = get_walker(key).walk_subexpression
      (kb, known_context_type, bound_vars, expression,
       function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class ObjectDefinitionWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons &&
	  Node._defobject.stringEquals(expression.car())))
      { parser_warn(expression + " is not a KIF object definition");
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node new_op = get_walker(_kif_operator).walk_subexpression
	  (kb, known_context_type, bound_vars, expression.car(),
	   function_to_apply);
	Node processed_constant =
	  get_walker(_object_constant).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.cadr(),
	       function_to_apply);
	if (expression.nth(2) instanceof AbstractSymbol &&
	    Node._equals_sign.stringEquals(expression.nth(2)))
	  { Node equal_node = expression.nth(2);
	    if (walker.replace_kif_symbols_with_keywords_p)
	      { equal_node = Node._equals_sign; }
	    new_expression =
	      Cons.list(new_op,
			processed_constant,
			get_walker(_kif_operator).walk_subexpression
			(kb, known_context_type, bound_vars, equal_node,
			 function_to_apply),
			get_walker(_term).walk_subexpression
			(kb, known_context_type, bound_vars, expression.nth(3),
			 function_to_apply));
	  }
	else if (expression.nth(2) instanceof AbstractSymbol &&
		 Node._conservative_axiom.stringEquals(expression.nth(2)))
	  { Node conserv_ax_node = expression.nth(2);
	    if (walker.replace_kif_symbols_with_keywords_p)
	      { conserv_ax_node = Node._conservative_axiom; }
	    new_expression =
	      Cons.list(new_op,
			processed_constant,
			get_walker(_kif_operator).walk_subexpression
			(kb, known_context_type, bound_vars, conserv_ax_node,
			 function_to_apply),
			get_walker(_sentence).walk_subexpression
			(kb, known_context_type, bound_vars, expression.nth(3),
			 function_to_apply));
	  }
	else
	  {
	    Node new_sentences = Node._NIL;
	    Cons tail = null;
	    Node new_sentence = get_walker(_sentence).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.cadr(),
	       function_to_apply);
	    Enumerator e = expression.cdr().cdr().enumerate_list();
	    while (e.has_more_p()) {
	      Node sentence = e.next();
	      Node this_res = get_walker(_sentence).walk_subexpression
		(kb, known_context_type, bound_vars, sentence,
		 function_to_apply);
	      if (tail == null)
		{ tail = new Cons(this_res);
		  new_sentences = tail;
		}
	      else { Cons new_tail = new Cons(this_res);
		     tail.set_cdr(new_tail);
		     tail = new_tail;
		   }
	    }
	    new_expression =
	      new Cons(new_op, new Cons(processed_constant, new_sentences));
	  }
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class FunctionDefinitionWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons &&
	  Node._deffunction.stringEquals(expression.car())))
      { parser_warn(expression + " is not a KIF function definition");
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node new_op = get_walker(_kif_operator).walk_subexpression
	  (kb, known_context_type, bound_vars, expression.car(),
	   function_to_apply);
	Node processed_constant =
	  get_walker(_function_constant).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.cadr(),
	       function_to_apply);
	Node third = expression.nth(2);
	if (third instanceof AbstractSymbol &&
	    Node._conservative_axiom.stringEquals(expression.nth(2)))
	  { Node conserv_ax_node = expression.nth(2);
	    if (walker.replace_kif_symbols_with_keywords_p)
	      { conserv_ax_node = Node._conservative_axiom; }
	    new_expression =
	      Cons.list(new_op,
			processed_constant,
			get_walker(_kif_operator).walk_subexpression
			(kb, known_context_type, bound_vars, conserv_ax_node,
			 function_to_apply),
			get_walker(_sentence).walk_subexpression
			(kb, known_context_type, bound_vars, expression.nth(3),
			 function_to_apply));
	  }
	else if (third instanceof Cons &&
		 TupleStore.variable_p(third.car()))
	  { Node conserv_ax_node = expression.nth(2);
	    if (walker.replace_kif_symbols_with_keywords_p)
	      { conserv_ax_node = Node._conservative_axiom; }
	    new_expression =
	      Cons.list(new_op,
			processed_constant,
			get_walker(_variable_list).walk_subexpression
			(kb, known_context_type, bound_vars, third,
			 function_to_apply),
			get_walker(_term).walk_subexpression
			(kb, known_context_type,
			 Cons.append_cons(third, bound_vars), expression.nth(4),
			 function_to_apply));
	  }
	else
	  {
	    Node new_sentences = Node._NIL;
	    Cons tail = null;
	    Enumerator e = expression.cdr().cdr().enumerate_list();
	    while (e.has_more_p()) {
	      Node sentence = e.next();
	      Node this_res = get_walker(_sentence).walk_subexpression
		(kb, known_context_type, bound_vars, sentence,
		 function_to_apply);
	      if (tail == null)
		{ tail = new Cons(this_res);
		  new_sentences = tail;
		}
	      else { Cons new_tail = new Cons(this_res);
		     tail.set_cdr(new_tail);
		     tail = new_tail;
		   }
	    }
	    new_expression =
	      new Cons(new_op, new Cons(processed_constant, new_sentences));
	  }
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class RelationDefinitionWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons &&
	  Node._defrelation.stringEquals(expression.car())))
      { parser_warn(expression + " is not a KIF relation definition");
	new_expression =
	  get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      {
	Node new_op = get_walker(_kif_operator).walk_subexpression
	  (kb, known_context_type, bound_vars, expression.car(),
	   function_to_apply);
	Node processed_constant =
	  get_walker(_relation_constant).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.cadr(),
	       function_to_apply);
	Node third = expression.nth(2);
	if (third instanceof AbstractSymbol &&
	    Node._conservative_axiom.stringEquals(expression.nth(2)))
	  { Node conserv_ax_node = expression.nth(2);
	    if (walker.replace_kif_symbols_with_keywords_p)
	      { conserv_ax_node = Node._conservative_axiom; }
	    new_expression =
	      Cons.list(new_op,
			processed_constant,
			get_walker(_kif_operator).walk_subexpression
			(kb, known_context_type, bound_vars, conserv_ax_node,
			 function_to_apply),
			get_walker(_sentence).walk_subexpression
			(kb, known_context_type, bound_vars, expression.nth(3),
			 function_to_apply));
	  }
	else if (third instanceof Cons &&
		 TupleStore.variable_p(third.car()))
	  {
	    Node fourth = expression.nth(3);
	    if (fourth instanceof AbstractSymbol &&
		(Node._equals_sign.stringEquals(fourth) ||
		 Node._implies.stringEquals(fourth)))
	      {}
	    else { parser_warn("Illegal syntax in relation definition around: "
			       + fourth);
		 }
	    Node fourth_op = fourth;
	    if (walker.replace_kif_symbols_with_keywords_p)
	      { if (Node._equals_sign.stringEquals(fourth))
		  { fourth_op = Node._equals_sign; }
		else if (Node._implies.stringEquals(fourth))
		  { fourth_op = Node._implies; }
		else {}
	      }
	    Node sixth = expression.nth(5);
	    Node processed_sixth = Node._NIL;
	    if (sixth != Node._NIL && sixth != null)
	      {
		Node car = null;
		if (sixth instanceof AbstractSymbol &&
		    Node._axiom.stringEquals(sixth))
		  { car = (walker.replace_kif_symbols_with_keywords_p
			   ? Node._axiom
			   : sixth);
		    car = get_walker(_kif_operator).walk_subexpression
		      (kb, known_context_type, bound_vars, car,
		       function_to_apply);
		  }
		else
		  {
		    parser_warn("Illegal syntax in relation definition around: "
				+ sixth);
		    car = get_walker(_lisp_object).walk_subexpression
		      (kb, known_context_type, bound_vars, sixth,
		       function_to_apply);
		  }
		processed_sixth =
		  Cons.list(car, get_walker(_sentence).walk_subexpression
			    (kb, known_context_type, bound_vars,
			     expression.nth(6), function_to_apply));
	      }
	    new_expression =
	      Cons.append_cons
	      (Cons.list(new_op,
			 processed_constant,
			 get_walker(_variable_list).walk_subexpression
			 (kb, known_context_type, bound_vars, third,
			  function_to_apply),
			 get_walker(_kif_operator).walk_subexpression
			 (kb, known_context_type, bound_vars, fourth_op,
			  function_to_apply),
			 get_walker(_sentence).walk_subexpression
			 (kb, known_context_type,
			  Cons.append_cons(third, bound_vars),
			  expression.nth(4),
			  function_to_apply)),
	       processed_sixth);
	  }
	else
	  {
	    Node new_sentences = Node._NIL;
	    Cons tail = null;
	    Enumerator e = expression.cdr().cdr().enumerate_list();
	    while (e.has_more_p()) {
	      Node sentence = e.next();
	      Node this_res = get_walker(_sentence).walk_subexpression
		(kb, known_context_type, bound_vars, sentence,
		 function_to_apply);
	      if (tail == null)
		{ tail = new Cons(this_res);
		  new_sentences = tail;
		}
	      else { Cons new_tail = new Cons(this_res);
		     tail.set_cdr(new_tail);
		     tail = new_tail;
		   }
	    }
	    new_expression =
	      new Cons(new_op, new Cons(processed_constant, new_sentences));
	  }
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class TermWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (expression instanceof Cons)
      {
	Node operator = expression.car();
	Node key = term_type_of_operator (operator, expression, kb, false);
	new_expression = get_walker(key).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else if (expression instanceof AbstractSymbol)
      {
	Node key = _constant;
	if (Cons.member_eql(expression, bound_vars, kb, Node._NIL))
	  { key = _bound_variable; }
	else if (TupleStore.variable_p(expression))
	  { key = _free_variable; }
	new_expression = get_walker(key).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else if (expression instanceof FrameHandle)
      { new_expression = get_walker(_constant).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      { new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class OptionalTermWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    Node key = (listp_or_kif_nil(expression) ? _lisp_object : _term);
    new_expression = get_walker(key).walk_subexpression
      (kb, known_context_type, bound_vars, expression,
       function_to_apply);
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class ConstantWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (walker.object_constant_p(expression, kb) ||
	walker.function_constant_p(expression, kb) ||
	walker.relation_constant_p(expression, kb) ||
	walker.logical_constant_p(expression, kb))
      {}
    else { parser_warn(expression + " should be a KIF constant."); }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class VariableWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (TupleStore.variable_p(expression))
      {}
    else { parser_warn(expression + " should be a KIF variable."); }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class BoundVariableWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (TupleStore.variable_p(expression) &&
	Cons.member_eql(expression, bound_vars, kb, Node._NIL))
      {}
    else { parser_warn(expression + " should be a bound KIF variable."); }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class FreeVariableWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (TupleStore.variable_p(expression) &&
	!(Cons.member_eql(expression, bound_vars, kb, Node._NIL)))
      {}
    else { parser_warn(expression + " should be a free KIF variable."); }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class FunctionalTermWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " should be a funcitonal term.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      { Node new_car = null;
	if (walker.function_constant_p(expression.car(), kb))
	  {
	    if (expression.cdr().length() == 1)
	      { new_car = get_walker(_function_constant).walk_subexpression
		(kb, Node._slot, bound_vars, expression.car(),
		 function_to_apply);
	      }
	    else
	      { new_car = get_walker(_function_constant).walk_subexpression
		(kb, known_context_type, bound_vars, expression.car(),
		 function_to_apply);
	      }
	  }
	else
	  { parser_warn(expression.car() + " should be a function constant.");
	    new_car = get_walker(_lisp_object).walk_subexpression
	      (kb, known_context_type, bound_vars, expression,
	       function_to_apply);
	  }
	new_expression = new Cons(new_car,
				  get_walker(_term_list).walk_subexpression
				  (kb, known_context_type, bound_vars,
				   expression.cdr(), function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class QuotationTermWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " should be a quotation term.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      { Node new_car = null;
	if (walker.quotation_operator_p(expression.car()) != Node._NIL)
	  { new_car = get_walker(_kif_operator).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.car(),
	       function_to_apply);
	  }
	else
	  { parser_warn(expression.car() + " should be a QUOTE.");
	    new_car = get_walker(_lisp_object).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.car(),
	       function_to_apply);
	  }
	new_expression = Cons.list(new_car,
				   get_walker(_lisp_object).walk_subexpression
				   (kb, known_context_type, bound_vars,
				    expression.cadr(), function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class ListTermWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " should be a list term.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      { Node new_car = null;
	if (walker.list_operator_p(expression.car()) != Node._NIL)
	  { new_car = get_walker(_kif_operator).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.car(),
	       function_to_apply);
	  }
	else
	  { parser_warn(expression.car() + " should be LISTOF.");
	    new_car = get_walker(_lisp_object).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.car(),
	       function_to_apply);
	  }
	new_expression = new Cons(new_car,
				  get_walker(_term_list).walk_subexpression
				  (kb, known_context_type, bound_vars,
				   expression.cadr(), function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class SetTermWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " should be a set term.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      { Node new_car = null;
	if (walker.set_operator_p(expression.car()) != Node._NIL)
	  { new_car = get_walker(_kif_operator).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.car(),
	       function_to_apply);
	  }
	else
	  { parser_warn(expression.car() + " should be SETOF.");
	    new_car = get_walker(_lisp_object).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.car(),
	       function_to_apply);
	  }
	new_expression = new Cons(new_car,
				  get_walker(_term_list).walk_subexpression
				  (kb, known_context_type, bound_vars,
				   expression.cadr(), function_to_apply));
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class LogicalTermWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons))
      { parser_warn(expression + " should be a logical term.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      { Node new_car = null;
	if (walker.logical_term_operator_p(expression.car()) != Node._NIL)
	  { new_car = get_walker(_kif_operator).walk_subexpression
	      (kb, known_context_type, bound_vars,
	       walker.logical_term_operator_p(expression.car()),
	       function_to_apply);
	  }
	else
	  { parser_warn(expression.car() +
			" should be a logical term operator.");
	  }
	if (expression.car() instanceof AbstractSymbol &&
	    Node._if.stringEquals(expression.car()))
	  {
	    Node new_cadr = get_walker(_sentence).walk_subexpression
	      (kb, known_context_type, bound_vars, expression.cadr(),
	       function_to_apply);
	    Node new_third = null;
	    if (expression.nth(2) != Node._NIL)
	      { new_third = get_walker(_term).walk_subexpression
		(kb, known_context_type, bound_vars, expression.nth(2),
		 function_to_apply);
	      }
	    else
	      {
		parser_warn("The syntax of KIF's IF statement is " +
			    "(if <sentence> <term1> [<term2>]).  " +
			    "This sentence is missing <term1>: " +
			    expression);
		new_third = get_walker(_lisp_object).walk_subexpression
		(kb, known_context_type, bound_vars, expression.nth(2),
		 function_to_apply);
	      }
	    Node new_tail = Node._NIL;
	    if (expression.nth(3) != Node._NIL && expression.nth(3) != null)
	      { new_tail = Cons.list
		(get_walker(_term).walk_subexpression
		 (kb, known_context_type, bound_vars, expression.nth(3),
		  function_to_apply));
	      }
	    new_expression = Cons.append_cons
	      (Cons.list(new_car, new_cadr, new_third), new_tail);
	  }
	else if (expression.car() instanceof AbstractSymbol &&
		 Node._cond.stringEquals(expression.car()))
	  {
	    Node new_specs = Node._NIL;
	    Cons tail = null;
	    Enumerator e = expression.cdr().enumerate_list();
	    while (e.has_more_p()) {
	      Node sentence_dot_term = e.next();
	      Node this_res = null;
	      if (sentence_dot_term instanceof Cons &&
		  sentence_dot_term.length() == 2)
		{ Node sentence = sentence_dot_term.car();
		  Node term = sentence_dot_term.cadr();
		  Node new_sentence =
		    get_walker(_sentence).walk_subexpression
		    (kb, known_context_type, bound_vars, sentence,
		     function_to_apply);
		  Node new_term =
		    get_walker(_term).walk_subexpression
		    (kb, known_context_type, bound_vars, term,
		     function_to_apply);
		  this_res = Cons.list(new_sentence, new_term);
		}
	      else
		{
		  parser_warn("The syntax of KIF's COND statement is " +
			      "(cond [(<sentence> <term>)]*).  " +
			      sentence_dot_term + " does not match.");
		  this_res =
		  get_walker(_lisp_object).walk_subexpression
		  (kb, known_context_type, bound_vars, sentence_dot_term,
		   function_to_apply);
		}
	      if (tail == null)
		{ tail = new Cons(this_res);
		  new_specs = tail;
		}
	      else { Cons new_tail = new Cons(this_res);
		     tail.set_cdr(new_tail);
		     tail = new_tail;
		   }
	    }
	    new_expression = new Cons(new_car, new_specs);
	  }
	else
	  { new_expression = get_walker(_lisp_object).walk_subexpression
	      (kb, known_context_type, bound_vars, expression,
	       function_to_apply);
	  }
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class QuantifiedTermWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node new_expression = expression;
    if (!(expression instanceof Cons) ||
	expression.length() != 3)
      { parser_warn(expression + " should be a quantified term.");
	new_expression = get_walker(_lisp_object).walk_subexpression
	  (kb, known_context_type, bound_vars, expression,
	   function_to_apply);
      }
    else
      { Node new_car = null;
	if (walker.quantified_term_operator_p(expression.car()) != Node._NIL)
	  { new_car = get_walker(_kif_operator).walk_subexpression
	      (kb, known_context_type, bound_vars,
	       walker.quantified_term_operator_p(expression.car()),
	       function_to_apply);
	  }
	else
	  { parser_warn(expression.car() +
			" should be a quantified term operator.");
	  }
	Node arg1 = expression.cadr();
	Node arg2 = expression.nth(2);
	Node args = expression.cdr();
	if (expression.car() instanceof AbstractSymbol &&
	    (Node._the.stringEquals(expression.car()) ||
	     Node._setofall.stringEquals(expression.car())))
	  {
	    Node new_arg1 = get_walker(_term).walk_subexpression
	      (kb, known_context_type, bound_vars, arg1,
	       function_to_apply);
	    Node new_arg2 = get_walker(_sentence).walk_subexpression
	      (kb, known_context_type, bound_vars, arg2,
	       function_to_apply);
	    new_expression = Cons.list(new_car, new_arg1, new_arg2);
	  }
	else if (expression.car() instanceof AbstractSymbol &&
		 Node._setof.stringEquals(expression.car()))
	  {
	    Node new_args = get_walker(_term_list).walk_subexpression
	      (kb, known_context_type, bound_vars, args,
	       function_to_apply);
	    new_expression = new Cons(new_car, new_args);
	  }
	else if (expression.car() instanceof AbstractSymbol &&
		 (Node._lambda.stringEquals(expression.car()) ||
		  Node._kappa.stringEquals(expression.car())))
	  {
	    Node new_arg1 = get_walker(_variable_list).walk_subexpression
	      (kb, known_context_type, bound_vars, arg1,
	       function_to_apply);
	    Node new_arg2 = null;
	    if (Node._lambda.stringEquals(expression.car()))
	      { new_arg2 =
		get_walker(_term).walk_subexpression
		(kb, known_context_type, Cons.append_cons(arg1, bound_vars),
		 arg2, function_to_apply);
	      }
	    else
	      { new_arg2 =
		get_walker(_sentence).walk_subexpression
		(kb, known_context_type, Cons.append_cons(arg1, bound_vars),
		 arg2, function_to_apply);
	      }
	    new_expression = Cons.list(new_car, new_arg1, new_arg2);
	  }
	else
	  { new_expression = get_walker(_lisp_object).walk_subexpression
	    (kb, known_context_type, bound_vars, expression, function_to_apply);
	  }
      }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     new_expression);
  }
}


class FunctionConstantWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (walker.function_constant_p(expression, kb))
      {}
    else { parser_warn(expression + " should be a KIF function constant."); }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class RelationConstantWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (walker.relation_constant_p(expression, kb))
      {}
    else { parser_warn(expression + " should be a KIF relation constant."); }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class ObjectConstantWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (walker.object_constant_p(expression, kb))
      {}
    else { parser_warn(expression + " should be a KIF object constant."); }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     expression);
  }
}


class KifOperatorWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     get_walker(_lisp_object).walk_subexpression
				     (kb, known_context_type, bound_vars,
				      expression, function_to_apply));
  }
}


class LispObjectWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node key = null;
    if (expression instanceof OKBCInteger ||
	expression instanceof Real)
      { key = _number; }
    else if (expression instanceof Cons)
      { key = _cons_as_lisp_object; }
    else if (expression instanceof OKBCString)
      { key = _string; }
    else if (expression instanceof AbstractSymbol)
      { key = _symbol; }
    else { key = _primitive_lisp_object; }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     get_walker(key).walk_subexpression
				     (kb, known_context_type, bound_vars,
				      expression, function_to_apply));
  }
}


class NumberWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (expression instanceof OKBCInteger ||
	expression instanceof Real)
      { return function_to_apply.funcall
	(kb, this, known_context_type,
	 bound_vars,
	 get_walker(_primitive_lisp_object).walk_subexpression
	 (kb, known_context_type, bound_vars,
	  expression, function_to_apply));
      }
    else { parser_warn(expression + " should be a number.");
	   return expression;
	 }
  }
}


class ConsAsLispObjectWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (expression instanceof Cons)
      { return function_to_apply.funcall
	(kb, this, known_context_type,
	 bound_vars,
	 get_walker(_primitive_lisp_object).walk_subexpression
	 (kb, known_context_type, bound_vars,
	  expression, function_to_apply));
      }
    else { parser_warn(expression + " should be a CONS.");
	   return expression;
	 }
  }
}


class GeneralizedStringWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    Node key = null;
    if (expression instanceof OKBCString)
      { key = _string; }
    else if (expression instanceof AbstractSymbol)
      { key = _symbol; }
    else { key = _primitive_lisp_object; }
    return function_to_apply.funcall(kb, this, known_context_type, bound_vars,
				     get_walker(key).walk_subexpression
				     (kb, known_context_type, bound_vars,
				      expression, function_to_apply));
  }
}


class StringWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (expression instanceof OKBCString)
      { return function_to_apply.funcall
	(kb, this, known_context_type,
	 bound_vars,
	 get_walker(_primitive_lisp_object).walk_subexpression
	 (kb, known_context_type, bound_vars,
	  expression, function_to_apply));
      }
    else { parser_warn(expression + " should be an OKBCString.");
	   return expression;
	 }
  }
}


class SymbolWalker extends AbstractSubExpressionWalker {

  public Node walk_subexpression_1 (Kb kb, Node known_context_type,
				    Node bound_vars, Node expression,
				    AbstractWalkFunction function_to_apply)
  {
    if (expression instanceof AbstractSymbol)
      { return function_to_apply.funcall
	(kb, this, known_context_type,
	 bound_vars,
	 get_walker(_primitive_lisp_object).walk_subexpression
	 (kb, known_context_type, bound_vars,
	  expression, function_to_apply));
      }
    else { parser_warn(expression + " should be a symbol.");
	   return expression;
	 }
  }
}


public abstract class AbstractKifWalker {

  Hashtable walker_table = new Hashtable();
  int depth = 0;
  public boolean trace_p = false;
  public boolean variables_ok_as_relation_expression_functors = false;
  public boolean symbols_ok_as_non_logical_constants_p = false;
  public boolean replace_kif_symbols_with_keywords_p = true;
  public boolean err_on_parse_errors_p = false;

  AbstractKeyValueWalker key_and_value_walker = null;

  void parser_warn (String string) {
    System.out.println();
    System.out.println("!!! Warning at depth " + depth + ": " + string);
    System.out.println();
  }

  void add_depth() { depth = depth + 1; }
  
  void decrement_depth() { depth = depth - 1; }
  
  public AbstractSubExpressionWalker get (Node key) {
    AbstractSubExpressionWalker res =
      (AbstractSubExpressionWalker)(walker_table.get(key));
    if (res == null)
      { return get(Node._T); }
    else { return res; }
  }

  public void add_walker(Node key, AbstractSubExpressionWalker walker) {
    walker_table.put(key, walker);
    walker.set_walker(this);
    walker.set_key_and_value_walker(key_and_value_walker);
  }

  public boolean object_constant_p (Node thing, Kb kb) {
    if (thing instanceof Cons ||
	TupleStore.kif_variable_p(thing) ||
	kif_operator_p(thing) != Node._NIL)
      { return false; }
    else
      {
	if (symbols_ok_as_non_logical_constants_p)
	  {
	    if (thing instanceof AbstractSymbol &&
		(!(((AbstractSymbol)thing).keywordp()) ||
		   // Special case for this one.  It's in the
		   // knowledge model.
		 thing == Node._union) &&
		(((thing instanceof Symbol) && 
		  ((Symbol)thing).symbolPackage != null) ||
		 ((thing instanceof QuasiSymbol) && 
		  ((QuasiSymbol)thing).symbolPackage != null)))
	      {
		return true;
	      }
	    else { return thing.frame_reference_p(kb); }
	  }
	else
	  {
	    return thing.frame_reference_p(kb);
	  }
      }
  }

  public boolean function_constant_p (Node thing, Kb kb) {
    return object_constant_p(thing, kb);
  }

  public boolean logical_constant_p (Node thing, Kb kb) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._true) || sym.stringEquals(Node._false))
	  { return true; }
      }
    return object_constant_p(thing, kb);
  }

  public boolean relation_constant_p (Node thing, Kb kb) {
    return
      Cons.member_eql(thing, Node.okbc_standard_class_names, kb, Node._NIL) ||
      Cons.member_eql(thing, Node.okbc_standard_slot_names, kb, Node._NIL) ||
      Cons.member_eql(thing, Node.okbc_standard_facet_names, kb, Node._NIL) ||
      object_constant_p(thing, kb);
  }

  public Node rule_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._angangeq))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._angangeq; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._eqangang))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._eqangang; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node definition_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._defobject))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._defobject; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._deffunction))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._deffunction; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._defrelation))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._defrelation; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node logical_term_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._if))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._if; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._cond))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._cond; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node quantified_term_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._the))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._the; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._setofall))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._setofall; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._lambda))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._lambda; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._kappa))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._kappa; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node quotation_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._quote))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._quote; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node list_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._listof))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._listof; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node set_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._setof))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._setof; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node logical_sentence_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._not))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._not; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._and))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._and; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._or))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._or; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._implies))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._implies; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._backwards_implies))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._backwards_implies; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._iff))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._iff; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node sentence_quantification_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._exists))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._exists; }
	    else { return sym; }
	  }
	else if (sym.stringEquals(Node._forall))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._forall; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node equality_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._equals_sign))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._equals_sign; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node inequality_operator_p (Node thing) {
    if (thing instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol)thing;
	if (sym.stringEquals(Node._not_equal))
	  { if (replace_kif_symbols_with_keywords_p)
	      { return Node._not_equal; }
	    else { return sym; }
	  }
	else { return Node._NIL; }
      }
    else { return Node._NIL; }
  }

  public Node term_operator_p (Node thing) {
    Node res = null;
    res = quantified_term_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = quotation_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = list_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = set_operator_p(thing);
    return res;
  }

  public Node sentence_operator_p (Node thing) {
    Node res = null;
    res = logical_sentence_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = sentence_quantification_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = equality_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = inequality_operator_p(thing);
    return res;
  }
       
  public Node kif_operator_p (Node thing) {
    Node res = null;
    res = term_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = sentence_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = rule_operator_p(thing);
    if (res != Node._NIL) { return res; }
    res = definition_operator_p(thing);
    return res;
  }

  public AbstractKifWalker () {
    initialize_walker_table();
    initialize_plist_walker_table();
  }
  
  public void initialize_walker_table() {
    add_walker(Node._T, new GenericProductionSubExpressionWalker());

    add_walker(AbstractSubExpressionWalker._kif, new KifWalker());
    add_walker(AbstractSubExpressionWalker._sentence, new SentenceWalker());
    add_walker(AbstractSubExpressionWalker._logical_constant,
	       new LogicalConstantWalker());
    add_walker(AbstractSubExpressionWalker._relational_sentence,
	       new RelationalSentenceWalker());
    add_walker(AbstractSubExpressionWalker._equation, new EquationWalker());
    add_walker(AbstractSubExpressionWalker._inequality, new InequalityWalker());
    add_walker(AbstractSubExpressionWalker._logical_sentence,
	       new LogicalSentenceWalker());
    add_walker(AbstractSubExpressionWalker._variable_list,
	       new VariableListWalker());
    add_walker(AbstractSubExpressionWalker._term_list, new TermListWalker());
    add_walker(AbstractSubExpressionWalker._constant_list,
	       new ConstantListWalker());
    add_walker(AbstractSubExpressionWalker._quantified_sentence,
	       new QuantifiedSentenceWalker());
    add_walker(AbstractSubExpressionWalker._rule, new RuleWalker());
    add_walker(AbstractSubExpressionWalker._premise, new PremiseWalker());
    add_walker(AbstractSubExpressionWalker._definition, new DefinitionWalker());
    add_walker(AbstractSubExpressionWalker._object_definition,
	       new ObjectDefinitionWalker());
    add_walker(AbstractSubExpressionWalker._function_definition,
	       new FunctionDefinitionWalker());
    add_walker(AbstractSubExpressionWalker._relation_definition,
	       new RelationDefinitionWalker());
    add_walker(AbstractSubExpressionWalker._term, new TermWalker());
    add_walker(AbstractSubExpressionWalker._optional_term,
	       new OptionalTermWalker());
    add_walker(AbstractSubExpressionWalker._constant, new ConstantWalker());
    add_walker(AbstractSubExpressionWalker._variable, new VariableWalker());
    add_walker(AbstractSubExpressionWalker._bound_variable,
	       new BoundVariableWalker());
    add_walker(AbstractSubExpressionWalker._free_variable,
	       new FreeVariableWalker());
    add_walker(AbstractSubExpressionWalker._functional_term,
	       new FunctionalTermWalker());
    add_walker(AbstractSubExpressionWalker._quotation_term,
	       new QuotationTermWalker());
    add_walker(AbstractSubExpressionWalker._list_term, new ListTermWalker());
    add_walker(AbstractSubExpressionWalker._set_term, new SetTermWalker());
    add_walker(AbstractSubExpressionWalker._logical_term,
	       new LogicalTermWalker());
    add_walker(AbstractSubExpressionWalker._quantified_term,
	       new QuantifiedTermWalker());
    add_walker(AbstractSubExpressionWalker._function_constant,
	       new FunctionConstantWalker());
    add_walker(AbstractSubExpressionWalker._relation_constant,
	       new RelationConstantWalker());
    add_walker(AbstractSubExpressionWalker._object_constant,
	       new ObjectConstantWalker());
    add_walker(AbstractSubExpressionWalker._kif_operator,
	       new KifOperatorWalker());
    add_walker(AbstractSubExpressionWalker._lisp_object,
	       new LispObjectWalker());
    add_walker(AbstractSubExpressionWalker._number, new NumberWalker());
    add_walker(AbstractSubExpressionWalker._cons_as_lisp_object,
	       new ConsAsLispObjectWalker());
    add_walker(AbstractSubExpressionWalker._generalized_string,
	       new GeneralizedStringWalker());
    add_walker(AbstractSubExpressionWalker._string, new StringWalker());
    add_walker(AbstractSubExpressionWalker._symbol, new SymbolWalker());
    add_walker(AbstractSubExpressionWalker._primitive_lisp_object,
	       new PrimitiveLispObjectWalker());
  }

  public void initialize_plist_walker_table() {}

}
