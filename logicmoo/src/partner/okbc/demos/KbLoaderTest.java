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

package demos;
import java.util.*;
import java.io.*;
import OKBC.*;

public class KbLoaderTest extends KbLoader {

  public static void main (String argv[]) {
    if (OKBCApp.argv_suppliedp("HELP", argv))
      { System.out.println("Usage: java demos.AtpListener [-option]*");
      System.out.println("where options include:");
      System.out.println
	("    -trace-evaluator    : trace client-side evaluator");
      System.out.println
	("    -count              : print a char as each forms is processed");
      System.out.println
	("    -forms              : print out forms as processed");
      System.out.println
	("    -listener (default) : go into a listener after loading");
      System.out.println
	("    -nolistener         : no listener after loading");
      }
    else
      {
	if (OKBCApp.argv_suppliedp("TRACE-EVALUATOR", argv)) 
	  { AbstractEvaluator.trace_p = true; }
	Kb kb = new TupleKb();
	String path_string = OKBCApp.argv_get("path", argv);
	if (path_string == null)
	  { path_string =
	      (KbSummary.prompt_and_read_string("KB file to load")).string;
	  }
	File path = new File(path_string);
	KbLoader loader = new KbLoaderTest();
	if (OKBCApp.argv_suppliedp("count", argv)) 
	  { loader.verbosity = BLIP; }
	if (OKBCApp.argv_suppliedp("forms", argv)) 
	  { loader.verbosity = VERBOSE; }
	loader.load_okbc_kb(path, kb);
	if (OKBCApp.argv_suppliedp("listener", argv) ||
	    !OKBCApp.argv_no_suppliedp("listener", argv) ||
	    OKBCApp.argv_yes_suppliedp("listener", argv))
	  { Listener.breakLoop(kb); }
      }
  }
}
