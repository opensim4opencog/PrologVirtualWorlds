package logicmoo.agent;

import logicmoo.*;
import logicmoo.api.*;
import logicmoo.net.*;
import logicmoo.util.*;


// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;

// OpenCyc
import org.opencyc.api.*;
import org.opencyc.creation.*;
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;

import ViolinStrings.*;



public class MooCommandLine extends MooAgentThread {

    protected PrintWriter out = null;
    protected BufferedReader in = null;
    protected boolean loggedin = false;

    public MooCommandLine() {
    }

    public MooCommandLine(PrintWriter pout, BufferedReader pin) { 
	try {
	    out = pout;
	    in = pin;
	    cyc = new LogicMooCycAccess();
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }


    public static void main(String[] params) {
	runClientAt(new PrintWriter(System.out,true),new BufferedReader(new InputStreamReader(System.in)));
    }

    public static void runClientAt(PrintWriter pout, BufferedReader pin) {
	try {
	    MooCommandLine who = new MooCommandLine(pout,pin);
	    who.doLogin();
	    who.run();
	} catch ( Exception e ){
	    e.printStackTrace();
	}
    }


    public void run(){
	isRunning = true;
	try {
	    doLogin();
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	while ( loggedin && isRunning ) {
	    try {
		if ( in.ready() ) {
		    String line = in.readLine();
		    if ( line!=null ) {
			me.enact(line.trim());
			promptStdIn();
		    }
		}
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	}
	disconnect();
    }


    public Object prompt(Object message) {
	printSimple(message);
	try {
	    return in.readLine();
	} catch ( Exception e ) {
	    return e;
	}
    }

    protected void disconnect() {
	loggedin = false;
	try {
	    out.close();
	    in.close();
	} catch ( IOException e ) {
	    e.printStackTrace();
	}
    }

    protected void doLogin() {
	CycFort userName = null;
	println("\nWelcome to LogicMoo!");
	while ( userName ==null ) {
	    printSimple("username: ");
	    try {
		String usernamestr = in.readLine();
		userName = cyc.makeCycConstant(usernamestr);
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	}

	loggedin = true;
	println("Password skipped (for a while)\n");
	try {
	    nclassloader = new NClassLoader("/opt/sourceforge/logicmoo/src/",".class","");
	    meclass = nclassloader.findClass("logicmoo.cmd.ActorCommandParser");
	    Constructor cons = meclass.getConstructors()[0];
	    me = (IActorCommandParser)meclass.newInstance();
	    me.setData(cyc,userName,null,null,this);
	    nclassloader = null;
	    meclass = null;
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	me.setData(cyc,userName,null,null,this);
	println("Authenticated!\n");
	promptStdIn();
    }

    protected void promptStdIn() {
	printRaw("["+me.getUserName()+"@"+me.getUserLocation()+" "+me.getInterp()+"]$ ");
    }


    public boolean printRaw(String message) {
	if ( message == null ) return true;
	try {
	    out.print(Strings.change(message,"<br>","\n"));
	    out.flush();
	} catch ( Exception e ) {
	}
	return true;
    }

    public boolean receiveEvent(Object event) {
	printSimple("<br>");
	if ( isEventsOn ) printFormat(event);
	promptStdIn();
	return true;
    }

}

