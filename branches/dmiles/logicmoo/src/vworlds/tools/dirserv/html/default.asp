<%
	'must resume next on error since ADSI will return error if property is empty.
	On Error Resume Next
	
	'Border is used to change table border globaly
	Border = 0
	'Set the machine name here when porting to different machine
	strServerName = "MYSERVERNAME"
	strOrg = "VWORLD"
	Server.ScriptTimeout = 90
	MaxRecord = 200

	'
	'Key for translating LDAP objects to meaningful names
	'
	Email = "rfc822Mailbox"
	FirstName = "givenName"
	LastName = "surName"
	'location
	'comment
	Hidden = "sFlags"                       ' 0=yes 1=no
	Country = "c"
	Audio = "ilsA32833566"          ' no=0 yes=1
	Video = "ilsA32964638"          ' no=0 yes=1
	InCall = "ilsA26214430"         ' yes=1 no=0
	UserType = "ilsA39321630"       ' personal=1 business=2 adult=4
	'ipAddress
	'securityToken
	'objectClass
	'Port
	'protocolMimeType
	'mimeType
	'smodop
	'guid
		
	'Retrieving the array from the Application Object
	aCountry = Application("aCountry")
	aBuckets = Application("aBuckets")
	iRecordCount = Application("iCount")

    Set oMembers = CreateObject("cadscontainer.cadscontainer.1")

	FrmDir = Request("FrmDir")
	If (FrmDir = "TRUE") Then
	
		multimedia_cookie       = Request("multimedia")                 
		incall_cookie           = Request("incall")     
		UserType_cookie         = Request("UserType")
	
		ExpDate = now + 350
		
		Response.Cookies("Search")("multimedia")= multimedia_cookie
		Response.Cookies("Search")("incall")    = incall_cookie
		Response.Cookies("Search")("UserType")  = UserType_cookie
		Response.Cookies("Search").Expires              = ExpDate
			
	Else
		multimedia_cookie       = Request.Cookies("Search")("multimedia")
		incall_cookie           = Request.Cookies("Search")("incall") 
		UserType_cookie         = Request.Cookies("Search")("UserType")
	End If
%>
<%
	' CREATE "strSearch" -- the query that will be executed
	strSearch = "(&(cn=*)(" & Hidden & "=1)"

	If multimedia_cookie <> "" Then
		Select case multimedia_cookie
			Case 0
				' leave it blank.  ALL is selected
			Case 1
				' audio is selected
				strSearch = strSearch & "(" & Audio & "=1)"
			Case 2
				' video is selected
				strSearch = strSearch & "(" & Video & "=1)"
			Case Else
				' if it is anything else, act like it is ALL
		End Select
	End If
	
	If incall_cookie <> "" Then
		strSearch = strSearch & "(" & InCall & "=" & incall_cookie & ")"
	End If
	
	If UserType_cookie <> "" Then
		strSearch = strSearch & "(" & UserType & "=" & UserType_cookie & ")"
	End If

	' close the last paren after adding any other search variables.
	strSearch = strSearch & ")"

	' set the Dn and call the query
	strDn = "LDAP://" & strServerName & "/o=" & strOrg & "/ou=Dynamic"
	'response.write strDN & "<BR>"
	'response.write strSearch & "<BR>"
    oMembers.GetInfo strDN, strSearch
%>

<HTML>
<HEAD>
<TITLE>Virtual World's World Directory</TITLE>
<SCRIPT LANGUAGE="VBScript">
<!-- comment out javascript for non-supporting browsers

sub DoWorldClick()
	msgbox "This will (eventually) connect ot the world", 0, "Test"
end sub

// -->                                                                                                                                                                                                                                                                                                                                                                                                             
</SCRIPT>
</HEAD>

<BODY BGCOLOR="#FFFFFF" VLINK="#999999" LINK="#0000FF" ALINK="#228B22">

<FORM ACTION="default.asp" METHOD="POST" NAME="Directory">
<INPUT TYPE="Hidden" NAME="FrmDir" VALUE="TRUE">        

