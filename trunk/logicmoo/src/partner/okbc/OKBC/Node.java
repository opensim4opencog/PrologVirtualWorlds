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

public abstract class Node {

  public static final False _NIL = new False();
  public static final True _T = new True();
  public static final EndOfFileNode __EOF = new EndOfFileNode();


  public static final Symbol _abort = Symbol.keyword("ABORT");
  public static final Symbol _add = Symbol.keyword("ADD");
  public static final Symbol _add_after = Symbol.keyword("ADD-AFTER");
  public static final Symbol _after = Symbol.keyword("AFTER");
  public static final Symbol _alien = Symbol.keyword("ALIEN");
  public static final SlotType _all = SlotType.slot_type_keyword("ALL");
  public static final Symbol _allocate = Symbol.keyword("ALLOCATE");
  public static final InferenceLevel _all_inferable = InferenceLevel.inference_level_keyword("ALL-INFERABLE");
  public static final Symbol _annotations = Symbol.keyword("ANNOTATIONS");
  public static final Symbol _anonymous = Symbol.keyword("ANONYMOUS");
  public static final Symbol _are_frames = Symbol.keyword("ARE-FRAMES");
  public static final Symbol _arguments = Symbol.keyword("ARGUMENTS");
  public static final Symbol _axiom = Symbol.keyword("AXIOM");
  public static final Symbol _bag = Symbol.keyword("BAG");
  public static final Symbol _capitalize = Symbol.keyword("CAPITALIZE");
  public static final ContextName _class = ContextName.context_name_keyword("CLASS");
  public static final Symbol _class_slot_types = Symbol.keyword("CLASS-SLOT-TYPES");
  public static final Symbol _collection_types = Symbol.keyword("COLLECTION-TYPES");
  public static final Symbol _compliance = Symbol.keyword("COMPLIANCE");
  public static final Symbol _connect = Symbol.keyword("CONNECT");
  public static final Symbol _conservative_axiom = Symbol.keyword("conservative-axiom");
  public static final Symbol _consis = Symbol.keyword("CONSIS");
  public static final Symbol _constraint_checking = Symbol.keyword("CONSTRAINT-CHECKING");
  public static final Symbol _constraint_checking_time = Symbol.keyword("CONSTRAINT-CHECKING-TIME");
  public static final Symbol _constraint_report_time = Symbol.keyword("CONSTRAINT-REPORT-TIME");
  public static final Symbol _constraints_checked = Symbol.keyword("CONSTRAINTS-CHECKED");
  public static final Symbol _constraint_violation = Symbol.keyword("CONSTRAINT-VIOLATION");
  public static final Symbol _continue = Symbol.keyword("CONTINUE");
  public static final Symbol _create_p = Symbol.keyword("CREATE-P");
  public static final Symbol _default = Symbol.keyword("DEFAULT");
  public static final Symbol _defaults = Symbol.keyword("DEFAULTS");
  public static final ValueSelector _default_only = ValueSelector.value_selector_keyword("DEFAULT-ONLY");
  public static final Symbol _default_template_values = Symbol.keyword("DEFAULT-TEMPLATE-VALUES");
  public static final Symbol _default_test_fn = Symbol.keyword("DEFAULT-TEST-FN");
  public static final Symbol _deferred = Symbol.keyword("DEFERRED");
  public static final Symbol _deferred_reporting = Symbol.keyword("DEFERRED-REPORTING");
  public static final Symbol _delete = Symbol.keyword("DELETE");
  public static final Symbol _delete_values_p = Symbol.keyword("DELETE-VALUES-P");
  public static final InferenceLevel _direct = InferenceLevel.inference_level_keyword("DIRECT");
  public static final Symbol _direct_instances = Symbol.keyword("DIRECT-INSTANCES");
  public static final Symbol _direct_subclasses = Symbol.keyword("DIRECT-SUBCLASSES");
  public static final Symbol _direct_superclasses = Symbol.keyword("DIRECT-SUPERCLASSES");
  public static final Symbol _direct_types = Symbol.keyword("DIRECT-TYPES");
  public static final Symbol _disconnect = Symbol.keyword("DISCONNECT");
  public static final Symbol _doc = Symbol.keyword("DOC");
  public static final Symbol _downcase = Symbol.keyword("DOWNCASE");
  public static final Symbol _do_not_coerce_to_frames = Symbol.keyword("DO-NOT-COERCE-TO-FRAMES");
  public static final ValueSelector _either = ValueSelector.value_selector_keyword("EITHER");
  public static final Symbol _ephemeral = Symbol.keyword("EPHEMERAL");
  public static final Symbol _eql = Symbol.keyword("EQL");
  public static final Symbol _eql_in_kb = Symbol.keyword("EQL-IN-KB");
  public static final Symbol _equal = Symbol.keyword("EQUAL");
  public static final Symbol _equal_in_kb = Symbol.keyword("EQUAL-IN-KB");
  public static final Symbol _equalp = Symbol.keyword("EQUALP");
  public static final Symbol _error = Symbol.keyword("ERROR");
  public static final Symbol _error_p = Symbol.keyword("ERROR-P");
  public static final Symbol _error_if_not_unique = Symbol.keyword("ERROR-IF-NOT-UNIQUE");
  public static final Symbol _establish_local_connection = Symbol.keyword("__ESTABLISH-LOCAL-CONNECTION");
  public static final ContextName _facet = ContextName.context_name_keyword("FACET");
  public static final Symbol _facets = Symbol.keyword("FACETS");
  public static final Symbol _facets_supported = Symbol.keyword("FACETS-SUPPORTED");
  public static final Symbol _file = Symbol.keyword("FILE");
  public static final Symbol _filled = Symbol.keyword("FILLED");
  public static final Symbol _filename = Symbol.keyword("FILENAME");
  public static final Symbol _flush_memory_p = Symbol.keyword("FLUSH-MEMORY-P");
  public static final Symbol _force_load_p = Symbol.keyword("FORCE-LOAD-P");
  public static final ContextName _frame = ContextName.context_name_keyword("FRAME");
  public static final Symbol _frames = Symbol.keyword("FRAMES");
  public static final Symbol _frame_names_required = Symbol.keyword("FRAME-NAMES-REQUIRED");
  public static final Symbol _frame_type = Symbol.keyword("FRAME-TYPE");
  public static final Symbol _okbc_frame_coersion = Symbol.keyword("OKBC-FRAME-COERSION");
  public static final Symbol _okbc_record_modifications = Symbol.keyword("okbc-RECORD-MODIFICATIONS");
  public static final Symbol _handle = Symbol.keyword("HANDLE");
  public static final Symbol _ignore = Symbol.keyword("IGNORE");
  public static final Symbol _immediate = Symbol.keyword("IMMEDIATE");
  public static final Symbol _incoherence = Symbol.keyword("INCOHERENCE");
  public static final Symbol _index = Symbol.keyword("INDEX");
  public static final Symbol _inheritance = Symbol.keyword("INHERITANCE");
  public static final Symbol _inherited = Symbol.keyword("INHERITED");
  public static final Symbol _initargs = Symbol.keyword("INITARGS");
  public static final ContextName _individual = ContextName.context_name_keyword("INDIVIDUAL");
  public static final Symbol _instance = Symbol.keyword("INSTANCE");
  public static final Symbol _instances = Symbol.keyword("INSTANCES");
  public static final Symbol _irrelevant = Symbol.keyword("IRRELEVANT");
  public static final Symbol _kb = Symbol.keyword("KB");
  public static final Symbol _kb_locator = Symbol.keyword("KB-LOCATOR");
  public static final Symbol _kb_type = Symbol.keyword("KB-TYPE");
  public static final Symbol _kif = Symbol.keyword("KIF");
  public static final ValueSelector _known_true = ValueSelector.value_selector_keyword("KNOWN-TRUE");
  public static final Symbol _lisp_predicate = Symbol.keyword("LISP-PREDICATE");
  public static final Symbol _live = Symbol.keyword("LIVE");
  public static final Symbol _locator = Symbol.keyword("LOCATOR");
  public static final Symbol _local = Symbol.keyword("LOCAL");
  public static final Symbol _local_info = Symbol.keyword("LOCAL-INFO");
  public static final Symbol _local_only_p = Symbol.keyword("LOCAL-ONLY-P");
  public static final Symbol _monotonic = Symbol.keyword("MONOTONIC");
  public static final Symbol _monitor_okbc = Symbol.keyword("MONITOR-OKBC");
  public static final Symbol _more = Symbol.keyword("MORE");
  public static final Symbol _must_name_frames = Symbol.keyword("MUST-NAME-FRAMES");
  public static final Symbol _multiple = Symbol.keyword("MULTIPLE");
  public static final Symbol _multiset = Symbol.keyword("MULTISET");
  public static final Symbol _name = Symbol.keyword("NAME");
  public static final Symbol _never = Symbol.keyword("NEVER");
  public static final Symbol _new = Symbol.keyword("NEW");
  public static final Symbol _new_kb = Symbol.keyword("NEW-KB");
  public static final Symbol _none = Symbol.keyword("NONE");
  public static final Symbol _number_of_values = Symbol.keyword("NUMBER-OF-VALUES");
  public static final Symbol _ok = Symbol.keyword("OK");
  public static final Symbol _old = Symbol.keyword("OLD");
  public static final Symbol _one_of = Symbol.keyword("ONE-OF");
  public static final Symbol _ontolingua = Symbol.keyword("ONTOLINGUA");
  public static final Symbol _open = Symbol.keyword("OPEN");
  public static final Symbol _options_if_not_unique = Symbol.keyword("OPTIONS-IF-NOT-UNIQUE");
  public static final Symbol _otherwise = Symbol.keyword("OTHERWISE");
  public static final Symbol _override = Symbol.keyword("OVERRIDE");
  public static final SlotType _own = SlotType.slot_type_keyword("OWN");
  public static final Symbol _own_facets = Symbol.keyword("OWN-FACETS");
  public static final Symbol _own_facet_specs = Symbol.keyword("OWN-FACET-SPECS");
  public static final Symbol _own_slots = Symbol.keyword("OWN-SLOTS");
  public static final Symbol _own_slot_specs = Symbol.keyword("OWN-SLOT-SPECS");
  public static final Symbol _package = Symbol.keyword("PACKAGE");
  public static final Symbol _pathname = Symbol.keyword("PATHNAME");
  public static final Symbol _permission = Symbol.keyword("PERMISSION");
  public static final Symbol _persistent = Symbol.keyword("PERSISTENT");
  public static final Symbol _portable = Symbol.keyword("PORTABLE");
  public static final Symbol _preserve = Symbol.keyword("PRESERVE");
  public static final Symbol _pretty_name = Symbol.keyword("PRETTY-NAME");
  public static final Symbol _primitive_p = Symbol.keyword("PRIMITIVE-P");
  public static final Symbol _print_case = Symbol.keyword("PRINT-CASE");
  public static final Symbol _read = Symbol.keyword("READ");
  public static final Symbol _read_only = Symbol.keyword("READ-ONLY");
  public static final Symbol _recursive_delete_p = Symbol.keyword("RECURSIVE-DELETE-P");
  public static final Symbol _save_updates_p = Symbol.keyword("SAVE-UPDATES-P");
  public static final Symbol _sentences = Symbol.keyword("SENTENCES");
  public static final Symbol _sequence = Symbol.keyword("SEQUENCE");
  public static final Symbol _set = Symbol.keyword("SET");
  public static final Symbol _single = Symbol.keyword("SINGLE");
  public static final Symbol _skip = Symbol.keyword("SKIP");
  public static final ContextName _slot = ContextName.context_name_keyword("SLOT");
  public static final Symbol _slot_chain = Symbol.keyword("SLOT-CHAIN");
  public static final Symbol _slot_type = Symbol.keyword("SLOT-TYPE");
  public static final Symbol _status = Symbol.keyword("STATUS");
  public static final Symbol _stop = Symbol.keyword("STOP");
  public static final Symbol _subclasses = Symbol.keyword("SUBCLASSES");
  public static final Symbol _superclasses = Symbol.keyword("SUPERCLASSES");
  public static final Symbol _system_default = Symbol.keyword("SYSTEM-DEFAULT");
  public static final InferenceLevel _taxonomic = InferenceLevel.inference_level_keyword("TAXONOMIC");
  public static final Symbol _tell = Symbol.keyword("TELL");
  public static final Symbol _tellable = Symbol.keyword("TELLABLE");
  public static final SlotType _template = SlotType.slot_type_keyword("TEMPLATE");
  public static final Symbol _template_and_own = Symbol.keyword("TEMPLATE-AND-OWN");
  public static final Symbol _template_facets = Symbol.keyword("TEMPLATE-FACETS");
  public static final Symbol _template_facet_specs = Symbol.keyword("TEMPLATE-FACET-SPECS");
  public static final Symbol _template_slots = Symbol.keyword("TEMPLATE-SLOTS");
  public static final Symbol _template_slot_specs = Symbol.keyword("TEMPLATE-SLOT-SPECS");
  public static final Symbol _template_values = Symbol.keyword("TEMPLATE-VALUES");
  public static final Symbol _test = Symbol.keyword("TEST");
  public static final Symbol _timeout = Symbol.keyword("TIMEOUT");
  public static final Symbol _type = Symbol.keyword("TYPE");
  public static final Symbol _types = Symbol.keyword("TYPES");
  public static final Symbol _union_multiple_values = Symbol.keyword("UNION-MULTIPLE-VALUES");
  public static final Symbol _unknown = Symbol.keyword("UNKNOWN");
  public static final Symbol _user = Symbol.keyword("USER");
  public static final Symbol _user_interface = Symbol.keyword("USER-INTERFACE");
  public static final Symbol _valid_extensions = Symbol.keyword("VALID-EXTENSIONS");
  public static final ContextName _value = ContextName.context_name_keyword("VALUE");
  public static final Symbol _untell = Symbol.keyword("UNTELL");
  public static final Symbol _upcase = Symbol.keyword("UPCASE");
  public static final Symbol _user_defined_facets = Symbol.keyword("USER-DEFINED-FACETS");

