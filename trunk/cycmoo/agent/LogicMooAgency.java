//<pre>
package logicmoo.agent;

import logicmoo.api.*;
import logicmoo.agent.*;
import logicmoo.util.*;
import logicmoo.obj.*;


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
import org.opencyc.chat.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;


import ViolinStrings.Strings;


public class LogicMooAgency extends Thread {
    
    public static CycSymbol SYMBOL_NIL = new CycSymbol("NIL");
    private static LogicMooCycAccess cyc = null;
    private static Hashtable agents = null; 

    public LogicMooAgency(LogicMooCycAccess aCyc) {
	cyc = aCyc;
	if (agents==null) {
	    agents = new Hashtable();
	    enumerateNPCs();
	}
    }

    public static void enumerateNPCs() {
	Iterator supposed = cyc.queryArrayListSEL("EverythingPSC","(#$isa ?SEL #$IndividualAgent)").iterator();
	while (supposed.hasNext()) {
	    confirmNPC((CycFort)supposed.next());
	}
    }
    public static void confirmNPC(CycFort agent) {
	    MooCycRobot robot = (MooCycRobot)agents.get(agent);
	    if (robot==null) {
		robot = new MooCycRobot(agent);
		agents.put(agent,robot);
		robot.start();
	    }
    }

    public static Hashtable getAgents() {
	return agents;
    }

    public void run(){
	while (!interrupted()) {
	    try {
		Thread.sleep(60000);
	    } catch (Exception e) {
	    }
	    enumerateNPCs();
	}
    }
}


