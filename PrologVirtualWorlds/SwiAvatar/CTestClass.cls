VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "CTestClass"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'*********************************************************************************************
'
' Implementing source interfaces
'
' Test class
'
'*********************************************************************************************
'
' Author: Eduardo A. Morcillo
' E-Mail: e_morcillo@yahoo.com
' Web Page: http://www.domaindlx.com/e_morcillo
'
' Distribution: You can freely use this code in your own applications but you
'               can't publish this code in a web site, online service, or any
'               other media, without my express permission.
'
' Usage: at your own risk.
'
' Tested on: Windows 98 + VB5
'
' History:
'           06/03/2000 - This code was released
'
'*********************************************************************************************
Option Explicit

' ==== Default source interface ====

' Source interface IID
Dim m_DefaultIID As UUID

' ==== Added source interface ====

' Connection point object
Dim m_NewEventsCP As stdole.IUnknown

' Source inteface ID
Dim m_NewEventsIID As UUID

' ==== IConectionPointContainer ====

' Implement the callback interface.
' The new functions on ICPC call
' this interface.
Implements ICPCCallback

' ==== Default source interface events ====

' Event on the default interface
Event Event1()

' ==== Events DISPIDs ====

' The code doesn't work with event
' names. Use the DISPIDs instead.
Const DISPID_EVENT2 = 1&
Const DISPID_EVENT3 = 2&

'*********************************************************************************************
'
' RaiseEvents
'
' Raise events from this object
'
'*********************************************************************************************
Sub RaiseTheEvents(ByVal Index As Long)

   Select Case Index
      
      Case 1
      
         ' Raise Event1 from the
         ' default interface
         RaiseEvent Event1
         
      Case 2
      
         ' Raise Event2 from the
         ' _Events interface.
         RaiseEvents m_NewEventsCP, DISPID_EVENT2
         
      Case 3
      
         ' Raise Event3 from the
         ' _Events interface.
         RaiseEvents m_NewEventsCP, DISPID_EVENT3, "Hello World!"
         
   End Select
   
End Sub

'*********************************************************************************************
'
' Class_Initialize
'
'*********************************************************************************************
Private Sub Class_Initialize()

   ' Initialize the source interface IID
   ' {7b710d21-38f0-11d4-b999-ba2af5032f4d}
   With m_NewEventsIID
      .Data1 = &H7B710D21
      .Data2 = &H38F0
      .Data3 = &H11D4
      .Data4(0) = &HB9
      .Data4(1) = &H99
      .Data4(2) = &HBA
      .Data4(3) = &H2A
      .Data4(4) = &HF5
      .Data4(5) = &H3
      .Data4(6) = &H2F
      .Data4(7) = &H4D
   End With
   
   ' Create the new connection
   ' point object
   Set m_NewEventsCP = CreateConnectionPoint(Me, m_NewEventsIID)
   
   ' Replace the IConnectionPointContainer
   ' members. Members are replaced
   ' only when the first instance is created.
   If cTestClass_lInstanceCounter = 0 Then
      ReplaceCPC Me
   End If

   ' Increment the instance counter
   cTestClass_lInstanceCounter = cTestClass_lInstanceCounter + 1
   
End Sub

'*********************************************************************************************
'
' Class_Terminate
'
'*********************************************************************************************
Private Sub Class_Terminate()
   
   ' Decrement the instance counter
   cTestClass_lInstanceCounter = cTestClass_lInstanceCounter - 1
   
   ' Restore the IConnectionPointContainer
   ' members if this is the last instance.
   If cTestClass_lInstanceCounter = 0 Then RestoreCPC Me
   
   Set m_NewEventsCP = Nothing
   
End Sub

'*********************************************************************************************
'
' ICPCCallback_FindConnectionPoint
'
' Returns a connection point for the requested
' interface ID.
'
'*********************************************************************************************
Private Function ICPCCallback_FindConnectionPoint(IID As ITLib.UUID, CP As ITLib.IConnectionPoint) As Long

   ' Compare the ID with the
   ' default interface ID
   If IsEqualGUID(IID, m_DefaultIID) Then
   
      Set CP = cTestClass_DefaultCP
      
   ' Compare the ID with the
   ' new interface ID
   ElseIf IsEqualGUID(IID, m_NewEventsIID) Then
   
      Set CP = m_NewEventsCP
      
   Else
   
      ' There's no connection point
      ' for the requested ID

      ICPCCallback_FindConnectionPoint = CONNECT_E_NOCONNECTION
      
   End If
   
End Function

'*********************************************************************************************
'
' ICPCCallback_GetPointers
'
' Returns the original function pointers.
'
'*********************************************************************************************
Private Sub ICPCCallback_GetPointers(FindPtr As Long, EnumPtr As Long)
   
   FindPtr = cTestClass_lOriginalFind
   EnumPtr = cTestClass_lOriginalEnum

End Sub

'*********************************************************************************************
'
' ICPCCallback_SavePointers
'
' Saves the original function pointers and
' the connection point object of the default
' source interface.
'
'*********************************************************************************************
Private Sub ICPCCallback_SavePointers(ByVal DefaultDCP As ITLib.IConnectionPoint, ByVal FindPtr As Long, ByVal EnumPtr As Long)

   If Not DefaultDCP Is Nothing Then
   
      Set cTestClass_DefaultCP = DefaultDCP
   
      ' Get the interface ID
      cTestClass_DefaultCP.GetConnectionInterface m_DefaultIID
      
   End If

   cTestClass_lOriginalFind = FindPtr
   cTestClass_lOriginalEnum = EnumPtr
   
End Sub
