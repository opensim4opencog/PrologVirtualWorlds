VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 1  'Persistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Morphism"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit


Public Sub import_object_to_instances(this)
Dim objtypeName, objloc
 objtypeName = LCase(typeName(this))
 Select Case objtypeName
    Case "empty", "nothing"
        Exit Sub
    Case "string"
        
    Case Else
        objloc = CStr(ObjPtr(this))
        If Not theKnownMorphs.IsValid(objloc) Then
            theKnownMorphs.Property(objloc) = this
            If Len(theSWIPrologShell.Execute("typeName(" & objtypeName & "),post(h)")) Then
                    theSWIPrologShell.Execute "assert(typeName(" & objtypeName & ")),post(h)"
                    'serialize the class
            End If
            theSWIPrologShell.Execute "assert(object_ptr(" & objtypeName & "," & objloc & "))."
            'serialize the Instance
        End If
 End Select
End Sub

Public Sub object_pl_pred(ByRef object_pl, ByRef this, Optional Cascade = 4, Optional Instance, Optional ByVal Cast As String = "", Optional ByRef Origin, Optional NamedArgs = "True")
'Exit Sub
Dim temparray() As String, objtype As String, thisObjectii, i As Long, proxyThis As Variant
Dim vt_temp As VariantTypeConstants, tarry(), tarry0(), memberscount, Instance_props, thememberName As String, thevalue, my_ref As String
Dim vtkind As String, tempmodelstr As String
Dim theTypeInfo As tli.typeInfo, theInterfaces, theMemberInfo
Dim tempModel As String
Dim tempDatum As a_Datum
Dim subObject As Object
Dim subObjectmodel As String, evalue
Dim eresultA(1000) As Variant, eint As Long, thevalue2(1000), eresult
Dim itemObject, Serialize
my_ref = theCOMMorphism.get_reference(this)

'If IsObject(This) Then If theKnownMorphs.IsValid(my_ref & CStr(Cascade)) Then object_pl = theKnownMorphs.Property(my_ref & CStr(Cascade)): Exit Sub
'If theKnownMorphs.IsValid(my_ref) Then object_pl = theKnownMorphs.Property(my_ref): Exit Sub
On Error Resume Next

'On Error Resume Next
'Debug.Print TypeName(This)
'    Stop
    If IsMissing(Instance) Then
        Instance = False
    Else
        Instance = True
    End If
    
    If Cascade < 1 Then
        object_pl = "(_)": Exit Sub
       ' Stop
        If Instance Then
            object_pl = "object(" & theCOMMorphism.get_class(this) & "," & theCOMMorphism.get_type(this) & "," & theCOMMorphism.get_reference(this) & ",_)": Exit Sub
        Else
            object_pl = "object(" & theCOMMorphism.get_class(this) & "," & theCOMMorphism.get_type(this) & ",_,_)": Exit Sub
        End If
    End If

    If IsArray(this) Then
        If UBound(this) = -1 Then
            object_pl = "array([])"
        Else
            ReDim temparray(UBound(this))
            For i = 0 To UBound(this)
                 theMorphism.object_pl temparray(i), this(i), Cascade, Instance
            Next i
            object_pl = "array([" & Join(temparray, ",") & "])"
        End If
        Exit Sub
    End If
    
    objtype = LCase(typeName(this))
    
    If Cast <> "" Then objtype = Cast
    
    If NamedArgs Then NamedArgs = (InStr(VectoredObjects, objtype) = 0)
    
