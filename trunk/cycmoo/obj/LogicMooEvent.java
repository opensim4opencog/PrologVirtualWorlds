package cycmoo.obj;

import cycmoo.*;
import cycmoo.agent.*;
import cycmoo.api.*;
import cycmoo.obj.*;

import java.lang.reflect.*;
import java.lang.*;
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
import org.opencyc.inferencesupport.*;
import org.opencyc.constraintsolver.*;
import org.apache.oro.util.*;

public class LogicMooEvent extends QueryLiteral {
    private CycFort source = null;
    private CycFort location = null;

    // send Object
    public boolean sendObject(Object exceptfor, Object target) {
	Hashtable allListeners = getListeners();

	IMooClient targ = (IMooClient)allListeners.get(target); 

	if ( targ!=null ) return targ.receiveEvent(this);
	Iterator all = allListeners.keySet().iterator();
	boolean ret = true;
	while ( all.hasNext() ) {
	    targ = (IMooClient)allListeners.get(all.next()); 
	    if ( targ.equals(target) && !(targ.equals(exceptfor)) ) 
		try {
		if ( !targ.receiveEvent(this) ) ret = false;
	    } catch ( Exception e) {
		e.printStackTrace();
	    }
	}
	return ret;
    }

    // send All
    public boolean sendAll() {
	Hashtable allListeners = this.getListeners();
	Iterator all = allListeners.keySet().iterator();
	boolean ret = true;
	while ( all.hasNext() ) {
	    IMooClient targ = (IMooClient)allListeners.get(all.next()); 
	    if ( targ!=null ) if ( !targ.equals(this.source) ) if ( !targ.receiveEvent(this) ) ret = false;
	}
	return ret;
    }

    // send Source
    public boolean sendSource() {
	return (((IMooClient)getListeners().get(source)).receiveEvent(this));
    }

    /*
	public boolean emoteQuery(LogicMooCycAccess cyc, String mt,String query) {
	Hashtable allListeners = message.getListeners();
	Iterator all = cyc.queryArrayListSEL(mt,query).iterator();
	boolean ret = true;
	while ( all.hasNext() ) {
	    IMooClient targ = (IMooClient)allListeners.get(all.next()); 
	    if ( !emoteToAgent(targ,this) ) ret = false;
	}
	return ret;
	}
    */  



    /*
    
    Constructors
    
    */

    public LogicMooEvent(CycFort source,String formula){
	super(initStringMsg(formula));
	this.source = source;
    }

    public LogicMooEvent(CycFort source, CycFort location, String formula){
	super(initStringMsg(formula));
	this.source = source;
	this.location = location;
    }

    public LogicMooEvent(IMooClient me, String formula){
	super(initStringMsg(formula));
	this.source = me.getActor().getUserName();
	this.location = me.getActor().getUserLocation();
    }

    public LogicMooEvent(CycFort source,CycList formula){
	super(formula);
	this.source = source;
    }

    public LogicMooEvent(CycFort source, CycFort location, CycList formula){
	super(formula);
	this.source = source;
	this.location = location;
    }

    public LogicMooEvent(IMooClient me, CycList formula){
	super(formula);
	this.source = me.getActor().getUserName();
	this.location = me.getActor().getUserLocation();
    }



    static public CycList initStringMsg(String formula) {
	CycList cyclist = new CycList();
	cyclist.add(formula);
	return cyclist;
    }

    public boolean send(){
	return true;
    }

    public String toString(){
	if (formula==null) return "";
	if(formula.size()==1) {
	    return ""+formula.first();
	}
	return formula.toPrettyString(" ");
    }

    static public LogicMooEvent makeEvent(IMooClient mc, String info){
	return new LogicMooEvent(mc,info);
    }


    static public Hashtable getListeners() {
	return LogicMooCycAccess.plugins;
    }




    // Inform parses Text to make events and Send
    // Send informs the agents


    // Inform Location
    static public boolean informLocation(IMooClient exceptfor, CycFort target, String message) {
	return eventLocation(exceptfor,target,makeEvent(exceptfor,message));
    }

    // Inform Actor
    static public boolean informActor(IMooClient exceptfor, CycFort target, String message) {
	return eventActor(exceptfor,target,makeEvent(exceptfor,message));
    }

    // Inform All
    static public boolean informAll(IMooClient exceptfor, String message) {
	return eventAll(exceptfor,makeEvent(exceptfor,message));
    }




    // event Location
    static public boolean eventLocation(IMooClient exceptfor, CycFort target, LogicMooEvent ev) {
	return ev.sendObject(exceptfor,target);
    }

    // event Actor
    static public boolean eventActor(IMooClient exceptfor, CycFort target, LogicMooEvent ev) {
	return ev.sendObject(exceptfor,target);
    }

    // event All
    static public boolean eventAll(IMooClient exceptfor, LogicMooEvent ev) {
	return ev.sendAll();
    }


}