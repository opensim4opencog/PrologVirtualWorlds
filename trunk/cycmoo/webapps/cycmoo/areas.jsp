<%@ page language="java" import=" jamud.*, jamud.object.*,  jamud.util.*, cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*,  org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.api.LogicMoo"/>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content="VBScript">
<%!
static void showObject(PrintWriter out, MudObject room, int level ){
      out.flush();
      StringBuffer indent = new StringBuffer(level);
      for (int i = 0 ; i < level ; i++) indent.append("        "); 
	  out.println(indent);
     // out.println(indent + "obj = "+ room+ "");
      out.println(indent + "object " + showLink(room.getName())+ "  \"" + room.getShortDescription()+ "\"");
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
static void showLink(PrintWriter out, String room){
      out.flush();
	  out.print("<a HREF=''>");
      out.print(room);
	  out.print("</a>");
      out.flush();
}
%>
<%!
static String showLink(String room){
	  return ("<a HREF=''>" + room + "</a>");
}
%>
%>
<%!
static String showLinkCyc(String room){
	  return ("<a HREF=''>" + room + "</a>");
}
%>

<%!
static void showContainer(PrintWriter out, MudObjectContainer room, int level ){
	  if (room==null) return;

      out.flush();
      StringBuffer indent = new StringBuffer(level);
      for (int i = 0 ; i < level ; i++) indent.append("        "); 
//      out.println(indent + "child container = " + room+ "");
      out.println(indent + "container " + showLinkCyc(room.getName()) + " \"" + room.getShortDescription()+ "\"");
      out.println(indent + "long = \"" + room.getDescription()+ "\"");
     // out.println(indent + "container long = \"" + room.getDescription().substring(0,20) + "\"");
      try {
		  out.println(indent + " parent object = " + showLink("" + room.parentObject()) + "");
	  } catch (Exception e) {
	  }
	  int nl = level + 1;
	  
	  out.println(indent + "  begin children:");
	  Iterator it = room.childObjects();
	  while (it.hasNext()) showObject(out,(MudObject)it.next(),nl);
	  out.println(indent + "  end children");
	  it = room.exits();
	  while (it.hasNext()) showExit(out,(Entrance)it.next(),nl);
	  it = room.getKeywords().iterator();
	  while (it.hasNext()) showKeyword(out,it.next(),nl);

	}

%>
<%!
static void showExit(PrintWriter out, Entrance pathway, int level ){
      out.flush();
      StringBuffer indent = new StringBuffer(level);
      for (int i = 0 ; i < level ; i++) indent.append("        "); 
      //out.println(indent + "pathway = " + pathway + "");
      try {
      out.println(indent + "(ExitToFn \"" + pathway.getName()+ "\" " + showLink(pathway.getDestination().parentObject().getName()) + ") short = \"" + pathway.getShortDescription()+ "\" long = \"" + pathway.getDescription()+ "\"");
	  }  catch (Exception e) {
	  }
     // out.println(indent + "container long = \"" + room.getDescription().substring(0,20) + "\"");
	}

%>
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form method="POST">
    <BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
    <h2>Edit Area in Jamud</h2> 
    <pre>
<%
 out.flush();
  showContainer(response.getWriter(), theLogicMoo.getMudObjectRoot(),0);
 // showObject(response.getWriter(), theLogicMoo.cycJamudAccess.jamudTop  ,0);
%>
</pre></body></html>
