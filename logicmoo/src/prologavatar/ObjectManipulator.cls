VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 1  'Persistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PrologInterface"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit


Public Function object_typeName(ByVal actxObject) As Variant
    object_typeName = typeName(actxObject)
End Function

Public Function object_world_ref(ByVal this) As Variant
    object_world_ref = this.ID
End Function

Public Function object_pl(ByVal this, Optional Cascade = 4, Optional Instance, Optional ByVal Cast As String = "", Optional ByVal Origin, Optional NamedArgs = "True") As Variant
    object_pl = theMorphism.object_pl(this, Cascade, Instance, Cast, Origin, NamedArgs)
End Function

Public Sub serial_set(ByVal thetypeName, ByVal list_of_properties)
    ClassMembers.Property(thetypeName) = list_of_properties
End Sub
Public Function serial_get(ByVal thetypeName)
    serial_get = ClassMembers.Property(thetypeName)
End Function
Public Sub serial_add(ByVal thetypeName, ByVal themember)
    If InStr(1, ClassMembers.Property(thetypeName), themember) = 0 Then ClassMembers.Property(thetypeName) = ClassMembers.Property(thetypeName) & "," & themember
End Sub
Public Sub serial_del(ByVal thetypeName, ByVal themember)
    ClassMembers.Property(thetypeName) = Trim(Replace(Replace(vbLf & Replace(Replace(ClassMembers.Property(thetypeName), themember, ""), ",,", ","), vbLf & ",", "") & vbLf, "," & vbLf, ""))
End Sub

Public Function pl_object(ByVal sObject) As Variant
    theMorphism.pl_object_pred pl_object, sObject
End Function

Public Function pl_object_pl(ByVal sObject) As Variant
 Dim the_pl_object
Stop
    theMorphism.pl_object_pred the_pl_object, sObject
    pl_object_pl = theMorphism.object_pl(the_pl_object)
End Function




Public Function set_property_value(ByVal theObject, ByVal theProperty, ByVal Value) As Variant
    set_property_value = theWorldMorphism.set_property_value(theObject, theProperty, Value)
End Function

Public Function set_property_pl(ByVal theObject, ByVal theProperty, ByVal Value) As Variant
    set_property_pl = theWorldMorphism.set_property_pl(theObject, theProperty, Value)
End Function

Public Function object_has_property(ByVal theObject, ByVal theProperty) As Variant
    object_has_property = theWorldMorphism.object_has_property(theObject, theProperty)
End Function

Public Function get_property_pl(ByVal theObject, ByVal theProperty) As Variant
    get_property_pl = theWorldMorphism.get_property_pl(theObject, theProperty)
End Function

Public Function get_property_type(ByVal theObject, ByVal theProperty) As Variant
    get_property_type = theWorldMorphism.get_property_type(theObject, theProperty)
End Function

Public Function get_property(ByVal theObject, ByVal theProperty) As Variant
    get_property = theWorldMorphism.get_property(theObject, theProperty)
End Function






















Public Function create_datum(ByVal this) As a_Datum
End Function

Public Function value_to_string(Value) As String
End Function

Public Function datum_to_prolog(ByVal theDatum As a_Datum, Optional ByVal Level = 2) As String
End Function

Public Function create_datum_from_com(Owner, nodekind, this) As a_Datum
End Function

Public Function create_datum_from_param(Owner As a_Datum, mp As ParameterInfo) As a_Datum
End Function

Public Function get_MODELKIND_from_object(ByVal this) As MODELKINDs
End Function

Public Function create_datum_from_value(Origin As a_Datum, vData As Variant) As a_Datum
End Function

Public Function create_datum_from_ithing(Owner As a_Datum, nodekind As NODEKINDs, ByVal this As Object) As a_Datum
End Function

Public Function create_datum_iobject_method(ByVal this As IThing, ByVal ItemName As String, vData) As a_Datum
End Function

Public Function create_datum_iobject_event(ByVal this As IThing, ByVal ItemName As String, vData) As a_Datum
End Function

Public Function create_datum_iobject_property(ByVal this As IThing, ByVal ItemName As String, vData) As a_Datum
End Function

Public Sub enum_model(ByVal this As a_Datum)
End Sub




Public Sub set_origin(ByVal this As a_Datum, theObject, MemberName)
End Sub

Public Function OrderedVTableFunctions(ByVal tLInf As TypeLibInfo, IFaceName As String) As tli.MemberInfo()
End Function

Public Sub pl_object_pred(ByVal pl_object, ByVal pl_string)
End Sub

Public Sub array_pl_pred(ByVal object_pl, ParamArray this())
End Sub

Public Sub add_com_dll(ByVal FileName)
End Sub

