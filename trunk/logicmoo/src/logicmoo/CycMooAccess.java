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
* @version $Id: CycMooAccess.java,v 1.4 2002-04-17 06:58:14 dmiles Exp $
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


            cycTrue = getConstantByName("#$True");
            cycFalse = getConstantByName("#$False");
            cycVoid = getConstantByName("#$voidValue");

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

        String tryName = name;
        if ( name.startsWith("#$") )
            tryName = name.substring(2);

        tryName = Strings.change(tryName,'$','_');
        tryName = Strings.change(tryName,')','_');
        tryName = Strings.change(tryName,'(','_');
        tryName = Strings.change(tryName,' ','_');
        tryName = Strings.change(tryName,'.','_');
        tryName = Strings.change(tryName,'@','_');
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


    public Object makePrimitiveObjectForCycList(Class jclass, Object value) {
        if ( jclass == java.lang.Boolean.TYPE ) if ( value.equals(Boolean.TRUE) ) return cycTrue;
            else return cycFalse;
        if ( jclass == java.lang.Integer.TYPE ) return new Integer(""+value);
        if ( jclass == java.lang.Byte.TYPE ) return new Integer(""+value);
        if ( jclass == java.lang.Void.TYPE ) return cycVoid;
        if ( jclass == java.lang.Long.TYPE ) return new Long(""+value);
        if ( jclass == java.lang.Character.TYPE ) return  new String("`" + value);
        if ( jclass == java.lang.Double.TYPE ) return new CycSymbol("|" + value + "|");
        if ( jclass == java.lang.Float.TYPE ) return new CycSymbol("|" + value + "|");
        return null;
    }

    public Object makeNonPrimitiveObjectForCycList(Object value) {
        if ( value instanceof String ) return value;
        if ( value instanceof Boolean )  if ( value.equals(Boolean.TRUE)) return cycTrue;
            else return cycFalse;
        if ( value instanceof Character ) return  new String("`" + value);
        if ( value instanceof Integer ) return value;
        if ( value instanceof Long ) return value;
        if ( value instanceof Double ) return value;
        if ( value instanceof Float ) return value;
        if ( value instanceof Byte ) return (Integer)value;
        return null;
    }

    public static HashMap cycKnowsObject = new HashMap();

    public Object makeCycJavaObject(CycFort dataMt,Object object) {
        if ( object==null ) return cycNull;
        Class jclass = object.getClass();
        if ( jclass.isPrimitive() )  return makePrimitiveObjectForCycList(jclass, object);
        Object cycjobject = makeNonPrimitiveObjectForCycList(object);
        if ( cycjobject!=null ) return cycjobject;

        if ( cycKnowsObject.containsKey(object) ) return cycKnowsObject.get(object);
        System.out.println("assertJavaObject: " + object);

        CycConstant  cycclass = makeCycJavaClass(jclass);
        String  thisobjname = "HYP-"+cycclass.toString()+object.hashCode();
        CycConstant cycobject = makeCycConstant(thisobjname);
        cycKnowsObject.put(object,cycobject);

        try {
            assertIsa((CycFort)cycobject,(CycFort)cycclass,dataMt);
        } catch ( Exception e ) {
            e.printStackTrace(System.out);
        }

        syncJavaObject(dataMt, object,cycobject);
        return cycobject;
    }

    public void syncJavaObjectArray(CycFort dataMt, Object object,CycConstant cycobject) {
        System.out.println("syncJavaObjectArray " + object );
        CycList assertme = new CycList(cycHasArrayMember);
        assertme.add(cycobject);
        assertme.add(null);
        assertme.add(null);
        for ( int i=0 ; i < ((Object[])object).length; i++ ) {
            Object submember = makeCycJavaObject(dataMt,((Object[])object)[i]);
            try {
                assertme.set(3,new Integer(i));
                assertme.set(4,submember);
                assertWithTranscriptNoWffCheck(assertme,dataMt);
            } catch ( Exception e ) {
                e.printStackTrace(System.err);
            }
        }
    }

    public void syncJavaObject(CycFort dataMt, Object object,CycConstant cycobject) {
        Class jclass = object.getClass();
        if ( jclass.isArray() ) {
            syncJavaObjectArray(dataMt,object, cycobject);
            return;
        }

        System.out.println("syncJavaObject " + object );
        HashMap template = (HashMap)cycAccessClassTemplates.get(jclass);
        Iterator it = template.keySet().iterator();
        while ( it.hasNext() ) {
            Object cycaccess = it.next();
            if ( cycaccess instanceof CycConstant ) {
                syncJavaObjectMember( dataMt, cycobject, object ,(CycConstant)cycaccess, template.get(cycaccess));
            }
        }
    }

    public void syncJavaObjectMember(CycFort dataMt, CycConstant cycobject, Object object, CycConstant cycaccess, Object accessmember) {
        System.out.println("while {syncJavaObject " + cycobject + " " + cycaccess + " " + accessmember + "}" );
        try {
            if ( accessmember instanceof DataMethod ) syncJavaObjectDataMethod( dataMt, cycobject, object, cycaccess, (DataMethod)accessmember);
            //	    if ( accessmember instanceof Method ) syncJavaObjectMethod( dataMt, cycobject, object, cycaccess, (Method)accessmember);
            if ( accessmember instanceof Field ) syncJavaObjectField( dataMt, cycobject, object, cycaccess, (Field)accessmember);
        } catch ( Exception e ) {
            e.printStackTrace( System.out);
        }
    }

    public void syncJavaObjectField(CycFort dataMt, CycConstant cycobject, Object object, CycConstant cycaccess, Field accessmember) 
    throws Exception{
        CycList assertme = new CycList(cycHasArrayMember);    //"#$hasFieldValue"
        assertme.add(cycaccess);
        assertme.add(makeCycJavaObject(dataMt,accessmember.get(object)));
        assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void syncJavaObjectMethod(CycFort dataMt, CycConstant cycobject, Object object, CycConstant cycaccess, Method accessmember) 
    throws Exception{
        CycList assertme = new CycList(cycHasArrayMember);    //"#$hasMethodValue"
        assertme.add(cycaccess);
        assertme.add(makeCycJavaObject(dataMt,accessmember.invoke(object,null)));
        assertWithTranscriptNoWffCheck(assertme,dataMt);
    }

    public void syncJavaObjectDataMethod(CycFort dataMt, CycConstant cycobject, Object object, CycConstant cycaccess, DataMethod accessmember) 
    throws Exception{
        CycList assertme = new CycList(cycHasArrayMember);    //"#$hasMethodValue"
        assertme.add(cycaccess);
        assertme.add(makeCycJavaObject(dataMt,accessmember.get(object)));
        assertWithTranscriptNoWffCheck(assertme,dataMt);
    }


    // CycConstant & Class -> CycConstant key of Fields | DataMethod | Method
    public static HashMap cycAccessClassTemplates = new HashMap();

    // CycConstant || Class -> Class || CycConstant 
    public static HashMap cycKnowsClass = new HashMap();

    public CycConstant makeCycJavaClass(Class jclass) throws CycApiException {
        if ( jclass==null ) throw new CycApiException("null makeCycJavaClass");
        CycConstant cycjclass  = (CycConstant)cycKnowsClass.get(jclass);
        if ( cycjclass!=null ) return cycjclass;
        String classname = jclass.getName();
        String fclassname = flattenJavaClassname(classname);
        System.out.println("makeCycJavaClass " + classname + " -> " + fclassname);
        cycjclass = makeCycConstant(fclassname + "Class");
        cycKnowsClass.put(jclass,cycjclass);
        cycKnowsClass.put(cycjclass,jclass);
        HashMap template = new HashMap();
        cycAccessClassTemplates.put(jclass,template);
        cycAccessClassTemplates.put(cycjclass,template);

        if ( jclass.toString().startsWith("class java.") || jclass.isPrimitive() )
            return cycjclass;

        try {
            assertIsa(cycjclass,collection,javaMt);

            Class superjclass = jclass.getSuperclass();
            if ( superjclass != null ) {
                CycConstant cycsuperjclass = makeCycJavaClass(superjclass);
                assertGenls(cycjclass,cycsuperjclass,javaMt);
            }
        } catch ( Exception e ) {
            e.printStackTrace(System.err);
            throw new CycApiException("makeCycJavaClass: " + e );
        }

        Field[] fields = jclass.getFields();
        for ( int i =0; i<fields.length;i++ ) {
            Field field=fields[i];
            CycConstant cycfieldjclass = makeCycJavaClass(field.getType());
            CycConstant cycfield = makeCycJavaMember("Field",field.getName());
            template.put(cycfield,field);
            assertIsaJavaFieldOf(cycjclass,cycfield,cycfieldjclass);
        }

        Method[] methods = jclass.getDeclaredMethods();
        for ( int i =0; i<methods.length;i++ ) {
            Method method = methods[i];
            CycConstant cycmethodjclass = makeCycJavaClass(method.getReturnType());
            String methodname = method.getName();
            CycConstant cycmethod = makeCycJavaMember("Method",methodname);
            template.put(cycmethod,method);
            Class[] params = method.getParameterTypes();
            assertIsaJavaMethodOf(cycjclass,cycmethod,params,cycmethodjclass);

            if ( methodname.startsWith("get") || params.length==0 ) {
                String dataname = methodname.substring(2);
                CycConstant cycdatamethod = makeCycJavaMember("DataMethod",dataname);
                Object returntypearray = Array.newInstance(method.getReturnType(),1);
                Method setmethod = null;
                try {
                    setmethod = jclass.getMethod("set"+dataname,(Class[])returntypearray); 
                } catch ( Exception e ) {
                }
                DataMethod datamethod = new DataMethod(dataname,method,setmethod);
                assertIsaJavaDataMethodOf(cycjclass,cycdatamethod,cycmethodjclass);
            }

        }
        return cycjclass;
    }

    public void assertIsaJavaFieldOf(CycConstant cycjclass,CycConstant cycfield,CycConstant cycfieldjclass) {
    }

    public void assertIsaJavaDataMethodOf(CycConstant cycjclass,CycConstant cycdatamethod, CycConstant cycmethodjclass) {
    }

    public void assertIsaJavaMethodOf(CycConstant cycjclass,CycConstant cycmethod,Class[] params, CycConstant cycmethodjclass) {
    }


    /*
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
    if ( methodname.startsWith("get") ) {
    cycDMName = makeCycJavaMember("DataMethod",methodname.substring(2));
    // if ( isJavaReconstuctable(returntype) )	cycDMName = makeCycJavaMember("DataMethod",methodname);
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
    }
    public String makeJavaClassesDef(Class jclass[]) {
    StringBuffer cdefs = new StringBuffer(10);
    for ( int i = 0 ; i < jclass.length ; i ++ ) {
        assertJavaClass(jclass[i]); 
        cdefs.append(" ").append(makeCycJavaConstantForJavaClass(jclass[i]));
    }
    return cdefs.toString();
    }
    */    



    public String flattenJavaClassname(String cltype) {
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
        String type = flattenJavaClassname(ctype);
        CycConstant nameC = makeCycConstant(name + "_" + type);
        CycConstant typeC = makeCycConstant("Java" + type);
        try {
            assertIsa(typeC,collection,javaMt);
            assertIsa(nameC,typeC,javaMt);
        } catch ( Exception e ) {
            e.printStackTrace(System.err);
        }
        return nameC;
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
        if ( w instanceof PrintWriter ) return(PrintWriter)w;
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

    public static CycConstant cycTrue = null;
    public static CycConstant cycFalse = null;
    public static CycConstant cycVoid = null;
    public static CycConstant cycNull = null;
    public static CycConstant cycHasArrayMember = null;
    public static CycConstant cycHasMethod = null;
    public static CycConstant cycHasDataMethod = null;
    public static CycConstant cycHasField = null;
    public static CycConstant cycHasMethodValue = null;
    public static CycConstant cycHasDataMethodValue = null;
    public static CycConstant cycHasFieldValue = null;

    public static Object cycMooHTMLTool = null;

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
            return getmethod.invoke(object,null);
        }
        public void set(Object object,Object value) {
            Object[] param = {value};
            setmethod.invoke(object,param);
        }
    }


}
















