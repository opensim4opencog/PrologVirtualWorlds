//<pre>
package cycmoo.agent;

import cycmoo.api.*;
import cycmoo.agent.*;
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
import org.opencyc.chat.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;


import ViolinStrings.Strings;


public class LogicMooAgency extends Thread {
    
    static public CycSymbol SYMBOL_NIL = new CycSymbol("NIL");
    private static LogicMooCycAccess cyc = null;
    private static Hashtable agents = null; 

    public LogicMooAgency(LogicMooCycAccess aCyc) {
	cyc = aCyc;
	if (agents==null) {
	    agents = new Hashtable();
	    enumerateNPCs();
	}
    }

    static public void enumerateNPCs() {
	Iterator supposed = cyc.queryArrayListSEL("EverythingPSC","(#$isa ?SEL #$IndividualAgent)").iterator();
	while (supposed.hasNext()) {
	    confirmNPC((CycFort)supposed.next());
	}
    }
    static public void confirmNPC(CycFort agent) {
	    MooCycRobot robot = (MooCycRobot)agents.get(agent);
	    if (robot==null) {
		robot = new MooCycRobot(agent);
		agents.put(agent,robot);
		robot.start();
	    }
    }

    static public Hashtable getAgents() {
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


