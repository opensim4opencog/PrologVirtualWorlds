<%@ page language="java" import=" jamud.*,jamud.object.*, logicmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="logicmoo.LogicMoo"/>

<pre>           
<%

CycAccess cycAccess = theLogicMoo.getCycAccess();

/*
File file =   theLogicMoo.ontologyFile("JavaMt.kif");
	out.println("constructMtFromFile " + file.getAbsolutePath() + " ");
	CycBulkAssertions theMt = new CycBulkAssertions(cycAccess);
	theMt.setDefaultMt("JavaMt");
	theMt.setDefaultCyclist("CycAdministrator");
	theMt.setDefaultProject("OpenCycProject");
	theMt.load(file);
	if ( theMt.testAssertionsHtml(out) )
	    theMt.commitAssertions(out);
         */
cycAccess.makeCycJavaObject(cycAccess.jamudMt,theLogicMoo.cycJamudAccess.jamudMudObjectRoot);
cycAccess.syncMud(cycAccess.jamudMt, theLogicMoo.cycJamudAccess.jamudInstance);


/*
MudObjectRoot mor = theLogicMoo.cycJamudAccess.jamudMudObjectRoot;
Iterator it = ((MudObject)mor.childObjects().next()).childContainer().childObjects();
while (it.hasNext()) {
    Object co= it.next();
    cycAccess.makeCycJavaObject(cycAccess.jamudMt,co);
    
    cycAccess.syncJavaObject(cycAccess.jamudMt,co,
                             (CycConstant)cycAccess.cycKnowsObject.get(co));

}
*/

%>                              

</pre>           





