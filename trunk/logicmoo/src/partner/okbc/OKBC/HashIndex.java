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

// An implementation of Adam Farquhar's magical 16-way indexing scheme for
// sentences.
public class HashIndex {

  static int hash_index_table_size = 5000;

  Hashtable table = new Hashtable(hash_index_table_size);
  Kb kb = null;
  Node name = null;

  static final int  var_hash_key = new Symbol( "+var-hash-key+").hashCode();
  static final int null_hash_key = new Symbol("+null-hash-key+").hashCode();

  void clear_hash_index () { table.clear(); }


  public static int fkey (Node pat) {
    if (pat instanceof Cons)
      { return fkey(pat.car()); }
    else if (pat instanceof AbstractSymbol)
      {
	String s = ((AbstractSymbol)pat).symbolName;
	boolean var_p = (s.length() > 0 && s.charAt(0) == '?');
	if (var_p) { return var_hash_key; }
	else { return pat.hashCode(); }
      }
    else { return pat.hashCode(); }
  }



  static final long modulus = 7514797;

  static long update_hash (long hash, long with) {
    hash = ((hash << 8) + (with & 255)) % modulus;
    with = with >> 8;
    hash = ((hash << 8) + (with & 255)) % modulus;
    with = with >> 8;
    hash = ((hash << 8) + (with & 255)) % modulus;
    with = with >> 8;
    hash = ((hash << 8) + (with & 255)) % modulus;
    return hash;
  }
  
  static int hash4 (int mask, int k0, int k1, int k2, int k3, int len) {
    long hash = 0;
    k0 = ((mask & 1) != 0 ? k0 : var_hash_key);
    k1 = ((mask & 2) != 0 ? k1 : var_hash_key);
    k2 = ((mask & 4) != 0 ? k2 : var_hash_key);
    k3 = ((mask & 8) != 0 ? k3 : var_hash_key);
    hash = update_hash(hash, k0);
    hash = update_hash(hash, k1);
    hash = update_hash(hash, k2);
    hash = update_hash(hash, k3);
    hash = update_hash(hash, len);
    // System.out.println((int)hash + " <- " +k0+","+k1+","+k2+","+k3+","+mask+","+len+" -> ");
    return (int)hash;
  }


  static public int pattern_hash_key (Node pat) {
    Node list = pat;
    int len = list.length();
    int mask = (2 << (len - 1));
    if (mask > 16) { mask = 16; }
    mask = mask - 1;
    int k0 = fkey(list.car());
    int k1 = null_hash_key;
    int k2 = null_hash_key;
    int k3 = null_hash_key;
    list = list.cdr();
    if (list instanceof Cons)
      {
	k1 = fkey(list.car());
	list = list.cdr();
	if (list instanceof Cons)
	  {
	    k2 = fkey(list.car());
	    list = list.cdr();
	    if (list instanceof Cons)
	      {
		k3 = fkey(list.car());
		list = list.cdr();
	      }
	  }
      }
    return hash4(mask, k0, k1, k2, k3, len);
  }



  public Node get_matches (Node pat) {
    return get_matches(pat, pattern_hash_key(pat));
  }



  Node get_matches (Node pat, int key) {
    Queue q = ((Queue)table.get(new Integer(key)));
    if (q == null) { return Node._NIL; }
    else { return q.queue_contents(); }
  }


  Queue get_matching_queue (Node pat) {
    return get_matching_queue(pat, pattern_hash_key(pat));
  }



  Queue get_matching_queue (Node pat, int key) {
    return ((Queue)table.get(new Integer(key)));
  }


  public Node value_is_indexed_p (Node val, Integer key_int) {
    if (key_int != null)
      { Queue this_q = (Queue)table.get(key_int);
	if (this_q != null)
	  {
	    Node res = Cons.find_equal(val, this_q.queue_contents(), null,
				       Node._NIL);
	    return res;
	  }
	else { return Node._NIL; }
      }
    else
      {
	Node list = val;
	int len = list.length();
	int k0 = fkey(list.car());
	int k1 = null_hash_key;
	int k2 = null_hash_key;
	int k3 = null_hash_key;
	list = list.cdr();
	if (list instanceof Cons)
	  {
	    k1 = fkey(list.car());
	    list = list.cdr();
	    if (list instanceof Cons)
	      {
		k2 = fkey(list.car());
		list = list.cdr();
		if (list instanceof Cons)
		  {
		    k3 = fkey(list.car());
		    list = list.cdr();
		  }
	      }
	  }
	int max = (2 << (len - 1));
	if (max > 16) { max = 16; }
	Queue this_q = (Queue)table.get
	  (new Integer(hash4(max - 1, k0, k1, k2, k3, len)));
	if (this_q != null)
	  { return Cons.find_equal(val, this_q.queue_contents(), null,
				   Node._NIL);
	  }
	else { return Node._NIL; }
      }
  }


