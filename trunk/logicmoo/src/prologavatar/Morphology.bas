Attribute VB_Name = "Morphology"
Option Explicit

Public Enum InvokeKinds
  INVOKE_UNKNOWN = 0
  INVOKE_FUNC = 1
  INVOKE_PROPERTYGET = 2
  INVOKE_PROPERTYPUT = 4
  INVOKE_PROPERTYPUTREF = 8
  'Special TLI values
  INVOKE_EVENTFUNC = 16
  INVOKE_CONST = 32
End Enum

Public Enum TypeKind
 TKIND_ENUM = 0 'enum Constants Set of related Long constant values
 TKIND_RECORD = 1 'record Records Ordered data members of intrinsic or defined types
 TKIND_MODULE = 2 'module Declarations Constants List of Dll-declared Public Functions and/or constant values of any type.
 TKIND_INTERFACE = 3 'interface Interfaces Description of an IUnknown derived vtable
 TKIND_DISPATCH = 4 'dispinterface Interfaces Description of an IDispatch derived object. Members are accessible using IDispatch::Invoke only, unless the dispinterface is also dual.
 TKIND_COCLASS = 5 'coclass CoClasses An object which can be created using CoCreateInstance (CreateObject or New in VB). A coclass is a list of interfaces.
 TKIND_ALIAS = 6 'alias IntrinsicAliases <All> An alias to an intrinsic type, or any of the other TypeKinds.
 TKIND_UNION = 7 'union Unions A description of one or more records which can share the same block of memory.
 TKIND_MAX = 8 ' N/A N/A Returned by TypeKind if data not available
End Enum

Private m_fCheckedTLIArrayBounds As Boolean
Private m_fFixArrayBounds As Boolean

Public Function OrderedVTableFunctions( _
  ByVal TLInf As TypeLibInfo, _
  IFaceName As String) As TLI.MemberInfo()
Dim TI As InterfaceInfo
Dim Bases As TypeInfos
Dim MI As MemberInfo
Dim OVF() As MemberInfo
Dim MaxOffset As Integer
Dim CurOffset As Integer
  'Make sure we get a VTable interface before proceeding
  On Error Resume Next
  Set TI = TLInf.TypeInfos.NamedItem(IFaceName)
  Set TI = TI.VTableInterface
  If Err Or TI Is Nothing Then Exit Function
  On Error GoTo 0
  With TI.Members
    'Largest VTableOffset is generally on the last
    'member in this collection
    MaxOffset = .item(.Count).VTableOffset
    ReDim OVF(MaxOffset \ 4)
  End With
  Do Until TI Is Nothing
    'Walk each member
    For Each MI In TI.Members
      CurOffset = MI.VTableOffset
      If CurOffset > MaxOffset Then
        'This is extremely rare
        MaxOffset = CurOffset
        ReDim Preserve OVF(MaxOffset \ 4)
      End If
      Set OVF(CurOffset \ 4) = MI
    Next
    'Get the next base
    Set Bases = TI.ImpliedInterfaces
    Set TI = Nothing
    If Bases.Count Then Set TI = Bases(1)
  Loop
  OrderedVTablePublic Functions = OVF
End Function



Public Sub pl_obj_pred(ByRef pl_obj, ByVal plstr)
'on error resume next
Dim token, args, arg(), temparg(), i As Long, tt
  Dim TheObject As IThing, Index As Long, temp, tempObject As ithingwithrating
  Dim theIevent As ievent
  Dim theinnerobj As IThing, tempcollection As Collection, targetedcollection As Collection
  Dim type_properties(), thevariant As Variant, member_pl_obj
  Dim theCount As Long
    If IsObject(plstr) Then Set pl_obj = plstr: Exit Sub
    plstr = pl_trim(plstr)
    i = InStr(plstr, "(")
    If i > 0 Then
            token = Left(plstr, i - 1)
            args = pl_trim(Mid(plstr, i))
        Else
            token = "atom"
            args = pl_trim(plstr)
    End If
    'If TypeName(pl_obj) <> "Empty" Then token = TypeName(pl_obj)
    
    'pl_split_pred type_properties, type_tlb_str(token), ","
    
    Select Case LCase(token)
        Case "single"
            pl_obj = CSng(pl_trim(args))
        Case "byte", "vt_ui1", "vt_si1"
            pl_obj = CByte(pl_trim(args))
        Case "long", "vt_ui4", "vt_si4"
            pl_obj = CLng(pl_trim(args))
        Case "double"
            pl_obj = CDbl(pl_trim(args))
        Case "integer", "short", "color", "int", "vt_ui2", "vt_si2"
            pl_obj = Val(pl_trim(args))
        Case "string", "vt_bstr"
            pl_obj = parse_plstr_vbstr(args)
        Case "world", "iworld"
            Set pl_obj = theIAvatar.World ': stop
        Case "array"
            pl_split_pred temp, pl_trim(args, 2), ","
            ReDim tempar(UBound(temp))
            For i = 0 To UBound(temp)
                If IsObject(pl_obj(temp(i))) Then
                    Set tempar(i) = pl_obj(temp(i))
                  Else
                    tempar(i) = pl_obj(temp(i))
                End If
            Next i
            pl_obj = tempar
        Case "nothing"
            Set pl_obj = Nothing
        Case "user_type"
            pl_obj_pred pl_obj, args
        Case "bool", "boolean"
            pl_obj = IIf(InStr("truefalsefail", args) = 1, True, False)
        Case "true"
            pl_obj = True
        Case "false", "fail"
            pl_obj = False
        Case "ithing"
            pl_ithing_pred pl_obj, "ithing(" & args & ")"
        Case "atom"
            If Val(pl_trim(plstr)) > 0 Or pl_trim(plstr) = "0" Then pl_obj = Val(pl_trim(plstr)): Exit Sub
            pl_obj = parse_plstr_vbstr(plstr)
        Case "string"
            pl_obj = parse_plstr_vbstr(args)
        Case "ipropertymap"
            pl_split_pred arg, pl_trim(args, 2), ","
            Set pl_obj = theIAvatar.World.CreatePropertyMap
            ReDim temparg(1)
            For i = 0 To UBound(arg)
                pl_split_pred temp, CStr(arg(i)), "="
                If InStr(arg(i), "=") > 0 Then
                    pl_obj.Property(parse_plstr_vbstr(temp(0))) = pl_trim(pl_obj(temp(1)))
                 Else
                    pl_obj.Property(parse_plstr_vbstr(temp(0))) = pl_trim(pl_obj(temp(0)))
                End If
            Next
        Case "ipropertylist", "propertylist"
            pl_split_pred arg, pl_trim(args, 2), ","
            Set pl_obj = New PropertyList
            For i = 0 To UBound(arg)
                pl_obj.Add pl_obj(arg(i))
            Next
        Case "collection"
            pl_split_pred arg, pl_trim(args, 2), ","
            Set pl_obj = New Collection
            For i = 0 To UBound(arg)
                pl_obj.Add pl_obj(arg(i))
            Next
        Case "ivector", "vector"
            Set temp = New Vector
            pl_split_pred arg, args, ","
            Select Case UBound(arg)
                Case 2
                    pl_obj_pred tt, arg(0): temp.X = tt
                    pl_obj_pred tt, arg(1): temp.Y = tt
                    pl_obj_pred tt, arg(2): temp.Z = tt
                Case 3
                    'temp.rotation = val(pl_trim(pl_obj(arg(0))))
                    pl_obj_pred tt, arg(1): temp.X = tt
                    pl_obj_pred tt, arg(2): temp.Y = tt
                    pl_obj_pred tt, arg(3): temp.Z = tt
           End Select
            Set pl_obj = temp
        Case Else
            If pl_obj = Null Then Set pl_obj = java_vbscript_engine.Eval("New " & token)
            pl_split_pred arg, pl_trim(args, 2), ","
            If UBound(arg) <= UBound(type_properties) Then theCount = UBound(arg) Else theCount = UBound(type_properties)
            Select Case theCount
                Case -1
                    pl_ithing_pred pl_obj, "ithing(" & token & "_" & args & "_)"
                Case Else
