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

public class NetworkKb extends MinimalNetworkKb {

/******************************************************************************


The following methods are the hand-written required client-side Network-OKBC
stub methods.  They are hand written either because they are not part of the
OKBC spec/defined using defokbcop, or have something magical with respect to
the network implementation.

create-kb-internal

******************************************************************************/

/******************************************************************************

  The following methods are likely to be shadowed by KB subclasses.

******************************************************************************/

  public NetworkKb() { }


  static Symbol network_okbc_kb_abstract_name = null;
  static NetworkKb network_okbc_kb_type = null;

  public static void ensure_registered () {
    if (network_okbc_kb_abstract_name == null)
      { network_okbc_kb_abstract_name = (Symbol)(register_concrete_kb_type
	  (Symbol.intern_in_ok_back("ABSTRACT-NETWORK-OKBC-KB-KB"),
	   get_kb_type_internal()));
      }
  }

  public static Kb get_kb_type_internal () {
    if (network_okbc_kb_type == null)
      { NetworkKb kb = new NetworkKb();
	kb.kb_type_name = Symbol.intern_in_ok_back("NETWORK-OKBC-KB");
	network_okbc_kb_type = kb;
	return kb;
      }
    else { return network_okbc_kb_type; }
  }





  public NetworkKb(NetworkConnection conn) {
    the_connection = conn;
  }

  static Node all_network_kb_types = _NIL;
  
