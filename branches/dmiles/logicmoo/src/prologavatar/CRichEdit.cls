VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CRichEdit"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private hInstRE As Long
Private hWndDoc As Long
Private ITextDoc As ITextDocument

Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, lParam As IUnknown) As Long
Private Declare Function CreateWindowEx Lib "user32" Alias "CreateWindowExA" ( _
      ByVal dwExStyle As Long, _
      ByVal lpClassName As String, _
      ByVal lpWindowName As String, _
      ByVal dwStyle As Long, _
      ByVal X As Long, _
      ByVal Y As Long, _
      ByVal nWidth As Long, _
      ByVal nHeight As Long, _
      ByVal hWndParent As Long, _
      ByVal hMenu As Long, _
      ByVal hInstance As Long, _
      ByVal lpParamas As Long) As Long

Private Declare Function DestroyWindow Lib "user32" (ByVal hWnd As Long) As Long

Private Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal Lib As String) As Long
Private Declare Function FreeLibrary Lib "kernel32" (ByVal Lib As Long) As Long

Const WS_EX_CLIENTEDGE = &H200&
Const WS_CHILD = &H40000000
Const WS_VISIBLE = &H10000000
Const WS_VSCROLL = &H200000
Const WS_HSCROLL = &H100000
Const ES_MULTILINE = &H4&
Const ES_AUTOVSCROLL = &H40&
Const ES_AUTOHSCROLL = &H80&

Const EM_GETOLEINTERFACE = &H400 + 60

Public Property Get Doc() As ITextDocument
   Set Doc = ITextDoc
End Property


Public Property Get hWnd() As Long
   hWnd = hWndDoc
End Property

Public Sub Init(ByVal hWnd As Long)
Dim IU As IUnknown

   ' Create the RichEdit window
   hWndDoc = CreateWindowEx(WS_EX_CLIENTEDGE, _
         "RichEdit20A", "", _
         WS_VISIBLE Or WS_CHILD Or _
         ES_MULTILINE Or WS_HSCROLL Or _
         ES_AUTOHSCROLL Or WS_VSCROLL Or ES_AUTOVSCROLL, _
         0, 0, 0, 0, hWnd, 0, App.hInstance, 0)
   
   If hWndDoc Then
         
      ' Get the Ole object
      SendMessage hWndDoc, EM_GETOLEINTERFACE, 0, IU
      
      ' Get the ITextDocument interface
      Set ITextDoc = IU
      
   Else
   
      Err.Raise vbObjectError, "CRichEdit", "Cannot create RichEdit window"
      
   End If

End Sub


Private Sub Class_Initialize()

   hInstRE = LoadLibrary("riched20")
   
   If hInstRE = 0 Then Err.Raise vbObjectError + 1, "CRichEdit", "Cannot load Riched20.dll"
   
End Sub

Private Sub Class_Terminate()

   ' Release the ITextDocument
   Set ITextDoc = Nothing
   
   ' Destroy the window
   DestroyWindow hWndDoc
   
   FreeLibrary hInstRE
   
End Sub




