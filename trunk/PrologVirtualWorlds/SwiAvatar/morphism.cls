VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "COMMorphism"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private m_fCheckedTLIArrayBounds As Boolean
Private m_fFixArrayBounds As Boolean

Public Function create_datum(this) As a_Datum
    If theDatums.IsValid(CStr(ObjPtr(this))) Then Set create_datum = theDatums.Property(CStr(ObjPtr(this))): Exit Function
    Set create_datum = New a_Datum
    theDatums.Property(CStr(ObjPtr(this))) = create_datum
    Set create_datum.Value = this
End Function

Public Function value_to_string(Value) As String
    value_to_string = parse_vbstr_pl_string(Value)
End Function

Public Function datum_to_prolog(ByVal theDatum As a_Datum, Optional ByVal Level = 2) As String
 Dim modelstr As String, model, tdm As a_Datum
    datum_to_prolog = nodekind_enum_string(theDatum.nodekind)
    If Level < 1 Then datum_to_prolog = datum_to_prolog & "(_)": Exit Function
'    Stop
    Select Case theDatum.SiteKind
    
        Case SiteKinds.SITEKIND_ACTX
            Select Case theDatum.MODELKIND
            
                Case MODELKINDs.MODELKIND_VALUE
                    ' value(type,Value)
                    datum_to_prolog = "value(" & theDatum.typeName & "," & value_to_string(theDatum.Value) & ")"
                Case MODELKINDs.MODELKIND_COLLECTION
                    ' collection(type,[ref],[members],site)
                    datum_to_prolog = "collection(" & theDatum.typeName & ",[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "])"
                Case MODELKINDs.MODELKIND_object
                    'object(class,type,[ref],[members],site)
                    datum_to_prolog = "object([" & theDatum.typeName & "],[" & theDatum.typeName & "],[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],actx)"
                Case MODELKINDs.MODELKIND_ARRAY
                    'array([ref],[members],site)
                    datum_to_prolog = "array([" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],actx)"
                Case MODELKINDs.MODELKIND_FUNCDESC
                    'funcdesc(class,type,[ref],[members],site)
                    datum_to_prolog = "funcdesc([" & theDatum.typeName & "],[" & theDatum.typeName & "],[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],actx)"
               End Select
        Case SiteKinds.SITEKIND_instance
            Select Case theDatum.MODELKIND
                Case MODELKINDs.MODELKIND_VALUE
                    ' value(class,type,Value,site)
                    datum_to_prolog = "value([" & theDatum.typeName & "],[" & theDatum.typeName & "]," & value_to_string(theDatum.Value) & ",morphism)"
                Case MODELKINDs.MODELKIND_COLLECTION
                    'collection(class,type,[ref],[members],site)
                    datum_to_prolog = "collection([" & theDatum.typeName & "],[" & theDatum.typeName & "],[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],morphism)"
                Case MODELKINDs.MODELKIND_object
                    'object(class,type,[ref],[members],site)
                    datum_to_prolog = "object([" & theDatum.typeName & "],[" & theDatum.typeName & "],[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],morphism)"
                Case MODELKINDs.MODELKIND_ARRAY
                    'array([ref],[members],site)
                    datum_to_prolog = "array([" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],morphism)"
                Case MODELKINDs.MODELKIND_FUNCDESC
                    'funcdesc(class,type,[ref],[members],site)
                    datum_to_prolog = "funcdesc([" & theDatum.typeName & "],[" & theDatum.typeName & "],[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],morphism)"
               End Select
        Case SiteKinds.SITEKIND_VWSERVER
            Select Case theDatum.MODELKIND
                Case MODELKINDs.MODELKIND_VALUE
                    ' value(class,type,Value,site)
                    datum_to_prolog = "value([" & theDatum.typeName & "],[" & theDatum.typeName & "]," & value_to_string(theDatum.Value) & ",vwserver)"
                Case MODELKINDs.MODELKIND_COLLECTION
                    'collection(class,type,[ref],[members],site)
                    datum_to_prolog = "collection([" & theDatum.typeName & "],[" & theDatum.typeName & "],[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],vwserver)"
                Case MODELKINDs.MODELKIND_object
                    'object(class,type,[ref],[members],site)
                    datum_to_prolog = "object([" & theDatum.typeName & "],[" & theDatum.typeName & "],[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],vwserver)"
                Case MODELKINDs.MODELKIND_ARRAY
                    'array([ref],[members],site)
                    datum_to_prolog = "array([" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],vwserver)"
                Case MODELKINDs.MODELKIND_FUNCDESC
                    'funcdesc(class,type,[ref],[members],site)
                    datum_to_prolog = "funcdesc([" & theDatum.typeName & "],[" & theDatum.typeName & "],[" & get_reference(theDatum.Origin) & "," & get_reference(theDatum) & "],["
                    For Each model In theDatum.Models
                        Set tdm = theDatum.Models.Property(model)
                        modelstr = modelstr & "," & datum_to_prolog(tdm, Level - 1)
                    Next
                    modelstr = Mid$(modelstr, 2)
                    datum_to_prolog = datum_to_prolog & modelstr & "],vwserver)"
               End Select
        Case Else
'            Stop
    End Select
    datum_to_prolog = nodekind_enum_string(theDatum.nodekind) & "(Name([" & theDatum.MemberName & "])," & datum_to_prolog & "," & sitekind_enum_string(theDatum.SiteKind) & ")"
End Function

Public Function create_datum_from_com(Owner, nodekind, this) As a_Datum
 Dim thisMember, thisAny, my_ref As String, i As Long, objtype As String
Stop
    my_ref = get_reference(create_datum_from_com)
Stop
    If theDatums.IsValid(my_ref) Then
        Set create_datum_from_com = theDatums.Property(my_ref)
Stop
    Else
        Set create_datum_from_com = New a_Datum
        my_ref = get_reference(create_datum_from_com)
        theDatums.Property(my_ref) = create_datum_from_com
Stop
    End If
    create_datum_from_com.nodekind = nodekind
    create_datum_from_com.SiteKind = Me.sitekind_from_object(this)
Stop
    Select Case typeName(Owner)
        Case "Nothing", "Empty", "Null"
            Set Owner = theRootDatum
        Case Else
            If sitekind_from_object(Owner) <> create_datum_from_com.SiteKind Then
                Debug.Print "cross domain errors"
            End If
    End Select
    
Stop
    objtype = LCase(VBA.typeName(this))
    Select Case objtype
        Case "string", "integer", "long", "short", "int", "double", "single", "number", "byte", "color", "currency", "date", "time"
            create_datum_from_com.MODELKIND = MODELKIND_VALUE
            create_datum_from_com.Value = this
        Case "nothing"
            create_datum_from_com.MODELKIND = MODELKIND_object
            Set create_datum_from_com.Value = this
        Case "null", "empty"
            create_datum_from_com.MODELKIND = MODELKIND_VALUE
            create_datum_from_com.Value = this
        Case "boolean", "bool"
            create_datum_from_com.MODELKIND = MODELKIND_VALUE
            create_datum_from_com.Value = this
        Case "thing", "ithing"
            Set create_datum_from_com = create_datum_from_ithing(this.Container, NODEKIND_CONTENT, this)
        Case Else
            create_datum_from_com.MODELKIND = Me.MODELKIND_from_object(this)
 Stop
            Select Case create_datum_from_com.MODELKIND
                Case MODELKINDs.MODELKIND_ARRAY
                        create_datum_from_com.Value = this
                        create_datum_from_com.MemberName = my_ref
                        Set create_datum_from_com.Origin = Owner
                        create_datum_from_com.typeName = typeName(this)
                        Set create_datum_from_com.Models = New PropertyMap
                        For i = 0 To UBound(this, 1)
                            create_datum_from_com.Models.Property(CStr(i)) = _
                                create_datum_from_com(create_datum_from_com, NODEKIND_CONTENT, thisMember)
                        Next
                Case MODELKIND_COLLECTION
                        Set create_datum_from_com.Value = this
                        create_datum_from_com.MemberName = my_ref
                        Set create_datum_from_com.Origin = Owner
                        create_datum_from_com.typeName = typeName(this)
                        Set create_datum_from_com.Models = New PropertyMap
                        For Each thisAny In get_members(this)
                            add_model_info create_datum_from_com, create_datum_from_com(this, NODEKIND_CONTENT, thisMember)
                        Next
                Case MODELKIND_object
                        Set create_datum_from_com.Value = this