  public static final Symbol __append = Symbol.intern("APPEND", "LISP");
  public static final Symbol __cons   = Symbol.intern("CONS",   "LISP");
  public static final Symbol __list   = Symbol.intern("LIST",   "LISP");
  public static final Symbol __quote  = Symbol.intern("QUOTE",  "LISP");
  public static final Symbol __fail = Symbol.keyword("**FAIL**");

  public static final Symbol __root = Symbol.keyword("ROOT");
  public static final Symbol __own_slot_of = Symbol.keyword("OWN-SLOT-OF");

  // OKBC standard names for relations etc.

  // KIF operator symbols
  public static final Symbol _lambda = Symbol.keyword("LAMBDA");
  public static final Symbol _listof = Symbol.keyword("LISTOF");
  public static final Symbol _cond = Symbol.keyword("COND");
  public static final Symbol _setofall = Symbol.keyword("SETOFALL");
  public static final Symbol _eqangang = Symbol.keyword("=>>");
  public static final Symbol _not = Symbol.keyword("NOT");
  public static final Symbol _angangeq = Symbol.keyword("<<=");
  public static final Symbol _the = Symbol.keyword("THE");
  public static final Symbol _backwards_implies = Symbol.keyword("<=");
  public static final Symbol _kappa = Symbol.keyword("KAPPA");
  public static final Symbol _or = Symbol.keyword("OR");
  public static final Symbol _and = Symbol.keyword("AND");
  public static final Symbol _setof = Symbol.keyword("SETOF");
  public static final Symbol _exists = Symbol.keyword("EXISTS");
  public static final Symbol _if = Symbol.keyword("IF");
  public static final Symbol _implies = Symbol.keyword("=>");
  public static final Symbol _forall = Symbol.keyword("FORALL");
  public static final Symbol _iff = Symbol.keyword("<=>");
  public static final Symbol _union = Symbol.keyword("UNION");
  public static final Symbol _holds = Symbol.keyword("HOLDS");
  public static final Symbol _quote = Symbol.keyword("QUOTE");
  public static final Symbol _false = Symbol.keyword("TRUE");
  public static final Symbol _true = Symbol.keyword("FALSE");
  public static final Symbol _defobject = Symbol.keyword("DEFOBJECT");
  public static final Symbol _deffunction = Symbol.keyword("DEFFUNCTION");
  public static final Symbol _defrelation = Symbol.keyword("DEFRELATION");


  public static final Node kif_operator_symbols =
    Cons.append_cons(Cons.list(_lambda, _listof, _cond, _setofall, _eqangang,
			       _not, _angangeq, _the, _backwards_implies,
			       _kappa, _or, _and, _setof, _exists),
		     Cons.list(_if, _implies, _forall, _iff, _union, _holds,
			       _quote));

  // Evaluable predicates
  public static final Symbol _is = Symbol.keyword("IS");
  public static final Symbol _less_than = Symbol.keyword("<");
  public static final Symbol _greater_than = Symbol.keyword(">");
  public static final Symbol _greater_than_or_equal = Symbol.keyword(">=");
  public static final Symbol _less_than_or_equal = Symbol.keyword("=<");
  public static final Symbol _equals_sign = Symbol.keyword("=");
  public static final Symbol _not_equal = Symbol.keyword("/=");

