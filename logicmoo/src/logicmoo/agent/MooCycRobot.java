package logicmoo.agent;

import logicmoo.*;
import logicmoo.api.*;

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

import ViolinStrings.Strings;


public class MooCycRobot extends MooAgentThread {

    CycFort cycme = null;

    public MooCycRobot(CycFort aself){
	 cycme = aself;
	 try {
	     cyc = new LogicMooCycAccess();   
	     reloadInterpretor(cycme);
	 } catch (Exception e) {
	     e.printStackTrace();
	 }
    }
    
    public void run(){
	while (this.isRunning) {
	    try {
		Thread.sleep(1000);
		 ArrayList mycommand = cyc.queryArrayListSEL(me.getUserMt().toString(),"(#$nextCommand " + cycme.cyclify() + " ?SEL )");
		 if (mycommand.size()>0) {
		    enact(mycommand.get(0));
		 }
	    } catch (Exception e) {
		System.out.println("MooCycRobot " + cycme + " had a problem!");
		e.printStackTrace();
	    }
	}
    }

    public boolean printRaw(String message) {
	System.out.println(cycme + " hears:" + message);
	return true;
    }
    
    public Object prompt(Object message) {
	System.out.println(cycme + " prompt:" + message);
	return "";
    }

    public boolean receiveEvent(Object event) {
	return true;
    }


}

