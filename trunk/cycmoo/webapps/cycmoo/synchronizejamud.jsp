<%@ page language="java" import=" jamud.*,jamud.object.*, cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.LogicMoo"/>
<html>
<body BGCOLOR="DARKGREEN">
<pre>
Jamud -> Cyc     Please wait..
</PRE>
</body>
</html>
<%
out.flush();


CycAccess cycAccess = theLogicMoo.getCycAccess();

	    try {
	     theLogicMoo.constructMtFromFile("JamudMt.kif","BaseKB");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	    try {
	     theLogicMoo.constructMtFromFile("JavaMt.kif","BaseKB");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }
	    try {
	     theLogicMoo.constructMtFromFile("LogicMooMt.kif","BaseKB");
	    } catch ( Exception e ) {
		e.printStackTrace();
	    }

		
theLogicMoo.assertJamud(theLogicMoo.jamudMt, theLogicMoo.getJamud());
        

%>                              
<pre>
Done   

Cyc -> Jamud      Please wait..
   
</pre>  
<%

out.flush();


//theLogicMoo.queryCycToJamudAll();

theLogicMoo.syncMudAll();

//cycAccess.makeCycJavaObject(cycAccess.jamudMt,theLogicMoo.cycJamudAccess.jamudMudObjectRoot);
//cycAccess.syncMud(cycAccess.jamudMt, theLogicMoo.cycJamudAccess.jamudInstance);
        
%>                              
<pre>
Done  
</PRE>
<h2>You may close this window</h2>         
         






