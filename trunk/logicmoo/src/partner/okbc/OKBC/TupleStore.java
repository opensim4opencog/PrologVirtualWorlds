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

class RootTrie extends Trie {
  Object up = null;
  
  RootTrie (Object the_up) {
    up = the_up;
  }
}

public class TupleStore extends AbstractFactStore {
  Hashtable dbclass_table = new Hashtable();
  public boolean full_index_p = true;
  public Hashtable full_index = new Hashtable();
  public HashIndex hash_index = null;
  public Trie atom_to_sentence_index = null;

  public TupleStore(AbstractFactStoreKb the_kb) {
    kb = the_kb;
  }

  public TupleStore(AbstractFactStoreKb the_kb, boolean hash_index_p) {
    kb = the_kb;
    if (hash_index_p)
      {
	hash_index = new HashIndex();
	atom_to_sentence_index = new RootTrie(this);
	full_index = null;
	dbclass_table = null;
      }
    else {}
  }

  public static boolean variable_p (Object x) {
    if (x instanceof AbstractSymbol)
      { String s = ((AbstractSymbol)x).symbolName;
	return (s.length() > 0 && s.charAt(0) == '?');
      }
    else { return false; }
  }

  public static boolean anonymous_variable_p (Object x) {
    if (x instanceof AbstractSymbol)
      { String s = ((AbstractSymbol)x).symbolName;
	return (s.length() == 1 && s.charAt(0) == '?');
      }
    else { return false; }
  }

  public static boolean sequence_variable_p (Object x) {
    if (x instanceof AbstractSymbol)
      { String s = ((AbstractSymbol)x).symbolName;
	return (s.length() > 0 && s.charAt(0) == '@');
      }
    else { return false; }
  }

  public static boolean kif_variable_p (Object x) {
    return (variable_p(x) || sequence_variable_p(x));
  }

  static Vector reverse (Vector vect) {
    Vector ret = new Vector();
    int top_index = vect.size() - 1;
    for(int i=top_index; i >= 0; i--) {
      ret.addElement(vect.elementAt(i));
    }
    return ret;
  }

  static Vector vars_in (Object pattern, Vector vars) {
    if (pattern instanceof Cons)
      { Cons pat = (Cons)pattern;
	if (pat.car() == Node.__quote) { return vars; }
	else { return vars_in(pat.cdr(), vars_in(pat.car(), vars)); }
      }
    else if (variable_p(pattern) && !(anonymous_variable_p(pattern)))
      { if (!vars.contains(pattern)) { vars.addElement(pattern); }
	return vars;
      }
    else { return vars; }
  }

  public static Vector variables_in (Object pattern, Vector vars) {
    return reverse(vars_in(pattern, reverse(vars)));
  }

  public static Vector variables_in (Object pattern) {
    return reverse(vars_in(pattern, new Vector()));
  }

  static boolean free_in_p (AbstractSymbol var, Object exp, Object bindings) {
    if (exp == null || exp == Node._NIL) { return true; }
    else if (var.equals(exp)) { return false; }
    else if (variable_p(exp))
      { if (bindings instanceof Cons)
	  { Cons b = (Cons) bindings;
	    Cons val = b.assoc(exp);
	    if (val == null) { return true; }
	    else { return free_in_p(var, val.cdr(), bindings); }
	  }
	else { return true; }
      }
    else if (!(listp(exp))) { return true; }
    else if (free_in_p(var, ((Cons)exp).car(), bindings))
      { return free_in_p(var, ((Cons)exp).cdr(), bindings); }
    else { return false; }
  }

  static boolean listp (Object x) {
    return ((x == null) || (x instanceof Cons));
  }

  static Object unify_variable(AbstractSymbol var, Object exp,
			       Object bindings) {
    Object res = null;
    // System.out.println("Try unify_variable(" + var + ", " + exp + ") : " + bindings);
    if (anonymous_variable_p(var)) { res = bindings; }
    else { Cons val = (bindings == null ? null : ((Cons)bindings).assoc(var));
	   if (!(val == null)) { res = unify(val.cdr(), exp, bindings); }
	   else if (free_in_p(var, exp, bindings))
	     { res = new Cons(new Cons(var, exp), bindings); }
	   else { res = Node.__fail; }
	 }
	 // System.out.println("unify_variable(" + var + ", " + exp + ") : " + bindings + " -> " + res);
    return res;
  }

  public static Object unify(Object a, Object b) {
    return unify(a, b, null);
  }

  public static Object unify(Object a, Object b, Object bindings) {
    // System.out.println("Try unify(" + a + ", " + b + ") : " + bindings);
    Object res = null;
    if (a == b) { res = bindings; }
    else if (a == null) { res = Node.__fail; }
    else if (a.equals(b)) { res = bindings; }
    else if (variable_p(a)) { res = unify_variable((AbstractSymbol)a,
						   b, bindings); }
    else if (variable_p(b)) { res = unify_variable((AbstractSymbol)b,
						   a, bindings); }
    else if (a instanceof Cons && b instanceof Cons)
      { Cons ca = (Cons) a;
	Cons cb = (Cons) b;
	Object new_bindings = unify(ca.car(), cb.car(), bindings);
	// System.out.println("partial result(" + ca.car() + ", " + cb.car() + ") : " + bindings + " -> " + new_bindings + " fail = " + Node.__fail);
	if (new_bindings == Node.__fail) { res = Node.__fail; }
	else { res = unify(ca.cdr(), cb.cdr(), new_bindings); }
      }
    else { res = Node.__fail; }
     // System.out.println("unify(" + a + ", " + b + ") : " + bindings + " -> " + res);
    return res;
  }


