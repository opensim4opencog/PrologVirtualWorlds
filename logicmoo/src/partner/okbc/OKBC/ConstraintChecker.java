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

public class ConstraintChecker extends Node {

  public static void enforce_domain_constraint
  (Node frame, Node slot, Kb kb, SlotType slot_type,
   InferenceLevel inference_level, ValueSelector value_selector,
   TruthValue kb_local_only_p)
  {
      Values2 slot_vals = kb.coerce_to_frame(slot, _NIL, kb_local_only_p);
      Node slot_frame = slot_vals.firstValue();
      Node slot_found_p = slot_vals.secondValue();
      Values2 constraint_vals = kb.coerce_to_slot(_domain, _NIL, 
						  kb_local_only_p);
      Node constraint_slot = constraint_vals.firstValue();
      Node found_p = constraint_vals.secondValue();

      if ((slot_found_p!=_NIL) && (found_p!=_NIL) &&
          (kb.frame_has_slot_p(slot_frame, constraint_slot, inference_level, 
                               _own, kb_local_only_p)!=_NIL))
      {
         Node slot_values = kb.get_slot_values(slot_frame, constraint_slot,
                              inference_level, _own, _all, value_selector,
                              kb_local_only_p).firstValue();
         check_slot_constraint_for_slot(frame, slot_frame, _domain, _NIL,
            slot_values, _NIL, _NIL, kb, inference_level, slot_type,
            _either, kb_local_only_p);
      }
  }

  static Node check_slot_constraint_for_slot(
      Node frame, Node slot, Node slot_name, Node constraint_slot,
      Node slot_values, Node current_values, Node future_values, Kb kb,
      InferenceLevel inference_level, Node slot_type, Node value_selector,
      TruthValue kb_local_only_p)
  {

    /*
      System.out.println("check_slot_constraint_for_slot: " +
      Cons.list(frame, slot, slot_name, constraint_slot,
      slot_values, current_values, future_values,
      inference_level, slot_type, value_selector,
      kb_local_only_p));
      */
     if (slot_name == _domain)
     {
        if (slot_type == _own)
        {
           Enumerator e_slot_values = slot_values.enumerate_list();
           e_slot_values.prefetch(_all);
           while (e_slot_values.has_more_p())
           {
              Node slot_value = e_slot_values.next();
              if (!value_matches_type_constraint_p(frame, slot_value,
                  kb, inference_level, kb_local_only_p))
              {
                 throw new ConstraintViolation(new OKBCString("Domain"),
                    frame, slot, slot_type, _NIL, kb, _T,
                    new OKBCString("Frame should match domain constraint "+
                                    slot_value)).signal();
              }
           }
           e_slot_values.free();
        }
	// System.out.println("Future values = " + future_values);
        return future_values;
     }
     else if (slot_name == _documentation)
     {
	System.out.println("Future values = " + future_values);
        return future_values;
     }
     else throw new GenericError
       (_T, new OKBCString
	("Invalid slot_name in check_slot_constraint_for_slot")).signal();
  }

  static boolean matches_union_p(Node thing, Kb kb,
				 TruthValue kb_local_only_p) {
    return (thing instanceof AbstractSymbol &&
	    "UNION".equalsIgnoreCase(((AbstractSymbol)thing).symbolName)) ||
	    (kb.eql_in_kb(thing, _union, kb_local_only_p) != _NIL);
  }
  
