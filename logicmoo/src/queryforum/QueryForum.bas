Attribute VB_Name = "MDIPrologVWMOD"
Option Explicit
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Public Declare Function WaitForSingleObject Lib "kernel32" (ByValhHandle As Long, ByVal dwMilliseconds As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (pDest As Any, pSource As Any, ByVal ByteLen As Long)
Public Declare Function StringFromGUID2 Lib "ole32.dll" (rguid As Any, ByVal lpstrClsId As Long, ByVal cbMax As Integer) As Long
Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Any) As Long
Public Declare Function OSWinHelp% Lib "user32" Alias "WinHelpA" (ByVal hwnd&, ByVal HelpFile$, ByVal wCommand%, dwData As Any)

Const EM_UNDO = &HC7

Public Type DISPPARAMS
             rgvarg As Variant
             rgdispidNameArgs As Long 'dispid
             cArgs As Long
             cNamedArgs As Long
End Type


'VARIANTARG FAR *rgvarg;
'DISPID FAR *rgdispidNameArgs;
'unsigned int cArgs;
'unsigned int cNamedArgs;
'} DISPPARAMS;

Public Type PROCESS_INFORMATION
      hProcess As Long
      hThread As Long
      dwProcessID As Long
      dwThreadID As Long
   End Type
   

Public Type STARTUPINFO
      cb As Long
      lpReserved As String
      lpDesktop As String
      lpTitle As String
      dwX As Long
      dwY As Long
      dwXSize As Long
      dwYSize As Long
      dwXCountChars As Long
      dwYCountChars As Long
      dwFillAttribute As Long
      dwFlags As Long
      wShowWindow As Integer
      cbReserved2 As Integer
      lpReserved2 As Long
      hStdInput As Long
      hStdOutput As Long
      hStdError As Long
   End Type

Public Type uGUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(7) As Byte
End Type

Public Declare Function CreateProcessA Lib "kernel32" (ByVal _
      lpApplicationName As Long, ByVal lpCommandLine As String, ByVal _
      lpProcessAttributes As Long, ByVal lpThreadAttributes As Long, _
      ByVal bInheritHandles As Long, ByVal dwCreationFlags As Long, _
      ByVal lpEnvironment As Long, ByVal lpCurrentDirectory As Long, _
      lpStartupInfo As STARTUPINFO, lpProcessInformation As _
      PROCESS_INFORMATION) As Long

Public Declare Function CloseHandle Lib "kernel32" _
      (ByVal hObject As Long) As Long

Public Declare Function GetExitCodeProcess Lib "kernel32" _
      (ByVal hProcess As Long, lpExitCode As Long) As Long

Public Const NORMAL_PRIORITY_CLASS = &H20&
Public Const INFINITE = -1&

Public Const IID_IDispatch = "{00020400-0000-0000-C000-000000000046}"
Public Const IID_IPersistStorage = "{0000010A-0000-0000-C000-000000000046}"
Public Const IID_IPersistStream = "{00000109-0000-0000-C000-000000000046}"
Public Const IID_IPersistPropertyBag = "{37D84F60-42CB-11CE-8135-00AA004BB851}"

Public Const INTERFACESAFE_FOR_UNTRUSTED_CALLER = &H1   ' Caller of interface may be untrusted
Public Const INTERFACESAFE_FOR_UNTRUSTED_DATA = &H2     ' Data passed into interface may be untrusted

Public Const E_NOINTERFACE = &H80004002 ' No such interface supported
Public Const E_FAIL = &H80004005        ' Unspecified error

Public Const MAX_GUIDLEN = 40

Public Const WorkPath = "C:\Program Files\pl\nephlab\msr\output\"

Public m_fSafeForScripting      As Boolean
Public m_fSafeForInitializing   As Boolean
Public Const SWP_NOMOVE = 2
Public Const SWP_NOSIZE = 1
Public Const FLAGS = SWP_NOMOVE Or SWP_NOSIZE
Public Const HWND_TOPMOST = -1
Public Const HWND_NOTOPMOST = -2

Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long

Type POINTAPI
        X As Long
        Y As Long
End Type
Declare Function GetCaretPos Lib "user32" (lpPoint As POINTAPI) As Long

Public Enum QueryLanguages
    NullQuery = 0
    VBScriptQuery = 1
    MSBNQuery = 2
    RAIDQuery = 3
    PLQuery = 4
    MOOQuery = 5
    WebQuery = 6
    JScriptQuery = 7
    XMLQuery = 8
    SQLQuery = 9
    NLPQuery = 10
    SwiBotQuery = 11
    TelnetQuery = 12
    InConcertQuery = 13
    VWorldQuery = 14
    UserQuery = 15
    ScriptQuery = 16
End Enum

'Dim TestObj() As QueryActorsCtl
Public fMainForm As frmMain


Public Function NewRegistrant(Name, Language, Optional Resources = "")

    Dim newQuery As QueryObject
    Set newQuery = New QueryObject
    If Name <> "" Then
 '       newQuery.Caption = Name & ":" & Language & ":" & Resources
  '      newQuery.NativeLanguage = Language
       ' newQuery.FriendlyName = Name
    End If
    newQuery.Show

End Function

