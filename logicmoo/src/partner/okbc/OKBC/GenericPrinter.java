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

public class GenericPrinter {

  Object stream = null;
  ByteArrayOutputStream bytes = null;

  public GenericPrinter(PrintStream str) {
    stream = str;
  }

  public GenericPrinter(PrintWriter str) {
    stream = str;
  }

  public OKBCString extract_string () {
    ((PrintWriter)stream).flush();
    return new OKBCString(bytes.toString());
  }

  public GenericPrinter(Node str) {
    if (str == Node._NIL)
      { bytes = new ByteArrayOutputStream();
	PrintWriter stringStream = new PrintWriter(bytes);
	stream = stringStream;
      }
    else { stream = System.out; }
  }

  void println(Object o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(Object o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println(boolean o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(boolean o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println(char o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(char o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println(int o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(int o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println(long o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(long o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println(float o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(float o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println(double o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(double o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println(char s[]) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(s);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(s);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(char s[]) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(s);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(s);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println(String o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void print(String o) { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.print(o);
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.print(o);
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

  void println() { 
    if (stream instanceof PrintWriter)
      { PrintWriter str = (PrintWriter)stream; 
	str.println();
      }
    else if (stream instanceof PrintStream)
      { PrintStream str = (PrintStream)stream; 
	str.println();
      }
    else { throw new RuntimeException("Unknown stream " + stream); }
  }

}
