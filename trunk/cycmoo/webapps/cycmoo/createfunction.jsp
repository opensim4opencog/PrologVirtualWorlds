<%@ page language="java" import=" cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  
JspUtils.sessionizeVariables(pageContext);

JspUtils.cycAccess = cycAccess;

String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String function = JspUtils.getString(pageContext,"function","<function>");
String arity  = JspUtils.getString(pageContext,"arity","2");
String comment  = JspUtils.getString(pageContext,"comment",function + " created for..");
String resultisagenl  = JspUtils.getString(pageContext,"comment","Isa");

%>
<HTML>
<HEAD>
<META name=VI60_defaultClientScript content="JavaScript">
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
<SCRIPT ID=clientEventHandlersVBS LANGUAGE=JavaScript>
<!--

function change_arity(p) {
	for (var i =1 ; i<6; i++) {
		var ele = form1.all("argvis"+i);
		if (p.value >= i)
				ele.style.display = "";
			else
				ele.style.display = "NONE";
	}
}

-->
</SCRIPT>
</HEAD>
<form id=form1 method="POST">
    <BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
    <h2>Create a new Function in OpenCyc</h2> 
    &nbsp;function&nbsp;&nbsp;<input TYPE="TEXT" name="function" Value="<%=function%>"></input> 
    &nbsp;isa&nbsp;
    <select size="1" name="arity" onChange="change_arity(this);">
        <option value="1">UnaryFunction</option>
        <option SELECTED value="2">BinaryFunction</option>
        <option value="3">TernaryFunction</option>
        <option value="4">QuaternaryFunction</option>
        <option value="5">QuintaryFunction</option>
        <option value="*"></option>
    </select>
    <br>
    <br>
&nbsp;category&nbsp;<select size="4" name="category" MULITPLE>
<% 
out.flush();
String category = JspUtils.writeHttpValueOptionsSingle(pageContext,"category","(#$isa ?SEL #$FunctionCategory )"," ",true);
%>
</select><font SIZE="-2">(isa FunctionCategory)</font>
<P>
&nbsp;genlFuncts&nbsp;<select size="4" name="genlFuncts" MULITPLE>
<% 
out.flush();
String genlFuncts = JspUtils.writeHttpValueOptionsSingle(pageContext,"genlFuncts","(#$isa ?SEL #$FunctionalRelation)"," ",true);
%>
</select><font SIZE="-2">(isa Function)</font>
<p>
&nbsp;comment <textarea COLS="60" name="comment"><%=comment%></textarea>
<p>
<SELECT id=resultIsaGenls name=resultIsaGenls><OPTION value=resultIsa selected>resultIsa</OPTION><OPTION value=resultGenl>resultGenl</OPTION></SELECT>
<select size="1" name="resultisa">
<% 
out.flush();
String resultisa = JspUtils.writeHttpValueOptionsSingle(pageContext,"resultisa","(#$isa ?SEL #$Collection )","Thing",true);
%></select>
<P>
<%
String[] arg = null;
String[] argFormat = null;
String[] isagenl = null;
%>
<SELECT id=args0 name=isagenl0><OPTION value=arg0Isa selected>argsIsa</OPTION><OPTION value=arg0Genl>argsGenl</OPTION></SELECT>
&nbsp;
<select size="1" name="arg0">
<option> </option><% 
out.flush();
	try {
		arg[0] = JspUtils.writeHttpValueOptionsSingle(pageContext,"arg0","(#$isa ?SEL #$Collection)","Thing",true);
	} catch (Exception e) {
	}
out.flush();
%></select>

<%
out.flush();



for (int i=1; i<6; i++) {

  if (i<3) {
	%><div id=argvis<%=i%>><%
  } else {
	%><div id=argvis<%=i%> style="DISPLAY: none;"><%
  }
%>
<SELECT id=arg<%=i%>Isa name=isagenl<%=i%>><OPTION value=arg<%=i%>Isa selected>arg<%=i%>Isa</OPTION><OPTION value=arg<%=i%>Genl>arg<%=i%>Genl</OPTION></SELECT>
&nbsp;
<select size="1" name="arg<%=i%>">
<option> </option><% 
out.flush();
	try {
		arg[i] = JspUtils.writeHttpValueOptionsSingle(pageContext,"arg"+i,"(#$isa ?SEL #$Collection)","Thing",true);
	} catch (Exception e) {
	}
out.flush();
%></select>
<select size="1" name="argFormat<%=i%>">
<option> </option><% 
out.flush();
	try {
		argFormat[i] = JspUtils.writeHttpValueOptionsSingle(pageContext,"arg"+i,"(#$isa ?SEL #$Format)","SetTheFormat",true);
	} catch (Exception e) {
	}
out.flush();
%></select>
</div>
<%
}
%>

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
                
                out.flush(); 

        int arityN = 2;
        try {
                 arityN = Integer.parseInt(arity,0);
    } catch (Exception e) {     
    }
                %>
    <p>

    Creating arity <b><%=arityN%></b> Function: "<font COLOR="GREEN"><%=function%></font>
    "
    <p>



<%
    out.flush(); 

   try {

		arg= new String[arityN+1];
		isagenl = new String[arityN+1];
	
		for (int i=1 ; i <= arityN ; i++ ) {
				arg[i] = request.getParameter("arg" + i);
				isagenl[i] = request.getParameter("isagenl" + i);
		}
   } catch (Exception e) {
      // e.printStrackTrace(out);
   }
	out.flush(); 
	try {
		cycAccess.createFunction(microtheory,user,function,genlFuncts,arity,comment,category,resultisa,resultisagenl,arg,isagenl);
	} catch (Exception e) {
	  //  e.printStrackTrace(out);
	}
}               out.flush(); 
%>
    </body>
</html>


