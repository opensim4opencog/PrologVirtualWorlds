VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "YClass"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit
'Implements Thing

' Dispinterface IThing
' IID {590D9852-A417-11CF-9BF0-0080C7A56A8A}
' IThing Interface

' Inherited interfaces: IDispatch

'Functions:
'==========

' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60000000
'Sub QueryInterface (riid As GUID, ppvObj As Any)


' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60000001
Function AddRef() As Long

End Function

' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60000002
Function Release() As Long

End Function

' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60010000

Sub GetTypeInfoCount(pctinfo As Long)

End Sub

' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60010001
'Sub GetTypeInfo (ByVal itinfo As Long, ByVal lcid As Long, pptinfo As Any)


' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60010002
'Sub GetIDsOfNames(riid As GUID, rgszNames As Byte, ByVal cNames As Long, ByVal lcid As Long, rgdispid As Long)
'
'End Sub

' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60010003
'Sub Invoke(ByVal dispidMember As Long, riid As GUID, ByVal lcid As Long, ByVal wFlags As Integer, pdispparams As DISPPARAMS, pvarResult As Variant, pexcepinfo As EXCEPINFO, puArgErr As Long)

' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020000
Sub Terminate()

End Sub

' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60020001
Property Get RefCount() As Long

End Property

' Description: Returns base CLSID
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020002
'Property Get CLSID() As GUID

' Description: Returns base IID
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020003
'Property Get IID() As GUID

' Description: Gets world the property is attached to
' Function Kind: Dispatch
' MemID: &H60020004
Property Get World() As IWorld

End Property

' Description: Gets world the property is attached to
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020004
Property Let World(NoNamedArg0 As IWorld)

End Property

' Description: Gets the Thing the property is attached to
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020006
Property Get Parent() As IThing

End Property

' Description: Gets the Thing the property is attached to
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020006
Property Let Parent(NoNamedArg0 As IThing)

End Property

' Description: Gets the name of the property
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020008
Property Get PropertyName() As String

End Property

' Description: Gets the name of the property
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020008
Property Let PropertyName(ByVal NoNamedArg0 As String)

End Property

' Description: Gets the type of property (Avatar, etc.)
' Function Kind: Dispatch
' MemID: &H64
'Property Get type() As String

' Description: Gets the type of property (Avatar, etc.)
' Function Kind: Dispatch
' MemID: &H64
'Property Let type(ByVal NoNamedArg0 As String)



' Description: Internal method to get the associated security slot - see IPropertySecurity.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6002000C
Property Get Security() As IPropertySecurity

End Property

' Description: Internal method to get the associated security slot - see IPropertySecurity.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6002000C
Property Let Security(NoNamedArg0 As IPropertySecurity)

End Property

' Description: Checks if an object is a stub first, and if not, compares string value to see if a particular type (Avatar, etc.).
' Function Kind: Dispatch
' MemID: &H6002000E
Function IsOfType(ByVal BSTR As String) As Boolean

End Function

' Description: Converts property to string for tracing purposes
' Function Kind: Dispatch
' MemID: &H6002000F
Function ToAscii() As String

End Function

' Description: Dump human-readable form of property to debugger
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60020010
Sub Dump()

End Sub

' Description: Create a copy of an IObjectProperty
' Function Kind: Dispatch
' Flags: Restricted


Sub Clone(pobjectproperty As IObjectProperty)

End Sub

' Description: Write data into buffer for copying/remoting between client and server.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020012
Sub Marshall(ByVal dwOptions As Long, pmarshallbuffer As IMarshallBuffer)

End Sub

' Description: Read data from a buffer
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020013
Sub UnMarshall(punmarshallbuffer As IUnMarshallBuffer)

End Sub

' Description: Implemented by various interfaces IThing, IPropertyList, etc. derived from IObjectProperty to write out object state to text script.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60020014
Sub MarshallToScript(ByVal dwOptions As Long, ByVal bstrContext As String, pbAsVal As Boolean, pbstrOut As String)

End Sub

' Description: Gets object's unique ID (numbering starts with Global at 1)
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60030000
Property Get ID() As Long

End Property

' Description: Gets object's unique ID (numbering starts with Global at 1)
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030000
Property Let ID(ByVal NoNamedArg0 As Long)

End Property

' Description: Returns a string with the instance type
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030002
Property Get InstanceType() As String

End Property

' Description: Helper method to determine if an Thing is an exemplar
' Function Kind: Dispatch
' MemID: &H60030003
Property Get IsExemplar() As Boolean

End Property

' Description: Internal helper method to determine if a Thing is a wizard
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030004
Property Get IsWizard() As Boolean

