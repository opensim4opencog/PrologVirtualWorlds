package cycmoo.cmd;

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


// BeanShell
import bsh.*;
import bsh.util.*;

// OpenCyc
import org.opencyc.api.*;
import org.opencyc.cycobject.*;
import org.opencyc.constraintsolver.*;
import org.opencyc.inferencesupport.*;

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

    public void setParaphrased(boolean onoff) {
        client.setParaphrased(onoff);
    }
    public Object prompt(Object message) {
        return client.prompt(message);
    }
    public boolean receiveEvent(LogicMooEvent event) {
        return client.receiveEvent(event);
    }
    public void reloadInterpretor() {
        client.reloadInterpretor();
    }
    public boolean running() {
        return client.running();
    }
    public boolean isWantingEvents() {
        return client.isWantingEvents();
    }
    public void setWantingEvents(boolean onoff) {
        client.setWantingEvents(onoff);
    }

    public IActorCommandParser getActor() {
        return this;
    }
    public boolean println(Object message) {
        return client.println(message);
    }
    public boolean printRaw(String message) {
        return client.printRaw(message);
    }
    public boolean printSimple(Object message) {
        return client.printSimple(message);
    }
    public boolean printFormat(Object results) {
        return client.printFormat(results);
    }
    public boolean printFormat(Object results,Object format) {
        return client.printFormat(results,format);
    }
    public void run() {
        if( !client.running() ) client.run();
    }
    public void end() {
        if( client.running() ) client.end();
    }

    public ActorCommandParser() {
    }

    public ActorCommandParser(LogicMooCycAccess cm, CycFort user,CycFort loc,CycFort mt, IMooClient mc) {
        setData(cm,user,loc,mt,mc);
    }

    public void setData(LogicMooCycAccess cm, CycFort user,CycFort loc,CycFort mt, IMooClient mc) {
        cyc = cm;
        client = mc;
        userName = user;
        if( mt==null ) mt =  cyc.makeCycConstant("JamudMt"    /*TODO*/);
        ArrayList finder = cyc.queryArrayListSEL(mt.toString(),"(#$in-ContGeneric " + userName.cyclify() + " ?SEL)");
        if( finder.size()>0 ) {
            loc = (CycFort) finder.get(0);
        } else {
            if( loc==null ) loc = cyc.makeCycConstant("Area1002");
        }

        userLocation = loc;
        userMt = mt;
        try {
            cyc.assertGaf(userMt,cyc.isa,userName,cyc.makeCycConstant("PlayerInstance"));
            bshell = new bsh.Interpreter();
            bshell.set("actor",this);
            bshell.set("client",mc);
            bshell.set("cyc",cm);
        } catch( Exception e ) {
    	    System.out.println("e: " + e);
            //e.printStackTrace();
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

    public boolean enact(Object location,Object user,Object event) {
        String params = ""+event;
        String cmd = ""; 
        String[] st = params.split(" ");
        int connect = 0;
        int colon = params.indexOf(":");
        int space = params.indexOf(" ");
        int comma = -1 ;// params.indexOf(",");
        if(colon>connect) connect=colon;
        if(space>connect) connect=space;
        if(comma>connect) connect=comma;
        if(connect==0) {
            cmd = params.toLowerCase();
        } else {
            cmd = params.substring(0,connect).toLowerCase();
            params = params.substring(connect+1);
        }
        
        cmd = cmd.trim();
        params = params.trim();
        if( cmd.equalsIgnoreCase(params) ) params="";
        System.out.println(cmd + ": '" + params + "'");


        if( cmd.startsWith("mode") ) {
            interp = params.toLowerCase();
            printSimple("\n\nYour mode is now set to '" + interp +"'.\n\n");
            return true;
        }
        if( cmd.equals("prove") ) {
            enactProve( params);
            return true;
        }
        if( cmd.equals("ask") ) {
            enactQueryUser( params);
            return true;
        }
        if( cmd.equals("assert") ) {
            enactAssert( params);
            return true;
        }
        if( cmd.startsWith("(") ) {
            //  enactQuery(st[0]+" "+params);
            return true;
        }
        if( cmd.startsWith("rinterp") ) {
            cyc.reloadInterpretors();
        }
        if( cmd.startsWith("load") ) {
            try {
                cyc.constructMtFromFile(params,(CycConstant)userMt);
            } catch( Exception e ) {
                println(""+e);
            }
            return true;
        }
        if( cmd.startsWith("bsh") ) {
            try {
                println(bshell.eval(params));
            } catch( Exception e ) {
                println(""+e);
            }
            return true;
        }
        if( cmd.startsWith("evlog") ) {
            try {
                client.setWantingEvents( new Boolean(params).booleanValue());
                println("event is set " + isWantingEvents());
            } catch( Exception e ) {
                println(""+e);
            }
            return true;
        }
        if( cmd.startsWith("vent") ) {
            enactSendEvent(params);
            return true;
        }
        if( cmd.startsWith("listeners") ) {
            printFormat(cyc.plugins);
            printRaw("<br>");
            return true;
        }
        if( cmd.equals("time") && st.length==1 ) {
            println( "the time was " + (new Date()).toString());
            return true;
        }

        if( cmd.equals("echo") ) {
            println( params);
            return true;
        }
        if( cmd.equals("reload") && st.length==1 ) {
            try {
                cyc.constructMtFromFile("JamudMt.kif",(CycConstant)userMt);
            } catch( Exception e ) {
                println(""+e);
            }
            return true;
        }
        if( cmd.equals("cyclify") ) {
            println( cyc.toCycListString(params));
            return true;
        }
        if( cmd.equals("paraphrase") ) {
            client.setParaphrased(true);
            return true;
        }
        if( cmd.equals("noparaphrase") ) {
            client.setParaphrased(false);
            return true;
        }
        if( cmd.equals("subl") ) {
            enactSubL(params);
            return true;
        }
        if( cmd.equals("subp") ) {
            enactSubP(params);
            return true;
        }
        if( cmd.equals("query") ) {
            enactQuery(params);
            return true;
        }
        if( cmd.equals("isa") ) {
            enactIsa(params);
            return true;
        }
        if( cmd.startsWith("prolog") ) {
            enactProlog(params);
            return true;
        }
        if( cmd.startsWith("e2c") ) {
            enactE2C(params);
            return true;
        }
        if( cmd.equals("mt") ) {
            try {
                userMt = cyc.makeCycConstant(params);
                cyc.assertIsa(userMt,cyc.makeCycConstant("#$Microtheory"),cyc.baseKB);
            } catch( Exception e ) {
            }
            return true;
        }

//       if ( enact2( location, user, event,  cmd,  params) ) return true;

        //if( cmd.startsWith(interp) ) 
            return enactPrologChat(location,user, event.toString().trim());
        //else
          //  return enact(location,user, interp + " " + st[0] + " " + params);
    }


    public boolean enact2(Object location,Object user,Object event, String cmd, String params) {

    if( cmd.startsWith("log") ) {
        try {
            boolean log = new Boolean(params).booleanValue();
            //LogicMoo.setWriteToOut(log);
            println("logging is set " + log);
        } catch( Exception e ) {
            println(""+e);
        }
        return true;
    }

    if( cmd.equals("take")|| cmd.equals("get") ) {
        enactTake(params);
        return true;
    }
    if( cmd.startsWith("inv") ) {
        enactInventory();
        return true;
    }

    if( cmd.equals("go") ) {
        enactGo( params);
        return true;
    }
    if( cmd.equals("n") ) {
        enactGo( "north");
        return true;
    }
    if( cmd.equals("s") ) {
        enactGo( "south");
        return true;
    }
    if( cmd.equals("e") ) {
        enactGo( "east");
        return true;
    }
    if( cmd.equals("w") ) {
        enactGo( "west");
        return true;
    }
    if( cmd.equals("u") ) {
        enactGo( "up");
        return true;
    }
    if( cmd.equals("d") ) {
        enactGo( "down");
        return true;
    }
    if( cmd.equals("north") ) {
        enactGo( "north");
        return true;
    }
    if( cmd.equals("south") ) {
        enactGo( "south");
        return true;
    }
    if( cmd.equals("east") ) {
        enactGo( "east");
        return true;
    }
    if( cmd.equals("west") ) {
        enactGo( "west");
        return true;
    }
    if( cmd.equals("up") ) {
        enactGo( "up");
        return true;
    }
    if( cmd.equals("down") ) {
        enactGo( "down");
        return true;
    }
    if( cmd.startsWith("look") ) {
        enactLook(params);
        return true;
    }
    if( cmd.startsWith("say") ) {
        enactSay(params);
        return true;
    }
    if( cmd.startsWith("emote") || cmd.startsWith("*") || cmd.startsWith("/me") ) {
        enactEmote(params);
        return true;
    }
    if( cmd.startsWith("tele") ) {
        enactTeleport(params);
        return true;
    }
    if( cmd.startsWith("quit") ) {
        client.end();
    }
    if( cmd.equals("drop") ) {
        enactDrop(params);
        return true;
    }

    return false;
    }


    public void enactE2C(String params) {
        params = params.trim();
        try {
            printRaw( Strings.change(Strings.change(Strings.change(cyc.e2c(params), "\\n"," "), "  "," "), "  "," ") + "\n");
            printRaw( "\n");
        } catch( Exception e ) {
        }
    }

    public void enactProlog(String params) {
        params = params.trim();
        try {
            if( params.endsWith(".") ) {
                printFormat( cyc.queryRawPrologServer(params));
            } else {
                printFormat( cyc.queryRawPrologServer(params + "."));
            }
        } catch( Exception e ) {
        }
    }

    public static String normalizeP(String text) {
        return Strings.change(Strings.change(text,"\\","\\\\"),"\"","\\\"");
    }

    public void enactSubP( String subp) {
        enactProlog("serviceCycApiRequestSubP(\"" + normalizeP(subp) + "\").");
    }

    public void enactCycLBot( String subp) {
        enactProlog("enactCycLBot(\"" + normalizeP(subp) + "\").");
    }

    public boolean enactPrologChat(Object location, Object user, Object speech) {
        enactProlog("from(\"" + location + "\",\"" + user + "\",say(\"" + normalizeP(""+speech) + "\")).");
        return true;
    }

    public boolean equals(Object A) {
        //   System.out.println(""+A+" " + A.getClass());
        if( A==null ) return false;
        if( A instanceof ActorCommandParser ) return(A==this);
        if( A instanceof String )
            if( (((String)A).equals(userName.toString())) || (((String)A).equals(userLocation.toString())) )
                return true;
        if( A instanceof CycFort )
            if( (((CycFort)A).equals(userName)) || (((CycFort)A).equals(userLocation)) )
                return true;
        return false;
    }


    public void enactSendEvent(String params) {
        //  println("result = " + LogicMooEvent.informLocation(this,userLocation,userName.toString() + "  " + mesg+"<br>");
        // println("result = " + cyc.emoteToLocation(split[0],new LogicMooEvent(this,split[1])));
    }


    public void enactGo(String params) {
        Iterator answers = cyc.queryMt2("(#$containsPortals  " + userLocation.stringApiValue() + " (#$PortalFromToFn " + userLocation.stringApiValue() + " ?A ?B))").iterator();
        if( !answers.hasNext() ) {
            println("There are no exits from here!");
            return;
        }
        while( answers.hasNext() ) {
            CycList ans = (CycList)answers.next();
            String text = (String) ans.first().toString();
            // System.out.println(text);
            if( text.toLowerCase().startsWith(params.toLowerCase()) ) {
                LogicMooEvent.informLocation(this,userLocation,userName + " leaves to the " + text + "<br>");
                moveTo(userName,(CycFort) ans.second());
                userLocation = (CycFort)ans.second();
                LogicMooEvent.informLocation(this,userLocation,userName + " enters.<br>");
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
        enactEmote("said: "+params); 
    }

    public void enactEmote(String params) {
        String mesg = params;
        println("You " + mesg);
        LogicMooEvent.informLocation(this,userLocation,userName.toString() + " " + mesg);
    }


    public void enactIsa(String params) {
        CycFort cycfort = selfLocatFort(params,"Collection");
        if( cycfort==null ) {
            println("Cannot find collection " + params);
        } else {
            try {
                println(cyc.getAllInstances(cycfort));
            } catch( Exception e ) {
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
        if( target instanceof String ) {
            String targetstr = (String)target.toString().toLowerCase();
            if( targetstr.equals("here") ) return userLocation;
            if( targetstr.equals("me") ) return userName;
            if( targetstr.equals("room") ) return userLocation;
            if( targetstr.equals("east") || targetstr.equals("e") ) return dirAreaOf(userLocation,"East");
            if( targetstr.equals("west") || targetstr.equals("w") ) return dirAreaOf(userLocation,"West");
            if( targetstr.equals("up") || targetstr.equals("u") ) return dirAreaOf(userLocation,"Up");
            if( targetstr.equals("down") || targetstr.equals("d") ) return dirAreaOf(userLocation,"Down");
            if( targetstr.equals("north") || targetstr.equals("n") ) return dirAreaOf(userLocation,"North");
            if( targetstr.equals("south") || targetstr.equals("s") ) return dirAreaOf(userLocation,"South");
        }
        CycFort found = cyc.locateCycFort(target,type);
        return found;
    }


    public CycFort dirAreaOf(CycFort where, String params) {
        Iterator answers = cyc.queryMt2("(#$containsPortals  " + where.stringApiValue() + " (#$PortalFromToFn " + where.stringApiValue() + " ?A ?B))").iterator();
        if( !answers.hasNext() ) {
            println("There are no exits from here!");
            return null;
        }
        while( answers.hasNext() ) {
            CycList ans = (CycList)answers.next();
            String text = (String) ans.first().toString();
            // System.out.println(text);
            if( text.toLowerCase().startsWith(params.toLowerCase()) ) {
                return(new CycNart(cyc.toCycList("(#$PortalFromToFn " + where.stringApiValue() + " " +  ((CycFort)ans.first()).cyclify() +  " " +((CycFort)ans.second()).cyclify() + " )")));
            }
        }
        return null;
    }

    public void enactInventory() {
        ArrayList answers = cyc.queryArrayListSEL(userMt.toString(),"(#$in-ContGeneric ?SEL " + " " + userName.cyclify() + " )");
        int maxAnswers = answers.size();
        if( maxAnswers==0 ) {
            printRaw("You do not have any items<br>");
            return;
        }
        if( maxAnswers>1 ) printRaw(""+maxAnswers+" items.<br>");
        printFormat(answers,"<br>");
        printRaw("<br>");
    }

    public void enactLook(String params) {

        if( params.length()==0 ) {
            //printRaw("newer ven look versions<br>");
            giveRoomLook();
            return;
        }

        CycFort cycobject = selfLocatFort(params,"PartiallyTangible");

        if( cycobject==null ) {
            //println("Unable to locate " + params);
            return;
        }

        giveLookDescription(cycobject,params);
    }         

    //"(nearestIsa DouglasMilesOffice ?W)"



    public void giveRoomLook() {
        giveLookDescription(userLocation, "here");
        // printFormat(cyc.queryArrayListSEL("EverythingPSC","(#$mudObjectVisibleToWhom " + userName.cyclify() + "  ?SEL)"),"<br>");
        // printRaw("\n\n");
    }

    public void giveLookDescription(CycFort cycobject, String name) {
        ArrayList al = null;
        printRaw("<br>");
        if( cycobject==null ) {
            printRaw("I cannot locate " + name + "<br>");
        }
        try {
            //printFormat(queryMatch(cyc.makeCycList("(#$isa "+cycobject.cyclify() + " ?X)"),getUserMt(),1,3)," and ");
            //printRaw("<br>");
            //printFormat(queryMatch(cyc.makeCycList("(#$hasAttributes "+cycobject.cyclify() + " ?X)"),getUserMt(),1,2)," and ");
            //printRaw("<br>");
            //printFormat(queryMatch(cyc.makeCycList("(#$salientAssertions "+cycobject.cyclify() + " ?X)"),getUserMt(),2,20)," and ");
            //Iterator ans =  cyc.converseList("(cyc-query '(#$mudVisibleTermSentences "+cycobject.cyclify() + " ?SEL) " + getUserMt().cyclify() + " '(:backchain 1 :answer 10 :maxSeconds 5))").iterator();
            Iterator ans =  cyc.converseList("(cyc-query '(#$mudDescriptiveTermSentences "+cycobject.cyclify() + " ?SEL) " + getUserMt().cyclify() + " '(:backchain 6 :answer 10 :maxSeconds 5))").iterator();
            //      Iterator ans =  cyc.converseList("(cyc-query '(#$assertedTermSentences "+cycobject.cyclify() + " ?SEL) " + getUserMt().cyclify() + " '(:backchain 1 :answer 10 :maxSeconds 5))").iterator();
            while( ans.hasNext() ) {
                Object an = ans.next();
                if( an instanceof CycList ) {
                    CycList cl = ((CycList)((CycList)an).first());
                    //              printFormat(cl.first().toString());
                    printFormat(cl.rest());
                } else {
                    printFormat(an);
                }
                printRaw("<br>");
            }
            //  printRaw("<br>");
        } catch( Exception e ) {
            e.printStackTrace();
        }

        /*  al = cyc.queryArrayListSEL("EverythingPSC","(#$mudDescription " + cycobject.cyclify() + " ?SEL)");
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
          */
    }



    public void enactTeleport(String params) {
        CycFort fort = selfLocatFort(params,"GeographicalRegion");
        if( fort==null ) {
            println("Teleport to where?");
        } else {
            doTeleport(userName,fort,params);
        }
    }

    public void enactTake(String params) {
        CycFort fort = selfLocatFort(params,"PartiallyTangible");
        if( fort==null ) {
            println("Take what?");
        } else {
            moveTo(fort,userName);
        }
    }

    public void enactDrop(String params) {
        CycFort fort = selfLocatFort(params,"PartiallyTangible");
        if( fort==null ) {
            println("Drop what?");
        } else {
            moveTo(fort,userLocation);
        }
    }


    public void doTeleport(CycFort what, CycFort where, String params) {
        if( where!=null ) {
            try {
                if( what.equals(userName) ) {
                    println("you are now in " + where); 
                    moveTo(what,where);
                    userLocation = where;
                } else {
                    boolean doit =  LogicMooEvent.informActor(this,what,"\nYou have been teleported to " + where + " \n\n");
                    if( doit ) moveTo(what,where);
                }

            } catch( Exception e ) {
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
            printRaw("<br>");
        } catch( Exception e ) {
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
        } catch( Exception e ) {
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
        } catch( Exception e ) {
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
        } catch( Exception e ) {
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


