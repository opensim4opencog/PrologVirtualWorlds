VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "AvatarMorphism"
Attribute VB_GlobalNameSpace = True
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Type MYDISPPARAMS
             rgvarg As Variant
             rgdispidNameArgs As Long 'dispid
             cArgs As Long
             cNamedArgs As Long
End Type

#Const DebugMode = 1

Type InstanceWithInfo
    theinterfaceinfo As tli.InterfaceInfo
    theObject As Object
    theName As String
End Type


Public WithEvents theclientocx As VWSYSTEMLib.VWClientOCX
Attribute theclientocx.VB_VarHelpID = -1
Public WithEvents theworld As VWSYSTEMLib.World
Attribute theworld.VB_VarHelpID = -1

Private mvar_stacked_events As String

Private theclient As Object

Dim theiavatarName

Event answerready(ByVal textBSTR As String)
Event KeyPress(KeyAscii As Integer)


Event onuserconnect(ByVal pthing As VWSYSTEMLib.IThing)
Event onuserdisconnect(ByVal pthing As VWSYSTEMLib.IThing)
Event oninvokemethod(ByVal pthing As VWSYSTEMLib.IThing, ByVal bstrName As String, ByVal varHintData As Variant)
Event onmethodchanged(ByVal pthing As VWSYSTEMLib.IThing, ByVal bstrName As String, ByVal lHint As Long, ByVal varHintData As Variant)
Event onmodulechanged(ByVal bstrName As String, ByVal lHint As Long, ByVal varHintData As Variant)
Event onpropertychanged(ByVal pthing As VWSYSTEMLib.IThing, ByVal bstrName As String, ByVal lHint As Long, ByVal varHintData As Variant)
Event onreport(ByVal textBSTR As String, ByVal ltype As Long)
Event ontrace(ByVal textBSTR As String)
Event onuievent(ByVal pthing As VWSYSTEMLib.IThing, ByVal bstreventName As String, ByVal vararg As Variant)

Property Get theIWorld() As World
    Set theIWorld = theworld
End Property

Property Set theIWorld(the)
    Set theworld = the
End Property


Property Get theIClientOcx() As VWClientOCX
    Set theIClientOcx = theclientocx
End Property

Property Set theIClientOcx(the)
    Set theclientocx = the
End Property

Public Sub event_send(textBSTR)
    'textBSTR = trim(replace(textBSTR, "sample", "")):textBSTR = replace(textBSTR, "sample", "")
    textBSTR = Trim(Replace(textBSTR, "  ", " "))
    textBSTR = Replace(textBSTR, "''", "'")
    textBSTR = Replace(textBSTR, Chr(34), " ")
    textBSTR = Replace(textBSTR, ",,", ",")
    textBSTR = Trim(Replace(textBSTR, ", ,", ","))
    textBSTR = Trim(Replace(textBSTR, ",]", "]"))
    textBSTR = Trim(Replace(textBSTR, "|", " "))
    While Right(textBSTR, 1) = ",": textBSTR = Trim(Left(textBSTR, Len(textBSTR) - 1)): Wend
    
    events_stack textBSTR

End Sub

Public Property Set the_avatar_object(ByVal vData As Object)
    Set theIAvatar = vData
End Property

Public Property Get the_avatar_object() As Object
    Set the_avatar_object = theIAvatar
End Property

Public Function connect_avatar(ByVal theWorldName As String, ByVal the_avatar_theObject As String)
On Error Resume Next
    theWorldName = theWorldName
    theiavatarName = the_avatar_theObject
    
    Set theclientocx = New VWSYSTEMLib.VWClientOCX
    Set theclient = theclientocx.VWClient
    Set theworld = theclient.Connect(theWorldName)
    Set theIAvatar = theworld.Connect(theiavatarName, theiavatarName)
    While Not (theclient.IsConnected): DoEvents: Wend
    
    java_vbscript_engine.AddObject "theiavatar", theIAvatar, True
    java_vbscript_engine.AddObject "user", theIAvatar.World.User, True
    java_vbscript_engine.AddObject "world", theIAvatar.World, True

    m_ready_flag = True
connect_avatar = error_string(Err)
End Function

Private Sub theclientocx_onreport(ByVal textBSTR As String, ByVal lval As Long)
If Ignored(textBSTR) Then Exit Sub
'   If sendout_test("onreport", report_pl(textBSTR, lval)) Then RaiseEvent onreport(textBSTR, lval)
End Sub