End Property

' Description: System method to set type of object - e.g., Global or exemplar name
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H72
Property Let TypeInt(ByVal NoNamedArg0 As String)

End Property

' Description: System method to set owner of a thing (the avatar that creates the thing.)
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H73
Property Let OwnerInt(NoNamedArg0 As IThing)

End Property

' Description: Helper method to determine if an object is a stub (has not been completely remoted to client or has been deleted)
' Function Kind: Dispatch
' MemID: &H60030007
Property Get IsStub() As Boolean

End Property

' Description: System method to set Thing to be a stub.  Used during object creation and deletion.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H74
Property Let IsStubInt(ByVal NoNamedArg0 As Boolean)

End Property

' Description: Get Thing's exemplar
' Function Kind: Dispatch
' MemID: &H60030009
Property Get Exemplar() As IThing

End Property

' Description: System method to set Things' exemplar.  Logged and remoted.  Currently only used to change avatars into hosts
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6E
Property Let ExemplarInt(NoNamedArg0 As IThing)

End Property

' Description: Find object by name within a containing object, e.g., a room (walks contents recursively)
' Function Kind: Dispatch
' MemID: &H6003000B
Function FindWithin(ByVal bstrName As String, Optional ByVal varQuiet As Variant = 0) As IThing

End Function

' Description: Internal method to change a thing's container.  Has the ShouldExecute, ShouldRemote blocks and calls MoveIntoInt to do work.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003000C
Sub MoveIntoExt(pthing As IThing)

End Sub

' Description: Internal method to change a thing's container to pthing and fire appropriates methods, remote change, etc.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6F
Sub MoveIntoInt(pthing As IThing)

End Sub

' Description: Check if property with bstrName exists on Thing instance or exemplar, returns TRUE if so
' Function Kind: Dispatch
' MemID: &H6003000E
Function IsValidProperty(ByVal bstrName As String) As Boolean

End Function

' Description: Check if  property with bstrName exists on Thing instance, returns TRUE if so
' Function Kind: Dispatch
' MemID: &H6003000F
Function IsValidInstanceProperty(ByVal bstrName As String) As Boolean


End Function

' Description: Helper function to create and add a new property using a ProgID. Not used.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60030010
Sub CreateAndAddProperty(ByVal bstrProgID As String, ByVal bstrName As String, Optional ByVal Var As Variant)

End Sub

' Description: Adds a property with bstrName and value var to an object.  Optionally, can specify security permissions and client/server flags. Calls AddPropertyInt.
' Function Kind: Dispatch
' MemID: &H60030011
Sub AddProperty(ByVal bstrName As String, Optional ByVal Var As Variant, Optional ByVal Flags As Variant = 1, Optional ByVal permission As Variant = 66315, Optional ByVal thetype As Variant = 12, Optional ByVal bstrIID As String = "", Optional ByVal bstrType As String = "")

End Sub


' Description: Internal C++ entry point for calling AddPropertyINt
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030012
'Sub AddPropertyExt(ByVal bstrName As String, ByVal var As Variant, ByVal flags As Long, ByVal perms As Long, ByVal thetype As Long, riid As GUID, ByVal bstrType As String)
'
'End Sub


' Description: Adds a property - has ShouldExecute/Should Remote bloks.  Calls AddPropertyIntHelper to validate name, check security, do a put_Property.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H65
Sub AddPropertyInt(ByVal bstrName As String, ByVal Var As Variant, pExemplar As IThing, ByVal psbits As Long, ByVal perms As Long, ByVal thetype As Long, ByVal bstrIID As String, ByVal bstrType As String)

End Sub


' Description: Removes a property of bstrName from things's property map.  Does not allow removing of internally added Properties.
' Function Kind: Dispatch
' MemID: &H66
Sub RemoveProperty(ByVal bstrName As String)

End Sub

' Description: On client, gets a property that only exists on server using synchronous call (use with caution as this will lock the client waiting for a return value from the server.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H70
Property Get RemoteProperty(ByVal bstrName As String) As Variant

End Property

' Description: Gets a property of bstrName
' Function Kind: Dispatch
' MemID: &H67
Property Get Property(ByVal bstrName As String) As Variant

End Property

' Description: Gets a property of bstrName
' Function Kind: Dispatch
' MemID: &H67
Property Let Property(ByVal bstrName As String, ByVal NoNamedArg1 As Variant)

End Property

' Description: Internal method to get a property and exemplar once hash value has been computer.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030018
Property Get PropertyExt(ByVal bstrName As String, ByVal nHashOrig As Long, ppexemplar As IThing) As Variant

