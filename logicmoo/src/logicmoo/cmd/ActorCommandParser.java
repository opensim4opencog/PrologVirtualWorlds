package logicmoo.cmd;

import logicmoo.*;
import logicmoo.api.*;
import logicmoo.util.*;

// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;


// BeanShell
import bsh.*;
import bsh.util.*;

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


public class ActorCommandParser implements IMooClient, IActorCommandParser {

    public CycFort userName = null;
    public CycFort userLocation = null;
    public CycFort userMt = null;
    public String interp = "mud";

    protected LogicMooCycAccess cyc = null;
    protected IMooClient client = null;
    protected bsh.Interpreter bshell = null; 
    protected static CycSymbol SYMBOL_NIL = new CycSymbol("NIL");

    public void setParaphrased(boolean onoff) {client.setParaphrased(onoff);}
    public Object prompt(Object message) {return client.prompt(message);}
    public boolean receiveEvent(Object event) {return client.receiveEvent(event);}
    public void reloadInterpretor() { client.reloadInterpretor();}
    public boolean running() { return client.running();}
    public boolean isWantingEvents() {
	return isEventsOn;
    }
    protected boolean isEventsOn = true;
    public void setWantingEvents(boolean onoff) {
	isEventsOn = onoff;
    }

    public IActorCommandParser getActor() {return this;}
    public boolean println(Object message) { return client.println(message);}
    public boolean printRaw(String message) { return client.printRaw(message);}
    public boolean printSimple(Object message) { return client.printSimple(message);}
    public boolean printFormat(Object results) { return client.printFormat(results);}
    public boolean printFormat(Object results,Object format) {return client.printFormat(results,format);}
    public void run() { if ( !client.running() ) client.run();}
    public void end() { if ( client.running() )	client.end();}

    public ActorCommandParser(){
    }

    public ActorCommandParser(LogicMooCycAccess cm, CycFort user,CycFort loc,CycFort mt, IMooClient mc) {
	setData(cm,user,loc,mt,mc);
    }

