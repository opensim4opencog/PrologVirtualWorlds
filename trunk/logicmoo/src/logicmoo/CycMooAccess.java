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

import javax.servlet.jsp.*;



/**
* Provides wrappers for the OpenCyc API.<p>
*
* Collaborates with the <tt>CycConnection</tt> class which manages the api connections.
*
* @version $Id: CycMooAccess.java,v 1.1 2002-04-12 15:29:24 dmiles Exp $
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
	    argIsa = getConstantByName("#$argIsa");
	    argGenl = getConstantByName("#$argGenl");
	    predicate = getConstantByName("#$Predicate");
	    relation = getConstantByName("#$Relation");
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
	    return super.makeCycConstant(tryName);
	} catch ( Exception e ) {
	    try {
		return super.makeCycConstant(tryName+ "LogicMoo");
	    } catch ( Exception ee ) {
		return makeCycConstantError; 
	    }
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


    public void writeOptionsHtml(PageContext pageContext,String collectionname, String sel) {
	writeOptionsHtml( pageContext, getArrayList(pageContext,collectionname),  sel);
    }

    public void writeOptionsHtml(PageContext pageContext,String collectionname1, String collectionname2, String sel) {
	writeOptionsHtml( pageContext, intersectsArrayList(getArrayList(pageContext,collectionname1),getArrayList(pageContext,collectionname2)),  sel);
    }

    public void writeOptionsHtml(PageContext pageContext,String collectionname1, String collectionname2, String collectionname3, String sel) {
	writeOptionsHtml( pageContext, intersectsArrayList(getArrayList(pageContext,collectionname1), intersectsArrayList(getArrayList(pageContext,collectionname2),getArrayList(pageContext,collectionname3))),  sel);
    }

    public ArrayList getArrayList(PageContext pageContext,String collectionname) {
	try {
	    String mt = (String)pageContext.getSession().getAttribute("microtheory");
	    if ( !(mt instanceof String) ) {
		return(ArrayList)getAllInstances(makeCycConstant(collectionname));
	    } else {
	      //  System.out.println("getArrayList: using " + mt);
		CycList query = new CycList(isa);
		CycVariable variable = new CycVariable("?INST");
		query.add(variable);
		query.add(makeCycConstant(collectionname));
		return(ArrayList) askWithVariable(query,variable,makeCycConstant(mt));
	    }
	} catch ( Exception e ) {
	    return new ArrayList();
	}
    }

    public ArrayList intersectsArrayList(ArrayList l1, ArrayList l2) {
	ArrayList list = new ArrayList();
	Iterator list1 = l1.iterator();
	while ( list1.hasNext() ) {
	    Object  item = list1.next();
	    if ( l2.contains(item) ) list.add(item);
	}
	return list;
    }

    public void writeOptionsHtml(PageContext pageContext,ArrayList list, String sel) {
	PrintWriter  pw=null;
	try {
	    pw = pageContext.getResponse().getWriter();
	    pw.flush();
	} catch ( IOException ioe ) {
	}
	writeOptionsHtml(pw, list,  sel);
    }

    public void writeOptionsHtml(Writer pw,ArrayList list, String sel) {
	CycFort s = null;
	try {
	    pw.flush();
	    Collections.sort(list);
	    Iterator result = list.iterator();
	    while ( result.hasNext() ) {
		s = ((CycFort) result.next());
		if ( s instanceof CycConstant ) {
		    if ( s.toString().startsWith(sel) )
			pw.write("<option SELECTED>" + s + "</option>");
		    else
			pw.write("<option>" + s + "</option>");
		}
	    }

	    pw.flush();
	} catch ( Exception e ) {
	}
    }

    public void createCollection(PageContext pageContext,String microtheoryname,String username, String collectionname,String genlsname,String commentname,String categoryname) {
	PrintWriter  pw=null;
	try {
	    pw = pageContext.getResponse().getWriter();
	    pw.flush();
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
	    e.printStackTrace(pw);
	}
	pw.flush();

    }
    public void createPredicate(PageContext pageContext,
				String microtheoryname,String username, 
				String predicatename, String genlpredsname,String arityname,
				String commentname, String categoryname,
				String[] arg, String[] isagenl ) {
	PrintWriter  pw=null;
	try {
	    pw = pageContext.getResponse().getWriter();
	    pw.flush();
	    setCyclist(username);
	    CycConstant microtheorynameC = makeCycConstantOrNull(microtheoryname);
	    CycConstant usernameC = makeCycConstantOrNull(username);
	    CycConstant microtheoryC = makeCycConstantOrNull(microtheoryname);
	    CycConstant predicatenameC = makeCycConstantOrNull(predicatename);
	    CycConstant genlpredsnameC = makeCycConstantOrNull(genlpredsname);
	    CycConstant categorynameC = makeCycConstantOrNull(categoryname);
	    int arityN = Integer.parseInt(arityname);
	    if ( microtheoryname!=null && predicatename!=null ) {
		assertIsa(predicatenameC,predicate,microtheoryC);
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
		for (int i=1 ; i >= arityN ;i++) {
		    if (isagenl[i].startsWith("G")) {
			assertGaf(microtheoryC,argGenl,predicatenameC, new Integer( i),makeCycConstant(arg[i]));
		    } else {
			assertGaf(microtheoryC,argIsa,predicatenameC, new Integer( i),makeCycConstant(arg[i]));
		    }
		}
	    }

	} catch ( Exception e ) {
	    e.printStackTrace(pw);
	}
	pw.flush();

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



}
