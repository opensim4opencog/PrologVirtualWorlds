VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "TestClass"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Dim m_InvokeAddr As Long
Dim m_GIONAddr As Long

Implements IDispatchCallback

Dim m_AllowShowMessage As Boolean
Dim m_TestProperty As String

Private Enum DISPIDs
   DISPID_SHOWMESSAGE = &H1&
   DISPID_TESTPROP
End Enum

Public Property Let AllowShowMessage(ByVal Allow As Boolean)

   m_AllowShowMessage = Allow
   
End Property

Public Property Get AllowShowMessage() As Boolean
   
   AllowShowMessage = m_AllowShowMessage
   
End Property

Private Sub pvShowMessage(ByVal Msg As String)

   MsgBox Msg
   
End Sub

Private Sub Class_Initialize()

   ' Hook the IDispatch interface
   HookDispatch Me
   
End Sub

Private Sub Class_Terminate()

   ' Remove the IDispatch hook
   UnHookDispatch Me
  
End Sub

Private Function IDispatchCallback_GetDISPID(ByVal name As String) As Long
Dim lIdx As Long

   Select Case LCase$(name)
      
      Case "showmessage"
      
         If m_AllowShowMessage Then
            IDispatchCallback_GetDISPID = DISPID_SHOWMESSAGE
         Else
            IDispatchCallback_GetDISPID = DISPID_UNKNOWN
         End If
      
      Case "testproperty"
         IDispatchCallback_GetDISPID = DISPID_TESTPROP
      
      Case Else
         IDispatchCallback_GetDISPID = DISPID_UNKNOWN
         
   End Select
   
End Function

Private Property Let IDispatchCallback_GIONAddr(ByVal RHS As Long)

   m_GIONAddr = RHS
   
End Property


Private Property Get IDispatchCallback_GIONAddr() As Long

   IDispatchCallback_GIONAddr = m_GIONAddr
   
End Property

Private Property Let IDispatchCallback_InvokeAddr(ByVal RHS As Long)

   m_InvokeAddr = RHS
   
End Property

Private Property Get IDispatchCallback_InvokeAddr() As Long

   IDispatchCallback_InvokeAddr = m_InvokeAddr
   
End Property


Private Function IDispatchCallback_InvokeMethod(ByVal InvokeType As ITLib.InvokeKind, ByVal DISPID As Long, Params() As Variant, Result As Variant) As Long
Dim Count As Long

   On Error Resume Next
   
   ' Get the parameters count
   Count = UBound(Params) + 1
   
   Select Case DISPID
      
      Case DISPID_SHOWMESSAGE
      
         ' Show message has only 1
         ' parameter
         If Count = 1 Then
         
            If VarType(Params(0)) = vbString Then
            
               ' All parameters are OK.
               ' Call the sub.
               pvShowMessage Params(0)
               
            Else
            
               ' Type mismatch error
               IDispatchCallback_InvokeMethod = DISP_E_TYPEMISMATCH
               
            End If
            
         Else
         
            ' Return Wrong number of parameters...
            IDispatchCallback_InvokeMethod = DISP_E_BADPARAMCOUNT
            
         End If
      
      Case DISPID_TESTPROP
      
         If (InvokeType And INVOKE_PROPERTYGET) = INVOKE_PROPERTYGET Or (InvokeType And INVOKE_FUNC) = INVOKE_FUNC Then
         
            ' Property Get
            If Count = 0 Then
            
               ' Return the property value
               Result = m_TestProperty
               
            Else
            
               ' Return Wrong number of parameters...
               IDispatchCallback_InvokeMethod = DISP_E_BADPARAMCOUNT
               
            End If
         
         ElseIf InvokeType = INVOKE_PROPERTYPUT Then
         
            ' Parameter 1 contains the new value
            If Count = 1 Then
            
               ' TestProperty is a String
               If VarType(Params(0)) = vbString Then
               
                  m_TestProperty = Params(0)
               
               Else
               
                  ' Type mismatch error
                  IDispatchCallback_InvokeMethod = DISP_E_TYPEMISMATCH

               End If
               
            Else
               
               ' Return Wrong number of parameters...
               IDispatchCallback_InvokeMethod = DISP_E_BADPARAMCOUNT
               
            End If
         
         Else
            IDispatchCallback_InvokeMethod = DISP_E_UNKNOWNNAME
         End If
         
   End Select
   

End Function



