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

public class Test extends AbstractTester {

  static Symbol $age              = Symbol.intern("AGE",         "OKBC-TEST");
  static Node age = null;
  static Symbol $animal           = Symbol.intern("ANIMAL",      "OKBC-TEST");
  static Node animal = null;
  static Symbol $bird             = Symbol.intern("BIRD",        "OKBC-TEST");
  static Node bird = null;
  static Symbol $bogus_facet      = Symbol.intern("BOGUS-FACET", "OKBC-TEST");
  static Symbol $bogus_slot       = Symbol.intern("BOGUS-SLOT",  "OKBC-TEST");
  static Node cardinality = null;
  static Symbol $color            = Symbol.intern("COLOR",       "OKBC-TEST");
  static Node color = null;
  static Symbol $elephant         = Symbol.intern("ELEPHANT",    "OKBC-TEST");
  static Node elephant = null;
  static Symbol $fish             = Symbol.intern("FISH",        "OKBC-TEST");
  static Node fish = null;
  static Symbol $food             = Symbol.intern("FOOD",        "OKBC-TEST");
  static Node food = null;
  static Symbol $foods            = Symbol.intern("FOODS",       "OKBC-TEST");
  static Node foods = null;
  static Symbol $dinosaur         = Symbol.intern("DINOSAUR",    "OKBC-TEST");
  static Node dinosaur = null;
  static Symbol $grass            = Symbol.intern("GRASS",       "OKBC-TEST");
  static Node grass = null;
  static Symbol $has_tail         = Symbol.intern("HAS-TAIL",    "OKBC-TEST");
  static Node has_tail = null;
  static Symbol $has_leaves       = Symbol.intern("HAS-LEAVES",  "OKBC-TEST");
  static Node has_leaves = null;
  static Symbol $human            = Symbol.intern("HUMAN",       "OKBC-TEST");
  static Node human = null;
  static Symbol $living_thing     = Symbol.intern("LIVING-THING","OKBC-TEST");
  static Node living_thing = null;
  static Symbol $lizard           = Symbol.intern("LIZARD",      "OKBC-TEST");
  static Node lizard = null;
  static Symbol $mammal           = Symbol.intern("MAMMAL",      "OKBC-TEST");
  static Node mammal = null;
  static Symbol $palm             = Symbol.intern("PALM",        "OKBC-TEST");
  static Node palm = null;
  static Symbol $plant            = Symbol.intern("PLANT",       "OKBC-TEST");
  static Node plant = null;
  static Symbol $reptile          = Symbol.intern("REPTILE",     "OKBC-TEST");
  static Node reptile = null;
  static Symbol $salmon           = Symbol.intern("SALMON",      "OKBC-TEST");
  static Node salmon = null;
  static Symbol $tiger            = Symbol.intern("TIGER",       "OKBC-TEST");
  static Node tiger = null;
  static Symbol $tree             = Symbol.intern("TREE",        "OKBC-TEST");
  static Node tree = null;
  static Symbol $trout            = Symbol.intern("TROUT",       "OKBC-TEST");
  static Node trout = null;
  static Symbol $ted              = Symbol.intern("TED",         "OKBC-TEST");
  static Node ted = null;
  static Symbol $bill             = Symbol.intern("BILL",        "OKBC-TEST");
  static Node bill = null;
  static Symbol $harry            = Symbol.intern("HARRY",       "OKBC-TEST");
  static Node harry = null;
  static Symbol $edward           = Symbol.intern("EDWARD",      "OKBC-TEST");
  static Node edward = null;
  static Symbol $linda            = Symbol.intern("LINDA",       "OKBC-TEST");
  static Node linda = null;
  static Symbol $derek            = Symbol.intern("DEREK",       "OKBC-TEST");
  static Node derek = null;
  static Symbol $sally            = Symbol.intern("SALLY",       "OKBC-TEST");
  static Node sally = null;
  static Symbol $single_valued    = Symbol.intern("SINGLE-VALUED", "OKBC-TEST");
  static Node single_valued = null;
  static Symbol $silly_facet      = Symbol.intern("SILLY-FACET", "OKBC-TEST");
  static Node silly_facet = null;
  static Symbol $trudy            = Symbol.intern("TRUDY",       "OKBC-TEST");
  static Node trudy = null;
  static Symbol $something_edible=Symbol.intern("SOMETHING-EDIBLE","OKBC-TEST");
  static Node something_edible = null;
  static Symbol $seeds            = Symbol.intern("SEEDS",       "OKBC-TEST");
  static Node seeds = null;
  static Symbol $berries          = Symbol.intern("BERRIES",     "OKBC-TEST");
  static Node berries = null;
  static Symbol $fries            = Symbol.intern("FRIES",       "OKBC-TEST");
  static Node fries = null;
  static Symbol $chips            = Symbol.intern("CHIPS",       "OKBC-TEST");
  static Node chips = null;

  static Symbol $default_food_for_animals 
    = Symbol.intern("DEFAULT-FOOD-FOR-ANIMALS", "OKBC-TEST");
  static Node default_food_for_animals = null;
  static Symbol $default_food_for_mammals
    = Symbol.intern("DEFAULT-FOOD-FOR-MAMMALS", "OKBC-TEST");
  static Node default_food_for_mammals = null;

  static Node $_class = null;
  static Node $_thing = null;
  static Node $_string = null;
  static Node $_integer = null;
  static Node $_number = null;
  static Node $_list = null;

  public static void main (String argv[]) {
    if (OKBCApp.argv_suppliedp("HELP", argv))
      { System.out.println("Usage: java OKBC.Test [-option]*");
	System.out.println("where options include:");
	System.out.println("    -break              : listener on errors");
	System.out.println("    -cache              : enable cachine");
	System.out.println("    -quiet              : minimal output");
      }
    else
      { boolean break_p = OKBCApp.argv_suppliedp("BREAK", argv);
	if (break_p) { Condition.listener_on_error_p = true; }
	boolean cache_p = OKBCApp.argv_suppliedp("CACHE", argv);
	boolean verbose_p = !OKBCApp.argv_suppliedp("QUIET", argv);
	violations = 0;
	Symbol test_name = Symbol.intern("TAXA", "OKBC-TEST");
	Kb kb = create_taxa_kb(cache_p);
	kb.goto_kb();

	// Temporary HACK ALERT !!!!!
	kb.put_behavior_values(_frame_names_required, Cons.list(_T));

	new TupleKb(false); // force class loader
	TupleKb kb_as_copy_target = new TupleKb();
	TupleKb tell_and_ask_kb = new TupleKb();
	run_tests(kb, kb_as_copy_target, tell_and_ask_kb, verbose_p);
      }
  }


  public static void initialize_test_kb (Kb kb) {
    initialize_test_kb(kb, false);
  }

  public static void initialize_test_kb (Kb kb, boolean verbose_p) {
    initialize_test_kb(kb, verbose_p, true);
  }

  public static void initialize_test_kb (Kb kb, boolean verbose_p, 
					 boolean slots_and_facets_p) {
    create_classes(kb, verbose_p);
    create_individuals(kb, verbose_p);
    test_class_instance_relationships(kb, verbose_p);
    if (slots_and_facets_p)
      { test_slot_attachment(kb, verbose_p);
	test_slot_values(kb, verbose_p);
	if (Cons.member_eql(_user_defined_facets,
			    kb.get_behavior_values(_compliance), kb, _NIL))
	  { test_facet_attachment(kb, verbose_p);
	    test_facets(kb, verbose_p); 
	  }
      }
  }

  public static Kb create_taxa_kb(boolean cache_p) {
    Symbol test_name = Symbol.intern("TAXA", "OKBC-TEST");
    CachingKb.ensure_registered();
    MinimalNetworkKb.ensure_registered();
    NetworkKb.ensure_registered();
    TupleKb.ensure_registered();
    TupleKb kb = null;
    Kb kb_type = get_kb_type(Symbol.intern("TUPLE-KB", "OK-BACK"));
    kb = (TupleKb)kb_type.create_kb
      (test_name, _NIL, Cons.list(Node._package, new OKBCString("OKBC-TEST"),
				  Node._print_case, Node._downcase),
       LocalConnection.local_connection());
    // kb.ioSyntax = new IoSyntax("OKBC-TEST");
    kb.allow_caching_p = cache_p;
    return kb;
  }

  public static void run_tests (Kb kb, Kb new_kb, Kb tell_and_ask_kb,
				boolean verbose_p) {
       test_kb_finding(kb, verbose_p);
       create_classes(kb, verbose_p);
       create_individuals(kb, verbose_p);
       test_keywordified_frames(kb, verbose_p);
       test_frame_ops(kb, verbose_p);
       test_class_instance_relationships(kb, verbose_p);
       test_slot_attachment(kb, verbose_p);
       test_slot_values(kb, verbose_p);
       if (Cons.member_eql(_user_defined_facets,
			   kb.get_behavior_values(_compliance), kb, _NIL))
	   { test_facet_attachment(kb, verbose_p);
	     test_facets(kb, verbose_p); 
	   }
       test_constraints(kb, verbose_p);
       // if (new_kb != null) { test_copy_kb(kb, new_kb, verbose_p); } // !!!!!
       if (verbose_p)
	 { kb.print_frame
	     (food,         _all, _all, _T, _taxonomic, _all, _either, _NIL);
	   kb.print_frame
	     (living_thing, _all, _all, _T, _taxonomic, _all, _either, _NIL);
	   kb.print_frame
	     (bill,         _all, _all, _T, _taxonomic, _all, _either, _NIL);
	 }
       test_coerce_to_kb_value(kb, verbose_p);
       test_procedures(kb, verbose_p);
       test_cache_management(kb, verbose_p);
       test_get_frames_with_slot_or_facet_value(kb, verbose_p);
       if (tell_and_ask_kb != null)
	 { tell_and_ask_kb.goto_kb();
	   TellAndAsk.test_tell_and_ask(tell_and_ask_kb, verbose_p); 
	 }
       kb.goto_kb();
       if (verbose_p)
	 {
	   System.out.println("Known KB kb types are: ");
	   Cons.pretty_print(get_kb_types());
	   System.out.println();
	   System.out.println("Known KBs are: ");
	   Cons.pretty_print(get_kbs());
	   System.out.println();
	   System.out.println("Finished testing.  Violations: " + 
			      violations);
	 }
     }


