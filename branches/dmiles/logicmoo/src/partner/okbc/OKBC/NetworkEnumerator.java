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

public class NetworkEnumerator extends Enumerator {

  Node local_elements = _NIL;
  long number_of_local_elements = 0;
  boolean remote_exhausted_p = false;
  Connection connection = null;
  long last_prefetch_increment = -1;

  static final Symbol fetch_fname = Symbol.intern_in_ok_back("FETCH-INTERNAL");
  static final Symbol free_fname = Symbol.intern_in_ok_back("FREE-INTERNAL");
  static final Symbol has_more_fname = 
    Symbol.intern_in_ok_back("HAS-MORE-INTERNAL");
  static final Symbol next_fname = Symbol.intern_in_ok_back("NEXT-INTERNAL");
  static final Symbol prefetch_fname = 
    Symbol.intern_in_ok_back("PREFETCH-INTERNAL");


  NetworkEnumerator(Node the_id, Connection the_connection) {
    id = (OKBCInteger)the_id;
    connection = the_connection;
  }

  Node makeNetworkCall(Node fname) {
    Node args = Cons.list(fname, this);
    return ((Node)((NetworkConnection)connection)
	    .makeNetworkCall(args)).car();
  }

  Node makeNetworkCall(Node fname, Node arg0) {
    Node args = Cons.list(fname, this, arg0);
    return ((Node)((NetworkConnection)connection)
	    .makeNetworkCall(args)).car();
  }

  Node makeNetworkCall(Node fname, Node arg0, Node arg1) {
    Node args = Cons.list(fname, this, arg0, arg1);
    return ((Node)((NetworkConnection)connection)
	    .makeNetworkCall(args)).car();
  }

  public TruthValue has_more() {
    super.has_more();
    if (number_of_local_elements > 0)
      { return _T; }
    else
      { if (remote_exhausted_p) { return _NIL; }
	else { return remote_has_more(); }
      }
  }

  TruthValue remote_has_more() {
    Node result = makeNetworkCall(has_more_fname);
    if (result == _NIL)
      { free_remote_enumerator(); 
	remote_exhausted_p = true;
	return _NIL;
      }
    else { return _T; }
  }

  public void prefetch(Node number_of_values, Node increment) {
    super.prefetch(number_of_values, increment);
    if (number_of_values instanceof OKBCInteger)
      { last_prefetch_increment = ((OKBCInteger)number_of_values).number; }
    else { last_prefetch_increment = 0; }
    if (!remote_exhausted_p)
      { if ((number_of_values == _all) ||
	    (((OKBCInteger)number_of_values).number > number_of_local_elements))
	  { Node arg0 = _NIL;
	    if (increment == _all) { arg0 = _all; }
	    else if (increment == _NIL) { arg0 = number_of_values; }
	    else { arg0 = increment; }
	    if (remote_has_more() != _NIL)
	      { Node results = makeNetworkCall(fetch_fname, arg0);
		local_elements = Cons.append_cons(local_elements, results);
		number_of_local_elements = number_of_local_elements +
		  results.length();
		if (number_of_values == _all)
		  { free_remote_enumerator(); 
		    remote_exhausted_p = true;
		  }
	      }
	    // Else we already have all of the local elements, and the correct
	    // number recorded.
	    else {}
	  }
      }
  }
   
  void free_remote_enumerator() {
    makeNetworkCall(free_fname);
  }

  public void free() {
    if (!remote_exhausted_p) { free_remote_enumerator(); }
    remote_exhausted_p = true;
    connection = null;
    last_prefetch_increment = -1;
    super.free();
  }

  public Node next() {
    super.next();
    if (number_of_local_elements > 0)
      { number_of_local_elements = number_of_local_elements - 1;
	Node value = local_elements.car();
	local_elements = local_elements.cdr();
	return value;
      }
    else
      { if (local_elements == _NIL || local_elements == null)
	  { if (remote_exhausted_p)
	      { throw new EnumeratorExhausted(this).signal(); }
	    // We've snagged the number the user last asked for.
	    // We'll heuristically use this as a cue to tell us how
	    // many values to get this time.
	    if (last_prefetch_increment >= 0)
	      // Heuristic prefetch
	      { if (last_prefetch_increment == 0)
		  { prefetch(_all, _all); }
		else { OKBCInteger inc =
		         new OKBCInteger(last_prefetch_increment);
		       prefetch(_all, inc); 
		     }
		if (number_of_local_elements > 0)
		  // We know that we got a local value, so try again.
		  { return next(); }
		else { throw new EnumeratorExhausted(this).signal(); }
	      }
	    else { return makeNetworkCall(next_fname); }
	  }
	else
	  { throw new GenericError
	    (new OKBCString("Internal consistency error!  Value(s) found in " +
			   "network enumerator, but value count indicates " +
			   "that there should be none."),
	     _NIL).signal();
	  }
      }
  }

  public Node fetch(Node number_of_values) {
    super.fetch();
    if (number_of_values == _all)
      { if (remote_exhausted_p)
	  { if (number_of_local_elements > 0)
	      { number_of_local_elements = 0;
		Node res = local_elements;
		local_elements = _NIL;
		return res;
	      }
	    else { throw new EnumeratorExhausted(this).signal(); }
	  }
	else
	  { Node extra = makeNetworkCall(fetch_fname, number_of_values); 
	    Node existing = local_elements;
	    free_remote_enumerator();
	    remote_exhausted_p = true;
	    local_elements = _NIL;
	    number_of_local_elements = 0;
	    return Cons.append_cons(existing, extra);
	  }
      }
    else if (((OKBCInteger)number_of_values).number > number_of_local_elements)
      { Node arg0 = new OKBCInteger(((OKBCInteger)number_of_values).number -
				   number_of_local_elements);
	Node extra = makeNetworkCall(fetch_fname, arg0); 
	Node existing = local_elements;
	local_elements = _NIL;
	number_of_local_elements = 0;
	return Cons.append_cons(existing, extra);
      }
    else { return fetch_list_of_at_most(number_of_values); }
  }

}
