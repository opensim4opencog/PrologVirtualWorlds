//<pre>
package logicmoo.agent;

import logicmoo.api.*;
import logicmoo.agent.*;
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
import org.opencyc.chat.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;


import ViolinStrings.Strings;


public class MooIrcBot extends IrcChat implements IMooClient  {
    
    public static CycSymbol SYMBOL_NIL = new CycSymbol("NIL");
    private LogicMooCycAccess cyc = null;
    private IActorCommandParser me = null;
    private Class meclass = null;
    private NClassLoader nclassloader = null;
    public boolean isRunning = false;
    
    public IMooClient getClient(){
	return (IMooClient)this;
    }
    public void reloadInterpretor() {
	CycFort name = cyc.makeCycConstant(ircNick);
	CycFort loc = cyc.makeCycConstant("Area1002");
	CycFort mt =  cyc.makeCycConstant("JamudMt");
	try {
	    //
//	    Object[] setData = {(LogicMooCycAccess)cyc,(CycFort)name,(CycFort)loc,(CycFort)mt,(IMooClient)this};
            nclassloader = new NClassLoader("/opt/sourceforge/logicmoo/src/",".class","");
//	    nclassloader = new NClassLoader(fullPath,".class","");
	    meclass = nclassloader.findClass("logicmoo.cmd.ActorCommandParser");
	    Constructor cons = meclass.getConstructors()[0];
	    me = (IActorCommandParser)meclass.newInstance();
 	    me.setData(cyc,name,loc,mt,this);
	    nclassloader = null;
	    meclass = null;
	    System.out.println("reloadInterpretor = " + me.getClass());
	    //Class[] setclass = {cyc.getClass(),name.getClass(),loc.getClass(),mt.getClass(),this.getClass()}; 
	    //meclass.getMethod("setData",setclass).invoke(me,setData);

	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	
    }

    public void setParaphrased(boolean onoff) {
        isParaphrased=onoff;
    }

    public boolean running() {
        return isRunning;
    }
    public boolean isWantingEvents() {
        return isEventsOn;
    }
    protected boolean isEventsOn = true;
    public void setWantingEvents(boolean onoff) {
        isEventsOn = onoff;
        //me.isEventsOn = onoff;
    }

    public Object prompt(Object message) {
        return "";
    }

    public void end() {
	this.ircDisconnect();
        return;
    }

    public boolean enact(Object goal) {
	if (goal==null) return true;
	try {
	    ((IActorCommandParser)me).enact(goal);
	} catch (Exception e){
	    e.printStackTrace();
	}
	return true;
    }

    public IActorCommandParser getActor() {
        return (IActorCommandParser)me;
    }
    public boolean receiveEvent(Object event) {
        if ( isEventsOn ) printFormat(event);
        return true;
    }


    /**
     * Creates a basic unstarted IRC Bot
     *     
     */
    public MooIrcBot() {
    }

    /**
     * Creates a full IRC Bot
    *     
    */

    public MooIrcBot(LogicMooCycAccess cycm, String nick, String comment, String server, int port, String channel) throws IOException, CycApiException{
        super((CycAccess)cycm, nick, comment, server, port, channel);
        Log.makeLog();
        cyc = cycm;
        reloadInterpretor();
    }

    public boolean isParaphrased = false;

    private StringBuffer sendBuffer = new StringBuffer("");

    public boolean printRaw(String message) {
        message =  Strings.change(message,"<br>","\n");
        message =  Strings.change(message,"\\n","\n");
        int crlf = message.indexOf("\n");
        if ( crlf<0 ) {
            sendBuffer.append(message);
            return true;
        }

        sendBuffer.append(message.substring(0,crlf).trim()+" ");
        String willSend = Strings.change(sendBuffer.toString(),"\n"," ").trim();
        if ( willSend.length()>0 );ircSend("privmsg " + ircDestination + " :" + willSend);
        sendBuffer = new StringBuffer(message.substring(crlf+1));
        try {
            Thread.sleep(500);
        } catch ( Exception e ) {
        }
	return true;
    }

    public boolean println(Object message) {
	boolean continueprint = printSimple(message);
	if ( !continueprint ) return false;
	return printRaw("\n");
    }

    public boolean printSimple(Object post) {
	if ( post==null ) return false;
	if ( post instanceof Iterator ) {
	    while ( ((Iterator)post).hasNext() ) {
		println(((Iterator)post).next());
	    }
	    return true;
	}

	if ( post instanceof BufferedReader ) {
	    String line = null;
	    try {
		while ( (line = ((BufferedReader)post).readLine()) != null ) println(line);
	    } catch ( Exception e ) {
		return false;
	    }
	    return true;
	}

	if ( isParaphrased && !(post instanceof String) ) return printSimple(cyc.attemptParaphrase(post));

	String message = post.toString();

	if ( ViolinStrings.Strings.contains(message,"\n") || ViolinStrings.Strings.contains(message,"\r") )
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

	if ( results instanceof String ) return printRaw(""+results);

	if ( results instanceof CycSymbol || results.equals(SYMBOL_NIL) ) return println("no answers found");

	if ( results instanceof Iterator ) {
	    Iterator it = (Iterator)results;
	    boolean keepgoing = true;
	    while ( it.hasNext() && keepgoing )
	    { 
		if ( format!=null ) printRaw(""+format);
		else printRaw(" ");
		keepgoing = printFormat(it.next(),format);
	    }
	    return keepgoing;
	}
	if ( results instanceof CycList ) {
	    CycList answers = (CycList) results;

	    if ( answers.size()==1 && answers.first().equals(MooCommandLine.SYMBOL_NIL) ) return println("true sentence");

	    if ( answers.size()==0 ) return printRaw("(!)");

	    if ( !answers.isProperList() ) return printRaw(cyc.attemptParaphrase(answers));

	    if ( answers.size()==1 ) return printFormat(answers.get(0),format);

	    if ( answers.size()>50 ) {
		println("Your question returned " + answers.size() + " answers .. please refine. (here are the first five)");
		CycList five = new CycList();
		for ( int i=0 ; i<5 ; i++ ) five.add(answers.get(i));
		return printFormat(five,format);
	    }
	    return printFormat(answers.iterator(),format);
	}
	return printSimple(cyc.attemptParaphrase(results));
    }

    public boolean servicePlugin(String from, String hostmask, String returnpath,String token,String params) {
        ircDestination = returnpath;
        if ( token.startsWith("reloadme") ) {
            printRaw("i am reloading<br>");
            reloadInterpretor();
            printRaw("i am done reloading<br>");
        }
	return enact(token+ " " + params);
    }

}