Select Case objtype
    Case "string", "integer", "long", "short", "int", "double", "single", "number", "byte", "color", "currency", "date", "time"
        If object_data Then
            object_pl = "object([datum]," & vartype_enum_string(VarType(this)) & "," & theCOMMorphism.get_reference(this) & ",[value=" & parse_vbstr_pl_string(this) & "])"
        Else
            object_pl = LCase(vartype_enum_string(VarType(this)) & "(" & parse_vbstr_pl_string(this) & ")")
        
        End If
        object_pl = LCase(parse_vbstr_pl_string(this))
    Case "nothing", "null", "empty"
            If Instance Then
                object_pl = "unset([" & objtype & "])": Exit Sub
            Else
                object_pl = "unset([" & objtype & "])": Exit Sub
            End If
    Case "boolean", "bool"
        If object_data Then
            object_pl = "object(class([data])," & vartype_enum_string(VarType(this)) & "," & theCOMMorphism.get_reference(this) & ",[value=" & IIf(this, "true", "false") & "])"
        Else
            object_pl = "datum(" & vartype_enum_string(VarType(this)) & "," & IIf(this, "true", "false") & ")"
        End If
    'Case "iworld", "world"
     '       object_pl = object_opener & "$" & LCase(This.Name) & object_terminator
    'Case "ivwgeometrysprite", "vwspritedgeometry"
    '        object_pl = "geometrysprite([" & object_pl_string(This.AccessoryScene, This.CurrentGesture, This.Frame, This.GeometryFrame, This.GeometryName, This.ImageFileName, This.RotationAxis) & "])" '                           thisName = "geometrysprite([accessoryscene=" & object_pl(This.accessoryscene) & ",currentgesture=" & object_pl(This.currentgesture) & ", frame=" & object_pl(This.frame) & ",geometryframe=" & object_pl(This.geometryframe) & " ,geometryName= " & object_pl(This.geometryName) & " , imagefileName=" & object_pl(This.imagefileName) & ",rotationaxis=" & This.rotationaxis & "])"
    'Case "ivwgeometry", "vwgeometry"
    '        object_pl = "geometry([" & object_pl_string(This.Frame, This.GeometryFrame, This.GeometryName) & "])" '                            thisName = "geometry([ frame=" & object_pl(This.frame) & ",geometryframe=" & object_pl(This.geometryframe) & " ,geometryName= " & object_pl(This.geometryName) & "])"
    'Case "isound", "sound"
    '        object_pl = "sound([" & object_pl_string(This.Is3D, This.IsLooping, This.SoundURL) & "])" '                            thisName = "sound([ is3d=" & object_pl(This.is3d) & ",islooping=" & object_pl(This.islooping) & " ,soundurl= " & object_pl(This.soundurl) & "])"
    'Case "iboundary", "boundary"
    '        object_pl = "boundry([" & object_pl_string(This.HeightLower, This.HeightUpper, This.IsAutoSolidBoundary, This.IsClosed, This.IsInverse, This.IsPassable, This.IsProximity, This.VertexCount) & "])" '                            thisName = "sound([ is3d=" & object_pl(This.is3d) & ",islooping=" & object_pl(This.islooping) & " ,soundurl= " & object_pl(This.soundurl) & "])"
    'Case "imethod", "method"
    '        object_pl = "method([" & object_pl_string(This.methodName, This.IsInline, This.FLAGS, This.Module) & "])"
    'Case "imodule", "module"
    '
    '        object_pl = "module([" & object_pl_string(This.methodName, This.IsInline, This.FLAGS, This.Module) & "])"
    'Case "iscriptmodule", "scriptmodule"
    '        object_pl = "scriptmodule([" & object_pl_string(This.ModuleName, This.ModuleType, This.IsShutdown) & "])"
    'Case "icommodule", "commodule"
    '        object_pl = "commodule([" & object_pl_string(This.ModuleName, This.ModuleType, This.FLAGS) & "])"
    ''Case "typeinfos"
    ''        object_pl = typeinfos_pl(This, Cascade - 1)
    'Case "members"
    '        object_pl = members_pl(This, Cascade - 1)
    'Case "parameters"
    '        object_pl = parameters_pl(This, Cascade - 1)
    'Case "interfaces"
    '        object_pl = interfaces_pl(This, Cascade - 1)
    'Case "customdata"
    '        object_pl = customdata_pl(This, Cascade - 1)
    'Case "memberinfo"
    '        object_pl = memberinfo_pl(This, Cascade - 1)
    'Case "parameterinfo"
    '        object_pl = parameterinfo_pl(This, Cascade - 1)
    'Case "typeinfo"
    '        object_pl = typeinfo_pl(This, Cascade - 1)
    'Case "typelibinfo"
    '        object_pl = typelibinfo_pl(This, Cascade - 1)
    'Case "interfaceinfo"
    '        object_pl = interfaceinfo_pl(This, Cascade - 1)
    'Case "vartype"
    '        object_pl = vartype_enum_string(This, Cascade - 1)
   ' Case "invokekinds"
    '        object_pl = invokekinds_enum_string(This, Cascade - 1)
    'Case "typekinds"
    '        object_pl = typekinds_enum_string(This, Cascade - 1)
    'Case "coclassinfo"
    '        object_pl = coclassinfo_pl(This, Cascade - 1)
    'Case "vartypeinfo"
    '        object_pl = vartypeinfo_pl(This, Cascade - 1)
    
    Case "memberinfo"
            Select Case this.InvokeKind
                Case tli.INVOKE_PROPERTYGET, tli.INVOKE_PROPERTYGET Or tli.INVOKE_PROPERTYPUT, tli.INVOKE_PROPERTYPUTREF
                    object_pl = "property('" & this.Name & "','" & vartype_enum_string(this.ReturnType.VarType) & "',"
                    If Instance Then