  static void test_kb_finding (Kb kb, boolean verbose_p) {
    if (verbose_p) { System.out.println("Testing kb finding..."); }
    Node test_kb = find_kb_of_type(kb.kbname, kb, kb.connection());
    test_equal(kb, "Kb finds itself", test_kb, kb, verbose_p);
    Object test_kb2 = null;
    try { Class cl = Class.forName("OKBC.Kb");
	  test_kb2 = cl.newInstance();
	}
    catch (Exception exception) {}
    // Should still find Kb as the right instance.
    Node test_kb3 = find_kb_of_type(kb.kbname, (Kb)test_kb2,
				    kb.connection());
    test_equal(kb, "Kb finds itself from superclass", test_kb3, kb, verbose_p);
  }

  static Node trivial_create_class
    (Kb kb, Node name, Node direct_superclass) {
    return kb.create_frame(name, _class, _NIL, 
			   Cons.list_if_not(direct_superclass),
			   _NIL, _NIL, _NIL, _NIL, _NIL, _T, _NIL, _NIL, _T);
  }

  static Node trivial_create_individual(Kb kb, Node name, Node of_class) {
    return kb.create_individual(name, of_class, _NIL, _NIL, _NIL, 
				_NIL, _NIL, _NIL);
  }

  static Node appropriate_root_classes(Kb kb) {
    return _NIL;
  }


  static void create_classes (Kb kb, boolean verbose_p) {
    if (verbose_p) { System.out.println("Creating classes..."); }

    Node roots = appropriate_root_classes(kb);

    living_thing = kb.create_frame($living_thing, _class, _NIL,
				   roots, _NIL,
				   _NIL, _NIL,
				   _NIL, _NIL,
				   _T, _NIL, _NIL, _T);
    
    animal = kb.create_frame($animal, _class, _NIL, Cons.list(living_thing),
			     _NIL,
			     Cons.list(Cons.list($color),
				       Cons.list($age),
				       Cons.list($foods),
				       Cons.list($has_tail)),
			     _NIL, 
			     _NIL, _NIL,
			     _T, _NIL, _NIL, _T);
    if (reified_slots_p(kb))
      { color = get_unique_frame($color, kb);
	age = get_unique_frame($age, kb);
	foods = get_unique_frame($foods, kb);
	has_tail = get_unique_frame($has_tail, kb);
      }
    else
      { color = $color;
	age = $age;
	foods = $foods;
	has_tail = $has_tail;
      }

    plant = kb.create_frame($plant, _class, _NIL, Cons.list(living_thing), 
			    _NIL,
			    Cons.list(Cons.list(color),
				      Cons.list($has_leaves)),
			    _NIL,
			    _NIL, _NIL,
			    _T, _NIL, _NIL, _T);
		 
    if (reified_slots_p(kb))
      { has_leaves = get_unique_frame($has_leaves, kb); }
    else { has_leaves = $has_leaves; }

    mammal = trivial_create_class(kb, $mammal, animal);
    reptile = trivial_create_class(kb, $reptile, animal);
    fish = trivial_create_class(kb, $fish, animal);
    grass = trivial_create_class(kb, $grass, plant);
    tree = trivial_create_class(kb, $tree, plant);
    palm = trivial_create_class(kb, $palm, tree);
    food = trivial_create_class(kb, $food, roots);
    tiger = trivial_create_class(kb, $tiger, mammal);
    bird = trivial_create_class(kb, $bird, animal);
    human = trivial_create_class(kb, $human, mammal);
    elephant = trivial_create_class(kb, $elephant, mammal);
    lizard = trivial_create_class(kb, $lizard, reptile);
    dinosaur = trivial_create_class(kb, $dinosaur, reptile);
    salmon = trivial_create_class(kb, $salmon, fish);
    trout = trivial_create_class(kb, $trout, fish);
  }


  static void create_individuals (Kb kb, boolean verbose_p) {
    if (verbose_p) { System.out.println("Creating individuals..."); }
    ted = trivial_create_individual(kb, $ted, tiger);
    bill = trivial_create_individual(kb, $bill, bird);
    harry = trivial_create_individual(kb, $harry, human);
    edward = trivial_create_individual(kb, $edward, elephant);
    linda = trivial_create_individual(kb, $linda, lizard);
    derek = trivial_create_individual(kb, $derek, dinosaur);
    sally = trivial_create_individual(kb, $sally, salmon);
    trudy = trivial_create_individual(kb, $trudy, trout);
    something_edible = trivial_create_individual(kb, $something_edible, food);
    seeds = trivial_create_individual(kb, $seeds, food);
    berries = trivial_create_individual(kb, $berries, food);
    fries = trivial_create_individual(kb, $fries, food);
    chips = trivial_create_individual(kb, $chips, food);
    default_food_for_animals = trivial_create_individual
      (kb, $default_food_for_animals, food);
    default_food_for_mammals = trivial_create_individual
      (kb, $default_food_for_mammals, food);
  }

  static Node test_name_of_standard_frame(Node name, Kb kb, 
					  boolean verbose_p) {
    if (kb.coercible_to_frame_p(name, _NIL) != _NIL)
      { test_true(kb, "get_frame_name(" + name + ", _NIL)",
		  kb.get_frame_name(name, _NIL), verbose_p);
	if (Cons.member_eql(name, okbc_standard_class_names, kb, _NIL) ||
	    Cons.member_eql(name, okbc_class_relation_symbols, kb, _NIL))
	  { test_true(kb, "class_p(" + name + ", _NIL)",
		      kb.class_p(name, _NIL), verbose_p);
	    return kb.coerce_to_class(name, _T, _NIL).firstValue();
	  }
	else if (Cons.member_eql(name, okbc_standard_slot_names, kb, _NIL))
	  { test_true(kb, "slot_p(" + name + ", _NIL)",
		      kb.slot_p(name, _NIL), verbose_p);
	    return kb.coerce_to_slot(name, _T, _NIL).firstValue();
	  }
	else if (Cons.member_eql(name, okbc_standard_facet_names, kb, _NIL))
	  { test_true(kb, "facet_p(" + name + ", _NIL)",
		      kb.facet_p(name, _NIL), verbose_p);
	    return kb.coerce_to_facet(name, _T, _NIL).firstValue();
	  }
	else { throw new RuntimeException("Should never get here!"); }
      }
    else { return null; }
  }

  static void test_keywordified_frames (Kb kb, boolean verbose_p) {
    if (verbose_p) { System.out.println("Testing keywordified names..."); }
    Enumerator e = okbc_standard_names.enumerate_list();
    Node frames = _NIL;
    while (e.has_more_p()) {
      Symbol name = (Symbol)(e.next());
      Symbol s = Symbol.intern("$_" + name.symbolName, "OKBC-TEST");
      Node frame = test_name_of_standard_frame(name, kb, verbose_p);
      s.symbolValue = frame;
      if (frame != null) { frames = new Cons(frame, frames); }
    }
    if (Cons.remove_eql
	  (_NIL, kb.get_behavior_values(_frame_names_required), kb)
	!= _NIL &&
	verbose_p)
      { System.out.println("Frames are: ");
	Cons.pretty_print(frames); 
	System.out.println();
      }
  }

  static void test_frame_ops (Kb kb, boolean verbose_p) {
    if (verbose_p) { System.out.println("Testing frame ops..."); }
    test_true(kb, "coercible_to_frame_p(tiger, _NIL)",
	      kb.coercible_to_frame_p(tiger, _NIL), verbose_p);
    test_true(kb, "coercible_to_frame_p(ted, _NIL)",
	      kb.coercible_to_frame_p(ted, _NIL), verbose_p);
    test_true (kb, "coercible_to_frame_p(coerce_to_frame(ted, _T, _NIL),_NIL)",
	       kb.coercible_to_frame_p
	       (kb.coerce_to_frame(ted, _T, _NIL).firstValue(),_NIL), 
	       verbose_p);
    if (Cons.remove_eql(_NIL,
			kb.get_behavior_values(_frame_names_required), kb)
	!= _NIL)
      { test_equal(kb, "get_frame_name(coerce_to_frame(ted, _T, _NIL), _NIL)",
		   kb.get_frame_name
		   (kb.coerce_to_frame(ted, _T, _NIL).firstValue(), _NIL),
		   $ted, verbose_p);
      }
  }


