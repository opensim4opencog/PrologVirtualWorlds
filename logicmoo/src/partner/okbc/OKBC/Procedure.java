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

import java.net.Socket;
import java.net.URLEncoder;
import java.net.UnknownHostException;

public class Procedure extends Node 
{
  static final int typeCode = PROCEDURE;
  static final Hashtable all_procedures = new Hashtable();
  static final Hashtable name_to_procedure_table = new Hashtable();
  static Procedure _EQ = null; // Not yet implemented.
  Node name = Node._NIL;
  Node arguments;
  Node expression;
  Node environment;
  boolean as_sexpression_p = false;
  Node real_arguments = null;
  Node real_expression = null;
  Node real_environment = null;
  Kb interpreted_wrt_kb = null;

  Procedure() {}
  
  Procedure(Node theName,
		    Node theArguments, 
		    Node theExpression,
		    Node theEnvironment) {
    name        = theName;
    arguments   = theArguments;
    expression  = theExpression;
    environment = theEnvironment;
  }
  
  static Cons procedure_key
  (Node theName, Node theArguments, Node theExpression,
   Node theEnvironment) {
    return new Cons(theName,
		    new Cons(theArguments,
			     new Cons(theExpression, theEnvironment)));
  }
  
  static Procedure intern_funspec
  (Node theName, Node theArguments, Node theExpression,
   Node theEnvironment) {
    Cons key = procedure_key(theName, theArguments, theExpression,
				      theEnvironment);
    Procedure match =
      (Procedure)(all_procedures.get(key));
    if (match == null)
      { match = new Procedure(theName, theArguments, theExpression,
				      theEnvironment);
	all_procedures.put(key, match);
      }
    return match;
  }

  public static Procedure intern_procedure
  (Node theName, Node theArguments, Node theExpression,
   Node theEnvironment) {
    return intern_funspec(theName, theArguments, theExpression,
			  theEnvironment);
  }