'                        Stop
                        theCOMMorphism.call_by_Name_pred thevalue, Origin, this.Name, VbGet
'                        Stop
                        object_pl = object_pl & object_pl(thevalue, Cascade - 1, Instance, "", Origin) & ")"
                    Else
                        object_pl = object_pl & "_)"
                    End If
                                    
                Case tli.INVOKE_UNKNOWN
                    object_pl = "unknown('" & this.Name & "','" & vartype_enum_string(this.ReturnType.VarType) & "',"
                    If Instance Then
'                        Stop
                        theCOMMorphism.call_by_Name_pred thevalue, Origin, this.Name, VbGet
'                        Stop
                        object_pl = object_pl & object_pl(thevalue, Cascade - 1, Instance, "", Origin) & ")"
                    Else
                        object_pl = object_pl & "_)"
                    End If
                                    
                Case tli.INVOKE_FUNC, tli.INVOKE_EVENTFUNC
'                    Stop
                    object_pl = "method('" & this.Name & "','" & vartype_enum_string(this.ReturnType.VarType) & "',[" & object_pl(this.Parameters, Cascade, Instance, "parameters") & "])"
                
                Case Else
                    Stop
                    object_pl = "memberinfo(_)"
            End Select
    
    Case "memberinfovectored"
'    Stop
            Select Case this.InvokeKind
                Case tli.INVOKE_PROPERTYGET, tli.INVOKE_PROPERTYGET Or tli.INVOKE_PROPERTYPUT, tli.INVOKE_PROPERTYPUTREF
                    If Instance Then
                        theCOMMorphism.call_by_Name_pred thevalue, Origin, this.Name, VbGet
                        object_pl = object_pl(thevalue, Cascade - 1, Instance, "", Origin)
                    Else
                        object_pl = "_"
                    End If
            End Select
                                       
    Case "parameterinfo"
            If this Is Nothing Then object_pl = "parameterinfo(nothing)": Exit Sub
            object_pl = "datum(" & object_pl(this.VarTypeInfo, Cascade, Instance, "vartypeinfo") & "," & VBA.IIf(this.Optional, "?", "+") & this.Name & ")"
    Case "vartypeinfo"
            If this Is Nothing Then object_pl = "vartypeinfo(nothing)": Exit Sub
            object_pl = vartype_enum_string(this.VarType, Cascade - 1)
    
    Case "parameters"
'    Stop
        If this.Count = 0 Then object_pl = "[]": Exit Sub
        For Each itemObject In this
            object_pl = object_pl & "," & object_pl(itemObject, Cascade - 1, Instance, "parameterinfo")
        Next
        object_pl = Mid$(object_pl, 2)
    
    Case "datum"
        object_pl = theCOMMorphism.datum_to_prolog(this, Cascade)
        
    Case Else
    Dim memberlist, memberitem
        If ClassMembers.IsValid(objtype) Then
