<%@ page language="java" import=" logicmoo.*, org.opencyc.constraintsolver.*,org.opencyc.cycobject.*, org.opencyc.creation.*, org.opencyc.kif.*, org.opencyc.jsp.*, org.opencyc.util.*,org.opencyc.api.*,org.opencyc.inferencesupport.*,org.opencyc.webserver.*, java.util.*,com.oreilly.servlet.*,com.oreilly.servlet.multipart.*,java.io.*, javax.servlet.jsp.*, javax.servlet.http.*" %>
<jsp:useBean id="theLogicMoo" scope="application" class="logicmoo.LogicMoo"/>

<%!
static void writeAnswers(JspWriter out,ArrayList al) {
try {	  
    Iterator answers = al.iterator();
	int ansi = 1;
    while (answers.hasNext()) {
		CycList answerset = (CycList)answers.next();
		out.println(answerset);
		while (answers.hasNext()) {
			answerset = (CycList)answers.next();
			out.println(answerset);
       	
		}
	}
} catch (IOException e){
}
}
%>
<%


CycAssertionsFactory cycAccess = theLogicMoo.getCycAssertionsFactory();  

JspUtils.cycAccess = cycAccess;
JspUtils.sessionizeVariables(pageContext);

String sentence = JspUtils.getString(pageContext,"sentence","(isa ?What Dog)");
String user = JspUtils.getString(pageContext,"user","CycAdministrator");
String interp  = JspUtils.getString(pageContext,"interp","*");
String wff  = JspUtils.getString(pageContext,"wff","checked");

CycList cycsentence = null;

if (interp!=null) {
    if (interp.equals("cycl") || interp.equals("kif") || interp.equals("subl")) {
         CycListKifParser kp = new CycListKifParser(cycAccess);
		 cycsentence  = kp.read(sentence);
		 sentence = cycsentence.stringApiValue();

	} else if (interp.equals("daml")) {
	} else if (interp.equals("english")) {
	} else if (interp.equals("sql")) {
	}
}

%>
    
    <html>
    <head>
    <meta name=VI60_defaultClientScript content="VBScript">
    <title>OpenCyc and LogicMOO Engineering - Ask/Assert</title>
    <link REL="stylesheet" TYPE="text/css" HREF="http://msdn.microsoft.com/msdn-online/shared/css/ie4.css" />
    </head>
    <body link="#0000aa" vlink="#0000aa" alink="#0000ff" BGCOLOR="white">
    <form id="theForm" name="theForm" METHOD="POST">
        <b>Ask / Assert in Microtheory&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
        &nbsp;</b>
        <select size="1" name="microtheory">
<% 
out.flush();
String microtheory = JspUtils.writeHttpValueOptionsSingle(pageContext,"microtheory","(#$isa ?SEL #$Microtheory)","EverythingPSC",true);
microtheory = JspUtils.getString(pageContext,"microtheory","EverythingPSC");
CycFort cycmt = cycAccess.makeCycConstant(microtheory);

cycmt = cycAccess.makeCycConstant(microtheory);

%>
        
        </select>
        <br>
        <textarea name="sentence" rows="5" cols="78"><%=sentence%></textarea>&nbsp;<br>
            <label for=interp1><input id=interp1 type="radio" CHECKED value="cycl" name="interp">CycL</label> 
            <label for=interp2><input id=interp2 type="radio" value="kif" name="interp">KIF</label> 
            <label for=interp5><input id=interp5 disabled type="radio" value="daml" name="interp">DAML</label> 
            <label for=interp6><input id=interp6 disabled type="radio" value="english" name="interp">English</label> 
            <label for=interp7><input id=interp7 type="radio" value="subl" name="interp">SubL</label>
        <label for=interp8><input id=interp8 disabled type="radio" value="sql" name="interp">SQLQuery</label> 
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;
        <table borderColor="#000000" cellSpacing="0" cellPadding="0" width="642" bgColor="#dddddd" border="0">
            <tbody>
            <tr>
                <td width="239"><b>Direction</b> 
                     <input type="radio" CHECKED value="backward" name="direction">Backward 
                     <input type="radio" value="forward" name="direction">Forward
                </td>
                <td width="251"> <b>Strength</b> <input type="radio" CHECKED value="default" name="strength">Default
                    <input type="radio" value="monotonic" name="strength">Monotonic</td>
                <td width="146"><input type="submit" value=" Assert " name="submit">&nbsp;&nbsp;
                  <input disabled type="submit" value=" retract " name="submit1"></td>
            </tr>
            </tbody>
        </table>  
