<%@ page language="java" import=" jamud.*, jamud.object.*, cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.LogicMoo"/>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content="VBScript">
<%
String filename = request.getParameter("filename"); 
String mtname = request.getParameter("microtheory"); 
File file = null;
KifFileLoader theMt = new KifFileLoader(theLogicMoo.cycMooAccess);
%>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">
    <BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<%
if (filename!=null && mtname!=null) {
	  file = theLogicMoo.ontologyFile(filename);
%>
    <h2>Loading CycLish File "<%=file.getAbsolutePath()%>" into "<%=mtname%>"</h2> 
    <pre>
<%
		out.flush();
	theMt.addFileEachLine(out,file,mtname);
} else {
    %>
    <h2>Pick A CycLish File to Load</h2>
    file:&nbsp;<input NAME="filename"/> <br>
    mt:&nbsp;&nbsp;<input NAME="microtheory"/> 
    <pre>
<%
	out.flush();
   // File dir = new File(theLogicMoo.);
}
%>
</pre></body></html>

