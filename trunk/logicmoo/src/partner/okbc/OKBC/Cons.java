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

public class Cons extends Node {
  Node car = null;
  Node cdr = null;

  public Enumerator enumerate_list() {
    return new ExhaustiveEnumerator(this);
  }

  public Node car() { return this.car; }
  public Node cdr() { return this.cdr; }
  public Node cdr_or_NIL() { 
    if (cdr == null)
      { return _NIL; }
    else { return cdr; }
  }

  public static Node cdr_or_NIL(Node expression) { 
    Node cdr = expression.cdr();
    if (cdr == null) { return _NIL; }
    else { return cdr; }
  }

  public void set_car(Object new_car) {
    car = Node.toNode(new_car);
  }

  public void set_cdr(Object new_cdr) {
    // By convention we terminate lists of Conses with null, not _NIL.
    if (new_cdr == _NIL) { cdr = null; }
    else { cdr = Node.toNode(new_cdr); }
  }

  public Enumeration elements() {
    return new ConsEnumeration(this); 
  }

  public static Node list() { return _NIL; }
  public static Node list(Node n0) {
    return new Cons(n0);
  }
  public static Node list(Node n0, Node n1) {
    return new Cons(n0, new Cons(n1));
  }
  public static Node list(Node n0, Node n1, Node n2) {
    return new Cons(n0, new Cons(n1, new Cons(n2)));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3) {
    return new Cons(n0, new Cons(n1, new Cons(n2, new Cons(n3))));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3, Node n4) {
    return new Cons(n0, new Cons(n1, new Cons(n2, new Cons(n3,new Cons(n4)))));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3, Node n4,
			  Node n5) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5))))));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3, Node n4, Node n5,
			  Node n6) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6)))))));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3, Node n4, Node n5,
			  Node n6, Node n7) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7))))))));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3, Node n4, Node n5,
			  Node n6, Node n7, Node n8) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8)))))))));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3, Node n4, Node n5,
			  Node n6, Node n7, Node n8, Node n9) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8,
	   new Cons(n9))))))))));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3, Node n4, Node n5,
			  Node n6, Node n7, Node n8, Node n9, Node n10) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8,
	   new Cons(n9,
	   new Cons(n10)))))))))));
  }
  public static Node list(Node n0, Node n1, Node n2, Node n3, Node n4, Node n5,
			  Node n6, Node n7, Node n8, Node n9, Node n10,
			  Node n11) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8,
	   new Cons(n9,
	   new Cons(n10,
           new Cons(n11))))))))))));
  }
  public static Node list(Node  n0, Node  n1, Node  n2, Node  n3, Node  n4,
			  Node  n5, Node  n6, Node  n7, Node  n8, Node  n9,
			  Node n10, Node n11, Node n12) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8,
	   new Cons(n9,
	   new Cons(n10,
           new Cons(n11, 
	   new Cons(n12)))))))))))));
  }
  public static Node list(Node  n0, Node  n1, Node  n2, Node  n3, Node  n4,
			  Node  n5, Node  n6, Node  n7, Node  n8, Node  n9,
			  Node n10, Node n11, Node n12, Node n13) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8,
	   new Cons(n9,
	   new Cons(n10,
           new Cons(n11, 
           new Cons(n12,
	   new Cons(n13))))))))))))));
  }
  public static Node list(Node  n0, Node  n1, Node  n2, Node  n3, Node  n4,
			  Node  n5, Node  n6, Node  n7, Node  n8, Node  n9,
			  Node n10, Node n11, Node n12, Node n13, Node n14) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8,
	   new Cons(n9,
	   new Cons(n10,
           new Cons(n11, 
           new Cons(n12,
           new Cons(n13,
	   new Cons(n14)))))))))))))));
  }

  public static Node list(Node  n0, Node  n1, Node  n2, Node  n3, Node  n4,
			  Node  n5, Node  n6, Node  n7, Node  n8, Node  n9,
			  Node n10, Node n11, Node n12, Node n13, Node n14,
			  Node n15) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8,
	   new Cons(n9,
	   new Cons(n10,
           new Cons(n11, 
           new Cons(n12,
           new Cons(n13,
           new Cons(n14,
	   new Cons(n15))))))))))))))));
  }

  public static Node list(Node  n0, Node  n1, Node  n2, Node  n3, Node  n4,
			  Node  n5, Node  n6, Node  n7, Node  n8, Node  n9,
			  Node n10, Node n11, Node n12, Node n13, Node n14,
			  Node n15, Node n16) {
    return new Cons(n0,
           new Cons(n1,
           new Cons(n2,
           new Cons(n3,
           new Cons(n4,
           new Cons(n5,
           new Cons(n6,
           new Cons(n7,
           new Cons(n8,
	   new Cons(n9,
	   new Cons(n10,
           new Cons(n11, 
           new Cons(n12,
           new Cons(n13,
           new Cons(n14,
           new Cons(n15,
	   new Cons(n16)))))))))))))))));
  }

  public static Node toNode_or_tail(Object o) {
    if (o == _NIL) { return null; }
    else { return toNode(o); }
  }

  public Cons(Object theCar, Object theCdr) {
    car = Node.toNode(theCar);
    cdr = toNode_or_tail(theCdr);
  }

  public Cons(byte theCar, Object theCdr) {
    car = Node.toNode(new Integer(theCar));
    cdr = toNode_or_tail(theCdr);
  }

  public Cons(short theCar, Object theCdr) {
    car = Node.toNode(new Float(theCar));
    cdr = toNode_or_tail(theCdr);
  }

  public Cons(long theCar, Object theCdr) {
    car = Node.toNode(new Long(theCar));
    cdr = toNode_or_tail(theCdr);
  }

  public Cons(float theCar, Object theCdr) {
    car = Node.toNode(new Float(theCar));
    cdr = toNode_or_tail(theCdr);
  }

  public Cons(double theCar, Object theCdr) {
    car = Node.toNode(new Double(theCar));
    cdr = toNode_or_tail(theCdr);
  }

  public Cons(boolean theCar, Object theCdr) {
    if (theCar) { car = _T; }
    else { car = _NIL; }
    cdr = toNode_or_tail(theCdr);
  }

