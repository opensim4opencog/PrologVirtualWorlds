<%@ page language="java" import=" cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*,  org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.api.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  

JspUtils.sessionizeVariables(pageContext);
JspUtils.cycAccess = cycAccess;

String action  = JspUtils.getString(pageContext,"action","<new action>");
String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String comment  = JspUtils.getString(pageContext,"comment",action + " created for..");

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">                                    
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
<h2>Create a new Agent Action in Jamud (SimpleActionPredicate)</h2>
&nbsp;action&nbsp;&nbsp;<input TYPE="TEXT" name=action Value="<%=action%>"></input>
&nbsp;for who&nbsp;
<select size="4" name="forwho"  MULTIPLE>
<% 
out.flush();
String forwho = JspUtils.writeHttpValueOptionsSingle(pageContext,"forwho","(#$isa ?SEL #$PersonTypeByActivity)","Thing",true);
%></select><br>
<br>&nbsp;will be a specialization of&nbsp;
<select size="4" name="genlpreds" MULTIPLE>
<% 
out.flush();
String genlpreds = JspUtils.writeHttpValueOptionsSingle(pageContext,"genlpreds","(#$isa ?SEL #$SimpleActionPredicate)"," ",true);
%></select><font SIZE=-2> (forwho Action)</font><br>
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
   // 	out.flush(); cycAccess.createAction(microtheory,user,action,forwho,comment,genlpreds);
	}
%>
</body>
</html>

