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

package demos;
import java.util.*;
import java.io.*;
import OKBC.*;


public class WalkerTest extends Walker {

  public static void main (String argv[]) {
    TupleKb kb = new TupleKb();
    TestWalkFunction fn = new TestWalkFunction();
    //    Node expression = LispReader.readFromString("(=> (p ?x) (r (s ?x) ?y))");
    Node expression = LispReader.readFromString("(define-okbc-frame foo :pretty-name \"Hello\" :type :class :own-slots ((s1 v1)) :template-facets ((s2 (f1 v2) (f2 (:default v3)))) :direct-types (c1) :sentences ((=> (p ?x) (q ?x))) :handle 42)");
    WalkerTest walker = new WalkerTest();
    if (OKBCApp.argv_suppliedp("TRACE", argv)) 
      { walker.trace_p = true; }
    walker.symbols_ok_as_non_logical_constants_p = true;
    Node result = walker.walk(expression, fn, kb,
			      //			      AbstractSubExpressionWalker._sentence);
			      _define_okbc_frame);
    System.out.println();
    System.out.print("Result = ");
    Cons.pretty_print(result, 9);
    System.out.println();
  }
}


class TestWalkFunction extends AbstractWalkFunction {
  public Node funcall(Kb kb, MostAbstractSubWalker production,
		      Node known_context_type, Node bound_vars,
		      Node expression) {
    return expression;
  }
}
