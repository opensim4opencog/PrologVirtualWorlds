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

public class EncodeDecoder {

  public static Node decode (byte[] bytes, Kb kb_type, boolean client_p,
			     Connection connection) {
    return decode(bytes, kb_type, client_p, connection,
		  kb_type.ioSyntax.io_package);
  }

  public static Node decode (InputStream s, Kb kb_type, boolean client_p,
			     Connection connection) {
    Node res = decode(s, kb_type, client_p, connection,
		      kb_type.ioSyntax.io_package);
    // System.out.println("Decoded " + res);
    return res;
  }

  public static Node decode (byte[] bytes, Kb kb_type, boolean client_p,
			     Connection connection, AbstractPackage pack) {
    ByteArrayInputStream s = new ByteArrayInputStream(bytes);
    return decode(s, kb_type, client_p, connection, pack);
  }

  public static Node decode (InputStream s, Kb kb_type, boolean client_p,
			     Connection connection, AbstractPackage pack) {
    return decode(s, kb_type, client_p, connection, pack, null);
  }

  public static Node decode (InputStream s, Kb kb_type, boolean client_p,
			     Connection connection, AbstractPackage pack,
			     Node[] table) {
    Node result = null;
    try { result = decode_internal
	  (s, kb_type, client_p, connection, pack, table);
	}
    catch (IOException e) { throw new RuntimeException(e.toString()); }
    return result;
  }

  public static Node decode_using_table
    (byte[] bytes, Kb kb_type, boolean client_p, Connection connection) {
    return decode_using_table
      (bytes, kb_type, client_p, connection, kb_type.ioSyntax.io_package);
  }

  public static Node decode_using_table
    (InputStream s, Kb kb_type, boolean client_p, Connection connection) {
    Node res = decode_using_table
      (s, kb_type, client_p, connection, kb_type.ioSyntax.io_package);
    // System.out.println("Decoded " + res);
    return res;
  }

  public static Node decode_using_table
     (byte[] bytes, Kb kb_type, boolean client_p, Connection connection,
      AbstractPackage pack) {
    ByteArrayInputStream s = new ByteArrayInputStream(bytes);
    return decode_using_table(s, kb_type, client_p, connection, pack);
  }

  public static Node decode_using_table
      (InputStream s, Kb kb_type, boolean client_p,
       Connection connection, AbstractPackage pack) {
    Node result = null;
    try
      {
	Node thing = decode_internal
	  (s, kb_type, client_p, connection, pack, null);
	if (thing instanceof NodeVectorNode)
	  { Node[] nodes = ((NodeVectorNode)thing).nodes;
//	    System.out.println("Nodes = " + nodes);
//	    for (int i = 0; i < nodes.length ; i++) {
//	      System.out.println("Node[" + i + "] = " + nodes[i]);
//	    }
	    result = decode_internal
	      (s, kb_type, client_p, connection, pack, nodes);
	  }
	else result = thing;
      }
    catch (IOException e) { throw new RuntimeException(e.toString()); }
    return result;
  }

  public static byte[] encode (Node n, boolean client_p,
			       AbstractPackage pack) {
    ByteArrayOutputStream s = new ByteArrayOutputStream();
    try { encode_internal(n, s, client_p, pack, null); }
    catch (IOException e) { throw new RuntimeException(e.toString()); }
    return s.toByteArray();
  }

  public static void encode (OutputStream s, Node n, boolean client_p,
			     AbstractPackage pack) {
    encode(s, n, client_p, pack, null);
  }

  public static void encode (OutputStream s, Node n, boolean client_p,
			     AbstractPackage pack, Hashtable ht) {
    try { encode_internal(n, s, client_p, pack, ht); }
    catch (IOException e) { throw new RuntimeException(e.toString()); }
  }

  public static byte[] encode_node_uniquifying
      (Node n, boolean client_p, AbstractPackage pack) {
    ByteArrayOutputStream s = new ByteArrayOutputStream();
    encode_node_uniquifying(s, n, client_p, pack);
    return s.toByteArray();
  }

  public static void encode_node_uniquifying
      (OutputStream s, Node node, boolean client_p, AbstractPackage pack) {
    Hashtable ht = new Hashtable();
    int count = preallocate_unique_ids(node, ht);
    try { encode_hashtable(ht, s, client_p, pack);
	  encode_internal(node, s, client_p, pack, ht);
	}
    catch (IOException e) { throw new RuntimeException(e.toString()); }
  }