  public static final Node evaluable_predicate_symbols =
    Cons.list(_is, _less_than, _greater_than, _greater_than_or_equal, 
	      _less_than_or_equal, _equals_sign, _not_equal);

  // OKBC relation symbols

  public static final Symbol __default_template_facet_value
    = Symbol.keyword("DEFAULT-TEMPLATE-FACET-VALUE");
  public static final Symbol __default_facet_value
    = Symbol.keyword("DEFAULT-FACET-VALUE");
  public static final Symbol __template_facet_value
    = Symbol.keyword("TEMPLATE-FACET-VALUE");
  public static final Symbol __default_template_slot_value
    = Symbol.keyword("DEFAULT-TEMPLATE-SLOT-VALUE");
  public static final Symbol __default_slot_value
    = Symbol.keyword("DEFAULT-SLOT-VALUE");
  public static final Symbol __template_slot_value
    = Symbol.keyword("TEMPLATE-SLOT-VALUE");
  public static final Symbol __template_facet_of
    = Symbol.keyword("TEMPLATE-FACET-OF");
  public static final Symbol __template_slot_of
    = Symbol.keyword("TEMPLATE-SLOT-OF");
  public static final Symbol __facet_of = Symbol.keyword("FACET-OF");
  public static final Symbol __slot_of = Symbol.keyword("SLOT-OF");
  public static final Symbol __subclass_of = Symbol.keyword("SUBCLASS-OF");
  public static final Symbol __superclass_of = Symbol.keyword("SUPERCLASS-OF");
  public static final Symbol __instance_of = Symbol.keyword("INSTANCE-OF");
  public static final Symbol __type_of = Symbol.keyword("TYPE-OF");
  public static final Symbol __primitive = Symbol.keyword("PRIMITIVE");
  public static final Symbol __name = Symbol.keyword("NAME");
  public static final Symbol __pretty_name = Symbol.keyword("PRETTY-NAME");
  public static final Symbol __handle = Symbol.keyword("HANDLE");

  public static final Node okbc_class_relation_symbols =
    Cons.list(_frame, _class, _individual, _facet, _slot, __primitive);
  public static final Node okbc_relation_symbols =
    Cons.append_cons(Cons.list(__template_facet_value, __template_slot_value,
			       __template_facet_of, __template_slot_of,
			       __facet_of, __slot_of, __subclass_of,
			       __superclass_of),
		     Cons.append_cons(Cons.list(__instance_of, __type_of,
						__name, __pretty_name,
						__handle),
				      okbc_class_relation_symbols));

  // OKBC standard class names

  public static final Symbol _thing = Symbol.keyword("THING");
  public static final Symbol _number = Symbol.keyword("NUMBER");
  public static final Symbol _integer = Symbol.keyword("INTEGER");
  public static final Symbol _string = Symbol.keyword("STRING");
  public static final Symbol _symbol = Symbol.keyword("SYMBOL");
  public static final Symbol _list = Symbol.keyword("LIST");

  public static final Node okbc_standard_class_names =
    Cons.list(_thing, _number, _integer, _string, _symbol, _list);

  // OKBC standard facet names

  public static final Symbol _value_type = Symbol.keyword("VALUE-TYPE");
  public static final Symbol _inverse = Symbol.keyword("INVERSE");
  public static final Symbol _cardinality = Symbol.keyword("CARDINALITY");
  public static final Symbol _maximum_cardinality 
    = Symbol.keyword("MAXIMUM-CARDINALITY");
  public static final Symbol _minimum_cardinality
    = Symbol.keyword("MINIMUM-CARDINALITY");
  public static final Symbol _same_values = Symbol.keyword("SAME-VALUES");
  public static final Symbol _not_same_values
    = Symbol.keyword("NOT-SAME-VALUES");
  public static final Symbol _subset_of_values
    = Symbol.keyword("SUBSET-OF-VALUES");
  public static final Symbol _numeric_minimum
    = Symbol.keyword("NUMERIC-MINIMUM");
  public static final Symbol _numeric_maximum
    = Symbol.keyword("NUMERIC-MAXIMUM");
  public static final Symbol _some_values = Symbol.keyword("SOME-VALUES");
  public static final Symbol _collection_type
    = Symbol.keyword("COLLECTION-TYPE");
  public static final Symbol _documentation_in_frame
    = Symbol.keyword("DOCUMENTATION-IN-FRAME");

  public static final Node okbc_standard_facet_names =
    Cons.list(_value_type, _inverse, _cardinality, _maximum_cardinality, 
	      _minimum_cardinality, _same_values, _not_same_values, 
	      _subset_of_values, _numeric_minimum, _numeric_maximum,
	      _some_values, _collection_type, _documentation_in_frame);

  // OKBC standard slot names

  public static final Symbol _documentation = Symbol.keyword("DOCUMENTATION");
  public static final Symbol _domain = Symbol.keyword("DOMAIN");
  public static final Symbol _slot_inverse = Symbol.keyword("SLOT-INVERSE");
  public static final Symbol _slot_value_type
    = Symbol.keyword("SLOT-VALUE-TYPE");
  public static final Symbol _slot_cardinality
    = Symbol.keyword("SLOT-CARDINALITY");
  public static final Symbol _slot_maximum_cardinality
    = Symbol.keyword("SLOT-MAXIMUM-CARDINALITY");
  public static final Symbol _slot_minimum_cardinality
    = Symbol.keyword("SLOT-MINIMUM-CARDINALITY");
  public static final Symbol _slot_same_values
    = Symbol.keyword("SLOT-SAME-VALUES");
  public static final Symbol _slot_not_same_values
    = Symbol.keyword("SLOT-NOT-SAME-VALUES");
  public static final Symbol _slot_subset_of_values
    = Symbol.keyword("SLOT-SUBSET-OF-VALUES");
  public static final Symbol _slot_numeric_minimum
    = Symbol.keyword("SLOT-NUMERIC-MINIMUM");
  public static final Symbol _slot_numeric_maximum
    = Symbol.keyword("SLOT-NUMERIC-MAXIMUM");
  public static final Symbol _slot_some_values
    = Symbol.keyword("SLOT-SOME-VALUES");
  public static final Symbol _slot_collection_type
    = Symbol.keyword("SLOT-COLLECTION-TYPE");

  public static final Node okbc_standard_slot_names =
    Cons.list(_documentation, _domain, _slot_value_type, _slot_inverse,
	      _slot_cardinality, _slot_maximum_cardinality, 
	      _slot_minimum_cardinality, _slot_same_values, 
	      _slot_not_same_values, _slot_subset_of_values,
	      _slot_numeric_minimum, _slot_numeric_maximum, _slot_some_values,
	      _slot_collection_type);

  public static final Node okbc_standard_names =
    Cons.append_cons
    (okbc_standard_class_names,
     Cons.append_cons(okbc_standard_slot_names,
		      Cons.append_cons(okbc_standard_facet_names,
				       okbc_class_relation_symbols)));


  //==================================================

  static final int LIST = 0;
  static final int SYMBOL = 1;
  static final int STRING = 2;
  static final int INTEGER = 3;
  static final int REAL = 4;
  static final int TRUE = 5;
  static final int FALSE = 6;
  static final int OTHER = 7; // not used
  static final int Kb = 8;
  static final int PROCEDURE = 9;
  static final int FRAMEHANDLE = 10;
  static final int REMOTEVALUE = 11;
  static final int ENUMERATOR = 12;
  
  static final int IN_HASH_TABLE = 14;
  static final int EXTENDED_TYPE = 15; // Hex F goes into extended types.

  static final int HASH_TABLE = EXTENDED_TYPE + (16 * 1); // #x1F
  static final int ENDOFFILE  = EXTENDED_TYPE + (16 * 2); // #x2F

  public static Kb all_OKBC_KBs [] = new Kb[0];
  public static Kb the_current_kb = null;
  public static boolean trace_network_interactions_p = false;

  protected static final Node okbc_standard_class_direct_superclass_alist =
    Cons.list(Cons.list(_thing),
	      Cons.list(_integer, _number),
	      Cons.list(_list, _individual),
	      Cons.list(_otherwise, _thing));

  int typeCode;
  
  static Node registered_concrete_kbs = _NIL;
  
  protected static AbstractSymbol register_concrete_kb_type
        (AbstractSymbol abstract_type, Kb kb) 
  {
    // System.out.println("Registering " + abstract_type + " as " + kb);
    if (kb.kb_type_name == null)
      { throw new RuntimeException("kb_type not defined for " + kb); }
    Node existing = Cons.assoc(abstract_type, registered_concrete_kbs);
    if (existing == null || existing == _NIL)
      { registered_concrete_kbs = new Cons(Cons.list(abstract_type, kb),
					   registered_concrete_kbs);
      }
    // System.out.println("Registered now = " + registered_concrete_kbs);
    return abstract_type;
  }

