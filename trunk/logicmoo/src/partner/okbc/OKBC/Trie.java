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

class Byte {
  byte value;

  Byte(byte the_value) {
    value = the_value;
  }

  public String toString() {
    return "(byte)" + value;
  }

}

class Short {
  short value;

  Short(short the_value) {
    value = the_value;
  }

  public String toString() {
    return "(short)" + value;
  }

}

public class Trie extends Node {
  public Object value = __deleted__;
  Object arcs  = null;

  static Symbol __start__    = Symbol.intern("__start__",    "Trie");
  static Symbol __end__      = Symbol.intern("__end__",      "Trie");
  static Symbol __OKBCStart__ = Symbol.intern("__OKBCStart__", "Trie");
  static Symbol __OKBCEnd__   = Symbol.intern("__OKBCEnd__",   "Trie");
  static Symbol __dot__      = Symbol.intern("__dot__",      "Trie");
  static Symbol __deleted__  = Symbol.intern("__deleted__",  "Trie");

  static int entries_provokes_hashification_threshold = 20;
  static int trie_entry_initial_hash_table_size
  = entries_provokes_hashification_threshold * 15;
  static int trie_entry_rehash_size = 6;


  public String toString() {
    return "Trie[v=" + value + 
      // ",a=" + arcs + 
	   "]";
  }

  int trie_entries() {
    if (arcs == null) {return 0;}
    else { return ((Cons)arcs).length(); }
  }

  boolean trie_hashed_p () {
    if (arcs == null) {return false;}
    else { if (arcs instanceof Cons)
	     { return false; }
    else { return true; }
	 }
  }

  Object trie_hash_key (Object thing) { return thing; }

  Trie follow_arc (Object component, boolean extend_p) {
    if (component == null) 
      { throw new RuntimeException("Cannot store a null in a trie"); }
    boolean hashified_p = this.trie_hashed_p();
    Object hash_key = null;

    Cons arc = null;
    if (hashified_p)
      { hash_key = this.trie_hash_key(component);
	arc = (Cons)(((Hashtable)arcs).get(hash_key));
      }
    else
      {  // loop and return matching entry
	for (Cons e = (Cons)arcs ; e != null ; e = (Cons)e.cdr) {
	  // System.out.println("Compare: " + component + " to " + ((Cons) e.car).car + " result= " + (((Cons) e.car).car.equals(component)));
	  Cons the_car = (Cons)e.car;
	  if (the_car.car == null) 
	    { throw new RuntimeException("eh? " + the_car); }
	  if (the_car.car.equals(component)) {
	    arc = (Cons)e.car;
	    break;
	  }
	}
      }

    if (arc == null)
      { if (extend_p)
	  { Trie new_trie = new Trie();
	    new_trie.value = __deleted__;
	    if (!hashified_p && 
		(entries_provokes_hashification_threshold
		 == this.trie_entries()))
	      {
		Hashtable ht
		  = new Hashtable(trie_entry_initial_hash_table_size);

		Enumeration e = ((Cons)arcs).elements();
		for (; e.hasMoreElements() ;) {
		  arc = (Cons)(e.nextElement());
		  ht.put(this.trie_hash_key(arc.car), arc);
		}
		arcs = ht;
		ht.put(this.trie_hash_key(component),
		       new Cons(component, new_trie));
	      }
	    else if (hashified_p)
	      {
		((Hashtable)arcs).put(hash_key, new Cons(component, new_trie));
	      }
	    else 
	      {
		arcs = new Cons(new Cons(component, new_trie), arcs);
	      }
	    return new_trie;
	  }
	else { return null; }
      }
    else { return (Trie) (((Cons)arc).cdr); }
  }

  public Trie find_trie(Object key, boolean extend_p, Trie trie) {
    if (trie == null)
      { return null; }
    else if (key instanceof Cons)
      { return find_list(key, extend_p, 
			 trie.find_trie(__start__, extend_p, trie));
      }
    else { return trie.follow_arc(key, extend_p); }
  }

