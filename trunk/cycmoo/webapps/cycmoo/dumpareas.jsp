<%@ page language="java" import=" jamud.*, jamud.object.*,  jamud.util.*, cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.LogicMoo"/>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content="VBScript">
<%!
static void showObject(PrintWriter out, MudObject room, int level ){
      out.flush();
      StringBuffer indent = new StringBuffer(level);
      for (int i = 0 ; i < level ; i++) indent.append("        "); 
	  out.println(indent);
      out.println(indent + "obj = "+ room+ "");
      out.println(indent + "name = \"" + room.getName()+ "\" ("+room.isTemplate()+"="+room.getTemplateId()+"@"+room.getLoadId()+")");
      out.println(indent + "short = \"" + room.getShortDescription()+ "\"");
      out.println(indent + "long = \"" + room.getDescription()+ "\"");
     // out.println(indent + "long = \"" + room.getDescription().substring(0,20)+ "\"");
      out.println(indent + "parent container object = " + room.getParentContainer() + "");
	  showContainer(out,room.childContainer(),level);
	  out.println(indent + "begin keywords");
	  Iterator it = room.getKeywords().iterator();
	  while (it.hasNext()) showKeyword(out,it.next(),level);
	  out.println(indent + ":end keywords");
	  out.println(indent + "begin flags");
	  it = room.flags().iterator();
	  while (it.hasNext()) showKeyword(out,it.next(),level);
	  out.println(indent + ":end flags");
	}

%>

<%!
static void showKeyword(PrintWriter out, Object fl, int level ){
      StringBuffer indent = new StringBuffer(level);
      for (int i = 0 ; i < level ; i++) indent.append("        "); 
	  out.println(indent + " " + fl);

	}

%>

<%!
static void showContainer(PrintWriter out, MudObjectContainer room, int level ){
      out.flush();
      StringBuffer indent = new StringBuffer(level);
      for (int i = 0 ; i < level ; i++) indent.append("        "); 
      out.println(indent + "child container = " + room+ "");
      out.println(indent + "container name= \"" + room.getName()+ "\"");
      out.println(indent + "container short = \"" + room.getShortDescription()+ "\"");
      out.println(indent + "long = \"" + room.getDescription()+ "\"");
     // out.println(indent + "container long = \"" + room.getDescription().substring(0,20) + "\"");
      try {
		  out.println(indent + "container parent object = \"" + room.parentObject() + "\"");
	  } catch (Exception e) {
	  }
	  int nl = level + 1;
	  
	  out.println(indent + "begin children:");
	  Iterator it = room.childObjects();
	  while (it.hasNext()) showObject(out,(MudObject)it.next(),nl);
	  out.println(indent + ":end children");
	  out.println(indent + "begin exits");
	  it = room.exits();
	  while (it.hasNext()) showExit(out,(Entrance)it.next(),nl);
	  out.println(indent + ":end exits");
	  out.println(indent + "begin keywords");
	  it = room.getKeywords().iterator();
	  while (it.hasNext()) showKeyword(out,it.next(),nl);
	  out.println(indent + ":end keywords");

	}

%>
<%!
static void showExit(PrintWriter out, Entrance pathway, int level ){
      out.flush();
      StringBuffer indent = new StringBuffer(level);
      for (int i = 0 ; i < level ; i++) indent.append("        "); 
      out.println(indent + "pathway = " + pathway + "");
      out.println(indent + "pathway name= \"" + pathway.getName()+ "\"");
      out.println(indent + "pathway short = \"" + pathway.getShortDescription()+ "\"");
     // out.println(indent + "container long = \"" + room.getDescription().substring(0,20) + "\"");
      try {
		  out.println(indent + "to = \"" + pathway.getDestination().parentObject().getName() + "\"");
	  } catch (Exception e) {
	  }
	}

%>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">
    <BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
    <h2>Select Area in Jamud</h2> 
    <pre>
<%
 out.flush();
  showContainer(response.getWriter(), theLogicMoo.getMudObjectRoot(),0);
 // showObject(response.getWriter(), theLogicMoo.cycJamudAccess.jamudTop  ,0);
%>
</pre></body></html>