<TABLE CELLSPACING="0" WIDTH="100%">
	<TR>
		<TD WIDTH="288">
			<IMG SRC="/vworldlogo.gif" ALT="Microsoft Virtual Worlds" HEIGHT="100" WIDTH="350">
		</TD>
		<TD ALIGN="CENTER">
			<B><FONT FACE="Arial, Helvetica" SIZE=5><%= strServerName %> Directory</FONT></B>
		</TD>
	</TR>
</TABLE>
<P>

<TABLE WIDTH="100%" BORDER="0" CELLSPACING=0 CELLPADDING=0 BORDER="<%= border %>>
	<TR>
		<TD VALIGN="middle">
			<B><FONT FACE="Arial, Helvetica" SIZE=2>
			Press the Refresh button to update this list:
			</FONT></B>
		</TD>
	</TR>
	<TR>
		<TD COLSPAN="4">
			<TABLE CELLPADDING="0" CELLSPACING="10" BORDER="<%= Border %>" width="100%">
				<TR>
					<TD COLSPAN="3">
						<CENTER>
						<INPUT TYPE=SUBMIT VALUE="Refresh">&nbsp;
						</CENTER>
						<HR>
					</TD>
				</TR>
			</TABLE>
		</TD>
	</TR>
</TABLE>
</FORM>
<%  If (FrmDir = "TRUE") Then %>
	<P>
	<FONT SIZE="2" FACE="Arial, Helvetica">
	<% If oMembers.Count <= MaxRecord then %>
		<B><I>Total of <%= oMembers.Count %> worlds found.</I></B>
	<% Else %>
		<B><I>
			Displaying first <%= MaxRecord %> of total <%= oMembers.Count %> users found.
			Refine your search to reduce number of users found.
		</I></B>
	<% End If %>
	</FONT>
	<P>

<TABLE BORDER=<%=border%>>
	<TR>
		<TD WIDTH=250 VALIGN="CENTER">
			<B><U>World Name</B></U>		
		</TD>
		<TD ALIGN=CENTER VALIGN="CENTER">
			<B><U># users</B></U>		
		</TD>
		<TD VALIGN="CENTER">
			<B><U>GUID</B></U>		
		</TD>
	</TR>

<%
    For Each oUser In oMembers

		ils_EmailName = ""
		ils_FirstName = ""
		ils_LastName = ""
		ils_Location = ""
		ils_Comment = ""
		ils_Hidden = ""
		ils_Country = ""
		ils_Audio = ""
		ils_Video = ""
		ils_InCall = ""
		ils_Catagory = ""

		ils_EmailName = oUser.GetEx(email)(0)
		ils_FirstName = oUser.GetEx(FirstName)(0)
		ils_LastName = oUser.GetEx(LastName)(0)
		ils_Location = oUser.GetEx("location")(0)
		ils_Comment = oUser.GetEx("comment")(0)
		ils_Hidden = oUser.GetEx(Hidden)(0)
		ils_Country = oUser.GetEx(Country)(0)
		ils_Audio = oUser.GetEx(Audio)(0)
		ils_Video = oUser.GetEx(Video)(0)
		ils_InCall = oUser.GetEx(InCall)(0)
		ils_Catagory = oUser.GetEx(Catagory)(0)

		If ils_FirstName <> "" Then
			fullname = ils_FirstName & " " & ils_LastName
		Else
			fullname = ils_EmailName
		End If
%>
	<TR>
		<TD WIDTH=250 VALIGN="CENTER">
			<FONT FACE="Arial, Helvetica" SIZE=2>
			<SPAN TITLE="Connect" OnClick=DoWorldClick worldUrl="<%= ils_location %>"><%= ils_firstname%></BUTTON>
		</TD>
		<TD ALIGN="center" VALIGN="CENTER">
			<xmp><%= ils_Comment %></xmp>
		</TD>
		<TD VALIGN="CENTER">
			<FONT FACE="Courier New" SIZE=2>
			<xmp><%= ils_EmailName %></xmp>
		</TD>
	</TR>
<%
		iMyRecordCount = iMyRecordCount + 1
		If iMyRecordCount = MaxRecord then Exit For
	Next
End If
%>
</TABLE>
<HR SIZE="1" NOSHADE>
</BODY>
</HTML>
