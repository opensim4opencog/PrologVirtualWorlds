Attribute VB_Name = "trashcan"
Option Explicit

Public Sub obj_pl_pred_old(ByRef obj_pl, ParamArray objlist() As Variant)
Dim j As Long, q As data_type

Dim tempstr As String, thisname As String, timestart, thisobj, t$, i%, ii%, thisnameii As String, thisobjii
Dim thisobjiii As Object, objtype As String, thisthing As Object   'on error resume next
Dim temparray() As String, thearray()
    If IsObject(objlist) Then obj_pl = obj_pl(Array(objlist)): Exit Sub
    ReDim thearray(UBound(objlist))
    For i% = 0 To UBound(objlist)
   ' thearray(i) = "cccccccccc"
        If IsObject(objlist(i%)) Then Set thisobj = objlist(i%) Else thisobj = objlist(i%)
        Select Case LCase(TypeName(thisobj))
            Case "ipropertymap", "propertymap"
                If thisobj.Count > 0 Then ReDim temparray(thisobj.Count - 1)
                
                For Each thisobjii In thisobj
                    Select Case LCase(TypeName(thisobj.Property(thisobjii)))
                        Case "integer", "long", "short", "double", "byte", "single"
                            temparray(ii) = thisobjii & "=" & "(" & thisobj.Property(thisobjii) & ")"
                        Case "string", "ivector", "vector", "propertylist", "ipropertylist", "ivwgeometrysprite", "vwspritedgeometry", "iivwgeometry", "ivwgeometry", "isound", "sound", "collection", "array"
                            temparray(ii) = thisobjii & "=" & obj_pl(thisobj.Property(thisobjii))
                        Case "thing", "ithing"
                            Set thisobjiii = thisobj.Property(thisobjii)
                            temparray(ii) = thisobjii & "=" & "ithing(" & LCase(thisobjiii.Type) & "," & thisobjiii.ID & "," & argument_opener & parse_vbstr_plstr(LCase(thisobjiii.Name)) & argument_terminator & ")"
                        Case "boolean"
                            temparray(ii) = thisobjii & "=" & IIf(thisobj.Property(thisobjii), "true", "false")
                        Case "iworld", "world"
                            Set thisobjiii = thisobj.Property(thisobjii)
                            temparray(ii) = object_opener & "$" & LCase(thisobjiii.Name) & object_terminator
                        Case Else
                            temparray(ii) = thisobjii & "=" & obj_pl(thisobj.Property(thisobjii))
                    End Select
                    ii = ii + 1
                Next
                If Right(Join(temparray, ","), 1) = "," Then
                thearray(i) = "ipropertymap(" & set_opener & Join(temparray, ",") & set_terminator & " )"
            Case "collection"
            '
                If thisobj.Count > 0 Then ReDim temparray(thisobj.Count - 1)
                
                For Each thisobjii In thisobj
                    Select Case LCase(TypeName(thisobjii))
                        Case "integer", "long", "short", "double", "byte", "single"
                            temparray(ii) = "( " & thisobjii & ")"
                        Case "string", "ivector", "vector", "propertylist", "ipropertylist", "ivwgeometrysprite", "vwspritedgeometry", "iivwgeometry", "ivwgeometry", "isound", "sound", "collection", "array"
                            temparray(ii) = obj_pl(thisobjii)
                        Case "thing", "ithing"
                            temparray(ii) = "ithing(" & LCase(thisobjii.Type) & "," & thisobjii.ID & "," & argument_opener & parse_vbstr_plstr(LCase(thisobjii.Name)) & argument_terminator & ")"
                        Case "boolean"
                            temparray(ii) = IIf(thisobjii, "true", "false")
                        Case "iworld", "world"
                            temparray(ii) = object_opener & "$" & LCase(thisobjii.Name) & object_terminator
                        Case Else
                            temparray(ii) = obj_pl(thisobjii)
                    End Select
                    ii = ii + 1
                Next
                If Right(Join(temparray, ","), 1) = "," Then
                thearray(i) = "collection(" & set_opener & Join(temparray, ",") & set_terminator & " )"
            
            Case Is = "string"
                thearray(i) = Trim(Replace(thisobj, vbTab, " "))
                If nl_proc_flag Then thearray(i) = parse_vbstr_plstr(thearray(i)) 'nlpobj.nlproccess(thisname)
                If InStr(thearray(i), "[") = 0 Then thearray(i) = string_opener & thearray(i) & string_terminator
                If stringlcase_flag Then thearray(i) = LCase(thearray(i))
            Case "ithing", "thing"
                If thisobj.IsStub = False Then
                    Set thisobjiii = thisobj.InstanceProperties
                    thearray(i) = parse_vbstr_plstr(theMorphism.obj_name(thisobj))
                    If typeobject_flag Then thearray(i) = "ithing(" & LCase(thisobj.Type) & " ," & LCase(thisobj.ID) & " ," & argument_opener & thearray(i) & argument_terminator & ")" '''if expand_ithings_flag then thisname = thisname '& " = [" & obj_pl(thisobjiii) & " ] "
                Else
                    thearray(i) = object_opener & theMorphism.obj_name(thisobj) & object_terminator
                    If typeobject_flag Then thearray(i) = LCase(TypeName(thisobj)) & thearray(i)
                End If
                If objectlcase_flag Then thearray(i) = LCase(thearray(i))
            Case "propertylist", "ipropertylist"
                If thisobj.Count > 0 Then ReDim temparray(thisobj.Count - 1)
                For ii% = 0 To thisobj.Count - 1
                     theMorphism.obj_pl_pred temparray(ii%), thisobj.Property(ii%)
                Next ii%
                If Right(Join(temparray, ","), 1) = "," Then
                thearray(i) = "ipropertylist(" & list_opener & Join(temparray, ",") & list_terminator & ")"
            Case "ivector", "vector"
                thearray(i) = "ivector" & argument_opener & thisobj.Rotation & "," & thisobj.X & "," & thisobj.Y & "," & thisobj.Z & argument_terminator
            Case "iworld", "world"
                thearray(i) = object_opener & "$" & LCase(theMorphism.obj_name(thisobj)) & object_terminator
            Case "integer", "long", "short", "double", "byte", "single" ', "color", "string"
                thearray(i) = CStr(thisobj)
            Case "boolean"
                thearray(i) = IIf(thisobj, "true", "false")
            Case "ivwgeometrysprite", "vwspritedgeometry"
                thearray(i) = "geometrysprite([" & theMorphism.obj_pl2(thisobj.AccessoryScene, thisobj.CurrentGesture, thisobj.Frame, thisobj.GeometryFrame, thisobj.GeometryName, thisobj.ImageFileName, thisobj.RotationAxis) & "])" '                           thisname = "geometrysprite([accessoryscene=" & obj_pl(thisobj.accessoryscene) & ",currentgesture=" & obj_pl(thisobj.currentgesture) & ", frame=" & obj_pl(thisobj.frame) & ",geometryframe=" & obj_pl(thisobj.geometryframe) & " ,geometryname= " & obj_pl(thisobj.geometryname) & " , imagefilename=" & obj_pl(thisobj.imagefilename) & ",rotationaxis=" & thisobj.rotationaxis & "])"
            Case "ivwgeometry", "vwgeometry"
                thearray(i) = "geometry([" & theMorphism.obj_pl2(thisobj.Frame, thisobj.GeometryFrame, thisobj.GeometryName) & "])" '                            thisname = "geometry([ frame=" & obj_pl(thisobj.frame) & ",geometryframe=" & obj_pl(thisobj.geometryframe) & " ,geometryname= " & obj_pl(thisobj.geometryname) & "])"
            Case "isound", "sound"
                thearray(i) = "sound([" & theMorphism.obj_pl2(thisobj.Is3D, thisobj.IsLooping, thisobj.SoundURL) & "])" '                            thisname = "sound([ is3d=" & obj_pl(thisobj.is3d) & ",islooping=" & obj_pl(thisobj.islooping) & " ,soundurl= " & obj_pl(thisobj.soundurl) & "])"
            Case "iboundary", "boundary"
                thearray(i) = "boundry([" & theMorphism.obj_pl2(thisobj.HeightLower, thisobj.HeightUpper, thisobj.IsAutoSolidBoundary, thisobj.IsClosed, thisobj.IsInverse, thisobj.IsPassable, thisobj.IsProximity, thisobj.VertexCount) & "])" '                            thisname = "sound([ is3d=" & obj_pl(thisobj.is3d) & ",islooping=" & obj_pl(thisobj.islooping) & " ,soundurl= " & obj_pl(thisobj.soundurl) & "])"
            Case "imethod", "method"
                thearray(i) = "method([" & theMorphism.obj_pl2(thisobj.methodname, thisobj.IsInline, thisobj.FLAGS, thisobj.Module) & "])"
            Case "imodule", "module"
                thearray(i) = "module([" & theMorphism.obj_pl2(thisobj.methodname, thisobj.IsInline, thisobj.FLAGS, thisobj.Module) & "])"
            Case "iscriptmodule", "scriptmodule"
                thearray(i) = "scriptmodule([" & theMorphism.obj_pl2(thisobj.ModuleName, thisobj.ModuleType, thisobj.IsShutdown) & "])"
            Case "icommodule", "commodule"
                thearray(i) = "commodule([" & theMorphism.obj_pl2(thisobj.ModuleName, thisobj.ModuleType, thisobj.FLAGS) & "])"
            Case "nothing", "null", "empty", "iavatarprofile", "avatarprofile", "imenuitem", "menuitem"
                thearray(i) = "user_type(" & LCase(TypeName(thisobj)) & ")"
            Case Else
                
                thearray(i) = "user_type(" & LCase(TypeName(thisobj)) & ")": '
        End Select
        If IsArray(thisobj) Then
 '
                ii = UBound(thisobj)
                ReDim temparray(ii)
                If ii > -1 Then
                    For j = 0 To ii
                        theMorphism.Print obj_pl; temparray(j), thisobj(j)
 '
                    Next j
                    thearray(i) = "array([" & Join(temparray, ",") & "])"
'
                Else
                  '  thearray(i) = "array([])"
'
                End If
        End If
        
    Next i%
    For i = 0 To UBound(thearray)
    If Trim(thearray(i)) = "" Then
    Next
    If Right(Trim(Join(thearray, ",")), 1) = "," Then


obj_pl = Join(thearray, ",")
End Sub
