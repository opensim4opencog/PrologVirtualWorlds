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

public class LispReader {

  InputStream stream = null;
  Reader reader = null;
  int last_char = -2;
  boolean char_unread_p = false;
  boolean chars_read_yet = false;
  int escape_character = 0;

  static char eof_char = (char) -1;
  static int eof_char_as_int = eof_char;
  static final Symbol open_paren = new Symbol("(");
  static final Symbol close_paren = new Symbol(")");
  static final Symbol dot = new Symbol(".");
  static final Symbol quote = new Symbol("'");
  static final Symbol backquote = new Symbol("`");
  static final Symbol comma = new Symbol(",");
  static final Symbol semicolon = new Symbol(";");
  static final Symbol comma_at = new Symbol(",@");

  static final Symbol xappend = new Symbol("xappend");
  static final Symbol bq_comma = new Symbol("bq_comma");
  static final Symbol bq_comma_at = new Symbol("bq_comma_at");

  public IoSyntax io_syntax = new IoSyntax();
  public Node eof_value;
  public boolean eof_error_p;
  public int read_base = 10;
  public Node readtable_case = Node._upcase;
  public boolean null_for_leading_lfs = false;

  // Can be set to 0 to disable.
  public void set_escape_character (char ch) {
    escape_character = (int) ch;
  }

  // Defaulot sets the escape char to Escape
  public void set_escape_character () {
    escape_character = (int) '\u001B';
  }

  public LispReader (InputStream str, IoSyntax ioSyntax, 
		     boolean the_eof_error_p, Node the_eof_value, int base,
		     Node the_readtable_case) {
    stream = str;
    io_syntax = ioSyntax;
    eof_error_p = the_eof_error_p;
    eof_value = the_eof_value;
    read_base = base;
    readtable_case = the_readtable_case;
    io_syntax.ensure_package();
  }

  public LispReader (Reader str, IoSyntax ioSyntax, 
		     boolean the_eof_error_p, Node the_eof_value, int base,
		     Node the_readtable_case) {
    reader = str;
    io_syntax = ioSyntax;
    eof_error_p = the_eof_error_p;
    eof_value = the_eof_value;
    read_base = base;
    readtable_case = the_readtable_case;
    io_syntax.ensure_package();
  }

  public LispReader (String string, IoSyntax ioSyntax) {
    reader = new StringReader(string);
    io_syntax = ioSyntax;
    eof_error_p = false;
    eof_value = Node.__EOF;
    read_base = 10;
    readtable_case = Node._upcase;
    io_syntax.ensure_package();
  }
  
  public LispReader (OKBCString string, IoSyntax ioSyntax) {
    reader = new StringReader(string.string);
    io_syntax = ioSyntax;
    eof_error_p = false;
    eof_value = Node.__EOF;
    read_base = 10;
    readtable_case = Node._upcase;
    io_syntax.ensure_package();
  }
  
  public static Node readFromString(String string) {
    return readFromString(string, new IoSyntax(),
			  true, Node._NIL);
  }
  
  public static Node readFromString(String string, AbstractPackage pack) {
    return readFromString(string, new IoSyntax(pack), true, Node._NIL);
  }
  
  public static Node readFromString(String string, IoSyntax ioSyntax) {
    return readFromString(string, ioSyntax, true, Node._NIL);
  }
  
  public static Node readFromString(String string, IoSyntax ioSyntax,
				    boolean eof_error_p, Node eof_value) {
    StringReader str = new StringReader(string);
    LispReader r = new LispReader(str, ioSyntax, eof_error_p, eof_value, 10,
				  Node._upcase);
    return r.read();
  }
  
  public static Node readFromString(Node generalized_string,
				    AbstractPackage pack) {
    return readFromString(generalized_string, new IoSyntax(pack), true,
			  Node._NIL);
  }
  
  public static Node readFromString(Node generalized_string,
				    IoSyntax ioSyntax) {
    return readFromString(generalized_string, ioSyntax, true, Node._NIL);
  }
  
  public static Node readFromString(Node generalized_string,
				    IoSyntax ioSyntax,
				    boolean eof_error_p, Node eof_value) {
    if (generalized_string instanceof OKBCString)
      { return readFromString(((OKBCString)generalized_string).string,
			      ioSyntax, eof_error_p, eof_value);
      }
    else if (generalized_string instanceof AbstractSymbol)
      { return readFromString(((AbstractSymbol)generalized_string).symbolName,
			      ioSyntax, eof_error_p, eof_value);
      }
    else
      { throw new GenericError
	  (Node._T, new OKBCString
	   (generalized_string + " is not a generalized string")).signal();
      }
  }
  
