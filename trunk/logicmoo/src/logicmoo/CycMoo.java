//<pre>
package logicmoo;

// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;

// Jamud
import logicmoo.net.*;
import logicmoo.obj.*;
import logicmoo.obj.event.JamudEventManager;
import logicmoo.cmd.*;
import logicmoo.plugin.JamudPlugin;
import logicmoo.script.ScriptLanguage;
import logicmoo.util.*;
import net.n3.nanoxml.*;

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
* @version $Id: CycMoo.java,v 1.7 2002-05-14 05:02:38 dmiles Exp $
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
public class CycMoo extends CycAssertionsFactory {

    private  static  CycMooAgent thisCycMooAgent = null;
    private  static  Jamud logicmooInstance;
    private  static  Area logicmooArea;
    //    private  static  PlayerManager logicmooPlayerManager;
    private  static  Room logicmooTop;     
    private  static  Object initialObject; 
    private  static  bsh.Interpreter logicmooBeanShell;     
    public  static  CycMoo staticAccess=null;
    private  static  MooIrcBot efnetIRCBot=null;
    private  static  MooIrcBot opnIRCBot=null;
    private  static  boolean running = false;



    public  static  CycConstant logicMooMt = null;
    public  static  CycConstant jamudMt = null;
    public  static  CycConstant javaMt = null;
    public  static  CycConstant cycadministrator = null;
    public  static  CycConstant opencycproject = null;
    public  static  CycConstant functionalRelation = null;

    public  static  CycConstant cycVoid = null;
    public  static  CycConstant cycNull = null;
    public  static  CycConstant cycHasArrayMember = null;
    public  static  CycConstant cycHasMethod = null;
    public  static  CycConstant cycHasSlot = null;
    public  static  CycConstant cycClassInstance = null;
    public  static  CycConstant cycHasSlotValue = null;
    public  static  CycConstant geographicalRegion = null;
    public  static  CycConstant cycArrayOfClass = null;

    private  static  org.opencyc.webserver.WebServer cycWebserverThread=null;

    // CycConstant & Class -> CycConstant key of Fields | DataMethod | Method
    public  static  HashMap thisClassTemplates = new HashMap();

    // CycConstant || Class -> Class || CycConstant 
    public  static  HashMap cycKnowsClass = new HashMap();
    public  static  HashMap cycKnowsObjectAsConstant = new HashMap();
    //public  static  HashMap cycKnowsMicrotheory = new HashMap();
    //public  static  HashMap cycKnowsExit = new HashMap();
    public  static  HashMap cycKnowsNPC = new HashMap();
    //public  static  HashMap cycKnowsPlayer = new HashMap();
    //public  static  HashMap cycKnowsInanimate = new HashMap();
    public  static  String AREA_HOLDER="MIDGAARD";



    /**
     * Constructs a new CycAssertionsFactory object.
     */
    public CycMoo() throws IOException, CycApiException {
	super();
	if ( staticAccess==null ) staticAccess = this;
    }

    /**
     * Constructs a new CycMoo object to the given CycProxyAgent in the given
     * agent community.
     *
     * @param myAgentName the name of the local agent
     * @param cycProxyAgentName the name of the cyc proxy agent
     * @param agentCommunity the agent community to which the cyc proxy agent belongs
     */
    public CycMoo (String myAgentName,
		   String cycProxyAgentName,
		   int agentCommunity) throws IOException, CycApiException  {
	super(myAgentName,cycProxyAgentName,agentCommunity);
	if ( staticAccess==null ) staticAccess = this;
    }

    /**
     * Constructs a new CycMoo object given a host name, port, communication mode and persistence indicator.
     *
     * @param hostName the host name
     * @param basePort the base (HTML serving) TCP socket port number
     * @param communicationMode either ASCII_MODE or BINARY_MODE
     * @param persistentConnection when <tt>true</tt> keep a persistent socket connection with
     * the OpenCyc server
     */
    public CycMoo(String hostName, int basePort, int communicationMode, boolean persistentConnection)
    throws IOException, UnknownHostException, CycApiException {
	super( hostName,  basePort,  communicationMode,  persistentConnection);
	if ( staticAccess==null ) staticAccess = this;
    }

    /***********************************************************
     *  Gets for private  static fields
     *
     **********************************************************/

    public  static  CycAccess getCycAccess() {
	staticAccess.start();
	return(CycAccess)staticAccess;
    }
    public  static  Jamud getJamud() {
	staticAccess.start();
	return logicmooInstance;
    }
    public  static Room getJamudTop() {
	staticAccess.start();
	return logicmooTop;
    }
    public  static  CycMoo currentInstance() {
	staticAccess.start();
	return staticAccess;
    }
    public  static  bsh.Interpreter getBeanShell() {
	staticAccess.start();
	return logicmooBeanShell;
    }
    public  static  CycAssertionsFactory getCycAssertionsFactory() {
	staticAccess.start();
	return(CycAssertionsFactory)staticAccess;
    }

