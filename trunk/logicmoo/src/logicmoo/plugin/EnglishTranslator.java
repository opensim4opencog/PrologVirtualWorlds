package logicmoo.plugin;

import logicmoo.*;
import logicmoo.api.*;
import logicmoo.obj.*;
import logicmoo.util.*;



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
import org.opencyc.creation.*;
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;

// Util
import ViolinStrings.*;

public class EnglishTranslator{

    private LogicMooCycAccess cyc = null;

    public EnglishTranslator(LogicMooCycAccess cycA) throws Exception {
	if ( cycA==null ) {
	    cyc = new LogicMooCycAccess();
	} else {
	    cyc = cycA;
	}
	cyc.getCycAccess();
	startConnection();
    }

    public void startConnection(){
	pushNLSystemToProlog();
    }

    public void pushNLSystemToProlog(){

    }

    public CycList toAssertion(String english, String userctx){
	return new CycList();
    }
}