Private Sub theclientocx_ontrace(ByVal textBSTR As String)
'    If sendout_test("ontrace", theMorphism.object_array_pl_string(Trim(textBSTR))) Then RaiseEvent ontrace(theMorphism.object_array_pl_string(textBSTR))
End Sub

Private Sub theclientocx_onuievent(ByVal pthing As Object, ByVal bstreventName As String, ByVal vararg As Variant)
  'Stop
'    If sendout_test("onuievent", uievent_string(pthing, bstreventName, vararg)) Then RaiseEvent onuievent(pthing, bstreventName, vararg)
End Sub


Private Sub theworld_OnInvokeMethod(ByVal pthing As VWSYSTEMLib.IThing, ByVal bstrName As String, pdispparams As Variant)
   Static a
   a = a
End Sub

Private Sub theworld_onuserconnect(ByVal pthing As VWSYSTEMLib.IThing)
'    if sendout_test("onuserconnect", object_array_pl_string(pthing)) then raiseevent onuserconnect(pthing)
End Sub

Private Sub theworld_onuserdisconnect(ByVal pthing As VWSYSTEMLib.IThing)
'    if sendout_test("onuserdisconnect", object_array_pl_string(pthing)) then raiseevent onuserdisconnect(pthing)
End Sub

Private Sub theworld_onmethodchanged(ByVal pthing As VWSYSTEMLib.IThing, ByVal bstrName As String, ByVal lHint As Long, ByVal varHintData As Variant)
    If sendout_test("onmethodchanged", theMorphism.object_array_pl_string(pthing, bstrName, lHint, varHintData)) Then RaiseEvent onmethodchanged(pthing, bstrName, lHint, varHintData)
End Sub

Private Sub theworld_onmodulechanged(ByVal bstrName As String, ByVal lHint As Long, ByVal varHintData As Variant)
    If sendout_test("onmodulechanged ", theMorphism.object_array_pl_string(bstrName, lHint, varHintData)) Then RaiseEvent onmodulechanged(bstrName, lHint, varHintData)
End Sub

Private Sub theworld_onpropertychanged(ByVal pthing As VWSYSTEMLib.IThing, ByVal bstrName As String, ByVal lHint As Long, ByVal varHintData As Variant)
    If lHint = 999 Then
        If sendout_test("oninvokemethod", theMorphism.object_array_pl_string(pthing, bstrName, varHintData)) Then RaiseEvent oninvokemethod(pthing, bstrName, varHintData)
    Else
        If sendout_test("onpropertychanged", theMorphism.object_array_pl_string(pthing, bstrName, varHintData)) Then RaiseEvent onpropertychanged(pthing, bstrName, lHint, varHintData)
    End If
End Sub

Private Sub theworld_onreport(ByVal textBSTR As String, ByVal ltype As Long)
   If sendout_test("onreport", report_pl(textBSTR, ltype)) Then RaiseEvent onreport(textBSTR, ltype)
End Sub

Private Sub theworld_ontrace(ByVal textBSTR As String)
  '  If Ignored(textBSTR) Then Exit Sub
    If sendout_test("ontrace", theMorphism.object_array_pl_string(Trim(textBSTR))) Then RaiseEvent ontrace(theMorphism.object_array_pl_string(textBSTR))
End Sub

Private Sub theworld_onuievent(ByVal pthing As VWSYSTEMLib.IThing, ByVal bstreventName As String, ByVal vararg As Variant)
    If sendout_test("onuievent", uievent_string(pthing, bstreventName, vararg)) Then RaiseEvent onuievent(pthing, bstreventName, vararg)
End Sub

Public Function sendout_test(what, ByVal TextRight As String) As Boolean
    Dim lngct As Long, thebstr As String
    
    sendout_test = True
    thebstr = LCase(what) & "(" & TextRight & ")"
    'Debug.Print thebstr
    
    If Ignored(TextRight) Then sendout_test = False: Exit Function
    On Error Resume Next
    thePrologTracer.SendToOutputWindow thebstr
    event_send thebstr
    
End Function

Public Function approach_vector(ByVal vectorstring)
On Error Resume Next
Dim myvector As IVector, myvecresult As IVector, newvector As IVector, x, y, Z, angle, i%, vpl_split()
        Set myvector = theIAvatar.Property("position")
        Set myvecresult = theMorphism.pl_object(vectorstring)
        Set newvector = theIAvatar.Property("direction") ' is h[swi e facing this?
        theIAvatar.position = myvecresult
