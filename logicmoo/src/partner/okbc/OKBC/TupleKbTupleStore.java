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

public class TupleKbTupleStore extends TupleStore {
  public static final TupleStoreTester          v = new          v_Tester();
  public static final TupleStoreTester        f_V = new        f_V_Tester();
  public static final TupleStoreTester   _Sf_V2_V = new   _Sf_V2_V_Tester();
  public static final TupleStoreTester   _Sf_V2_S = new   _Sf_V2_S_Tester();
  public static final TupleStoreTester _Fa_Sf_V2_S = new _Fa_Sf_V2_S_Tester();
  public static final TupleStoreTester r_Fa_Sf_V2_S =new r_Fa_Sf_V2_S_Tester();
  public static final TupleStoreTester fa_Sf_V2_V = new fa_Sf_V2_V_Tester();
  public static final TupleStoreTester fa_Sf_V2_S = new fa_Sf_V2_S_Tester();
  public static final TupleStoreTester       sf_V = new       sf_V_Tester();
  public static final TupleStoreTester     fasf_V = new     fasf_V_Tester();
  public static final TupleStoreTester    rfasf_V = new    rfasf_V_Tester();
  public static final TupleStoreTester  _Fasf_V2_V = new  _Fasf_V2_V_Tester();
  public static final TupleStoreTester r_Fasf_V2_V = new r_Fasf_V2_V_Tester();
  public static final TupleStoreTester  _Fasf_V2_Fa =new  _Fasf_V2_Fa_Tester();
  public static final TupleStoreTester r_Fasf_V2_Fa =new r_Fasf_V2_Fa_Tester();
  public static final TupleStoreTester _V = new _V_Tester();
  public static final TupleStoreTester _F_V2_F = new _F_V2_F_Tester();
  public static final TupleStoreTester _F_V2_V = new _F_V2_V_Tester();
  public static final TupleStoreTester _S_F_V2_V = new _S_F_V2_V_Tester();
  public static final TupleStoreTester  s_F_V2_F = new  s_F_V2_F_Tester();
  public static final TupleStoreTester fa_S_F_V2_V = new fa_S_F_V2_V_Tester();
  public static final TupleStoreTester rfa_S_F_V2_V =new rfa_S_F_V2_V_Tester();
  public static final TupleStoreTester  s_F_V2_V = new  s_F_V2_V_Tester();
  public static final TupleStoreTester  _Fas_F_V2_V =new  _Fas_F_V2_V_Tester();
  public static final TupleStoreTester r_Fas_F_V2_V =new r_Fas_F_V2_V_Tester();
  public static final TupleStoreTester  fas_F_V2_V = new  fas_F_V2_V_Tester();
  public static final TupleStoreTester  _Fv = new  _Fv_Tester();
  public static final TupleStoreTester s_Fv = new s_Fv_Tester();
  public static final TupleStoreTester fas_Fv = new fas_Fv_Tester();
  public static final TupleStoreTester rfas_Fv = new rfas_Fv_Tester();
  public static final TupleStoreTester _S_Fv2_F = new _S_Fv2_F_Tester();
  public static final TupleStoreTester fa_S_Fv2_F = new fa_S_Fv2_F_Tester();
  public static final TupleStoreTester _S_Fv2_S = new _S_Fv2_S_Tester();
  public static final TupleStoreTester     fv = new     fv_Tester();
  public static final TupleStoreTester    sfv = new    sfv_Tester();
  public static final TupleStoreTester  fasfv = new  fasfv_Tester();
  public static final TupleStoreTester rfasfv = new rfasfv_Tester();
  public static final TupleStoreTester   _Sfv = new   _Sfv_Tester();
  public static final TupleStoreTester fa_Sfv = new fa_Sfv_Tester();


  public TupleKbTupleStore(AbstractFactStoreKb the_kb, boolean hash_index_p) {
    super(the_kb, hash_index_p);
  }


