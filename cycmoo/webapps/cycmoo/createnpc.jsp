<%@ page language="java" import=" cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*,  org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.api.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  

JspUtils.sessionizeVariables(pageContext);
JspUtils.cycAccess = cycAccess;

String agent  = JspUtils.getString(pageContext,"agent","<new agent>");
String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String comment  = JspUtils.getString(pageContext,"comment",agent + " created for..");

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">                                    
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<h2>Create a new NPC Agent in Jamud</h2>
<label for=hero1><INPUT type=radio name=hero id=hero1 value="true"  checked>Hero</label>&nbsp;
<label for=hero3><INPUT type=radio name=hero id=hero3 value="false" >Mob </label>
&nbsp;&nbsp;<input TYPE="TEXT" name=agent Value="<%=agent%>"></input>
&nbsp;basicly isa&nbsp;
<select size="1" name="inside">
<% 
out.flush();
String inside = JspUtils.writeHttpValueOptionsSingle(pageContext,"inside","(#$genls ?SEL #$IndividualAgent)","Thing",true);
%></select><font SIZE=-2> (genls IndividualAgent)</font><br>
<br>&nbsp;also isa&nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String type = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$genls ?SEL #$IndividualAgent)"," ",true);
%></select><font SIZE=-2> (genls IndividualAgent)</font><br>
<h2>Located</h2>
<br>&nbsp;at&nbsp;
<select size="1" name="connected">
<% 
out.flush();
String located = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$isa ?SEL #$GeographicalRegion)"," ",true);
%></select><font SIZE=-2> (genls IndividualAgent)</font>
&nbsp;prefers&nbsp;
<select size="4" name="inside">
<% 
out.flush();
String locatedgenls = JspUtils.writeHttpValueOptionsSingle(pageContext,"inside","(#$genls ?SEL #$GeographicalRegion)","Thing",true);
%></select><font SIZE=-2> (genls IndividualAgent)</font><br>
<h2>Scripting</h2>
<br>&nbsp;can plan / knows &nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String connected = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$isa ?SEL #$DefaultDisjointScriptType)"," ",true);
%></select><font SIZE=-2> (isa DefaultDisjointScriptType)</font><br>
<h2>Possessions</h2>
<br>&nbsp;has&nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String ofeels = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$isa ?SEL #$TemporalStuffType)"," ",true);
%></select><font SIZE=-2></font>
&nbsp;&nbsp;wants&nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String odesires = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$isa ?SEL #$TemporalStuffType)"," ",true);
%></select><font SIZE=-2></font><br>
<h2>Personally</h2>
<br>&nbsp;exibits&nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String feels = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$isa ?SEL #$FeelingType)"," ",true);
%></select><font SIZE=-2></font>
&nbsp;&nbsp;desires&nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String desires = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$isa ?SEL #$FeelingType)"," ",true);
%></select><font SIZE=-2></font><br>
<h2>In others</h2>
&nbsp;&nbsp;invokes&nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String invokesothers = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$and (#$not (#$genls ?SEL #$BiologicalEvent))(#$isa ?SEL #$FeelingType))"," ",true);
%></select><font SIZE=-2></font>
&nbsp;&nbsp;prefers&nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String desiresothers = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$and (#$not (#$genls ?SEL #$BiologicalEvent))(#$isa ?SEL #$FeelingType))"," ",true);
%></select><font SIZE=-2></font>
<h2>Physiology</h2>
<br>&nbsp;conditons &nbsp;
<select size="4" name="connected" MULTIPLE>
<% 
out.flush();
String conditons = JspUtils.writeHttpValueOptionsSingle(pageContext,"connected","(#$genls ?SEL #$BiologicalEvent)"," ",true);
%></select><font SIZE=-2> (isa DefaultDisjointScriptType)</font><br>


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
   // 	out.flush(); cycAccess.createRole(microtheory,user,agent,inside,comment,connected);
	}
%>
</body>
</html>