approach_vector = error_string(Err)
End Function

Public Function approach(ByVal theObject)
 On Error Resume Next
    theIAvatar.approach theWorldMorphism.pl_ithing(theObject)
    approach = error_string(Err)
End Function

Public Function say(ByVal theObject As String)
On Error Resume Next
    theIAvatar.say parse_pl_string_vbstr(theObject)
say = error_string(Err)
End Function
Public Function emote(ByVal theObject)
On Error Resume Next
    theIAvatar.SayGesture parse_pl_string_vbstr(theObject)
emote = error_string(Err)
End Function
Public Function moveinto(ByVal theObject)
On Error Resume Next
    theIAvatar.moveinto theWorldMorphism.pl_ithing(theObject)
moveinto = error_string(Err)
End Function
Public Function take(ByVal theObject)
On Error Resume Next
    theIAvatar.take theWorldMorphism.pl_ithing(theObject)
take = error_string(Err)
End Function
Public Function drop(ByVal theObject)
On Error Resume Next
    theIAvatar.drop theWorldMorphism.pl_ithing(theObject)
drop = error_string(Err)
End Function
Public Function report(ByVal theObject)
On Error Resume Next
    theIAvatar.report parse_pl_string_vbstr(theObject)
report = error_string(Err)
End Function
Public Function announce(ByVal theObject)
On Error Resume Next
    theIAvatar.announce parse_pl_string_vbstr(theObject)
announce = error_string(Err)
End Function
Public Function wield(ByVal theObject)
On Error Resume Next
    theIAvatar.wield theWorldMorphism.pl_ithing(theObject)
wield = error_string(Err)
End Function
Public Function unwield(ByVal theObject)
On Error Resume Next
    theIAvatar.unwield theWorldMorphism.pl_ithing(theObject)
unwield = error_string(Err)
End Function
Public Function wear(ByVal theObject)
On Error Resume Next
    theIAvatar.wear theWorldMorphism.pl_ithing(theObject)
wear = error_string(Err)
End Function
Public Function unwear(ByVal theObject)
On Error Resume Next
    theIAvatar.unwear theWorldMorphism.pl_ithing(theObject)
unwear = error_string(Err)
End Function
Public Function whisper(ByVal theObject, Optional strObject = "")
On Error Resume Next
    If strObject <> "" Then
        theIAvatar.whisper theObject, theWorldMorphism.pl_ithing(strObject)
    Else
        theIAvatar.whisper theObject
    End If
whisper = error_string(Err)
End Function
Public Function sleep_time(ByVal theObject)
    Sleep Val(theObject)
    sleep_time = "true"
End Function
Public Function lookat(ByVal theObject)
On Error Resume Next
 Dim myvector As IVector, myvecresult As IVector, targetvector As IVector, x, y, Z, angle, i%, distance As Long, thetarget As IThing
    Set myvector = theIAvatar.Property("position") ' is he facing this?
    Set thetarget = theWorldMorphism.pl_ithing(theObject)
    Set targetvector = thetarget.Property("position") ' is he facing this?
    Set myvecresult = theIAvatar.World.CreateVector(0, 0, 0)
    targetvector.Subtract myvector, myvecresult
    targetvector.x = Atn(myvecresult.Z / myvecresult.y)
    targetvector.y = Atn(myvecresult.Z / myvecresult.x)
    targetvector.Z = Atn(myvecresult.y / myvecresult.x)
    theIAvatar.Property("direction") = targetvector
lookat = error_string(Err)
End Function
Public Function lookat3(ByVal theObject)
On Error Resume Next
 Dim myvector As IVector, myvecresult As IVector, targetvector As IVector, x, y, Z, angle, i%, distance As Long, thetarget As IThing
    Set thetarget = theWorldMorphism.pl_ithing(theObject)
    Set myvecresult = theIAvatar.World.CreateVector(0, 0, 0)
    Set myvector = theIAvatar.Property("position") ' is he facing this?
    Set targetvector = thetarget.Property("position") ' is he facing this?
    angle = myvector.Rotation
    myvecresult.x = -(targetvector.x - myvector.x)
    myvecresult.y = (targetvector.y - myvector.y)
    myvecresult.Z = (targetvector.Z - myvector.Z)
    theIAvatar.Property("direction") = myvecresult
lookat3 = error_string(Err)
End Function
Public Function lookat2(ByVal theObject)
On Error Resume Next
 Dim thetarget As IThing
    Set thetarget = theWorldMorphism.pl_ithing(theObject)
    theIAvatar.Property("direction") = thetarget.Property("position") ' is he facing this?