  static void install(TupleStoreTester tester, Node pattern,
		      Node return_pattern, Vector v) {
    Vector elt = new Vector();
    elt.addElement(tester);
    elt.addElement(pattern);
    elt.addElement(return_pattern);
    v.addElement(elt);
  }

  
  // Test harness.
  public static void main (String arg[]) {

    Symbol _frame_var = Symbol.intern("?FRAME", "VARS");
    Symbol _class_var = Symbol.intern("?CLASS", "VARS");
    Symbol _facet_var = Symbol.intern("?FACET", "VARS");
    Symbol _instance_var = Symbol.intern("?INSTANCE", "VARS");
    Symbol _slot_var = Symbol.intern("?SLOT", "VARS");
    Symbol _sub_var = Symbol.intern("?SUB", "VARS");
    Symbol _super_var = Symbol.intern("?SUPER", "VARS");
    Symbol _value_var = Symbol.intern("?VALUE", "VARS");
    Symbol __default_template_facet_value
    = Symbol.keyword("DEFAULT-TEMPLATE-FACET-VALUE");
    Symbol __template_facet_value = Symbol.keyword("TEMPLATE-FACET-VALUE");
    Symbol __default_template_slot_value
    = Symbol.keyword("DEFAULT-TEMPLATE-SLOT-VALUE");
    Symbol __template_slot_value = Symbol.keyword("TEMPLATE-SLOT-VALUE");
    Symbol __default_template_facet_of
    = Symbol.keyword("DEFAULT-TEMPLATE-FACET-OF");
    Symbol __template_facet_of = Symbol.keyword("TEMPLATE-FACET-OF");
    Symbol __default_template_slot_of
    = Symbol.keyword("DEFAULT-TEMPLATE-SLOT-OF");
    Symbol __template_slot_of = Symbol.keyword("TEMPLATE-SLOT-OF");
    Symbol __default_facet_of = Symbol.keyword("DEFAULT-FACET-OF");
    Symbol __facet_of = Symbol.keyword("FACET-OF");
    Symbol __default_slot_of = Symbol.keyword("DEFAULT-SLOT-OF");
    Symbol __slot_of = Symbol.keyword("SLOT-OF");
    Symbol __subclass_of = Symbol.keyword("SUBCLASS-OF");
    Symbol __superclass_of = Symbol.keyword("SUPERCLASS-OF");
    Symbol __instance_of = Symbol.keyword("INSTANCE-OF");
    Symbol __type_of = Symbol.keyword("TYPE-OF");
    Symbol __individual = Symbol.keyword("INDIVIDUAL");
    Symbol __class = Symbol.keyword("CLASS");
    Symbol __facet = Symbol.keyword("FACET");
    Symbol __slot = Symbol.keyword("SLOT");
    Symbol __primitive = Symbol.keyword("PRIMITIVE");
    Symbol __name = Symbol.keyword("NAME");
    Symbol __pretty_name = Symbol.keyword("PRETTY-NAME");
    Symbol __handle = Symbol.keyword("HANDLE");

    Node _a = Symbol.intern("A", "OKBC");
    Node _b = Symbol.intern("B", "OKBC");
    Node _c = Symbol.intern("C", "OKBC");
    Node _d = Symbol.intern("D", "OKBC");
    Node _e = Symbol.intern("E", "OKBC");
    Node _v1 = new OKBCInteger(42);
    Node _v = new OKBCInteger(42);
    Node facts = Cons.list(_a,
			   _v1,
			   Cons.list(_v1),
			   Cons.list(_a, _v1),
			   Cons.list(_a, _b, _v1),
			   Cons.list(_a, _b, _c, _v1),
			   Cons.list(__template_slot_value, _a, _b, _v1),
			   Cons.list(__template_facet_value, _a, _b, _c, _v1),
			   Cons.list(__subclass_of, _a, _b),
			   Cons.list(__class, _a));
    Vector testers = new Vector();
    install(v, Cons.list(_a), Node._NIL, testers);
    install(v, Cons.list(_v), Node._NIL, testers);
    install(_V, Cons.list(_value_var), _value_var, testers);
    install(fv, Cons.list(_a, _v), Node._NIL, testers);
    install(_Fv, Cons.list(_frame_var, _v), _frame_var, testers);
    install(f_V, Cons.list(_a, _value_var), _value_var, testers);
    install(_F_V2_F, Cons.list(_frame_var, _value_var), _frame_var, testers);
    install(_F_V2_V, Cons.list(_frame_var, _value_var), _value_var, testers);
    install(sfv, Cons.list(_a, _b, _v), Node._NIL, testers);
    install(_Sfv, Cons.list(_slot_var, _b, _v), _slot_var, testers);
    install(_S_Fv2_F, Cons.list(_slot_var, _frame_var, _v), _frame_var,
	    testers);
    install(_S_Fv2_S, Cons.list(_slot_var, _frame_var, _v), _slot_var,
	    testers);
    install(_Sf_V2_V, Cons.list(_slot_var, _b, _value_var), _value_var,
	    testers);
    install(_Sf_V2_S, Cons.list(_slot_var, _b, _value_var), _slot_var,
	    testers);
    install(_S_F_V2_V, Cons.list(_slot_var, _frame_var, _value_var),
	    _value_var, testers);
    install(sf_V, Cons.list(_a, _b, _value_var), _value_var, testers);
    install(s_Fv, Cons.list(_a, _frame_var, _v), _frame_var, testers);
    install(fas_Fv, Cons.list(_a, _b, _frame_var, _v), _frame_var, testers);
    install(rfas_Fv, Cons.list(_a, _b, _c, _frame_var, _v), _frame_var,
	    testers);
    install(s_F_V2_F, Cons.list(_a, _frame_var, _value_var), _frame_var,
	    testers);
    install(s_F_V2_V, Cons.list(_a, _frame_var, _value_var), _value_var,
	    testers);
    install(fasfv, Cons.list(_a, _b, _c, _v), Node._NIL, testers);
    install(rfasfv, Cons.list(__template_slot_value, _a, _b, _c, _v),
	    Node._NIL, testers);
    install(fa_S_F_V2_V, Cons.list(_a, _slot_var, _frame_var, _value_var),
	    _value_var, testers);
    install(fa_S_Fv2_F, Cons.list(_a, _slot_var, _frame_var, _v), _frame_var,
	    testers);
    install(fa_Sf_V2_V, Cons.list(_a, _slot_var, _c, _value_var), _value_var,
	    testers);
    install(fa_Sf_V2_S, Cons.list(_a, _slot_var, _c, _value_var), _slot_var,
	    testers);
    install(fas_F_V2_V, Cons.list(_a, _b, _frame_var, _value_var), _value_var,
	    testers);
    install(fa_Sfv, Cons.list(_a, _slot_var, _c, _v), _slot_var, testers);
    install(fasf_V, Cons.list(__template_slot_value, _a, _b, _value_var),
	    _value_var, testers);
    install(fasf_V, Cons.list(_a, _b, _c, _value_var), _value_var, testers);
    install(_Fasf_V2_V, Cons.list(_facet_var, _b, _c, _value_var), _value_var,
	    testers);
    install(_Fasf_V2_Fa, Cons.list(_facet_var, _b, _c, _value_var), _facet_var,
	    testers);
    install(_Fas_F_V2_V, Cons.list(_facet_var, _b, _frame_var, _value_var),
	    _value_var, testers);
    install(_Fa_Sf_V2_S, Cons.list(_facet_var, _slot_var, _c, _value_var),
	    _slot_var, testers);
    install(rfasf_V, Cons.list(__template_facet_value, _a, _b, _c, _value_var),
	    _value_var, testers);
    install(rfa_S_F_V2_V, Cons.list(__template_facet_value, _a, _slot_var,
				    _frame_var, _value_var),
	    _value_var, testers);
    install(r_Fa_Sf_V2_S, Cons.list(__template_facet_value, _facet_var,
				    _slot_var, _c, _value_var),
	    _slot_var, testers);
    install(r_Fas_F_V2_V, Cons.list(__template_facet_value, _facet_var, _b,
				    _frame_var, _value_var),
	    _value_var, testers);
    install(r_Fasf_V2_Fa, Cons.list(__template_facet_value, _facet_var, _b, _c,
				    _value_var),
	    _facet_var, testers);
    install(r_Fasf_V2_V, Cons.list(__template_facet_value, _facet_var, _b, _c,
				   _value_var),
	    _value_var, testers);
    install(r_Fasf_V2_V, Cons.list(__template_facet_value, _facet_var, _b, _c,
				   _value_var),
	    _value_var, testers);
  
    System.out.println();
    Enumeration e = facts.elements();
    Kb kb = null;
    for (; e.hasMoreElements() ;) {
      Node fact = (Node)(e.nextElement());
      Enumeration f = testers.elements();
      while (f.hasMoreElements()) {
	Vector spec = (Vector)(f.nextElement());
	TupleStoreTester tester = (TupleStoreTester)spec.elementAt(0);
	Node pattern = (Node)spec.elementAt(1);
	Node reply_pattern = (Node)spec.elementAt(2);
	Object result = tester.test(pattern, fact, kb);
	Object unify_result = unify(pattern, fact);
	if ((result == Node.__fail && unify_result != Node.__fail) ||
	    (result != Node.__fail && unify_result == Node.__fail))
	  { System.out.println();
	    System.out.println(tester.getClass().getName() + ": " + 
			     fact + " -> " + result);
	    System.out.println("  Should be: " + unify_result);
	  }
	else if (result == Node._NIL && unify_result == null)
	  { System.out.print("."); }
	else if (result != Node.__fail && unify_result != Node.__fail)
	  { Node substituted = Cons.sublis((Node)unify_result, reply_pattern,
					   null);
	    if (substituted.equals(result)) {}
	    else { System.out.println(tester.getClass().getName() + ": " + 
				      fact + " -> " + result);
		   System.out.println("  Should be: " + unify_result);
		   System.out.println("Fact = " + fact);
		   System.out.println("Spec = " + spec);
		   System.out.println("tester = " + tester);
		   System.out.println("result = " + result);
		   System.out.println("unify-result = " + unify_result);
		   System.out.println("reply-pattern = " + reply_pattern);
		   System.out.println("subst = " + substituted);
		 }
	  }
	else { System.out.print("."); }
      }
    }
    System.out.println();
  }

}