Stop
                        create_datum_from_com.MemberName = my_ref
                        Set create_datum_from_com.Origin = Owner
Stop
                        create_datum_from_com.typeName = typeName(this)
                        Set create_datum_from_com.Models = New PropertyMap
                        For Each thisMember In get_members(this)
                            If (InStr(1, theHiddenMembers, thisMember.Name, vbTextCompare) = 0 And (thisMember.InvokeKind And (INVOKE_FUNC Or INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYGET Or INVOKE_EVENTFUNC))) Then
            '                    Stop
                                add_model_info create_datum_from_com, create_datum_from_memberinfo(create_datum_from_com, thisMember)
                            End If
                        Next
                        
                Case MODELKIND_VALUE
                    create_datum_from_com.Value = this
                    create_datum_from_com.Models = New PropertyMap
                    create_datum_from_com.Origin = Owner
                    create_datum_from_com.typeName = typeName(this)
                Case Else
                    Stop
            End Select
    End Select
End Function

Private Function create_datum_from_memberinfo(ByVal Origin As a_Datum, ByVal thisMember As MemberInfo) As a_Datum
 Dim mp As ParameterInfo, newDatum As a_Datum, t
            If thisMember.Name = "value" Then Exit Function
            Set create_datum_from_memberinfo = New a_Datum
            create_datum_from_memberinfo.SiteKind = SITEKIND_ACTX
'            Stop
            If Origin Is Nothing Then Stop
            Select Case thisMember.InvokeKind
            
                Case InvokeKinds.INVOKE_FUNC, InvokeKinds.INVOKE_EVENTFUNC
                    create_datum_from_memberinfo.MemberName = thisMember.Name
                    create_datum_from_memberinfo.nodekind = thisMember.InvokeKind
                    create_datum_from_memberinfo.MODELKIND = MODELKIND_FUNCDESC
                    If IsObject(Origin.Value) Then
                        Set create_datum_from_memberinfo.Origin = Origin.Value
                    Else
                        Set create_datum_from_memberinfo.Origin = Origin
                    End If
                    create_datum_from_memberinfo.typeName = vartype_enum_string(thisMember.ReturnType.VarType)
                    Set create_datum_from_memberinfo.Models = New PropertyMap
                    For Each mp In thisMember.Parameters
                        add_model_info create_datum_from_memberinfo, create_datum_from_param(create_datum_from_memberinfo, mp)
                    Next
                
                Case InvokeKinds.INVOKE_PROPERTYPUTREF, InvokeKinds.INVOKE_PROPERTYGET
                    Set create_datum_from_memberinfo = New a_Datum
                    'Set create_datum_from_memberinfo.Value = thisMember
                    create_datum_from_memberinfo.nodekind = NODEKIND_property
                    create_datum_from_memberinfo.MemberName = thisMember.Name
                    If IsObject(Origin.Value) Then
                        Set create_datum_from_memberinfo.Origin = Origin.Value
                    Else
                        Set create_datum_from_memberinfo.Origin = Origin
                    End If
                        
                    Set create_datum_from_memberinfo.Models = New PropertyMap
                    call_by_Name_pred t, Origin.Value, thisMember.Name
                    create_datum_from_memberinfo.MODELKIND = MODELKIND_from_object(t)
            End Select
End Function

Public Function create_datum_from_param(Owner As a_Datum, mp As ParameterInfo) As a_Datum
        Set create_datum_from_param = New a_Datum
On Error Resume Next
        create_datum_from_param.MemberName = mp.Name
        create_datum_from_param.nodekind = NODEKIND_PARAMETER ' me.m NODEKIND_from_object(mp.DefaultValue)
'        Stop
        If mp.Default Then create_datum_from_param.MODELKIND = MODELKIND_from_object(mp.DefaultValue)
        Set create_datum_from_param.Origin = Owner.Origin
        Set create_datum_from_param.Models = New PropertyMap
        create_datum_from_param.typeName = vartype_enum_string(mp.VarTypeInfo.typeInfo)
        assign create_datum_from_param.Value, mp.DefaultValue
'        Stop
        create_datum_from_param.SiteKind = Owner.SiteKind
End Function

Public Function get_MODELKIND_from_object(this) As MODELKINDs
  Dim m_TypeName As String
        m_TypeName = LCase(VBA.typeName(this))
    If Right$(m_TypeName, 10) = "collection" Then
        get_MODELKIND_from_object = MODELKIND_COLLECTION
        Exit Function
    End If
    If Right$(m_TypeName, 8) = "typeinfo" Then
        get_MODELKIND_from_object = MODELKIND_COLLECTION
    Exit Function
    End If
    If Right$(m_TypeName, 1) = "s" Then
'        Stop
        get_MODELKIND_from_object = MODELKIND_COLLECTION
        Exit Function
    End If
    If Right$(m_TypeName, 3) = "map" Then
        get_MODELKIND_from_object = MODELKIND_COLLECTION
        Exit Function
    End If
    If Right$(m_TypeName, 2) = "()" Then
        get_MODELKIND_from_object = MODELKIND_ARRAY
        Exit Function
    End If
    Select Case m_TypeName
        Case "empty", "null"
            get_MODELKIND_from_object = MODELKIND_VALUE
        Case "nothing"
            get_MODELKIND_from_object = MODELKIND_object
        Case "ithing", "thing"
            get_MODELKIND_from_object = MODELKIND_object
        Case Else
            If Not IsObject(this) Then
                get_MODELKIND_from_object = MODELKIND_VALUE
            Else
                get_MODELKIND_from_object = MODELKIND_object
            End If
    End Select
End Function

Public Function create_datum_from_value(Origin As a_Datum, vData As Variant) As a_Datum
        
Set create_datum_from_value = New a_Datum
        create_datum_from_value.Value = vData
        create_datum_from_value.MemberName = "value"
        create_datum_from_value.MODELKIND = Me.MODELKIND_from_object(vData)
        Set create_datum_from_value.Models = New PropertyMap
        Set create_datum_from_value.Origin = Nothing
        create_datum_from_value.typeName = vartype_enum_string(VarType(vData))
'Stop
        enum_model create_datum_from_value
End Function

Public Function create_datum_from_ithing(Owner As a_Datum, nodekind As NODEKINDs, ByVal this As Object) As a_Datum
 Dim ItemName As Variant
    If this.IsStub Then On Error Resume Next
    If theDatums.IsValid(CStr(ObjPtr(this))) Then
        Set create_datum_from_ithing = theDatums.Property(CStr(ObjPtr(this)))
        create_datum_from_ithing.Models = New PropertyMap
    Else
        Set create_datum_from_ithing = New a_Datum
        theDatums.Property(CStr(ObjPtr(this))) = create_datum_from_ithing
        create_datum_from_ithing.MemberName = this.Property("Name")
        create_datum_from_ithing.typeName = this.Type
        create_datum_from_ithing.SiteKind = SITEKIND_VWSERVER
        create_datum_from_ithing.Value = this
        create_datum_from_ithing.Origin = this.Container
        create_datum_from_ithing.nodekind = NODEKIND_CONTENT
        create_datum_from_ithing.Models = New PropertyMap
    End If
    For Each ItemName In this.Properties
        add_model_info create_datum_from_ithing, create_datum_iobject_property(this, ItemName, this.Property(ItemName))
    Next
    For Each ItemName In this.Contents
        add_model_info create_datum_from_ithing, create_datum_from_ithing(create_datum_from_ithing, NODEKIND_CONTENT, this.Contents(ItemName))
    Next
    For Each ItemName In this.Methods
        If LCase(Left$(ItemName, 2)) = "on" Then
            add_model_info create_datum_from_ithing, create_datum_iobject_method(this, ItemName, this.Method(ItemName))
        Else
            add_model_info create_datum_from_ithing, create_datum_iobject_event(this, ItemName, this.Method(ItemName))
        End If
    Next