Public Function RegistrantID(ByVal Name)
    Dim Item As Long
        Name = LCase(Name)
      '  For Item = 0 To frmQueryTester.TargetObject.ListCount - 1
       '     If LCase(frmQueryTester.TargetObject.List(Item)) = Name Then RegistrantID = Item: Exit Function
        ' Next Item
        RegistrantID = 0
End Function

Public Function Registrant(ByVal Item)
   '     Registrant = frmQueryTester.TargetObject.List(Item)
End Function


Function RegistrantCount()
 '   RegistrantCount = frmQueryTester.TargetObject.ListCount
End Function

Function Msg(DestinationName, SourceName, Message) As Boolean
   Dim i As Long
   For i = 1 To VB.Forms.Count - 1
   
        If Forms(i).MDIChild = True And InStr(Forms(i).Caption, DestinationName) Then
            If Not (InStr(Forms(i).Caption, SourceName)) Then
                Forms(i).InputBuffer DestinationName, SourceName, Message
            'do give it the message
            End If
         End If
    Next i
End Function

Public Sub WriteToUser(Who, Text)

 '   frmQueryTester.OutputWindow.Text = frmQueryTester.OutputWindow.Text + Who + ": " + Text + vbCrLf
   ' frmQueryTester.OutputWindow.SelStart = Len(frmQueryTester.OutputWindow.Text)

End Sub

Sub LoadResStrings(frm As Form)
    On Error Resume Next

    Dim ctl As Control
    Dim obj As Object
    Dim fnt As Object
    Dim sCtlType As String
    Dim nVal As Integer


    'set the form's caption
    frm.Caption = LoadResString(CInt(frm.Tag))
    

    'set the font
    Set fnt = frm.Font
    fnt.Name = LoadResString(20)
    fnt.Size = CInt(LoadResString(21))
    

    'set the controls' captions using the caption
    'property for menu items and the Tag property
    'for all other controls
    For Each ctl In frm.Controls
        Set ctl.Font = fnt
        sCtlType = TypeName(ctl)
        If sCtlType = "Label" Then
            ctl.Caption = LoadResString(CInt(ctl.Tag))
        ElseIf sCtlType = "Menu" Then
            ctl.Caption = LoadResString(CInt(ctl.Caption))
        ElseIf sCtlType = "TabStrip" Then
            For Each obj In ctl.Tabs
                obj.Caption = LoadResString(CInt(obj.Tag))
                obj.ToolTipText = LoadResString(CInt(obj.ToolTipText))
            Next
        ElseIf sCtlType = "Toolbar" Then
            For Each obj In ctl.Buttons
                obj.ToolTipText = LoadResString(CInt(obj.ToolTipText))
            Next
        ElseIf sCtlType = "ListView" Then
            For Each obj In ctl.ColumnHeaders
                obj.Text = LoadResString(CInt(obj.Tag))
            Next
        Else
            nVal = 0
            nVal = Val(ctl.Tag)
            If nVal > 0 Then ctl.Caption = LoadResString(nVal)
            nVal = 0
            nVal = Val(ctl.ToolTipText)
            If nVal > 0 Then ctl.ToolTipText = LoadResString(nVal)
        End If
    Next


End Sub

'End Sub

Public Function GetNextClassDebugID() As Long
    'class ID generator
    Static lClassDebugID As Long
    lClassDebugID = lClassDebugID + 1
    GetNextClassDebugID = lClassDebugID
End Function

Function ReverseChars(ByVal Source As String) As String
Dim i As Long, Temp As String
For i = 1 To Len(Source)
Temp = Mid(Source, i, 1) & Temp
Next i
ReverseChars = Temp
End Function

Public Sub Void(foo)

End Sub


Function GetHead(ByVal Source As String) As String
Dim i As Long
i = InStr(Source, vbLf)
If i < 1 Then GetHead = Source Else GetHead = Left(Source, i - 1)
End Function

Function GetTail(ByVal Source As String) As String
Dim i As Long
i = InStr(Source, vbLf)
If i < 1 Then GetTail = "" Else GetTail = Mid(Source, i + 1)
End Function
   
   
Public Function ExecCmd(cmdline$)
      Dim proc As PROCESS_INFORMATION
      Dim start As STARTUPINFO
      Dim ret As Long

      ' Initialize the STARTUPINFO structure:
      start.cb = Len(start)

      ' Start the shelled application:
      ret = CreateProcessA(0&, cmdline$, 0&, 0&, 1&, _
         NORMAL_PRIORITY_CLASS, 0&, 0&, start, proc)


      ' Wait for the shelled application to finish:
         ret = WaitForSingleObject(proc.hProcess, INFINITE)
         Call GetExitCodeProcess(proc.hProcess, ret)
         Call CloseHandle(proc.hThread)
         Call CloseHandle(proc.hProcess)
         ExecCmd = ret
End Function



Public Function countchars(Source, char) As Long
Dim i As Long, output As Long
output = 0
For i = 1 To Len(Source)
If Mid(Source, i, 1) = char Then output = output + 1
Next i
countchars = output
End Function

Public Function ResolveLanguage(ByVal vData As String) As String
    vData = VBA.LCase(vData)
    
    vData = Replace(vData, "plquery", "prologquery")
    vData = Replace(vData, "query", "")
    If vData = "english" Then vData = "nlp"
    If vData = "nlpwin" Then vData = "nlp"
    If vData = "script" Then vData = "vbscript"
    If vData = "user" Then vData = "ui"
    ResolveLanguage = vData
End Function