  Trie find_list(Object list, boolean extend_p, Trie trie) {
    if (list instanceof Cons)
      { return trie.find_list(((Cons)list).cdr, extend_p,
			 trie.find_trie(((Cons)list).car, extend_p, trie));
      }
    else if (list == null || list == Node._NIL)
      { return trie.find_trie(__end__, extend_p, trie); }
    else 
      { return trie.find_trie(list, extend_p,
			      trie.find_trie(__dot__, extend_p, trie));
      }
  }

  public Object get_trie(Object key) {
    return get_trie(key, null);
  }

  public Object get_trie(Object key, Object deflt) {
    Trie key_trie = this.find_trie(key, true, this);
    Object val;
    Object result;
    if (key_trie == null)
      { result = deflt; }
    else { val = key_trie.value;
	   if (val == __deleted__) 
	     { result = deflt; }
	   else { result = val; }
	 }
    return result;
  }

  public TrieResult get_trie_returning_node(Object key, Object deflt) {
    if (key == null) { throw new RuntimeException ("null key found!"); }
    Trie key_trie = this.find_trie(key, true, this);
    Object val;
    TrieResult result;
    if (key_trie == null)
      { result = new TrieResult(deflt, false, key_trie); }
    else { val = key_trie.value;
	   if (val == __deleted__) 
	     { result = new TrieResult(deflt, false, key_trie); }
	   else { result = new TrieResult(val, true, key_trie); }
	 }
    // System.out.println("TrieGet on : " + key + " result " + result + " Trie=" + this);
    return result;
  }

  public TrieResult get_trie_returning_node(Object key) {
    return get_trie_returning_node(key, null);
  }

  public TrieResult get_trie_returning_node(byte key) {
    return get_trie_returning_node(new Byte(key), null);
  }

  public TrieResult get_trie_returning_node(short key) {
    return get_trie_returning_node(new Short(key), null);
  }

  public TrieResult get_trie_returning_node(int key) {
    return get_trie_returning_node(new Integer(key), null);
  }

  public TrieResult get_trie_returning_node(long key) {
    return get_trie_returning_node(new Long(key), null);
  }

  public TrieResult get_trie_returning_node(float key) {
    return get_trie_returning_node(new Float(key), null);
  }

  public TrieResult get_trie_returning_node(double key) {
    return get_trie_returning_node(new Double(key), null);
  }

  public TrieResult get_trie_returning_node(boolean key) {
    return get_trie_returning_node(new Boolean(key), null);
  }

  public void map_trie_nodes (TrieMapFunction function) {
    map_trie_nodes(function, false);
  }
  
  public void map_trie_nodes (TrieMapFunction function, boolean deleted_p) {
    try { map_trie_nodes_internal(function, false); }
    catch (TrieMapAbort e) {}
  }
  
  public void map_trie_nodes_internal
     (TrieMapFunction function, boolean deleted_p) throws TrieMapAbort {
    if (deleted_p || (value != __deleted__))
      { boolean continue_p = function.funcall(this);
	if (!continue_p) { throw TrieMapAbort.$abort$; }
      }
    Object hash_key = null;
    Cons arc = null;
    boolean hashified_p = trie_hashed_p();
    if (hashified_p)
      {
	Hashtable ht = ((Hashtable)arcs);
	Enumeration e = ht.keys();
	while(e.hasMoreElements()) {
	  Cons c = (Cons)(ht.get(e.nextElement()));
	  if (c != null)
	    { Trie t = (Trie)(c.cdr);
	      t.map_trie_nodes_internal(function, deleted_p);
	    }
	}
      }
    else
      {  // loop over alist
	Cons alist = (Cons)arcs;
	if (alist == null) {}
	else
	  {
	    if (alist.cdr() instanceof Cons)
	      {
		// Do it the iterative way, we can't be tail recursive!
		for (Cons e = alist ; e != null ; e = (Cons)e.cdr) {
		  Cons the_car = (Cons)e.car;
		  if (the_car.car == null) 
		    { throw new RuntimeException("eh? " + the_car); }
		  ((Trie)the_car.cdr).map_trie_nodes_internal
		    (function, deleted_p);
		}
	      }
	    else
	      {
		((Trie)alist.car().cdr()).map_trie_nodes_internal
		  (function, deleted_p);
	      }
	  }
      }
  }
  