  public static Kb get_kb_type (AbstractSymbol type) {
    Node entry = Cons.assoc(type, all_network_kb_types);
    if (entry == _NIL || entry == null)
      { NetworkKb kb = new NetworkKb();
	kb.kb_type_name = type;
	all_network_kb_types = new Cons(Cons.list(type, kb),
					  all_network_kb_types);
	return kb;
      }
    else { return (Kb)(entry.nth(1)); }
  }

/******************************************************************************
  
  
  The remaining methods in this file were programmatically generated.
  
  ****************************************************************************/

// ---------------------- member-behavior-values-p

/**
 * Returns   when   value  is one of the variants of
 *   behavior  that is currently active for   kb , and returns
 *   otherwise.
 */
  protected Node member_behavior_values_p_internal
     (Node behavior,
      Node value) {
     Node args =
        Cons.list(member_behavior_values_p_fname,
                  behavior,
                  value,
                  this);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- member-facet-value-p

/**
 * Returns   iff   value  is a value in the specified   facet 
 * of   slot  on   frame , as determined by the predicate
 *   test , and returns   otherwise.
 */
  protected Values2 member_facet_value_p_internal
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(member_facet_value_p_fname,
                  frame,
                  slot,
                  facet,
                  value,
                  this,
                  inference_level,
                  test,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- member-slot-value-p

/**
 * Returns   iff   value  is a value in   slot  of
 *   frame , as determined by the predicate   test , and returns
 *   otherwise.
 */
  protected Values2 member_slot_value_p_internal
     (Node frame,
      Node slot,
      Node value,
      InferenceLevel inference_level,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(member_slot_value_p_fname,
                  frame,
                  slot,
                  value,
                  this,
                  inference_level,
                  test,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- print-frame

/**
 * Prints a textual representation to   stream  of the
 *   frame .  A warning is printed when   frame  is not
 * coercible to a frame.  Stream objects as values to the   stream 
 * argument are generally system- and implementation-specific, so stream
 * objects will not in general be
 * transmissible in networked implementations.  Two special values for the
 *   stream  argument are also supported.  If   stream  is  ,
 * then   print-frame  prints to a string stream and
 * that string is returned as   false-or-string .
 * If   stream  is  ,   print-frame  attempts to print to the
 * standard output stream    of the server on which    print-frame 
 *    runs.   When   kb  is
 * a network-acessed KB, this latter option is unlikely to have a useful
 * effect.  Unless   stream  is  ,   false-or-string  is
 *  .
 * 
 * The   slots  and   facets  arguments control which slots (facets)
 * are to be displayed.  They can each take on one of the following values:
 *  
 *      :all  -- Shows all applicable slots (facets)
 *      :none  -- Shows no slots (facets)
 *      :filled  -- Shows the subset of slots (facets) that have at
 * least one value for   frame 
 *   list of slots (facets) -- Only the listed slots (facets) are shown,
 * if present
 */
  protected Node print_frame_internal
     (Node frame,
      Node slots,
      Node facets,
      Node stream,
      InferenceLevel inference_level,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(print_frame_fname,
                  frame,
                  this,
                  slots,
                  facets,
                  stream,
                  inference_level,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- put-facet-value

/**
 * Sets the values of the specified facet to be a singleton set
 * consisting of a single element:   value .  Returns no values.
 */
  protected void put_facet_value_internal
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_facet_value_fname,
                  frame,
                  slot,
                  facet,
                  value,
                  this,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- put-frame-details

/**
 * Redefines   frame  to have the specified   details .
 *   Details  is a property list as specified for
 *   get-frame-details .  This operation is useful for systems that allow
 * transaction-oriented editing of multiple aspects of a frame.
 * The properties    :handle ,    :frame-type , and    :primitive-p 
 * are ignored, since these may not be put.
 * Returns no values.
 */
  protected void put_frame_details_internal
     (Node frame,
      Node details,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_frame_details_fname,
                  frame,
                  details,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- put-slot-value

/**
 * Sets the values of   slot  in   frame  to be a singleton set
 * consisting of a single element:    value .
 * This operation may signal constraint violation conditions
 * (see Section  ).  Returns no values.
 */
  protected void put_slot_value_internal
     (Node frame,
      Node slot,
      Node value,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(put_slot_value_fname,
                  frame,
                  slot,
                  value,
                  this,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- register-procedure

/**
 * Associates the   procedure  with   name  in   kb .
 * Subsequent calls to   call-procedure  may invoke the procedure merely by
 * using the name.  If   name  is  , then   procedure 
 * must be a    named  procedure, in which case the   name 
 * argument will default to the name of the procedure.
 * Returns no values.
 */
  protected void register_procedure_internal
     (Node name,
      Node procedure) {
     Node args =
        Cons.list(register_procedure_fname,
                  name,
                  procedure,
                  this);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- remove-class-superclass

/**
 * Removes   superclass-to-remove  from the superclasses of   class .
 * Returns no values.
 */
  protected void remove_class_superclass_internal
     (Node the_class,
      Node superclass_to_remove,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(remove_class_superclass_fname,
                  the_class,
                  superclass_to_remove,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- remove-facet-value

/**
 * If   value  is currently a member of the set of direct values of
 * the specified facet, then   value  is removed from the values of
 * the facet.  Returns no values.
 */
  protected void remove_facet_value_internal
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(remove_facet_value_fname,
                  frame,
                  slot,
                  facet,
                  value,
                  this,
                  test,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- remove-instance-type

/**
 * Removes   type-to-remove  from the types of   frame  -- that is,
 * makes   frame  no longer be an instance of   type-to-remove .
 * Returns no values.
 */
  protected void remove_instance_type_internal
     (Node frame,
      Node type_to_remove,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(remove_instance_type_fname,
                  frame,
                  type_to_remove,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- remove-local-facet-values

/**
 * Removes all direct values of   facet  in   slot  of   frame .
 * Returns no values.
 */
  protected void remove_local_facet_values_internal
     (Node frame,
      Node slot,
      Node facet,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(remove_local_facet_values_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- remove-local-slot-values

/**
 * Removes all direct values in   slot  of   frame .
 * Returns no values.
 */
  protected void remove_local_slot_values_internal
     (Node frame,
      Node slot,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(remove_local_slot_values_fname,
                  frame,
                  slot,
                  this,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- remove-slot-value

/**
 * If   value  is currently a member of the set of direct values of
 *   slot , then   value  is removed from the values of
 *   slot .  Only values matching the   test  are removed.
 * If   index  is    :all , then all occurrences of
 *   value  will be removed.  Otherwise,   index  should be
 * an integer index into the values list, and only the value at that
 * position, if it matches   value , will be removed (the first
 * value of the slot has index 0).  The index argument is used only by slots
 * whose    :collection-type  is    :list .  Returns no values.
 */
  protected void remove_slot_value_internal
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type,
      Node index,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(remove_slot_value_fname,
                  frame,
                  slot,
                  value,
                  this,
                  test,
                  slot_type,
                  index,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- replace-facet-value

/**
 * If   old-value  is currently a member of the set of direct values of
 * the specified facet, then   old-value  is replaced by
 *   new-value  in the facet.  Returns no values.
 */
  protected void replace_facet_value_internal
     (Node frame,
      Node slot,
      Node facet,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(replace_facet_value_fname,
                  frame,
                  slot,
                  facet,
                  old_value,
                  new_value,
                  this,
                  test,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- replace-slot-value

/**
 * If   old-value  is currently a member of the set of direct values of
 *   slot , then   old-value  is replaced by   new-value  in
 *   slot .  If   index  is    :all  then all occurrences of
 *   old-value  will be replaced.  Otherwise,   index  should be
 * an integer index into the values list, and only the value at that
 * position, if it matches   old-value , will be replaced (the first
 * value of the slot has index 0).
 * This operation may signal constraint violation conditions
 * (see Section  ).  The   index  argument is used only
 * by slots whose    :collection-type  is    :list .
 * Returns no values.
 */
  protected void replace_slot_value_internal
     (Node frame,
      Node slot,
      Node old_value,
      Node new_value,
      Node test,
      SlotType slot_type,
      Node index,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(replace_slot_value_fname,
                  frame,
                  slot,
                  old_value,
                  new_value,
                  this,
                  test,
                  slot_type,
                  index,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- revert-kb

/**
 * This operation is called when the user wishes to discard any changes
 * made to a KB since it was last saved, and revert to the previously saved
 * state.
 * 
 * Equivalent to successive calls to   close-kb  and then   open-kb .
 * The   reverted-kb  returned has the same content as   kb  had at
 * the time it was last saved (or empty, if the kb had never
 * been saved).  No guarantee is made that the   reverted-kb 
 * will have the same identity (==, EQLness) as   kb ,
 * but some implementations may choose to recycle the existing KB object.
 * After a call to   revert-kb , portable applications must refer only
 * to the   reverted-kb  object rather than   kb  in case it was
 * not recycled.  References to the original   kb  may result in
 * an   object-freed  condition being signaled.
 * If   error-p  is  , tries to catch errors that occur, and
 * attempts to continue with reverting to the extent possible.
 */
  protected Node revert_kb_internal
     (TruthValue error_p) {
     Node args =
        Cons.list(revert_kb_fname,
                  this,
                  error_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- slot-has-facet-p

/**
 * Returns   iff   facet  is a valid facet for   slot  on
 *   frame , and   otherwise.  What constitutes a valid facet is
 * KB-specific, but a facet with a value locally asserted, or with a value
 * that is accessible from a template slot will return   for this
 * operation.
 */
  protected Node slot_has_facet_p_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(slot_has_facet_p_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  inference_level,
                  slot_type,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- slot-has-value-p

/**
 * Returns   iff   slot  on   frame  has at least one
 * value, otherwise returns  .
 */
  protected Values2 slot_has_value_p_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(slot_has_value_p_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- subclass-of-p

/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
  protected Values2 subclass_of_p_internal
     (Node subclass,
      Node superclass,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(subclass_of_p_fname,
                  subclass,
                  superclass,
                  this,
                  inference_level,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- superclass-of-p

/**
 * Returns   if class   subclass  is a subclass of class
 *   superclass , and returns   otherwise.
 */
  protected Values2 superclass_of_p_internal
     (Node superclass,
      Node subclass,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(superclass_of_p_fname,
                  superclass,
                  subclass,
                  this,
                  inference_level,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- tell

/**
 * The   tell  operation asserts the   sentence  to be true in
 * the knowledge base   kb .  Some KRSs may allow users to attach a
 * sentence to a specific frame in the KB.  If that is possible and desired,
 * the   frame  argument may be supplied.  When   kb-local-only-p 
 * is  , the   sentence  should be asserted in the   kb , even
 * if   kb  includes another KB containing the   sentence .  When the
 *   sentence  argument is syntactically invalid,   tell  signals
 * a   syntax-error  error.  A KRS may not accept all valid sentences
 * of the OKBC Assertion Language, and for such cases,   tell  raises the
 * condition   cannot-handle .  Returns no values.
 */
  protected void tell_internal
     (Node sentence,
      Node frame,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(tell_fname,
                  sentence,
                  this,
                  frame,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- tellable

/**
 * The   tellable  operation returns   if the KRS can
 * determine that   tell ing the   sentence  would result in a
 *   cannot-handle  error being signaled, and   otherwise.
 * It may raise a   syntax-error  error as specified with the
 * definition of   tell .  Even if   tellable  returns
 *  ,   tell  may still not be able to handle the
 *   sentence .
 */
  protected Node tellable_internal
     (Node sentence,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(tellable_fname,
                  sentence,
                  this,
                  value_selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- type-of-p

/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  protected Values2 type_of_p_internal
     (Node the_class,
      Node thing,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(type_of_p_fname,
                  the_class,
                  thing,
                  this,
                  inference_level,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- unregister-procedure

/**
 * Removes any procedure association for the   name  in   kb .
 * Returns no values.
 */
  protected void unregister_procedure_internal
     (Node name) {
     Node args =
        Cons.list(unregister_procedure_fname,
                  name,
                  this);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- untell

/**
 * The   untell  operation can be used to remove assertions from the
 * knowledge base.  Returns   if the sentence was removed, and
 *   otherwise.
 * 
 * The effect of   untell ing a sentence of the OKBC Assertion Language
 * is equivalent to the effect of executing an equivalent OKBC operation.
 * The OKBC operations equivalent to   untell ing a sentence in the
 * Assertion Language are specified in Section  .
 * For example, the operation
 *   
 * (Untell `(slot-name frame-name ,slot-value))
 *  
 * is equivalent to the OKBC operation    (remove-slot-value 'frame-name
 * 'slot-name slot-value :slot-type :own). 
 * 
 * The effect of   untell ing an arbitrary   tellable  sentence
 * is not predictable across implementations.  For a given sentence   
 * s , executing    (untell s :kb kb)  after executing    (tell s :kb
 * kb)  should remove the sentence    s  from the    kb .  That is, a
 * second call to   untell  should return    nil .  This
 * does not mean that    s  is no longer true, as it may be implied by
 * other assertions in the KB.
 * 
 * Some KRSs may allow users to attach an assertion to a frame in the KB.
 * If the   sentence  was attached to a frame, the   frame 
 * argument must be supplied.  The default value for   frame  is
 *  .  When the   sentence  argument is syntactically invalid, it
 * may signal the   syntax-error  error.
 */
  protected Node untell_internal
     (Node sentence,
      Node frame,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(untell_fname,
                  sentence,
                  this,
                  frame,
                  value_selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- value-as-string

/**
 * Given some   value , returns two values, a string that is a printed
 * representation of that value, and a list of three-tuples.
 * In the second   location-list  value, one tuple is supplied for each
 * frame reference encountered in   value  in the order they appear
 * in the printed representation.  Each three-tuple is of the form
 *    (index0 index1 frame) , where    index0  is the zero-indexed
 * start index of a printed representation of the
 *    frame  in the printed representation and    index1  is the
 * index one past the end.  For example, printing the list    ( <frame1>) 
 * might deliver the values "(Frame1)" and    ((1 8  <frame1>)) .
 * 
 * This operation is useful for user interfaces that have no way in
 * general to print the arbitrary data structures that might be returned by,
 * for example,   get-slot-values .  The second value allows user
 * interfaces to print the string with mouse-sensitive regions that point to
 * frames. 
 *   Purpose 
 * -- The   purpose  argument can be one of     :file ,
 *    :user-interface  .  When it is    :file , it allows the
 * KB to print data in a manner that will be readable again (e.g.,
 * for dumping to files),
 * preserving object identity where necessary.  The output will
 * therefore typically fully escape strings, and will probably
 * package qualify symbols.  When   purpose  is
 *    :user-interface , the string returned will be as pretty
 * and readable as possible for a user interface, but such
 * output will probably not be readable by OKBC. 
 *   Pretty-p 
 * -- When  , OKBC will attempt to print everything in as pretty a
 * manner as possible.  This includes printing frames by using their
 * pretty-names, and by printing strings without escaping or
 * quotes.
 */
  protected Values2 value_as_string_internal
     (Node value,
      Node purpose,
      Node pretty_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(value_as_string_fname,
                  value,
                  this,
                  purpose,
                  pretty_p,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- enumerate-class-instances

/**
 * Returns an enumerator for the elements returned by
 *   get-class-instances .
 */
  protected Enumerator enumerate_class_instances_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_class_instances_fname,
                  the_class,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- enumerate-class-subclasses

/**
 * Returns an enumerator for the elements returned by
 *   get-class-subclasses .
 */
  protected Enumerator enumerate_class_subclasses_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_class_subclasses_fname,
                  the_class,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- enumerate-class-superclasses

/**
 * Returns an enumerator for the elements returned by
 *   get-class-superclasses .
 */
  protected Enumerator enumerate_class_superclasses_internal
     (Node the_class,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_class_superclasses_fname,
                  the_class,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-classes-in-domain-of

/**
 * Returns a list of   classes  that are known to be in the domain of
 *   slot  with respect to   frame-attachment  (if supplied).  If
 *   frame-attachment  is supplied, it may be used as a hint to the KRS
 * to limit the amount of computation performed by constraining the search
 * for classes to the superclasses or types of   frame-attachment .
 * Each class returned (and any subclass) is guaranteed to be a legal
 *   frame  argument for a slot operation on   slot  with
 *   slot-type     :template , e.g.,   put-slot-values .
 */
  protected Node get_classes_in_domain_of_internal
     (Node slot,
      Node frame_attachment,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_classes_in_domain_of_fname,
                  slot,
                  this,
                  frame_attachment,
                  inference_level,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-facet-value

/**
 * Returns the sole member of the set of values
 * of the specified facet.  It is most commonly used when that set is
 * expected to have only one member.  When the facet has no value,
 *   value-or-false  is  .  It is an error to
 * call this operation on a non-single-valued facet; if it is called, a
 *   cardinality-violation  error should be signaled.
 */
  protected Values2 get_facet_value_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_facet_value_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  inference_level,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- get-facet-values

/**
 * Returns the set of values of the specified facet,
 * in no guaranteed order.  It always returns a (possibly empty) list
 * of values.
 */
  protected Values3 get_facet_values_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_facet_values_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  inference_level,
                  slot_type,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- enumerate-facet-values

/**
 * Returns an enumerator for the elements returned by
 *   get-facet-values .
 */
  protected Enumerator enumerate_facet_values_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_facet_values_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  inference_level,
                  slot_type,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- enumerate-facet-values-in-detail

/**
 * Returns an enumerator for the elements returned by
 *   get-facet-values-in-detail .
 */
  protected Enumerator enumerate_facet_values_in_detail_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_facet_values_in_detail_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  inference_level,
                  slot_type,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-frame-details

/**
 * Returns a property list (list of alternating keywords and values)
 * describing the   frame .  The properties of the frame are computed
 * using the   inference-level ,   number-of-values-p , and
 *   kb-local-only-p  arguments, whenever applicable to the appropriate
 * OKBC operator used to compute any given property.  The set of properties
 * computed is at least the following:
 * 
 *    ll 
 *    Property          &    Operation(s) used to compute property   
 *    :handle           &   get-frame-handle        
 *    :name             &   get-frame-name          
 *    :pretty-name      &   get-frame-pretty-name   
 *    :frame-type       &   get-frame-type          
 *    :primitive-p      &   primitive-p             
 *    :superclasses     &   get-class-superclasses        
 *    :subclasses       &   get-class-subclasses          
 *    :types            &   get-instance-types               
 *    :own-slots        &   get-frame-slots ,   get-slot-values  
 *    :template-slots   &   get-frame-slots ,   get-slot-values  
 *    :own-facets       &   get-frame-slots ,   get-slot-values ,   get-slot-facets ,   get-facet-values   
 *    :template-facets  &   get-frame-slots ,   get-slot-values ,   get-slot-facets ,   get-facet-values   
 *    :sentences        &   get-frame-sentences     
 *  
 * 
 * The    :own-slots ,    :own-facets ,    :template-slots , and
 *    :template-facets  properties returned are slot and facet
 * specifications as defined for   create-frame .  This operation is most
 * useful in low-bandwidth or high-latency applications.  A single call to
 *   get-frame-details  is often sufficient to display all the
 * interesting features of a frame.  The value of the    :sentences 
 * component is a set of non-OKBC sentences, i.e. what would be returned by
 *   get-frame-sentences  with the    :okbc-sentences-p  argument set
 * to
 */
  protected Values2 get_frame_details_internal
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frame_details_fname,
                  frame,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- get-frame-handle

/**
 * Returns a   frame-handle  that uniquely identifies the   frame 
 * argument in   kb .
 */
  protected Node get_frame_handle_internal
     (Node frame,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frame_handle_fname,
                  frame,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-frame-sentences

/**
 * Returns a list of all the logical sentences associated with a   frame .
 * The sentences may have been asserted using   tell , or any other
 * OKBC update operation.  If   okbc-sentences-p  is  , then all
 * sentences are returned, including the ones that are equivalent to
 * basic OKBC operations.  The sentences equivalent to OKBC operations are
 * defined in Table  .  If   okbc-sentences-p  is
 *  , sentences that are equivalent to OKBC operations are not
 * returned.  This is very useful for user interface applications that do not
 * want to present redundant information.  If no matching sentences are
 * found,   list-of-sentences  will be  .
 */
  protected Values3 get_frame_sentences_internal
     (Node frame,
      Node number_of_values,
      Node okbc_sentences_p,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frame_sentences_fname,
                  frame,
                  this,
                  number_of_values,
                  okbc_sentences_p,
                  value_selector,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- enumerate-frame-slots

/**
 * Returns an enumerator for the elements returned by
 *   get-frame-slots .
 */
  protected Enumerator enumerate_frame_slots_internal
     (Node frame,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_frame_slots_fname,
                  frame,
                  this,
                  inference_level,
                  slot_type,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-frame-type

/**
 * When   thing  identifies a frame, returns either
 *    :slot ,    :facet ,    :class , or    :individual ,
 * depending on the type of the frame.
 * When   thing  does not identify a frame,   frame-type  is
 *  .     :Slot  and    :facet  will be returned only in
 * those systems that support the values    :slot  and    :facet ,
 * respectively, for the    :are-frames  behavior.
 */
  protected Node get_frame_type_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frame_type_fname,
                  thing,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-frames-matching

/**
 * Given a   pattern , which is a string or a symbol,
 * finds a set of matching frames for that pattern.
 * The match of a frame to a pattern could take into account the frame's name
 * (if meaningful), printed representation, pretty-name, or any KB-specific
 * feature such as a list of synonyms.
 * 
 * Returns the following two values:
 *   
 *     matching-frames  -- The list of matching frames (which is
 *   if no matches are found).
 *     longest-matching-substring  -- The longest matching initial
 * substring.  This returned value is useful in applications that use
 * get-frames-matching to implement a completion facility, or prompt
 * users for frames (  if no matches are found).
 *  
 *   Wildcards-allowed-p 
 * --- When  , the pattern may contain * (zero or more characters)
 * and ? (exactly one character) wildcards.  Wildcard characters are
 * escaped with the backslash character.  If this argument is  ,
 * the * and ? characters simply denote themselves and need not be
 * escaped.
 *   Selector 
 * --- May be a procedure (see Section  ) of
 * signature
 * (candidate-name, kb, kb-local-only-p)
 * that returns   if the candidate name is to be
 * accepted and   otherwise, or one of the following keywords:
 *  
 *      :all  -- Select all frames
 *      :class  -- Select only class frames
 *      :individual  -- Select only individual frames
 *      :slot  -- Select only slot frames
 *      :facet  -- Select only facet frames
 *  
 *   Force-Case-Insensitive-P 
 * --- When  , cause the comparison is to be case-insensitive,
 * irrespective of the IO syntax of the KB.
 */
  protected Values2 get_frames_matching_internal
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frames_matching_fname,
                  pattern,
                  this,
                  wildcards_allowed_p,
                  selector,
                  force_case_insensitive_p,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- enumerate-frames-matching

/**
 * Returns an enumerator for the elements returned by
 *   get-frames-matching .
 */
  protected Enumerator enumerate_frames_matching_internal
     (Node pattern,
      Node wildcards_allowed_p,
      Node selector,
      Node force_case_insensitive_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_frames_matching_fname,
                  pattern,
                  this,
                  wildcards_allowed_p,
                  selector,
                  force_case_insensitive_p,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-frames-with-facet-value

/**
 * Returns the set of frames in which the specified facet value is accessible
 * on the specified slot.
 * If the system is unable to find any frame/slot/facet combinations with the
 * specified value,   is returned.
 * This operation allows user interfaces to take users from a value
 * displayed as a facet value on a particular frame/slot to the place
 * that asserted the value.
 */
  protected Values2 get_frames_with_facet_value_internal
     (Node slot,
      Node facet,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frames_with_facet_value_fname,
                  slot,
                  facet,
                  value,
                  this,
                  inference_level,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- get-frames-with-slot-value

/**
 * Returns the set of frames in which the specified slot value is accessible.
 * If the system is unable to find any frame/slot combinations with the
 * specified value,   is returned.
 * This operation allows user interfaces to take users from a value
 * displayed as a slot value on a particular frame to the place that
 * asserted the value.
 */
  protected Values2 get_frames_with_slot_value_internal
     (Node slot,
      Node value,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_frames_with_slot_value_fname,
                  slot,
                  value,
                  this,
                  inference_level,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- enumerate-instance-types

/**
 * Returns an enumerator for the elements returned by
 *   get-instance-types .
 */
  protected Enumerator enumerate_instance_types_internal
     (Node frame,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_instance_types_fname,
                  frame,
                  this,
                  inference_level,
                  number_of_values,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-kb-classes

/**
 * Returns   list-of-classes , a list of the classes in the KB.
 *   Selector  can be one of the following:
 *  
 *      :all  -- Returns all classes
 *      :frames  -- Returns classes that are represented as frames
 *      :system-default  -- Returns either all classes or
 * only class frames, according to which is the KRS's default
 */
  protected Node get_kb_classes_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_kb_classes_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-kb-classes

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-classes .
 */
  protected Enumerator enumerate_kb_classes_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_kb_classes_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-kb-direct-children

/**
 * Returns the   list-of-child-kbs  -- that is, the list of KBs that
 * directly include   kb .  Note that certain KB implementations may
 * allow circular inclusion dependencies in KBs.  The semantics of KB
 * inclusion are not specified by OKBC, but where possible, processing can
 * be limited to a particular KB by the use of the   kb-local-only-p 
 * argument.
 */
  protected Node get_kb_direct_children_internal
     () {
     Node args =
        Cons.list(get_kb_direct_children_fname,
                  this);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-kb-direct-children

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-direct-children .
 */
  protected Enumerator enumerate_kb_direct_children_internal
     () {
     Node args =
        Cons.list(enumerate_kb_direct_children_fname,
                  this);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-kb-direct-parents

/**
 * Returns the   list-of-parent-kbs  -- that is, the list of KBs directly
 * included by   kb .  Note that certain KB implementations may allow
 * circular inclusion dependencies in KBs.  The semantics of KB inclusion
 * are not specified by OKBC, but where possible, processing can be limited
 * to a particular KB by the use of the   kb-local-only-p  argument.
 */
  protected Node get_kb_direct_parents_internal
     () {
     Node args =
        Cons.list(get_kb_direct_parents_fname,
                  this);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-kb-direct-parents

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-direct-parents .
 */
  protected Enumerator enumerate_kb_direct_parents_internal
     () {
     Node args =
        Cons.list(enumerate_kb_direct_parents_fname,
                  this);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-kb-facets

/**
 * Returns the   list-of-facets  in   kb .
 *   Selector  can be one of the following:
 *  
 *      :all  -- Returns all facets
 *      :frames  -- Returns facets that are represented as frames
 *      :system-default  -- Returns either all facets or
 * only facets represented as frames, according to which is the KRS's
 * default
 */
  protected Node get_kb_facets_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_kb_facets_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-kb-facets

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-facets .
 */
  protected Enumerator enumerate_kb_facets_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_kb_facets_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- enumerate-kb-frames

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-frames .
 */
  protected Enumerator enumerate_kb_frames_internal
     (TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_kb_frames_fname,
                  this,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-kb-individuals

/**
 * Returns   list-of-individuals , a list of the individual frames in
 *   kb .    Selector  can be one of the following:
 *  
 *      :all  -- Returns all accessible individuals
 *      :frames  -- Returns only individuals that are frames
 *      :system-default  -- Returns either all individuals or
 * only individual frames, according to which is the KRS's default
 */
  protected Node get_kb_individuals_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_kb_individuals_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-kb-individuals

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-individuals .
 */
  protected Enumerator enumerate_kb_individuals_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_kb_individuals_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-kb-roots

/**
 * Every KB has one or more frames at the top (root) of the
 * KB.  A frame   is a root of the KB   if there exists no class   such
 * that   is a superclass of   and   is in the KB   and if there
 * exists no class   such that   is a type of   and   is in the
 * KB  , or available in   when   kb-local-only-p  is  .  This
 * operation identifies and returns those roots, the   list-of-roots .
 * Note that this means that unparented individuals, slots and facets will
 * also be returned.
 * 
 * Some KRSs allow    user-defined  classes to be roots of a KB, whereas
 * other KRSs always import certain    system-defined  classes (for example,
 *    thing ) into each KB and force all
 * user classes to be subclasses of    thing.  These system-defined classes
 * may normally be invisible to the user in some KRSs.  The
 *   selector  argument controls which root classes are returned as
 * follows:
 * 
 *  
 *      selector = :all  returns all the true root classes of the
 * KB, regardless of whether they are    system-defined  or
 *    user-defined .
 *      selector = :user  returns the user-defined
 * root classes of the KB, namely all classes   available in the KB such that
 *   was defined by a user application as opposed to being a built-in
 * part of every KB, and such that there exists no class   that is both
 *    user-defined  and a superclass of  .  That is, there may
 * exist    system-defined  superclasses of  .
 *  
 * 
 * If   kb-local-only-p  is  , the list returned may return only
 * the root classes defined in   kb  itself; classes that were
 * inherited from other (included) KBs may be excluded.  This means
 * that a class that has superclasses in some KB included by   kb ,
 * but has no superclasses defined in   kb , may be returned as a
 * root class if   kb-local-only-p  is  .
 */
  protected Node get_kb_roots_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_kb_roots_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-kb-roots

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-roots .
 */
  protected Enumerator enumerate_kb_roots_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_kb_roots_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-kb-slots

/**
 * Returns the   list-of-slots  that are defined in the KB.
 *   Selector  can be one of the following:
 *  
 *      :all  -- Returns all slots
 *      :frames  -- Returns slots that are represented frames
 *      :system-default  -- Returns either all slots or
 * only slot frames, according to which is the KRS's default
 */
  protected Node get_kb_slots_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_kb_slots_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-kb-slots

/**
 * Returns an enumerator for the elements returned by
 *   get-kb-slots .
 */
  protected Enumerator enumerate_kb_slots_internal
     (Node selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_kb_slots_fname,
                  this,
                  selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- enumerate-kbs-of-type

/**
 * Returns an enumerator for the elements returned by
 *   get-kbs-of-type .
 */
  protected Enumerator enumerate_kbs_of_type_internal
     (Connection connection) {
     Node args =
        Cons.list(enumerate_kbs_of_type_fname,
                  this,
                  Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION"));
     Enumerator result = (Enumerator)(((NetworkConnection)connection).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-procedure

/**
 * Returns the   procedure  that is the procedure association for
 * the   name , or   if there is no such procedure association.
 * See   register-procedure ,   unregister-procedure , and
 *   call-procedure .
 */
  protected Node get_procedure_internal
     (Node name) {
     Node args =
        Cons.list(get_procedure_fname,
                  name,
                  this);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-slot-facets

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-facets .
 */
  protected Enumerator enumerate_slot_facets_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_slot_facets_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- get-slot-type

/**
 * Returns one of     :own ,    :template ,    to
 * identify the   slot-type  of the slot on question.  If there are both
 * an own and a template slot on   frame  identified by   slot , then
 *    :own  is returned.
 * If no such slot is known, then   is returned.
 */
  protected Node get_slot_type_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_slot_type_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- get-slot-value

/**
 * Returns the single member of the set of values
 * of the   slot .  This operation is meaningful only for single-valued
 * slots.  It is an error to call   get-slot-value  on a non-single-valued
 * slot, and implementations should signal a   cardinality-violation  if
 * this occurs.  When there is no value for the slot,   value-or-false 
 * is  .
 */
  protected Values2 get_slot_value_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_slot_value_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- get-slot-values

/**
 * Returns the   list-of-values  of   slot 
 * within   frame .  If the    :collection-type  of the slot is
 *    :list , and only    :direct  own slots have been asserted, then
 * order is preserved; otherwise, the values are returned in no guaranteed
 * order.    Get-slot-values  always returns a list of
 * values.  If the specified slot has no values,   is
 * returned.
 */
  protected Values3 get_slot_values_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(get_slot_values_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- enumerate-slot-values

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-values .
 */
  protected Enumerator enumerate_slot_values_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_slot_values_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- enumerate-slot-values-in-detail

/**
 * Returns an enumerator for the elements returned by
 *   get-slot-values-in-detail .
 */
  protected Enumerator enumerate_slot_values_in_detail_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      Node number_of_values,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_slot_values_in_detail_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  number_of_values,
                  value_selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- individual-p

/**
 * Returns   if   thing  identifies an individual entity, and
 * returns   if   thing  identifies a class.
 */
  protected Node individual_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(individual_p_fname,
                  thing,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- instance-of-p

/**
 * Returns   if   thing  is an instance of   class , otherwise
 * returns  .
 */
  protected Values2 instance_of_p_internal
     (Node thing,
      Node the_class,
      InferenceLevel inference_level,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(instance_of_p_fname,
                  thing,
                  the_class,
                  this,
                  inference_level,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- kb-modified-p

/**
 * Returns   if   kb  has been modified since it was last saved.
 */
  protected Node kb_modified_p_internal
     () {
     Node args =
        Cons.list(kb_modified_p_fname,
                  this);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- add-class-superclass

/**
 * Adds the   new-superclass  to the superclasses of   class .
 * Returns no values.
 */
  protected void add_class_superclass_internal
     (Node the_class,
      Node new_superclass,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(add_class_superclass_fname,
                  the_class,
                  new_superclass,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- add-facet-value

/**
 * If the specified facet does not already contain   value ,
 * then   value  is added to the set of values of the facet.
 * Returns no values.
 */
  protected void add_facet_value_internal
     (Node frame,
      Node slot,
      Node facet,
      Node value,
      Node test,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(add_facet_value_fname,
                  frame,
                  slot,
                  facet,
                  value,
                  this,
                  test,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- add-instance-type

/**
 * Adds the   new-type  to the types of   frame .
 * Returns no values.
 */
  protected void add_instance_type_internal
     (Node frame,
      Node new_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(add_instance_type_fname,
                  frame,
                  new_type,
                  this,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- add-slot-value

/**
 * Value  is added to the set of values of   slot .  If the
 * collection-type of   slot  is    :set , then   value  is
 * added only if   slot  does not already contain   value .
 *   Add-before , if supplied, should be   or a nonnegative
 * integer.  If the collection-type of
 *   slot  is    :list ,   value  is added immediately before
 * the value whose index is   add-before .  For example, if
 *   add-before  is 1, the new value will be added between the first
 * and second old values.  If   add-before  is greater than or equal
 * to the current number of slot values, or is  , and the
 * collection-type of   slot  is    :list , then   value  is
 * added after all other values.  This operation may signal constraint
 * violation conditions (see Section  ).  It is an error
 * to provide a nonpositive integer as a value for   add-before .
 * Returns no values.
 */
  protected void add_slot_value_internal
     (Node frame,
      Node slot,
      Node value,
      Node test,
      SlotType slot_type,
      Node add_before,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(add_slot_value_fname,
                  frame,
                  slot,
                  value,
                  this,
                  test,
                  slot_type,
                  add_before,
                  value_selector,
                  kb_local_only_p);
     ((NetworkConnection)this.connection()).makeNetworkCall(args);
  }


// ---------------------- ask

/**
 * Asks a   query  of the OKBC   kb .    Query  may be any
 * sentence in the OKBC Assertion Language that is   askable .  A
 *   cannot-handle  error
 * may be signaled if it is not   askable .   Reply-pattern  is an
 * expression mentioning KIF variables contained in   query .
 * 
 *   Reply-pattern  is any list structure mentioning the variables
 * in the query, or just the name of a variable.  For example, consider
 * a query that is a sentence of the form,
 *   
 * (subclass-of ?x ?y)
 *  
 * that is, find me the things that are subclasses of other things.  If there
 * is a match in the KB for    ?x = human  and    ?y = animal .
 * -- the class    human  is a subclass of the class    animal  -- then
 * if the   reply-pattern  were to be
 *   
 * (superclass-of ?y ?x)
 *  
 * we would be returned a list of sentences of which one
 * would be    (superclass-of animal human) .  The explicit use of a reply
 * pattern in this manner allows the user to get either sentences
 * that can be conveniently reasserted using   tell , or tuples of
 * matches in a shape that is convenient to the application.
 * 
 * When   error-p  is  , any errors resulting from the execution of
 * the query are signaled.  When error-p is  , all possible attempts are
 * made to continue with the query and deliver as many results as were
 * requested.
 * 
 * If the resources used by   ask  are a concern, the time (in
 * seconds) allowed to answer a query will be limited, if possible, as
 * specified by   timeout .  If the value of   timeout  is
 *  , an unlimited time is allowed for   ask  to complete.
 * 
 * The   where  clause can be used to specify a list of bindings to
 * be used for any variables appearing in the   query .  During query
 * evaluation, such variables are replaced by the values specified by
 *   where .  A valid value of   where  is a list of 2-tuples,
 * with each tuple consisting of a variable and value pair.
 * 
 *   Ask  returns three values.
 *   
 *     reply-pattern-list  -- In this list, each element is an
 * instance of   reply-pattern , with all variables mentioned in
 *   query  substituted.
 *     exact-p  -- This has its normal interpretation.
 *     more-status  -- This has its normal interpretation, except
 * that an additional option    :timeout  may be returned for the
 * more-status value by   ask  if the call terminates because
 * execution time exceeds the limit specified by the   timeout 
 * argument.
 *  
 * When   ask  is given a syntactically invalid   query , it
 * signals the   syntax-error  error.  When   ask  realizes that
 * the   query  cannot be handled by the KRS, it signals a
 *   cannot-handle  error.
 * 
 * The following query matches four channel oscilloscopes with a
 * bandwidth greater than 80MHz.  It returns a list of pairs   
 * (?osc ?bandwidth)  satisfying the query.
 *   
 * (ask '(and (oscilloscope ?osc)
 * (number-of-channels ?osc ?chans)
 * (= ?chans 4)
 * (bandwidth ?osc ?bandwidth)
 * (> ?bandwidth (* 80 mega-hertz)))
 * :reply-pattern '(?osc ?bandwidth)
 * :number-of-values 10 :kb kb)
 *  
 * All KIF operators in the   query  are parsed in a
 * package-insensitive manner.  For example,    (and A B)  and   
 * (:and A B)  have the same effect.  Object, relation, and function constant
 * references in   query  are taken as arguments to
 *   get-frames-matching .  Frame references in the query must uniquely
 * identify frames.  (See   get-frames-matching .)
 */
  protected Values3 ask_internal
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(ask_fname,
                  query,
                  this,
                  reply_pattern,
                  inference_level,
                  number_of_values,
                  error_p,
                  where,
                  timeout,
                  value_selector,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- enumerate-ask

/**
 * Returns an enumerator for the elements returned by
 *   ask .
 */
  protected Enumerator enumerate_ask_internal
     (Node query,
      Node reply_pattern,
      InferenceLevel inference_level,
      Node number_of_values,
      TruthValue error_p,
      Node where,
      Node timeout,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(enumerate_ask_fname,
                  query,
                  this,
                  reply_pattern,
                  inference_level,
                  number_of_values,
                  error_p,
                  where,
                  timeout,
                  value_selector,
                  kb_local_only_p);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- askable

/**
 * The   askable  operation returns   if the KRS can
 * determine that   ask ing the   sentence  would result in a
 *   cannot-handle  error being signaled, and   otherwise.
 * It may also signal the   syntax-error  condition.  Even if
 *   askable  returns  ,
 *   ask  may still not be able to handle the   sentence .
 */
  protected Node askable_internal
     (Node sentence,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(askable_fname,
                  sentence,
                  this,
                  value_selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- call-procedure

/**
 * Returns the   value  resulting from applying
 *   procedure  to
 *   arguments .  See section   for a definition of
 * procedures.
 */
  protected Node call_procedure_internal
     (Node procedure,
      Node arguments) {
     Node args =
        Cons.list(call_procedure_fname,
                  procedure,
                  this,
                  arguments);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- enumerate-call-procedure

/**
 * Returns an enumerator for the elements returned by
 *   call-procedure .
 */
  protected Enumerator enumerate_call_procedure_internal
     (Node procedure,
      Node arguments) {
     Node args =
        Cons.list(enumerate_call_procedure_fname,
                  procedure,
                  this,
                  arguments);
     Enumerator result = (Enumerator)(((NetworkConnection)this.connection()).makeNetworkCall(args).car()); // enumerator
     return result;
  }


// ---------------------- coerce-to-class

/**
 * Coerces   thing  to a class.  This operation returns two values.
 *  
 *     class  -- If   thing  identifies a class for   kb ,
 * then this value is the class so identified, or
 *   otherwise.
 *     class-found-p  -- If the class is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the class is not found, then a
 *   class-not-found  error is signaled.
 * 
 * It is an error to call   coerce-to-class  with   error-p  being
 *  , and with a value of   thing  that does not uniquely identify
 * a class.  If this happens, a   not-unique-error  error should be
 * signaled.
 * 
 * Note that in some KRS,   may be a valid class.  No portable
 * program may assume that a returned value of   for the first
 * (  class ) returned value implies that   class-found-p 
 * is  .
 */
  protected Values2 coerce_to_class_internal
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(coerce_to_class_fname,
                  thing,
                  this,
                  error_p,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- coerce-to-facet

/**
 * Coerces   thing  to a facet.  This operation returns two values.
 *  
 *     facet  -- If   thing  identifies a facet for   kb ,
 * then this value is the facet so identified, or
 *   otherwise.
 *     facet-found-p  -- If the facet is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the facet is not found, then a
 *   slot-not-found  error is signaled.
 * 
 * It is an error to call   coerce-to-facet  with   error-p  being
 *  , and with a value of   thing  that does not uniquely identify
 * a facet.  If this happens, a   not-unique-error  error should be
 * signaled.
 * 
 * Note that in some KRS,   may be a valid facet.  No portable
 * program may assume that a returned value of   for the first
 * (  facet ) returned value implies that   facet-found-p  is
 *  .
 */
  protected Values2 coerce_to_facet_internal
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(coerce_to_facet_fname,
                  thing,
                  this,
                  error_p,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- coerce-to-individual

/**
 * Coerces   thing  to an individual.  This operation returns two values.
 *  
 *     individual  -- If   thing  identifies an individual for
 *   kb , then this value is the individual so identified, or
 *   otherwise.
 *     individual-found-p  -- If the individual is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the individual is not found, then a
 *   individual-not-found  error is signaled.
 * 
 * It is an error to call   coerce-to-individual  with   error-p 
 * being  , and with a value of   thing  that does not uniquely
 * identify an individual.  If this happens, a   not-unique-error  error
 * should be signaled.
 * 
 * Note that in most KRS,   is a valid individual.  No portable
 * program may assume that a returned value of   for the first
 * (  individual ) returned value implies that   individual-found-p 
 * is  .
 */
  protected Values2 coerce_to_individual_internal
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(coerce_to_individual_fname,
                  thing,
                  this,
                  error_p,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- coerce-to-kb-value

/**
 * This operation is called by applications that receive input, often from a
 * user in the form of typed input, or a value.    Coerce-to-kb-value 
 * takes this input and delivers a value that is meaningful to the KRS.  This
 * allows applications to interact with users and prompt for expressions
 * containing frame references in a manner that will work predictably across
 * implementation languages, and in networked implementations.
 *   Coerce-to-kb-value  implements OKBC's reading model just as
 *   value-as-string  implements OKBC's printing model.
 * 
 *   string-or-value  may be one of the following.
 *  
 *   an arbitrary OKBC value entity -- If this is a list, then the
 * coercion process applies recursively to the elements of the list.
 * For example, if in the KB the symbol    fred  is coercible to the
 * frame     <frame FRED 763736> , the value    (a fred 42) 
 * would be coerced to the KB value
 *    (a  <frame FRED 763736> 42) .
 *   a string -- This must be the printed representation of an OKBC entity,
 * possibly containing wildcards.  For example, the string
 *    "(a fred 42)"  would be coerced to the same KB value as in
 * the example above.
 *  
 * 
 * Note that there is an asymmetry in the way that arguments are handled,
 * in that a string as the value of   string-or-value  will   always 
 * be interpreted as a pattern from which to read a value, i.e. it will not
 * be interpreted as an OKBC value entity.  This is useful in coercing strings
 * into KB values.  However, this means that applications that use
 *   coerce-to-kb-value  as a way to read in values entered by the user
 * must provide string quotes in order to get a string-valued result.  Thus,
 * to get the string "fred" returned by   coerce-to-kb-value  the
 * value of the   string-or-value  argument would have to be
 * " fred ".  This problem arises if a user interface application
 * is using a prompting model (such as the :string presentation type in CLIM)
 * that elicits a string from the user, perhaps because it already knows that
 * the value-type has to be a string.  If it does this, it will have to
 * quote the string before it passes it to   coerce-to-kb-value  or, of
 * course, not pass it to   coerce-to-kb-value  at all.
 * 
 * Given a   string-or-value  and a   target-context , returns three
 * values.
 *   
 *     result-of-read  -- the result of reading from the string or
 * value, interpreting objects in the   target-context  for the
 *   kb 
 *     success-p  --   if an error occurred during the
 * coercion, and   otherwise
 *     completions-alist  -- an association list of possible
 * completions
 *  
 * The first value returned (  result-of-read ) will be an entity such
 * as a string, number, symbol, list (possibly containing other such values),
 * or frame.
 * 
 *   Target-context  is one of     :frame ,    :class ,
 *    :slot ,    :individual ,    :facet ,    :value   and
 * identifies the way in which the value to be extracted from
 *   string-or-value  is to be interpreted.
 *  
 *      :frame  -- It is to be used as a   frame  argument to
 * an OKBC operation.  It will be expected to resolve to a frame.
 *      :slot  -- It is to be used as a   slot  argument to
 * an OKBC operation.  It will be expected to resolve to a slot name if slots
 * are not represented as frames in   kb , or to a slot frame if slots
 * are represented as frames.
 *      :facet  -- It is to be used as a   facet  argument to
 * an OKBC operation.  It will be expected to resolve to a facet name if facets
 * are not represented as frames in   kb , or to a facet frame if facets
 * are represented as frames.
 *      :class  -- It is to be used as a   class  argument to
 * an OKBC operation.  It will be expected to resolve to a class name if classes
 * are not represented as frames in   kb , or to a class frame if classes
 * are represented as frames.
 *      :individual  -- It is to be used as an   individual 
 * argument to an OKBC operation.  It will be expected to resolve to an
 * individual, which may or may not be a frame.
 *      :value  -- it is to be used as a   value  argument to
 * an OKBC operation, such as   put-slot-value .
 *  
 * The   frame-action  argument
 * controls how the reading process interprets entities that can be interpreted
 * as frames.  The   result-of-read  value is   if an
 * error occurs.  The third value returned (  completions-alist ) is
 *   if an error occurs, or otherwise is an association list of
 * the form
 *   
 * ((<<string1>> <<substring1>> <<frame1>> <<frame2>>... <<frameN>>)
 * (<<string2>> ....) ...)
 *  
 * where    <<stringX>>  are strings found in   string-or-value 
 * that match the frames   <<frame1>> ... <<frameN>> 
 * (possibly by using any specified wildcards), and   <<substringX>> 
 * are the corresponding
 * longest matching initial substrings for each   <<stringX>>  (see the
 * specification of   get-frames-matching ).
 *  
 *     Wildcards-allowed-p 
 * --- has the same meaning as in   get-frames-matching .  Wildcards are
 * interpreted piecewise for the components extracted from
 *   string-or-value .  Thus,    "(fr* j*)"  and
 *    ("fr*" "j*")  both denote a list
 * expression with two wildcarded components, and would match
 *   (fred joe) .
 *     Force-Case-Insensitive-P 
 * --- when   causes frame identification comparison to be
 * case-insensitive, irrespective of the preferred case sensitivity of the
 *   KB  itself.
 *     Error-p 
 * --- when   will signal a   kb-value-read-error  error if a
 * problem arises during the reading process, for example, due to
 * mismatched parentheses.
 *    Frame-action 
 * --- is a keyword from the following list:
 *  
 *      :error-if-not-unique 
 * --- If any substring is found that matches more than one frame then
 * signal a   not-unique-error  error.
 *      :do-not-coerce-to-frames 
 * --- Substrings of   string-or-value  (if a string), or strings
 * and symbols in   string-or-value  (if a nonstring) that
 * match frames are not converted into frames, but may be mapped
 * into strings or symbols.
 *      :must-name-frames 
 * --- Any symbol or string value must be coercible to a frame.  If it
 * is not, a   not-coercible-to-frame  error is signaled.
 *      :options-if-not-unique 
 * --- For each ambiguous frame reference in   string-or-value ,
 * give the possible matches in an entry in the
 *   completions-alist  returned value.
 *  
 *  
 * For example, if in a KB there are frames called    "FRED" ,
 *    "FREDDY" , and    "FRESH"  and the call
 *   
 * (coerce-to-kb-value "fr*" :frame-action :options-if-not-unique)
 *  
 * is made, the values returned would be
 *   
 *      --- The coercion could not complete because of
 * the ambiguity.
 *     --- The operation completed without error.
 *      (("FR*" "FRE" FRED FREDDY FRESH))  --- Only one ambiguous
 * reference was found, and for that the longest matching substring for
 * the pattern    "FR*"  is    "FRE" , and the matching frames
 * are  FRED, FREDDY, FRESH .
 *  
 * See also   get-frames-matching , which is called to identify frames.
 */
  protected Values3 coerce_to_kb_value_internal
     (Node string_or_value,
      ContextName target_context,
      Node wildcards_allowed_p,
      Node force_case_insensitive_p,
      TruthValue error_p,
      Node frame_action,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(coerce_to_kb_value_fname,
                  string_or_value,
                  target_context,
                  this,
                  wildcards_allowed_p,
                  force_case_insensitive_p,
                  error_p,
                  frame_action,
                  kb_local_only_p);
     Values3 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues3(); // multiple-values
     return result;
  }


// ---------------------- coerce-to-slot

/**
 * Coerces   thing  to a slot.  This operation returns two values.
 *  
 *     slot  -- If   thing  identifies a slot for   kb ,
 * then this value is the slot so identified, or
 *   otherwise.
 *     slot-found-p  -- If the slot is found then  ,
 * otherwise  .
 *  
 * If   error-p  is   and the slot is not found, then a
 *   slot-not-found  error is signaled.
 * 
 * It is an error to call   coerce-to-slot  with   error-p  being
 *  , and with a value of   thing  that does not uniquely identify
 * a slot.  If this happens, a   not-unique-error  error should be
 * signaled.
 * 
 * Note that in some KRS,   may be a valid slot.  No portable
 * program may assume that a returned value of   for the first
 * (  slot ) returned value implies that   slot-found-p  is  .
 */
  protected Values2 coerce_to_slot_internal
     (Node thing,
      TruthValue error_p,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(coerce_to_slot_fname,
                  thing,
                  this,
                  error_p,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- coercible-to-frame-p

/**
 * Returns   when   thing  can be coerced to a frame
 * by using   coerce-to-frame , and otherwise returns  .
 */
  protected Node coercible_to_frame_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(coercible_to_frame_p_fname,
                  thing,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- create-frame

/**
 * Creates a new frame called   name  of type   frame-type .
 *   Frame-type  is one of     :class ,    :slot ,    :facet ,
 *    :individual  .  A call to   create-frame  is equivalent to
 * a call to one of   create-class ,   create-individual ,
 *   create-slot , or   create-facet  passing through the appropriate
 * arguments, depending on the value of   frame-type .  If
 *   frame-type  is either    :slot  or    :facet , the slot
 * (or facet) created will have unconstrained domains.
 * 
 * If the    :frame-names-required  behavior has the value   for
 *   kb ,   new-name  may be  .  If the
 *    :frame-names-required  behavior is   for   kb ,
 *   new-name  must uniquely name the new frame, and a
 *   frame-already-exists  error will be signaled if   new-name 
 * is coercible to an existing frame.
 * 
 *   Direct-types  is a list of classes (or class) of which this new
 * frame is to be a direct instance.
 *   Direct-superclasses  is a list of classes (or class) of which the
 * new frame is to be a direct subclass.
 *   Doc , if specified, is a string documenting the new frame.
 *   Pretty-name  is the pretty-name of the new frame.  Returns
 *   new-frame , which identifies the newly created frame.
 * 
 *   Template-slots  and   own-slots  each take a list of slot
 * specifications.  A slot specification assigns a set of values to a
 * slot.  The syntax of a slot specification is
 *   
 * slot-spec ::= (slot slot-value-spec*)
 * slot-value-spec ::= default-slot-value   slot-value
 * default-slot-value ::= (:default slot-value)
 *  
 * where    slot  identifies a slot, or names a slot to be created.  If
 *    slot  already exists, it is simply attached to the new frame, if
 * it does not currently exist, it is created and attached to the new frame.
 * Each    slot-value  is an entity suitable as a value of
 * the specified slot.  Default slot values are identified by appearing in
 * a list whose first element is    :default .  Template slots are only
 * allowed for class frames -- that is, when   frame-type  is
 *    :class .
 * 
 *   Template-facets  and   own-facets  each take a list of facet
 * specifications, which can assign a set of facet values.  A facet
 * specification has the form:
 *   
 * facet-spec ::= (slot fspec*)
 * fspec ::= (facet facet-value-spec*)
 * facet-value-spec ::= default-facet-value   facet-value
 * default-facet-value ::= (:default facet-value)
 *  
 * where    slot  identifies a slot, or names a slot to be created.  If
 *    slot  already exists, it is simply attached to the new frame, if
 * it does not currently exist, it is created and attached to the new frame.
 *    Facet  identifies a facet, or names a facet to be created.  If
 *    facet  already exists, it is simply attached to   slot  on
 * the new frame, if it does not currently exist, it is created and attached
 * to   slot  on the new frame.  Each    facet-value  is an object
 * suitable as a value of the specified facet.  Default facet values are
 * identified by appearing in a list whose first element is    :default .
 * Template facets are allowed only for class frames -- that is, when
 *   frame-type  is    :class .
 * 
 * All slot and facet names in slot and facet specs are defined in a unified
 * namespace that operates across all of the    :own-slots ,
 *    :own-facets ,    :template-slots , and    :template-facets 
 * arguments.  Thus, in the following  example, all occurrences of the slot
 *    s1  and the facet    f1  denote the same slot and facet
 * respectively.
 * 
 * The values specified in slot and facet specifications are interpreted
 * conjunctively.  Thus, in the following example, the slot
 *    s1  will have three values; 42, 100 and 2001, rather than just
 * the value 2001.
 *   
 * (create-frame 'foo :class
 * :own-slots '((s1 42 100)
 * (s1 2001))
 * :own-facets '((s1 (:value-type :integer))
 * (s1 (f1 "Own hello")))
 * :template-facets '((s1 (f1 "Template hello"))))
 *  
 * 
 *   Primitive-p  may be used only when creating a class.
 * When   primitive-p  is  , the KRS will make
 * the class nonprimitive, if possible.
 * 
 *   Handle , if supplied, is a previously allocated frame handle for the
 * new frame to be created.  This is used by network applications, and
 * operations such as   copy-frame  and   copy-kb .  (See
 *   allocate-frame-handle .)  It is an error to supply a value for the
 *   handle  argument that is already coercible to a frame.  If this
 * occurs, a   frame-already-exists  error should be signaled.
 * 
 * Note that if   frame-type  is either    :slot  or    :facet ,
 * then a    frame  might not be created because slots (or facets) might not
 * be represented as frames in   kb .  If this is the case, and slots
 * (or facets) with unconstrained domains are not supported, a
 *   domain-required  error will be signaled.
 * 
 * It is an error to supply   own-slots ,   own-facets  if a frame
 * will not be created, according to the    :are-frames  behavior, and a
 *   not-a-frame-type  error should be signaled.
 */
  protected Node create_frame_internal
     (Node name,
      ContextName frame_type,
      Node direct_types,
      Node direct_superclasses,
      Node doc,
      Node template_slots,
      Node template_facets,
      Node own_slots,
      Node own_facets,
      Node primitive_p,
      Node handle,
      Node pretty_name,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(create_frame_fname,
                  name,
                  frame_type,
                  this,
                  direct_types,
                  direct_superclasses,
                  doc,
                  template_slots,
                  template_facets,
                  own_slots,
                  own_facets,
                  primitive_p,
                  handle,
                  pretty_name,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- create-procedure

/**
 * Defines and returns a procedure in the OKBC procedure language.  The
 * arguments are defined as follows:
 *  
 *     arguments  -- the argument list for the procedure.  The
 * argument list can be expressed in one of three forms.
 *   
 *   A list of symbols
 *   A string that is the printed representation of a list
 * of symbols
 *     -- the null argument list
 *  
 * For example, the argument lists    (a b c) , and
 *    "(a b c)"  are equivalent, as are    "()"  and  .
 * The string representation is provided for language bindings in
 * which it may be inconvenient to create lists of symbols.
 *     body  -- The body for the procedure expressed in the syntax
 * defined in section  .  The body can be
 * provided in one of two forms:
 *   
 *   A    body-form 
 *   A list of    body-form s
 *   A string that is the printed representation of a sequence
 * of    body-form s
 *  
 * For example, the following procedure bodies are equivalent:
 *   
 * ((put-slot-values frame slot values :slot-type :own)
 * (get-slot-value frame slot :slot-type :own))
 *  
 * and
 *   
 * "(put-slot-values frame slot values :slot-type :own)
 * (get-slot-value frame slot :slot-type :own)"
 *  
 * The string representation is provided for language bindings in
 * which it may be inconvenient to create the complex list structure
 * required in the procedure language.
 *     environment  -- A predefined set of bindings between variables
 * mentioned in the procedure   body  and their associated values.
 * The environment is a list of 2-tuples of the form
 *   
 * ((var1 value1)
 * (var2 value2)
 * ....
 * (varn valuen))
 *  
 * where    varN  are the variables mentioned in   body , and
 *    valueN  are the associated values for the variables.
 *  
 * A procedure is a legal argument to any OKBC operator in a position that
 * expects a procedure.
 * For example,
 *   
 * (call-procedure
 * #'(lambda (frame) (get-frame-pretty-name frame :kb kb))
 * :kb kb :arguments (list my-frame))
 *  
 * and
 *   
 * (call-procedure
 * (create-procedure :arguments '(frame)
 * :body '(get-frame-pretty-name frame :kb kb))
 * :kb my-kb :arguments (list my-frame))
 *  
 * are semantically identical.
 * 
 * The main differences between
 * procedures and lambda expressions in Lisp are as follows:
 *  
 *   All bindings in procedures are dynamic, not lexical.
 *   Only a restricted set of operations is available in procedures.
 *   Lambda defines a    lexical  closure over any free references.
 *   procedure  defines a    dynamic  closure over its free
 * references.  The environment of the procedure is
 * prefilled with bindings for the names of the arguments to
 * the OKBC operator in which it is being executed.  In the above
 * case,   call-procedure  takes arguments   KB ,
 *   Arguments , and   Kb-local-only-p 
 * which will take on the values   my-kb ,   (my-frame) , and
 *   nil  (the default), respectively.
 *   Lambda expressions are meaningful only within the Lisp system
 * in which the OKBC system is running.  procedures are
 * executable on any (possibly network-connected) OKBC KB.
 *   procedures are package-insensitive in all respects
 * other than quoted constants.
 *  
 * 
 * Note that persistent side effects to   <<var1>>  and   <<var2>> 
 * cannot be made from within the procedure.  The arguments and variables
 * mentioned in the procedure exist in a different space from the variables
 * in a user program.  The only ways to establish associations between values
 * in a user program and variables in a procedure are through the use of the
 *   environment  argument to   create-procedure , or by the
 *   arguments  argument to   call-procedure .
 */
  protected Node create_procedure_internal
     (Node arguments,
      Node body,
      Node environment) {
     Node args =
        Cons.list(create_procedure_fname,
                  this,
                  arguments,
                  body,
                  environment);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- eql-in-kb

/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * and otherwise returns  .
 *   Arg0  and   arg1  may be any combination of objects coercible
 * to frames.
 */
  protected Node eql_in_kb_internal
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(eql_in_kb_fname,
                  arg0,
                  arg1,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- equal-in-kb

/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * or they are strings containing the same characters (case sensitively), or
 * both are lists with the same structure, and each of the elements
 * recursively is true according to   equal-in-kb .  Returns   
 * otherwise.
 */
  protected Node equal_in_kb_internal
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(equal_in_kb_fname,
                  arg0,
                  arg1,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- equalp-in-kb

/**
 * Returns   iff   arg0  and   arg1 
 * identify the same frame in   kb , or are the same object (==, EQLness),
 * or they are strings containing the same characters (case-insensitively), or
 * both are lists with the same structure, and each of the elements
 * recursively is true according to   equalp-in-kb .  Returns
 *   otherwise.
 */
  protected Node equalp_in_kb_internal
     (Node arg0,
      Node arg1,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(equalp_in_kb_fname,
                  arg0,
                  arg1,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- facet-has-value-p

/**
 * Returns   iff the specified facet has a value for the specified slot
 * and frame, and otherwise returns  .
 */
  protected Values2 facet_has_value_p_internal
     (Node frame,
      Node slot,
      Node facet,
      InferenceLevel inference_level,
      SlotType slot_type,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(facet_has_value_p_fname,
                  frame,
                  slot,
                  facet,
                  this,
                  inference_level,
                  slot_type,
                  value_selector,
                  kb_local_only_p);
     Values2 result = ((NetworkConnection)this.connection()).makeNetworkCall(args).toValues2(); // multiple-values
     return result;
  }


// ---------------------- follow-slot-chain

/**
 * Allows a program to traverse a chain of slot references,
 * gathering own slot values.  For example, imagine that we wish to determine
 * the sisters of the father of the mother of John.  The following two calls
 * accomplish this goal:
 *   
 * (follow-slot-chain 'john '(mother father sisters))
 * 
 * (get-slot-values
 * (get-slot-value
 * (get-slot-value 'john 'mother)
 * 'father)
 * 'sisters)
 *  
 * This operation is complicated by the fact that slots can have multiple
 * values.  For example, imagine that John has two mothers---adopted and
 * biological.  If    union-multiple-values  is   and a slot
 * has more than one value, a   cardinality-violation  error is signaled;
 * if  , then the slot chain becomes a tree, and the union of
 * all values found at the leaves of the tree is returned.
 */
  protected Node follow_slot_chain_internal
     (Node frame,
      Node slot_chain,
      Node union_multiple_values,
      InferenceLevel inference_level,
      ValueSelector value_selector,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(follow_slot_chain_fname,
                  frame,
                  slot_chain,
                  this,
                  union_multiple_values,
                  inference_level,
                  value_selector,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- frame-has-slot-p

/**
 * Returns   iff   slot  is a slot in   frame ,
 * otherwise returns  .
 */
  protected Node frame_has_slot_p_internal
     (Node frame,
      Node slot,
      InferenceLevel inference_level,
      SlotType slot_type,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(frame_has_slot_p_fname,
                  frame,
                  slot,
                  this,
                  inference_level,
                  slot_type,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- frame-in-kb-p

/**
 * Returns   when   thing  is both coercible to
 * a frame, and that frame is known to be resident in   kb , and otherwise
 * returns  .  See   get-frame-in-kb .
 */
  protected Node frame_in_kb_p_internal
     (Node thing,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(frame_in_kb_p_fname,
                  thing,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- frs-independent-frame-handle

/**
 * Given a frame, returns   frame-handle , which is a KRS-independent
 * OKBC frame handle object.    Frame-handle  may now be used in network
 * applications to refer to   frame  or in communication between KBs.
 * The correspondence between   frame  and   frame-handle  is
 * maintained, so that subsequent calls with the same frame will return the
 * same frame-handle.
 * 
 * It is not anticipated that this operation will ever be called by user
 * applications, but must be used by back ends to implement
 *   decontextualize .
 * 
 * Note:  This operation is named   frs-independent-frame-handle  for
 * historical reasons.  Frame Representation Systems are now uniformly
 * called Knowledge Representation Systems with the exception of in the names
 * of this operator and   frs-name .
 */
  protected Node frs_independent_frame_handle_internal
     (Node frame,
      TruthValue kb_local_only_p) {
     Node args =
        Cons.list(frs_independent_frame_handle_fname,
                  frame,
                  this,
                  kb_local_only_p);
     Node result = ((Node)((NetworkConnection)this.connection()).makeNetworkCall(args)).car();
     return result;
  }


// ---------------------- frs-name

/**
 * Returns the   krs-name  of the underlying KRS associated with the
 *   kb-type , which is accessed over   connection .
 *   Krs-name  is a string.  For example,
 * given    loom-kb  as the kb-type, it might return the string
 *    "LOOM" .  This operation is used by user interfaces that need to
 * display a printed representation of the underlying KRS for a particular
 * kb-type.
 * 
 * Note:  This operation is named   frs-name  for
 * historical reasons.  Frame Representation Systems are now uniformly
 * called Knowledge Representation Systems with the exception of in the names
 * of this operator and   frs-independent-frame-handle .
 */
  static protected Node frs_name_internal
     (Kb kb_type,
      Connection connection) {
     Node args =
        Cons.list(frs_name_fname,
                  kb_type,
                  Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION"));
     Node mask = Cons.list();
     Node result = ((Node)((NetworkConnection)connection).makeNetworkCall(args, kb_type)).car();
     return result;
  }



/**
 * Logs you in to Host:port under the specified user-id and creates a
 * session called Session-description owned by Group-ID-for-Session with
 * the specified expected duration.  Returns three values:
 *   - Session key
 *   - Session ID
 *   - A string containing any messages that were emitted whilst creating
 *     the session, such as log messages from scripts being run.
 */
  static final Symbol login_user_and_create_session_fname =
         Symbol.intern("LOGIN-USER-AND-CREATE-SESSION", "HTTP");

  static public SessionResponse login_user_and_create_session
     (String host,
      int    port,
      String user_id,
      String password,
      String group_id_for_session,
      String session_description,
      int expected_duration) {
     NetworkConnection conn =
           KslNetworkConnection.establish_anonymous_connection(host, port);
     Node args =
       Cons.list(login_user_and_create_session_fname,
		 new OKBCString(user_id),
		 new OKBCString(password),
		 new OKBCString(group_id_for_session),
		 new OKBCString(session_description),
		 new OKBCInteger(expected_duration));
     Node values = conn.makeNetworkCall(args);
     return new SessionResponse(values);
   }




/**
 * Logs you in to Host:port under the specified user-id and connects
 * you to the session named by Key-Or-Session-ID.  It is important to
 * use this function when reconnecting, otherwise you'll create a new
 * session and will appear to lose all of your work.  It'll still be
 * there, though. Note that unlike other functions, this one accepts
 * either a session ID or a session key as a legal way to identify
 * the session to which you are going to connect.  This is because
 * the function is password protected, and session keys are user and
 * ip address specific.
 * Returns three values:
 *   - Session key
 *   - Session ID
 *   - A string containing any messages that were emitted whilst
 *     creating the session, such as log messages from scripts being run.
 */
  static final Symbol login_user_and_connect_to_session_fname =
         Symbol.intern("LOGIN-USER-AND-CONNECT-TO-SESSION", "HTTP");

  static public SessionResponse login_user_and_connect_to_session
     (String host,
      int    port,
      String user_id,
      String password,
      String key_or_sid) {
     NetworkConnection conn =
           KslNetworkConnection.establish_anonymous_connection(host, port);
     Node args =
       Cons.list(login_user_and_connect_to_session_fname,
		 new OKBCString(user_id),
		 new OKBCString(password),
		 new OKBCString(key_or_sid));
     Node values = conn.makeNetworkCall(args);
     return new SessionResponse(values);
   }




/**
 * Returns a list of lists (session-id group description-string) for each
 *  of the sessions to which you can connect
 */
  static final Symbol active_sessions_fname =
         Symbol.intern("ACTIVE-SESSIONS", "HTTP");

  static public SessionDescriptor[] active_sessions
     (String host,
      int    port,
      String user_id,
      String password,
      String session_key_to_use_instead_of_password) {
     NetworkConnection conn =
           KslNetworkConnection.establish_anonymous_connection(host, port);
     Node args =
       Cons.list(active_sessions_fname,
		 new OKBCString(user_id),
		 new OKBCString(password),
		 new OKBCString(session_key_to_use_instead_of_password));
     Node values = conn.makeNetworkCall(args);
     Node list = values.car();
     SessionDescriptor[] descriptor_vector
       = new SessionDescriptor[list.length()];
     for(int i=0; i < list.length(); i++) {
       descriptor_vector[i]= new SessionDescriptor((Node)list.nth(i));
     }
     return descriptor_vector;
   }




/**
 * Kills the session named by Session-Key-Or-ID.  If you supply a non-null
 * value for key-to-use-instead_of_password, then you can supply a null string
 * for the password, and this is used instead of session-key-or-id.
 */
  static final Symbol network_kill_session_fname =
         Symbol.intern("KILL-SESSION", "HTTP");

  static public void kill_session
     (String host,
      int    port,
      String user_id,
      String password,
      String key_or_sid,
      String session_key_to_use_instead_of_password) {
     NetworkConnection conn =
           KslNetworkConnection.establish_anonymous_connection(host, port);
     Node args =
       Cons.list(network_kill_session_fname,
		 new OKBCString(user_id),
		 new OKBCString(password),
		 new OKBCString(key_or_sid),
		 new OKBCString(session_key_to_use_instead_of_password));
     conn.makeNetworkCall(args);
   }




/**
 * Returns a list of the group IDs applicable to the current user.
 */
  static final Symbol get_groups_fname =
         Symbol.intern("GET-GROUPS", "HTTP");

  static public String[] get_groups
     (String host,
      int    port,
      String user_id,
      String password,
      String session_key_to_use_instead_of_password) {
     NetworkConnection conn =
           KslNetworkConnection.establish_anonymous_connection(host, port);
     Node args =
       Cons.list(get_groups_fname,
		 new OKBCString(user_id),
		 new OKBCString(password),
		 new OKBCString(session_key_to_use_instead_of_password));
     Node values = conn.makeNetworkCall(args);
     Node list = values.car();
     String[] string_vector = new String[list.length()];
     for(int i=0; i < list.length(); i++) {
       string_vector[i]= ((OKBCString)list.nth(i)).string;
     }
     return string_vector;
   }



}