    public void setData(LogicMooCycAccess cm, CycFort user,CycFort loc,CycFort mt, IMooClient mc){
	cyc = cm;
	client = mc;
	userName = user;
	if ( mt==null )	mt =  cyc.makeCycConstant("JamudMt");
	if ( loc==null ) loc = cyc.makeCycConstant("Area1002");
	userLocation = loc;
	userMt = mt;
	try {
	    cyc.assertGaf(userMt,cyc.isa,userName,cyc.makeCycConstant("PlayerInstance"));
	    bshell = new bsh.Interpreter();
	    bshell.set("actor",this);
	    bshell.set("client",mc);
	    bshell.set("cyc",cm);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	moveTo(userName,userLocation);
	cyc.registerUser(userName,client);
    }

    public IMooClient getClient() {
	return client;
    }


    public void finalize() {
	cyc.unregisterUser(userName);
    }

    public boolean enact(Object event) { 
	if ( event==null ) return false;
	String line = event.toString();
	enact(line.trim(),Strings.split(line.trim()));
	return true;
    }


    public void enact(String line,String[] st) {
	if ( st.length==0 ) return;
	try {
	    enact(st[0].toLowerCase(),line.substring(st[0].length()).trim(),st);
	} catch ( Exception e ){
	    e.printStackTrace();
	}
    }
    public static String[] splitStr(String params) {
	String[] ret = {"",""};
	if ( params==null ) return ret;
	int spc = params.indexOf(" ");
	if ( spc==-1 ) { 
	    ret[0]=params;
	    return ret;
	}
	ret[0] = params.substring(0,spc);
	ret[1] = params.substring(spc+1);
	return ret;
    }


    public boolean enact(String cmd,  String params, String[] st) {

	params = params.trim();
	if ( cmd.equalsIgnoreCase(params) ) params="";
	System.out.println(cmd + ": '" + params + "'");


	if ( cmd.startsWith("mode") ) {
	    interp = params.toLowerCase();
	    printSimple("\n\nYour mode is now set to '" + interp +"'.\n\n");
	    return true;
	}
	if ( cmd.equals("prove") ) {
	    enactProve( params);
	    return true;
	}
	if ( cmd.equals("ask") ) {
	    enactQueryUser( params);
	    return true;
	}
	if ( cmd.equals("assert") ) {
	    enactAssert( params);
	    return true;
	}
	if ( cmd.equals("go") ) {
	    enactGo( params);
	    return true;
	}
	if ( cmd.equals("n") ) {
	    enactGo( "north");
	    return true;
	}
	if ( cmd.equals("s") ) {
	    enactGo( "south");
	    return true;
	}
	if ( cmd.equals("e") ) {
	    enactGo( "east");
	    return true;
	}
	if ( cmd.equals("w") ) {
	    enactGo( "west");
	    return true;
	}
	if ( cmd.equals("u") ) {
	    enactGo( "up");
	    return true;
	}
	if ( cmd.equals("d") ) {
	    enactGo( "down");
	    return true;
	}
	if ( cmd.equals("north") ) {
	    enactGo( "north");
	    return true;
	}
	if ( cmd.equals("south") ) {
	    enactGo( "south");
	    return true;
	}
	if ( cmd.equals("east") ) {
	    enactGo( "east");
	    return true;
	}
	if ( cmd.equals("west") ) {
	    enactGo( "west");
	    return true;
	}
	if ( cmd.equals("up") ) {
	    enactGo( "up");
	    return true;
	}
	if ( cmd.equals("down") ) {
	    enactGo( "down");
	    return true;
	}
	if ( cmd.startsWith("look") ) {
	    enactLook(params);
	    return true;
	}
	if ( cmd.startsWith("say") ) {
	    enactSay(params);
	    return true;
	}
	if ( cmd.startsWith("emote") || cmd.startsWith("*") || cmd.startsWith("/me") ) {
	    enactEmote(params);
	    return true;
	}

	if ( cmd.startsWith("tele") ) {
	    enactTeleport(params);
	    return true;
	}

	if ( cmd.startsWith("quit") ) {
	    client.end();
	}
	if ( cmd.startsWith("rinterp") ) {
	    cyc.reloadInterpretors();
	}
	if ( cmd.startsWith("load") ) {
	    try {
		cyc.constructMtFromFile(params,(CycConstant)userMt);
	    } catch ( Exception e ){
		println(""+e);
	    }
	    return true;
	}
	if ( cmd.startsWith("bsh") ) {
	    try {
		println(bshell.eval(params));
	    } catch ( Exception e ){
		println(""+e);
	    }
	    return true;
	}
	if ( cmd.startsWith("log") ) {
	    try {
		boolean log = new Boolean(params).booleanValue();
		//LogicMoo.setWriteToOut(log);
		println("logging is set " + log);
	    } catch ( Exception e ){
		println(""+e);
	    }
	    return true;
	}
	if ( cmd.startsWith("evlog") ) {
	    try {
		client.setWantingEvents( new Boolean(params).booleanValue());
		println("event is set " + this.isEventsOn);
	    } catch ( Exception e ){
		println(""+e);
	    }
	    return true;
	}
	if ( cmd.startsWith("vent") ) {
	    enactSendEvent(params);
	    return true;
	}
	if ( cmd.startsWith("listeners") ) {
	    printFormat(cyc.plugins);
	    printRaw("<br>");
	    return true;
	}
	if ( cmd.equals("hello") ) {
	    println( "hello " + userName);
	    return true;
	}
	if ( cmd.equals("time") ) {
	    println( "the time was " + (new Date()).toString());
	    return true;
	}

	if ( cmd.equals("echo") ) {
	    println( params);
	    return true;
	}
	if ( cmd.equals("reload") ) {
	    try {
		cyc.constructMtFromFile("JamudMt.kif",(CycConstant)userMt);
	    } catch ( Exception e ){
		println(""+e);
	    }
	    return true;
	}
	if ( cmd.equals("cyclify") ) {
	    println( cyc.toCycListString(params));
	    return true;
	}
	if ( cmd.equals("paraphrase") ) {
	    client.setParaphrased(true);
	    return true;
	}
	if ( cmd.equals("noparaphrase") ) {
	    client.setParaphrased(false);
	    return true;
	}
	if ( cmd.equals("subl") ) {
	    enactSubL(params);
	    return true;
	}
	if ( cmd.equals("query") ) {
	    enactQuery(params);
	    return true;
	}
	if ( cmd.equals("take")|| cmd.equals("get") ) {
	    enactTake(params);
	    return true;
	}
	if ( cmd.equals("drop") ) {
	    enactDrop(params);
	    return true;
	}
	if ( cmd.equals("isa") ) {
	    enactIsa(params);
	    return true;
	}
	if ( cmd.startsWith("inv") ) {
	    enactInventory();
	    return true;
	}
	if ( cmd.startsWith("prolog") ) {
	    enactProlog(params);
	    return true;
	}
	if ( cmd.startsWith("e2c") ) {
	    enactE2C(params);
	    return true;
	}
	if ( cmd.equals("mt") ) {
	    try {
		userMt = cyc.makeCycConstant(params);
		cyc.assertIsa(userMt,cyc.makeCycConstant("#$Microtheory"),cyc.baseKB);
	    } catch ( Exception e ) {
	    }
	    return true;
	}

	if ( cmd.startsWith(interp) ) return false;
	//enact(interp + " " + st[0] + " " + params);
	return false;

    }

    public void enactE2C(String params) {
	params = params.trim();
	try {
	    printRaw( Strings.change(Strings.change(Strings.change(cyc.e2c(params), "\\n"," "), "  "," "), "  "," ") + "\n");
	    printRaw( "\n");
	} catch ( Exception e ) {
	}
    }

    public void enactProlog(String params) {
	params = params.trim();
	try {
	    if ( params.endsWith(".") ) {
		printFormat( cyc.queryRawPrologServer(params));
	    } else {
		printFormat( cyc.queryRawPrologServer(params + "."));
	    }
	} catch ( Exception e ) {
	}
    }

    public boolean equals(Object A){
	//   System.out.println(""+A+" " + A.getClass());
	if ( A==null ) return false;
	if ( A instanceof ActorCommandParser ) return(A==this);
	if ( A instanceof String )
	    if ( (((String)A).equals(userName.toString())) || (((String)A).equals(userLocation.toString())) )
		return true;
	if ( A instanceof CycFort )
	    if ( (((CycFort)A).equals(userName)) || (((CycFort)A).equals(userLocation)) )
		return true;
	return false;
    }


    public void enactSendEvent(String params) {
	String[] split = splitStr(params);
	println("result = " + cyc.emoteTo(split[0],split[1]));
    }


    public void enactGo(String params) {
	Iterator answers = cyc.queryMt2("(#$containsPortals  " + userLocation.stringApiValue() + " (#$PortalFromToFn " + userLocation.stringApiValue() + " ?A ?B))").iterator();
	if ( !answers.hasNext() ) {
	    println("There are no exits from here!");
	    return;
	}
	while ( answers.hasNext() ) {
	    CycList ans = (CycList)answers.next();
	    String text = (String) ans.first().toString();
	    // System.out.println(text);
	    if ( text.toLowerCase().startsWith(params.toLowerCase()) ) {
		moveTo(userName,(CycFort) ans.second());
		userLocation = (CycFort)ans.second();
		enactLook("");
		return;
	    }
	}

	println("Go where?");

    }


    public void enactCmdLoad(String params) {
	//Class.getClassLoader().getSystemClassLoader().loadClass().
    }

    public void enactSay(String params) {
	String mesg = "\""+params+"\"";
	println("You say, " + mesg);
	cyc.emoteAll(this,userName.toString() + " says, " + mesg+"<br>");
	//Class.getClassLoader().getSystemClassLoader().loadClass().
    }

    public void enactEmote(String params) {
	String mesg = "\""+params+"\"";
	println("You " + mesg);
	cyc.emoteAll(this,userName.toString() + " " + mesg+"<br>");
	//Class.getClassLoader().getSystemClassLoader().loadClass().
    }


    public void enactIsa(String params) {
	CycFort cycfort = selfLocatFort(params,"Collection");
	if ( cycfort==null ) {
	    println("Cannot find collection " + params);
	} else  {
	    try {
		println(cyc.getAllInstances(cycfort));
	    } catch ( Exception e ) {
	    }
	}
	return;
    }

    public Object queryMt1First(String query) {
	return(((CycList)cyc.queryMt(query).get(0)).get(0));
    }

    public void moveTo(CycFort cycobj,CycFort  cycwhere) {
	cyc.assertSlotValue(userMt,cycobj,"in-ContGeneric",cycwhere,"GeographicalRegion",true);
    }

    public bsh.Interpreter getBeanShell() {
	return bshell;
    }
    public CycFort getUserName() {
	return userName;
    }
    public void setUserName(CycFort name) {
	userName = name;
    }
    public CycFort getUserLocation() {
	return userLocation;
    }
    public void setUserLocation(CycFort loc) {
	userLocation = loc;
    }
    public CycFort getUserMt() {
	return userMt;
    }
    public void setUserMt(CycFort mt) {
	userMt = mt;
    }
    public String getInterp() {
	return interp;
    }
    public void setInterp(String in) {
	interp = in;
    }

    public CycFort selfLocatFort(Object target, String type) { 
	if ( target instanceof String ) {
	    String targetstr = (String)target.toString().toLowerCase();
	    if ( targetstr.equals("here") ) return userLocation;
	    if ( targetstr.equals("me") ) return userName;
	    if ( targetstr.equals("room") ) return userLocation;
	    if ( targetstr.equals("east") || targetstr.equals("e") ) return dirAreaOf(userLocation,"East");
	    if ( targetstr.equals("west") || targetstr.equals("w") ) return dirAreaOf(userLocation,"West");
	    if ( targetstr.equals("up") || targetstr.equals("u") ) return dirAreaOf(userLocation,"Up");
	    if ( targetstr.equals("down") || targetstr.equals("d") ) return dirAreaOf(userLocation,"Down");
	    if ( targetstr.equals("north") || targetstr.equals("n") ) return dirAreaOf(userLocation,"North");
	    if ( targetstr.equals("south") || targetstr.equals("s") ) return dirAreaOf(userLocation,"South");
	}
	CycFort found = cyc.locateCycFort(target,type);
	return found;
    }


    public CycFort dirAreaOf(CycFort where, String params) {
	Iterator answers = cyc.queryMt2("(#$containsPortals  " + where.stringApiValue() + " (#$PortalFromToFn " + where.stringApiValue() + " ?A ?B))").iterator();
	if ( !answers.hasNext() ) {
	    println("There are no exits from here!");
	    return null;
	}
	while ( answers.hasNext() ) {
	    CycList ans = (CycList)answers.next();
	    String text = (String) ans.first().toString();
	    // System.out.println(text);
	    if ( text.toLowerCase().startsWith(params.toLowerCase()) ) {
		return(new CycNart(cyc.toCycList("(#$PortalFromToFn " + where.stringApiValue() + " " +  ((CycFort)ans.first()).cyclify() +  " " +((CycFort)ans.second()).cyclify() + " )")));
	    }
	}
	return null;
    }

    public void enactInventory() {
	ArrayList answers = cyc.queryArrayListSEL(userMt.toString(),"(#$in-ContGeneric ?SEL " + " " + userName.cyclify() + " )");
	int num = answers.size();
	if ( num==0 ) {
	    printRaw("You do not have any items<br>");
	    return;
	}
	if ( num>1 ) printRaw(""+num+" items.<br>");
	printFormat(answers,"<br>");
	printRaw("<br>");
    }

    public void enactLook(String params) {

	if ( params.length()==0 ) {
	   //printRaw("newer ven look versions<br>");
	    giveRoomLook();
	    return;
	}

	CycFort cycobject = selfLocatFort(params,"PartiallyTangible");

	if ( cycobject==null ) {
	    //println("Unable to locate " + params);
	    return;
	}

	giveLookDescription(cycobject,params);

    }
    public void giveRoomLook() {
	giveLookDescription(userLocation, "here");
	printFormat(cyc.queryArrayListSEL("EverythingPSC","(#$mudObjectVisibleToWhom " + userName.cyclify() + "  ?SEL)"));
	printRaw("\n\n");
    }

    public void giveLookDescription(CycFort cycobject, String name) {
	ArrayList al = null;
	printRaw("<br>");
	if ( cycobject==null ) {
	    printRaw("I cannot locate " + name + "<br>");
	}
	al = cyc.queryArrayListSEL("EverythingPSC","(#$mudDescription " + cycobject.cyclify() + " ?SEL)");
	if ( al.size()>0 ) {
	    printFormat(al);
	    printRaw("<br>");
	    return;
	}
	al = cyc.queryArrayListSEL("EverythingPSC","(#$mudAreaView " + cycobject.cyclify() + " ?SEL)");
	if ( al.size()>0 ) {
	    printFormat(al);
	    printRaw("<br>");
	    return;
	}
	printSimple("Nothing appearent about " + cyc.attemptParaphrase(cycobject) + " '"+cycobject.cyclify()+"'<br>");
    }



    public void enactTeleport(String params) {
	CycFort fort = selfLocatFort(params,"GeographicalRegion");
	if ( fort==null ) {
	    println("Teleport to where?");
	} else {
	    doTeleport(userName,fort,params);
	}
    }
    
    public void enactTake(String params) {
	CycFort fort = selfLocatFort(params,"PartiallyTangible");
	if ( fort==null ) {
	    println("Take what?");
	} else {
	    moveTo(fort,userName);
	}
    }

    public void enactDrop(String params) {
	CycFort fort = selfLocatFort(params,"PartiallyTangible");
	if ( fort==null ) {
	    println("Drop what?");
	} else {
	    moveTo(fort,userLocation);
	}
    }


    public void doTeleport(CycFort what, CycFort where, String params) {     
	if ( where!=null ) {
	    try {
		if (what.equals(userName)) {
		    println("you are now in " + where); 
		    moveTo(what,where);
		    userLocation = where;
		} else {
		    boolean doit = cyc.emoteTo(what,"\nYou have been teleported to " + where + " \n\n");
		    if ( doit ) moveTo(what,where);
		}
	    
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	}
    }

    /**
    * Process a SubL command 
     * @param who String nickname of the user who sentence the message
     * @param message String the command
    
    params are in the form
    <my nick> :<message> 
    or
    <my nick> <message>	
    
     */
    public void enactSubL( String subl) {
	try {
	    printFormat(cyc.converseObject(subl));
	} catch ( Exception e ) {
	    println( ""+ e);
	}
    }

    /**
    * Process a Query command 
     * @param who String nickname of the user who sentence the message
     * @param message String the command

    params are in the form
    <my nick> :<message> 
    or
    <my nick> <message>	

     */
    public void enactQuery( String query) {
	try {
	    println("");
	    printFormat( cyc.converseObject( "(cyc-query '" +cyc.toCycListString(query) + " #$InferencePSC)"));
	    println("");
	} catch ( Exception e ) {
	    println( ""+ e);
	}
    }
    /**
    * Process a Prove command (Query with proof)
     * @param who String nickname of the user who sentence the message
     * @param message String the command

    params are in the form
    <my nick> :<message> 
    or
    <my nick> <message>	

     */
    public void enactProve( String query) {
	try {
	    printFormat( cyc.converseObject( "(fi-prove '" +cyc.toCycListString(query) + " #$InferencePSC)"));
	} catch ( Exception e ) {
	    println( ""+ e);
	}
    }

    /**
    * Process an Ask command 
     * @param who String nickname of the user who sentence the message
     * @param message String the command

    params are in the form
    <my nick> :<message> 
    or
    <my nick> <message>	

     */

    public void enactQueryUser( String query) {
	try {
	    println("");
	    printFormat( cyc.converseObject( "(cyc-query '" +cyc.toCycListString(query) + " " + userMt.stringApiValue() + ")"),"<br>");
	    println("");
	} catch ( Exception e ) {
	    println( ""+ e);
	}
    }

    /**
    * Process an Assert command 
     * @param who String nickname of the user who sentence the message
     * @param message String the command

    params are in the form
    assert:<message> 
    or
    <my nick>  <message>	

     */

    public void enactAssert(String sentence) {
	println("");
	enactSubL("(cyc-assert '" +cyc.toCycListString(sentence) + " " + userMt.stringApiValue() + ")");
	println("");
    }


}


