package cycmoo.api;

import cycmoo.agent.*;
import cycmoo.cmd.*;
import cycmoo.obj.*;
import cycmoo.plugin.*;
import cycmoo.net.*;
import cycmoo.util.*;

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
import org.opencyc.javashell.*;
import org.opencyc.cycobject.*;
import org.opencyc.cyclobject.*;
import org.opencyc.kif.*;
import org.opencyc.javashell.*;
import org.opencyc.util.*;
import org.opencyc.chat.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;
import org.opencyc.inferencesupport.*;
import org.opencyc.constraintsolver.*;
import org.apache.oro.util.*;

// Util
import ViolinStrings.*;

/**
* Provides wrappers for the OpenCyc API.<p>
*
* Collaborates with the <tt>Jamud</tt> class which manages the api connections.
*
* @version $Id: LogicMoo.java,v 1.3 2003-06-13 10:04:47 dmiles Exp $
* @author Douglas R. Miles
*
* <p>Copyright 2001 Cycorp, Inc., license is open source GNU LGPL.
* <p><a href="http://www.opencyc.org/license.txt">the license</a>
* <p><a href="http://www.opencyc.org">www.opencyc.org</a>
* <p><a href="http://www.sourceforge.net/projects/opencyc">OpenCyc at SourceForge</a>
* <p>
* THIS SOFTWARE AND KNOWLEDGE BASE CONTENT ARE PROVIDED ``AS IS'' AND
* ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OPENCYC
* ORGANIZATION OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE AND KNOWLEDGE
* BASE CONTENT, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

public class LogicMoo extends Thread {

    static public void main(String[] args) {
	try {
	    LogicMoo cm = new LogicMoo();
	    while( cm.running && !cm.interrupted() ) {
		Thread.sleep(10000);
	    }
	    //MooCommandLine.runClientAt(new PrintWriter(System.out,true),new BufferedReader(new InputStreamReader(System.in)));
	} catch( Exception e ) {
	    e.printStackTrace();
	}
    }


    /**
     * Constructs a new CycAssertionsFactory object.
     */
    public LogicMoo() throws IOException, CycApiException {
	try {
	    if( cyc==null ) cyc =  new LogicMooCycAccess();
	    primaryStart();
	    System.out.println("loaded LogicMoo");
	    moo = this;
	} catch( Exception e ) {
	    e.printStackTrace();
	}
    }


    static public LogicMoo getInstance() {
	return moo;
    }

    /**
     * Constructs a new LogicMoo object to the given CycProxyAgent in the given
     * agent community.
     *
     * @param myAgentName the name of the local agent
     * @param cycProxyAgentName the name of the cyc proxy agent
     * @param agentCommunity the agent community to which the cyc proxy agent belongs
     */
    public LogicMoo (String myAgentName,
		     String cycProxyAgentName,
		     int agentCommunity) throws IOException, CycApiException  {
	if( cyc==null )	cyc = new LogicMooCycAccess(myAgentName,cycProxyAgentName,agentCommunity);
	primaryStart();
	System.out.println("loaded LogicMoo");
    }

    /**
     * Constructs a new LogicMoo object given a host name, port, communication mode and persistence indicator.
     *
     * @param hostName the host name
     * @param basePort the base (HTML serving) TCP socket port number
     * @param communicationMode either ASCII_MODE or BINARY_MODE
     * @param persistentConnection when <tt>true</tt> keep a persistent socket connection with
     * the OpenCyc server
     */
    public LogicMoo(String hostName, int basePort, int communicationMode, boolean persistentConnection)
    throws IOException, UnknownHostException, CycApiException {
	if( cyc==null )	cyc = new LogicMooCycAccess(hostName,basePort,communicationMode,persistentConnection);
	primaryStart();
	System.out.println("loaded LogicMoo");
    }

    /***********************************************************
     *  Gets for private  static fields
     *
     **********************************************************/

    static public CycConstant logicMooMt =  null;
    static public CycConstant jamudMt =  null;
    static public LogicMooAgency npc1 = null;
    static public MooIrcBot opnIRCBot = null;
    static public org.opencyc.chat.IrcChat wfIRCBot = null;
    static public bsh.Interpreter logicmooBeanShell;     
    static public LogicMooCycAccess cyc=null;
    static public LogicMoo moo=null;
    static public boolean running = false;

    private static org.opencyc.webserver.WebServer cycWebserverThread=null;
    private static cycmoo.net.LogicMooTelnetServer cycMooserverThread = null;

    public CycAccess getCycAccess() {
	return(CycAccess)cyc;
    }
    public LogicMooCycAccess getCycMooAccess() {
	return(LogicMooCycAccess)cyc;
    }
    public bsh.Interpreter getBeanShell() {
	return logicmooBeanShell;
    }
    public CycAssertionsFactory getCycAssertionsFactory() {
	return(CycAssertionsFactory)cyc;
    }

    /***********************************************************
     *  Initialize all services
     *
     **********************************************************/

    private synchronized static void primaryStart() {
	if( running ) return;
	else running = true;
	Log.makeLog();
	try {
	    logicMooMt =  cyc.makeCycConstantSafe("#$LogicMooMt");
	    jamudMt =  cyc.makeCycConstantSafe("#$JamudMt"    /*TODO*/);
	    cyc.assertIsaSafe(logicMooMt,     cyc.microtheory,     cyc.baseKB);
	    cyc.assertIsaSafe(jamudMt,     cyc.microtheory,     cyc.baseKB);
	    cyc.assertIsaSafe(jamudMt, cyc.reifiedMicrotheory,     cyc.baseKB);
	    cyc.assertGenlMt(jamudMt,logicMooMt);
	    cyc.assertGenlMt(logicMooMt,"HumanActivitiesMt");
	    cyc.assertGenlMt(logicMooMt,"BuildingMt");
	    cyc.assertGenlMt(logicMooMt,cyc.javaMt);
	} catch( Exception e ) {
	    e.printStackTrace();
	}
	Log.current.println("Starting Cyc Beanshell");
	try {
	    if( logicmooBeanShell==null ) {
		// Interpreter(java.io.Reader in, java.io.PrintStream out, java.io.PrintStream err, boolean interactive) 
		logicmooBeanShell = new bsh.Interpreter(new InputStreamReader(System.in),(PrintStream)System.out ,(PrintStream)System.out,false);
		logicmooBeanShell.set("cyc",cyc);
		logicmooBeanShell.set("LogicMoo",cyc);
		logicmooBeanShell.set("CycLBot",opnIRCBot);
		logicmooBeanShell.set("interpretor",logicmooBeanShell);
	    }
	} catch( EvalError e ) {
	    Log.current.println("Aborted Beanshell");
	    e.printStackTrace();
	}
	Log.current.println("Starting LogicMOO (LogicMoo)");
	if( cycWebserverThread==null ) {
	    try {
		Log.current.println("Starting Cyc Webserver");
		cycWebserverThread = new org.opencyc.webserver.WebServer();
		cycWebserverThread.start();
		//cyc.registerUser("CycWebserver",cycWebserverThread);
	    } catch( Exception e ) {
		Log.current.println("Aborted Cyc Webserver");
	    }
	}
	if( wfIRCBot==null ) {
	    try {
	   wfIRCBot = new IrcChat(cyc,"CycLBot","http://www.opencyc.org","irc.freenode.net",6667,"#opencyc");
		wfIRCBot .start();
		//opnIRCBot = new MooIrcBot(cyc,"CycLBot","http://www.opencyc.org","irc.freenode.net",6667,"#opencyc");
		//opnIRCBot.start();
	    } catch( Exception e ) {
		e.printStackTrace();
		Log.current.println("Aborted Cyc IrcBot");
	    }
	}
	try {
	    cyc.constructMtFromFile("JamudMt.kif","BaseKB");
	} catch( Exception e ) {
	    //e.printStackTrace();
	}
      /*
        if( cycMooserverThread==null ) {
	    try {
		Log.current.println("Starting Cyc Mooserver");
		cycMooserverThread = new LogicMooTelnetServer();
		cycMooserverThread.start();
		//cyc.registerUser("CycMooserver",cycWebserverThread);
	    } catch( Exception e ) {
		Log.current.println("Aborted Cyc Mooserver");
	    }
	}
        npc1 = new LogicMooAgency(cyc);
	npc1.start();
      */
	/*
	    if( !cycPlcStarted ) {
	    try {
	    cycPlcStarted = true; 
	    //       JPL.init();
	    //	consultFile("e2c.pl");
	    } catch( Exception e ) {
	    Log.current.println("Aborted Cyc NLParser");
	    }
	}
	*/
	//efnetIRCBot = startCycLBot(efnetIRCBot, "jllykifsh","irc.rt.ru");
	//   cyc.constructMtFromFile("JavaMt.kif","JavaMt");
	//        cyc.constructMtFromFile("JamudMt.kif","BaseKB");
	//  constructMtFromFile("LogicMooMt.kif","LogicMooMt");
	/*
	    File dir = new File("logicmoo/plugins/");
	    File[] files = dir.listFiles();
	    for( int i = 0 ; i<files.length; i++ ) {
	    String classname = files[i].getName();
	    if( classname.endsWith(".class") ) {
	    classname = "cycmoo.plugins."+classname.substring(0,classname.indexOf('.'));
	    if( !plugins.contains(classname) ) {
	    Log.current.println("Starting " + classname);
	
	    try {
		Object obj = Class.forName(classname).newInstance();
		((Thread)obj).start();
		plugins.put(classname,obj);
	    } catch( Exception e ) {
		e.printStackTrace();
	    }
	    }
	    }
	    }            
	*/
    }
}

//</pre>






