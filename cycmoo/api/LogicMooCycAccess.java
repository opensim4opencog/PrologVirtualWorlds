package cycmoo.api;

import cycmoo.*;
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
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;
import org.opencyc.templateparser.*;
import org.opencyc.inferencesupport.*;
import org.opencyc.constraintsolver.*;
import org.opencyc.conversation.*;
import org.apache.oro.util.*;

// Util
import ViolinStrings.*;

/**
* Provides wrappers for the OpenCyc API.<p>
*
* Collaborates with the <tt>Jamud</tt> class which manages the api connections.
*
* @version $Id: LogicMooCycAccess.java,v 1.2 2002-11-27 21:47:23 dmiles Exp $
* @author Douglas R. Miles
*
* <p>Copyright 2001 Cycorp, Inc., license is open source GNU LGPL.
* <p><a href="http://www.openorg/license.txt">the license</a>
* <p><a href="http://www.openorg">www.openorg</a>
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
public class LogicMooCycAccess extends CycAssertionsFactory {

    //private  static Jamud logicmooInstance;
    //private  static Area logicmooArea;
    //    private  static PlayerManager logicmooPlayerManager;
    //private  static Room logicmooTop;     
    private  static Object initialObject; 
    private  static bsh.Interpreter logicmooBeanShell;     
    private  static boolean running = false;



    public static CycConstant reifiedMicrotheory = null;
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
    public static CycConstant geographicalRegion = null;
    public static CycConstant cycArrayOfClass = null;

    public static LogicMoo moo = null;

    // CycConstant & Class -> CycConstant key of Fields | DataMethod | Method
    public static HashMap thisClassTemplates = new HashMap();

    // CycConstant || Class -> Class || CycConstant 
    public static HashMap cycKnowsClass = new HashMap();
    public static HashMap cycKnowsObjectAsConstant = new HashMap();
    //public static HashMap cycKnowsMicrotheory = new HashMap();
    //public static HashMap cycKnowsExit = new HashMap();
    public static HashMap cycKnowsNPC = new HashMap();
    //public static HashMap cycKnowsPlayer = new HashMap();
    //public static HashMap cycKnowsInanimate = new HashMap();

    /**
     * Constructs a new CycAssertionsFactory object.
     */
    public LogicMooCycAccess() throws IOException, CycApiException {
	super();
	startPrimary();
    }

    /**
     * Constructs a new LogicMooCycAccess object to the given CycProxyAgent in the given
     * agent community.
     *
     * @param myAgentName the name of the local agent
     * @param cycProxyAgentName the name of the this proxy agent
     * @param agentCommunity the agent community to which the this proxy agent belongs
     */
    public LogicMooCycAccess (String myAgentName,
		   String cycProxyAgentName,
		   int agentCommunity) throws IOException, CycApiException  {
	super(myAgentName,cycProxyAgentName,agentCommunity);
	startPrimary();
    }

    /**
     * Constructs a new LogicMooCycAccess object given a host name, port, communication mode and persistence indicator.
     *
     * @param hostName the host name
     * @param basePort the base (HTML serving) TCP socket port number
     * @param communicationMode either ASCII_MODE or BINARY_MODE
     * @param persistentConnection when <tt>true</tt> keep a persistent socket connection with
     * the OpenCyc server
     */
    public LogicMooCycAccess(String hostName, int basePort, int communicationMode, boolean persistentConnection)
    throws IOException, UnknownHostException, CycApiException {
	super( hostName,  basePort,  communicationMode,  persistentConnection);
	startPrimary();
    }

    /***********************************************************
     *  Gets for private  static fields
     *
     **********************************************************/

    public  CycAccess getCycAccess() {
	return(CycAccess)this;
    }
    public  bsh.Interpreter getBeanShell() {
	return logicmooBeanShell;
    }
    public  CycAssertionsFactory getCycAssertionsFactory() {
	return(CycAssertionsFactory)this;
    }
    public  LogicMoo getLogicMoo() {
	moo = cycmoo.LogicMoo.getInstance();
	return moo;
    }


    /***********************************************************
     *  Initialize all services
     *
     **********************************************************/

    private synchronized  void startPrimary() {
	if ( running ) return;
	else running = true;
	Log.makeLog();
	Log.current.makeLog();
	Log.current.println("Starting Cyc Beanshell");
	try {
	    if ( logicmooBeanShell==null ) {
		logicmooBeanShell = new bsh.Interpreter(new InputStreamReader(System.in),(PrintStream)System.out ,(PrintStream)System.out,false);
	       logicmooBeanShell.set("cyc",this);
	    }
	} catch ( EvalError e ) {
	    Log.current.println("Aborted Beanshell");
	    e.printStackTrace();
	}
	cycVoid =  makeCycConstantSafe("#$voidValue");
	cycNull =  makeCycConstantSafe("#$nullValue");
	cycHasArrayMember =  makeCycConstantSafe("#$javaArrayContains");
	cycHasMethod =  makeCycConstantSafe("#$hasMethod");
	cycHasSlot =  makeCycConstantSafe("#$relationAllExists");
	cycClassInstance =  makeCycConstantSafe("#$ClassInstance");
	cycHasSlotValue =  makeCycConstantSafe("#$hasSlotValue");
	javaMt =  makeCycConstantSafe("#$JavaMt");
	reifiedMicrotheory =  makeCycConstantSafe("#$ReifiedMicrotheory");
	geographicalRegion =  makeCycConstantSafe("#$Room");
	cycArrayOfClass =  makeCycConstantSafe("#$SetOfTypeFn");

	makeCycConstantError =  makeCycConstantSafe("#$MakeCycConstantErrorFn");
	assertIsaSafe(reifiedMicrotheory,     makeCycConstantSafe("#$MicrotheoryType"),     baseKB);
	assertIsaSafe(javaMt,     microtheory,        baseKB);
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

    public synchronized  void constructMtFromFile(String file,String mtName) throws Exception {
	constructMtFromFile(ontologyFile(file),mtName);
    }

    public synchronized  void constructMtFromFile(File file,String mtName) throws Exception {
	constructMtFromFile(file,makeCycConstantSafe(mtName));
    }

    public synchronized  void constructMtFromFile(String file,CycConstant mtName) throws Exception {
	constructMtFromFile(ontologyFile(file),mtName);
    }

    public synchronized  void constructMtFromFile(File file,CycConstant mtName) throws Exception {
	//  kill(mtName);
	Log.current.println("constructMtFromFile " + file.getAbsolutePath() + " " + mtName.cyclify());
	KifFileLoader theMt = new KifFileLoader(this);
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

    public  synchronized ArrayList getArrayListQuerySELStatic(String mt, String query) {
	return queryArrayListSEL(mt,query);
    }


    public CycList queryMt(String query) {
	try {
	    return converseList( "(cyc-query '" +toCycListString(query) + " " + " #$EverythingPSC)");
	} catch ( Exception e ) {
	    return null;
	}
    }
    public CycList queryMt2(String query) {
	try {
	    CycList result =  converseList( "(cyc-query '" +toCycListString(query) + " #$EverythingPSC)");
	    if ( result.size()==0 ) return result;
	    CycList answer = new CycList();
	    Iterator iter = result.iterator();
	    while ( iter.hasNext() ) {
		CycList set = (CycList)iter.next();
		answer.add(new CycList(((CycList)set.first()).rest(),((CycList)set.second()).rest()));
	    }
	    return answer;
	} catch ( Exception e ) {
	    e.printStackTrace();
	    return new CycList();
	}
    }

    public CycList queryMt3(String query) {
	try {
	    CycList result =  converseList( "(cyc-query '" +toCycListString(query) + " #$EverythingPSC)");
	    if ( result.size()==0 ) return result;
	    CycList answer = new CycList();
	    Iterator iter = result.iterator();
	    while ( iter.hasNext() ) {
		CycList set = (CycList)iter.next();
		CycList ans = new CycList(((CycList)set.first()).rest(),((CycList)set.second()).rest());
		ans.add(((CycList)set.third()).rest());
		answer.add(ans);
	    }
	    return answer;
	} catch ( Exception e ) {
	    e.printStackTrace();
	    return new CycList();
	}
    }


    /***********************************************************
     *  Mt Handling Extras
     *
     **********************************************************/

    public synchronized void killMt(String mtName) throws IOException, CycApiException {
	kill(makeCycConstantSafe(mtName));
    }


    /***********************************************************
     *  BeanShell Access
     *
     **********************************************************/

    public synchronized  Object eval(String cmd) throws bsh.EvalError  {
	return  logicmooBeanShell.eval(cmd);
    }

    public synchronized  void set(String name, Object obj) {
	try {
	    logicmooBeanShell.set(name,obj);
	} catch ( Exception e ) {
	    Log.current.printStackTrace(e);
	}
    }


    /***********************************************************
     *  CycL Paraphrase
     *
     **********************************************************/

    public String attemptParaphrase(Object post) {
	//Log.current.println("attemptParaphrase=" + post);
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
		return getGeneratedPhrase((CycFort)post);

	    if ( post instanceof CycNart )
		return getGeneratedPhrase((CycFort)post);
		//return converseString("(generate-phrase '" + ((CycNart)post).cyclify() +  ")");

	    if ( post instanceof CycVariable )
		return(((CycVariable)post).stringApiValue());

	    if ( post instanceof CycList ) {

		if ( ((CycList)post).isEmpty() )
		    return "an empty list ";

		if ( !((CycList)post).isProperList() )
		    return attemptParaphrase(((CycList)post).first()) + " = " + (((CycList)post).rest());
		//return attemptParaphrase(((CycList)post).first()) + " = " + attemptParaphrase(((CycList)post).rest());

		if ( ((CycList)post).first() instanceof CycList ) return attemptParaphrase(((CycList)post).iterator());

		return getParaphrase((CycList)post); // converseString("(generate-phrase '" + ((CycList)post).cyclify() +  ")");
	    }

	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	return post.toString();
    }


    public void assertKif(String sentence, CycFort mt) throws Exception {
	startPrimary();
	assertWithTranscriptNoWffCheck(toCycList(sentence),mt);
    }

    /***********************************************************
     *  Jamud Booting
     *
     **********************************************************/

    //private  Area logicmooTopMicrotheory;
    private  Hashtable logicmooTopRooms;
    private  Hashtable logicmooTopArtifacts;
    private  Hashtable logicmooTopBodies;

    public  CycFort nearestFort(String type, Object value ){
	if ( value==null ) return null;
	String test = value.toString().toLowerCase();
	Iterator kws = queryArrayListSEL("EverythingPSC","(#$isa ?SEL #$"+type+")").iterator();
	while ( kws.hasNext() ) {
	    Object sample = kws.next();
	    if ( sample instanceof CycFort ) if ( sample.toString().toLowerCase().indexOf(test)==0 ) return(CycFort)sample;
	}
	return null;
    }

    public synchronized  CycFort makeCycFort(Object obj) {
	if ( obj==null ) return null;

	CycFort possible = locateCycFort(obj,""+obj);
	if ( possible!=null ) return possible;

	if ( obj instanceof CycFort ) {
	    return(((CycFort)obj));
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
		    return makeCycFort((CycList)(new CycListKifParser(this)).read(sobj));
		} catch ( Exception e ) {

		}
	    }
	}

	if ( obj instanceof Object ) {
	    return makeCycFort(getObjNameCyc(obj));
	}

	return null;

    }



    public String cyclify(Object obj) {

	if ( obj instanceof CycFort ) {
	    return(((CycFort)obj).cycListApiValue().toString());
	}

	if ( obj instanceof Object ) {
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
		    return cyclify(((CycList)((new CycListKifParser(this)).read(sobj))));
		} catch ( Exception e ) {

		}
	    }
	}

	return obj.toString();
    }


    //6239075950(49)


    public synchronized  void assertIsaSafe(CycFort cycobject, CycFort cycclass, CycFort cycmt) {
	try {
	    assertIsa(cycobject,cycclass,cycmt);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }


    public CycConstant makeCycConstantSafe(String constname) {
	if ( constname==null ) return null;
	try {
	    return makeCycConstant(constname);
	} catch ( Exception e ) {
	    return null;
	}
    }


    /**
     * Preforms query in mt with parameters
     *   then unasserts the insanciated gafs derived from the query 
     *	 
     *  NOTE:  Only if they exist in the same microtheory as the query is in.
     *   see queryMatch for how these insanciated gafs are produced
     *
     */


    public ArrayList deleteMatchGaf(CycList query,CycFort mt,int maxBackchains,int maxAnswers, int maxSeconds) {
	    ArrayList al = queryMatch(query,mt,maxBackchains,maxAnswers,maxSeconds);
	    Iterator its = al.iterator();
	    while(its.hasNext()) {
		try {
		    deleteGaf((CycList)its.next(),mt);
		} catch (Exception e){
		}
	    }
	    return al;
    }

    /**
     * Preforms query in mt with parameters
     *   returns the insanciated gafs derived from the query 
     *	
     *   a query of (#$isa ?X #$Dog) in #$BiologyMt
     *     will return a ArrayList with a single CycList formula containing:
     *     (#$isa (#$GenericInstanceFn #$Dog) #$Dog)
     *
     */

    public ArrayList queryMatch(CycList query,CycFort mt,int maxBackchains,int maxAnswers, int maxSeconds) {

	ArrayList match = new CycList();
	try {
	    CycList results = queryWithMaximums(query,mt,maxBackchains,maxAnswers,maxSeconds);
	    Iterator its = results.iterator();
	    while( its.hasNext() ) {
		try {
		    CycList bindingset = (CycList)its.next();
		    CycList result = replaceVarsWithBindingSet(query,bindingset).getFormula();
		    System.out.println(result);
		    match.add(result);
		} catch( Exception e ) {
		    e.printStackTrace();
		}
	    }
	} catch( Exception ee ) {
	    ee.printStackTrace();
	}
	return match;
    }

    public CycList queryWithMaximums(CycList query,CycFort mt,int maxBackchains,int maxAnswers, int maxSeconds) 
    throws IOException, CycApiException {
	StringBuffer queryBuffer = new StringBuffer();
	queryBuffer.append("(clet ((*cache-inference-results* nil) ");
	queryBuffer.append("       (*compute-inference-results* nil) ");
	queryBuffer.append("       (*unique-inference-result-bindings* t) ");
	queryBuffer.append("       (*generate-readable-fi-results* nil)) ");
	queryBuffer.append("  (without-wff-semantics ");
	queryBuffer.append("    (cyc-query '" + query.cyclify() + " ");
	queryBuffer.append("                  " + mt.stringApiValue() + " ");
	queryBuffer.append("                  '(:backchain "+maxBackchains+" :number "+maxAnswers+" :time "+ maxSeconds +"))))");
	return converseList(queryBuffer.toString());
    }

    public QueryLiteral replaceVarsWithBindingSet(CycList query, ArrayList bindingset) {
	QueryLiteral querylit = new QueryLiteral(query);
	Iterator bindings = bindingset.iterator();
	while( bindings.hasNext() ) {
	    CycList binding =  (CycList)bindings.next();
	    CycVariable variable = (CycVariable)binding.first();
	    Object value = binding.rest();
	    querylit.substituteVariable(variable,value);
	}
	return querylit;
    }





    /***********************************************************
     * Java Editing
     *
     **********************************************************/


    public synchronized String findNamedProperty(Object target, String property) {
	// Normally more deeper analysis then string concatination (that is why the target object is here)
	return "set" + toProperCase(property);
    }

    public synchronized boolean attemptJavaSetMethod(Object target,String property,Object value) {
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

    public synchronized Method findJavaMethod(Class cls, String methodname)
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


    public synchronized Object makeInstanceFromClass(String theType,Object value)
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

    public synchronized  String toProperCase(String property) {
	return property.substring(0,0).toUpperCase()+property.substring(1).toLowerCase();
    }




    public  Object getObjNameCyc(Object object) {
	return makeCycJavaObject( javaMt, object, false);
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

		Log.current.println("\n\n PRIMITIVE!!!!\n\n PRIMITIVE!!!!\n\n PRIMITIVE!!!!");
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

	// Only make this constant for object 
	if ( assertobj ) assertObjectData(dataMt,object,cycobject);
	return cycobject;
    }

    public void assertObjectData(CycFort dataMt, Object object,CycFort cycobject) {
	Log.current.println("assertObjectData " + object );

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
	Log.current.println("assertArrayData " + object );
	CycList assertme = new CycList(cycHasArrayMember);
	assertme.add(cycobject);
	assertme.add(null);
	assertme.add(null);
	for ( int i=0 ; i < ((Object[])object).length; i++ ) {
	    Object submember = makeCycJavaObject(dataMt,((Object[])object)[i]);
	    try {
		assertme.set(2,new Integer(i));
		assertme.set(3,submember);
		assertWithTranscriptNoWffCheck(assertme,dataMt);
	    } catch ( Exception e ) {
		e.printStackTrace(System.err);
	    }
	}
    }

    public void assertIteratorData(CycFort dataMt, Iterator object,CycFort cycobject) {
	Log.current.println("assertIteratorData " + object );
	CycList assertme = new CycList(cycHasArrayMember);
	assertme.add(cycobject);
	assertme.add(3,new Integer(0));
	assertme.add(null);
	while ( object.hasNext() ) {
	    Object submember = makeCycJavaObject(dataMt,object.next());
	    try {
		assertme.set(3,submember);
		assertWithTranscriptNoWffCheck(assertme,dataMt);
	    } catch ( Exception e ) {
		e.printStackTrace(System.err);
	    }
	}
    }

    public  void assertSlotValue(CycFort dataMt,CycFort cycobject, Object slot, Object value, boolean singlevalued) {
	assertSlotValue(dataMt,cycobject,slot,value,null,singlevalued);
    }


    public  void assertSlotValue(CycFort dataMt,CycFort cycobject, Object slot, Object value, Object type, boolean singlevalued) {
	CycConstant cycslot = null;
	if ( cycobject==null ) {
	    Log.current.println("assertSlotValue(CycFort " + dataMt + ",CycConstant " +cycobject+", Object " +slot+", Object " +value+", boolean " +singlevalued +")");
	    return;
	}

	if ( slot instanceof CycConstant ) {
	    cycslot = (CycConstant)slot;
	} else {
	    if ( slot instanceof String ) {
		cycslot = makeTypedCycFort(dataMt,"JavaSlot",(String)slot);
	    }
	}

	if ( singlevalued ) clearSlot(dataMt,cycobject,cycslot);

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
	Object cycvalue = makeCycJavaObject(dataMt,value,false);

	if ( type!=null ) {
	    if ( cycvalue instanceof CycFort ) {
		assertIsaSafe((CycFort)cycvalue,(CycFort)makeCycFort(type),dataMt);
	    }
	    if ( cycvalue instanceof CycList ) {
		assertIsaSafe(((CycFort)new CycNart((CycList)cycvalue)),(CycFort)makeCycFort(type),dataMt);
	    }
	}

	if ( cycvalue!=null ) {
	    try {
		assertGafNoWff(dataMt,cycslot,cycobject,cycvalue);
	    } catch ( Exception e ) {
		e.printStackTrace(System.out);
		Log.current.println("assertSlotValue(CycFort " + dataMt + ",CycConstant " +cycobject+", Object " +slot+", Object " +value+", boolean " +singlevalued +")");
	    }
	}
    }

    public void assertMemberValue(CycFort dataMt, CycFort cycobject, Object object, CycConstant cycaccess, Object accessmember) {
	Log.current.println("while {assertObjectData " + cycobject + " " + cycaccess + " " + accessmember + "}" );
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
	assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void assertMethodResult(CycFort dataMt, CycFort cycobject, Object object, CycConstant cycaccess, Method accessmember) 
    throws Exception{
	CycList assertme = new CycList(cycHasMethod);	 //"#$hasMethodValue"
	assertme.add(cycaccess);
	assertme.add(makeCycJavaObject(dataMt,accessmember.invoke(object,null)));
	assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void assertDataMethodResult(CycFort dataMt, CycFort cycobject, Object object, CycConstant cycaccess, DataMethod accessmember) 
    throws Exception{
	CycList assertme = new CycList(cycHasSlotValue);    //"#$hasMethodValue"
	assertme.add(cycobject);
	assertme.add(cycaccess);
	assertme.add(makeCycJavaObject(dataMt,accessmember.get(object)));
	assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public synchronized  void clearSlot(CycFort dataMt,CycFort cycobject, Object cycslot) {
	// Delete all previous
	CycList query = new CycList(cycslot);
	query.add(cycobject);
	CycVariable cv = new CycVariable("Prev");
	query.add(cv);

	try {
	    Iterator result =  askWithVariable(query,cv,dataMt).iterator();
	    while ( result.hasNext() ) {
		query.set(2,result.next());
		deleteGaf(query,dataMt);
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

	Log.current.println("cycclassname =" + cycclassname);

	try {

	    cycjclass =  makeCycConstantSafe(cycclassname);    
	} catch ( Exception e ) {
	    Log.current.println("makeCycConstantSafe: " +cycclassname+" "+ e );
	    e.printStackTrace(System.err);
	}
	cycKnowsClass.put(jclass,cycjclass);
	cycKnowsClass.put(cycjclass,jclass);

	// Save Isa
	try {

	    assertIsaSafe(cycjclass,      cycClassInstance,javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	try {
	    // Make super classes
	    Class superjclass = jclass.getSuperclass();
	    if ( superjclass != null ) {
		CycFort cycsuperjclass = makeCycClassInstance(superjclass);
		assertGenls(cycjclass,cycsuperjclass,javaMt);
	    }
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	// Decide if we should make a template
	String classstring = jclass.toString();
	if ( classstring.startsWith("class java.lang") 
	     || classstring.startsWith("class java.io")
	     || classstring.startsWith("class cycmoo.")
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

	    assertWithTranscriptNoWffCheck(
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

	    assertWithTranscriptNoWffCheck(
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

	    assertWithTranscriptNoWffCheck( sentence,mt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}

    }

    public void assertIsaJavaMethodOf(CycConstant cycjclass,Class jclass,CycConstant cycmethod,String methodname,Class[] params, CycFort cycmethodjclass,Method method,HashMap template) {

	assertWithTranscriptNoWffCheckJava(
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

	    assertIsaSafe(typeC,     collection,dataMt);
	    assertIsaSafe(nameC,typeC,dataMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	return nameC;
    }


    public synchronized  PrintWriter getPrintWriter(Writer w) {
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


    /**
     * Cyclifys a sentence a string
     */
    public  CycList toCycList(String sentence) {
	try {
	    return(((CycList)((new CycListKifParser(this)).read(sentence))));
	} catch ( Exception e ) {
	    return null;
	}

    }
    /**
     * Cyclifys a sentence to a string
     */
    public  String toCycListString(String sentence) {
	try {
	    return(((CycList)((new CycListKifParser(this)).read(sentence))).cyclify());
	} catch ( Exception e ) {
	    return null;
	}

    }


    public static String cleanString(String name) {
	if ( name==null ) return "null";
	String tryName = name;
	if ( name.startsWith("#$") ) tryName = name.substring(2);
	tryName = Strings.change(tryName,'$','_');
	tryName = Strings.change(tryName,')','_');
	tryName = Strings.change(tryName,'(','_');
	tryName = Strings.change(tryName,' ','_');
	tryName = Strings.change(tryName,'.','_');
	return Strings.change(tryName,'@','_');
    }

    public  CycFort locateCycFort(Object inp, String type) {
	if ( inp instanceof CycFort ) return(CycFort)inp;
	if ( inp instanceof CycList ) return new CycNart((CycList)inp);
	if ( inp instanceof String ) return locateCycFortString((String)inp,type);
	return new CycNart( toCycList(""+inp)); 
    }

    public  CycFort locateCycFortString(String name, String type) {
	Log.current.println("locateCycFortString '"+name+"' of '"+type+"'");
	if ( name==null ) return null;
	CycFort cycfort = null;

	cycfort = locateCycConstantStringPremuted(name);
	if ( cycfort!=null ) {
	    Log.current.println("locateCycConstantStringPremuted=" + cycfort);
	    return cycfort;
	}

	try {
	    CycConstant cyctype = getConstantByName(type);

	    CycList canidates  = getAllInstances(cyctype);
	    int  max = -999;
	    Iterator ps = null;
	    int howmuch = 0;

	    // Name match
	    ps = canidates.iterator();
	    while ( ps.hasNext() ) {
		CycFort p = (CycFort) ps.next();
		howmuch =  matchable(name,p);
		if ( howmuch>-999 ) {
		    Log.current.println("'"+name +  "'<>'" + p.toString() + "'=" + howmuch);
		    if ( howmuch>max ) {
			max = howmuch;
			cycfort = p;
		    }
		}
	    }

	    //if ( (name.indexOf("(")+name.indexOf(")"))>-2 )  return null;

	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	if ( cycfort!=null ) {
	    Log.current.println("locateCycFort worked '" + cycfort+"'");
	    return cycfort;
	}
	Log.current.println("locateCycFortTyped did not find");
	return null;

    }

    public CycConstant locateCycConstantStringPremuted(String name) {
	if ( name==null ) return null;
	CycConstant cycconst = null;

	try {
	    if ( !isValidConstantName(name) ) return null;

	    cycconst = getConstantByName(name);
	    if ( cycconst!=null ) return cycconst;

	    String tryName = name.toLowerCase();
	    cycconst = getConstantByName(tryName);
	    if ( cycconst!=null ) return cycconst;

	    tryName = toProperCase(tryName);
	    cycconst = getConstantByName(tryName);
	    if ( cycconst!=null ) return cycconst;

	    tryName = tryName.toUpperCase();
	    cycconst = getConstantByName(tryName);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}


	return cycconst;
    }

    private static HashMap keywordHashMap = null;

    public HashMap getKeywordHashMap() {

	if ( keywordHashMap!=null ) return keywordHashMap;
	keywordHashMap = new HashMap();

	ArrayList al = null;
	try {
	    al = converseList("(cyc-query '(#$mudKeyword ?Const ?String) #$EverythingPSC )");
	} catch ( Exception e ) {
	    e.printStackTrace();
	}

	Iterator iter = al.iterator();
	while ( iter.hasNext() ) {
	    CycList cyclist = (CycList)iter.next();
	    CycFort cycconst = (CycFort)((CycList)cyclist.first()).rest();
	    String keyword = ((CycList)cyclist.first()).rest().toString();
	    keywordHashMap.put(keyword,cycconst);
	}

	return keywordHashMap;
    }


    public int matchable(String word, CycFort term) {
	if ( term==null || word==null )	return -999;
	int  pos1 =  matchableStr(word,term.toString());
	int  pos2 =  matchableStr(word,attemptParaphrase(term));
	if ( pos1>pos2 ) return pos1;
	return pos2;
    }

    public int matchableStr(String word, String termstr) {
	if ( termstr.equals(word) ) return 1000;
	if ( termstr.equalsIgnoreCase(word) ) return 500;
	if ( termstr.startsWith(word) )	return(word.length()*4);
	if ( termstr.toLowerCase().startsWith(word.toLowerCase()) ) return(word.length()*3);
	if ( termstr.toLowerCase().endsWith(word.toLowerCase()) ) return(word.length()*2);
	if ( termstr.indexOf(word)>0 ) return(int)(termstr.length() - word.length());
	return -999;
    }

    public static String queryRawPrologServer(String raw) throws Exception {
	return LogicMoo.queryRawPrologServer(raw,500);
    }

    public static String queryRawPrologServer(String raw, int time) throws Exception {
	 return LogicMoo.queryRawPrologServer(raw,time);
    }

    public static String e2c(String eng) throws Exception {
	return LogicMoo.e2c(eng);
    }



     /***********************************************************
     *  Plugin communication system
     *
     **********************************************************/

    public static Hashtable plugins = new Hashtable();
    

    public synchronized void reloadInterpretors() {
	Iterator clients = plugins.values().iterator(); 
	while (clients.hasNext()) {
	    try {
		((IMooClient)clients.next()).reloadInterpretor();
	    } catch (Exception e) {
		e.printStackTrace();
	    }
	}
    }

    public synchronized void registerUser(String user, IMooClient who){
	//if (who instanceof IActorCommandParser) return;
	registerUser(makeCycConstantSafe(user),who);
    }
    public synchronized void registerUser(CycFort user, IMooClient who){
	//if (who instanceof IActorCommandParser) return;
	set(user.toString(),who);
	plugins.put(user,who);
    }
    
    public synchronized void unregisterUser(String user){
	unregisterUser(makeCycConstantSafe(user));
    }
    public synchronized IMooClient getClient(String user){
	return getClient(makeCycConstantSafe(user));
    }

    public synchronized void unregisterUser(CycFort user){
	set(user.toString(),null);
	plugins.remove(user);
    }

    public synchronized static IMooClient getClient(CycFort user){
	return (IMooClient)plugins.get(user);
    }

}



