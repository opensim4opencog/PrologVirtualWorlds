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

public class ExhaustiveEnumerator extends Enumerator {

  ExhaustiveEnumerator() {}

  ExhaustiveEnumerator(Node list) {
    all_elements = list;
  }

  Node all_elements = _NIL;


  public TruthValue has_more() {
    super.has_more();
    if (all_elements == _NIL || all_elements == null)
      { return _NIL; }
    else { return _T; }
  }

  public void prefetch(Node number_of_values, Node increment) {
    super.prefetch(number_of_values, increment);
  }
   
  public void free() {
    all_elements = null;
    super.free();
  }

  public Node next() {
    if (has_more_p())
      { Node val = all_elements.car();
	all_elements = all_elements.cdr();
	return val;
      }
    else { throw new EnumeratorExhausted(this).signal(); }
  }

  public Node fetch(Node number_of_values) {
    super.fetch(number_of_values);
    if (has_more_p())
      { if (number_of_values == _all)
	  { Node values = all_elements;
	    all_elements = null;
	    return values;
	  }
	else { return fetch_list_of_at_most(number_of_values); }
      }
    else { throw new EnumeratorExhausted(this).signal(); }
  }

}