  static Node decode_internal(InputStream s, Kb kb_type, boolean client_p,
			      Connection connection, AbstractPackage pack,
			      Node[] table)
     throws IOException 
  {
    int tag_byte = s.read();
    int type_code = tag_byte & 0xf;
    Node result = null;
    if (type_code == Node.LIST) 
      { result = decode_list(tag_byte, s, kb_type, client_p, connection, pack,
			     table);
      }
    else if (type_code == Node.IN_HASH_TABLE) 
      { result = decode_existing
	(tag_byte, s, kb_type, client_p, connection, pack, table);
      }
    else if (type_code == Node.SYMBOL) 
      { result = decode_symbol
	(tag_byte, s, kb_type, client_p, connection, pack, table);
      }
    else if (type_code == Node.STRING) 
      { result = decode_string
	(tag_byte, s, kb_type, client_p, connection, table);
      }
    else if (type_code == Node.INTEGER) 
      { result = decode_integer
	(tag_byte, s, kb_type, client_p, connection, table);
      }
    else if (type_code == Node.REAL) 
      { result = decode_real
	(tag_byte, s, kb_type, client_p, connection, table);
      }
    else if (type_code == Node.TRUE) 
      { result = decode_true(tag_byte, s, kb_type, client_p, connection,table);
      }
    else if (type_code == Node.FALSE) 
      { result = decode_false
	(tag_byte, s, kb_type, client_p, connection, table);
      }
    else if (type_code == Node.Kb) 
      { result = decode_kb
	(tag_byte, s, kb_type, client_p, connection, pack, table);
      }
    else if (type_code == Node.FRAMEHANDLE)
      { result = decode_frame_handle
	  (tag_byte, s, kb_type, client_p, connection, pack, table);
      }
    else if (type_code == Node.PROCEDURE)
      { result = decode_procedure
	  (tag_byte, s, kb_type, client_p, connection, pack, table);
      }
    else if (type_code == Node.EXTENDED_TYPE)
      { result = decode_extended_type
	  (tag_byte, s, kb_type, client_p, connection, pack, table);
      }

    else { throw new RuntimeException("Unknown type code: " + type_code); }
    // System.out.println("Decoded: " + result);
    return result;
  }

  
  static Node decode_extended_type(int tag_byte, InputStream s, Kb kb_type,
				   boolean client_p, Connection connection,
				   AbstractPackage pack, Node[] table)
     throws IOException 
  {
    // System.out.println("tag_byte = " + tag_byte);
    int type_code = tag_byte & 0xff;
    // System.out.println("type-code = " + type_code);
    Node result = null;
         if (type_code == Node.ENDOFFILE) 
      { result = decode_EndOfFileNode(tag_byte, s, kb_type, client_p,
				      connection, table);
      }
    else if (type_code == Node.HASH_TABLE) 
      { result = decode_hashtable
	(tag_byte, s, kb_type, client_p, connection, pack);
      }
    else { throw new RuntimeException("Unknown type code: " + type_code); }
    // System.out.println("Decoded: " + result);
    return result;
  }

  
  static void encode_internal (Node n, OutputStream s, boolean client_p,
			       AbstractPackage pack, Hashtable ht)
    throws IOException
  {
    Object mapping = null;
    if (ht != null) { mapping = ht.get(n); }
    //
    if (mapping != null)
      { encode_existing(n, (Integer)mapping, s); }
    else if (n instanceof Cons)
      { encode_list((Cons) n, s, client_p, pack, ht); }
    else if (n instanceof AbstractSymbol)
      { encode_symbol((Symbol) n, s, client_p, pack, ht); }
    else if (n instanceof OKBCString)
      { encode_string((OKBCString) n, s, client_p, ht); }
    else if (n instanceof OKBCInteger)
      { encode_integer((OKBCInteger) n, s, client_p, ht); }
    else if (n instanceof Real)
      { encode_real((Real) n, s, client_p, ht); }
    else if (n instanceof True)
      { encode_true((True) n, s, client_p, ht); }
    else if (n instanceof False)
      { encode_false((False) n, s, client_p, ht); }
    else if (n instanceof Kb)
      { encode_kb((Kb) n, s, client_p, pack, ht); }
    else if (n instanceof FrameHandle)
      { encode_frame_handle((FrameHandle) n, s, client_p, pack, ht); }
    else if (n instanceof Procedure)
      { encode_procedure((Procedure) n, s, client_p, pack, ht); }
    else if (n instanceof QuasiSymbol)
      { encode_quasi_symbol((QuasiSymbol) n, s, client_p, pack, ht); }
    else if (n instanceof EndOfFileNode)
      { encode_EndOfFileNode((EndOfFileNode) n, s, client_p, ht); }
    else { throw new RuntimeException("Cannot encode node: " + n); }
  }