lookat2 = error_string(Err)
End Function

Public Function walk(ByVal strDistance)
On Error Resume Next
Dim myvector As IVector, myvecresult As IVector, newvector As IVector, x, y, Z, angle, i%, distance As Long
  distance = Val(strDistance)
  For i% = Sgn(distance) To distance Step Sgn(distance)
        Set myvector = theIAvatar.Property("position")
        Set myvecresult = theIAvatar.World.CreateVector(0, 0, 0)
        Set newvector = theIAvatar.Property("direction") ' is he facing this?
        angle = myvector.Rotation
        'newvector.x = distance * cos(angle)
        'newvector.y = 0
        'newvector.Z = -distance * sin(angle)
        newvector.Add myvector, myvecresult
        theIAvatar.position = myvecresult
  Next i%
walk = error_string(Err)
End Function
Public Function normalize(ByVal strvector)
Dim sobjvector As IVector
On Error Resume Next
sobjvector = theMorphism.pl_object(strvector)
Dim myvector As IVector, x, y, Z, angle
      Set myvector = theIAvatar.Property("position")
      myvector.normalize (sobjvector)
      theIAvatar.Property("position") = myvector
normalize = theMorphism.object_array_pl_string(myvector)
If Err Then normalize = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function


Public Function self_Name()
    self_Name = LCase(theIAvatar.Name)
End Function

Public Function self_connected()
    self_connected = IIf(theIAvatar.World.theclient.IsConnected, "true", "fail")
End Function

Public Property Get proxy_avatar() As Object
   Set proxy_avatar = theIAvatar
End Property


Public Function get_room()
    get_room = theMorphism.object_array_pl_string(theIAvatar.Container)
End Function

Public Function get_avatar()
    get_avatar = theMorphism.object_array_pl_string(theIAvatar)
End Function

Private Sub Class_Initialize()
On Error Resume Next
   ' java_vbscript_engine.AddObject "AvatarMorphism", Me, True
End Sub
Public Function test_connect()
On Error Resume Next
test_connect = connect_avatar(theWorldName, theAvatarName)
test_connect = error_string(Err)
End Function

Public Sub events_stack(ByVal textBSTR)
mvar_stacked_events = mvar_stacked_events & textBSTR & vbCr
mvar_stacked_events = Trim(mvar_stacked_events)
RaiseEvent answerready(textBSTR)
End Sub

Public Function events_get() As String
Dim i As Long
mvar_stacked_events = Trim(mvar_stacked_events)
i = InStr(mvar_stacked_events & vbCr, vbCr)
events_get = Left(mvar_stacked_events, i)
'
mvar_stacked_events = Mid$(mvar_stacked_events, i + 1)
If events_get = "" Then events_get = "end_of_file"
End Function
Public Function events_get_all()
events_get_all = mvar_stacked_events
End Function
Public Function events_flag()
events_flag = IIf(mvar_stacked_events <> "", "true", "fail")
End Function
Public Function events_clear()
mvar_stacked_events = ""
events_clear = "true"
End Function