  static Node unify_variable_against_literal
       (AbstractSymbol var, Object exp, Node bindings) 
  {
    Node res = null;
    if (anonymous_variable_p(var)) { res = bindings; }
    else { Cons val = (bindings == null ? null : ((Cons)bindings).assoc(var));
	   if (val != null)
	     { if (variable_p(val.cdr()))
		 { if (val.cdr() == exp)
		     { res = bindings; }
		   else { res = Node.__fail; }
		 }
	       else { res = unify_against_literal(val.cdr(), exp, bindings); }
	     }
	   else if (var == exp || free_in_p(var, exp, bindings))
	     { res = new Cons(new Cons(var, exp), bindings); }
	   else { res = Node.__fail; }
	 }
    // System.out.println("unify_variable_against_literal" + Cons.list(var, (Node)exp, bindings) + " -> " + res);
    return res;
  }


  public static Node unify_against_literal
      (Object a, Object b, Node bindings) 
  {
    Node res = null;
    if (a == b && variables_in(a).size() == 0) { res = bindings; }
    else if (a == null) { res = Node.__fail; }
    else if (a.equals(b) && variables_in(a).size() == 0) { res = bindings; }
    else if (variable_p(a))
      { res = unify_variable_against_literal((AbstractSymbol)a, b, bindings); }
    else if (variable_p(b))
      { res = unify_variable_against_literal((AbstractSymbol)b, a, bindings); }
    else if (a instanceof Cons && b instanceof Cons)
      { Cons ca = (Cons) a;
	Cons cb = (Cons) b;
	Node new_bindings = unify_against_literal
	  (ca.car(), cb.car(), bindings);
	if (new_bindings == Node.__fail) { res = Node.__fail; }
	else { res = unify_against_literal(ca.cdr(), cb.cdr(), new_bindings); }
      }
    else { res = Node.__fail; }

    // System.out.println("Varsin = " + variables_in(a) + " : " + variables_in(a).size());
    //  System.out.println("unify_against_literal" + Cons.list((Node)a, (Node)b, bindings) + " -> " + res);
    return res;
  }


  public static Node unify_against_literal(Object a, Object b) {
    Node res = unify_against_literal(a, b, null);
    // System.out.println("unify_against_literal" + Cons.list((Node)a, (Node)b) + " -> " + res);
    return res;
  }


  static Trie make_index(Object up) {
    return new RootTrie(up);
  }

  Object get_indexed(Object pattern, Trie index) {
    return index.get_trie(pattern);
  }

  void set_indexed(Object new_value, Object pattern, Trie index) {
    index.put_trie(pattern, new_value);
  }

  DbClass construct_dbclass(Object name) {
    DbClass instance = new DbClass(name, kb);
    instance.index = make_index(instance);
    return instance;
  }

  DbClass get_dbclass_from_hash_table(Object fact) {
    DbClass res = ((DbClass)dbclass_table.get(fact));
    if (res == null)
      { res = construct_dbclass(fact);
	dbclass_table.put(fact, res);
	return res;
      }
    else { return res; }
  }

  DbClass get_dbclass(Object fact) {
    if (fact == null) 
      { throw new GenericError
	   (Node._T, new OKBCString("null can't be a dbclass")).signal(); }
    else if (fact instanceof Cons) { return get_dbclass(((Cons)fact).car()); }
    else if (fact instanceof Number) 
      { return get_dbclass_from_hash_table(fact); }
    else if (fact instanceof AbstractSymbol)
      { if (variable_p(fact))
	  { throw new GenericError
	   (Node._T, new OKBCString 
	    ("Bad dbclass type: " + fact +
	     "  variables cannot be indexed.")).signal(); 
	  }
	else { return get_dbclass_from_hash_table(fact); }
      }
    else if (fact instanceof DbClass) { return (DbClass)fact; }
    else { return get_dbclass_from_hash_table(fact); }
  }

  public PeekResult peek_for_insert(Object fact,
				    boolean check_included_kbs_p) {
    return peek_for_insert(fact, new Vector(), check_included_kbs_p);
  }

  public PeekResult peek_for_insert(Object fact, Vector visited_kbs, 
				    boolean check_included_kbs_p) {
    if (check_included_kbs_p)
      { visited_kbs.addElement(this);
	Enumeration e = parent_kbs().elements();
	for (; e.hasMoreElements() ;) {
	  TupleStore prev = (TupleStore)
	    ((TupleStore)((AbstractFactStoreKb)(e.nextElement())).fact_store);
	  if (!visited_kbs.contains(prev))
	    { PeekResult res = prev.peek_for_insert(fact, visited_kbs,
						    check_included_kbs_p);
	      if (res.found_p) { return res; }
	    }
	}
      }
    DbClass cl = get_dbclass(fact);
    TrieResult tr = cl.index.get_trie_returning_node(fact, null);
    return new PeekResult(tr.result, tr.found_p, tr.trie_node, cl);
  }