'                    thevariant = pl_obj
                    For i = 0 To theCount
                        pl_obj_pred member_pl_obj, arg(i)

'                        assign_value RecordField(pl_obj, type_properties(i)), member_pl_obj
'                        If Err Then assign_value pl_obj.Property(type_properties(i)), member_pl_obj
                        
                        If IsObject(member_pl_obj) Then
                            Set RecordField(pl_obj, CStr(type_properties(i))) = member_pl_obj
                        Else
                            RecordField(pl_obj, CStr(type_properties(i))) = member_pl_obj
                        End If
                        If Err Then
                            If IsObject(member_pl_obj) Then
                                Set pl_obj.Property(type_properties(i)) = member_pl_obj
                            Else
                                pl_obj.Property(type_properties(i)) = member_pl_obj
                            End If
                        End If
                    Next
            End Select
    End Select
End Sub


'ID-147-60-74

Public Sub array_pl_pred(ByRef obj_pl, ParamArray AnyObject())
Dim temparray(), i As Long
Select Case UBound(AnyObject)
    Case -1
        obj_pl = ""
    Case 0
        obj_pl_pred obj_pl, AnyObject(0)
    Case Else
        ReDim temparray(UBound(AnyObject))
        For i = 0 To UBound(AnyObject)
             obj_pl_pred temparray(i), AnyObject(i)
        Next i
        obj_pl = Join(temparray, ",")
End Select

End Sub


Public Sub add_com_dll(ByVal Filename)
If TypeName(DLLFiles) = "Nothing" Then Set DLLFiles = New Collection
Dim ttlinfo  As TypeLibInfo
Set ttlinfo = TLI.TLIApplication.TypeLibInfoFromFile(Filename)
DLLFiles.Add ttlinfo
theComClassBrowser.Text1.AddItem Filename
'Stop

End Sub


Public Function create_instance1(ByVal objecttype As String, ByVal instancename)
Dim ttlinfo  As TypeLibInfo
Dim tinterfaceinfo As InterfaceInfo
Dim theinstancewithinfo As instancewithinfo
Dim ttypeinfo As TypeInfo

For Each ttlinfo In DLLFiles
        Set tinterfaceinfo = ttlinfo.TypeInfos.NamedItem(objecttype)
            If Not (tinterfaceinfo Is Nothing) Then
                Set theinstancewithinfo.TheObject = CreateObject(ttlinfo.Name & "." & objecttype)
                Set theinstancewithinfo.theinterfaceinfo = tinterfaceinfo
                    theinstancewithinfo.thename = instancename
              Set theInstances.Property(instancename) = theinstancewithinfo: create_instance1 = instancename
                'stop
                Exit Function
            End If
Next
create_instance1 = "notfound"
End Function

Public Function set_property_value(ByVal instancename, ByVal propertyname, ByVal value)
Dim tinterfaceinfo As TLI.InterfaceInfo
Dim theinstancewithinfo As instancewithinfo
Dim tobject As Object
Dim themembers As Members
Dim theproperty As MemberInfo
Dim ttypeinfo As TypeInfo

    Set tobject = theInstances.Property(instancename).TheObject
    Set tinterfaceinfo = theInstances.Property(instancename).theinterfaceinfo
    Set themembers = tinterfaceinfo.Members
    For Each theproperty In themembers
        'themember 'memberinfo
        If theproperty.Name = propertyname Then
            Select Case theproperty.InvokeKind
            
                Case INVOKE_PROPERTYPUT
                Case INVOKE_PROPERTYGET
                Case INVOKE_FUNC
                                
            
            End Select
                
        End If
    Next

set_property_value = "notfound"
End Function
    
    
Public Function vp(ByVal TheObject): Set vp = TheObject: End Function


Public Sub pl_ithing_pred(ByRef pl_ithing, ByVal plstrIn)
  Dim vbstr, temp As Object, tempcollection As Collection, targetedcollection As Collection, plstr
