
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


/**
* Provides wrappers for the OpenCyc API.<p>
*
* Collaborates with the <tt>Jamud</tt> class which manages the api connections.
*
* @version $Id: JamudWorldAccess.java,v 1.1 2002-04-12 15:29:24 dmiles Exp $
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
public class JamudWorldAccess {


    public static Jamud jamudInstance;
    public static MudObjectRoot jamudMudObjectRoot; 
    public static MudObject jamudMudObjectBelowRoot;     
    public static Object initialObject; 

    /**
     * Constructs a new running JamudWorldAccess object.
     */
    public JamudWorldAccess() {
	start();
    }


    public void start() {

	if ( jamudInstance==null )
	    realStart(initialObject);
    }

    private void realStart(Object initial) {

	String[] args = { "jamud.xml"} ; 
	try {
	    Jamud.main( args );
	    jamudInstance = Jamud.currentInstance();
	    jamudMudObjectRoot = jamudInstance.mudObjectRoot();
	    //Iterator cobjs = jamudMudObjectRoot.childObjects();
	    //jamudMudObjectBelowRoot = (MudObject)cobjs.next();

	} catch ( Exception e ) {
	    e.printStackTrace(System.err);
	}

    }

    public void restart() {
	start();
	stop();
    }

    public void stop() {
	jamudInstance.terminate();
	jamudInstance = null;
    }


}

