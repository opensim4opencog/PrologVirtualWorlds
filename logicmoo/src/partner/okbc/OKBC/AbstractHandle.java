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

public class AbstractHandle extends Node 
{
  static final UndefinedValue undefinedValue = UndefinedValue.undefinedValue;
  Node id;
  Node kb_id;
  Node thing = undefinedValue;

  AbstractHandle(Node the_id, Node the_kb_id) {
    id = the_id;
    kb_id = the_kb_id;
  }
  
  AbstractHandle(Node the_id, Node the_kb_id, Node the_thing) {
    id = the_id;
    kb_id = the_kb_id;
    thing = the_thing;
  }
  
  public String toString() {
    return
      "["
      + getClass().getName()
      + " " 
      + kb_id.toString()
      + ":" 
      + id.toString()
      + (thing == undefinedValue 
	 ? ""
	 : " Representing " + thing.toString())
      + " "
      + pointerForToString()
      + "]";
  }

}