'''VarPtr (theobject)
    If IsObject(plstr) Then Set pl_ithing = plstr: Exit Sub
    
    get_all_ithings_pred tempcollection
    Set targetedcollection = New Collection

'inline
    plstr = plstrIn
    plstr = pl_trim(plstr, , True)
    plstr = Replace(plstr, ")", "\)")
    plstr = Replace(plstr, "(", "\(")
    plstr = Replace(plstr, "_", ".+")
    plstr = Replace(plstr, " ", "")
    java_vbscript_engine.ExecuteStatement ("regex.pattern = """ & LCase(plstr) & """")
    
    For Each temp In tempcollection
        vbstr = Replace(LCase("ithing(" & temp.Type & "," & temp.ID & "," & parse_vbstr_plstr(temp.Name) & ")"), " ", "")
        If java_vbscript_engine.Eval("regex.test(""" & vbstr & """)") Then targetedcollection.Add temp
'        Stop
    Next
    
    If targetedcollection.Count = 1 Then
        For Each pl_ithing In targetedcollection
            Exit Sub
        Next
    End If
    If targetedcollection.Count = 0 Then pl_ithing = Null: Exit Sub
    Set pl_ithing = targetedcollection
'Stop
End Sub

Public Sub get_all_ithings_pred(ByRef get_all_ithings)
Dim thisobj, tempcollection As New Collection, innerobj, innerinnerobj
    For Each thisobj In theIAvatar.Container.Contents
        tempcollection.Add thisobj
        For Each innerobj In thisobj.Contents
            tempcollection.Add innerobj
            For Each innerinnerobj In thisobj.Contents
                tempcollection.Add innerinnerobj
            Next
        Next
    Next
    Set get_all_ithings = tempcollection
End Sub


Public Function obj_name(ByVal objthing) As String
   On Error Resume Next
    If (objthing Is Nothing) Then obj_name = "nothing": Exit Function
  
    If LCase(TypeName(objthing)) = "iworld" Then obj_name = "iworld": Exit Function
    'if objthing.isstub then obj_name = "isstub": exit Function
    obj_name = objthing.Property("name")

End Function

Public Sub assign_pred(ByRef LeftSide, ByVal RightSide)
If IsObject(RightSide) Then
    Set LeftSide = RightSide
Else
    'if typename(leftside)="N
    LeftSide = RightSide
End If
End Sub

Public Function PrototypeMember( _
  TLInf As TypeLibInfo, _
  ByVal SearchData As Long, _
  ByVal InvokeKinds As InvokeKinds, _
  Optional ByVal MemberId As Long = -1, _
  Optional ByVal MemberName As String) As String
Dim pi As ParameterInfo
Dim fFirstParameter As Boolean
Dim fIsConstant As Boolean
Dim fByVal As Boolean
Dim retVal As String
Dim ConstVal As Variant
Dim strTypeName As String
Dim VarTypeCur As Integer
Dim fDefault As Boolean, fOptional As Boolean, fParamArray As Boolean
Dim TIType As TypeInfo
Dim TIResolved As TypeInfo
Dim TKind As TypeKinds
Dim theObj
  With TLInf
'  Stop
  
    fIsConstant = GetSearchType(SearchData) And tliStConstants
      Set theObj = .GetMemberInfo(SearchData, InvokeKinds, MemberId, MemberName)
    With .GetMemberInfo(SearchData, InvokeKinds, MemberId, MemberName)
'Stop
      If fIsConstant Then
        retVal = "Const "
        
      Else
        retVal = "+" & invokekinds_pl(InvokeKinds) & "("
        Select Case .ReturnType.VarType
          Case VT_VOID, VT_HRESULT
            retVal = retVal & "object_public sub(" & LCase(TLInf.GetTypeInfo(SearchData And &HFFFF&).Name)
          Case Else
            retVal = retVal & "object_func(" & LCase(TLInf.GetTypeInfo(SearchData And &HFFFF&).Name)
        End Select
      
      
      'ElseIf InvokeKinds = INVOKE_FUNC Or InvokeKinds = INVOKE_EVENTFUNC Then
       ' Select Case .ReturnType.VarType
       '   Case VT_VOID, VT_HRESULT
       '     retVal = "object_public sub(" & LCase(TLInf.GetTypeInfo(SearchData And &HFFFF&).name)
       '   Case Else
       '     retVal = "object_fuct(" & LCase(TLInf.GetTypeInfo(SearchData And &HFFFF&).name)
       ' End Select
       'Else
       ' retVal = "object_prop(" & LCase(TLInf.GetTypeInfo(SearchData And &HFFFF&).name)
      End If
      
      retVal = retVal & ",_ ," & LCase(.Name) & ", "
      
      With .Parameters
        If .Count Then
          retVal = retVal & "("
          fFirstParameter = True
          fParamArray = .OptionalCount = -1
          For Each pi In .Me
            If Not fFirstParameter Then
              retVal = retVal & ", "
            End If
            fFirstParameter = False
            fDefault = pi.Default
            fOptional = fDefault Or pi.Optional
            If fOptional Then
              If fParamArray Then
                'This will be the only optional parameter
                retVal = retVal & "[ParamArray "
              Else
                retVal = retVal & "["
              End If
            End If
            With pi.VarTypeInfo
              Set TIType = Nothing
              Set TIResolved = Nothing
              TKind = TKIND_MAX
              VarTypeCur = .VarType
              If (VarTypeCur And Not (VT_ARRAY Or VT_VECTOR)) = 0 Then
              'If Not .TypeInfoNumber Then 'This may error, don't use here
                On Error Resume Next
                Set TIType = .TypeInfo
                If Not TIType Is Nothing Then
                  Set TIResolved = TIType
                  TKind = TIResolved.TypeKind
                  Do While TKind = TKIND_ALIAS
                    TKind = TKIND_MAX
                    Set TIResolved = TIResolved.ResolvedType
                    If Err Then
                      Err.Clear
                    Else
                      TKind = TIResolved.TypeKind
                    End If
                  Loop
                End If
                Select Case TKind
                  Case TKIND_INTERFACE, TKIND_COCLASS, TKIND_DISPATCH
                    fByVal = .PointerLevel = 1
                  Case TKIND_RECORD
                    'Records not passed ByVal in VB
                    fByVal = False
                  Case Else
                    fByVal = .PointerLevel = 0
                End Select
                If fByVal Then retVal = retVal & "+" Else retVal = retVal & "?" ''ByVal Else ByRef
                retVal = retVal & pi.Name
                If VarTypeCur And (VT_ARRAY Or VT_VECTOR) Then retVal = retVal & "()"
                If TIType Is Nothing Then 'Error
                  retVal = retVal & " As ?"
                Else
                  If .IsExternalType Then
                    retVal = retVal & " As " & _
                             .TypeLibInfoExternal.Name & "." & TIType.Name
                  Else
                    retVal = retVal & " As " & TIType.Name
                  End If
                End If
                On Error GoTo 0
              Else
                If .PointerLevel = 0 Then retVal = retVal & "+" Else retVal = retVal & "?" ''ByVal Else ByRef
                retVal = retVal & pi.Name
                If VarTypeCur <> vbVariant Then
                  strTypeName = TypeName(.TypedVariant)
                  If VarTypeCur And (VT_ARRAY Or VT_VECTOR) Then
                    retVal = retVal & "() As " & Left$(strTypeName, Len(strTypeName) - 2)
                  Else
                    retVal = retVal & " As " & strTypeName
                  End If
                End If
              End If
              If fOptional Then
                If fDefault Then
                  retVal = retVal & ProduceDefaultValue(pi.DefaultValue, TIResolved)
                End If
                retVal = retVal & "]"
              End If
            End With
          Next
          retVal = retVal & ")"
        End If
      End With
      If fIsConstant Then
        ConstVal = .value
        retVal = retVal & " = " & ConstVal
        Select Case VarType(ConstVal)
          Case vbInteger, vbLong
            If ConstVal < 0 Or ConstVal > 15 Then
              retVal = retVal & " (&H" & Hex$(ConstVal) & ")"
            End If
        End Select
      Else
        With .ReturnType
          VarTypeCur = .VarType
          If VarTypeCur = 0 Or (VarTypeCur And Not (VT_ARRAY Or VT_VECTOR)) = 0 Then
          'If Not .TypeInfoNumber Then 'This may error, don't use here
            On Error Resume Next
            If Not .TypeInfo Is Nothing Then
              If Err Then 'Information not available
                retVal = retVal & " As ?"
              Else
                If .IsExternalType Then
                  retVal = retVal & " As " & _
                           .TypeLibInfoExternal.Name & "." & .TypeInfo.Name
                Else
                  retVal = retVal & " As " & .TypeInfo.Name
                End If
              End If
            End If
            If VarTypeCur And (VT_ARRAY Or VT_VECTOR) Then retVal = retVal & "()"
            On Error GoTo 0
          Else
            Select Case VarTypeCur
              Case VT_VARIANT, VT_VOID, VT_HRESULT
              Case Else
                strTypeName = TypeName(.TypedVariant)
                If VarTypeCur And (VT_ARRAY Or VT_VECTOR) Then
                  retVal = retVal & "() As " & Left$(strTypeName, Len(strTypeName) - 2)
                Else
                  retVal = retVal & " As " & strTypeName
                End If
            End Select
          End If
        End With
      End If
      PrototypeMember = retVal & "  " & _
                        "% Member of " & TLInf.Name & "." & _
                        TLInf.GetTypeInfo(SearchData And &HFFFF&).Name & _
                        vbCr & "  " & .HelpString
    End With
  End With
End Function


Public Function GetSearchType(ByVal SearchData As Long) As TliSearchTypes
  If SearchData And &H80000000 Then
    GetSearchType = ((SearchData And &H7FFFFFFF) \ &H1000000 And &H7F&) Or &H80
  Else
    GetSearchType = SearchData \ &H1000000 And &HFF&
  End If
End Function
Public Function GetTypeInfoNumber(ByVal SearchData As Long) As Integer
  GetTypeInfoNumber = SearchData And &HFFF&
End Function
Public Function GetLibNum(ByVal SearchData As Long) As Integer
  SearchData = SearchData And &H7FFFFFFF
  GetLibNum = ((SearchData \ &H2000& And &H7) * &H100&) Or _
               (SearchData \ &H10000 And &HFF&)
End Function
Public Function GetHidden(ByVal SearchData As Long) As Boolean
    If SearchData And &H1000& Then GetHidden = True
End Function
Public Function BuildSearchData( _
   ByVal TypeInfoNumber As Integer, _
   ByVal SearchTypes As TliSearchTypes, _
   Optional ByVal LibNum As Integer, _
   Optional ByVal Hidden As Boolean = False) As Long
  If SearchTypes And &H80 Then
    BuildSearchData = _
      (TypeInfoNumber And &H1FFF&) Or _
      ((SearchTypes And &H7F) * &H1000000) Or &H80000000
  Else
    BuildSearchData = _
      (TypeInfoNumber And &H1FFF&) Or _
      (SearchTypes * &H1000000)
  End If

  If LibNum Then
    BuildSearchData = BuildSearchData Or _
      ((LibNum And &HFF) * &H10000) Or _
      ((LibNum And &H700) * &H20&)
  End If
  If Hidden Then
    BuildSearchData = BuildSearchData Or &H1000&
  End If
End Function



Public Function ProduceDefaultValue(DefVal As Variant, ByVal TI As TypeInfo) As String
Dim lTrackVal As Long
Dim MI As MemberInfo
Dim TKind As TypeKinds
    If TI Is Nothing Then
        Select Case VarType(DefVal)
            Case vbString
                If Len(DefVal) Then
                    ProduceDefaultValue = """" & DefVal & """"
                End If
            Case vbBoolean 'Always show for Boolean
                ProduceDefaultValue = DefVal
            Case vbDate
                If DefVal Then
                    ProduceDefaultValue = "#" & DefVal & "#"
                End If
            Case Else 'Numeric Values
                If DefVal <> 0 Then
                    ProduceDefaultValue = DefVal
                End If
        End Select
    Else
        'See if we have an enum and track the matching member
        'If the type is an object, then there will never be a
        'default value other than Nothing
        TKind = TI.TypeKind
        Do While TKind = TKIND_ALIAS
            TKind = TKIND_MAX
            On Error Resume Next
            Set TI = TI.ResolvedType
            If Err = 0 Then TKind = TI.TypeKind
            On Error GoTo 0
        Loop
        If TI.TypeKind = TKIND_ENUM Then
            lTrackVal = DefVal
            For Each MI In TI.Members
                If MI.value = lTrackVal Then
                    ProduceDefaultValue = MI.Name
                    Exit For
                End If
            Next
        End If
    End If
End Function



Public Function FixArrayBounds(VTInfo As VarTypeInfo, Bounds() As Long) As Integer
Dim Dims As Integer
  If Not m_fCheckedTLIArrayBounds Then CheckTLIArrayBounds
  FixArrayBounds = VTInfo.ArrayBounds(Bounds)
  If m_fFixArrayBounds Then
    For Dims = 1 To FixArrayBounds
      Bounds(Dims, 2) = Bounds(Dims, 2) + 2 * Bounds(Dims, 1) - 1
    Next
  End If
End Function
Public Sub CheckTLIArrayBounds()
Dim Bounds() As Long
  Debug.Assert Not m_fCheckedTLIArrayBounds
  On Error Resume Next
  TLI.TypeLibInfoFromFile("stdole2.tlb").TypeInfos.NamedItem("GUID").Members(4).ReturnType.ArrayBounds Bounds
  If Bounds(1, 2) = 8 Then m_fFixArrayBounds = True
  m_fCheckedTLIArrayBounds = True
End Sub



Public Function ImplementedInterfaces( _
  ByVal CCInfo As CoClassInfo, _
  Libs As Collection, _
  Optional ByVal fIncludeDefault As Boolean = True) As SearchResults
Dim SR As SearchResults
Dim IFace As InterfaceInfo
Dim TLInf As TypeLibInfo
Dim TLInfLast As TypeLibInfo
Dim strKey As String
Dim TypeInfos(0) As Integer
Dim tinfoDefault As IUnknown
Dim fLookupLib As Boolean
  Set Libs = New Collection
  If Not fIncludeDefault Then
    Set tinfoDefault = CCInfo.DefaultInterface.ITypeInfo
  End If
  On Error GoTo Error
  For Each IFace In CCInfo.Interfaces
    If 0 = (IFace.AttributeMask And _
            (IMPLTYPEFLAG_FSOURCE Or IMPLTYPEFLAG_FRESTRICTED)) Then
      If Not fIncludeDefault Then
        If tinfoDefault Is IFace.ITypeInfo Then
          'Turn off default check once we find it
          fIncludeDefault = True
          GoTo DoNext
        End If
      End If
      Set TLInf = IFace.Parent
      TypeInfos(0) = IFace.TypeInfoNumber
      fLookupLib = True
      If TLInfLast Is Nothing Then
      ElseIf TLInf.IsSameLibrary(TLInfLast) Then
        'TLInfLast is simply an optimization.
        'Checking IsSameLibrary is faster than
        'doing a collection lookup.
        fLookupLib = False
        'Use the TypeLibInfo object from the
        'collection so that it has the LibNum set
        Set TLInf = TLInfLast
      End If
If fLookupLib Then
        strKey = CStr(ObjPtr(TLInf.ITypeLib))
        On Error Resume Next
        'Use the TypeLibInfo object from the
        'collection so that it has the LibNum set
        Set TLInf = Libs(strKey)
        If Err Then
          'New library
          TLInf.LibNum = Libs.Count + 1
          Libs.Add TLInf, strKey
          Set TLInfLast = TLInf
        End If
        On Error GoTo Error
      End If
      'Call AddTypes, appending to the previous collection
      Set ImplementedInterfaces = _
       TLInf.AddTypes(TypeInfos, ImplementedInterfaces, , False)
    End If
DoNext:
  Next
  Exit Function
Error:
  Resume DoNext
End Function

Public Function GetMissing(Optional ByVal DontPass As Variant) As Variant
  GetMissing = DontPass
End Function
Public Function BestClassInfo(ByVal object As Object) As TypeInfo
Dim dll, thistype, classname
On Error Resume Next
  Set BestClassInfo = TLI.ClassInfoFromObject(object)
  If Err Then
        Err.Clear
        thistype = TypeName(object)
        For Each dll In DLLFiles
            For Each classname In dll.CoClasses
                If classname.Name = thistype Or "I" & classname.Name = thistype Then Set BestClassInfo = classname: 'Stop: exit Function
                
            Next
        Next
       ' Stop
    End If
  On Error GoTo NotAvailable
  With BestClassInfo.Parent
    With TLI.TypeLibInfoFromRegistry _
        (.Guid, .MajorVersion, .MinorVersion, .LCID)
      Set BestClassInfo = .Me.TypeInfos.IndexedItem(BestClassInfo.TypeInfoNumber)
    End With
  End With
  Exit Function
NotAvailable:
  Err.Clear
  'Stop
End Function

Public Function tli_from_anything(ByRef sObject) 'As InterfaceInfo
Dim tli_file As TLI.TypeLibInfo, tempObj
On Error Resume Next
Stop

If IsObject(sObject) Then Set tli_from_anything = TLI.InterfaceInfoFromObject(sObject): Exit Function
Err.Clear
For Each tli_file In DLLFiles
    If tli_file.GetTypesWithpublic subString(sObject).Count > 1 Then Set tli_from_anything = tli_file: Exit Function
Next
Set tempObj = CreateObject(sObject)
If tempObj Is Nothing Then Stop
Set tli_from_anything = tli_from_anything(tempObj)
'Stop
End Function
Public Function typeinfo_int(TypeInfo)
If TypeInfo Is Nothing Then typeinfo_int = 0: Exit Function
typeinfo_int = java_vbscript_engine.Eval(TypeInfo)
End Function


Public Function coclass_array(tempObj, Optional Cascade = 2)
Dim thememberinfo As MemberInfo 'object_defualt_members
Dim temparray(), temppropmap As PropertyMap, i As Long
Dim themembername, theDataType, thememberprologname, theprologname, item
    
    ReDim temparray0(tempObj.Count - 1): i = 0
    ReDim temparray1(tempObj.Count - 1)
    ReDim temparray2(tempObj.Count - 1)
    ReDim temparray3(tempObj.Count - 1)
    For Each thememberinfo In tempObj
        If Left(thememberinfo.Name, 1) = "_" Then GoTo Havit:
        If InStr("[startithere,QueryInterface,AddRef,Release,GetTypeInfoCount,GetTypeInfo,GetIDsOfNames,Terminate,RefCount,CLSID,IID,Invoke,endithere]", "," & thememberinfo.Name & ",") Then GoTo Havit:
        
        If i > 0 Then
            If temparray0(i - 1) = thememberinfo.Name Then
                temparray2(i - 1) = temparray2(i - 1) Or thememberinfo.InvokeKind
                temparray3(i - 1) = thememberinfo.Name & "(" & LCase(vartypeinfo_pl(thememberinfo.ReturnType, Cascade - 1)) & "(" & invokekinds_pl_io(temparray2(i - 1)) & "" & thememberinfo.Name & ")"
                GoTo Havit:
            End If
        End If
        temparray0(i) = thememberinfo.Name
        temparray1(i) = thememberinfo.ReturnType.VarType
        temparray2(i) = thememberinfo.InvokeKind
        temparray3(i) = thememberinfo.Name & "(" & LCase(vartypeinfo_pl(thememberinfo.ReturnType, Cascade - 1)) & "(" & invokekinds_pl_io(temparray2(i)) & "" & thememberinfo.Name & ")"
        If thememberinfo.Parameters.Count > 0 Then
            temparray3(i) = temparray3(i) & "," & parameters_pl(thememberinfo.Parameters, Cascade) & ")"
         Else
            temparray3(i) = temparray3(i) & ")"
        End If
        
        i = i + 1
Havit:
'  Stop
    Next
    ReDim Preserve temparray0(i - 1)
    ReDim Preserve temparray1(i - 1)
    ReDim Preserve temparray2(i - 1)
    ReDim Preserve temparray3(i - 1)
    coclass_array = Array(temparray0, temparray1, temparray2, temparray3)
'    Stop
End Function

Public Function members_from_anything(sObject) As Members
    obj_pl_pred members_from_anything, tli_from_anything(sObject).Members
End Function


'''Collections

