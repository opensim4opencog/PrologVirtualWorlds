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


public abstract class AbstractKeyValueWalker {

  static final Symbol _okbc_slot_spec = Symbol.keyword("OKBC-SLOT-SPEC");
  static final Symbol _slot_specs = Symbol.keyword("SLOT-SPECS");

  Hashtable walker_table = new Hashtable();
  AbstractKifWalker kif_walker = null;

  public AbstractSubKeyValueWalker get (Node key) {
    AbstractSubKeyValueWalker res =
      (AbstractSubKeyValueWalker)(walker_table.get(key));
    if (res == null)
      { return get(Node._T); }
    else { return res; }
  }

  public void add_walker(Node key, AbstractSubKeyValueWalker walker) {
    walker_table.put(key, walker);
    walker.kif_walker = kif_walker;
    walker.set_key_and_value_walker(this);
  }

}  
