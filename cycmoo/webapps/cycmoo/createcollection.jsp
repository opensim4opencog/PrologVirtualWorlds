<%@ page language="java" import=" cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  

JspUtils.sessionizeVariables(pageContext);
JspUtils.cycAccess = cycAccess;

String collection  = JspUtils.getString(pageContext,"collection","<new collection>");
String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String comment  = JspUtils.getString(pageContext,"comment",collection + " created for..");

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<h2>Create a new Collection in OpenCyc</h2>
&nbsp;collection&nbsp;&nbsp;<input TYPE="TEXT" name=collection Value="<%=collection%>"></input>
<P>
&nbsp;&nbsp;genls&nbsp;&nbsp;&nbsp;
<select size="4" name="genls" MULTISELECT>
<% 
out.flush();
String genls = JspUtils.writeHttpValueOptionsSingle(pageContext,"genls","(#$isa ?SEL #$Collection )","Thing",true);
%></select><br>
<br>&nbsp;category&nbsp;
<select size="4" name="category" MULTIPLE>
<% 
out.flush();
String category = JspUtils.writeHttpValueOptionsSingle(pageContext,"category","(#$isa ?SEL #$CollectionType )"," ",true);
%></select><font SIZE=-2> (isa CollectionType)</font><br>
<p>&nbsp;comment <textarea COLS="60" name=comment><%=comment%></textarea><P>    
&nbsp;in&nbsp;
<select size="1" name="microtheory">
<% 
out.flush();
String microtheory = JspUtils.writeHttpValueOptionsSingle(pageContext,"microtheory","(#$isa ?SEL #$Microtheory )","BaseKB",true);
%>
</select>&nbsp;<input type="submit" value="update selections" name="update">
<p>  
<input type="submit" value="create" name="create">
<% 
String submission = request.getParameter("create");
    if (submission != null ) {
		//out.flush(); cycAccess.createCollection(microtheory,user,collection,genls,comment,category);
	}
%>
</body>
</html>

