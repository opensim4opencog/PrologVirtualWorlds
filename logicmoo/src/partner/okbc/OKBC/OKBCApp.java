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
import java.applet.*;

public class OKBCApp extends SessionInfo {

  public String groupid = "JUST-ME";
  public String session_description = "Java Client Session";
  public int session_duration = 8;
  public Applet parentApplet;
  public NetworkConnection the_connection = null;

  public OKBCApp() {}

  public OKBCApp(Applet theApplet) {
    parentApplet = theApplet ;
  }

  public String getUser_Id () { return user_id; }
  public void setUser_Id (String id) { user_id = id; }

  public String getPassword () { return password; }
  public void setPassword (String pwd) { password = pwd; }

  public String getSession_id () { return session_id; }
  public void setSession_id (String id) { session_id = id; }

  public String getKey () { return session_key; }
  public void setKey (String key) { session_key = key; }

  public int getSession_duration () { return session_duration; }
  public void setSession_duration (int dur) { session_duration = dur; }

  public String getSession_description () { return session_description; }
  public void setSession_description (String desc)
    { session_description = desc; }

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

  public boolean has_persistent_connection() {
    return connection().has_persistent_connection();
  }

  public void set_connection_persistence (boolean new_state) {
    connection().set_connection_persistence(new_state);
  }

  public void establish_ksl_connection () {
    set_connection 
      (KslNetworkConnection.establish_NetworkConnection_connection
       (host, port, _ephemeral, user_id, password,
	session_id, session_key, kb_library));
  }

  public void establish_simple_connection () {
    set_connection 
      (SimpleNetworkConnection.establish_NetworkConnection_connection
       (host, port, kb_library));
  }

  public void connect_to_a_session() {
    SessionDescriptor[] current_sessions;
      if ( "".equals(password))
      { user_id = "ALIEN";
	session_id = "ANONYMOUS";
	session_key = "";
      }
    else 
      { current_sessions = this.active_sessions();
	if ( current_sessions.length == 0)
	  { this.login_user_and_create_session(); }
	else 
	  { // this should really pop up a menu.
	    this.login_user_and_connect_to_session(current_sessions[0]);
	  }
      }
  }

  public void initialize_from_applet_parameters() {
    // System.out.println("Host is " + parentApplet.getCodeBase().getHost());
    // System.out.println("Port is " + parentApplet.getCodeBase().getPort());
        String hst = parentApplet.getParameter("okbchost");
	if (hst != null && !hst.equals("")) 
	  { host = hst; }
        hst = parentApplet.getParameter("host");
	if (hst != null && !hst.equals("")) 
	  { host = hst; }
        else { hst = parentApplet.getCodeBase().getHost();
	       if (hst != null && !hst.equals(""))
		 { host = hst; };
	     }
	String portString = parentApplet.getParameter("port");
	if (portString != null && !portString.equals("")) 
	  {
	    port = Integer.valueOf(portString).intValue(); 
	  }
	else { port = parentApplet.getCodeBase().getPort(); }

	String usrid = parentApplet.getParameter("user_id");
	if (usrid != null && !usrid.equals("")) 
	  { user_id = usrid; };
	String pwd = parentApplet.getParameter("password");
	if (pwd != null && !pwd.equals("")) { password = pwd; };

	String sess_id = parentApplet.getParameter("session_id");
	if (sess_id != null && !sess_id.equals(""))
	  { session_id = sess_id; };
	String key = parentApplet.getParameter("key");
	if (key != null && !key.equals("")) { session_key = key; };


	String groupidString = parentApplet.getParameter("groupid");
	if (groupidString != null && !groupidString.equals(""))
	  { groupid = groupidString; };
	String durationString = parentApplet.getParameter("duration");
	if (durationString != null && !durationString.equals("")) {
	  session_duration = Integer.valueOf(durationString).intValue(); 
	};
	String sessiond = parentApplet.getParameter("session_description");
	if (sessiond != null && !sessiond.equals(""))
	  { session_description = sessiond; };

    }

  public void test_for_correct_initialization() {
	 if (host.equals(""))
	   throw new RuntimeException ("No host specified");

	 if (port == 0)
	   throw new RuntimeException ("No port specified");
       }

  public static String argv_get(Symbol key, String argv[]) {
    return argv_get(key.symbolName, argv);
  }

  public static String argv_get(String key, String argv[]) {
    String real_key = "-" + key;
    for(int i = 0; i < argv.length; i++) {
      if (real_key.equalsIgnoreCase(argv[i]))
	{ return argv[i+1]; }
    }
    return null;
  }