End Function
Public Function create_datum_iobject_method(this As IThing, ByVal ItemName As String, vData) As a_Datum
        Set create_datum_iobject_method = New a_Datum
        create_datum_iobject_method.Origin = this
        create_datum_iobject_method.MemberName = ItemName
        create_datum_iobject_method.SiteKind = SITEKIND_VWTHING
        create_datum_iobject_method.typeName = "vt_variant"
        create_datum_iobject_method.nodekind = NODEKIND_FUNCTION
        Set create_datum_iobject_method.Value = vData
End Function

Public Function create_datum_iobject_event(this As IThing, ByVal ItemName As String, vData) As a_Datum
        Set create_datum_iobject_event = New a_Datum
        create_datum_iobject_event.Origin = this
        create_datum_iobject_event.MemberName = ItemName
        create_datum_iobject_event.SiteKind = SITEKIND_VWTHING
        create_datum_iobject_event.typeName = "vt_variant"
        create_datum_iobject_event.nodekind = NODEKIND_EVENT
        Set create_datum_iobject_event.Value = vData
End Function

Public Function create_datum_iobject_property(this As IThing, ByVal ItemName As String, vData) As a_Datum
        Set create_datum_iobject_property = New a_Datum
        create_datum_iobject_property.Origin = this
        create_datum_iobject_property.MemberName = ItemName
        create_datum_iobject_property.SiteKind = SITEKIND_VWTHING
        create_datum_iobject_property.typeName = typeName(vData)
        create_datum_iobject_property.nodekind = NODEKIND_property
        If IsObject(vData) Then
            Set create_datum_iobject_property.Value = vData
        Else
            create_datum_iobject_property.Value = vData
        End If
End Function

Public Sub enum_model(this As a_Datum)
Exit Sub
    Set this.Models = New PropertyMap
    Select Case this.SiteKind
        Case SiteKinds.SITEKIND_VWSERVER, SiteKinds.SITEKIND_ACTX ' Standard COM
        Case SiteKinds.SITEKIND_VWTHING
        Case SiteKinds.SITEKIND_PROLOG_CLIENT
        Case SiteKinds.SITEKIND_PROLOG_SERVER
        Case Else
    End Select
End Sub

Private Sub Class_Initialize()
If Not m_ready_flag Then preptheobjects
'   HookDispatch Me
'On Error Resume Next
'    theAddRefs.Add Me, CStr(ObjPtr(Me))
'If Err Then Stop
End Sub
Private Sub Class_Terminate()
 '  UnHookDispatch Me
End Sub

Property Get Models(this As a_Datum) As PropertyMap
    Set Models = this.Models
End Property
Property Get Properties(this As a_Datum) As PropertyMap
    Set Properties = typelist(this, NODEKIND_property)
End Property
Property Get Methods(this As a_Datum) As PropertyMap
    Set Methods = typelist(this, NODEKIND_FUNCTION)
End Property
Property Get Events(this As a_Datum) As PropertyMap
    Set Events = typelist(this, NODEKIND_EVENT)
End Property
Property Get Contents(this As a_Datum) As PropertyMap
    Set Contents = typelist(this, NODEKIND_CONTENT)
End Property

Private Sub add_model_info(ByVal this As a_Datum, thisDatum As a_Datum)
        If thisDatum Is Nothing Then Exit Sub
        If thisDatum.MemberName = "" Then Exit Sub
        If this.Models.IsValid(thisDatum.MemberName) Then Exit Sub
'        Stop
        this.Models.Property(thisDatum.MemberName) = thisDatum
End Sub

Public Sub set_origin(this As a_Datum, theObject, MemberName)
    Set this.Origin = theObject
    this.MemberName = MemberName
End Sub

'SDF  253-219-5129
' scarlet68@hotmail.com

Private Function typelist(this As a_Datum, NODEKINDSearch As NODEKINDs) As PropertyMap
    Dim MemberName
    Set typelist = New PropertyMap
    For Each MemberName In this.Models
        If this.Models.Property(MemberName).this.nodekind = NODEKINDSearch Then typelist.Property(MemberName) = this.Models.Property(MemberName)
    Next
End Function



Public Function OrderedVTableFunctions( _
  ByVal tLInf As TypeLibInfo, _
  IFaceName As String) As tli.MemberInfo()
Dim ti As InterfaceInfo
Dim Bases As TypeInfos
Dim MI As MemberInfo
Dim OVF() As MemberInfo
Dim MaxOffset As Integer
Dim CurOffset As Integer
  'Make sure we get a VTable interface before proceeding
  On Error Resume Next
  Set ti = tLInf.TypeInfos.NamedItem(IFaceName)
  Set ti = ti.VTableInterface
  If Err Or ti Is Nothing Then Exit Function
  On Error GoTo 0
  With ti.Members
    'Largest VTableOffset is generally on the last
    'member in this collection
    MaxOffset = .Item(.Count).VTableOffset
    ReDim OVF(MaxOffset \ 4)
  End With
  Do Until ti Is Nothing
    'Walk each member
    For Each MI In ti.Members
      CurOffset = MI.VTableOffset
      If CurOffset > MaxOffset Then
        'This is extremely rare
        MaxOffset = CurOffset
        ReDim Preserve OVF(MaxOffset \ 4)
      End If
      Set OVF(CurOffset \ 4) = MI
    Next
    'Get the next base
    Set Bases = ti.ImpliedInterfaces
    Set ti = Nothing
    If Bases.Count Then Set ti = Bases(1)
  Loop
  OrderedVTableFunctions = OVF
End Function




Public Sub add_com_dll(ByVal FileName)
If typeName(DLLFiles) = "Nothing" Then Set DLLFiles = New PropertyMap
If DLLFiles.IsValid(FileName) Then Exit Sub
On Error Resume Next
Dim ttlinfo  As TypeLibInfo
Set ttlinfo = tli.TLIApplication.TypeLibInfoFromFile(FileName)
DLLFiles.Property(FileName) = ttlinfo
theComClassBrowser.Text1.AddItem FileName
'

End Sub


Public Function create_instance1(ByVal objecttype As String, ByVal instanceName)
Dim ttlinfo  As TypeLibInfo
Dim tinterfaceinfo As InterfaceInfo
Dim theCOMInstanceWithInfo As InstanceWithInfo
Dim ttypeinfo As tli.typeInfo

For Each ttlinfo In DLLFiles
        Set tinterfaceinfo = ttlinfo.TypeInfos.NamedItem(objecttype)
            If Not (tinterfaceinfo Is Nothing) Then
                Set theCOMInstanceWithInfo.theObject = CreateObject(ttlinfo.Name & "." & objecttype)
                Set theCOMInstanceWithInfo.theinterfaceinfo = tinterfaceinfo
                    theCOMInstanceWithInfo.theName = instanceName
              Set theKnownMorphs.Property(instanceName) = theCOMInstanceWithInfo: create_instance1 = instanceName
                '
                Exit Function
            End If
Next
create_instance1 = "notfound"
End Function

Public Function set_property_value(ByVal instanceName, ByVal propertyName, ByVal Value)
Dim tinterfaceinfo As tli.InterfaceInfo
Dim theCOMInstanceWithInfo As InstanceWithInfo
Dim tobject As Object
Dim themembers As Members
Dim theProperty As MemberInfo
Dim ttypeinfo As tli.typeInfo

    Set tobject = theKnownMorphs.Property(instanceName).theObject
    Set tinterfaceinfo = theKnownMorphs.Property(instanceName).theinterfaceinfo
    Set themembers = tinterfaceinfo.Members
    For Each theProperty In themembers
        'themember 'memberinfo
        If theProperty.Name = propertyName Then
            Select Case theProperty.InvokeKind
            
                Case INVOKE_PROPERTYPUT
                Case INVOKE_PROPERTYGET
                Case INVOKE_FUNC
                                
            
            End Select
                
        End If
    Next

