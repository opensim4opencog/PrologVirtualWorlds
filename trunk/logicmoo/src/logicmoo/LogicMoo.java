package logicmoo;

import logicmoo.api.*;
import logicmoo.agent.*;
import logicmoo.cmd.*;
import logicmoo.obj.*;
import logicmoo.plugin.*;
import logicmoo.net.*;
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
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;

// Util
import ViolinStrings.*;

/**
* Provides wrappers for the OpenCyc API.<p>
*
* Collaborates with the <tt>Jamud</tt> class which manages the api connections.
*
* @version $Id: LogicMoo.java,v 1.9 2002-06-09 19:23:51 dmiles Exp $
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

public class LogicMoo {

    //private  static Jamud logicmooInstance;
    //private  static Area logicmooArea;
    //    private  static PlayerManager logicmooPlayerManager;
    //private  static Room logicmooTop;     
    private  static Object initialObject; 
    private  static bsh.Interpreter logicmooBeanShell;     
    public static LogicMooCycAccess cyc=null;
    private  static boolean running = false;

    private static org.opencyc.webserver.WebServer cycWebserverThread=null;
    private static logicmoo.net.LogicMooTelnetServer cycMooserverThread = null;

    /**
     * Constructs a new CycAssertionsFactory object.
     */
    public LogicMoo() throws IOException, CycApiException {
	if ( cyc==null ) cyc =  new LogicMooCycAccess();
	primaryStart();
	System.out.println("loaded LogicMoo");

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
	if ( cyc==null ) cyc = new LogicMooCycAccess(myAgentName,cycProxyAgentName,agentCommunity);
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
	if ( cyc==null ) cyc = new LogicMooCycAccess(hostName,basePort,communicationMode,persistentConnection);
	primaryStart();
	System.out.println("loaded LogicMoo");
    }

    /***********************************************************
     *  Gets for private  static fields
     *
     **********************************************************/

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

    public static void main(String[] args){
	try {
	    LogicMoo cm = new LogicMoo();  
	} catch ( Exception e ) {
	    e.printStackTrace();
	}

	MooCommandLine.runClientAt(new PrintWriter(System.out,true),new BufferedReader(new InputStreamReader(System.in)));
    }


    private synchronized static void primaryStart() {
	if ( running ) return;
	else running = true;
	Log.makeLog();
	startPrimary();
	startBeanShellThread();
	Log.current.println("Starting LogicMOO (LogicMoo)");
	startWebserverThread();
	try {
	    startIrcBot();
	 //   cyc.constructMtFromFile("JamudMt.kif","BaseKB");
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	startMooServerThread();
	startNPCs();

	//startNLParser();
	//startSecondaryThread
	//runPluginsDir();
    }

    static MooCycRobot npc1 = null;

    private synchronized static void startNPCs() {
	npc1 = new MooCycRobot(cyc.makeCycConstant("SampleEliza"));
	npc1.start();
    }


					  /*
    private synchronized static void runPluginsDir() {
	File dir = new File("logicmoo/plugins/");
	File[] files = dir.listFiles();
	for ( int i = 0 ; i<files.length; i++ ) {
	    String classname = files[i].getName();
	    if ( classname.endsWith(".class") ) {
		classname = "logicmoo.plugins."+classname.substring(0,classname.indexOf('.'));
		if ( !plugins.contains(classname) ) {
		    Log.current.println("Starting " + classname);

		    try {  
			Object obj = Class.forName(classname).newInstance();
			((Thread)obj).start();
			plugins.put(classname,obj);
		    } catch ( Exception e ){
			e.printStackTrace();
		    }
		}
	    }
	}
    }		     */

    public static MooIrcBot opnIRCBot = null;

    public synchronized static void startIrcBot() {
	//return;
	
	if ( opnIRCBot==null ) {
	    try {
		opnIRCBot = new MooIrcBot(cyc,"CycLBot","http://www.opencyc.org","irc.openprojects.net",6667,"#logicmoo");
	        opnIRCBot.start();
	    } catch ( Exception e ) {
		e.printStackTrace();
		Log.current.println("Aborted Cyc IrcBot");
	    }
	}  
    }

    public synchronized static void startWebserverThread() {
	if ( cycWebserverThread==null ) {
	    try {
		Log.current.println("Starting Cyc Webserver");
		cycWebserverThread = new org.opencyc.webserver.WebServer();
		cycWebserverThread.start();
		//cyc.registerUser("CycWebserver",cycWebserverThread);
	    } catch ( Exception e ) {
		Log.current.println("Aborted Cyc Webserver");
	    }
	}
    }

    public synchronized static void startMooServerThread() {
	if ( cycMooserverThread==null ) {
	    try {
		Log.current.println("Starting Cyc Mooserver");
		cycMooserverThread = new LogicMooTelnetServer();
		cycMooserverThread.start();
		//cyc.registerUser("CycMooserver",cycWebserverThread);
	    } catch ( Exception e ) {
		Log.current.println("Aborted Cyc Mooserver");
	    }
	}
    }



    public static void setWriteToOut(boolean bool) {
	Log.current.writeToOut=bool;
    }

    public synchronized static void startBeanShellThread() {
	Log.current.println("Starting Cyc Beanshell");
	try {
	    if ( logicmooBeanShell==null ) {
		// Interpreter(java.io.Reader in, java.io.PrintStream out, java.io.PrintStream err, boolean interactive) 
		logicmooBeanShell = new bsh.Interpreter(new InputStreamReader(System.in),(PrintStream)System.out ,(PrintStream)System.out,false);
		logicmooBeanShell.set("cyc",cyc);
		logicmooBeanShell.set("LogicMoo",cyc);
		logicmooBeanShell.set("CycLBot",opnIRCBot);
		logicmooBeanShell.set("interpretor",logicmooBeanShell);
	    }
	} catch ( EvalError e ) {
	    Log.current.println("Aborted Beanshell");
	    e.printStackTrace();
	}
    }

    public static CycConstant logicMooMt =  null;
    public static CycConstant jamudMt =  null;

    public synchronized static void startPrimary() {
	if ( logicMooMt!=null ) return;
	try {
	    logicMooMt =  cyc.makeCycConstantSafe("#$LogicMooMt");
	    jamudMt =  cyc.makeCycConstantSafe("#$JamudMt");
	    cyc.assertIsaSafe(logicMooMt,     cyc.microtheory,     cyc.baseKB);
	    cyc.assertIsaSafe(jamudMt,     cyc.microtheory,     cyc.baseKB);
	    cyc.assertIsaSafe(jamudMt, cyc.reifiedMicrotheory,     cyc.baseKB);
	    cyc.assertGenlMt(jamudMt,logicMooMt);
	    cyc.assertGenlMt(logicMooMt,"HumanActivitiesMt");
	    cyc.assertGenlMt(logicMooMt,"BuildingMt");
	    cyc.assertGenlMt(logicMooMt,cyc.javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }


    private synchronized void startSecondaryThread() {
	//efnetIRCBot = startCycLBot(efnetIRCBot, "jllykifsh","irc.rt.ru");
	try {
	    try {
	     //   cyc.constructMtFromFile("JavaMt.kif","JavaMt");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	    try {
	    //        cyc.constructMtFromFile("JamudMt.kif","BaseKB");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	    try {
		//  constructMtFromFile("LogicMooMt.kif","LogicMooMt");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	} catch ( Exception ee ) {
	    ee.printStackTrace();
	}
    }

    /***********************************************************
     *  Jamud Booting
     *
     **********************************************************/

    //private static Area logicmooTopMicrotheory;
    private static Hashtable logicmooTopRooms;
    private static Hashtable logicmooTopArtifacts;
    private static Hashtable logicmooTopBodies;

    public static boolean cycPlcStarted = false;

    public synchronized static void startNLParser() {
	if ( !cycPlcStarted ) {
	    try {
		cycPlcStarted = true; 
		//       JPL.init();
		//	consultFile("e2c.pl");
	    } catch ( Exception e ) {
		Log.current.println("Aborted Cyc NLParser");
	    }
	}

    }

    public static String queryRawPrologServer(String raw) throws Exception {
	return queryRawPrologServer(raw,500);
    }

    public static String queryRawPrologServer(String raw, int time) throws Exception {
	//ensureConnectedRawPrologServer();
	StringBuffer sb = new StringBuffer("");
	try {    
	    Socket pSock =new Socket("localhost" ,777);
	    pSock.setSoTimeout(2);
	    //pSock.connect();
	    PrintWriter pw = new PrintWriter(pSock.getOutputStream());
	    pw.println("+"+raw + "\n");
	    pw.flush();
	    BufferedReader st = new BufferedReader(new InputStreamReader( pSock.getInputStream()));
	    Thread.sleep(time);
	    while ( st.ready() ) {
		String line = st.readLine();
		if ( line == null ) return sb.toString();
		System.out.println(line);
		sb.append(line + "\n");
	    }
	    System.out.println("pl");
	    pSock.close();
	} catch ( Exception e ){
	    e.printStackTrace();
	}
	return sb.toString();

    }

    public static String e2c(String eng) throws Exception {
	return cyc.queryRawPrologServer("(e2c('"+eng+"',Meaning),writeq(Meaning),nl).",2000);
    }

    public void sendCycNlWords() {
	sendCycNlWords(new File("cycdump"));
    }


    public void allWordsSend(PrintWriter dump, Iterator allWords) {
	while ( allWords.hasNext() ) {
	    CycFort thisWord = (CycFort)allWords.next();
	    wordsSend(dump,thisWord);
	}
    }

    public void wordsSend(PrintWriter dump, CycFort thisWord) {
	Iterator ata = null;
	try {
	    ata = cyc.converseList("(ALL-TERM-ASSERTIONS " + thisWord.cyclify() + ")").iterator();
	    while ( ata.hasNext() ) {
		assertToProlog(dump,ata.next().toString());
	    }
	} catch ( Exception ee ) {
	    dump.println("/*");
	    dump.println(""+thisWord);
	    dump.println(""+ee);
	    dump.println("*/");
	}
    }

    public void sendCycNlWords(File dumpfile) {
	FileWriter fw = null;
	try {
	    fw = new FileWriter(dumpfile);
	    PrintWriter dump = new PrintWriter(fw);
	    //Iterator allWords = cyc.getAllInstances(cyc.getConstantByName("LexicalWord")).iterator();
	    allWordsSend(dump,cyc.getAllInstances(cyc.getConstantByName("LexicalWord")).iterator());
	    allWordsSend(dump,cyc.getAllInstances(cyc.getConstantByName("NLTerm")).iterator());
	    allWordsSend(dump,cyc.getAllInstances(cyc.getConstantByName("ProposedPublicConstant-NL")).iterator());
	    wordsSend(dump,cyc.getConstantByName("ThoughtTreasureMt"));
	    wordsSend(dump,cyc.getConstantByName("TTGeneralEnglishMt"));            
	} catch ( Exception e ){
	    e.printStackTrace();
	}
	try {
	    fw.close();
	} catch ( Exception e ){
	    e.printStackTrace();
	}
    }

    public void assertToProlog(PrintWriter dump,Object term) throws Exception {
	try {
	    String id = term.toString().split(":")[1];
	    CycList af = cyc.converseList("(ASSERTION-FORMULA (find-assertion-by-id " + id + "))");
	    try {
		if ( !af.first().equals(cyc.comment) ) {
		    dump.println(toPrologCycTerm(af)+".");
		}
	    } catch ( Exception e ) {
	    }
	} catch ( Exception ee ) {
	}
    }

    public static String toPrologCycTerm(Object term) {
	if ( term==null ) return "null";
	if ( term instanceof CycList ) return toPrologCycList((CycList)term);
	if ( term instanceof CycConstant ) return "'"+(((CycConstant)term).cyclify())+"'";
	if ( term instanceof CycNart ) return toPrologCycListNart(((CycNart)term));
	if ( term instanceof String ) return toPrologCycString(term.toString());
	if ( term instanceof Integer ) return(term.toString());
	if ( term instanceof Float ) return(term.toString());
	if ( term instanceof CycSymbol ) return "'"+(term.toString())+"'";
	if ( term instanceof CycAssertion ) return toPrologCycTerm(((CycAssertion)term).getFormula());
	return toPrologCycString(term.toString());
    }

    public static String toPrologCycListNart(CycNart cyclist) {
	return toPrologCycString(cyclist.toString());
    }
    
    public static String toPrologCycList(CycList cyclist) {
	if ( cyclist==null ) return "[]";
	if ( cyclist.isEmpty() ) return "[]";
	StringBuffer sb = new StringBuffer("[");
	if ( !cyclist.isProperList() ) {
	    for ( int i =0; i < cyclist.size(); i++ ) {
		sb.append(toPrologCycTerm(cyclist.get(i))).append(", ");
	    }
	    return sb.append(toPrologCycTerm(cyclist.rest())).append("]").toString();
	}
	for ( int i =0; i < cyclist.size()-1; i++ ) {
	    sb.append(toPrologCycTerm(cyclist.get(i))).append(", ");
	}

	return sb.append(toPrologCycTerm(cyclist.get(cyclist.size()-1))).append("]").toString();
    }


    public static String toPrologCycString(String term) {
	return "'"+Strings.change(term,"'","\\'")+"'";
    }

}

//</pre>