Public Function typeinfos_pl(AnyObject, Optional Cascade = 2)
Dim itemObject, Serialize
    If AnyObject.Count = 0 Then typeinfos_pl = "typeinfos([])": Exit Function
    For Each itemObject In AnyObject
        Serialize = Serialize & "," & obj_pl(itemObject, Cascade - 1)
    Next
 typeinfos_pl = "typeinfos([" & Mid(Serialize, 2) & "])"
End Function

Public Function members_pl(AnyObject, Optional Cascade = 2)
Dim itemObject, Serialize
    If AnyObject.Count = 0 Then members_pl = "members([])": Exit Function
    For Each itemObject In AnyObject
        Serialize = Serialize & "," & obj_pl(itemObject, Cascade - 1)
    Next
 members_pl = "members([" & Mid(Serialize, 2) & "])"
End Function

Public Function parameters_pl(AnyObject, Optional Cascade = 2)
Dim itemObject, Serialize
    If AnyObject.Count = 0 Then parameters_pl = "": Exit Function
    For Each itemObject In AnyObject
        Serialize = Serialize & "," & parameterinfo_pl(itemObject, Cascade - 1)
    Next
 parameters_pl = Mid(Serialize, 2)
End Function

Public Function interfaces_pl(AnyObject, Optional Cascade = 2)
Dim itemObject, Serialize
    If AnyObject.Count = 0 Then interfaces_pl = "interfaces([])": Exit Function
    For Each itemObject In AnyObject
        Serialize = Serialize & "," & obj_pl(itemObject, Cascade - 1)
    Next
 interfaces_pl = "interfaces([" & Mid(Serialize, 2) & "])"