    public static void main(String[] args){
	try {
	    CycMoo cm = new CycMoo();
	    cm.start();    
	    while ( true ) {
		Thread.sleep(10000);
	    }
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }

    /***********************************************************
     *  Initialize all services
     *
     **********************************************************/

    private synchronized void start() {
	if ( running ) return;
	running = true;
	System.out.println("Starting LogicMOO (CycMoo)");
	Log.makeLog();
	if ( logicmooInstance==null ) startJamud(initialObject);
	if ( cycWebserverThread==null ) {
	    System.out.println("cycWebserverThread==null");
	    try {
		cycWebserverThread = new org.opencyc.webserver.WebServer();
		cycWebserverThread.start();
		//Thread            

	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	}

	try {
	    cycVoid =  makeCycConstantSafe("#$voidValue");
	    cycNull =  makeCycConstantSafe("#$nullValue");
	    cycHasArrayMember =  makeCycConstantSafe("#$javaArrayContains");
	    cycHasMethod =  makeCycConstantSafe("#$hasMethod");
	    cycHasSlot =  makeCycConstantSafe("#$relationAllExists");
	    cycClassInstance =  makeCycConstantSafe("#$ClassInstance");
	    cycHasSlotValue =  makeCycConstantSafe("#$hasSlotValue");
	    logicMooMt =  makeCycConstantSafe("#$LogicMooMt");
	    jamudMt =  makeCycConstantSafe("#$JamudMt");
	    javaMt =  makeCycConstantSafe("#$JavaMt");
	    CycFort reifiedMicrotheory =  makeCycConstantSafe("#$ReifiedMicrotheory");
	    geographicalRegion =  makeCycConstantSafe("#$Room");
	    cycArrayOfClass =  makeCycConstantSafe("#$SetOfTypeFn");

	    staticAccess.makeCycConstantError =  makeCycConstantSafe("#$MakeCycConstantErrorFn");
	    assertIsaSafe(logicMooMt,     staticAccess.microtheory,     staticAccess.baseKB);
	    assertIsaSafe(jamudMt,     staticAccess.microtheory,     staticAccess.baseKB);
	    assertIsaSafe(reifiedMicrotheory,     makeCycConstantSafe("#$MicrotheoryType"),     staticAccess.baseKB);
	    assertIsaSafe(jamudMt,reifiedMicrotheory,     staticAccess.baseKB);
	    assertIsaSafe(javaMt,     staticAccess.microtheory,        staticAccess.baseKB);
	    staticAccess.assertGenlMt(jamudMt,logicMooMt);
	    staticAccess.assertGenlMt(logicMooMt,"HumanActivitiesMt");
	    staticAccess.assertGenlMt(logicMooMt,"BuildingMt");
	    staticAccess.assertGenlMt(logicMooMt,javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}

	efnetIRCBot = startCycLBot(efnetIRCBot, "jllykifsh","irc.rt.ru");
	//opnIRCBot = startCycLBot(opnIRCBot, "CycLBot","irc.openprojects.net");
	try {
	    try {
		constructMtFromFile("JavaMt.kif","JavaMt");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	    try {
		constructMtFromFile("JamudMt.kif","BaseKB");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	    try {
		constructMtFromFile("LogicMooMt.kif","LogicMooMt");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	} catch ( Exception ee ) {
	    ee.printStackTrace();
	}

	try {
	    if ( logicmooBeanShell==null ) {
		// Interpreter(java.io.Reader in, java.io.PrintStream out, java.io.PrintStream err, boolean interactive) 
		logicmooBeanShell = new bsh.Interpreter(new InputStreamReader(System.in),(PrintStream)System.out ,(PrintStream)System.out,false);
		logicmooBeanShell.run();
		logicmooBeanShell.set("logicmooInstance",logicmooInstance);
		logicmooBeanShell.set("logicmooTop",logicmooTop);
		logicmooBeanShell.set("staticAccess",staticAccess);
		logicmooBeanShell.set("LogicMoo",staticAccess);
		logicmooBeanShell.set("CycMoo",staticAccess);
		logicmooBeanShell.set("jllykifsh",efnetIRCBot);
		logicmooBeanShell.set("CycLBot",opnIRCBot);
		logicmooBeanShell.set("interpretor",logicmooBeanShell);
	    }
	} catch ( EvalError e ) {
	    e.printStackTrace();
	}
	try {
	    if ( thisCycMooAgent==null ) {
		thisCycMooAgent =  new CycMooAgent();
		thisCycMooAgent.start();
	    }
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }




    /***********************************************************
     *  IRC Bots
     *
     **********************************************************/


    public  static  synchronized void stopCycLBot(MooIrcBot acycIRCBot) {
	try {
	    acycIRCBot.running = false;
	    acycIRCBot.ircDisconnect();
	    acycIRCBot = null;

	} catch ( Exception e ) {
	}
    }

    public  static  synchronized MooIrcBot startCycLBot(MooIrcBot acycIRCBot, String name,String server) {
	if ( acycIRCBot==null ) {
	    try {
		acycIRCBot = new MooIrcBot(name,"http://www.opencyc.org",server,6667,"#ai");
		acycIRCBot.start();
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	}
	return acycIRCBot;
    }


    /***********************************************************
     *  File Handling
     *
     **********************************************************/

    public  static  File ontologyFile(String file) {
	File f = null;
	f=new File(file);
	if ( f.exists() ) return f;
	f=new File("../ontologies/" + file);
	if ( f.exists() ) return f;
	f=new File("../ontologies/kif/" + file);
	if ( f.exists() ) return f;
	f=new File("../ontologies/daml/" + file);
	if ( f.exists() ) return f;
	return new File(file);
    }

    public synchronized  static   void constructMtFromFile(String file,String mtName) throws Exception {
	constructMtFromFile(ontologyFile(file),mtName);
    }

    public synchronized  static   void constructMtFromFile(File file,String mtName) throws Exception {
	constructMtFromFile(file,makeCycConstantSafe(mtName));
    }

    public synchronized  static   void constructMtFromFile(String file,CycConstant mtName) throws Exception {
	constructMtFromFile(ontologyFile(file),mtName);
    }

    public synchronized  static  void constructMtFromFile(File file,CycConstant mtName) throws Exception {
	//  staticAccess.kill(mtName);
	System.out.println("constructMtFromFile " + file.getAbsolutePath() + " " + mtName.cyclify());
	KifFileLoader theMt = new KifFileLoader(staticAccess);
	theMt.setDefaultMt(mtName);
	theMt.setDefaultCyclist("CycAdministrator");
	theMt.setDefaultProject("OpenCycProject");
	theMt.addFileEachLine(new PrintWriter(System.out),file,mtName.toString());

	/*
	theMt.load(file);
	if ( theMt.testAssertionsHtml(null) )
	    theMt.commitAssertions(new PrintWriter(System.out));
	    */
    }

    public static synchronized ArrayList getArrayListQuerySELStatic(String mt, String query) {
	return staticAccess.getArrayListQuerySEL(mt,query);
    }

    /***********************************************************
     *  Mt Handling Extras
     *
     **********************************************************/

    public synchronized    void killMt(String mtName) throws IOException, CycApiException {
	staticAccess.kill(     makeCycConstantSafe(mtName));
    }


    /***********************************************************
     *  BeanShell Access
     *
     **********************************************************/

    public synchronized  static  Object eval(String cmd) throws bsh.EvalError  {
	return  logicmooBeanShell.eval(cmd);
    }

    public synchronized  static  void set(String name, Object obj) throws bsh.EvalError  {
	logicmooBeanShell.set(name,obj);
    }


    /***********************************************************
     *  CycL Paraphrase
     *
     **********************************************************/

    public synchronized    String attemptParaphrase(Object post) {
	//  System.out.println("post = " + post);
	if ( post == null ) return null;
	try {
	    if ( post instanceof Iterator ) {
		if ( !(((Iterator)post).hasNext()) ) return "none.";
		StringBuffer sb = new StringBuffer(attemptParaphrase(((Iterator)post).next()));
		while ( ((Iterator)post).hasNext() )
		    sb.append(", ").append(attemptParaphrase(((Iterator)post).next()));
		return sb.toString();
	    }
	    if ( post instanceof CycConstant )
		return  staticAccess.converseString("(generate-phrase " + ((CycConstant)post).stringApiValue() +  ")");
	    //        if ( post instanceof CycFort )
	    //          return  staticAccess.converseString("(generate-phrase '" + ((CycFort)post).stringApiValue() +  ")");
	    if ( post instanceof CycVariable )
		return(((CycVariable)post).stringApiValue());
	    if ( post instanceof CycList ) {
		if ( ((CycList)post).isEmpty() )
		    return "an empty list ";
		if ( !((CycList)post).isProperList() )
		    return attemptParaphrase(((CycList)post).first()) 
		    + " = " + attemptParaphrase(((CycList)post).rest());
		try {
		    if ( ((CycList)post).first() instanceof CycList ) return attemptParaphrase(((CycList)post).iterator());
		} catch ( Exception e ) {
		    e.printStackTrace();
		}

		return  staticAccess.converseString("(generate-phrase '" + ((CycList)post).stringApiValue() +  ")");
	    }
	    //          if ( post instanceof CycNart )
	    //          return  staticAccess.converseString("(generate-phrase '" + ((CycFort)post).stringApiValue() +  ")");

	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	return post.toString();
    }


    public synchronized    String mudExec(PartiallyTangible actor, String args) {
	System.out.println("mudExec " + actor.getName()+ " " + args);
	return "ok.";
	/*
	if ( args==null ) return "nothing to do (null)";
	PartiallyTangible newplace = findObject(args);
	if ( newplace!=null ) {
	   actor.setParentContainer(newplace.childContainer());
	   // actor.enact("look");
	   return "teleported to " + newplace.getName();
	}
	String[] params = Strings.split(args);
	if ( params==null ) return "nothing to do (params==null)";
	return mudExec(actor,params[0].toLowerCase(),params);
	*/
    }


    /*
    public synchronized  static  class PartiallyTangible extends PartiallyTangible {

    // Default Constructor
    public PartiallyTangible() {
    super();
    }

    public PartiallyTangible(Race race, CycFort gender, CycFort job) {
    super(race,gender,job);
    }
    public synchronized  static  PartiallyTangible asCycReifiable(PartiallyTangible object) {
    if (object instanceof PartiallyTangible) return (PartiallyTangible)object;
    PartiallyTangible outobj = new PartiallyTangible();
    object.copycat(outobj);
    return outobj;
    }
    }

   */

    /***********************************************************
     *  Cyc Controled NPC
     *
     **********************************************************/

    public class ReifiableNPC extends	 /*PartiallyfTangible*/ PartiallyTangible {
	/*
public CycFort isa = null;
public ArrayList alsoisa = new ArrayList();
public ArrayList goalMicrotheorys = new ArrayList();
public ArrayList goalObjects = new ArrayList();
public ArrayList knowsHow = new ArrayList();

public ArrayList invokesSelfFeelings = new ArrayList();
public ArrayList prefersSelfFeelings = new ArrayList();
public ArrayList invokesOthersFeelings = new ArrayList();
public ArrayList prefersOthersFeelings = new ArrayList();
public ArrayList primitiveCommandStack = new ArrayList();
public ArrayList goalScriptStack = new ArrayList();
		 */
	public ReifiableNPC() {
	    super();
	}

    }

    /*
    
    commands 
    
    <exit>
    go <exit>
    say <text>
    look
    look <objext>
    [eat|drink] <object>
    throw <object> at <object>
    open <object> with <object>
    put <object> <in|on|under|near> <object>
    get <object>
    drop <object>
    
    
    
    */

    /***********************************************************
     *  Jamud Booting
     *
     **********************************************************/

    private static Area logicmooTopMicrotheory;
    private static Hashtable logicmooTopRooms;
    private static Hashtable logicmooTopArtifacts;
    private static Hashtable logicmooTopBodies;

    private static void startJamud(Object initial) {
	String[] args = { "ini/jamud.xml"} ; 
	try {
	    Jamud.main( args );
	    logicmooInstance = Jamud.currentInstance();
	    //logicmooArea =  logicmooInstance.getArea();
	    //logicmooPlayerManager =  logicmooInstance.getPlayerManager();
	    //logicmooTop = logicmooArea.getDefaultRoom();
	    //logicmooTopMicrotheory = logicmooTop.getArea();
	    //   logicmooTopRooms =  logicmooTopMicrotheory.getRooms();

	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
    }

    public static CycFort nearestFort(String type, Object value ){
	if ( value==null ) return null;
	String test = value.toString().toLowerCase();
	Iterator kws = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$isa ?SEL #$"+type+")").iterator();
	while ( kws.hasNext() ) {
	    Object sample = kws.next();
	    if ( sample instanceof CycFort ) if ( sample.toString().toLowerCase().indexOf(test)==0 ) return(CycFort)sample;
	}
	return null;
    }



    /***********************************************************
     *  Reify Cyc To Jamud
     *
     **********************************************************/


    public static HashMap roomVNumToCycFort = new HashMap();

    public synchronized static void syncMudAll() {

	try {



	    System.out.println("tick syncMudAll");



	    //  logicmooTopRooms =  logicmooTopMicrotheory.getRooms();
	    //   logicmooTopArtifacts =  logicmooTopMicrotheory.getArtifacts();
	    //  logicmooTopBodies =  logicmooTopMicrotheory.getAgentGenerics();



	    System.out.println("syncAllRooms: starting");
	    /*
	    ArrayList cycMicrotheorysL = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$isa ?SEL #$Room)");
	    Iterator roomVrefs =  logicmooTopRooms.entrySet().iterator();
    
	    while (roomVrefs.hasNext()) {
	    Object roomVref = roomVrefs.next();
	    Room room = logicmooTopRooms.get(roomVref);
	    if (!roomVref.equals(room.getCycFortString())) System.out.println("syncAllRooms: "+roomVref+"!="+room.getCycFortString());
	    String roomname = room.getName();
	    String roomfullvnum = room.getFullCycFort();
	    CycFort cycroomconst=roomVNumToCycFort.get(roomfullvnum);
	    if (cycroomconst==null) {
		cycroomconst = makeTypedCycFort("Room",roomname);
		roomVNumToCycFort.put(cycroomconst,roomfullvnum);
		roomVNumToCycFort.put(roomfullvnum,cycroomconst);
	    }
	    }
    
	    Iterator cycMicrotheorys =  cycMicrotheorysL.iterator();
    
	    while (cycMicrotheorys.hasNext()) {
	    CycFort cycMicrotheory = (CycFort) cycMicrotheorys.next();
	    String vnumforMicrotheory = (String)roomVNumToCycFort.get(cycMicrotheory);
	    if (vnumforMicrotheory==null) {
		Room room = new Room(logicmooTopMicrotheory);
		room.setName(attemptParaphrase(cycMicrotheory));
		room.setName(attemptParaphrase(cycMicrotheory));
    
    
	    }
    
	    }
	      */
	    /*
	   syncAllRooms(logicmooTopRooms);
   
   
   
	   syncAllMudExits(logicmooTopRooms);
	   syncAllMudArtifacts(logicmooTopArtifacts);  
   
	   syncAllMudLocations(logicmooTopRooms);
   
	   syncAllDescriptions(allRooms,true);
	   syncAllDescriptions(allMudArtifacts,false);
	   syncAllFlags(allRooms,true);
	   syncAllFlags(allMudArtifacts,false);
	   syncAllKeywords(allRooms,true);
	   syncAllKeywords(allMudArtifacts,false);
   
	   //syncMudFlag(allRooms);
	   //syncMudAttribute();
	   //syncMudKeyword();
	   */
	} catch ( Exception e ) {
	    e.printStackTrace();
	}

	System.out.println("tock syncMudAll");

    }



    // --------------------------------------------------------------------------
    // Synchronize Cyc and Jamud's Known Microtheorys
    // --------------------------------------------------------------------------

    public synchronized void syncAllRooms(Hashtable allRooms) {
	//  ArrayList cycMicrotheorysL = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$isa ?SEL #$Room)");
    }

    // --------------------------------------------------------------------------
    // Synchronize Cyc and Jamud's Known Pathways/Exits
    // --------------------------------------------------------------------------

    public synchronized void syncAllMudExits(HashMap allRooms) {
	System.out.println("syncAllMudExits");
	//     Iterator cycExits = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudExit " + cycobject.stringApiValue() + " ?SEL)").iterator();
    }

    public void assertRoomExits(CycFort cycobject, Iterator exits, boolean clearfirst) {
	try {
	    assertSlotValue(jamudMt,cycobject,"mudExit",exits,clearfirst);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }

    // --------------------------------------------------------------------------
    // Synchronize Cyc and Jamud's Known Atritfacts
    // --------------------------------------------------------------------------

    public synchronized void syncAllMudArtifacts(HashMap allMudArtifacts) {
	System.out.println("syncAllMudArtifacts");
	ArrayList cycArtifactsL = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$isa ?SEL #$MudArtifact)");
    }


    public synchronized  static CycFort makeCycFort(Object obj) {

	if ( obj instanceof CycFort ) {
	    return(((CycFort)obj));
	}

	if ( obj instanceof PartiallyTangible ) {
	    return makeCycFort(getObjNameCyc(obj));
	}

	if ( obj instanceof CycList ) {
	    return new CycNart((CycList)obj);
	}

	if ( obj instanceof String ) {
	    String sobj = (String)obj;
	    if ( sobj.indexOf('(')<0 ) {
		return makeCycConstantSafe((String)obj);
	    } else {
		try {
		    return makeCycFort((CycList)(new CycListKifParser(staticAccess)).read(sobj));
		} catch ( Exception e ) {

		}
	    }
	}
	return null;

    }



    public  String cyclify(Object obj) {

	if ( obj instanceof CycFort ) {
	    return(((CycFort)obj).cycListApiValue().toString());
	}

	if ( obj instanceof PartiallyTangible ) {
	    return makeCycFort(getObjNameCyc(obj)).stringApiValue();
	}

	if ( obj instanceof CycList ) {
	    return(((CycList)obj).stringApiValue());
	}

	if ( obj instanceof String ) {
	    String sobj = (String)obj;
	    if ( sobj.indexOf('(')<0 ) {
		return(makeCycConstantSafe((String)obj).stringApiValue());
	    } else {
		try {
		    return cyclify(((CycList)((new CycListKifParser(staticAccess)).read(sobj))));
		} catch ( Exception e ) {

		}
	    }
	}

	return obj.toString();
    }


    //6239075950(49)

    public synchronized void syncAllMudLocations(HashMap allMudArtifacts) {
	System.out.println("syncAllMudLocations");
	//  Iterator cycLocations = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudObjectFoundInMicrotheory " + cycobject.stringApiValue() + " ?SEL)").iterator();

	//   ArrayList mudArtifactsL = new ArrayList(allMudArtifacts.keySet());
    }

    public synchronized void syncAllDescriptions(HashMap allMudArtifacts,boolean isarea) {
	System.out.println("syncAllDescriptions");
	//       Iterator cycDescs = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudDescription " + cycobject.stringApiValue() + " ?SEL)").iterator();
    }

    public synchronized void syncAllFlags(HashMap allMudArtifacts, boolean isarea) {
	System.out.println("syncAllFlags" );
	//      Iterator cycFlags = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudFlag " + cycobject.stringApiValue() + " ?SEL)").iterator();
    }

    public synchronized void syncAllKeywords(HashMap allMudArtifacts, boolean isarea) {
	System.out.println("syncAllKeywords" );
	//   Iterator cycKeywords = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudKeyword " + cycobject.stringApiValue() + " ?SEL)").iterator();
    }

    public synchronized void syncAllMudAttributes(HashMap allMudArtifacts, boolean isarea) {
	System.out.println("syncAllMudAttributes" );

	/*  ArrayList mudArtifactsL = new ArrayList(allMudArtifacts.keySet());
	  Iterator mudArtifacts =  mudArtifactsL.iterator();
	  
	  while ( mudArtifacts.hasNext() ) {
	  String mudArtifactName = mudArtifacts.next().toString();
	  PartiallyTangible mudArtifactObject = (PartiallyTangible) allMudArtifacts.get(mudArtifactName); 
	  CycFort cycobject = (CycFort)getObjNameCyc(mudArtifactObject);
	  Iterator cycAttributes = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudAttribute " + cycobject.stringApiValue() + " ?SEL)").iterator();
	  assertObjectAttributes(cycobject,mudArtifactObject.attributes(),true);
	  while ( cycAttributes.hasNext() ) {
	  Object cycAttribute = cycAttributes.next();
	  if ( cycAttribute instanceof CycList ) {
	      Object direction = ((CycList)cycAttribute).second();
	      Object cycto = ((CycList)cycAttribute).third();
	      createAttributeIfNew(mudArtifactObject,findObject((CycFort)cycto),cycto.toString());
	  }
	  if ( cycAttribute instanceof CycNart ) {
	      Object direction = ((CycNart)cycAttribute).toCycList().second();
	      Object cycto = ((CycNart)cycAttribute).toCycList().third();
	      createAttributeIfNew(mudArtifactObject,findObject((CycFort)cycto),cycto.toString());
	  }
	  }
	  }
	  */
    }


    public void assertObjectAttributes(CycFort cycobject,Object attribs, boolean clearfirst) {

    }


    public synchronized static void assertIsaSafe(CycFort cycobject, CycFort cycclass, CycFort cycmt) {
	try {
	    staticAccess.assertIsa(cycobject,cycclass,cycmt);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }


    public synchronized static CycConstant makeCycConstantSafe(String constname) {
	if ( constname==null ) return null;
	try {
	    return staticAccess.makeCycConstant(constname);
	} catch ( Exception e ) {
	    return null;
	}
    }



    /***********************************************************
     *  Find In Jamud
     *
     **********************************************************/

    /***********************************************************
     *  Create In Jamud
     *
     **********************************************************/

    /***********************************************************
     * Java Editing
     *
     **********************************************************/


    public synchronized    String findNamedProperty(Object target, String property) {
	// Normally more deeper analysis then string concatination (that is why the target object is here)
	return "set" + toProperCase(property);
    }

    public synchronized    boolean attemptJavaSetMethod(Object target,String property,Object value) {
	if ( value==null ) {
	    try {
		target.getClass().getMethod(findNamedProperty(target,property),null).invoke(target,null);
		return true;
	    } catch ( Exception e ) {
		return false;
	    }

	} else {

	    Class[] sc = { value.getClass()};
	    Object[] sa = { value};

	    try {
		target.getClass().getMethod(findNamedProperty(target,property),sc).invoke(target,sa);
		return true;
	    } catch ( Exception e ) {
		return false;
	    }
	}
    }

    public synchronized    Method findJavaMethod(Class cls, String methodname)
    throws NoSuchMethodError {
	Method[] meths = cls.getMethods();
	int len = meths.length;
	for ( int i = 0; i < len ; i++ )
	    if ( meths[i].getName().equals(methodname) ) return meths[i];

	throw new NoSuchMethodError(methodname +" for " + cls.getName());
    }

    /***********************************************************
     * Java Typing
     *
     **********************************************************/


    public synchronized    Object makeInstanceFromClass(String theType,Object value)
    throws Exception {
	if ( value==null ) {
	    Class newClass = Class.forName(theType);	// Can Throw ClassNotFound
	    return newClass.newInstance();    // Can Throw Instanciation
	} else {
	    Class newClass = Class.forName(theType);
	    Class[] sc = { value.getClass()};
	    Object[] sa = { value};

	    try {
		return newClass.getConstructor(sc).newInstance(sa);    //  throw == second chance
	    } catch ( Exception e ) {
		// Second Chance
		return newClass.newInstance();	  // Can Throw Instanciation
	    }
	}
    }

    /***********************************************************
     * Strings
     *
     **********************************************************/

    public synchronized    String toProperCase(String property) {
	return property.substring(0,0).toUpperCase()+property.substring(1).toLowerCase();
    }




    /***********************************************************
     *  Jamud To Cyc
     *
     **********************************************************/
    public static Object getObjNameCyc(Object object) {
	return staticAccess.makeCycJavaObject( jamudMt, object, false);
    }

    public Object makeCycJavaObject(CycFort dataMt,Object object) {
	return makeCycJavaObject( dataMt, object, true);
    }

    public Object makeCycJavaObject(CycFort dataMt,Object object, boolean assertobj) {
	if ( object==null ) return null;
	if ( cycKnowsObjectAsConstant.containsKey(object) ) return cycKnowsObjectAsConstant.get(object);
	if ( object instanceof CycFort ) return object;
	if ( object instanceof CycList ) return object;
	if ( object instanceof String )	return Strings.change((String)object,"\"","\\\"");
	if ( object instanceof Boolean )  if ( object.equals(Boolean.TRUE) ) return cycTrue;
	    else return cycFalse;
	if ( object instanceof Character ) return  new String("`" + object);
	if ( object instanceof Integer ) return object;
	if ( object instanceof Long ) return object;
	if ( object instanceof Double )	return object;
	if ( object instanceof Float ) return object;
	if ( object instanceof Byte ) return(Integer)object;
	Class jclass = object.getClass();
	if ( !(jclass.isArray()) ) {
	    if ( jclass.isPrimitive() ) {

		System.out.println("\n\n PRIMITIVE!!!!\n\n PRIMITIVE!!!!\n\n PRIMITIVE!!!!");
		if ( jclass == java.lang.Boolean.TYPE )	if ( object.equals(Boolean.TRUE) ) return cycTrue;
		    else return cycFalse;
		if ( jclass == java.lang.Integer.TYPE )	return new Integer(""+object);
		if ( jclass == java.lang.Byte.TYPE ) return new Integer(""+object);
		if ( jclass == java.lang.Void.TYPE ) return cycVoid;
		if ( jclass == java.lang.Long.TYPE ) return new Long(""+object);
		if ( jclass == java.lang.Character.TYPE ) return  new String("`" + object);
		if ( jclass == java.lang.Double.TYPE ) return new Double(""+object);
		if ( jclass == java.lang.Float.TYPE ) return new Float(""+object);
	    }
	}

	String classname = jclass.getName();
	if ( classname.startsWith("org.opencyc") ) return object;

	CycFort cycobject = null;

	cycobject =  makeCycConstantSafe("HYP-"+classname.substring(0,classname.length()-8)+object.hashCode());

	String stringKey =  ""+object.hashCode();
	if ( cycKnowsObjectAsConstant.containsKey(stringKey) ) return cycKnowsObjectAsConstant.get(stringKey);

	cycKnowsObjectAsConstant.put(stringKey,cycobject);

	CycFort cycclass = makeCycClassInstance(jclass);

	try {

	    assertIsaSafe((CycFort)cycobject,(CycFort)cycclass,dataMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}

	// Only make cyc constant for object 
	if ( assertobj ) assertObjectData(dataMt,object,cycobject);
	return cycobject;
    }

    public void assertObjectData(CycFort dataMt, Object object,CycFort cycobject) {
	System.out.println("assertObjectData " + object );

	if ( object instanceof CycList ) {
	    assertWithTranscriptNoWffCheckJava(((CycList)object).stringApiValue(), dataMt);
	    return;
	}

	Class jclass = object.getClass();
	if ( jclass.isArray() ) {
	    assertArrayData(dataMt,object, cycobject);
	    return;
	}

	if ( object instanceof Iterator ) {
	    assertIteratorData(dataMt,(Iterator)object, cycobject);
	    return;
	}

	assertObjectDataFromTemplate(jclass,dataMt,object, cycobject);

    }

    public void  assertObjectDataFromTemplate(Class jclass,CycFort dataMt, Object object,CycFort cycobject) {
	HashMap template = (HashMap)thisClassTemplates.get(jclass);
	Iterator it = template.keySet().iterator();
	while ( it.hasNext() ) {
	    Object cycdataaccess = it.next();
	    if ( cycdataaccess instanceof CycConstant ) {
		assertMemberValue( dataMt, cycobject, object ,(CycConstant)cycdataaccess, template.get(cycdataaccess));
	    }

	}
    }

    public void assertArrayData(CycFort dataMt, Object object,CycFort cycobject) {
	System.out.println("assertArrayData " + object );
	CycList assertme = new CycList(cycHasArrayMember);
	assertme.add(cycobject);
	assertme.add(null);
	assertme.add(null);
	for ( int i=0 ; i < ((Object[])object).length; i++ ) {
	    Object submember = makeCycJavaObject(dataMt,((Object[])object)[i]);
	    try {
		assertme.set(2,new Integer(i));
		assertme.set(3,submember);
		staticAccess.assertWithTranscriptNoWffCheck(assertme,dataMt);
	    } catch ( Exception e ) {
		e.printStackTrace(System.err);
	    }
	}
    }

    public void assertIteratorData(CycFort dataMt, Iterator object,CycFort cycobject) {
	System.out.println("assertIteratorData " + object );
	CycList assertme = new CycList(cycHasArrayMember);
	assertme.add(cycobject);
	assertme.add(3,new Integer(0));
	assertme.add(null);
	while ( object.hasNext() ) {
	    Object submember = makeCycJavaObject(dataMt,object.next());
	    try {
		assertme.set(3,submember);
		staticAccess.assertWithTranscriptNoWffCheck(assertme,dataMt);
	    } catch ( Exception e ) {
		e.printStackTrace(System.err);
	    }
	}
    }

    public static void assertSlotValue(CycFort dataMt,CycFort cycobject, Object slot, Object value, boolean singlevalued) {
	assertSlotValue(dataMt,cycobject,slot,value,null,singlevalued);
    }


    public static void assertSlotValue(CycFort dataMt,CycFort cycobject, Object slot, Object value, Object type, boolean singlevalued) {
	CycConstant cycslot = null;
	if ( cycobject==null ) {
	    System.out.println("assertSlotValue(CycFort " + dataMt + ",CycConstant " +cycobject+", Object " +slot+", Object " +value+", boolean " +singlevalued +")");
	    return;
	}

	if ( slot instanceof CycConstant ) {
	    cycslot = (CycConstant)slot;
	} else {
	    if ( slot instanceof String ) {
		cycslot = staticAccess.makeTypedCycFort(dataMt,"JavaSlot",(String)slot);
	    }
	}

	if ( singlevalued ) staticAccess.clearSlot(dataMt,cycobject,cycslot);

	if ( value == null ) return;

	if ( value instanceof Iterator ) {
	    while ( ((Iterator)value).hasNext() )
		assertSlotValue(dataMt,cycobject, cycslot, ((Iterator)value).next(), type,false);
	    return;
	}
	if ( value instanceof Enumeration ) {
	    while ( ((Enumeration)value).hasMoreElements() )
		assertSlotValue(dataMt,cycobject, cycslot, ((Enumeration)value).nextElement(),type, false);
	    return;
	}

	if ( value.getClass().isArray() ) {
	    assertSlotValue(dataMt,cycobject, cycslot, Arrays.asList((Object[])value).iterator(), type, false);
	    return;
	}
	Object cycvalue = staticAccess.makeCycJavaObject(dataMt,value,false);
	
	if (type!=null) {
	    if (cycvalue instanceof CycFort ) {
		assertIsaSafe((CycFort)cycvalue,(CycFort)makeCycFort(type),dataMt);
	    }
	    if (cycvalue instanceof CycList ) {
		assertIsaSafe(((CycFort)new CycNart((CycList)cycvalue)),(CycFort)makeCycFort(type),dataMt);
	    }
	}
	
	if ( cycvalue!=null ) {
	    try {
		staticAccess.assertGafNoWff(dataMt,cycslot,cycobject,cycvalue);
	    } catch ( Exception e ) {
		e.printStackTrace(System.out);
		System.out.println("assertSlotValue(CycFort " + dataMt + ",CycConstant " +cycobject+", Object " +slot+", Object " +value+", boolean " +singlevalued +")");
	    }
	}
    }

    public void assertMemberValue(CycFort dataMt, CycFort cycobject, Object object, CycConstant cycaccess, Object accessmember) {
	System.out.println("while {assertObjectData " + cycobject + " " + cycaccess + " " + accessmember + "}" );
	try {
	    if ( accessmember instanceof DataMethod ) assertDataMethodResult( dataMt, cycobject, object, cycaccess, (DataMethod)accessmember);
	    //      if ( accessmember instanceof Method ) assertMethodResult( dataMt, cycobject, object, cycaccess, (Method)accessmember);
	    if ( accessmember instanceof Field ) assertFieldValue( dataMt, cycobject, object, cycaccess, (Field)accessmember);
	} catch ( Exception e ) {
	    e.printStackTrace( System.out);
	}
    }

    public void assertFieldValue(CycFort dataMt, CycFort cycobject, Object object, CycConstant cycaccess, Field accessmember) 
    throws Exception{
	CycList assertme = new CycList(cycHasSlotValue);    //"#$hasFieldValue"
	assertme.add(cycobject);
	assertme.add(cycaccess);
	assertme.add(makeCycJavaObject(dataMt,accessmember.get(object)));
	staticAccess.assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void assertMethodResult(CycFort dataMt, CycFort cycobject, Object object, CycConstant cycaccess, Method accessmember) 
    throws Exception{
	CycList assertme = new CycList(cycHasMethod);	 //"#$hasMethodValue"
	assertme.add(cycaccess);
	assertme.add(makeCycJavaObject(dataMt,accessmember.invoke(object,null)));
	staticAccess.assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void assertDataMethodResult(CycFort dataMt, CycFort cycobject, Object object, CycConstant cycaccess, DataMethod accessmember) 
    throws Exception{
	CycList assertme = new CycList(cycHasSlotValue);    //"#$hasMethodValue"
	assertme.add(cycobject);
	assertme.add(cycaccess);
	assertme.add(makeCycJavaObject(dataMt,accessmember.get(object)));
	staticAccess.assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public synchronized static void clearSlot(CycFort dataMt,CycFort cycobject, Object cycslot) {
	// Delete all previous
	CycList query = new CycList(cycslot);
	query.add(cycobject);
	CycVariable cv = new CycVariable("Prev");
	query.add(cv);

	try {
	    Iterator result =  staticAccess.askWithVariable(query,cv,dataMt).iterator();
	    while ( result.hasNext() ) {
		query.set(2,result.next());
		staticAccess.deleteGaf(query,dataMt);
	    }
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}

    }




    public synchronized CycFort makeCycClassInstance(Class jclass) {
	if ( jclass==null ) return cycNull;
	CycConstant cycjclass  = (CycConstant)cycKnowsClass.get(jclass);
	if ( cycjclass!=null ) return cycjclass;
	String classname = jclass.getName();
	if ( classname.startsWith("[L") ) classname = classname.substring(2);
	if ( classname.startsWith("[") ) classname = classname.substring(1);
	if ( classname.endsWith(";") ) classname = classname.substring(0,classname.length()-1);
	//String packagename = jclass.getPackage().getName();
	String classextension = "Instance";
	if ( jclass.isPrimitive() ) {
	    /*
	    jboolean z;
	    jbyte    b;
	    jchar    c;
	    jshort   s;
	    jint     i;
	    jlong    j;
	    jfloat   f;
	    jdouble  d;
	    jobject  l;
	    */    
	    if ( jclass.isArray() ) {
		switch ( classname.charAt(0) ) {
		    case 'Z':
			classname = "boolean";
		    case 'B':
			classname = "byte";
		    case 'C':
			classname = "char";
		    case 'S':
			classname = "short";
		    case 'I':
			classname = "int";
		    case 'J':
			classname = "long";
		    case 'F':
			classname = "float";
		    case 'D':
			classname = "double";
		}
		classextension = "Array";
	    } else {
		classextension = "Value";
	    }
	} else {
	    try {
		if ( jclass.isArray() )	return new CycNart((CycFort)(cycArrayOfClass),(Object)makeCycClassInstance(Class.forName(classname)));
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	    if ( classname.startsWith("java")
		 || classname.startsWith("logicmoo")
		 || classname.startsWith("org")
		 //    || classname.startsWith("com")
	       ) {
		int lp = classname.lastIndexOf(".");
		if ( lp>2 ) classname = classname.substring(lp+1);
	    }
	}

	classname = Strings.change(classname,".","_");
	classname = Strings.change(classname,"$","_");

	String cycclassname = classname + classextension;

	System.out.println("cycclassname =" + cycclassname);

	try {

	    cycjclass =  makeCycConstantSafe(cycclassname);    
	} catch ( Exception e ) {
	    System.out.println("makeCycConstantSafe: " +cycclassname+" "+ e );
	    e.printStackTrace(System.err);
	}
	cycKnowsClass.put(jclass,cycjclass);
	cycKnowsClass.put(cycjclass,jclass);

	// Save Isa
	try {

	    assertIsaSafe(cycjclass,      staticAccess.cycClassInstance,javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	try {
	    // Make super classes
	    Class superjclass = jclass.getSuperclass();
	    if ( superjclass != null ) {
		CycFort cycsuperjclass = makeCycClassInstance(superjclass);
		staticAccess.assertGenls(cycjclass,cycsuperjclass,javaMt);
	    }
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	// Decide if we should make a template
	String classstring = jclass.toString();
	if ( classstring.startsWith("class java.lang") 
	     || classstring.startsWith("class java.io")
	     || classstring.startsWith("class logicmoo.")
	     || classstring.startsWith("class com.logicmoo")
	     || jclass.isPrimitive() )
	    return cycjclass;

	HashMap template = new HashMap();

	Method[] methods = jclass.getMethods();
	for ( int i =0; i<methods.length;i++ ) {
	    Method method = methods[i];
	    CycFort cycmethodjclass = makeCycClassInstance(method.getReturnType());
	    String methodname = method.getName();
	    CycConstant cycmethod = makeTypedCycFort("JavaMethod",methodname + "_method");
	    template.put(cycmethod,method);
	    Class[] params = method.getParameterTypes();
	    assertIsaJavaMethodOf(cycjclass,jclass,cycmethod,methodname,params,cycmethodjclass,method,template);
	}

	Field[] fields = jclass.getFields();
	for ( int i =0; i<fields.length;i++ ) {
	    Field field=fields[i];
	    CycFort cycfieldjclass = makeCycClassInstance(field.getType());
	    CycConstant cycfield = makeTypedCycFort("JavaSlot",field.getName() + "_field");
	    template.put(cycfield,field);
	    assertIsaJavaFieldOf(cycjclass,cycfield,cycfieldjclass);
	}

	thisClassTemplates.put(jclass,template);
	thisClassTemplates.put(cycjclass,template);
	return cycjclass;
    }

    public void assertIsaJavaFieldOf(CycConstant cycjclass,CycConstant cycfield,CycFort cycfieldjclass) {
	try {

	    staticAccess.assertWithTranscriptNoWffCheck(
						       "(#$relationAllExists "  
						       + " " + cycfield.stringApiValue()
						       + " " + cycjclass.stringApiValue()
						       + "  " + cycfieldjclass.stringApiValue() +  " )",(CycFort) javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
    }

    public void assertIsaJavaDataMethodOf(CycConstant cycjclass,CycConstant cycdatamethod, CycFort cycmethodjclass) {
	try {

	    staticAccess.assertWithTranscriptNoWffCheck(
						       "(#$relationAllExists " 
						       + " " + cycdatamethod.stringApiValue()
						       + " " + cycjclass.stringApiValue()
						       + "  " + cycmethodjclass.stringApiValue() +  " )",(CycFort) javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
    }

    public void  assertWithTranscriptNoWffCheckJava(String sentence, CycFort mt) {
	try {

	    staticAccess.assertWithTranscriptNoWffCheck( sentence,mt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}

    }

    public void assertIsaJavaMethodOf(CycConstant cycjclass,Class jclass,CycConstant cycmethod,String methodname,Class[] params, CycFort cycmethodjclass,Method method,HashMap template) {

	staticAccess.assertWithTranscriptNoWffCheckJava(
						       "(#$hasJavaMethod " + cycjclass.stringApiValue()
						       + " (#$JavaMethodFn " + cycmethod.stringApiValue() + " " + makeClassInstanceesDef(params) + " ) " 
						       + "  " + cycmethodjclass.stringApiValue() +  " )",(CycFort) javaMt);
	if ( params.length>0 ) return;
	if ( methodname.startsWith("to") ) return;

	if ( methodname.startsWith("get") ) {
	    String dataname = methodname.substring(3);
	    Method setmethod = null;  
	    CycConstant cycdatamethod = null;
	    try {
		setmethod = jclass.getMethod("set"+dataname,(Class[])Array.newInstance(method.getReturnType(),1)); 
		cycdatamethod = makeTypedCycFort("JavaSlot",dataname+ "_getSet");
	    } catch ( Exception e ) {
		cycdatamethod = makeTypedCycFort("JavaSlot",dataname+ "_get");
		setmethod = null;
	    }
	    template.put(cycjclass,new DataMethod(dataname,method,setmethod));
	    assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
	    return;
	}

	if ( methodname.startsWith("child") ||  methodname.endsWith("es") ) {
	    String dataname = methodname;
	    CycConstant cycdatamethod = makeTypedCycFort("JavaSlot",dataname+ "_getAdd");
	    template.put(cycjclass,new DataMethod(dataname,method,null));
	    assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
	    return;
	}

	String firstthree = methodname.substring(0,2);

	if ( methodname.equalsIgnoreCase("iterator") ) {
	    String dataname = methodname;
	    CycConstant cycdatamethod = makeTypedCycFort("JavaSlot",dataname+ "_getIterator");
	    Method setmethod = null;
	    DataMethod datamethod = new DataMethod(dataname,method,setmethod);
	    template.put(cycjclass,datamethod);
	    assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
	    return;
	}


	if ( firstthree.equalsIgnoreCase("rem")
	     || firstthree.equalsIgnoreCase("add") 
	     || firstthree.equalsIgnoreCase("set") 
	     || firstthree.equalsIgnoreCase("clo") 
	     || firstthree.equalsIgnoreCase("cle") 
	     || firstthree.equalsIgnoreCase("ter") 
	     ||          firstthree.equalsIgnoreCase(         "kil")          
	     || firstthree.equalsIgnoreCase("cre") 
	     || firstthree.equalsIgnoreCase("mak") ) return;



	/*
	 if ( !(methodname.endsWith("s")) ) return;
	 String dataname = methodname;
	 CycConstant cycdatamethod = makeTypedCycFort("Slot",dataname+ "_get");
	 Method setmethod = null;
	 DataMethod datamethod = new DataMethod(dataname,method,setmethod);
	 template.put(cycjclass,datamethod);
	 assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
	 */
    }

    public String makeClassInstanceesDef(Class jclass[]) {
	StringBuffer cdefs = new StringBuffer(10);
	try {
	    for ( int i = 0 ; i < jclass.length ; i ++ ) cdefs.append(" ").append(makeCycClassInstance(jclass[i]).stringApiValue());
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	return cdefs.toString();
    }

    public CycConstant makeTypedCycFort(String ctype,String name) {
	return makeTypedCycFort(javaMt, ctype, name);
    }

    public CycConstant makeTypedCycFort(CycFort dataMt, String type,String name) {
	CycConstant nameC =  makeCycConstantSafe(name);
	CycConstant typeC =  makeCycConstantSafe(type);
	try {

	    assertIsaSafe(typeC,     staticAccess.collection,dataMt);
	    assertIsaSafe(nameC,typeC,dataMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	return nameC;
    }


    public synchronized    PrintWriter getPrintWriter(Writer w) {
	if ( w==null ) return new PrintWriter(System.out);
	if ( w instanceof PrintWriter )	return(PrintWriter)w;
	return new PrintWriter(w);
    }


    public final class DataMethod {

	public String dataname = null;
	public Method getmethod = null;
	public Method setmethod = null;
	public DataMethod(String adataname, Method agetmethod, Method asetmethod) {
	    dataname = adataname;  
	    getmethod = agetmethod;  
	    setmethod = asetmethod;  
	}

	public Object get(Object object) {
	    try {
		return getmethod.invoke(object,null);
	    } catch ( Exception e ) {
		return "" + e;
	    }
	}
	public void set(Object object,Object value) {
	    try {
		Object[] param = {value} ;
		setmethod.invoke(object,param);
	    } catch ( Exception e ) {
	    }
	}
    }

    private    class CycMooAgent extends Thread {
	private CycMoo tstaticAccess = null; 
	public CycMooAgent() throws IOException,CycApiException {
	    tstaticAccess = new CycMoo();
	    //tstaticAccess = aCycMoo;
	}
	public void run() {
	    while ( !this.interrupted() ) {
		try {
		    Thread.sleep(500);
		} catch ( java.lang.InterruptedException e ) {
		}
		tstaticAccess.syncMudAll();
	    }
	}
    }
}

//</pre>