// -----------------

  public Cons(Object theCar) {
    car = Node.toNode(theCar);
    cdr = null;
  }

  public Cons(byte theCar) {
    car = Node.toNode(new Integer(theCar));
    cdr = null;
  }

  public Cons(short theCar) {
    car = Node.toNode(new Float(theCar));
    cdr = null;
  }

  public Cons(long theCar) {
    car = Node.toNode(new Long(theCar));
    cdr = null;
  }

  public Cons(float theCar) {
    car = Node.toNode(new Float(theCar));
    cdr = null;
  }

  public Cons(double theCar) {
    car = Node.toNode(new Double(theCar));
    cdr = null;
  }

  public Cons(boolean theCar) {
    car = Node.toNode(new Boolean(theCar));
    cdr = null;
  }

  public int length () {
    Node current = this;
    int length = 0;
    while (current instanceof Cons) {
      length = length + 1;
      current = current.cdr();
    }
    return length;
  }

  public static boolean listp (Object x) {
    return ((x == null) || (x == _NIL) || (x instanceof Cons));
  }

  public int length (int soFar) {
    if (cdr == null) { return 1 + soFar; }
    else { return ((Cons)cdr).length(1 + soFar);}
  }

  public static Node assoc (Object key, Node list) {
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Cons elt = (Cons)(e.nextElement());
      if (key.equals(elt.car())) { return elt; }
    }
    return _NIL;
  }

  public static Node getf (Node list, Object key) {
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Node elt = (Node)(e.nextElement());
      if (key.equals(elt)) { return (Node)e.nextElement(); }
    }
    return _NIL;
  }

  public Cons assoc (Object key) {
    Enumeration e = this.elements();
    for (; e.hasMoreElements() ;) {
      Cons elt = (Cons)(e.nextElement());
      if (key.equals(elt.car())) { return elt; }
    }
    return null;
  }

  public static Node set_difference(Node list1, Node list2, Kb kb,
				    TruthValue kb_local_only_p) {
    if (list2 == null || list2 == _NIL) { return list1; }
    Cons c = (Cons) list2;
    return set_difference(remove_eql(c.car(), list1, kb), c.cdr(), kb,
			  kb_local_only_p);
  }

  public static Node firstn(int n, Node list) {
    if (list == null || list == _NIL) { return _NIL; }
    if (n <= 0) { return _NIL; }
    Cons c = (Cons) list;
    return new Cons(c.car(), firstn(n - 1, c.cdr()));
  }

  public static Node nthcdr(int n, Node list) {
    if (n <= 0) { return list; }
    else if (list == _NIL || list == null) { return _NIL; }
    else { Cons c = (Cons) list;
	   return nthcdr(n - 1, c.cdr());
	 }
  }

  public static Node reverse (Node list) {
    return reverse(list, _NIL);
  }

  static Node reverse (Node list, Node accumulated_result) {
    if (list == null || list == _NIL)
      { return accumulated_result; }
    else { return reverse(list.cdr(), 
			  new Cons(list.car(), accumulated_result));
	 }
  }

  public static Node list_if_not(Node thing) {
    if (thing == null || thing == _NIL) { return _NIL; }
    else if (thing instanceof Cons) { return thing; }
    else { return new Cons(thing); }
  }

  public static Node first_if_list(Node thing) {
    if (thing == null || thing == _NIL) { return _NIL; }
    else if (thing instanceof Cons) { return thing.car(); }
    else { return thing; }
  }

  public static Node rest_if_list(Node thing) {
    if (thing == null || thing == _NIL) { return _NIL; }
    else if (thing instanceof Cons) { return thing.cdr(); }
    else { return thing; }
  }

  public static Node substitute_test
  (Object new_val, Object old_val, Node tree, Node test, Kb kb) {
    if (tree == null || tree == _NIL) { return _NIL; }
    else
      {
	Node n = Node.toNode(new_val);
	Node o = Node.toNode(old_val);
	if (tree instanceof Cons) 
	  { return new Cons(substitute_test(n, o, ((Cons)tree).car, test, kb),
			    substitute_test(n, o, ((Cons)tree).cdr, test, kb));
	  }
	if (Evaluator.getOkbcEvaluator().trivial_eval_expression
	    (enquote_args(Cons.list(test, tree, o)), new EvalContext(), 0, kb)
	    != _NIL)
	  { return n; }
	else { return tree; }
      }
  }

  public static Node substitute(Object new_val, Object old_val, Node tree,
				Kb kb) {
    if (tree == null || tree == _NIL) { return _NIL; }
    else
      {
	Node n = Node.toNode(new_val);
	Node o = Node.toNode(old_val);
	if (tree instanceof Cons)
	  { return new Cons(substitute(n, o, ((Cons)tree).car, kb),
			    substitute(n, o, ((Cons)tree).cdr, kb));
	  }
	else
	  {
	    if (kb == null)
	      { if (tree == o) { return n; }
		else { return tree; }
	      }
	    else
	      { if (tree == o ||
		      // Do it the fast way so we don't have to cache.
		    kb.eql_in_kb(tree, o, _NIL) != _NIL) { return n; }
		else { return tree; }
	      }
	  }
      }
  }

  public static Node sublis(Object substitutions, Node tree, Kb kb) {
    Node res = null;
    if (substitutions == null || substitutions == _NIL) { res = tree; }
    else { Cons s = (Cons)substitutions;
	   Cons substitution = (Cons)(s.car);
	   Node partial = substitute(substitution.cdr, substitution.car, tree,
				     kb);
	   res = sublis(s.cdr(), partial, kb);
	 }
// System.out.println("Sublis(" + tree + ", " + substitutions + ") -> " + res);
    return res;
  }

  public static Node mapcar_first(Node list) {
    if (list == null || list == _NIL) { return _NIL; }
    Enumeration e = list.elements();
    Cons root = null;
    Cons tail = null;
    for (; e.hasMoreElements() ;) {
      Node elt = ((Node)e.nextElement()).car();
      if (tail == null)
	{ tail = new Cons(elt);
	  root = tail;
	}
      else { Cons new_tail = new Cons(elt);
	     tail.set_cdr(new_tail);
	     tail = new_tail;
	   }
    }
    return root;
  }
  
  public static Node mapcar_cons(Node list1, Node list2) {
    if (list1 == null || list1 == _NIL) { return _NIL; }
    if (list2 == null || list2 == _NIL) { return _NIL; }
    Enumerator e = list1.enumerate_list();
    Enumerator f = list2.enumerate_list();
    Cons root = null;
    Cons tail = null;
    while (e.has_more_p()) {
      Node elt1 = e.next();
      Node elt2 = f.next();
      Cons new_entry = new Cons(elt1, elt2);
      if (tail == null)
	{ tail = new Cons(new_entry);
	  root = tail;
	}
      else { Cons new_tail = new Cons(new_entry);
	     tail.set_cdr(new_tail);
	     tail = new_tail;
	   }
    }
    return root;
  }
  
  public static Node remove_eql(Object obj, Node n, Kb kb) {
    Node o = Node.toNode(obj);
    if (n == null || n == _NIL) { return _NIL; }
    Cons c = (Cons) n;
    boolean match = false;
    if (kb == null) { match = o == c.car(); }
    else { match = kb.eql_in_kb(o, c.car(), _NIL) != _NIL; }
    if (match)
      { if (c.cdr() == null || c.cdr() == _NIL) { return _NIL; }
	else { return remove_eql(o, c.cdr(), kb); }
      }
    else { return new Cons(c.car(), remove_eql(o, c.cdr(), kb)); }
  }
  
  public static Node remove_equal(Object obj, Node list, Kb kb) {
    if (list == _NIL || list == null) { return _NIL; }
    Node o = Node.toNode(obj);
    Cons c = (Cons)list;
    boolean match = false;
    if (kb == null) { match = o.equals(c.car); }
    else { match = kb.equal_in_kb(o, c.car(), _NIL) != _NIL; }
    if (match)
      { if (c.cdr == null) { return _NIL; }
	else { return remove_equal(o, c.cdr, kb); }
      }
    else { return new Cons(c.car, remove_equal(o, c.cdr, kb)); }
  }

  /**************
  public static Node remove_duplicates(Node list) {
    if (list == null || list == _NIL) { return _NIL; }
    Cons ret = null;
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Node element = (Node)(e.nextElement());
      if (ret == null) { ret = new Cons(element); }
      else if (!member_eql(element, ret, kb, kb_local_only_p)) 
	{ ret = new Cons(element, ret); }
    }
    // System.out.println("remove_duplicates(" + list + ") -> " + ret);
    return ret;
  }

  public static Node remove_duplicates(Node list, Node test) {
    if (list == null || list == _NIL) { return _NIL; }
    Cons ret = null;
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Node element = (Node)(e.nextElement());
      if (ret == null) { ret = new Cons(element); }
      else if (!member_test(element, ret, test, kb, kb_local_onlyl_p))
	{ ret = new Cons(element, ret); }
    }
    // System.out.println("remove_duplicates(" + list + ", " + test + ") -> " + ret);
    return ret;
  }

  *************/
  
  public static Node remove_duplicates(Node list, Node test, Kb kb, 
				       TruthValue kb_local_only_p) {
    if (list == null || list == _NIL) { return _NIL; }
    Cons ret = null;
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Node element = (Node)(e.nextElement());
      if (ret == null) { ret = new Cons(element); }
      else if (!member_test(element, ret, test, kb, kb_local_only_p)) 
	{ ret = new Cons(element, ret); }
    }
    // System.out.println("remove_duplicates(" + list + ", " + test + ") -> " + ret);
    return ret;
  }

  public static Node remove_test(Object obj, Node list, Node test, Kb kb,
				 TruthValue kb_local_only_p) {
    if (list == _NIL || list == null) { return _NIL;}
    if (test == null || test == _NIL) { return remove_equal(obj, list, kb); }
    Node o = Node.toNode(obj);
    Cons c = (Cons) list;
    Node elt = c.car();
    Node tail = c.cdr();
    EvalContext env = new EvalContext();
    Node res = null;
    if (test == _eql_in_kb)
      { res = kb.eql_in_kb(elt, o, kb_local_only_p); }
    else if (test == _equal_in_kb)
      { 
	// System.out.println("Equal in kb?: " + elt + ", " + o);
	res = kb.equal_in_kb(elt, o, kb_local_only_p); }
    else { res = Evaluator.getOkbcEvaluator().trivial_eval_expression
	   (enquote_args(Cons.list(test, elt, o)), env, 0, kb);
	 }
    if (res
	!= _NIL)
      { if (tail == null) { return null; }
	else { return remove_test(o, tail, test, kb, kb_local_only_p); }
      }
    else { return new Cons(elt, remove_test(o, tail, test, kb, 
					    kb_local_only_p)); }
  }
  
  public static Node remove_test(Object obj, Node list, Node test, long index,
				 int max, Kb kb,
				 TruthValue kb_local_only_p) {
    return remove_test(obj, list, test, index, max, kb, kb_local_only_p, 0);
  }
  
  static Node remove_test(Object obj, Node list, Node test, long index,
				 int max, Kb kb, TruthValue kb_local_only_p,
				 int so_far) {
    if (list == _NIL || list == null) { return _NIL;}
    Node o = Node.toNode(obj);
    Cons c = (Cons) list;
    Node elt = c.car();
    Node tail = c.cdr();
    EvalContext env = new EvalContext();
    if (index <= 0 && so_far < max && 
	((Evaluator.getOkbcEvaluator().trivial_eval_expression
	  (enquote_args(Cons.list(test, elt, o)), env, 0, kb)
	  != _NIL)))
      { if (tail == null) { return null; }
	else { return remove_test(o, tail, test, index, max, kb,
				  kb_local_only_p, so_far + 1);
	     }
      }
    else { return new Cons(elt, remove_test(o, tail, test, index - 1, max, 
					    kb, kb_local_only_p, so_far));
	 }
  }
  
  public static boolean member_test(Object obj, Node list, Node test,
				    Kb kb, TruthValue kb_local_only_p) {
    if (list == _NIL || list == null) { return false; }
    if (test == null || test == _NIL)
      { return Cons.member_equal(obj, list, kb, kb_local_only_p); }
    else { Node o = Node.toNode(obj);
	   Enumeration e = list.elements();
	   EvalContext env = new EvalContext();
	   for (; e.hasMoreElements() ;) {
	     Node elt = (Node)(e.nextElement());
	     Node res = null;
	     if (test == _eql_in_kb)
	       { res = kb.eql_in_kb(elt, o, kb_local_only_p); }
	     else if (test == _equal_in_kb)
	       { res = kb.equal_in_kb(elt, o, kb_local_only_p); }
	     else { res = Evaluator.getOkbcEvaluator().trivial_eval_expression
		    (enquote_args(Cons.list(test, elt, o)), env, 0, kb);
		  }
	     if (res != _NIL) { return true; }
	   }
	   return false;
	 }
  }
  
  public boolean isEmpty() {
    return false;
  }

  public static Node find_equal(Object obj, Node list, Kb kb,
				TruthValue kb_local_only_p) {
    if (list == _NIL || list == null) { return _NIL; }
    Node o = Node.toNode(obj);
    Enumerator e = list.enumerate_list();
    for (; e.has_more_p() ;) {
      Node elt = e.next();
      if (kb == null)
	{ if (elt.equals(o)) { return elt; } }
      else
	{ Node this_node = Node.toNode(elt);
	  if (kb.equal_in_kb(this_node, o, kb_local_only_p) != _NIL)
	    { return this_node; }
	}
    }
    return _NIL;
  }
  
  public static boolean member_equal(Object obj, Node list, Kb kb,
				     TruthValue kb_local_only_p) {
    if (list == _NIL || list == null) { return false; }
    Node o = Node.toNode(obj);
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Object elt = e.nextElement();
      if (kb == null)
	{ if (elt.equals(o)) { return true; } }
      else
	{ if (kb.equal_in_kb(Node.toNode(elt), o, kb_local_only_p) != _NIL)
	    { return true; }
	}
    }
    return false;
  }
  
  public static boolean member_eql(Object obj, Node list, Kb kb,
				   TruthValue kb_local_only_p) {
    if (list == _NIL || list == null) { return false; }
    Node o = Node.toNode(obj);
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Object elt = e.nextElement();
      if (kb == null)
	{ if (elt == o) { return true; } }
      else
	{ if (kb.eql_in_kb(Node.toNode(elt), o, kb_local_only_p) != _NIL)
	    { return true; }
	}
    }
    return false;
  }

  /***** Obsolete
  public boolean member_equal(Object obj) {
    Node o = Node.toNode(obj);
    Enumeration e = this.elements();
    for (; e.hasMoreElements() ;) {
      Object elt = e.nextElement();
      if (elt.equals(o)) { return true; }
    }
    return false;
  }
  ******/
  
  public Cons union_equal(Object obj, Kb kb, TruthValue kb_local_only_p) {
    if (obj == null) { return this; }
    else { Node o = Node.toNode(obj);
           Cons c = (Cons) o;
	   if (Cons.member_equal(c.car, this, kb, kb_local_only_p))
	     { return union_equal(c.cdr, kb, kb_local_only_p); }
	   else { Cons augmented = new Cons(c.car, this);
		  return augmented.union_equal(c.cdr, kb, kb_local_only_p);
		}
	 }
  }
  
  public static Node intersection_equal(Node lista, Node listb, Kb kb) {
    if (lista == _NIL || lista == null) { return _NIL; }
    if (listb == _NIL || listb == null) { return _NIL; }
    Cons ca = (Cons)lista;
    if (member_equal(ca.car(), listb, kb , _NIL))
      { return new Cons(ca.car(),
			intersection_equal(ca.cdr(), listb, kb));
      }
    else { return intersection_equal(ca.cdr(), listb, kb); }
  }
  
  public boolean equals(Object thing) {
    if (thing instanceof Cons)
      { Cons c = (Cons) thing;
	if (car == null)
	  { if (c.car() == null)
	      { if (cdr == null)
		  { return c.cdr == null; }
		else { return cdr.equals(c.cdr()); }
	      }
	    else { return false; }
	  }
	if (car.equals(c.car()))
	  { if (cdr == null)
	      { return c.cdr == null; }
	    else { return cdr.equals(c.cdr()); }
	  }
	else { return false; }
      }
    else { return false; }
  }

  public static Node append_cons (Node a, Node b) {
    if (a == null) { return b; }
    if (b == null) { return a; }
    if (a == _NIL) { return b; }
    if (b == _NIL) { return a; }
    else { Cons ca = (Cons) a;
	   return new Cons(ca.car(), append_cons((Cons)(ca.cdr()), b)); }
  }

  public static Cons append_all (Cons list_of_lists) {
    if (list_of_lists == null) { return null; }
    else { return (Cons)(append_cons((Cons)(list_of_lists.car()),
				     append_all((Cons)(list_of_lists.cdr()))));
	 }
  }

  public String toString() {
    if (cdr == null || cdr == _NIL)
      { if (car == null) { return "(null)"; }
	else { return "(" + car.toString() + ")"; }
      }
    else { StringBuffer result = new StringBuffer();
	   int count = 0;
	   result.append("(");
	   Object e = this;
	   do 
	     {
	       if (count > 0) result.append(" ");
	       count = count + 1;
	       Object the_car = ((Cons)e).car;
	       String arg = null;
	       if (the_car == null) { arg = "null"; }
	       else { arg = the_car.toString(); }
	       result.append(arg);
	       e = ((Cons)e).cdr;
	     }
	   while (e instanceof Cons);
	   if (e != null)
	     { result.append(" . ");
	       result.append(e.toString());
	     }
	   result.append(")");
	   return result.toString();
	 }
  }

  public static Node pretty_print(Node form) {
    return pretty_print(form, 0);
  }

  public static Node pretty_print(Node form, int indent) {
    return pretty_print(form, indent, System.out);
  }
  
  public static Node pretty_print(Node form, PrintStream stream) {
    return pretty_print(form, 0, stream);
  }
  
  public static Node pretty_print(Node form, PrintWriter stream) {
    return pretty_print(form, 0, stream);
  }
  
  public static Node pretty_print(Node form, Node stream) {
    return pretty_print(form, 0, stream);
  }
  
  public static Node pretty_print(Node form, int indent, Node stream) {
    if (stream == _NIL)
      {
	ByteArrayOutputStream str = new ByteArrayOutputStream();
	PrintWriter stringStream = new PrintWriter(str);
	pretty_print(form, indent, stringStream);
	stringStream.flush();
	return new OKBCString(str.toString());
      }
    else { return pretty_print(form, indent, System.out); }
  }
  
  public static Node pretty_print(Node form, int indent, PrintStream stream) {
    if (form instanceof Cons &&
	form.car() == __quote && 
	form.cdr() instanceof Cons)
      { stream.print("\'");
        pretty_print(form.cadr(), indent + 1, stream);
      }
    else if (form instanceof Cons)
      { stream.print("(");
	if (((Cons)form).cdr == null || ((Cons)form).cdr == _NIL)
	  { pretty_print(((Cons)form).car, indent + 1, stream);
	    stream.print(")");
	  }
	else 
	  {
            pretty_print(((Cons)form).car, indent + 1, stream);
	    Node e = ((Cons)form).cdr;
	    while (e instanceof Cons)
	      {
		stream.println();
		for(int i=0; i < indent + 2; i++) {
		  stream.print(" ");
		}
		pretty_print(((Cons)e).car, indent + 2);
		e = ((Cons)e).cdr;
	      }
	    if (e != null)
	      { stream.print(" . ");
		stream.println();
		for(int i=0; i < indent + 4; i++) {
		  stream.print(" ");
		}
		pretty_print(e, indent);
	      }
	    stream.print(")");
	  }
      }
    else if (form instanceof OKBCString)
      {
	stream.print("\"");
	String s = ((OKBCString)form).string;
	for(int i=0; i < s.length(); i++) {
	  char ch = s.charAt(i);
	  if (ch == '\\' || ch == '"')
	    { stream.print("\\"); }
	  stream.print(ch);
	}
	stream.print("\"");
      }
    else { stream.print(form); }
    return _NIL;
  }

  public static Node pretty_print(Node form, int indent, PrintWriter stream) {
    if (form instanceof Cons &&
	form.car() == __quote && 
	form.cdr() instanceof Cons)
      { stream.print("\'");
        pretty_print(form.cadr(), indent + 1, stream);
      }
    else if (form instanceof Cons)
      { stream.print("(");
	if (((Cons)form).cdr == null || ((Cons)form).cdr == _NIL)
	  { pretty_print(((Cons)form).car, indent + 1, stream);
	    stream.print(")");
	  }
	else 
	  {
            pretty_print(((Cons)form).car, indent + 1, stream);
	    Node e = ((Cons)form).cdr;
	    while (e instanceof Cons)
	      {
		stream.println();
		for(int i=0; i < indent + 2; i++) {
		  stream.print(" ");
		}
		pretty_print(((Cons)e).car, indent + 2);
		e = ((Cons)e).cdr;
	      }
	    if (e != null)
	      { stream.print(" . ");
		stream.println();
		for(int i=0; i < indent + 4; i++) {
		  stream.print(" ");
		}
		pretty_print(e, indent);
	      }
	    stream.print(")");
	  }
      }
    else if (form instanceof OKBCString)
      {
	stream.print("\"");
	String s = ((OKBCString)form).string;
	for(int i=0; i < s.length(); i++) {
	  char ch = s.charAt(i);
	  if (ch == '\\' || ch == '"')
	    { stream.print("\\"); }
	  stream.print(ch);
	}
	stream.print("\"");
      }
    else { stream.print(form); }
    return _NIL;
  }

  public static Node toList(Vector v) {
    if (v == null || v.size() == 0) { return _NIL;}
    else { return vectorToCons(v); }
  }

  public static Cons vectorToCons(Vector v) {
    if (v == null || v.size() == 0) { return null;}
    else { Enumeration e = v.elements();
	   Cons root = null;
	   Cons tail = null;
	   for (; e.hasMoreElements() ;) {
	     Object elt = e.nextElement();
	     if (tail == null)
	       { tail = new Cons(elt);
		 root = tail;
	       }
	     else { Cons new_tail = new Cons(elt);
		    tail.set_cdr(new_tail);
		    tail = new_tail;
		  }
	   }
	   return root;
	 }
  }

  protected void encode(PrintWriter s, boolean client_p) {
    // System.out.println("Cons encode = " + this);

    s.print(typeCode);
    newline(s);
    s.print(length());
    newline(s);
    Enumeration e = elements();
    for (; e.hasMoreElements() ;) {
      Node element = (Node)(e.nextElement());
      if (element == null)
	{
	  System.out.println("Warning: null found in this list.  " +
			     "NIL inserted instead!!!!" + this);
	  _NIL.encode(s, client_p);
	}
      else { element.encode(s, client_p); }
    }
  }

  public Node nth(int n) {
    if (n < 0) { return _NIL; }
    Cons res = this;
    for(int i=0; i < n; i++) {
      // System.out.println("Res = ["+ res + "]");
      if (res.cdr() == null) { return _NIL; }
      res = (Cons)(res.cdr());
    }
    // System.out.println("Returning = ["+ res.car() + "]");
    return res.car();
  }

  public Node nth(Integer n) {
    return nth(n.intValue());
  }

  public void set_nth(int n, Node to) {
    if (n >= 0) 
      { Cons res = this;
	for(int i=0; i < n; i++) {
	  if (res.cdr() == null) { res = null; break; }
	  res = (Cons)(res.cdr());
	}
	res.car = to;
      }
  }

  public Values toValues() {
    return Values.toValues(this);
  }

  public Values2 toValues2() {
    return Values.toValues2(this);
  }

  public Values3 toValues3() {
    return Values.toValues3(this);
  }

  public Values4 toValues4() {
    return Values.toValues4(this);
  }

  public static Node copy_tree(Node tree) {
    if (tree instanceof Cons)
      { Cons c = (Cons) tree;
	return new Cons(copy_tree(c.car()), copy_tree(c.cdr()));
      }
    else { return tree; }
  }

  public static Node last(Node list) {
    if (list == null || list == _NIL) { return _NIL; }
    else if (list instanceof Cons)
      { while (list.cdr() instanceof Cons) { list = list.cdr(); }
	return list;
      }
    else { throw new GenericError
	   (Node._T, new OKBCString("Cannot take the Last of a non-list: "
				    + list)).signal();
	 }
  }

  static public boolean member_frame_list(Node thing, Node list, Kb kb,
					  TruthValue kb_local_only_p) {
    Node frame = thing;
    if (!(frame instanceof AbstractSymbol))
      { frame = kb.get_frame_handle(frame, kb_local_only_p); }
    Enumerator e = list.enumerate_list();
    while (e.has_more_p()) {
      Node f = e.next();
      if (kb.eql_in_kb(f, frame, kb_local_only_p) != _NIL)
	{ return true; }
    }
    return false;
  }

  public static Vector toVector (Node list) {
    Vector v = new Vector(list.length());
    Enumeration e = list.elements();
    for (; e.hasMoreElements() ;) {
      Node elt = (Node)(e.nextElement());
      v.addElement(elt);
    }
    return v;
  }

  public int hashCode() {
    int ret;
    int car_code = 2001;
    if (car != null) { car_code = car.hashCode(); }
    int cdr_code = 19009;
    if (cdr != null) { cdr_code = cdr.hashCode(); }
    ret = (car_code ^ cdr_code);
    // System.out.println("    computed for " + this + " = " + ret);
    return ret;
  }

  static boolean is_in_tree_p (Node thing, Node tree) {
    return is_in_tree_p(thing, tree, null);
  }
  
  static boolean is_in_tree_p (Node thing, Node tree, Kb kb) {
    if (tree instanceof Cons)
      { Cons c = (Cons) tree;
	if (is_in_tree_p(thing, c.car(), kb)) { return true; }
	else if (c.cdr() instanceof Cons) 
	  { return is_in_tree_p(thing, c.cdr(), kb);}
	else { return false; }
      }
    else if (kb != null) { return kb.eql_in_kb(tree, thing, _NIL) != _NIL; }
    else { return tree == thing; }
  }

  public static void main (String arg[]) {
    Node l0 = _NIL;
    System.out.println("Length of l0 = " + l0.length());
    Node l1 = Cons.list(_all, _all, _all);
    System.out.println("Length of l1 = " + l1.length());
    Node l2 = new Cons(_all, new Cons(_all, _all));
    System.out.println("Length of l2 = " + l2.length());
  }

}