<%
// Show results
String submit  = JspUtils.getString(pageContext,"submit",null);

try {
	if (submit!=null) {
		submit = submit.trim();
		if (submit.equals("Ask")) {
			%><pre><font color="green">Asking <%=cycsentence%> in <%=cycmt%></font>
Result:
<%
  writeAnswers(out,cycAccess.converseList("(CYC-query '"+cycsentence.stringApiValue() + " " + cycmt.stringApiValue()+")"));%></pre><%

		} else if (submit.equals("Assert")) {

		try{
			%><pre><font color="green">Asserting <%=cycsentence%> in <%=cycmt%></font></pre><%
				cycAccess.assertWithTranscript(cycsentence,cycmt);
		} catch (Exception ee) {
			%><pre><font color="red"><%=ee%></font></pre><%
		}

		} else if (submit.equals("Preform")) {
			%><pre><font color="green">Preforming: <%=sentence%></font>
Result: <%=(""+cycAccess.converseObject(sentence))%>
            </font></pre><%
		}
	}
} catch (Exception e) {
    %><pre><font color="red"><%=e%></font></pre><%
}






%>        
        <table cellSpacing="0" cellPadding="0" width="645" border="0">
            <tbody>
            <tr bgColor="#dddddd">
                <td width="1"></td>
                <td width="382"><strong>Inference Parameters</strong></td>
                <td width="104">   </td>
                <td width="151"><input type="submit" value="   Ask    " name="submit">&nbsp;&nbsp;
                  <input type="submit" value="Preform" name="submit2"></td>
            </tr>
            </tbody>
        </table>
        <table cellSpacing="0" cellPadding="0" width="642" border="0">
            <tbody>
            <tr>
                <td width="135"><strong>Number Cutoff</strong></td>
                <td width="131"><strong>Time Cutoff</strong></td>
                <td width="184"><strong>Backchaining Control</strong></td>
                <td width="184"><strong>Depth Cutoff</strong></td>
            </tr>
            <tr>
                <td noWrap width="135"><input onclick="with (this.form.elements['number']){focus();}" type="radio" value="T" name=cutoff-number><input size="4" name="number">answers  </td>
                <td noWrap width="131"><input onclick="with (this.form.elements['time']){focus();}" type="radio" CHECKED value="T" name=cutoff-time><input size="4" value="30" name="time">seconds   </td>
                <td noWrap width="184"><input onclick="with (this.form.elements['backchain']){value = '';}" type="radio" CHECKED value="NIL" name=cutoff-backchain>No Backchaining   </td>
                <td noWrap width="184"><input onclick="with (this.form.elements['depth']){focus();}" type="radio" CHECKED value="T" name=cutoff-depth><input size="4" value="20" name="depth">proof steps   </td>
            </tr>
            <tr>
                <td noWrap width="135"><input onclick="with (this.form.elements['number']){value = '';}" type="radio" CHECKED value="NIL" name=cutoff-number>Get Everything   </td>
                <td noWrap width="131"><input onclick="with (this.form.elements['time']){value = '';}" type="radio" value="NIL" name=cutoff-time>Run Forever   </td>
                <td noWrap width="184"><input onclick="with (this.form.elements['backchain']){focus();}" type="radio" value="1" name=cutoff-backchain><input size="4" name="backchain">backchain steps   </td>
                <td noWrap width="184"><input onclick="with (this.form.elements['depth']){value = '';}" type="radio" value="NIL" name=cutoff-depth>Infinite Depth  </td>
            </tr>
            <tr>
                <td colSpan="2" width="268"></td>
                <td noWrap colSpan="2" width="370"><input onclick="with (this.form.elements['backchain']){value = '';}" type="radio" value="T" name=cutoff-backchain>Backchain Forever</td>
            </tr>
            </tbody>
        </table>
        <table cellSpacing="0" cellPadding="0" width="642" border="0">
            <tbody>
            <tr>
                <td colSpan="3" width="640">
                    <table border="0" width="638">
                        <tbody>
                        <tr>
                            <td colSpan="3" width="630">
                                <hr>
                            </td>
                        </tr>
                        <tr>
                            <td vAlign="top" width="289"><b>Enable HL predicate backchaining</b><br>
                                <input type="radio" CHECKED value="0" name=radio-S#6975>No <input type="radio" value="1" name=radio-S#6975>Yes (expensive)</td>
                            <td width="16">     </td>
                            <td vAlign="top" width="313"><b>Cache backward query results</b><br>
                                <input type="radio" CHECKED value="0" name=radio-S#7095>No <input type="radio" value="1" name=radio-S#7095>Yes</td>
                        </tr>
                        <tr>
                            <td colSpan="3" width="630">
                                <hr>
                            </td>
                        </tr>
                        <tr>
                            <td vAlign="top" width="289"><b>Enable negation by failure</b><br>
                                <input type="radio" CHECKED value="0" name=radio-S#6977>No <input type="radio" value="1" name=radio-S#6977>Yes</td>
                            <td width="16">     </td>
                            <td vAlign="top" width="313"><b>Enable unbound predicate rule backchaining</b><br>
                                <input type="radio" CHECKED value="0" name=radio-S#6979>No <input type="radio" value="1" name=radio-S#6979>Yes (expensive)</td>
                        </tr>
                        <tr>
                            <td colSpan="3" width="630">
                                <hr>
                            </td>
                        </tr>
                        <tr>
                            <td vAlign="top" width="289"><b>Semantic pruning of results of backchaining</b><br>
                                <input type="radio" value="0" name=radio-S#13981>Yes <input type="radio" CHECKED value="1" name=radio-S#13981>No</td>
                            <td width="16">     </td>
                        </tr>
            </tbody>
                    </table>
                </td>
            </tr>
            </tbody>
        </table>
    </form>