Public Function InvokeMethod(ByVal methodName, Optional ByVal Arg0 = "", Optional ByVal Arg1 = "")
Dim vb_or_java_script As String
On Error Resume Next
    vb_or_java_script = "theiavatar.invokemethod """ & methodName & """"
    If Arg0 <> "" Then vb_or_java_script = "theiavatar.invokemethod " & vbQt & methodName & vbQt & " , pl_object(" & vbQt & Arg0 & vbQt & ")"
    If Arg1 <> "" Then vb_or_java_script = "theiavatar.invokemethod " & vbQt & methodName & vbQt & " , pl_object(" & vbQt & Arg0 & vbQt & ") , pl_object(" & vbQt & Arg1 & vbQt & ")"
    java_vbscript_engine.ExecuteStatement vb_or_java_script
    InvokeMethod = error_string(Err)
End Function
Public Function invoke_method(ByVal theObject)
    invoke_method = invoke_eval(theObject)
End Function


Public Function invoke_eval(ByVal theObject)
On Error Resume Next
Dim token, Args, newobj, temparg(), i As Long
  Dim Index As Long, pl_string, objar()
  Dim theinnerobj As IThing, tempCollection As Collection, ipropertylist As ipropertylist
    pl_string = pl_trim(theObject)
    i = InStr(pl_string, "(")
    If i = 0 Then i = InStr(pl_string, " ")
    If i > 0 Then
        token = Left(pl_string, i - 1): Args = Mid(pl_string, i)
        Set ipropertylist = theMorphism.pl_object("ipropertylist" & Args)
        ReDim objar(ipropertylist.Count - 1)
        For i = 0 To ipropertylist.Count - 1
            If InStr("string,integer,long,short,byte", LCase(typeName(ipropertylist.Property(i)))) > 0 Then
                objar(i) = ipropertylist.Property(i)
            Else
                Set objar(i) = ipropertylist.Property(i)
            End If
        Next
        invoke_eval = theMorphism.object_array_pl_string(theIAvatar.InvokeMethod(token, objar))
    Else
        invoke_eval = theMorphism.object_array_pl_string(theIAvatar.InvokeMethod(pl_string))
    End If
        If Err Then invoke_eval = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function



Public Function invoke_script_exec(ByVal vb_or_java_script)
Dim tmpmethod As Method, a
On Error Resume Next
    vb_or_java_script = Trim(vb_or_java_script)
    If vb_or_java_script = "" Then invoke_script_exec = "fail": Exit Function
    Set tmpmethod = theIAvatar.World.CreateInlineMethod(2, "vbscript", "tmpmethod", vb_or_java_script)
    If theIAvatar.World.User.IsValidMethod("tmpmethod") Then theIAvatar.World.User.RemoveMethod ("tmpmethod")
    theIAvatar.World.User.AddMethod "tmpmethod", tmpmethod
    theIAvatar.World.User.InvokeMethod ("tmpmethod")
    invoke_script_exec = error_string(Err)
End Function
Public Function invoke_script_eval(ByVal vb_or_java_script)
Dim tmpmethod As Method
On Error Resume Next
    vb_or_java_script = Trim(vb_or_java_script)
    If vb_or_java_script = "" Then Exit Function
    Set tmpmethod = theIAvatar.World.CreateInlineMethod(2, "vbscript", "tmpmethod", vb_or_java_script)
    If theIAvatar.World.User.IsValidMethod("tmpmethod") Then theIAvatar.World.User.RemoveMethod ("tmpmethod")
    theIAvatar.World.User.AddMethod "tmpmethod", tmpmethod
    invoke_script_eval = theMorphism.object_array_pl_string(theIAvatar.World.User.InvokeMethod("tmpmethod"))
End Function






Public Function Ignored(ByVal textBSTR) As Boolean
    Ignored = True
   'If Parent.FilterEx = False Then Ignored = False: Exit Function
    If InStr(textBSTR, "Multimedia") Then Exit Function
    If InStr(textBSTR, "rror") Then Exit Function
    If InStr(textBSTR, "xcept") Then Exit Function
    Ignored = False
End Function




















Public Function tell_pl(ByVal objorigin, ByVal objfrom, ByVal objto, ByVal strmsg, inttCtype, intuitype) As String
'      Stop
      tell_pl = LCase(tell_enum_string(inttCtype)) & "(" & theMorphism.object_array_pl_string(objorigin, objfrom, objto, strmsg) & ")"
End Function

Public Function report_pl(ByVal textBSTR As String, ByVal ltype As Long)
    report_pl = ""
    If Ignored(textBSTR) Then Exit Function
    textBSTR = theMorphism.object_array_pl_string(textBSTR)
    If textBSTR <> "" Then textBSTR = list_seperator & textBSTR
    report_pl = int_reporttype(ltype) & textBSTR
End Function

Public Function uievent_string(ByVal pthing As Object, ByVal bstreventName As String, ByVal vararg As Variant)
        uievent_string = action_pl(pthing, bstreventName, vararg)
End Function

Public Function action_pl(ByVal objfrom As VWSYSTEMLib.IThing, ByVal bstreventName As String, ByVal vararg As Variant) As String
    Dim intuitype As Long, strmsg As String
    If bstreventName = "OnTell" Then
            intuitype = 0
            action_pl = tell_pl(objfrom, vararg.Property(0), vararg.Property(1), vararg.Property(2), vararg.Property(3), intuitype)
            Exit Function
    End If
    strmsg = theMorphism.object_array_pl_string(vararg)
    If strmsg <> "" Then strmsg = list_seperator & strmsg
    'action_pl = LCase(bstreventName) & "(" & theMorphism.object_array_pl_string(objfrom) & strmsg & ")"

End Function

