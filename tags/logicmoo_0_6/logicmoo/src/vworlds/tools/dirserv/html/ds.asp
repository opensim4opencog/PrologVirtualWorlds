<%
	Response.Expires=0
	'must resume next on error since ADSI will return error if property is empty.
	On Error Resume Next

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
	'EmailName
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

		ils_EmailName = oUser.GetEx("surName")(0)
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
		'the following line outputs data needed for the Directory dialog
		'it should be in the form of:
		'	world URL
		'	display text for World
		'	number of users connected to a world
		'	Web page URL
		'each item is separated by a comma and each record ends in a semi-colon
		'if this layout/form changes please update the dServiceContent.htm file.
		response.write ils_location & chr(9) & ils_firstname & chr(9) & ils_Comment & chr(9) & ils_EmailName & vbCrLf
		iMyRecordCount = iMyRecordCount + 1
		If iMyRecordCount = MaxRecord then Exit For
	Next
%>