End Property

' Description: Called from get_Property to instance a particular property
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H71
Sub InstanceProperty(ByVal bstrName As String, pExemplar As IThing)

End Sub

' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6003001A
Property Get PropertySecurity(ByVal bstrName As String) As IPropertySecurity

End Property

' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003001B
Property Get PropertySecurityExt(ByVal bstrName As String, ByVal nHashOrig As Long, ppexemplar As IThing, pfInstance As Boolean) As IPropertySecurity

End Property

' Description: Returns a thing's property map of Properties defined on a particular instance
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6003001C
Property Get InstanceProperties() As IPropertyMap

End Property


' Description: Copies thing's property to ppropertymap
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003001E
Sub CopyPropertiesTo(ppropertymap As IPropertyMap)

End Sub

' Description: Check if method with bstrName exists on Thing instance or exemplar, returns TRUE if so
' Function Kind: Dispatch
' MemID: &H6003001F
Function IsValidMethod(ByVal bstrName As String) As Boolean

End Function

' Description: Check if method with bstrName exists on Thing instance, returns TRUE if so
' Function Kind: Dispatch
' MemID: &H60030020
Function IsValidInstanceMethod(ByVal bstrName As String) As Boolean

End Function

' Description: Script entry point to create a new method bstrName in module pmodule (can be named by varFuncName in module), with optional security permissions and client/server flags
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60030021
Sub CreateAndAddMethod(ByVal lFlags As Long, pmodule As IModule, ByVal bstrName As String, Optional ByVal varFuncName As Variant = "", Optional ByVal varPsbits As Variant = 0, Optional ByVal varPermissions As Variant = 67343)

End Sub

' Description: C++ entry point for creating a new method bstrName in module pmodule, called bstrFunctName within the module, with optional security permissions and client/server flags
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030022
Sub CreateAndAddMethodExt(ByVal lFlags As Long, pmodule As IModule, ByVal bstrFuncName As String, ByVal bstrMethName As String, ByVal psbits As Long, ByVal Permissions As Long)

End Sub

' Description: Script entry point todds a new method to a Thing (if method already exists with bstrName, an error is returned)
' Function Kind: Dispatch
' MemID: &H60030023
Sub AddMethod(ByVal bstrName As String, pmethod As IMethod, Optional ByVal varPsbits As Variant = 0, Optional ByVal varPermissions As Variant = 67343)

End Sub

' Description: Internal C++ method for adding a method to a Thing -  gets context and calls into AddMethodInt
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030024
Sub AddMethodExt(ByVal bstrName As String, pmethod As IMethod, ByVal psbits As Long, ByVal Permissions As Long)

End Sub

' Description: Internal C++ method for adding a method - checks security, fires event, and calls into AddMethodIntHelper to do rest of work.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H68
Sub AddMethodInt(ByVal bstrName As String, pmethod As IMethod, pExemplar As IThing, ByVal psbits As Long, ByVal Permissions As Long)

End Sub

' Description: Removes a method from a Thing's method map
' Function Kind: Dispatch
' MemID: &H69
Sub RemoveMethod(ByVal bstrName As String)

End Sub

' Description: Script entry point to get a method of bstrName, calls into get_MethodExt to check security.
' Function Kind: Dispatch
' MemID: &H6A
Property Get Method(ByVal bstrName As String) As IMethod

End Property

' Description: Script entry point to get a method of bstrName, calls into get_MethodExt to check security.
' Function Kind: Dispatch
' MemID: &H6A
Property Let Method(ByVal bstrName As String, NoNamedArg1 As IMethod)

End Property

' Description: Internal method to get a method once hash value was been computed.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030029
Property Get MethodExt(ByVal bstrName As String, ByVal nHashOrig As Long, ppexemplar As IThing) As IMethod

End Property

' Description: Computes hash key for method and calls get_MethodSecurityExt to get the security slot for a method
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6003002A
Property Get MethodSecurity(ByVal bstrName As String) As IPropertySecurity

End Property

' Description: Checks permission for accessing a method's security, looks up method and security slot
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003002B
Property Get MethodSecurityExt(ByVal bstrName As String, ByVal nHashOrig As Long, ppexemplar As IThing, pfInstance As Boolean) As IPropertySecurity

End Property

' Description: Get property map of methods defined on this particular instance.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6003002C
Property Get InstanceMethods() As IPropertyMap

End Property

' Description: Copies methods in current thing to ppropertymap
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003002E
Sub CopyMethodsTo(ppropertymap As IPropertyMap)

End Sub

