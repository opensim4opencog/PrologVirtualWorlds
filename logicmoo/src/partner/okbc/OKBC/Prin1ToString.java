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

public class Prin1ToString {

  public static OKBCString prin1ToString (Node form) {
    return prin1ToString(form, new IoSyntax());
  }
  
  public static OKBCString prin1ToString (Node form, IoSyntax ioSyntax) {
    StringWriter s = new StringWriter();
    try { print_internal(form, s, ioSyntax); }
    catch (IOException e)
      {
	throw new GenericError
	   (Node._T, new OKBCString(e.toString())).signal();
      }
    return new OKBCString(s.toString());
  }

  static void print_internal(Node form, Writer stream, IoSyntax ioSyntax)
  throws IOException {
    if (form instanceof Cons)
      {
	stream.write("(");
	Node rest = form;
	boolean first_p = true;
	while (rest instanceof Cons) {
	  if (!first_p) { stream.write(" "); }
	  Node car = rest.car();
	  rest = rest.cdr();
	  print_internal(car, stream, ioSyntax);
	  first_p = false;
	}

	if (rest == null || rest == Node._NIL) {}
	else
	  {
	    stream.write(" . ");
	    print_internal(rest, stream, ioSyntax);
	  }
	stream.write(")");
      }
    else if (form instanceof OKBCInteger)
      {
	OKBCInteger i = (OKBCInteger) form;
	long l = i.number;
	if (ioSyntax.print_base == 10)
	  { stream.write(Long.toString(l)); }
	else if (ioSyntax.print_base == 16)
	  { stream.write(Long.toHexString(l)); }
	else if (ioSyntax.print_base == 8)
	  { stream.write(Long.toOctalString(l)); }
	else
	  {
	    throw new GenericError
	      (Node._T, new OKBCString("Illegel print base " +
				       ioSyntax.print_base))
	      .signal();
	  }
      }
    else if (form instanceof Real)
      {
	Real i = (Real) form;
	double d = i.number;
	if (ioSyntax.print_base == 10)
	  { stream.write(Double.toString(d)); }
	else
	  {
	    throw new GenericError
	      (Node._T, new OKBCString("Illegel print base " +
				       ioSyntax.print_base))
	      .signal();
	  }
      }
    else if (form instanceof OKBCString)
      {
	OKBCString s = (OKBCString) form;
	String string = s.string;
	if (ioSyntax.print_escape) { stream.write("\""); }
	for (int i = 0; i < string.length(); i++) {
	  char ch = string.charAt(i);
	  if (ioSyntax.print_escape &&
	      (ch == '"'
	       || ch == '\\'
	       ))
	    { stream.write("\\"); }
	  stream.write(ch);
	}
	if (ioSyntax.print_escape) { stream.write("\""); }
      }    
    else if (form instanceof AbstractSymbol)
      {
	AbstractSymbol s = (AbstractSymbol) form;
	String symbol_name = s.symbolName;
	if (ioSyntax.print_escape)
	  {
	    AbstractPackage p = s.getSymbolPackage();
	    if (p == null)
	      { stream.write("#:"); }
	    else
	      {
		String package_name = p.name;
		if (p == ioSyntax.io_package) {}
		else if (s instanceof Symbol &&
			 (package_name.equals("KEYWORD")))
		  { stream.write(":"); }
		else
		  {
		    print_string_maybe_with_bars(stream, package_name,
						 ioSyntax);
		    stream.write("::");
		  }
	      }
	  }
	print_string_maybe_with_bars(stream, symbol_name, ioSyntax);
      }
    else if (form instanceof TruthValue)
      {
	if (form == Node._NIL)
	  { print_string_maybe_with_bars(stream, "NIL", ioSyntax); }
	else if (form == Node._T)
	  { print_string_maybe_with_bars(stream, "NIL", ioSyntax); }
	else
	  {
	    throw new GenericError
	      (Node._T, new OKBCString("Unknown TruthValue found: " + form))
	      .signal();
	  }
      }    
    else if (ioSyntax.print_readably)
      {
	throw new GenericError
	  (Node._T, new OKBCString("Cannot print " + form + " readably"))
	  .signal();
      }
    else
      // This should be Kb, AbstractHandle, Procedure, etc.
      { stream.write(form.toString()); }
  }

  static void print_string_maybe_with_bars
       (Writer stream, String name, IoSyntax ioSyntax) throws IOException {
    if (non_upper_case_p(name))
      {
	stream.write("|");
	stream.write(name);
	stream.write("|");
      }
    else if (ioSyntax.print_case == Node._upcase ||
	     name.length() == 0)
      { stream.write(name); }
    else if (ioSyntax.print_case == Node._downcase)
      { stream.write(name.toLowerCase()); }
    else if (ioSyntax.print_case == Node._capitalize)
      {
	stream.write(name.substring(0, 1));
	stream.write(name.substring(1).toLowerCase());
      }
    else
      {
	throw new GenericError
	  (Node._T, new OKBCString("Illegel print case " +
				   ioSyntax.print_case))
	  .signal();
      }
  }
  
  static boolean non_upper_case_p (String string) {
    boolean res = false;
    for (int i = 0; i < string.length(); i++) {
      char ch = string.charAt(i);
      if (ch >= 'a' && ch <= 'z')
	{ res = true; }
    }
    return res;
  }

  public static void main (String argv[]) {
    OKBCString s;
    IoSyntax ios2;
    Node one = new OKBCInteger(1);
    Node one_point_two = new Real(1.2);
    Node FRED = Symbol.intern("FRED", "OKBC-USER");
    Node QFRED = QuasiSymbol.intern("QFRED", "QOKBC-USER");
    Node fred = Symbol.intern("fred", "OKBC-USER");
    Node qfred = QuasiSymbol.intern("qfred", "QOKBC-USER");
    Node test_form = Cons.list
      (one,
       one_point_two,
       new Symbol("UnInterned"),
       Node._class,
       Node._NIL,
       Node._T,
       new HashtableNode(),
       fred,
       qfred,
       FRED,
       QFRED,
       new OKBCString("Hello Backslash\\ Quote\" some text"),
       Cons.list(one, one),
       new Cons(one, new Cons(one, one_point_two))
       );

    IoSyntax ios = new IoSyntax();
    ios.io_package = Package.find_or_create_package("OKBC-TEST");
    s = prin1ToString(test_form, ios);
    System.out.println("Test1 = \"" + s + "\"");
    
    ios2 = ios.copy();
    ios2.print_escape = false;
    s = prin1ToString(test_form, ios2);
    System.out.println("Princ Test = \"" + s + "\"");
    
    ios2 = ios.copy();
    ios2.print_case = Node._downcase;
    s = prin1ToString(test_form, ios2);
    System.out.println("Downcase Test = \"" + s + "\"");
    
    ios2 = ios.copy();
    ios2.print_case = Node._capitalize;
    s = prin1ToString(test_form, ios2);
    System.out.println("Capitalize Test = \"" + s + "\"");
    
    ios2 = ios.copy();
    ios2.io_package = Package.find_or_create_package("OKBC-USER");
    s = prin1ToString(test_form, ios2);
    System.out.println("Package Test = \"" + s + "\"");
    
  }  
}
