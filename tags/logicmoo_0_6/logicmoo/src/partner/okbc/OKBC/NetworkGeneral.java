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

public class NetworkGeneral extends Connection {

  static String request_cookie = "REQUEST!";
  static final String query_format = "PORTABLE";
  static final String reply_format = "PORTABLE";
  static final String max_supported_transport_version = "C";
  static final String min_supported_transport_version = "B";

  static Node decodeList(PushableBufferedReader s, Kb kb_type, int depth, 
			 boolean client_p, Connection connection) {
	 Cons root = null;
         Cons tail = null;
	 int length = 0;
	 try { length = Integer.parseInt(s.readLine()); }
	 catch (IOException e) { throw new RuntimeException(e.toString()); }
	 for(int i = 0; i < length; i++) {
	   Node elt = decode(s, kb_type, depth + 1, client_p, connection);
	   if (tail == null)
	     { tail = new Cons(elt);
	       root = tail;
	     }
	   else { Cons new_tail = new Cons(elt);
		  tail.set_cdr(new_tail);
		  tail = new_tail;
		}
	 }
	 // System.out.println("Decoded: " + root);
	 if (root == null) { return _NIL; }
	 else { return root; }
       }
  
  static Node decodeSymbol(PushableBufferedReader s, boolean client_p,
			   Connection connection) {
         String packageName;
	 String symbolName;
	 try {
	   s.readLine();		// skip length
	   packageName = s.readLine();
	   s.readLine();		// skip length
	   symbolName = s.readLine();
	 }
	 catch (IOException e) { throw new RuntimeException(e.toString()); }
	 // System.out.println("Decoded: " + n);
	 if (packageName.equalsIgnoreCase("NIL"))
	   { Symbol uninterned = new Symbol();
	     uninterned.symbolName = symbolName;
	     return uninterned;
	   }
	 // Try to intern as a quasi symbol.  This will do the right thing
	 // if there's already a real package.
	 else { Node sym = QuasiSymbol.intern(symbolName, packageName);
		if (sym == _establish_local_connection)
		  { return LocalConnection.local_connection(); }
		else { return sym; }
	      }
       }

  static char[] readChars(PushableBufferedReader stream, int n, boolean eatEol){
	char[] buf = new char[n];
	try 
	  {
	    for(int i = 0; i < n; i++) {
	      int c = stream.read();
	      if ( c == -1 ) 
		throw new IOException("Not enough chars to read");
	      buf[i] = (char) c;
	    }
	    // if eatEol, make sure that we get have a blank line remaining.
	    if (eatEol)
	      if (stream.readLine().length() > 0)
	      throw new IOException("Expected end of line");
	  }
	catch (IOException e) { throw new RuntimeException(e.toString()); }
	return buf ;
      }

  static OKBCString decodeOKBCString(PushableBufferedReader s,
				     boolean client_p, 
				     Connection connection) {
	 OKBCString n = new OKBCString();
	 int length = 0;
	 try { length = Integer.parseInt(s.readLine()); }
	 catch (IOException e) { throw new RuntimeException(e.toString()); }
	 n.string = String.valueOf(readChars(s,length,true));
	 // System.out.println("Decoded: " + n);
	 return n;
       }

  static OKBCInteger decodeOKBCInteger(PushableBufferedReader s,
				       boolean client_p, 
				       Connection connection) {
	 OKBCInteger n = new OKBCInteger();
	 try { s.readLine(); // discard the length line
	       n.number = Long.parseLong(s.readLine()); 
	     }
	 catch (IOException e) { throw new RuntimeException(e.toString()); }
	 // System.out.println("Decoded: " + n);
	 return n;
       }

  static Real decodeReal(PushableBufferedReader s, boolean client_p, 
			 Connection connection) {
	 Real n = new Real();
	 try
	   {
	     s.readLine(); // discard the length line
	     n.number = Double.valueOf(s.readLine()).doubleValue();
	   }
	 catch (IOException e) { throw new RuntimeException(e.toString()); }
	 // System.out.println("Decoded: " + n);
	 return n;
       }

  static True decodeTrue(PushableBufferedReader s, boolean client_p, 
			 Connection connection) {
	 True n = _T;
	 // System.out.println("Decoded: " + n);
	 return n;
       }

  static False decodeFalse(PushableBufferedReader s, boolean client_p, 
			   Connection connection) {
	 False n = _NIL;
	 // System.out.println("Decoded: " + n);
	 return n;
       }