  public void put_package (AbstractPackage pack) {
    io_syntax.io_package = pack;
    io_syntax.ensure_package();
  }
  
  public void put_stream (Reader str) {
    reader = str;
    stream = null;
    last_char = -2;
    char_unread_p = false;
  }
  
  public void put_stream (InputStream str) {
    stream = str;
    reader = null;
    last_char = -2;
    char_unread_p = false;
  }
  
  public int read_char () {
    int ret = 0;
    if (char_unread_p)
      {
	// last_char is left alone, we can still unread it.
	char_unread_p = false;
	int ch = last_char;
	ret = ch;
      }
    else
      {
	int ch;
	try
	  {
	    if (reader != null) { ch = (char)(reader.read()); }
	    else if (stream != null) { ch = (char)(stream.read()); }
	    else
	      { throw new GenericError
		  (Node._T, new OKBCString
		   ("Input stream has not been supplied")).signal();
	      }
	  }
	catch (IOException exception)
	  { throw new GenericError
	      (Node._T, new OKBCString("A read error occurred.  Exception was "
				  + exception.toString()))
	      .signal();
	  }
	last_char = ch;
	ret = ch;
      }
    // System.out.println("Read: " + ret + " = " + (char)ret);
   if (ret == escape_character) { throw new EscapeFound(); }
   if (!chars_read_yet)
     {
       chars_read_yet = true;
       if (null_for_leading_lfs && (ret == 10 || ret == 13)) // CR or LF
	 { throw new CrOrLfFound(); }
     }
    return ret;
  }


  public void unread_char (int ch) {
    if (ch == last_char)
      { char_unread_p = true; }
    else
      { throw new GenericError
	  (Node._T, new OKBCString("Cannot unread a char (" + (char)ch +
				   " other than the last "
				   + "char read: " + last_char)).signal();
      }
  }

  public Node read () 
    {
      try { return (Node)(read(false, false)); }
      catch (EscapeFound e)
	{ System.out.println ("... escape ...");
	  return null; 
	}
      catch (CrOrLfFound e)
	{ chars_read_yet = false;
          return null; 
	}
      catch (LispReaderAbort e)
	{ return null; }
    }

  public Object read (boolean recursive_p, boolean inside_backquote_p) {
    Object token = read_token();
    // System.out.println("Token was: " + token);
    if (token == semicolon)
      {
	int ch;
	do { ch = read_char(); }
	while (ch != '\n' && ch != '\r' && ch != eof_char_as_int);
	return read(recursive_p, inside_backquote_p);
      }
    else if (token == open_paren)
      { return read_list(inside_backquote_p); }
    else if (token == quote)
      {
	if (inside_backquote_p)
	  { return Cons.list
	      (bq_comma,
	       Cons.list(Node.__list,
			 Cons.list(Node.__quote, Node.__quote),
			 (Node)(read(true, inside_backquote_p))));
	  }
	else { return Cons.list(Node.__quote,
				(Node)(read(true, inside_backquote_p)));
	     }
      }
    else if (token == backquote)
      {
	Node res = (Node)read(true, true);
	// System.out.println("Result*** = " + res);
	if (res instanceof Cons)
	  { if (res.car() == xappend) { return unwind_append(res); }
	    else if (res.car() == bq_comma) { return res.cadr(); }
	    else if (res.car() == bq_comma_at) 
	      { throw new RuntimeException
		  ("Illegal ,@ in backquoted expression.");
	      }
	    else { return Cons.list(Node.__quote, res); }
	  }
	else { return Cons.list(Node.__quote, res); }
      }
    else if (token == comma)
      {
	if (inside_backquote_p)
	  {
	    Node next = (Node)(read(true, false));
	    return Cons.list(bq_comma, next);
	  }
	else 
	  { throw new GenericError
	      (Node._T, new OKBCString
	       ("Comma found outside a backquote.")).signal();
	  }
      }
    else if (token == comma_at)
      {
	if (inside_backquote_p)
	  {
	    Node next = (Node)(read(true, false));
	    return Cons.list(bq_comma_at, next);
	  }
	else 
	  { throw new GenericError
	      (Node._T, new OKBCString
	       ("Comma-at found outside a backquote.")).signal();
	  }
      }
    else if (token == Node.__EOF)
      { 
	if (eof_error_p)
	  { throw new GenericError
	      (Node._T, new OKBCString("EOF found during a read")).signal();
	  }
	else { return eof_value; }
      }
    else if (token instanceof Node) { return token; }
    else if (recursive_p &&
	     (token == close_paren ||
	      token == dot))
      { return token; }
    else 
      { throw new GenericError
	  (Node._T, new OKBCString("Cannot unread a char other than the last "
			      + "char read: " + last_char)).signal();
      }
  }

