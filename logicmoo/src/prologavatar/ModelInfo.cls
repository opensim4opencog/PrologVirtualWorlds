VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ModelInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Public memberName As String
Public Parameters As Parameters
Public CallConv As CallConvs
Public InvokeKind As InvokeKinds
Public modelkind As ModelKinds
Public ReturnVARType As VarTypeInfo
Public SiteName As SiteNames
Public Value As Variant

Private Sub Class_Initialize()
memberName = "unknown"
'Parameters = Empty
CallConv = CC_CDECL
SiteName = SiteNames.SITENAME_UNBOUND
InvokeKind = InvokeKinds.INVOKE_UNKNOWN
modelkind = MODELKIND_UNKNOWN
Value = Empty
'ReturnVARType = tli.VT_VOID
End Sub
