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


public class ComplianceChecker extends AbstractTester {


  public static void main (String argv[]) {
    Symbol test_name = Symbol.intern("TAXA", "OKBC-TEST");
    Kb kb = Test.create_taxa_kb(false);
    test(kb);
  }

  public static void test (Kb kb) {
    violations = 0;


    try { kb.member_behavior_values_p
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.member_facet_value_p
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.member_slot_value_p
                 (_NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.next
                 (null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.open_kb
                 (_NIL,
                 null,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.openable_kbs
                 (null,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.prefetch
                 (null,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.primitive_p
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.print_frame
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_behavior_values
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_class_superclasses
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_facet_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_facet_values
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_frame_details
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_frame_name
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_frame_pretty_name
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_instance_types
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_slot_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.put_slot_values
                 (_NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.register_procedure
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.remove_class_superclass
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.remove_facet_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.remove_instance_type
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.remove_local_facet_values
                 (_NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.remove_local_slot_values
                 (_NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.remove_slot_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.rename_facet
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.rename_slot
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.replace_facet_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.replace_slot_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.revert_kb
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.save_kb
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.save_kb_as
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.slot_has_facet_p
                 (_NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _own,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.slot_has_value_p
                 (_NIL,
                 _NIL,
                 _direct,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.slot_p
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.subclass_of_p
                 (_NIL,
                 _NIL,
                 _direct,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.superclass_of_p
                 (_NIL,
                 _NIL,
                 _direct,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.tell
                 (_NIL,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.tellable
                 (_NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.type_of_p
                 (_NIL,
                 _NIL,
                 _direct,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.unregister_procedure
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.untell
                 (_NIL,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.value_as_string
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_behavior_supported_values
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_behavior_values
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_class_instances
                 (_NIL,
                 _direct,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_class_subclasses
                 (_NIL,
                 _direct,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_class_superclasses
                 (_NIL,
                 _direct,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_classes_in_domain_of
                 (_NIL,
                 _NIL,
                 _direct,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_facet_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_facet_values
                 (_NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _own,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_facet_values_in_detail
                 (_NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _own,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frame_details
                 (_NIL,
                 _direct,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frame_handle
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frame_in_kb
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frame_name
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frame_pretty_name
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frame_sentences
                 (_NIL,
                 _NIL,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frame_slots
                 (_NIL,
                 _direct,
                 _own,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frame_type
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frames_matching
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frames_with_facet_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_frames_with_slot_value
                 (_NIL,
                 _NIL,
                 _direct,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_instance_types
                 (_NIL,
                 _direct,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_behaviors
                 (null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_classes
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_direct_children
                 (); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_direct_parents
                 (); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_facets
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_frames
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_individuals
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_roots
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_slots
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kb_type
                 (_NIL,
                 null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_kbs_of_type
                 (null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_procedure
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_slot_facets
                 (_NIL,
                 _NIL,
                 _direct,
                 _own,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_slot_type
                 (_NIL,
                 _NIL,
                 _direct,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_slot_value
                 (_NIL,
                 _NIL,
                 _direct,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_slot_values
                 (_NIL,
                 _NIL,
                 _direct,
                 _own,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.get_slot_values_in_detail
                 (_NIL,
                 _NIL,
                 _direct,
                 _own,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.okbc_condition_p
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.has_more
                 (null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.individual_p
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.instance_of_p
                 (_NIL,
                 _NIL,
                 _direct,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.kb_p
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.kb_modified_p
                 (); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.add_class_superclass
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.add_facet_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.add_instance_type
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.add_slot_value
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.all_connections
                 (); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.allocate_frame_handle
                 (_NIL,
                 _class,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.ask
                 (_NIL,
                 _NIL,
                 _direct,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.askable
                 (_NIL,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.attach_facet
                 (_NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.attach_slot
                 (_NIL,
                 _NIL,
                 _own,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.call_procedure
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.class_p
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.close_connection
                 (null,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.close_kb
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.coerce_to_class
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.coerce_to_facet
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.coerce_to_frame
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.coerce_to_individual
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.coerce_to_kb_value
                 (_NIL,
                 _class,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.coerce_to_slot
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.coercible_to_frame_p
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.create_class
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.create_facet
                 (_NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.create_frame
                 (_NIL,
                 _class,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.create_individual
                 (_NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.create_kb_locator
                 (_NIL,
                 null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.create_procedure
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.create_slot
                 (_NIL,
                 _NIL,
                 _own,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.decontextualize
                 (_NIL,
                 _class); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.delete_facet
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.delete_frame
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.delete_slot
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.detach_facet
                 (_NIL,
                 _NIL,
                 _NIL,
                 _own,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.detach_slot
                 (_NIL,
                 _NIL,
                 _own,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.enumerate_list
                 (_NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.eql_in_kb
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.equal_in_kb
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.equalp_in_kb
                 (_NIL,
                 _NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.establish_connection
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.expunge_kb
                 (_NIL,
                 null,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.facet_has_value_p
                 (_NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _own,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.facet_p
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.fetch
                 (null,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.find_kb
                 (_NIL,
                 null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.find_kb_locator
                 (_NIL,
                 null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.follow_slot_chain
                 (_NIL,
                 _NIL,
                 _NIL,
                 _direct,
                 _known_true,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.frame_has_slot_p
                 (_NIL,
                 _NIL,
                 _direct,
                 _own,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.frame_in_kb_p
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.free
                 (null); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    try { kb.frs_independent_frame_handle
                 (_NIL,
                 _NIL); }
    catch (MethodMissing e)
      { System.out.println("Method missing for " + e.okbcop); 
        violations = violations + 1;
      }
    catch (Exception e) {}
    catch (Error e) {}

    System.out.println();
    System.out.println("Finished testing.  Violations: " + violations);
  }
}