Public Function create_instance1(ByVal objecttype As String, ByVal theObject) As Variant
End Function

Public Function vp(ByVal theObject) As Variant
End Function

Public Function pl_regexp(ByVal pl_stringin) As Variant
End Function

Public Function pl_ithing_pred(ByVal pl_ithing, ByVal pl_stringin) As Collection
End Function

Public Sub get_all_ithings_pred(ByVal get_all_ithings)
End Sub

Public Function object_Name(ByVal objthing) As String
End Function

Public Sub assign_pred(ByVal LeftSide, ByVal RightSide)
End Sub

Public Function PrototypeMember(ByVal tLInf As TypeLibInfo, ByVal SearchData As Long, ByVal InvokeKinds As InvokeKinds, Optional ByVal MemberId As Long = -1, Optional ByVal MemberName As String) As String
End Function

Public Function GetSearchType(ByVal SearchData As Long) As TliSearchTypes
End Function

Public Function GetTypeInfoNumber(ByVal SearchData As Long) As Integer
End Function

Public Function GetLibNum(ByVal SearchData As Long) As Integer
End Function

Public Function GetHidden(ByVal SearchData As Long) As Boolean
End Function

Public Function BuildSearchData(ByVal TypeInfoNumber As Integer, ByVal SearchTypes As TliSearchTypes, Optional ByVal LibNum As Integer, Optional ByVal hidden As Boolean = False) As Long
End Function

Public Function ProduceDefaultValue(ByVal DefVal As Variant, ByVal ti As tli.typeInfo) As String
End Function

Public Function FixArrayBounds(ByVal VTInfo As VarTypeInfo, Bounds() As Long) As Integer
End Function

Public Sub CheckTLIArrayBounds()
End Sub

Public Function ImplementedInterfaces(ByVal CCInfo As CoClassInfo, Libs As Collection, Optional ByVal fIncludeDefault As Boolean = True) As SearchResults
End Function

Public Function GetMissing(Optional ByVal DontPass As Variant) As Variant
End Function

Public Function tli_from_anything(ByVal theObject) As Variant
End Function

Public Function typeinfo_int(ByVal typeInfo) As Variant
End Function

Public Function coclass_array(ByVal tempObj, Optional Cascade = 2) As Variant
End Function

Public Function members_from_anything(ByVal theObject) As Members
End Function

Public Function typeinfos_pl(ByVal this, Optional Cascade = 2) As Variant
End Function

Public Function members_pl(ByVal this, Optional Cascade = 2) As Variant
End Function

Public Function interfaces_pl(ByVal this, Optional Cascade = 2) As Variant
End Function

Public Function customdata_pl(ByVal this, Optional Cascade = 2) As Variant
End Function

Public Function memberinfo_pl(ByVal this As MemberInfo, Optional Cascade = 2) As Variant
End Function

Public Function parameterinfo_pl(ByVal this As ParameterInfo, Optional Cascade = 2) As Variant
End Function

Public Function typelibinfo_pl(ByVal this As TypeLibInfo, Optional Cascade = 2) As Variant
End Function

Public Function typeinfo_pl(ByVal this As tli.typeInfo, Optional Cascade = 2) As Variant
End Function

Public Function interfaceinfo_pl(ByVal this As InterfaceInfo, Optional Cascade = 2) As Variant
End Function

Public Function vartype_enum_string(ByVal this As VariantTypeConstants, Optional Cascade) As Variant
End Function

Public Function invokekinds_enum_string(ByVal this As tli.InvokeKinds, Optional Cascade = 2) As Variant
End Function

Public Function invokekinds_enum_string_io(ByVal this As tli.InvokeKinds, Optional Cascade = 2) As Variant
End Function

Public Function typekinds_enum_string(ByVal this As TypeKinds, Optional Cascade = 2) As Variant
End Function







Public Function object_array_pl_string(ParamArray theObject()) As Variant
End Function


Public Function coclassinfo_pl(ByVal this As CoClassInfo, Optional Cascade = 2) As Variant
End Function

Public Sub assign(ByVal theAnything, ByVal ToBecome)
End Sub

Public Sub getrecord(ByVal Result, ByVal Obj, ByVal themem As String)
End Sub

Public Sub resolve_object(ByVal realObject, ByVal Name, ByVal Obj)
End Sub

Public Sub set_Name_object(objectName, theObject)
End Sub

Public Sub import_object_to_instances(ByVal this)
End Sub

Public Sub call_by_Name_pred(ByVal theresult, ByVal theObject, ByVal thecall As String, Optional ByVal thecalltype, Optional theArgs)
End Sub

Public Function get_ptr(ByVal Anything) As Variant
End Function

Public Function vb_call_method(ByVal this As tli.InvokeKinds) As VbCallType
End Function