  protected static boolean null_is_in_tree_p (Object x) {
    if (x instanceof Cons)
      { Cons c = (Cons) x;
	if (null_is_in_tree_p(c.car())) { return true; }
	else if (c.cdr() instanceof Cons) { return null_is_in_tree_p(c.cdr());}
	else { return false; }
      }
    else { return x == null; }
  }
       
  static Vector get_all_atoms (Object fact) {
    Vector all = new Vector();
    get_all_atoms_1 (fact, all);
    return all;
  }

  static void get_all_atoms_1 (Object fact, Vector all) {
    if (fact == null) {}
    else if (fact instanceof Cons) 
      { Cons c = (Cons) fact;
	get_all_atoms_1(c.car(), all);
	get_all_atoms_1(c.cdr(), all);
      }
    else if (fact instanceof AbstractSymbol)
      { if (!variable_p(fact) && !all.contains(fact)) { all.addElement(fact);}}
    else if (!all.contains(fact))
      { all.addElement(fact); }
  }

  boolean is_full_indexed_p(Object hash_key) {
    if (hash_key == null)
      { throw new GenericError
	   (Node._T, new OKBCString ("Null hash key found.")).signal();
      }
    Vector visited_kbs = new Vector();
    return is_full_indexed_p_1(hash_key, visited_kbs);
  }

  boolean is_full_indexed_p_1(Object hash_key, Vector visited_kbs) {
    visited_kbs.addElement(this);
    Object res = null;
    if (hash_index != null)
      {
	TrieResult tr =
	  atom_to_sentence_index.get_trie_returning_node(hash_key);
	if (tr.found_p) { res = tr.result; }
	else { res = null; }
      }
    else { res = full_index.get(hash_key); }
    
    if (!(res == null)) { return true; }
    else {Enumeration e = parent_kbs().elements();
	  for (; e.hasMoreElements() ;) {
	    TupleStore prev = 
	      (TupleStore)
	      (((AbstractFactStoreKb)(e.nextElement())).fact_store);
	    if (!visited_kbs.contains(prev))
	      { boolean b = prev.is_full_indexed_p_1(hash_key, visited_kbs);
		if (b) { return b; }
	      }
	  }
	}
    return false;
  }

  public void forcibly_full_index_fact (Object fact) {
    // Remove duplicates has already been done!
    if (full_index_p)
      { Vector all_atoms = get_all_atoms(fact);
	Enumeration e = all_atoms.elements();
	for (; e.hasMoreElements() ;) {
	  Object o = e.nextElement();
	  if (o == null)
	    { throw new GenericError
	      (Node._T, new OKBCString ("Null hash key found.")).signal();
	    }
	  Object entry = full_index.get(o);
	  if (entry != null)
	    { Cons c = (Cons) entry;
	      c.set_car(new Cons(fact, c.car())); 
	    }
	  else { full_index.put(o, new Cons(new Cons(fact))); }
	}
      }
  }

  public Node facts(Node atom) {
    if (full_index == null) { maybe_post_hoc_full_index_kb(); }
    return facts_full_indexed_under(atom);
  }

  public Cons facts_full_indexed_under (Object hash_key) {
    return facts_full_indexed_under(hash_key, false);
  }
    
  public Cons facts_full_indexed_under (Object hash_key, boolean uniquify_p) {
    Cons res = facts_full_indexed_under_internal(hash_key);
    if (uniquify_p) { return remove_duplicates_using_trie_star(res); }
    else return Cons.append_all(res);
  }
  
  static Cons remove_duplicates_using_trie_star(Cons list_of_lists) {
    if (list_of_lists == null) { return null; }
    RootTrie trie = new RootTrie("REMOVE-DUPLICATES");
    Cons result = null;
    Enumeration e = list_of_lists.elements();
    for (; e.hasMoreElements() ;) {
      Cons c = (Cons)(e.nextElement());
      Enumeration f = c.elements();
      for (; f.hasMoreElements() ;) {
	Object o = f.nextElement();
	TrieResult tr = trie.get_trie_returning_node(o);
	if (!tr.found_p) { result = new Cons(o, result); }
      }
    }
    return result;
  }

  public Cons facts_locally_full_indexed_under (Object hash_key) {
    if (hash_index == null)
      {
	if (variable_p(hash_key))
	  { Node result = Node._NIL; 
	    Enumeration e = full_index.elements();
	    while (e.hasMoreElements()) {
	      Node index_entry = (Node)(e.nextElement());
	      result = Cons.append_cons(index_entry.car(), result);
	    }
	    if (result == Node._NIL) { return null; }
	    else { return (Cons)result; }
	  }
	else
	  { Cons index_entry = (Cons)(full_index.get(hash_key));
	    if (index_entry == null) { return null; }
	    else { return (Cons)(index_entry.car()); }
	  }
      }
    else
      {
	if (variable_p(hash_key))
	  {
	    InnerTupleStoreMapper inner_map = new InnerTupleStoreMapper();
	    TrieMapFunction outer_map = new OuterTupleStoreMapper(inner_map);
	    atom_to_sentence_index.map_trie_nodes(outer_map);
	    return inner_map.get_results();
	  }
	else
	  {
	    InnerTupleStoreMapper simple_map = new InnerTupleStoreMapper();
	    TrieResult resnode =
	      atom_to_sentence_index.get_trie_returning_node(hash_key);
	    if (resnode.found_p)
	      {
		((Trie)(resnode.result)).map_trie_nodes(simple_map);
		return simple_map.get_results();
	      }
	    else { return null; }
	  }
      }
  }

