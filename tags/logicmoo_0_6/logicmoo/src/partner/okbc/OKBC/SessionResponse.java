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

public class SessionResponse {

  public String session_id;
  public String session_key;
  public String status;

  public SessionResponse() { }

  public SessionResponse
       (String theKey, String theSessionId, String theStatus) {
    session_key = theKey;
    session_id = theSessionId;
    status = theStatus;
  }

  public SessionResponse(Node spec) {
    session_key = ((OKBCString)spec.nth(0)).string;
    session_id = ((OKBCString)spec.nth(1)).string;
    status = ((OKBCString)spec.nth(2)).string;
  }

  public String toString() {
    return "SessionResponse(" + session_key + ", " + session_id + ", " +
           status + ")" ;
  }

}