  static Node unwind_append (Node arg) {
    // System.out.println("Unwinding: " + arg);
    if (arg == Node._NIL || arg == null) { return Node._NIL; }
    else if (arg instanceof Cons)
      { if (arg.car() == xappend)
	  { int len = arg.length();
	    if (len == 1) { return Node._NIL; }
	    else if (len == 2) { return unwind_append(arg.cadr()); }
	    else { return Cons.list(Node.__append,
				    unwind_append(arg.cadr()),
				    unwind_append
				    (new Cons(xappend, arg.cdr().cdr())));
	         }
	    }
        else { return new Cons(unwind_append(arg.car()),
			       unwind_append(arg.cdr()));
             }
      }
    else { return arg; }
  }

  Node read_list (boolean inside_backquote_p) {
    // We have already read the opening paren.
    // System.out.println("Start list");
    Node res = Node._NIL;
    Cons tail = null;
    if (inside_backquote_p)
      {
	tail = new Cons(xappend);
	res = tail;
      }
    while (true) {
      Object token = read(true, inside_backquote_p);
      if (token == close_paren) { break; }
      else if (token == dot) 
	{ 
	  if (tail == null)
	    { throw new GenericError
		(Node._T, new OKBCString
		 ("Cannot have a dotted list with no CAR")).signal();
	    }
	  else
	    { 
	      Object next_tok = read(true, inside_backquote_p);
	      if (next_tok instanceof Node)
		{
		  tail.set_cdr((Node) next_tok);
		  break;
		}
	      else
		{ throw new GenericError
		    (Node._T, new OKBCString
		     ("Cannot have a dotted list with a bogus CDR")).signal();
		}
	    }
	}
      else if (token instanceof Node)
	{
	  if (inside_backquote_p)
	    {
	      Node node = (Node)token;
	      if (node instanceof Cons && node.car() == bq_comma)
		{ node = listify(node.cadr()); }
	      else if (node instanceof Cons && node.car() == bq_comma_at)
		{ node = node.cadr(); }
	      else if (node instanceof Cons && node.car() == xappend)
		{ node = listify(node); }
	      else
		{ node = listify(Cons.list(Node.__quote, node)); }
	      Cons new_cell = new Cons(node);
	      tail.set_cdr(new_cell);
	      tail = new_cell;
	    }
	  else
	    {
	      Node node = (Node)token;
	      if (tail == null)
		{
		  Cons new_cell = new Cons(node);
		  res = new_cell;
		  tail = new_cell;
		}
	      else
		{
		  Cons new_cell = new Cons(node);
		  tail.set_cdr(new_cell);
		  tail = new_cell;
		}
	    }
	}
    }
    // System.out.println("Result list = " + res);
    return res;
  }


  Node listify (Node x) {
     if (x instanceof Cons && x.car() == Node.__quote)
       { return Cons.list(Node.__quote, Cons.list(x.cadr())); }
     else { return Cons.list(Node.__list, x); }
  }


  boolean whitespace_char_p (int ch) {
    return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
  }


  Node read_atom () {
    // We have already read the opening quote.
    StringBuffer sb = new StringBuffer();
    int ch;
    do
      {
	ch = read_char();
	if (ch == '"' || whitespace_char_p(ch) || ch == ')' || ch == '(' ||
	    ch == ';' || ch == eof_char_as_int)
	  { unread_char(ch);
	    break; 
	  }
	else { sb.append((char)ch); }
      }
    while (true);
    String string = sb.toString();
    long l;
    try { l = Long.parseLong(string, read_base); }
    catch (NumberFormatException e)
      {
	double d;
	try { d = Double.valueOf(string).doubleValue(); }
	catch (NumberFormatException e2)
	  {
	    if (".".equals(string)) { return dot; }
	    else { return read_symbol(string); }
	  }
	return new Real(d);
	
      }
    return new OKBCInteger(l);
  }


  String hack_case_of_read_symbol (String string) {
    if (readtable_case == Node._upcase)
      { return string.toUpperCase(); }
    else if (readtable_case == Node._downcase)
      { return string.toLowerCase(); }
    else { return string; }
  }

  
  AbstractSymbol read_symbol (String string) {
    int colon_index = string.indexOf(':');
    if (colon_index == -1)
      { return QuasiSymbol.intern(hack_case_of_read_symbol(string),
				  io_syntax.io_package); }
    else { String package_name = string.substring(0, colon_index);
	   if (colon_index == string.length()-1)
	     { throw new GenericError
	       (Node._T, new OKBCString("Illegal symbol found: " + string))
	       .signal();
	     }
	   String symbol_name = (string.charAt(colon_index + 1) == ':'
				 ? string.substring(colon_index + 2)
				 : string.substring(colon_index + 1));
	   return QuasiSymbol.intern(hack_case_of_read_symbol(symbol_name),
				     (package_name.equals("")
				      ? "KEYWORD"
				      : hack_case_of_read_symbol
				      (package_name)));
	 }
  }