'            Stop
            memberlist = ClassMembers.Property(objtype)
            Select Case memberlist
                Case "ForEach"
                    If this.Count = 0 Then object_pl = objtype & "([])": Exit Sub
                    ReDim temparray(this.Count - 1): i = 0
                    For Each thisObjectii In this
                        theMorphism.object_pl_pred temparray(i), thisObjectii, Cascade - 1
                        i = i + 1
                    Next
                    object_pl = objtype & "([" & Join(temparray, ",") & "])"
                
                Case "ForEachName"
                    If this.Count = 0 Then object_pl = objtype & "([])": Exit Sub
                    ReDim temparray(this.Count - 1): i = 0
                    For Each thisObjectii In this
                        theMorphism.object_pl_pred thevalue, this.Property(thisObjectii), Cascade, Instance
                        temparray(i) = "property(" & thisObjectii & "," & thevalue & ")"
                        i = i + 1
                    Next
                    object_pl = objtype & "([" & Join(temparray, ",") & "])"
                
                Case "ForNameArray"
                    If this.Count = 0 Then object_pl = objtype & "([])": Exit Sub
                    ReDim temparray(this.Count - 1): i = 0
                        For i = 0 To this.Count - 1
                            theMorphism.object_pl_pred temparray(i), this.Property(i), Cascade - 1, Instance
                        Next
                    object_pl = objtype & "([" & Join(temparray, ",") & "])"
                    
                Case ""
                    object_pl = objtype & "([])"
                    
                Case Else
                
                 '   Stop
                    
                    For Each memberitem In Split(memberlist, ",")
                        
                        
                        object_pl = object_pl & "," & theCOMMorphism.get_member_by_Name_pl(this, memberitem, Cascade - 1)
                    Next
                    object_pl = objtype & "([" & Mid$(object_pl, 2) & "])"
            
            End Select
            Exit Sub
        Else
        ': Stop
        
        End If
        
        
    Debug.Print "' " & objtype
Exit Sub

        If Right(objtype, 8) = "typeinfo" Then
'            Stop
            Set theInterfaces = theCOMMorphism.get_members(this)
'            Stop
            If theInterfaces Is Nothing Then
                If Instance Then
                    object_pl = "typeinfo([" & objtype & "],[])": Exit Sub
                Else
                    object_pl = "typeinfo([" & objtype & "],[])": Exit Sub
                End If
                Exit Sub
            End If
'            Stop
            For Each theMemberInfo In theInterfaces
                Err.Clear
'                Stop
                If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then
                    If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or Not (theMemberInfo.InvokeKind = tli.INVOKE_PROPERTYPUT) Then
                        tempModel = tempModel & "," & object_pl_string(theMemberInfo, Cascade, Instance, "memberinfo", Origin.CodeObject)
                    End If
                End If
            Next
        '    Stop
            object_pl = "typeinfo([" & objtype & "],[" & Mid$(tempModel, 2) & "])"
            Exit Sub
        End If
'        Stop
        Dim theModelkind As MODELKINDs, model
        theModelkind = theCOMMorphism.get_MODELKIND_from_object(this)
        Select Case theModelkind
            Case MODELKINDs.MODELKIND_COLLECTION
'            Stop
                On Error Resume Next: Err.Clear
                For Each model In this
'                    Stop
                    tempmodelstr = tempmodelstr & "," & object_pl(model, Cascade - 1, Instance, "", this)
