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


class SymValEvaluator extends AbstractEvaluator {
  public Node eval (Node arg, EvalContext environment, int depth, Kb ckb) {
    AbstractSymbol sym = (AbstractSymbol)arg;
    if (sym.getSymbolPackage().name.equals("KEYWORD"))
      { String string = sym.symbolName;
	boolean lowercase_found_p = false;
	for (int i = 0; i < string.length() ; i++) {
	  char c = string.charAt(i);
	  if ((c >= 'a') && (c <= 'z'))
	    { lowercase_found_p = true;
	      break;
	    }
	}
	if (lowercase_found_p)
	  { return QuasiSymbol.intern(string.toUpperCase(),
				      sym.getSymbolPackage()); }
	else { return sym; }
      }
    else 
      { Node entry = environment.get_binding_list_entry(sym);
	if (entry != null) { return entry.nth(1); }
	else if (sym.symbolName.equalsIgnoreCase("T")) { return Kb._T; }
	else if (sym.symbolName.equalsIgnoreCase("NIL"))
	  { return Kb._NIL; }
	else { throw new GenericError
	       (_T, new OKBCString(sym + 
				  " is unbound.  The known bindings are: " +
				  environment.bindings)).signal();
	     }
      }
  }
}

class Debug extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    Node all_args = _NIL;
    Enumeration e = args.elements();
    while (e.hasMoreElements()) {
      Node arg = (Node)(e.nextElement());
      all_args = new Cons(trivial_eval_expression(arg, environment, depth + 1,
						  ckb),
			  all_args);
    }
    return Node.current_kb().debug_node(all_args);
  }
}

class PlusEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 instanceof OKBCInteger)
      { long arg0_val = ((OKBCInteger)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    return new OKBCInteger(arg0_val + arg1_val);
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    return new Real(arg0_val + arg1_val);
	  }
      }
    else
      { double arg0_val = ((Real)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    return new Real(arg0_val + arg1_val);
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    return new Real(arg0_val + arg1_val);
	  }
      }
  }
}

class MinusEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 instanceof OKBCInteger)
      { long arg0_val = ((OKBCInteger)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    return new OKBCInteger(arg0_val - arg1_val);
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    return new Real(arg0_val - arg1_val);
	  }
      }
    else
      { double arg0_val = ((Real)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    return new Real(arg0_val - arg1_val);
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    return new Real(arg0_val - arg1_val);
	  }
      }
  }
}

class TimesEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 instanceof OKBCInteger)
      { long arg0_val = ((OKBCInteger)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    return new OKBCInteger(arg0_val * arg1_val);
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    return new Real(arg0_val * arg1_val);
	  }
      }
    else
      { double arg0_val = ((Real)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    return new Real(arg0_val * arg1_val);
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    return new Real(arg0_val * arg1_val);
	  }
      }
  }
}

class SlashEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 instanceof OKBCInteger)
      { long arg0_val = ((OKBCInteger)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    return new OKBCInteger(arg0_val / arg1_val);
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    return new Real(arg0_val / arg1_val);
	  }
      }
    else
      { double arg0_val = ((Real)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    return new Real(arg0_val / arg1_val);
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    return new Real(arg0_val / arg1_val);
	  }
      }
  }
}

class SortEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node list = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node kb = trivial_eval_expression(args.nth(1), environment, depth + 1,
				      ckb);
    EvaluatorSortPredicate p = new EvaluatorSortPredicate((Kb)kb);
    return ListSorter.sort(list, p);
  }
}

class LtEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (Node.generalised_less_than(arg0, arg1)) { return _T; }
    else { return _NIL; }
  }
}

class GtEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (Node.generalised_less_than(arg1, arg0)) { return _T; }
    else { return _NIL; }
  }
}

class LteEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (!(Node.generalised_less_than(arg1, arg0))) { return _T; }
    else { return _NIL; }
  }
}

class GteEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (!(Node.generalised_less_than(arg0, arg1))) { return _T; }
    else { return _NIL; }
  }
}

class EqualsEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 instanceof OKBCInteger)
      { long arg0_val = ((OKBCInteger)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    if (arg0_val == arg1_val) { return _T; }
	    else { return _NIL; }
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    if (arg0_val == arg1_val) { return _T; }
	    else { return _NIL; }
	  }
      }
    else
      { double arg0_val = ((Real)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    if (arg0_val == arg1_val) { return _T; }
	    else { return _NIL; }
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    if (arg0_val == arg1_val) { return _T; }
	    else { return _NIL; }
	  }
      }
  }
}

class EqEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 == arg1) { return _T; }
    else { return _NIL; }
  }
}

class EqlEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 == arg1) { return _T; }
    else if (arg0 instanceof OKBCInteger ||
	arg0 instanceof Real ||
	arg0 instanceof OKBCString)
      { if (arg0.equals(arg1)) { return _T; }
	else { return _NIL; }
      }
    else if (ckb != null)
      { return ckb.eql_in_kb(arg0, arg1, _NIL); }
    else if (arg0 == arg1) { return _T; }
    else { return _NIL; }
  }
}

class EqualEvaluator extends AbstractEvaluator {
  Node lisp_equal(Node arg0, Node arg1, Kb ckb) {
    if (arg0 == arg1) { return _T; }
    else if (arg0 instanceof Cons)
      if (arg1 instanceof Cons)
	{
	  Node car_result = lisp_equal(arg0.car(), arg1.car(), ckb);
	  if (car_result == _T)
	    { return lisp_equal(Cons.cdr_or_NIL(arg0), Cons.cdr_or_NIL(arg1),
				ckb); }
	  else { return _NIL; }
	}
      else { return _NIL; }
    else if (arg0 instanceof OKBCInteger ||
	     arg0 instanceof Real ||
	     arg0 instanceof OKBCString)
      { if (arg0.equals(arg1)) { return _T; }
	else { return _NIL; }
      }
    else if (ckb != null)
      { return ckb.equal_in_kb(arg0, arg1, _NIL); }
    else if (arg0 == arg1) { return _T; }
    else if (arg0 != null && arg0.equals(arg1)) { return _T; }
    else { return _NIL; }
  }
    
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    return lisp_equal(arg0, arg1, ckb);
  }
}

class EqualpEvaluator extends AbstractEvaluator {
  Node lisp_equalp(Node arg0, Node arg1, Kb ckb) {
    if (arg0 == arg1) { return _T; }
    else if (arg0 instanceof Cons)
      if (arg1 instanceof Cons)
	{
	  Node car_result = lisp_equalp(arg0.car(), arg1.car(), ckb);
	  if (car_result == _T)
	    { return lisp_equalp(Cons.cdr_or_NIL(arg0), Cons.cdr_or_NIL(arg1),
				 ckb); }
	  else { return _NIL; }
	}
      else { return _NIL; }
    else if (arg0 instanceof OKBCInteger ||
	     arg0 instanceof Real)
      { if (arg0.equals(arg1)) { return _T; }
	else { return _NIL; }
      }
    else if (arg0 instanceof OKBCString &&
	     arg1 instanceof OKBCString)
      { if (((OKBCString)arg0).string.equalsIgnoreCase
	    (((OKBCString)arg1).string))
	  { return _T; }
	else { return _NIL; }
      }
    else if (ckb != null)
      { return ckb.equalp_in_kb(arg0, arg1, _NIL); }
    else if (arg0 == arg1) { return _T; }
    else if (arg0 != null && arg0.equals(arg1)) { return _T; }
    else { return _NIL; }
  }
    
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    return lisp_equalp(arg0, arg1, ckb);
  }
}

class QuoteEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    return args.car();
  }
}

class AndEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    Enumeration e = args.elements();
    Node result = _NIL;
    for (; e.hasMoreElements() ;) {
      Node elt = (Node)(e.nextElement());
      result = trivial_eval_expression(elt, environment, depth + 1, ckb);
      if (result == _NIL) { return _NIL; }
    }
    return result;
  }
}

class OrEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    Enumeration e = args.elements();
    for (; e.hasMoreElements() ;) {
      Node elt = (Node)(e.nextElement());
      Node result = trivial_eval_expression(elt, environment, depth + 1, ckb);
      if (result != _NIL) { return result; }
    }
    return _NIL;
  }
}

class FirstEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node result = trivial_eval_expression(args.car(), environment, depth + 1,
					  ckb);
    return result.car();
  }
}

class RestEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node result = trivial_eval_expression(args.car(), environment, depth + 1,
					  ckb);
    return Cons.cdr_or_NIL(result);
  }
}

class NthEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 instanceof OKBCInteger)
      { return arg1.nth((int)(((OKBCInteger)arg0).number)); }
    else { throw new GenericError
	     (_T, new OKBCString(arg0 + 
				" is not an integer argument to Nth."))
	     .signal();
	 }
  }
}

class NthCdrEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 instanceof OKBCInteger)
      { return Cons.nthcdr((int)(((OKBCInteger)arg0).number), arg1); }
    else { throw new GenericError
	     (_T, new OKBCString(arg0 + 
				" is not an integer argument to NthCdr."))
	     .signal();
	 }
  }
}

class FirstNEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg0 instanceof OKBCInteger)
      { return Cons.firstn((int)(((OKBCInteger)arg0).number), arg1); }
    else { throw new GenericError
	     (_T, new OKBCString(arg0 + 
				" is not an integer argument to FirstN."))
	     .signal();
	 }
  }
}

class PushEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node new_value = trivial_eval_expression
      (args.car(), environment, depth + 1, ckb);
    AbstractSymbol sym = (AbstractSymbol)(args.nth(1));
    Cons entry = (Cons)(environment.get_binding_list_entry(sym));
    if (entry == null)
      { throw new GenericError
	  (_T, new OKBCString("Cannot push onto " + sym +
			     " it is not in the environment.  " +
			     "You can only push onto bound Symbols."))
	  .signal();
      }
    Cons tail = (Cons)(entry.cdr());
    tail.set_car(new Cons(new_value, tail.car()));
    return tail.car();
  }
}

class LetEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 1);
    Node bindings = args.car();
    Node body = Cons.cdr_or_NIL(args);
    Node augmentation = _NIL;
    Cons tail = null;
    Enumeration bindings_enumeration = bindings.elements();
    for (; bindings_enumeration.hasMoreElements() ;) {
      Cons entry = (Cons)(bindings_enumeration.nextElement());
      AbstractSymbol car = (AbstractSymbol)(entry.car());
      Node value = trivial_eval_expression
	(entry.nth(1), environment, depth + 1, ckb);
      Node evaluated_binding = Cons.list(car, value);
      if (tail == null)
	{ tail = new Cons(evaluated_binding);
	  augmentation = tail;
	}
      else { Cons new_tail = new Cons(evaluated_binding);
	     tail.set_cdr(new_tail);
	     tail = new_tail;
	   }
    }
    EvalContext new_env = environment.augment_multiple(augmentation);
    Node value = _NIL;
    if (body != null)
      {
	Enumeration body_expressions = body.elements();
	for (; body_expressions.hasMoreElements() ;) {
	  Node form = (Node)(body_expressions.nextElement());
	  value = trivial_eval_expression(form, new_env, depth + 1, ckb);
	}
      }
    return value;
  }
}

class LetStarEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 1);
    Node bindings = args.car();
    Node body = Cons.cdr_or_NIL(args);
    EvalContext new_env = environment;
    Enumeration bindings_enumeration = bindings.elements();
    for (; bindings_enumeration.hasMoreElements() ;) {
      Cons entry = (Cons)(bindings_enumeration.nextElement());
      AbstractSymbol car = (AbstractSymbol)(entry.car());
      Node value = trivial_eval_expression(entry.nth(1), new_env, depth + 1,
					   ckb);
      Node evaluated_binding = Cons.list(car, value);
      new_env = new_env.augment(evaluated_binding);
    }
    Node value = _NIL;
    if (body != null)
      {
	Enumeration body_expressions = body.elements();
	for (; body_expressions.hasMoreElements() ;) {
	  Node form = (Node)(body_expressions.nextElement());
	  value = trivial_eval_expression(form, new_env, depth + 1, ckb);
	}
      }
    return value;
  }
}

class MVBEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 2);
    Node varlist = args.car();
    Node values_form = args.cadr();
    Node body = Cons.cdr_or_NIL(args.cdr());
    Values values = (Values)trivial_eval_expression_maybe_to_values
      (values_form, environment, depth + 1, ckb);
    EvalContext new_env = environment.copy_self();
    Enumerator varliste = varlist.enumerate_list();
    int index = 0;
    while (varliste.has_more_p()) {
      AbstractSymbol var = (AbstractSymbol)(varliste.next());
      Node value = values.nthValue(index);
      index = index + 1;
      new_env.push(Cons.list(var, value));
    }
    Node value = _NIL;
    if (body != null)
      {
	Enumeration body_expressions = body.elements();
	for (; body_expressions.hasMoreElements() ;) {
	  Node form = (Node)(body_expressions.nextElement());
	  value = trivial_eval_expression(form, new_env, depth + 1, ckb);
	}
      }
    return value;
  }
}

class DoListEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 1);
    Node binding_spec = args.car();
    Node loop_var = (AbstractSymbol)(binding_spec.car());
    Node list = trivial_eval_expression
      (binding_spec.nth(1), environment, depth + 1, ckb);
    Node body = Cons.cdr_or_NIL(args);
    Node result = _NIL;
    Cons tail = null;
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Node element = (Node)(e.nextElement());
      EvalContext new_env = 
	environment.augment(Cons.list(loop_var, element));
      Node value = _NIL;
      if (body != null)
	{
	  Enumeration body_expressions = body.elements();
	  for (; body_expressions.hasMoreElements() ;) {
	    Node form = (Node)(body_expressions.nextElement());
	    value = trivial_eval_expression(form, new_env, depth + 1, ckb);
	  }
	}
      if (tail == null)
	{ tail = new Cons(value);
	  result = tail;
	}
      else { Cons new_tail = new Cons(value);
	     tail.set_cdr(new_tail);
	     tail = new_tail;
	   }
    }
    return result;
  }
}

class WhileEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 1);
    Node condition = args.car();
    Node body = Cons.cdr_or_NIL(args);
    Node result = _NIL;
    while (trivial_eval_expression
	   (condition, environment, depth + 1, ckb) != Node._NIL) {
      if (body != null)
	{
	  Enumerator body_expressions = body.enumerate_list();
	  while(body_expressions.has_more_p()) {
	    Node form = body_expressions.next();
	    Node value = trivial_eval_expression(form, environment, depth + 1,
						 ckb);
	  }
	}
    }
    return result;
  }
}

class WhileCollectEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 1);
    Node condition = args.car();
    Node body = Cons.cdr_or_NIL(args);
    Node result = _NIL;
    Cons tail = null;
    while (trivial_eval_expression
	   (condition, environment, depth + 1, ckb) != Node._NIL) {
      if (body != null)
	{
	  Enumerator body_expressions = body.enumerate_list();
	  while(body_expressions.has_more_p()) {
	    Node form = body_expressions.next();
	    Node value = trivial_eval_expression(form, environment, depth + 1,
						 ckb);
	    if (tail == null)
	      { tail = new Cons(value);
		result = tail;
	      }
	    else { Cons new_tail = new Cons(value);
		   tail.set_cdr(new_tail);
		   tail = new_tail;
		 }
	  }
	}
    }
    return result;
  }
}

class AppendEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    Node result = _NIL;
    if (args != null)
      {
	Enumeration e = args.elements();
	for (; e.hasMoreElements() ;) {
	  Node element = (Node)(e.nextElement());
	  Node value = trivial_eval_expression
	    (element, environment, depth + 1, ckb);
	  result = Cons.append_cons(result, value);
	}
      }
    return result;
  }
}

class IfEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 1);
    Node discriminator = trivial_eval_expression
      (args.car(), environment, depth + 1, ckb);
    if (discriminator == _NIL)
      { return trivial_eval_expression(args.nth(2), environment, depth + 1,
				       ckb); }
    else { return trivial_eval_expression
	   (args.nth(1), environment, depth + 1, ckb); 
	 }
  }
}

class VariablepEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    if (TupleStore.variable_p(trivial_eval_expression
			      (args.car(), environment, depth + 1, ckb)))
      { return Node._T; }
    else { return Node._NIL; }
  }
}

class LengthEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    return new OKBCInteger(trivial_eval_expression
			   (args.car(), environment, depth + 1, ckb).length());
  }
}

class ReverseEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    return Cons.reverse(trivial_eval_expression
			(args.car(), environment, depth + 1, ckb));
  }
}

class ConspEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node arg = trivial_eval_expression (args.car(), environment, depth + 1,
					ckb);
    if (arg instanceof Cons) { return _T; }
    else { return _NIL; }
  }
}

class NotEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node result = trivial_eval_expression(args.car(), environment, depth + 1,
					  ckb);
    if (result == _NIL) { return _T;}
    else { return _NIL; }
  }
}

class ErrorEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node string = trivial_eval_expression(args.car(), environment, depth + 1,
					  ckb);
    throw new GenericError(_NIL, string).signal();
  }
}

class ConnectionPEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node result = trivial_eval_expression(args.car(), environment, depth + 1,
					  ckb);
    return Node.connection_p(result);
  }
}

class ContinuableErrorPEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node result = trivial_eval_expression(args.car(), environment, depth + 1,
					  ckb);
    return Node.continuable_error_p(result);
  }
}

class ProcedurePEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node result = trivial_eval_expression(args.car(), environment, depth + 1
					  , ckb);
    return Node.procedure_p(result);
  }
}

class CurrentKbEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    return Node.current_kb();
  }
}

class LocalConnectionEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 0);
    return Node.local_connection();
  }
}

class GetKbTypeEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    Node thing = (Node)trivial_eval_expression
                              (args.car(), environment, depth + 1, ckb);
    check_length_exceeds(args, 0);
    if (args.length() == 0)
      { return Node.get_kb_type(thing); }
    else 
      {
	throw new GenericError
	  (_T, new OKBCString("Cannot get kb_type for " + args)).signal();
      }
  }

  public Node positional_eval (Node args, EvalContext environment, int depth,
			       Kb ckb) {
    Node thing = (Node)trivial_eval_expression
      (args.car(), environment, depth + 1, ckb);
    args = cdr_or_NIL(args); // pop
    return Node.get_kb_type(thing);
  }
}

class ConnectionEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node result = trivial_eval_expression(args.car(), environment, depth + 1,
					  ckb);
    Kb kb = (Kb)result;
    return kb.connection();
  }
}

class EstablishConnectionEvaluator extends AbstractEvaluator {
  static final Symbol _local_okbc_connection
    = Symbol.intern_in_okbc("LOCAL-CONNECTION");

  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 0);
    Node result = trivial_eval_expression(args.car(), environment, depth + 1,
					  ckb);
    if (result == _local_okbc_connection)
      { return LocalConnection.local_connection(); }
    else
      {
	throw new GenericError
	  (_NIL,
	   new OKBCString("Cannot establish connection of type " + result))
	  .signal();
      }
  }
}

class PrognEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    Node value = _NIL;
    if (args != null)
      {
	Enumeration body_expressions = args.elements();
	for (; body_expressions.hasMoreElements() ;) {
	  Node form = (Node)(body_expressions.nextElement());
	  value = trivial_eval_expression(form, environment, depth + 1, ckb);
	}
      }
    return value;
  }
}

class ListEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    Node result = _NIL;
    Cons tail = null;
    if (args != null)
      {
	Enumeration e = args.elements();
	for (; e.hasMoreElements() ;) {
	  Node element = (Node)(e.nextElement());
	  Node value = trivial_eval_expression
	    (element, environment, depth + 1, ckb);
	  if (tail == null)
	    { tail = new Cons(value);
	      result = tail;
	    }
	  else { Cons new_tail = new Cons(value);
		 tail.set_cdr(new_tail);
		 tail = new_tail;
	       }
	}
      }
    return result;
  }
}

class ListStarEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 0);
    Node result = _NIL;
    Cons tail = null;
    Enumeration e = args.elements();
    for (; e.hasMoreElements() ;) {
      Node element = (Node)(e.nextElement());
      Node value = trivial_eval_expression(element, environment, depth + 1,
					   ckb);
      if (e.hasMoreElements())
	{
	  if (tail == null)
	    { tail = new Cons(value);
	      result = tail;
	    }
	  else { Cons new_tail = new Cons(value);
		 tail.set_cdr(new_tail);
		 tail = new_tail;
	       }
	}
      else
	{ if (value == _NIL) { return result; }
	  else if (value instanceof Cons)
	    { if (tail == null) { result = value; }
	      else { tail.set_cdr(value); }
	    }
	  else { throw new GenericError
		   (_T, new OKBCString("The last arg to LIST* " + value + 
				      " is not a list.")).signal();
	       }
	}
    }
    return result;
  }
}

class MemberEvaluator extends AbstractEvaluator {
  static Node member(Node arg0, Node arg1, EvalContext environment) {
    if (arg1 == null || arg1 == _NIL) { return _NIL; }
    Enumeration e = arg1.elements();
    Node tail = arg1;
    Node kb_entry = environment.get_binding_list_entry(_kb);
    Kb kb = null;
    if (kb_entry != null) { kb = (Kb)(kb_entry.nth(1)); }
    if (kb == null)
      {
	for (; e.hasMoreElements() ;) {
	  Node elt = (Node)e.nextElement();
	  if (elt.equals(arg0)) { return tail; }
	  tail = Cons.cdr_or_NIL(tail);
	}
      }
    else
      {
	for (; e.hasMoreElements() ;) {
	  Node elt = (Node)e.nextElement();
	  if (kb.eql_in_kb(elt, arg0, _NIL) != _NIL) { return tail; }
	  tail = Cons.cdr_or_NIL(tail);
	}
      }
    return _NIL;
  }

  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    return member(arg0, arg1, environment);
  }
}

class RemoveEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    if (arg1 == null || arg1 == _NIL) { return _NIL; }
    Enumeration e = arg1.elements();
    Cons tail = null;
    Node result = _NIL;
    Node kb_entry = environment.get_binding_list_entry(_kb);
    Kb kb = null;
    if (kb_entry != null) { kb = (Kb)(kb_entry.nth(1)); }
    if (kb == null)
      {
	for (; e.hasMoreElements() ;) {
	  Node value = (Node)e.nextElement();
	  if (!value.equals(arg0))
	    { 
	      if (tail == null)
		{ tail = new Cons(value);
		  result = tail;
		}
	      else { Cons new_tail = new Cons(value);
		     tail.set_cdr(new_tail);
		     tail = new_tail;
		   }
	    }
	}
      }
    else
      {
	for (; e.hasMoreElements() ;) {
	  Node value = (Node)e.nextElement();
	  if (kb.eql_in_kb(value, arg0, _NIL) == _NIL)
	    { 
	      if (tail == null)
		{ tail = new Cons(value);
		  result = tail;
		}
	      else { Cons new_tail = new Cons(value);
		     tail.set_cdr(new_tail);
		     tail = new_tail;
		   }
	    }
	}
      }
    return result;
  }
}

class RemoveDuplicatesEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 1);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    if (arg0 == null || arg0 == _NIL) { return _NIL; }
    Enumeration e = arg0.elements();
    Cons tail = null;
    Node result = _NIL;
    for (; e.hasMoreElements() ;) {
      Node value = (Node)e.nextElement();
      if (MemberEvaluator.member(value, result, environment) == _NIL)
	{ 
	  if (tail == null)
	    { tail = new Cons(value);
	      result = tail;
	    }
	  else { Cons new_tail = new Cons(value);
		 tail.set_cdr(new_tail);
		 tail = new_tail;
	       }
	}
      }
    return result;
  }
}

class GetfEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    return Cons.getf(arg0, arg1);
  }
}

class AssocEvaluator extends AbstractEvaluator {
  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length(args, 2);
    Node arg0 = trivial_eval_expression(args.car(), environment, depth + 1,
					ckb);
    Node arg1 = trivial_eval_expression(args.nth(1), environment, depth + 1,
					ckb);
    return Cons.assoc(arg0, arg1);
  }
}

class ProcedureEvaluator extends AbstractEvaluator {
  static Symbol _progn = Symbol.intern("PROGN", "LISP");

  Node add_progn_if_necessary (Node body) {
    if (body.length() < 2) { return body.car(); }
    else { return new Cons(_progn, body); }
  }

  public Node eval_funspec_diadic (Node args) {
    return Procedure.intern_procedure
	(_NIL, args.car(), add_progn_if_necessary(args.cdr()), _NIL); 
  }

  public Node eval_funspec_non_diadic (Node args) {
    Node car = args.car();
    if (car == _NIL || car instanceof OKBCString)
      { return Procedure.intern_procedure
	(_NIL, args.car(), add_progn_if_necessary(args.cdr()), _NIL); 
      }
    else
      { return Procedure.intern_procedure
	(args.car(), args.nth(1), add_progn_if_necessary(Cons.nthcdr(2, args)),
	 _NIL);
      }
  }

  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    check_length_exceeds(args, 1);
    if (args.length() == 2)      { return eval_funspec_diadic(args); }
    else if (args.length() >= 3) { return eval_funspec_non_diadic(args); }
    else { throw new GenericError
	   (_T, 
	    new OKBCString("Wrong number of arguments to Procedure")).signal();
	 }
  }
}


public class AbstractEvaluator {

  static public boolean trace_p = false;
  static False _NIL = Node._NIL;
  static True _T = Node._T;
  static final Symbol _kb = Symbol.keyword("KB");
  public static final Symbol _current_kb = Symbol.intern_in_okbc("CURRENT-KB");
  public boolean initialized_p = false;
  public Hashtable evaluators = null;
  static public boolean allow_interpreted_global_assignmenst_p = false;
  AbstractEvaluator missing_procedure_handler = null;

  public void set_missing_procedure_handler (AbstractEvaluator ev) {
    missing_procedure_handler = ev;
  }

  public static Node getenv(AbstractSymbol key, EvalContext environment) {
    return environment.getenv(key);
  }

  public static void check_length (Node expression, int l) {
    int length = 0;
    if (expression != null) { length = expression.length(); }
    if (l != length)
      { throw new RuntimeException 
	("Wrong number of arguments in " + expression + ".  " + l + 
	 " expected."); 
      }
  }

  public static void check_length_exceeds (Node expression, int l) {
    int length = 0;
    if (expression != null) { length = expression.length(); }
    if (length <= l)
      { throw new RuntimeException 
	("Wrong number of arguments in " + expression + ".  At least " + 
	 (l + 1) + " expected.");
      }
  }

  static Node cdr_or_NIL (Node expression) {
    Node cdr = expression.cdr();
    if (cdr == null) { return _NIL; }
    else { return cdr; }
  }

  Node trivial_eval_expression_for_symbol
          (AbstractSymbol sym, EvalContext environment, int depth) {
    if (sym.getSymbolPackage().name.equals("KEYWORD"))
      { String string = sym.symbolName;
	boolean lowercase_found_p = false;
	for (int i = 0; i < string.length() ; i++) {
	  char c = string.charAt(i);
	  if ((c >= 'a') && (c <= 'z'))
	    { lowercase_found_p = true;
	      break;
	    }
	}
	if (lowercase_found_p)
	  { return QuasiSymbol.intern(string.toUpperCase(),
				      sym.getSymbolPackage()); }
	else { return sym; }
      }
    else 
      { Node entry = environment.get_binding_list_entry(sym);
	if (entry != null) { return entry.nth(1); }
	else if (sym.symbolName.equalsIgnoreCase("T")) { return Kb._T; }
	else if (sym.symbolName.equalsIgnoreCase("NIL"))
	  { return Kb._NIL; }
	else { throw new GenericError
	       (_T, new OKBCString(sym + 
				  " is unbound.  The known bindings are: " +
				  environment.bindings)).signal();
	     }
      }
  }

  Node trivial_eval_lambda_call 
    (Node function, Node expression, EvalContext environment, int depth,
     Kb ckb) {
    if (!((Symbol)function.car()).string_equals_sign("LAMBDA"))
      { throw new RuntimeException ("Illegal function " + function + "." +
				    "  Expression = " + expression + "." +
				    "  Environment = " + environment);
      }
    Node args = _NIL;
    Cons tail = null;
    // First evaluate the actual parameters to the lambda expression
    // in the environment passed in, building up a new binding
    // list for the args to the lambda expression from the values of the
    // actual parameters.
    Enumeration args_enumeration = cdr_or_NIL(expression).elements();
    Enumeration formals = function.nth(1).elements();
    for (; args_enumeration.hasMoreElements() ;) {
      Node arg = (Node)(args_enumeration.nextElement());
      Node formal = (Node)(formals.nextElement());
      Node arg_result =
	Cons.list(arg, trivial_eval_expression(arg, environment, depth + 1,
					       ckb));
      if (tail == null)
	{ tail = new Cons(arg_result);
	  args = tail;
	}
      else { Cons new_tail = new Cons(arg_result);
	     tail.set_cdr(new_tail);
	     tail = new_tail;
	   }
    }
    // Now call the lambda expression with the augmented environment that
    // contains bindings for all of the args.
    Node value = _NIL;
    Node body = cdr_or_NIL(cdr_or_NIL(function));
    if (body != null)
      {
	EvalContext new_env = environment.augment_multiple(args);
	Enumeration body_expressions = body.elements();
	for (; body_expressions.hasMoreElements() ;) {
	  Node form = (Node)(body_expressions.nextElement());
	  value = trivial_eval_expression(form, new_env, depth + 1, ckb);
	}
      }
    return value;
  }