  // Since used internally, returns boolean (i.e., 
  // converts all non-nil return values to 'true')
  static boolean value_matches_type_constraint_p(Node value, Node constraint,
     Kb kb, InferenceLevel inference_level, TruthValue kb_local_only_p)
  {
     boolean bMatch;
     if (constraint instanceof Cons)
     {
        Node constraint_car = constraint.car();
        if ((constraint_car instanceof AbstractSymbol) &&
            (((AbstractSymbol)constraint_car).symbolName.equalsIgnoreCase
	     ("SETOF")))
        {
           bMatch = false;
           Enumerator e_vals = constraint.cdr().enumerate_list();
           e_vals.prefetch(_all);
           while (e_vals.has_more_p())
           {
              Node v = e_vals.next();
              if (kb.equal_in_kb(value, v, kb_local_only_p)!=_NIL)
              {
                 bMatch = true; // fall through to free the enumerator
                 break;
              }
           }
           e_vals.free();
           return bMatch;
        }
        else if (matches_union_p(constraint_car, kb, kb_local_only_p))
        {
           bMatch = false;
           Enumerator e_vals = constraint.cdr().enumerate_list();
           e_vals.prefetch(_all);
           while (e_vals.has_more_p())
           {
              Node v = e_vals.next();
              if (value_matches_type_constraint_p(value, v, kb, 
                     inference_level, kb_local_only_p))
              {
                 bMatch = true;
                 break;
              }
           }
           e_vals.free();
           return bMatch;
        }
        else throw new GenericError(_T, new OKBCString(
           "Illegal value "+constraint+" in type constraint")).signal();
     }
     else if (constraint instanceof AbstractSymbol)
     {
        bMatch = false;
        if (constraint == _thing ||
	    kb.eql_in_kb(constraint, _thing, kb_local_only_p) != _NIL)
           return true;
        else if (constraint == _class ||
		 kb.eql_in_kb(constraint, _class, kb_local_only_p) != _NIL)
        {
           if (Cons.member_eql(value, okbc_standard_class_names, kb,
			       kb_local_only_p) ||
               (kb.class_p(value, kb_local_only_p)!=_NIL))
               return true;
           else if (value instanceof Cons)
           {
              Node value_car = value.car();
              if ((value_car instanceof AbstractSymbol) &&
                  (value_car.toString().equalsIgnoreCase(":setof")))
                  return true;
              else if (matches_union_p(value_car, kb, kb_local_only_p))
              {
                 Enumerator e_vals = value.cdr().enumerate_list();
                 e_vals.prefetch(_all);
                 while(e_vals.has_more_p())
                 {
                    Node v = e_vals.next();
                    if (value_matches_type_constraint_p(v, _class, kb,
                       inference_level, kb_local_only_p))
                    {
                       bMatch = true;
                       break;
                    }
                 }
                 e_vals.free();
               }
            }
            return bMatch;
         }
        else if (constraint == _individual ||
		 kb.eql_in_kb(constraint, _individual, kb_local_only_p)
		 != _NIL)
        {
           return !value_matches_type_constraint_p(value, _class,
              kb, inference_level, kb_local_only_p);
        }
        else if (constraint == _slot ||
		 kb.eql_in_kb(constraint, _slot, kb_local_only_p) != _NIL)
        {
           return kb.slot_p(value, kb_local_only_p)!=_NIL;
        }
        else if (constraint == _slot_chain ||
		 kb.eql_in_kb(constraint, _slot_chain, kb_local_only_p)
		 != _NIL)
        {
           if (kb.slot_p(value, kb_local_only_p)!=_NIL)
              return true;
           else if (value instanceof Cons)
           {
              bMatch = true;
              Enumerator e_vals = value.enumerate_list();
              e_vals.prefetch(_all);
              while(e_vals.has_more_p())
              {
                 Node v = e_vals.next();
                 if (kb.slot_p(v, kb_local_only_p)==_NIL)
                 {
                    bMatch = false;
                    break;
                 }
              }
              e_vals.free();
              return bMatch;
           }
           else return false;
        }
        else if (constraint == _number ||
		 kb.eql_in_kb(constraint, _number, kb_local_only_p) != _NIL)
           return (value instanceof Real) || (value instanceof OKBCInteger);
        else if (constraint == _integer ||
		 kb.eql_in_kb(constraint, _integer, kb_local_only_p) != _NIL)
           return (value instanceof OKBCInteger);
        else if (constraint == _string ||
		 kb.eql_in_kb(constraint, _string, kb_local_only_p) != _NIL)
           return (value instanceof OKBCString);
        else if (constraint == _list ||
		 kb.eql_in_kb(constraint, _list, kb_local_only_p) != _NIL)
           return (value instanceof Cons) || (value == _NIL);
     }
     return (kb.instance_of_p(value, constraint, inference_level,
                               kb_local_only_p).firstValue()!=_NIL);
  }