' Description: Script entry point to fire an event with name strEvent and optional arguments in var
' Function Kind: Dispatch
' MemID: &H6003002F
Sub FireEvent(ByVal bstrEvent As String, Optional ByVal Var As Variant)

End Sub

' Description: C++ entry point to fire an event with name strEvent, paramters, and flags.  Has ShouldExecute, ShouldRemote blocks.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030030
'Sub FireEventExt(ByVal bstrEvent As String, pdispparams As DISPPARAMS, ByVal lFlags As Long)

' Description: Internal method - converts property list to DISPPARAMS and calls FireEventExtHelper to do work of firing event.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6C
Sub FireEventInt(ByVal bstrEvent As String, pproplist As ipropertylist, ByVal lFlags As Long)

End Sub

' Description: Calls a method with optional parameters. Calls into module interface to either invoke by name or ID.
' Function Kind: Dispatch
' MemID: &H60030032
Function InvokeMethod(ByVal bstrMethod As String, Optional ByVal Var As Variant) As Variant

End Function

' Description: Internal method to convert ppropertylist to DISPPARMS and calls InvokeMethodExtHelper to call method.
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H6B
Function InvokeMethodInt(pthing As IThing, ByVal bstrMethod As String, pproplist As ipropertylist) As Variant

End Function

' Description: Internal method to call a method with paramters.  Calls into module interface to either invoke by name or ID.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030034
'Function InvokeMethodExt(ByVal bstrMethod As String, pdispparams As DISPPARAMS) As Variant

' Description: Script entry point to call method on Thing superclass - if you overwrite a system-defined method, you should then call the underlying super method
' Function Kind: Dispatch
' MemID: &H60030035
Function InvokeSuperMethod(Optional ByVal Var As Variant) As Variant

End Function

' Description: C++ entry point to call method on Thing superclass - - if you overwrite a system-defined method, you should then call the underlying super method
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030036
'Function InvokeSuperMethodExt(Optional pdispparams As DISPPARAMS) As Variant

' Description: Get current method name to determine status.  Not used.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030037
Property Get CurrentMethodName() As String

End Property

' Description: Get current method name to determine status.  Not used.
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030037
Property Let CurrentMethodName(ByVal NoNamedArg0 As String)

End Property

' Description: Get name of Thing
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030039
Property Get name() As String

End Property

' Description: Get name of Thing
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030039
Property Let name(ByVal NoNamedArg0 As String)

End Property

' Description: Get text description of Thing
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003003B
Property Get Description() As String

End Property

' Description: Get text description of Thing
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003003B
Property Let Description(ByVal NoNamedArg0 As String)

End Property

' Description: Returns a property list of things that have been moved into the the current thing
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003003D
Property Get Contents() As ipropertylist
  '  If thisContents Is Nothing Then
 '       enum_model
   ' End If
'    Set Contents = thisContents
End Property

' Description: Returns the the object the current thing is in (e.g., a room)
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003003E
Property Get Container() As IThing

End Property

' Description: Returns the the object the current thing is in (e.g., a room)
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003003E
Property Let Container(NoNamedArg0 As IThing)

End Property

' Description: Returns the avatar that created a thing (avatars have NULL owners)
' Function Kind: Dispatch
' MemID: &H60030040
Property Get Owner() As IThing

End Property

' Description: Script entry point to check thing's Properties map for a specific permission(PS_ADD, PS_READ, PS_WRITE)
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60030041
Function CheckPropertiesSecurity(ByVal ulFlags As Long) As Boolean

End Function

' Description: Script entry point to check thing's methods map for a specific permission(PS_ADD, PS_READ, PS_WRITE)
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60030042
Function CheckMethodsSecurity(ByVal ulFlags As Long) As Boolean


End Function


' Description: Internal entry point to check thing's Properties map for a specific permission(PS_ADD, PS_READ, PS_WRITE)
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030043
Sub CheckPropertiesSecurityExt(ByVal ulFlags As Long)

End Sub

' Description: Internal entry point to check thing's methods map for a specific permission(PS_ADD, PS_READ, PS_WRITE)
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030044
Sub CheckMethodsSecurityExt(ByVal ulFlags As Long)

End Sub

' Description: Script entry point to check a thing's property for a specific permission (PS_ADD, PS_READ, PS_WRITE)
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60030045
Function CheckPropertySecurity(ByVal bstrName As String, ByVal ulFlags As Long) As Boolean

End Function


' Description: Script entry point to check a thing's method for a specific permission (PS_ADD, PS_READ, PS_WRITE)
' Function Kind: Dispatch
' Flags: Hidden
' MemID: &H60030046
Function CheckMethodSecurity(ByVal bstrName As String, ByVal ulFlags As Long) As Boolean