  Cons facts_full_indexed_under_internal(Object hash_key) {
    Vector visited_kbs = new Vector ();
    return facts_full_indexed_under_internal(hash_key, visited_kbs);
  }

  Cons facts_full_indexed_under_internal(Object hash_key, Vector visited_kbs) {
    Cons local = facts_locally_full_indexed_under(hash_key);
    visited_kbs.addElement(this);
    Node non_local = null;
    Enumeration e = parent_kbs().elements();
    for (; e.hasMoreElements() ;) {
      TupleStore prev = (TupleStore)
	(((AbstractFactStoreKb)(e.nextElement())).fact_store);
      if (!visited_kbs.contains(prev))
	{ non_local = Cons.append_cons(prev.facts_full_indexed_under_internal
				       (hash_key, visited_kbs),
				       non_local);
	}
    }
    return new Cons(local, non_local);
  }

  public void insert(Object fact) {
    insert(fact, false, true);
  }

  public Node hash_index_peek_for_insert(Integer key, Node fact,
					       boolean check_included_kbs_p) {
    return hash_index_peek_for_insert
      (key, fact, new Vector(), check_included_kbs_p);
  }

  public Node hash_index_peek_for_insert(Integer key, Node fact,
					       Vector visited_kbs, 
					       boolean check_included_kbs_p) {
    if (check_included_kbs_p)
      { visited_kbs.addElement(this);
	Enumeration e = parent_kbs().elements();
	for (; e.hasMoreElements() ;) {
	  TupleStore prev = (TupleStore)
	    ((TupleStore)((AbstractFactStoreKb)(e.nextElement())).fact_store);
	  if (!visited_kbs.contains(prev))
	    { Node res = prev.hash_index_peek_for_insert
	      (key, fact, visited_kbs, check_included_kbs_p);
	      if (res != null && res != Node._NIL) { return res; }
	    }
	}
      }
    return hash_index.value_is_indexed_p(fact, key);
  }

  public InsertResult insert(Object fact, boolean containing_null_ok_p,
			     boolean check_included_kbs_p) {
    // System.out.println("Insert fact: " + fact);
    if (fact == null)
      { throw new GenericError
	   (Node._T, new OKBCString ("Can't assert null.")).signal(); 
      }
    if (!containing_null_ok_p && null_is_in_tree_p(fact))
      { throw new GenericError
	 (Node._T, 
	  new OKBCString ("Can't assert a fact containing null.")).signal();
      }
    if (hash_index == null)
      {
	PeekResult pr = peek_for_insert(fact, check_included_kbs_p);
	if (pr.found_p) { return new InsertResult(null, pr.result); }
	else { Cons new_struct = new Cons(fact, new Cons(null));
	       pr.trie_node.value = new_struct;
	       pr.dbclass.add_fact(fact);
	       forcibly_full_index_fact(fact);
	       register_side_effect("insert");
	       return new InsertResult(fact, new_struct);
	     }
      }
    else
      {
	Node fact_node = (Node)fact;
	Node existing = hash_index_peek_for_insert
	  (new Integer(HashIndex.pattern_hash_key(fact_node)),
	   fact_node, check_included_kbs_p);
	if (existing != null && existing != Node._NIL)
	  { return new InsertResult(null, existing); }
	else
	  {
	    hash_index.put_in_table(fact_node, fact_node);
	    Vector atoms = get_all_atoms(fact);
	    Enumeration e = atoms.elements();
	    while (e.hasMoreElements()) {
	      Node atom = (Node)(e.nextElement());
	      TrieResult tr =
		atom_to_sentence_index.get_trie_returning_node(atom, null);
	      if (!tr.found_p)
		{ tr.trie_node.value = new RootTrie(this); }
	      Trie sentences_for_atom = (Trie)tr.trie_node.value;
	      sentences_for_atom.put_trie(fact_node, fact_node);
	    }
	    register_side_effect("insert");
	    return new InsertResult(fact_node, fact_node);
	  }
      }
  }

  public boolean is_indexed_in_re_p (Object atom) {
    if (full_index == null) { maybe_post_hoc_full_index_kb(); }
    return is_full_indexed_p(atom);
  }

  public void ensure_full_index() {
    if (full_index == null) { full_index = new Hashtable(); }
  }

  public void maybe_post_hoc_full_index_kb() {
    Vector visited_kbs = new Vector();
    maybe_post_hoc_full_index_kb(visited_kbs);
  }