  protected static Kb get_kb_type_from_class (Class cl) {
    Kb res = null;
    Enumerator e = registered_concrete_kbs.enumerate_list();
    while (e.has_more_p()) {
      Node spec = e.next();
      Kb kb = (Kb)(spec.cadr());
      if (cl == kb.getClass())
	{ res = kb; break; }
    }
    return res;
  }

  protected static Kb concrete_kb_from_abstract_kb(AbstractSymbol abstract_type)
  {
    Node entry = Cons.assoc(abstract_type, registered_concrete_kbs);
    if (entry == null) { return null; }
    Node ret = entry.nth(1);
    if (ret == _NIL) return null;
    return (Kb) ret;
  }

  protected static AbstractSymbol abstract_type_from_concrete_kb(Kb kb) {
    Enumeration e = registered_concrete_kbs.elements();
    Class my_class = kb.getClass();
    for (; e.hasMoreElements() ;) {
      Node entry = (Node)(e.nextElement());
      if (my_class == entry.nth(1).getClass())
	{ return (AbstractSymbol)(entry.car()); }
    }
    return null;
  }

  protected static AbstractSymbol abstract_type_from_concrete_type
  (AbstractSymbol kb_type) {
    String abstract_string = null;
    if (kb_type.symbolName.startsWith("ABSTRACT-") &&
	kb_type.symbolName.endsWith("-KB"))
      { abstract_string = kb_type.symbolName; }
    else { abstract_string = "ABSTRACT-" + kb_type.symbolName + "-KB"; }
    Enumeration e = registered_concrete_kbs.elements();
    for (; e.hasMoreElements() ;) {
      Node entry = (Node)(e.nextElement());
      AbstractSymbol s = (AbstractSymbol)(entry.car());
      if (s.symbolName.equals(abstract_string))
	{ return (AbstractSymbol)(entry.car()); }
    }
    return null;
  }

  static Node enquote (Node form) {
    if (form == null || form == _NIL)
      { return null; }
    else { return new Cons(Cons.list(__quote, form.car()),
			   enquote(form.cdr()));
	 }
  }

  protected static Node enquote_args (Node form) {
    return new Cons(form.car(), enquote(form.cdr()));
  }

  protected void encode(PrintWriter stream, boolean client_p) {
    if (client_p)
      { throw new RuntimeException
	("Can't encode a generic Node " + this.toString() +
	 " from a client.");
      }
    else
      { RemoteValue.intern_remote_value
	(this, current_kb()).encode(stream, client_p); }
  }

  public static void newline (PrintWriter stream) {
    stream.print('\n');
  }

  protected String pointerForToString () {
    return Integer.toString(hashCode() << 1 >>> 1, 16);
  }

  public static Node makeList(int length) {
    if (length < 0) { throw new RuntimeException ("Length is negative"); }
    else if (length == 0) { return _NIL; }
    else { Cons res = null;
	   for(int i = 0; i < length; i++) {
	     res = new Cons(_NIL, res);
	   }
	   return res;
	 }
  }

// ---------- default implementation of meta-kb

  static protected Node meta_kb_internal (Connection connection) {
    MetaKb.ensure_registered();
    MetaKb theKb = MetaKb.getMetaKb();
    return theKb;
  }

  static public Node meta_kb (Connection connection) {
    if (connection instanceof NetworkConnection)
      { return NetworkKb.meta_kb(connection); }
    else { return meta_kb_internal(connection); }
  }

  static public Node meta_kb () {
    return meta_kb(local_connection());
  }

  protected static void add_kb (Kb kb) {
    add_kb(kb, _NIL);
  }

  protected static void add_kb (Kb kb, Node kb_locator) {
    // This is the new way of doing it:
    Connection localcon = LocalConnection.local_connection();
    MetaKb meta_kb = (MetaKb)meta_kb(localcon);
    // Optimisation alert.  Constraint checking is expensive
    // in the meta_kb, and we know what we're doing here!
    meta_kb.fast_inhibit_constraint_checking = true;
    Class kb_class = kb.getClass();
    Node class_frame = meta_kb.coerce_to_frame
      (Symbol.intern_in_ok_back(kb_class.getName()), _T, _T).firstValue();
    // System.out.println("1:: " + System.currentTimeMillis());
    meta_kb.put_instance_types(kb, Cons.list(class_frame), _T);
    // System.out.println("2:: " + System.currentTimeMillis());
    if (meta_kb.slot_p(_locator, _T) == _NIL)
      { meta_kb.create_slot(_locator, _NIL, _own, _NIL, _NIL, _NIL, _NIL, _NIL,
			    new OKBCString("Locator"), _NIL);
      }
    if (kb_locator == _NIL)
      // Note: we create this locator at the local connection.  It is the
      // responsibility of the guy at the other end to create any remote
      // locators.
      { kb_locator = kb.create_kb_locator (kb, localcon); }
    // System.out.println("3:: " + System.currentTimeMillis());
    meta_kb.put_slot_value(kb, _locator, kb_locator, _own, _known_true,
			   _T);
    // System.out.println("4:: " + System.currentTimeMillis());
    meta_kb.fast_inhibit_constraint_checking = false;
    // This is the old way of doing it:
    Kb new_KBs [] = new Kb[all_OKBC_KBs.length + 1];
    for(int i=0; i < all_OKBC_KBs.length; i++) {
      new_KBs[i] = all_OKBC_KBs[i];
    }
    new_KBs[all_OKBC_KBs.length] = kb;
    all_OKBC_KBs = new_KBs;
  }

  static protected Node local_find_kb_of_type
     (Node name_or_kb, Kb kb_type) {
       return local_find_kb_of_type(name_or_kb, kb_type, null);
     }
       
  static protected Node local_find_kb_of_type
     (Node name_or_kb, Kb kb_type, Node kb_type_name) {
       Node result = null;
       MetaKb meta_kb = (MetaKb)meta_kb(LocalConnection.local_connection());
//       System.out.println("MetaKb = " + meta_kb);
       Class kb_class = kb_type.getClass();
       Node class_frame = meta_kb.coerce_to_frame
         (Symbol.intern_in_ok_back(kb_class.getName()), _NIL, _T).firstValue();
       if (class_frame == _NIL) { return _NIL; }
//       System.out.println("Class_frame = " + class_frame);
       Node instances = 
	 meta_kb.get_class_instances(class_frame, _all_inferable, _all,
				     _T).firstValue();
       Enumeration e = instances.elements();
       for (; e.hasMoreElements() ;) {
	 Kb thisKb = (Kb)(e.nextElement());
	 if (name_or_kb.equals(thisKb.kbname)
	     && (kb_type_name == null || thisKb.kb_type_name == kb_type_name))
	   { result = thisKb;
	     break;
	   }
       }

       // System.out.println("Local find kb of type: " + class_frame);
       /******************************************************
       System.out.println("Instances: " + instances);
       Node facts = meta_kb.tuple_store.facts_full_indexed_under
	 (TupleKb._instance_var);
       if (facts == null) { facts = _NIL; }
       Enumerator f = facts.enumerate_list();
       while (f.has_more_p())
	 { Cons.pretty_print(f.next());
	   System.out.println(); 
	 }
       ******************************************************/
       // System.out.println("Selected KB: " + result);
       if (result == null)
	 { result = _NIL; }
       return result;
    }

  static final Symbol find_kb_of_type_fname =
         Symbol.intern_in_ok_back("FIND-KB-OF-TYPE-INTERNAL");

  static public Node find_kb_of_type
     (Node name_or_kb, Kb kb_type, Connection connection) {
     if (connection instanceof NetworkConnection)
       {
	 Node result;
	 Node args =
	   Cons.list(find_kb_of_type_fname,
		     name_or_kb,
		     Cons.list(Symbol.intern("GET-KB-TYPE", "OKBC"), kb_type),
		     _establish_local_connection);
	 Node values = 
	   ((NetworkConnection)connection).makeNetworkCall(args);
	 return values.car();
       }
     { return local_find_kb_of_type(name_or_kb, kb_type); }
    }


  protected static Node toNode(Object o) {
    if (o == null) { return null; }
    else if (o instanceof Node) { return (Node)o; }
    else if (o instanceof Integer) { return new OKBCInteger((Integer) o); }
    else if (o instanceof Long)    { return new OKBCInteger((Long) o); }
    else if (o instanceof Float) { return new Real((Float) o); }
    else if (o instanceof String) { return new OKBCString((String) o); }
    else if (o instanceof Values) { return ((Values)o).firstValue(); }
    else { throw new RuntimeException ("Illegal Object type.  " + o +
				       " is not a node.  It's of type " +
				       o.getClass().getName());
	 }
  }


