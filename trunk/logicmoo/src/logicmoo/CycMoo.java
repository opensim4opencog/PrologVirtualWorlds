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
import jamud.*;
import jamud.command.*;
import jamud.object.*;
import jamud.object.event.*;
import jamud.util.*;
import com.jamud.commands.olc.*;
import com.jamud.commands.*;
import com.jamud.connections.*;
import com.jamud.communications.*;
import com.jamud.creation.*;
//import com.jamud.mud.*;
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
* @version $Id: CycMoo.java,v 1.6 2002-05-10 19:47:58 dmiles Exp $
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
    private  static  Jamud jamudInstance;
    private  static  MudObjectRoot jamudMudObjectRoot; 
    private  static  MudObject jamudTop;     
    private  static  Object initialObject; 
    private  static  bsh.Interpreter jamudBeanShell;     
    private  static  CycMoo staticAccess=null;
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
    //public  static  HashMap cycKnowsArea = new HashMap();
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
    public  static  MudObjectRoot getMudObjectRoot() {
	staticAccess.start();
	return jamudMudObjectRoot;
    }
    public  static  Jamud getJamud() {
	staticAccess.start();
	return jamudInstance;
    }
    public  static  MudObject getJamudTop() {
	staticAccess.start();
	return jamudTop;
    }
    public  static  CycMoo currentInstance() {
	staticAccess.start();
	return staticAccess;
    }
    public  static  bsh.Interpreter getBeanShell() {
	staticAccess.start();
	return jamudBeanShell;
    }
    public  static  CycAssertionsFactory getCycAssertionsFactory() {
	staticAccess.start();
	return(CycAssertionsFactory)staticAccess;
    }

    /***********************************************************
     *  Initialize all services
     *
     **********************************************************/

    private    synchronized void start() {
	if ( running ) return;
	running = true;
	System.out.println("Starting LogicMOO (CycMoo)");
	Log.makeLog();
	if ( jamudInstance==null ) startJamud(initialObject);
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
	    geographicalRegion =  makeCycConstantSafe("#$GeographicalRegion");
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

	//efnetIRCBot = startCycLBot(efnetIRCBot, "jllykifsh","irc.rt.ru");
	opnIRCBot = startCycLBot(opnIRCBot, "CycLBot","irc.openprojects.net");
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
	    if ( jamudBeanShell==null ) {
		// Interpreter(java.io.Reader in, java.io.PrintStream out, java.io.PrintStream err, boolean interactive) 
		jamudBeanShell = new bsh.Interpreter(new InputStreamReader(System.in),(PrintStream)System.out ,(PrintStream)System.out,false);
		jamudBeanShell.run();
		jamudBeanShell.set("jamudInstance",jamudInstance);
		jamudBeanShell.set("jamudMudObjectRoot",jamudMudObjectRoot);
		jamudBeanShell.set("jamudTop",jamudTop);
		jamudBeanShell.set("staticAccess",staticAccess);
		jamudBeanShell.set("LogicMoo",staticAccess);
		jamudBeanShell.set("CycMoo",staticAccess);
		jamudBeanShell.set("jllykifsh",efnetIRCBot);
		jamudBeanShell.set("CycLBot",opnIRCBot);
		jamudBeanShell.set("interpretor",jamudBeanShell);
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


    public synchronized    void syncBot() {
	System.out.println("tick syncBot");
	try {
	    syncMudAll();
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	System.out.println("tock syncBot");
    }

    /***********************************************************
     *  Jamud Booting
     *
     **********************************************************/


    private static void startJamud(Object initial) {
	String[] args = { "jamud.xml"} ; 
	try {
	    Jamud.main( args );
	    jamudInstance = Jamud.currentInstance();
	    jamudMudObjectRoot = jamudInstance.mudObjectRoot();
	    jamudTop = (MudObject) jamudMudObjectRoot.getChildObject(AREA_HOLDER);

	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
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
	return  jamudBeanShell.eval(cmd);
    }

    public synchronized  static  void set(String name, Object obj) throws bsh.EvalError  {
	jamudBeanShell.set(name,obj);
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


    public synchronized    String mudExec(MudObject actor, String args) {
	System.out.println("mudExec " + actor.getName()+ " " + args);
	return "ok.";
	/*
	if ( args==null ) return "nothing to do (null)";
	MudObject newplace = findObject(args);
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

    public synchronized    String mudExec(MudObject actor, String lctok, String[] params) {
	if ( lctok.equals("cd") ) return moveTo(actor,params);
	return lctok + " not recognized";
	//if (lctok.equals("dumproom")) 
    }

    public synchronized    String moveTo(MudObject actor, String[] params) {
	String place = actor.getName()+"-home";
	if ( params.length==2 ) {
	    place = params[1];
	}
	MudObject newplace = findObject(place);
	if ( newplace==null ) return "cannot find  " + place;
	actor.setParentContainer(newplace.childContainer());
	return "you are now at " + actor.getParentContainer().parentObject().getName();

    }

    /*
    public synchronized  static  class MudObject extends MudObject {

    // Default Constructor
    public MudObject() {
    super();
    }

    public MudObject(Race race, Gender gender, Job job) {
    super(race,gender,job);
    }
    public synchronized  static  MudObject asCycMudObject(MudObject object) {
    if (object instanceof MudObject) return (MudObject)object;
    MudObject outobj = new MudObject();
    object.copycat(outobj);
    return outobj;
    }
    }

   */

    /***********************************************************
     *  Cyc Controled NPC
     *
     **********************************************************/

    public class MudObjectNPC extends MudObject {

	public CycFort isa = null;
	public ArrayList alsoisa = new ArrayList();
	public ArrayList goalAreas = new ArrayList();
	public ArrayList goalObjects = new ArrayList();
	public ArrayList knowsHow = new ArrayList();

	public ArrayList invokesSelfFeelings = new ArrayList();
	public ArrayList prefersSelfFeelings = new ArrayList();
	public ArrayList invokesOthersFeelings = new ArrayList();
	public ArrayList prefersOthersFeelings = new ArrayList();
	public ArrayList primitiveCommandStack = new ArrayList();
	public ArrayList goalScriptStack = new ArrayList();

	public MudObjectNPC() {
	    super();
	}

	public Command getNextCommand() {
	    return null;
	}           

	public void selfEnact(Command cmd) {

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
     *  Reify Cyc To Jamud
     *
     **********************************************************/

    public synchronized    void queryCycToJamudAll() {
	reifyAllAreas();
	// TODO queryCycToJamudAllActors();
    }

    public synchronized void syncMudAll() {

	HashMap allMudArtifacts = new HashMap(); HashMap allMudAreas = new HashMap();
	getChildrenFrom(jamudMudObjectRoot, allMudAreas, allMudArtifacts);    

	syncAllMudAreas(allMudAreas);
	syncAllMudEntrances(allMudAreas);
	syncAllMudArtifacts(allMudArtifacts);  

	syncAllMudLocations(allMudArtifacts);

	syncAllDescriptions(allMudAreas,true);
	syncAllDescriptions(allMudArtifacts,false);
	syncAllFlags(allMudAreas,true);
	syncAllFlags(allMudArtifacts,false);
	syncAllKeywords(allMudAreas,true);
	syncAllKeywords(allMudArtifacts,false);

	//syncMudFlag(allMudAreas);
	//syncMudAttribute();
	//syncMudKeyword();
    }

    // --------------------------------------------------------------------------
    // Test if something is an Area
    // --------------------------------------------------------------------------
    public static boolean isArea(MudObject obj) {
	if ( obj==null ) return false;
	if ( obj.flags().contains("MudArea") 
	     || obj.getKeywords().contains("MudArea")
	     || obj.isFlagged("MudArea")
	     || obj.getRace().equals("MudArea")
	     || obj.getJob().equals("MudArea")
	   ) return true;

	MudObjectContainer objcont = obj.childContainer();
	if ( objcont==null ) return false;
	if ( objcont.exits().hasNext() ) return true;
	return false;
    }


    public void getChildrenFrom(MudObjectContainer cont, HashMap isaArea, HashMap isaArtifact) {
	//System.out.println("getChildrenFrom( " + cont + ",  " + isaArea + ",  " + isaArtifact + ",  "+ isaEntrance +")");
	if ( cont==null ) return;
	Iterator childs = cont.childObjects();
	while ( childs.hasNext() ) {
	    MudObject child = (MudObject) childs.next();
	    MudObjectContainer  subcont =  child.childContainer();
	    Object cycname = getObjNameCyc(child);
	    String name = cycname.toString();
	    child.setName(name);
	    try {
		child.setTemplateId(name);
	    } catch ( TemplateConflictException e ) {
	    }
	    if ( subcont!=null ) {
		subcont.setName(name);
	    }
	    if ( isArea(child) ) {
		isaArea.put(name,child);
	    } else {
		isaArtifact.put(name,child);
	    }
	    getChildrenFrom(subcont,isaArea, isaArtifact);
	}
    }

    // --------------------------------------------------------------------------
    // Synchronize Cyc and Jamud's Known Areas
    // --------------------------------------------------------------------------

    public synchronized void syncAllMudAreas(HashMap allMudAreas) {
	System.out.println("syncAllMudAreas");
	ArrayList mudAreasL = new ArrayList(allMudAreas.keySet());
	ArrayList cycAreasL = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$isa ?SEL #$MudArea)");
	Iterator cycAreas =  cycAreasL.iterator();
	Iterator mudAreas =  mudAreasL.iterator();

	while ( cycAreas.hasNext() ) {
	    CycFort cycArea = (CycFort)cycAreas.next();
	    if ( !mudAreasL.contains(cycArea.toString()) )
		allMudAreas.put(cycArea.toString(),reifyAsNewArea(cycArea));
	}

	while ( mudAreas.hasNext() ) {
	    String mudArea = mudAreas.next().toString();
	    CycFort cycArea = makeCycConstantSafe(mudArea);

	    if ( !cycAreasL.contains(mudArea) ) {
		MudObject area = (MudObject)allMudAreas.get(mudArea);
		if ( area!=null ) {
		    assertExistingArea(jamudMt,area);
		    allMudAreas.put(mudArea,area);
		}
	    }
	}

    }

    // --------------------------------------------------------------------------
    // Synchronize Cyc and Jamud's Known Pathways/Entrances
    // --------------------------------------------------------------------------

    public synchronized void syncAllMudEntrances(HashMap allMudAreas) {
	System.out.println("syncAllMudEntrances");

	ArrayList mudAreasL = new ArrayList(allMudAreas.keySet());
	Iterator mudAreas =  mudAreasL.iterator();

	while ( mudAreas.hasNext() ) {
	    String mudAreaName = mudAreas.next().toString();
	    MudObject mudAreaObject = (MudObject) allMudAreas.get(mudAreaName); 
	    CycFort cycobject = (CycFort)getObjNameCyc(mudAreaObject);
	    Iterator cycEntrances = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudExit " + cycobject.stringApiValue() + " ?SEL)").iterator();
	    assertRoomExits(cycobject,mudAreaObject.childContainer().exits(),true);
	    while ( cycEntrances.hasNext() ) {
		Object cycEntrance = cycEntrances.next();
		if ( cycEntrance instanceof CycList ) {
		    Object direction = ((CycList)cycEntrance).second();
		    Object cycto = ((CycList)cycEntrance).third();
		    createPathwayIfNew(mudAreaObject,findObject((CycFort)cycto),direction.toString());
		}
		if ( cycEntrance instanceof CycNart ) {
		    Object direction = ((CycNart)cycEntrance).toCycList().second();
		    Object cycto = ((CycNart)cycEntrance).toCycList().third();
		    createPathwayIfNew(mudAreaObject,findObject((CycFort)cycto),direction.toString());
		}
	    }
	}
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
	ArrayList mudArtifactsL = new ArrayList(allMudArtifacts.keySet());
	ArrayList cycArtifactsL = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$isa ?SEL #$MudArtifact)");
	Iterator cycArtifacts =  cycArtifactsL.iterator();
	Iterator mudArtifacts =  mudArtifactsL.iterator();

	while ( cycArtifacts.hasNext() ) {
	    CycFort cycArtifact = (CycFort)cycArtifacts.next();
	    if ( !mudArtifactsL.contains(cycArtifact.toString()) ) {
		System.out.print("telling jamud about " + cycArtifact.cyclify());
		MudObject newobj =  reifyArtifactObject(cycArtifact);
		System.out.println(newobj);
		allMudArtifacts.put(cycArtifact.toString(),newobj);
	    }
	}

	while ( mudArtifacts.hasNext() ) {
	    String mudArtifactName = mudArtifacts.next().toString();
	    if ( !cycArtifactsL.contains(mudArtifactName) ) {
		System.out.println("telling cyc about " + mudArtifactName);
		MudObject mudObject = (MudObject)allMudArtifacts.get(mudArtifactName);
		CycFort cycobject = (CycFort)makeCycJavaObject(jamudMt,mudObject,false);
		allMudArtifacts.put(cycobject.toString(),mudObject);
	    }
	}
    }


    public  CycFort cyclifyFort(Object obj) {

	if ( obj instanceof CycFort ) {
	    return(((CycFort)obj));
	}

	if ( obj instanceof Viewable ) {
	    return cyclifyFort(getObjNameCyc(obj));
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
		    return cyclifyFort((CycList)(new CycListKifParser(staticAccess)).read(sobj));
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

	if ( obj instanceof Viewable ) {
	    return cyclifyFort(getObjNameCyc(obj)).stringApiValue();
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

	ArrayList mudArtifactsL = new ArrayList(allMudArtifacts.keySet());
	Iterator mudArtifacts =  mudArtifactsL.iterator();

	while ( mudArtifacts.hasNext() ) {
	    String mudArtifactName = mudArtifacts.next().toString();
	    MudObject mudArtifactObject = (MudObject) allMudArtifacts.get(mudArtifactName); 
	    CycFort cycobject = (CycFort)getObjNameCyc(mudArtifactObject);
	    System.out.print("locating " + cycobject.cyclify() + " " );
	    MudObjectContainer locationContainer = mudArtifactObject.getParentContainer();
	    System.out.print("with parent " + locationContainer + " " );
	    MudObject location = null;
	    CycFort cycmudlocation =null;
	    if ( locationContainer!=null ) {
		// Update Cyc
		location = locationContainer.parentObject();
		if ( location!=null ) {
		    cycmudlocation = (CycFort)getObjNameCyc(location);
		    assertSlotValue(jamudMt,cycobject,"mudObjectFoundInArea",cycmudlocation,true);
		}
		System.out.println(""+cycmudlocation + " ");
	    } else {
		Iterator cycLocations = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudObjectFoundInArea " + cycobject.stringApiValue() + " ?SEL)").iterator();
		if ( cycLocations.hasNext() ) {
		    CycFort cycthinks = null;
		    Object cycLocation = cycLocations.next();
		    if ( cycLocation instanceof CycConstant ) {
			cycthinks = ((CycConstant)cycLocation);
		    }
		    if ( cycLocation instanceof CycList ) {
			cycthinks = new CycNart(((CycList)cycLocation));
		    }
		    if ( cycLocation instanceof CycNart ) {
			cycthinks = ((CycNart)cycLocation);
		    }
		    MudObject cycthinksParent =  findObject(cycthinks);
		    System.out.println(" "+cycthinks + " " +cycthinksParent + " ");
		    if ( cycthinksParent!=null ) {
			MudObjectContainer cycthinkscont = cycthinksParent.childContainer();
			if ( cycthinkscont!=null ) {
			    mudArtifactObject.setParentContainer(cycthinkscont);
			} else System.out.println(" cyc wants it to be in a non Area ");
		    } else System.out.println(" cyc points a missing object ");
		} else System.out.println(" cyc still does not know ");
	    }

	}    // End while
    }

    public synchronized void syncAllDescriptions(HashMap allMudArtifacts,boolean isarea) {
	System.out.println("syncAllDescriptions");
	ArrayList mudArtifactsL = new ArrayList(allMudArtifacts.keySet());
	Iterator mudArtifacts =  mudArtifactsL.iterator();

	while ( mudArtifacts.hasNext() ) {
	    String mudArtifactName = mudArtifacts.next().toString();
	    MudObject mudArtifactObject = (MudObject) allMudArtifacts.get(mudArtifactName); 
	    CycFort cycobject = (CycFort)getObjNameCyc(mudArtifactObject);
	    MudObjectContainer mudArtifactObjectChild =  mudArtifactObject.childContainer();
	    String mudArtifactDesc = null;

	    if ( isarea && mudArtifactObjectChild!=null ) {
		mudArtifactDesc = mudArtifactObjectChild.getDescription();
	    } else {
		mudArtifactDesc = mudArtifactObject.getDescription();
	    }

	    mudArtifactDesc = mudArtifactObject.getDescription();

	    Iterator cycDescs = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudDescription " + cycobject.stringApiValue() + " ?SEL)").iterator();
	    while ( cycDescs.hasNext() ) {
		String thisCycDec = cycDescs.next().toString();
		if ( mudArtifactDesc.indexOf(thisCycDec)>-1 ) continue;
		if ( thisCycDec.indexOf(mudArtifactDesc)>-1 ) mudArtifactDesc = thisCycDec;
		// mudArtifactDesc = mudArtifactDesc + " " + thisCycDec;
	    }

	    assertSlotValue(jamudMt,cycobject,"mudDescription",mudArtifactDesc,true);

	    if ( isarea && mudArtifactObjectChild!=null ) {
		mudArtifactObjectChild.setDescription(mudArtifactDesc);
	    } else {
		mudArtifactObject.setDescription(mudArtifactDesc);
	    }
	}
    }

    public synchronized void syncAllFlags(HashMap allMudArtifacts, boolean isarea) {
	System.out.println("syncAllFlags" );
	ArrayList mudArtifactsL = new ArrayList(allMudArtifacts.keySet());
	Iterator mudArtifacts =  mudArtifactsL.iterator();

	while ( mudArtifacts.hasNext() ) {
	    String mudArtifactName = mudArtifacts.next().toString();
	    MudObject mudArtifactObject = (MudObject) allMudArtifacts.get(mudArtifactName); 
	    CycFort cycobject = (CycFort)getObjNameCyc(mudArtifactObject);
	    FlagList mudArtifactFlags = mudArtifactObject.flags(); 

	    Iterator cycFlags = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudFlag " + cycobject.stringApiValue() + " ?SEL)").iterator();
	    while ( cycFlags.hasNext() ) {
		String thisCycFlag = cycFlags.next().toString();
		if ( !mudArtifactFlags.contains(thisCycFlag) ) {
		    mudArtifactFlags.add(thisCycFlag); 
		}
		assertSlotValue(jamudMt,cycobject,"mudFlag",mudArtifactFlags.iterator(),true);
	    }
	}
    }

    public synchronized void syncAllKeywords(HashMap allMudArtifacts, boolean isarea) {
	System.out.println("syncAllKeywords" );
	ArrayList mudArtifactsL = new ArrayList(allMudArtifacts.keySet());
	Iterator mudArtifacts =  mudArtifactsL.iterator();

	while ( mudArtifacts.hasNext() ) {
	    String mudArtifactName = mudArtifacts.next().toString();
	    MudObject mudArtifactObject = (MudObject) allMudArtifacts.get(mudArtifactName); 
	    CycFort cycobject = (CycFort)getObjNameCyc(mudArtifactObject);
	    FlagList mudArtifactKeywords = mudArtifactObject.getKeywords(); 

	    Iterator cycKeywords = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudKeyword " + cycobject.stringApiValue() + " ?SEL)").iterator();
	    while ( cycKeywords.hasNext() ) {
		String thisCycKeyword = cycKeywords.next().toString();
		if ( !mudArtifactKeywords.contains(thisCycKeyword) ) {
		    mudArtifactKeywords.add(thisCycKeyword); 
		}
		assertSlotValue(jamudMt,cycobject,"mudKeyword",mudArtifactKeywords.iterator(),true);
	    }
	}
    }

    public synchronized void syncAllMudAttributes(HashMap allMudArtifacts, boolean isarea) {
	System.out.println("syncAllMudAttributes" );

	/*  ArrayList mudArtifactsL = new ArrayList(allMudArtifacts.keySet());
	  Iterator mudArtifacts =  mudArtifactsL.iterator();
	  
	  while ( mudArtifacts.hasNext() ) {
	  String mudArtifactName = mudArtifacts.next().toString();
	  MudObject mudArtifactObject = (MudObject) allMudArtifacts.get(mudArtifactName); 
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


    public void assertObjectAttributes(CycFort cycobject,Attributes attribs, boolean clearfirst) {

    }

    /*
    ;;  isa MudThingType
    ;;  isa MudArea, MudEntrance, MudArtifact, MudBeing 
    
    ;; (mudObjectFoundInArea ?MudArtifactOrMudBeing  ?MudArea)
    ;; (mudPathwayToArea ?Area1  ?Area1 ?MudEntrance)
    ;; (mudInstance ?MT ?Obj ?MudThingType)
    ;; (mudDescription ?Obj ?TextString)
    ;; (mudFlag ?Obj ?Flag)  ;; Attribt
    ;; (mudAttribute ?Obj ?Attrib ?Value)
    ;; (mudKeyword ?Obj  ?TextString ) ;; isa's
    ;; (mudChild ?Obj  ?Obj )
    */


    // --------------------------------------------------------------------------
    // Reify Cyc #$GeographicalRegion(s) To Jamud
    // --------------------------------------------------------------------------
    public synchronized    void reifyAllAreas() {
	Iterator places =  staticAccess.getArrayListQuerySEL("EverythingPSC","(#$isa ?SEL #$MudArea)").iterator();
	while ( places.hasNext() ) reifyAsNewArea((CycFort) places.next());
    }

    // --------------------------------------------------------------------------
    // Reify Single Area (reifyAreaObjectAndExits and 'objectFoundInLocation's) 
    // --------------------------------------------------------------------------
    public synchronized    MudObject reifyAsNewArea(CycFort cycplace) {
	// Make sure Area Object Exists
	MudObject area = reifyAreaObjectAndExits(cycplace);
	// Make sure All objects found in location exisdts in Mud
	Iterator objects =  staticAccess.getArrayListQuerySEL("EverythingPSC","(#$mudObjectFoundInArea "+ cycplace.stringApiValue() +" ?SEL)").iterator();
	while ( objects.hasNext() ) reifyObject((CycFort) objects.next());
	return area;
    }

    // --------------------------------------------------------------------------
    // Reify Single Object (All types: Area, Artifact)
    // --------------------------------------------------------------------------
    public synchronized    MudObject reifyObject(CycFort cycobject) {
	if ( cycobject==null ) return null;
	// (#$isa ?SEL #$GeographicalRegion)
	try {
	    if ( isa(cycobject,geographicalRegion) ) return reifyAsNewArea(cycobject);
	} catch ( Exception io ) {    //CycApiException IOException
	    io.printStackTrace();
	}

	return reifyArtifactObject(cycobject);
    }

    public synchronized static void assertIsaSafe(CycFort cycobject, CycFort cycclass, CycFort cycmt) {
	try {
	    staticAccess.assertIsa(cycobject,cycclass,cycmt);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }


    // --------------------------------------------------------------------------
    // Reify Single Artifact Object (will create if not found)
    // --------------------------------------------------------------------------
    public synchronized    MudObject reifyArtifactObject(CycFort cycobject) {
	if ( cycobject==null ) return null;
	MudObject object = findObject(cycobject);
	if ( object==null ) {
	    object = makeArtifactObject(cycobject);
	}
	assertIsaSafe(cycobject,makeCycConstantSafe("#$MudArtifact"),jamudMt);
	return object;
	//Basic
    }

    public synchronized static CycConstant makeCycConstantSafe(String constname) {
	if ( constname==null ) return null;
	try {
	    return staticAccess.makeCycConstant(constname);
	} catch ( Exception e ) {
	    return null;
	}
    }


    // --------------------------------------------------------------------------
    // Reify Unknown Artifact Object
    // --------------------------------------------------------------------------
    public synchronized    MudObject makeArtifactObject(CycFort cycobject) {
	if ( cycobject==null ) return null;
	String objName= cycobject.toString();
	MudObject object = new MudObject();
	assertIsaSafe(cycobject,makeCycConstantSafe("#$MudArtifact"),jamudMt);
	queryUpdateViewable(object,cycobject);
	object.setName(objName);
	object.setShortDescription(objName);
	return object;
    }

    // --------------------------------------------------------------------------
    // Reify Single Area's Object and Exits
    // --------------------------------------------------------------------------
    public synchronized    MudObject reifyAreaObjectAndExits(CycFort cycplace) {
	if ( cycplace==null ) return null;
	MudObject area = reifyAreaObjectOnly(cycplace);
	// Get and add internal objects
	//reifyAreaPortals(cycplace);
	return area;
    }

    // --------------------------------------------------------------------------
    // Reify Single Area's Object Onlky
    // --------------------------------------------------------------------------
    public synchronized    MudObject reifyAreaObjectOnly(CycFort cycplace) {
	MudObject area = findObject(cycplace);
	if ( area!=null ) return area;
	String areaName= cycplace.toString();
	// If area was not made then make it
	area = createAreaObject(areaName);     
	// get and set name
	area.setName(areaName);
	// Get and add description
	queryUpdateViewable(area,cycplace);
	return area;
    }

    // --------------------------------------------------------------------------
    // Reify Long and Short Descriptions
    // --------------------------------------------------------------------------
    public synchronized    void queryUpdateViewable(Viewable object,CycFort cycobject) {
	if ( object==null || cycobject==null ) return;
	FlagList kw = new FlagList();

	String cycname = cycobject.toString();
	String cycpara = attemptParaphrase(cycobject);

	kw.add(cycname);
	MudObjectContainer contaner = null;
	if ( object instanceof MudObject ) contaner = ((MudObject)object).childContainer();

	object.setShortDescription(cycpara);

	StringBuffer desc = new StringBuffer("");

	Iterator answers =  staticAccess.getArrayListQuerySEL("EverythingPSC","(#$comment "+cycobject.stringApiValue()+" ?SEL)").iterator();
	while ( answers.hasNext() ) desc.append(answers.next().toString());

	String oisa = null;

	Iterator isas =  staticAccess.getArrayListQuerySEL("EverythingPSC","(#$nearestIsa "+cycobject.stringApiValue()+" ?SEL)").iterator();

	while ( isas.hasNext() ) {
	    oisa = isas.next().toString();
	    desc.append("\nIs " + oisa );
	    kw.add(oisa);
	    while ( isas.hasNext() ) {
		oisa = isas.next().toString();
		desc.append(", " + oisa);
		kw.add(oisa);
	    }
	}

	object.setKeywords(new FlagList(kw));
	String longdesc = Strings.change(desc.toString(),"#$","");
	object.setDescription(longdesc);
	if ( contaner!=null ) {
	    contaner.setName(cycname);
	    contaner.setShortDescription(cycpara);
	    contaner.setDescription(longdesc);
	    contaner.setKeywords(new FlagList(kw));
	    //  System.out.println(cycobject+"-> " + longdesc);
	}
	// Get and append descriptivness
    }

    // --------------------------------------------------------------------------
    // Reify Single Area Exits
    // --------------------------------------------------------------------------
    public synchronized    void reifyAreaPortals(CycFort cycplace) {
	/*
	 if ( cycplace==null ) return;
	 reifyAreaPortals("EverythingPSC","(#$geographicalSubRegions "+cycplace.stringApiValue()+" ?SEL)",
	      "local transport into",cycplace);
	 reifyAreaPortals("EverythingPSC","(#$and (#$isa (#$BorderBetweenFn ?W ?SEL) #$Border) (#$equals ?W "+cycplace.stringApiValue()+"))",
	      "border transport into",cycplace);
	 reifyAreaPortals("EverythingPSC","(#$and (#$isa (#$BorderBetweenFn ?SEL ?W ) #$Border) (#$equals ?W "+cycplace.stringApiValue()+"))",
	      "border transport into",cycplace);
	 reifyAreaPortals("EverythingPSC","(#$geographicalSubRegions ?SEL "+cycplace.stringApiValue()+")",
	      "regional transport out to",cycplace);
	      */
    }

    public synchronized    void  reifyAreaPortals(String mt,String query,String desc,CycFort cycplace) {
	ArrayList regionList =  staticAccess.getArrayListQuerySEL(mt,query);
	System.out.println(""+cycplace+" " + desc + " " + regionList.size());
	Iterator regions = regionList.iterator();
	while ( regions.hasNext() ) {
	    CycFort cycdest = (CycFort) regions.next();
	    if ( !cycplace.equals(cycdest) ) reifyPathway(cycplace,cycdest,desc);
	}
    }

    // --------------------------------------------------------------------------
    // Reify Single Area Exits
    // --------------------------------------------------------------------------
    public synchronized    Entrance reifyPathway(CycFort cycsource,CycFort cyctarget,String desc) {
	Entrance pathway =  createPathwayIfNew(reifyAreaObjectOnly(cycsource),reifyAreaObjectOnly(cyctarget),desc);
	// TODO Add more information to pathway before returning it
	return pathway;
    }

    /***********************************************************
     *  Find In Jamud
     *
     **********************************************************/

    // --------------------------------------------------------------------------
    // Locate Object
    // --------------------------------------------------------------------------
    public synchronized    MudObject findObject(CycFort cycconst) {
	if ( cycconst==null ) return null;
	return findObject(cycconst.toString());
    }
    // --------------------------------------------------------------------------
    // Locate Object
    // --------------------------------------------------------------------------
    public synchronized    MudObject findObject(String objectName) {
	if ( objectName==null )	return null;
	return findObject(jamudMudObjectRoot, objectName);
    }


    // --------------------------------------------------------------------------
    // Locate Object
    // --------------------------------------------------------------------------
    //private static HashMap objectFindHash = new HashMap();

    public synchronized    MudObject findObject(MudObjectContainer  scopearea, String objectName) {
	if ( objectName==null )	return null;
	//MudObject object = (MudObject)objectFindHash.get(objectName); 
	//  if ( object!=null ) return object;
	MudObject object = findObjectNoCache(scopearea,objectName); 
	// objectFindHash.put(objectName,object);
	return object; 
    }


    public  synchronized    MudObject findObjectNoCache(MudObjectContainer  scopearea, String objectName) {
	// System.out.println("findObject " + objectName + " in " + scopearea.getName() );
	if ( objectName==null )	return null;
	if ( scopearea == null || objectName==null ) return null;

	MudObject object = /*MudObject.asCycMudObject*/(scopearea.getChildObject(objectName));
	if ( object!=null ) return object;

	Iterator i = scopearea.childObjects();
	while ( i.hasNext() ) {
	    object = findObjectNoCache(((MudObject) i.next()).childContainer(),objectName);
	    if ( object!=null )	return object;
	}
	return null;
    }

    /***********************************************************
     *  Create In Jamud
     *
     **********************************************************/

    // --------------------------------------------------------------------------
    // Create Single Area
    // --------------------------------------------------------------------------

    public synchronized    MudObject  createAreaObject(String areaName) {
	return createAreaObject(jamudTop, areaName);
    }

    public synchronized    MudObject createAreaObject(MudObject parent,  String areaName) {

	if ( areaName==null || areaName.length()<1 )
	    areaName = "new area in " + parent.getName();

	MudObject newAreaObj = /*MudObject.asCycMudObject*/( MudObject.getMudObjectTemplate(areaName));
	if ( newAreaObj!=null )	return newAreaObj;


	// Make  area object 
	newAreaObj = new MudObject();
	// Make  child container object 
	MudObjectContainer newAreaChildContainer = new MudObjectContainer(newAreaObj);
	// get and set tid
	/*      try {
	newAreaObj.isTemplate(true);
	} catch ( TemplateConflictException e ) {
	e.printStackTrace();
	}
	*/
	try {
	    newAreaObj.setTemplateId(areaName);
	} catch ( TemplateConflictException e ) {
	    e.printStackTrace();
	}
	try {
	    newAreaObj.isTemplate(true);
	} catch ( TemplateConflictException e ) {
	    e.printStackTrace();
	}
	newAreaObj.setName(areaName);
	newAreaObj.setShortDescription("area object for " + areaName);
	newAreaObj.setDescription("Area object for " + areaName);
	newAreaObj.setParentContainer(parent.childContainer());

	// Make  area ChildContainer 
	FlagList fl2 = new FlagList();
	fl2.add(areaName);
	newAreaChildContainer.setName(areaName);
	newAreaChildContainer.setShortDescription("child container for " + areaName);
	newAreaChildContainer.setDescription("Child container for " + areaName);
	//ensureUnique(newAreaChildContainer);
	newAreaChildContainer.setKeywords(fl2);
	newAreaObj.setKeywords(fl2);
	return newAreaObj;
    }

    // --------------------------------------------------------------------------
    // Create Single One-way Exit
    // --------------------------------------------------------------------------
    public synchronized    Entrance createPathwayIfNew(MudObject source,MudObject target,String desc) {
	if ( source==null || target==null || desc==null ) return null;
	Entrance pathway = null; 
	MudObjectContainer targetContainer = target.childContainer();
	MudObjectContainer sourceContainer = source.childContainer();

	if ( targetContainer==null || sourceContainer==null ) return null;

	// Check Exits
	Iterator exits = sourceContainer.exits();
	while ( exits.hasNext() ) {
	    pathway = (Entrance) exits.next();
	    if ( pathway==null ) continue;
	    MudObjectContainer pdest = pathway.getDestination();
	    if ( pdest==null ) continue;
	    if ( pdest.equals(targetContainer) ) return pathway;
	}

	try {
	    pathway = new Entrance(targetContainer);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    pathway.setDestinationMessage("came from " + source.getName());
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    pathway.setOriginMessage("you are on your way to " +target.getName());
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    pathway.setKeywords(new FlagList(target.getName()));
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    pathway.setName(target.getName());
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    pathway.setShortDescription(desc + " " + target.getName());
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    pathway.setDescription("from " + source.getShortDescription() + " " + desc + " " + target.getShortDescription());
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    pathway.setType(         Entrance.TYPE_OPEN);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    pathway.setState(Entrance.STATE_OPEN);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	try {
	    sourceContainer.addExit(pathway);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	return pathway;  
    }

    /***********************************************************
     * Attribute Editing
     *
     **********************************************************/

    public synchronized    boolean addProperty(MudObjectViewable target, String property)
    throws JamudException {
	return addProperty( target,  property,  "" );
    }

    public synchronized    boolean addProperty(MudObjectViewable target, String property, Object value)
    throws JamudException {
	return setProperty(target,  property , value);
    }

    public synchronized    boolean setProperty(Object target, String property, Object value)
    throws JamudException {

	if ( attemptJavaSetMethod(target,property,value) )
	    return true;

	if ( target instanceof MudObject ) {
	    ((MudObject)target).attributes().setProperty(property,value.toString());
	    return true;
	}

	throw new JamudException("The type " + target.getClass().getName() + " did not have the interface to add the property " +value );

    }

    public synchronized    boolean unsetProperty(MudObjectViewable target, String property)
    throws JamudException {
	return removeProperty(  target,  property);
    }

    public synchronized    boolean removeProperty(MudObjectViewable target, String property)
    throws JamudException {
	if ( property==null || property.length()<1 )
	    throw new JamudException("removeProperty: no property name ");

	if ( target instanceof MudObject ) {
	    ((MudObject)target).attributes().remove(property) ;
	    return true;
	}
	throw new JamudException("The type " + target.getClass().getName() + " did not have the interface to remove the property " +property );

    }


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
     *  Flag Editing
     *
     **********************************************************/

    public synchronized    boolean flaglistEdit(MudObjectViewable target, String flagname , boolean value )
    throws JamudException {
	if ( flagname==null || flagname.length()<1 )
	    throw new JamudException("removeProperty: no property name ");
	FlagList fl = ((MudObject)target).flags();
	if ( value ) {
	    if ( !fl.contains(flagname) ) fl.add(flagname);
	    return true;
	} else {
	    if ( fl.contains(flagname) ) fl.remove(flagname);
	    return true;
	}
    }

    public synchronized    boolean destroyObject(MudObjectViewable target)
    throws JamudException {
	throw new JamudException("destroyObject: not immplemented ");
    }

    /***********************************************************
     * Java Typing
     *
     **********************************************************/


    public synchronized    Object mktype(String theType,String theData)
    throws JamudException {
	try {
	    if ( theType.equals("Long") )
		return new java.lang.Long(theData);
	    if ( theType.equals("Integer") )
		return new java.lang.Integer(theData);
	    if ( theType.equals("Short") )
		return new Short(theData);
	    if ( theType.equals("Float") )
		return new java.lang.Float(theData);
	    if ( theType.equals("Byte") )
		return new Byte(theData);
	    if ( theType.equals("Boolean") )
		return new Boolean(theData);
	    if ( theType.equals("Char") || theType.equals("Character") )
		return new Character(theData.charAt(0));
	    if ( theType.equals("Date") )
		return new MudDate(Long.parseLong(theData));
	    if ( theType.equals("Class") )
		return Class.forName(theData);
	    if ( theType.equals("String") ) return theData;

	    return makeInstanceFromClass( theType, theData);

	} catch ( Exception e ) {
	    throw new JamudException("Cannot construct "+ theType +" from \"" + theData + "\" (" + e + ")" );
	}
    }



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
     *  Messaging
     *
     **********************************************************/

    public synchronized    void inform(MudObject actor, String msg) {
	if ( ((Object)actor) instanceof Player ) {
	    actor.println(msg);
	} else {
	    System.out.println("  BuilderCommand: tells object " + actor.getName() + " \"" + msg + "\"");
	}
    }



    /***********************************************************
     *  Jamud To Cyc
     *
     **********************************************************/
    public Object getObjNameCyc(Object object) {
	return makeCycJavaObject( jamudMt, object, false);
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
	if ( object instanceof MudObjectContainer )
	    return makeCycJavaObject(dataMt,((MudObjectContainer)object).parentObject() ,assertobj);
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

	if ( object instanceof MudObject ) {
	    MudObjectContainer  subcont =  ((MudObject)object).childContainer();
	    String name = ((MudObject)object).getName();
	    if ( subcont!=null ) {
		if ( name.length()<2 ) {
		    name = subcont.getName();
		}
	    }
	    cycobject = makeCycConstantSafe(name);
	} else if ( object instanceof MudObjectContainer ) {
	    MudObject  subcont =  ((MudObjectContainer)object).parentObject();
	    String name = ((MudObjectContainer)object).getName();
	    if ( subcont!=null ) {
		if ( name == null ) {
		    name = subcont.getName();
		} else {
		    if ( name.length()<2 ) {
			name = subcont.getName();
		    }
		}
	    }
	    cycobject = makeCycConstantSafe(name);

	} else if ( object instanceof Entrance ) {
	    String pathname =  ((Entrance)object).getName();
	    MudObjectContainer dest =  ((Entrance)object).getDestination();
	    Object cycpath = makeCycJavaMember(dataMt,"MudPath",pathname);
	    Object cycdest = getObjNameCyc(dest);

	    if ( cycpath==null ) cycpath = makeCycConstantSafe("Down");
	    if ( cycdest==null ) cycdest = makeCycConstantSafe(AREA_HOLDER);
	    cycobject = new CycNart(makeCycConstantSafe("#$ExitToFn"),new CycList(cycpath,cycdest));
	
	} else if ( object instanceof Viewable ) {
	    cycobject =  makeCycConstantSafe(((Viewable)object).getName());
	} else {
	    cycobject =  makeCycConstantSafe("HYP-"+classname.substring(0,classname.length()-8)+object.hashCode());
	}

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

	if ( object instanceof MudObjectViewable ) {
	    assertMudObject(dataMt,(MudObject)object, cycobject);
	    return;
	}
	if ( object instanceof MudObjectContainer ) {
	    assertMudObject(dataMt,((MudObjectContainer)object).parentObject() ,cycobject);
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


    public void assertJamudAll() {

	staticAccess.assertJamud(     staticAccess.jamudMt,    staticAccess.getJamud());
    }

    // Assert Entire Mud
    public void assertJamud(CycFort dataMt, Jamud aJamud) {
	MudObjectRoot theJamudRoot = aJamud.currentInstance().mudObjectRoot();

	Iterator bellowRoot =  theJamudRoot.childObjects();
	while ( bellowRoot.hasNext() ) {
	    MudObject area = (MudObject)bellowRoot.next();
	    assertExistingArea(dataMt, area);
	}
	bellowRoot =  jamudTop.childContainer().childObjects();
	while ( bellowRoot.hasNext() ) {
	    MudObject area = (MudObject)bellowRoot.next();
	    assertExistingArea(dataMt, area);
	}
    }

    // Assert Room/Object/Players/NPCs into Cyc
    public void assertMudObject(CycFort dataMt, MudObject object,CycFort cycobject) {
	MudObject cobject = /*MudObject.asCycMudObject*/(object);
	if ( object.isTemplate() ) {
	    assertExistingArea(dataMt,cobject,cycobject);
	} else {
	    assertItem(dataMt,cobject,cycobject);
	}
    }

    public MudObject assertExistingArea(CycFort dataMt, MudObject object) {
	assertExistingArea(dataMt,object,(CycFort)getObjNameCyc(object));
	return object;
    }

    public void assertExistingArea(CycFort dataMt, MudObject object,CycFort cycobject) {
	assertMudObjectViewableParts(dataMt,object,cycobject);
	assertAddViewableParts(dataMt,object.childContainer(),cycobject);
	assertInsideParts(dataMt,object,cycobject);
	assertSlotValue(dataMt,cycobject,"mudExit",object.childContainer().exits(),true);
    }

    public void assertItem(CycFort dataMt, MudObject object,CycFort cycobject) {
	assertMudObjectViewableParts(dataMt,object,cycobject);
	assertAddViewableParts(dataMt,(Viewable) object,cycobject);
	assertInsideParts(dataMt,object,cycobject);
    }


    public void assertMudObjectViewableParts(CycFort dataMt, MudObject object,CycFort cycobject) {
	assertSlotValue(dataMt,cycobject,"mudKeyword",((FlagList)object.getKeywords()).iterator(),true);
	assertSlotValue(dataMt,cycobject,"mudObjectFoundInArea",getObjNameCyc(object.getParentContainer()),true);
	assertSlotValue(dataMt,cycobject,"mudFlag",object.flags().iterator(),true);
	//assertSlotValue(dataMt,cycobject,"Gender_getSet",object.getGender().getName(),true);
	//assertSlotValue(dataMt,cycobject,"Job_getSet",object.getJob().getName(),true);
	//assertSlotValue(dataMt,cycobject,"Race_getSet",object.getRace().getName(),true);
    }

    public void assertAddViewableParts(CycFort dataMt, Viewable object,CycFort cycobject) {
	try {

	    assertIsaSafe(cycobject,      staticAccess.makeCycClassInstance(object.getClass()),dataMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	assertSlotValue(dataMt,cycobject,"mudKeyword",((FlagList)object.getKeywords()).iterator(),false);
	assertSlotValue(dataMt,cycobject,"mudDescription",object.getDescription(),false);
	//assertSlotValue(dataMt,cycobject,"ShortDescription_getSet",object.getShortDescription(),false,true);
    }



    public void assertInsideParts(CycFort dataMt, MudObject object,CycFort cycobject) {
	MudObjectContainer container = object.childContainer();
	if ( container==null ) return;
	Iterator objs = container.childObjects();
	while ( objs.hasNext() )
	    makeCycJavaObject(dataMt,objs.next(),true);
    }

    public void assertSlotValue(CycFort dataMt,CycFort cycobject, Object slot, Object value, boolean singlevalued) {
	CycConstant cycslot = null;
	if ( cycobject==null ) {
	    System.out.println("assertSlotValue(CycFort " + dataMt + ",CycConstant " +cycobject+", Object " +slot+", Object " +value+", boolean " +singlevalued +")");
	    return;
	}

	if ( slot instanceof CycConstant ) {
	    cycslot = (CycConstant)slot;
	} else {
	    if ( slot instanceof String ) {
		cycslot = makeCycJavaMember(dataMt,"JavaSlot",(String)slot);
	    }
	}

	if ( singlevalued ) clearSlot(dataMt,cycobject,cycslot);

	if ( value == null ) return;

	if ( value instanceof Iterator ) {
	    while ( ((Iterator)value).hasNext() )
		assertSlotValue(dataMt,cycobject, cycslot, ((Iterator)value).next(), false);
	    return;
	}

	if ( value.getClass().isArray() ) {
	    assertSlotValue(dataMt,cycobject, cycslot, Arrays.asList((Object[])value).iterator(), false);
	    return;
	}
	Object cycvalue = makeCycJavaObject(dataMt,value,false);
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

    public void clearSlot(CycFort dataMt,CycFort cycobject, Object cycslot) {
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
		 || classname.startsWith("jamud")
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
	     || classstring.startsWith("class jamud.")
	     || classstring.startsWith("class com.jamud")
	     || jclass.isPrimitive() )
	    return cycjclass;

	HashMap template = new HashMap();

	Method[] methods = jclass.getMethods();
	for ( int i =0; i<methods.length;i++ ) {
	    Method method = methods[i];
	    CycFort cycmethodjclass = makeCycClassInstance(method.getReturnType());
	    String methodname = method.getName();
	    CycConstant cycmethod = makeCycJavaMember("JavaMethod",methodname + "_method");
	    template.put(cycmethod,method);
	    Class[] params = method.getParameterTypes();
	    assertIsaJavaMethodOf(cycjclass,jclass,cycmethod,methodname,params,cycmethodjclass,method,template);
	}

	Field[] fields = jclass.getFields();
	for ( int i =0; i<fields.length;i++ ) {
	    Field field=fields[i];
	    CycFort cycfieldjclass = makeCycClassInstance(field.getType());
	    CycConstant cycfield = makeCycJavaMember("JavaSlot",field.getName() + "_field");
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
		cycdatamethod = makeCycJavaMember("JavaSlot",dataname+ "_getSet");
	    } catch ( Exception e ) {
		cycdatamethod = makeCycJavaMember("JavaSlot",dataname+ "_get");
		setmethod = null;
	    }
	    template.put(cycjclass,new DataMethod(dataname,method,setmethod));
	    assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
	    return;
	}

	if ( methodname.startsWith("child") ||  methodname.endsWith("es") ) {
	    String dataname = methodname;
	    CycConstant cycdatamethod = makeCycJavaMember("JavaSlot",dataname+ "_getAdd");
	    template.put(cycjclass,new DataMethod(dataname,method,null));
	    assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
	    return;
	}

	String firstthree = methodname.substring(0,2);

	if ( methodname.equalsIgnoreCase("iterator") ) {
	    String dataname = methodname;
	    CycConstant cycdatamethod = makeCycJavaMember("JavaSlot",dataname+ "_getIterator");
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
	 CycConstant cycdatamethod = makeCycJavaMember("Slot",dataname+ "_get");
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

    public CycConstant makeCycJavaMember(String ctype,String name) {
	return makeCycJavaMember(javaMt, ctype, name);
    }

    public CycConstant makeCycJavaMember(CycFort dataMt, String type,String name) {
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
		tstaticAccess.syncBot();
	    }
	}
    }
}

//</pre>