set_property_value = "notfound"
End Function
    
    
Public Function vp(ByVal theObject): Set vp = theObject: End Function


Public Function object_Name(ByVal objthing) As String
   On Error Resume Next
    If (objthing Is Nothing) Then object_Name = "nothing": Exit Function
  
    If LCase(typeName(objthing)) = "iworld" Then object_Name = "iworld": Exit Function
    'if objthing.isstub then object_Name = "isstub": exit Function
    object_Name = objthing.Property("Name")

End Function

Public Sub assign_pred(ByRef LeftSide, ByVal RightSide)
If IsObject(RightSide) Then
    Set LeftSide = RightSide
Else
    'if typeName(leftside)="N
    LeftSide = RightSide
End If
End Sub

Public Function PrototypeMember( _
  tLInf As TypeLibInfo, _
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
Dim TIType As tli.typeInfo
Dim TIResolved As tli.typeInfo
Dim TKind As TypeKinds
Dim theObj
On Error Resume Next
  With tLInf
'
  
    fIsConstant = GetSearchType(SearchData) And tliStConstants
      Set theObj = .GetMemberInfo(SearchData, InvokeKinds, MemberId, MemberName)
    With .GetMemberInfo(SearchData, InvokeKinds, MemberId, MemberName)
'
      If fIsConstant Then
        retVal = "Const "
        
      Else
        retVal = "+" & invokekinds_enum_string(InvokeKinds) & "("
        Select Case .ReturnType.VarType
          Case VT_VOID, VT_HRESULT
            retVal = retVal & "object_public sub(" & LCase(tLInf.GetTypeInfo(SearchData And &HFFFF&).Name)
          Case Else
            retVal = retVal & "object_func(" & LCase(tLInf.GetTypeInfo(SearchData And &HFFFF&).Name)
        End Select
      
      
      'ElseIf InvokeKinds = INVOKE_FUNC Or InvokeKinds = INVOKE_EVENTFUNC Then
       ' Select Case .ReturnType.VarType
       '   Case VT_VOID, VT_HRESULT
       '     retVal = "object_public sub(" & LCase(TLInf.GetTypeInfo(SearchData And &HFFFF&).Name)
       '   Case Else
       '     retVal = "object_fuct(" & LCase(TLInf.GetTypeInfo(SearchData And &HFFFF&).Name)
       ' End Select
       'Else
       ' retVal = "object_prop(" & LCase(TLInf.GetTypeInfo(SearchData And &HFFFF&).Name)
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
                Set TIType = .typeInfo
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
                  strTypeName = typeName(.TypedVariant)
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
        ConstVal = .Value
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
            If Not .typeInfo Is Nothing Then
              If Err Then 'Information not available
                retVal = retVal & " As ?"
              Else
                If .IsExternalType Then
                  retVal = retVal & " As " & _
                           .TypeLibInfoExternal.Name & "." & .typeInfo.Name
                Else
                  retVal = retVal & " As " & .typeInfo.Name
                End If
              End If
            End If
            If VarTypeCur And (VT_ARRAY Or VT_VECTOR) Then retVal = retVal & "()"
            On Error GoTo 0
          Else
            Select Case VarTypeCur
              Case VT_VARIANT, VT_VOID, VT_HRESULT
              Case Else
                strTypeName = typeName(.TypedVariant)
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
                        "% Member of " & tLInf.Name & "." & _
                        tLInf.GetTypeInfo(SearchData And &HFFFF&).Name & _
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
   Optional ByVal hidden As Boolean = False) As Long
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
  If hidden Then
    BuildSearchData = BuildSearchData Or &H1000&
  End If
End Function

Public Function ProduceDefaultValue(DefVal As Variant, ByVal ti As tli.typeInfo) As String
Dim lTrackVal As Long
Dim MI As MemberInfo
Dim TKind As TypeKinds
    If ti Is Nothing Then
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
        TKind = ti.TypeKind
        Do While TKind = TKIND_ALIAS
            TKind = TKIND_MAX
            On Error Resume Next
            Set ti = ti.ResolvedType
            If Err = 0 Then TKind = ti.TypeKind
            On Error GoTo 0
        Loop
        If ti.TypeKind = TKIND_ENUM Then
            lTrackVal = DefVal
            For Each MI In ti.Members
                If MI.Value = lTrackVal Then
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
  tli.TypeLibInfoFromFile("stdole2.tlb").TypeInfos.NamedItem("GUID").Members(4).ReturnType.ArrayBounds Bounds
  If Bounds(1, 2) = 8 Then m_fFixArrayBounds = True
  m_fCheckedTLIArrayBounds = True
End Sub



Public Function ImplementedInterfaces( _
  ByVal CCInfo As CoClassInfo, _
  Libs As Collection, _
  Optional ByVal fIncludeDefault As Boolean = True) As SearchResults
Dim sr As SearchResults
Dim IFace As InterfaceInfo
Dim tLInf As TypeLibInfo
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
      Set tLInf = IFace.Parent
      TypeInfos(0) = IFace.TypeInfoNumber
      fLookupLib = True
      If TLInfLast Is Nothing Then
      ElseIf tLInf.IsSameLibrary(TLInfLast) Then
        'TLInfLast is simply an optimization.
        'Checking IsSameLibrary is faster than
        'doing a collection lookup.
        fLookupLib = False
        'Use the TypeLibInfo object from the
        'collection so that it has the LibNum set
        Set tLInf = TLInfLast
      End If
If fLookupLib Then
        strKey = CStr(ObjPtr(tLInf.ITypeLib))
        On Error Resume Next
        'Use the TypeLibInfo object from the
        'collection so that it has the LibNum set
        Set tLInf = Libs(strKey)
        If Err Then
          'New library
          tLInf.LibNum = Libs.Count + 1
          Libs.Add tLInf, strKey
          Set TLInfLast = tLInf
        End If
        On Error GoTo Error
      End If
      'Call AddTypes, appending to the previous collection
      Set ImplementedInterfaces = _
       tLInf.AddTypes(TypeInfos, ImplementedInterfaces, , False)
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


Public Function tli_from_anything(ByRef theObject) 'As InterfaceInfo
Dim tli_file As tli.TypeLibInfo, tempObj
On Error Resume Next


        If IsObject(theObject) Then Set tli_from_anything = tli.InterfaceInfoFromObject(theObject): Exit Function
        Err.Clear
        For Each tli_file In DLLFiles
            If tli_file.GetTypesWithSubString(theObject).Count > 1 Then Set tli_from_anything = tli_file: Exit Function
        Next
        Set tempObj = CreateObject(theObject)
        If tempObj Is Nothing Then
        Set tli_from_anything = tli_from_anything(tempObj)
        End If

End Function
Public Function typeinfo_int(typeInfo)
If typeInfo Is Nothing Then typeinfo_int = 0: Exit Function
typeinfo_int = java_vbscript_engine.Eval(typeInfo)
End Function


Public Function coclass_array(tempObj, Optional Cascade = 2)
Dim theMemberInfo As MemberInfo 'object_defualt_members
Dim temparray(), temppropmap As PropertyMap, i As Long
Dim thememberName, theDataType, thememberprologName, theprologName, Item
    
    ReDim temparray0(tempObj.Count - 1): i = 0
    ReDim temparray1(tempObj.Count - 1)
    ReDim temparray2(tempObj.Count - 1)
    ReDim temparray3(tempObj.Count - 1)
    For Each theMemberInfo In tempObj
        If Left(theMemberInfo.Name, 1) = "_" Then GoTo Havit:
        If InStr("[startithere,QueryInterface,AddRef,Release,GetTypeInfoCount,GetTypeInfo,GetIDsOfNames,Terminate,RefCount,CLSID,IID,Invoke,endithere]", "," & theMemberInfo.Name & ",") Then GoTo Havit:
        
        If i > 0 Then
            If temparray0(i - 1) = theMemberInfo.Name Then
                temparray2(i - 1) = temparray2(i - 1) Or theMemberInfo.InvokeKind
                temparray3(i - 1) = theMemberInfo.Name & "(" & theMorphism.object_pl(theMemberInfo.ReturnType, Cascade - 1, True, "vartypeinfo") & "(" & invokekinds_enum_string_io(temparray2(i - 1)) & "" & theMemberInfo.Name & ")"
                GoTo Havit:
            End If
        End If
        temparray0(i) = theMemberInfo.Name
        temparray1(i) = theMemberInfo.ReturnType.VarType
        temparray2(i) = theMemberInfo.InvokeKind
        temparray3(i) = theMemberInfo.Name & "(" & LCase(theMorphism.object_pl(theMemberInfo.ReturnType, Cascade - 1, True, "vartypeinfo")) & "(" & invokekinds_enum_string_io(temparray2(i)) & "" & theMemberInfo.Name & ")"
        If theMemberInfo.Parameters.Count > 0 Then
            temparray3(i) = temparray3(i) & "," & theMorphism.object_pl(theMemberInfo.Parameters, Cascade, True, "parameters") & ")"
         Else
            temparray3(i) = temparray3(i) & ")"
        End If
        
        i = i + 1