Public Function get_properties(ByVal theObject, ByVal PropertyListString) As Variant
End Function

Public Function is_good(ByVal theObject) As Boolean
End Function

Public Function object_model_array_from_class_string(ByVal theClassName As String) As Variant
End Function

Public Function get_events(ByVal theObject, Optional Cascade = 2) As String
End Function

Public Function get_methods(ByVal theObject As Object, Optional Cascade = 2) As String
End Function

Public Function get_parameters(ByVal theMemberInfo As MemberInfo) As Variant
End Function

Public Function BestClassInfo1(ByVal Object As Object) As tli.typeInfo
End Function

Public Function BestClassInfo(ByVal Object As Object) As tli.typeInfo
End Function

Public Function call_by_Name(ByVal theObject, ByVal theProcName As String, Optional ByVal thecalltype, Optional ByVal theArgs) As Variant
End Function

Public Function get_model(ByVal theObject, Optional Cascade = 2, Optional Instance = "false") As String
End Function

Public Function get_object(ByVal this) As Variant
End Function

Public Function get_reference(ByVal this) As String
End Function

Public Function get_classification(ByVal this) As String
End Function

Public Function get_class(ByVal theObject) As String
End Function

Public Function get_type(ByVal theObject) As String
End Function

Public Function object_pl_string(ByVal this, Optional Cascade = 3, Optional Instance, Optional ByVal Cast As String = "", Optional ByVal Origin, Optional NamedArgs = "True") As Variant
End Function

Public Function get_members_values(ByVal this, MemberNames, Optional Cascade = 2, Optional Morphismd = False) As Variant
End Function

Public Function get_member_list(ByVal this, MemberNames) As Variant
End Function

Public Function get_typelib_from_object(ByVal this) As TypeLibInfo
End Function

Public Function get_members_pl(sThis) As String
End Function

Public Function get_member_by_Name_pl(ByVal this, MemberName, Cascade) As Variant
End Function

Public Function get_members(sThis) As Members
End Function

Public Function get_vtable(sThis) As Members
End Function

Public Function get_memberlist_propget_public(ByVal this, MemberNames) As Variant
End Function

Public Function get_memberlist_propput_public(ByVal this, MemberNames) As Variant
End Function

Public Function get_memberlist_event_public(ByVal this, MemberNames) As Variant
End Function

Public Function get_memberlist_func_public(ByVal this, MemberNames) As Variant
End Function

Public Sub add_collection(Collection1 As Collection, collection2 As Collection)
End Sub

Public Sub any_type_to_object(ByVal theObject As a_Datum, ByVal pl_stringorObject As Variant)
End Sub

Public Function sitekind_from_object(m_Value) As SiteKinds
End Function


Public Function MODELKIND_from_object(m_Value) As MODELKINDs
End Function

Public Function tell_pl(ByVal objorigin, ByVal objfrom, ByVal objto, ByVal strmsg, inttCtype, intuitype) As String
End Function

Public Function report_pl(ByVal textBSTR As String, ByVal ltype As Long) As Variant
End Function

Public Function uievent_string(ByVal pthing As Object, ByVal bstreventName As String, ByVal vararg As Variant) As Variant
End Function

Public Function action_pl(ByVal objfrom As VWSYSTEMLib.IThing, ByVal bstreventName As String, ByVal vararg As Variant) As String
End Function

Public Sub Main()
End Sub

Public Sub event_send(ByVal textBSTR)
End Sub

Public Function connect_avatar(ByVal theWorldName As String, ByVal the_avatar_theObject As String) As Variant
End Function

Public Function sendout_test(what, ByVal TextRight As String) As Boolean
End Function

Public Function approach_vector(ByVal vectorstring) As Variant
End Function

Public Function get_object_ivector(ByVal theObject) As Variant
End Function

Public Function get_object_states(ByVal theObject) As Variant
End Function

Public Function get_object_contents(ByVal theObject) As Variant
End Function

Public Function get_object_methods(ByVal theObject) As Variant
End Function

Public Function get_object_goals(ByVal theObject) As Variant
End Function

Public Function place_object_at_ivector(ByVal theObject, ByVal theObjecttarget) As Variant
End Function

Public Function bless_object(ByVal theObject) As Variant
End Function

Public Function approach(ByVal theObject) As Variant
End Function

Public Function say(ByVal theObject As String) As Variant
End Function

Public Function emote(ByVal theObject) As Variant

End Function

Public Function moveinto(ByVal theObject) As Variant
End Function

Public Function take(ByVal theObject) As Variant
End Function

Public Function drop(ByVal theObject) As Variant
End Function

Public Function report(ByVal theObject) As Variant
End Function

