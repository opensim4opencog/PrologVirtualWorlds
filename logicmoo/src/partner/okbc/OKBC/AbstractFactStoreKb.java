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

public class AbstractFactStoreKb extends CachingKb {
  public Vector parent_kbs = new Vector();
  protected AbstractFactStore fact_store;

  public AbstractFactStore get_fact_store () { return fact_store; }

  public void include (AbstractFactStoreKb kb) {
    if (!(parent_kbs.contains(kb)))
      { register_side_effect("include");
	parent_kbs.addElement(kb); 
      }
  }

  public void uninclude (AbstractFactStoreKb kb) {
    register_side_effect("uninclude");
    parent_kbs.removeElement(kb);
  }

  static public void encode_node
      (Node node, PrintWriter stream, boolean client_p) {
    node.encode(stream, client_p);
  }

}