  public static Node enforce_slot_constraints
  (Node frame, Node slot, Node current_values, Node future_values,
   Kb kb, InferenceLevel inference_level, SlotType slot_type,
   ValueSelector value_selector, TruthValue kb_local_only_p)
  {
     check_assertion_of_constraint_slot_values(
        frame, slot, current_values, future_values, kb, inference_level,
        slot_type, value_selector, kb_local_only_p);
     Node facets = kb.get_slot_facets(frame, slot, inference_level,
                     slot_type, kb_local_only_p).firstValue();
     Enumeration e_facet_names = okbc_standard_facet_names.elements();
     while(e_facet_names.hasMoreElements())
     {
        Node facet_name = (Node)e_facet_names.nextElement();
        Values2 coerced = kb.coerce_to_facet(facet_name, _NIL,
					     kb_local_only_p);
        Node facet = coerced.firstValue();
        if ((coerced.secondValue()!=_NIL) &&
	    Cons.member_test(facet, facets, _eql, kb, kb_local_only_p))
        {
           Node facet_values = kb.get_facet_values(frame, slot, facet,
              inference_level, slot_type, _all, value_selector, 
              kb_local_only_p).firstValue();
           future_values = check_slot_constraint_for_facet(
              frame, slot, facet_name, facet, facet_values,
              current_values, future_values, kb, inference_level,
              slot_type, value_selector, kb_local_only_p);
        }
     }
     Node slots = kb.get_frame_slots(slot, inference_level, slot_type,
        kb_local_only_p).firstValue();
     Enumeration e_slot_names = okbc_standard_slot_names.elements();
     while (e_slot_names.hasMoreElements())
     {
        Node slot_name = (Node)e_slot_names.nextElement();
        Values2 coerced = kb.coerce_to_slot(slot_name, _NIL, kb_local_only_p);
        Node constraint_slot = coerced.firstValue();
	/*
	if(slot == Test.foods && slot_name == Node._domain && 
	   frame == Test.harry)
	  { 
	    System.out.println
	      ("enforce_slot_constraints: " +
	       Cons.list(frame, slot, current_values, future_values,
			 inference_level, slot_type, value_selector,
			 kb_local_only_p));
	    System.out.println("coerced = " + coerced);
	    System.out.println("constraint_slot = " + constraint_slot);
	    System.out.println("slots = " + slots);
	    ((TupleKb)kb).debug_node(slot);
	    TupleStore ts = ((TupleKb)kb).tuple_store;
	    Node sl = Symbol.intern("?sl", "OKBC-TEST");
	    Node fr = Symbol.intern("?fr", "OKBC-TEST");
	    Node v = Symbol.intern("?v", "OKBC-TEST");
	    System.out.println
	      ("(sf?v) = " + 
	       ts.hash_index.get_matches(Cons.list(constraint_slot, slot, v)));
	    System.out.println
	      ("(?sf?v) = " + 
	       ts.hash_index.get_matches(Cons.list(sl, slot, v)));
	  }
	  */
        if ((coerced.secondValue()!=_NIL) &&
	    Cons.member_test(constraint_slot, slots, _eql, kb,
			     kb_local_only_p))
        {
           Node slot_values = kb.get_slot_values(slot, constraint_slot,
              inference_level, _own, _all, value_selector, 
              kb_local_only_p).firstValue();
           Node facet = equivalent_constraint_facet(slot_name);
           if (facet!=_NIL)
           {
              future_values = check_slot_constraint_for_facet(
                 frame, slot, facet, _NIL, slot_values, current_values,
                 future_values, kb, inference_level, slot_type,
                 value_selector, kb_local_only_p);
           }
           else
           {
              future_values = check_slot_constraint_for_slot(
                 frame, slot, slot_name, constraint_slot, slot_values,
                 current_values, future_values, kb, inference_level,
                 slot_type, value_selector, kb_local_only_p);
           }
        }
     }
     /*
     if(slot == Test.foods)
       {
	 System.out.println("enforce_slot_constraints future values: " +
			    future_values);
       }
     */
     return future_values;
  }

