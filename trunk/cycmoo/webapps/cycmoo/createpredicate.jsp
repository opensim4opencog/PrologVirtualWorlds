<%@ page language="java" import=" cycmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*,  org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="cycmoo.api.LogicMoo"/>

<%

CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  


JspUtils.cycAccess = cycAccess;
JspUtils.sessionizeVariables(pageContext,cycAccess);

String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String predicate = JspUtils.getString(pageContext,"predicate","<predicate>");
String arity  = JspUtils.getString(pageContext,"arity","2");
String comment  = JspUtils.getString(pageContext,"comment",predicate + " created for..");

%>
<HTML>
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
<HEAD>
<META name=VI60_defaultClientScript content="JavaScript">
<TITLE>OpenCyc and LogicMOO Engineering - Main</TITLE>
<LINK REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
</HEAD>
<form id=form1 method="POST">
    <BODY link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
    <h2>Create a new Predicate in OpenCyc</h2> 
    &nbsp;predicate&nbsp;&nbsp;<input TYPE="TEXT" name="predicate" Value="<%=predicate%>"></input> 
    &nbsp;isa&nbsp;
    <select size="1" name="arity" id=arity onChange="change_arity(this);">
        <option value="1">UnaryPredicate</option>
        <option SELECTED value="2">BinaryPredicate</option>
        <option value="3">TernaryPredicate</option>
        <option value="4">QuaternaryPredicate</option>
        <option value="5">QuintaryPredicate</option>
        <option value="*"></option>
    </select>
    <br>
    <br>
&nbsp;category (isa)&nbsp;<select size="4" name="category" MULTIPLE>
<% 
out.flush();
//String inside = JspUtils.writeHttpValueOptionsSingle(pageContext,"inside","(#$genls ?SEL #$IndividualAgent)","Thing",true);

ArrayList category = JspUtils.writeHttpMultiSelect(pageContext,"category","(#$isa ?SEL #$PredicateCategory )"," ",true);
%>
</select><font SIZE="-2">(isa PredicateCategory)</font>
<P>
&nbsp;genlPreds&nbsp;<select size="4" name="genlpreds" MULTIPLE>
<% 
out.flush();
ArrayList genlpreds = JspUtils.writeHttpMultiSelect(pageContext,"genlpreds","(#$isa ?SEL #$Predicate)"," ",true);
%>
</select><font SIZE="-2">(isa Predicate)</font>
<p>
&nbsp;comment <textarea COLS="60" name="comment"><%=comment%></textarea>
<P>
<%
String[] arg = null;
String[] argFormat = null;
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
out.flush();

%><p>
&nbsp;in&nbsp;
<select size="1" name="microtheory">
<% 
out.flush();
String microtheory = JspUtils.writeHttpValueOptionsSingle(pageContext,"microtheory","(#$isa ?SEL #$Microtheory )","BaseKB",true);
%></select>&nbsp;<input type="submit" value="update selections" name="update"><p>

    
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

    Creating arity <b><%=arityN%></b> Predicate: "<font COLOR="GREEN"><%=predicate%></font>
    "
    <p>



<%
    out.flush(); 
    String[] isagenl = null;

   try {

		arg= new String[arityN+1];
		isagenl = new String[arityN+1];
	
		for (int i=1 ; i <= arityN ; i++ ) {
				arg[i] = request.getParameter("arg" + i);
				isagenl[i] = request.getParameter("isagenl" + i);
		}
   } catch (Exception e) {
	   out.println(""+e);
   }
	out.flush(); 
	try {
		cycAccess.createPredicate(microtheory,user,predicate,genlpreds,arity,comment,category,arg,isagenl);
	} catch (Exception e) {
        out.println(""+e);
	    //e.printStrackTrace(new Writer(out));
	}
}               out.flush(); 
%>
    </body>
</html>