  void check_consistency (Node f) {
    Node fa = Symbol.intern("?fa", "OKBC-TEST");
    Node sl = Symbol.intern("?sl", "OKBC-TEST");
    Node fr = Symbol.intern("?fr", "OKBC-TEST");
    Node v = Symbol.intern("?v", "OKBC-TEST");
    boolean fail_p = false;
    if (f.length() == 2)
      { 
	Enumerator framee = Cons.list(fr, f.car()).enumerate_list();
	while(framee.has_more_p()) {
	  Node frame = framee.next();
	  Enumerator valuee = Cons.list(v, f.cadr()).enumerate_list();
	  while(valuee.has_more_p()) {
	    Node value = valuee.next();
	    Node pattern = Cons.list(frame, value);
	    Queue results = get_matching_queue(pattern);
	    if (Cons.member_equal(f, results.queue_contents(),
				  null, Node._NIL)) {}
	    else
	      {
		fail_p = true;
		System.out.println
		  ("Missing fact " + f + " under pattern " + pattern); 
	      }
	  }
	}
      }
    else if (f.length() == 3)
      { 
	Enumerator slote = Cons.list(sl, f.car()).enumerate_list();
	while(slote.has_more_p()) {
	  Node slot = slote.next();
	  Enumerator framee = Cons.list(fr, f.cadr()).enumerate_list();
	  while(framee.has_more_p()) {
	    Node frame = framee.next();
	    Enumerator valuee = Cons.list(v, f.nth(2)).enumerate_list();
	    while(valuee.has_more_p()) {
	      Node value = valuee.next();
	      Node pattern = Cons.list(slot, frame, value);
	      Queue results = get_matching_queue(pattern);
	      if (Cons.member_equal(f, results.queue_contents(),
				    null, Node._NIL)) {}
	      else
		{
		  fail_p = true;
		  System.out.println
		    ("Missing fact " + f + " under pattern " + pattern +
		     " from queue " + results + 
		     ".  Other matches to pattern: " + 
		     results.queue_contents()); 
		}
	    }
	  }
	}
      }
    else if (f.length() == 4)
      { 
	Enumerator facete = Cons.list(sl, f.car()).enumerate_list();
	while(facete.has_more_p()) {
	  Node facet = facete.next();
	  Enumerator slote = Cons.list(sl, f.cadr()).enumerate_list();
	  while(slote.has_more_p()) {
	    Node slot = slote.next();
	    Enumerator framee = Cons.list(fr, f.nth(2)).enumerate_list();
	    while(framee.has_more_p()) {
	      Node frame = framee.next();
	      Enumerator valuee = Cons.list(v, f.nth(3)).enumerate_list();
	      while(valuee.has_more_p()) {
		Node value = valuee.next();
		Node pattern = Cons.list(facet, slot, frame, value);
		Queue results = get_matching_queue(pattern);
		if (Cons.member_equal(f, results.queue_contents(),
				      null, Node._NIL)) {}
		else
		  {
		    fail_p = true;
		    System.out.println
		      ("Missing fact " + f + " under pattern " + pattern); 
		  }
	      }
	    }
	  }
	}
      }
    if (fail_p) 
      { System.out.println("-----");
        throw new RuntimeException("Error"); 
      }
  }


