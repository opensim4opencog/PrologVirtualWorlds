<%@ include	file="jspheader.txt" %>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content=VBScript>
<TITLE>LogicMOO Engineering - Main</TITLE>

<SCRIPT ID=clientEventHandlersVBS LANGUAGE=vbscript>
<!--
Sub selectLogicMoo
   createarea.src="foo"
End sub

Sub selectOpenCyc
   createarea.src="http://192.168.0.20:88/cgi-bin/cyccgi/cg?cb-start"
End sub

-->
</SCRIPT>
</HEAD>
<BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR=lightblue RIGHTMARGIN=0 LEFTMARGIN=0 topMARGIN=0 bottemmargin=0>
&nbsp;&nbsp;<a href="http://192.168.0.20:88/cgi-bin/cyccgi/cg?cb-start" target="createarea">OpenCyc</a> 
&nbsp;&nbsp;<a href="create.jsp" target="createarea">Create</a> 
&nbsp;&nbsp;<a href="askwindow.jsp" target="createarea">CycL/KIF</a> 
&nbsp;&nbsp;<a href="askwindow.jsp" target="createarea">Options</a> 
&nbsp;&nbsp;<a href="askwindow.jsp" target="createarea">Files</a> 
<iframe name="createarea" id="createarea" src="http://192.168.0.20:88/cgi-bin/cyccgi/cg?cb-start" scrolling=yes width="100%" HEIGHT="95%">
</body>
</html>

