/***********************************************************************

  OKBC Implemementation for Java

  Copyright (c) 1996, 1997 Stanford University KSL.

  Original code by James Rice (rice@ksl.stanford.edu)


  This code was automatically generated on Tuesday, 21 December 1999.

  KSL Network Services available at:
            http://ontolingua.stanford.edu/


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

class MinimalNetworkKbLocator extends AbstractKbLocator {

  MinimalNetworkKbLocator(Node the_name) {
    name = the_name;
  }
}


public class MinimalNetworkKb extends CachingKb {

  public MinimalNetworkKb() {}

  static Symbol minimal_network_okbc_kb_abstract_name = null;
  static MinimalNetworkKb minimal_network_okbc_kb_type = null;

  public static void ensure_registered () {
    if (minimal_network_okbc_kb_abstract_name == null)
      { minimal_network_okbc_kb_abstract_name = (Symbol)
	(register_concrete_kb_type
	  (Symbol.intern_in_ok_back("ABSTRACT-MINIMAL-NETWORK-OKBC-KB-KB"),
	   get_kb_type_internal()));
      }
  }

  public static Kb get_kb_type_internal () {
    if (minimal_network_okbc_kb_type == null)
      { MinimalNetworkKb kb = new MinimalNetworkKb();
	kb.kb_type_name = Symbol.intern_in_ok_back("MINIMAL-NETWORK-OKBC-KB");
	minimal_network_okbc_kb_type = kb;
	return kb;
      }
    else { return minimal_network_okbc_kb_type; }
  }

// ---------------------- save-kb-as


/**
 * Saves the entire contents of the KB to persistent storage under the
 *   new-name-or-locator .    New-name-or-locator  is either a new
 * name for the KB, or a new kb-locator.  The in-core KB is renamed so that
 *   find-kb-of-type  will return   kb  when passed the new name of
 * the KB.  Returns no values.
 */
// Special case for network kbs.  We have to rename the local KB as well as
// just executing the network call!
/**
 * Saves the entire contents of the KB to persistent storage under the
 *   new-name-or-locator .    New-name-or-locator  is either a new
 * name for the KB, or a new kb-locator.  The in-core KB is renamed so that
 *   find-kb-of-type  will return   kb  when passed the new name of
 * the KB.  Returns no values.
 */
  protected void save_kb_as_internal (Node new_name_or_locator) {
     Node args = Cons.list(save_kb_as_fname, new_name_or_locator, this);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
     if (new_name_or_locator instanceof AbstractSymbol ||
	 new_name_or_locator instanceof OKBCString)
       { setName(new_name_or_locator); }
  }


