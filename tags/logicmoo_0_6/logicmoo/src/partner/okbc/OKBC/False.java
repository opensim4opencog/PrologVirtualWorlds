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

class FalseEnumeration implements Enumeration {

  FalseEnumeration() {}

  public Object nextElement() {
    throw new NoSuchElementException ("No more elements remaining in " + this);
  }

  public boolean hasMoreElements() {
    return false;
  }
}


public class False extends TruthValue {
  static final int typeCode =  FALSE;
  protected void encode(PrintWriter s, boolean client_p) {
    s.print(typeCode);
    newline(s);
  }
  public String toString() { return "()"; }

  public Node toList() {
    return _NIL;
  }

  public boolean equals(Object thing) {
    return (thing instanceof False);
  }

  public int length() {
    return 0;
  }

  public boolean isEmpty() {
    return true;
  }

  public Node nth(Integer n) {
    return _NIL;
  }

  public Node nth(int n) {
    return _NIL;
  }

  public void set_nth(int n, Node to) {
    throw new RuntimeException ("Illegal operation.  Not a list");
  }

  public Enumerator enumerate_list() {
    return new ExhaustiveEnumerator();
  }

  public Node car() {
    return _NIL;
  }

  public Node cdr() {
    return _NIL;
  }

  public Values toValues() {
    return new Values();
  }

  public Node member(Node thing) {
    return _NIL;
  }

  public Node member(Node thing, Node test) {
    return _NIL;
  }

  public Enumeration elements() {
    return new FalseEnumeration();
  }

}