End Function

Public Function customdata_pl(AnyObject, Optional Cascade = 2)
Dim itemObject, Serialize
    If AnyObject = Null Then customdata_pl = "customdata([])": Exit Function
    For Each itemObject In AnyObject
        Serialize = Serialize & "," & obj_pl(itemObject, Cascade - 1)
    Next
 customdata_pl = "customdata([" & Mid(Serialize, 2) & "])"
End Function

''' Foundation Objects


Public Function memberinfo_pl(ByVal AnyObject As MemberInfo, Optional Cascade = 2)
Dim Serialize
    memberinfo_pl = "memberinfo(" & _
        obj_pl(AnyObject.Name, Cascade - 1) & "," & _
        invokekinds_pl(AnyObject.InvokeKind, Cascade - 1) & "," & _
        obj_pl(AnyObject.ReturnType, Cascade - 1) & "," & _
        parameters_pl(AnyObject.Parameters, Cascade - 1) & _
    ")"
'Stop
End Function


Public Function parameterinfo_pl(ByVal AnyObject As ParameterInfo, Optional Cascade = 2)
Dim Serialize
    parameterinfo_pl = _
        LCase(vartypeinfo_pl(AnyObject.VarTypeInfo, Cascade - 1)) & "(" & _
        VBA.IIf(AnyObject.Optional, "?", "+") & _
        AnyObject.Name & _
    ")"
'Stop
End Function


Public Function typelibinfo_pl(ByVal AnyObject As TypeLibInfo, Optional Cascade = 2)
Dim Serialize
    typelibinfo_pl = "typelibinfo(" & _
        AnyObject.Name & "," & _
        typeinfos_pl(AnyObject.TypeInfos, Cascade - 1) & "," & _
        interfaces_pl(AnyObject.Interfaces, Cascade - 1) & "," & _
        obj_pl(AnyObject.Records, Cascade - 1) & _
    ")"
'Stop
End Function

Public Function typeinfo_pl(ByVal AnyObject As TypeInfo, Optional Cascade = 2)
Dim Serialize
    If Cascade < 0 Then typeinfo_pl = "typeinfo(_)"
    If AnyObject Is Nothing Then typeinfo_pl = "typeinfo(nothing)": Exit Function
    typeinfo_pl = "type_info(" & _
        AnyObject.Name & "," & _
        members_pl(AnyObject.Members, Cascade - 1) & "," & _
        typekinds_pl(AnyObject.TypeKind, Cascade - 1) & _
    ")"
'Stop
End Function

Public Function vartypeinfo_pl(ByVal AnyObject As VarTypeInfo, Optional Cascade = 2)
Dim Serialize
    If Cascade < 0 Then vartypeinfo_pl = "" '"vartypeinfo(_)"
    If AnyObject Is Nothing Then vartypeinfo_pl = "vartypeinfo(nothing)": Exit Function
    vartypeinfo_pl = vartype_pl(AnyObject.VarType, Cascade - 1)
'Stop
End Function

''Special types

Public Function interfaceinfo_pl(ByVal AnyObject As InterfaceInfo, Optional Cascade = 2)
Dim Serialize
    If AnyObject Is Nothing Then interfaceinfo_pl = "interfaceinfo(nothing)": Exit Function

    interfaceinfo_pl = "interfaceinfo(" & _
        obj_pl(AnyObject.Name, Cascade - 1) & "," & _
        members_pl(AnyObject.Members, Cascade - 1) & "," & _
        typekinds_pl(AnyObject.TypeKind, Cascade - 1) & _
    ")"

'        obj_pl(AnyObject.ResolvedType, Cascade - 1) & "," &

'Stop
End Function

''Enumerations


Public Function vartype_pl(ByVal AnyObject As VariantTypeConstants, Optional Cascade)
vartype_pl = "VT_VARIANT"
 Select Case AnyObject
    Case VT_EMPTY:
        vartype_pl = "VT_EMPTY"
    Case VT_NULL:
        vartype_pl = "VT_NULL"
    Case VT_I2:
        vartype_pl = "VT_I2"
    Case VT_I4:
        vartype_pl = "VT_I4"
    Case VT_I8:
        vartype_pl = "VT_I8"
    Case VT_UI2:
        vartype_pl = "VT_UI2"
    Case VT_UI4:
        vartype_pl = "VT_UI4"
    Case VT_UI8:
        vartype_pl = "VT_UI8"
    Case VT_R4:
        vartype_pl = "VT_R4"
    Case VT_R8:
        vartype_pl = "VT_R8"
    Case VT_CY:
        vartype_pl = "VT_CY"
    Case VT_DATE:
        vartype_pl = "VT_DATE"
    Case VT_BSTR:
        vartype_pl = "VT_BSTR"
    Case VT_ERROR:
        vartype_pl = "VT_ERROR"
    Case VT_BOOL:
        vartype_pl = "VT_BOOL"
    Case VT_VARIANT:
        vartype_pl = "VT_VARIANT"
    Case VT_DECIMAL:
        vartype_pl = "VT_DECIMAL"
    Case VT_I1:
        vartype_pl = "VT_I1"
    Case VT_UI1:
        vartype_pl = "VT_UI1"
    Case VT_INT:
        vartype_pl = "VT_INT"
    Case VT_UINT:
        vartype_pl = "VT_UINT"
    Case VT_VOID:
        vartype_pl = "VT_VOID"
    Case VT_SAFEARRAY:
        vartype_pl = "VT_SAFEARRAY"
    Case VT_USERDEFINED:
        vartype_pl = "VT_USERDEFINED"
    Case VT_LPSTR:
        vartype_pl = "VT_LPSTR"
    Case VT_LPWSTR:
        vartype_pl = "VT_LPWSTR"
    Case VT_RECORD:
        vartype_pl = "VT_RECORD"
    Case VT_FILETIME:
        vartype_pl = "VT_FILETIME"
    Case VT_BLOB:
        vartype_pl = "VT_BLOB"
    Case VT_STREAM:
        vartype_pl = "VT_STREAM"
    Case VT_STORAGE:
        vartype_pl = "VT_STORAGE"
    Case VT_STREAMED_OBJECT:
        vartype_pl = "VT_STREAMED_OBJECT"
    Case VT_STORED_OBJECT:
        vartype_pl = "VT_BLOB_OBJECT"
    Case VT_CF:
        vartype_pl = "VT_CF"
    Case VT_CLSID:
        vartype_pl = "VT_CLSID"
 End Select
End Function

Public Function invokekinds_pl(ByVal AnyObject As TLI.InvokeKinds, Optional Cascade = 2)
  Select Case AnyObject
    Case INVOKE_EVENTFUNC
      invokekinds_pl = "INVOKE_EVENTFUNC" '"Event"
    Case INVOKE_FUNC
      invokekinds_pl = "INVOKE_FUNC" '"Method"
    Case INVOKE_PROPERTYGET
      invokekinds_pl = "INVOKE_PROPERTYGET" '"Property Get"
    Case INVOKE_PROPERTYPUT
      invokekinds_pl = "INVOKE_PROPERTYPUT" '"Property Let"
    Case INVOKE_PROPERTYPUTREF
      invokekinds_pl = "INVOKE_PROPERTYPUTREF" '"Property Set"
    Case INVOKE_PROPERTYPUT Or INVOKE_PROPERTYGET
      invokekinds_pl = "INVOKE_PROPERTYPUT_PROPERTYGET" '"Property Get/Let"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYGET
      invokekinds_pl = "INVOKE_PROPERTYPUTREF_PROPERTYGET" '"Property Get/Set"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
      invokekinds_pl = "INVOKE_PROPERTYPUTREF_PROPERTYPUT" '"Property Let/Set"
    Case INVOKE_PROPERTYGET Or INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
      invokekinds_pl = "INVOKE_PROPERTYGET_PROPERTYPUTREF_PROPERTYPUT" '"Property Get/Let/Set"
  End Select
  If AnyObject And INVOKE_CONST Then invokekinds_pl = invokekinds_pl & "_INVOKE_CONST"
End Function

Public Function invokekinds_pl_io(ByVal AnyObject As TLI.InvokeKinds, Optional Cascade = 2)
  Select Case AnyObject
    Case INVOKE_EVENTFUNC
      invokekinds_pl_io = "-" '"Event"
    Case INVOKE_FUNC
     invokekinds_pl_io = "-" '"Method"
    Case INVOKE_PROPERTYGET
      invokekinds_pl_io = "-" '"Property Get"
    Case INVOKE_PROPERTYPUT
      invokekinds_pl_io = "+" '"Property Let"
    Case INVOKE_PROPERTYPUTREF
      invokekinds_pl_io = "?" '"Property Set"
    Case INVOKE_PROPERTYPUT Or INVOKE_PROPERTYGET
      invokekinds_pl_io = "?" '"Property Get/Let"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYGET
      invokekinds_pl_io = "?" '"Property Get/Set"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
      invokekinds_pl_io = "?" '"Property Let/Set"
    Case INVOKE_PROPERTYGET Or INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
      invokekinds_pl_io = "?" '"Property Get/Let/Set"
  End Select
  'If AnyObject And INVOKE_CONST Then invokekinds_pl_io = invokekinds_pl_io & "_INVOKE_CONST"
End Function

Public Function typekinds_pl(ByVal AnyObject As TypeKinds, Optional Cascade = 2)
  Select Case AnyObject
    Case TKIND_ENUM
        typekinds_pl = "TKIND_ENUM"
    Case TKIND_RECORD
        typekinds_pl = "TKIND_RECORD"
    Case TKIND_MODULE
        typekinds_pl = "TKIND_MODULE"
    Case TKIND_INTERFACE
        typekinds_pl = "TKIND_INTERFACE"
    Case TKIND_DISPATCH
        typekinds_pl = "TKIND_DISPATCH"
    Case TKIND_ALIAS
        typekinds_pl = "TKIND_ALIAS"
    Case TKIND_UNION
        typekinds_pl = "TKIND_UNION"
    Case TKIND_MAX
        typekinds_pl = "TKIND_MAX"
 End Select
End Function


Public Function obj_pl(ByRef sObject, Optional Cascade = 2)
    obj_pl_pred obj_pl, sObject, Cascade
End Function

Public Function obj_pl2(ParamArray sObject())
 Dim i
    Select Case UBound(sObject)
        Case -1
            obj_pl2 = ""
        Case 0
            obj_pl_pred obj_pl2, sObject(0), 1
        Case Else
            ReDim temparray(UBound(sObject))
            For i = 0 To UBound(sObject)
                obj_pl_pred temparray(i), sObject(i), 1
            Next
            obj_pl2 = Join(temparray, ",")
    End Select
End Function



Public Sub obj_pl_pred(ByRef obj_pl, ByRef AnyObject, Optional Cascade = 2)
Dim temparray() As String, objtype As String, thisobjii, i As Long, thevalue, proxyAnyObject As Variant
Dim vt_temp As VariantTypeConstants, tarry(), tarry0(), memberscount, instance_props, themembername As String
Dim vtkind As String
If Cascade < 0 Then obj_pl = LCase(TypeName(AnyObject)) & "(_)": Exit Sub

If IsArray(AnyObject) Then
    If AnyObject.Count = 0 Then
        obj_pl = "array([])"
    Else
        ReDim temparray(AnyObject.Count - 1)
        For i = 0 To AnyObject.Count - 1
             obj_pl_pred temparray(i), AnyObject(i)
        Next i
        obj_pl = "array([" & Join(temparray, ",") & "])"
    End If
    Exit Sub
End If
'Stop
objtype = LCase(TypeName(AnyObject))
vtkind = LCase(vartype_pl(VarType(AnyObject)))
Select Case objtype
    Case "string"
            obj_pl = vtkind & "(" & parse_vbstr_plstr(AnyObject) & ")"
    Case "integer", "long", "short", "int", "double", "single", "number", "byte", "color"
            obj_pl = vtkind & "(" & AnyObject & ")"
    Case "nothing", "null", "empty"
            obj_pl = vtkind & "(" & objtype & ")"
    Case "boolean", "bool"
            obj_pl = vtkind & "(" & IIf(AnyObject, "true", "false") & ")"
    Case "ipropertymap", "propertymap"
            If AnyObject.Count = 0 Then obj_pl = objtype & "([])": Exit Sub
            ReDim temparray(AnyObject.Count - 1): i = 0
            For Each thisobjii In AnyObject
                obj_pl_pred thevalue, AnyObject.Property(thisobjii)
                temparray(i) = thisobjii & "=" & thevalue
                i = i + 1
            Next
            obj_pl = objtype & "([" & Join(temparray, ",") & "])"
    Case "ipropertylist", "propertylist"
            If AnyObject.Count = 0 Then obj_pl = objtype & "([])": Exit Sub
            ReDim temparray(AnyObject.Count - 1): i = 0
                For i = 0 To AnyObject.Count - 1
                     obj_pl_pred temparray(i), AnyObject.Property(i)
                Next i
            obj_pl = objtype & "([" & Join(temparray, ",") & "])"
    Case "thing", "ithing"
            'obj_pl = "ithing(" & getproperties(AnyObject, "type,id,name") & ")"
    Case "collection"
            If AnyObject.Count = 0 Then obj_pl = objtype & "([])": Exit Sub
            ReDim temparray(AnyObject.Count - 1): i = 0
            For Each thisobjii In AnyObject
                obj_pl_pred temparray(i), thisobjii
                i = i + 1
            Next
            obj_pl = objtype & "([" & Join(temparray, ",") & "])"
    Case "ivector", "vector"
            obj_pl = "ivector" & argument_opener & AnyObject.Rotation & "," & AnyObject.X & "," & AnyObject.Y & "," & AnyObject.Z & argument_terminator
    'Case "iworld", "world"
     '       obj_pl = object_opener & "$" & LCase(AnyObject.name) & object_terminator
    Case "ivwgeometrysprite", "vwspritedgeometry"
            obj_pl = "geometrysprite([" & obj_pl2(AnyObject.AccessoryScene, AnyObject.CurrentGesture, AnyObject.Frame, AnyObject.GeometryFrame, AnyObject.GeometryName, AnyObject.ImageFileName, AnyObject.RotationAxis) & "])" '                           thisname = "geometrysprite([accessoryscene=" & obj_pl(AnyObject.accessoryscene) & ",currentgesture=" & obj_pl(AnyObject.currentgesture) & ", frame=" & obj_pl(AnyObject.frame) & ",geometryframe=" & obj_pl(AnyObject.geometryframe) & " ,geometryname= " & obj_pl(AnyObject.geometryname) & " , imagefilename=" & obj_pl(AnyObject.imagefilename) & ",rotationaxis=" & AnyObject.rotationaxis & "])"
    Case "ivwgeometry", "vwgeometry"
            obj_pl = "geometry([" & obj_pl2(AnyObject.Frame, AnyObject.GeometryFrame, AnyObject.GeometryName) & "])" '                            thisname = "geometry([ frame=" & obj_pl(AnyObject.frame) & ",geometryframe=" & obj_pl(AnyObject.geometryframe) & " ,geometryname= " & obj_pl(AnyObject.geometryname) & "])"
    Case "isound", "sound"
            obj_pl = "sound([" & obj_pl2(AnyObject.Is3D, AnyObject.IsLooping, AnyObject.SoundURL) & "])" '                            thisname = "sound([ is3d=" & obj_pl(AnyObject.is3d) & ",islooping=" & obj_pl(AnyObject.islooping) & " ,soundurl= " & obj_pl(AnyObject.soundurl) & "])"
    Case "iboundary", "boundary"
            obj_pl = "boundry([" & obj_pl2(AnyObject.HeightLower, AnyObject.HeightUpper, AnyObject.IsAutoSolidBoundary, AnyObject.IsClosed, AnyObject.IsInverse, AnyObject.IsPassable, AnyObject.IsProximity, AnyObject.VertexCount) & "])" '                            thisname = "sound([ is3d=" & obj_pl(AnyObject.is3d) & ",islooping=" & obj_pl(AnyObject.islooping) & " ,soundurl= " & obj_pl(AnyObject.soundurl) & "])"
    Case "imethod", "method"
            obj_pl = "method([" & obj_pl2(AnyObject.methodname, AnyObject.IsInline, AnyObject.FLAGS, AnyObject.Module) & "])"
    Case "imodule", "module"
            Stop
            obj_pl = "module([" & obj_pl2(AnyObject.methodname, AnyObject.IsInline, AnyObject.FLAGS, AnyObject.Module) & "])"
    Case "iscriptmodule", "scriptmodule"
            obj_pl = "scriptmodule([" & obj_pl2(AnyObject.ModuleName, AnyObject.ModuleType, AnyObject.IsShutdown) & "])"
    Case "icommodule", "commodule"
            obj_pl = "commodule([" & obj_pl2(AnyObject.ModuleName, AnyObject.ModuleType, AnyObject.FLAGS) & "])"
    'Case "typeinfos"
    '        obj_pl = typeinfos_pl(AnyObject, Cascade - 1)
    'Case "members"
    '        obj_pl = members_pl(AnyObject, Cascade - 1)
    'Case "parameters"
    '        obj_pl = parameters_pl(AnyObject, Cascade - 1)
    'Case "interfaces"
    '        obj_pl = interfaces_pl(AnyObject, Cascade - 1)
    'Case "customdata"
    '        obj_pl = customdata_pl(AnyObject, Cascade - 1)
    'Case "memberinfo"
    '        obj_pl = memberinfo_pl(AnyObject, Cascade - 1)
    'Case "parameterinfo"
    '        obj_pl = parameterinfo_pl(AnyObject, Cascade - 1)
    'Case "typeinfo"
    '        obj_pl = typeinfo_pl(AnyObject, Cascade - 1)
    'Case "typelibinfo"
    '        obj_pl = typelibinfo_pl(AnyObject, Cascade - 1)
    'Case "interfaceinfo"
    '        obj_pl = interfaceinfo_pl(AnyObject, Cascade - 1)
    'Case "vartype"
    '        obj_pl = vartype_pl(AnyObject, Cascade - 1)
   ' Case "invokekinds"
    '        obj_pl = invokekinds_pl(AnyObject, Cascade - 1)
    'Case "typekinds"
    '        obj_pl = typekinds_pl(AnyObject, Cascade - 1)
    'Case "coclassinfo"
    '        obj_pl = coclassinfo_pl(AnyObject, Cascade - 1)
    'Case "vartypeinfo"
    '        obj_pl = vartypeinfo_pl(AnyObject, Cascade - 1)
    Case Else
     '   Stop
            Set thisobjii = BestClassInfo(AnyObject)
            If thisobjii Is Nothing Then::
            Select Case TypeName(thisobjii)
                Case "Nothing"
                    
            obj_pl = "object(nothing)"
                Case "CoClassInfo"
                    tarry = coclass_array(thisobjii.DefaultInterface.Members, Cascade - 1)
            obj_pl = "object(" & objtype & ",[" & Join(tarry(3), ",") & "])"
                Case Else
                    tarry = coclass_array(thisobjii.Members, Cascade - 1)
            obj_pl = "object(" & objtype & ",[" & Join(tarry(3), ",") & "])"
            End Select
            instance_props = ""
            
            For i = 0 To UBound(tarry, 1)
   
                If (tarry(2)(i) And INVOKE_PROPERTYGET) > 0 Then
                 Dim thevalue2
                 'java_vbscript_engine.ExecuteStatement "Dim foofoo "
                 'java_vbscript_engine.AddObject "foofoo", AnyObject, True
                 getrecord thevalue2, AnyObject, tarry(0)(i)
                 obj_pl_pred thevalue, TypeName(thevalue2)
                 instance_props = instance_props & "," & themembername & "=" & thevalue

                End If
            Next
            obj_pl = obj_pl & "inst_object([" & Mid$(instance_props, 2) & "]," & obj_pl & ")"
    
            
 End Select
End Sub

Public Function coclassinfo_pl(ByVal AnyObject As CoClassInfo, Optional Cascade = 2)
Dim Serialize
    coclassinfo_pl = "coclassinfo(" & _
        obj_pl(AnyObject.Name, Cascade - 1) & "=" & _
        obj_pl(AnyObject.Interfaces, Cascade - 1) & "," & _
    ")"
'Stop
End Function


Public Sub assign(ByRef theAnything, ByVal ToBecome)
If IsObject(ToBecome) Then Set theAnything = ToBecome Else theAnything = ToBecome
End Sub

Public Sub getrecord(ByRef result, ByRef Obj, ByVal themem As String)
Dim realObject, Name As String
'Stop
resolve_object realObject, Name, Obj
On Error Resume Next
assign result, CallByName(realObject, themem, VbGet)
If Err Then result = "error(" & vbstr_plstr("morphology getrecord " & Err.Description) & ")"
'Stop
End Sub

Public Sub resolve_object(ByRef realObject, ByRef Name, ByVal Obj)

    If IsObject(Obj) Then
        If TypeName(Obj) = "Nothing" Then
            'object nothing
            If Name <> "" Then
                ''' have name and no object
            Else
                ''' no name and no object (we're not happy)
            End If
        Else
            If Name <> "" Then
                ' have name and object
                Set realObject = Obj
                If theInstances.IsValid(Name) Then
                        'name found in dictionary
                        If theInstances.Property(Name) Is Obj Then
                            'All matched up
                            Set realObject = Obj
                            Exit Sub
                        Else
                            'they are trying to rename the object
                            If theInstances.Property(CStr(ObjPtr(Obj))) Is Obj Then
                                '''we have the object but they are giving it a new name
                            Else
                                '''let search for the damn object
                                '''first lets try a quick prolog resolve
                                '''if this fails then we'll for each theInstances collection
                            End If
                        End If
                    Else
                        '''name not found in dictionary
                End If
            Else
                '''have unnamed object
            End If
        End If
    Else
        ''' Not an object
        Select Case TypeName(Obj)
            Case "String", "Integer", "Long"
            ''' this is a reference or a constant
                If theInstances.IsValid(Obj) Then
                    ''' this is a reference
                    If Name = Obj Then
                        'they supplied the correct name lets give it to them
                        Set realObject = theinstance.Property(Name): Exit Sub
                    Else
                        '''name is different then found object
                        If Name <> "" Then
                            Set realObject = theInstances.Property(Obj)
                            '''lets check to see if the name points to the same object
                            If theInstances.Property(Name) Is realObject Then
                                'they supplied a correct alias
                                Exit Sub
                            Else
                                '''their alias did not match
                                If TypeName(realObject) = Name Then
                                    'they supplied a class as a name
                                    Exit Sub
                                Else
                                    
                                
                            End If
                        Else
                            'name was null set it and give it
                            Name = Obj
                            Set realObject = theinstance.Property(Name): Exit Sub
                        End If
                    End If
                    
                        Set realObject = theInstances.Property(Obj)
                        GoTo HaveRealObject
                Else
                    ''' obj-string is not in dictionary
                    
                    
                End If
            Case Else
                Stop
                
                If theInstances.IsValid(ObjPtr(Obj)) Then Set realObject = theInstances.Property(ObjPtr(Obj)):: GoTo HaveRealObject:
                For Each Name In theInstances
                    If theInstances.Property(Name) Is Obj Then
                        If Name <> "" Then Set realObject = Obj: Exit Sub
                    End If
                Next
        End Select
    End If