'                    Stop
                Next
                If Err Then Stop
                On Error GoTo 0
                object_pl = "collection(" & objtype & ",[" & Mid$(tempmodelstr, 2) & "])"
            Case MODELKINDs.MODELKIND_ARRAY
                For Each model In this
                    tempmodelstr = tempmodelstr & "," & object_pl(model, Cascade - 1, Instance, "", this)
                Next
                object_pl = "array(" & objtype & ",[" & Mid$(tempmodelstr, 2) & "])"
            Case MODELKINDs.MODELKIND_object
                
                Set theInterfaces = theCOMMorphism.get_members(this)
                
                If theInterfaces Is Nothing Then
                    If Instance Then
                        object_pl = "object([todo],[" & objtype & "],_,_)": Exit Sub
                    Else
                        object_pl = "object([todo],[" & objtype & "],_,_)": Exit Sub
                    End If
                    Exit Sub
                End If
'                        Stop
                If NamedArgs Then
                    For Each theMemberInfo In theInterfaces
                        Err.Clear
                        If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then
                            If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or Not (theMemberInfo.InvokeKind = tli.INVOKE_PROPERTYPUT) Then
                                tempModel = tempModel & "," & object_pl_string(theMemberInfo, Cascade - 1, Instance, "memberinfo", this)
                            End If
                        End If
                    Next
                Else
'                    Stop
                    For Each theMemberInfo In theInterfaces
                        Err.Clear
                        'Stop
                        If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then
                            
                            
                            If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or (theMemberInfo.InvokeKind = tli.INVOKE_PROPERTYGET) Then
                            ' Stop
                                tempModel = tempModel & "," & object_pl_string(theMemberInfo, Cascade, Instance, "memberinfovectored", this)
                            End If
                        End If
                    Next
'                    Stop
                End If

                object_pl = "object(" & theCOMMorphism.get_class(this) & "," & theCOMMorphism.get_type(this) & ",objptr(" & ObjPtr(this) & "),[" & Mid(tempModel, 2) & "])"
            Case MODELKINDs.MODELKIND_VALUE
                object_pl = "datum(" & theCOMMorphism.get_type(this) & "," & theCOMMorphism.value_to_string(this) & ")"
        End Select
 End Select
 If IsObject(this) Then theKnownMorphs.Property(my_ref & CStr(Cascade)) = object_pl: theKnownMorphs.Property(my_ref) = object_pl
'Stop
End Sub
 
Public Function object_pl_string(ByRef this, Optional Cascade = 3, Optional Instance, Optional ByVal Cast As String = "", Optional ByRef Origin, Optional NamedArgs = "True")
    theMorphism.object_pl_pred object_pl_string, this, Cascade, Instance, Cast, Origin, NamedArgs
End Function



Public Function pl_object(pl_string)
    pl_object_pred pl_object, pl_string
End Function


Public Sub pl_object_pred(ByRef pl_object, ByVal pl_string)

'Dim p As a_Datum
'Dim That As Integer
'
'    p.Value = 2323.3
    
 'pl_object_pred That, p.prolog

'on error resume next
Dim token, Args, arg(), temparg(), i As Long, tt
  Dim theObject As IThing, Index As Long, temp, tempObject As ithingwithrating
  Dim theinnerobj As IThing, tempCollection As Collection, targetedCollection As Collection
  Dim type_Properties() As String, thevariant As Variant, member_pl_object, arg_str
  Dim theCount As Long
    If IsObject(pl_string) Then Set pl_object = pl_string: Exit Sub
