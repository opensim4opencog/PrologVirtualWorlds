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
* @version $Id: CycMoo.java,v 1.5 2002-05-04 21:26:40 dmiles Exp $
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

    private static CycMooAgent thisCycMooAgent = null;
    private static Jamud jamudInstance;
    private static MudObjectRoot jamudMudObjectRoot; 
    private static MudObject jamudTop;     
    private static Object initialObject; 
    private static bsh.Interpreter jamudBeanShell;     
    private static CycMoo staticAccess=null;
    private static MooIrcBot efnetIRCBot=null;
    private static MooIrcBot opnIRCBot=null;
    private static boolean running = false;



    public static CycConstant logicMooMt = null;
    public static CycConstant jamudMt = null;
    public static CycConstant javaMt = null;
    public static CycConstant cycadministrator = null;
    public static CycConstant opencycproject = null;
    public static CycConstant functionalRelation = null;

    public static CycConstant cycVoid = null;
    public static CycConstant cycNull = null;
    public static CycConstant cycHasArrayMember = null;
    public static CycConstant cycHasMethod = null;
    public static CycConstant cycHasSlot = null;
    public static CycConstant cycClassInstance = null;
    public static CycConstant cycHasSlotValue = null;

    private static org.opencyc.webserver.WebServer cycWebserverThread=null;

    // CycConstant & Class -> CycConstant key of Fields | DataMethod | Method
    public static HashMap thisClassTemplates = new HashMap();

    // CycConstant || Class -> Class || CycConstant 
    public static HashMap cycKnowsClass = new HashMap();
    public static HashMap cycKnowsObject = new HashMap();
    public static HashMap cycKnowsArea = new HashMap();
    public static HashMap cycKnowsExit = new HashMap();
    public static HashMap cycKnowsNPC = new HashMap();
    public static HashMap cycKnowsPlayer = new HashMap();
    public static HashMap cycKnowsInanimate = new HashMap();
    public static String AREA_HOLDER="MIDGAARD";



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
     *  Gets for private static fields
     *
     **********************************************************/

    public static CycAccess getCycAccess() {
	start();
	return(CycAccess)staticAccess;
    }
    public static MudObjectRoot getMudObjectRoot() {
	start();
	return jamudMudObjectRoot;
    }
    public static Jamud getJamud() {
	start();
	return jamudInstance;
    }
    public static MudObject getJamudTop() {
	start();
	return jamudTop;
    }
    public static CycMoo currentInstance() {
	start();
	return staticAccess;
    }
    public static bsh.Interpreter getBeanShell() {
	start();
	return jamudBeanShell;
    }
    public static CycAssertionsFactory getCycAssertionsFactory() {
	start();
	return(CycAssertionsFactory)staticAccess;
    }

    /***********************************************************
     *  Initialize all services
     *
     **********************************************************/

    private static synchronized void start() {
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
	    cycVoid = staticAccess.makeCycConstant("#$voidJava");
	    cycNull = staticAccess.makeCycConstant("#$nullJava");
	    cycHasArrayMember = staticAccess.makeCycConstant("#$javaArrayContains");
	    cycHasMethod = staticAccess.makeCycConstant("#$hasMethod");
	    cycHasSlot = staticAccess.makeCycConstant("#$relationAllExists");
	    cycClassInstance = staticAccess.makeCycConstant("#$ClassInstance");
	    cycHasSlotValue = staticAccess.makeCycConstant("#$hasSlotValue");
	    logicMooMt = staticAccess.makeCycConstant("#$LogicMooMt");
	    jamudMt = staticAccess.makeCycConstant("#$JamudMt");
	    javaMt = staticAccess.makeCycConstant("#$JavaMt");
	    CycFort reifiedMicrotheory = staticAccess.makeCycConstant("#$ReifiedMicrotheory");

	    staticAccess.makeCycConstantError = staticAccess.makeCycConstant("#$MakeCycConstantErrorFn");
	    staticAccess.assertIsa(logicMooMt,staticAccess.microtheory,staticAccess.baseKB);
	    staticAccess.assertIsa(jamudMt,staticAccess.microtheory,staticAccess.baseKB);
	    staticAccess.assertIsa(reifiedMicrotheory,staticAccess.makeCycConstant("#$MicrotheoryType"),staticAccess.baseKB);
	    staticAccess.assertIsa(jamudMt,reifiedMicrotheory,staticAccess.baseKB);
	    staticAccess.assertIsa(javaMt,staticAccess.microtheory,staticAccess.baseKB);
	    staticAccess.assertGenlMt(jamudMt,logicMooMt);
	    staticAccess.assertGenlMt(logicMooMt,"HumanActivitiesMt");
	    staticAccess.assertGenlMt(logicMooMt,"BuildingMt");
	    staticAccess.assertGenlMt(logicMooMt,javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}

	efnetIRCBot = startCycLBot(efnetIRCBot, "Jllykifsh","irc.rt.ru");
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
		jamudBeanShell.set("Jllykifsh",efnetIRCBot);
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


    public synchronized static void syncBot() {
	System.out.println("tick syncBot");
	staticAccess.queryCycToJamudAll();
	staticAccess.assertCycToJamudAll();
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


    public static synchronized void stopCycLBot(MooIrcBot acycIRCBot) {
	try {
	    acycIRCBot.running = false;
	    acycIRCBot.ircDisconnect();
	    acycIRCBot = null;

	} catch ( Exception e ) {
	}
    }

    public static synchronized MooIrcBot startCycLBot(MooIrcBot acycIRCBot, String name,String server) {
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

    public static File ontologyFile(String file) {
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

    public static void constructMtFromFile(String file,String mtName) throws Exception {
	constructMtFromFile(ontologyFile(file),mtName);
    }

    public static void constructMtFromFile(File file,String mtName) throws Exception {
	constructMtFromFile(file,staticAccess.makeCycConstant(mtName));
    }

    public static void constructMtFromFile(String file,CycConstant mtName) throws Exception {
	constructMtFromFile(ontologyFile(file),mtName);
    }

    public static void constructMtFromFile(File file,CycConstant mtName) throws Exception {
	// staticAccess.kill(mtName);
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

    public static void killMt(String mtName) throws IOException, CycApiException {
	staticAccess.kill(staticAccess.makeCycConstant(mtName));
    }


    /***********************************************************
     *  BeanShell Access
     *
     **********************************************************/

    public static Object eval(String cmd) throws bsh.EvalError  {
	return  jamudBeanShell.eval(cmd);
    }

    public static void set(String name, Object obj) throws bsh.EvalError  {
	jamudBeanShell.set(name,obj);
    }


    /***********************************************************
     *  CycL Paraphrase
     *
     **********************************************************/

    public static String attemptParaphrase(Object post) {
	System.out.println("post = " + post);
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
		return staticAccess.converseString("(generate-phrase " + ((CycConstant)post).stringApiValue() +  ")");
	    //        if ( post instanceof CycFort )
	    //		return staticAccess.converseString("(generate-phrase '" + ((CycFort)post).stringApiValue() +  ")");
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

		return staticAccess.converseString("(generate-phrase '" + ((CycList)post).stringApiValue() +  ")");
	    }
	    //          if ( post instanceof CycNart )
	    //		return staticAccess.converseString("(generate-phrase '" + ((CycFort)post).stringApiValue() +  ")");

	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	return post.toString();
    }


    public static String mudExec(MudObject actor, String args) {
	System.out.println("mudExec " + actor.getName()+ " " + args);

	if ( args==null ) return "nothing to do (null)";
	MudObject newplace = findArea(args);
	if ( newplace!=null ) {
	    actor.setParentContainer(newplace.childContainer());
	    actor.enact("look");
	    return "teleported to " + newplace.getName();
	}
	String[] params = Strings.split(args);
	if ( params==null ) return "nothing to do (params==null)";
	return mudExec(actor,params[0].toLowerCase(),params);

    }

    public static String mudExec(MudObject actor, String lctok, String[] params) {
	if ( lctok.equals("cd") ) return moveTo(actor,params);
	return lctok + " not recognized";
	//if (lctok.equals("dumproom")) 
    }

    public static String moveTo(MudObject actor, String[] params) {
	String place = actor.getName()+"-home";
	if ( params.length==2 ) {
	    place = params[1];
	}
	MudObject newplace = findArea(place);
	if ( newplace==null ) return "cannot find  " + place;
	actor.setParentContainer(newplace.childContainer());
	return "you are now at " + actor.getParentContainer().parentObject().getName();

    }

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
     *  Sync Cyc To Jamud
     *
     **********************************************************/

    public static void queryCycToJamudAll() {
	queryAllAreas();
	// TODO queryCycToJamudAllActors();
    }


    // --------------------------------------------------------------------------
    // Sync Cyc #$GeographicalRegion(s) To Jamud
    // --------------------------------------------------------------------------
    public static void queryAllAreas() {
	Iterator places = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$or (#$isa ?SEL #$LandTopographicalFeature) (#$isa ?SEL #$GeographicalRegion))").iterator();
	while ( places.hasNext() ) queryAreaObject((CycFort) places.next());
    }


    // --------------------------------------------------------------------------
    // Sync Single Area Object
    // --------------------------------------------------------------------------
    public static MudObject queryAreaObject(CycFort cycplace) {
	if ( cycplace==null ) return null;
	MudObject area = findArea(cycplace);
	if ( area!=null ) return area;
	area = createAreaObject(cycplace.toString());     
	// get and set name
	area.setName(cycplace.toString());
	// Get and add description
	queryViewable(area,cycplace);
	// Get and add internal objects
	queryCycAreaExits(cycplace);
	return area;
    }

    // --------------------------------------------------------------------------
    // Sync Long and Short Descriptions
    // --------------------------------------------------------------------------
    public static void queryViewable(Viewable area,CycFort cycplace) {

	FlagList kw = new FlagList();
	MudObjectContainer contaner = null;
	if ( area instanceof MudObject ) contaner = ((MudObject)area).childContainer();
	area.setShortDescription("At " + attemptParaphrase(cycplace));
	if ( contaner!=null ) contaner.setShortDescription(attemptParaphrase(cycplace));

	StringBuffer desc = new StringBuffer("");

	Iterator answers = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$comment "+cycplace.stringApiValue()+" ?SEL)").iterator();
	while ( answers.hasNext() ) desc.append(answers.next().toString());

	String oisa = null;

	Iterator isas = staticAccess.getArrayListQuerySEL("EverythingPSC","(#$and (#$nearestIsa "+cycplace.stringApiValue()+" ?SEL) (#$not (#$genls #$Entity ?SEL)) )").iterator();
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
	area.setKeywords(kw);
	String longdesc = Strings.change(desc.toString(),"#$","");
	area.setDescription(longdesc);
	if ( contaner!=null ) contaner.setDescription(longdesc);
	System.out.println(cycplace+"-> " + longdesc);
	// Get and append descriptivness
    }


    // --------------------------------------------------------------------------
    // Sync Single Area Exits
    // --------------------------------------------------------------------------
    public static void queryCycAreaExits(CycFort cycplace) {
	if ( cycplace==null ) return;
	queryCycAreaExits("EverythingPSC","(#$geographicalSubRegions "+cycplace.stringApiValue()+" ?SEL)",
			  "local transport into",cycplace);
	queryCycAreaExits("EverythingPSC","(#$and (#$isa (#$BorderBetweenFn ?W ?SEL) #$Border) (#$equals ?W "+cycplace.stringApiValue()+"))",
			  "border transport into",cycplace);
	queryCycAreaExits("EverythingPSC","(#$and (#$isa (#$BorderBetweenFn ?SEL ?W ) #$Border) (#$equals ?W "+cycplace.stringApiValue()+"))",
			  "border transport into",cycplace);
	queryCycAreaExits("EverythingPSC","(#$geographicalSubRegions ?SEL "+cycplace.stringApiValue()+")",
			  "regional transport out to",cycplace);
    }

    public static void  queryCycAreaExits(String mt,String query,String desc,CycFort cycplace) {
	ArrayList regionList = staticAccess.getArrayListQuerySEL(mt,query);
	System.out.println(""+cycplace+" " + desc + " " + regionList.size());
	Iterator regions = regionList.iterator();
	while ( regions.hasNext() ) {
	    CycFort cycdest = (CycFort) regions.next();
	    if ( !cycplace.equals(cycdest) ) queryPathway(cycplace,cycdest,desc);
	}
    }

    // --------------------------------------------------------------------------
    // Sync Single Area Exits
    // --------------------------------------------------------------------------
    public static Entrance queryPathway(CycFort cycsource,CycFort cyctarget,String desc) {
	Entrance pathway =  createPathway(queryAreaObject(cycsource),queryAreaObject(cyctarget),desc);
	// TODO Add more information to pathway before returning it
	return pathway;
    }

    /***********************************************************
     *  Find In Jamud
     *
     **********************************************************/

    // --------------------------------------------------------------------------
    // Locate Areas
    // --------------------------------------------------------------------------
    public static MudObject findArea(CycFort cycconst) {
	return findArea(cycconst.toString());
    }
    public static MudObject findArea(String name) {
	MudObject area =  jamudMudObjectRoot.getChildObject(AREA_HOLDER).childContainer().getChildObject(name);
	if ( area!=null ) return area;
	area =  jamudMudObjectRoot.getChildObject(name + "-home");
	return area;
    }
    // --------------------------------------------------------------------------
    // Locate Things
    // --------------------------------------------------------------------------
    public static MudObject findThing(CycFort cycconst) {
	return findArea(cycconst.toString());
    }

    public static ArrayList findType(String type) {
	ArrayList found = null;
	if ( type.equals("Homes") ) {
	    found = new ArrayList();
	    Iterator areashome =  jamudMudObjectRoot.childObjects();
	    while ( areashome.hasNext() ) {
		found.add(areashome.next());
	    }
	    return found;
	}
	if ( type.equals("Area") ) {
	    found = findType("Homes");
	    Iterator areas =  jamudMudObjectRoot.getChildObject(AREA_HOLDER).childContainer().childObjects();
	    while ( areas.hasNext() ) {
		found.add(areas.next());
	    }
	    return found;
	}
	if ( type.equals("Roomies") ) {
	    Iterator areas = findType("Area").iterator();
	    while ( areas.hasNext() ) {
		found = new ArrayList();
		MudObject area = (MudObject)areas.next();
		Iterator roomies = area.childContainer().childObjects();
	    }
	    return found;
	}
	return new ArrayList();
    }

    /***********************************************************
     *  Create In Jamud
     *
     **********************************************************/

    // --------------------------------------------------------------------------
    // Create Single Area
    // --------------------------------------------------------------------------

    public static MudObject  createAreaObject(String areaName) {
	return createAreaObject(jamudMudObjectRoot.getChildObject(AREA_HOLDER), areaName);
    }

    public static MudObject createAreaObject(MudObject parent,  String areaName) {

	if ( areaName==null || areaName.length()<1 )
	    areaName = "new area in " + parent.getName();

	// Make  area object 
	MudObject newAreaObj = new MudObject();
	newAreaObj.setName(areaName);
	newAreaObj.setShortDescription("area object for " + areaName);
	newAreaObj.setDescription("Area object for " + areaName);
	ensureUnique(newAreaObj);
	newAreaObj.setParentContainer(parent.childContainer());

	// Make  area ChildContainer 
	FlagList fl2 = new FlagList();
	fl2.add(areaName);
	MudObjectContainer newAreaChildContainer = new MudObjectContainer(newAreaObj);
	newAreaChildContainer.setName(areaName);
	newAreaChildContainer.setShortDescription("child container for " + areaName);
	newAreaChildContainer.setDescription("Child container for " + areaName);
	ensureUnique(newAreaChildContainer);
	newAreaChildContainer.setKeywords(fl2);
	return newAreaObj;
    }

    // --------------------------------------------------------------------------
    // Create Single One-way Exit
    // --------------------------------------------------------------------------
    public static Entrance createPathway(MudObject source,MudObject target,String desc) {
	if ( source==null || target==null ) return null;
	Entrance pathway = null; 
	MudObjectContainer targetContainer = target.childContainer();
	MudObjectContainer sourceContainer = source.childContainer();
	if ( targetContainer==null || sourceContainer==null ) return null;
	Iterator exits = sourceContainer.exits();
	if ( exits==null ) return null;
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
	    pathway.setType(Entrance.TYPE_OPEN);
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

    // --------------------------------------------------------------------------
    // TIDS, LIDS and tracking
    // --------------------------------------------------------------------------
    private static void ensureUnique(MudObjectViewable newThing) {
	if ( newThing instanceof MudObject ) {
	    ((MudObject)newThing).nextLoadId();
	} else if ( newThing instanceof MudObjectContainer ) {
	    // TODO: ((MudObjectContainer)newThing).setTemplateId(...);
	    // ((MudObjectContainer)newThing).setTemplateId(incrementTID(((MudObjectContainer)newThing).getTemplateId()));

	}
    }

    /***********************************************************
     * Attribute Editing
     *
     **********************************************************/

    public static boolean addProperty(MudObjectViewable target, String property)
    throws JamudException {
	return addProperty( target,  property,  "" );
    }

    public static boolean addProperty(MudObjectViewable target, String property, Object value)
    throws JamudException {
	return setProperty(target,  property , value);
    }

    public static boolean setProperty(Object target, String property, Object value)
    throws JamudException {

	if ( attemptJavaSetMethod(target,property,value) )
	    return true;

	if ( target instanceof MudObject ) {
	    ((MudObject)target).attributes().setProperty(property,value.toString());
	    return true;
	}

	throw new JamudException("The type " + target.getClass().getName() + " did not have the interface to add the property " +value );

    }

    public static boolean unsetProperty(MudObjectViewable target, String property)
    throws JamudException {
	return removeProperty(  target,  property);
    }

    public static boolean removeProperty(MudObjectViewable target, String property)
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


    public static String findNamedProperty(Object target, String property) {
	// Normally more deeper analysis then string concatination (that is why the target object is here)
	return "set" + toProperCase(property);
    }

    public static boolean attemptJavaSetMethod(Object target,String property,Object value) {
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

    public static Method findJavaMethod(Class cls, String methodname)
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

    public static boolean flaglistEdit(MudObjectViewable target, String flagname , boolean value )
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

    public static boolean destroyObject(MudObjectViewable target)
    throws JamudException {
	throw new JamudException("destroyObject: not immplemented ");
    }

    /***********************************************************
     * Java Typing
     *
     **********************************************************/


    public static Object mktype(String theType,String theData)
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


    /***********************************************************
     *  Searching
     *
     **********************************************************/

    public static MudObjectViewable getPeerObject(MudObject s,
						  String keyword) {

	int len = keyword.length();

	if ( len==0 )
	    return null;

	int ampersand = keyword.indexOf('@');

	try {
	    switch ( ampersand ) {
		
		case 0:
		    // is @LID
		    return
		    findByLID(s.getParentContainer(),Integer.parseInt(keyword.substring(1)));
		case -1:
		    // is name
		    return
		    findByName( s,  keyword);
		default:
		    if ( len==ampersand ) {
			// is TID@
			return findByTID(s.getParentContainer(),keyword.substring(0,len-1));
		    } else {
			// is TID@LID
			return findByLID(s.getParentContainer(),Integer.parseInt(keyword.substring(ampersand+1)));
		    }
	    }
	} catch ( NumberFormatException e ) {
	    return null;
	}
    }



    public static MudObjectViewable findByLID(MudObjectContainer area, int lid) {
	if ( area == null ) {
	    return null;
	}

	for ( Iterator i = area.childObjects(); i.hasNext(); ) {
	    MudObjectViewable m = (MudObjectViewable) i.next();
	    if ( m instanceof MudObject ) {
		if ( ((MudObject)m).getLoadId()==lid ) return m;
	    } else {
		if ( m instanceof MudObjectContainer ) {
		    if ( ((MudObjectContainer)m).getLoadId()==lid ) return m;
		    MudObjectViewable pos = findByLID((MudObjectContainer)m,lid);
		    if ( pos!=null ) return pos;
		}
	    }
	}
	return null;
    }


    public static  MudObjectViewable findByTID(MudObjectContainer area, String tid) {
	if ( area == null ) {
	    return null;
	}

	for ( Iterator i = area.childObjects(); i.hasNext(); ) {
	    MudObjectViewable m = (MudObjectViewable) i.next();
	    if ( m instanceof MudObject ) {
		if ( ((MudObject)m).isTemplate() && ((MudObject)m).getTemplateId().equalsIgnoreCase(tid) )
		    return m;
	    } else {
		if ( m instanceof MudObjectContainer ) {
		    if ( ((MudObjectContainer)m).isTemplate() && ((MudObjectContainer)m).getTemplateId().equalsIgnoreCase(tid) ) return m;
		    MudObjectViewable pos = findByTID((MudObjectContainer)m,tid);
		    if ( pos!=null ) return pos;
		}
	    }
	}
	return null;

    }

    public static  MudObjectViewable findByName(MudObject s, String keyword) {

	MudObjectContainer  area = s.getParentContainer();
	if ( area == null ) {
	    return null;
	}

	for ( Iterator i = area.childObjects(keyword); i.hasNext(); ) {
	    MudObject m = (MudObject) i.next();
	    if ( m != s && m.matchKeyword(keyword) ) {
		return m;
	    }
	}

	// TODO fix loop bug ;>
	//  if ( keyword.equalsIgnoreCase("staticAccess") && s.attributes().containsKey(BuilderCommand.PROPERTY_OBJECTSCOPE) )
	//    return findByName(s,s.attributes().getAttribute(BuilderCommand.PROPERTY_OBJECTSCOPE,null));

	if ( keyword.equalsIgnoreCase("area") ||
	     keyword.equalsIgnoreCase("parent") )
	    return s.getParentContainer();

	if ( keyword.equalsIgnoreCase("self") ||
	     keyword.equalsIgnoreCase("me") )
	    return s;

	return null;
    }


    public static Object makeInstanceFromClass(String theType,Object value)
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

    public static String toProperCase(String property) {
	return property.substring(0,0).toUpperCase()+property.substring(1).toLowerCase();
    }


    /***********************************************************
     *  Messaging
     *
     **********************************************************/

    public static void inform(MudObject actor, String msg) {
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

    public Object makeCycJavaObject(CycFort dataMt,Object object) {
	return makeCycJavaObject( dataMt, object, true);
    }


    public CycConstant makeCycClassInstance(Class jclass) throws CycApiException {
	if ( jclass==null ) throw new CycApiException("null makeCycClassInstance");
	CycConstant cycjclass  = (CycConstant)cycKnowsClass.get(jclass);
	if ( cycjclass!=null ) return cycjclass;
	String classname = jclass.getName();
	String fclassname = flattenJavaName(classname);
	System.out.println("makeCycClassInstance " + classname + " -> " + fclassname);
	cycjclass = staticAccess.makeCycConstant(fclassname + "Instance");
	cycKnowsClass.put(jclass,cycjclass);
	cycKnowsClass.put(cycjclass,jclass);
	HashMap template = new HashMap();
	thisClassTemplates.put(jclass,template);
	thisClassTemplates.put(cycjclass,template);

	String classstring = jclass.toString();
	if ( classstring.startsWith("class java.lang") 
	     || classstring.startsWith("class java.io")
	     || jclass.isPrimitive() )
	    return cycjclass;

	try {
	    staticAccess.assertIsa(cycjclass,staticAccess.cycClassInstance,javaMt);

	    Class superjclass = jclass.getSuperclass();
	    if ( superjclass != null ) {
		CycConstant cycsuperjclass = makeCycClassInstance(superjclass);
		staticAccess.assertGenls(cycjclass,cycsuperjclass,javaMt);
	    }
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	    throw new CycApiException("makeCycClassInstance: " + e );
	}

	Method[] methods = jclass.getMethods();
	for ( int i =0; i<methods.length;i++ ) {
	    Method method = methods[i];
	    CycConstant cycmethodjclass = makeCycClassInstance(method.getReturnType());
	    String methodname = method.getName();
	    CycConstant cycmethod = makeCycJavaMember("Method",methodname + "_method");
	    template.put(cycmethod,method);
	    Class[] params = method.getParameterTypes();
	    assertIsaJavaMethodOf(cycjclass,jclass,cycmethod,methodname,params,cycmethodjclass,method,template);
	}

	Field[] fields = jclass.getFields();
	for ( int i =0; i<fields.length;i++ ) {
	    Field field=fields[i];
	    CycConstant cycfieldjclass = makeCycClassInstance(field.getType());
	    CycConstant cycfield = makeCycJavaMember("Slot",field.getName() + "_field");
	    template.put(cycfield,field);
	    assertIsaJavaFieldOf(cycjclass,cycfield,cycfieldjclass);
	}

	return cycjclass;
    }

    public void assertIsaJavaFieldOf(CycConstant cycjclass,CycConstant cycfield,CycConstant cycfieldjclass) {
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

    public void assertIsaJavaDataMethodOf(CycConstant cycjclass,CycConstant cycdatamethod, CycConstant cycmethodjclass) {
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

    public void assertIsaJavaMethodOf(CycConstant cycjclass,Class jclass,CycConstant cycmethod,String methodname,Class[] params, CycConstant cycmethodjclass,Method method,HashMap template) {
	staticAccess.assertWithTranscriptNoWffCheckJava(
						       "(#$hasJavaMethod " + cycjclass.stringApiValue()
						       + " (#$JavaMethodAccessFn " + cycmethod.stringApiValue() + " " + makeClassInstanceesDef(params) + " ) " 
						       + "  " + cycmethodjclass.stringApiValue() +  " )",(CycFort) javaMt);
	if ( params.length>0 ) return;
	if ( methodname.startsWith("to") ) return;

	if ( methodname.startsWith("get") ) {
	    String dataname = methodname.substring(3);
	    CycConstant cycdatamethod = makeCycJavaMember("Slot",dataname+ "_getSet");
	    Method setmethod = null;
	    try {
		setmethod = jclass.getMethod("set"+dataname,(Class[])Array.newInstance(method.getReturnType(),1)); 
	    } catch ( Exception e ) {
		setmethod = null;
	    }
	    template.put(cycjclass,new DataMethod(dataname,method,setmethod));
	    assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
	    return;
	}

	if ( methodname.startsWith("child") ||  methodname.endsWith("es") ) {
	    String dataname = methodname;
	    CycConstant cycdatamethod = makeCycJavaMember("Slot",dataname+ "_getAdd");
	    template.put(cycjclass,new DataMethod(dataname,method,null));
	    assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
	    return;
	}

	String firstthree = methodname.substring(0,2);

	if ( methodname.equalsIgnoreCase("iterator") ) {
	    String dataname = methodname;
	    CycConstant cycdatamethod = makeCycJavaMember("Slot",dataname+ "_getIterator");
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
	     || firstthree.equalsIgnoreCase("kil") 
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

    public String flattenJavaName(String cltype) {
	String ctype = cltype;
	String suffix = "";
	if ( cltype.startsWith("[L") ) {
	    ctype = cltype.substring(2);
	    suffix = "Array";
	} else if ( cltype.startsWith("[") ) {
	    ctype = cltype.substring(1);
	    suffix = "Array";
	}
	int i = ctype.lastIndexOf('.');
	if ( i>0 ) {
	    return Strings.change(ctype.substring(i+1),';','_')+suffix;
	} else {
	    return Strings.change(ctype,';','_')+suffix;
	}
    }

    public CycConstant makeCycJavaMember(String ctype,String name) {
	return makeCycJavaMember(javaMt, ctype, name);
    }

    public CycConstant makeCycJavaMember(CycFort dataMt, String ctype,String name) {
	String type = flattenJavaName(ctype);
	CycConstant nameC = staticAccess.makeCycConstant(flattenJavaName(name));
	CycConstant typeC = staticAccess.makeCycConstant("Java" + type);
	try {
	    staticAccess.assertIsa(typeC,staticAccess.collection,dataMt);
	    staticAccess.assertIsa(nameC,typeC,dataMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	return nameC;
    }


    public static PrintWriter getPrintWriter(Writer w) {
	if ( w==null ) return new PrintWriter(System.out);
	if ( w instanceof PrintWriter )	return(PrintWriter)w;
	return new PrintWriter(w);
    }


    public void assertObject(CycFort dataMt, Object object,CycConstant cycobject) {
	System.out.println("assertObject " + object );

	if ( object instanceof CycList ) {
	    assertWithTranscriptNoWffCheckJava(((CycList)object).stringApiValue(), dataMt);
	    return;
	}

	Class jclass = object.getClass();
	if ( jclass.isArray() ) {
	    assertArray(dataMt,object, cycobject);
	    return;
	}

	if ( object instanceof Iterator ) {
	    assertIterator(dataMt,(Iterator)object, cycobject);
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

	assertObjectFromTemplate(jclass,dataMt,object, cycobject);

    }

    public void  assertObjectFromTemplate(Class jclass,CycFort dataMt, Object object,CycConstant cycobject) {
	HashMap template = (HashMap)thisClassTemplates.get(jclass);
	Iterator it = template.keySet().iterator();
	while ( it.hasNext() ) {
	    Object cycdataaccess = it.next();
	    if ( cycdataaccess instanceof CycConstant ) {
		assertMember( dataMt, cycobject, object ,(CycConstant)cycdataaccess, template.get(cycdataaccess));
	    }

	}
    }

    public void assertArray(CycFort dataMt, Object object,CycConstant cycobject) {
	System.out.println("assertArray " + object );
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

    public void assertIterator(CycFort dataMt, Iterator object,CycConstant cycobject) {
	System.out.println("assertIterator " + object );
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


    public void assertCycToJamudAll() {
	staticAccess.assertJamud(staticAccess.jamudMt,staticAccess.getJamud());
    }

    // Assert Entire Mud
    public void assertJamud(CycFort dataMt, Jamud aJamud) {
	MudObjectRoot theJamudRoot = aJamud.currentInstance().mudObjectRoot();

	Iterator bellowRoot =  theJamudRoot.childObjects();
	while ( bellowRoot.hasNext() ) {
	    MudObject area = (MudObject)bellowRoot.next();
	    assertArea(dataMt, area);
	}
	bellowRoot =  jamudTop.childContainer().childObjects();
	while ( bellowRoot.hasNext() ) {
	    MudObject area = (MudObject)bellowRoot.next();
	    assertArea(dataMt, area);
	}
    }

    // Assert Room/Object/Players/NPCs into Cyc
    public void assertMudObject(CycFort dataMt, MudObject object,CycConstant cycobject) {
	if ( object.isTemplate() ) {
	    assertArea(dataMt,object,cycobject);
	} else {
	    assertItem(dataMt,object,cycobject);
	}
    }

    public void assertArea(CycFort dataMt, MudObject object) {
	assertArea(dataMt,object,(CycConstant) makeCycJavaObject(dataMt,object,false));
    }

    public void assertItem(CycFort dataMt, MudObject object,CycConstant cycobject) {
	assertViewableParts(dataMt,(Viewable) object,cycobject);
	assertItemViewableParts(dataMt,object,cycobject);
	assertSlot(dataMt,cycobject,"Flags",object.flags().iterator(),true,true);
	assertInsideParts(dataMt,object,cycobject);
    }

    public void assertArea(CycFort dataMt, MudObject object,CycConstant cycobject) {
	assertViewableParts(dataMt,object.childContainer(),cycobject);
	assertSlot(dataMt,cycobject,"Keyword",object.getKeywords().iterator(),true,false);
	assertItemViewableParts(dataMt,object,cycobject);
	assertSlot(dataMt,cycobject,"Tid",object.getTemplateId(),true,true);
	assertExitParts(dataMt,object,cycobject);
	assertInsideParts(dataMt,object,cycobject);
    }

    public void assertItemViewableParts(CycFort dataMt, MudObject object,CycConstant cycobject) {
	assertSlot(dataMt,cycobject,"Gender_getSet",object.getGender().getName(),true,true);
	assertSlot(dataMt,cycobject,"Job_getSet",object.getJob().getName(),true,true);
	assertSlot(dataMt,cycobject,"Race_getSet",object.getRace().getName(),true,true);
	
	assertSlot(dataMt,cycobject,"getParentContainer",object.getParentContainer(),true,true);

       /* attribs
	Iterator attrit = attribs.entrySet().iterator();
	while ( attrit.hasNext() ) {
	    Object attrib = attrit.next();
	    assertSlot(dataMt,cycobject,attrib.toString(),attribs.get(attribs),true,false);
	}
	*/
    }

    public void assertViewableParts(CycFort dataMt, Viewable object,CycConstant cycobject) {
	try {
	    staticAccess.assertIsa(cycobject,staticAccess.makeCycClassInstance(object.getClass()),dataMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}
	assertSlot(dataMt,cycobject,"Name_getSet",object.getName(),false,true);
	assertSlot(dataMt,cycobject,"Description_getSet",object.getDescription(),false,true);
	assertSlot(dataMt,cycobject,"ShortDescription_getSet",object.getShortDescription(),false,true);
	assertSlot(dataMt,cycobject,"Keyword",object.getKeywords().iterator(),true,true);
    }



    public void assertInsideParts(CycFort dataMt, MudObject object,CycConstant cycobject) {
	MudObjectContainer container = object.childContainer();
	if ( container==null ) return;
	Iterator objs = container.childObjects();
	while ( objs.hasNext() )
	    makeCycJavaObject(dataMt,objs.next(),true);
    }

    public void assertExitParts(CycFort dataMt, MudObject object,CycConstant cycobject) {
	assertSlot(dataMt,cycobject,"Exits",object.childContainer().exits(),true,false);
    }

    /*
    public void queryAsAreaExit(CycFort dataMt,CycConstant cycobject, Entrance inside) {
    CycConstant cycinside = (CycConstant)makeCycJavaObject(dataMt,inside,false);
    assertMudObject("EntrancInstance",dataMt,cycinside,inside);
    assertSlot(dataMt ,cycobject,"Exit",cycinside,false,false);
    }
    */


    public void assertSlot(CycFort dataMt,CycConstant cycobject, Object slot, Object value, boolean constantvalue, boolean singlevalued) {
//	System.out.println("assertSlot(CycFort " + dataMt + ",CycConstant " +cycobject+", Object " +slot+", Object " +value+", boolean " +constantvalue+", boolean " +singlevalued +")");
	CycConstant cycslot = null;
	if ( slot instanceof CycConstant ) {
	    cycslot = (CycConstant)slot;
	} else {
	    if ( slot instanceof String ) {
		cycslot = makeCycJavaMember(dataMt,"Slot",(String)slot);
	    }
	}

	if ( singlevalued ) clearSlot(dataMt,cycobject,cycslot);

	if ( value == null ) return;

	if ( value instanceof Iterator ) {
	    while ( ((Iterator)value).hasNext() ) 
		assertSlot(dataMt,cycobject, cycslot, ((Iterator)value).next(), constantvalue, false);
	    return;
	}

	if ( value.getClass().isArray() ) {
	    assertSlot(dataMt,cycobject, cycslot, Arrays.asList((Object[])value).iterator(), constantvalue, false);
	    return;
	}
	Object cycvalue = null;

	/*        if ( constantvalue ) {
	if ( value instanceof String )
	cycvalue = makeCycJavaMember((String)slot,(String)value);
	else
	cycvalue = makeCycJavaObject(dataMt,value,false);
	
	} else {
	cycvalue = makeCycJavaObject(dataMt,value,false);
	}
	 */
	cycvalue = makeCycJavaObject(dataMt,value,false);
	staticAccess.assertGafNoWff(dataMt,cycHasSlotValue,cycobject,cycslot,cycvalue);
    }



    public Object makeCycJavaObject(CycFort dataMt,Object object, boolean assertobj) {
	if ( object==null ) return cycNull;
	if ( object instanceof CycConstant ) return object;
	if ( object instanceof CycFort ) return object;
	if ( object instanceof CycList ) return object;
	if ( object instanceof String )	return object;
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
	if ( object instanceof MudObjectContainer )
	    return makeCycJavaObject(dataMt,((MudObjectContainer)object).parentObject() ,assertobj);


	if ( cycKnowsObject.containsKey(object) ) return cycKnowsObject.get(object);
	//System.out.println("makeCycJavaObject: " + object);

	CycConstant  cycclass = null;
	try {
	    cycclass = makeCycClassInstance(jclass);
	} catch ( Exception e ) {
	    String  thisobjname = "HYP-NULL"+object.hashCode();
	    CycConstant cycobject = staticAccess.makeCycConstant(thisobjname);
	    cycKnowsObject.put(object,cycobject);
	    e.printStackTrace(System.out);
	    return cycobject;
	}

	String  classname = cycclass.toString();
	
	// Only make cyc constant for object 
	CycConstant cycobject = null;

	if ( object instanceof Viewable ) {
	    cycobject = staticAccess.makeCycConstant(((Viewable)object).getName());
	} else if ( object instanceof MudObjectViewable ) {
	    cycobject = staticAccess.makeCycConstant(((MudObjectViewable)object).getName());
	} else {
	    cycobject = staticAccess.makeCycConstant("HYP-"+classname.substring(0,classname.length()-8)+object.hashCode());
	}

	cycKnowsObject.put(object,cycobject);


	try {
	    staticAccess.assertIsa((CycFort)cycobject,(CycFort)cycclass,dataMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}

	if ( assertobj ) assertObject(dataMt,object,cycobject);
	return cycobject;
    }

    public void assertMember(CycFort dataMt, CycConstant cycobject, Object object, CycConstant cycaccess, Object accessmember) {
	System.out.println("while {assertObject " + cycobject + " " + cycaccess + " " + accessmember + "}" );
	try {
	    if ( accessmember instanceof DataMethod ) assertDataMethod( dataMt, cycobject, object, cycaccess, (DataMethod)accessmember);
	    //	    if ( accessmember instanceof Method ) assertMethod( dataMt, cycobject, object, cycaccess, (Method)accessmember);
	    if ( accessmember instanceof Field ) assertField( dataMt, cycobject, object, cycaccess, (Field)accessmember);
	} catch ( Exception e ) {
	    e.printStackTrace( System.out);
	}
    }

    public void assertField(CycFort dataMt, CycConstant cycobject, Object object, CycConstant cycaccess, Field accessmember) 
    throws Exception{
	CycList assertme = new CycList(cycHasSlotValue);    //"#$hasFieldValue"
	assertme.add(cycobject);
	assertme.add(cycaccess);
	assertme.add(makeCycJavaObject(dataMt,accessmember.get(object)));
	staticAccess.assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void assertMethod(CycFort dataMt, CycConstant cycobject, Object object, CycConstant cycaccess, Method accessmember) 
    throws Exception{
	CycList assertme = new CycList(cycHasMethod);	 //"#$hasMethodValue"
	assertme.add(cycaccess);
	assertme.add(makeCycJavaObject(dataMt,accessmember.invoke(object,null)));
	staticAccess.assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void assertDataMethod(CycFort dataMt, CycConstant cycobject, Object object, CycConstant cycaccess, DataMethod accessmember) 
    throws Exception{
	CycList assertme = new CycList(cycHasSlotValue);    //"#$hasMethodValue"
	assertme.add(cycobject);
	assertme.add(cycaccess);
	assertme.add(makeCycJavaObject(dataMt,accessmember.get(object)));
	staticAccess.assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void clearSlot(CycFort dataMt,CycConstant cycobject, Object cycslot) {
	// Delete all previous
	CycList query = new CycList(cycHasSlotValue);
	query.add(cycobject);
	query.add(cycslot);
	CycVariable cv = new CycVariable("Prev");
	query.add(cv);

	try {
	    Iterator result = staticAccess.askWithVariable(query,cv,dataMt).iterator();
	    while ( result.hasNext() ) {
		query.set(3,result.next());
		staticAccess.deleteGaf(query,dataMt);
	    }
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	}

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

    private static class CycMooAgent extends Thread {
	private CycMoo tstaticAccess = null; 
	public CycMooAgent() throws IOException,CycApiException {
	    tstaticAccess = new CycMoo();
	    //tstaticAccess = aCycMoo;
	}
	public void run() {
	    while ( !this.interrupted() ) {
		try {
		    Thread.sleep(5000);
		} catch ( java.lang.InterruptedException e ) {
		}
		tstaticAccess.syncBot();
	    }
	}
    }
}

//</pre>

