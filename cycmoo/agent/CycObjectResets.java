package logicmoo.agent;

import logicmoo.*;
import logicmoo.api.*;
import logicmoo.obj.*;
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
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;

// Util
import ViolinStrings.*;


public class CycObjectResets  extends Thread {
    static LogicMooCycAccess cyc = LogicMoo.cyc;
    public boolean listening = true;
    private org.opencyc.webserver.WebServer webserver;

    public CycObjectResets() throws IOException {
	cyc = LogicMoo.cyc;
    }

    public void run() {
	while ( !interrupted() ) {
	    //System.out.println("Resets firing");
	    try {
		Iterator resets =  cyc.queryMt3("(#$mudReset ?A ?B ?C)").iterator();
		while ( resets.hasNext() ) {
		    CycList ans = (CycList)resets.next();
		    applyReset((CycFort)ans.first(),(CycFort)ans.second(),(Integer)ans.third());
		}
		Thread.sleep(60000);
	    } catch ( Exception e ) {
	    }
	}
    }

    public void applyReset(CycFort place, CycFort type, Integer number) {
       // System.out.println("mudReset " + place + " " +type + " " + number);

    }

    public synchronized boolean receiveEvent(Object from, LogicMooEvent event) {
	return true;
    }

}