Public Function announce(ByVal theObject) As Variant
End Function

Public Function wield(ByVal theObject) As Variant
End Function

Public Function unwield(ByVal theObject) As Variant
End Function

Public Function wear(ByVal theObject) As Variant
End Function

Public Function unwear(ByVal theObject) As Variant
End Function

Public Function world_createinstance(ByVal theObject, ByVal theObjectexemplar) As Variant
End Function

Public Function world_destroyMorphism(ByVal theObject) As Variant
End Function

Public Function whisper(ByVal theObject, Optional strObject = "") As Variant
End Function

Public Function sleep_time(ByVal theObject) As Variant
End Function

Public Function lookat(ByVal theObject) As Variant
End Function

Public Function lookat3(ByVal theObject) As Variant
End Function

Public Function lookat2(ByVal theObject) As Variant
End Function

Public Function walk(ByVal strDistance) As Variant
End Function

Public Function normalize(ByVal strvector) As Variant
End Function




Public Function object_near_object(ByVal theObject, ByVal theObject2) As Variant
End Function

Public Function vb_object_contains(ByVal theObject, ByVal theObject2) As Variant
End Function

Public Function vb_object_has_method(ByVal theObject, ByVal smethod) As Variant
End Function




Public Function invoke_script_exec(ByVal vb_or_java_script) As Variant
End Function

Public Function invoke_script_eval(ByVal vb_or_java_script) As Variant
End Function

Public Function self_Name() As Variant
    self_Name = theAvatarMorphism.self_Name
End Function

Public Function self_connected() As Variant
self_connected = theAvatarMorphism.self_connected
End Function

Public Property Get proxy_avatar() As Object
'used when retrieving value of a property, on the right side of an assignment.
'Syntax: Debug.Print X.proxy_avatar
    Set proxy_avatar = theAvatarMorphism.proxy_avatar
End Property




Public Function get_object_pl(ByVal theObject) As Variant

End Function

Public Function pl_ithing(ByVal theObject) As Variant
End Function

Public Function pl_object_id(ByVal theObject) As Long
End Function

Public Function get_room() As Variant
Set get_room = theAvatarMorphism.get_room
End Function

Public Function get_avatar() As Variant
End Function

Public Function InvokeMethod(ByVal methodName, Optional ByVal Arg0 = "", Optional ByVal Arg1 = "") As Variant

End Function

Public Function invoke_method(ByVal theObject) As Variant
End Function

Public Function invoke_eval(ByVal theObject) As Variant
End Function

Public Function error_string(ByVal theerror As ErrObject) As Variant
End Function

Public Function get_all_ithings() As Collection
End Function

Public Function pl_pl(ByVal plobj) As Variant
End Function

Public Function test_connect() As Variant
End Function

Public Sub events_stack(ByVal textBSTR)
End Sub

Public Function events_get() As String
End Function

Public Function events_get_all() As Variant
End Function

Public Function events_flag() As Variant
End Function

Public Function events_clear() As Variant
End Function

Public Function debugger_send(ByVal textBSTR) As Variant
End Function

Public Function inputbox_pl_string(ByVal textBSTR) As Variant
End Function

Public Function die() As Variant
End Function

Public Function pl_split(ByVal theString, substr) As Variant
End Function

Public Function parse_vbstr_pl_string(ByVal theString) As String
End Function

Public Function parse_pl_string_vbstr(ByVal pl_string) As String
End Function

Public Function scripting_engine_eval(ByVal vb_or_java_script) As Variant
End Function

Public Function scripting_engine_exec(ByVal vb_or_java_script) As Variant
End Function

Public Function scripting_engine_run(ByVal vb_or_java_script, ParamArray Args() As Variant) As Variant
End Function

Public Function scripting_engine_addobject(ByVal pl_string) As Variant
End Function

Public Function scripting_engine_createobject_pl(ByVal pl_string) As Variant
End Function

Public Function scripting_engine_createobject(ByVal Name, ByVal thetypeName) As Variant
End Function

Public Function scripting_engine_addcode(ByVal vb_or_java_script) As Variant
End Function

Public Function common_gate_eval(ByVal vb_or_java_script) As Variant
End Function

Public Function common_gate_eval_pl(ByVal vb_or_java_script) As Variant
End Function

Public Function get_unify(ByVal Object, ByVal propName) As Variant
End Function

Public Function get_unify_pl(ByVal Object, ByVal propName) As Variant
End Function

Public Function unify(ByVal Object, ByVal propName, ByVal testvalue) As Variant
End Function

Public Function pipe(ByVal p, ParamArray Anything() As Variant) As Variant
End Function

Public Function pl_compare(ByVal pl_string, ByVal vbstr) As Variant
End Function