If Name <> "" Then resolve_object realObject, "", Name

HaveRealObject:
import_obj_to_instances realObject
Name = "cccccccccc"
   On Error Resume Next
   Name = Obj.Name
End Sub


Public Sub set_name_obj(ObjName, TheObject)
    
    If Not IsObject(TheObject) Then
        pl_obj_pred NewObject, TheObject
        assign TheObject, NewObject
        If IsObject(TheObject) Then GoTo HaveObject:
        
        ''looks like we have a fixed DataType
        
        


HaveObject:
    If IsNumeric(ObjName) Then
        ObjName = CStr(ObjName)
        If Val(ObjName) = ObjPtr(TheObject) Then
            'They supplied ptr (we must have given it)
            assign theInstances.Property(ObjName), TheObject
            Exit Sub
        Else
            '''They have a bad ptr (lets find out why)
            If theInstances.IsValid(ObjName) Then
                '''their ptr was alive
                ''go on to friendly creation
            Else
                '''their ptr was dead and not in the instances
                If theInstances.Property(ObjPtr(TheObject)) Then
                    '''their object did exist in theInstances lets create a new ptr
                    assign theInstances.Property(ObjName), TheObject 'but go onto friendly creation
                Else
                    'thier object is not in theInstances and has a bad ptr
                    'so treat it as a friendly name
                End If
            End If
        End If
    End If
    '''they supplied friendly name
    On Error Resume Next
    ptr = CStr(ObjPtr(TheObject))
    objtype = LCase(TypeName(TheObject))
    objclass = LCase(TheObject.Type)
    If objclass = "" Then objclass = objtype
    ObjName = LCase(vbstr_plstr(ObjName))
    
    theSWIPrologShell.Execute "assert_new(name_ptr(" & ObjName & ",(" & ptr & ")))"
    theSWIPrologShell.Execute "assert_new(type_ptr(" & objtype & ",(" & ptr & ")))"
    theSWIPrologShell.Execute "assert_new(class_ptr(" & objclass & ",(" & ptr & ")))"
    theSWIPrologShell.Execute "assert_new(name_type(" & ObjName & "," & objtype & "))"
    theSWIPrologShell.Execute "assert_new(class_type(" & objclass & "," & objtype & "))"
    theSWIPrologShell.Execute "assert_new(name_class(" & ObjName & "," & objclass & "))"

    assign theInstances.Property(ptr), TheObject
            
End Sub



Public Sub nn()


    If ObjName = ObjPtr(TheObject) Then
        Set theInstances.Property(ObjName) = TheObject
        Exit Sub
    Else
        theSWIPrologShell.Execute ("assert(objptr_name(" & vbstr_plstr & ",(" & ObjPtr(TheObject) & ")))")
        theSWIPrologShell.Execute ("assert(objptr_typename(" & vbstr_plstr & ",(" & ObjPtr(TheObject) & ")))")
            End Select
        Case Else
    End Select

                
            
        
        
    Select Case TypeName(ObjName)
    
    
    If IsObject(ObjName) Then Set Obj = ObjName: Exit Sub
    
    For Each NameValuePair In theInstances
    
    

End Sub


Public Sub import_obj_to_instances(AnyObject)
Dim objtypename, objloc
 objtypename = LCase(TypeName(AnyObject))
 Select Case objtypename
    Case "empty", "nothing"
        Exit Sub
    Case "string"
        Stop
    Case Else
        objloc = CStr(ObjPtr(AnyObject))
        If Not theInstances.IsValid(objloc) Then
            theInstances.Property(objloc) = AnyObject
            If Len(theSWIPrologShell.Execute("typename(" & objtypename & "),post(h)")) Then
                    theSWIPrologShell.Execute "assert(typename(" & objtypename & ")),post(h)"
                    'serialize the class
            End If
            theSWIPrologShell.Execute "assert(object_ptr(" & objtypename & "," & objloc & "))."
            'serialize the instance
        End If
 End Select
End Sub

Public Sub call_by_name(theresult, TheObject, thecall As String, Optional theArgs = "ccccccc")
Stop
If theArgs = "ccccccc" Then assign theresult, CallByName(TheObject, thecall, vb_call_method(BestClassInfo(TheObject).DefaultInterface.GetMember(thecall).InvokeKind)): Exit Sub
assign theresult, CallByName(TheObject, thecall, vb_call_method(BestClassInfo(TheObject).DefaultInterface.GetMember(thecall).InvokeKind), theArgs)
End Sub

Public Function vb_call_method(AnyObject As TLI.InvokeKinds) As VbCallType
vb_call_method = VbCallType.VbGet
  Select Case AnyObject
    Case INVOKE_EVENTFUNC
      vb_call_method = VbCallType.VbMethod
    Case INVOKE_FUNC
      vb_call_method = VbCallType.VbMethod '"Method"
    Case INVOKE_PROPERTYGET
      vb_call_method = VbCallType.VbMethod '"Property Get"
    Case INVOKE_PROPERTYPUT
      vb_call_method = VbCallType.VbLet '"Property Let"
    Case INVOKE_PROPERTYPUTREF
      vb_call_method = VbCallType.VbSet '"Property Set"
    Case INVOKE_PROPERTYPUT Or INVOKE_PROPERTYGET
      vb_call_method = VbCallType.VbGet '"Property Get/Let"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYGET
      vb_call_method = VbCallType.VbSet '"Property Get/Set"
    Case INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
  '    invokekinds_pl = "INVOKE_PROPERTYPUTREF_PROPERTYPUT" '"Property Let/Set"
    Case INVOKE_PROPERTYGET Or INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
   '   invokekinds_pl = "INVOKE_PROPERTYGET_PROPERTYPUTREF_PROPERTYPUT" '"Property Get/Let/Set"
  End Select
End Function

Public Function getproperties(sObject, ByVal PropertyListString)
 Dim PropertyArray(), answer
 Dim objloc As Long, objtypename As String, i As Long
 Dim AnyObject
' Stop

pl_split_pred PropertyArray, pl_trim(PropertyListString), ","
resolve_object AnyObject, "", sObject
 objtypename = LCase(TypeName(AnyObject))
 objloc = ObjPtr(AnyObject)
 
    For i = 0 To UBound(PropertyArray)
        'Try Prolog First
        answer = theSWIPrologShell.Execute("object_member_get_prolog_version(" & objtypename & "," & PropertyArray(i) & ",X)),post(X)")
        If answer = "" Then
            'Try VW Instance Next
            On Error Resume Next
            assign answer, AnyObject.Property(PropertyArray(i))
            If Not is_good(answer) Then
                'The Real Object
                getrecord answer, AnyObject, PropertyArray(i)
            End If
        End If
        If is_good(answer) Then obj_pl_pred PropertyArray(i), answer
    Next i
getproperties = Join(PropertyArray, ",")
End Function
    
Public Function is_good(ByRef sObject) As Boolean
    If TypeName(sObject) = "Nothing" Then is_good = False: Exit Function
    If IsObject(sObject) Then is_good = True: Exit Function
    If sObject = "" Then is_good = False: Exit Function
    is_good = True
End Function