  static final int out_of_line_nibble_tag = 15;

  static int length_nibble (int length) {
    if (length < out_of_line_nibble_tag) { return length; }
    else { return out_of_line_nibble_tag; }
  }

  static void output_4_byte_int (int i, OutputStream s)  throws IOException {
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
  }

  static void output_2_byte_int (int i, OutputStream s)  throws IOException {
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
  }

  static void output_8_byte_int (long i, OutputStream s)  throws IOException {
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
    i = i >>> 8;
    s.write((int)(i & 0xff));
  }

  static int input_2_byte_int (InputStream s)  throws IOException {
    int result = 0;
    int b0 = s.read() <<  0;
    int b1 = s.read() <<  8;
    result = b0 ^ b1;
    return result;
  }

  static int input_4_byte_int (InputStream s)  throws IOException {
    int result = 0;
    int b0 = s.read() <<  0;
    int b1 = s.read() <<  8;
    int b2 = s.read() << 16;
    int b3 = s.read() << 24;
    result = b0 ^ b1 ^ b2 ^ b3;
    return result;
  }

  static long input_8_byte_int (InputStream s)  throws IOException {
    long result = 0;
    long b0 = ((long)s.read() <<  0);
    long b1 = ((long)s.read() <<  8);
    long b2 = ((long)s.read() << 16);
    long b3 = ((long)s.read() << 24);
    long b4 = ((long)s.read() << 32);
    long b5 = ((long)s.read() << 40);
    long b6 = ((long)s.read() << 48);
    long b7 = ((long)s.read() << 56);
    result = b0 ^ b1 ^ b2 ^ b3 ^ b4 ^ b5 ^ b6 ^ b7;
    // System.out.println("Long integer value = " + result);
    return result;
  }

  static void output_string
      (String string, OutputStream s)  throws IOException {
    int len = string.length();
    for (int i = 0; i < len; i++) {
      s.write(string.charAt(i));
    }
  }

  static void output_string_with_length
      (String string, OutputStream s)  throws IOException {
    int len = string.length();
    output_4_byte_int(len, s);
    output_string(string, s);
  }

  static char[] readChars(InputStream stream, int n) throws IOException {
    char[] buf = new char[n];
    for(int i = 0; i < n; i++) {
      int c = stream.read();
      if ( c == -1 ) 
	throw new IOException("Not enough chars to read");
      buf[i] = (char) c;
    }
    return buf ;
  }

  static String input_string (int len, InputStream s)  throws IOException {
    return String.valueOf(readChars(s, len));
  }

  static String input_string_with_length (InputStream s)  throws IOException {
    int len = input_4_byte_int(s);
    return input_string(len, s);
  }

  public static int preallocate_unique_ids (Node n, Hashtable ht) {
    return preallocate_unique_ids(n, ht, 0);
  }

  static int preallocate_unique_ids
       (Node n, Hashtable ht, int current_count) {
    if (n == Node._NIL)
      { return current_count; }
    else if (n instanceof Cons)
      { Node remainder = n;
	while (remainder instanceof Cons) {
	  current_count = preallocate_unique_ids
	    (remainder.car(), ht, current_count);
	  remainder = remainder.cdr();
	}
	if (remainder == null || remainder == Node._NIL)
	  { return current_count; }
	else { return preallocate_unique_ids(remainder, ht, current_count); }
      }
    else
      { Object existing = ht.get(n);
	if (existing == null)
	  { ht.put(n, new Integer(current_count));
	    return current_count + 1;
	  }
	else { return current_count; }
      }
  }

  
  // ------------ Existing in print circle table -----------
  static void encode_existing(Node n, Integer i, OutputStream s)
    throws IOException
  {
    s.write(Node.IN_HASH_TABLE);
    output_4_byte_int(i.intValue(), s);
    // System.out.println("Encode existing: " + n + " as " + i);
  }