Havit:
'
    Next
    ReDim Preserve temparray0(i - 1)
    ReDim Preserve temparray1(i - 1)
    ReDim Preserve temparray2(i - 1)
    ReDim Preserve temparray3(i - 1)
    coclass_array = Array(temparray0, temparray1, temparray2, temparray3)
'
End Function

Public Function members_from_anything(theObject) As Members
    theMorphism.object_pl_pred members_from_anything, tli_from_anything(theObject).Members
End Function


'''Collections

Public Function typeinfos_pl(this, Optional Cascade = 2)
Dim itemObject, Serialize
    If this.Count = 0 Then typeinfos_pl = "typeinfos([])": Exit Function
    For Each itemObject In this
        Serialize = Serialize & "," & theMorphism.object_pl(itemObject, Cascade - 1)
    Next
 typeinfos_pl = "typeinfos([" & Mid(Serialize, 2) & "])"
End Function

Public Function members_pl(this, Optional Cascade = 2)
Dim itemObject, Serialize
    If this.Count = 0 Then members_pl = "members([])": Exit Function
    For Each itemObject In this
        Serialize = Serialize & "," & theMorphism.object_pl(itemObject, Cascade - 1)
    Next
 members_pl = "members([" & Mid(Serialize, 2) & "])"
End Function



Public Function interfaces_pl(this, Optional Cascade = 2)
Dim itemObject, Serialize
    If this.Count = 0 Then interfaces_pl = "interfaces([])": Exit Function
    For Each itemObject In this
        Serialize = Serialize & "," & theMorphism.object_pl(itemObject, Cascade - 1)
    Next
 interfaces_pl = "interfaces([" & Mid(Serialize, 2) & "])"
End Function

Public Function customdata_pl(this, Optional Cascade = 2)
Dim itemObject, Serialize
    If this = Null Then customdata_pl = "customdata([])": Exit Function
    For Each itemObject In this
        Serialize = Serialize & "," & theMorphism.object_pl(itemObject, Cascade - 1)
    Next
 customdata_pl = "customdata([" & Mid(Serialize, 2) & "])"
End Function

''' Foundation Objects


Public Function memberinfo_pl(ByVal this As MemberInfo, Optional Cascade = 2)
Dim Serialize
If Cascade < 1 Then memberinfo_pl = "memberinfo(_)": Exit Function
    memberinfo_pl = "memberinfo(" & _
        theMorphism.object_pl(this.Name, Cascade - 1) & "," & _
        invokekinds_enum_string(this.InvokeKind, Cascade - 1) & "," & _
        theMorphism.object_pl(this.ReturnType, Cascade - 1) & "," & _
        theMorphism.object_pl(this.Parameters, Cascade - 1, True, "parameters") & _
    ")"

End Function


Public Function parameterinfo_pl(ByVal this As ParameterInfo, Optional Cascade = 2)

End Function


Public Function typelibinfo_pl(ByVal this As TypeLibInfo, Optional Cascade = 2)
Dim Serialize
    typelibinfo_pl = "typelibinfo(" & _
        this.Name & "," & _
        typeinfos_pl(this.TypeInfos, Cascade - 1) & "," & _
        interfaces_pl(this.Interfaces, Cascade - 1) & "," & _
        theMorphism.object_pl(this.Records, Cascade - 1) & _
    ")"
'
End Function

Public Function typeinfo_pl(ByVal this As tli.typeInfo, Optional Cascade = 2)
Dim Serialize
    If Cascade < 1 Then typeinfo_pl = "typeinfo(_)": Exit Function
    If this Is Nothing Then typeinfo_pl = "typeinfo(nothing)": Exit Function
    typeinfo_pl = "type_info(" & _
        this.Name & "," & _
        members_pl(this.Members, Cascade - 1) & "," & _
        typekinds_enum_string(this.TypeKind, Cascade - 1) & _
    ")"
'
End Function



''Special types

Public Function interfaceinfo_pl(ByVal this As InterfaceInfo, Optional Cascade = 2)
Dim Serialize
    If this Is Nothing Then interfaceinfo_pl = "interfaceinfo(nothing)": Exit Function

    interfaceinfo_pl = "interfaceinfo(" & _
        theMorphism.object_pl(this.Name, Cascade - 1) & "," & _
        members_pl(this.Members, Cascade - 1) & "," & _
        typekinds_enum_string(this.TypeKind, Cascade - 1) & _
    ")"

'        object_pl(This.ResolvedType, Cascade - 1) & "," &

'
End Function


Public Function coclassinfo_pl(ByVal this As CoClassInfo, Optional Cascade = 2)
Dim Serialize
    coclassinfo_pl = "coclassinfo(" & _
        theMorphism.object_pl(this.Name, Cascade - 1) & "=" & _
        theMorphism.object_pl(this.Interfaces, Cascade - 1) & "," & _
    ")"
'
End Function


Public Sub assign(ByRef theAnything, ByVal ToBecome)
If IsObject(ToBecome) Then Set theAnything = ToBecome Else theAnything = ToBecome
End Sub

Public Sub getrecord(ByRef Result, ByRef Obj, ByVal themem As String)
Dim realObject, Name As String
'
theMorphism.resolve_object realObject, Name, Obj
On Error Resume Next
assign Result, CallByName(realObject, themem, VbGet)
If Err Then Result = "error(" & parse_vbstr_pl_string("morphology getrecord " & Err.Description) & ")"
'
End Sub


Public Sub call_by_Name_pred(theresult, ByRef theObject, ByVal thecall As String, Optional ByRef thecalltype, Optional theArgs)
Dim myptr, tempVariant As Variant
On Error Resume Next: Err.Clear

If Left(thecall, 3) = "obj" Then Exit Sub

If IsMissing(thecalltype) Then thecalltype = VbGet

tempVariant = theArgs
'Stop


If IsMissing(theArgs) Then
    assign theresult, CallByName(theObject, thecall, thecalltype)
Else
    assign theresult, CallByName(theObject, thecall, thecalltype, tempVariant)
End If

If Err = 0 Then Exit Sub

Err.Clear
    
assign theresult, InvokeHook(theObject, thecall, thecalltype, theArgs)

If Err = 0 Then Exit Sub
Err.Clear

Dim Args() As Variant
Dim i As Integer
Dim iArr() As Integer
Dim vt As Integer
Dim InvokeID As Long
  'Call Foo(i As Integer, iArr() As Integer)
  ReDim Args(1)
  'Use this code only with variable sized arrays.
  Args(0) = VarPtrArray(iArr)
  vt = VT_BYREF Or VT_ARRAY Or VT_I2
  CopyMemory Args(0), vt, 2
  Args(1) = VarPtr(i)
  vt = VT_BYREF Or VT_I2
  CopyMemory Args(1), vt, 2
'Stop
assign theresult, tli.InvokeHookArray(theObject, thecall, thecalltype, Args)

If Err = 0 Then Exit Sub
'Err.Clear

'Stop
'If thecall = "AddObject" Then Stop

theresult = parse_vbstr_pl_string("error callbyName " & thecall & " " & Err.Description)
Debug.Print theresult
End Sub