  void maybe_post_hoc_full_index_kb(Vector visited_kbs) {
    if (hash_index != null) {}
    else
      {
	if (!full_index_p)
	  { full_index_p = true;
	    ensure_full_index();
	    Enumeration e = dbclass_table.elements();
	    for (; e.hasMoreElements() ;) {
	      DbClass dbclass = (DbClass)(e.nextElement());
	      if (dbclass.facts != null)
		{ Enumeration f = dbclass.facts.elements();
		  for (; f.hasMoreElements() ;) {
		    Object fact = f.nextElement();
		    forcibly_full_index_fact(fact);
		  }
		}
	    }
	  }
	visited_kbs.addElement(this);
	Enumeration e = parent_kbs().elements();
	for (; e.hasMoreElements() ;) {
	  TupleStore prev = (TupleStore)
	    (((AbstractFactStoreKb)(e.nextElement())).fact_store);
	  if (!visited_kbs.contains(prev))
	    { prev.maybe_post_hoc_full_index_kb(visited_kbs); }
	}
      }
  }

  boolean drop_fact_from_hash_index (Node fact) {
    Node deleted_p = hash_index.delete_from_table(fact, fact);
    if (deleted_p!= Node._NIL)
      {
	// System.out.println("Dropped fact " + fact);
	Vector atoms = get_all_atoms(fact);
	Enumeration e = atoms.elements();
	while (e.hasMoreElements()) {
	  Node atom = (Node)(e.nextElement());
	  TrieResult tr =
	    atom_to_sentence_index.get_trie_returning_node(atom, null);
	  if (tr.found_p)
	    { ((Trie)tr.result).delete_trie(fact); }
	}

	/*
	Enumerator e3 = fact.enumerate_list();
	while(e3.has_more_p()) {
	  Node facts = facts_full_indexed_under(e3.next());
	  if (facts != null)
	    { 
	      Enumerator e2 = facts.enumerate_list(facts);
	      while(e2.has_more_p()) {
		Node fact2 = e2.next();
		Node matches =hash_index.get_matches(fact2);
		if (matches == null || matches == Node._NIL)
		  { throw new RuntimeException("Missing fact: " + fact2); }
	      }}}

	*/
	return true;
      }
    else { return false; }
  }
  
  protected Object drop_asserted_fact(Object fact) {
    if (fact == null)
      { throw new GenericError
	   (Node._T, new OKBCString ("Can't drop null.")).signal();
      }
    if (hash_index != null)
      {
	boolean dropped_p = drop_fact_from_hash_index((Node)fact);
	if (dropped_p)
	  { register_side_effect("drop_asserted_fact");
	    return "FACT-REMOVED";
	  }
	else { return Node._NIL; }
      }
    else
      {
	DbClass dbclass = get_dbclass(fact);
	Cons struct = (Cons)(get_indexed(fact, dbclass.index));
	Object unique_fact = struct.car();
	// System.out.println("Unique-fact = " + unique_fact);
	if (unique_fact == null) { return "FACT-NOT-KNOWN"; }
	else { dbclass.index.delete_trie(unique_fact);
	       if (dbclass.facts != null)
		 { Node trimmed = Cons.remove_eql(unique_fact, dbclass.facts,
						  null);
		   if (trimmed == Node._NIL || trimmed == null)
		     { dbclass.facts = null; }
		   else { dbclass.facts = (Cons)trimmed; }
		   // System.out.println("Unique-fact = " + unique_fact + " removed from " + dbclass);
		 }
	       if (full_index_p)
		 { Vector atoms = get_all_atoms(fact);
		   Enumeration e = atoms.elements();
		   for (; e.hasMoreElements() ;) {
		     Object atom = e.nextElement();
		     Cons entry = (Cons)(full_index.get(atom));
		     if (entry != null)
		       { entry.set_car
			   (Cons.remove_eql(unique_fact, (Cons)entry.car(),
					    null));
			 // System.out.println("Unique-fact = " + unique_fact + " removed from index " + atom);
		       }
		   }
		 }
	       register_side_effect("drop_asserted_fact");
	       return "FACT-REMOVED";
	     }
      }
  }

  public void drop
       (Object fact, boolean exact_p, boolean check_included_kbs_p) {
    drop(fact, exact_p, check_included_kbs_p, 
	 (check_included_kbs_p ? new Vector() : null));
  }

  public void drop(Object fact, boolean exact_p, boolean check_included_kbs_p,
		   Vector visited_kbs) {
    if (fact == null)
      { throw new GenericError
	   (Node._T, new OKBCString ("Can't drop null.")).signal();
      }
    Cons fact_as_cons = (Cons)fact;
    if (exact_p)
      { Node matches = (Node)(fetch(fact_as_cons, false).firstValue());
	if (Cons.member_equal(fact, matches, null, null))
	  { Object res = drop_asserted_fact(fact);
	    Node matches_now = (Node)(fetch(fact_as_cons, false).firstValue());
	    if (matches_now instanceof Cons &&
		Cons.member_equal(fact, matches_now, null, null))
	      { throw new GenericError
		  (Node._T, new OKBCString
		   ("Drop of fact " + fact + " failed.  Drop result was: " +
		    res + ".  Matches now are: " +
		    matches_now)).signal(); 
	      }
	  }
	else { throw new GenericError
	       (Node._T, new OKBCString
		("Fact " + fact + " not found.")).signal();
	     }
      }
    else       
      { Node matches = (Node)(fetch(fact_as_cons, false).firstValue());
	Enumeration e = matches.elements();
	for (; e.hasMoreElements() ;) {
	  Object match = e.nextElement();
	  drop_asserted_fact(match);
	}
	Node matches_now = (Node)(fetch(fact_as_cons, false).firstValue());
	if (!(matches_now != null || matches_now != Node._NIL))
	  { throw new GenericError
	   (Node._T, new OKBCString
	    ("Fact " + fact + " not successfully droppped.  Current matches: "
	     + matches_now + ".  Old matches were: " + matches)).signal();
	  }
      }
    if (check_included_kbs_p)
      { visited_kbs.addElement(this);
	Enumeration e = parent_kbs().elements();
	for (; e.hasMoreElements() ;) {
	  TupleStore prev = (TupleStore)
	    (((AbstractFactStoreKb)(e.nextElement())).fact_store);
	  if (!visited_kbs.contains(prev))
	    { prev.drop(fact, exact_p, check_included_kbs_p, visited_kbs); }
	}
      }
  }