'    pl_string = pl_trim(pl_string)
    i = InStr(pl_string, "(")
    If i > 0 Then
            token = Left(pl_string, i - 1)
            arg_str = Mid(pl_string, i)
            Args = pl_trim(arg_str)
        Else
            token = "atom"
            arg_str = pl_string
            Args = pl_trim(pl_string)
    End If
    'Stop
    'If TypeName(pl_object) <> "Empty" Then token = TypeName(pl_object)
        
    Select Case LCase(token)
        Case "world_ref"
            Set pl_object = theIAvatar.World.ObjectByID(Val(pl_trim(Args)))
        Case "objptr"
            CopyMemory ObjPtr(pl_object), Val(pl_trim(Args)), 4
            'Not Supported Yet (I need to AddRef)
            'pl_object.AddRef
        Case "single"
            pl_object = CSng(pl_trim(Args))
        Case "byte", "vt_ui1", "vt_si1"
            pl_object = CByte(pl_trim(Args))
        Case "long", "vt_ui4", "vt_si4"
            pl_object = CLng(pl_trim(Args))
        Case "double"
            pl_object = CDbl(pl_trim(Args))
        Case "integer", "short", "color", "int", "vt_ui2", "vt_si2"
            pl_object = Val(pl_trim(Args))
        Case "string", "vt_textBSTR"
            pl_object = parse_pl_string_vbstr(Args)
        Case "world", "iworld"
            Set pl_object = theIAvatar.World
        Case "array"
            pl_split_pred temp, pl_trim(Args, 2), ","
            ReDim tempar(UBound(temp))
            For i = 0 To UBound(temp)
                If IsObject(pl_object(temp(i))) Then
                    Set tempar(i) = pl_object(temp(i))
                  Else
                    tempar(i) = pl_object(temp(i))
                End If
            Next i
            pl_object = tempar
        Case "nothing"
            Set pl_object = Nothing
        Case "user_type"
            pl_object_pred pl_object, Args
        Case "bool", "boolean"
            pl_object = IIf(InStr("truefalsefail", Args) = 1, True, False)
        Case "true"
            pl_object = True
        Case "false", "fail"
            pl_object = False
        Case "ithing"
            theWorldMorphism.pl_ithing_pred pl_object, arg_str
        Case "atom"
            If Val(pl_trim(pl_string)) > 0 Or pl_trim(pl_string) = "0" Then pl_object = Val(pl_trim(pl_string)): Exit Sub
            pl_object = parse_pl_string_vbstr(pl_string)
        Case "string"
            pl_object = parse_pl_string_vbstr(Args)
        Case "ipropertymap"
            pl_split_pred arg, pl_trim(Args, 2), ","
            Set pl_object = theIAvatar.World.CreatePropertyMap
            ReDim temparg(1)
            For i = 0 To UBound(arg)
                pl_split_pred temp, CStr(arg(i)), "="
                If InStr(arg(i), "=") > 0 Then
                    pl_object.Property(parse_pl_string_vbstr(temp(0))) = pl_trim(pl_object(temp(1)))
                 Else
                    pl_object.Property(parse_pl_string_vbstr(temp(0))) = pl_trim(pl_object(temp(0)))
                End If
            Next
        Case "ipropertylist", "propertylist"
            pl_split_pred arg, pl_trim(Args, 2), ","
            Set pl_object = New PropertyList
            For i = 0 To UBound(arg)
                pl_object.Add pl_object(arg(i))
            Next
        Case "collection"
            pl_split_pred arg, pl_trim(Args, 2), ","
            Set pl_object = New Collection
            For i = 0 To UBound(arg)
                pl_object.Add pl_object(arg(i))
            Next
        Case "ivector", "vector"
            Set temp = New Vector
            pl_split_pred arg, Args, ","
            Select Case UBound(arg)
                Case 2
                    pl_object_pred tt, arg(0): temp.x = tt
                    pl_object_pred tt, arg(1): temp.y = tt
                    pl_object_pred tt, arg(2): temp.Z = tt
                Case 3
                    'temp.rotation = val(pl_trim(pl_object(arg(0))))
                    pl_object_pred tt, arg(1): temp.x = tt
                    pl_object_pred tt, arg(2): temp.y = tt
                    pl_object_pred tt, arg(3): temp.Z = tt
           End Select
            Set pl_object = temp
        Case "ithing", "thing"
            theWorldMorphism.pl_ithing_pred pl_object, arg_str
        Case Else
            If ClassMembers.IsValid(token) Then
                Stop
                pl_object = token & "(["
                type_Properties = Split(ClassMembers.Property(token), ",")
                For i = 0 To UBound(type_Properties)
                    'Copy
                Next
