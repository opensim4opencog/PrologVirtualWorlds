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

public class Values {
  public Node elements[] = null;
  
  public Values () { }

  Values(int i) {
    elements =  new Node[i];
  }

  public Values (Node n0) {
    elements =  new Node[1];
    elements[0] = n0;
  }
  
  public Values (int i, Node n0) {
    elements =  new Node[i];
    elements[0] = n0;
  }
  
  public Enumeration elements() {
    return firstValue().elements();
  }

  public Node nthValue(Integer n) {
    return elements[n.intValue()];
  }

  public Node nthValue(int n) {
    return elements[n];
  }

  public Node firstValue() {
    return elements[0];
  }

  public void set_nthValue(int n, Node to) {
    elements[n] = to;
  }

  public int length() {
    return elements.length;
  }

  public String toString() {
    StringBuffer result = new StringBuffer();
    result.append("(values ");
    for(int i=0; i < elements.length; i++) {
      if (i > 0) result.append(" ");
      result.append(elements[i].toString());
    }
    result.append(")");
    return result.toString();
  }

  public static Values makeValues(int length) {
    Values values = new Values();
    values.elements = new Node[length];
    return values;
  }

  public Values toValues() {
    return this;
  }

  public static Values toValues(Node list) {
    Values v = new Values();
    int len = list.length();
    v.elements = new Node[len];
    for(int i=0; i < len; i++) {
      v.elements[i] = list.nth(i);
    }
    return v;
  }

  public static Values2 toValues2(Node list) {
    return new Values2(list.nth(0), list.nth(1));
  }

  public static Values3 toValues3(Node list) {
    return new Values3(list.nth(0), list.nth(1), list.nth(2));
  }

  public static Values4 toValues4(Node list) {
    return new Values4(list.nth(0), list.nth(1), list.nth(2), list.nth(2));
  }

  public Node toList() {
    if (elements.length == 0) { return Node._NIL; }
    else { Cons root = null;
	   Cons tail = null;
	   for(int i=0; i < elements.length; i++) {
	     Node elt = elements[i];
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

  public boolean values_equals(Values thing) {
    boolean result;
    if (this == thing)
      { result = true; }
    else if (this.elements.length == thing.elements.length)
      { result = true;
	for(int i = 0; i < elements.length; i++) {
	  if (!elements[i].equals(thing.elements[i]))
	    { result = false;
	      break;
	    }
	}
      }
    else { result = false; }
    return result;
  }

  public boolean equals(Object thing) {
    if (thing instanceof Values)
      { return this.values_equals((Values)thing); }
    else { return false; }
  }
}