  static Node decode_existing(int tag_byte, InputStream s, Kb kb_type,
			      boolean client_p, Connection connection,
			      AbstractPackage pack, Node[] table)
      throws IOException
  { int index = input_4_byte_int(s);
    return table[index];
  }


  // ------------ Hashtable for print circle table -----------
  static void encode_hashtable(Hashtable ht, OutputStream s, boolean client_p,
			       AbstractPackage pack)
    throws IOException
  {
    // System.out.println("Encoding ht: " + Node.HASH_TABLE);
    s.write(Node.HASH_TABLE);
    int length = ht.size();
    output_4_byte_int(length, s);
    Enumeration e = ht.keys();
    while (e.hasMoreElements()) {
      Node element = (Node)(e.nextElement()); 
      Integer i = (Integer)(ht.get(element));
      if (length > 32767)
	{ output_4_byte_int(i.intValue(), s); }
      else { output_2_byte_int(i.intValue(), s); }
      encode_internal(element, s, client_p, pack, null);
    }
    // System.out.println("Encoded ht");
  }

  static Node decode_hashtable(int tag_byte, InputStream s, Kb kb_type,
			       boolean client_p, Connection connection,
			       AbstractPackage pack)
      throws IOException
  { int length = input_4_byte_int(s);
    Node [] table = new Node[length];
    for (int i = 0; i < length ; i++) {
      int index;
      if (length > 32767)
	{ index = input_4_byte_int(s); }
      else { index = input_2_byte_int(s); }
      Node n = decode_internal(s, kb_type, client_p, connection, pack, null);
      table[index] = n;
      // System.out.println("table[" + index + "] = " + table[index]);
    }
    return new NodeVectorNode(table);
  }


  // ------------ Lists -----------
  static void encode_list(Cons n, OutputStream s, boolean client_p,
			  AbstractPackage pack, Hashtable ht)
    throws IOException
  {
    int len = n.length();
    int rh_nibble = Node.LIST;
    int lh_nibble = length_nibble(len);
    s.write((lh_nibble << 4) + rh_nibble);
    if (lh_nibble == out_of_line_nibble_tag) { output_4_byte_int(len, s); }
    Enumerator e = n.enumerate_list();
    while (e.has_more_p()) {
      Node element = e.next();
      encode_internal(element, s, client_p, pack, ht);
    }
  }