Stop
            Else
                pl_object = token
            End If
            
            Exit Sub
            
            If IsNull(pl_object) Then Set pl_object = java_vbscript_engine.Eval("New " & token)
            pl_split_pred arg, pl_trim(Args, 2), ","
            pl_split_pred type_Properties, theCOMMorphism.object_model_array_from_class_string(token), ","

            If UBound(arg) <= UBound(type_Properties) Then theCount = UBound(arg) Else theCount = UBound(type_Properties)
            Select Case theCount
                Case -1
                    theWorldMorphism.pl_ithing_pred pl_object, "ithing(" & token & "_" & Args & "_)"
                Case Else
'                    thevariant = pl_object
                    For i = 0 To theCount
                        pl_object_pred member_pl_object, arg(i)

'                        assign_value RecordField(pl_object, type_Properties(i)), member_pl_object
'                        If Err Then assign_value pl_object.Property(type_Properties(i)), member_pl_object
                        
                        If IsObject(member_pl_object) Then
                            Set RecordField(pl_object, CStr(type_Properties(i))) = member_pl_object
                        Else
                            tli.RecordField(pl_object, CStr(type_Properties(i))) = member_pl_object
                        End If
                        If Err Then
                            If IsObject(member_pl_object) Then
                                Set pl_object.Property(type_Properties(i)) = member_pl_object
                            Else
                                pl_object.Property(type_Properties(i)) = member_pl_object
                            End If
                        End If
                    Next
            End Select
    End Select
End Sub


'ID-147-60-74

Public Sub array_pl_pred(ByRef object_pl, ParamArray this())
Dim temparray(), i As Long
Select Case UBound(this)
    Case -1
        object_pl = ""
    Case 0
        theMorphism.object_pl object_pl, this(0)
    Case Else
        ReDim temparray(UBound(this))
        For i = 0 To UBound(this)
             theMorphism.object_pl temparray(i), this(i)
        Next i
        object_pl = Join(temparray, ",")
End Select

End Sub


Public Sub set_Name_object(objectName, theObject)
Dim NewObject, Ptr, objtype, objclass
    If Not IsObject(theObject) Then
        pl_object_pred NewObject, theObject
        assign theObject, NewObject
        If IsObject(theObject) Then GoTo HaveObject:
        
        ''looks like we have a fixed DataType
        
    End If


