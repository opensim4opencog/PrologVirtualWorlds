VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "ConnectionPoint"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Implements IConnectionPoint

Dim m_Parent As IConnectionPointContainer
Dim m_Connections As Collection
Dim m_IID As UUID

Friend Sub Init(ByVal Parent As IConnectionPointContainer, IID As UUID)

   Set m_Parent = Parent
   m_IID = IID
   
End Sub


Public Sub RaiseEvents(ByVal EventName As String, ParamArray Params() As Variant)

End Sub

Friend Sub ReleaseParent()
   Set m_Parent = Nothing
End Sub

Private Function IConnectionPoint_Advise(ByVal pObjetPtr As Object) As Long
   
   IConnectionPoint_Advise = ObjPtr(pObjetPtr)
   
   m_Connections.Add pObjetPtr, CStr(ObjPtr(pObjetPtr))
   
End Function

Private Function IConnectionPoint_EnumConnections() As ITLib.IEnumConnections

   Err.Raise E_NOTIMPL
   
End Function


Private Sub IConnectionPoint_GetConnectionInterface(piid As ITLib.UUID)

   piid = m_IID
   
End Sub


Private Function IConnectionPoint_GetConnectionPointContainer() As ITLib.IConnectionPointContainer

   Set IConnectionPoint_GetConnectionPointContainer = m_Parent
   
End Function


Private Sub IConnectionPoint_Unadvise(ByVal dwCookie As Long)

   m_Connections.Remove CStr(dwCookie)
   
End Sub


