<%@ page language="java" import=" logicmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="logicmoo.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  

JspUtils.cycAccess = cycAccess;

JspUtils.sessionizeVariables(pageContext);

String object  = JspUtils.getString(pageContext,"object","<new object>");
String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String comment  = JspUtils.getString(pageContext,"comment",object + " created for..");

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<h2>Create a new Object in Jamud</h2>
&nbsp;object&nbsp;&nbsp;<input TYPE="TEXT" name=object Value="<%=object%>"></input>
&nbsp;isa&nbsp;
<select size="1" name="isa">
<% 
out.flush();
//String isa = JspUtils.writeHttpValueOptionsSingle(pageContext,"isa","(#$isa ?SEL #$ExistingStuffType)","Thing",true);
String isa = JspUtils.writeHttpValueOptionsSingle(pageContext,"isa","(#$genls ?SEL #$Individual)","Thing",true);
%></select><br>
<br>&nbsp;category&nbsp;
<select size="4" name="category" MULTIPLE>
<% 
out.flush();
ArrayList category = JspUtils.writeHttpMultiSelect(pageContext,"category","(#$isa ?SEL #$ObjectType)"," ",true);
%></select><font SIZE=-2> (isa ObjectType)</font><br>
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
   // 	out.flush(); cycAccess.createObject(microtheory,user,object,isa,comment,category);
	}
%>
</body>
</html>