  static void test_class_instance_relationships (Kb kb, boolean verbose_p) {
    if (verbose_p) 
      { System.out.println("Testing class/instance relationships..."); }

    test_true (kb, "class_p(tiger)", kb.class_p(tiger, _NIL), verbose_p);
    test_false(kb, "class_p(ted)",   kb.class_p(ted, _NIL), verbose_p);
    
    test_equal_sets
       (kb, Cons.list(grass, tree),
	   "get_class_subclasses(plant, _direct, _all, _NIL)",
	kb.get_class_subclasses(plant, _direct, _all, _NIL), verbose_p);
    test_equal_sets
       (kb, Cons.list(grass, tree, palm),
	   "get_class_subclasses(plant, _taxonomic, _all, _NIL)",
	kb.get_class_subclasses(plant, _taxonomic, _all, _NIL), verbose_p);

    test_equal_sets
       (kb, Cons.list(ted),
	   "get_class_instances(tiger, _direct, _all, _NIL)",
	kb.get_class_instances(tiger, _direct, _all, _NIL), verbose_p);

    test_equal_sets
       (kb, _NIL,
	   "get_class_instances(animal, _direct, _all, _NIL)",
	kb.get_class_instances(animal, _direct, _all, _NIL), verbose_p);

    test_equal_sets
       (kb, Cons.list(ted, harry, edward),
	   "get_class_instances(mammal, _taxonomic, _all, _NIL)",
	kb.get_class_instances(mammal, _taxonomic, _all, _NIL), verbose_p);

    test_equal_sets
       (kb, Cons.list(animal),
	   "get_class_superclasses(mammal, _direct, _all, _NIL)",
	kb.get_class_superclasses(mammal, _direct, _all, _NIL), verbose_p);

    test_true(kb, "subclass_of_p(lizard, animal, _taxonomic, _NIL)",
	      kb.subclass_of_p(lizard, animal, _taxonomic, _NIL), verbose_p);
    test_true(kb, "superclass_of_p(animal, lizard, _taxonomic, _NIL)",
	      kb.superclass_of_p(animal, lizard, _taxonomic, _NIL), verbose_p);
    test_true(kb, "type_of_p(animal, sally, _taxonomic, _NIL)",
	      kb.type_of_p(animal, sally, _taxonomic, _NIL), verbose_p);

    test_true (kb, "individual_p(derek, _NIL)",
	       kb.individual_p(derek, _NIL), verbose_p);
    test_false(kb, "individual_p(lizard, _NIL)",
	       kb.individual_p(lizard, _NIL), verbose_p);

    // If slots are reified, we have to include them as frames.
    Node slot_frames = _NIL;
    if (reified_slots_p(kb))
      { slot_frames = Cons.list(has_leaves, has_tail, foods, color, age); }
    
    test_at_least
      (kb, Cons.append_cons
            (Cons.append_cons(slot_frames,
			      Cons.list(ted, bill, harry, edward, linda,
					derek, sally, trudy)),
	     Cons.append_cons(Cons.list(something_edible, seeds, berries,
					fries, chips,
					default_food_for_animals,
					default_food_for_mammals),
			      Cons.append_cons(okbc_standard_facet_names,
					       okbc_standard_slot_names))),
       "get_kb_individuals(_frames, _T)",
       kb.get_kb_individuals(_frames, _T), verbose_p);

    test_at_least
      (kb, Cons.list(lizard),
       "get_instance_types(linda, _direct, _all, _NIL)",
       kb.get_instance_types(linda, _direct, _all, _NIL), verbose_p);

    test_true(kb, "type_of_p(reptile, linda, _taxonomic, _NIL)",
	      kb.type_of_p(reptile, linda, _taxonomic, _NIL), verbose_p);
  }


  static boolean reified_slots_p(Kb kb) {
    return Cons.member_eql
      (_slot, kb.get_behavior_values(_are_frames), kb, _NIL);
  }


  static boolean reified_facets_p(Kb kb) {
    return Cons.member_eql
      (_facet, kb.get_behavior_values(_are_frames), kb, _NIL);
  }


  static void test_slot_attachment (Kb kb, boolean verbose_p) {
       if (verbose_p) { System.out.println("Testing slot attachment..."); }
       test_equal_sets
	 (kb, Cons.list(color, age, foods, has_tail),
	  "get_frame_slots(animal, _taxonomic, _template, _NIL)",
	   kb.get_frame_slots(animal, _taxonomic, _template, _NIL),
	  verbose_p);
       test_all_in_list
	 (kb, Cons.list(color, age, foods, has_tail),
	  "get_frame_slots(bill, _taxonomic, _own, _NIL)",
	   kb.get_frame_slots(bill, _taxonomic, _own, _NIL),
	  verbose_p);
       kb.attach_slot(bill, has_leaves, _own, _NIL);
       test_all_in_list
	 (kb, Cons.list(color, age, foods, has_tail, has_leaves),
	  "get_frame_slots(bill, _taxonomic, _own, _NIL)",
	   kb.get_frame_slots(bill, _taxonomic, _own, _NIL),
	  verbose_p);
       kb.detach_slot(bill, has_leaves, _own, _NIL);
       test_all_in_list
	 (kb, Cons.list(color, age, foods, has_tail),
	  "get_frame_slots(bill, _taxonomic, _own, _NIL)",
	   kb.get_frame_slots(bill, _taxonomic, _own, _NIL),
	  verbose_p);
     }


  static void test_slot_values (Kb kb, boolean verbose_p) {
       if (verbose_p) { System.out.println("Testing slot values..."); }

       test_equal_sets
	 (kb, _NIL,
	  "get_slot_values(bill, foods, _taxonomic, _own, _all, _either,_NIL)",
	   kb.get_slot_values(bill, foods, _taxonomic, _own, _all, _either,
			      _NIL),
	  verbose_p);

       kb.put_slot_value(bill, foods, seeds,
			 _own, _known_true, _NIL);

       test_equal(kb, "get_slot_type(bill, foods, _taxonomic, _NIL)",
		  kb.get_slot_type(bill, foods, _taxonomic, _NIL),
		  _own, verbose_p);

       test_equal_sets
	 (kb, Cons.list(seeds),
	  "get_slot_values(bill, foods, _taxonomic, _own, _all, _either,_NIL)",
	   kb.get_slot_values(bill, foods, _taxonomic, _own, _all, _either,
			      _NIL),
	  verbose_p);

       kb.put_slot_value(mammal, foods, something_edible,
			 _template, _known_true, _NIL);

       test_equal_sets
	 (kb, Cons.list(seeds),
	  "get_slot_values(bill, foods, _taxonomic, _own, _all, _either,_NIL)",
	   kb.get_slot_values(bill, foods, _taxonomic, _own, _all, _either,
			      _NIL),
	  verbose_p);

       kb.put_slot_values(harry, foods,
			  Cons.list(kb.coerce_to_frame(fries, _T, 
						       _NIL).firstValue(),
				    kb.coerce_to_frame(chips, _T,
						       _NIL).firstValue()), 
			  _own, _known_true, _NIL);

       test_equal_sets
	 (kb, Cons.list(fries ,chips),
	  "get_slot_values(harry, foods, _direct, _own, _all, _either, _NIL)",
	   kb.get_slot_values(harry, foods, _direct, _own, _all, _either,_NIL),
	  verbose_p);

       test_equal_sets
	 (kb, Cons.list(fries, chips, something_edible),
	  "get_slot_values(harry, foods, _taxonomic, _own, _all,_either,_NIL)",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, _either,
			      _NIL),
	  verbose_p);
       kb.add_slot_value(bill, foods,
			 kb.coerce_to_frame(berries, _T, _NIL).firstValue(),
			 _equal, _own, 
			 new OKBCInteger(0), _known_true, _NIL);

       test_equal_sets
	 (kb, Cons.list(seeds, berries),
	  "get_slot_values(bill, foods,_taxonomic,_own,_all,_either,_NIL)",
	   kb.get_slot_values(bill, foods, _taxonomic, _own, _all, _either,
			      _NIL),
	  verbose_p);

       kb.remove_slot_value(bill, foods, 
			    kb.coerce_to_frame(seeds, _T, _NIL).firstValue(),
			    _equal, _own, _all, _known_true, _NIL);

       test_equal_sets
	 (kb, Cons.list(berries),
	  "get_slot_values(bill, foods, _direct, _own, _all, _either, _NIL)",
	   kb.get_slot_values(bill, foods, _direct, _own, _all, _either,
			      _NIL),
	  verbose_p);

       test_equal_sets
	 (kb, Cons.list(berries),
	  "get_slot_values(bill, foods, _taxonomic, _own, _all, _either,_NIL)",
	   kb.get_slot_values(bill, foods, _taxonomic, _own, _all, _either,
			      _NIL),
	  verbose_p);



