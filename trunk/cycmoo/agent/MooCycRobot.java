package cycmoo.agent;

import cycmoo.*;
import cycmoo.api.*;
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

import ViolinStrings.Strings;


public class MooCycRobot extends MooAgentThread {

    CycFort cycme = null;

    static public Random rnd = new Random();
    public MooCycRobot(CycFort aself){
	super();
	 cycme = aself;
	 try {
	     System.out.println("Waking up " + aself);
	     cyc = new LogicMooCycAccess();   
	     reloadInterpretor(cycme);

	 } catch (Exception e) {
	     e.printStackTrace();
	 }
    }
    
    public void run(){
	isRunning = true;
	while (this.isRunning && !this.interrupted()) {
	    try {
		Thread.sleep(5000);
	      //  System.out.println("" + cycme + " is thinking...");
	   
		/*
	         ArrayList mycommand = cyc.queryArrayListSEL(me.getUserMt().toString(),"(#$nextNpcCommand " + cycme.cyclify() + " ?SEL )");
		 if (mycommand.size()>0 && false) {
		     Object cmd = mycommand.get(0);
		     System.out.println("" + cycme + " decided " + cmd + " ("+mycommand.size()+")");
		     enact(cmd);
		 } else {
	      //       System.out.println("" + cycme + " does nothing.");
		 }
		 */
		Thread.sleep(60000);
		doRandomly();
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

    public boolean receiveEvent(LogicMooEvent event) {
	return true;
    }

    public void doRandomly() {
	if (rnd.nextInt(4)==1) {
	   // moveRandomly();
	}
    }

    static public String dirrections = "nsewud";
    public void moveRandomly(){
	String rnddir = ""+dirrections.charAt(rnd.nextInt(6));
      //  System.out.println("" + cycme + " randomly tries go: " + rnddir);
	enact(me.getUserName(),me.getUserName(), "go " + rnddir);

    }


}

