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
<TITLE>OpenCyc and LogicMOO Engineering -  Microtheory</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">

<FORM name=mtUploader ID=mtUploader action="uploadkif.jsp" method="POST" onSubmit="return form_Submit()" enctype="multipart/form-data" >
<h2>Microtheory from location</h2>

<label for=clearmt1><INPUT type=radio name=clearmt id=clearmt1 value="true"  checked>Create</label>&nbsp;
<label for=clearmt3><INPUT type=radio name=clearmt id=clearmt3 value="true" >Replace </label>
<label for=clearmt2><INPUT type=radio name=clearmt id=clearmt2 value="false" >Add to Microtheory</label>&nbsp;
<P>
Name <INPUT id=newname name=newname value="<NameOfMt>" style="HEIGHT: 22px; WIDTH: 124px"> 
isa <select size="2" name="isa" MULTIPLE>
<% 
out.flush();

String isa = JspUtils.writeHttpValueOptionsSingle(pageContext,"isa","(#$isa ?SEL #$MicrotheoryType)","BaseKB",true);
%>
</select><p><p>    
genlMt <select size="4" name="genlmt" MULTIPLE>
<% 
out.flush();
String genlmt = JspUtils.writeHttpValueOptionsSingle(pageContext,"genlmt","(#$isa ?SEL #$Microtheory )","BaseKB",true);
%>
</select>
<p><BR>
Format&nbsp;
<label FOR=interp1 ><input type="radio" name="interp" id="interp1" value="cycl" checked>CycL</LABEL>
<label FOR=interp2 ><input type="radio" name="interp"  id="interp2" value="kif" >KIF</LABEL>
<label FOR=interp0 ><input type="radio" name="interp"  id="interp0" value="nofile" >No file</LABEL>
<label FOR=interp3 ><input type="radio" name="interp"  id="interp3" value="protege" DISABLED>Protege</LABEL>
<label FOR=interp4 ><input type="radio" name="interp"  id="interp4" value="cycml" DISABLED>CycML</label>
<label FOR=interp5 ><input type="radio" name="interp"  id="interp5" value="daml" DISABLED>DAML</LABEL>
<label FOR=interp6 ><input type="radio" name="interp"  id="interp6" value="english" DISABLED>English</LABEL>
<label FOR=interp7 ><input type="radio" name="interp"  id="interp7" value="subl">SubL</LABEL>
<label FOR=interp8 ><input type="radio" name="interp"  id="interp8" value="sql" DISABLED>SQLQuery</LABEL>
<P>
File or URI <INPUT type=file name=filebuttonUp id=filebuttonUp>
<p>
<INPUT type=submit name=submitbuttonUp id=submitbuttonUp value="create" > 
<label for=tms>&nbsp;&nbsp;<INPUT type=checkbox name=tms id=tms checked>TMS</label>

<BR>
</FORM>
</BODY></HTML>

																					  
																					  