class v_Tester extends TupleStoreTester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_value = pattern.car(); // get value var
	Node candidate_value = test_structure.car(); // get value
	// NOTE:  Values are tested using .equals, frame handles are tested
	// using ==
       if (pattern_value.equals(candidate_value))
	 {
	   test_structure = test_structure.cdr(); // pop value
	   if (test_structure == null || test_structure == _NIL)
	     { return Node._NIL; }
	 }
     }
     return Node.__fail;
  }
}

class f_V_Tester extends _V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_frame = pattern.car(); // get frame
	Node candidate_frame = test_structure.car(); // get frame
       if (frames_match_p(pattern_frame, candidate_frame, kb))
	 { return super.test(pattern.cdr(), test_structure.cdr(), kb); }
     }
     return Node.__fail;
  }
}

class _Sf_V2_V_Tester extends f_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { return super.test(pattern.cdr(), test_structure.cdr(), kb); }
     return Node.__fail;
  }
}

class _Sf_V2_S_Tester extends f_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_slot = pattern.car(); // get slot
	Node candidate_slot = test_structure.car(); // get slot
	if (super.test(pattern.cdr(), test_structure.cdr(), kb) != Node.__fail)
	  { return candidate_slot; }
     }
     return Node.__fail;
  }
}

class fa_Sf_V2_S_Tester extends _Sf_V2_S_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_facet = pattern.car(); // get facet
	Node candidate_facet = test_structure.car(); // get facet
       if (frames_match_p(pattern_facet, candidate_facet, kb))
	 { return super.test(pattern.cdr(), test_structure.cdr(), kb); }
     }
     return Node.__fail;
  }
}

