VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "EnumConnectionPoints"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Implements IEnumConnectionPoints

Dim m_Parent As ICPCCallback

Sub Init(ByVal Parent As ICPCCallback)
   
   Set m_Parent = Parent
   
End Sub


Private Function IEnumConnectionPoints_Clone() As ITLib.IEnumConnectionPoints

End Function

Private Function IEnumConnectionPoints_Next(ByVal cConnections As Long, rgpcn As ITLib.IConnectionPoint) As Long

End Function


Private Sub IEnumConnectionPoints_Reset()

End Sub


Private Sub IEnumConnectionPoints_Skip(ByVal cConnections As Long)

End Sub