  //-----------------------------------------------------


  public void delete_trie(Object key) {
    Trie key_trie = this.find_trie(key, true, this);
    key_trie.value = __deleted__;
  }

  public void put_trie(Object key, Object value) {
    Trie key_trie = this.find_trie(key, true, this);
    key_trie.value = value;
  }

  public void put_trie(Object key, byte value) {
    Trie key_trie = this.find_trie(key, true, this);
    key_trie.value = new Byte(value);
  }

  public void put_trie(Object key, short value) {
    Trie key_trie = this.find_trie(key, true, this);
    key_trie.value = new Short(value);
  }

  public void put_trie(Object key, long value) {
    Trie key_trie = this.find_trie(key, true, this);
    key_trie.value = new Long(value);
  }

  public void put_trie(Object key, float value) {
    Trie key_trie = this.find_trie(key, true, this);
    key_trie.value = new Float(value);
  }

  public void put_trie(Object key, double value) {
    Trie key_trie = this.find_trie(key, true, this);
    key_trie.value = new Double(value);
  }

  public static void main (String arg[]) {
    entries_provokes_hashification_threshold = 1; // hack alert for testing
    String s1 = "s1";
    String s2 = "s2";
    String s3 = "s3";
    Cons l1 = new Cons("a", new Cons("b"));
    Cons l2 = new Cons("b", new Cons("c"));
    Cons l3 = new Cons("b", "d");
    Node l4 = Cons.list(Symbol.keyword("A"), Symbol.keyword("B"));
    Node l5 = Cons.list(Symbol.keyword("A"), Symbol.keyword("C"));
    Trie x = new Trie();
    TrieResult res;

    System.out.println("Result Abefore= " + x.get_trie_returning_node(l4).toString());
    x.put_trie(l4, new Cons(2));
    System.out.println("Result Aafter= " + x.get_trie_returning_node(l4).toString());
    
    x.put_trie(l4, 42);
    System.out.println("Result Aafter2= " + x.get_trie_returning_node(l4).toString());

    System.out.println("Result Bbefore= " + x.get_trie_returning_node(l5).toString());
    x.put_trie(l5, l4);
    System.out.println("Result Bafter= " + x.get_trie_returning_node(l5).toString());
    x.put_trie(l4, l4);
    
    

    System.out.println("l1== " + l1);
    System.out.println("l2== " + l2);
    System.out.println("l3== " + l3);
    System.out.println("Trie== " + x);
    System.out.println("Result 3before= " + x.get_trie_returning_node(l1).toString());
    System.out.println("Trie== " + x);
    x.put_trie(l1, new Cons(2));
    System.out.println("Result 3after= " + x.get_trie_returning_node(l1).toString());
    System.out.println("Trie== " + x);


    System.out.println("Result 4before= " + x.get_trie_returning_node(l2).toString());
    x.put_trie(l2, 3);
    System.out.println("Result 4after= " + x.get_trie_returning_node(l2).toString());

    x.put_trie(s1, 1);
    System.out.println("Result 1b= " + x.get_trie_returning_node("s1").toString());
    System.out.println("Result 1b= " + x.get_trie_returning_node(s1).toString());

    x.put_trie(s2, 42);
    System.out.println("Result 2a= " + x.get_trie_returning_node("s2").toString());
    System.out.println("Result 2b= " + x.get_trie_returning_node(s2).toString());

    System.out.println("Result 5= " + x.get_trie_returning_node(new Cons(2)).toString());
    System.out.println("Values in nodes:");
    x.map_trie_nodes(TrieMapFunction.print, true);
    System.out.println("End of values");
  }

}

class TrieMapAbort extends Throwable {
  static final TrieMapAbort $abort$ = new TrieMapAbort();
}