  static Node check_assertion_of_constraint_slot_values(
     Node frame, Node slot, Node current_values, Node future_values,
     Kb kb, InferenceLevel inference_level, SlotType slot_type,
     ValueSelector value_selector, TruthValue kb_local_only_p)
  {
     Enumeration e_slot_names = okbc_standard_slot_names.elements();
     while (e_slot_names.hasMoreElements())
     {
        Node slot_name = (Node)e_slot_names.nextElement();
        Values2 coerced = kb.coerce_to_slot(slot_name, _NIL, kb_local_only_p);
        Node constraint_slot = coerced.firstValue();
        if ((coerced.secondValue()!=_NIL) &&
	    ((slot == constraint_slot) ||
	     kb.eql_in_kb(slot, constraint_slot, kb_local_only_p) != _NIL))
        {
           Node test_pairs = null;
           if (slot_name==_documentation)
              test_pairs = Cons.list(Cons.list(_slot_value_type, _string));
           else if (slot_name==_slot_value_type || slot_name==_domain)
              test_pairs = Cons.list(Cons.list(_slot_value_type, _class));
           else if (slot_name==_slot_inverse)
              test_pairs = Cons.list(Cons.list(_slot_value_type, _slot));
           else if (slot_name==_slot_cardinality ||
                    slot_name==_slot_maximum_cardinality ||
                    slot_name==_slot_minimum_cardinality)
              test_pairs = Cons.list(Cons.list(_slot_value_type, _integer),
                                     Cons.list(_slot_maximum_cardinality, 
                                               new OKBCInteger(1)),
                                     Cons.list(_slot_numeric_minimum, 
                                               new OKBCInteger(0)));
           else if (slot_name==_slot_same_values ||
                    slot_name==_slot_not_same_values ||
                    slot_name==_slot_subset_of_values)
              test_pairs = Cons.list(Cons.list(_slot_value_type, _slot_chain));
           else if (slot_name==_slot_numeric_minimum ||
                    slot_name==_slot_numeric_maximum)
              test_pairs = Cons.list(Cons.list(_slot_value_type, _number));
           else if (slot_name==_slot_some_values)
              test_pairs = _NIL;
           else if (slot_name==_slot_collection_type)
              test_pairs = Cons.list(Cons.list(_slot_value_type,
                              Cons.list(_setof, _set, _bag, _list)));
           else throw new GenericError
	     (_NIL, new OKBCString("Invalid slot name " + slot_name +
				  " in okbc_standard_slot_names!")).signal();

           Enumeration e_test_pairs = test_pairs.elements();
           while (e_test_pairs.hasMoreElements())
           {
              Node entry = (Node)e_test_pairs.nextElement();
              Node constraint = entry.car();
              Node constraint_values = entry.cdr();
              Node facet = equivalent_constraint_facet(constraint);
              future_values = check_slot_constraint_for_facet(
                 frame, slot, facet, _NIL, constraint_values,
                 current_values, future_values, kb, inference_level,
                 slot_type, value_selector, kb_local_only_p);
           }
        }
     }
     return future_values;
  }