  public static boolean argv_suppliedp(String key, String argv[]) {
    String real_key = "-" + key;
    for(int i = 0; i < argv.length; i++) {
      if (real_key.equalsIgnoreCase(argv[i]))
	{ return true; }
    }
    return false;
  }

  public static boolean argv_no_suppliedp(String key, String argv[]) {
    String res = argv_get(key, argv);
    if (res == null)
      { return argv_suppliedp("no" + key, argv); }
    else { return res.equalsIgnoreCase("NO"); }
  }

  public static boolean argv_yes_suppliedp(String key, String argv[]) {
    String res = argv_get(key, argv);
    if (res == null)
      { return argv_suppliedp(key, argv) ||
	       argv_suppliedp("yes" + key, argv);
      }
    else { return res.equalsIgnoreCase("YES"); }
  }

  public void initialize_from_argv(String argv[]) {
	 for(int i = 0; i < argv.length; i++) {
	   if ("-okbchost".equals(argv[i]))
	     host = argv[i+1];
	   else if ("-host".equals(argv[i]))
	     host = argv[i+1];
	   else if ("-port".equals(argv[i]))
	     port = Integer.parseInt(argv[i+1]);

	   else if ("-user_id".equals(argv[i]))
	     user_id = argv[i+1];
	   else if ("-password".equals(argv[i]))
	     password = argv[i+1];

	   else if ("-session_id".equals(argv[i]))
	     session_id = argv[i+1];
	   else if ("-session_key".equals(argv[i]))
	     session_key = argv[i+1];

	   else if ("-groupid".equals(argv[i]))
	     groupid = argv[i+1];
	   else if ("-session_description".equals(argv[i]))
	     session_description = argv[i+1];
	   else if ("-duration".equals(argv[i]))
	     session_duration = Integer.parseInt(argv[i+1]);
	 }
       }

  static char safe_read_char () {
    char ch;
    try { ch = (char)System.in.read(); }
    catch (IOException e) { throw new RuntimeException(e.toString()); }
    return ch;
  }

  public static OKBCString prompt_and_read_frame_name
        (String prompt, OKBCString frame) {
    String frame_temp = "";
    String lineSeparator
      = System.getProperties().getProperty("line.separator");
    System.out.print(prompt);
    if (frame != null)
      { System.out.print(" [default " + frame + "]: "); }
    char frame_char = safe_read_char();
    while (frame_char != lineSeparator.charAt(0))
      { frame_temp = frame_temp + frame_char;
	frame_char = safe_read_char();
      }
    // skip any remaining line separator characters.
    for (int i = 1; i < lineSeparator.length() ; i++) {
      safe_read_char();
    }
    if (frame_temp.equals("")) { frame_temp = frame.string; }
    return new OKBCString(frame_temp.toUpperCase());
  }

  public void echo_parameters () {
    System.out.println("Host = " + host);
    System.out.println("Port = " + port);
    System.out.println("User = " + user_id);
    System.out.println("Password = " + password);
    System.out.println("Session ID = " + session_id);
    System.out.println("Session Key = " + session_key);
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
  public void login_user_and_create_session () {
    SessionResponse resp = NetworkKb.login_user_and_create_session
      (host, port, user_id, password,
       groupid, session_description, session_duration);
    session_key = resp.session_key;
    session_id = resp.session_id;
  }

  public void login_user_and_create_session (String description,
					     int duration,
					     String group) {
    groupid = group;
    session_description = description;
    session_duration = duration;
    SessionResponse resp = NetworkKb.login_user_and_create_session
      (host, port, user_id, password,
       groupid, session_description, session_duration);
    session_key = resp.session_key;
    session_id = resp.session_id;
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
  public void login_user_and_connect_to_session (String the_session_id) {
    SessionResponse resp = 
      NetworkKb.login_user_and_connect_to_session
      (host, port, user_id, password, the_session_id);
    session_key = resp.session_key;
    session_id = resp.session_id;
  }

  public void login_user_and_connect_to_session
      (SessionDescriptor session) {
	 login_user_and_connect_to_session(session.session_id);
  }




/**
 * Returns a list of lists (session-id group description-string) for each
 *  of the sessions to which you can connect
 */
  public SessionDescriptor[] active_sessions() {
    return NetworkKb.active_sessions
             (host, port, user_id, password, session_key);
  }




/**
 * Returns a list of the group IDs applicable to the current user.
 */
  public String[] get_groups() {
    return NetworkKb.get_groups(host, port, user_id, password,
                                           session_key);
  }



}