  static Other decodeOther(PushableBufferedReader s, boolean client_p, 
			   Connection connection) {
	 Other n = new Other();
	 int length = 0;
	 try { length = Integer.parseInt(s.readLine()); }
	 catch (IOException e) { throw new RuntimeException(e.toString()); }
	 n.contents = String.valueOf(readChars(s,length,true));
	 // System.out.println("Decoded: " + n);
	 return n;
       }

  public static MinimalNetworkKb prototype_of_network_kbs =
      (NetworkKb)NetworkKb.get_kb_type_internal();

  static Kb decodeKb_given_name_and_type
       (AbstractSymbol nam, AbstractSymbol typ, boolean client_p,
	Connection connection) {
    if (client_p)
      {
	if (nam == _kb_type)
	  { return (NetworkKb)(NetworkKb.get_kb_type(typ)); }
	{ 
	  MinimalNetworkKb n = null;
	  Object o = local_find_kb_of_type
	    (nam, prototype_of_network_kbs, typ);
	  if (o instanceof MinimalNetworkKb)
	    { n = (MinimalNetworkKb)o; }
	  else { Exception e = null;
		 Class kb_class = prototype_of_network_kbs.getClass();
//		 System.out.println("nam = " + nam);
//		 System.out.println("typ = " + typ);
		 // System.out.println("Starting decode: " + System.currentTimeMillis());
		 try { n = (MinimalNetworkKb)(kb_class.newInstance()); }
		 catch (Exception exception) { e = exception; }
		 // System.out.println("n = " + n);
		 if (n == null) 
		   { throw new RuntimeException
		     (Node.error_message_for_illegal_access
		      (kb_class, prototype_of_network_kbs, e));
		   }
		 // Initialize these slots so that we're ready for the add_kb.
		 // System.out.println("Finished decode: " + System.currentTimeMillis());
		 n.setName(nam);
		 n.kb_type_name = typ;
		 n.set_connection(connection);
		 add_kb(n);
		 n.setName(nam);
		 n.kb_type_name = typ;
		 // System.out.println("Finished add: " + System.currentTimeMillis());
	       }
	  n.set_connection(connection);
	  // System.out.println("Decoded: " + n + " for " + nam + ", " + typ);
	  return n;
	}
      }
    else 
      { Kb concrete_instance_kb_type = 
	   (client_p ? null : concrete_kb_from_abstract_kb(typ));
	if (concrete_instance_kb_type != null)
	  { if (nam == _kb_type)
	      { return concrete_instance_kb_type; }
	    else 
	      { Node kb = find_kb_of_type
		   (nam, concrete_instance_kb_type,
		    LocalConnection.local_connection());
		if (kb == _NIL)
		  { System.out.println("Tried to get non-existent KB " +
				       nam);
		    return null;
		  }
		else { return (Kb)kb; }
	      }
	  }
	else { throw new GenericError
	       (_T, new OKBCString
		(typ + " is not the name of a defined KB class.  " +
		 "The known KB class bindings are: " + 
		 registered_concrete_kbs)).signal();
	     }
      }
  }


  static Kb decodeKb(PushableBufferedReader s, Kb kb_type, int depth, 
		     boolean client_p, Connection connection) {
    AbstractSymbol nam = (AbstractSymbol)
      decode(s, kb_type, depth + 1, client_p, connection);
    AbstractSymbol typ = (AbstractSymbol)
      decode(s, kb_type, depth + 1, client_p, connection);
    return decodeKb_given_name_and_type(nam, typ, client_p, connection);
  }

  static Procedure decodeProcedure
      (PushableBufferedReader s, Kb kb_type, int depth, boolean client_p, 
       Connection connection) {
    Node name = decode(s, kb_type, depth + 1, client_p, connection);
    name = (name instanceof False ? (Node)Node._NIL : name);
    Node arguments   = decode(s, kb_type, depth + 1, client_p, connection);
    Node expression  = decode(s, kb_type, depth + 1, client_p, connection);
    Node environment = decode(s, kb_type, depth + 1, client_p, connection);
    Procedure n = Procedure.intern_procedure
      (name, arguments, expression, environment);
    // System.out.println("Decoded: " + n);
    return n;
  }

