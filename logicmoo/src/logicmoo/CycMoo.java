package logicmoo;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.awt.*;

import jamud.*;

import jamud.command.*;
import jamud.object.*;
import jamud.object.event.*;
import jamud.util.*;
import com.jamud.commands.*;
import com.jamud.connections.*;
import com.jamud.communications.*;
import com.jamud.creation.*;
import net.n3.nanoxml.*;
import org.opencyc.webserver.WebServer;
import org.opencyc.util.*;
import org.opencyc.api.*;
import org.opencyc.cycobject.*;
import ViolinStrings.*;

import javax.servlet.jsp.*;


public class CycMoo extends LogicMoo {


    public static void CycMoo() throws IOException {
    }

    public static JamudWorldAccess jamudWorldAccess;
    public static CycIRCBot cycIRCBot;
    public static org.opencyc.webserver.WebServer cycWebserverThread;
    public static CycMooAccess cycMooAccess;

    /*
    
    For loading references to JamudWorldAccess basic objects as used in:

    loadJamudWorldAccessReferences:-
      jamud_object(JAMUD),
      java_invoke_method(JAMUD,getJamudWorldAccessInstance(Instance)),
      java_invoke_method(JAMUD,getJamudWorldAccessMudObjectRoot(MudRoot)),!,
    assert(jamud_instance(Instance)),
    assert(jamud_root(MudRoot)).
    
    
    */              

    public static JamudWorldAccess getJamudWorldAccessInstance() {
	return  jamudWorldAccess;
    }

    /*
    
    
    For loading references to JamudWorldAccess basic objects as used in:

    loadJamudWorldAccessReferences:-
      jamud_object(JAMUD),
      java_invoke_method(JAMUD,getJamudWorldAccessInstance(Instance)),
      java_invoke_method(JAMUD,getJamudWorldAccessMudObjectRoot(MudRoot)),!,
    assert(jamud_instance(Instance)),
    assert(jamud_root(MudRoot)).
    
    */         

    public static MudObjectRoot getJamudWorldAccessMudObjectRoot() {
	return  jamudMudObjectRoot;
    }

    public static void ensureReady() {
	System.out.println("Ensuring running");
	startCycHttpdProxy();
	startJamudWorldAccess();
	startCycMooAccess();
    }

    public static CycMooAccess getCycMooAccess() {
	ensureReady(); 
	return cycMooAccess;
    }

    public static CycAccess getCycAccess() {
	ensureReady(); 
	return(CycAccess)cycMooAccess;
    }

    public static void startCycMooAccess() {
	if ( cycMooAccess==null ) {
	    System.out.println("cycMooAccess==null");
	    try {
		cycMooAccess = new CycMooAccess();
		loadMooFiles();
	    } catch ( Exception e ) {
		e.printStackTrace(System.out);
	    }

	}
    }

    public static void loadMooFiles() {
	try {
	    constructMtFromFile("JavaMt.kif",cycMooAccess.javaMt);
	} catch ( Exception e ) {
	    System.err.println(e);
	}
	try {
	    constructMtFromFile("JamudMt.kif",cycMooAccess.jamudMt);
	} catch ( Exception e ) {
	    System.err.println(e);
	}
	try {
	    constructMtFromFile("LogicMooMt.kif",cycMooAccess.logicMooMt);
	} catch ( Exception e ) {
	    System.err.println(e);
	}
	try {
	    cycIRCBot = new CycIRCBot(cycMooAccess,"CycLBot","http://logicmoo.sourceforge.net","irc.openprojects.net",6667,"#opencyc");
	    cycIRCBot.start();
	} catch ( Exception e ) {
	    System.err.println(e);
	}
    }

    public static File ontologyFile(String file) {
	File f = null;
	f=new File(file);
	if ( f.exists() ) return f;
	f=new File("../ontologies/" + file);
	if ( f.exists() ) return f;
	f=new File("../ontologies/kif/" + file);
	if ( f.exists() ) return f;
	return new File(file);
    }

    public static void killMt(String mtName) throws IOException, CycApiException {
	cycMooAccess.kill(cycMooAccess.makeCycConstant(mtName));
    }

    public static void constructMtFromFile(String file,String mtName) throws Exception {
	constructMtFromFile(ontologyFile(file),mtName);
    }

    public static void constructMtFromFile(File file,String mtName) throws Exception {
	constructMtFromFile(file,cycMooAccess.makeCycConstant(mtName));
    }

    public static void constructMtFromFile(String file,CycConstant mtName) throws Exception {
	constructMtFromFile(ontologyFile(file),mtName);
    }

    public static void constructMtFromFile(File file,CycConstant mtName) throws Exception {
	// cycMooAccess.kill(mtName);
	System.out.println("constructMtFromFile " + file.getAbsolutePath() + " " + mtName.cyclify());
	AssertionsCollection theMt = new AssertionsCollection(cycMooAccess);
	theMt.setDefaultMt(mtName);
	theMt.setDefaultCyclist("CycAdministrator");
	theMt.setDefaultProject("OpenCycProject");
	theMt.load(file);
	if ( theMt.testAssertionsHtml(null) )
	    theMt.commitAssertions(null);
    }


    public static void startCycHttpdProxy() {
	if ( cycWebserverThread==null ) {
	    System.out.println("cycWebserverThread==null");
	    try {
		Log.makeLog();
		cycWebserverThread = new org.opencyc.webserver.WebServer();
		cycWebserverThread.start();
		//Thread            

	    } catch ( Exception e ) {
		e.printStackTrace(System.out);
	    }
	}
    }
    /*
    Boots the JamudWorldAccess for now from prolog as:
    
    startJamudWorldAccess:-
     jamud_object(JAMUD),!,
     java_invoke_method(JAMUD,startJamudWorldAccess(X)).
    */              
    public static void startJamudWorldAccess() {
	if ( jamudWorldAccess==null ) {
	    try {
		System.out.println("jamudWorldAccess==null");
		jamudWorldAccess = new JamudWorldAccess();
	    } catch ( Exception e ) {
		e.printStackTrace(System.out);
	    }
	}
    }


}

