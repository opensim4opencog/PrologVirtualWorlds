package logicmoo;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
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

import javax.servlet.jsp.*;


public class CycWebMoo extends CycMoo {

    public static String getHttpValue(PageContext  pageContext, String variable, String def) {
	String pp = pageContext.getRequest().getParameter(variable);	      
	if ( pp!=null )	pageContext.getSession().setAttribute(variable,pp);
	pp = (String)pageContext.getSession().getAttribute(variable);
	if ( pp!=null ) return pp; else return def;
    }


}




