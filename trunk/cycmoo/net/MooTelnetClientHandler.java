package logicmoo.net;

import logicmoo.api.*;
import logicmoo.agent.*;
import logicmoo.util.*;


// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;

// OpenCyc
import org.opencyc.api.*;
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;


public class MooTelnetClientHandler extends MooCommandLine {
    
    public MooTelnetClientHandler(Socket sock)  {   
	try {
	    out = new PrintWriter(sock.getOutputStream(), true);
	    in =   new BufferedReader(new InputStreamReader(sock.getInputStream()));
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }
}

