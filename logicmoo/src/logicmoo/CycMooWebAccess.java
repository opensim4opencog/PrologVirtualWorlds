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
* @version $Id: CycMooWebAccess.java,v 1.1 2002-04-12 16:45:43 dmiles Exp $
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
public class CycMooWebAccess extends CycMooAccess {

    /**
     * Constructs a new CycMooWebAccess object.
     */
    public CycMooWebAccess() throws IOException, UnknownHostException, CycApiException {
	super();
	cycMooAccessInit();
    }

    /**
     * Constructs a new CycMooWebAccess object to the given CycProxyAgent in the given
     * agent community.
     *
     * @param myAgentName the name of the local agent
     * @param cycProxyAgentName the name of the cyc proxy agent
     * @param agentCommunity the agent community to which the cyc proxy agent belongs
     */
    public CycMooWebAccess (String myAgentName,
			 String cycProxyAgentName,
			 int agentCommunity) throws IOException, CycApiException  {
	super(myAgentName,cycProxyAgentName,agentCommunity);
	cycMooAccessInit();
    }

    /**
     * Constructs a new CycMooWebAccess object given a host name, port, communication mode and persistence indicator.
     *
     * @param hostName the host name
     * @param basePort the base (HTML serving) TCP socket port number
     * @param communicationMode either ASCII_MODE or BINARY_MODE
     * @param persistentConnection when <tt>true</tt> keep a persistent socket connection with
     * the OpenCyc server
     */
    public CycMooWebAccess(String hostName, int basePort, int communicationMode, boolean persistentConnection)
    throws IOException, UnknownHostException, CycApiException {
	super( hostName,  basePort,  communicationMode,  persistentConnection);
	cycMooAccessInit();
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
	    pw.println("createPredicate for " + username);
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
		for (int i=1 ; i <= arityN ;i++) {
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


}

