<%@ page language="java" import="cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.LogicMoo"/>

<pre> 
<%
LogicMooCycAccess cycAccess = theLogicMoo.getCycMooAccess();  

JspUtils.sessionizeVariables(pageContext);

String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String interp = JspUtils.getString(pageContext,"interp","kif");
String newname = JspUtils.getString(pageContext,"newname","");

boolean clearmt = JspUtils.getBoolean(pageContext,"clearmt",false);
//boolean clearmt = multi.getParameter("clearmt");
boolean tms = JspUtils.getBoolean(pageContext,"tms",false);

ArrayList isa = JspUtils.getArrayList(pageContext,"isa","") ;
ArrayList genlmt = JspUtils.getArrayList(pageContext,"genlmt","");
String rtype = ""+JspUtils.getHttpValue(pageContext,"requesttype",null);

MultipartRequest multi = (MultipartRequest)JspUtils.getHttpValue(pageContext,"request",null);
CycConstant cycmt = cycAccess.makeCycConstant(newname);

out.println("newname=" + newname + "  (" + cycmt + ")");
out.println("clearmt=" + clearmt);
out.println("user=" + user);
out.println("interp=" + interp);
out.println("tms=" + tms);
out.println("isa=" + isa);
out.println("genlmt=" + genlmt);
//out.println("rtype=" + rtype);
//out.println("multi=" + multi);
out.println("\n\n");
out.flush();

Iterator iterator = null;

if (clearmt) cycAccess.kill(cycmt);

String comment = "Microtheory created from web by " + user +" with TMS="+tms;

cycAccess.assertGaf(cycAccess.baseKB, cycAccess.isa, cycmt, cycAccess.microtheory);

iterator = genlmt.iterator();
while (iterator.hasNext()) {
    CycFort aGenlMt = (CycFort) cycAccess.makeCycConstant(""+iterator.next());
    cycAccess.assertGaf(cycAccess.baseKB, cycAccess.genlMt, cycmt, aGenlMt);
}

iterator = isa.iterator();
while (iterator.hasNext()) {
    CycFort aGenlMt = (CycFort) cycAccess.makeCycConstant(""+iterator.next());
    cycAccess.assertGaf(cycAccess.baseKB, cycAccess.isa, cycmt, aGenlMt);
}

String srcDir = pageContext.getServletContext().getRealPath("/");


try {
	Enumeration files = multi.getFileNames();
    while (files.hasMoreElements()) {
                String name = (String)files.nextElement();
                String filename = multi.getFilesystemName(name);
                String type = multi.getContentType(name);
                File f = null;
                if (request.getRemoteAddr() != "127.0.0.1") {
                    f = multi.getFile(name);
                } else {
                    f = new File(srcDir  + name);
                }
                out.println("adding file " + f + " to " + newname + "\n");
				comment = comment + "\n from file ("+interp+") "+ f.getName();

                if (f != null) cycAccess.constructMtFromFile(f,newname);
	}
} catch(Exception e) {
        e.printStackTrace(new PrintWriter( out));
}
cycAccess.assertComment(cycmt,comment, cycAccess.baseKB);
//out.println("comment="+comment);

%>





