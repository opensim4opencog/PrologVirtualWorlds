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

public class ConsLocation {
  public Node cons = Node._NIL;
  
  public ConsLocation() {}
  public ConsLocation(Cons c) { cons = c; }
  public ConsLocation(Node c)
    { if (c == Node._NIL || c instanceof Cons)
	{ cons = c; }
      else if (c == null) { cons = Node._NIL; }
      else
	{ throw new RuntimeException("Illegal value for initialization: "+c); }
    }

  public void addElement(Object o) {
    cons = new Cons(o, cons);
  }

  public Node pop() {
    Node element = cons.car();
    cons = cons.cdr();
    return element;
  }

}


