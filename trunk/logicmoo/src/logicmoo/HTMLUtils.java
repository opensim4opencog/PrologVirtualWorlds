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
import ViolinStrings.*;

import javax.servlet.jsp.*;


public class HTMLUtils {


    public HTMLUtils() throws IOException {
    }

    public static CycMooAccess cycAccess;

    public static String getHttpValue(PageContext  pageContext, String variable, String def) {
	String pp = pageContext.getRequest().getParameter(variable);          
	if ( pp!=null )	pageContext.getSession().setAttribute(variable,pp);
	pp = (String)pageContext.getSession().getAttribute(variable);
	if ( pp!=null )	return pp;
	else return def;
    }
    public static String getHttpValueCheckbox(PageContext  pageContext, String variable, String def) {
	String pp = pageContext.getRequest().getParameter(variable);          
	if ( pp!=null )	pageContext.getSession().setAttribute(variable,pp);
	pp = (String)pageContext.getSession().getAttribute(variable);
	if ( pp!=null )	return pp;
	else return def;
    }
    public static String getHttpValueRadio(PageContext  pageContext, String variable, String def) {
	String pp = pageContext.getRequest().getParameter(variable);          
	if ( pp!=null )	pageContext.getSession().setAttribute(variable,pp);
	pp = (String)pageContext.getSession().getAttribute(variable);
	if ( pp!=null )	return pp;
	else return def;
    }

    public static String writeHttpValueRadio(PageContext  pageContext, String variable,String choices, String def) {
	PrintWriter  pw= getPrintWriter(pageContext);
	String pp = pageContext.getRequest().getParameter(variable);          
	if ( pp!=null )	pageContext.getSession().setAttribute(variable,pp);
	pp = (String)pageContext.getSession().getAttribute(variable);
	String checked = "";
	String[] choice = Strings.split(choices);
	for ( int ch = 0 ; ch < choice.length ; ch++ ) {
	    String val=choice[ch];
	    if ( val.startsWith(pp) ) checked = "CHECKED";
	    else checked = "";
	    pw.println("<label FOR="+variable+""+ch+" ><input type=radio name="+variable+"  id="+variable+""+ch+" value="+val+" "+checked+">"+val+"</label>");
	}
	if ( pp!=null )
	    return pp;
	else
	    return def;
    }

    public static boolean writeHttpValueCheck(PageContext  pageContext, String variable,String label, boolean def) {
	PrintWriter  pw= getPrintWriter(pageContext);

	boolean pp = false;
	String checked ="";

	if ( (pageContext.getSession().getAttribute(variable)==null) && (pageContext.getRequest().getParameter(variable)==null) ) {
	    pp = def;
	} else {
	    pp = (pageContext.getRequest().getParameter(variable)!=null);         
	}
	if ( pp )   {
	    pageContext.getSession().setAttribute(variable,"true");
	    checked = "CHECKED";
	} else {
	    pageContext.getSession().setAttribute(variable,"false");
	    checked = "";
	}
	pw.flush();
	pw.println("<label FOR="+variable+"><input type=checkbox name="+variable+"  id="+variable+" "+checked+">"+label+"</label>");
	pw.flush();
	return pp;
    }

    public static String writeHttpValueOptions(PageContext  pageContext, String variable,
					       String query, String defaultValue, boolean sort) {
	return writeHttpValueOptions(pageContext,variable,cycAccess.getArrayListQuerySEL(getMt(pageContext),query),defaultValue,sort);
    }


    public static String writeHttpValueOptions(PageContext  pageContext, String variable,
					     ArrayList list, String defaultValue, boolean sort) {
	PrintWriter  pw= getPrintWriter(pageContext);
	if ( sort ) Collections.sort(list);
	String pp = pageContext.getRequest().getParameter(variable);          
	if ( pp!=null )	pageContext.getSession().setAttribute(variable,pp);
	pp = (String)pageContext.getSession().getAttribute(variable);
	if (pp==null) pp=defaultValue;
	
	Object s = null;
	pw.flush();
	Iterator result = list.iterator();
	while ( result.hasNext() ) {
	    s = result.next();
	    if ( s instanceof CycConstant || s instanceof String ) {
		if ( s.toString().startsWith(pp) && !pp.startsWith(" ") )
		    pw.print("<option SELECTED>" + s + "</option>");
		else
		    pw.print("<option>" + s + "</option>");
	    }
	}
	    return pp;
	}


    
    public static PrintWriter getPrintWriter(PageContext  pageContext) {
	try {
	    return pageContext.getResponse().getWriter();
	} catch ( IOException ioe ) {
	    return new PrintWriter(System.out); 
	}
    }

    public static String getMt(PageContext pageContext){
	String pp = pageContext.getRequest().getParameter("microtheory");          
	if ( pp!=null )	pageContext.getSession().setAttribute("microtheory",pp);
	if (pp==null) return "BaseKB";
	else return pp;
    }

    public static void writeOptionsHtml(PageContext pageContext,String collectionname, String defaultValue) {
	writeOptionsHtml( pageContext, cycAccess.getArrayList(getMt(pageContext),collectionname),  defaultValue);
    }

    public static void writeOptionsHtml(PageContext pageContext,String collectionname1, String collectionname2, String defaultValue) {
	writeOptionsHtml( pageContext, cycAccess.intersectsArrayList(cycAccess.getArrayList(getMt(pageContext),collectionname1),cycAccess.getArrayList(getMt(pageContext),collectionname2)),  defaultValue);
    }

    public static void writeOptionsHtml(PageContext pageContext,String collectionname1, String collectionname2, String collectionname3, String defaultValue) {
	writeOptionsHtml( pageContext, cycAccess.intersectsArrayList(cycAccess.getArrayList(getMt(pageContext),collectionname1), cycAccess.intersectsArrayList(cycAccess.getArrayList(getMt(pageContext),collectionname2),cycAccess.getArrayList(getMt(pageContext),collectionname3))),  defaultValue);
    }                   


    public static void writeOptionsHtml(PageContext pageContext,ArrayList list, String defaultValue) {
	PrintWriter  pw=null;
	try {
	    pw = pageContext.getResponse().getWriter();
	    pw.flush();
	} catch ( IOException ioe ) {
	}
	writeOptionsHtml(pw, list,  defaultValue);
    }

    public static void writeOptionsHtml(Writer pw,ArrayList list, String defaultValue) {
	CycFort s = null;
	try {
	    pw.flush();
	    Collections.sort(list);
	    Iterator result = list.iterator();
	    while ( result.hasNext() ) {
		s = ((CycFort) result.next());
		if ( s instanceof CycConstant ) {
		    if ( s.toString().startsWith(defaultValue) && !defaultValue.startsWith(" ") )
			pw.write("<option SELECTED>" + s + "</option>");
		    else
			pw.write("<option>" + s + "</option>");
		}
	    }

	    pw.flush();
	} catch ( Exception e ) {
	}
    }



}


