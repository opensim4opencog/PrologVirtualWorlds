package logicmoo;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;

import jamud.*;

import jamud.command.*;
import jamud.object.*;
import jamud.object.event.*;
import jamud.util.*;
import com.jamud.commands.*;
import com.jamud.connections.*;
import com.jamud.communications.*;
import com.jamud.creation.*;
import net.n3.nanoxml.*;
import org.opencyc.webserver.WebServer;
import org.opencyc.util.*;
import org.opencyc.api.*;
import org.opencyc.cycobject.*;
import org.apache.oro.util.*;
import ViolinStrings.*;


import javax.servlet.jsp.*;


/**
* Provides wrappers for the OpenCyc API.<p>
*
* Collaborates with the <tt>CycConnection</tt> class which manages the api connections.
*
* @version $Id: CycMooAccess.java,v 1.3 2002-04-16 21:05:53 dmiles Exp $
* @author Douglas R. Miles, Stephen L. Reed
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
public class CycMooAccess extends CycAccess {

    /**
     * Constructs a new CycMooAccess object.
     */
    public CycMooAccess() throws IOException, UnknownHostException, CycApiException {
	super();
	cycMooAccessInit();
    }

    /**
     * Constructs a new CycMooAccess object to the given CycProxyAgent in the given
     * agent community.
     *
     * @param myAgentName the name of the local agent
     * @param cycProxyAgentName the name of the cyc proxy agent
     * @param agentCommunity the agent community to which the cyc proxy agent belongs
     */
    public CycMooAccess (String myAgentName,
			 String cycProxyAgentName,
			 int agentCommunity) throws IOException, CycApiException  {
	super(myAgentName,cycProxyAgentName,agentCommunity);
	cycMooAccessInit();
    }

    /**
     * Constructs a new CycMooAccess object given a host name, port, communication mode and persistence indicator.
     *
     * @param hostName the host name
     * @param basePort the base (HTML serving) TCP socket port number
     * @param communicationMode either ASCII_MODE or BINARY_MODE
     * @param persistentConnection when <tt>true</tt> keep a persistent socket connection with
     * the OpenCyc server
     */
    public CycMooAccess(String hostName, int basePort, int communicationMode, boolean persistentConnection)
    throws IOException, UnknownHostException, CycApiException {
	super( hostName,  basePort,  communicationMode,  persistentConnection);
	cycMooAccessInit();
    }


    public ArrayList getMts()  throws Exception {
	return(ArrayList) getAllInstances(microtheory);
    }

    public ArrayList getCollections()  throws Exception {
	return(ArrayList) getAllInstances(collection);
    }

    private void cycMooAccessInit()  throws CycApiException {
	try {
	    cycMooHTMLTool = new HTMLUtils();
	    HTMLUtils.cycAccess = this;

	    argIsa = getConstantByName("#$argIsa");
	    argGenl = getConstantByName("#$argGenl");
	    resultIsa = getConstantByName("#$resultIsa");
	    resultGenl = getConstantByName("#$resultGenl");
	    predicate = getConstantByName("#$Predicate");
	    relation = getConstantByName("#$Relation");
	    functionalRelation = getConstantByName("#$FunctionalRelation");
	    microtheory = getConstantByName("#$Microtheory");
	    cyclist = getConstantByName("#$Cyclist");
	    predarity = getConstantByName("#$arity");
	    genlpreds = getConstantByName("#$genlPreds");
	    cycadministrator = getConstantByName("#$CycAdministrator");
	    opencycproject = getConstantByName("#$OpenCycProject");
	    cyclistDefinitionalMt = getConstantByName("#$CyclistDefinitionalMt");
	    cycBasedProject = getConstantByName("#$Cyc-BasedProject");
	    metaRelation = getConstantByName("#$MetaRelation");
	    logicMooMt = makeCycConstant("#$LogicMooMt");
	    jamudMt = makeCycConstant("#$JamudMt");
	    javaMt = makeCycConstant("#$JavaMt");
	    makeCycConstantError = makeCycConstant("#$MakeCycConstantErrorFn");
	    assertIsa(logicMooMt,microtheory,baseKB);
	    assertIsa(jamudMt,microtheory,baseKB);
	    assertIsa(javaMt,microtheory,baseKB);
	    assertGenlMt(jamudMt,logicMooMt);
	    assertGenlMt(logicMooMt,"ArtifactGVocabularyMt");
	    assertGenlMt(logicMooMt,"ArtifactGMt");
	    assertGenlMt(logicMooMt,"HumanActivitiesMt");
	    assertGenlMt(logicMooMt,"AgentGVocabularyMt");
	    assertGenlMt(logicMooMt,"AnimalActivitiesVocabularyMt");
	    assertGenlMt(logicMooMt,javaMt);

	} catch ( Exception e ) {
	    throw new CycApiException(""+e);
	}

    }

    /**
     * Asserts a ground atomic formula (gaf) in the specified microtheory MT.  The operation
     * will be added to the KB transcript for replication and archive.
     *
     * @param mt the microtheory in which the assertion is made
     * @param predicate the binary predicate of the assertion
     * @param arg1 the first argument of the predicate
    * @param arg2 the second argument of the predicate
    * @param arg3 the third argument of the predicate
     */
    public void assertGaf (CycFort mt,
			   CycConstant predicate,
			   CycFort arg1,
			   CycFort arg2,
			   CycFort arg3)
    throws IOException, UnknownHostException, CycApiException {
	// (predicate <CycFort> <CycFort>)
	String command = withBookkeepingInfo() +
			 "(cyc-assert '(" +
			 predicate.stringApiValue() + " " +
			 arg1.stringApiValue() + " " +
			 arg2.stringApiValue() + " " +
			 arg3.stringApiValue() + " )" +
			 mt.stringApiValue() + " ))";
	converseVoid(command);
    }

    /**
     * Asserts a ground atomic formula (gaf) in the specified microtheory MT.  The operation
     * will be added to the KB transcript for replication and archive.
     *
     * @param mt the microtheory in which the assertion is made
     * @param predicate the binary predicate of the assertion
     * @param arg1 the first argument of the predicate
    * @param arg2 the second argument of the predicate
    * @param arg3 the third argument of the predicate
     */
    public void assertGaf (CycFort mt,
			   CycConstant predicate,
			   CycFort arg1,
			   Integer arg2,
			   CycFort arg3)
    throws IOException, UnknownHostException, CycApiException {
	// (predicate <CycFort> <CycFort>)
	String command = withBookkeepingInfo() +
			 "(cyc-assert '(" +
			 predicate.stringApiValue() + " " +
			 arg1.stringApiValue() + " " +
			 arg2.toString() + " " +
			 arg3.stringApiValue() + " )" +
			 mt.stringApiValue() + " ))";
	converseVoid(command);
    }

    /**
     * Asserts a ground atomic formula (gaf) in the specified microtheory MT.  The operation
     * will be added to the KB transcript for replication and archive.
     *
     * @param mt the microtheory in which the assertion is made
     * @param predicate the binary predicate of the assertion
     * @param arg1 the first argument of the predicate
    * @param arg2 the second argument of the predicate
    * @param arg3 the third argument of the predicate
     */
    public void assertGaf (CycFort mt,
			   CycConstant predicate,
			   CycFort arg1,
			   Integer arg2)
    throws IOException, UnknownHostException, CycApiException {
	// (predicate <CycFort> <CycFort>)
	String command = withBookkeepingInfo() +
			 "(cyc-assert '(" +
			 predicate.stringApiValue() + " " +
			 arg1.stringApiValue() + " " +
			 arg2.toString() + " )" +
			 mt.stringApiValue() + " ))";
	converseVoid(command);
    }



    /**
     * Returns a with-bookkeeping-info macro expresssion.
     */
    private String withBookkeepingInfo () {
	return "(with-bookkeeping-info (new-bookkeeping-info " +
	cycadministrator.stringApiValue() + " (the-date) " +
	opencycproject.stringApiValue() + " (the-second)) ";
    }

    /**
     * Asserts a genlMt formula in BaseKB.  The operation
     * will be added to the KB transcript for replication and archive.
     *
     * @param <tt>CycFort</tt> mt1 the child Microtheory that inherits from mt2
    *  @param <tt>CycFort</tt> mt2 the parent Microtheory that lends it's assertions to the child
     */
    public void assertGenlMt(CycFort mt1, CycFort mt2) throws IOException, CycApiException {
	assertGaf(baseKB,genlMt,mt1,mt2);
    }

    /**
     * Asserts a genlMt formula in BaseKB.  The operation
     * will be added to the KB transcript for replication and archive.
     *
     * @param <tt>CycFort</tt> mt1 the child Microtheory that inherits from mt2
    *  @param <tt>String</tt> mt2 the parent Microtheory that lends it's assertions to the child
     */

    public void assertGenlMt(CycFort mt1, String mt2) throws IOException, CycApiException {
	assertGaf(baseKB,genlMt,mt1,makeCycConstant(mt2));
    }


    /**
    * Override of <tt>CycAccess</tt>
    * Constructs a new <tt>CycConstant</tt> object using the constant name.
     *
     * @param name Name of the constant. If prefixed with "#$", then the prefix is
     * removed for canonical representation.
     * SPECIAL:  If the name is not considered makeable it appends "LogicMoo" to the end.
     *      If it still fails, it returns the #$MakeCycConstantErrorFn (public CycConstant makeCycConstantError)
     */


    public CycConstant makeCycConstant(String name) {
	String tryName = null;
	if ( name.startsWith("#$") )
	    tryName = name.substring(2);
	else
	    tryName = name;

	try {
	    if ( isValidConstantName(tryName) )
		try {
		    return super.makeCycConstant(tryName);
		} catch ( Exception e ) {
		    return super.makeCycConstant(tryName+ "LogicMoo");
		} else
		System.out.println("!isValidConstantName='"+name+"'");
	    return null;
	    //throw new CycApiException("makeCycConstant: !isValidConstantName='"+name+"'");
	} catch ( Exception ee ) {
	    System.out.println("!makeCycConstant: "+ ee + "='"+name+"'");
	    return null;
	}
    }

    /**
     * Returns true if cycPredicate is a MetaRelation.
     *
     * @param cycPredicate the predicate for determination as a MetaRelation
     * @return <tt>true</tt> iff cycPredicate is a MetaRelation,
     */

    public boolean isMetaRelation( CycFort cycPredicate) throws IOException, CycApiException {
	return isa(cycPredicate,metaRelation);
    }

    /**
    * Constructs a new <tt>CycList</tt> object representing a Gaf using the the three parameters
     *
     * @param <tt>CycConstant</tt> cycPredicate of the Gaf
     * @param <tt>CycConstant</tt> arg1 of the Gaf
     * @param <tt>CycConstant</tt> arg2 of the Gaf
     *
     * @return new <tt>CycList</tt> that represents the new Gaf
     */

    public static CycList makeGaf(CycConstant cycPredicate, CycConstant arg1, CycConstant arg2) {
	CycList cycList = new CycList(cycPredicate);
	cycList.add(arg1);
	cycList.add(arg2);
	return cycList;
    }

    /**
    * Constructs a new <tt>CycList</tt> object representing a Isa based Gaf using the the two parameters
     *
     * @param <tt>CycConstant</tt> term of the Gaf
     * @param <tt>String</tt> type the Collections name
     *
     * @return new <tt>CycList</tt> that represents the new Gaf
     */

    public CycList makeIsa(CycConstant term, String type) {
	return makeGaf(isa,term,makeCycConstant(type));
    }

    /**
    * Constructs a new <tt>CycList</tt> object representing a Isa based Gaf using the the two parameters
     *
     * @param <tt>CycConstant</tt> term of the Gaf
     * @param <tt>CycConstant</tt> type Collection
     *
     * @return new <tt>CycList</tt> that represents the new Gaf
     */

    public CycList makeIsa(CycConstant term, CycConstant type) {
	return makeGaf(isa,term,type);
    }

    public static ArrayList cycKnowsClass = new ArrayList();
    public static ArrayList cycKnowsObject = new ArrayList();

    public void assertJavaObjectAndDef(CycFort dataMt,Object object) {
	if ( object==null ) return;
	assertJavaObject(dataMt,object);
    }


    public String flattenJavaClassname(String cltype) {
	String ctype = cltype;
	String suffix = "";

	if ( cltype.startsWith("[L") ) {
	    ctype = cltype.substring(2);
	    suffix = "Array";
	} else if ( cltype.startsWith("[") ) {
	    ctype = cltype.substring(1);
	    suffix = "Array";
	} else {

	}

	int i = ctype.lastIndexOf('.');
	if ( i>0 ) {
	    return Strings.change(ctype.substring(i+1),';','_')+suffix;
	} else {
	    return Strings.change(ctype,';','_')+suffix;
	}
    }

    public String makeCycJavaConstant(String ctype,String name) {
	String type = flattenJavaClassname(ctype);
	CycConstant nameC = makeCycConstant(name + "_" + type);
	CycConstant typeC = makeCycConstant("Java" + type);
	try {
	    assertIsa(typeC,collection,javaMt);
	    assertIsa(nameC,typeC,javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	return nameC.stringApiValue();
    }

    public static HashMap cycJavaConstantForJavaClass = new HashMap();

    public String makeCycJavaConstantForJavaClass(Class name) {
	CycConstant nameC = (CycConstant)cycJavaConstantForJavaClass.get(name);
	if ( nameC!=null ) return nameC.stringApiValue();
	String classname = name.getName();
	String fclassname = flattenJavaClassname(classname);
	System.out.println("makeCycJavaConstantForJavaClass " + classname + " -> " + fclassname);
	nameC = makeCycConstant(fclassname + "Class");
	cycJavaConstantForJavaClass.put(name,nameC);
	CycConstant typeC = makeCycConstant("ClassClass");
	try {
	    assertIsa(nameC,collection,javaMt);
	    assertIsa(nameC,typeC,javaMt);
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	return nameC.stringApiValue();
    }



    public String assertJavaObject(CycFort dataMt,Object object) {
	System.out.println("assertJavaObject: " + object);
	if ( object==null ) return null;
	Class aclass = object.getClass();
	if ( isJavaReconstuctable(aclass) ) return toReification(dataMt,object);
	if ( cycKnowsObject.contains(object) ) return toReification(dataMt,object);
	String thisobjname = toReification(dataMt,object);
	String thisobjclass = makeCycJavaConstantForJavaClass(aclass);
	try {
	    assertIsa((CycFort)makeCycConstant(thisobjname),(CycFort)makeCycConstant(thisobjclass),dataMt);
	} catch (Exception e) {
	}
	cycKnowsObject.add(object);
	if ( aclass.isArray() ) {
	    for ( int i=0 ; i < ((Object[])object).length; i++ ) {
		String submember = assertJavaObject(dataMt,((Object[])object)[i]);
		try {
		    if ( submember!=null ) assertWithTranscriptNoWffCheck("(#$javaArrayContains " + thisobjname + " " + i + " " + submember + " )",dataMt);
		} catch ( Exception e ) {
		    e.printStackTrace(System.err);
		}
	    }
	    return toReification(dataMt,object);
	}
	HashMap template = makeJavaClassDef(aclass);
	assertJavaObjectMemberValues(dataMt, thisobjname, object,template);
	return toReification(dataMt,object);
    }

    public void assertJavaObjectMemberValues(CycFort dataMt, String objectname, Object object,HashMap template) {
	Iterator it = template.keySet().iterator();
	while ( it.hasNext() ) {
	    Object value = null;
	    Object accessname = it.next();
	    Object accessmethod =template.get(accessname);
	    String name = null;

	    if ( accessmethod instanceof Field ) {
		try {
		    value = ((Field)accessmethod).get(object);
		    if ( value!=null ) assertWithTranscriptNoWffCheck( "#$hasFieldValue " + objectname + " " + accessname + " " + assertJavaObject(dataMt,value) + " )",dataMt);
		} catch ( Exception e ) {
		    e.printStackTrace(System.err);
		}

	    } else {
		if ( accessmethod instanceof DataMethod ) {
		    DataMethod dm = (DataMethod)accessmethod;
		    name = dm.cycname;
		    try {
			value  = dm.method.invoke(object,null);
			if ( value!=null ) assertWithTranscriptNoWffCheck( "#$hasMethodValue " + objectname  + " " + accessname + " " + assertJavaObject(dataMt,value) + " )",dataMt);
		    } catch ( Exception e ) {
			e.printStackTrace(System.err);
		    }

		}
	    }

	}
    }

    // hasMethodValue
    // hasFieldValue 
    // javaArrayContains

    // hasJavaMethod
    // JavaMethodFn
    public static HashMap cycAccessClassTemplates = new HashMap();


    public HashMap makeJavaClassDef(Class aclass) {
	if ( aclass==null ) return new HashMap();
	if ( isJavaReconstuctable(aclass) ) return new HashMap();

	HashMap template = (HashMap)cycAccessClassTemplates.get(aclass);
	if ( template != null )	return template;
	template = new HashMap();
	String cycClassName = makeCycJavaConstantForJavaClass(aclass);
	System.out.println("serializing " + cycClassName);
	cycAccessClassTemplates.put(aclass,template);
	cycAccessClassTemplates.put(cycClassName,template);
	makeJavaClassDef(aclass.getSuperclass());

	Field[] fields = aclass.getFields();
	for ( int i =0; i<fields.length;i++ ) {
	    Field field=fields[i];
	    makeJavaClassDef(field.getType());
	    String cycFieldName = makeCycJavaConstant("Field",field.getName());
	    String cycFieldResultType = makeCycJavaConstantForJavaClass(field.getType());
	    template.put(cycFieldName,field);
	    try {
		assertWithTranscriptNoWffCheck(
					      "(#$hasJavaField " + cycClassName + " " + cycFieldName + "  " + cycFieldResultType +  " )", javaMt);
	    } catch ( Exception e ) {
		e.printStackTrace(System.err);
	    }
	}

	Method[] methods = aclass.getMethods();
	for ( int i =0; i<methods.length;i++ ) {
	    Method method = methods[i];
	    Class returntype = method.getReturnType();
	    String returntypename = makeCycJavaConstantForJavaClass(returntype);
	    makeJavaClassDef(returntype);
	    String methodname = makeCycJavaConstant("Method",method.getName());
	    template.put(methodname,method);
	    Class[] params = method.getParameterTypes();
	    String methodparams = makeJavaClassesDef(params);
	    try {
		assertWithTranscriptNoWffCheck(
					      "(#$hasJavaMethod " +  cycClassName 
					      + " (#$JavaMethodAccessFn " + methodname + " " + methodparams + " ) " 
					      + returntypename +  " )", (CycFort)javaMt);
	    } catch ( Exception e ) {
		e.printStackTrace(System.err);
	    }
	    String cycDMName = null;
	    if ( params.length==0 || !(returntype == Void.class) ) {
		if ( methodname.startsWith("get") ) cycDMName = makeCycJavaConstant("DataMethod",methodname.substring(2));
		// if ( isJavaReconstuctable(returntype) )	cycDMName = makeCycJavaConstant("DataMethod",methodname);
		if ( cycDMName!=null ) {
		    // DataMethod
		    DataMethod dm = new DataMethod(cycDMName,method);
		    template.put(cycDMName,dm);
		    try {
			assertWithTranscriptNoWffCheck(
						      "(#$hasJavaDataMethod " + cycClassName 
						      + " " + cycDMName + "  " 
						      + returntypename +  " )",(CycFort) javaMt);
		    } catch ( Exception e ) {
			e.printStackTrace(System.err);
		    }

		}
	    }
	}
	return template;
    }

    public String makeJavaClassesDef(Class aclass[]) {
	StringBuffer cdefs = new StringBuffer(10);
	for ( int i = 0 ; i < aclass.length ; i ++ ) {
	    makeJavaClassDef(aclass[i]); 
	    cdefs.append(" ").append(makeCycJavaConstantForJavaClass(aclass[i]));
	}
	return cdefs.toString();
    }

    public String toReification(CycFort dataMt,Object object) {
	if ( object instanceof String )	return "\""+object+"\"";
	Class aclass = object.getClass();
	if ( isJavaReconstuctable(aclass) ) return ""+object;
	if ( aclass.isArray() ) {
	    return reifyConstant(dataMt,"HYPCollectionOf-"+flattenJavaClassname(aclass.getName())+object.hashCode(),aclass);
	}
	return reifyConstant(dataMt,"HYP-"+flattenJavaClassname(aclass.getName())+object.hashCode(),aclass);
    }

    public String reifyConstant(CycFort dataMt,String name, Class aclass) {
	try {
	    CycConstant nameC = makeCycConstant(name);
	    assertIsa((CycFort)nameC,(CycFort)makeCycConstant(makeCycJavaConstantForJavaClass(aclass)),dataMt);
	    return nameC.stringApiValue();
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	    return name; 
	}
    }

    public boolean isJavaReconstuctable(Class aclass) {
	if ( aclass == null ) return false;
	if ( aclass.isArray() )	return false;
	try {
	    if ( aclass.isPrimitive()
		 || (Modifier.isFinal(aclass.getModifiers()) && aclass.getDeclaredMethod("toString",null)!=null ) )  return true;
	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}
	return false;
    }


    /**
     * Asserts the given sentence and also places it on the transcript queue
     * with default strength and direction.
     * However, no WFF checks are performed
     *
     * @param <tt>String</tt> sentence the given sentence for assertion
     * @param <tt>CycFort</tt> mt the microtheory in which the assertion is placed
     */

    public void assertWithTranscriptNoWffCheck(String sentence, CycFort mt)
    throws IOException, UnknownHostException, CycApiException {

	String command =
	"(clet ((*the-cyclist* " + cyclist.cyclify() + "))\n" +
	"   (without-wff-semantics (ke-assert-now\n" +
	"    '" + sentence + "\n" +
	"    " + mt.cyclify() + ")))";

	// System.out.println("assertWithTranscript: " + command);
	converseVoid(command);
    }


    /**
     * Asserts the given sentence and also places it on the transcript queue
     * with default strength and direction.
     * However, no WFF checks are performed
     *
     * @param <tt>CycList</tt> sentence the given sentence for assertion
     * @param <tt>CycFort</tt> mt the microtheory in which the assertion is placed
     */

    public void assertWithTranscriptNoWffCheck(CycList sentence, CycFort mt)
    throws IOException, UnknownHostException, CycApiException {
	if ( sentence==null )
	    throw new CycApiException("assertWithTranscriptNoWFFCheck: sentence==null");
	if ( mt==null )
	    throw new CycApiException("assertWithTranscriptNoWFFCheck: mt==null " + sentence.cyclify());
	if ( cyclist==null )
	    throw new CycApiException("assertWithTranscriptNoWFFCheck: cyclist==null" + sentence.cyclify());

	String command =
	"(clet ((*the-cyclist* " + cyclist.cyclify() + "))\n" +
	"   (without-wff-semantics (ke-assert-now\n" +
	"    '" + sentence.cyclify() + "\n" +
	"    " + mt.cyclify() + ")))";

	// System.out.println("assertWithTranscript: " + command);
	converseVoid(command);
    }

    public static ArrayList intersectsArrayList(ArrayList l1, ArrayList l2) {
	ArrayList list = new ArrayList();
	Iterator list1 = l1.iterator();
	while ( list1.hasNext() ) {
	    Object  item = list1.next();
	    if ( l2.contains(item) ) list.add(item);
	}
	return list;
    }

    public void createCollection(
				String microtheoryname,String username, String collectionname,
				String genlsname,String commentname,String categoryname)  throws CycApiException {
	try {
	    setCyclist(username);
	    CycConstant microtheorynameC = makeCycConstantOrNull(microtheoryname);
	    CycConstant usernameC = makeCycConstantOrNull(username);
	    CycConstant microtheoryC = makeCycConstantOrNull(microtheoryname);
	    CycConstant collectionnameC = makeCycConstantOrNull(collectionname);
	    CycConstant genlsnameC = makeCycConstantOrNull(genlsname);
	    CycConstant categorynameC = makeCycConstantOrNull(categoryname);
	    if ( microtheoryname!=null && collectionname!=null ) {
		assertIsa(collectionnameC,collection,microtheoryC);
		if ( genlsnameC!= null ) {
		    assertGenls(collectionnameC,genlsnameC,microtheoryC);
		}
		if ( categorynameC!= null ) {
		    assertIsa(collectionnameC,categorynameC,microtheoryC);
		}
		if ( commentname.length()>3 ) {
		    assertComment(collectionnameC,commentname,microtheoryC);
		}
	    }

	} catch ( Exception e ) {
	    throw new CycApiException("createPredicate: " + e);
	}

    }

    public static PrintWriter getPrintWriter(Writer w) {
	if ( w==null ) return new PrintWriter(System.out);
	if ( w instanceof PrintWriter )	return(PrintWriter)w;
	return new PrintWriter(w);
    }


    public void createPredicate(
			       String microtheoryname,String username, 
			       String predicatename, String genlpredsname, String arityname,
			       String commentname, String categoryname,
			       String[] arg, String[] isagenl ) throws CycApiException {
	try {
	    setCyclist(username);
	    String realarityname = aryFromString(arityname);
	    int arityN = arityFromString(realarityname).intValue();
	    CycConstant microtheorynameC = makeCycConstantOrNull(microtheoryname);
	    CycConstant usernameC = makeCycConstantOrNull(username);
	    CycConstant microtheoryC = makeCycConstantOrNull(microtheoryname);
	    CycConstant predicatenameC = makeCycConstantOrNull(predicatename);
	    CycConstant genlpredsnameC = makeCycConstantOrNull(genlpredsname);
	    CycConstant categorynameC = makeCycConstantOrNull(categoryname);
	    CycConstant realaritynameC = makeCycConstantOrNull(realarityname);


	    if ( microtheoryname!=null && predicatename!=null && realarityname!=null ) {
		assertIsa(predicatenameC,predicate,microtheoryC);
		assertIsa(predicatenameC,realaritynameC,microtheoryC);
		assertGaf(microtheoryC,predarity,predicatenameC, new Integer(arityN));
		if ( genlpredsnameC!= null ) {
		    assertGaf(microtheoryC,genlpreds,predicatenameC,genlpredsnameC);
		}
		if ( categorynameC!= null ) {
		    assertIsa(predicatenameC,categorynameC,microtheoryC);
		}
		if ( commentname.length()>3 ) {
		    assertComment(predicatenameC,commentname,microtheoryC);
		}
		for ( int i=1 ; i <= arityN ;i++ ) {
		    if ( isagenl[i].startsWith("G") ) {
			assertGaf(microtheoryC,argGenl,predicatenameC, new Integer( i),makeCycConstant(arg[i]));
		    } else {
			assertGaf(microtheoryC,argIsa,predicatenameC, new Integer( i),makeCycConstant(arg[i]));
		    }
		}
	    }

	} catch ( Exception e ) {
	    throw new CycApiException("createPredicate: " + e);
	}

    }
    public void createFunction(
			      String microtheoryname,String username, 
			      String functionname, String genlpredsname,String arityname,
			      String commentname, String categoryname,
			      String resultisaname,String resultisagenlname,
			      String[] arg, String[] isagenl )  throws CycApiException {
	try {
	    setCyclist(username);
	    CycConstant microtheorynameC = makeCycConstantOrNull(microtheoryname);
	    CycConstant usernameC = makeCycConstantOrNull(username);
	    CycConstant microtheoryC = makeCycConstantOrNull(microtheoryname);
	    CycConstant functionnameC = makeCycConstantOrNull(functionname);
	    CycConstant genlpredsnameC = makeCycConstantOrNull(genlpredsname);
	    CycConstant categorynameC = makeCycConstantOrNull(categoryname);
	    CycConstant resultisanameC = makeCycConstantOrNull(resultisaname);
	    int arityN = Integer.parseInt(arityname);

	    if ( microtheoryname!=null && functionname!=null ) {
		assertIsa(functionnameC,functionalRelation,microtheoryC);
		assertGaf(microtheoryC,predarity,functionnameC, new Integer(arityN));
		if ( genlpredsnameC!= null ) {
		    assertGaf(microtheoryC,genlpreds,functionnameC,genlpredsnameC);
		}
		if ( categorynameC!= null ) {
		    assertIsa(functionnameC,categorynameC,microtheoryC);
		}
		if ( resultisanameC!= null ) {
		    if ( resultisagenlname.startsWith("G") )
			assertGaf(microtheoryC,resultGenl,functionnameC,resultisanameC);
		    else
			assertGaf(microtheoryC,resultIsa,functionnameC,resultisanameC);
		}
		if ( commentname.length()>3 ) {
		    assertComment(functionnameC,commentname,microtheoryC);
		}
		for ( int i=1 ; i <= arityN ;i++ ) {
		    if ( isagenl[i].startsWith("G") ) {
			assertGaf(microtheoryC,argGenl,functionnameC, new Integer( i),makeCycConstant(arg[i]));
		    } else {
			assertGaf(microtheoryC,argIsa,functionnameC, new Integer( i),makeCycConstant(arg[i]));
		    }
		}
	    }

	} catch ( Exception e ) {
	    throw new CycApiException("createPredicate: " + e);
	}
    }


    public ArrayList getArrayList(String mt,String collectionname) {
	try {
	    CycList query = new CycList(isa);
	    CycVariable variable = new CycVariable("?INST");
	    query.add(variable);
	    query.add(makeCycConstant(collectionname));
	    return(ArrayList) askWithVariable(query,variable,makeCycConstant(mt));
	} catch ( Exception e ) {
	    return new ArrayList();
	}
    }

    /**
     * Returns a list of bindings for a query with a single unbound variable.
     *
     * @param query the query to be asked in the knowledge base
     * @param variable the single unbound variable in the query for which bindings are sought
     * @param mt the microtheory in which the query is asked
     * @return a list of bindings for the query
     */

    public ArrayList getArrayListQuerySEL(String mt,String query) {
	try {
	    StringBuffer queryBuffer = new StringBuffer();
	    queryBuffer.append("(clet ((*cache-inference-results* nil) ");
	    queryBuffer.append("       (*compute-inference-results* nil) ");
	    queryBuffer.append("       (*unique-inference-result-bindings* t) ");
	    queryBuffer.append("       (*generate-readable-fi-results* nil)) ");
	    queryBuffer.append("  (without-wff-semantics ");
	    queryBuffer.append("    (ask-template '" + "?SEL" + " ");
	    queryBuffer.append("                  '" + query + " ");
	    queryBuffer.append("                  #$" + mt + " ");
	    queryBuffer.append("                  0 nil nil nil)))");
	    System.out.println(queryBuffer.toString());
	    return(ArrayList)converseList(queryBuffer.toString());
	} catch ( Exception e ) {
	    return new ArrayList();
	}
    }




    public static Integer arityFromString(String term) {
	switch ( term.charAt(0) ) {
	    case 'V':
		return new Integer(0);
	    case 'U':
		return new Integer(1);
	    case 'B':
		return new Integer(2);
	    case 'T':
		return new Integer(3);
	    case 'Q':
		if ( term.charAt(2) =='a' )
		    return new Integer(4);
		else
		    return new Integer(5);
	}
	return null;
    }

    public static String aryFromString(String term) {
	return term;
    }

    public static String aryFromInt(int term) {

	switch ( term ) {
	    case 0:
		return "VariableArity" ;
	    case 1:
		return "Unary" ;
	    case 2:
		return "Binary";
	    case 3:
		return "Ternary";
	    case 4:
		return "Quaternary";
	    case 5:
		return "Quintary" ;
	}
	return null;
    }


    public CycConstant makeCycConstantOrNull(String name) {
	if ( name==null ) return null;
	if ( name.length()<2 ) return null;
	if ( name.startsWith("<") ) return null;
	if ( name.startsWith("*") ) return null;
	return makeCycConstant(name);
    }

    public static CycConstant argIsa = null;
    public static CycConstant argGenl = null;
    public static CycConstant resultIsa = null;
    public static CycConstant resultGenl = null;
    public static CycConstant predicate = null;
    public static CycConstant relation = null;
    public static CycConstant microtheory = null;
    public static CycConstant cyclist = null;
    public static CycConstant predarity = null;
    public static CycConstant genlpreds = null;
    public static CycConstant cyclistDefinitionalMt = null;
    public static CycConstant cycBasedProject = null;
    public static CycConstant metaRelation = null;
    public static CycConstant logicMooMt = null;
    public static CycConstant jamudMt = null;
    public static CycConstant javaMt = null;
    public static CycConstant makeCycConstantError = null;
    public static CycConstant cycadministrator = null;
    public static CycConstant opencycproject = null;
    public static CycConstant functionalRelation = null;

    public static Object cycMooHTMLTool = null;

    public final class DataMethod {
	public String cycname = null;
	public String dataname = null;
	public Method method = null;
	public DataMethod(String acycname, Method amethod) {
	    cycname = acycname;  
	    method = amethod;  
	}
    }


}







