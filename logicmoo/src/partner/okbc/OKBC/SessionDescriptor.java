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

public class SessionDescriptor extends Node 
{
  public String session_id;
  public String group_ownership;
  public String session_description;
  
  public SessionDescriptor() {}
  
  public SessionDescriptor(Node theList) {
    session_id          = ((OKBCString)theList.nth(0)).string;
    Node group = theList.nth(1);
    if (group instanceof OKBCString)
      { group_ownership     = ((OKBCString)group).string; }
    else { group_ownership     = ((Symbol)group).symbolName; }
    session_description = ((OKBCString)theList.nth(2)).string;
  }
  
  public SessionDescriptor(String theSession_id,
			   String theGroup_ownership,
			   String theSession_description) {
    session_id  = theSession_id;
    group_ownership  = theGroup_ownership;
    session_description  = theSession_description;
  }
  
  public String toString() {
    if (session_description == null || session_description.equals(""))
      { return "[Session " + session_id + " (" + group_ownership + ")]"; }
    else { return "Session " + session_id + " - " + session_description; }
  }
}