       kb.put_slot_value(animal, foods, default_food_for_animals, _template,
			 _default_only, _NIL);

       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 1",
	   kb.get_slot_values(animal, foods, _direct, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 1b",
	   kb.get_slot_values(animal, foods, _direct, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 1c",
	   kb.get_slot_values(animal, foods, _direct, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 2",
	   kb.get_slot_values(animal, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 2b",
	   kb.get_slot_values(animal, foods, _taxonomic, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 2c",
	   kb.get_slot_values(animal, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       kb.remove_slot_value(mammal, foods, something_edible,
			    _equal, _template, _all, _known_true, _NIL);

       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 3",
	   kb.get_slot_values(mammal, foods, _direct, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 3b",
	   kb.get_slot_values(mammal, foods, _direct, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 3c",
	   kb.get_slot_values(mammal, foods, _direct, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 4",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 4b",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 4c",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       kb.put_slot_value(animal, foods, something_edible, _template,
			 _known_true, _NIL);

       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 4d",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(something_edible),
	  "Slot defaults 4e",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(something_edible),
	  "Slot defaults 4f",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       kb.remove_slot_value(animal, foods, something_edible,
			    _equal, _template, _all, _known_true, _NIL);


       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 4g",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 4h",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 4i",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       


       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 5",
	   kb.get_slot_values(human, foods, _direct, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 5b",
	   kb.get_slot_values(human, foods, _direct, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 5c",
	   kb.get_slot_values(human, foods, _direct, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 6",
	   kb.get_slot_values(human, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 6b",
	   kb.get_slot_values(human, foods, _taxonomic, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 6c",
	   kb.get_slot_values(human, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 7",
	   kb.get_slot_values(harry, foods, _direct, _own, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(chips, fries),
	  "Slot defaults 7b",
	   kb.get_slot_values(harry, foods, _direct, _own, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(chips, fries),
	  "Slot defaults 7c",
	   kb.get_slot_values(harry, foods, _direct, _own, _all, 
			      _either, _NIL),
	  verbose_p);
       
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 8",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(chips, fries),
	  "Slot defaults 8b",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(chips, fries),
	  "Slot defaults 8c",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);
       
       kb.remove_slot_value(harry, foods, chips,
			    _equal, _own, _all, _known_true, _NIL);
       kb.remove_slot_value(harry, foods, fries,
			    _equal, _own, _all, _known_true, _NIL);

       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 8d",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);

       kb.put_slot_value(mammal, foods, default_food_for_mammals, _template,
			 _default_only, _NIL);

       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 9",
	   kb.get_slot_values(animal, foods, _direct, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 9b",
	   kb.get_slot_values(animal, foods, _direct, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 9c",
	   kb.get_slot_values(animal, foods, _direct, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 10",
	   kb.get_slot_values(animal, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 10b",
	   kb.get_slot_values(animal, foods, _taxonomic, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 10c",
	   kb.get_slot_values(animal, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 11",
	   kb.get_slot_values(mammal, foods, _direct, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 11b",
	   kb.get_slot_values(mammal, foods, _direct, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 11c",
	   kb.get_slot_values(mammal, foods, _direct, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       
       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 12",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 12b",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 12c",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);

       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 13",
	   kb.get_slot_values(human, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 13b",
	   kb.get_slot_values(human, foods, _taxonomic, _template, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 13c",
	   kb.get_slot_values(human, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);


       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 14",
	   kb.get_slot_values(harry, foods, _direct, _own, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 14b",
	   kb.get_slot_values(harry, foods, _direct, _own, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 14c",
	   kb.get_slot_values(harry, foods, _direct, _own, _all, 
			      _either, _NIL),
	  verbose_p);

       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 15",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 15b",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _known_true, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 15c",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);

       kb.remove_slot_value(mammal, foods, default_food_for_mammals,
			    _equal, _template, _all, _default_only, _NIL);

       test_equal_sets
	 (kb, Cons.list(default_food_for_animals),
	  "Slot defaults 16",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);

       kb.put_slot_value(mammal, foods, default_food_for_mammals, _template,
			 _default_only, _NIL);

       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 17",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);

       kb.remove_slot_value(human, foods, default_food_for_mammals,
			    _equal, _template, _all, _default_only, _NIL);

       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 18",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 18b",
	   kb.get_slot_values(human, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 18c",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _default_only, _NIL),
	  verbose_p);

       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 19",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 19b",
	   kb.get_slot_values(human, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);
       test_equal_sets
	 (kb, Cons.list(default_food_for_mammals),
	  "Slot defaults 19c",
	   kb.get_slot_values(mammal, foods, _taxonomic, _template, _all, 
			      _either, _NIL),
	  verbose_p);

       kb.put_slot_value(human, foods, berries, _template,
			 _default_only, _NIL);

       test_equal_sets
	 (kb, Cons.list(berries),
	  "Slot defaults 20",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);

       kb.remove_slot_value(harry, foods, berries,
			    _equal, _own, _all, _default_only, _NIL);

       test_equal_sets
	 (kb, _NIL,
	  "Slot defaults 21",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);

       kb.put_slot_value(human, foods, berries, _template,
			 _known_true, _NIL);

       test_equal_sets
	 (kb, Cons.list(berries),
	  "Slot defaults 22",
	   kb.get_slot_values(harry, foods, _taxonomic, _own, _all, 
			      _either, _NIL),
	  verbose_p);
     }


  static void test_facet_attachment (Kb kb, boolean verbose_p) {
    if (verbose_p) { System.out.println("Testing facet attachment..."); }
  }


  static void test_facets (Kb kb, boolean verbose_p) {
    if (verbose_p) { System.out.println("Testing facets..."); }
    OKBCInteger forty_two = new OKBCInteger(42);
    OKBCInteger three = new OKBCInteger(3);
    OKBCInteger zero = new OKBCInteger(0);
    kb.put_slot_value(bill, age, new OKBCInteger(42), _own, _known_true, _NIL);
    single_valued = kb.create_facet($single_valued, bill, age, _own, _NIL,
				    _NIL, _NIL, _NIL, _NIL, _NIL, _NIL);
    kb.put_facet_value(bill, age, single_valued,
		       _T, _own, _known_true, _NIL);
    test_true 
      (kb, 
       "slot_has_facet_p(bill, age, single_valued, _taxonomic,_own, _NIL)",
       kb.slot_has_facet_p(bill, age, single_valued, _taxonomic, _own, _NIL), 
       verbose_p);
    test_false
      (kb, 
       "slot_has_facet_p(bill, color, single_valued, _taxonomic,_own, _NIL)",
       kb.slot_has_facet_p(bill, color,single_valued, _taxonomic, _own, _NIL), 
       verbose_p);
    test_false(kb, "facet_p($bogus_facet, _NIL)",
	       kb.facet_p($bogus_facet, _NIL), verbose_p);
    cardinality = _NIL;
    if (kb.facet_p(_cardinality, _NIL) != _NIL)
      { cardinality = kb.coerce_to_facet(_cardinality, _T,
				      _NIL).firstValue();
      }
    else if (Cons.member_eql(_user_defined_facets,
			     kb.get_behavior_values(_compliance), kb, _NIL))
      
      { cardinality = kb.create_facet(_cardinality, animal, foods, 
				      _template, _NIL, _NIL, _NIL, _NIL, _NIL,
				      _NIL, _NIL);
      }
    if (cardinality != _NIL)
      { kb.put_facet_value(animal, foods, cardinality, forty_two,
			   _template, _known_true, _NIL);
	kb.put_facet_value(bird, foods, cardinality, three,
			   _template, _known_true, _NIL);
	kb.add_facet_value(bill, foods, cardinality, zero,
			   _equal, _own, _known_true, _NIL);
	test_equal_sets
	  (kb, Cons.list(zero, three, forty_two),
	   "get_facet_values(bill,foods,cardinality,_taxonomic,_own,_all,_known_true,_NIL)",
	   kb.get_facet_values(bill, foods, cardinality, _taxonomic, _own,
			       _all, _known_true, _NIL), 
	   verbose_p);
	kb.remove_facet_value(bill, foods, cardinality, zero,
			      _equal, _own, _known_true, _NIL);
	test_equal_sets
	  (kb, Cons.list(three, forty_two),
	   "get_facet_values(bill,foods,cardinality,_taxonomic,_own,_all,_known_true,_NIL)",
	   kb.get_facet_values(bill, foods, cardinality, _taxonomic, _own,
			       _all, _known_true, _NIL), 
	   verbose_p);
	kb.remove_facet_value(bird, foods, cardinality, three,
			      _equal, _template, _known_true, _NIL);
	test_equal_sets
	  (kb, Cons.list(forty_two),
	   "get_facet_values(bill,foods,cardinality,_taxonomic,_own,_all,_known_true,_NIL)",
	   kb.get_facet_values(bill, foods, cardinality, _taxonomic, _own,
			       _all, _known_true, _NIL), 
	   verbose_p);
      }
    test_equal_sets
      (kb, Cons.list(single_valued, _minimum_cardinality),
       "get_slot_facets(bill, age, _taxonomic, _own, _NIL)",
       new Cons(_minimum_cardinality,
		kb.get_slot_facets(bill, age, _taxonomic, _own,
				   _NIL).firstValue()),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(cardinality, _minimum_cardinality),
       "get_slot_facets(bill, foods, _taxonomic, _own, _NIL)",
       new Cons(_minimum_cardinality,
		kb.get_slot_facets(bill, foods, _taxonomic, _own,
				   _NIL).firstValue()),
       verbose_p);
    silly_facet = kb.create_facet($silly_facet, bill, age, _own, _NIL,
				  _NIL, _NIL, _NIL, _NIL, _NIL, _NIL);
    kb.put_facet_value(bird, foods, silly_facet, _T, _template, _known_true,
		       _NIL);
    if (cardinality != _NIL)
      { test_equal_sets
	(kb, Cons.list(cardinality, silly_facet, 
		       _minimum_cardinality),
	 "get_slot_facets(bill, foods, _taxonomic, _own, _NIL)",
	 new Cons(_minimum_cardinality,
		  kb.get_slot_facets(bill, foods, _taxonomic, _own,
				     _NIL).firstValue()),
	 verbose_p);
	kb.remove_facet_value(animal, foods, cardinality, forty_two,
			      _equal, _template, _known_true, _NIL);
	test_equal_sets
	(kb, _NIL,
	 "get_facet_values(bill, foods, cardinality, _taxonomic, _own, _all, _known_true, _NIL)",
	 kb.get_facet_values(bill, foods, cardinality, _taxonomic, _own, _all,
			  _known_true, _NIL),
	 verbose_p);
      }
    else
      { test_equal_sets
	(kb, Cons.list(silly_facet, _minimum_cardinality),
	 "get_slot_facets(bill, foods, _taxonomic, _own, _NIL)",
	 new Cons(_minimum_cardinality,
		  kb.get_slot_facets(bill, foods, _taxonomic, _own,
				     _NIL).firstValue()),
	 verbose_p);
      }

    kb.put_facet_value(harry, foods, silly_facet, chips, _own, _known_true,
		       _NIL);
    kb.add_facet_value(harry, foods, silly_facet, fries, _equal, _own,
		       _known_true, _NIL);

    kb.put_facet_value(animal, foods, silly_facet, default_food_for_animals,
		       _template, _default_only, _NIL);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 1",
       kb.get_facet_values(animal, foods, silly_facet, _direct, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 1b",
       kb.get_facet_values(animal, foods, silly_facet, _direct, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 1c",
       kb.get_facet_values(animal, foods, silly_facet, _direct, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 2",
       kb.get_facet_values(animal, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 2b",
       kb.get_facet_values(animal, foods, silly_facet, _taxonomic, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 2c",
       kb.get_facet_values(animal, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    kb.remove_facet_value(mammal, foods, silly_facet, something_edible,
			  _equal, _template, _known_true, _NIL);

    test_equal_sets
      (kb, _NIL,
       "Facet defaults 3",
       kb.get_facet_values(mammal, foods, silly_facet, _direct, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 3b",
       kb.get_facet_values(mammal, foods, silly_facet, _direct, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 3c",
       kb.get_facet_values(mammal, foods, silly_facet, _direct, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 4",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 4b",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 4c",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    kb.put_facet_value(animal, foods, silly_facet, something_edible, _template,
		       _known_true, _NIL);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 4d",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(something_edible),
       "Facet defaults 4e",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(something_edible),
       "Facet defaults 4f",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    kb.remove_facet_value(animal, foods, silly_facet, something_edible,
			  _equal, _template, _known_true, _NIL);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 4g",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 4h",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 4i",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, _NIL,
       "Facet defaults 5",
       kb.get_facet_values(human, foods, silly_facet, _direct, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 5b",
       kb.get_facet_values(human, foods, silly_facet, _direct, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 5c",
       kb.get_facet_values(human, foods, silly_facet, _direct, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 6",
       kb.get_facet_values(human, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 6b",
       kb.get_facet_values(human, foods, silly_facet, _taxonomic, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 6c",
       kb.get_facet_values(human, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, _NIL,
       "Facet defaults 7",
       kb.get_facet_values(harry, foods, silly_facet, _direct, _own,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(chips, fries),
       "Facet defaults 7b",
       kb.get_facet_values(harry, foods, silly_facet, _direct, _own,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(chips, fries),
       "Facet defaults 7c",
       kb.get_facet_values(harry, foods, silly_facet, _direct, _own,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 8",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(chips, fries),
       "Facet defaults 8b",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(chips, fries),
       "Facet defaults 8c",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);

    kb.remove_facet_value(harry, foods, silly_facet, chips,
			  _equal, _own, _known_true, _NIL);
    kb.remove_facet_value(harry, foods, silly_facet, fries,
			  _equal, _own, _known_true, _NIL);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 8d",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);

    kb.put_facet_value(mammal, foods, silly_facet, default_food_for_mammals,
		       _template, _default_only, _NIL);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 9",
       kb.get_facet_values(animal, foods, silly_facet, _direct, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 9b",
       kb.get_facet_values(animal, foods, silly_facet, _direct, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 9c",
       kb.get_facet_values(animal, foods, silly_facet, _direct, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 10",
       kb.get_facet_values(animal, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 10b",
       kb.get_facet_values(animal, foods, silly_facet, _taxonomic, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 10c",
       kb.get_facet_values(animal, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 11",
       kb.get_facet_values(mammal, foods, silly_facet, _direct, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 11b",
       kb.get_facet_values(mammal, foods, silly_facet, _direct, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 11c",
       kb.get_facet_values(mammal, foods, silly_facet, _direct, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 12",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 12b",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 12c",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 13",
       kb.get_facet_values(human, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 13b",
       kb.get_facet_values(human, foods, silly_facet, _taxonomic, _template,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 13c",
       kb.get_facet_values(human, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, _NIL,
       "Facet defaults 14",
       kb.get_facet_values(harry, foods, silly_facet, _direct, _own,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 14b",
       kb.get_facet_values(harry, foods, silly_facet, _direct, _own,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 14c",
       kb.get_facet_values(harry, foods, silly_facet, _direct, _own,
			   _all, _either, _NIL),
       verbose_p);

    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 15",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 15b",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 15c",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);

    kb.remove_facet_value(mammal, foods, silly_facet, default_food_for_mammals,
			  _equal, _template, _default_only, _NIL);

    test_equal_sets
      (kb, Cons.list(default_food_for_animals),
       "Facet defaults 16",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);

    kb.put_facet_value(mammal, foods, silly_facet, default_food_for_mammals,
		       _template, _default_only, _NIL);

    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 17",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);

    kb.remove_facet_value(human, foods, silly_facet, default_food_for_mammals,
			  _equal, _template, _default_only, _NIL);

    test_equal_sets
      (kb, _NIL,
       "Facet defaults 18",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 18b",
       kb.get_facet_values(human, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 18c",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _default_only, _NIL),
       verbose_p);

    test_equal_sets
      (kb, _NIL,
       "Facet defaults 19",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);
    test_equal_sets
      (kb, _NIL,
       "Facet defaults 19b",
       kb.get_facet_values(human, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(default_food_for_mammals),
       "Facet defaults 19c",
       kb.get_facet_values(mammal, foods, silly_facet, _taxonomic, _template,
			   _all, _either, _NIL),
       verbose_p);

    kb.put_facet_value(human, foods, silly_facet, berries,
		       _template, _default_only, _NIL);

    test_equal_sets
      (kb, Cons.list(berries),
       "Facet defaults 20",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);

    kb.remove_facet_value(harry, foods, silly_facet, berries,
			  _equal, _own, _default_only, _NIL);

    test_equal_sets
      (kb, _NIL,
       "Facet defaults 21",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);

    kb.put_facet_value(human, foods, silly_facet, berries,
		       _template, _known_true, _NIL);

    test_equal_sets
      (kb, Cons.list(berries),
       "Facet defaults 22",
       kb.get_facet_values(harry, foods, silly_facet, _taxonomic, _own,
			   _all, _either, _NIL),
       verbose_p);

  }

  static void find_constraint_facet(Node facet_name, Kb kb) {
    Symbol facet_sym = Symbol.intern(((Symbol)facet_name).symbolName, 
				     "OKBC-TEST");
    Values2 v = kb.coerce_to_facet(facet_name, _NIL, _NIL);
    Node facet = v.firstValue();
    Node found_p = v.secondValue();
    if (found_p != _NIL)
      { facet_sym.symbolValue = facet; }
    else if (Cons.member_eql(_user_defined_facets,
			     kb.get_behavior_values(_compliance), kb, _NIL))
      { facet_sym.symbolValue =
	kb.create_facet(facet_name, _NIL, _NIL, _own, _NIL,
				    _NIL, _NIL, _NIL, _NIL, _NIL, _NIL);
      }
    else { facet_sym.symbolValue = _NIL; }
  }

  static void find_constraint_slot(Node slot_name, Kb kb) {
    Symbol slot_sym = Symbol.intern(((Symbol)slot_name).symbolName, 
				    "OKBC-TEST");
    Values2 v = kb.coerce_to_slot(slot_name, _NIL, _NIL);
    Node slot = v.firstValue();
    Node found_p = v.secondValue();
    if (found_p != _NIL)
      { slot_sym.symbolValue = slot; }
    else 
      { slot_sym.symbolValue =
	kb.create_slot(slot_name, _NIL, _own, _NIL,
		       _NIL, _NIL, _NIL, _NIL, _NIL, _NIL);
      }
  }

  static void find_constraint_class(Node class_name, Kb kb) {
    Symbol class_sym = Symbol.intern(((Symbol)class_name).symbolName, 
				     "OKBC-TEST");
    Values2 v = kb.coerce_to_class(class_name, _NIL, _NIL);
    Node the_class = v.firstValue();
    Node found_p = v.secondValue();
    if (found_p != _NIL)
      { class_sym.symbolValue = the_class; }
    else
      { Node direct_superclasses = _NIL;
	if (class_name == _integer) 
	  { direct_superclasses = Cons.list(_number); }
	else if (class_name == _list) 
	  { direct_superclasses = Cons.list(_individual); }
	else { direct_superclasses = Cons.list(_thing); }
	class_sym.symbolValue =
	  trivial_create_class(kb, class_name, direct_superclasses);
      }
  }

  static void test_constraints (Kb kb, boolean verbose_p) {
    boolean supported_p = Cons.member_eql(_immediate,
					  kb.get_behavior_values
					  (_constraint_checking_time), kb,
					  _NIL);
    Node current = kb.get_behavior_values(_constraint_checking_time);
    if (supported_p)
      { try { kb.put_behavior_values(_constraint_checking_time, 
				     Cons.list(_immediate));
	      if (verbose_p) { System.out.println("Testing constraints..."); }
	      Enumerator e = null;
	      e = okbc_standard_facet_names.enumerate_list();
	      while (e.has_more_p()) { find_constraint_facet(e.next(), kb); }
	      e = okbc_standard_class_names.enumerate_list();
	      while (e.has_more_p()) { find_constraint_class(e.next(), kb); }
	      e = okbc_standard_slot_names.enumerate_list();
	      while (e.has_more_p()) { find_constraint_slot(e.next(), kb); }
	      Node checked = kb.get_behavior_values(_constraints_checked);
	      e = okbc_standard_slot_names.enumerate_list();
	      while (e.has_more_p()) { 
		Node slot = e.next();
		if (Cons.member_eql(slot, checked, kb, _NIL))
		  { test_constraint_slot(slot, kb, verbose_p); }
	      }
	      e = checked.enumerate_list();
	      while (e.has_more_p()) {
		Node name = e.next();
		if (!(Cons.member_eql(name, okbc_standard_slot_names, kb,
				      _NIL)))
		  { test_constraint_facet(name, kb, verbose_p); }
	      }
	    }
	finally
	  { kb.put_behavior_values(_constraint_checking_time, current); }
      }
  }

  static void cerror(Kb kb, Node constraint) {
    System.out.println("Should have signaled an error for " + constraint);
    Listener.breakLoop(kb);
  }


  static void test_constraint_slot
      (Node constraint, Kb kb, boolean verbose_p) {
    OKBCInteger two_k_plus_one = new OKBCInteger(2001);
    OKBCInteger one_o_one = new OKBCInteger(101);
    OKBCInteger forty_two = new OKBCInteger(42);
    OKBCInteger three = new OKBCInteger(3);
    OKBCInteger two = new OKBCInteger(2);
    OKBCInteger one = new OKBCInteger(1);
    OKBCInteger zero = new OKBCInteger(0);
    OKBCInteger minus_one = new OKBCInteger(-1);
    if (constraint == _slot_collection_type)
     {}
    else if (constraint == _slot_some_values)
     {}
    else if ((constraint == _slot_inverse) ||
	     (constraint == _slot_same_values) ||
	     (constraint == _slot_not_same_values) ||
	     (constraint == _slot_subset_of_values))
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_slot_values(foods, constraint,
				       Cons.list(ted), _own, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(has_tail), _own, _known_true, _NIL);
	    }
	// Zero out constraints
	finally { kb.put_slot_values(foods, constraint, _NIL,
				     _own, _known_true, _NIL);
		}
      }
    else if ((constraint == _slot_numeric_minimum) ||
	     (constraint == _slot_numeric_maximum))
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_slot_values(foods, constraint,
				       Cons.list(ted), _own, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(forty_two, three),
				 _own, _known_true, _NIL);
	      if (constraint == _slot_numeric_minimum)
		{ kb.put_slot_values(ted, foods,
				     Cons.list(two_k_plus_one, one_o_one, 
					       forty_two),
				     _own, _known_true, _NIL);
		  try { kb.put_slot_values
			(ted, foods,
			 Cons.list(two_k_plus_one, one_o_one, 
				   forty_two, three),
			 _own, _known_true, _NIL);
			cerror(kb, constraint);
		      }
		  catch (ConstraintViolation e) {}
		}
	      if (constraint == _slot_numeric_maximum)
		{ kb.put_slot_values(ted, foods,
				     Cons.list(zero, one, two, three),
				     _own, _known_true, _NIL);
		  try { kb.put_slot_values
			(ted, foods,
			 Cons.list(zero, one, two, three, forty_two),
			 _own, _known_true, _NIL);
			cerror(kb, constraint);
		      }
		  catch (ConstraintViolation e) {}
		}
	    }
	// Zero out constraints
	finally { kb.put_slot_values(foods, constraint, _NIL,
				     _own, _known_true, _NIL);
		}
      }
    else if ((constraint == _slot_minimum_cardinality) ||
	     (constraint == _slot_maximum_cardinality))
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_slot_values(foods, constraint,
				       Cons.list(ted), _own, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      try { kb.put_slot_values(foods, constraint,
				       Cons.list(minus_one), _own, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(three),
				 _own, _known_true, _NIL);
	      if (constraint == _slot_maximum_cardinality)
		{ kb.put_slot_values(ted, foods,
				     Cons.list(seeds),
				     _own, _known_true, _NIL);
		  try { kb.put_slot_values
			(ted, foods,
			 Cons.list(ted, linda, seeds, chips),
			 _own, _known_true, _NIL);
			cerror(kb, constraint);
		      }
		  catch (ConstraintViolation e) {}
		}
	      if (constraint == _slot_minimum_cardinality)
		{ kb.put_slot_values(ted, foods,
				     Cons.list(seeds, chips, linda, ted),
				     _own, _known_true, _NIL);
		  try { kb.put_slot_values
			(ted, foods,
			 Cons.list(ted, chips),
			 _own, _known_true, _NIL);
			cerror(kb, constraint);
		      }
		  catch (ConstraintViolation e) {}
		}
	    }
	// Zero out constraints
	finally { kb.put_slot_values(foods, constraint, _NIL,
				     _own, _known_true, _NIL);
		}
      }
    else if (constraint == _slot_cardinality)
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_slot_values(foods, constraint,
				       Cons.list(ted), _own, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      try { kb.put_slot_values(foods, constraint,
				       Cons.list(one, three), _own,
				       _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      try { kb.put_slot_values(foods, constraint,
				       Cons.list(minus_one), _own,
				       _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(one),
				 _own, _known_true, _NIL);
	      kb.put_slot_values(ted, foods,
				 Cons.list(seeds),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (ted, foods,
		     Cons.list(seeds, chips),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	    }
	// Zero out constraints
	finally { kb.put_slot_values(foods, constraint, _NIL,
				     _own, _known_true, _NIL);
		}
      }
    else if (constraint == _slot_value_type)
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_slot_values(foods, constraint,
				       Cons.list(ted), _own, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(food),
				 _own, _known_true, _NIL);
	      kb.put_slot_values(ted, foods,
				 Cons.list(something_edible, seeds),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (ted, foods,
		     Cons.list(something_edible, harry),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(food, 
					   Cons.list(_setof, 
						     berries, chips)),
				 _own, _known_true, _NIL);
	      kb.put_slot_values(harry, foods,
				 Cons.list(chips),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (harry, foods,
		     Cons.list(seeds),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(age, constraint,
				 Cons.list(_number),
				 _own, _known_true, _NIL);
	      kb.put_slot_values(harry, age,
				 Cons.list(forty_two),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (harry, age,
		     Cons.list(seeds),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(Cons.list(_union,
						     food, reptile)),
				 _own, _known_true, _NIL);
	      kb.put_slot_values(harry, foods,
				 Cons.list(linda),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (harry, foods,
		     Cons.list(ted),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	    }
	// Zero out constraints
	finally { kb.put_slot_values(foods, constraint, _NIL,
				     _own, _known_true, _NIL);
		}
      }
    else if (constraint == _domain)
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_slot_values(foods, constraint,
				       Cons.list(ted), _own, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(animal),
				 _own, _known_true, _NIL);
	      try
		{ 
		  kb.put_slot_values
		    (chips, foods,
		     Cons.list(chips),
		     _own, _known_true, _NIL);
		  cerror(kb, constraint);
		}
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(animal, 
					   Cons.list(_setof, 
						     ted, linda)),
				 _own, _known_true, _NIL);
	      kb.put_slot_values(ted, foods,
				 Cons.list(chips),
				 _own, _known_true, _NIL);
	      try 
		{ 
		  kb.put_slot_values
		    (harry, foods,
		     Cons.list(seeds),
		     _own, _known_true, _NIL);
		  cerror(kb, constraint);
		}
	      catch (ConstraintViolation e) {}
	      kb.put_slot_values(foods, constraint,
				 Cons.list(Cons.list(_union,
						     elephant, reptile)),
				 _own, _known_true, _NIL);
	      kb.put_slot_values(edward, foods,
				 Cons.list(chips),
				 _own, _known_true, _NIL);
	      try
		{
		  kb.put_slot_values
		    (harry, foods,
		     Cons.list(chips),
		     _own, _known_true, _NIL);
		  cerror(kb, constraint);
		}
	      catch (ConstraintViolation e) {}
	    }
	// Zero out constraints
	finally { kb.put_slot_values(foods, constraint, _NIL,
				     _own, _known_true, _NIL);
		  kb.put_slot_values(age, constraint, _NIL,
				     _own, _known_true, _NIL);
		}
      }
    else { System.out.println
	   ("Constraint checking test not implemented for slot " 
	    + constraint); 
	 }
  }


  static void test_constraint_facet
      (Node constraint, Kb kb, boolean verbose_p) {
    OKBCInteger two_k_plus_one = new OKBCInteger(2001);
    OKBCInteger one_o_one = new OKBCInteger(101);
    OKBCInteger forty_two = new OKBCInteger(42);
    OKBCInteger three = new OKBCInteger(3);
    OKBCInteger two = new OKBCInteger(2);
    OKBCInteger one = new OKBCInteger(1);
    OKBCInteger zero = new OKBCInteger(0);
    OKBCInteger minus_one = new OKBCInteger(-1);
    if ((constraint == _collection_type) ||
	(constraint == _domain) ||
	(constraint == _some_values))
     {}
    else if ((constraint == _inverse) ||
	     (constraint == _same_values) ||
	     (constraint == _not_same_values) ||
	     (constraint == _subset_of_values))
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_facet_values(tiger, foods, constraint,
					Cons.list(ted), _template, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_facet_values(tiger, foods, constraint,
				  Cons.list(has_tail), _template, 
				  _known_true, _NIL);
	    }
	// Zero out constraints
	finally { kb.put_facet_values(tiger, foods, constraint, _NIL,
				      _template, _known_true, _NIL);
		}
      }
    else if ((constraint == _numeric_minimum) ||
	     (constraint == _numeric_maximum))
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_facet_values(tiger, foods, constraint,
					Cons.list(ted), _template, _known_true,
				       _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_facet_values(tiger, foods, constraint,
				  Cons.list(forty_two, three),
				  _template, _known_true, _NIL);
	      if (constraint == _numeric_minimum)
		{ kb.put_slot_values(ted, foods,
				     Cons.list(two_k_plus_one, one_o_one, 
					       forty_two),
				     _own, _known_true, _NIL);
		  try { kb.put_slot_values
			(ted, foods,
			 Cons.list(two_k_plus_one, one_o_one, 
				   forty_two, three),
			 _own, _known_true, _NIL);
			cerror(kb, constraint);
		      }
		  catch (ConstraintViolation e) {}
		}
	      if (constraint == _numeric_maximum)
		{ kb.put_slot_values(ted, foods,
				     Cons.list(zero, one, two, three),
				     _own, _known_true, _NIL);
		  try { kb.put_slot_values
			(ted, foods,
			 Cons.list(zero, one, two, three, forty_two),
			 _own, _known_true, _NIL);
			cerror(kb, constraint);
		      }
		  catch (ConstraintViolation e) {}
		}
	    }
	// Zero out constraints
	finally { kb.put_facet_values(tiger, foods, constraint, _NIL,
				      _template, _known_true, _NIL);
		}
      }
    else if ((constraint == _minimum_cardinality) ||
	     (constraint == _maximum_cardinality))
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_facet_values(tiger, foods, constraint,
					Cons.list(ted), _template, _known_true,
					_NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      try { kb.put_facet_values(tiger, foods, constraint,
					Cons.list(minus_one), _template,
					_known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_facet_values(tiger, foods, constraint,
				  Cons.list(three),
				  _template, _known_true, _NIL);
	      if (constraint == _maximum_cardinality)
		{ kb.put_slot_values(ted, foods,
				     Cons.list(seeds),
				     _own, _known_true, _NIL);
		  try { kb.put_slot_values
			(ted, foods,
			 Cons.list(ted, linda, seeds, chips),
			 _own, _known_true, _NIL);
			cerror(kb, constraint);
		      }
		  catch (ConstraintViolation e) {}
		}
	      if (constraint == _minimum_cardinality)
		{ kb.put_slot_values(ted, foods,
				     Cons.list(seeds, chips, linda, ted),
				     _own, _known_true, _NIL);
		  try { kb.put_slot_values
			(ted, foods,
			 Cons.list(ted, chips),
			 _own, _known_true, _NIL);
			cerror(kb, constraint);
		      }
		  catch (ConstraintViolation e) {}
		}
	    }
	// Zero out constraints
	finally { kb.put_facet_values(tiger, foods, constraint, _NIL,
				      _template, _known_true, _NIL);
		}
      }
    else if (constraint == _cardinality)
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_facet_values(tiger, foods, constraint,
					Cons.list(ted), _template, _known_true,
					_NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      try { kb.put_facet_values(tiger, foods, constraint,
					Cons.list(one, three), _template,
					_known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      try { kb.put_facet_values(tiger, foods, constraint,
					Cons.list(minus_one), _template,
					_known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_facet_values(tiger, foods, constraint,
				  Cons.list(one),
				  _template, _known_true, _NIL);
	      kb.put_slot_values(ted, foods,
				 Cons.list(seeds),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (ted, foods,
		     Cons.list(seeds, chips),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	    }
	// Zero out constraints
	finally { kb.put_facet_values(tiger, foods, constraint, _NIL,
				      _template, _known_true, _NIL);
		}
      }
    else if (constraint == _value_type)
      { try { try { if (verbose_p)
		      {System.out.println("Testing constraint " + constraint);}
		    kb.put_facet_values(tiger, foods, constraint,
					Cons.list(ted), _template, _known_true,
					_NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_facet_values(tiger, foods, constraint,
				  Cons.list(food),
				  _template, _known_true, _NIL);
	      kb.put_slot_values(ted, foods,
				 Cons.list(something_edible, seeds),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (ted, foods,
		     Cons.list(something_edible, harry),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_facet_values(human, foods, constraint,
				  Cons.list(food, 
					    Cons.list(_setof, 
						      berries, chips)),
				  _template, _known_true, _NIL);
	      kb.put_slot_values(harry, foods,
				 Cons.list(chips),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (harry, foods,
		     Cons.list(seeds),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_facet_values(human, age, constraint,
				 Cons.list(_number),
				 _template, _known_true, _NIL);
	      kb.put_slot_values(harry, age,
				 Cons.list(forty_two),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (harry, age,
		     Cons.list(seeds),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	      kb.put_facet_values(human, foods, constraint,
				  Cons.list(Cons.list(_union,
						      food, reptile)),
				  _template, _known_true, _NIL);
	      kb.put_slot_values(harry, foods,
				 Cons.list(linda),
				 _own, _known_true, _NIL);
	      try { kb.put_slot_values
		    (harry, foods,
		     Cons.list(ted),
		     _own, _known_true, _NIL);
		    cerror(kb, constraint);
		  }
	      catch (ConstraintViolation e) {}
	    }
	// Zero out constraints
	finally { kb.put_facet_values(tiger, foods, constraint, _NIL,
				      _template, _known_true, _NIL);
		  kb.put_facet_values(human, foods, constraint, _NIL,
				      _template, _known_true, _NIL);
		  kb.put_facet_values(human, age, constraint, _NIL,
				      _template, _known_true, _NIL);
		}
      }
    else { System.out.println
	   ("Constraint checking test not implemented for facet "
	    + constraint); 
	 }
  }


  static void test_copy_kb (Kb kb, Kb new_kb, boolean verbose_p) {
       if (verbose_p) { System.out.println("Testing copy kb..."); }
       copy_kb_internal(kb, new_kb, _T, _stop, _NIL);
       Node extra = Cons.list(_pretty_name, _documentation);
       test_equal_sets
	 (kb, Cons.append_cons(extra, kb.get_kb_roots(_all, _NIL)),
	  "New kb has the same root classes",
	  Cons.append_cons(extra, new_kb.get_kb_roots(_all, _NIL)), verbose_p);
       test_equal_sets
	 (kb, Cons.append_cons(extra, kb.get_kb_classes(_system_default,
	 _NIL)),
	  "New kb has the same classes",
	  Cons.append_cons(extra, new_kb.get_kb_classes(_system_default,
	  _NIL)), verbose_p);
       // Allow for pretty name, which might not be a frame on one end.
       test_equal_sets
	 (kb, Cons.append_cons
	 (extra, kb.get_kb_individuals(_system_default, _NIL)),
	  "New kb has the same instances",
	  Cons.append_cons
	  (extra, new_kb.get_kb_individuals(_system_default, _NIL)), 
	  verbose_p);
     }

  static void test_coerce_to_kb_value (Kb kb, boolean verbose_p) {
       if (verbose_p) { System.out.println("Testing coerce_to_kb_value..."); }
       Values3 cookie = new Values3(Cons.list(new OKBCInteger(1), 
					      new OKBCInteger(2), 
					      new OKBCString("a string"),
					      Cons.list(food, bill)),
				    _T, _NIL);
       Values3 cookie_with_names
	 = new Values3(Cons.list(new OKBCInteger(1), 
				 new OKBCInteger(2), 
				 new OKBCString("a string"),
				 Cons.list($food, $bill)),
		       _T, _NIL);
       OKBCString string = new OKBCString("(1 2 \"a string\" (food bill))");
       OKBCString wild_string= new
	 OKBCString("(1 2 \"a string\" (food* bill))");
       Node just_frames = Cons.list(food, bill, living_thing);
       Values3 just_frames_cookie = new Values3(just_frames, _T, _NIL);
       OKBCString just_frames_string= new
	 OKBCString("(food bill living-thing)");
       OKBCString alist_string = new OKBCString("(food* *t)");
       Symbol star_t = Symbol.intern("*T", "OKBC-TEST");
       Symbol food_star = Symbol.intern("FOOD*", "OKBC-TEST");
       Node alist_results = null;
       if (Cons.member_eql(_user_defined_facets,
			   kb.get_behavior_values(_compliance), kb, _NIL))
	 { Symbol list_sym = Symbol.intern("LIST", "OKBC-TEST");
	   if (list_sym.symbolValue != null)
	     { alist_results = Cons.list(Cons.list(star_t, new OKBCString(""),
						   trout, elephant, plant, 
						   silly_facet,
						   list_sym.symbolValue),
					 Cons.list(food_star,
						   new OKBCString("FOOD"),
						   food, foods));
	     }
	   else
	     { alist_results = Cons.list(Cons.list(star_t, new OKBCString(""),
						   trout, elephant, plant, 
						   silly_facet),
					 Cons.list(food_star,
						   new OKBCString("FOOD"),
						   food, foods));
	     }
	 }
       else
	 { alist_results = Cons.list(Cons.list(star_t, new OKBCString(""),
					       trout, elephant, plant),
				     Cons.list(food_star,
					       new OKBCString("FOOD"),
					       food, foods));
	 }
       test_equal(kb, "coerce_to_kb_value 1",
		  kb.coerce_to_kb_value(string, _value, _NIL, _NIL, _T,
					_error_if_not_unique, _NIL),
		  cookie, verbose_p);
       test_equal(kb, "coerce_to_kb_value 2",
		  kb.coerce_to_kb_value(string, _value,   _T, _NIL, _T,
					_error_if_not_unique, _NIL),
		  cookie, verbose_p);

       try
	 {
	   kb.coerce_to_kb_value(wild_string, _value, _T, _NIL, _T,
				 _error_if_not_unique, _NIL);
	   throw new RuntimeException ("Should never get here");
	 }
       catch (NotUniqueError e) 
	 { if (verbose_p) 
	     { System.out.println("  Ok -- coerce_to_kb_value 3");}
	 }
       test_equal_except_packages(kb, "coerce_to_kb_value 4",
				  kb.coerce_to_kb_value
				  (string, _value, _NIL, _NIL, _T,
				   _do_not_coerce_to_frames,
				   _NIL).firstValue(),
				  cookie_with_names.firstValue(), verbose_p);
       try
	 {
	   kb.coerce_to_kb_value(string, _value, _NIL, _NIL, _T,
				 _must_name_frames, _NIL);
	   throw new RuntimeException ("Should never get here");
	 }
       catch (NotCoercibleToFrame e) 
	 { OKBCString original = new OKBCString("a string");
	   if (e.frame.equals(original))
	     { if (verbose_p)
		 { System.out.println("  Ok -- coerce_to_kb_value 5"); }
	     }
	   else { System.out.println("**** Test violation on condition:  " 
				     + e);
		  System.out.println("Expected result was: " + original);
		  System.out.println("  Actual result was: " + e.frame);
		  violations = violations + 1;
		}
	 }
       test_equal_except_packages
	 (kb, "coerce_to_kb_value 6",
	  kb.coerce_to_kb_value(just_frames_string, _value,_NIL, _NIL, _T,
				_must_name_frames, _NIL),
	  just_frames_cookie, verbose_p);
       Values3 res = kb.coerce_to_kb_value(alist_string, _value, _T,
					  _NIL, _T,
					  _options_if_not_unique, _NIL);
       Node found_p = res.firstValue();
       Node alist = res.thirdValue();
       // System.out.println("Res = " + res);
       test_false(kb, "coerce_to_kb_value 7 result is NIL", found_p,
		  verbose_p);
       Enumeration e = alist.elements();
       Enumeration f = alist_results.elements();
       for (; e.hasMoreElements() ;) {
	 Node entrya = (Node)(e.nextElement());
	 Node entryb = (Node)(f.nextElement());
	 test_equal_except_packages
	   (kb, "coerce_to_kb_value 7a", entrya.car(), entryb.car(),
	    verbose_p);
	 test_equal
	   (kb, "coerce_to_kb_value 7b", entrya.cadr(),
	    entryb.cadr(), verbose_p);
	 test_at_least
	   (kb, entryb.cdr().cdr(), "coerce_to_kb_value 7c",
	    entrya.cdr().cdr(), verbose_p);
       }
     }

  static void test_procedures (Kb kb, boolean verbose_p) {
    if (verbose_p) { System.out.println("Testing procedures..."); }
    Node test1_name = Symbol.intern("TEST1", "OKBC-TEST"); 
    Procedure test1
    = (Procedure)kb.create_procedure
    ("(frame kb)",
     "(list frame (get-frame-pretty-name frame :kb kb) kb)",
     _NIL);
    kb.register_procedure(test1_name, test1);

    Node test2_name = Symbol.intern("TEST2", "OKBC-TEST"); 
    Procedure test2
    = (Procedure)kb.create_procedure
	("(frame kb)",
"    (do-list (subclass (get-class-subclasses frame :kb kb \n" +
"                                             :kb-local-only-p t \n" +
"                                             :inference-level :direct)) \n" +
"       (list subclass (call-procedure 'test1 :kb kb \n" +
"                                      :arguments (list subclass kb))\n"+
"             (test1 subclass kb)))",
	 _NIL);
    kb.register_procedure(test2_name, test2);
    Node expected_result
      = Cons.list(Cons.list(animal,
			    Cons.list(animal,
				      kb.get_frame_pretty_name(animal, _NIL),
				      kb),
			    Cons.list(animal,
				      kb.get_frame_pretty_name(animal, _NIL),
				      kb)),
		  Cons.list(plant,
			    Cons.list(plant,
				      kb.get_frame_pretty_name(plant, _NIL),
				      kb),
			    Cons.list(plant,
				      kb.get_frame_pretty_name(plant, _NIL),
				      kb)));
    Node result1 = kb.call_procedure(test2_name, Cons.list(living_thing, kb));
    test_equal_sets(kb, expected_result, "procedure test 1", result1,
		    verbose_p);
    Node result2 = kb.call_procedure(test2, Cons.list(living_thing, kb));
    test_equal_sets(kb, expected_result, "procedure test 2", result2,
		    verbose_p);
  }

  static void test_cache_management (Kb the_kb, boolean verbose_p) {
    if (the_kb instanceof CachingKb &&
	((CachingKb)the_kb).allow_caching_p)
      { CachingKb kb = (CachingKb) the_kb;
	if (verbose_p) { System.out.println("Testing cache management..."); }
	Values3 result = kb.get_slot_values(bill, foods, _taxonomic, _own,
					    _all, _known_true, _NIL);
	test_true
	  (kb, "cached_p_get_slot_values(bill, foods, _taxonomic, _own)",
	   kb.cached_p_get_slot_values
	   (bill, foods, _taxonomic, _own, _all, _known_true, _NIL), 
	   verbose_p);
	kb.flush_cache();
	test_false
	  (kb, "cached_p_get_slot_values(bill, foods, _taxonomic, _own)",
	   kb.cached_p_get_slot_values
	   (bill, foods, _taxonomic, _own, _all, _known_true, _NIL), 
		   verbose_p);
	kb.encache_get_slot_values
	  (result, bill, foods, _taxonomic, _own, _all, _known_true, _NIL);
	test_true
	  (kb, "cached_p_get_slot_values(bill, foods, _taxonomic, _own)",
	   kb.cached_p_get_slot_values
	   (bill, foods, _taxonomic, _own, _all, _known_true, _NIL), 
	   verbose_p);
      }
  }


  static Symbol $c1              = Symbol.intern("C1", "OKBC-TEST");
  static Node c1 = null;
  static Symbol $c2              = Symbol.intern("C2", "OKBC-TEST");
  static Node c2 = null;
  static Symbol $c3              = Symbol.intern("C3", "OKBC-TEST");
  static Node c3 = null;
  static Symbol $c4              = Symbol.intern("C4", "OKBC-TEST");
  static Node c4 = null;
  static Symbol $s1              = Symbol.intern("S1", "OKBC-TEST");
  static Node s1 = null;
  static Symbol $s2              = Symbol.intern("S2", "OKBC-TEST");
  static Node s2 = null;
  static Symbol $fa              = Symbol.intern("FA", "OKBC-TEST");
  static Node fa = null;
  static Symbol $i1              = Symbol.intern("I1", "OKBC-TEST");
  static Node i1 = null;
  static Symbol $i2              = Symbol.intern("I2", "OKBC-TEST");
  static Node i2 = null;
  static Symbol $i3              = Symbol.intern("I3", "OKBC-TEST");
  static Node i3 = null;
  static Symbol $i4              = Symbol.intern("I4", "OKBC-TEST");
  static Node i4 = null;

  static void test_get_frames_with_slot_or_facet_value
    (Kb kb, boolean verbose_p) {
      if (verbose_p)
	{ System.out.println("Testing get_frames_with_slot/facet_value..."); }
      OKBCInteger forty_two = new OKBCInteger(42);
      OKBCInteger forty_three = new OKBCInteger(43);
      c1 = trivial_create_class(kb, $c1, _NIL);
      c2 = trivial_create_class(kb, $c2, c1);
      c3 = trivial_create_class(kb, $c3, c1);
      c4 = trivial_create_class(kb, $c4, c2);
      s1 = kb.create_slot($s1, _NIL, _own, _NIL,
			  _NIL, _NIL, _NIL, _NIL, _NIL, _NIL);
      s2 = kb.create_slot($s2, _NIL, _own, _NIL,
			  _NIL, _NIL, _NIL, _NIL, _NIL, _NIL);
      fa = kb.create_facet($fa, _NIL, _NIL, _own, _NIL,
			   _NIL, _NIL, _NIL, _NIL, _NIL, _NIL);
      i1 = trivial_create_individual(kb, $i1, c1);
      i2 = trivial_create_individual(kb, $i2, c2);
      i3 = trivial_create_individual(kb, $i3, c3);
      i4 = trivial_create_individual(kb, $i4, c4);
      kb.put_slot_value(c1, s1, forty_two, _template, _known_true, _NIL);
      kb.put_slot_value(c1, s2, forty_two, _template, _default_only,
			_NIL);
      kb.remove_slot_value(c4, s2, forty_two,
			   _equal, _template, _all, _default_only, _NIL);
      kb.put_slot_value(i1, s1, forty_three, _own, _known_true, _NIL);
      
      kb.put_facet_value(c1, s1, fa, forty_two, _template, _known_true, _NIL);
      kb.put_facet_value(c1, s2, fa, forty_two, _template, _default_only,
			 _NIL);
      kb.remove_facet_value(c4, s2, fa, forty_two,
			   _equal, _template, _default_only, _NIL);
      kb.put_facet_value(i1, s1, fa, forty_three, _own, _known_true, _NIL);
      run_tests_for_get_frames_with_slot_or_facet_value(kb, verbose_p);
    }


  static void run_tests_for_get_frames_with_slot_or_facet_value
     (Kb kb, boolean verbose_p) {
    OKBCInteger forty_two = new OKBCInteger(42);
    OKBCInteger forty_three = new OKBCInteger(43);
    // First test the slots
    test_equal_sets
      (kb, Cons.list(c1), "get_frames_with_slot_value test 1",
       kb.get_frames_with_slot_value(s1, forty_two, _direct, _template,
			 	     _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(c1), "get_frames_with_slot_value test 2",
       kb.get_frames_with_slot_value(s1, forty_two, _direct, _all,
				     _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(i1), "get_frames_with_slot_value test 3",
       kb.get_frames_with_slot_value(s1, forty_three, _direct, _own,
				     _known_true, _NIL),
       verbose_p);
    
    test_equal_sets
      (kb, Cons.list(c1, c2, c3, c4), "get_frames_with_slot_value test 4",
       kb.get_frames_with_slot_value(s1, forty_two, _taxonomic, _template,
				     _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(i1), "get_frames_with_slot_value test 5",
       kb.get_frames_with_slot_value(s1, forty_three, _taxonomic, _own,
				     _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(i1, i2, i3, i4), "get_frames_with_slot_value test 6",
       kb.get_frames_with_slot_value(s1, forty_two, _taxonomic, _own,
				     _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(c1, c2, c3, c4, i1, i2, i3, i4),
       "get_frames_with_slot_value test 7",
       kb.get_frames_with_slot_value(s1, forty_two, _taxonomic, _all,
				     _known_true, _NIL),
       verbose_p);

    // Now test the facets
    test_equal_sets
      (kb, Cons.list(c1), "get_frames_with_facet_value test 1",
       kb.get_frames_with_facet_value(s1, fa, forty_two, _direct, _template,
				      _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(c1), "get_frames_with_facet_value test 2",
       kb.get_frames_with_facet_value(s1, fa, forty_two, _direct, _all,
				      _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(i1), "get_frames_with_facet_value test 3",
       kb.get_frames_with_facet_value(s1, fa, forty_three, _direct, _own,
				      _known_true, _NIL),
       verbose_p);
    
    test_equal_sets
      (kb, Cons.list(c1, c2, c3, c4), "get_frames_with_facet_value test 4",
       kb.get_frames_with_facet_value(s1, fa, forty_two, _taxonomic, _template,
				      _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(i1), "get_frames_with_facet_value test 5",
       kb.get_frames_with_facet_value(s1, fa, forty_three, _taxonomic, _own,
				      _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(i1, i2, i3, i4), "get_frames_with_facet_value test 6",
       kb.get_frames_with_facet_value(s1, fa, forty_two, _taxonomic, _own,
				      _known_true, _NIL),
       verbose_p);
    test_equal_sets
      (kb, Cons.list(c1, c2, c3, c4, i1, i2, i3, i4),
       "get_frames_with_facet_value test 7",
       kb.get_frames_with_facet_value(s1, fa, forty_two, _taxonomic, _all,
				     _known_true, _NIL),
       verbose_p);
    

    // Now do the defaults.
    if (kb.get_behavior_values(_defaults) != _NIL)
      {
        test_equal_sets
          (kb, Cons.list(c1), "get_frames_with_slot_value test 8",
           kb.get_frames_with_slot_value(s2, forty_two, _direct, _template,
    				     _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, _NIL, "get_frames_with_slot_value test 9",
           kb.get_frames_with_slot_value(s2, forty_three, _direct, _own,
    				     _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, Cons.list(c1), "get_frames_with_slot_value test 10",
           kb.get_frames_with_slot_value(s2, forty_two, _direct, _all,
    				     _default_only, _NIL),
           verbose_p);
        
        test_equal_sets
          (kb, Cons.list(c1, c2, c3), "get_frames_with_slot_value test 11",
           kb.get_frames_with_slot_value(s2, forty_two, _taxonomic, _template,
    				     _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, Cons.list(i1, i2, i3), "get_frames_with_slot_value test 12",
           kb.get_frames_with_slot_value(s2, forty_two, _taxonomic, _own,
    				     _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, Cons.list(c1, c2, c3, i1, i2, i3),
           "get_frames_with_slot_value test 13",
           kb.get_frames_with_slot_value(s2, forty_two, _taxonomic, _all,
    				     _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, Cons.list(c1), "get_frames_with_facet_value test 8",
           kb.get_frames_with_facet_value(s2, fa,forty_two, _direct, _template,
    				      _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, _NIL, "get_frames_with_facet_value test 9",
           kb.get_frames_with_facet_value(s2, fa,forty_three, _direct, _own,
    				      _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, Cons.list(c1), "get_frames_with_facet_value test 10",
           kb.get_frames_with_facet_value(s2, fa,forty_two, _direct, _all,
    				      _default_only, _NIL),
           verbose_p);
        
        test_equal_sets
          (kb, Cons.list(c1, c2, c3), "get_frames_with_facet_value test 11",
           kb.get_frames_with_facet_value
	   (s2, fa,forty_two, _taxonomic, _template, _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, Cons.list(i1, i2, i3), "get_frames_with_facet_value test 12",
           kb.get_frames_with_facet_value(s2, fa,forty_two, _taxonomic, _own,
    				      _default_only, _NIL),
           verbose_p);
        test_equal_sets
          (kb, Cons.list(c1, c2, c3, i1, i2, i3),
           "get_frames_with_facet_value test 13",
           kb.get_frames_with_facet_value(s2, fa,forty_two, _taxonomic, _all,
    				      _default_only, _NIL),
           verbose_p);
      }
  }
}
