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

public class RemoteValue extends AbstractHandle 
{
  static final int typeCode = REMOTEVALUE;
  static final Hashtable remote_value_key_to_remote_value_table
  = new Hashtable();
  static final Hashtable remote_value_to_remote_value_key_table
  = new Hashtable();
  static int remote_value_id_counter = 0;

  static int allocate_remote_value_id () {
    remote_value_id_counter = remote_value_id_counter + 1;
    return remote_value_id_counter;
  }

  public static Node intern_remote_value(Node thing, Node kb) {
    Object key = remote_value_to_remote_value_key_table.get(thing);
    if (key != null)
      // We have already allocated a remote value for this thing.
      { RemoteValue rv =
	   (RemoteValue)remote_value_key_to_remote_value_table.get(key);
	if (rv == null)
	  { throw new GenericError
	    (_T, new OKBCString("Cannot find remote value for key: " + key))
	    .signal();
	  }
	return rv;
      }
    else
      {
	OKBCInteger the_id = new OKBCInteger(allocate_remote_value_id());
	Node kb_id;
	if (kb == null || kb == _NIL)
	  { kb_id = new OKBCInteger(-1); }
	else { kb_id = ((Kb)kb).unique_id; }
	Node new_key = Cons.list(the_id, kb_id);
	remote_value_to_remote_value_key_table.put(thing, new_key);
	RemoteValue rv = (RemoteValue)internThing(the_id, kb_id);
	rv.thing = thing;
	return rv;
      }
  }
  
  public static Node internThing(Node the_id, Node the_kb_id) {
    Node key = Cons.list(the_id, the_kb_id);
    RemoteValue existing 
      = (RemoteValue)remote_value_key_to_remote_value_table.get(key);
    if (existing == null)
      { 
	RemoteValue instance = new RemoteValue(the_id, the_kb_id);
	remote_value_key_to_remote_value_table.put(key, instance);
	return instance;
      }
    else if (existing.thing == undefinedValue) { return existing; }
    else { return existing.thing; }
  }

  RemoteValue(Node the_id, Node the_kb_id) {
    super(the_id, the_kb_id);
  }
  
  protected void encode(PrintWriter s, boolean client_p) {
    s.print(typeCode);
    newline(s);
    id.encode(s, client_p);
    kb_id.encode(s, client_p);
  }
  
}