  protected boolean bad_bindings_p (Object bindings) {
    if (bindings == Node.__fail) { return true; }
    else if (bindings == null) { return false; } // no bindings sought.
    else { Enumeration e = ((Cons)bindings).elements();
	   for (; e.hasMoreElements() ;) {
	     Cons binding = (Cons)(e.nextElement());
	     if (binding.cdr() == null) { return true; }
	   }
	   return false;
	}
  }

  public Cons get_candidates(Object pattern) {
    DbClass dbclass = get_dbclass(pattern); 
    return dbclass.facts;
  }

  Object get_some_non_variable_internal
      (Object tree, NumberLocation hint_locn) {
    if (tree instanceof Cons)
      { Cons c = (Cons)tree;
	Object car_result = get_some_non_variable_internal(c.car(), hint_locn);
	if (car_result == null)
	  { return get_some_non_variable_internal (c.cdr(), hint_locn); }
	else { return car_result; }
      }
    else if (tree == null) { return null; }
    else if (tree instanceof AbstractSymbol)
      { if (variable_p(tree)) { return null; }
	else if (hint_locn.number <= 0) { return tree; }
	else { hint_locn.number = hint_locn.number - 1;
	       return null;
	     }
      }
    else if (hint_locn.number <= 0) { return tree; }
    else { hint_locn.number = hint_locn.number - 1;
	   return null;
	 }
  }
      
  Object get_some_non_variable(Object tree, int index_hint) {
    NumberLocation hint_locn = new NumberLocation(index_hint);
    Object res = get_some_non_variable_internal (tree, hint_locn);
    if (!(hint_locn.number == 0))
      { throw new GenericError
       (Node._T, 
	new OKBCString ("Index hint " + index_hint + " too high")).signal();
      }
    return res;
  }

  public Node get_candidates_maybe_with_full_index(Object pattern) {
    return get_candidates_maybe_with_full_index(pattern, 0);
  }

  public Node get_candidates_maybe_with_full_index
       (Object pattern, int index_hint) {
    if (hash_index != null)
      { return hash_index.get_matches((Node)pattern); }
    else if (full_index_p)
      { if (pattern == null) 
	  { throw new GenericError
	    (Node._T, new OKBCString ("Cannot have a null pattern")).signal();
	  }
	else if (pattern instanceof Cons)
	  { Object key = get_some_non_variable(pattern, index_hint);
	    Cons pat = (Cons) pattern;
	    if (key == null)
	      // do it the hard way 
	      { 
		Enumeration e = dbclass_table.elements();
		Cons candidates = null;
		int length = pat.length();
		for (; e.hasMoreElements() ;) {
		  DbClass dbclass = (DbClass)(e.nextElement());
		  Node facts = dbclass.facts;
		  if (facts != null)
		    {
		      Enumeration f = facts.elements();
		      for (; f.hasMoreElements() ;) {
			Cons fact = (Cons)(f.nextElement());
			if (fact.length() == length)
			  { candidates = new Cons(fact, candidates); }
		      }
		    }
		}
		return candidates;
	      }
	    else 
	      { return get_candidates_maybe_with_full_index(key, index_hint); }
	  }
	else 
	  {
	    Node res = (Node)full_index.get(pattern);
	    if (res == null || res == Node._NIL) { return null; }
	    else { return (Node)(res.car()); }
	  }
      }
    else { return get_candidates(pattern); }
  }

  Cons all_values_from_trie(Trie tr) {
    ConsLocation cl = new ConsLocation();
    all_values_from_trie_1(tr, cl, null);
    Node n = cl.cons;
    if (n == Node._NIL) { return null; }
    else { return (Cons)n; }
  }
  
  void all_values_from_trie_1(Trie tr, ConsLocation cl, Trie parent) {
    if (parent != null && tr.value != Trie.__deleted__)
      { cl.addElement(tr.value); }
    if (tr.trie_hashed_p())
      { Enumeration e = ((Hashtable)(tr.arcs)).elements();
	for (; e.hasMoreElements() ;) {
	  Cons arc = (Cons)(e.nextElement());
	  // System.out.println("Arc = " + arc);
	  all_values_from_trie_1((Trie)(arc.cdr()), cl, tr);
	}
      }
    else
      { Cons arcs = (Cons)tr.arcs;
	if (arcs != null) 
	  { if (arcs.cdr() == null)
	      { Trie trie = (Trie)(((Cons)(arcs.car())).cdr());
		all_values_from_trie_1(trie, cl, tr); }
	    else { Enumeration e = arcs.elements();
		   for (; e.hasMoreElements() ;) {
		     Cons arc = (Cons)(e.nextElement());
		     all_values_from_trie_1((Trie)(arc.cdr()), cl, tr);
		   }
		 }
	  }
      }
  }