Public Function get_ptr(ByRef Anything)
    If IsObject(Anything) Then
        get_ptr = ObjPtr(Anything)
    Else
        get_ptr = VarPtr(Anything)
    End If
End Function


Public Function vb_call_method(this As tli.InvokeKinds) As VbCallType
vb_call_method = VbCallType.VbGet
  Select Case this
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
  '    invokekinds_enum_string = "INVOKE_PROPERTYPUTREF_PROPERTYPUT" '"Property Let/Set"
    Case INVOKE_PROPERTYGET Or INVOKE_PROPERTYPUTREF Or INVOKE_PROPERTYPUT
   '   invokekinds_enum_string = "INVOKE_PROPERTYGET_PROPERTYPUTREF_PROPERTYPUT" '"Property Get/Let/Set"
  End Select
End Function

Public Function get_properties(theObject, ByVal PropertyListString)
 Dim PropertyArray(), answer
 Dim objloc As Long, objtypeName As String, i As Long
 Dim this
'

pl_split_pred PropertyArray, pl_trim(PropertyListString), ","
theMorphism.resolve_object this, "", theObject
 objtypeName = LCase(typeName(this))
 objloc = ObjPtr(this)
 
    For i = 0 To UBound(PropertyArray)
        'Try Prolog First
        answer = theSWIPrologShell.Execute("object_member_get_prolog_version(" & objtypeName & "," & PropertyArray(i) & ",X)),post(X)")
        If answer = "" Then
            'Try VW Instance Next
            On Error Resume Next
            assign answer, this.Property(PropertyArray(i))
            If Not is_good(answer) Then
                'The Real Object
                getrecord answer, this, PropertyArray(i)
            End If
        End If
        If is_good(answer) Then theMorphism.object_pl PropertyArray(i), answer
    Next i
get_properties = Join(PropertyArray, ",")
End Function
    
Public Function is_good(ByRef theObject) As Boolean
    If typeName(theObject) = "Nothing" Then is_good = False: Exit Function
    If IsObject(theObject) Then is_good = True: Exit Function
    If theObject = "" Then is_good = False: Exit Function
    is_good = True
End Function


Public Function object_model_array_from_class_string(ByVal theClassName As String)
    pl_split_pred object_model_array_from_class_string, theSWIPrologShell.Execute("get_classmembers(" & LCase(theClassName) & ")")
End Function


Public Function get_events(ByVal theObject, Optional Cascade = 2) As String
    On Error Resume Next
    get_events = "events(_)"
End Function

Public Function get_methods(theObject As Object, Optional Cascade = 2) As String
    Dim tempmethods As String
    Dim theInterface, theMemberInfo
    If Cascade < 1 Then get_methods = "methods(" & get_class(theObject) & "," & get_type(theObject) & ",_)"
    Set theInterface = get_members(theObject)
    For Each theMemberInfo In theInterface
        If theMemberInfo.InvokeKind And tli.InvokeKinds.INVOKE_FUNC Then tempmethods = tempmethods & ",method(" & theMemberInfo.Name & "," & get_parameters(theMemberInfo) & "," & vartype_enum_string(theMemberInfo.ReturnType.VarType) & ")"
'
    Next
'
    get_methods = "methods(" & get_class(theObject) & ",_," & get_type(theObject) & ",[" & LCase(Mid(tempmethods, 2)) & "])"
End Function

Public Function get_parameters(ByVal theMemberInfo As MemberInfo)
Dim tempParams As String
    get_parameters = "parameters(_)"
    Dim theParameterInfo As ParameterInfo
    For Each theParameterInfo In theMemberInfo.Parameters
        If theParameterInfo.Default Then
            tempParams = tempParams & ",object(" & theMorphism.object_pl(theParameterInfo.VarTypeInfo, 4, True, "vartypeinfo") & "," & theParameterInfo.Name & "," & "_" & get_model(theParameterInfo.DefaultValue) & ")"
        Else
            tempParams = tempParams & ",object(" & theMorphism.object_pl(theParameterInfo.VarTypeInfo, 4, True, "vartypeinfo") & "," & theParameterInfo.Name & "," & "_)"
        End If
    Next
    get_parameters = "parameters([" & LCase(Mid(tempParams, 2)) & "])"
End Function
Public Function BestClassInfo1(ByVal Object As Object) As tli.typeInfo
Dim dll, thistype, className
On Error Resume Next
  Set BestClassInfo1 = tli.ClassInfoFromObject(Object)
  If Err Then
        Err.Clear
        thistype = typeName(Object)
        For Each dll In DLLFiles
            For Each className In dll.CoClasses
                If className.Name = thistype Or "I" & className.Name = thistype Then Set BestClassInfo1 = className: ': exit Function
                
            Next
        Next
       '
    End If
  On Error GoTo NotAvailable
  With BestClassInfo1.Parent
    With tli.TypeLibInfoFromRegistry _
        (.GUID, .MajorVersion, .MinorVersion, .LCID)
      Set BestClassInfo1 = .Me.TypeInfos.IndexedItem(BestClassInfo1.TypeInfoNumber)
    End With
  End With
  Exit Function
NotAvailable:
  Err.Clear
  '
End Function


Public Function BestClassInfo(ByVal Object As Object) As tli.typeInfo
Dim dll, thistype, className
'
If Object Is Nothing Then Set BestClassInfo = Nothing: Exit Function
'
'If TypeName(object) = "Datum" Then Set BestClassInfo = tli.TypeInfoFromRecordVariant
On Error Resume Next
  Set BestClassInfo = tli.ClassInfoFromObject(Object)
  If Not (BestClassInfo Is Nothing) Then
'
        add_com_dll BestClassInfo.Parent.ContainingFile
        Exit Function
    Else
        
    End If
    
  On Error GoTo NotAvailable:
  With BestClassInfo.Parent
    With tli.TypeLibInfoFromRegistry _
        (.GUID, .MajorVersion, .MinorVersion, .LCID)
      Set BestClassInfo = .Me.TypeInfos.IndexedItem(BestClassInfo.TypeInfoNumber)
       'bestclassinfo.
    End With
    add_com_dll BestClassInfo.Parent.ContainingFile
  End With

  Exit Function
NotAvailable:
'
  If Err Then
'
       ' Err.Clear
        thistype = typeName(Object)
reSearch:
'
        For Each dll In DLLFiles
            For Each className In DLLFiles.Property(dll).CoClasses
                If className.Name = thistype Or "I" & className.Name = thistype Then Set BestClassInfo = className: Exit Function ': exit Function
                
            Next
            On Error Resume Next
'
            For Each className In DLLFiles.Property(dll).Interfaces
                
                If className.Name = thistype Or "I" & className.Name = thistype Then
                    Set BestClassInfo = className: Exit Function ': exit Function
                Else
                
                End If
            Next
'
             For Each className In DLLFiles.Property(dll).TypeInfos
                
                If className.Name = thistype Or "I" & className.Name = thistype Then
                    Set BestClassInfo = className: Exit Function ': exit Function
                Else
                
                End If
            Next
'
            If Left$(thistype, 1) = "I" Then thistype = Mid$(thistype, 2): GoTo reSearch
        Next
        If Right$(thistype, 10) = "Collection" Then thistype = Left$(thistype, Len(thistype) - 10) & "s": GoTo reSearch
        Debug.Print "'" & thistype
       ' Set dll = DLLFiles.Property("C:\WINNT\System32\msscript.ocx")
         '

    End If
  '
End Function
Public Function call_by_Name(ByRef theObject, ByRef theProcName As String, Optional ByRef thecalltype, Optional ByRef theArgs)
    call_by_Name_pred call_by_Name, theObject, theProcName, thecalltype, theArgs
End Function





