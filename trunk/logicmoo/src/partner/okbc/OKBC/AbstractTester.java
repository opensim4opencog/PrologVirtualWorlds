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

public class AbstractTester extends Node {

  public static int violations = 0;

  protected static void test_true
    (Kb kb, String s, Object r, boolean verbose_p) {
    Node result = null;
    if (r instanceof Values) { result = ((Values)r).firstValue(); }
    else { result = (Node) r; }
    if (result == _NIL || result == null)
      { System.out.println("**** Test violation on test:  " + s);
        System.out.println("Actual result was: " + result);
	violations = violations + 1;
      }
    else { if (verbose_p) { System.out.println("  Ok -- " + s); } }
  }

  protected static void test_false
      (Kb kb, String s, Object r, boolean verbose_p) {
    Node result = null;
    if (r instanceof Values) { result = ((Values)r).firstValue(); }
    else { result = (Node) r; }
    if (result == _NIL || result == null)
      { if (verbose_p) { System.out.println("  Ok -- " + s); } }
    else { System.out.println("**** Test violation on test:  " + s);
	   System.out.println("Actual result was: " + result);
	   violations = violations + 1;
	   if (Condition.listener_on_error_p)
	     { Listener.breakLoop(kb); }
	 }
  }

  protected static void test_equal
     (Kb kb, String s, Object r, Object original, boolean verbose_p) {
    Object result = null;
    if (r instanceof Values && (!(original instanceof Values)))
      { result = ((Values)r).firstValue(); }
    else { result =  r; }
    if (result.equals(original)) 
      { if (verbose_p) { System.out.println("  Ok -- " + s); } }
    else { System.out.println("**** Test violation on test:  " + s);
	   System.out.println("Expected result was: " + original);
	   System.out.println("  Actual result was: " + result);
	   violations = violations + 1;
	   if (Condition.listener_on_error_p)
	     { Listener.breakLoop(kb); }
	 }
  }

  protected static boolean equal_except_packages(Object a, Object b) {
    boolean res = false;
    if (a == null)
      { res = a == b; }
    else if (b == null) { res = false; }
    else if (a instanceof Cons)
      { if (b instanceof Cons)
	  { res =
	    equal_except_packages(((Cons)a).car(), ((Cons)b).car()) &&
	    equal_except_packages(((Cons)a).cdr(), ((Cons)b).cdr());
	  }
	else { res = false; }
      }
    else if (a instanceof AbstractSymbol)
      { if (b instanceof AbstractSymbol)
	  { res = ((AbstractSymbol)a).symbolName.equals
	      (((AbstractSymbol)b).symbolName); 
	  }
	else { res = false; }
      }
    else { res = a.equals(b); }
    if (!res) { System.out.println("Not equal: " + a + " and " + b); }
    return res;
  }


  protected static void test_equal_except_packages
         (Kb kb, String s, Object r, Object original, 
	  boolean verbose_p) {
    Object result = null;
    if (r instanceof Values && (!(original instanceof Values)))
      { result = ((Values)r).firstValue(); }
    else { result = r; }
    if (equal_except_packages(result, original))
      { if (verbose_p) { System.out.println("  Ok -- " + s); } }
    else { System.out.println("**** Test violation on test:  " + s);
	   System.out.println("Expected result was: " + original);
	   System.out.println("  Actual result was: " + result);
	   violations = violations + 1;
	   if (Condition.listener_on_error_p)
	     { Listener.breakLoop(kb); }
	 }
  }


  static boolean equal_sets_1(Node a, Node b, Kb kb) {
    if (a == null) { return b == null; }
    if (a == _NIL) { return b == _NIL; }
    if (b == null) { return a == null; }
    if (b == _NIL) { return a == _NIL; }
    Cons ca = (Cons)a;
    Cons cb = (Cons)b;
    if (Cons.member_equal(ca.car(), cb, kb, _NIL))
      { return equal_sets_1(Cons.remove_equal(ca.car(), ca, kb),
                            Cons.remove_equal(ca.car(), cb, kb),
			    kb);
      }
    else { return false; }
  }