  static Node decodeFrameHandle(PushableBufferedReader s, Kb kb_type, int depth,
				boolean client_p, Connection connection) {
	 return FrameHandle.internThing
	   (decode(s, kb_type, depth + 1, client_p, connection),
	    decode(s, kb_type, depth + 1, client_p, connection),
	    client_p);
       }

  static Node decodeRemoteValue(PushableBufferedReader s, Kb kb_type, int depth,
				boolean client_p, Connection connection) {
    
    Node decoded = RemoteValue.internThing
      (decode(s, kb_type, depth + 1, client_p, connection),
       decode(s, kb_type, depth + 1, client_p, connection));
    if (!client_p && decoded instanceof RemoteValue)
      { System.out.println("Unsatisfied RemoteValue " + decoded +
			   " passed to server!!  Mapping it to NIL");
	// Just give a warning here becase to err might desynchronise the
	// transport layer.  Maybe we should send this back to the client
	// some how.
	return Node._NIL;
      }
    else { return decoded; }
  }

  static Node decodeEnumerator(PushableBufferedReader s, Kb kb_type, int depth,
			       boolean client_p, Connection connection) {
	 return Enumerator.internThing
	   (decode(s, kb_type, depth + 1, client_p, connection), connection);
       }

  static public Node decode(PushableBufferedReader s, Kb kb_type, 
			    boolean client_p, Connection connection) {
	 return decode(s, kb_type, 0, client_p, connection);
       }

  static public Node decode(PushableBufferedReader s, Kb kb_type, int depth, 
			    boolean client_p, Connection connection) {
         String line = null;
         try { line = s.readLine(); }
         catch (IOException e) { throw new RuntimeException(e.toString()); }

	 // System.out.println("Decode line = " + line);
	 if (line == null)
	   { throw new RuntimeException("No decode line found where one " +
                                        "expected at depth = " + depth);
	   }
	 int code = 0;

	// NMJ 02/03/98
	 /**
	  * If we get an Exception when trying to read result, we may have an
	  * embedded "REQUEST!" tag, indicating an embedded error packet.
	  * Push the recently read line back, and pass on the Exception
	  */
	 // Changed by Rice also to handle the issue of wrong connection
	 // protocol type.
	 try
	   {
	     code = Integer.parseInt(line);
	   }
	 catch (NumberFormatException e)
	   {
	     s.push(line);
	     if (line.startsWith("GET /") || line.startsWith("get /"))
	       {
		 throw new RuntimeException
		   ("PROTOCOL ERROR.  You are probably trying to connect to " +
		    "this server using the wrong type of connection " +
		    "(KSL-Network-Connection?).  What should have been an " +
		    "integer type code was really: \"" + line + "\"");
	       }
	     else { throw e; }
	   };

	 //System.out.println("Decode code = " + code);
	 Node result = null;
	 if ( code == LIST)
	   { result = decodeList(s, kb_type, depth + 1, client_p, 
				 connection); }
	 else if ( code == SYMBOL)
	   { result = decodeSymbol(s, client_p, connection); }
	 else if ( code == STRING)
	   { result = decodeOKBCString(s, client_p, connection); }
	 else if ( code == INTEGER)
	   { result = decodeOKBCInteger(s, client_p, connection); }
	 else if ( code == REAL)
	   { result = decodeReal(s, client_p, connection); }
	 else if ( code == TRUE)
	   { result = decodeTrue(s, client_p, connection); }
	 else if ( code == FALSE)
	   { result = decodeFalse(s, client_p, connection); }
	 else if ( code == OTHER)
	   { result = decodeOther(s, client_p, connection); }
	 else if ( code == Kb)
	   { result = decodeKb(s, kb_type, depth + 1, client_p, connection); }
	 else if ( code == PROCEDURE)
	   { result = decodeProcedure(s, kb_type, depth + 1, client_p, 
				      connection); }
	 else if ( code == FRAMEHANDLE)
	   { result = decodeFrameHandle(s, kb_type, depth + 1, client_p, 
					connection); }
	 else if ( code == REMOTEVALUE)
	   { result = decodeRemoteValue(s, kb_type, depth + 1, client_p, 
					connection); }
	 else if ( code == ENUMERATOR)
	   { result = decodeEnumerator(s, kb_type, depth + 1, client_p, 
				       connection); }
	 else 
	   { throw new RuntimeException("Illegal Network OKBC code: " +
					String.valueOf(code));
	   }
	 // System.out.println("Decoded [" + depth + "] = " + result);
	 return result;
       }

}