Public Function get_model(ByVal theObject, Optional Cascade = 2, Optional Instance = "false") As String
'
'Stop
'On Error Resume Next
    Dim theTypeInfo As tli.typeInfo, theInterface, theMemberInfo
    Dim tempModel As String
    Dim tempDatum As a_Datum
    Dim subObject As Object
    Dim subObjectmodel As String
    
    
    'If IsMissing(Instance) Then
    '    Instance = False
    'Else
        Instance = True
    'End If
    
    If Cascade < 1 Then
       ' Stop
        If Instance Then
            get_model = "object(" & get_class(theObject) & "," & get_type(theObject) & ",objptr(" & ObjPtr(theObject) & "),_)": Exit Function
        Else
            get_model = "object(" & get_class(theObject) & "," & get_type(theObject) & ",_,_)": Exit Function
        End If
    End If

'
    If IsObject(theObject) Then
    
        If theObject Is Nothing Then
            If Instance Then
                get_model = "object(class([unset]),type([nothing]),0,_)": Exit Function
            Else
                get_model = "object(class([unset]),type([nothing]),_,_)": Exit Function
            End If
        End If
        
        Set theTypeInfo = theCOMMorphism.BestClassInfo(theObject)
'todo
'        If theTypeInfo Is Nothing Then Set theTypeInfo = InterfaceInfoFromObject(TheObject)
    Set theTypeInfo = InterfaceInfoFromObject(theObject)
        If InStr(1, theTypeInfo.Parent.ContainingFile, "vw", vbTextCompare) Then


'
'                theMorphism.object_pl get_model, TheObject, Cascade
            
            
          '      Exit Function
                   
            '
            
        End If
        
        On Error Resume Next: Err.Clear
:
        Set theInterface = theTypeInfo.DefaultInterface.Members
        If Err Then Err.Clear: Set theInterface = theTypeInfo.Members
        If Err Then Err.Clear: Set theInterface = theTypeInfo.Interfaces
 
        
        
        
        If theInterface Is Nothing Then
'            :
            If Right$(typeName(theObject), 10) = "Collection" Then
:
                For Each subObject In theObject
                    subObjectmodel = subObjectmodel & "," & get_model(subObject, Cascade, Instance)
                Next
                
                get_model = "collection( " & get_class(theObject) & ",[" & Mid$(subObjectmodel, 2) & "])"
                Exit Function
            End If
            If Instance Then
                get_model = "object(class([todo]),type([" & LCase(typeName(theObject)) & "]),_,_)": Exit Function
            Else
                get_model = "object(class([todo]),type([" & LCase(typeName(theObject)) & "]),_,_)": Exit Function
            End If
            Exit Function
        End If
            
        If Instance Then
        
                For Each theMemberInfo In theInterface
                    If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or (theMemberInfo.InvokeKind And tli.INVOKE_PROPERTYGET) Then tempModel = tempModel & ",object(class([" & vartype_enum_string(theMemberInfo.ReturnType.VarType) & "]),type(" & parse_vbstr_pl_string(theMemberInfo.Name) & "),_," & get_model(call_by_Name(theObject, theMemberInfo.Name, VbGet, Null), Cascade - 1, Instance) & ")"
                Next
        Else
                For Each theMemberInfo In theInterface
                    If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or (theMemberInfo.InvokeKind And tli.INVOKE_PROPERTYGET) Then tempModel = tempModel & ",object(class([" & vartype_enum_string(theMemberInfo.ReturnType.VarType) & "]),type(" & parse_vbstr_pl_string(theMemberInfo.Name) & "),_," & get_model(call_by_Name(theObject, theMemberInfo.Name, VbGet, Null), Cascade - 1, Instance) & ")"
                Next
        End If
        
        get_model = "object(" & get_class(theObject) & "," & get_type(theObject) & ",objptr(" & ObjPtr(theObject) & "),[" & LCase(Mid(tempModel, 2)) & "])"
    Else
        
        If IsNull(theObject) Then
            If Instance Then
                get_model = "object(class([unset]),type([null]),0,[null])": Exit Function
            Else
                get_model = "object(class([unset]),type([null]),_,_)": Exit Function
            End If
        End If
        
        If IsEmpty(theObject) Then
            If Instance Then
                get_model = "object(class([unset]),type([empty]),0,[empty])": Exit Function
            Else
                get_model = "object(class([unset]),type([empty]),_,_)": Exit Function
            End If
        End If
        
        get_model = theMorphism.object_pl(theObject, Cascade)
        
        'get_model = "object(class([data])," & get_type(TheObject) & ",varptr(" & VarPtr(TheObject) & ")," & parse_vbstr_pl_string(TheObject) & ")"
        ':
        
        If Err Then get_model = "object(class([data])," & get_type(theObject) & ",error," & parse_vbstr_pl_string(Err.Description) & ")"
    
    End If
        
End Function












Public Function get_object(this)

End Function

Public Function get_reference(ByRef this) As String
        If IsObject(this) Then
            If this Is Nothing Then get_reference = "(_Nothing)" Else get_reference = "objptr(" & ObjPtr(this) & ")"
         Else
            If IsNull(this) Then get_reference = "objptr(null)" _
            Else If IsMissing(this) Then get_reference = "(_Missing)" _
            Else If IsEmpty(this) Then get_reference = "(_Empty)" _
            Else get_reference = "varptr(" & VarPtr(this) & ")"
        End If
End Function

Public Function get_classification(ByRef this) As String
get_classification = "classification(" & get_class(this) & "," & get_type(this) & ")"
End Function

Public Function get_class(ByRef theObject) As String
    On Error Resume Next
    get_class = "" & parse_vbstr_pl_string(typeName(theObject)) & ""
    If Err Then: get_class = "class(" & parse_vbstr_pl_string(Err.Description) & ")": Err.Clear
End Function


Public Function get_type(ByVal theObject) As String
    On Error Resume Next
    get_type = "" & parse_vbstr_pl_string(typeName(theObject)) & ""
    If Err Then: get_type = "type(" & parse_vbstr_pl_string(Err.Description) & ")": Err.Clear
End Function
Public Function get_members_values(this, MemberNames, Optional Cascade = 2, Optional Morphismd = False)
Dim mNames(), temppairs
Dim i, eresult
mNames = Split(MemberNames, ",")
If Morphismd Then
        For i = 0 To UBound(mNames)
            call_by_Name_pred eresult, this, mNames(i)
            temppairs = temppairs & "," & mNames(i) & "=" & theMorphism.object_pl(eresult, Cascade, Morphismd)
            eresult = Null
        Next i
Else
        For i = 0 To UBound(mNames)
            call_by_Name_pred eresult, this, mNames(i)
            temppairs = temppairs & "," & mNames(i) & "=" & theMorphism.object_pl(eresult, Cascade, Morphismd)
            eresult = Null
        Next i
End If

get_members_values = Mid$(temppairs, 2)
End Function

Public Function get_member_list(this, MemberNames)
Dim theInterface, theMemberInfo
Dim i, eresult, tempModel
    Set theInterface = get_members(this)
            For Each theMemberInfo In theInterface
                        tempModel = tempModel & "," & theMemberInfo.Name
            Next
    get_member_list = Mid$(tempModel, 2)

End Function



Public Function get_typelib_from_object(this) As TypeLibInfo
    On Error Resume Next
    Set get_typelib_from_object = InterfaceInfoFromObject(this).Parent
    If Err = 0 Then Exit Function
'    Stop
End Function

Public Function get_members_pl(sThis) As String
'    Set myMembers = get_members(sThis)
    
End Function

Public Function get_member_by_Name_pl(this, MemberName, Cascade)
    Dim theresult
    On Error Resume Next: Err.Clear
    If this.IsValidProperty(MemberName) Then
        assign theresult, this.Property(MemberName)
    Else
        call_by_Name_pred theresult, this, MemberName
    End If
    theMorphism.object_pl_pred get_member_by_Name_pl, theresult, Cascade
End Function

Public Function get_members(sThis) As Members
Dim theTypeInfo As tli.typeInfo, theInterfaces
Dim i, eresult, tempModel, model, this, thetypeName As String
'    Stop
    If Not IsObject(sThis) Then Stop
   assign this, sThis
