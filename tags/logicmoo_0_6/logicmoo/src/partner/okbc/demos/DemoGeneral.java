/***********************************************************************

  OKBC demos for Java

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

package demos;
import java.util.*;
import java.io.*;
import OKBC.*;

class DemoGeneral {

  static final Node ksl_connection_type
    = Symbol.intern("KSL-NETWORK-CONNECTION", "KSL-OKBC");

  static Connection get_ksl_connection (OKBCString host, OKBCInteger port,
					OKBCString user_id,
					OKBCString password) {
    if (user_id.string.equalsIgnoreCase("Alien"))
      { password = new OKBCString(""); }
    Node _kill = Symbol.keyword("KILL");
    Node session = null;
    while (true) {
      SessionDescriptor[] sessions =
	NetworkKb.active_sessions
	(host.string, (int)(port.number), user_id.string,
	 password.string, "");
      if (user_id.string.equalsIgnoreCase("Alien"))
	{ session = sessions[0]; }
      else if (sessions.length == 0) { session = Node._NIL;  break; }
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
	  for (int i=0; i < sessions.length; i++) {
	    SessionDescriptor s = sessions[i];
	    menu_items = new Cons(Cons.list
				  (Cons.list(_kill, s),
				   new OKBCString("Kill session " +
						  s.session_description +
						  " owned by " +
						  s.group_ownership)),
				  menu_items);
	  }
	  menu_items = new Cons(Cons.list(Node._NIL,
					  new OKBCString
					  ("Create a new session")),
				menu_items);
	  session = KbSummary.menu_choose("Select a session to open:",
					  menu_items);
	}
      if (session instanceof Cons &&
	  session.car() == _kill)
	{
	  NetworkKb.kill_session
	    (host.string, (int)(port.number), user_id.string,
	     password.string, ((SessionDescriptor)(session.cadr())).session_id,
	     "");
	}
      else { break; }
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
	  KbSummary.menu_choose("Select a group to own a new session.",
				menu_items);
	OKBCString description =
	  KbSummary.prompt_and_read_string("Session description");
	OKBCInteger duration =
	  KbSummary.prompt_and_read_integer("Session duration in hours");
	resp = NetworkKb.login_user_and_create_session
	  (host.string, (int)(port.number), user_id.string,
	   password.string, group.string, description.string,
	   (int)(duration.number));
      }
    Connection connection =
      Node.establish_connection(ksl_connection_type,
				Cons.list(KbSummary._host, host,
					  KbSummary._port, port,
					  KbSummary._user_id, user_id,
					  KbSummary._password, password,
					  KbSummary._session_id,
					  new OKBCString(resp.session_id),
					  KbSummary._key, new OKBCString
					  (resp.session_key),
					  KbSummary._kb_library, KbSummary._a));
    connection.set_connection_persistence(true);
    return connection;
  }

}