HaveObject:
    If IsNumeric(objectName) Then
        objectName = CStr(objectName)
        If Val(objectName) = ObjPtr(theObject) Then
            'They supplied ptr (we must have given it)
            assign theKnownMorphs.Property(objectName), theObject
            Exit Sub
        Else
            '''They have a bad ptr (lets find out why)
            If theKnownMorphs.IsValid(objectName) Then
                '''their ptr was alive
                ''go on to friendly creation
            Else
                '''their ptr was dead and not in the Morphisms
                If theKnownMorphs.Property(ObjPtr(theObject)) Then
                    '''their object did exist in theKnownMorphs lets create a new ptr
                    assign theKnownMorphs.Property(objectName), theObject 'but go onto friendly creation
                Else
                    'thier object is not in theKnownMorphs and has a bad ptr
                    'so treat it as a friendly Name
                End If
            End If
        End If
    End If
    '''they supplied friendly Name
    On Error Resume Next
    Ptr = CStr(ObjPtr(theObject))
    objtype = LCase(typeName(theObject))
    objclass = LCase(theObject.Type)
    If objclass = "" Then objclass = objtype
    objectName = LCase(parse_vbstr_pl_string(objectName))
    
    theSWIPrologShell.Execute "assert_new(Name_ptr(" & objectName & ",(" & Ptr & ")))"
    theSWIPrologShell.Execute "assert_new(type_ptr(" & objtype & ",(" & Ptr & ")))"
    theSWIPrologShell.Execute "assert_new(class_ptr(" & objclass & ",(" & Ptr & ")))"
    theSWIPrologShell.Execute "assert_new(Name_type(" & objectName & "," & objtype & "))"
    theSWIPrologShell.Execute "assert_new(class_type(" & objclass & "," & objtype & "))"
    theSWIPrologShell.Execute "assert_new(Name_class(" & objectName & "," & objclass & "))"

    assign theKnownMorphs.Property(Ptr), theObject
            
End Sub

Public Function object_pl(ByRef this, Optional Cascade = 4, Optional Instance, Optional ByVal Cast As String = "", Optional ByRef Origin, Optional NamedArgs = "True")
    theMorphism.object_pl_pred object_pl, this, Cascade, Instance, Cast, Origin, NamedArgs
End Function
Public Function object_pl2(ByRef this, Optional Cascade = 4)
    theMorphism.object_pl object_pl2, this, Cascade
End Function

Public Function object_array_pl_string(ParamArray theObject())
 Dim i, temparray()
    Select Case UBound(theObject)
        Case -1
            object_array_pl_string = ""
        Case 0
            theMorphism.object_pl_pred object_array_pl_string, theObject(0)
        Case Else
            ReDim temparray(UBound(theObject))
            For i = 0 To UBound(theObject)
                theMorphism.object_pl_pred temparray(i), theObject(i)
            Next
            object_array_pl_string = Join(temparray, ",")
    End Select
End Function

Public Sub resolve_object(ByRef realObject, ByRef Name, ByVal Obj)

    If IsObject(Obj) Then
        If typeName(Obj) = "Nothing" Then
            'object nothing
            If Name <> "" Then
                ''' have Name and no object
            Else
                ''' no Name and no object (we're not happy)
            End If
        Else
            If Name <> "" Then
                ' have Name and object
                Set realObject = Obj
                If theKnownMorphs.IsValid(Name) Then
                        'Name found in dictionary
                        If theKnownMorphs.Property(Name) Is Obj Then
                            'All matched up
                            Set realObject = Obj
                            Exit Sub
                        Else
                            'they are trying to reName the object
                            If theKnownMorphs.Property(CStr(ObjPtr(Obj))) Is Obj Then
                                '''we have the object but they are giving it a new Name
                            Else
                                '''let search for the damn object
                                '''first lets try a quick prolog resolve
                                '''if this fails then we'll for each theKnownMorphs collection
                            End If
                        End If
                    Else
                        '''Name not found in dictionary
                End If
            Else
                '''have unNamed object
            End If
        End If
    
        ''' Not an object
       ' Select Case TypeName(Obj)
        '    Case "String", "Integer", "Long"
            ''' this is a reference or a constant
                If theKnownMorphs.IsValid(Obj) Then
                    ''' this is a reference
                    If Name = Obj Then
                        'they supplied the correct Name lets give it to them
                        Set realObject = theKnownMorphs.Property(Name): Exit Sub
                    Else
                        '''Name is different then found object
                        If Name <> "" Then
                            Set realObject = theKnownMorphs.Property(Obj)
                            '''lets check to see if the Name points to the same object
                            If theKnownMorphs.Property(Name) Is realObject Then
                                'they supplied a correct alias
                                Exit Sub
                            Else
                                '''their alias did not match
                                If typeName(realObject) = Name Then
                                    'they supplied a class as a Name
                                    Exit Sub
                                Else
                                    
                                
                            End If
                        'Else
                            'Name was null set it and give it
                            Name = Obj
                            Set realObject = theKnownMorphs.Property(Name): Exit Sub
                        End If
                    End If
                        Set realObject = theKnownMorphs.Property(Obj)
                        GoTo HaveRealObject
                'Else
                    ''' obj-string is not in dictionary
                    
            
                    

            'Case Else
             '
                
                If theKnownMorphs.IsValid(ObjPtr(Obj)) Then Set realObject = theKnownMorphs.Property(ObjPtr(Obj)):: GoTo HaveRealObject:
                For Each Name In theKnownMorphs
                    If theKnownMorphs.Property(Name) Is Obj Then
                        If Name <> "" Then Set realObject = Obj: Exit Sub
                    End If
                Next
        'End Select
            End If

If Name <> "" Then resolve_object realObject, "", Name

HaveRealObject:
import_object_to_instances realObject
Name = "cccccccccc"
   On Error Resume Next
   Name = Obj.Name
   'End If
  ' End Select
   End If
   End If
End Sub



