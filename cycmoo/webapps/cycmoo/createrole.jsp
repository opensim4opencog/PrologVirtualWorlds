<%@ page language="java" import=" logicmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="logicmoo.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  

JspUtils.sessionizeVariables(pageContext);
JspUtils.cycAccess = cycAccess;

String region  = JspUtils.getString(pageContext,"region","<new region>");
String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String comment  = JspUtils.getString(pageContext,"comment",region + " created for..");

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">                                    
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<h2>Create a new Agent Role in Jamud</h2>
&nbsp;region&nbsp;&nbsp;<input TYPE="TEXT" name=region Value="<%=region%>"></input>
&nbsp;isa&nbsp;
<select size="1" name="inside">
<% 
out.flush();
String inside = JspUtils.writeHttpValueOptionsSingle(pageContext,"inside","(#$genls ?SEL #$PersonTypeByActivity)","Thing",true);
%></select><br>
<br>&nbsp;connected&nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String connected = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$isa ?SEL #$PersonTypeByActivity)"," ",true);
%></select><font SIZE=-2> (inside Role)</font><br>
<p>&nbsp;comment <textarea COLS="60" name=comment><%=comment%></textarea><P>    
&nbsp;in&nbsp;
<select size="1" name="microtheory">
<% 
out.flush();
String microtheory = JspUtils.writeHttpValueOptionsSingle(pageContext,"microtheory","(#$isa ?SEL #$Microtheory)","BaseKB",true);
%>
</select>&nbsp;<input type="submit" value="update selections" name="update">
<p>  
<input type="submit" value="create" name="create">
<% 
String submission = request.getParameter("create");
    if (submission != null ) {
   // 	out.flush(); cycAccess.createRole(microtheory,user,region,inside,comment,connected);
	}
%>
</body>
</html>