class _Fa_Sf_V2_S_Tester extends _Sf_V2_S_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node candidate_facet = test_structure.car(); // get facet
	return super.test(pattern.cdr(), test_structure.cdr(), kb);
     }
     return Node.__fail;
  }
}

class r_Fa_Sf_V2_S_Tester extends _Fa_Sf_V2_S_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_relation = pattern.car(); // get relation
	Node candidate_relation = test_structure.car(); // get relation
       if (frames_match_p(pattern_relation, candidate_relation, kb))
	 { return super.test(pattern.cdr(), test_structure.cdr(), kb); }
     }
     return Node.__fail;
  }
}

class fa_Sf_V2_V_Tester extends _Sf_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_facet = pattern.car(); // get facet
	Node candidate_facet = test_structure.car(); // get facet
       if (frames_match_p(pattern_facet, candidate_facet, kb))
	 { return super.test(pattern.cdr(), test_structure.cdr(), kb); }
     }
     return Node.__fail;
  }
}

class sf_V_Tester extends TupleStoreTester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node pattern_slot = pattern.car();
       pattern = pattern.cdr(); // pop off slot
       Node candidate_slot = test_structure.car();
       test_structure = test_structure.cdr(); // pop off slot
       if (frames_match_p(pattern_slot, candidate_slot, kb) &&
	   test_structure != null)
         { Node pattern_frame = pattern.car();
           pattern = pattern.cdr(); // pop off frame
           Node candidate_frame = test_structure.car();
           test_structure = test_structure.cdr(); // pop off frame
           if (frames_match_p(pattern_frame, candidate_frame, kb))
             { if (test_structure != null && test_structure != Node._NIL)
                 { Node tail = test_structure.cdr(); // pop off value
		   if (tail == null || tail == Node._NIL)
		     { return test_structure.car(); }
		 }
             }
         }
     }
     return Node.__fail;
  }
}

