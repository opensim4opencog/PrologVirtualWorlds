/***********************************************************************

  OKBC demos for Java

  Copyright (c) 1996, 1997, 1998 Stanford University KSL.

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

package demos;
import java.util.*;
import java.io.*;
import OKBC.*;

public class KbSummary {

  public static void main (String argv[]) {
    Kb kb = load_test_kb();
    while (true) { find_and_summarize_kb(); }
  }

  // Make sure that we have at least one local KB defined, the Test TupleKb.
  static Kb load_test_kb () {
    TupleKb.ensure_registered();
    Symbol test_name = Symbol.intern("TAXA", "OKBC-TEST");
    Kb kb_type = Node.get_kb_type(Symbol.intern("TUPLE-KB", "OK-BACK"));
    Kb kb = kb_type.create_kb (test_name, Node._NIL, Node._NIL,
			       LocalConnection.local_connection());
    Test t = new Test();
    t.initialize_test_kb(kb, false);
    System.out.println("Finished initializing test KB.");
    return kb;
  }
  

// ***********************************************************************
// Code to handle opening connections to an OKBV server, and finding
// a KB on that server.

  static final Node _host = Symbol.keyword("HOST");
  static final Node _port = Symbol.keyword("PORT");
  static final Node _user_id = Symbol.keyword("USER-ID");
  static final Node _password = Symbol.keyword("PASSWORD");
  static final Node _session_id = Symbol.keyword("SESSION-ID");
  static final Node _key = Symbol.keyword("KEY");
  static final Node _kb_library = Symbol.keyword("KB-LIBRARY");
  static final Node _a = Symbol.keyword("A");
  
// Start get-connection-for-kb-summary
  static final Node local_connection_type
    = Symbol.intern("LOCAL-CONNECTION", "OK-BACK");
  static final Node simple_connection_type
    = Symbol.intern("SIMPLE-NETWORK-CONNECTION", "OK-BACK");
  static final Node ksl_connection_type
    = Symbol.intern("KSL-NETWORK-CONNECTION", "KSL-OKBC");

  static Connection get_connection_for_kb_summary() {
    LocalConnection.ensure_registered();
    SimpleNetworkConnection.ensure_registered();
    KslNetworkConnection.ensure_registered();
    Node connection_type = menu_choose
      ("Pick a connection type:",
       Cons.list(Cons.list(local_connection_type,
			   new OKBCString("Local connection")),
		 Cons.list(simple_connection_type,
			   new OKBCString("Simple network connection")),
		 Cons.list(ksl_connection_type,
			   new OKBCString("KSL network connection"))));
    if (connection_type == local_connection_type)
      { return Node.local_connection(); }
    else if (connection_type == simple_connection_type)
      { 
	OKBCString host  = prompt_and_read_string("Host");
	OKBCInteger port = prompt_and_read_integer("Port");
	return Node.establish_connection(connection_type,
					 Cons.list(_host, host,
						   _port, port));
      }
    else if (connection_type == ksl_connection_type)
      { return get_ksl_connection(connection_type); }
    else { throw new GenericError
	   (Node._T, new OKBCString("Unknown connection type: " +
				    connection_type)).signal();
	 }
  }
// End get-connection-for-kb-summary

  
// Start get-ksl-connection
  static Connection get_ksl_connection (Node connection_type) {
    OKBCString host  = prompt_and_read_string("Host");
    OKBCInteger port = prompt_and_read_integer("Port");
    OKBCString user_id  = prompt_and_read_string("User ID", "Alien");
    OKBCString password = null;
    if (user_id.string.equalsIgnoreCase("Alien"))
      { password = new OKBCString(""); }
    else { password = prompt_and_read_string("Password"); }
    SessionDescriptor[] sessions =
      NetworkKb.active_sessions
      (host.string, (int)(port.number), user_id.string,
       password.string, "");
    Node session = null;
    if (user_id.string.equalsIgnoreCase("Alien"))
      { session = sessions[0]; }
    else
      {
	Node menu_items = Node._NIL;
	for (int i=0; i < sessions.length; i++) {
	  SessionDescriptor s = sessions[i];
	  menu_items = new Cons(Cons.list
				(s,
				 new OKBCString(s.session_description +
						" owned by " +
						s.group_ownership)),
				menu_items);
	}
	menu_items = new Cons(Cons.list(Node._NIL,
					new OKBCString
					("Create a new session")),
			      menu_items);
	session = menu_choose("Select a session to open:", menu_items);
      }
    SessionResponse resp = null;
    if (session != Node._NIL)
      {
	SessionDescriptor s = (SessionDescriptor)session;
	resp = NetworkKb.login_user_and_connect_to_session
	  (host.string, (int)(port.number), user_id.string,
	   password.string, s.session_id);
      }
    else
      {
	String[] groups = NetworkKb.get_groups
	  (host.string, (int)(port.number), user_id.string,
	   password.string, "");
	Node menu_items = Node._NIL;
	for (int i=0; i < groups.length; i++) {
	  String g = groups[i];
	  menu_items = new Cons(Cons.list
				(new OKBCString(g), new OKBCString(g)),
				menu_items);
	}
	OKBCString group = (OKBCString)
	  menu_choose("Select a group to own a new session.", menu_items);
	OKBCString description =
	  prompt_and_read_string("Session description");
	OKBCInteger duration =
	  prompt_and_read_integer("Session duration in hours");
	resp = NetworkKb.login_user_and_create_session
	  (host.string, (int)(port.number), user_id.string,
	   password.string, group.string, description.string,
	   (int)(duration.number));
      }
    Connection connection =
      Node.establish_connection(connection_type,
				Cons.list(_host, host,
					  _port, port,
					  _user_id, user_id,
					  _password, password,
					  _session_id,
					  new OKBCString(resp.session_id),
					  _key, new OKBCString
					  (resp.session_key),
					  _kb_library, _a));
    connection.set_connection_persistence(true);
    return connection;
  }
// End get-ksl-connection


// Start get-kb-type-for-kb-summary
  static Kb get_kb_type_for_kb_summary (Connection connection) {
    Node menu_items = Node._NIL;
    Enumerator kb_types_e = Node.enumerate_kb_types(connection);
    kb_types_e.prefetch();
    while (kb_types_e.has_more_p()) {
      Kb kb_type = (Kb)(kb_types_e.next());
      Node name = Kb.frs_name(kb_type, connection);
      menu_items = new Cons(Cons.list(kb_type, name), menu_items);
    }
    kb_types_e.free();
    return (Kb)menu_choose("Pick a KB type", menu_items);
  }
// End get-kb-type-for-kb-summary


// Start get-kb-for-kb-summary
  static Kb get_kb_for_kb_summary (Connection connection) {
    Kb result = null;
    while (true) {
      Kb kb_type = get_kb_type_for_kb_summary(connection);
      Kb meta_kb = (Kb)(Kb.meta_kb(connection));
      Node kbs = kb_type.get_kbs_of_type(connection);
      if (kbs != Node._NIL)
	{
	  Node menu_items = Node._NIL;
	  Enumerator e = kbs.enumerate_list();
	  while (e.has_more_p()) {
	    Kb kb = (Kb)(e.next());
	    menu_items =
	      new Cons(Cons.list(kb, meta_kb.get_frame_pretty_name(kb)),
		       menu_items);
	  }
	  e.free();
	  result = (Kb)menu_choose ("Pick a KB", Cons.reverse(menu_items));
	  break;
	}
      else
	{
	  System.out.println("Sorry, there are no KBs for KB type " +
			     Kb.frs_name(kb_type, connection)
			     + ".  Please try some other KB type.");
	}
    }
    return result;
  }
// End get-kb-for-kb-summary


// Start find-and-summarize-kb
  public static void find_and_summarize_kb () {
    Connection connection = null;
    try
      {
	connection = get_connection_for_kb_summary();
	Kb kb = get_kb_for_kb_summary(connection);
	InferenceLevel inference_level
	  = (InferenceLevel) menu_choose
	  ("Inference level",
	   Cons.list(Cons.list(Node._direct, new OKBCString("Direct")),
		     Cons.list(Node._taxonomic, new OKBCString("Taxonomic")),
		     Cons.list(Node._all_inferable,
			       new OKBCString("All inferable"))));
	OKBCInteger version_switch
	  = (OKBCInteger) menu_choose
	  ("Pick a summary method",
	   Cons.list(Cons.list(new OKBCInteger(SIMPLE),
			       new OKBCString("Simple")),
		     Cons.list(new OKBCInteger(DETAILS),
			       new OKBCString("Using get-frame-details")),
		     Cons.list(new OKBCInteger(FILTERED),
			       new OKBCString("Filtered")),
		     Cons.list(new OKBCInteger(FILTERED_ON_SERVER),
			       new OKBCString("Filtered on the server"))));
	boolean just_frames_in_this_kb_p =
	  y_or_n_p("Tabulate only frames in this KB? ");
	OKBCString path = prompt_and_read_string("Output file");
	    try
	      { 
		File file = new File(path.string);
		FileWriter stream = new FileWriter(file.getPath());
		htmlify_kb_summary
		  (stream, kb, inference_level, (int)(version_switch.number),
		   "L", true);
	      }
	catch (IOException e)
	  { System.out.println("Error: " + e); }
	
	Node.close_connection(connection);
	connection = null;
      }
    finally
      {
	if (connection != null)
	  { System.out.println("Closing connection " + connection +
			       " due to abnormal exit.");
	    Node.close_connection(connection);
	  }
      }
  }
// End find-and-summarize-kb

//============================================================
//============================================================
// HTML support stuff for the KB summarizer.

// Start html-support
  static void start_table (Writer stream) throws IOException {
    stream.write("\n");
    stream.write("<TABLE border=1>");
  }

  static void end_table (Writer stream) throws IOException {
    stream.write("\n");
    stream.write("</TABLE>");
  }

  static void start_table_row (Writer stream) throws IOException {
    stream.write("\n");
    stream.write("<TR>");
  }

  static void end_table_row (Writer stream) throws IOException {
    stream.write("\n");
    stream.write("</TR>");
  }

  static void start_table_cell_internal (Writer stream, String tag)
       throws IOException {
    stream.write("<" + tag + " valign=top>");
  }
  
  static void start_table_header_cell (Writer stream) throws IOException {
    start_table_cell_internal(stream, "TH");
  }
  
  static void end_table_header_cell (Writer stream) throws IOException {
    stream.write("</TH>");
  }
  
  static void start_table_cell (Writer stream) throws IOException {
    start_table_cell_internal(stream, "TD");
  }
  
  static void end_table_cell (Writer stream) throws IOException {
    stream.write("</TD>");
  }

  static void start_italics (Writer stream) throws IOException {
    stream.write("<I>");
  }
  
  static void end_italics (Writer stream) throws IOException {
    stream.write("</I>");
  }
  
  static void write_html(Writer stream, String string) throws IOException {
    for (int i=0; i < string.length() ; i++) {
      char c = string.charAt(i);
      if (c == '<') { stream.write("&lt;"); }
      else if (c == '>') { stream.write("&gt;"); }
      else if (c == '"') { stream.write("&quot;"); }
      else if (c == '&') { stream.write("&amp;"); }
      else { stream.write(c); }
    }
  }
  
  static void write_html(Writer stream, Node node) throws IOException {
    write_html(stream, node.toString());
  }
// End html-support

// ***********************************************************************
// The KB summarizer
  
// Start htmlify-kb-summary
  static final int SIMPLE = 0; 
  static final int DETAILS = 1; 
  static final int FILTERED = 2; 
  static final int FILTERED_ON_SERVER = 3; 

  public static void htmlify_kb_summary
    (Writer stream, Kb kb, InferenceLevel inference_level, int version_switch,
     String link_tag, boolean just_frames_in_this_kb_p)
       throws IOException
  {
    TruthValue just_in_this_kb_p = (just_frames_in_this_kb_p
				  ? (TruthValue)Node._T
				  : (TruthValue)Node._NIL);
    if (version_switch == SIMPLE)
      { tabulate_kb_frames_simple(stream, kb, inference_level,
				  just_in_this_kb_p, link_tag, false);
      }
    else if (version_switch == DETAILS)
      { tabulate_kb_frames_details(stream, kb, inference_level,
				   just_in_this_kb_p, link_tag, false);
      }
    else if (version_switch == FILTERED)
      { tabulate_kb_frames_filtered(stream, kb, inference_level,
				    just_in_this_kb_p, link_tag, true);
      }
    else if (version_switch == FILTERED_ON_SERVER)
      { tabulate_kb_frames_filtered_on_server(stream, kb, inference_level,
                                              just_in_this_kb_p,
					      link_tag, true);
      }
    else { System.out.println("Switch option out of range."); }
    stream.flush();
  }
// End htmlify-kb-summary


// Start tabulate-kb-frames-simple
  static void tabulate_kb_frames_simple
    (Writer stream, Kb kb, InferenceLevel inference_level,
     TruthValue just_frames_in_this_kb_p, String link_tag,
     boolean frame_must_be_there_to_link_p)
       throws IOException
  {
    start_table(stream);
    {
      print_table_column_headers(stream);
      Enumerator frames_e = kb.enumerate_kb_frames(just_frames_in_this_kb_p);
      Hashtable link_table = new Hashtable();

      frames_e.prefetch();
      while (frames_e.has_more_p()) {
	Node frame = frames_e.next();
	tabulate_frame_simple(stream, frame, kb, inference_level,
			      just_frames_in_this_kb_p, link_table, link_tag,
			      frame_must_be_there_to_link_p);
      }
      frames_e.free();
    }
    end_table(stream);
  }
// End tabulate-kb-frames-simple


// Start print-table-column-headers
  static void print_table_column_headers (Writer stream) throws IOException {
    start_table_row(stream);
    {
      start_table_header_cell(stream);
      { stream.write("Frame"); }
      end_table_header_cell(stream);
      start_table_header_cell(stream);
      { stream.write("Type"); }
      end_table_header_cell(stream);
      start_table_header_cell(stream);
      { stream.write("Own Slots"); }
      end_table_header_cell(stream);
      start_table_header_cell(stream);
      { stream.write("Template slots"); }
      end_table_header_cell(stream);
    }
    end_table_row(stream);
  }
// End print-table-column-headers


// Start maybe-allocate-link-name
  static Integer maybe_allocate_link_name
      (Node frame, Kb kb, TruthValue just_frames_in_this_kb_p,
       Hashtable link_table, boolean frame_must_be_there_to_link_p)
  {
    if (kb.frame_in_kb_p(frame, just_frames_in_this_kb_p) != Node._NIL)
      {
	Object match = link_table.get(frame);
	if (match != null) { return (Integer)match; }
	else if (frame_must_be_there_to_link_p) { return null; }
	else { Integer index = new Integer(link_table.size());
	       link_table.put(frame, index);
	       return index;
	     }
      }
    else { return null; }
  }
// End maybe-allocate-link-name


// Start tabulate-frame-simple
  static void tabulate_frame_simple
    (Writer stream, Node frame, Kb kb, InferenceLevel inference_level,
     TruthValue just_frames_in_this_kb_p, Hashtable link_table,
     String link_tag, boolean frame_must_be_there_to_link_p)
       throws IOException
  {
    Integer link_name = maybe_allocate_link_name
      (frame, kb, just_frames_in_this_kb_p, link_table,
       frame_must_be_there_to_link_p);
    Node frame_type = kb.get_frame_type(frame);
    Enumerator own_slots_e = kb.enumerate_frame_slots
      (frame, inference_level, Node._own);
    Enumerator template_slots_e = null;
    if (frame_type == Node._class)
      { template_slots_e = kb.enumerate_frame_slots
	  (frame, inference_level, Node._template);
      }
    start_table_row(stream);
    {
      start_table_cell(stream);
      {
	stream.write("<A name=\"" + link_tag + link_name + "\">");
	write_html(stream, kb.value_as_string(frame, Node._user_interface,
					      Node._T).firstValue());
      }
      end_table_cell(stream);

      start_table_cell(stream);
      {
	print_value_maybe_with_link
	  (stream, frame_type, kb, just_frames_in_this_kb_p, link_table,
	   link_tag, frame_must_be_there_to_link_p);
      }
      end_table_cell(stream);
      start_table_cell(stream);
      {
	own_slots_e.prefetch();
	tabulate_slots_in_frame_simple
	  (stream, frame, own_slots_e, Node._own, kb,
	   inference_level, just_frames_in_this_kb_p, link_table,
	   link_tag, frame_must_be_there_to_link_p);
	own_slots_e.free();
      }
      end_table_cell(stream);

      if (template_slots_e != null)
	{
	  start_table_cell(stream);
	    {
	      template_slots_e.prefetch();
	      tabulate_slots_in_frame_simple
		(stream, frame, template_slots_e, Node._template, kb,
		 inference_level, just_frames_in_this_kb_p, link_table,
		 link_tag, frame_must_be_there_to_link_p);
	      template_slots_e.free();
	    }
	  end_table_cell(stream);
	}
    }
    end_table_row(stream);
  }
// End tabulate-frame-simple


// Start print-value-maybe-with-link
  static void print_value_maybe_with_link
      (Writer stream, Node value, Kb kb, TruthValue just_frames_in_this_kb_p,
       Hashtable link_table, String link_tag,
       boolean frame_must_be_there_to_link_p) throws IOException
  {
    Values2 vals = kb.value_as_string(value, Node._user_interface, Node._T);
    String string = ((OKBCString)(vals.firstValue())).string;
    Node location_list = vals.secondValue();
    int index = 0;
    Enumerator location_e = location_list.enumerate_list();
    while (location_e.has_more_p()) {
      Node spec = location_e.next();
      int start = (int)(((OKBCInteger)spec.car()).number);
      int end = (int)(((OKBCInteger)spec.cadr()).number);
      Node frame = spec.nth(2);
      Integer link = maybe_allocate_link_name
	(frame, kb, just_frames_in_this_kb_p, link_table, 
	 frame_must_be_there_to_link_p);
      if (link != null)
	{
	  stream.write("<A href=\"#" + link_tag + link + "\">");
	  write_html(stream, string.substring(start, end));
	  stream.write("</A>");
	}
      else { write_html(stream, string.substring(start, end)); }
      index = end;
    }
    location_e.free();
    write_html(stream, string.substring(index));
  }
// End print-value-maybe-with-link


// Start slot-or-facet-table-header-row
  static void slot_or_facet_table_header_row 
     (Writer stream, String type_string) throws IOException {
    start_table_row(stream);
    {
      start_table_header_cell(stream);
      { stream.write(type_string); }
      end_table_header_cell(stream);
      start_table_header_cell(stream);
      { stream.write("Values"); }
      end_table_header_cell(stream);
      start_table_header_cell(stream);
      { stream.write("DValues"); }
      end_table_header_cell(stream);
    }
    end_table_row(stream);
  }
// End slot-or-facet-table-header-row


// Start tabulate-slots-in-frame-simple
  static void tabulate_slots_in_frame_simple
  (Writer stream, Node frame, Enumerator slots_e, SlotType slot_type, Kb kb, 
   InferenceLevel inference_level, TruthValue just_frames_in_this_kb_p,
   Hashtable link_table, String link_tag, 
   boolean frame_must_be_there_to_link_p) throws IOException
    {
      if (slots_e.has_more_p())
	{ 
	  start_table(stream);
	  {
	    slot_or_facet_table_header_row(stream, "Slot");
	    while (slots_e.has_more_p()) {
	      Node slot = slots_e.next();
	      Enumerator values_e = kb.enumerate_slot_values
		(frame, slot, inference_level, slot_type, Node._all,
		 Node._known_true);
	      Enumerator default_values_e = kb.enumerate_slot_values
		(frame, slot, inference_level, slot_type, Node._all,
		 Node._default_only);

	      start_table_row(stream);
	      {
		start_table_cell(stream);
		{
		  print_value_maybe_with_link
		    (stream, slot, kb, just_frames_in_this_kb_p, link_table,
		     link_tag, frame_must_be_there_to_link_p);
		}
		end_table_cell(stream);

		start_table_cell(stream);
		{
		  values_e.prefetch();
		  boolean firstp = true;
		  while (values_e.has_more_p()) {
		    Node value = values_e.next();
		    if (!firstp) { stream.write(", "); }
		    print_value_maybe_with_link
		      (stream, value, kb, just_frames_in_this_kb_p, 
		       link_table, link_tag, frame_must_be_there_to_link_p);
		    firstp = false;
		  }
		  values_e.free();
		}
		end_table_cell(stream);

		start_table_cell(stream);
		{
		  start_italics(stream);
		  {
		    default_values_e.prefetch();
		    boolean firstp = true;
		    while (default_values_e.has_more_p()) {
		      Node value = default_values_e.next();
		      if (!firstp) { stream.write(", "); }
		      print_value_maybe_with_link
			(stream, value, kb, just_frames_in_this_kb_p,
			 link_table, link_tag, frame_must_be_there_to_link_p);
		      firstp = false;
		    }
		    default_values_e.free();
		  }
		  end_italics(stream);
		}
		end_table_cell(stream);
	      }
	      end_table_row(stream);
	      tabulate_facets_in_slot_simple
		(stream, frame, slot, slot_type, kb, inference_level,
		 just_frames_in_this_kb_p, link_table, link_tag,
		 frame_must_be_there_to_link_p);
	    }
	  }
	  end_table(stream);
	}
      else { stream.write("<I>None</I>"); }
    }
// End tabulate-slots-in-frame-simple  


// Start tabulate-facets-in-slot-simple
  static void tabulate_facets_in_slot_simple
     (Writer stream, Node frame, Node slot, SlotType slot_type, Kb kb,
      InferenceLevel inference_level, TruthValue just_frames_in_this_kb_p,
      Hashtable link_table, String link_tag, 
      boolean frame_must_be_there_to_link_p) throws IOException
    {
      Enumerator facets_e = kb.enumerate_slot_facets
	(frame, slot, inference_level, slot_type);
      if (facets_e.has_more_p())
	{
	  start_table_row(stream);
	  {
	    start_table_cell(stream);
	    {} // empty cell
	    end_table_cell(stream);
	    start_table_cell(stream);
	    {
	      start_table(stream);
	      {
		slot_or_facet_table_header_row(stream, "Facet");
		facets_e.prefetch();
		while (facets_e.has_more_p()) {
		  Node facet = facets_e.next();
		  Enumerator facet_values_e = kb.enumerate_facet_values
		    (frame, slot, facet, inference_level, slot_type,
		     Node._all, Node._known_true);
		  Enumerator default_facet_values_e = kb.enumerate_facet_values
		    (frame, slot, facet, inference_level, slot_type,
		     Node._all, Node._default_only);
		  start_table_row(stream);
		  {
		    start_table_cell(stream);
		    {
		      print_value_maybe_with_link
			(stream, facet, kb, just_frames_in_this_kb_p,
			 link_table, link_tag, frame_must_be_there_to_link_p);
		    }
		    end_table_cell(stream);

		    start_table_cell(stream);
		    {
		      facet_values_e.prefetch();
		      boolean firstp = true;
		      while (facet_values_e.has_more_p()) {
			Node value = facet_values_e.next();
			if (!firstp) { stream.write(", "); }
			print_value_maybe_with_link
			  (stream, value, kb, just_frames_in_this_kb_p, 
			   link_table, link_tag, 
			   frame_must_be_there_to_link_p);
			firstp = false;
		      }
		      facet_values_e.free();
		    }
		    end_table_cell(stream);

		    start_table_cell(stream);
		    {
		      start_italics(stream);
		      {
			default_facet_values_e.prefetch();
			boolean firstp = true;
			while (default_facet_values_e.has_more_p()) {
			  Node value = default_facet_values_e.next();
			  if (!firstp) { stream.write(", "); }
			  print_value_maybe_with_link
			    (stream, value, kb, just_frames_in_this_kb_p, 
			     link_table, link_tag, 
			     frame_must_be_there_to_link_p);
			  firstp = false;
			}
			default_facet_values_e.free();
		      }
		      end_italics(stream);
		    }
		    end_table_cell(stream);
		  }
		  end_table_row(stream);
		}
	      }
	      end_table(stream);
	    }
	    end_table_cell(stream);
	  }
	  end_table_row(stream);
	}
      facets_e.free();
    }
// End tabulate-facets-in-slot-simple

  
//=========================================================================
// Code to handle summarizing a KB by calling get_frame_details.
  
// Start tabulate-kb-frames-details
  static void tabulate_kb_frames_details
    (Writer stream, Kb kb, InferenceLevel inference_level,
     TruthValue just_frames_in_this_kb_p, String link_tag,
     boolean frame_must_be_there_to_link_p)
       throws IOException
  {
    start_table(stream);
    {
      print_table_column_headers(stream);
      Enumerator frames_e = kb.enumerate_kb_frames(just_frames_in_this_kb_p);
      Hashtable link_table = new Hashtable();

      frames_e.prefetch();
      while (frames_e.has_more_p()) {
	Node frame = frames_e.next();
	tabulate_frame_details(stream, frame, kb, inference_level,
			       just_frames_in_this_kb_p, link_table, link_tag,
			       frame_must_be_there_to_link_p);
      }
      frames_e.free();
    }
    end_table(stream);
  }
// End tabulate-kb-frames-details


// Start tabulate-frame-details
  static void tabulate_frame_details
    (Writer stream, Node frame, Kb kb, InferenceLevel inference_level,
     TruthValue just_frames_in_this_kb_p, Hashtable link_table,
     String link_tag, boolean frame_must_be_there_to_link_p)
       throws IOException
  {
    Node details = kb.get_frame_details
      (frame, inference_level, Node._all).firstValue();
    Integer link_name = maybe_allocate_link_name
      (frame, kb, just_frames_in_this_kb_p, link_table,
       frame_must_be_there_to_link_p);
    Node frame_type = Cons.getf(details, Node._frame_type);
    Enumerator own_slots_e =
      Cons.getf(details, Node._own_slots).enumerate_list();
    Enumerator template_slots_e = null;
    if (frame_type == Node._class)
      { template_slots_e = Cons.getf(details, Node._template_slots)
	  .enumerate_list();
      }
    start_table_row(stream);
    {
      start_table_cell(stream);
      {
	stream.write("<A name=\"" + link_tag + link_name + "\">");
	write_html(stream, kb.value_as_string(frame, Node._user_interface,
					      Node._T).firstValue());
      }
      end_table_cell(stream);

      start_table_cell(stream);
      {
	print_value_maybe_with_link
	  (stream, frame_type, kb, just_frames_in_this_kb_p, link_table,
	   link_tag, frame_must_be_there_to_link_p);
      }
      end_table_cell(stream);
      start_table_cell(stream);
      {
	own_slots_e.prefetch();
	tabulate_slots_in_frame_details
	  (stream, own_slots_e, Node._own, details, kb,
	   just_frames_in_this_kb_p, link_table, link_tag,
	   frame_must_be_there_to_link_p);
	own_slots_e.free();
      }
      end_table_cell(stream);

      if (template_slots_e != null)
	{
	  start_table_cell(stream);
	    {
	      template_slots_e.prefetch();
	      tabulate_slots_in_frame_details
		(stream, template_slots_e, Node._template, details, kb,
		 just_frames_in_this_kb_p, link_table, link_tag,
		 frame_must_be_there_to_link_p);
	      template_slots_e.free();
	    }
	  end_table_cell(stream);
	}
    }
    end_table_row(stream);
  }
// End tabulate-frame-details


// Start tabulate-slots-in-frame-details
  static void tabulate_slots_in_frame_details
  (Writer stream, Enumerator slots_e, SlotType slot_type, Node details, Kb kb, 
   TruthValue just_frames_in_this_kb_p, Hashtable link_table, String link_tag, 
   boolean frame_must_be_there_to_link_p) throws IOException
    {
      if (slots_e.has_more_p())
	{ 
	  start_table(stream);
	  {
	    slot_or_facet_table_header_row(stream, "Slot");
	    while (slots_e.has_more_p()) {
	      Node slot_spec = slots_e.next();
	      Node slot = slot_spec.car();
	      Node values = slot_spec.cdr();
	      if (values == null) { values = Node._NIL; }
	      Enumerator values_e = values.enumerate_list();
	      Node default_values = Node._NIL;

	      start_table_row(stream);
	      {
		start_table_cell(stream);
		{
		  print_value_maybe_with_link
		    (stream, slot, kb, just_frames_in_this_kb_p, link_table,
		     link_tag, frame_must_be_there_to_link_p);
		}
		end_table_cell(stream);

		start_table_cell(stream);
		{
		  values_e.prefetch();
		  boolean firstp = true;
		  while (values_e.has_more_p()) {
		    Node value = values_e.next();
		    if (!firstp) { stream.write(", "); }
		    if (value instanceof Cons &&
			value.car() == Node._default)
		      { default_values = 
			  new Cons(value.cadr(), default_values);
		      }
		    else
		      {
			print_value_maybe_with_link
			  (stream, value, kb, just_frames_in_this_kb_p, 
			   link_table, link_tag,
			   frame_must_be_there_to_link_p);
			firstp = false;
		      }
		  }
		  values_e.free();
		}
		end_table_cell(stream);
		print_out_default_values
		  (stream, default_values, kb, just_frames_in_this_kb_p,
		   link_table, link_tag, frame_must_be_there_to_link_p);
	      }
	      end_table_row(stream);
	      tabulate_facets_in_slot_details
		(stream, slot, slot_type, details, kb,
		 just_frames_in_this_kb_p, link_table, link_tag,
		 frame_must_be_there_to_link_p);
	    }
	  }
	  end_table(stream);
	}
      else { stream.write("<I>None</I>"); }
    }
// End tabulate-slots-in-frame-details


// Start print-out-default-values
  static void print_out_default_values
    (Writer stream, Node default_values, Kb kb, 
     TruthValue just_frames_in_this_kb_p, Hashtable link_table,
     String link_tag, boolean frame_must_be_there_to_link_p) throws IOException
    {
      start_table_cell(stream);
      {
	Enumerator default_values_e = default_values.enumerate_list();
	start_italics(stream);
	{
	  default_values_e.prefetch();
	  boolean firstp = true;
	  while (default_values_e.has_more_p()) {
	    Node value = default_values_e.next();
	    if (!firstp) { stream.write(", "); }
	    print_value_maybe_with_link
	      (stream, value, kb, just_frames_in_this_kb_p,
	       link_table, link_tag, frame_must_be_there_to_link_p);
	    firstp = false;
	  }
	  default_values_e.free();
	}
	end_italics(stream);
      }
      end_table_cell(stream);
    }
// End print-out-default-values


// Start tabulate-facets-in-slot-details
  static void tabulate_facets_in_slot_details
     (Writer stream, Node slot, SlotType slot_type, Node details, Kb kb,
      TruthValue just_frames_in_this_kb_p, Hashtable link_table,
      String link_tag, boolean frame_must_be_there_to_link_p) 
    throws IOException
    {
      Enumerator facets_e =
	Cons.assoc(slot, Cons.getf(details,
				   (slot_type == Node._own
				    ? Node._own_facets
				    : Node._template_facets)))
	          .cdr().enumerate_list();
      if (facets_e.has_more_p())
	{
	  start_table_row(stream);
	  {
	    start_table_cell(stream);
	    {} // empty cell
	    end_table_cell(stream);
	    start_table_cell(stream);
	    {
	      start_table(stream);
	      {
		slot_or_facet_table_header_row(stream, "Facet");
		facets_e.prefetch();
		while (facets_e.has_more_p()) {
		  Node facet_spec = facets_e.next();
		  Node facet = facet_spec.car();
		  Node values = facet_spec.cdr();
		  if (values == null) { values = Node._NIL; }
		  Enumerator facet_values_e = values.enumerate_list();
		  Node default_values = Node._NIL;
		  start_table_row(stream);
		  {
		    start_table_cell(stream);
		    {
		      print_value_maybe_with_link
			(stream, facet, kb, just_frames_in_this_kb_p,
			 link_table, link_tag, frame_must_be_there_to_link_p);
		    }
		    end_table_cell(stream);

		    start_table_cell(stream);
		    {
		      facet_values_e.prefetch();
		      boolean firstp = true;
		      while (facet_values_e.has_more_p()) {
			Node value = facet_values_e.next();
			if (!firstp) { stream.write(", "); }
			if (value instanceof Cons &&
			    value.car() == Node._default)
			  { default_values = new Cons(value.cadr(),
						      default_values);
			  }
			else
			  {
			    print_value_maybe_with_link
			      (stream, value, kb, just_frames_in_this_kb_p, 
			       link_table, link_tag, 
			       frame_must_be_there_to_link_p);
			    firstp = false;
			  }
		      }
		      facet_values_e.free();
		    }
		    end_table_cell(stream);

		    print_out_default_values
		      (stream, default_values, kb, just_frames_in_this_kb_p,
		       link_table, link_tag, frame_must_be_there_to_link_p);
		  }
		  end_table_row(stream);
		}
	      }
	      end_table(stream);
	    }
	    end_table_cell(stream);
	  }
	  end_table_row(stream);
	}
      facets_e.free();
    }
// End tabulate-facets-in-slot-details
  
//=========================================================================
// Code to summarize a filtered subset of the frames in a KB by running
// the filter on the client side.

// Start tabulate-kb-frames-filtered
  static void tabulate_kb_frames_filtered
    (Writer stream, Kb kb, InferenceLevel inference_level,
     TruthValue just_frames_in_this_kb_p, String link_tag,
     boolean frame_must_be_there_to_link_p)
       throws IOException
  {
    start_table(stream);
    {
      print_table_column_headers(stream);
      Node frames = get_filtered_frames(kb, inference_level,
					just_frames_in_this_kb_p);
      Hashtable link_table = new Hashtable();
      preallocate_links_for_frames
	(frames, kb, just_frames_in_this_kb_p, link_table);
      Enumerator frames_e = frames.enumerate_list();
      frames_e.prefetch();
      while (frames_e.has_more_p()) {
	Node frame = frames_e.next();
	tabulate_frame_details(stream, frame, kb, inference_level,
			       just_frames_in_this_kb_p, link_table, link_tag,
			       frame_must_be_there_to_link_p);
      }
      frames_e.free();
    }
    end_table(stream);
  }
// End tabulate-kb-frames-filtered


// Start preallocate-links-for-frames
  static void preallocate_links_for_frames 
    (Node frames, Kb kb, TruthValue just_frames_in_this_kb_p,
     Hashtable link_table)
    {
      Enumerator frames_e = frames.enumerate_list();
      frames_e.prefetch();
      while (frames_e.has_more_p()) {
	Node frame = frames_e.next();
	maybe_allocate_link_name(frame, kb, just_frames_in_this_kb_p,
				 link_table, false);
      }
      frames_e.free();
    }
// End preallocate-links-for-frames


// Start get-filtered-frames
  static Node get_filtered_frames (Kb kb, InferenceLevel inference_level,
				   TruthValue just_frames_in_this_kb_p)
    {
      Enumerator frames_e = kb.enumerate_kb_frames(just_frames_in_this_kb_p);
      Node result = Node._NIL;
      frames_e.prefetch();
      while (frames_e.has_more_p()) {
	Node frame = frames_e.next();
	Enumerator slots_e = kb.enumerate_frame_slots
	  (frame, inference_level, Node._own);
	boolean facet_found_p = false;
	slots_e.prefetch();
	while (slots_e.has_more_p() && !facet_found_p) {
	  if (kb.get_slot_facets(frame, slots_e.next(), inference_level,
				 Node._own).firstValue() != Node._NIL)
	    { facet_found_p = true; }
	}
	slots_e.free();
	if (facet_found_p) { result = new Cons(frame, result); }
	else 
	  {
	    slots_e = kb.enumerate_frame_slots
	      (frame, inference_level, Node._template);
	    slots_e.prefetch();
	    while (slots_e.has_more_p() && !facet_found_p) {
	      if (kb.get_slot_facets(frame, slots_e.next(), inference_level,
				     Node._template).firstValue() != Node._NIL)
		{ facet_found_p = true; }
	    }
	    slots_e.free();
	    if (facet_found_p) { result = new Cons(frame, result); }
	  }
      }
      frames_e.free();
      return result;
    }
// End get-filtered-frames


//=========================================================================
// Code to summarize a filtered subset of the frames in a KB by running
// the filter on the server side.

// Start tabulate-kb-frames-filtered-on-server
  static void tabulate_kb_frames_filtered_on_server
    (Writer stream, Kb kb, InferenceLevel inference_level,
     TruthValue just_frames_in_this_kb_p, String link_tag,
     boolean frame_must_be_there_to_link_p)
       throws IOException
  {
    start_table(stream);
    {
      print_table_column_headers(stream);
      Node frames = get_filtered_frames_on_server(kb, inference_level,
						  just_frames_in_this_kb_p);
      Hashtable link_table = new Hashtable();
      preallocate_links_for_frames
	(frames, kb, just_frames_in_this_kb_p, link_table);
      Enumerator frames_e = frames.enumerate_list();
      frames_e.prefetch();
      while (frames_e.has_more_p()) {
	Node frame = frames_e.next();
	tabulate_frame_details(stream, frame, kb, inference_level,
			       just_frames_in_this_kb_p, link_table, link_tag,
			       frame_must_be_there_to_link_p);
      }
      frames_e.free();
    }
    end_table(stream);
  }
// End tabulate-kb-frames-filtered-on-server


// Start get-filtered-frames-on-server
  static Node get_filtered_frames_on_server
     (Kb kb, InferenceLevel inference_level,
      TruthValue just_frames_in_this_kb_p)
    {
      Node procedure_name = Symbol.intern("GET-FILTERED-FRAMES", "DEMOS");
      Procedure procedure =
	(Procedure)kb.create_procedure
	 (new OKBCString("(kb inference-level just-frames-in-this-kb-p)"),
	  new OKBCString(
"          (let ((frames-e (enumerate-kb-frames :kb kb :kb-local-only-p \n" +
"                                               just-frames-in-this-kb-p)) \n"+
"                (result nil)) \n" +
"            (prefetch frames-e) \n" +
"            (while (has-more frames-e) \n" +
"              (let ((frame (next frames-e))) \n" +
"                (let ((slots-e (enumerate-frame-slots \n" +
"                                frame :kb kb \n" +
"                                :inference-level inference-level \n" +
"                                :slot-type :own)) \n" +
"                      (facet-found-p nil)) \n" +
"                  (prefetch slots-e) \n" +
"                  (while (and (has-more slots-e) (not facet-found-p)) \n" +
"                    (if (get-slot-facets \n" +
"                         frame (next slots-e) :kb kb \n" +
"                         :inference-level inference-level \n" +
"                         :slot-type :own) \n" +
"                        (setq facet-found-p t) \n" +
"                        nil)) \n" +
"                  (free slots-e) \n" +
"                  (if facet-found-p \n" +
"                      (push frame result) \n" +
"                      (let ((slots-e (enumerate-frame-slots \n" +
"                                      frame :kb kb \n" +
"                                      :inference-level inference-level \n" +
"                                      :slot-type :template)) \n" +
"                            (facet-found-p nil)) \n" +
"                        (prefetch slots-e) \n" +
"                        (while (and (has-more slots-e) \n" +
"                                    (not facet-found-p)) \n" +
"                          (if (get-slot-facets \n" +
"                               frame (next slots-e) :kb kb \n" +
"                               :inference-level inference-level \n" +
"                               :slot-type :template) \n" +
"                              (setq facet-found-p t) \n" +
"                              nil)) \n" +
"                        (free slots-e) \n" +
"                        (if facet-found-p \n" +
"                            (push frame result) \n" +
"                            nil)))))) \n" +
"            (free frames-e) \n" +
"            result)"));
      kb.register_procedure(procedure_name, procedure);
      return kb.call_procedure(procedure_name,
			       Cons.list(kb, inference_level,
					 just_frames_in_this_kb_p));
    }
// End get-filtered-frames-on-server


// ***************************************************************************
// Simple support for TTY-based menus and prompting from the user.

  static char safe_read_char () {
    char ch;
    try { ch = (char)System.in.read(); }
    catch (IOException e) { throw new RuntimeException(e.toString()); }
    return ch;
  }

  static String read_line () {
    String result = "";
    String lineSeparator
      = System.getProperties().getProperty("line.separator");
    char input_char = safe_read_char();
    while (input_char != lineSeparator.charAt(0))
      { result = result + input_char;
	input_char = safe_read_char();
      }
    // skip any remaining line separator characters.
    for (int i = 1; i < lineSeparator.length() ; i++) {
      safe_read_char();
    }
    return result;
  }

// Start menu-choose
  static Node menu_choose(String title, Node alist)
  {
    return menu_choose(title, alist, alist.car().car());
  }

  static Node menu_choose(String title, Node alist, Node deflt)
  {
    Node result = Node._NIL;
    while (true) {
      System.out.println();
      System.out.print(title);
      int i = 0;
      Enumerator e = alist.enumerate_list();
      while (e.has_more_p()) {
	Node spec = e.next();
	Node printed_representation = spec.cadr();
	System.out.println();
	System.out.print("  " + i + " " + printed_representation);
	i = i + 1;
      }
      System.out.println();
      System.out.print("Choose [default " + Cons.assoc(deflt, alist).cadr()
		       + "]: ");
      String line = read_line();
      if (line.equals("")) { result = deflt; break; }
      else
	{
	  Integer choice = null;
	  try { choice = new Integer(line); }
	  catch (Exception ex) {}
	  if (choice != null
	      && choice.intValue() >= 0
	      && choice.intValue() < alist.length())
	    { result = alist.nth(choice).car(); break; }
	  else
	    { System.out.println();
	      System.out.println("!! Selection should be in the range [0..." +
				 (alist.length() - 1) + "]");
	    }
	}
    }
    return result;
  }
// End menu-choose


// Start prompt-and-read-string
  static OKBCString prompt_and_read_string(String label)
  {
    return prompt_and_read_string(label, null);
  }

  
  static OKBCString prompt_and_read_string(String label, String deflt)
  {
    String result = null;
    while (true) {
      System.out.println();
      System.out.print(label);
      if (deflt != null)
	{ System.out.print(" [default " + deflt + "]"); }
      System.out.print(": ");
      result = read_line();
      if (result.equals(""))
	{
	  if (deflt != null) { result = deflt; break; }
	  else { System.out.println("!! Please enter a string."); }
	}
      else { break; }
    }
    return new OKBCString(result);
  }
// End prompt-and-read-string


// Start prompt-and-read-integer
  static OKBCInteger prompt_and_read_integer(String label)
  {
    Integer result = null;
    while (true) {
      System.out.println();
      System.out.print(label + ": ");
      String line = read_line();
      result = null;
      try { result = new Integer(line); }
      catch (Exception e) {}
      if (result != null) { break; }
      else { System.out.println("!! Please enter an integer."); }
    }
    return new OKBCInteger(result);
  }
// End prompt-and-read-integer


// Start y-or-n-p
  static boolean y_or_n_p(String label)
  {
    while (true) {
      String result = prompt_and_read_string(label + "(y/n) ").string;
      if (result.equalsIgnoreCase("Y")) { return true; }
      if (result.equalsIgnoreCase("N")) { return false; }
    }
  }
// End y-or-n-p
}

/***** Text to insert in the paper in the right place.

// Start enumerator-example
  Enumerator e = kb.enumerate_slot_values(frame, slot);
  e.prefetch();
  while (e.has_more_p()) {
    Node value = e.next();
    ..... // do something with the slot value.
  }
  e.free();
// End enumerator-example

// Start default-selection
  if (value instanceof Cons && value.car() == Node._default)
    { default_values = 
      new Cons(value.cadr(), default_values);
    }
  else
    {
      print_value_maybe_with_link
        (stream, value, kb, just_frames_in_this_kb_p, link_table, link_tag,
         frame_must_be_there_to_link_p);
      firstp = false;
    }
// End default-selection

*******/