  public Node err_on_missing_procedure (Node name, Node expression,
					EvalContext environment) {
    if (missing_procedure_handler != null &&
	missing_procedure_handler != this)
      { return missing_procedure_handler.err_on_missing_procedure
	  (name, expression, environment);
      }
    else
      {
	Node.err_on_missing_procedure(name);
	return Node._NIL;
      }
  }
  
  Node trivial_eval_for_procedure
    (String name, Node function, Node expression, EvalContext environment, 
     int depth, Kb ckb)
  {
    Node result = null;
    Hashtable ht = Procedure.name_to_procedure_table;
    Procedure funspec = 
      (Procedure) ht.get(((AbstractSymbol)function).symbolName.toUpperCase());
    if (funspec == null)
      { result = err_on_missing_procedure(function, expression, environment); }
    else { Node kb = getenv(_current_kb, environment);
	   if (kb == null || kb == _NIL)
	     { kb = getenv(_kb, environment); }
	   if (kb == null || kb == _NIL)
	     { throw new RuntimeException 
	       ("Cannot find KB in environment.  Environment is "
		+ environment); 
	     }
	   Enumeration e = cdr_or_NIL(expression).elements();
	   Cons evaluated_args = null;
	   Cons tail = null;
	   for (; e.hasMoreElements() ;) {
	     Node elt = (Node)(e.nextElement());
	     Node evaled = trivial_eval_expression
	       (elt, environment, depth + 1, ckb);
	     if (tail == null)
	       { tail = new Cons(evaled);
		 evaluated_args = tail;
	       }
	     else { Cons new_tail = new Cons(evaled);
		    tail.set_cdr(new_tail);
		    tail = new_tail;
		  }
	   }
	   result = funspec.apply_with_existing_environment
	     ((Kb)kb, evaluated_args, environment, depth);
	 }
    return result;
  }

  public AbstractEvaluator get_evaluator(Object key) {
    return (AbstractEvaluator)evaluators.get(key);
  }

