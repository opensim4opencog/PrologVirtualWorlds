package cycmoo.agent;

import cycmoo.*;
import cycmoo.api.*;
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
import org.opencyc.javashell.*;
import org.opencyc.cycobject.*;
import org.opencyc.cyclobject.*;
import org.opencyc.kif.*;
import org.opencyc.javashell.*;
import org.opencyc.util.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;

import ViolinStrings.Strings;


public abstract class MooAgentThread extends Thread implements IMooClient {

    protected static CycSymbol SYMBOL_NIL = new CycSymbol("NIL");
    protected LogicMooCycAccess cyc = null;
    protected boolean isRunning = false;
    protected IActorCommandParser me = null;
    protected Class meclass = null;
    protected boolean isParaphrased = false;
    protected boolean isEventsOn = true;

    protected NClassLoader nclassloader  = null;


    public MooAgentThread() {
	try {
	    cyc = new LogicMooCycAccess();
	} catch( Exception e ) {
	    e.printStackTrace();
	}
    }


    abstract public void run();
    abstract public boolean printRaw(String raw);
    abstract public boolean receiveEvent(LogicMooEvent event);
    abstract public Object prompt(Object message);


    public boolean enact(Object returnpath,Object from,Object params) {
	return me.enact(returnpath,from,params);
    }


    public boolean println(Object message) {
	boolean continueprint = printSimple(message);
	if( !continueprint ) return false;
	return printRaw("\n");
    }

    public boolean printSimple(Object post) {
	if( post==null ) return false;
	if( post instanceof Iterator ) {
	    while( ((Iterator)post).hasNext() ) {
		println(((Iterator)post).next());
	    }
	    return true;
	}

	if( post instanceof BufferedReader ) {
	    String line = null;
	    try {
		while( (line = ((BufferedReader)post).readLine()) != null ) println(line);
	    } catch( Exception e ) {
		return false;
	    }
	    return true;
	}

	if( isParaphrased && !(post instanceof String) ) return printSimple(cyc.attemptParaphrase(post));

	String message = post.toString();

	if( ViolinStrings.Strings.contains(message,"\n") || ViolinStrings.Strings.contains(message,"\r") )
	    return println(new BufferedReader(new StringReader(message)));

	return printRaw(""+message); 

    }

    /**
     * Sends the Answer message from Cyc to returnpath
     */

    public boolean printFormat(Object results) {
	return printFormat(results,null);
    }

    public boolean printFormat(Object results,Object format) {

	if( results instanceof String )	return printRaw(""+results);

	if( results instanceof CycSymbol || results.equals(SYMBOL_NIL) ) return println("no answers found");

	if( results instanceof Iterator ) {
	    Iterator it = (Iterator)results;
	    boolean keepgoing = true;
	    while( it.hasNext() && keepgoing ) {
		if( format!=null ) printSimple(format);
		else printRaw(" ");
		keepgoing = printFormat(it.next(),format);
	    }
	    return keepgoing;
	}
	if( results instanceof CycList ) {
	    CycList answers = (CycList) results;

	    if( answers.size()==1 && answers.first().equals(MooCommandLine.SYMBOL_NIL) ) return println("true sentence");

	    if( answers.size()==0 ) return printRaw("(!)");

	    if( !answers.isProperList() ) return printRaw(cyc.attemptParaphrase(answers));

	    if( answers.size()==1 ) return printFormat(answers.get(0),format);

	    if( answers.size()>50 ) {
		println("Your question returned " + answers.size() + " answers .. please refine. (here are the first five)");
		CycList five = new CycList();
		for( int i=0 ; i<5 ; i++ ) five.add(answers.get(i));
		return printFormat(five,format);
	    }
	    //return printFormat(answers.iterator(),format);
	}
	return printSimple(cyc.attemptParaphrase(results));
	//printSimple(format);
    }



    public IMooClient getClient() {
	return(IMooClient)this;
    }

    public IActorCommandParser getActor() {
	return me;
    }

    protected static void initPrivates(MooAgentThread who) {
	try {
	    Log.makeLog();
	    who.cyc = new LogicMooCycAccess();
	} catch( Exception e ) {
	    e.printStackTrace();
	}
    }

    public void reloadInterpretor(CycFort name) {
	CycFort loc = null;
	CycFort mt = null;

	if( me!=null ) {
	    loc = me.getUserLocation();
	    mt = me.getUserMt();
	}
	try {
	    nclassloader = new NClassLoader("/opt/sourceforge/logicmoo/src/",".class","");
	    meclass = nclassloader.findClass("cycmoo.cmd.ActorCommandParser");
	    Constructor cons = meclass.getConstructors()[0];
	    me = (IActorCommandParser)meclass.newInstance();
	    me.setData(cyc,name,loc,mt,this);
	    nclassloader = null;
	    meclass = null;
	    // System.out.println("reloadInterpretor = " + me.getClass());
	} catch( Exception e ) {
	    e.printStackTrace();
	}

    }

    public void reloadInterpretor() {
	CycFort name = me.getUserName();
	reloadInterpretor(name);
    }

    public boolean running() {
	return isRunning;
    }

    public boolean isWantingEvents() {
	return isEventsOn;
    }

    public void setWantingEvents(boolean onoff) {
	isEventsOn = onoff;
    }

    public void end() {
	isRunning=false;
    }

    public void setParaphrased(boolean onoff) {
	isParaphrased = onoff;
    }

    public String toString() {
	return me.getUserName().toString();
    }

    public boolean equals(Object A) {
	boolean res = same(A);
	System.out.println("("+A+"=="+this+") = "+res);
	return res;

    }

    public boolean same(Object A) {
	if( A==null ) return false;
	if( A==this ) return true;
	try {
	    if( A instanceof IMooClient ) return same(((IMooClient)A).getActor().getUserName());
	    if( A instanceof String ) {
		if( ((String)A).startsWith("?") ) return true;
		if( (((String)A).equals(me.getUserName().toString())) || (((String)A).equals(me.getUserLocation().toString())) )
		    return true;
	    }
	    if( A instanceof CycVariable ) return true;
	    if( A instanceof CycConstant ) {
		if( (((CycConstant)A).equals(me.getUserName())) || (((CycConstant)A).equals(me.getUserLocation())) )
		    return true;
	    }
	    if( A instanceof CycFort ) {
		if( (((CycFort)A).equals(me.getUserName().toString())) || (((CycFort)A).equals(me.getUserLocation().toString())) )
		    return true;
	    }
	    return false;
	} catch( Exception e ) {
	    return false;
	}
    }


}


