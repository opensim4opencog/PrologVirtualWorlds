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

public class Enumerator extends Node { 

  static final int typeCode = ENUMERATOR;
  static Map threadGroupToEnumeratorTableMap = new HashMap(10);

  /**
    * Use {@link #getEnumeratorIdToEnumeratorTable()} instead.
    * @deprecated
    */
  static final Hashtable enumerator_id_to_enumerator_table = new Hashtable();
  static long enumerator_id_counter = 0;

  /**
    * Returns the appropriate Enumerator-ID-to-Enumerator hashtable based on the 
    * ThreadGroup that the Thread this is running in belongs to.
    */
  public static Hashtable getEnumeratorIdToEnumeratorTable() {
    Thread thisThread = Thread.currentThread();
    ThreadGroup thisThreadGroup = thisThread.getThreadGroup();
    Hashtable idToEnum 
      = (Hashtable)threadGroupToEnumeratorTableMap.get(thisThreadGroup);

    if (idToEnum == null) {
      idToEnum = new Hashtable();
      threadGroupToEnumeratorTableMap.put(thisThreadGroup, idToEnum);
    }
    return idToEnum;
  }

  OKBCInteger id = null;
  boolean freed = false;
  Node creating_args = null;

  static long allocate_enumerator_id () {
    enumerator_id_counter = enumerator_id_counter + 1;
    return enumerator_id_counter;
  }

  protected void encode(PrintWriter s, boolean client_p) {
    s.print(typeCode);
    newline(s);
    if (id == null)
      { long idnum = allocate_enumerator_id(); 
	id = new OKBCInteger(idnum);
      }
    Object existing = getEnumeratorIdToEnumeratorTable().get(id);
    if (existing == null)
      { getEnumeratorIdToEnumeratorTable().put(id, this); }
    id.encode(s, client_p);
  }

  public static Node internThing(Node the_id, Connection connection) {
    Enumerator existing 
      = (Enumerator)getEnumeratorIdToEnumeratorTable().get(the_id);
    if (existing == null)
      { if (connection == null)
	  { return _NIL; } // HACK ALERT!! Shouls issue a warning here!!
	else
	  { NetworkEnumerator instance = 
	      new NetworkEnumerator(the_id, connection);
	    getEnumeratorIdToEnumeratorTable().put(the_id, instance);
	    return instance;
	  }
      }
    else { return existing; }
  }

  public void free() {
    // System.out.println("Freeing enumerator " + this);
    if (freed) { throw new ObjectFreed(this).signal(); }
    if (id != null)
      { getEnumeratorIdToEnumeratorTable().remove(id); }
    freed = true;
  }

  public TruthValue has_more() {
    if (freed) { throw new ObjectFreed(this).signal(); }
    return _T;
  }

  public boolean has_more_p() {
    return has_more() != _NIL;
  }

  public void prefetch() {
    prefetch(_all, _NIL);
  }

  public void prefetch(Node number_of_values) {
    prefetch(number_of_values, _NIL);
  }

  public void prefetch(Node number_of_values, Node increment) {
    if (freed) { throw new ObjectFreed(this).signal(); }
    if (number_of_values == _all ||
	((number_of_values instanceof OKBCInteger) &&
	 (((OKBCInteger)number_of_values).number > 0)))
      {}
    else { throw new GenericError
	   (new OKBCString("Illegal number-of-values.  Number-Of-Valuess must "+
                          ":ALL, or a positive integer."),
	    null).signal();
	  }
    if (increment == _NIL || increment == _all ||
	((increment instanceof OKBCInteger) &&
	 (((OKBCInteger)increment).number > 0)))
      {}
    else { throw new GenericError
	   (new OKBCString("Illegal increment.  Increment must "+
                          "NIL, :ALL, or a positive integer."),
	    null).signal();
	  }
  }

  public Node fetch() {
    return fetch(_all);
  }

  public Node fetch(Node number_of_values) {
    if (freed) { throw new ObjectFreed(this).signal(); }
    if (number_of_values == _all ||
	((number_of_values instanceof OKBCInteger) &&
	 (((OKBCInteger)number_of_values).number > 0)))
      {}
    else { throw new GenericError
	   (new OKBCString("Illegal number-of-values.  Number-Of-Valuess must "+
                          ":ALL, or a positive integer."),
	    null).signal();
	  }
    return _NIL;
  }

  public Node next() {
    if (freed) { throw new ObjectFreed(this).signal(); }
    return _NIL;
  }

  public Node fetch_list_of_at_most(Node number_of_values) {
    Cons root = null;
    Cons tail = null;
    for(int i=0; i < ((OKBCInteger)number_of_values).number; i++) {
      if (has_more_p())
	{ Node value = next();
	  if (tail == null)
	    { root = new Cons(value);
	      tail = root;
	    }
	  else
	    { Cons new_tail = new Cons(value);
	      tail.set_cdr(new_tail);
	      tail = new_tail;
	    }
	}
      else { break; }
    }
    return root;
  }

  public String toString() {
    return "#<" + getClass().getName()
                + " ID= " + id + " "
		+ (creating_args != null
		   ? " args=" + creating_args.toString()
		   : "")
		+ pointerForToString() + ">";
  }

}