  public static Procedure intern_procedure
  (Cons theArguments, Cons theExpression) {
    return intern_funspec(Node._NIL, theArguments, theExpression,
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (False theArguments, Cons theExpression) {
    return intern_funspec(Node._NIL, theArguments, theExpression,
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, Cons theArguments, Cons theExpression) {
    return intern_funspec(theName, theArguments, theExpression,
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (String theArguments, String theExpression) {
    return intern_funspec(Node._NIL, new OKBCString(theArguments),
			  new OKBCString(theExpression),
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, String theArguments, String theExpression) {
    return intern_funspec(theName, new OKBCString(theArguments),
			  new OKBCString(theExpression),
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Cons theArguments, String theExpression) {
    return intern_funspec(Node._NIL, theArguments,
			  new OKBCString(theExpression),
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, Cons theArguments, String theExpression) {
    return intern_funspec(theName, theArguments, new OKBCString(theExpression),
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Cons theArguments, Cons theExpression, Node theEnvironment) {
    return intern_funspec(Node._NIL, theArguments, theExpression,
			  theEnvironment);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, Cons theArguments, Cons theExpression,
   Node theEnvironment) {
    return intern_funspec(theName, theArguments, theExpression,
			  theEnvironment);
  }
  
  public static Procedure intern_procedure
  (Cons theArguments, String  theExpression, Node theEnvironment) {
    return intern_funspec(Node._NIL, theArguments,
			  new OKBCString(theExpression),
			  theEnvironment);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, Cons theArguments, String theExpression,
   Node theEnvironment) {
    return intern_funspec(theName, theArguments, new OKBCString(theExpression),
			  theEnvironment);
  }
  
  public static Procedure intern_procedure
  (String theArguments, String theExpression, Node theEnvironment) {
    return intern_funspec(Node._NIL, new OKBCString(theArguments),
			  new OKBCString(theExpression), theEnvironment);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, String theArguments, String theExpression, 
   Node theEnvironment) {
    return intern_funspec(theName, new OKBCString(theArguments),
			  new OKBCString(theExpression), theEnvironment);
  }
  
  public static Procedure intern_procedure
  (Cons theArguments, Cons theExpression, False theEnvironment) {
    return intern_funspec(Node._NIL, theArguments, theExpression,
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, Cons theArguments, Cons theExpression,
   False theEnvironment) {
    return intern_funspec(theName, theArguments, theExpression,
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Cons theArguments, String theExpression, False theEnvironment) {
    return intern_funspec(Node._NIL, theArguments,
			  new OKBCString(theExpression),
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, Cons theArguments, String theExpression, 
   False theEnvironment) {
    return intern_funspec(theName, theArguments,
			  new OKBCString(theExpression),
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (String theArguments, String theExpression, False theEnvironment) {
    return intern_funspec(Node._NIL, new OKBCString(theArguments),
			  new OKBCString(theExpression),
			  _NIL);
  }
  
  public static Procedure intern_procedure
  (Symbol theName, String theArguments, String theExpression,
   False theEnvironment) {
    return intern_funspec(theName, new OKBCString(theArguments),
			  new OKBCString(theExpression),
			  _NIL);
  }
  
  protected void encode(PrintWriter s, boolean client_p) {
    s.print(typeCode);
    newline(s);
    name.encode(s, client_p); // in transport_version >= B
    arguments.encode(s, client_p);
    expression.encode(s, client_p);
    environment.encode(s, client_p);
  }
  

  public static boolean print_procedures_verbosely_p = true;


  public String toString() {
    if (print_procedures_verbosely_p)
      {
	return
	  "[Procedure "
	  + ((name == null || name == Node._NIL) ? "" : name.toString() + " ")
	  + arguments.toString()
	  + " &environment " + environment.toString() + "\n"
	  + expression.toString()
	  // + " " + pointerForToString() 
	  + "]";
      }
    else
      {
	return
	  "[Procedure "
	  + ((name == null || name == Node._NIL) ? "" : name.toString() + " ")
	  + arguments.toString()
	  + "]";
      }
  }

  public boolean fsequals(Procedure thing) {
    return this == thing ||
      (   name.equals(thing.name)
       && arguments.equals(thing.arguments)
       && expression.equals(thing.expression)
       && environment.equals(thing.environment)) ;
  }

  public boolean equals(Object thing) {
    if (thing instanceof Procedure)
      { return this.fsequals((Procedure)thing); }
    else { return false; }
  }

  public Node funcall(Node arg0, Node arg1) {
    return apply(Cons.list(arg0, arg1));
  }

  public Node funcall(Node arg0, Node arg1, Node arg2) {
    return apply(Cons.list(arg0, arg1, arg2));
  }

  public Node apply(Node args) {
    return apply_with_kb_environment((Kb)current_kb(), args, 0);
  }

  public Node apply_with_kb_environment(Kb kb, Node args, int depth) {
    prepare_procedure(kb);
    if (args == null) { args = _NIL; }
    if (args.length() != real_arguments.length())
      { throw new RuntimeException
	("Wrong number (" + args.length() + 
	 ") of actual parameters supplied to procedure " + this +
	 ".  " + real_arguments.length() + " expected.");
      }
    Node bindings = Cons.list(Cons.list(_arguments, args));
    if (kb != null)
      { bindings = new Cons(Cons.list(_kb, kb), bindings); }
    EvalContext environment = new EvalContext(kb, bindings, null);
    Enumeration formals = real_arguments.elements();
    Enumeration actuals = args.elements();
    for (; formals.hasMoreElements() ;) {
      Symbol arg = (Symbol)(formals.nextElement());
      Node value = (Node)(actuals.nextElement());
      environment.push(Cons.list(arg, value));
    }
    // System.out.println("Funspec Real Expression  = " + real_expression);
    // System.out.println("Funspec Environment = " + environment);
    return Evaluator.getOkbcEvaluator().trivial_eval_expression
      (real_expression, environment, depth, kb);
  }

  public Node apply_with_existing_environment
        (Kb kb, Node args, EvalContext existing_environment, int depth) {
    prepare_procedure(kb);
    if (args == null) { args = _NIL; }
    if (args.length() != real_arguments.length())
      { throw new RuntimeException
	("Wrong number (" + args.length() + 
	 ") of actual parameters supplied to procedure " + this +
	 ".  " + real_arguments.length() + " expected.");
      }
    EvalContext environment = existing_environment;
    Enumeration formals = real_arguments.elements();
    Enumeration actuals = args.elements();
    for (; formals.hasMoreElements() ;) {
      Symbol arg = (Symbol)(formals.nextElement());
      Node value = (Node)(actuals.nextElement());
      environment = environment.augment(Cons.list(arg, value));
    }
    // System.out.println("Funspec Real Expression  = " + real_expression);
    // System.out.println("Funspec Environment = " + environment);
    return Evaluator.getOkbcEvaluator().trivial_eval_expression
      (real_expression, environment, depth, kb);
  }

  Node add_progn_if_necessary (Node body) {
    if (body.length() < 2) { return body.car(); }
    else { return new Cons(_progn, body); }
  }

  static Symbol _progn = Symbol.intern("PROGN", "LISP");

  public void prepare_procedure (Kb kb) {
    IoSyntax ioSyntax;
    if (kb == null) { ioSyntax = new IoSyntax(); }
    else { ioSyntax = kb.ioSyntax; }
    if (kb != interpreted_wrt_kb || !as_sexpression_p)
      { if (arguments instanceof OKBCString)
	  { real_arguments = LispReader.readFromString
	    (((OKBCString)arguments).string, ioSyntax); 
	  }
	else if (arguments instanceof Cons || arguments == _NIL)
	  { real_arguments = arguments; }
	else { throw new GenericError
		 (_T, new OKBCString
		  (arguments + " is not a legal procedure " +
		   "arglist")).signal();
	     }

	if (expression instanceof OKBCString)
	  { LispReader r = new LispReader
	    (((OKBCString)expression).string, ioSyntax); 
	    Node body_expressions = _NIL;
	    do 
	      {
		Node form = r.read();
		if (form == __EOF) { break; }
		else { body_expressions =
			 Cons.append_cons(body_expressions, Cons.list(form));
		}
	      }
	    while (true);
	    real_expression = add_progn_if_necessary(body_expressions);
	  }
	else if (expression == _NIL || expression instanceof Cons)
	  { real_expression = expression; }
	else { throw new GenericError
		 (_T, new OKBCString
		  (expression + " is not a legal procedure " +
		   "body")).signal();
	     }

	if (environment instanceof OKBCString)
	  { real_environment = LispReader.readFromString
	    (((OKBCString)environment).string, ioSyntax); 
	  }
	else if (environment == _NIL)
	  { real_environment = environment; }
	else if (environment instanceof Cons)
	  { 
	    Enumeration e = environment.elements();
	    for (; e.hasMoreElements() ;) {
	      Node entry = (Node)(e.nextElement());
	      if ((entry instanceof Cons) && 
		  (entry.length() == 2) &&
		  (entry.car() instanceof AbstractSymbol))
		{}
	      else 
		{ throw new GenericError
		    (_T, new OKBCString
		     (entry + " is not a legal procedure " +
		      "environment element")).signal();
		}
	    }
	    real_environment = environment;
	  }
	else { throw new GenericError
		 (_T, new OKBCString
		  (environment + 
		   " is not a legal procedure environment")).signal();
	     }
	as_sexpression_p = true;
      }
  }




  static void perform_test(int testno, String string, Node result, Kb kb) {
    perform_test(testno, string, result, _NIL, kb);
  }

  static void perform_test(int testno, String string, Node expected_result,
			   Node blist, Kb kb) {
    Package pack = Package.find_or_create_package("TEST");
    LispReader r = new LispReader (string, new IoSyntax(pack));
    Node sexp = r.read();
    Node result = Evaluator.getOkbcEvaluator().trivial_eval_expression
      (sexp, new EvalContext(null, blist, null), 0, kb);
    if (result.equals(expected_result))
      { System.out.println("Test " + testno + " passed."); 
      }
    else { System.out.println("Input expression = " + sexp);
	   if (blist != _NIL) { System.out.println("Environment = " + blist); }
	   System.out.println("Result = " + result);
	   System.out.println("Test " + testno + 
			      " FAILED.  Expected result was " +
			      expected_result + ".") ;
	 }
  }

  public static void main (String arg[]) {
    Package pack = Package.find_or_create_package("TEST");
    Node env;
    Symbol _a = Symbol.intern("A", pack);
    Symbol _b = Symbol.intern("B", pack);
    Symbol _c = Symbol.intern("C", pack);
    Symbol _d = Symbol.intern("D", pack);
    Symbol _plus = Symbol.intern("+", pack);
    Kb kb = new TupleKb(false);
    env = Cons.list(Cons.list(_a, new Real(101.1)));
    perform_test(1, "(+ (+ 42 2001) (+ a 1))", new Real(2145.1), env, kb);

    env = Cons.list(Cons.list(_a, new Real(101.1)));
    perform_test(2, "(< (+ 42 2001) (+ a 1))", _NIL, env, kb);

    perform_test(3, "(= (+ 42 2001) 2043)", _T, kb);
    perform_test(4, "(eq 42 42)", _NIL, kb);
    perform_test(5, "(eql 42 42)", _T, kb);
    perform_test(6, "(equal \"A\" \"A\")", _T, kb);
    perform_test(7, "(equal \"a\" \"A\")", _NIL, kb);
    perform_test(8, "(equalp \"A\" \"A\")", _T, kb);
    perform_test(9, "(equalp \"a\" \"A\")", _T, kb);
    perform_test(10, "(eq 'a 'a)", _T, kb);
    perform_test(11, "(eq 'a 'b)", _NIL, kb);

    perform_test(12, "(and 'a 'b)", _b, kb);
    perform_test(13, "(and 'a nil)", _NIL, kb);
    perform_test(14, "(or 'a 'b)", _a, kb);
    perform_test(15, "(or nil 'a)", _a, kb);

    perform_test(16, "(first nil)", _NIL, kb);
    perform_test(17, "(first '(a b)) ", _a, kb);
    perform_test(18, "(rest nil)", _NIL, kb);
    perform_test(19, "(rest '(a b))", Cons.list(_b), kb);

    perform_test(20, "(nth 12 nil)", _NIL, kb);
    perform_test(21, "(nth 12 '(a b)) ", _NIL, kb);
    perform_test(22, "(nth 1 '(a b))", _b, kb);

    perform_test(23, "(nthcdr 12 nil)", _NIL, kb);
    perform_test(24, "(nthcdr 12 '(a b)) ", _NIL, kb);
    perform_test(25, "(nthcdr 0 '(a b))", Cons.list(_a, _b), kb);
    perform_test(25, "(nthcdr 1 '(a b))", Cons.list(_b), kb);

    env = Cons.list(Cons.list(_a, new Real(101.1)));
    perform_test(26, "(let ((b (+ a 42)) (c 2001) (a 100)) (+ a (+ b c)))",
		 new Real(2244.1), env, kb);

    env = Cons.list(Cons.list(_a, new Real(101.1)));
    perform_test(27, "(let* ((b (+ a 42))(c 2001)(a (+ b 100)))(+ a (+ b c)))",
		 new Real(2387.2), env, kb);

    env = Cons.list(Cons.list(_a, new OKBCInteger(42)));
    perform_test(28, "(do-list (x NIL) (+ a x))", _NIL, env, kb);

    env = Cons.list(Cons.list(_a, new OKBCInteger(42)));
    perform_test(29, "(do-list (x '(1 2 3)) (+ a x))",
		 Cons.list(new OKBCInteger(43), new OKBCInteger(44),
			   new OKBCInteger(45)), 
		 env, kb);

    perform_test(30, "(append nil nil)", _NIL, kb);
    perform_test(31, "(append nil '(a) nil)", Cons.list(_a), kb);
    perform_test(32, "(append nil '(a b) '(a) nil)", Cons.list(_a, _b, _a), kb);

    perform_test(33, "(firstn 12 nil)", _NIL, kb);
    perform_test(34, "(firstn 12 '(a b)) ", Cons.list(_a, _b), kb);
    perform_test(35, "(firstn 0 '(a b))", _NIL, kb);
    perform_test(35, "(firstn 1 '(a b))", Cons.list(_a), kb);

    perform_test(36, "(if (> 3 4) (+ 2 3))", _NIL, kb);
    perform_test(37, "(if (< 3 4) (+ 2 3))", new OKBCInteger(5), kb);
    perform_test(38, "(if (> 3 4) (+ 2 3) (+ 4 5))", new OKBCInteger(9), kb);
    perform_test(39, "(if (< 3 4) (+ 2 3) (+ 4 5))", new OKBCInteger(5), kb);

    perform_test(40, "(not nil)", _T, kb);
    perform_test(41, "(not 42)", _NIL, kb);

    env = Cons.list(Cons.list(_a, new OKBCInteger(42)));
    perform_test(42, "(progn (+ a 42) (- a 42))", new OKBCInteger(0), env, kb);
    perform_test(43, "(progn)", _NIL, env, kb);

    perform_test(44, "(list)", _NIL, kb);
    perform_test(45, "(list 'a)", Cons.list(_a), kb);
    perform_test(46, "(list 'a 'b)", Cons.list(_a, _b), kb);

    perform_test(47, "(list* nil)", _NIL, kb);
    perform_test(48, "(list* 'a nil)", Cons.list(_a), kb);
    perform_test(49, "(list* 'a 'b '(a b))", Cons.list(_a, _b, _a, _b), kb);

    env = Cons.list(Cons.list(_a, _NIL));
    perform_test(50, "(push 'a a)", Cons.list(_a), env, kb);
    env = Cons.list(Cons.list(_a, Cons.list(_b)));
    perform_test(51, "(push 'a a)", Cons.list(_a, _b), env, kb);

    perform_test(52, "(member 'a nil)", _NIL, kb);
    perform_test(53, "(member 'a '(c a b))", Cons.list(_a, _b), kb);

    perform_test(54, "(remove 'a nil)", _NIL, kb);
    perform_test(55, "(remove 'a '(c a b))", Cons.list(_c, _b), kb);
    perform_test(56, "(remove 'a '(c b a))", Cons.list(_c, _b), kb);
    perform_test(57, "(remove 'a '(b c d))", Cons.list(_b, _c, _d), kb);

    perform_test(58, "(remove-duplicates nil)", _NIL, kb);
    perform_test(59, "(remove-duplicates '(b c d))", Cons.list(_b, _c, _d), kb);
    perform_test(60, "(remove-duplicates '(b b))", Cons.list(_b), kb);
    perform_test(61, "(remove-duplicates '(b c b d))", Cons.list(_b, _c, _d),
		 kb);

    perform_test(62, "(procedure () (+ a b))",
		 intern_procedure
		 (_NIL, _NIL, Cons.list(_plus, _a, _b), _NIL), kb);

    perform_test(63, "(procedure \"()\" (+ a b))",
		 intern_procedure
		 (_NIL, new OKBCString("()"), Cons.list(_plus, _a, _b), _NIL),
		 kb);

    perform_test(64, "(procedure d (a b) (+ a b))",
		 intern_procedure
		 (_d, Cons.list(_a, _b), Cons.list(_plus, _a, _b), _NIL), kb);
  }



}