/******************************************************************************

  The following methods are likely to be shadowed by KB subclasses.

******************************************************************************/

  NetworkConnection the_connection = null;

  public Connection connection () {
    return the_connection;
  }

  public void set_connection (Connection conn) {
    if (conn == null)
      { the_connection = null; }
    else if (conn instanceof NetworkConnection)
      { the_connection = (NetworkConnection)conn; }
    else { throw new RuntimeException
	   ("Connection " + conn + " is not a NetworkConnection");
	 }
  }

  static final OKBCInteger [] fixnums = 
  { new OKBCInteger(0),
    new OKBCInteger(1),
    new OKBCInteger(2),
    new OKBCInteger(3),
    new OKBCInteger(4),
    new OKBCInteger(5),
    new OKBCInteger(6),
    new OKBCInteger(7),
    new OKBCInteger(8),
    new OKBCInteger(9),
    new OKBCInteger(10),
    new OKBCInteger(11),
    new OKBCInteger(12),
    new OKBCInteger(13),
    new OKBCInteger(14),
    new OKBCInteger(15)
  };

  public String toString() {
    return "#<" + getClass().getName() + " " + kbname + "|" + kb_type_name 
                + " " + pointerForToString() + ">";
  }

  static final Symbol get_kbs_of_type_fname =
         Symbol.intern_in_ok_back("GET-KBS-OF-TYPE-INTERNAL");

  public Node get_kbs_of_type
     (Connection connection) {
       if (connection instanceof NetworkConnection)
	 {
	   Node result;
	   Node args =
	     Cons.list(get_kbs_of_type_fname, this,
		       _establish_local_connection);
	   Node values = 
	     ((NetworkConnection)connection).makeNetworkCall(args);
	   return values.car();
	 }
       { return get_kbs_of_type_internal(connection); }
     }

  static final Symbol get_kbs_fname =
         Symbol.intern_in_ok_back("GET-KBS-INTERNAL");

  static public Node get_kbs
     (Connection connection) {
       if (connection instanceof NetworkConnection)
	 { Node result;
	   Node args =
	     Cons.list(get_kbs_fname,
		       _establish_local_connection);
	   Node values = 
	     ((NetworkConnection)connection).makeNetworkCall(args);
	   return Cons.remove_duplicates(values.car(), _eql, null, null);
	 }
       { return get_kbs_internal(connection); }
     }

  static final Symbol get_kb_types_fname =
         Symbol.intern_in_ok_back("GET-KB-TYPES-INTERNAL");

  static public Node get_kb_types
     (Connection connection) {
       if (connection instanceof NetworkConnection)
	 { Node result;
	   Node args =
	     Cons.list(get_kb_types_fname,
		       _establish_local_connection);
	   Node values = 
	     ((NetworkConnection)connection).makeNetworkCall(args);
	   return Cons.remove_duplicates(values.car(), _eql, null, null);
	 }
       { return get_kb_types_internal(connection); }
     }

  static public Node meta_kb
     (Connection connection) {
       if (connection instanceof NetworkConnection)
	 { Node result;
	   Node args =
	     Cons.list(meta_kb_fname, _establish_local_connection);
	   Node values = 
	     ((NetworkConnection)connection).makeNetworkCall(args);
	   return values.car();
	 }
       { return meta_kb_internal(connection); }
     }

  protected Node find_kb_locator_internal(Node thing, Connection connection)
  {
    if (connection instanceof NetworkConnection)
      { Node args = Cons.list(find_kb_locator_fname, thing, this,
			      Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION"));
	Node result = ((Node)((NetworkConnection)connection)
		       .makeNetworkCall(args)).car();
	return result;
      }
    else if (thing instanceof MinimalNetworkKbLocator) { return thing; }
    else if (thing instanceof AbstractSymbol)
      { Kb type = Node.concrete_kb_from_abstract_kb((AbstractSymbol)thing);
	if (type == null) { return _NIL; }
	else { return find_kb_locator(type, connection); }
      }
    else if (thing instanceof MinimalNetworkKb)
      { MetaKb meta_kb = 
	(MetaKb)meta_kb(LocalConnection.local_connection());
	Node locators = meta_kb.get_class_instances(_kb_locator, _taxonomic, 
						    _all, _T).firstValue();
	Enumeration e = locators.elements();
	while (e.hasMoreElements()) {
	  Node locator = (Node)(e.nextElement());
	  if (locator instanceof MinimalNetworkKbLocator &&
	      (((MinimalNetworkKbLocator)locator).name == kbname))
	    { return locator; }
	}
	return create_kb_locator(thing, connection);
      }
    else { throw new GenericError
	   (_T, new OKBCString
	    ("Cannot find a kb locator for " + thing)).signal();
	 }
  }


  protected Node create_kb_locator_internal
     (Node thing, Connection connection) 
  {
    if (connection instanceof NetworkConnection)
      { Node args = Cons.list(create_kb_locator_fname, thing, this,
			      Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION"));
	Node result = ((Node)((NetworkConnection)connection)
		       .makeNetworkCall(args)).car();
	return result;
      }
    else
      { MetaKb meta_kb = 
	  (MetaKb)meta_kb(LocalConnection.local_connection());
	MinimalNetworkKbLocator instance = 
	  new MinimalNetworkKbLocator(kbname);
	meta_kb.put_instance_types(instance, _kb_locator, _T);
	return instance;
      }
  }


  protected void encode(PrintWriter s, boolean client_p) {
    // System.out.println("NetworkKb encode = " + this);
    s.print(typeCode);
    newline(s);
    if (kbname == null) { NetworkKb._kb_type.encode(s, client_p); }
    else { kbname.encode(s, client_p); }
    kb_type_name.encode(s, client_p);
  }
  

/****************************************************************************
  
  
  The remaining methods in this file were programmatically generated.
  
  ****************************************************************************/

// ---------------------- open-kb

/**
 * Given a   kb-locator , a   kb-type , and a
 *   connection , returns a KB object
 * for that KB locator that will behave as if all the objects in the
 * KB are accessible (the implementation is not actually required to load the
 * whole KB into memory).
 * 
 * Implementations are at liberty to accept other values in place of the
 *   kb-locator , such as a pathname that identifies the location of
 * the KB to the system.  Such usage is convenient, but is not portable.  It
 * is not portable for an OKBC application to use anything other than
 * a KB locator for this argument.  If   error-p  is  , catches
 * errors that occur, and attempts to continue with the opening/loading
 * process.  If the KB could not be successfully opened, returns  .
 */
  protected Node open_kb_internal
     (Node kb_locator,
      Connection connection,
      TruthValue error_p) {
     Node args =
        Cons.list(open_kb_fname,
                  kb_locator,
                  this,
                  Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION"),
                  error_p);
     Node result = ((Node)((NetworkConnection)connection).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- openable-kbs

/**
 * Given a   kb-type  and a   connection , returns
 *   list-of-kb-locators , a list of frame handles to
 * frames in the   meta-kb  that are instances of the class
 *    kb-locator .
 * Each kb-locator instance describes one openable KB, identified at the
 * time of the call.  Subsequent calls to openable-kbs will refresh the
 * set of kb-locator instances in the meta-kb.  Kb-locators refering to
 * KBs of   kb-type  that are no longer openable will be removed.  KBs
 * of   kb-type  that have become openable since the last call will
 * become represented in the   meta-kb .
 * 
 *   Place  allows the application to communicate to the KRS in an
 * KRS-specific way where to look for the openable KBs (e.g., a directory).
 * The use of the   place  argument is not portable.
 * If a particular   kb-type  does not understand the value of
 * the   place  argument supplied,   openable-kbs  returns
 *  , that is, there are no known openable KBs consistent
 * with the supplied   place .
 */
  protected Node openable_kbs_internal
     (Connection connection,
      Node place) {
     Node args =
        Cons.list(openable_kbs_fname,
                  this,
                  Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION"),
                  place);
     Node result = ((Node)((NetworkConnection)connection).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- primitive-p

/**
 * Returns   iff   class  is a class whose definition is
 * primitive, and   otherwise.  For KRSs that do not distinguish
 * primitive from defined classes,   primitive-p 
 * must return   for all classes.
 */
  protected Node primitive_p_internal
     (Node the_class,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(primitive_p_fname,
                  the_class,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- put-behavior-values

/**
 * Sets the list of active   values  of the   behavior  under
 * which the KB is to operate.  The elements in   values  must be a
 * subset of the values returned by a call to
 *   get-behavior-supported-values  for the same   behavior .
 * If they are not, an   illegal-behavior-values  error will be signaled.
 * Note that for some behaviors, the order of values is significant (e.g.,
 *    :collection-type ).  Returns no values.
 */
  protected void put_behavior_values_internal
     (Node behavior,
      Node values) {
     Node args =
        Cons.list(put_behavior_values_fname,
                  behavior,
                  values,
                  this);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- put-class-superclasses

/**
 * Changes   class  to be a subclass of all the classes listed in
 *   new-superclasses .  If   frame  was a subclass of any
 * superclasses not mentioned in   new-superclasses , these superclasses
 * are removed.
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  protected void put_class_superclasses_internal
     (Node the_class,
      Node new_superclasses,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_class_superclasses_fname,
                  the_class,
                  new_superclasses,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- put-facet-values

/**
 * Sets the values of the specified facet to be
 *   values , which is assumed to be a set.  Any existing facet values
 * that are not in   values  will be removed.  The order of the elements of
 *   values  will not necessarily be maintained by the KRS.
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  protected void put_facet_values_internal
     (Node frame,
      Node slot,
      Node facet,
      Node values,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_facet_values_fname,
                  frame,
                  slot,
                  facet,
                  values,
                  this,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- put-frame-name

/**
 * Changes the name of   frame  to be   new-name .  All references
 * to   frame  in   kb  (e.g., in slot values) will point to the
 * frame named   new-name .  Returns the frame with the new name,
 *   renamed-frame .  It is not necessary that the frame object identified
 * by   frame  be identical (   == /   EQLness ) to the frame
 * object called   new-name , only that the KB consistently use the
 * new frame instead of the old one.
 * 
 * Implementation note:  KRSs that use frame names as frame handles must
 * replace    all  references to the old name of   frame  with
 *   new-name .  This specification allows for implementations that
 * are forced to replace the representation of the frame with a new, renamed
 * version.
 */
  protected Node put_frame_name_internal
     (Node frame,
      Node new_name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_frame_name_fname,
                  frame,
                  new_name,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- put-frame-pretty-name

/**
 * Stores the   name , a string, as the new pretty-name of the
 *   frame .  OKBC mandates no constraints on the new pretty-  name ,
 * but to maximize the liklihood that applications will interoperate smoothly,
 * implementations are encouraged to make pretty-names be short, and are
 * strongly encouraged to include no whitespace characters other than the space
 * characters, or any display device-specific control characters.  Returns no
 * values.
 */
  protected void put_frame_pretty_name_internal
     (Node frame,
      Node name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_frame_pretty_name_fname,
                  frame,
                  name,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- put-instance-types

/**
 * Changes   frame  to be an instance of all the classes listed in
 *   new-types .  If   frame  was an instance of any types not
 * mentioned in   new-types , these types are removed.
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  protected void put_instance_types_internal
     (Node frame,
      Node new_types,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_instance_types_fname,
                  frame,
                  new_types,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- put-slot-values

/**
 * Sets the values of   slot  in   frame  to
 * be   values .  Any existing slot values that are not in   values 
 * will be removed.  The order of the elements
 * of   values  will not necessarily be maintained by the KRS, unless the
 *    :collection-type  of the slot is    :list .
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  protected void put_slot_values_internal
     (Node frame,
      Node slot,
      Node values,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_slot_values_fname,
                  frame,
                  slot,
                  values,
                  this,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- rename-facet

/**
 * Renames the facet for all frames containing that facet.
 *  
 *   If the facet identified by   facet  is represented as a frame,
 * that frame is renamed.
 *   If the facet identified by   facet  is not represented as
 * a frame,   facet-p  applied to   facet  will return
 *    and   facet  will not be returned by any of
 * the facet-returning operations, such as   get-kb-facets .
 *   New-name  will now identify the
 * facet, and will be returned by operations such as
 *   get-kb-facets  and   get-frame-facets .
 *  
 * All the facet values and facet values associated with   facet  are
 * preserved under the   new-name .  For example, for any frame in
 * the KB, the values returned by   get-facet-values  for   facet 
 * before the rename are identical to the values returned  for
 *   new-name  after the rename.  In addition, the    attached-to 
 * relationship is preserved -- that is, if   facet  is attached to a
 * frame before the rename,   new-name  is attached to that frame
 * after the rename.  In some implementations, references to   facet 
 * may still remain in the KB after   rename-facet .  Returns the
 *   renamed-facet .
 */
  protected Node rename_facet_internal
     (Node facet,
      Node new_name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(rename_facet_fname,
                  facet,
                  new_name,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- rename-slot

/**
 * Renames the slot for all frames containing that slot.
 *  
 *   If the slot identified by   slot  is represented as a frame,
 * that frame is renamed.
 *   If the slot identified by   slot  is not represented as
 * a frame,   slot-p  applied to   slot  will return   
 * and   slot  will not be returned by any of the slot-returning
 * operations, such as   get-kb-slots  and   get-frame-slots .
 *   New-name  will now identify the slot, and will be returned by
 * operations such as   get-kb-slots  and   get-frame-slots .
 *  
 * All the slot values and facet values associated with   slot  are
 * preserved under the   new-name .  For example, for any frame in
 * the KB, the values returned by   get-slot-values  for   slot 
 * before the rename are identical to the values returned  for
 *   new-name  after the rename.  In addition, the    attached-to 
 * relationship is preserved -- that is, if   slot  is attached to a
 * frame before the rename,   new-name  is attached to that frame
 * after the rename.  In some implementations, references to   slot 
 * may still remain in the KB after   rename-slot .  Returns the
 *   renamed-slot .
 */
  protected Node rename_slot_internal
     (Node slot,
      Node new_name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(rename_slot_fname,
                  slot,
                  new_name,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- save-kb

/**
 * Saves the contents of the KB to persistent storage.  No commitment is
 * made as to the location of the KB in persistent storage, other than that
 * it will be openable given the name, kb-type and connection first used
 * to access it.  No commitment is made as to whether the save operation
 * results in a complete dump of the KB, or whether it results only in a
 * dump of the changes made since the KB was last saved.  If   error-p 
 * is  , tries to catch errors that occur, and attempts to continue
 * with saving to the extent possible.
 * Returns   iff the KB was saved successfully, and   
 * otherwise.
 */
  protected Node save_kb_internal
     (TruthValue error_p) {
     Node args =
        Cons.list(save_kb_fname,
                  this,
                  error_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- slot-p

/**
 * Returns   iff   thing  is a slot, and
 * otherwise returns  .
 */
  protected Node slot_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(slot_p_fname,
                  thing,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-behavior-supported-values

/**
 * Returns a list of the supported values of the   behavior 
 * the KB is capable of supporting.  For example, the KB might support both
 * the   :immediate  and   :never  variants of the behavior
 *   :constraint-checking-time .  These two options would be returned
 * as a list.  The returned value   behavior-values  is always a list,
 * even when no variants are supported -- that is, it is  .
 */
  protected Node get_behavior_supported_values_internal
     (Node behavior) {
     Node args =
        Cons.list(get_behavior_supported_values_fname,
                  behavior,
                  this);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-behavior-values

/**
 * Returns a list of active values of the   behavior  under
 * which the KB is currently operating.  For example, the KB might support
 * both the   :immediate  and   :never  variants of the behavior
 *   :constraint-checking-time , but only one of these modes can be
 * enabled at a given time.  A list containing, for example, just
 *   :never  would be returned.
 * The returned value   behavior-values  is always a list, even when
 * no variants are active -- that is, it is the  .
 */
  protected Node get_behavior_values_internal
     (Node behavior) {
     Node args =
        Cons.list(get_behavior_values_fname,
                  behavior,
                  this);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-class-instances

/**
 * Returns a   list-of-instances  for   class .
 */
  protected Values3 get_class_instances_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_class_instances_fname,
                  the_class,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- get-class-subclasses

/**
 * Returns the   list-of-subclasses  of   class .
 */
  protected Values3 get_class_subclasses_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_class_subclasses_fname,
                  the_class,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- get-class-superclasses

/**
 * Returns the   list-of-superclasses  of   class .
 */
  protected Values3 get_class_superclasses_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_class_superclasses_fname,
                  the_class,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- get-facet-values-in-detail

/**
 * Returns the   list-of-specs  describing the values
 * of the   facet  of   slot  within   frame , in no guaranteed
 * order.   It always returns a list of specifications as values.  If the
 * specified slot has no values,   is returned.
 * 
 * Each spec is a 3-tuple of the form (value direct-p default-p).
 *  
 *   value -- a value of the facet
 *   direct-p -- a flag that is   if the value is known to be
 * directly asserted for the facet in the   frame  and
 *   otherwise
 *   default-p -- a flag that is   if the value is known to be
 * a default value, and   otherwise
 *  
 * The fourth returned value (  default-p ) is true if the
 *   list-of-specs  is  , and the fact that there are no values
 * is itself a default.
 */
  protected Values4 get_facet_values_in_detail_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_facet_values_in_detail_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  inference_level,
                  slot_type,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Values4 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues4(); // multiple-values
     return result;
  }


// ---------------------- get-frame-in-kb

/**
 * Returns two values.  The first value is the   frame 
 * identified by   thing  if such a frame is found, or  .
 * The second value (  frame-found-p ) is   iff   thing  is
 * coercible to a frame, and that frame is resident in   KB .  In
 * all cases it is verified that the frame does, in fact, reside in   kb .
 * Otherwise, the   frame-found-p  value is    nil  (unless
 *   error-p  is  , in which case the operation signals a
 *   not-coercible-to-frame  error because
 *   thing  is not a valid frame in   kb ).
 */
  protected Values2 get_frame_in_kb_internal
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frame_in_kb_fname,
                  thing,
                  this,
                  error_p,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- get-frame-name

/**
 * Returns   frame-name , an entity that is the name of the frame
 * identified by   frame , usually a symbol or string.
 */
  protected Node get_frame_name_internal
     (Node frame,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frame_name_fname,
                  frame,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-frame-pretty-name

/**
 * Returns a string that is a pretty, printed representation for   frame 
 * -- that is, the name is suitable for use within a user interface for
 * display purposes.
 * 
 * There is no guarantee that it will be possible to find a unique frame
 * given only its pretty-name, but   get-frames-matching  can be used to
 * find frames matching such strings when possible.
 */
  protected Node get_frame_pretty_name_internal
     (Node frame,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frame_pretty_name_fname,
                  frame,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-frame-slots

/**
 * Returns   list-of-slots , a list of all the own, template, or own
 * and template slots that are associated with   frame , depending on the
 * value of   slot-type .
 */
  protected Values2 get_frame_slots_internal
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frame_slots_fname,
                  frame,
                  this,
                  inference_level,
                  slot_type,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- get-instance-types

/**
 * Returns the   list-of-types  of   frame , that is, the list of
 * classes of which   frame  is an instance.
 */
  protected Values3 get_instance_types_internal
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_instance_types_fname,
                  frame,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- get-kb-behaviors

/**
 * When   kb-type-or-kb  is either a KB or a kb-type, returns
 *   list-of-behaviors , which is a list of keywords naming all
 * the behaviors recognized by this KB, or identified by the kb-type,
 * respectively.
 */
  protected Node get_kb_behaviors_internal
     (Connection connection) {
     Node args =
        Cons.list(get_kb_behaviors_fname,
                  this,
                  Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION"));
     Node result = ((Node)((NetworkConnection)connection).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-kb-frames

/**
 * Returns the   list-of-frames  in the KB, including class, slot,
 * facets and individual frames, when present.
 */
  protected Node get_kb_frames_internal
     (TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_kb_frames_fname,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-slot-facets

/**
 * Returns the   list-of-facets  associated with   slot  in
 *   frame .
 */
  protected Values2 get_slot_facets_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_slot_facets_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- get-slot-values-in-detail

/**
 * Returns the   list-of-specs  describing the values
 * of   slot  within   frame .  If the    :collection-type 
 * of the slot is    :list , and only    :direct  own slots have been
 * asserted, then order is preserved; otherwise, the values are returned in
 * no guaranteed order.   Get-slot-values-in-detail  always returns a
 * list of specifications as its   list-of-specs  value.  If the specified
 * slot has no values,   is returned.
 * 
 * Each spec is a 3-tuple of the form (value direct-p default-p).
 *  
 *   value -- A value of the slot
 *   direct-p -- A flag that is   if the value is known to be
 * directly asserted for the slot and   otherwise
 *   default-p -- A flag that is   if the value is known to be
 * a default value, and   otherwise
 *  
 * The   default-p  returned value is true if the   list-of-specs 
 * is  , and the fact that there are no values is itself a default.
 */
  protected Values4 get_slot_values_in_detail_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_slot_values_in_detail_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Values4 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues4(); // multiple-values
     return result;
  }


// ---------------------- allocate-frame-handle

/**
 * Allocates a frame handle in   kb .  It is not anticipated that this
 * operation will be called by OKBC applications, but rather by OKBC back end
 * implementations.  This operation can be used in two distinct ways:
 *   
 *   Given a frame located in an arbitrary KB, typically different
 * from   kb , passing its   frame-name ,   frame-type , and
 *   frame-handle  will return a frame handle to represent that frame if
 * such a frame were to be created in   kb .  This is useful in OKBC
 * operations such as   copy-frame  and   copy-kb  where it is often
 * necessary to make forward references to frame objects.
 *   Providing just a   frame-name  and a   frame-type 
 * will return a frame handle to represent that frame if such a frame were
 * to be created in   kb .  This is useful when an implementation wants
 * to allocate a frame handle either during the frame creation process, or
 * to create forward references to frames when faulting them in from a
 * lazy persistent store.    Frame-name  may be  .
 *  
 *   Frame-type  is the type of the frame as identified by the operation
 *   get-frame-type ; that is, it must be in the set     :class ,
 *    :individual ,    :slot ,    :facet  .
 * 
 * The rationale for the arguments to this operation is as follows:
 *  
 *     frame-name  -- In some KRSs, the name of a frame cannot be
 * changed after the frame handle has been allocated.  OKBC therefore
 * mandates that the name be supplied.  If the
 *    :frame-names-required  behavior has the value  ,
 * this argument may be  .
 *     frame-type  -- In some KRSs, the type of data structure used
 * to represent the frame handles of (say) classes is completely
 * different from that of (say) individual frames.  OKBC therefore
 * mandates that the frame type be specified.  Implementations that use
 * the same representation for all frame handles will be able to ignore
 * this argument, but it is not portable.
 *     frame-handle  -- Some KRSs may choose to use a frame handle
 * provided as the value of the   frame-handle  argument as the
 * new frame handle.  This allows implementations that do not have a
 * special data structure for frame handles to save memory and to
 * maximize the correspondence between the objects in different KBs.
 *  
 * The contract of   allocate-frame-handle  does not require the callee to
 * return the same frame handle if called multiple times with identical
 * arguments.  Note that this is particularly important in case 2, above,
 * with   frame-name  being  .  It is the responsibility of
 * the caller to remember the
 * correspondence between its frames and the frame handles allocated.  A frame
 * handle allocated using   allocate-frame-handle  can be used as the
 * value of the   handle  argument to   create-frame ,
 *   create-class ,   create-slot ,   create-facet , and
 *   create-individual .  During the execution of these operations, it is
 * the responsibility of the   kb  to preserve any necessary object
 * identity so that, for example,
 *   
 * new-handle = allocate-frame-handle(name, :class, kb, handle);
 * new-frame = create-class(name, .... :handle new-handle);
 * new-handle == get-frame-handle(new-frame) // this identity must hold!
 */
  protected Node allocate_frame_handle_internal
     (Node frame_name,
      ContextName frame_type,
      Node frame_handle) {
     Node args =
        Cons.list(allocate_frame_handle_fname,
                  frame_name,
                  frame_type,
                  this,
                  frame_handle);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- attach-facet

/**
 * Explicitly associates the   facet  with   slot  on   frame ,
 * in the sense of recording that values of the facet may be asserted with
 *   frame  or with instances of   frame  if   slot-type  is
 *    :template .
 * As a result,   facet  is returned by   get-slot-facets  at the
 *    :direct  inference level, and   slot-has-facet-p  will be
 *   for   facet  in   slot  on   frame .  It is an
 * error to attempt to attach a non-existent facet.  Doing so should signal
 * a   facet-not-found  error.  Returns no values.
 */
  protected void attach_facet_internal
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(attach_facet_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  slot_type,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- attach-slot

/**
 * Explicitly associates the   slot  with   frame , in the sense
 * of recording that values of slot may be asserted with   frame  or
 * with instances of   frame  if   slot-type  is    :template .
 * As a result,   slot  is returned by   get-frame-slots  at the
 *    :direct  inference level, and   frame-has-slot-p  will be
 *   for   slot  on   frame .  It is an error to attempt to
 * attach a non-existent slot.  Doing so should signal a
 *   slot-not-found  error.  Returns no values.
 */
  protected void attach_slot_internal
     (Node frame,
      Node slot,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(attach_slot_fname,
                  frame,
                  slot,
                  this,
                  slot_type,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- class-p

/**
 * Returns   if   thing  identifies a class.
 */
  protected Node class_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(class_p_fname,
                  thing,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- close-kb

/**
 * Deletes any in-core/accessible representation of   kb , but does not
 * remove it from any persistent store if the persistent version still
 * constitutes a meaningful KB (i.e., temporary disk work files would be
 * deleted).  It is an error ever to use   kb  again for any purpose.  If
 * this occurs, an   object-freed  error should be signaled.
 * Implementations may free any storage allocated for KB.  If   save-p 
 * is  , then any unsaved changes to   kb  will be saved
 * before it is closed.  Note that the default value of   save-p 
 * is  .  Returns no values.
 */
  protected void close_kb_internal
     (Node save_p) {
     Node args =
        Cons.list(close_kb_fname,
                  this,
                  save_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- coerce-to-frame

/**
 * Coerces   thing  to be a frame object, if such an object exists for
 * the underlying KRS, or a frame handle otherwise.    Thing  can be a
 * frame object or a frame handle.  This operation may be
 * less careful than   get-frame-in-kb  about ensuring that the
 * frame for   thing  is actually in   kb  when the supplied
 *   thing  is a frame object.    Coerce-to-frame  verifies that
 *   thing  is the
 * appropriate    type  of frame object for   kb , but may not
 * actually determine whether the frame resides in   kb .  Therefore, this
 * operation may be faster than   get-frame-in-kb  for some KRSs.
 * 
 * For user convenience, implementors are encouraged to support the coercion
 * into a frame of any data-structure that uniquely identifies a frame in
 * the KRS as well as frame handles and frame objects.  It is not
 * portable to provide any value for   thing  other than a frame
 * object or frame handle;   get-frames-matching  should be used instead.
 * 
 * If the    :frame-names-required  behavior has the value   for
 *   kb , the names of frames are always coercible to frames.
 * If the    :frame-names-required  behavior is  , frame names
 * (if defined) are not guaranteed to be coercible to frames.
 * 
 * This operation returns two values.
 *  
 *     frame  -- If   thing  identifies a frame for   kb ,
 * then this value is the frame so identified, or
 *   otherwise.
 *     frame-found-p  -- If the frame is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the frame is not found, then a
 *   not-coercible-to-frame  error is signaled.
 * 
 * It is an error to call   coerce-to-frame  with   error-p  being
 *  , and with a value of   thing  that does not uniquely identify
 * a frame.  If this happens, a   not-unique-error  error should be
 * signaled.
 * 
 * Note that in some KRS,   may be a valid frame object.  No portable
 * program may assume that a returned value of   for the first
 * (  frame ) returned value implies that   frame-found-p  is
 *  .
 */
  protected Values2 coerce_to_frame_internal
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(coerce_to_frame_fname,
                  thing,
                  this,
                  error_p,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- create-class

/**
 * Creates a class called   name  as a direct subclass
 * of the list of classes (or class)   direct-superclasses .  For KRSs
 * that support the distinction between primitive and nonprimitive concepts,
 *   primitive-p  specifies the primitiveness of the created class.
 * The parameters   doc ,   template-slots ,
 *   template-facets ,   own-slots ,   own-facets ,
 *   direct-types ,   handle , and   pretty-name  have the
 * same meaning as for   create-frame .  For KRSs that support metaclasses,
 * the   direct-types  argument specifies the type(s) of the class
 * to be created (i.e., metaclasses).  Returns the   new-class .
 */
  protected Node create_class_internal
     (Node name,
      Node direct_types,
      Node direct_superclasses,
      Node primitive_p,
      Node doc,
      Node template_slots,
      Node template_facets,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(create_class_fname,
                  name,
                  this,
                  direct_types,
                  direct_superclasses,
                  primitive_p,
                  doc,
                  template_slots,
                  template_facets,
                  own_slots,
                  own_facets,
                  handle,
                  pretty_name,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- create-facet

/**
 * Creates a facet called   name  on    slot-or-nil  that is
 * associated with   frame-or-nil .  If   frame-or-nil  is  ,
 * the facet's frame domain is unconstrained (i.e., the facet may apply to
 *   slot-or-nil  in any frame).
 * 
 * If   slot-or-nil  is  , the slot domain of the facet is
 * unconstrained (i.e., the facet may apply to all slots in
 *   frame-or-nil , and if   frame-or-nil  is also  , may
 * apply to all slots in all frames.)
 * If    :facet  is a member of the behavior values for the
 *    :are-frames  behavior,   direct-types ,
 *   doc ,   own-slots ,   own-facets ,   handle  and
 *   pretty-name  have the same interpretation as for   create-frame .
 * If either   frame-or-nil  or   slot-or-nil  is  ,
 *   slot-type  is ignored.  If either of the   frame-or-nil  or
 *   slot-or-nil  arguments is
 *  , and the KRS does not support facets with unconstrained domains,
 * a   domain-required  error will be signaled.
 * If facets must be uniquely named and a facet named   name  already
 * exists, a   facet-already-exists  error will be signalled.
 * Returns the   new-facet .
 */
  protected Node create_facet_internal
     (Node name,
      Node frame_or_nil,
      Node slot_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(create_facet_fname,
                  name,
                  this,
                  frame_or_nil,
                  slot_or_nil,
                  slot_type,
                  direct_types,
                  doc,
                  own_slots,
                  own_facets,
                  handle,
                  pretty_name,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- create-individual

/**
 * Creates an individual called   name .  The one or more classes
 * that are the direct types of the instance are given by
 *   direct-types .
 * The parameters   doc ,   own-slots ,   own-facets ,
 *   handle , and   pretty-name  all have the same meaning as for
 *   create-frame .  Returns   new-individual , which identifies the
 * new frame.
 */
  protected Node create_individual_internal
     (Node name,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(create_individual_fname,
                  name,
                  this,
                  direct_types,
                  doc,
                  own_slots,
                  own_facets,
                  handle,
                  pretty_name,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- create-slot

/**
 * Creates a slot called   name  in the frame specified by
 *   frame-or-nil .  Returns the   new-slot .  If the slot to be
 * created is to be represented as a frame (   :slot  is a member of the
 *    :are-frames  behavior),   direct-types ,
 *   doc ,   own-slots ,   own-facets ,   handle , and
 *   pretty-name  have the same interpretation as for   create-frame .
 * If   frame-or-nil  is  ,   slot-type  is ignored, and the
 * slot's domain is ignored.  If the
 *   frame  argument is  , and the KRS does not support slots with
 * unconstrained domains, a   domain-required  error will be signaled.
 * If slots must be uniquely named and a slot named   name  already
 * exists, a   slot-already-exists  error will be signalled.
 */
  protected Node create_slot_internal
     (Node name,
      Node frame_or_nil,
      SlotType slot_type,
      Node direct_types,
      Node doc,
      Node own_slots,
      Node own_facets,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(create_slot_fname,
                  name,
                  this,
                  frame_or_nil,
                  slot_type,
                  direct_types,
                  doc,
                  own_slots,
                  own_facets,
                  handle,
                  pretty_name,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- decontextualize

/**
 * Given a value from   kb , returns a
 *   decontextualized-value , which contains no KB or KRS-specific
 * data structures.  In particular,
 * any references to frame objects will be replaced with KRS-independent frame
 * handles (produced using   frs-independent-frame-handle ), and all
 * values outside the standard set of OKBC data types that have
 * no interpretation outside   kb  will be
 * replaced with remote-value references.  Any frame references that are
 * the result of an KRS-specific mapping of a canonically named frame will be
 * replaced with the canonical name.  Thus, for example, a facet
 * frame called    cardinality-of-slot  would be mapped back to a frame
 * handle for the canonical facet-reference    :cardinality .
 * 
 *   From-context  is one of     :frame ,    :slot ,
 *    :facet ,    :value  .  It identifies the context of the argument
 * to be decontextualized.  For example, if the
 * decontextualization is to be applied to a slot value, then
 *   from-context  should be    :value .  If the
 * decontextualization is to be applied to a slot (i.e., something that would
 * be used as a   slot  argument to an operation such as
 *   get-slot-values ), then   from-context  should be    :slot .
 * 
 * It is not anticipated that this operation will be called by OKBC
 * applications, but rather by OKBC back end implementations.  It is used to
 * ensure correct operation in networked applications and during communication
 * between KBs of different kb-types.
 */
  protected Node decontextualize_internal
     (Node value,
      ContextName from_context) {
     Node args =
        Cons.list(decontextualize_fname,
                  value,
                  from_context,
                  this);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- delete-facet

/**
 * Deletes the facet from all frames containing that facet, and the facet frame
 * itself if the facet is represented as a frame. As a result of
 *   delete-facet ,   facet  will return   for calls to
 *   facet-p , and   facet  is not returned by any of the
 * facet-returning operations, such as   get-kb-facets  and
 *   get-slot-facets .  It is no longer possible to access any values of
 *   facet .  Returns no values.
 * 
 * Many implementations may, in fact, delete the values associated the
 * facet in frames as well as making the facet no longer facet-p.  Other
 * implementations will simply make these values inaccessible.
 */
  protected void delete_facet_internal
     (Node facet,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(delete_facet_fname,
                  facet,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- delete-frame

/**
 * Deleting a frame from a KB is difficult to specify in a portable way.
 * After calling   delete-frame , the   frame  argument
 * will no longer be a valid frame reference (  frame-p  will return
 *  ).  As a consequence, the value of   frame  will
 * not be a valid argument to any OKBC operation requiring a frame reference,
 * such as   get-frame-slots .  It will no longer be possible to access
 * any of the properties (e.g., slots, facets) of
 *   frame .  Implementations will delete at least the
 * properties documented as being returned by   get-frame-details 
 * from the   kb .
 * 
 * Note that after a call to   delete-frame , references
 * to   frame  may still remain in the KB.  Returns no values.
 */
  protected void delete_frame_internal
     (Node frame,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(delete_frame_fname,
                  frame,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- delete-slot

/**
 * Deletes the slot from all frames containing that slot, and the slot frame
 * itself if the slot is represented as a frame. As a result of
 *   delete-slot ,   slot  will return   for calls to
 *   slot-p , and   slot  is not returned by any of the
 * slot-returning operations, such as   get-kb-slots  and
 *   get-frame-slots .  It is no longer possible to access any values of
 *   slot  or any facets or facet values on   slot .
 * Returns no values.
 * 
 * Many implementations may, in fact, delete the values associated the
 * slot in frames as well as making the slot no longer slot-p.  Other
 * implementations will simply make these values inaccessible.
 */
  protected void delete_slot_internal
     (Node slot,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(delete_slot_fname,
                  slot,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- detach-facet

/**
 * Removes any explicit association between the   facet  and
 *   slot  on   frame .  As a result,   facet  is not returned
 * by   get-slot-facets  at inference-level    :direct  unless there are
 * facet values associated with   facet  in   slot  on
 *   frame .
 */
  protected void detach_facet_internal
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(detach_facet_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  slot_type,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- detach-slot

/**
 * Removes any explicit association between the   slot  and
 *   frame .  As a result,   slot  is not returned by
 *   get-frame-slots  at inference-level    :direct  unless there are
 * slot or facet values associated with   slot  in   frame .
 */
  protected void detach_slot_internal
     (Node frame,
      Node slot,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(detach_slot_fname,
                  frame,
                  slot,
                  this,
                  slot_type,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- expunge-kb

/**
 * Given a   kb-locator , removes the KB identified by that locator
 * and any backup copies of it from permanent storage.  Returns no values.
 * Any currently open KB identified by the locator will be unaffected,
 * and may be saved to other locations using   save-kb-as .  If
 *   error-p  is  ,   expunge-kb  catches errors that
 * occur, and attempts to continue with the deletion process.
 */
  protected void expunge_kb_internal
     (Node kb_locator,
      Connection connection,
      TruthValue error_p) {
     Node args =
        Cons.list(expunge_kb_fname,
                  kb_locator,
                  this,
                  Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION"),
                  error_p);
     ((NetworkConnection)connection).makeNetworkCall(args);
  }


// ---------------------- facet-p

/**
 * Returns   iff   thing  is a facet, and   otherwise.
 */
  protected Node facet_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(facet_p_fname,
                  thing,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


}