  public Node nth(Integer n) {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public Node nth(int n) {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public void set_nth(int n, Node to) {
    throw new RuntimeException ("Illegal operation.  " + this +
				" is not a list");
  }

  public Enumerator enumerate_list() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public Node car() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public Node cadr() { 
    Node tail = this.cdr();
    if (tail == null || tail == _NIL)
      { return tail; }
    else { return tail.car(); }
  }

  public Node cdr() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public int length() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public boolean isEmpty() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public Values toValues() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public Values2 toValues2() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public Values3 toValues3() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public Values4 toValues4() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public Node member(Node thing) {
    return (Cons.member_eql(thing, this, the_current_kb, _NIL)
	    ? (Node)_NIL
	    : (Node)_T);
  }

  public Node member(Node thing, Node test) {
    return (Cons.member_test(thing, this, test, the_current_kb, _NIL)
	    ? (Node)_NIL
	    : (Node)_T);
  }

  public Enumeration elements() {
    throw new RuntimeException ("Illegal operation.  " + this + 
				" is not a list or NIL");
  }

  public static Vector all_connection_prototypes = null;

  static void ensure_all_connection_prototypes() {
    if (all_connection_prototypes == null) 
      { all_connection_prototypes = new Vector(); }
  }

  protected static Connection register_connection_prototype
     (Node name, Connection connection) {
       Vector v = new Vector();
       v.addElement(name);
       v.addElement(connection);
       // Note: we handle the initialization this way because there's a race
       // condition between the static init of Node and the static init of
       // connection classes.
       ensure_all_connection_prototypes();
       all_connection_prototypes.addElement(v);
       return connection;
     }
    
  public static Connection establish_connection(Node connection_type) {
    return establish_connection(connection_type, _NIL);
  }

  public static Connection establish_connection
     (Node connection_type, Node initargs) {
    Connection prototype = null;
    Enumeration e = all_connection_prototypes.elements();
    for (; e.hasMoreElements() ;) {
      Vector elt = (Vector)(e.nextElement());
      if (elt.elementAt(0) == connection_type)
	{ prototype = (Connection)elt.elementAt(1); }
    }
    if (prototype == null)
      {
	String message = "There is no registered connection type called " +
	  connection_type + ".  The known connection types are:";
	Enumeration e2 = all_connection_prototypes.elements();
	for (; e2.hasMoreElements() ;) {
	  Vector elt = (Vector)(e2.nextElement());
	  Node ct = (Node) elt.elementAt(0);
	  message = message + "\n  " + ct.toString();
	}
	throw new GenericError (_T, new OKBCString(message)).signal();
      }
    return prototype.establish(initargs);
  }

  public static void close_connection(Connection connection, Node force_p, 
				 TruthValue error_p) {
    connection.close_connection();
  }

  public static void close_connection(Connection connection, Node force_p) {
    close_connection(connection, force_p, _T);
  }

  public static void close_connection(Connection connection) {
    close_connection(connection, _NIL, _T);
  }

  public static Node all_connections() {
    Node result = _NIL;
    Enumeration e = Connection.all_connections_enum();
    while (e.hasMoreElements()) {
      Connection c = (Connection)(e.nextElement());
      result = new Cons(c, result);
    }
    return result;
  }

  public static Enumerator enumerate_all_connections() {
      Node result_list = all_connections();
      return result_list.enumerate_list();
    }



  public static boolean generalised_less_than(Node arg0, Node arg1) {
    if (arg0 instanceof OKBCInteger)
      { long arg0_val = ((OKBCInteger)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    if (arg0_val < arg1_val) { return true; }
	    else { return false; }
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    if (arg0_val < arg1_val) { return true; }
	    else { return false; }
	  }
      }
    else
      { double arg0_val = ((Real)arg0).number;
	if (arg1 instanceof OKBCInteger)
	  { long arg1_val = ((OKBCInteger)arg1).number;
	    if (arg0_val < arg1_val) { return true; }
	    else { return false; }
	  }
	else
	  { double arg1_val = ((Real)arg1).number;
	    if (arg0_val < arg1_val) { return true; }
	    else { return false; }
	  }
      }
  }



// ---------- default implementation of enumerate-list

  public static Enumerator enumerate_list(Node thing) {
    return thing.enumerate_list();
  }

// ---------- default implementation of free

  public static void free(Enumerator e) {
    e.free();
  }

// ---------- default implementation of has-more

  public static Node has_more(Enumerator e) {
    return e.has_more();
  }

// ---------- default implementation of prefetch

  public static void prefetch(Enumerator e) {
    e.prefetch();
  }

  public static void prefetch(Enumerator e, Node number_of_values) {
    e.prefetch(number_of_values);
  }

  public static void prefetch(Enumerator e, Node number_of_values,
			      Node increment) {
    e.prefetch(number_of_values, increment);
  }

// ---------- default implementation of fetch

  public static Node fetch(Enumerator e) {
    return e.fetch();
  }

  public static Node fetch(Enumerator e, Node number_of_values) {
    return e.fetch(number_of_values);
  }

// ---------- default implementation of next

  public static Node next(Enumerator e) {
    return e.next();
  }


// ---------- default implementation of get-kbs

  public static Node get_kbs () {
    return get_kbs(LocalConnection.local_connection());
  }

  public static Node get_kbs (Connection connection) {
    return get_kbs_internal(connection);
  }

  protected static Node get_kbs_internal (Connection connection) {
    if (connection instanceof LocalConnection)
      { Node result = _NIL;
	Enumeration e = get_kb_types(connection).elements();
	for (; e.hasMoreElements() ;) {
	  Kb proto = (Kb)(e.nextElement());
	  result = Cons.append_cons(proto.get_kbs_of_type(connection), 
				    result);
	}
	return Cons.remove_duplicates(result, _eql, null, null);
      }
    else { return MinimalNetworkKb.get_kbs(connection); }
  }

// ---------- default implementation of enumerate-kbs

  public static Enumerator enumerate_kbs () {
    return enumerate_kbs(LocalConnection.local_connection());
  }

  public static Enumerator enumerate_kbs (Connection connection) {
    return get_kbs_internal(connection).enumerate_list();
  }

// ---------- default implementation of find-kb

  static public Node find_kb
     (Node name_or_kb_or_kb_locator,
      Connection connection) {
       if (name_or_kb_or_kb_locator instanceof Kb)
	 { return name_or_kb_or_kb_locator; }
       else { Enumerator e = enumerate_kbs(connection);
	      e.prefetch(_all);
	      while(e.has_more_p()) {
		Kb k = (Kb)(e.next());
		if (name_or_kb_or_kb_locator.equals(k.kbname))
		  { return k; }
	      }
	      return _NIL;
	    }
    }


// ---------- default implementation of get-kb-types

  public static Node get_kb_types () {
    return get_kb_types
      (LocalConnection.local_connection());
  }

  protected static Node get_kb_types_internal (Connection connection){
    if (connection instanceof LocalConnection)
      { Node result = _NIL;
	Enumeration e = registered_concrete_kbs.elements();
	for (; e.hasMoreElements() ;) {
	  Node pair = (Node)(e.nextElement());
	  result = new Cons(pair.nth(1), result);
	}
	return result;
      }
    else { return MinimalNetworkKb.get_kb_types(connection); }
  }

  public static Node get_kb_types (Connection connection) {
    return get_kb_types_internal(connection);
  }

// ---------- default implementation of enumerate-kb-types

  public static Enumerator enumerate_kb_types () {
    return enumerate_kb_types(LocalConnection.local_connection());
  }

  public static Enumerator enumerate_kb_types (Connection connection) {
    return get_kb_types_internal(connection).enumerate_list();
  }


// ---------- default implementation of create-kb

  public Kb create_kb
   (Node name, Node kb_locator, Node initargs, Connection connection) {
    throw new RuntimeException ("Kb_Type is not a Kb.");
  }

// ---------- default implementation of kb-p

  public static Node kb_p (Node thing) {
       if (thing instanceof Kb) { return _T; }
       else { return _NIL; }
     }


// ---------- default implementation of okbc-condition-p

  public static Node okbc_condition_p (Node thing) {
    // can never be true because the condition class is disjoint from Node!
    return _NIL;
  }


// ---------- default implementation of frs-name

  protected static Node frs_name_internal (Kb kb_type, Connection connection) {
    if (connection instanceof NetworkConnection)
      { return NetworkKb.frs_name_internal (kb_type, connection); }
    else if (kb_type.kb_type_name instanceof AbstractSymbol)
      { AbstractSymbol sym = (AbstractSymbol) kb_type.kb_type_name;
	return new OKBCString(sym.symbolName);
      }
    else { return new OKBCString(kb_type.kb_type_name.toString()); }
  }


// ---------- default implementation of copy-kb

  static protected void copy_kb_internal
     (Kb from_kb,
      Kb to_kb,
      TruthValue error_p,
      Node missing_frame_action,
      TruthValue kb_local_only_p) {
       Hashtable visited_table = new Hashtable();
       HashtableNode frame_handle_mapping_table = new HashtableNode();
       Node roots = from_kb.get_kb_roots(_all, kb_local_only_p);
       // System.out.println("Roots " + roots);
       Enumeration e = roots.elements();
       for (; e.hasMoreElements() ;) {
	 Node root = (Node)(e.nextElement());
	 copy_subclasses(root, from_kb, to_kb, error_p, missing_frame_action,
			 visited_table, frame_handle_mapping_table,
			 kb_local_only_p);
       }

       Node classes = from_kb.get_kb_classes(_all, kb_local_only_p);
       // System.out.println("Classes are: " + classes);
       e = classes.elements();
       for (; e.hasMoreElements() ;) {
	 Node c = (Node)(e.nextElement());
	 if (visited_table.get(c) == null)
	   { System.out.println("Class " + c + 
				" was not copied for some reason");
	   }
       }

       Node instances = from_kb.get_kb_individuals(_all, kb_local_only_p);
       // System.out.println("Instances are: " + instances);
       e = instances.elements();
       for (; e.hasMoreElements() ;) {
	 Node i = (Node)(e.nextElement());
	 if (visited_table.get(i) == null)
	   { Node new_name = from_kb.get_frame_name(i, kb_local_only_p);
	     if ((!Cons.member_eql(new_name, okbc_standard_names, to_kb,
				   kb_local_only_p)) ||
		 to_kb.coercible_to_frame_p(new_name, kb_local_only_p) == _NIL)
	       { from_kb.copy_frame_forward(i, new_name, to_kb, error_p, 
					    missing_frame_action,
					    frame_handle_mapping_table,
					    kb_local_only_p);
	       }
	   }
       }
     }

  static void copy_subclasses (Node parent, Kb from_kb, Kb to_kb,
			       TruthValue error_p, Node missing_frame_action,
			       Hashtable visited_table,
			       HashtableNode frame_handle_mapping_table,
			       TruthValue kb_local_only_p) {
       if (visited_table.get(parent) == null)
	 { 
	   // System.out.println("Enter copy_subclasses for " + parent);
	   visited_table.put(parent, _T);
	   Node superclasses = from_kb.get_class_superclasses
	     (parent, _direct, _all, kb_local_only_p).firstValue();
	   Enumeration e = superclasses.elements();
	   for (; e.hasMoreElements() ;) {
	     Node the_super = (Node)(e.nextElement());
	     if (visited_table.get(the_super) == null)
	       { copy_subclasses(the_super, from_kb, to_kb, error_p, 
				 missing_frame_action, visited_table,
				 frame_handle_mapping_table, kb_local_only_p);
	       }
	   }
	   if (to_kb.frame_in_kb_p(parent, kb_local_only_p) == _NIL ||
	       to_kb.class_p(parent, kb_local_only_p) == _NIL)
	     { from_kb.copy_frame_forward
	       (parent, from_kb.get_frame_name(parent, kb_local_only_p),
		to_kb, error_p, missing_frame_action,
		frame_handle_mapping_table, kb_local_only_p);
	     }
	   Node subclasses = from_kb.get_class_subclasses
	     (parent, _direct, _all, kb_local_only_p).firstValue();
	   Enumeration f = subclasses.elements();
	   for (; f.hasMoreElements() ;) {
	     Node subclass = (Node)(f.nextElement());
	     if (visited_table.get(subclass) == null)
	       { copy_subclasses(subclass, from_kb, to_kb, error_p,
				 missing_frame_action, visited_table,
				 frame_handle_mapping_table, kb_local_only_p);
	       }
	   }
	   // System.out.println("Exit copy_subclasses for " + parent);
	 }
    }


  static final Symbol _no_details = Symbol.keyword("NO-DETAILS");
  static final Symbol _error_found = Symbol.keyword("ERROR-FOUND");
  static final Symbol _new_frame_created = Symbol.keyword("NEW-FRAME-CREATED");
  static final Symbol _missing_slot = Symbol.keyword("MISSING-SLOT");
  static final Node missing_frame_tag = new Symbol("**missing-frame-tag**");


// ---------- default implementation of copy-frame

  static protected Values2 copy_frame_internal
     (Node frame, Node new_name, Kb kb, Kb to_kb, 
      TruthValue error_p, Node missing_frame_action,
      Node frame_handle_mapping_table_or_null, TruthValue kb_local_only_p) {
      Node details = kb.get_frame_details(frame, _direct, _all,
					       kb_local_only_p).firstValue();
      HashtableNode frame_handle_mapping_table;
      if (frame_handle_mapping_table_or_null == null ||
	  frame_handle_mapping_table_or_null == _NIL)
	 { frame_handle_mapping_table = new HashtableNode(); }
      else { frame_handle_mapping_table = 
		(HashtableNode)frame_handle_mapping_table_or_null;
	   }
      if (details == _NIL)
	{ if (error_p == _NIL) 
	    { return new Values3(_NIL, _NIL, _no_details); }
	  else 
	    { throw new NotCoercibleToFrame
	      (frame, kb).signal(); 
	    }
	}
      else
	{
	  ConsLocation allocated_frame_handle_alist
	    = new ConsLocation();
	  Values3 result = null;
	  if (error_p!=_NIL)
	    {
	      result = 
		copy_frame_inside_error_handler
		(frame, new_name, kb, to_kb, error_p,
		 missing_frame_action, frame_handle_mapping_table,
		 kb_local_only_p, details, allocated_frame_handle_alist);
	    }
	  else
	    {
	      try {
		result =
		  copy_frame_inside_error_handler
		  (frame, new_name, kb, to_kb, error_p,
		   missing_frame_action, frame_handle_mapping_table,
		   kb_local_only_p, details, allocated_frame_handle_alist);
	      }
	      catch (AbstractError e) {
		return new Values3(_NIL, allocated_frame_handle_alist.cons,
				   _error_found);
	      }
	    }
	  return result;
	}
    }


  static Values3 copy_frame_inside_error_handler
	  (Node frame, Node new_name, Kb kb, Kb to_kb,
		TruthValue error_p, Node missing_frame_action,
		HashtableNode frame_handle_mapping_table,
		TruthValue kb_local_only_p, Node details,
		ConsLocation allocated_frame_handle_alist)
  {
    try {
      // System.out.println("Copying" + frame);
      Node handle = Cons.getf(details, _handle);
      Node pretty_name = Cons.getf(details, _pretty_name);
      Node primitive_p = Cons.getf(details, _primitive_p);
      ContextName frame_type
	= (ContextName)(Cons.getf(details, _frame_type));
      Node types = Cons.getf(details, _types);
      Node superclasses = Cons.getf(details, _superclasses);
      Node own_slots = Cons.getf(details, _own_slots);
      Node own_facets = Cons.getf(details, _own_facets);
      Node template_slots = Cons.getf(details, _template_slots);
      Node template_facets = Cons.getf(details, _template_facets);
      
      if (Cons.member_eql(_user_defined_facets,
			  to_kb.get_behavior_values(_compliance), to_kb,
			  kb_local_only_p))
	{ template_facets = _NIL;
	  own_facets = _NIL;
	}
      // System.out.println("Copy frame " + frame);
      ConsLocation loc = new ConsLocation();
      if (to_kb.coercible_to_frame_p(new_name, kb_local_only_p) != _NIL)
	{ new_name = to_kb.get_frame_name(new_name, kb_local_only_p); 
	}
      Node missing = _NIL;
      if (kb != to_kb)
	{
	  missing = missing_classes
	    (frame, new_name, kb, to_kb, error_p, 
	     frame_type, types, superclasses, kb_local_only_p,
	     allocated_frame_handle_alist, frame_handle_mapping_table);
	}
      if (missing!=_NIL)
	{
	  if (missing_frame_action==_allocate)
	    {
	      if (frame_type==_individual ||
		  frame_type==_slot ||
		  frame_type==_facet)
		{
		  types = kb.allocate_any_necessary_frame_handles
		    (types, frame_handle_mapping_table, kb_local_only_p,
		     to_kb, _allocate, error_p, frame, 
		     allocated_frame_handle_alist);
		}
	      else
		{
		  superclasses = kb.allocate_any_necessary_frame_handles
		    (superclasses, frame_handle_mapping_table, kb_local_only_p,
		     to_kb, _allocate, error_p, frame,
		     allocated_frame_handle_alist);
		}
	    }
	  else if (missing_frame_action==_stop)
	    {
	      if (error_p!=_NIL)
		throw new MissingFrames
		  (missing, frame, kb).signal();
	      else
		{
		  return new Values4(_NIL, allocated_frame_handle_alist.cons,
				     _stop, missing);
		}
	    }
	  else if (missing_frame_action==_abort)
	    {
	      System.out.println("Not all parent frames of "+handle+
				 "in KB "+kb+" exist in KB "+to_kb+
				 " - skipping copy.");
	      if (error_p!=_NIL)
		throw new MissingFrames(missing, frame, kb).signal();
	      else
		{
		  return new Values4(_NIL, allocated_frame_handle_alist.cons,
				     _abort, missing);
		}
	    }
	  else if (missing_frame_action==_continue ||
		   missing_frame_action==_ignore)
	    {
	      System.out.println("Not all parent frames of "+
				 handle+" in KB "+kb+" exist in KB "+ 
				 to_kb+
				 ". Missing frame(s) to be ignored: "+
				 missing);
	      if (frame_type==_individual ||
		  frame_type==_slot ||
		  frame_type==_facet)
		{ types = Cons.set_difference(types, missing, to_kb,
					      kb_local_only_p);
		}
	      else
		{ superclasses = Cons.set_difference(superclasses, missing,
						     to_kb, kb_local_only_p);
		}
	    }
	  else
	    throw new GenericError
	      (_T, new OKBCString("Illegal missing_frame_action argument: "+
				 missing_frame_action)).signal();
	}
      // MISSING: *untranslated-facet-frames* stuff
      
      //? if (frame_type == _class && superclasses == _NIL) 
      //? { superclasses = _class; }
      types = kb.allocate_any_necessary_frame_handles
	(types, frame_handle_mapping_table, kb_local_only_p,
	 to_kb, _allocate, error_p, frame, 
	 allocated_frame_handle_alist);
      superclasses = kb.allocate_any_necessary_frame_handles
	(superclasses, frame_handle_mapping_table, kb_local_only_p,
	 to_kb, _allocate, error_p, frame,
	 allocated_frame_handle_alist);
      Node key = Cons.list(new_name, handle, frame_type);
      Node new_handle = frame_handle_mapping_table.get(key);
      if (new_handle == null && (!(handle instanceof AbstractSymbol)))
	{ Node newh = to_kb.allocate_frame_handle(new_name, frame_type,
						   handle);
	  frame_handle_mapping_table.put(key, newh);
	  frame_handle_mapping_table.put(newh, key);
	  allocated_frame_handle_alist.cons =
	    Cons.append_cons(Cons.list(handle, newh),
			     allocated_frame_handle_alist.cons);
	  new_handle = newh;
	}
      if (new_handle == null) { new_handle = _NIL; }
      Node supers = _NIL;
      if (kb.class_p(frame, kb_local_only_p)!=_NIL)
	supers = superclasses;
      Node args = Cons.list
	(new_name, frame_type, types, supers, _NIL,
	 fix_up_slot_specs(template_slots, kb, to_kb,
			   frame_handle_mapping_table, kb_local_only_p,
			   missing_frame_action, error_p, frame,
			   allocated_frame_handle_alist),
	 fix_up_facet_specs(template_facets, kb, to_kb,
			    frame_handle_mapping_table, kb_local_only_p,
			    missing_frame_action, error_p, frame,
			    allocated_frame_handle_alist),
	 fix_up_slot_specs(own_slots, kb, to_kb,
			   frame_handle_mapping_table, kb_local_only_p,
			   missing_frame_action, error_p, frame,
			   allocated_frame_handle_alist),
	 fix_up_facet_specs(own_facets, kb, to_kb,
			    frame_handle_mapping_table, kb_local_only_p,
			    missing_frame_action, error_p, frame,
			    allocated_frame_handle_alist),
	 primitive_p, new_handle, pretty_name,
	 kb_local_only_p);
      
      Node decon_args = args;
      if (kb instanceof NetworkKb ||
	  to_kb instanceof NetworkKb)
	{  decon_args = kb.decontextualize(args, _value); }
      
      Node new_frame =
	to_kb.create_frame(decon_args.nth(0),  // name
			   (ContextName)(decon_args.nth(1)),  // frame_type
			   decon_args.nth(2),  // types
			   decon_args.nth(3),  // supers
			   decon_args.nth(4),  // doc
			   decon_args.nth(5),  // template_slots
			   decon_args.nth(6),  // template_facets
			   decon_args.nth(7),  // own_slots
			   decon_args.nth(8),  // own_facets
			   decon_args.nth(9),  // primitive_p
			   decon_args.nth(10), // handle
			   decon_args.nth(11), // pretty_name
			   (TruthValue)(decon_args.nth(12)) // kb_local_only_p
			    );
      
      return new Values3(new_frame, allocated_frame_handle_alist.cons,
			 _new_frame_created);
      
    }
    catch (StopCopy c) { return c.v; }
  }


  static Node missing_classes
  (Node frame, Node new_name, Kb kb, Kb to_kb, TruthValue error_p,
   Node type, Node types, Node superclasses, TruthValue kb_local_only_p,
   ConsLocation allocated_frame_handle_alist, 
   HashtableNode frame_handle_mapping_table) {
    Node missing = _NIL;
    Node all = (type==_class ? Cons.append_cons(types, superclasses) : types);
    Node converted = kb.allocate_any_necessary_frame_handles
      (all, frame_handle_mapping_table, kb_local_only_p,
       to_kb, _allocate, error_p, frame, allocated_frame_handle_alist);
    Enumeration eConverted = converted.elements();
    while (eConverted.hasMoreElements())
      {
	Node the_class = (Node)eConverted.nextElement();
	if (to_kb.frame_in_kb_p(the_class, kb_local_only_p)==_NIL)
	  missing = new Cons(the_class, missing);
      }
    return missing;
  }


  static Node fix_up_slot_specs
    (Node slot_specs, Kb from_kb, Kb to_kb, 
     HashtableNode frame_handle_mapping_table, TruthValue kb_local_only_p,
     Node missing_frame_action, TruthValue error_p, Node for_frame,
     ConsLocation allocated_frame_handle_alist) {
      boolean was_reified_p = from_kb.slots_are_reified_p();
      boolean will_be_reified_p = to_kb.slots_are_reified_p();
    return fix_up_1
     (slot_specs, from_kb, to_kb, frame_handle_mapping_table, kb_local_only_p,
      was_reified_p, will_be_reified_p, missing_frame_action, error_p,
      for_frame, _missing_slot, allocated_frame_handle_alist);
    }

  static Node fix_up_facet_specs
    (Node facet_specs, Kb from_kb, Kb to_kb, 
     HashtableNode frame_handle_mapping_table, TruthValue kb_local_only_p,
     Node missing_frame_action, TruthValue error_p, Node for_frame,
     ConsLocation allocated_frame_handle_alist) {
      boolean was_reified_p = from_kb.facets_are_reified_p();
      boolean will_be_reified_p = to_kb.facets_are_reified_p();
      Node new_specs = _NIL;
      Enumerator e = facet_specs.enumerate_list();
      while (e.has_more_p()) {
	Node spec = e.next();
	Node slot = spec.car();
	Node specs = spec.cdr();
	new_specs = new Cons(new Cons(slot,
				      fix_up_1
				      (specs, from_kb, to_kb,
				       frame_handle_mapping_table,
				       kb_local_only_p, was_reified_p,
				       will_be_reified_p,
				       missing_frame_action, error_p,
				       for_frame, _missing_slot, 
				       allocated_frame_handle_alist)),
			     new_specs);
      }
      return fix_up_slot_specs
       (new_specs, from_kb, to_kb, frame_handle_mapping_table, kb_local_only_p,
	missing_frame_action, error_p, for_frame, 
	allocated_frame_handle_alist);
    }

  static Node fix_up_1
     (Node slot_specs, Kb from_kb, Kb to_kb, 
      HashtableNode frame_handle_mapping_table, TruthValue kb_local_only_p,
      boolean was_reified_p, boolean will_be_reified_p, 
      Node missing_frame_action, TruthValue error_p,
      Node for_frame, Node missing_name, 
      ConsLocation allocated_frame_handle_alist) {
       if ((was_reified_p && will_be_reified_p) ||
	   (!was_reified_p && !will_be_reified_p))
	 { return from_kb.allocate_any_necessary_frame_handles
		    (slot_specs, frame_handle_mapping_table, kb_local_only_p,
		     to_kb, missing_frame_action, error_p, for_frame,
		     allocated_frame_handle_alist);
	 }
       else if (was_reified_p && !will_be_reified_p)
	 // then we need to dereify the slots
	 { Node result = _NIL;
	   Enumerator e = slot_specs.enumerate_list();
	   while (e.has_more_p()) {
	     Node entry = e.next();
	     Node slot = entry.car();
	     Node spec = entry.cdr();
	     Node allocated_spec =
	       from_kb.allocate_any_necessary_frame_handles
		    (spec, frame_handle_mapping_table, kb_local_only_p,
		     to_kb, missing_frame_action, error_p, for_frame,
		     allocated_frame_handle_alist);
	     Node frame = from_kb.coerce_to_frame
	       (slot, _NIL, kb_local_only_p).firstValue();
	     Node allocated_frame = _NIL;
	     if (frame != _NIL)
	       { allocated_frame = 
		   from_kb.allocate_any_necessary_frame_handles
		    (frame, frame_handle_mapping_table, kb_local_only_p,
		     to_kb, missing_frame_action, error_p, for_frame,
		     allocated_frame_handle_alist);
	       }
	     Node frame_to_use = allocated_frame;
	     if (allocated_frame == missing_frame_tag)
	       { frame_to_use = missing_name; }
	     if (frame_to_use != _NIL)
	       { result = new Cons(new Cons(from_kb.get_frame_name
					    (slot, kb_local_only_p),
					    allocated_spec),
				   result);
	       }
	     else if (slot instanceof AbstractSymbol)
	       { result = new Cons(new Cons(from_kb.get_frame_name
					    (slot, kb_local_only_p),
					    allocated_spec),
				   result);
	       }
	     else
	       { result = new Cons(new Cons(Symbol.intern
					    (slot.toString(),
					     "GENSYM-SLOT-AND-FACETS"),
					    allocated_spec),
				   result);
	       }
	   }
	   return result;
	 }
       else
	 { Node result = _NIL;
	   Enumerator e = slot_specs.enumerate_list();
	   while (e.has_more_p()) {
	     Node entry = e.next();
	     Node slot = entry.car();
	     Node spec = entry.cdr();
	     Node frame = from_kb.coerce_to_frame
	       (slot, _NIL, kb_local_only_p).firstValue();
	     Node allocated_spec =
	       from_kb.allocate_any_necessary_frame_handles
		    (spec, frame_handle_mapping_table, kb_local_only_p,
		     to_kb, missing_frame_action, error_p, for_frame,
		     allocated_frame_handle_alist);
	     Node allocated_frame = _NIL;
	     if (frame != _NIL)
	       { allocated_frame = 
		   from_kb.allocate_any_necessary_frame_handles
		    (frame, frame_handle_mapping_table, kb_local_only_p,
		     to_kb, missing_frame_action, error_p, for_frame,
		     allocated_frame_handle_alist);
	       }
	     Node frame_to_use = allocated_frame;
	     if (allocated_frame == missing_frame_tag)
	       { frame_to_use = missing_name; }
	     if (frame_to_use != _NIL)
	       { if (frame_to_use != missing_name)
		   { result = new Cons(new Cons(from_kb.get_frame_handle
						(frame_to_use,
						 kb_local_only_p),
						allocated_spec),
				       result);
		   }
		 else
		   { result = new Cons(new Cons(frame_to_use,
						allocated_spec),
				       result);
		   }
	       }
	     else if (slot instanceof AbstractSymbol)
	       { Node handle = to_kb.find_or_create_frame_handle_for_symbol
		   (slot, _individual, frame_handle_mapping_table,
		    allocated_frame_handle_alist);
		 result = new Cons(new Cons(handle, allocated_spec),
				   result);
	       }
	     else
	       { Node symbol = Symbol.intern(slot.toString(),
					     "GENSYM-SLOT-AND-FACETS");
		 Node handle = to_kb.find_or_create_frame_handle_for_symbol
		   (symbol, _individual, frame_handle_mapping_table,
		    allocated_frame_handle_alist);
		 result = new Cons(new Cons(handle, allocated_spec),
				   result);
	       }
	   }
	   return result;
	 }
     }

  static final Symbol get_kb_type_fname =
         Symbol.intern_in_ok_back("GET-KB-TYPE-INTERNAL");

  public static Kb get_kb_type(Node thing, Connection connection)
    { if (thing instanceof Kb)
	{ return (Kb)thing; }
      else if (thing instanceof AbstractSymbol)
       { if (connection instanceof NetworkConnection)
	   { Node result;
	     Node args =
	       Cons.list(get_kb_type_fname,
			 thing, _establish_local_connection);
	     Node values = 
	       ((NetworkConnection)connection).makeNetworkCall(args);
	     result = values.car();
	     if (result instanceof Kb) { return (Kb)result; }
	     else { throw new RuntimeException ("Cannot get KB-TYPE"); }
	   }
	 else
	   { AbstractSymbol abstract_name = 
	     abstract_type_from_concrete_type((AbstractSymbol)thing);
	     if (abstract_name == null)
	       { throw new RuntimeException 
		 ("Cannot get KB-TYPE from " + thing + 
		  ".  Maybe you need to call <<my_kb_type>>" +
		  ".ensure_registered()"); 
	       }
	     Kb kb = concrete_kb_from_abstract_kb(abstract_name);
	     if (kb == null)
	       { throw new RuntimeException 
		 ("Cannot get KB-TYPE.  Maybe you need to call " + kb +
		  ".ensure_registered()"); 
	       }
	     else { return kb; }
	   }
       }
      else { throw new RuntimeException ("Cannot get KB-TYPE"); }
    }

  public static Kb get_kb_type(Node thing)
    { return get_kb_type(thing, LocalConnection.local_connection()); }

  public static Kb get_kb_type()
    { throw new RuntimeException ("Cannot get KB-TYPE"); }

  public static Node static_call_procedure
     (Node procedure,
      Node arguments) {
      Kb kb = null;
      return static_call_procedure(kb, procedure, arguments);
    }

  static void err_on_missing_procedure (Node name) {
    Hashtable ht = Procedure.name_to_procedure_table;
    System.out.println("Procedure " + name + " not found.");
    Enumeration e = ht.keys();
    System.out.println("Current known procedure names:");
    for (; e.hasMoreElements() ;) {
      Object key = e.nextElement();
      System.out.println("    " + key);
    }
    e = ht.keys();
    System.out.println("Current known procedures:");
    for (; e.hasMoreElements() ;) {
      Object key = e.nextElement();
      System.out.println("    " + key + " -> " + ht.get(key));
    }
    throw new GenericError
      (_T, new OKBCString(name + " does not name a procedure")).signal(); 
  }
  
  public static Node static_call_procedure
     (Kb kb,
      Node procedure,
      Node arguments) {
       Procedure funspec = null;
       if (procedure instanceof AbstractSymbol)
	 { Hashtable ht = Procedure.name_to_procedure_table;
	   Procedure res = 
	     (Procedure) ht.get
	      (((AbstractSymbol)procedure).symbolName);
	   if (res == null)
	     { err_on_missing_procedure (procedure); }
	   else { funspec = (Procedure)res; }
	 }
       else { funspec = (Procedure)procedure; }
       return funspec.apply_with_kb_environment(kb, arguments, 0);
    }

  public static Node connection_p(Object o) {
    if (o instanceof Connection) { return _T; }
    else { return _NIL; }
  }


  public static Node continuable_error_p(Object o) {
    if (o instanceof AbstractError)
      { AbstractError e = (AbstractError)o;
	if (e.continuable != _NIL) { return _T; }
	else { return _NIL; }
      }
    else { return _NIL; }
  }


  public static Node procedure_p(Object o) {
    if (o instanceof Procedure) { return _T; }
    else { return _NIL; }
  }


  public static Connection local_connection() {
    return LocalConnection.local_connection();
  }

  public static Node current_kb() { 
    if (the_current_kb == null) { return _NIL; }
    else { return the_current_kb; }
  }

  static String error_message_for_illegal_access (Class kb_class, Node thing, 
						  Exception exception)
  { String extra_string = "";
    if (thing != _NIL) { extra_string = " (" + thing + ")"; }
    return ("Couldn't instantiate " + kb_class + extra_string +	    
	    ".  You probably haven't made " +
	    kb_class.getName() + " public, and defined a " +
	    "public, no argument initialization method:  public " +
	    kb_class.getName() + " () {}, or the KB hasn't been " +
	    "properly registered using " + kb_class.getName() +
	    ".ensure_registered();.  The exception thrown was " + exception);
  }

  // Stub debug methods.
  protected Node debug_node(Node args) { return _NIL; }


  public static final Node OKBC_implementation_version =
  Cons.list(new OKBCInteger(2),
	    new OKBCInteger(1),
	    new OKBCInteger(32));

  public static final Node OKBC_protocol_version =
  Cons.list(new OKBCInteger(2),
	    new OKBCInteger(1),
	    new OKBCInteger(1));


  // Internal protocol used by the code walker.
  public boolean frame_reference_p (Kb kb) {
    return kb.coercible_to_frame_p(this, _NIL) != _NIL;
  }

}