  protected static void test_equal_sets
      (Kb kb, Node original, String s, Object r, boolean verbose_p) {
    Node result = null;
    if (r instanceof Values) { result = ((Values)r).firstValue(); }
    else { result = (Node) r; }
    if (result == null) { result = _NIL; }
    if (equal_sets_1(original, result, kb))
      { if (verbose_p) { System.out.println("  Ok -- " + s); } }
    else { System.out.println("**** Test violation on equal sets test:  " + s);
	   System.out.println("Expected result was: " + original);
	   System.out.println("  Actual result was: " + result);
	   violations = violations + 1;
	   if (Condition.listener_on_error_p)
	     { Listener.breakLoop(kb); }
	 }
  }

  static Node non_subsets_1(Node a, Node b, Kb kb) {
    if (a == null) { return _NIL; }
    if (a == _NIL) { return _NIL; }
    if (b == null) { return ((a == null) ? (Node)_NIL : a); }
    if (b == _NIL) { return ((a == null) ? (Node)_NIL : a); }
    Cons ca = (Cons)a;
    Cons cb = (Cons)b;
    if (Cons.member_test(ca.car(), cb, _equal_in_kb, kb, _NIL))
      { return non_subsets_1
	(Cons.remove_test(ca.car(), ca, _equal_in_kb, kb, _NIL),
	 Cons.remove_test(ca.car(), cb, _equal_in_kb, kb, _NIL),
	 kb);
      }
    else { return new Cons(ca.car(), 
			   non_subsets_1
			   (Cons.remove_test(ca.car(), ca, _equal_in_kb, kb,
					     _NIL),
			    cb, kb));
	 }
  }

  protected static void test_all_in_list
      (Kb kb, Node must_all_be_in_this_list, String s, 
       Object all_in_this_list, boolean verbose_p) {
    Node result = null;
    if (all_in_this_list instanceof Values)
      { result = ((Values)all_in_this_list).firstValue(); }
    else { result = (Node) all_in_this_list; }
    if (result == null) { result = _NIL; }
    Node difference = non_subsets_1(result, must_all_be_in_this_list, kb);
    if (difference == _NIL)
      { if (verbose_p) { System.out.println("  Ok -- " + s); } }
    else { System.out.println("**** Test violation on subset test:  " + s);
	   System.out.println("Expected result was: " + 
			      
			      must_all_be_in_this_list);
	   System.out.println("  Actual result was: " + result);
	   System.out.println("  Excess was: " + difference);
	   violations = violations + 1;
	   if (Condition.listener_on_error_p)
	     { Listener.breakLoop(kb); }
	 }
  }

  protected static void test_at_least
      (Kb kb, Node original, String s, Object r, boolean verbose_p) {
    Node result = null;
    if (r instanceof Values) { result = ((Values)r).firstValue(); }
    else { result = (Node) r; }
    if (result == null) { result = _NIL; }
    Node difference = non_subsets_1(original, result, kb);
    if (difference == _NIL)
      { if (verbose_p) { System.out.println("  Ok -- " + s); } }
    else { System.out.println("**** Test violation on subset test:  " + s);
	   System.out.println("Expected result was: " + original);
	   System.out.println("  Actual result was: " + result);
	   System.out.println("  Excess was: " + difference);
	   violations = violations + 1;
	   if (Condition.listener_on_error_p)
	     { Listener.breakLoop(kb); }
	 }
  }

  protected static Node get_unique_frame(AbstractSymbol symbol, Kb kb) {
    Node res = kb.get_frames_matching(symbol, _NIL, _all, _NIL, 
				      _NIL).firstValue();
    if (res != _NIL)
      { return res.car(); }
    else { throw new NotCoercibleToFrame(symbol, kb).signal(); }
  }


}