class _Fasf_V2_Fa_Tester extends sf_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_facet = test_structure.car();
       pattern = pattern.cdr(); // pop
       test_structure = test_structure.cdr(); // pop
       if (super.test(pattern, test_structure, kb) != Node.__fail)
	 { return candidate_facet; }
     }
     return Node.__fail;
  }
}

class r_Fasf_V2_Fa_Tester extends _Fasf_V2_Fa_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node pattern_relation = pattern.car();
       Node candidate_relation = test_structure.car();
       if (frames_match_p(pattern_relation, candidate_relation, kb))
	 { 
	   pattern = pattern.cdr(); // pop
	   test_structure = test_structure.cdr(); // pop
	   return super.test(pattern, test_structure, kb);
         }
     }
     return Node.__fail;
  }
}

class fasf_V_Tester extends sf_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node pattern_facet = pattern.car();
       Node candidate_facet = test_structure.car();
       if (frames_match_p(pattern_facet, candidate_facet, kb))
	 { 
	   pattern = pattern.cdr(); // pop
	   test_structure = test_structure.cdr(); // pop
	   return super.test(pattern, test_structure, kb);
         }
     }
     return Node.__fail;
  }
}

class rfasf_V_Tester extends fasf_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_relation = test_structure.car();
       Node pattern_relation = pattern.car();
       if (frames_match_p(candidate_relation, pattern_relation, kb))
	 {
	   pattern = pattern.cdr(); // pop relation
	   test_structure = test_structure.cdr(); // pop relation
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}

class _Fasf_V2_V_Tester extends sf_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { pattern = pattern.cdr(); // pop facet
       test_structure = test_structure.cdr(); // pop facet
       return super.test(pattern, test_structure, kb);
     }
     return Node.__fail;
  }
}

class r_Fasf_V2_V_Tester extends _Fasf_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_relation = test_structure.car();
       Node pattern_relation = pattern.car();
       if (frames_match_p(candidate_relation, pattern_relation, kb))
	 {
	   pattern = pattern.cdr(); // pop relation
	   test_structure = test_structure.cdr(); // pop relation
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}

class _V_Tester extends TupleStoreTester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node candidate_value = test_structure.car();
	test_structure = test_structure.cdr(); // pop value
	if (test_structure == null || test_structure == Node._NIL)
	  { return candidate_value; }
     }
     return Node.__fail;
  }
}

class _F_V2_V_Tester extends _V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { test_structure = test_structure.cdr(); // pop frame
	pattern = pattern.cdr(); // pop frame var
	if (test_structure != null && test_structure != Node._NIL)
	  { return super.test(pattern, test_structure, kb); }
     }
     return Node.__fail;
  }
}

class s_F_V2_V_Tester extends _F_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_slot = pattern.car(); // get slot var
	Node candidate_slot = test_structure.car(); // get slot
       if (frames_match_p(pattern_slot, candidate_slot, kb))
	 {
	   test_structure = test_structure.cdr(); // pop slot
	   pattern = pattern.cdr(); // pop slot
	   if (test_structure != null)
	     { return super.test(pattern, test_structure, kb); }
	 }
     }
     return Node.__fail;
  }
}