  public Node put_in_table (Node key, Node val) {
    Node list = key;
    int len = list.length();
    int k0 = fkey(list.car());
    int k1 = null_hash_key;
    int k2 = null_hash_key;
    int k3 = null_hash_key;
    list = list.cdr();
    if (list instanceof Cons)
      {
	k1 = fkey(list.car());
	list = list.cdr();
	if (list instanceof Cons)
	  {
	    k2 = fkey(list.car());
	    list = list.cdr();
	    if (list instanceof Cons)
	      {
		k3 = fkey(list.car());
		list = list.cdr();
	      }
	  }
      }
    int max = (2 << (len - 1));
    if (max > 16) { max = 16; }
    Queue this_q = (Queue)table.get
      (new Integer(hash4(max - 1, k0, k1, k2, k3, len)));
    if (this_q != null &&
	Cons.member_equal(val, this_q.queue_contents(), null, Node._NIL))
      { return Node._NIL; }
    else
      {
	for (int mask=0 ; mask < max ; mask++) {
	  int hash_key = hash4(mask, k0, k1, k2, k3, len);
	  Integer hash_key_obj = new Integer(hash_key);
	  Queue q = (Queue)table.get(hash_key_obj);
	  if (q != null)
	    { q.enqueue(val); }
	  else 
	    {
	      q = new Queue();
	      q.enqueue(val);
	      table.put(hash_key_obj, q);
	    }
	  /* 
	  if (val.length() == 3 && val.car() == Test.foods
	      && val.cadr() == Test.bill && val.nth(2) == Test.seeds)
	    { System.out.println("Adding fact to " + q + " contents = " +
				 q.queue_contents());
	    }
	  */
	}
	// check_consistency(val);
	return val;
      }
  }


  public Node delete_from_table (Node pat, Node item) {
    Node matches = get_matches(pat);
    Node selected_item = Node._NIL;
    if (matches instanceof Cons)
      { 
	while (matches instanceof Cons) {
	  Node car = matches.car();
	  if (car.equals(item))
	    { selected_item = car; break; }
	  else { matches = matches.cdr(); }
	}
      }
    if (selected_item != Node._NIL)
      {
	Node list = pat;
	int len = list.length();
	int k0 = fkey(list.car());
	int k1 = null_hash_key;
	int k2 = null_hash_key;
	int k3 = null_hash_key;
	list = list.cdr();
	if (list instanceof Cons)
	  {
	    k1 = fkey(list.car());
	    list = list.cdr();
	    if (list instanceof Cons)
	      {
		k2 = fkey(list.car());
		list = list.cdr();
		if (list instanceof Cons)
		  {
		    k3 = fkey(list.car());
		    list = list.cdr();
		  }
	      }
	  }
	int max = (2 << (len - 1));
	if (max > 16) { max = 16; }
	for (int mask=0 ; mask < max ; mask++) {
	  int hash_key = hash4(mask, k0, k1, k2, k3, len);
	  Integer hash_key_obj = new Integer(hash_key);
	  Queue q = (Queue)table.get(hash_key_obj);
	  if (q != null)
	    {
	      // System.out.println("Previously contents = " + q.queue_contents());
	      q.delete(selected_item);
	      // System.out.println("With mask " + mask + ", removing " + pat + " results in " + q.queue_contents());
	    }
	}
	return Node._T;
      }
    else { return Node._NIL; }
  }


  // test harness

  static final Node a = Symbol.intern("A", "OKBC-TEST");
  static final Node b = Symbol.intern("B", "OKBC-TEST");
  static final Node p = Symbol.intern("P", "OKBC-TEST");
  static final Node xvar = Symbol.intern("?X", "OKBC-TEST");
  static final Node yvar = Symbol.intern("?Y", "OKBC-TEST");


  static final Node syms = Cons.list
  (a,
   b,
   Symbol.intern("C", "OKBC-TEST"),
   Symbol.intern("D", "OKBC-TEST"),
   Symbol.intern("E", "OKBC-TEST"),
   Symbol.intern("F", "OKBC-TEST"),
   Symbol.intern("G", "OKBC-TEST"),
   Symbol.intern("H", "OKBC-TEST"),
   Symbol.intern("I", "OKBC-TEST"),
   Symbol.intern("J", "OKBC-TEST"));

  static Node build_facts (int n) {
    Node facts = Node._NIL;
    Node atoms = Cons.firstn(n, syms);
    Enumerator ei = atoms.enumerate_list();
    while (ei.has_more_p()) {
      Node i = ei.next();
      Enumerator ej = atoms.enumerate_list();
      while (ej.has_more_p()) {
	Node j = ej.next();
	Enumerator ek = atoms.enumerate_list();
	while (ek.has_more_p()) {
	  Node k = ek.next();
	  facts = new Cons(Cons.list(p, i, j, k), facts);
	}
      }
    }
    return facts;
  }

