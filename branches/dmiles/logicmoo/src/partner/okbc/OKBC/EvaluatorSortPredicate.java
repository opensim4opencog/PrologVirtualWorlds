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

class EvaluatorSortPredicate extends SortPredicate {
  Kb kb;

  EvaluatorSortPredicate (Kb the_kb) {
    kb = the_kb;
  }

  public boolean test (Node x, Node y) {
    if (x instanceof Cons)
      { return test(x.car(), y); }
    else if (y instanceof Cons)
      { return test(x, y.car()); }
    else if (x == Node._NIL)
      { if (y == Node._NIL) { return false; }
	else { return true; }
      }
    else if (x == Node._T)
      { if (y == Node._T) { return false; }
	else if (y == Node._NIL) { return false; }
	else { return true; }
      }
    else if (x instanceof OKBCInteger || x instanceof Real)
      { if (y == Node._NIL || y == Node._T) { return false; }
	else if (y instanceof OKBCInteger)
	  { return Node.generalised_less_than(x, y); }
	else if (y instanceof Real)
	  { return Node.generalised_less_than(x, y); }
	else { return true; }
      }
    else if (x instanceof OKBCString)
      { if (y == Node._NIL || y == Node._T) { return false; }
	else if (y instanceof OKBCInteger) { return false; }
	else if (y instanceof Real) { return false; }
	else if (y instanceof OKBCString)
	  { return ((OKBCString)x).string.compareTo(((OKBCString)y).string)<0;}
	else { return true; }
      }
    else if (x instanceof AbstractSymbol)
      { if (y == Node._NIL || y == Node._T) { return false; }
	else if (y instanceof OKBCInteger) { return false; }
	else if (y instanceof Real) { return false; }
	else if (y instanceof OKBCString) { return false; }
	else if (y instanceof AbstractSymbol)
	  { 
	    if (y instanceof Symbol)
	      {
		Symbol a = (Symbol)x;
		Symbol b = (Symbol)y;
		if (a.symbolPackage == b.symbolPackage)
		  { return a.symbolName.compareTo(b.symbolName) < 0; }
		else { return a.symbolPackage.name.compareTo
			 (b.symbolPackage.name)
			 < 0;
		}
	      }
	    else
	      {
		QuasiSymbol a = (QuasiSymbol)x;
		QuasiSymbol b = (QuasiSymbol)y;
		if (a.symbolPackage == b.symbolPackage)
		  { return a.symbolName.compareTo(b.symbolName) < 0; }
		else { return a.symbolPackage.name.compareTo
			 (b.symbolPackage.name)
			 < 0;
		}
	      }
	  }
	else { return true; }
      }
    else if (x instanceof Kb)
      { if (y == Node._NIL || y == Node._T) { return false; }
	else if (y instanceof OKBCInteger) { return false; }
	else if (y instanceof Real) { return false; }
	else if (y instanceof OKBCString) { return false; }
	else if (y instanceof AbstractSymbol) { return false; }
	else if (y instanceof Kb)
	  { return test(((Kb)x).kbname, ((Kb)y).kbname); }
	else { return true; }
      }
    else
      {
	Values2 xvals = kb.coerce_to_frame(x, Node._NIL, Node._NIL);
	Values2 yvals = kb.coerce_to_frame(y, Node._NIL, Node._NIL);
	if (xvals.secondValue() != Node._NIL) // xfound-p
	  {
	    if (yvals.secondValue() != Node._NIL) // yfound-p
	      {
		Node xname = kb.get_frame_pretty_name
		  (xvals.firstValue(), Node._NIL);
		Node yname = kb.get_frame_pretty_name
		  (yvals.firstValue(), Node._NIL);
		boolean result = test(xname, yname);
		/***
		System.out.println("xname = " + xname);
		System.out.println("yname = " + yname + " -> " + result);
		***/
		return result;
	      }
	    else { return true; }
	  }
	else if (yvals.secondValue() != Node._NIL) // yfound-p
	  { return false; }
	else
	  {
	    return test(kb.value_as_string
			(xvals.firstValue(),
			 Node._user_interface, Node._T,
			 Node._NIL).firstValue(),
			kb.value_as_string
			(yvals.firstValue(),
			 Node._user_interface, Node._T,
			 Node._NIL).firstValue());
	  }
      }
  }

  
}