class s_F_V2_F_Tester extends _F_V2_F_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_slot = pattern.car(); // get slot var
	Node candidate_slot = test_structure.car(); // get slot
       if (frames_match_p(pattern_slot, candidate_slot, kb))
	 {
	   test_structure = test_structure.cdr(); // pop slot
	   pattern = pattern.cdr(); // pop slot
	   if (test_structure != null)
	     { return super.test(pattern, test_structure, kb); }
	 }
     }
     return Node.__fail;
  }
}

class _Fas_F_V2_V_Tester extends s_F_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { test_structure = test_structure.cdr(); // pop slot
	pattern = pattern.cdr(); // pop slot
	if (test_structure != null)
	  { return super.test(pattern, test_structure, kb); }
     }
     return Node.__fail;
  }
}

class r_Fas_F_V2_V_Tester extends _Fas_F_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_relation = pattern.car(); // get relation
	Node candidate_relation = test_structure.car(); // get relation
       if (frames_match_p(pattern_relation, candidate_relation, kb))
	 { return super.test(pattern.cdr(), test_structure.cdr(), kb); }
     }
     return Node.__fail;
  }
}

class fas_F_V2_V_Tester extends s_F_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_facet = pattern.car(); // get facet var
	Node candidate_facet = test_structure.car(); // get facet
       if (frames_match_p(pattern_facet, candidate_facet, kb))
	 {
	   test_structure = test_structure.cdr(); // pop facet
	   pattern = pattern.cdr(); // pop facet
	   if (test_structure != null)
	     { return super.test(pattern, test_structure, kb); }
	 }
     }
     return Node.__fail;
  }
}

class _S_F_V2_V_Tester extends _F_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { test_structure = test_structure.cdr(); // pop slot
	pattern = pattern.cdr(); // pop slot var
	if (test_structure != null && test_structure != Node._NIL)
	  { return super.test(pattern, test_structure, kb); }
     }
     return Node.__fail;
  }
}

class fa_S_F_V2_V_Tester extends _S_F_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_facet = pattern.car(); // get facet var
	Node candidate_facet = test_structure.car(); // get facet
       if (frames_match_p(pattern_facet, candidate_facet, kb))
	 {
	   test_structure = test_structure.cdr(); // pop facet
	   pattern = pattern.cdr(); // pop facet
	   if (test_structure != null)
	     { return super.test(pattern, test_structure, kb); }
	 }
     }
     return Node.__fail;
  }
}

class rfa_S_F_V2_V_Tester extends fa_S_F_V2_V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_relation = pattern.car(); // get relation var
	Node candidate_relation = test_structure.car(); // get relation
       if (frames_match_p(pattern_relation, candidate_relation, kb))
	 {
	   test_structure = test_structure.cdr(); // pop relation
	   pattern = pattern.cdr(); // pop relation
	   if (test_structure != null)
	     { return super.test(pattern, test_structure, kb); }
	 }
     }
     return Node.__fail;
  }
}

class _F_V2_F_Tester extends _V_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node candidate_frame = test_structure.car();
	test_structure = test_structure.cdr(); // pop frame
	pattern = pattern.cdr(); // pop frame var
	if (test_structure != null && test_structure != Node._NIL)
	  { if (super.test(pattern, test_structure, kb) != Node.__fail)
	      { return candidate_frame; }
	  }
     }
     return Node.__fail;
  }
}

class _Fv_Tester extends v_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node candidate_frame = test_structure.car(); // get frame
	test_structure = test_structure.cdr(); // pop frame 
	pattern = pattern.cdr(); // pop frame var
	if (test_structure != null && test_structure != Node._NIL)
	  { if (super.test(pattern, test_structure, kb) != Node.__fail)
	      { return candidate_frame; }
	  }
     }
     return Node.__fail;
  }
}

class s_Fv_Tester extends _Fv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node candidate_slot = test_structure.car(); // get slot
	Node pattern_slot = pattern.car();
       if (frames_match_p(pattern_slot, candidate_slot, kb))
	 { return super.test(pattern.cdr(), test_structure.cdr(), kb); }
     }
     return Node.__fail;
  }
}

