VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 1  'Persistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "WorldMorphism"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Public Function pl_ithing_pred(ByRef pl_ithing, ByVal pl_stringin) As Collection
  Dim temp As Object, tempCollection As Collection
    
    If IsObject(pl_stringin) Then Set pl_ithing = pl_stringin: Exit Function
    If Val(pl_stringin) > 0 Then Set pl_ithing = theIAvatar.World.ObjectByIDVal(pl_stringin): Exit Function
        
    get_all_ithings_pred tempCollection
    Set pl_ithing_pred = New Collection

    java_vbscript_engine.ExecuteStatement "regex.pattern = """ & pl_regexp(pl_stringin) & """"
    
    For Each temp In tempCollection
        If java_vbscript_engine.Eval("regex.test(""" & theMorphism.object_pl2(temp, 4) & """)") Then pl_ithing_pred.Add temp
      '  Stop
    Next
 
    Select Case pl_ithing_pred.Count
        Case 0
            Set pl_ithing = Nothing
        Case 1
            For Each pl_ithing In pl_ithing_pred
                Exit Function
            Next
        Case Else
            Set pl_ithing = pl_ithing_pred
    End Select

End Function

Public Sub get_all_ithings_pred(ByRef get_all_ithings)
Dim thisObject, tempCollection As New Collection, innerobj, innerinnerobj
    For Each thisObject In theIAvatar.Container.Contents
        tempCollection.Add thisObject
        For Each innerobj In thisObject.Contents
            tempCollection.Add innerobj
            For Each innerinnerobj In thisObject.Contents
                tempCollection.Add innerinnerobj
            Next
        Next
    Next
    Set get_all_ithings = tempCollection
End Sub










Public Function world_createinstance(ByVal theObject, ByVal theObjectexemplar)
On Error Resume Next
    world_createinstance = theMorphism.object_array_pl_string(theIAvatar.World.CreateInstance(theObject, theObjectexemplar))
    If Err Then world_createinstance = error_string(Err)
End Function
Public Function world_destroyMorphism(ByVal theObject)
On Error Resume Next
    theIAvatar.World.DestroyObject pl_ithing(theObject)
world_destroyMorphism = error_string(Err)
End Function


Public Function get_object_ivector(ByVal theObject)
Dim thisObject
    Set thisObject = pl_ithing(theObject)
    If LCase(typeName(thisObject)) = "nothing" Then
        get_object_ivector = "nothing"
    Else
        get_object_ivector = theMorphism.object_array_pl_string(thisObject.Property("position"))
    End If
    
End Function


Public Function get_object_states(ByVal theObject)
Dim thisObject
    Set thisObject = pl_ithing(theObject)
    If LCase(typeName(thisObject)) = "nothing" Then
        get_object_states = "nothing"
    Else
        get_object_states = LCase("[" & theMorphism.object_array_pl_string(thisObject) & ",[" & theMorphism.object_array_pl_string(thisObject.MorphismProperties) & "],[" & theMorphism.object_array_pl_string(thisObject.Exemplar.MorphismProperties) & "]]")
    End If
End Function

Public Function get_object_contents(ByVal theObject)
Dim thisObject
    Set thisObject = pl_ithing(theObject)
    If LCase(typeName(thisObject)) = "nothing" Then
        get_object_contents = "nothing"
    Else
        get_object_contents = theMorphism.object_array_pl_string(thisObject.Contents)
    End If
End Function
Public Function get_object_methods(ByVal theObject)
Dim thisObject
    Set thisObject = pl_ithing(theObject)
    If LCase(typeName(thisObject)) = "nothing" Then
        get_object_methods = "nothing"
    Else
        get_object_methods = LCase("[" & theMorphism.object_array_pl_string(thisObject) & ",[" & theMorphism.object_array_pl_string(thisObject.MorphismMethods) & "],[" & theMorphism.object_array_pl_string(thisObject.Exemplar.MorphismMethods) & "]]")
    End If
End Function
Public Function get_object_goals(ByVal theObject)
    get_object_goals = get_property_pl(theObject, "goals")
End Function

Public Function place_object_at_ivector(ByVal theObject, ByVal theObjecttarget)
Dim thisObject, startvect As IVector, endvect As IVector
    Set thisObject = pl_ithing(theObjecttarget)
    endvect = thisObject.Property("position")
    
    Set thisObject = pl_ithing(theObject)
    thisObject.Property("position") = endvect
    
    place_object_at_ivector = theMorphism.object_array_pl_string(thisObject.Property("position"))
End Function



Public Function set_property_pl(ByVal theObject, ByVal propertyBSTR, ByVal Value)
On Error Resume Next
Dim i, typeBSTR
    propertyBSTR = parse_pl_string_vbstr(propertyBSTR)
    Set theObject = theMorphism.pl_object(theObject)
    theObject.RemoveProperty propertyBSTR
    typeBSTR = "vb" & LCase(typeName(theMorphism.pl_object(Value)))
    i = Val(scripting_engine_eval(typeBSTR))
    If i = 0 And typeBSTR <> "vbempty" Then i = 9
    theObject.AddProperty propertyBSTR, theMorphism.pl_object(Value), , &HF000F, i, , typeName(theMorphism.pl_object(Value))
    
    theCOMMorphism.assign theObject.Property(propertyBSTR), theMorphism.pl_object(Value)
    
    theCOMMorphism.assign set_property_pl, get_property_pl(theObject, propertyBSTR)

If Err Then set_property_pl = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function

Public Function set_property_value(ByVal theObject, ByVal propertyBSTR, ByVal Value) As Variant
On Error Resume Next
Dim i, typeBSTR
    propertyBSTR = parse_pl_string_vbstr(propertyBSTR)
    Set theObject = theMorphism.pl_object(theObject)
    theObject.RemoveProperty propertyBSTR
    typeBSTR = "vb" & LCase(typeName(Value))
    i = Val(scripting_engine_eval(typeBSTR))
    If i = 0 And typeBSTR <> "vbempty" Then i = 9
    theObject.AddProperty propertyBSTR, Value, , &HF000F, i, , typeName(Value)
    
    theCOMMorphism.assign theObject.Property(propertyBSTR), Value
    
    theCOMMorphism.assign set_property_value, get_property_pl(theObject, propertyBSTR)

If Err Then set_property_value = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function


Public Function object_has_property(ByVal theObject, ByVal sProperty)
On Error Resume Next
Dim thisObject
    sProperty = parse_pl_string_vbstr(sProperty)
    object_has_property = "false"
    Set thisObject = pl_ithing(theObject)
    If thisObject.IsValidProperty(sProperty) Then object_has_property = "true"
If Err Then object_has_property = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function

Public Function object_near_object(ByVal theObject, ByVal theObject2)
On Error Resume Next
Dim thisObject, dist, thisvect As IVector, x, y, Z
    object_near_object = "false"
    Set thisObject = pl_ithing(theObject)
    Set thisvect = thisObject.Property("position")
    x = thisvect.x
    y = thisvect.y
    Z = thisvect.Z
    Set thisObject = pl_ithing(theObject2)
    Set thisvect = thisObject.Property("position")
    x = x - thisvect.x
    y = y - thisvect.y
    Z = Z - thisvect.Z
    object_near_object = Sqr(x * x + y * y + Z * Z)
If Err Then object_near_object = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function

Public Function vb_object_contains(ByVal theObject, ByVal theObject2)
On Error Resume Next
Dim thisObject, thatObj
    Set thisObject = pl_ithing(theObject)
    Set thatObj = pl_ithing(theObject2)
    If thatObj Is thisObject.FindWithin(thatObj.Name) Then
        vb_object_contains = "true"
     Else
        vb_object_contains = "false"
    End If
If Err Then vb_object_contains = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function

Public Function vb_object_has_method(ByVal theObject, ByVal smethod)
On Error Resume Next
Dim thisObject
    vb_object_has_method = "false"
    Set thisObject = pl_ithing(theObject)
    If thisObject.IsValidMethod(smethod) Then vb_object_has_method = "true"
If Err Then vb_object_has_method = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function

Public Function get_property_pl(ByVal theObject, ByVal propertyBSTR)
    get_property_pl = theMorphism.object_array_pl_string(get_property(theObject, propertyBSTR))
End Function

Public Function get_property_type(ByVal theObject, ByVal propertyBSTR)
    get_property_type = LCase(typeName(get_property(theObject, propertyBSTR)))
End Function

Public Function get_property(ByVal theObject, ByVal propertyBSTR)
On Error Resume Next
'Stop
get_property = ""
    Set theObject = pl_ithing(theObject)
    If LCase(typeName(theObject)) = "nothing" Then
        get_property = Nothing
    Else
        If theObject.IsValidProperty(propertyBSTR) Then
            theCOMMorphism.assign get_property, theObject.Property(propertyBSTR)
        Else
            theCOMMorphism.call_by_Name_pred get_property, theObject, propertyBSTR
        End If
    End If
If Err Then get_property = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function

Public Function get_object_pl(ByVal theObject)
On Error Resume Next
    get_object_pl = "nothing"
    Set theObject = pl_ithing(theObject)
    get_object_pl = theMorphism.object_array_pl_string(theObject)
    If Err And get_object_pl = "" Then get_object_pl = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function


Public Function pl_ithing(ByVal pl_string)
    pl_ithing_pred pl_ithing, pl_string
End Function



Public Function pl_object_id(ByVal theObject) As Long
    Set theObject = pl_ithing(theObject)
    If LCase(typeName(theObject)) <> "nothing" Then pl_object_id = theObject.ID
End Function


Public Function get_all_ithings() As Collection
    get_all_ithings_pred get_all_ithings

End Function

Public Function pl_pl(ByVal plobj)
    pl_pl = LCase(theMorphism.object_array_pl_string(theMorphism.pl_object(plobj)))
End Function






























Public Function debugger_send(ByVal textBSTR)
'parent.sendtooutputwindow (parse_pl_string_vbstr(textBSTR))
End Function
Public Function inputbox_pl_string(ByVal textBSTR)
inputbox_pl_string = parse_vbstr_pl_string(Input(parse_pl_string_vbstr(textBSTR), ""))
End Function
Public Function die()
        
End Function
Public Function pl_split(theString, substr)
    pl_split_pred pl_split, theString, substr
End Function

Public Function parse_vbstr_pl_string(ByVal theString) As String
    parse_vbstr_pl_string = parse_vbstr_pl_string(theString)
End Function
Public Function parse_pl_string_vbstr(ByVal pl_string) As String
    parse_pl_string_vbstr = parse_pl_string_vbstr(pl_string)
End Function

Public Function scripting_engine_eval(ByVal vb_or_java_script)
On Error Resume Next
Err.Clear
    scripting_engine_eval = java_vbscript_engine.Eval(vb_or_java_script)
   If Err Then Err.Clear: scripting_engine_eval = java_vbscript_engine.Eval("me." & vb_or_java_script)
'    scripting_engine_eval = object_array_pl_string(java_vbscript_engine.eval("user." & vb_or_java_script))
'    scripting_engine_eval = object_array_pl_string(java_vbscript_engine.eval("world." & vb_or_java_script))
   If Err Then scripting_engine_eval = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function

Public Function scripting_engine_exec(ByVal vb_or_java_script)
    scripting_engine_exec = True
     java_vbscript_engine.ExecuteStatement vb_or_java_script
End Function

Public Function scripting_engine_run(ByVal vb_or_java_script, ParamArray Args() As Variant)
    scripting_engine_run = java_vbscript_engine.Run(vb_or_java_script, Args)
End Function

Public Function scripting_engine_addobject(ByVal pl_string)
Dim objectName As String
    objectName = theCOMMorphism.object_Name(theMorphism.pl_object(pl_string))
    java_vbscript_engine.AddObject objectName, theMorphism.pl_object(pl_string), True
    scripting_engine_addobject = objectName
End Function

Public Function scripting_engine_createobject_pl(ByVal pl_string)
Dim objectName As String
    objectName = theCOMMorphism.object_Name(theMorphism.pl_object(pl_string))
    java_vbscript_engine.AddObject objectName, theMorphism.pl_object(pl_string), True
    scripting_engine_createobject_pl = objectName
End Function

Public Function scripting_engine_createobject(ByVal Name, ByVal thetypeName)
Dim objectName As String
    java_vbscript_engine.AddObject Name, CreateObject(thetypeName), True
    scripting_engine_createobject = Name
End Function

Public Function scripting_engine_addcode(ByVal vb_or_java_script)
     scripting_engine_addcode = "true"
     java_vbscript_engine.AddCode vb_or_java_script
End Function

Public Function common_gate_eval(ByVal vb_or_java_script)
    On Error Resume Next
     common_gate_eval = java_vbscript_engine.Eval("me." & vb_or_java_script)
     If Err Then Err.Clear: common_gate_eval = java_vbscript_engine.Eval(vb_or_java_script)
     If Err Then common_gate_eval = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function
Public Function common_gate_eval_pl(ByVal vb_or_java_script)
    common_gate_eval_pl = theMorphism.object_array_pl_string(common_gate_eval(vb_or_java_script))
End Function

Public Function get_unify(ByVal Object, ByVal propName)
On Error Resume Next
    Err.Clear
    get_unify = java_vbscript_engine.Eval("user.findwithin(""" & Object & """)." & propName)
    If Err Then Err.Clear: get_unify = theMorphism.object_array_pl_string(java_vbscript_engine.Eval("user.container.findwithin(""" & Object & """)." & propName))
    If Err Then Err.Clear: get_unify = theMorphism.object_array_pl_string(java_vbscript_engine("user.findwithin(""" & Object & """).property(""" & propName & """)"))
    If Err Then Err.Clear: get_unify = theMorphism.object_array_pl_string(java_vbscript_engine.Eval("user.container.findwithin(""" & Object & """).property(""" & propName & """)"))
    If Err Then get_unify = "vwError(" & parse_vbstr_pl_string(Err.Description) & ")"
End Function

Public Function get_unify_pl(ByVal Object, ByVal propName)
get_unify_pl = LCase(theMorphism.object_array_pl_string(get_unify(Object, propName)))
End Function

Public Function unify(ByVal Object, ByVal propName, ByVal testvalue)
Dim Value
Value = theMorphism.pl_object(get_unify(Object, propName))
unify = "false"
If testvalue = Value Then unify = "true"
End Function


Public Function pipe(ByVal p, ParamArray Anything() As Variant)
pipe = "[" & p & "," & Join(Anything, ",") & "]"
'if IsObject(anything) then set pipe = anything else pipe = anything
End Function





















Public Function bless_object(ByVal theObject)
Dim thisObject As Thing
    bless_object = "false"
    Set thisObject = pl_ithing(theObject)
    If LCase(typeName(thisObject)) <> "nothing" Then
        bless_object = "true"
        
        Select Case LCase(thisObject.Type)
            Case "avatar", "sampleavatar"
                If Not thisObject.IsValidProperty("goals") Then thisObject.AddProperty ("goals")
                If Not thisObject.IsValidProperty("plan") Then thisObject.AddProperty ("plan")
            
            Case "livingarea"
            Case "chair", "sofa"
            Case "tvset"
            Case "videogame"
            Case "telephone"
            
            Case "kitchenarea"
            Case "microwave"
            Case "refrigerator"
            Case "food"
            Case "water", "beer"
            Case "trashcan"
            
            Case "bedroomarea"
            Case "clothes"
            Case "bed"
            Case "closet"
            
            Case "officearea"
            Case "paperwork"
            Case "computer"
            Case "desk"
            
            Case "hygenearea"
            Case "shower"
            Case "toilet"
            Case "washer"
            Case "dryer"
            
            Case "trash"
            Case "wallet"
            Case "money"
            Case "floor"
            
            Case "backstagearea"
        End Select
        
      Else
      
    End If
    
End Function



