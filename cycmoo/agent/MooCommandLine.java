package cycmoo.agent;

import cycmoo.*;
import cycmoo.api.*;
import cycmoo.net.*;
import cycmoo.util.*;
import cycmoo.obj.*;


// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;

// OpenCyc
import org.opencyc.api.*;
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
	super();
    }

    public MooCommandLine(PrintWriter pout, BufferedReader pin) {
	super();
	try {
	    out = pout;
	    in = pin;
	} catch( Exception e ) {
	    e.printStackTrace();
	}
    }


    static public void main(String[] params) {
	runClientAt(new PrintWriter(System.out,true),new BufferedReader(new InputStreamReader(System.in)));
    }

    static public void runClientAt(PrintWriter pout, BufferedReader pin) {
	MooCommandLine who = new MooCommandLine(pout,pin);
	who.run();
    }


    public void run() {
	isRunning = true;
	doLogin();
	while( loggedin && isRunning && !interrupted() )
	   enact(me.getUserName(),me.getUserName(),promptStdIn());
	disconnect();
    }

    protected void disconnect() {
	loggedin = false;
	out.println("goodbye.");
	try {
	    out.close();
	    in.close();
	} catch( IOException e ) {
	    e.printStackTrace();
	}
    }

    protected void doLogin() {
	CycFort userName = null;
	println("<br>Welcome to the LogicMoo.");
	while( userName == null ) {
	    try {
		userName = cyc.makeCycConstant(""+prompt("username: "));
	    } catch( Exception e ) {
		e.printStackTrace(out);
	    }
	}
	println("Passwords are not implemented on this development version.");
	loggedin = true;
	reloadInterpretor(userName);
	println("<br>Authenticated as " + me.getUserName().cyclify());
    }


    protected Object promptStdIn() {
	writePrmpt();
	String ret = (String)prompt("");
	if( ret==null )	return promptStdIn();
	ret=ret.trim();
	if( ret.equals("") ) return promptStdIn();
	//this.reloadInterpretor();
	return ret;
    }

    protected void writePrmpt() {
	out.write("["+me.getUserName()+"@"+me.getUserLocation()+" "+me.getInterp()+"]$ ");
	out.flush();
    }

    protected void unInteruptStdIn() {
	writePrmpt();
	out.write(typeBuffer.toString());
	out.flush();
    }

    public Object prompt(Object message) {
	printSimple(message);
	out.flush();
	try {
	    return readToLine();
	} catch( IOException e ) {
	    loggedin=false;    // Kills IOException loops
	    isRunning=false;
	    return e;
	}
    }

    public boolean isUserTyping() {
	return typeBuffer.toString().length()>0;
    }

    private StringBuffer typeBuffer = new StringBuffer(""); 

    private char lastVal = 0;

    public String readToLine() throws IOException {
	char[] abyte = {0};
	while( true ) {
	    in.read(abyte);
	    String last =  typeBuffer.toString();
	    if( abyte[0]==13 || abyte[0]==10 ) {
		if( last.length()>0 ) {
		    typeBuffer = new StringBuffer(""); 
		    return last;
		}
	    } else {
		typeBuffer.append(abyte);
	    }
	}
    }

    public boolean printRaw(String message) {
	if( message == null ) return true;
	try {
	    out.print(Strings.change(message,"<br>","\n"));
	    out.flush();
	} catch( Exception e ) {
	}
	return true;
    }

    public boolean receiveEvent(LogicMooEvent event) {
	if( isEventsOn ) {
	    printSimple("<br>");
	    printFormat(event.getFormula());
	    printSimple("<br>");
	    unInteruptStdIn();
	}
	return true;
    }

}

