<%@ page language="java" import=" cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*,  org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.api.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  
JspUtils.sessionizeVariables(pageContext,cycAccess);
String user = JspUtils.getString(pageContext,"user","CycAdministrator");

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>OpenCyc and LogicMOO Engineering -  Taxonomic Pair Selector</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<h2>Taxonomic Pair Selector</h2>
<%


ArrayList allCollections = cycAccess.converseList("(cyc-query '(#$isa ?W #$Collection) #$EverythingPSC)");

java.util.TreeSet ts = new java.util.TreeSet();
out.println(allCollections);
javax.swing.text.html.HTMLDocument html = new javax.swing.text.html.HTMLDocument();
//html.setOuterHTML();



%>

</BODY></HTML>