  public Node trivial_eval_expression
    (Node expression, EvalContext environment, int depth, Kb ckb) {
    Node result = null;
    if (trace_p)
      { 
	System.out.print(depth);
	for (int i=0 ; i < depth ; i++) {
	  System.out.print("  ");
	}
	System.out.println(" Enter " + expression); 
      }
    if (expression == _T ||
	expression == _NIL ||
	expression instanceof OKBCInteger ||
	expression instanceof Real ||
	expression instanceof Procedure ||
	expression instanceof OKBCString)
      { result = expression; }
    else if (expression instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol) expression;
        initialize_evaluators();
        AbstractEvaluator ev = get_evaluator("SYMBOL-VALUE");
	result = ev.eval(sym, environment, depth, ckb);
      }
    else if (expression instanceof Cons)
      { Node function = expression.car();
	if (function instanceof Cons)
	  // Then we should have a lambda expression
	  {result = trivial_eval_lambda_call
	     (function, expression, environment, depth, ckb);
	 }
	else if (function instanceof AbstractSymbol)
	  { String name = ((AbstractSymbol)function).symbolName;
	    initialize_evaluators();
	    boolean internal_p = name.endsWith("-INTERNAL");
	    if (internal_p)
	      { AbstractEvaluator ev = get_evaluator
		    (name.substring(0, name.length() - 9));
		// 9 is the length of "-INTERNAL"
		if (ev == null)
		  // Check for funspec
		  { result = trivial_eval_for_procedure
		      (name, function, expression, environment, depth, ckb);
		  }
		else { result = ev.positional_eval
		       (cdr_or_NIL(expression), environment, depth, ckb); 
		     }
	      }
	    else { AbstractEvaluator ev = get_evaluator(name);
		   if (ev == null)
		     // Check for funspec
		     { result = trivial_eval_for_procedure
		       (name, function, expression, environment, depth, ckb);
		     }
		   else { result = ev.eval(cdr_or_NIL(expression), environment,
					   depth, ckb); 
			}
		 }
	  }
	else { throw new RuntimeException
		 ("Illegal function [" + function + 
		  "] at the head of an OKBC expression " + expression + ".");
	     }
      }
    // If we make it through to here we're losing.
    else { throw new RuntimeException
	   ("Illegal OKBC expression " + expression + ".");
	 }
    if (trace_p)
      { 
	System.out.print(depth);
	for (int i=0 ; i < depth ; i++) {
	  System.out.print("  ");
	}
	System.out.println(" Exit " + expression + " -> " + result); 
      }
    return result;
  }

  public Object trivial_eval_expression_maybe_to_values
    (Node expression, EvalContext environment, int depth, Kb ckb) {
    Object result = null;
    Kb kb_at_start = environment.get_last_kb();
    if (ckb == null) { ckb = kb_at_start; }
    if (trace_p)
      { 
	System.out.print(depth);
	for (int i=0 ; i < depth ; i++) {
	  System.out.print("  ");
	}
	System.out.println(" Enter " + expression); 
      }
    if (expression == _T ||
	expression == _NIL ||
	expression instanceof OKBCInteger ||
	expression instanceof Real ||
	expression instanceof Procedure ||
	expression instanceof OKBCString)
      { result = expression; }
    else if (expression instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol) expression;
        initialize_evaluators();
        AbstractEvaluator ev = get_evaluator("SYMBOL-VALUE");
	result = ev.eval(sym, environment, depth, ckb);
      }
    else if (expression instanceof Cons)
      { Node function = expression.car();
	if (function instanceof Cons)
	  // Then we should have a lambda expression
	  { result = trivial_eval_lambda_call
	     (function, expression, environment, depth, ckb);
	  }
	else if (function instanceof AbstractSymbol)
	  { String name = ((AbstractSymbol)function).symbolName;
	    initialize_evaluators();
	    boolean internal_p = name.endsWith("-INTERNAL");
	    if (internal_p)
	      { AbstractEvaluator ev = get_evaluator
		// 9 is the length of "-INTERNAL"
		    (name.substring(0, name.length() - 9));
		if (ev == null)
		  // Check for funspec
		  { result = trivial_eval_for_procedure
		      (name, function, expression, environment, depth, ckb);
		  }
		else { result = ev.positional_eval_maybe_to_values
		       (cdr_or_NIL(expression), environment, depth, ckb); 
		     }
	      }
	    else { AbstractEvaluator ev = get_evaluator(name);
		   if (ev == null)
		     // Check for funspec
		     { result = trivial_eval_for_procedure
		       (name, function, expression, environment, depth, ckb);
		     }
		   else { result = ev.eval_maybe_to_values
			    (cdr_or_NIL(expression), environment, depth, ckb); 
			}
		 }
	  }
	else { throw new RuntimeException
		 ("Illegal function [" + function + 
		  "] at the head of an OKBC expression " + expression + ".");
	     }
      }
    // If we make it through to here we're losing.
    else { throw new RuntimeException
	   ("Illegal OKBC expression " + expression + ".");
	 }
    if (trace_p)
      { 
	System.out.print(depth);
	for (int i=0 ; i < depth ; i++) {
	  System.out.print("  ");
	}
	System.out.println(" Exit " + expression + " -> " + result); 
      }
    if (kb_at_start != environment.get_last_kb())
      { if (environment.get_last_kb() != null)
	  { result = decontextualize(result, environment.get_last_kb());
	    return result;
	  }
	else { environment.record_last_kb(kb_at_start);
	       return result;
	     }
      }
    else { return result; }
  }

  Object decontextualize(Object result, Kb wrt_kb) {
    if (result instanceof Values)
      { Node list = ((Values)result).toList();
	Node decon = wrt_kb.decontextualize(list, Node._value);
	int len = decon.length();
	if (len == 1) { return Values.toValues(decon); }
	else if (len == 2) { return Values.toValues2(decon); }
	else if (len == 3) { return Values.toValues3(decon); }
	else if (len == 4) { return Values.toValues4(decon); }
	return null; // should never get here.
      }
    else { return wrt_kb.decontextualize((Node)result, Node._value); }
  }


  public Node eval (Node args, EvalContext environment, int depth, Kb ckb) {
    throw new RuntimeException ("Missing eval method for " + this + ".");
  }

  public Object eval_maybe_to_values (Node args, EvalContext environment, 
				      int depth, Kb ckb) {
    return eval(args, environment, depth, ckb);
  }

  public Node positional_eval (Node args, EvalContext environment, int depth,
			       Kb ckb) {
    throw new RuntimeException 
      ("Missing positional_eval method for " + this + ".");
  }

  public Object positional_eval_maybe_to_values 
      (Node args, EvalContext environment, int depth, Kb ckb) {
    return positional_eval(args, environment, depth, ckb);
  }

  public void initialize_evaluators() {
    if (!initialized_p)
      { throw new
	  RuntimeException("initialize_evaluators method not defined!");
      }
  }

  static Symbol get_keyword(Node thing) {
    if (thing instanceof Symbol)
      { Symbol sym = (Symbol)thing;
	if (sym.keywordp()) { return sym; }
      }
    throw new GenericError
	     (_T, new OKBCString("Illegal keyword argument.  " + thing + 
				" was expected to be a keyword."))
	     .signal();
  }

  static ContextName toContextName(Node thing) {
    if (thing instanceof ContextName)
      { return (ContextName)thing; }
    throw new GenericError
	     (_T, new OKBCString("Illegal argument.  " + thing + 
				" was expected to be a ContextName."))
	     .signal();
  }

  static Connection toConnection(Node thing) {
    if (thing instanceof Connection)
      { return (Connection)thing; }
    throw new GenericError
	     (_T, new OKBCString("Illegal argument.  " + thing + 
				" was expected to be a Connection."))
	     .signal();
  }

  static Enumerator toEnumerator(Node thing) {
    if (thing instanceof Enumerator)
      { return (Enumerator)thing; }
    throw new GenericError
	     (_T, new OKBCString("Illegal argument.  " + thing + 
				" was expected to be a Enumerator."))
	     .signal();
  }

  static Kb toKb(Node thing) {
    if (thing instanceof Kb)
      { return (Kb)thing; }
    throw new GenericError
	     (_T, new OKBCString("Illegal argument.  " + thing + 
				" was expected to be a Kb."))
	     .signal();
  }

  static InferenceLevel toInferenceLevel(Node thing) {
    if (thing instanceof InferenceLevel)
      { return (InferenceLevel)thing; }
    throw new GenericError
	     (_T, new OKBCString("Illegal argument.  " + thing + 
				" was expected to be an InferenceLevel."))
	     .signal();
  }

  static SlotType toSlotType(Node thing) {
    if (thing instanceof SlotType)
      { return (SlotType)thing; }
    throw new GenericError
	     (_T, new OKBCString("Illegal argument.  " + thing + 
				" was expected to be a SlotType."))
	     .signal();
  }

  static TruthValue toTruthValue(Node thing) {
    if (thing instanceof TruthValue)
      { return (TruthValue)thing; }
    throw new GenericError
	     (_T, new OKBCString("Illegal argument.  " + thing + 
				" was expected to be a TruthValue."))
	     .signal();
  }

  static ValueSelector toValueSelector(Node thing) {
    if (thing instanceof ValueSelector)
      { return (ValueSelector)thing; }
    throw new GenericError
	     (_T, new OKBCString("Illegal argument.  " + thing + 
				" was expected to be a ValueSelector."))
	     .signal();
  }

  public void basic_initialize_evaluators(Hashtable ht) {
    ht.put("+",                    new PlusEvaluator());
    ht.put("-",                    new MinusEvaluator());
    ht.put("*",                    new TimesEvaluator());
    ht.put("/",                    new SlashEvaluator());
    ht.put("<",                    new LtEvaluator());
    ht.put(">",                    new GtEvaluator());
    ht.put("=",                    new EqualsEvaluator());
    ht.put("<=",                   new LteEvaluator());
    ht.put(">=",                   new GteEvaluator());
    
    ht.put("EQ",                   new EqEvaluator());
    ht.put("EQL",                  new EqlEvaluator());
    ht.put("EQUAL",                new EqualEvaluator());
    ht.put("EQUALP",               new EqualpEvaluator());
    
    ht.put("QUOTE",                new QuoteEvaluator());
    ht.put("AND",                  new AndEvaluator());
    ht.put("OR",                   new OrEvaluator());
    
    ht.put("VARIABLEP",            new VariablepEvaluator());
    ht.put("LENGTH",               new LengthEvaluator());
    ht.put("CONSP",                new ConspEvaluator());
    ht.put("FIRST",                new FirstEvaluator());
    ht.put("REST",                 new RestEvaluator());
    ht.put("NTH",                  new NthEvaluator());
    ht.put("NTHCDR",               new NthCdrEvaluator());
    ht.put("APPEND",               new AppendEvaluator());
    ht.put("FIRSTN",               new FirstNEvaluator());
    ht.put("LIST",                 new ListEvaluator());
    ht.put("LIST*",                new ListStarEvaluator());
    ht.put("PUSH",                 new PushEvaluator());
    ht.put("SETQ",                 new SetqEvaluator());
    ht.put("REVERSE",              new ReverseEvaluator());
    ht.put("MEMBER",               new MemberEvaluator());
    ht.put("REMOVE",               new RemoveEvaluator());
    ht.put("REMOVE-DUPLICATES",    new RemoveDuplicatesEvaluator());
    ht.put("GETF",                 new GetfEvaluator());
    ht.put("ASSOC",                new AssocEvaluator());
    ht.put("SORT",                 new SortEvaluator());
    
    ht.put("LET",                  new LetEvaluator());
    ht.put("LET*",                 new LetStarEvaluator());
    ht.put("MULTIPLE-VALUE-BIND",  new MVBEvaluator());
    ht.put("DO-LIST",              new DoListEvaluator());
    ht.put("WHILE",                new WhileEvaluator());
    ht.put("WHILE-COLLECT",        new WhileCollectEvaluator());
    
    ht.put("IF",                   new IfEvaluator());
    ht.put("NOT",                  new NotEvaluator());
    ht.put("PROGN",                new PrognEvaluator());
    ht.put("PROCEDURE",            new ProcedureEvaluator());
    ht.put("PROCEDURE-P",          new ProcedurePEvaluator());
    
    ht.put("ERROR",                new ErrorEvaluator());
    ht.put("CURRENT-KB",           new CurrentKbEvaluator());
    ht.put("LOCAL-CONNECTION",     new LocalConnectionEvaluator());
    ht.put("CONTINUABLE-ERROR-P",  new ContinuableErrorPEvaluator());
    ht.put("CONNECTION",           new ConnectionEvaluator());
    ht.put("CONNECTION-P",         new ConnectionPEvaluator());
    ht.put("GET-KB-TYPE",          new GetKbTypeEvaluator());
    ht.put("ESTABLISH-CONNECTION", new EstablishConnectionEvaluator());
    ht.put("SYMBOL-VALUE",         new SymValEvaluator());

    ht.put("FETCH",                new FetchEv());
    ht.put("FREE",                 new FreeEv());
    ht.put("HAS-MORE",             new HasMoreEv());
    ht.put("NEXT",                 new NextEv());
    ht.put("PREFETCH",             new PrefetchEv());

    ht.put("DEBUG",                new Debug());

    ht.put("MEMBER-BEHAVIOR-VALUES-P", new MemberBehaviorValuesPEv());
    ht.put("MEMBER-FACET-VALUE-P", new MemberFacetValuePEv());
    ht.put("MEMBER-SLOT-VALUE-P", new MemberSlotValuePEv());
    ht.put("META-KB", new MetaKbEv());
    ht.put("OPEN-KB", new OpenKbEv());
    ht.put("OPENABLE-KBS", new OpenableKbsEv());
    ht.put("PRIMITIVE-P", new PrimitivePEv());
    ht.put("PRINT-FRAME", new PrintFrameEv());
    ht.put("PUT-BEHAVIOR-VALUES", new PutBehaviorValuesEv());
    ht.put("PUT-CLASS-SUPERCLASSES", new PutClassSuperclassesEv());
    ht.put("PUT-FACET-VALUE", new PutFacetValueEv());
    ht.put("PUT-FACET-VALUES", new PutFacetValuesEv());
    ht.put("PUT-FRAME-DETAILS", new PutFrameDetailsEv());
    ht.put("PUT-FRAME-NAME", new PutFrameNameEv());
    ht.put("PUT-FRAME-PRETTY-NAME", new PutFramePrettyNameEv());
    ht.put("PUT-INSTANCE-TYPES", new PutInstanceTypesEv());
    ht.put("PUT-SLOT-VALUE", new PutSlotValueEv());
    ht.put("PUT-SLOT-VALUES", new PutSlotValuesEv());
    ht.put("REGISTER-PROCEDURE", new RegisterProcedureEv());
    ht.put("REMOVE-CLASS-SUPERCLASS", new RemoveClassSuperclassEv());
    ht.put("REMOVE-FACET-VALUE", new RemoveFacetValueEv());
    ht.put("REMOVE-INSTANCE-TYPE", new RemoveInstanceTypeEv());
    ht.put("REMOVE-LOCAL-FACET-VALUES", new RemoveLocalFacetValuesEv());
    ht.put("REMOVE-LOCAL-SLOT-VALUES", new RemoveLocalSlotValuesEv());
    ht.put("REMOVE-SLOT-VALUE", new RemoveSlotValueEv());
    ht.put("RENAME-FACET", new RenameFacetEv());
    ht.put("RENAME-SLOT", new RenameSlotEv());
    ht.put("REPLACE-FACET-VALUE", new ReplaceFacetValueEv());
    ht.put("REPLACE-SLOT-VALUE", new ReplaceSlotValueEv());
    ht.put("REVERT-KB", new RevertKbEv());
    ht.put("SAVE-KB", new SaveKbEv());
    ht.put("SAVE-KB-AS", new SaveKbAsEv());
    ht.put("SLOT-HAS-FACET-P", new SlotHasFacetPEv());
    ht.put("SLOT-HAS-VALUE-P", new SlotHasValuePEv());
    ht.put("SLOT-P", new SlotPEv());
    ht.put("SUBCLASS-OF-P", new SubclassOfPEv());
    ht.put("SUPERCLASS-OF-P", new SuperclassOfPEv());
    ht.put("TELL", new TellEv());
    ht.put("TELLABLE", new TellableEv());
    ht.put("TYPE-OF-P", new TypeOfPEv());
    ht.put("UNREGISTER-PROCEDURE", new UnregisterProcedureEv());
    ht.put("UNTELL", new UntellEv());
    ht.put("VALUE-AS-STRING", new ValueAsStringEv());
    ht.put("GET-BEHAVIOR-SUPPORTED-VALUES",
           new GetBehaviorSupportedValuEv());
    ht.put("GET-BEHAVIOR-VALUES", new GetBehaviorValuesEv());
    ht.put("GET-CLASS-INSTANCES", new GetClassInstancesEv());
    ht.put("ENUMERATE-CLASS-INSTANCES", new EnumerateClassInstancesEv());
    ht.put("GET-CLASS-SUBCLASSES", new GetClassSubclassesEv());
    ht.put("ENUMERATE-CLASS-SUBCLASSES",
           new EnumerateClassSubclassesEv());
    ht.put("GET-CLASS-SUPERCLASSES", new GetClassSuperclassesEv());
    ht.put("ENUMERATE-CLASS-SUPERCLASSES",
           new EnumerateClassSuperclassEv());
    ht.put("GET-CLASSES-IN-DOMAIN-OF", new GetClassesInDomainOfEv());
    ht.put("GET-FACET-VALUE", new GetFacetValueEv());
    ht.put("GET-FACET-VALUES", new GetFacetValuesEv());
    ht.put("ENUMERATE-FACET-VALUES", new EnumerateFacetValuesEv());
    ht.put("GET-FACET-VALUES-IN-DETAIL", new GetFacetValuesInDetailEv());
    ht.put("ENUMERATE-FACET-VALUES-IN-DETAIL",
           new EnumerateFacetValuesInDeEv());
    ht.put("GET-FRAME-DETAILS", new GetFrameDetailsEv());
    ht.put("GET-FRAME-HANDLE", new GetFrameHandleEv());
    ht.put("GET-FRAME-IN-KB", new GetFrameInKbEv());
    ht.put("GET-FRAME-NAME", new GetFrameNameEv());
    ht.put("GET-FRAME-PRETTY-NAME", new GetFramePrettyNameEv());
    ht.put("GET-FRAME-SENTENCES", new GetFrameSentencesEv());
    ht.put("GET-FRAME-SLOTS", new GetFrameSlotsEv());
    ht.put("ENUMERATE-FRAME-SLOTS", new EnumerateFrameSlotsEv());
    ht.put("GET-FRAME-TYPE", new GetFrameTypeEv());
    ht.put("GET-FRAMES-MATCHING", new GetFramesMatchingEv());
    ht.put("ENUMERATE-FRAMES-MATCHING", new EnumerateFramesMatchingEv());
    ht.put("GET-FRAMES-WITH-FACET-VALUE",
           new GetFramesWithFacetValueEv());
    ht.put("GET-FRAMES-WITH-SLOT-VALUE", new GetFramesWithSlotValueEv());
    ht.put("GET-INSTANCE-TYPES", new GetInstanceTypesEv());
    ht.put("ENUMERATE-INSTANCE-TYPES", new EnumerateInstanceTypesEv());
    ht.put("GET-KB-BEHAVIORS", new GetKbBehaviorsEv());
    ht.put("GET-KB-CLASSES", new GetKbClassesEv());
    ht.put("ENUMERATE-KB-CLASSES", new EnumerateKbClassesEv());
    ht.put("GET-KB-DIRECT-CHILDREN", new GetKbDirectChildrenEv());
    ht.put("ENUMERATE-KB-DIRECT-CHILDREN",
           new EnumerateKbDirectChildreEv());
    ht.put("GET-KB-DIRECT-PARENTS", new GetKbDirectParentsEv());
    ht.put("ENUMERATE-KB-DIRECT-PARENTS",
           new EnumerateKbDirectParentsEv());
    ht.put("GET-KB-FACETS", new GetKbFacetsEv());
    ht.put("ENUMERATE-KB-FACETS", new EnumerateKbFacetsEv());
    ht.put("GET-KB-FRAMES", new GetKbFramesEv());
    ht.put("ENUMERATE-KB-FRAMES", new EnumerateKbFramesEv());
    ht.put("GET-KB-INDIVIDUALS", new GetKbIndividualsEv());
    ht.put("ENUMERATE-KB-INDIVIDUALS", new EnumerateKbIndividualsEv());
    ht.put("GET-KB-ROOTS", new GetKbRootsEv());
    ht.put("ENUMERATE-KB-ROOTS", new EnumerateKbRootsEv());
    ht.put("GET-KB-SLOTS", new GetKbSlotsEv());
    ht.put("ENUMERATE-KB-SLOTS", new EnumerateKbSlotsEv());
    ht.put("GET-KB-TYPES", new GetKbTypesEv());
    ht.put("ENUMERATE-KB-TYPES", new EnumerateKbTypesEv());
    ht.put("GET-KBS", new GetKbsEv());
    ht.put("ENUMERATE-KBS", new EnumerateKbsEv());
    ht.put("GET-KBS-OF-TYPE", new GetKbsOfTypeEv());
    ht.put("ENUMERATE-KBS-OF-TYPE", new EnumerateKbsOfTypeEv());
    ht.put("GET-PROCEDURE", new GetProcedureEv());
    ht.put("GET-SLOT-FACETS", new GetSlotFacetsEv());
    ht.put("ENUMERATE-SLOT-FACETS", new EnumerateSlotFacetsEv());
    ht.put("GET-SLOT-TYPE", new GetSlotTypeEv());
    ht.put("GET-SLOT-VALUE", new GetSlotValueEv());
    ht.put("GET-SLOT-VALUES", new GetSlotValuesEv());
    ht.put("ENUMERATE-SLOT-VALUES", new EnumerateSlotValuesEv());
    ht.put("GET-SLOT-VALUES-IN-DETAIL", new GetSlotValuesInDetailEv());
    ht.put("ENUMERATE-SLOT-VALUES-IN-DETAIL",
           new EnumerateSlotValuesInDetEv());
    ht.put("GOTO-KB", new GotoKbEv());
    ht.put("INDIVIDUAL-P", new IndividualPEv());
    ht.put("INSTANCE-OF-P", new InstanceOfPEv());
    ht.put("KB-MODIFIED-P", new KbModifiedPEv());
    ht.put("ADD-CLASS-SUPERCLASS", new AddClassSuperclassEv());
    ht.put("ADD-FACET-VALUE", new AddFacetValueEv());
    ht.put("ADD-INSTANCE-TYPE", new AddInstanceTypeEv());
    ht.put("ADD-SLOT-VALUE", new AddSlotValueEv());
    ht.put("ALLOCATE-FRAME-HANDLE", new AllocateFrameHandleEv());
    ht.put("ASK", new AskEv());
    ht.put("ENUMERATE-ASK", new EnumerateAskEv());
    ht.put("ASKABLE", new AskableEv());
    ht.put("ATTACH-FACET", new AttachFacetEv());
    ht.put("ATTACH-SLOT", new AttachSlotEv());
    ht.put("CALL-PROCEDURE", new CallProcedureEv());
    ht.put("ENUMERATE-CALL-PROCEDURE", new EnumerateCallProcedureEv());
    ht.put("CLASS-P", new ClassPEv());
    ht.put("CLOSE-KB", new CloseKbEv());
    ht.put("COERCE-TO-CLASS", new CoerceToClassEv());
    ht.put("COERCE-TO-FACET", new CoerceToFacetEv());
    ht.put("COERCE-TO-FRAME", new CoerceToFrameEv());
    ht.put("COERCE-TO-INDIVIDUAL", new CoerceToIndividualEv());
    ht.put("COERCE-TO-KB-VALUE", new CoerceToKbValueEv());
    ht.put("COERCE-TO-SLOT", new CoerceToSlotEv());
    ht.put("COERCIBLE-TO-FRAME-P", new CoercibleToFramePEv());
    ht.put("COPY-FRAME", new CopyFrameEv());
    ht.put("COPY-KB", new CopyKbEv());
    ht.put("CREATE-CLASS", new CreateClassEv());
    ht.put("CREATE-FACET", new CreateFacetEv());
    ht.put("CREATE-FRAME", new CreateFrameEv());
    ht.put("CREATE-INDIVIDUAL", new CreateIndividualEv());
    ht.put("CREATE-KB", new CreateKbEv());
    ht.put("CREATE-KB-LOCATOR", new CreateKbLocatorEv());
    ht.put("CREATE-PROCEDURE", new CreateProcedureEv());
    ht.put("CREATE-SLOT", new CreateSlotEv());
    ht.put("DECONTEXTUALIZE", new DecontextualizeEv());
    ht.put("DELETE-FACET", new DeleteFacetEv());
    ht.put("DELETE-FRAME", new DeleteFrameEv());
    ht.put("DELETE-SLOT", new DeleteSlotEv());
    ht.put("DETACH-FACET", new DetachFacetEv());
    ht.put("DETACH-SLOT", new DetachSlotEv());
    ht.put("EQL-IN-KB", new EqlInKbEv());
    ht.put("EQUAL-IN-KB", new EqualInKbEv());
    ht.put("EQUALP-IN-KB", new EqualpInKbEv());
    ht.put("EXPUNGE-KB", new ExpungeKbEv());
    ht.put("FACET-HAS-VALUE-P", new FacetHasValuePEv());
    ht.put("FACET-P", new FacetPEv());
    ht.put("FIND-KB-LOCATOR", new FindKbLocatorEv());
    ht.put("FIND-KB-OF-TYPE", new FindKbOfTypeEv());
    ht.put("FOLLOW-SLOT-CHAIN", new FollowSlotChainEv());
    ht.put("FRAME-HAS-SLOT-P", new FrameHasSlotPEv());
    ht.put("FRAME-IN-KB-P", new FrameInKbPEv());
    ht.put("FRS-INDEPENDENT-FRAME-HANDLE",
           new FrsIndependentFrameHandlEv());
    ht.put("FRS-NAME", new FrsNameEv());
  }
}



class MemberBehaviorValuesPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node behavior = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "member-behavior-values-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.member_behavior_values_p(behavior,
                                       value);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node behavior = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.member_behavior_values_p(behavior,
                                       value);
  }
}

class MemberFacetValuePEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node test = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("TEST"))
        { test = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "member-facet-value-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (test == null) { test = Kb._equal; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.member_facet_value_p(frame,
                                   slot,
                                   facet,
                                   value,
                                   inference_level,
                                   test,
                                   slot_type,
                                   value_selector,
                                   kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node test = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.member_facet_value_p(frame,
                                   slot,
                                   facet,
                                   value,
                                   inference_level,
                                   test,
                                   slot_type,
                                   value_selector,
                                   kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class MemberSlotValuePEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node test = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("TEST"))
        { test = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "member-slot-value-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (test == null) { test = Kb._equal; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.member_slot_value_p(frame,
                                  slot,
                                  value,
                                  inference_level,
                                  test,
                                  slot_type,
                                  value_selector,
                                  kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node test = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.member_slot_value_p(frame,
                                  slot,
                                  value,
                                  inference_level,
                                  test,
                                  slot_type,
                                  value_selector,
                                  kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class MetaKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "meta-kb.")).signal();
	   }
    }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.meta_kb(connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.meta_kb(connection);
  }
}

class NextEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Enumerator enumerator = toEnumerator(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    return enumerator.next();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Enumerator enumerator = toEnumerator(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.next(enumerator);
  }
}

class OpenKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node kb_locator = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = null;
    Connection connection = null;
    TruthValue error_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "open-kb.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    if (error_p == null) { error_p = Kb._T; }
    _env_.record_last_kb(kb_type);
    return kb_type.open_kb(kb_locator,
                           connection,
                           error_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node kb_locator = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type);
    return kb_type.open_kb(kb_locator,
                           connection,
                           error_p);
  }
}

class OpenableKbsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb_type = null;
    Connection connection = null;
    Node place = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else if (pname.equalsIgnoreCase("PLACE"))
        { place = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "openable-kbs.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    if (place == null) { place = Kb._NIL; }
    _env_.record_last_kb(kb_type);
    return kb_type.openable_kbs(connection,
                                place);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node place = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type);
    return kb_type.openable_kbs(connection,
                                place);
  }
}

class PrefetchEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Enumerator enumerator = toEnumerator(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = null;
    Node increment = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("INCREMENT"))
        { increment = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "prefetch.")).signal();
	   }
    }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (increment == null) { increment = Kb._NIL; }
    enumerator.prefetch(number_of_values,
                        increment);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Enumerator enumerator = toEnumerator(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node increment = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb.prefetch(enumerator,
                number_of_values,
                increment);
    return Kb._NIL;
  }
}

class PrimitivePEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "primitive-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.primitive_p(the_class,
                          kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.primitive_p(the_class,
                          kb_local_only_p);
  }
}

class PrintFrameEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node slots = null;
    Node facets = null;
    Node stream = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOTS"))
        { slots = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("FACETS"))
        { facets = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("STREAM"))
        { stream = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "print-frame.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slots == null) { slots = Kb._filled; }
    if (facets == null) { facets = Kb._filled; }
    if (stream == null) { stream = Kb._T; }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.print_frame(frame,
                          slots,
                          facets,
                          stream,
                          inference_level,
                          number_of_values,
                          value_selector,
                          kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slots = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facets = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node stream = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.print_frame(frame,
                          slots,
                          facets,
                          stream,
                          inference_level,
                          number_of_values,
                          value_selector,
                          kb_local_only_p);
  }
}

class PutBehaviorValuesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node behavior = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node values = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-behavior-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    kb.put_behavior_values(behavior,
                           values);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node behavior = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_behavior_values(behavior,
                           values);
    return Kb._NIL;
  }
}

class PutClassSuperclassesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_superclasses = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-class-superclasses.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.put_class_superclasses(the_class,
                              new_superclasses,
                              kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_superclasses = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_class_superclasses(the_class,
                              new_superclasses,
                              kb_local_only_p);
    return Kb._NIL;
  }
}

class PutFacetValueEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-facet-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.put_facet_value(frame,
                       slot,
                       facet,
                       value,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_facet_value(frame,
                       slot,
                       facet,
                       value,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
    return Kb._NIL;
  }
}

class PutFacetValuesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node values = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-facet-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.put_facet_values(frame,
                        slot,
                        facet,
                        values,
                        slot_type,
                        value_selector,
                        kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_facet_values(frame,
                        slot,
                        facet,
                        values,
                        slot_type,
                        value_selector,
                        kb_local_only_p);
    return Kb._NIL;
  }
}

class PutFrameDetailsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node details = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-frame-details.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.put_frame_details(frame,
                         details,
                         kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node details = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_frame_details(frame,
                         details,
                         kb_local_only_p);
    return Kb._NIL;
  }
}

class PutFrameNameEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-frame-name.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.put_frame_name(frame,
                             new_name,
                             kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.put_frame_name(frame,
                             new_name,
                             kb_local_only_p);
  }
}

class PutFramePrettyNameEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-frame-pretty-name.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.put_frame_pretty_name(frame,
                             name,
                             kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_frame_pretty_name(frame,
                             name,
                             kb_local_only_p);
    return Kb._NIL;
  }
}

class PutInstanceTypesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_types = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-instance-types.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.put_instance_types(frame,
                          new_types,
                          kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_types = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_instance_types(frame,
                          new_types,
                          kb_local_only_p);
    return Kb._NIL;
  }
}

class PutSlotValueEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-slot-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.put_slot_value(frame,
                      slot,
                      value,
                      slot_type,
                      value_selector,
                      kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_slot_value(frame,
                      slot,
                      value,
                      slot_type,
                      value_selector,
                      kb_local_only_p);
    return Kb._NIL;
  }
}

class PutSlotValuesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node values = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "put-slot-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.put_slot_values(frame,
                       slot,
                       values,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.put_slot_values(frame,
                       slot,
                       values,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
    return Kb._NIL;
  }
}

class RegisterProcedureEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node procedure = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "register-procedure.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    kb.register_procedure(name,
                          procedure);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node procedure = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.register_procedure(name,
                          procedure);
    return Kb._NIL;
  }
}

class RemoveClassSuperclassEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node superclass_to_remove = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "remove-class-superclass.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.remove_class_superclass(the_class,
                               superclass_to_remove,
                               kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node superclass_to_remove = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.remove_class_superclass(the_class,
                               superclass_to_remove,
                               kb_local_only_p);
    return Kb._NIL;
  }
}

class RemoveFacetValueEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node test = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("TEST"))
        { test = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "remove-facet-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (test == null) { test = Kb._equal; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.remove_facet_value(frame,
                          slot,
                          facet,
                          value,
                          test,
                          slot_type,
                          value_selector,
                          kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node test = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.remove_facet_value(frame,
                          slot,
                          facet,
                          value,
                          test,
                          slot_type,
                          value_selector,
                          kb_local_only_p);
    return Kb._NIL;
  }
}

class RemoveInstanceTypeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node type_to_remove = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "remove-instance-type.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.remove_instance_type(frame,
                            type_to_remove,
                            kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node type_to_remove = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.remove_instance_type(frame,
                            type_to_remove,
                            kb_local_only_p);
    return Kb._NIL;
  }
}

class RemoveLocalFacetValuesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "remove-local-facet-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.remove_local_facet_values(frame,
                                 slot,
                                 facet,
                                 slot_type,
                                 value_selector,
                                 kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.remove_local_facet_values(frame,
                                 slot,
                                 facet,
                                 slot_type,
                                 value_selector,
                                 kb_local_only_p);
    return Kb._NIL;
  }
}

class RemoveLocalSlotValuesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "remove-local-slot-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.remove_local_slot_values(frame,
                                slot,
                                slot_type,
                                value_selector,
                                kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.remove_local_slot_values(frame,
                                slot,
                                slot_type,
                                value_selector,
                                kb_local_only_p);
    return Kb._NIL;
  }
}

class RemoveSlotValueEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node test = null;
    SlotType slot_type = null;
    Node index = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("TEST"))
        { test = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("INDEX"))
        { index = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "remove-slot-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (test == null) { test = Kb._equal; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (index == null) { index = Kb._all; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.remove_slot_value(frame,
                         slot,
                         value,
                         test,
                         slot_type,
                         index,
                         value_selector,
                         kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node test = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node index = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.remove_slot_value(frame,
                         slot,
                         value,
                         test,
                         slot_type,
                         index,
                         value_selector,
                         kb_local_only_p);
    return Kb._NIL;
  }
}

class RenameFacetEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "rename-facet.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.rename_facet(facet,
                           new_name,
                           kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.rename_facet(facet,
                           new_name,
                           kb_local_only_p);
  }
}

class RenameSlotEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "rename-slot.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.rename_slot(slot,
                          new_name,
                          kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.rename_slot(slot,
                          new_name,
                          kb_local_only_p);
  }
}

class ReplaceFacetValueEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node old_value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node test = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("TEST"))
        { test = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "replace-facet-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (test == null) { test = Kb._equal; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.replace_facet_value(frame,
                           slot,
                           facet,
                           old_value,
                           new_value,
                           test,
                           slot_type,
                           value_selector,
                           kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node old_value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node test = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.replace_facet_value(frame,
                           slot,
                           facet,
                           old_value,
                           new_value,
                           test,
                           slot_type,
                           value_selector,
                           kb_local_only_p);
    return Kb._NIL;
  }
}

class ReplaceSlotValueEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node old_value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node test = null;
    SlotType slot_type = null;
    Node index = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("TEST"))
        { test = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("INDEX"))
        { index = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "replace-slot-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (test == null) { test = Kb._equal; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (index == null) { index = Kb._all; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.replace_slot_value(frame,
                          slot,
                          old_value,
                          new_value,
                          test,
                          slot_type,
                          index,
                          value_selector,
                          kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node old_value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node test = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node index = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.replace_slot_value(frame,
                          slot,
                          old_value,
                          new_value,
                          test,
                          slot_type,
                          index,
                          value_selector,
                          kb_local_only_p);
    return Kb._NIL;
  }
}

class RevertKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    TruthValue error_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "revert-kb.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    _env_.record_last_kb(kb);
    return kb.revert_kb(error_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.revert_kb(error_p);
  }
}

class SaveKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    TruthValue error_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "save-kb.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    _env_.record_last_kb(kb);
    return kb.save_kb(error_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.save_kb(error_p);
  }
}

class SaveKbAsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node new_name_or_locator = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "save-kb-as.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    kb.save_kb_as(new_name_or_locator);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node new_name_or_locator = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.save_kb_as(new_name_or_locator);
    return Kb._NIL;
  }
}

class SlotHasFacetPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "slot-has-facet-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.slot_has_facet_p(frame,
                               slot,
                               facet,
                               inference_level,
                               slot_type,
                               kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.slot_has_facet_p(frame,
                               slot,
                               facet,
                               inference_level,
                               slot_type,
                               kb_local_only_p);
  }
}

class SlotHasValuePEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "slot-has-value-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.slot_has_value_p(frame,
                               slot,
                               inference_level,
                               slot_type,
                               value_selector,
                               kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.slot_has_value_p(frame,
                               slot,
                               inference_level,
                               slot_type,
                               value_selector,
                               kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class SlotPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "slot-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.slot_p(thing,
                     kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.slot_p(thing,
                     kb_local_only_p);
  }
}

class SubclassOfPEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node subclass = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node superclass = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "subclass-of-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.subclass_of_p(subclass,
                            superclass,
                            inference_level,
                            kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node subclass = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node superclass = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.subclass_of_p(subclass,
                            superclass,
                            inference_level,
                            kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class SuperclassOfPEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node superclass = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node subclass = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "superclass-of-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.superclass_of_p(superclass,
                              subclass,
                              inference_level,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node superclass = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node subclass = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.superclass_of_p(superclass,
                              subclass,
                              inference_level,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class TellEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node sentence = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node frame = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("FRAME"))
        { frame = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "tell.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (frame == null) { frame = Kb._NIL; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.tell(sentence,
            frame,
            value_selector,
            kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node sentence = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.tell(sentence,
            frame,
            value_selector,
            kb_local_only_p);
    return Kb._NIL;
  }
}

class TellableEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node sentence = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "tellable.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.tellable(sentence,
                       value_selector,
                       kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node sentence = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.tellable(sentence,
                       value_selector,
                       kb_local_only_p);
  }
}

class TypeOfPEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "type-of-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.type_of_p(the_class,
                        thing,
                        inference_level,
                        kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.type_of_p(the_class,
                        thing,
                        inference_level,
                        kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class UnregisterProcedureEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "unregister-procedure.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    kb.unregister_procedure(name);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.unregister_procedure(name);
    return Kb._NIL;
  }
}

class UntellEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node sentence = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node frame = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("FRAME"))
        { frame = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "untell.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (frame == null) { frame = Kb._NIL; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.untell(sentence,
                     frame,
                     value_selector,
                     kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node sentence = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.untell(sentence,
                     frame,
                     value_selector,
                     kb_local_only_p);
  }
}

class ValueAsStringEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node purpose = null;
    Node pretty_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("PURPOSE"))
        { purpose = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("PRETTY-P"))
        { pretty_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "value-as-string.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (purpose == null) { purpose = Kb._user_interface; }
    if (pretty_p == null) 
      { pretty_p = (purpose == Kb._user_interface ? (Node)Kb._T : (Node)Kb._NIL); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.value_as_string(value,
                              purpose,
                              pretty_p,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node purpose = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node pretty_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.value_as_string(value,
                              purpose,
                              pretty_p,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class GetBehaviorSupportedValuEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node behavior = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-behavior-supported-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.get_behavior_supported_values(behavior);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node behavior = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_behavior_supported_values(behavior);
  }
}

class GetBehaviorValuesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node behavior = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-behavior-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.get_behavior_values(behavior);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node behavior = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_behavior_values(behavior);
  }
}

class GetClassInstancesEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-class-instances.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_class_instances(the_class,
                                  inference_level,
                                  number_of_values,
                                  kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_class_instances(the_class,
                                  inference_level,
                                  number_of_values,
                                  kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateClassInstancesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-class-instances.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_class_instances(the_class,
                                        inference_level,
                                        number_of_values,
                                        kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_class_instances(the_class,
                                        inference_level,
                                        number_of_values,
                                        kb_local_only_p);
  }
}

class GetClassSubclassesEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-class-subclasses.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_class_subclasses(the_class,
                                   inference_level,
                                   number_of_values,
                                   kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_class_subclasses(the_class,
                                   inference_level,
                                   number_of_values,
                                   kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateClassSubclassesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-class-subclasses.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_class_subclasses(the_class,
                                         inference_level,
                                         number_of_values,
                                         kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_class_subclasses(the_class,
                                         inference_level,
                                         number_of_values,
                                         kb_local_only_p);
  }
}

class GetClassSuperclassesEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-class-superclasses.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_class_superclasses(the_class,
                                     inference_level,
                                     number_of_values,
                                     kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_class_superclasses(the_class,
                                     inference_level,
                                     number_of_values,
                                     kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateClassSuperclassEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-class-superclasses.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_class_superclasses(the_class,
                                           inference_level,
                                           number_of_values,
                                           kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_class_superclasses(the_class,
                                           inference_level,
                                           number_of_values,
                                           kb_local_only_p);
  }
}

class GetClassesInDomainOfEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node frame_attachment = null;
    InferenceLevel inference_level = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("FRAME-ATTACHMENT"))
        { frame_attachment = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-classes-in-domain-of.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (frame_attachment == null) { frame_attachment = Kb._NIL; }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_classes_in_domain_of(slot,
                                       frame_attachment,
                                       inference_level,
                                       kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node frame_attachment = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_classes_in_domain_of(slot,
                                       frame_attachment,
                                       inference_level,
                                       kb_local_only_p);
  }
}

class GetFacetValueEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-facet-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_facet_value(frame,
                              slot,
                              facet,
                              inference_level,
                              slot_type,
                              value_selector,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_facet_value(frame,
                              slot,
                              facet,
                              inference_level,
                              slot_type,
                              value_selector,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class GetFacetValuesEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-facet-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_facet_values(frame,
                               slot,
                               facet,
                               inference_level,
                               slot_type,
                               number_of_values,
                               value_selector,
                               kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_facet_values(frame,
                               slot,
                               facet,
                               inference_level,
                               slot_type,
                               number_of_values,
                               value_selector,
                               kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateFacetValuesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-facet-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_facet_values(frame,
                                     slot,
                                     facet,
                                     inference_level,
                                     slot_type,
                                     number_of_values,
                                     value_selector,
                                     kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_facet_values(frame,
                                     slot,
                                     facet,
                                     inference_level,
                                     slot_type,
                                     number_of_values,
                                     value_selector,
                                     kb_local_only_p);
  }
}

class GetFacetValuesInDetailEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-facet-values-in-detail.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_facet_values_in_detail(frame,
                                         slot,
                                         facet,
                                         inference_level,
                                         slot_type,
                                         number_of_values,
                                         value_selector,
                                         kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values4 values = (Values4)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_facet_values_in_detail(frame,
                                         slot,
                                         facet,
                                         inference_level,
                                         slot_type,
                                         number_of_values,
                                         value_selector,
                                         kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values4 values = (Values4)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateFacetValuesInDeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-facet-values-in-detail.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_facet_values_in_detail(frame,
                                               slot,
                                               facet,
                                               inference_level,
                                               slot_type,
                                               number_of_values,
                                               value_selector,
                                               kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_facet_values_in_detail(frame,
                                               slot,
                                               facet,
                                               inference_level,
                                               slot_type,
                                               number_of_values,
                                               value_selector,
                                               kb_local_only_p);
  }
}

class GetFrameDetailsEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frame-details.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frame_details(frame,
                                inference_level,
                                number_of_values,
                                kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frame_details(frame,
                                inference_level,
                                number_of_values,
                                kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class GetFrameHandleEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frame-handle.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frame_handle(frame,
                               kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frame_handle(frame,
                               kb_local_only_p);
  }
}

class GetFrameInKbEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue error_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frame-in-kb.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frame_in_kb(thing,
                              error_p,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frame_in_kb(thing,
                              error_p,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class GetFrameNameEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frame-name.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frame_name(frame,
                             kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frame_name(frame,
                             kb_local_only_p);
  }
}

class GetFramePrettyNameEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frame-pretty-name.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frame_pretty_name(frame,
                                    kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frame_pretty_name(frame,
                                    kb_local_only_p);
  }
}

class GetFrameSentencesEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node number_of_values = null;
    Node okbc_sentences_p = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OKBC-SENTENCES-P"))
        { okbc_sentences_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frame-sentences.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (okbc_sentences_p == null) { okbc_sentences_p = Kb._T; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frame_sentences(frame,
                                  number_of_values,
                                  okbc_sentences_p,
                                  value_selector,
                                  kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node okbc_sentences_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frame_sentences(frame,
                                  number_of_values,
                                  okbc_sentences_p,
                                  value_selector,
                                  kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class GetFrameSlotsEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frame-slots.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frame_slots(frame,
                              inference_level,
                              slot_type,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frame_slots(frame,
                              inference_level,
                              slot_type,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateFrameSlotsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-frame-slots.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_frame_slots(frame,
                                    inference_level,
                                    slot_type,
                                    kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_frame_slots(frame,
                                    inference_level,
                                    slot_type,
                                    kb_local_only_p);
  }
}

class GetFrameTypeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frame-type.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frame_type(thing,
                             kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frame_type(thing,
                             kb_local_only_p);
  }
}

class GetFramesMatchingEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node pattern = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node wildcards_allowed_p = null;
    Node selector = null;
    Node force_case_insensitive_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("WILDCARDS-ALLOWED-P"))
        { wildcards_allowed_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("FORCE-CASE-INSENSITIVE-P"))
        { force_case_insensitive_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frames-matching.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (wildcards_allowed_p == null) { wildcards_allowed_p = Kb._T; }
    if (selector == null) { selector = Kb._all; }
    if (force_case_insensitive_p == null) 
      { force_case_insensitive_p = Kb._NIL; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frames_matching(pattern,
                                  wildcards_allowed_p,
                                  selector,
                                  force_case_insensitive_p,
                                  kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node pattern = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node wildcards_allowed_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node force_case_insensitive_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frames_matching(pattern,
                                  wildcards_allowed_p,
                                  selector,
                                  force_case_insensitive_p,
                                  kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateFramesMatchingEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node pattern = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node wildcards_allowed_p = null;
    Node selector = null;
    Node force_case_insensitive_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("WILDCARDS-ALLOWED-P"))
        { wildcards_allowed_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("FORCE-CASE-INSENSITIVE-P"))
        { force_case_insensitive_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-frames-matching.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (wildcards_allowed_p == null) { wildcards_allowed_p = Kb._T; }
    if (selector == null) { selector = Kb._all; }
    if (force_case_insensitive_p == null) 
      { force_case_insensitive_p = Kb._NIL; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_frames_matching(pattern,
                                        wildcards_allowed_p,
                                        selector,
                                        force_case_insensitive_p,
                                        kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node pattern = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node wildcards_allowed_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node force_case_insensitive_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_frames_matching(pattern,
                                        wildcards_allowed_p,
                                        selector,
                                        force_case_insensitive_p,
                                        kb_local_only_p);
  }
}

class GetFramesWithFacetValueEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frames-with-facet-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frames_with_facet_value(slot,
                                          facet,
                                          value,
                                          inference_level,
                                          slot_type,
                                          value_selector,
                                          kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frames_with_facet_value(slot,
                                          facet,
                                          value,
                                          inference_level,
                                          slot_type,
                                          value_selector,
                                          kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class GetFramesWithSlotValueEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-frames-with-slot-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_frames_with_slot_value(slot,
                                         value,
                                         inference_level,
                                         slot_type,
                                         value_selector,
                                         kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_frames_with_slot_value(slot,
                                         value,
                                         inference_level,
                                         slot_type,
                                         value_selector,
                                         kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class GetInstanceTypesEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-instance-types.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_instance_types(frame,
                                 inference_level,
                                 number_of_values,
                                 kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_instance_types(frame,
                                 inference_level,
                                 number_of_values,
                                 kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateInstanceTypesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-instance-types.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_instance_types(frame,
                                       inference_level,
                                       number_of_values,
                                       kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_instance_types(frame,
                                       inference_level,
                                       number_of_values,
                                       kb_local_only_p);
  }
}

class GetKbBehaviorsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb_type_or_kb = null;
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE-OR-KB"))
        { kb_type_or_kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-behaviors.")).signal();
	   }
    }
    if (kb_type_or_kb == null) 
      { kb_type_or_kb = (Kb)getenv(_current_kb, _env_); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    _env_.record_last_kb(kb_type_or_kb);
    return kb_type_or_kb.get_kb_behaviors(connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb_type_or_kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type_or_kb);
    return kb_type_or_kb.get_kb_behaviors(connection);
  }
}

class GetKbClassesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-classes.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._system_default; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_kb_classes(selector,
                             kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_kb_classes(selector,
                             kb_local_only_p);
  }
}

class EnumerateKbClassesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-classes.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._system_default; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_classes(selector,
                                   kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_classes(selector,
                                   kb_local_only_p);
  }
}

class GetKbDirectChildrenEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-direct-children.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.get_kb_direct_children();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_kb_direct_children();
  }
}

class EnumerateKbDirectChildreEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-direct-children.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_direct_children();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_direct_children();
  }
}

class GetKbDirectParentsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-direct-parents.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.get_kb_direct_parents();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_kb_direct_parents();
  }
}

class EnumerateKbDirectParentsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-direct-parents.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_direct_parents();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_direct_parents();
  }
}

class GetKbFacetsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-facets.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._system_default; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_kb_facets(selector,
                            kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_kb_facets(selector,
                            kb_local_only_p);
  }
}

class EnumerateKbFacetsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-facets.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._system_default; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_facets(selector,
                                  kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_facets(selector,
                                  kb_local_only_p);
  }
}

class GetKbFramesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-frames.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_kb_frames(kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_kb_frames(kb_local_only_p);
  }
}

class EnumerateKbFramesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-frames.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_frames(kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_frames(kb_local_only_p);
  }
}

class GetKbIndividualsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-individuals.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._system_default; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_kb_individuals(selector,
                                 kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_kb_individuals(selector,
                                 kb_local_only_p);
  }
}

class EnumerateKbIndividualsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-individuals.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._system_default; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_individuals(selector,
                                       kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_individuals(selector,
                                       kb_local_only_p);
  }
}

class GetKbRootsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-roots.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_kb_roots(selector,
                           kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_kb_roots(selector,
                           kb_local_only_p);
  }
}

class EnumerateKbRootsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-roots.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._all; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_roots(selector,
                                 kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_roots(selector,
                                 kb_local_only_p);
  }
}

class GetKbSlotsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-slots.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._system_default; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_kb_slots(selector,
                           kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_kb_slots(selector,
                           kb_local_only_p);
  }
}

class EnumerateKbSlotsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SELECTOR"))
        { selector = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-slots.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (selector == null) { selector = Kb._system_default; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_slots(selector,
                                 kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node selector = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_kb_slots(selector,
                                 kb_local_only_p);
  }
}

class GetKbTypeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-type.")).signal();
	   }
    }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.get_kb_type(thing,
                          connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.get_kb_type(thing,
                          connection);
  }
}

class GetKbTypesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kb-types.")).signal();
	   }
    }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.get_kb_types(connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.get_kb_types(connection);
  }
}

class EnumerateKbTypesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kb-types.")).signal();
	   }
    }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.enumerate_kb_types(connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.enumerate_kb_types(connection);
  }
}

class GetKbsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kbs.")).signal();
	   }
    }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.get_kbs(connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.get_kbs(connection);
  }
}

class EnumerateKbsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kbs.")).signal();
	   }
    }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.enumerate_kbs(connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.enumerate_kbs(connection);
  }
}

class GetKbsOfTypeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb_type = null;
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-kbs-of-type.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    _env_.record_last_kb(kb_type);
    return kb_type.get_kbs_of_type(connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type);
    return kb_type.get_kbs_of_type(connection);
  }
}

class EnumerateKbsOfTypeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb_type = null;
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-kbs-of-type.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    _env_.record_last_kb(kb_type);
    return kb_type.enumerate_kbs_of_type(connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type);
    return kb_type.enumerate_kbs_of_type(connection);
  }
}

class GetProcedureEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-procedure.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.get_procedure(name);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_procedure(name);
  }
}

class GetSlotFacetsEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-slot-facets.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_slot_facets(frame,
                              slot,
                              inference_level,
                              slot_type,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_slot_facets(frame,
                              slot,
                              inference_level,
                              slot_type,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateSlotFacetsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-slot-facets.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_slot_facets(frame,
                                    slot,
                                    inference_level,
                                    slot_type,
                                    kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_slot_facets(frame,
                                    slot,
                                    inference_level,
                                    slot_type,
                                    kb_local_only_p);
  }
}

class GetSlotTypeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-slot-type.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_slot_type(frame,
                            slot,
                            inference_level,
                            kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_slot_type(frame,
                            slot,
                            inference_level,
                            kb_local_only_p);
  }
}

class GetSlotValueEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-slot-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_slot_value(frame,
                             slot,
                             inference_level,
                             slot_type,
                             value_selector,
                             kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_slot_value(frame,
                             slot,
                             inference_level,
                             slot_type,
                             value_selector,
                             kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class GetSlotValuesEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-slot-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_slot_values(frame,
                              slot,
                              inference_level,
                              slot_type,
                              number_of_values,
                              value_selector,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_slot_values(frame,
                              slot,
                              inference_level,
                              slot_type,
                              number_of_values,
                              value_selector,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateSlotValuesEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-slot-values.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_slot_values(frame,
                                    slot,
                                    inference_level,
                                    slot_type,
                                    number_of_values,
                                    value_selector,
                                    kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_slot_values(frame,
                                    slot,
                                    inference_level,
                                    slot_type,
                                    number_of_values,
                                    value_selector,
                                    kb_local_only_p);
  }
}

class GetSlotValuesInDetailEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "get-slot-values-in-detail.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.get_slot_values_in_detail(frame,
                                        slot,
                                        inference_level,
                                        slot_type,
                                        number_of_values,
                                        value_selector,
                                        kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values4 values = (Values4)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.get_slot_values_in_detail(frame,
                                        slot,
                                        inference_level,
                                        slot_type,
                                        number_of_values,
                                        value_selector,
                                        kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values4 values = (Values4)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateSlotValuesInDetEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    Node number_of_values = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-slot-values-in-detail.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_slot_values_in_detail(frame,
                                              slot,
                                              inference_level,
                                              slot_type,
                                              number_of_values,
                                              value_selector,
                                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_slot_values_in_detail(frame,
                                              slot,
                                              inference_level,
                                              slot_type,
                                              number_of_values,
                                              value_selector,
                                              kb_local_only_p);
  }
}

class OkbcConditionPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    return Kb.okbc_condition_p(thing);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.okbc_condition_p(thing);
  }
}

class GotoKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = toKb(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    _env_.record_last_kb(kb);
    _env_.putenv(_current_kb, kb);
    kb.goto_kb();
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    _env_.putenv(_current_kb, kb);
    kb.goto_kb();
    return Kb._NIL;
  }
}

class HasMoreEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Enumerator enumerator = toEnumerator(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    return enumerator.has_more();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Enumerator enumerator = toEnumerator(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.has_more(enumerator);
  }
}

class IndividualPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "individual-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.individual_p(thing,
                           kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.individual_p(thing,
                           kb_local_only_p);
  }
}

class InstanceOfPEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "instance-of-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.instance_of_p(thing,
                            the_class,
                            inference_level,
                            kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.instance_of_p(thing,
                            the_class,
                            inference_level,
                            kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class KbPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    return Kb.kb_p(thing);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.kb_p(thing);
  }
}

class KbModifiedPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "kb-modified-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.kb_modified_p();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.kb_modified_p();
  }
}

class AddClassSuperclassEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node the_class = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_superclass = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "add-class-superclass.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.add_class_superclass(the_class,
                            new_superclass,
                            kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node the_class = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_superclass = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.add_class_superclass(the_class,
                            new_superclass,
                            kb_local_only_p);
    return Kb._NIL;
  }
}

class AddFacetValueEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node test = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("TEST"))
        { test = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "add-facet-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (test == null) { test = Kb._equal; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.add_facet_value(frame,
                       slot,
                       facet,
                       value,
                       test,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node test = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.add_facet_value(frame,
                       slot,
                       facet,
                       value,
                       test,
                       slot_type,
                       value_selector,
                       kb_local_only_p);
    return Kb._NIL;
  }
}

class AddInstanceTypeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_type = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "add-instance-type.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.add_instance_type(frame,
                         new_type,
                         kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_type = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.add_instance_type(frame,
                         new_type,
                         kb_local_only_p);
    return Kb._NIL;
  }
}

class AddSlotValueEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node test = null;
    SlotType slot_type = null;
    Node add_before = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("TEST"))
        { test = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("ADD-BEFORE"))
        { add_before = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "add-slot-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (test == null) { test = Kb._equal; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (add_before == null) { add_before = new OKBCInteger(0); }
    if (value_selector == null) { value_selector = Kb._known_true; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.add_slot_value(frame,
                      slot,
                      value,
                      test,
                      slot_type,
                      add_before,
                      value_selector,
                      kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node test = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node add_before = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.add_slot_value(frame,
                      slot,
                      value,
                      test,
                      slot_type,
                      add_before,
                      value_selector,
                      kb_local_only_p);
    return Kb._NIL;
  }
}

class AllConnectionsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    return Kb.all_connections();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    return Kb.all_connections();
  }
}

class EnumerateAllConnectionsEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    return Kb.enumerate_all_connections();
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    return Kb.enumerate_all_connections();
  }
}

class AllocateFrameHandleEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame_name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ContextName frame_type = toContextName(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node frame_handle = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("FRAME-HANDLE"))
        { frame_handle = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "allocate-frame-handle.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (frame_handle == null) { frame_handle = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.allocate_frame_handle(frame_name,
                                    frame_type,
                                    frame_handle);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ContextName frame_type = toContextName(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node frame_handle = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.allocate_frame_handle(frame_name,
                                    frame_type,
                                    frame_handle);
  }
}

class AskEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node query = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node reply_pattern = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue error_p = null;
    Node where = null;
    Node timeout = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("REPLY-PATTERN"))
        { reply_pattern = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("WHERE"))
        { where = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("TIMEOUT"))
        { timeout = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "ask.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (reply_pattern == null) { reply_pattern = query; }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (error_p == null) { error_p = Kb._T; }
    if (where == null) { where = Kb._NIL; }
    if (timeout == null) { timeout = Kb._NIL; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.ask(query,
                  reply_pattern,
                  inference_level,
                  number_of_values,
                  error_p,
                  where,
                  timeout,
                  value_selector,
                  kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node query = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node reply_pattern = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node where = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node timeout = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.ask(query,
                  reply_pattern,
                  inference_level,
                  number_of_values,
                  error_p,
                  where,
                  timeout,
                  value_selector,
                  kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class EnumerateAskEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node query = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node reply_pattern = null;
    InferenceLevel inference_level = null;
    Node number_of_values = null;
    TruthValue error_p = null;
    Node where = null;
    Node timeout = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("REPLY-PATTERN"))
        { reply_pattern = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("WHERE"))
        { where = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("TIMEOUT"))
        { timeout = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-ask.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (reply_pattern == null) { reply_pattern = query; }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (number_of_values == null) { number_of_values = Kb._all; }
    if (error_p == null) { error_p = Kb._T; }
    if (where == null) { where = Kb._NIL; }
    if (timeout == null) { timeout = Kb._NIL; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_ask(query,
                            reply_pattern,
                            inference_level,
                            number_of_values,
                            error_p,
                            where,
                            timeout,
                            value_selector,
                            kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node query = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node reply_pattern = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node where = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node timeout = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_ask(query,
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

class AskableEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node sentence = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "askable.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.askable(sentence,
                      value_selector,
                      kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node sentence = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.askable(sentence,
                      value_selector,
                      kb_local_only_p);
  }
}

class AttachFacetEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "attach-facet.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.attach_facet(frame,
                    slot,
                    facet,
                    slot_type,
                    kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.attach_facet(frame,
                    slot,
                    facet,
                    slot_type,
                    kb_local_only_p);
    return Kb._NIL;
  }
}

class AttachSlotEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "attach-slot.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.attach_slot(frame,
                   slot,
                   slot_type,
                   kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.attach_slot(frame,
                   slot,
                   slot_type,
                   kb_local_only_p);
    return Kb._NIL;
  }
}

class CallProcedureEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node procedure = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node arguments = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ARGUMENTS"))
        { arguments = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "call-procedure.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (arguments == null) { arguments = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.call_procedure(procedure,
                             arguments);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node procedure = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arguments = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.call_procedure(procedure,
                             arguments);
  }
}

class EnumerateCallProcedureEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node procedure = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node arguments = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ARGUMENTS"))
        { arguments = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "enumerate-call-procedure.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (arguments == null) { arguments = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.enumerate_call_procedure(procedure,
                                       arguments);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node procedure = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arguments = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.enumerate_call_procedure(procedure,
                                       arguments);
  }
}

class ClassPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "class-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.class_p(thing,
                      kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.class_p(thing,
                      kb_local_only_p);
  }
}

class CloseConnectionEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Connection connection = toConnection(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node force_p = null;
    TruthValue error_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("FORCE-P"))
        { force_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "close-connection.")).signal();
	   }
    }
    if (force_p == null) { force_p = Kb._NIL; }
    if (error_p == null) { error_p = Kb._T; }
    Kb.close_connection(connection,
                        force_p,
                        error_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node force_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb.close_connection(connection,
                        force_p,
                        error_p);
    return Kb._NIL;
  }
}

class CloseKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node save_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SAVE-P"))
        { save_p = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "close-kb.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (save_p == null) { save_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.close_kb(save_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node save_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.close_kb(save_p);
    return Kb._NIL;
  }
}

class CoerceToClassEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue error_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "coerce-to-class.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.coerce_to_class(thing,
                              error_p,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.coerce_to_class(thing,
                              error_p,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class CoerceToFacetEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue error_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "coerce-to-facet.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.coerce_to_facet(thing,
                              error_p,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.coerce_to_facet(thing,
                              error_p,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class CoerceToFrameEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue error_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "coerce-to-frame.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.coerce_to_frame(thing,
                              error_p,
                              kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.coerce_to_frame(thing,
                              error_p,
                              kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class CoerceToIndividualEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue error_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "coerce-to-individual.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.coerce_to_individual(thing,
                                   error_p,
                                   kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.coerce_to_individual(thing,
                                   error_p,
                                   kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class CoerceToKbValueEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node string_or_value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ContextName target_context = toContextName(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node wildcards_allowed_p = null;
    Node force_case_insensitive_p = null;
    TruthValue error_p = null;
    Node frame_action = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("WILDCARDS-ALLOWED-P"))
        { wildcards_allowed_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("FORCE-CASE-INSENSITIVE-P"))
        { force_case_insensitive_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("FRAME-ACTION"))
        { frame_action = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "coerce-to-kb-value.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (wildcards_allowed_p == null) { wildcards_allowed_p = Kb._NIL; }
    if (force_case_insensitive_p == null) 
      { force_case_insensitive_p = Kb._NIL; }
    if (error_p == null) { error_p = Kb._T; }
    if (frame_action == null) { frame_action = Kb._error_if_not_unique; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.coerce_to_kb_value(string_or_value,
                                 target_context,
                                 wildcards_allowed_p,
                                 force_case_insensitive_p,
                                 error_p,
                                 frame_action,
                                 kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node string_or_value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ContextName target_context = toContextName(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node wildcards_allowed_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node force_case_insensitive_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node frame_action = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.coerce_to_kb_value(string_or_value,
                                 target_context,
                                 wildcards_allowed_p,
                                 force_case_insensitive_p,
                                 error_p,
                                 frame_action,
                                 kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values3 values = (Values3)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class CoerceToSlotEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue error_p = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "coerce-to-slot.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.coerce_to_slot(thing,
                             error_p,
                             kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.coerce_to_slot(thing,
                             error_p,
                             kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class CoercibleToFramePEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "coercible-to-frame-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.coercible_to_frame_p(thing,
                                   kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.coercible_to_frame_p(thing,
                                   kb_local_only_p);
  }
}

class CopyFrameEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Kb to_kb = null;
    TruthValue error_p = null;
    Node missing_frame_action = null;
    Node frame_handle_mapping_table = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("TO-KB"))
        { to_kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("MISSING-FRAME-ACTION"))
        { missing_frame_action = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("FRAME-HANDLE-MAPPING-TABLE"))
        { frame_handle_mapping_table = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "copy-frame.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (to_kb == null) { to_kb = (Kb)getenv(_current_kb, _env_); }
    if (error_p == null) { error_p = Kb._T; }
    if (missing_frame_action == null) { missing_frame_action = Kb._stop; }
    if (frame_handle_mapping_table == null) 
      { frame_handle_mapping_table = Kb._NIL; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    return Kb.copy_frame(frame,
                         new_name,
                         kb,
                         to_kb,
                         error_p,
                         missing_frame_action,
                         frame_handle_mapping_table,
                         kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node new_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb to_kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node missing_frame_action = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node frame_handle_mapping_table = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.copy_frame(frame,
                         new_name,
                         kb,
                         to_kb,
                         error_p,
                         missing_frame_action,
                         frame_handle_mapping_table,
                         kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class CopyKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb from_kb = toKb(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb to_kb = toKb(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = null;
    Node missing_frame_action = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else if (pname.equalsIgnoreCase("MISSING-FRAME-ACTION"))
        { missing_frame_action = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "copy-kb.")).signal();
	   }
    }
    if (error_p == null) { error_p = Kb._T; }
    if (missing_frame_action == null) { missing_frame_action = Kb._stop; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    Kb.copy_kb(from_kb,
               to_kb,
               error_p,
               missing_frame_action,
               kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb from_kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb to_kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node missing_frame_action = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb.copy_kb(from_kb,
               to_kb,
               error_p,
               missing_frame_action,
               kb_local_only_p);
    return Kb._NIL;
  }
}

class CreateClassEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node direct_types = null;
    Node direct_superclasses = null;
    Node primitive_p = null;
    Node doc = null;
    Node template_slots = null;
    Node template_facets = null;
    Node own_slots = null;
    Node own_facets = null;
    Node handle = null;
    Node pretty_name = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("DIRECT-TYPES"))
        { direct_types = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("DIRECT-SUPERCLASSES"))
        { direct_superclasses = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("PRIMITIVE-P"))
        { primitive_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("DOC"))
        { doc = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("TEMPLATE-SLOTS"))
        { template_slots = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("TEMPLATE-FACETS"))
        { template_facets = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-SLOTS"))
        { own_slots = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-FACETS"))
        { own_facets = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("HANDLE"))
        { handle = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("PRETTY-NAME"))
        { pretty_name = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "create-class.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (direct_types == null) { direct_types = Kb._NIL; }
    if (direct_superclasses == null) { direct_superclasses = Kb._NIL; }
    if (primitive_p == null) { primitive_p = Kb._T; }
    if (doc == null) { doc = Kb._NIL; }
    if (template_slots == null) { template_slots = Kb._NIL; }
    if (template_facets == null) { template_facets = Kb._NIL; }
    if (own_slots == null) { own_slots = Kb._NIL; }
    if (own_facets == null) { own_facets = Kb._NIL; }
    if (handle == null) { handle = Kb._NIL; }
    if (pretty_name == null) { pretty_name = Kb._NIL; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.create_class(name,
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

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node direct_types = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node direct_superclasses = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node primitive_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node doc = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node template_slots = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node template_facets = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_slots = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_facets = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node handle = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node pretty_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.create_class(name,
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
}

class CreateFacetEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node frame_or_nil = null;
    Node slot_or_nil = null;
    SlotType slot_type = null;
    Node direct_types = null;
    Node doc = null;
    Node own_slots = null;
    Node own_facets = null;
    Node handle = null;
    Node pretty_name = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("FRAME-OR-NIL"))
        { frame_or_nil = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-OR-NIL"))
        { slot_or_nil = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("DIRECT-TYPES"))
        { direct_types = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("DOC"))
        { doc = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-SLOTS"))
        { own_slots = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-FACETS"))
        { own_facets = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("HANDLE"))
        { handle = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("PRETTY-NAME"))
        { pretty_name = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "create-facet.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (frame_or_nil == null) { frame_or_nil = Kb._NIL; }
    if (slot_or_nil == null) { slot_or_nil = Kb._NIL; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (direct_types == null) { direct_types = Kb._NIL; }
    if (doc == null) { doc = Kb._NIL; }
    if (own_slots == null) { own_slots = Kb._NIL; }
    if (own_facets == null) { own_facets = Kb._NIL; }
    if (handle == null) { handle = Kb._NIL; }
    if (pretty_name == null) { pretty_name = Kb._NIL; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.create_facet(name,
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

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node frame_or_nil = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot_or_nil = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node direct_types = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node doc = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_slots = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_facets = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node handle = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node pretty_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.create_facet(name,
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
}

class CreateFrameEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ContextName frame_type = toContextName(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node direct_types = null;
    Node direct_superclasses = null;
    Node doc = null;
    Node template_slots = null;
    Node template_facets = null;
    Node own_slots = null;
    Node own_facets = null;
    Node primitive_p = null;
    Node handle = null;
    Node pretty_name = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("DIRECT-TYPES"))
        { direct_types = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("DIRECT-SUPERCLASSES"))
        { direct_superclasses = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("DOC"))
        { doc = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("TEMPLATE-SLOTS"))
        { template_slots = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("TEMPLATE-FACETS"))
        { template_facets = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-SLOTS"))
        { own_slots = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-FACETS"))
        { own_facets = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("PRIMITIVE-P"))
        { primitive_p = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("HANDLE"))
        { handle = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("PRETTY-NAME"))
        { pretty_name = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "create-frame.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (direct_types == null) { direct_types = Kb._NIL; }
    if (direct_superclasses == null) { direct_superclasses = Kb._NIL; }
    if (doc == null) { doc = Kb._NIL; }
    if (template_slots == null) { template_slots = Kb._NIL; }
    if (template_facets == null) { template_facets = Kb._NIL; }
    if (own_slots == null) { own_slots = Kb._NIL; }
    if (own_facets == null) { own_facets = Kb._NIL; }
    if (primitive_p == null) { primitive_p = Kb._T; }
    if (handle == null) { handle = Kb._NIL; }
    if (pretty_name == null) { pretty_name = Kb._NIL; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.create_frame(name,
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

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ContextName frame_type = toContextName(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node direct_types = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node direct_superclasses = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node doc = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node template_slots = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node template_facets = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_slots = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_facets = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node primitive_p = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node handle = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node pretty_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.create_frame(name,
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
}

class CreateIndividualEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node direct_types = null;
    Node doc = null;
    Node own_slots = null;
    Node own_facets = null;
    Node handle = null;
    Node pretty_name = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("DIRECT-TYPES"))
        { direct_types = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("DOC"))
        { doc = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-SLOTS"))
        { own_slots = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-FACETS"))
        { own_facets = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("HANDLE"))
        { handle = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("PRETTY-NAME"))
        { pretty_name = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "create-individual.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (direct_types == null) { direct_types = Kb._NIL; }
    if (doc == null) { doc = Kb._NIL; }
    if (own_slots == null) { own_slots = Kb._NIL; }
    if (own_facets == null) { own_facets = Kb._NIL; }
    if (handle == null) { handle = Kb._NIL; }
    if (pretty_name == null) { pretty_name = Kb._NIL; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.create_individual(name,
                                direct_types,
                                doc,
                                own_slots,
                                own_facets,
                                handle,
                                pretty_name,
                                kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node direct_types = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node doc = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_slots = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_facets = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node handle = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node pretty_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.create_individual(name,
                                direct_types,
                                doc,
                                own_slots,
                                own_facets,
                                handle,
                                pretty_name,
                                kb_local_only_p);
  }
}

class CreateKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = null;
    Node kb_locator = null;
    Node initargs = null;
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCATOR"))
        { kb_locator = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("INITARGS"))
        { initargs = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "create-kb.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (kb_locator == null) { kb_locator = Kb._NIL; }
    if (initargs == null) { initargs = Kb._NIL; }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    _env_.record_last_kb(kb_type);
    return kb_type.create_kb(name,
                             kb_locator,
                             initargs,
                             connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node kb_locator = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node initargs = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type);
    return kb_type.create_kb(name,
                             kb_locator,
                             initargs,
                             connection);
  }
}

class CreateKbLocatorEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = null;
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "create-kb-locator.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    _env_.record_last_kb(kb_type);
    return kb_type.create_kb_locator(thing,
                                     connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type);
    return kb_type.create_kb_locator(thing,
                                     connection);
  }
}

class CreateProcedureEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb = null;
    Node arguments = null;
    Node body = null;
    Node environment = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("ARGUMENTS"))
        { arguments = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("BODY"))
        { body = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("ENVIRONMENT"))
        { environment = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "create-procedure.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (arguments == null) { arguments = Kb._NIL; }
    if (body == null) { body = Kb._NIL; }
    if (environment == null) { environment = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.create_procedure(arguments,
                               body,
                               environment);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arguments = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node body = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node environment = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.create_procedure(arguments,
                               body,
                               environment);
  }
}

class CreateSlotEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node frame_or_nil = null;
    SlotType slot_type = null;
    Node direct_types = null;
    Node doc = null;
    Node own_slots = null;
    Node own_facets = null;
    Node handle = null;
    Node pretty_name = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("FRAME-OR-NIL"))
        { frame_or_nil = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("DIRECT-TYPES"))
        { direct_types = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("DOC"))
        { doc = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-SLOTS"))
        { own_slots = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("OWN-FACETS"))
        { own_facets = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("HANDLE"))
        { handle = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("PRETTY-NAME"))
        { pretty_name = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "create-slot.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (frame_or_nil == null) { frame_or_nil = Kb._NIL; }
    if (slot_type == null) { slot_type = Kb._all; }
    if (direct_types == null) { direct_types = Kb._NIL; }
    if (doc == null) { doc = Kb._NIL; }
    if (own_slots == null) { own_slots = Kb._NIL; }
    if (own_facets == null) { own_facets = Kb._NIL; }
    if (handle == null) { handle = Kb._NIL; }
    if (pretty_name == null) { pretty_name = Kb._NIL; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.create_slot(name,
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

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node frame_or_nil = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node direct_types = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node doc = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_slots = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node own_facets = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node handle = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node pretty_name = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.create_slot(name,
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
}

class DecontextualizeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node value = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ContextName from_context = toContextName(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "decontextualize.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    _env_.record_last_kb(kb);
    return kb.decontextualize(value,
                              from_context);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node value = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ContextName from_context = toContextName(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.decontextualize(value,
                              from_context);
  }
}

class DeleteFacetEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "delete-facet.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.delete_facet(facet,
                    kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.delete_facet(facet,
                    kb_local_only_p);
    return Kb._NIL;
  }
}

class DeleteFrameEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "delete-frame.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.delete_frame(frame,
                    kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.delete_frame(frame,
                    kb_local_only_p);
    return Kb._NIL;
  }
}

class DeleteSlotEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "delete-slot.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.delete_slot(slot,
                   kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.delete_slot(slot,
                   kb_local_only_p);
    return Kb._NIL;
  }
}

class DetachFacetEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "detach-facet.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.detach_facet(frame,
                    slot,
                    facet,
                    slot_type,
                    kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.detach_facet(frame,
                    slot,
                    facet,
                    slot_type,
                    kb_local_only_p);
    return Kb._NIL;
  }
}

class DetachSlotEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "detach-slot.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (slot_type == null) { slot_type = Kb._own; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    kb.detach_slot(frame,
                   slot,
                   slot_type,
                   kb_local_only_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    kb.detach_slot(frame,
                   slot,
                   slot_type,
                   kb_local_only_p);
    return Kb._NIL;
  }
}

class EnumerateListEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node list = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    return Kb.enumerate_list(list);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node list = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.enumerate_list(list);
  }
}

class EqlInKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node arg0 = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arg1 = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "eql-in-kb.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.eql_in_kb(arg0,
                        arg1,
                        kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node arg0 = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arg1 = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.eql_in_kb(arg0,
                        arg1,
                        kb_local_only_p);
  }
}

class EqualInKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node arg0 = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arg1 = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "equal-in-kb.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.equal_in_kb(arg0,
                          arg1,
                          kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node arg0 = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arg1 = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.equal_in_kb(arg0,
                          arg1,
                          kb_local_only_p);
  }
}

class EqualpInKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node arg0 = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arg1 = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "equalp-in-kb.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.equalp_in_kb(arg0,
                           arg1,
                           kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node arg0 = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node arg1 = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.equalp_in_kb(arg0,
                           arg1,
                           kb_local_only_p);
  }
}

class EstablishConnectionEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node connection_type = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node initargs = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("INITARGS"))
        { initargs = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "establish-connection.")).signal();
	   }
    }
    if (initargs == null) { initargs = Kb._NIL; }
    return Kb.establish_connection(connection_type,
                                   initargs);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node connection_type = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node initargs = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.establish_connection(connection_type,
                                   initargs);
  }
}

class ExpungeKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node kb_locator = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = null;
    Connection connection = null;
    TruthValue error_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else if (pname.equalsIgnoreCase("ERROR-P"))
        { error_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "expunge-kb.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    if (error_p == null) { error_p = Kb._T; }
    _env_.record_last_kb(kb_type);
    kb_type.expunge_kb(kb_locator,
                       connection,
                       error_p);
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node kb_locator = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue error_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type);
    kb_type.expunge_kb(kb_locator,
                       connection,
                       error_p);
    return Kb._NIL;
  }
}

class FacetHasValuePEv extends AbstractEvaluator {
  public Object eval_maybe_to_values (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "facet-has-value-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.facet_has_value_p(frame,
                                slot,
                                facet,
                                inference_level,
                                slot_type,
                                value_selector,
                                kb_local_only_p);
  }

  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)eval_maybe_to_values(args, _env_, depth, ckb);
    return values.firstValue();
  }

  public Object positional_eval_maybe_to_values
       
       (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node facet = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.facet_has_value_p(frame,
                                slot,
                                facet,
                                inference_level,
                                slot_type,
                                value_selector,
                                kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Values2 values = (Values2)positional_eval_maybe_to_values
                       (args, _env_, depth, ckb);
    return values.firstValue();
  }
}

class FacetPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "facet-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.facet_p(thing,
                      kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.facet_p(thing,
                      kb_local_only_p);
  }
}

class FetchEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Enumerator enumerator = toEnumerator(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("NUMBER-OF-VALUES"))
        { number_of_values = (Node)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "fetch.")).signal();
	   }
    }
    if (number_of_values == null) { number_of_values = Kb._all; }
    return enumerator.fetch(number_of_values);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Enumerator enumerator = toEnumerator(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node number_of_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.fetch(enumerator,
                    number_of_values);
  }
}

class FindKbEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name_or_kb_or_kb_locator = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "find-kb.")).signal();
	   }
    }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.find_kb(name_or_kb_or_kb_locator,
                      connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name_or_kb_or_kb_locator = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.find_kb(name_or_kb_or_kb_locator,
                      connection);
  }
}

class FindKbLocatorEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = null;
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "find-kb-locator.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    _env_.record_last_kb(kb_type);
    return kb_type.find_kb_locator(thing,
                                   connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb_type);
    return kb_type.find_kb_locator(thing,
                                   connection);
  }
}

class FindKbOfTypeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node name_or_kb = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = null;
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "find-kb-of-type.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.find_kb_of_type(name_or_kb,
                              kb_type,
                              connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node name_or_kb = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.find_kb_of_type(name_or_kb,
                              kb_type,
                              connection);
  }
}

class FollowSlotChainEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot_chain = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    Node union_multiple_values = null;
    InferenceLevel inference_level = null;
    ValueSelector value_selector = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("UNION-MULTIPLE-VALUES"))
        { union_multiple_values = (Node)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("VALUE-SELECTOR"))
        { value_selector = (ValueSelector)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "follow-slot-chain.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (union_multiple_values == null) { union_multiple_values = Kb._T; }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (value_selector == null) { value_selector = Kb._either; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.follow_slot_chain(frame,
                                slot_chain,
                                union_multiple_values,
                                inference_level,
                                value_selector,
                                kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot_chain = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node union_multiple_values = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    ValueSelector value_selector = toValueSelector(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.follow_slot_chain(frame,
                                slot_chain,
                                union_multiple_values,
                                inference_level,
                                value_selector,
                                kb_local_only_p);
  }
}

class FrameHasSlotPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    InferenceLevel inference_level = null;
    SlotType slot_type = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("INFERENCE-LEVEL"))
        { inference_level = (InferenceLevel)evaluated; }
      else if (pname.equalsIgnoreCase("SLOT-TYPE"))
        { slot_type = (SlotType)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "frame-has-slot-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (inference_level == null) { inference_level = Kb._taxonomic; }
    if (slot_type == null) { slot_type = Kb._own; }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.frame_has_slot_p(frame,
                               slot,
                               inference_level,
                               slot_type,
                               kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Node slot = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    InferenceLevel inference_level = toInferenceLevel(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    SlotType slot_type = toSlotType(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.frame_has_slot_p(frame,
                               slot,
                               inference_level,
                               slot_type,
                               kb_local_only_p);
  }
}

class FrameInKbPEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node thing = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "frame-in-kb-p.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.frame_in_kb_p(thing,
                            kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node thing = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.frame_in_kb_p(thing,
                            kb_local_only_p);
  }
}

class FreeEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Enumerator enumerator = toEnumerator(trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
    }
    enumerator.free();
    return Kb._NIL;
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Enumerator enumerator = toEnumerator(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb.free(enumerator);
    return Kb._NIL;
  }
}

class FrsIndependentFrameHandlEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Node frame = (trivial_eval_expression
                              (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = null;
    TruthValue kb_local_only_p = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB"))
        { kb = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("KB-LOCAL-ONLY-P"))
        { kb_local_only_p = (TruthValue)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "frs-independent-frame-handle.")).signal();
	   }
    }
    if (kb == null) { kb = (Kb)getenv(_current_kb, _env_); }
    if (kb_local_only_p == null) { kb_local_only_p = Kb._NIL; }
    _env_.record_last_kb(kb);
    return kb.frs_independent_frame_handle(frame,
                                           kb_local_only_p);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Node frame = (trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Kb kb = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    TruthValue kb_local_only_p = toTruthValue(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    _env_.record_last_kb(kb);
    return kb.frs_independent_frame_handle(frame,
                                           kb_local_only_p);
  }
}

class FrsNameEv extends AbstractEvaluator {
  public Node eval (Node args, EvalContext _env_, int depth, Kb ckb){
    Kb kb_type = null;
    Connection connection = null;
    Enumerator e = args.enumerate_list();
    while (e.has_more_p()) {
      Symbol key = get_keyword(e.next());
      String pname = key.symbolName;
      Node unevaluated = e.next();
      Node evaluated = trivial_eval_expression
          (unevaluated, _env_, depth + 1, ckb);
      if (pname.equalsIgnoreCase("KB-TYPE"))
        { kb_type = (Kb)evaluated; }
      else if (pname.equalsIgnoreCase("CONNECTION"))
        { connection = (Connection)evaluated; }
      else { throw new GenericError
	       (_T, new OKBCString
		    (key + " is not a legal keyword arg for " +
                     "frs-name.")).signal();
	   }
    }
    if (kb_type == null) { kb_type = Kb.get_kb_type(Kb._NIL); }
    if (connection == null) 
      { connection = LocalConnection.local_connection(); }
    return Kb.frs_name(kb_type,
                       connection);
  }

  public Node positional_eval
              (Node args, EvalContext _env_, int depth, Kb ckb) {
    Kb kb_type = toKb(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    Connection connection = toConnection(trivial_eval_expression
      (args.car(), _env_, depth + 1, ckb));
    args = cdr_or_NIL(args); // pop
    return Kb.frs_name(kb_type,
                       connection);
  }
}