<hr>
<!-- request attributes -->
<b><u>Debug Request Attributes</u>:</b>
<table width="90%" border="0">
<tr><th align="right"><u>Name</u></th><th>&nbsp;</th><th align="left"><u>Value</u></th></tr>
<%
    Enumeration enum = request.getAttributeNames();
    if (enum.hasMoreElements()) {
        while (enum.hasMoreElements()) {
            String name = (String)enum.nextElement();
%>
<tr><th align="right"><%= name %></th><td>-&gt;</td><td align="left"><%= request.getAttribute(name) %></td></tr>
<%    } %>
</table>
<%
  } 
%>

<p>

<!-- parameters -->
<%
    enum = request.getParameterNames();
    if (enum.hasMoreElements()) {
%>
<b><u>Parameters</u>:</b>
<table width="90%" border="0">
<tr><th align="right"><u>Name</u></th><th>&nbsp;</th><th align="left"><u>Value</u></th></tr>
<%
    while (enum.hasMoreElements()) {
        String name = (String)enum.nextElement();
%>
<tr>
<th align="right"><%= name %></th>
<td>-&gt;</td>
<td align="left"><%= request.getParameter(name) %></td>
</tr>
<%
    } // end while (enum.hasMoreElements())
%>
</table>
<%
    } //end if (enum.hasMoreElements()) for parameters
%>

<!-- application attributes -->
<%/*
    enum = application.getAttributeNames();
    if (enum.hasMoreElements()) {
%>
<b><u>Application Attributes</u>:</b>
<table width="90%" border="0">
<tr><th align="right"><u>Name</u></th><th>&nbsp;</th><th align="left"><u>Value</u></th></tr>
<%
    while (enum.hasMoreElements()) {
        String name = (String)enum.nextElement();
%>
<tr>
<th align="right"><%= name %></th>
<td>-&gt;</td>
<td align="left"><%= application.getAttribute(name) %></td>
</tr>
<%
    } // end while (enum.hasMoreElements())
%>
<tr>
<th align="right">Servlet Context</th>
<td>-&gt;</td>
<td align="left"><%= pageContext.getRequest().getRealPath("/") %></td>
</tr>
</table>
<%
    } //end if (enum.hasMoreElements()) for application attributes
*/%>

<p>
    </html>