End Function

' Description: Internal entry point to check a thing's property for a specific permission (PS_ADD, PS_READ, PS_WRITE)
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030047
Sub CheckPropertySecurityExt(ByVal bstrName As String, ByVal ulFlags As Long)

End Sub

' Description: Internal entry point to check a thing's method for a specific permission (PS_ADD, PS_READ, PS_WRITE)
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030048
Sub CheckMethodSecurityExt(ByVal bstrName As String, ByVal ulFlags As Long)

End Sub

' Description: Method called to fire property event change: OnXXXChanged
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030049
Sub NotifyObjectPropertyChanged(pprop As IObjectProperty, ByVal lHint As Long, ByVal varHintData As Variant)

End Sub

' Description: Method called to fire method event change: OnXXXChanged
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003004A
Sub NotifyMethodChanged(pmethod As IMethod, ByVal lHint As Long, ByVal varHintData As Variant)

End Sub

' Description: Internal helper method to add a property with bstrName name and a string value
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003004B
Sub AddString(ByVal bstrName As String, ByVal BSTR As String)

End Sub

' Description: Internal helper method to add a property with bstrName name and an object property value
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003004C
Sub AddObjectProperty(ByVal bstrName As String, pobjectproperty As IObjectProperty)

End Sub

' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003004D
'Sub AddObjectPropertyExt(ByVal bstrName As String, IID As GUID, pobjectproperty As IObjectProperty)

' Description: Internal helper method to add a property with bstrName name and a thing value
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003004E
Sub AddThing(ByVal bstrName As String, pthing As IThing)

End Sub

' Description: Internal helper method to add a property with bstrName name and a boolean value
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003004F
Sub AddBOOL(ByVal bstrName As String, ByVal boolval As Boolean)

End Sub

' Description: Internal helper method to add a property with bstrName name and a long value
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030050
Sub AddLong(ByVal bstrName As String, ByVal lval As Long)

End Sub

' Description: Internal helper method to add a property with bstrName name and a short value
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030051
Sub AddShort(ByVal bstrName As String, ByVal sval As Integer)

End Sub

' Description: Internal helper method to add a property with bstrName name and a dispatch value
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030052
Sub AddDispatch(ByVal bstrName As String, ByVal pdisp As Object)

End Sub

' Description: Internal method to return value of a string property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030053
'Property Get String (ByVal bstrName As String) As String

' Description: Internal method to return value of a string property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030053
'Property Let String (ByVal bstrName As String, ByVal NoNamedArg1 As String)

' Description: Internal method to return value of a an object property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030055
Property Get ObjectProperty(ByVal bstrName As String) As IObjectProperty

End Property

' Description: Internal method to return value of a an object property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030055
Property Let ObjectProperty(ByVal bstrName As String, NoNamedArg1 As IObjectProperty)

End Property

' Description: Internal method to return value of a object property with bstrName name and specific IID
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030057
'Property Get ObjectPropertyExt(ByVal bstrName As String, IID As GUID) As IObjectProperty

' Description: Internal method to return value of a thing property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030058
'Property Get Thing(ByVal bstrName As String) As IThing

'End Property

' Description: Internal method to return value of a thing property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030058
'Property Let Thing(ByVal bstrName As String, NoNamedArg1 As IThing)

'End Property

' Description: Internal method to return value of a boolean property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003005A
'Property Get BOOL(ByVal bstrName As String) As Boolean

'End Property

' Description: Internal method to return value of a boolean property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003005A
'Property Let BOOL(ByVal bstrName As String, ByVal NoNamedArg1 As Boolean)

' Description: Internal method to return value of a long property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003005C
'Property Get Long (ByVal bstrName As String) As Long

' Description: Internal method to return value of a long property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003005C
'Property Let Long (ByVal bstrName As String, ByVal NoNamedArg1 As Long)

' Description: Internal method to return value of a short property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003005E
Property Get Short(ByVal bstrName As String) As Integer

End Property

' Description: Internal method to return value of a short property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H6003005E
Property Let Short(ByVal bstrName As String, ByVal NoNamedArg1 As Integer)

End Property

' Description: Internal method to return value of a dispatch property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030060
Property Get Dispatch(ByVal bstrName As String) As Object

End Property

' Description: Internal method to return value of a dispatch property with bstrName name
' Function Kind: Dispatch
' Flags: Restricted
' MemID: &H60030060
Property Let Dispatch(ByVal bstrName As String, ByVal NoNamedArg1 As Object)

End Property