  static Node decode_list(int tag_byte, InputStream s, Kb kb_type,
			  boolean client_p, Connection connection,
			  AbstractPackage pack,
			  Node[] table)
      throws IOException
  { int len = 0;
    int lh_nibble = tag_byte >>> 4;
    if (lh_nibble == out_of_line_nibble_tag) { len = input_4_byte_int(s); }
    else { len = lh_nibble; }
    Cons root = null;
    Cons tail = null;
    for(int i = 0; i < len; i++) {
      Node elt = decode_internal(s, kb_type, client_p, connection, pack,
				 table);
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
    if (root == null) { return Node._NIL; }
    else { return root; }
  }


  // ------------ Symbols and QuasiSymbols -----------

  static void init_magic_alist_element
      (ConsLocation loc, String s, int num) {
    Node clause = Cons.list(new OKBCString(s), new OKBCInteger(num));
    loc.addElement(clause);
  }
  
  static Node init_magic_alist () {
    ConsLocation loc = new ConsLocation();
    init_magic_alist_element(loc, "OK-BACK",   1);
    init_magic_alist_element(loc, "KEYWORD",   2);
    init_magic_alist_element(loc, "LISP",      3);
    init_magic_alist_element(loc, "OKBC-USER", 4);
    init_magic_alist_element(loc, "OKBC",      5);
    return loc.cons;
  }

  static int get_magic_package_number (AbstractPackage p) {
    Node remainder = magic_coding_package_name_alist;
    int result = 0;
    if (p != null)
      {
	while (remainder != Node._NIL && remainder != null) {
	  Node entry = remainder.car();
	  if (p.name.equals(((OKBCString)(entry.car())).string))
	    { result = (int)(((OKBCInteger)entry.cadr()).number);
	      break;
	    }
	  else { remainder = remainder.cdr(); }
	}
      }
    return result;
  }
      
  static String get_magic_package_from_number (int n) {
    Node remainder = magic_coding_package_name_alist;
    String result = null;
    while (remainder != Node._NIL && remainder != null) {
      Node entry = remainder.car();
      if (n == (((OKBCInteger)(entry.cadr())).number))
	{ result = ((OKBCString)entry.car()).string;
	  break;
	}
      else { remainder = remainder.cdr(); }
    }
    return result;
  }
      
  static final Node magic_coding_package_name_alist = init_magic_alist();
  
  static void encode_symbol(Symbol n, OutputStream s, boolean client_p,
			    AbstractPackage pack, Hashtable ht)
    throws IOException
  {
    AbstractPackage p = n.symbolPackage;
    int rh_nibble = Node.SYMBOL;
    int lh_nibble = out_of_line_nibble_tag;
    if (p != null && p.name.equals(pack.name))
      { lh_nibble = 0; } // default package indicator
    else
      { int res = get_magic_package_number(p);
	if (res > 0) { lh_nibble = res; }
      }
    s.write((lh_nibble << 4) + rh_nibble);
    output_string_with_length(n.symbolName, s);
    if (lh_nibble == out_of_line_nibble_tag) 
      { output_string_with_length(p.name, s); }
  }

  static void encode_quasi_symbol(QuasiSymbol n, OutputStream s,
				  boolean client_p, AbstractPackage pack,
				  Hashtable ht)
    throws IOException
    // Encode QuasiSymbols as if they were symbols, but always go out of line
    // for AbstractPackage.
  {
    QuasiPackage p = n.symbolPackage;
    int rh_nibble = Node.SYMBOL;
    int lh_nibble = out_of_line_nibble_tag;
    s.write((lh_nibble << 4) + rh_nibble);
    output_string_with_length(n.symbolName, s);
    if (lh_nibble == out_of_line_nibble_tag) 
      { output_string_with_length(p.name, s); }
  }

  static Node decode_symbol(int tag_byte, InputStream s, Kb kb_type,
			    boolean client_p, Connection connection,
			    AbstractPackage pack, Node[] table)
      throws IOException
  { int len = 0;
    int lh_nibble = tag_byte >>> 4;
    String name = input_string_with_length(s);
    String packageName = null;
    if (lh_nibble == 0)
      { packageName = pack.name; } // default package
    else if (lh_nibble == out_of_line_nibble_tag)
      { packageName = input_string_with_length(s); }
    else { packageName = get_magic_package_from_number(lh_nibble); }
    if (packageName == null)
      { throw new RuntimeException
	     ("Illegal built in package index: " + lh_nibble); 
      }
    // Try to intern as a quasi symbol.  This will do the right thing is a
    // package already exists.
    return QuasiSymbol.intern(name, packageName);
  }


  // ------------ Strings -----------
  static void encode_string(OKBCString n, OutputStream s, boolean client_p,
			    Hashtable ht) 
    throws IOException
  {
    int rh_nibble = Node.STRING;
    int lh_nibble = out_of_line_nibble_tag;
    String string = n.string;
    int len = string.length();
    if (len < out_of_line_nibble_tag) { lh_nibble = len; }
    s.write((lh_nibble << 4) + rh_nibble);
    if (lh_nibble == out_of_line_nibble_tag) 
      { output_4_byte_int(len, s); }
    output_string(string, s);
  }

  static Node decode_string(int tag_byte, InputStream s, Kb kb_type,
			    boolean client_p, Connection connection,
			    Node[] table)
      throws IOException
  { int lh_nibble = tag_byte >>> 4;
    int len = lh_nibble;
    if (lh_nibble == out_of_line_nibble_tag) { len = input_4_byte_int(s); }
    // System.out.println("lh_nibble = " + lh_nibble);
    // System.out.println("len = " + len);
    return new OKBCString(input_string(len, s));
  }


  // ------------ OKBCIntegers -----------
  static void encode_integer(OKBCInteger n, OutputStream s, boolean client_p,
			     Hashtable ht) 
    throws IOException
  {
    int rh_nibble = Node.INTEGER;
    int lh_nibble = out_of_line_nibble_tag;
    long number = n.number;
    if (number >= out_of_line_nibble_tag || number < 0) 
      { lh_nibble = out_of_line_nibble_tag; }
    s.write((lh_nibble << 4) + rh_nibble);
    if (lh_nibble == out_of_line_nibble_tag) 
      { output_8_byte_int(number, s); }
  }

  static Node decode_integer(int tag_byte, InputStream s, Kb kb_type,
			     boolean client_p, Connection connection,
			     Node[] table)
      throws IOException
  { int lh_nibble = tag_byte >>> 4;
    long result = lh_nibble;
    if (lh_nibble == out_of_line_nibble_tag)
      { result = input_8_byte_int(s); }
    return new OKBCInteger(result);
  }


  // ------------ Reals -----------
  static void encode_real(Real n, OutputStream s, boolean client_p,
			  Hashtable ht) 
    throws IOException
  {
    int rh_nibble = Node.REAL;
    double number = n.number;
    s.write(rh_nibble);
    output_8_byte_int(Double.doubleToLongBits(number), s);
  }

  static Node decode_real(int tag_byte, InputStream s, Kb kb_type,
			  boolean client_p, Connection connection,
			  Node[] table)
      throws IOException
  { double result = Double.longBitsToDouble(input_8_byte_int(s));
    return new Real(result);
  }


  // ------------ True -----------
  static void encode_true(True n, OutputStream s, boolean client_p,
			  Hashtable ht) 
    throws IOException
  {
    int rh_nibble = Node.TRUE;
    s.write(rh_nibble);
  }

  static Node decode_true(int tag_byte, InputStream s, Kb kb_type,
			  boolean client_p, Connection connection, Node[] table)
      throws IOException
  { return Node._T; }


  // ------------ False -----------
  static void encode_false(False n, OutputStream s, boolean client_p,
			   Hashtable ht) 
    throws IOException
  {
    int rh_nibble = Node.FALSE;
    s.write(rh_nibble);
  }

  static Node decode_false(int tag_byte, InputStream s, Kb kb_type,
			   boolean client_p, Connection connection,
			   Node[] table)
      throws IOException
  { return Node._NIL; }


  // ------------ Kbs -----------
  static void encode_kb(Kb n, OutputStream s, boolean client_p,
			AbstractPackage pack, Hashtable ht)
    throws IOException
  {
    if (client_p) 
      { throw new RuntimeException
	("Can't encode a generic Node " + n);
      }
    else
      {
	int rh_nibble = Node.Kb;
	s.write(rh_nibble);
	if (n.kbname == null)
	  { encode_internal(n._kb_type, s, client_p, pack, ht);}
	else { encode_internal(n.kbname, s, client_p, pack, ht); }
	if (n.kb_type_name == null)
	  { AbstractSymbol type = Node.abstract_type_from_concrete_kb(n);
	    if (type == null)
	      { throw new RuntimeException
		  ("Cannot compute abstract kb type for " + n + 
		   ".  Its kb type has not been registered with "+
		   "register_concrete_kb_type.");
	      }
	    else { encode_internal(type, s, client_p, pack, ht); }
	  }
	else { encode_internal
	         (Node.abstract_type_from_concrete_type(n.kb_type_name),
		  s, client_p, pack, ht);
	     }
      }
  }

  static Node decode_kb(int tag_byte, InputStream s, Kb kb_type, 
			boolean client_p, Connection connection,
			AbstractPackage pack, Node[] table)
      throws IOException
  { Symbol name = (Symbol)
      (decode_internal(s, kb_type, client_p, connection, pack, table));
    Symbol type = (Symbol)
       (decode_internal(s, kb_type, client_p, connection, pack, table));
    return NetworkGeneral.decodeKb_given_name_and_type
      (name, type, client_p, connection);
  }


  // ------------ Frame handles -----------
  static void encode_frame_handle
  (FrameHandle n, OutputStream s, boolean client_p, AbstractPackage pack,
   Hashtable ht)
    throws IOException
  { int rh_nibble = Node.FRAMEHANDLE;
    s.write(rh_nibble);
    encode_internal(n.id, s, client_p, pack, ht);
    encode_internal(n.kb_id, s, client_p, pack, ht);
  }

  static Node decode_frame_handle(int tag_byte, InputStream s, Kb kb_type, 
				  boolean client_p, Connection connection,
				  AbstractPackage pack, Node[] table)
      throws IOException
  { Node    id = decode_internal(s, kb_type, client_p, connection, pack, table);
    Node kb_id = decode_internal(s, kb_type, client_p, connection, pack, table);
    return FrameHandle.internThing (id, kb_id, client_p, false);
  }


  // ------------ Procedures -----------
  static void encode_procedure
  (Procedure n, OutputStream s, boolean client_p, AbstractPackage pack,
   Hashtable ht)
    throws IOException
  { int rh_nibble = Node.PROCEDURE;
    s.write(rh_nibble);
    encode_internal(n.name,        s, client_p, pack, ht);
    encode_internal(n.arguments,   s, client_p, pack, ht);
    encode_internal(n.expression,  s, client_p, pack, ht);
    encode_internal(n.environment, s, client_p, pack, ht);
  }

  static Node decode_procedure(int tag_byte, InputStream s, Kb kb_type, 
			       boolean client_p, Connection connection,
			       AbstractPackage pack, Node[] table)
      throws IOException
  { Node name        =
      decode_internal(s, kb_type, client_p, connection, pack, table);
    Node arguments   =
      decode_internal(s, kb_type, client_p, connection, pack, table);
    Node expression  =
      decode_internal(s, kb_type, client_p, connection, pack, table);
    Node environment =
      decode_internal(s, kb_type, client_p, connection, pack, table);
    return Procedure.intern_procedure
      (name, arguments, expression, environment);
  }


  // ------------ EndOfFileNode -----------
  static void encode_EndOfFileNode
        (EndOfFileNode n, OutputStream s, boolean client_p, Hashtable ht)
    throws IOException
  {
    s.write(Node.ENDOFFILE);
  }

  static Node decode_EndOfFileNode(int tag_byte, InputStream s, Kb kb_type,
				   boolean client_p, Connection connection,
				   Node[] table)
      throws IOException
  { return Node.__EOF; }


  // ------------- Test harness ----------------
  public static void main (String argv[]) {
    Package testPackage = Package.find_or_create_package("OKBC-USER");
    CachingKb.ensure_registered();
    MinimalNetworkKb.ensure_registered();
    NetworkKb.ensure_registered();
    TupleKb.ensure_registered();
    Symbol test_name = Symbol.intern("TAXA", "OKBC-TEST");
    Kb kb_type = Node.get_kb_type(Symbol.intern("TUPLE-KB", "OK-BACK"));
    Kb kb = (TupleKb)kb_type.create_kb
      (test_name, Node._NIL, Node._NIL, LocalConnection.local_connection());
    Node handle = Test.trivial_create_class(kb, test_name, Node._NIL);
    Node keyword_handle = kb.coerce_to_frame(Node._inverse).firstValue();
    Procedure proc
    = (Procedure)kb.create_procedure
    ("(frame kb)",
     "(list frame (get-frame-pretty-name frame :kb kb) kb)",
     Node._NIL);
    Node input = Cons.list(proc,
			   handle,
			   keyword_handle,
			   kb_type,
			   kb,
			   Node._all, 
			   Symbol.intern("FOO", "OKBC"),
			   Symbol.intern("FOO", "FOO"),
			   new OKBCString("x"),
			   new OKBCString("0123456789ABCDEF09123456789"),
			   new OKBCInteger(0),
			   new OKBCInteger(-1),
			   new OKBCInteger(2001),
			   new Real(42.834756384763),
			   Node._T,
			   Node._NIL);
    byte[] encoded = null;
    Node decoded = null;
    Enumerator e = input.enumerate_list();
    while (e.has_more_p()) {
      Node arg = e.next();
      System.out.println("Testing: " + arg);
      encoded = encode(arg, false, testPackage);
      decoded = decode(encoded, null, false, null, testPackage);
      boolean equal_p = decoded.equals(arg);
      if (!equal_p)
	{
	  System.out.println
	    ("Encoded = " + encoded + ", length = " + encoded.length);
	  System.out.println("Decoded = " + decoded);
	  System.out.println("Same? " + equal_p);
	  for (int i = 0; i < encoded.length; i++) {
	    System.out.println(i + " -> " + encoded[i]);
	  }
	}
    }
    encoded = encode(input, false, testPackage);
    System.out.println("Encoded = " + encoded);
    decoded = decode(encoded, null, false, null, testPackage);
    System.out.println("Decoded = " + decoded);
    System.out.println("**Same? " + decoded.equals(input));
    System.out.println("Length of encoded = " + encoded.length);

    Node big_input = Cons.list(input, input, input);
    encoded = encode_node_uniquifying(big_input, false,
				      testPackage);
    System.out.println("Length of 3x encoded = " + encoded.length);
    decoded = decode_using_table(encoded, null, false, null, testPackage);
    System.out.println("**Same? " + decoded.equals(big_input));
  }

}

