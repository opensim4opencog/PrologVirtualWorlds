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

public class FrameHandle extends AbstractHandle 
{
  static final int typeCode = FRAMEHANDLE;
  static final Hashtable frame_handle_key_to_frame_handle_table
    = new Hashtable();
  static final Hashtable frame_object_to_frame_handle_table
    = new Hashtable();
  static int frame_handle_id_counter = 0;

  static int allocate_frame_handle_id () {
    frame_handle_id_counter = frame_handle_id_counter + 1;
    return frame_handle_id_counter;
  }

  static final OKBCInteger minus_one = new OKBCInteger(-1);

  public static FrameHandle find_or_create_frame_handle(Node thing, Kb kb) {
    FrameHandle existing = 
      (FrameHandle)frame_object_to_frame_handle_table.get(thing);
    if (existing != null) { return existing; }
    else if (thing instanceof Symbol &&
	     ((Symbol)thing).keywordp() &&
	     Cons.member_eql(thing, okbc_standard_names, null, _NIL))
      { FrameHandle handle = (FrameHandle)internThing(thing, minus_one);
	handle.thing = thing;
	frame_object_to_frame_handle_table.put(thing, handle);
	return handle;
      }
    else { int id = allocate_frame_handle_id();
	   Node kb_id = kb.unique_id();
	   FrameHandle handle = 
	     (FrameHandle)internThing(new OKBCInteger(id), kb_id);
	   handle.thing = thing;
	   frame_object_to_frame_handle_table.put(thing, handle);
	   return handle;
	 }
  }

  public static Node internThing(Node the_id, Node the_kb_id) {
    return internThing(the_id, the_kb_id, false);
  }

  public static Node internThing(Node the_id, Node the_kb_id, 
				 boolean client_p) {
    return internThing(the_id, the_kb_id, client_p, true);
  }

  public static Node internThing(Node the_id, Node the_kb_id, 
				 boolean client_p, 
				 boolean allow_mapping_to_objects_p)
  {
    Node key = Cons.list(the_id, the_kb_id);
    FrameHandle existing 
      = (FrameHandle)frame_handle_key_to_frame_handle_table.get(key);
    if (existing == null)
      { 
	FrameHandle instance = new FrameHandle(the_id, the_kb_id);
	frame_handle_key_to_frame_handle_table.put(key, instance);
//	System.out.println("    *** interning new frame handle: " + the_id + "|" + the_kb_id);
	return instance;
      }
    else if (!allow_mapping_to_objects_p ||
	     client_p ||
	     existing.thing == undefinedValue) 
      { return existing; }
    else { return existing.thing; }
  }

  public FrameHandle(Node the_id, Node the_kb_id) {
    super(the_id, the_kb_id);
  }

  protected void encode(PrintWriter s, boolean client_p) {
    s.print(typeCode);
    newline(s);
    id.encode(s, client_p);
    kb_id.encode(s, client_p);
  }

  public static FrameHandle create_frame_handle(Node for_thing, Kb kb) {
    int id = allocate_frame_handle_id();
    Node kb_id = kb.unique_id();
    FrameHandle handle = (FrameHandle)internThing(new OKBCInteger(id), kb_id);
    if (for_thing == _NIL)
      { handle.thing = undefinedValue; }
    frame_object_to_frame_handle_table.put(handle, handle);
    return handle;
  }

  public static boolean print_frame_names_with_handles_p = true;
  public static boolean force_pretty_toString_of_remote_frame_handles_p
     = false;

  public static boolean print_kb_ids_with_frame_names_p = true;
  public static boolean print_classnames_with_frame_names_p = true;
    
  public String toString() {
    if (print_frame_names_with_handles_p &&
	the_current_kb != null && 
	(!(the_current_kb instanceof MinimalNetworkKb) ||
	 force_pretty_toString_of_remote_frame_handles_p)) {
      
      if (the_current_kb.coercible_to_frame_p(this, _NIL) != _NIL) {
	Node name = the_current_kb.get_frame_pretty_name(this, _NIL);
	return
	  "["
	  + (print_classnames_with_frame_names_p ?
	     getClass().getName() : "")
	  + ((print_kb_ids_with_frame_names_p ||
	      (name.toString().equals(""))) ? 
	     ( (kb_id.equals(minus_one)) ? ""
	       : (" " + kb_id.toString() + ":" + id.toString() + " for "))
	     : "")
	  + name.toString()
	  + "]";
      } else if (thing == this || thing == undefinedValue) {
	return
	  "["
	  + getClass().getName()
	  + " " 
	  + kb_id.toString()
	  + ":" 
	  + id.toString()
	  + " "
	  + pointerForToString()
	  + "]";
      } else {
	return super.toString();
      }
    } else if (thing == this) {
      return
	"["
	+ getClass().getName()
	+ " " 
	+ kb_id.toString()
	+ ":" 
	+ id.toString()
	+ " "
	+ pointerForToString()
	+ "]";
    }
    else { return super.toString(); }
  }


  // Internal protoicol used by the code walker.
  public boolean frame_reference_p (Kb kb) {
    return true;
  }
}
