<%@ page language="java" import=" cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  
JspUtils.sessionizeVariables(pageContext);

boolean botrunning = (theLogicMoo.cycIRCBot!=null);

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">                                    
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<h2>CycLBot Administration Page</h2>

<%
if (botrunning) {
}   else {

}
%>

CycLBot running <%=botrunning%>

</body>
</html>