  Object read_token () {
    // System.out.println("Start token");
    int ch;
    // skip leading whitespace
    do { ch = read_char(); }
    while (whitespace_char_p(ch));
    if (ch == eof_char_as_int) { return Node.__EOF; }
    else if (ch == ';') { return semicolon; }
    else if (ch == '(') { return open_paren; }
    else if (ch == ')') { return close_paren; }
    else if (ch == '`') { return backquote; }
    else if (ch == '"') { return read_string (); }
    else if (ch == '\'') { return quote; }
    else if (ch == ',')
      {
	ch = read_char();
	if (ch == '@') { return comma_at; }
	else
	  { // System.out.println("-----");
	    unread_char(ch);
	    return comma;
	  }
      }
    else
      {
	unread_char(ch);
	return read_atom();
      }
  }

  OKBCString read_string () {
    // We have already read the opening quote.
    StringBuffer sb = new StringBuffer();
    int ch;
    do
      {
	ch = read_char();
	if (ch == eof_char_as_int)
	  { throw new GenericError
	    (Node._T, new OKBCString("EOF found in the middle of a string"))
	    .signal();
	  }
	else if (ch == '"') { break; }
	else if (ch == '\\') 
	  { ch = read_quoted_character(); 
	    sb.append((char)ch);
	  }
	else { sb.append((char)ch); }
    }
    while (true);
    return new OKBCString(sb.toString());
  }


  int read_quoted_character() {
    int ch = read_char();
    return ch;
  }

  public static void main (String arg[]) {
    LispReader r = null;
    IoSyntax syn = new IoSyntax("TEST");
    //    InputStream str = new StringReader("`(:and (:subclass-of ?x ,root) (:subclass-of ?y ?x) (:slot-of ?z ,@args))");
    // InputStream 
    Reader str;

    str = new StringReader("hello");    
    r = new LispReader(str, syn, false, Node._NIL, 10, Node._upcase);
    r.readtable_case = Node._downcase;

    str = new StringReader("hello");    
    r.put_stream(str);
    System.out.println("Result = ");
    Cons.pretty_print(r.read());
    System.out.println();

    str = new StringReader("\"hello\"");
    r.put_stream(str);
    System.out.println("Result = ");
    Cons.pretty_print(r.read());
    System.out.println();

    str = new StringReader("(hpkb::capital france ?x)");
    r.put_stream(str);
    System.out.println("Result = ");
    Cons.pretty_print(r.read());
    System.out.println();

    str = new StringReader(" 42 hello ");
    r.put_stream(str);
    System.out.println("Result = ");
    Cons.pretty_print(r.read());
    System.out.println();

    str = new StringReader(" -42e20 hello ");
    r.put_stream(str);
    System.out.println("Result = ");
    Cons.pretty_print(r.read());
    System.out.println();

    str = new StringReader("(:and (:subclass-of ?x root) '(:subclass-of ?y ?x) (:slot-of ?z \"Hello \\\"  Rice\" args)))");
    r.put_stream(str);
    System.out.println("Result = ");
    Cons.pretty_print(r.read());
    System.out.println();

    str = new StringReader("`(:and ,foo)");
    r.put_stream(str);
    System.out.println("Result = ");
    Cons.pretty_print(r.read());
    System.out.println();

    str = new StringReader("`(:and (:subclass-of ,?x root) '(:subclass-of ?y ?x) ,@(:slot-of ?z \"Hello \\\"  Rice\" args))");
//    str = new StringReader("`(:and (:subclass-of ,?x root) '(:subclass-of ?y ?x))");
    r.put_stream(str);
    System.out.println("Result = ");
    Cons.pretty_print(r.read());
    System.out.println();

    r = new LispReader(System.in, syn, false, Node._NIL, 10, Node._upcase);
    r.null_for_leading_lfs = true;
    r.set_escape_character();
    while(true) {
      System.out.println();
      System.out.print("=> ");
      Node form = r.read();
      System.out.println("Result is " + form);
    }

  }


}


class LispReaderAbort extends Error {}
class EscapeFound extends LispReaderAbort {}
class CrOrLfFound extends LispReaderAbort {}