  static Node check_assertion_of_constraint_facet_values(
     Node frame, Node slot, Node facet, Node current_values,
     Node future_values, Kb kb, InferenceLevel inference_level,
     SlotType slot_type, ValueSelector value_selector,
     TruthValue kb_local_only_p)
  {
     Enumeration e_facet_names = okbc_standard_facet_names.elements();
     while (e_facet_names.hasMoreElements())
     {
        Node facet_name = (Node)e_facet_names.nextElement();
        Values2 coerced = kb.coerce_to_facet(facet_name, _NIL,
					     kb_local_only_p);
        Node constraint_facet = coerced.firstValue();
        if ((coerced.secondValue()!=_NIL) &&
	    ((facet == constraint_facet) ||
	     kb.eql_in_kb(facet, constraint_facet, kb_local_only_p) != _NIL))
        {
           Node test_pairs = null;
           if (facet_name==_value_type)
              test_pairs = Cons.list(Cons.list(_value_type, _class));
           else if (facet_name==_inverse)
              test_pairs = Cons.list(Cons.list(_value_type, _slot));
           else if (facet_name==_cardinality)
              test_pairs = Cons.list(Cons.list(_value_type, _integer),
                                     Cons.list(_maximum_cardinality,
                                               new OKBCInteger(1)),
                                     Cons.list(_numeric_minimum,
                                               new OKBCInteger(0)));
           else if (facet_name==_maximum_cardinality ||
                    facet_name==_minimum_cardinality)
              test_pairs = Cons.list(Cons.list(_value_type, _integer),
                                     Cons.list(_numeric_minimum,
                                               new OKBCInteger(0)));
           else if (facet_name==_same_values ||
                    facet_name==_not_same_values ||
                    facet_name==_subset_of_values)
              test_pairs = Cons.list(Cons.list(_value_type, _slot_chain));
           else if (facet_name==_numeric_minimum ||
                    facet_name==_numeric_maximum)
              test_pairs = Cons.list(Cons.list(_value_type, _number));
           else if (facet_name==_some_values)
              test_pairs = _NIL;
           else if (facet_name==_collection_type)
              test_pairs = Cons.list(Cons.list(_value_type, 
                              Cons.list(_setof, _set, _bag, _list)));
           else throw new GenericError
	     (_NIL, new OKBCString("Invalid facet name " + facet_name + 
				  " in okbc_standard_facet_names!")).signal();

           Enumeration e_test_pairs = test_pairs.elements();
           while (e_test_pairs.hasMoreElements())
           {
              Node entry = (Node)e_test_pairs.nextElement();
              Node constraint = entry.car();
              Node constraint_values = entry.cdr();
              future_values = check_slot_constraint_for_facet(
                 frame, slot, constraint, _NIL, constraint_values,
                 current_values, future_values, kb, inference_level,
                 slot_type, value_selector, kb_local_only_p);
           }
        }
     }
     return future_values;
  }

  static void validate_slot_chain_syntax (Node frame, Node slot, 
					  SlotType slot_type, Node facet,
					  Kb kb, Node facet_value)
  { if (facet_value instanceof Cons &&
	((!(facet_value.car() instanceof AbstractSymbol)) ||
	 (!(((AbstractSymbol)(facet_value.car())).symbolName.
	    equalsIgnoreCase ("LISTOF")))))
      { throw new ValueTypeViolation
	(new OKBCString("Value type"),
	 frame, slot, kb, slot_type, facet, _T, new OKBCString
	 (("Slot chain should be either a slot or (listof ...), " +
	   "not " + facet_value))).signal();
      }
  }


  static Node follow_slot_chain_arg (Node facet_value) {
    return Cons.list_if_not((facet_value instanceof Cons
			     ? facet_value.cdr()
			     : facet_value));
  }