class fas_Fv_Tester extends s_Fv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_facet = test_structure.car();
       Node pattern_facet = pattern.car();
       if (frames_match_p(candidate_facet, pattern_facet, kb))
	 {
	   pattern = pattern.cdr(); // pop facet
	   test_structure = test_structure.cdr(); // pop facet
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}

class rfas_Fv_Tester extends fas_Fv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_relation = test_structure.car();
       Node pattern_relation = pattern.car();
       if (frames_match_p(candidate_relation, pattern_relation, kb))
	 {
	   pattern = pattern.cdr(); // pop relation
	   test_structure = test_structure.cdr(); // pop relation
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}

class _S_Fv2_F_Tester extends _Fv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node candidate_frame = test_structure.car(); // get frame
	test_structure = test_structure.cdr(); // pop frame 
	pattern = pattern.cdr(); // pop frame var
	if (test_structure != null && test_structure != Node._NIL)
	  { return super.test(pattern, test_structure, kb); }
     }
     return Node.__fail;
  }
}

class fa_S_Fv2_F_Tester extends _S_Fv2_F_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node pattern_relation = pattern.car(); // get relation var
	Node candidate_relation = test_structure.car(); // get relation
       if (frames_match_p(pattern_relation, candidate_relation, kb))
	 {
	   test_structure = test_structure.cdr(); // pop relation
	   pattern = pattern.cdr(); // pop relation var
	   if (test_structure != null)
	     { return super.test(pattern, test_structure, kb); }
	 }
     }
     return Node.__fail;
  }
}

class _S_Fv2_S_Tester extends _Fv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
      { Node candidate_slot = test_structure.car(); // get slot
	test_structure = test_structure.cdr(); // pop slot 
	pattern = pattern.cdr(); // pop slot var
	if (test_structure != null && test_structure != Node._NIL)
	  { if (super.test(pattern, test_structure, kb) != Node.__fail)
	      { return candidate_slot; }
	  }
     }
     return Node.__fail;
  }
}

class fv_Tester extends v_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_frame = test_structure.car();
       Node pattern_frame = pattern.car();
       if (frames_match_p(candidate_frame, pattern_frame, kb))
	 {
	   pattern = pattern.cdr(); // pop frame
	   test_structure = test_structure.cdr(); // pop frame
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}

class sfv_Tester extends fv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_slot = test_structure.car();
       Node pattern_slot = pattern.car();
       if (frames_match_p(candidate_slot, pattern_slot, kb))
	 {
	   pattern = pattern.cdr(); // pop slot
	   test_structure = test_structure.cdr(); // pop slot
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}

class fasfv_Tester extends sfv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_key = test_structure.car();
       Node pattern_key = pattern.car();
       if (frames_match_p(candidate_key, pattern_key, kb))
	 {
	   pattern = pattern.cdr(); // pop key
	   test_structure = test_structure.cdr(); // pop key
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}

class rfasfv_Tester extends fasfv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_key = test_structure.car();
       Node pattern_key = pattern.car();
       if (frames_match_p(candidate_key, pattern_key, kb))
	 {
	   pattern = pattern.cdr(); // pop key
	   test_structure = test_structure.cdr(); // pop key
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}

class _Sfv_Tester extends fv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { pattern = pattern.cdr(); // pop slot var
       Node candidate_slot = test_structure.car();
       test_structure = test_structure.cdr(); // pop slot
       if (test_structure != null && test_structure != _NIL)
	 { Node res = super.test(pattern, test_structure, kb);
	   if (res != Node.__fail)
	     { return candidate_slot; }
	 }
     }
     return Node.__fail;
  }
}

class fa_Sfv_Tester extends _Sfv_Tester {
  public Node test (Node pattern, Node test_structure, Kb kb) {
    if (test_structure instanceof Cons)
     { Node candidate_facet = test_structure.car();
       Node pattern_facet = pattern.car();
       if (frames_match_p(candidate_facet, pattern_facet, kb))
	 {
	   pattern = pattern.cdr(); // pop facet
	   test_structure = test_structure.cdr(); // pop facet
	   return super.test(pattern, test_structure, kb);
	 }
     }
     return Node.__fail;
  }
}


