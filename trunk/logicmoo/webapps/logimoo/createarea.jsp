<%@ page language="java" import=" logicmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="logicmoo.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  

JspUtils.cycAccess = cycAccess;
JspUtils.sessionizeVariables(pageContext);


String region  = JspUtils.getString(pageContext,"region","<new region>");
String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String description  = JspUtils.getString(pageContext,"description",region + " created for..");

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">                                    
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<h2>Create a new Area in Jamud</h2>
&nbsp;in&nbsp;
<select size="1" name="microtheory">
<% 
out.flush();
String microtheory = JspUtils.writeHttpValueOptionsSingle(pageContext,"microtheory","(#$isa ?SEL #$Microtheory)","JamudMt",true);
%>
</select>&nbsp;<input type="submit" value="update selections" name="update">
<p>  
&nbsp;region&nbsp;&nbsp;<input TYPE="TEXT" name=region Value="<%=region%>"></input>
&nbsp;nearestIsa&nbsp;
<select size="1" name="inside">
<% 
out.flush();
String nearestIsa = JspUtils.writeHttpValueOptionsSingle(pageContext,"nearestIsa","(#$genls ?SEL #$MudArea)","Thing",true);
%></select><br>
<br>&nbsp;isa&nbsp;
<select size="4"  width="520px" name="isa" MULTIPLE>
<% 
out.flush();
String isa = JspUtils.writeHttpValueOptionsSingle(pageContext,"isa","(#$genls ?SEL #$GeographicalRegion)"," ",true);
%></select><font SIZE=-2> (genls GeographicalRegion)</font><br>
<p>&nbsp;description <textarea COLS="60" name=description><%=description%></textarea><P>    
<input type="submit" value="create" name="create">
<% 
String submission = request.getParameter("create");
    if (submission != null ) {
   // 	out.flush(); cycAccess.createGeographicalRegion(microtheory,user,region,inside,description,connected);
	}
%>
<h2>If the room is successfully created the next page will alow you to edit the properties</h2>
</body>
</html>