   static Node check_slot_constraint_for_facet(
      Node frame, Node slot, Node facet_name, Node facet, Node facet_values,
      Node current_values, Node future_values, Kb kb,
      InferenceLevel inference_level, SlotType slot_type,
      ValueSelector value_selector, TruthValue kb_local_only_p)
   {
      if (facet_name==_inverse)
      {  if (facet_values != _NIL)
	   { if (facet_values.length()>1)
		 throw new CardinalityViolation
		   (new OKBCString("Single valued slot"),
		    frame, slot, slot_type, facet, kb, _T, new
		    OKBCString("Cardinality should be 1.")).signal();
	     Enumerator e = future_values.enumerate_list();
	     Node res = _NIL;
	     while (e.has_more_p()) {
	       Node v = e.next();
	       Values2 coerced = kb.coerce_to_frame(v, _NIL, kb_local_only_p);
	       Node value_frame = coerced.firstValue();
	       if (coerced.secondValue()==_NIL)
		 throw new NotCoercibleToFrame(v, kb).signal();
	       res = new Cons(v, res);
	     }
	     return res;
	   }
         return future_values;
      }
      else if (facet_name==_cardinality)
      {
         Enumerator e_facet_values = facet_values.enumerate_list();
         e_facet_values.prefetch(_all);
         while (e_facet_values.has_more_p())
         {
            Node facet_value = e_facet_values.next();
            if (!((facet_value instanceof OKBCInteger) &&
                (((OKBCInteger)facet_value).number >=0)))
                throw new ValueTypeViolation(new OKBCString("Value type"),
                  frame, slot, kb, slot_type, facet, _T, new OKBCString(
                  "Cardinality should have a nonnegative integer value, "+
                  "not "+facet_value)).signal();
            if (((slot_type==_template) &&
                (future_values.length() <= ((OKBCInteger)facet_value).number))
		||
               ((slot_type==_own) &&
                (future_values.length() == ((OKBCInteger)facet_value).number)))
            { }
            else throw new CardinalityViolation
	      (new OKBCString("Slot cardinality"),
               frame, slot, slot_type, facet, kb, _T, new OKBCString(
               "Cardinality should be "+facet_value)).signal();
         }
         e_facet_values.free();
         return future_values;
      }
      else if (facet_name==_maximum_cardinality)
      {
         Enumerator e_facet_values = facet_values.enumerate_list();
         e_facet_values.prefetch(_all);
         while (e_facet_values.has_more_p())
         {
            Node facet_value = e_facet_values.next();
            if (!((facet_value instanceof OKBCInteger) &&
                  (((OKBCInteger)facet_value).number >=0)))
                  throw new ValueTypeViolation(new OKBCString("Value type"),
                     frame, slot, kb, slot_type, facet, _T, new OKBCString(
                     "Maximum-cardinality should have a nonnegative "+
                     "integer value, not "+facet_value)).signal();
            if (!(future_values.length() <= ((OKBCInteger)facet_value).number))
               throw new CardinalityViolation
		 (new OKBCString("Maximum slot cardinality"),
                  frame, slot, slot_type, facet, kb, _T, new OKBCString(
                  "Cardinality should be at most "+facet_value)).signal();
         }
         e_facet_values.free();
         return future_values;
      }
      else if (facet_name==_minimum_cardinality)
      {
         if (slot_type == _own)
         {
            Enumerator e_facet_values = facet_values.enumerate_list();
            e_facet_values.prefetch(_all);
            while (e_facet_values.has_more_p())
            {
               Node facet_value = e_facet_values.next();
               if (!((facet_value instanceof OKBCInteger) &&
                     (((OKBCInteger)facet_value).number >=0)))
                     throw new ValueTypeViolation(new OKBCString("Value type"),
                        frame, slot, kb, slot_type, facet, _T, new OKBCString(
                        "Minimum-cardinality should have a nonnegative "+
                        "integer value, not "+facet_value)).signal();
               if (!(future_values.length() >= 
		     ((OKBCInteger)facet_value).number))
                  throw new CardinalityViolation
		    (new OKBCString("Minimum slot cardinality"),
                     frame, slot, slot_type, facet, kb, _T, new OKBCString(
                     "Cardinality should be at least "+facet_value)).signal();
            }
	    e_facet_values.free();
         }
         return future_values;
     }
      else if (facet_name==_numeric_minimum)
      {
         Enumerator e_facet_values = facet_values.enumerate_list();
         e_facet_values.prefetch(_all);
         while (e_facet_values.has_more_p())
         {
            Node facet_value = e_facet_values.next();
            Enumerator e_future_values = future_values.enumerate_list();
            e_future_values.prefetch(_all);
            while (e_future_values.has_more_p())
            {
               Node value = e_future_values.next();
               double d_facet_value = 0;
               if (facet_value instanceof OKBCInteger)
                  d_facet_value = ((OKBCInteger)facet_value).number;
               else if (facet_value instanceof Real)
                  d_facet_value = ((Real)facet_value).number;
               else
               {
                  throw new ValueTypeViolation(new OKBCString("Value type"),
                     frame, slot, kb, slot_type, facet, _T, new OKBCString(
                     "Numeric minimum should have a numerical "+
                     "value, not "+facet_value)).signal();
               }
               if (((value instanceof OKBCInteger) &&
                  (((OKBCInteger)value).number >= d_facet_value)) ||
                  ((value instanceof Real) &&
                  (((Real)value).number >= d_facet_value)))
               { }
               else throw new ConstraintViolation
		 (new OKBCString("Numeric Minimum"),
                  frame, slot, slot_type, facet, kb, _T, new OKBCString(
                  "Value should be at least "+facet_value)).signal();
            }
	    e_future_values.free();
         }
	 e_facet_values.free();
         return future_values;
      }
      else if (facet_name==_numeric_maximum)
      {
         Enumerator e_facet_values = facet_values.enumerate_list();
         e_facet_values.prefetch(_all);
         while (e_facet_values.has_more_p())
         {
            Node facet_value = e_facet_values.next();
            Enumerator e_future_values = future_values.enumerate_list();
            e_future_values.prefetch(_all);
            while (e_future_values.has_more_p())
            {
               Node value = e_future_values.next();
               double d_facet_value = 0;
               if (facet_value instanceof OKBCInteger)
                  d_facet_value = ((OKBCInteger)facet_value).number;
               else if (facet_value instanceof Real)
                  d_facet_value = ((Real)facet_value).number;
               else
               {
                  throw new ValueTypeViolation(new OKBCString("Value type"),
                     frame, slot, kb, slot_type, facet, _T, new OKBCString(
                     "Numeric maximum should have a numerical "+
                     "value, not "+facet_value)).signal();
               }
               if (((value instanceof OKBCInteger) &&
                  (((OKBCInteger)value).number <= d_facet_value)) ||
                  ((value instanceof Real) &&
                  (((Real)value).number <= d_facet_value)))
               { }
               else throw new ConstraintViolation
		 (new OKBCString("Numeric Maximum"),
                  frame, slot, slot_type, facet, kb, _T, new OKBCString(
                  "Value should be at most "+facet_value)).signal();
            }
	    e_future_values.free();
         }
	 e_facet_values.free();
         return future_values;
      }
      else if (facet_name==_some_values || facet_name==_collection_type)
      {
         return future_values;
      }
      else if (facet_name==_same_values)
      {
         if (slot_type==_own)
         {
            Enumerator e_facet_values = facet_values.enumerate_list();
            e_facet_values.prefetch(_all);
            while (e_facet_values.has_more_p())
            {
               Node facet_value = e_facet_values.next();
	       validate_slot_chain_syntax(frame, slot, slot_type, facet, kb,
					  facet_value);
               Node chain = kb.follow_slot_chain
		 (frame, follow_slot_chain_arg(facet_value),
		  _T, inference_level, value_selector, kb_local_only_p);
               if (set_xor_equal_in_kb(future_values, chain, kb,
				       kb_local_only_p))
                  throw new ConstraintViolation(new OKBCString("Same values"),
                     frame, slot, slot_type, facet, kb, _T, new OKBCString(
                     "Slot values should be the same as in "+facet_value))
		    .signal();
            }
            e_facet_values.free();
         }
         return future_values;
      }
      else if (facet_name==_not_same_values)
      {
         if (slot_type==_own)
         {
            Enumerator e_facet_values = facet_values.enumerate_list();
            e_facet_values.prefetch(_all);
            while (e_facet_values.has_more_p())
            {
               Node facet_value = e_facet_values.next();
	       validate_slot_chain_syntax(frame, slot, slot_type, facet, kb,
					  facet_value);
               Node chain = kb.follow_slot_chain
		 (frame, follow_slot_chain_arg(facet_value),
		  _T, inference_level, value_selector, kb_local_only_p);
               if (!set_xor_equal_in_kb(future_values, chain, kb,
					kb_local_only_p))
                  throw new ConstraintViolation(new OKBCString("Same values"),
                     frame, slot, slot_type, facet, kb, _T, new OKBCString(
                     "Slot values should not be the same as in "+facet_value))
		    .signal();
            }
            e_facet_values.free();
         }
         return future_values;
      }
      else if (facet_name==_subset_of_values)
      {
         if (slot_type==_own)
         {
            Enumerator e_facet_values = facet_values.enumerate_list();
            e_facet_values.prefetch(_all);
            while (e_facet_values.has_more_p())
            {
               Node facet_value = e_facet_values.next();
               Enumerator e_future_values = future_values.enumerate_list();
               e_future_values.prefetch(_all);
               while (e_future_values.has_more_p())
               {
                  Node value = e_future_values.next();
		  validate_slot_chain_syntax(frame, slot, slot_type, facet, kb,
					     facet_value);
                  Node chain = kb.follow_slot_chain
		    (frame, follow_slot_chain_arg(facet_value),
		     _T, inference_level, value_selector, kb_local_only_p);
                  Enumerator e_chain = chain.enumerate_list();
                  e_chain.prefetch(_all);
                  while (e_chain.has_more_p())
                  {
                     Node the_slot = e_chain.next();
                     if (kb.equal_in_kb(value, the_slot, kb_local_only_p)
			 == _NIL)
                        throw new ConstraintViolation
			  (new OKBCString("Subset of values"),
                           frame, slot, slot_type, facet, kb, _T,
			   new OKBCString(
                           "Slot value should be in the list "+facet_value))
			  .signal();
                  }
                  e_chain.free();
               }
               e_future_values.free();
            }
            e_facet_values.free();
         }
         return future_values;
      }
      else if (facet_name==_value_type)
      {
         Enumerator e_facet_values = facet_values.enumerate_list();
         e_facet_values.prefetch(_all);
         while (e_facet_values.has_more_p())
         {
            Node facet_value = e_facet_values.next();
            Enumerator e_future_values = future_values.enumerate_list();
            e_future_values.prefetch(_all);
            while (e_future_values.has_more_p())
            {
               Node value = e_future_values.next();
               if (!value_matches_type_constraint_p(
                  value, facet_value, kb, inference_level, kb_local_only_p))
                  throw new ConstraintViolation(new OKBCString("Value type"),
                     frame, slot, slot_type, facet, kb, _T, new OKBCString(
                     "Value should match value-type constraint "+facet_value))
		    .signal();
            }
            e_future_values.free();
         }
         e_facet_values.free();
         return future_values;
      }
      else if (facet_name==_documentation_in_frame)
	{ return future_values; }
      else throw new GenericError(_T, new OKBCString(
         "Unsupported facet constraint method for "+facet_name)).signal();
   }