'    Stop
    thetypeName = typeName(this)
    
    If theTypeNameInterfaces.IsValid(thetypeName) Then
        Set get_members = theTypeNameInterfaces.Property(thetypeName)
      '  Debug.Print "' found " & TypeName(This)
        Exit Function
    End If
    Set theTypeInfo = BestClassInfo(this)
    If theTypeInfo Is Nothing Then Set theTypeInfo = InterfaceInfoFromObject(this)
    
    On Error Resume Next
     Set theInterfaces = theTypeInfo.DefaultInterface.Members
    If Err Then Err.Clear: Set theInterfaces = theTypeInfo.Members
    If Err Then
        Err.Clear: Set get_members = theTypeInfo.Interfaces
        'Stop
    End If
    Err.Clear
    
    On Error GoTo 0
    If theInterfaces Is Nothing Then Set theInterfaces = New Collection
    
    theTypeNameInterfaces.Property(thetypeName) = theInterfaces
    Set get_members = theInterfaces
    Debug.Print "'adding " & typeName(this) & ":" & typeName(get_members)
'    Stop
End Function

Public Function get_vtable(sThis) As Members
Dim theTypeInfo As tli.typeInfo, theInterfaces
Dim i, eresult, tempModel, model, this, thetypeName As String
'    Stop
    If Not IsObject(sThis) Then Stop
   assign this, sThis
'    Stop
    thetypeName = typeName(this)
'    Stop
    If theTypeNameInterfaces.IsValid(thetypeName) Then
        Set get_vtable = theTypeNameInterfaces.Property(thetypeName)
      '  Debug.Print "' found " & TypeName(This)
        Exit Function
    End If
    Set theTypeInfo = BestClassInfo(this)
    If theTypeInfo Is Nothing Then Set theTypeInfo = InterfaceInfoFromObject(this)
    
    On Error Resume Next
     Set theInterfaces = theTypeInfo.DefaultInterface.VTableInterface.Members
    If Err Then Err.Clear: Set theInterfaces = theTypeInfo.VTableInterface.Members
    If Err Then
        Err.Clear: Set get_vtable = theTypeInfo.VTableInterface.Members
        'Stop
    End If
    Err.Clear
    
    On Error GoTo 0
    If theInterfaces Is Nothing Then Set theInterfaces = New Collection
    
    theTypeNameInterfaces.Property(thetypeName) = theInterfaces
    Set get_vtable = theInterfaces
    Debug.Print "'adding " & typeName(this) & ":" & typeName(get_vtable)
'    Stop
End Function





Public Function get_memberlist_propget_public(this, MemberNames)
Dim theInterface, theMemberInfo, tempModel
    Set theInterface = get_members(this)
            For Each theMemberInfo In theInterface
                If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then
                    If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or (theMemberInfo.InvokeKind And tli.INVOKE_PROPERTYGET) Then
                        tempModel = tempModel & "," & theMemberInfo.Name
                    End If
                End If
            Next
    get_memberlist_propget_public = Mid$(tempModel, 2)

End Function
Public Function get_memberlist_propput_public(this, MemberNames)
Dim theInterface, theMemberInfo, tempModel
    Set theInterface = get_members(this)
            For Each theMemberInfo In theInterface
                If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then
                    If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or (theMemberInfo.InvokeKind And (tli.INVOKE_PROPERTYPUT Or tli.INVOKE_PROPERTYPUTREF)) Then
                        tempModel = tempModel & "," & theMemberInfo.Name
                    End If
                End If
            Next
    get_memberlist_propput_public = Mid$(tempModel, 2)

End Function
Public Function get_memberlist_event_public(this, MemberNames)
Dim theInterface, theMemberInfo, tempModel
    Set theInterface = get_members(this)
            For Each theMemberInfo In theInterface
                If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then
                    If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or (theMemberInfo.InvokeKind And tli.INVOKE_EVENTFUNC) Then
                        tempModel = tempModel & "," & theMemberInfo.Name
                    End If
                End If
            Next
    get_memberlist_event_public = Mid$(tempModel, 2)
End Function
Public Function get_memberlist_func_public(this, MemberNames)
Dim theInterface, theMemberInfo, tempModel

    Set theInterface = get_members(this)
            For Each theMemberInfo In theInterface
                If InStr(1, "," & theHiddenMembers & ",", "," & theMemberInfo.Name & ",", vbTextCompare) = 0 Then
                    If InStr("," & theExposedMembers & ",", "," & theMemberInfo.Name & ",") Or (theMemberInfo.InvokeKind And tli.INVOKE_FUNC) Then
                        tempModel = tempModel & "," & theMemberInfo.Name
                    End If
                End If
            Next
    get_memberlist_func_public = Mid$(tempModel, 2)
End Function

Public Sub add_collection(Collection1 As Collection, collection2 As Collection)
Dim Item As Variant
For Each Item In Collection1
    collection2.Add Item, Item.Key
Next
End Sub

Public Sub any_type_to_object(theObject As a_Datum, ByVal pl_stringorObject As Variant)
    If Not IsObject(pl_stringorObject) Then
        'Prolog Object or Datum
        theMorphism.pl_object_pred theObject, pl_stringorObject
        Set pl_stringorObject = theObject
    End If
    If typeName(pl_stringorObject) = "Datum" Then Set theObject = pl_stringorObject: Exit Sub
    If theKnownMorphs.IsValid(CStr(ObjPtr(pl_stringorObject))) Then
        'object known
        Set theObject = theKnownMorphs.Property(CStr(ObjPtr(pl_stringorObject)))
        Exit Sub
    End If
    Set theObject = New a_Datum
    Set theObject.Value = pl_stringorObject
    theKnownMorphs.Property(CStr(ObjPtr(pl_stringorObject))) = theObject
    'exit sub (it is built)
End Sub
Public Function sitekind_from_object(m_Value) As SiteKinds
     Dim ContainingFileName As String, NativeTLI, m_TypeName As String
     
        m_TypeName = LCase(VBA.typeName(m_Value))

    Select Case m_TypeName
        Case "empty", "nothing", "null"
            sitekind_from_object = SITEKIND_UNBOUND
        Case "ithing", "thing"
            sitekind_from_object = SITEKIND_VWTHING
        
        Case Else
            Set NativeTLI = get_typelib_from_object(m_Value)

            On Error Resume Next: Err.Clear
            ContainingFileName = NativeTLI.ContainingFile
            If Err Then
                sitekind_from_object = SITEKIND_instance 'unknown source
            Else
                If InStr(1, NativeTLI.ContainingFile, "vw", vbTextCompare) > 0 Then
                    sitekind_from_object = SiteKinds.SITEKIND_VWSERVER
                Else
                    sitekind_from_object = SITEKIND_ACTX
                End If
            End If
    End Select
    
End Function


Public Function MODELKIND_from_object(m_Value) As MODELKINDs
 Dim m_TypeName As String
    
    MODELKIND_from_object = MODELKIND_FUNCDESC
    m_TypeName = LCase(VBA.typeName(m_Value))
    
    If Right$(m_TypeName, 10) = "collection" Then
        MODELKIND_from_object = MODELKIND_COLLECTION
        Exit Function
    End If
    If Right$(m_TypeName, 1) = "s" Then
        MODELKIND_from_object = MODELKIND_COLLECTION
        Exit Function
    End If
    'If Right$(m_TypeName, 3) = "map" Then
    '    MODELKIND_from_object = MODELKIND_MAP
    '    Exit Function
    'End If
    If Right$(m_TypeName, 2) = "()" Then
        MODELKIND_from_object = MODELKINDs.MODELKIND_ARRAY
        Stop
        Exit Function
    End If
    Select Case m_TypeName
        Case "empty"
            MODELKIND_from_object = MODELKINDs.MODELKIND_VALUE
        
        Case "nothing"
            MODELKIND_from_object = MODELKINDs.MODELKIND_object
        
        Case "null"
            MODELKIND_from_object = MODELKINDs.MODELKIND_VALUE
            
        Case "ithing", "thing"
            MODELKIND_from_object = MODELKIND_object
        
        Case Else
            If IsObject(m_Value) Then
                MODELKIND_from_object = MODELKIND_object
            Else
                MODELKIND_from_object = MODELKIND_VALUE
            End If
    End Select
'    Stop
End Function