  public Values2 fetch(Cons pattern, boolean check_included_kbs_p) {
    return fetch(pattern, check_included_kbs_p, pattern, 0, 10000000, null);
  }

  public Values2 fetch(Cons pattern, boolean check_included_kbs_p, 
		      TupleStoreTester tester) {
    return fetch(pattern, check_included_kbs_p, pattern, 0, 10000000, tester);
  }

  public Values2 fetch(Cons pattern, boolean check_included_kbs_p,
		      Node return_pattern, int index_hint, int max_matches,
		      TupleStoreTester tester) {
    // System.out.println("Enter fetch: " + pattern);
    Vector visited_kbs = new Vector();
    Trie table = new RootTrie("UNIQUIFY");
    NumberLocation number_to_find_loc = new NumberLocation(max_matches);
    Cons res = fetch_internal(pattern, check_included_kbs_p, return_pattern,
			      index_hint, table, visited_kbs,
			      number_to_find_loc, tester);
    Node status;
    if (number_to_find_loc.number <= 0) { status = Node._T; }
    else { status = Node._NIL; }
    if (check_included_kbs_p)
      { Cons vals = all_values_from_trie(table);
      // System.out.println("Exit fetch for: " + pattern + " with " + vals);
	if (vals == null)
	  { return new Values2(Node._NIL, status); }
	else { return new Values2(vals, status); }
      }
    else { if (res == null)
	     { return new Values2(Node._NIL, status); }
	   else { return new Values2(res, status); }
	 }
  }

  protected Cons fetch_local(Cons pattern, boolean check_included_kbs_p, 
			     Node return_pattern, int index_hint,
			     Trie uniquifying_table,
			     NumberLocation number_to_find_loc,
			     TupleStoreTester tester) {
    // test not implemented.
    Cons results = null;
    Node candidates = get_candidates_maybe_with_full_index(pattern,index_hint);
    if (candidates == null) { return null; }
    Enumeration e = candidates.elements();
    for (; e.hasMoreElements() ;) {
      Object candidate = e.nextElement();
      if (number_to_find_loc.number <= 0) { break; }
      Object bindings = Node.__fail;
      if (tester == null)
	{ bindings = unify(pattern, candidate); }
      else { Object res = tester.test(pattern, (Node)candidate, kb);
	     if (res != Node.__fail)
	       { bindings = unify(pattern, candidate); }
//	     Object unify_bindings = unify(pattern, candidate);
//	     if (!(bindings.equals(unify_bindings)))
//	       { System.out.println("Pattern:" + pattern);
//		 System.out.println("Candidate:" + candidate);
//		 System.out.println("Bindings:" + bindings);
//		 System.out.println("UnifyBindings:" + unify_bindings);
//		 throw new RuntimeException("Fix me!"); 
//	       }
	   }
      boolean bad_bindings_p = bad_bindings_p(bindings);
      if (!bad_bindings_p)
	{ if (check_included_kbs_p)
	    { Node key = return_pattern;
	      if (bindings != null)
		{ key = Cons.sublis(bindings, return_pattern, null); }
	      // ^ bindings changed to key.  JPR
	      TrieResult tr = uniquifying_table.get_trie_returning_node
		(key);
	      if (!tr.found_p)
		{ tr.trie_node.value = key;
		  number_to_find_loc.number = number_to_find_loc.number - 1;
// System.out.println("Found:" + key + ":" + return_pattern + ":" + bindings);
		}
	    }
	  else 
	    { Node key = return_pattern;
	      if (bindings != null)
		{ key = Cons.sublis(bindings, return_pattern, null); }
	      // ^ bindings changed to key.  JPR
	      number_to_find_loc.number = number_to_find_loc.number - 1;
	      results = new Cons(key, results);
	    }
	}
    }
    // System.out.println("Local results = " + results);
    return results;
  }

  Cons fetch_internal(Cons pattern, boolean check_included_kbs_p,
		      Node return_pattern, int index_hint, Trie table,
		      Vector visited_kbs, NumberLocation number_to_find_loc,
		      TupleStoreTester tester) {
    Cons local_result = fetch_local(pattern, check_included_kbs_p,
				    return_pattern, index_hint,
				    table, number_to_find_loc, tester);
    if (check_included_kbs_p)
      { visited_kbs.addElement(this);
	Enumeration e = parent_kbs().elements();
	Cons result = local_result;
	for (; e.hasMoreElements() ;) {
	  TupleStore prev = (TupleStore)
	    (((AbstractFactStoreKb)(e.nextElement())).fact_store);
	// System.out.println("Previous kb is: " + prev + ", this = " + this);
	  if (!visited_kbs.contains(prev))
	    { Cons previous_result = 
	      prev.fetch_internal(pattern, check_included_kbs_p,
				  return_pattern, index_hint, table,
				  visited_kbs, number_to_find_loc, tester);
	      if (result == null)
		{ result = previous_result; }
	      else { result = result.union_equal(previous_result, null, null); }
	    }
	}
	return result;
      }
    else { return local_result; }
  }