   static Node equivalent_constraint_facet(Node key)
   {
      if (key==_slot_value_type) return _value_type;
      if (key==_slot_inverse) return _inverse;
      if (key==_slot_cardinality) return _cardinality;
      if (key==_slot_maximum_cardinality) return _maximum_cardinality;
      if (key==_slot_minimum_cardinality) return _minimum_cardinality;
		if (key==_slot_same_values) return _same_values;
		if (key==_slot_not_same_values) return _not_same_values;
		if (key==_slot_subset_of_values) return _subset_of_values;
		if (key==_slot_numeric_minimum) return _numeric_minimum;
		if (key==_slot_numeric_maximum) return _numeric_maximum;
		if (key==_slot_some_values) return _some_values;
		if (key==_slot_collection_type) return _collection_type;
      return _NIL;
   }

   // returns true if all elements of the two args are equal_in_kb.
   static boolean set_xor_equal_in_kb
   (Node x, Node y, Kb kb, TruthValue kb_local_only_p)
   {
      Node untested_y = y.toValues().toList(); // copy the list
      Enumerator e_x = x.enumerate_list();
      e_x.prefetch(_all);
      while (e_x.has_more_p())
      {
         boolean bFound = false;
         Node curr_x = e_x.next();
         Enumerator e_y = y.enumerate_list();
         e_y.prefetch(_all);
         while (e_y.has_more_p())
         {
            Node curr_y = e_y.next();
            if (kb.equal_in_kb(curr_x, curr_y, kb_local_only_p)!=_NIL)
            {
               Cons.remove_eql(curr_y, untested_y, kb);
               bFound = true;
               break;
            }
         }
         e_y.free();
         if (!bFound) return false;
      }
      e_x.free();
      Enumerator e_untested_y = untested_y.enumerate_list();
      e_untested_y.prefetch(_all);
      while (e_untested_y.has_more_p())
      {
         boolean bFound = false;
         Node curr_y = e_untested_y.next();
         e_x = x.enumerate_list();
         e_x.prefetch(_all);
         while (e_x.has_more_p())
         {
            Node curr_x = e_x.next();
            if (kb.equal_in_kb(curr_y, curr_x, kb_local_only_p)!=_NIL)
            {
               bFound = true;
               break;
            }
         }
         e_x.free();
         if (!bFound) return false;
      }
      return true;
   }

}