  void index_em (Node facts) {
    Enumerator e = facts.enumerate_list();
    while (e.has_more_p()) {
      Node f = e.next();
      put_in_table(f, f);
    }
  }

  int tm () {
    Node pats = Cons.list(Cons.list(xvar, a, a, a),
			  Cons.list(p, a, xvar, xvar),
			  Cons.list(p, xvar, xvar, a),
			  Cons.list(p, xvar, a, b));
    int res = 0;
    Enumerator e = pats.enumerate_list();
    while (e.has_more_p()) {
      Node pat = e.next();
      res = res + get_matches(pat).length();
    }
    return res;
  }

  public static void main (String arg[]) {
    Queue q = new Queue();
    q.enqueue(Node._all);
    q.enqueue(Node._own);
    q.enqueue(Node._template);
    q.enqueue(Node._frame);
    System.out.println("Queue contents = " + q.queue_contents() + ", tail = " +
		       q.tail);
    q.delete(Node._own);
    System.out.println("Queue contents = " + q.queue_contents() + ", tail = " +
		       q.tail);
    q.delete(Node._frame);
    System.out.println("Queue contents = " + q.queue_contents() + ", tail = " +
		       q.tail);
    q.delete(Node._all);
    System.out.println("Queue contents = " + q.queue_contents() + ", tail = " +
		       q.tail);
    q.delete(Node._template);
    System.out.println("Queue contents = " + q.queue_contents() + ", tail = " +
		       q.tail);

    HashIndex h = new HashIndex();
    Node fact = Cons.list(Node._all, Node._class, Node._template,
			  Node._T, Node._own);
    h.put_in_table(fact, fact);
    Node fact2 = Cons.list(Node._all, Node._own, Node._template,
			   Node._T, Node._own);
    h.put_in_table(fact2, fact2);
    Node pat;
    pat = Cons.list(Node._all, xvar, Node._template,
			  Node._T, Node._own);
    System.out.println("Pat = " + pat + ", results = " + h.get_matches(pat));

    h.delete_from_table(fact2, fact2);
    System.out.println("Pat = " + pat + ", results = " + h.get_matches(pat));
    
    h.delete_from_table(fact, fact);
    System.out.println("Should be no results = " + h.get_matches(pat));

    h.put_in_table(fact, fact);
    System.out.println("Add back fact = " + h.get_matches(pat));
    
    h.clear_hash_index();
    System.out.println("Pat = " + pat + ", results = " + h.get_matches(pat));

    Node facts = build_facts(2);
    h.index_em(facts);
    int res = h.tm();
    System.out.println("Result of iterative test = " + res);

    Enumeration e = h.table.keys();
    while (e.hasMoreElements()) {
      Object key = e.nextElement();
      System.out.println(key + ", " + h.table.get(key));
    }

  }

}


class Queue {
  Node head = Node._NIL;
  Cons tail = null;

  void enqueue (Node item) {
    if (head == Node._NIL)
      { head = Cons.list(item);
        tail = (Cons)head;
      }
    else
      { 
	Cons newcons = (Cons)Cons.list(item);
	tail.set_cdr(newcons);
	tail = newcons;
      }
  }

  Node queue_contents () { return head; }

  void delete (Node item) {
    Node current = head;
    Cons loc = null;
    if (item == tail.car())
      {
	if (item == head.car())
	  { 
	    head = Node._NIL;
	    tail = null;
	  }
	else
	  { 
	    while (current.cdr() != tail) {
	      current = current.cdr();
	    }
	    ((Cons)current).set_cdr(null);
	    tail = (Cons)current;
	  }
      }
    else
      {
	while (current != null && current != Node._NIL) {
	  if (current.car() == item)
	    { 
	      if (loc == null)
		{ 
		  head = head.cdr();
		  current = head;
		}
	      else 
		{ 
		  loc.set_cdr(current.cdr());
		  current = current.cdr();
		}
	    }
	  else 
	    {
	      loc = (Cons)current;
	      current = current.cdr();
	    }
	}
	if (head == null)
	  {
	    head = Node._NIL;
	    tail = null;
	  }
      }
  }
}