  public Node fetch_one_binding (Cons pattern) {
    Vector visited_kbs = new Vector();
    return fetch_one_binding(pattern, true, 0, visited_kbs, null);
  }

  public Node fetch_one_binding (Cons pattern, boolean check_included_kbs_p) {
    Vector visited_kbs = (check_included_kbs_p ? new Vector() : null);
    return fetch_one_binding(pattern, check_included_kbs_p, 0,
			     visited_kbs, null);
  }

  public Node fetch_one_binding (Cons pattern, boolean check_included_kbs_p,
				 int index_hint) {
    Vector visited_kbs = (check_included_kbs_p ? new Vector() : null);
    return fetch_one_binding(pattern, check_included_kbs_p, index_hint,
			     visited_kbs, null);
  }

  public Node fetch_one_binding (Cons pattern, TupleStoreTester tester) {
    Vector visited_kbs = new Vector();
    return fetch_one_binding(pattern, true, 0, visited_kbs, tester);
  }

  public Node fetch_one_binding (Cons pattern, boolean check_included_kbs_p, 
				 TupleStoreTester tester) {
    Vector visited_kbs = (check_included_kbs_p ? new Vector() : null);
    return fetch_one_binding(pattern, check_included_kbs_p, 0,
			     visited_kbs, tester);
  }

  public Node fetch_one_binding (Cons pattern, boolean check_included_kbs_p,
				 int index_hint, TupleStoreTester tester) {
    Vector visited_kbs = (check_included_kbs_p ? new Vector() : null);
    return fetch_one_binding(pattern, check_included_kbs_p, index_hint,
			     visited_kbs, tester);
  }

  public Node fetch_one_binding (Cons pattern, boolean check_included_kbs_p,
				 int index_hint, Vector visited_kbs,
				 TupleStoreTester tester) {
    Node local_result = Node.__fail;
    Node result = local_result;
    Node candidates = get_candidates_maybe_with_full_index(pattern,index_hint);
    if (candidates != null)
      { Enumeration e = candidates.elements();
	for (; e.hasMoreElements() ;) {
	  Cons candidate = (Cons)(e.nextElement());
	  Object bindings = Node.__fail;
	  
	  if (tester == null)
	    { bindings = unify(pattern, candidate); }
	  else { Object res = tester.test(pattern, (Node)candidate, kb);
		 if (res != Node.__fail)
		   { bindings = unify(pattern, candidate); }
//		 Object unify_bindings = unify(pattern, candidate);
//		 if (!(bindings == null && unify_bindings == null))
//		   { if ((bindings == null && unify_bindings != null) ||
//			 (bindings != null && unify_bindings == null) ||
//			 (!(bindings.equals(unify_bindings))))
//			 { System.out.println("Pattern:" + pattern);
//			   System.out.println("Candidate:" + candidate);
//			   System.out.println("Bindings:" + bindings);
//			   System.out.println("UnifyBindings:" + 
//					      unify_bindings);
//			   throw new RuntimeException("Fix me!"); 
//			 }
//		     }
	       }

	  // System.out.println("Bindings are: " + bindings);
	  if (!(bad_bindings_p(bindings)))
	    { if (bindings == null) { local_result = Node._NIL; }
	      else { local_result = (Node)bindings; }
	      break;
	    }
	}
      }
    // System.out.println("fetch_one_binding " + Cons.list(pattern, tester));
    // System.out.println("Local result is: " + local_result);
    if (check_included_kbs_p && local_result == Node.__fail)
      { visited_kbs.addElement(this);
	Enumeration f = parent_kbs().elements();
	for (; f.hasMoreElements() ;) {
	  TupleStore prev = (TupleStore)
	    (((AbstractFactStoreKb)(f.nextElement())).fact_store);
	  if (!visited_kbs.contains(prev))
	    { Node res = prev.fetch_one_binding(pattern, check_included_kbs_p,
						index_hint, visited_kbs, 
						tester);
	      if (res != Node.__fail) { result = res; }
	    }
	}
	result = local_result;
      }
    result = local_result;
    // System.out.println("Exit fetch-one-binding for: " + pattern + " with " + result);
    return result;
  }


}


class OuterTupleStoreMapper extends TrieMapFunction {
  TrieMapFunction inner_mapper;

  OuterTupleStoreMapper (TrieMapFunction inner) { inner_mapper = inner; }

  public boolean funcall (Trie node) {
    Trie t = (Trie)(node.value); // node contains a trie
    if (t != null)
      { t.map_trie_nodes(inner_mapper); }
    return true;
  }
}


class InnerTupleStoreMapper extends TrieMapFunction {

  Cons result = null;
  Trie uniquifier = new RootTrie(this);

  Cons get_results () {
    return result;
  }

  public boolean funcall (Trie node) {
    Node form = (Node)(node.value);
    TrieResult resnode = uniquifier.get_trie_returning_node(form);
    if (!resnode.found_p)
      { resnode.trie_node.value = form;
	result = new Cons(form, result);
      }
    return true;
  }
}
