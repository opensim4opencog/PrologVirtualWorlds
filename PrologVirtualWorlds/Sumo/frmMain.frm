VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.MDIForm Client 
   BackColor       =   &H8000000C&
   Caption         =   "Sigma Logic Engine Tester"
   ClientHeight    =   7275
   ClientLeft      =   7080
   ClientTop       =   3930
   ClientWidth     =   10890
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "MDIForm1"
   Begin MSWinsockLib.Winsock telnet 
      Left            =   3960
      Top             =   2280
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
   End
   Begin MSComctlLib.Toolbar tbToolBar 
      Align           =   1  'Align Top
      Height          =   420
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   10890
      _ExtentX        =   19209
      _ExtentY        =   741
      ButtonWidth     =   609
      ButtonHeight    =   582
      Appearance      =   1
      ImageList       =   "imlToolbarIcons"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   17
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "sum"
            Object.ToolTipText     =   "Ask/Tell"
            ImageIndex      =   9
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            ImageKey        =   "Camera"
            Style           =   3
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Help"
            Object.ToolTipText     =   "Results"
            ImageKey        =   "Camera"
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Telnet"
            Object.ToolTipText     =   "Telnet Log"
            ImageKey        =   "Telnet"
            BeginProperty ButtonMenus {66833FEC-8583-11D1-B16A-00C0F0283628} 
               NumButtonMenus  =   1
               BeginProperty ButtonMenu1 {66833FEE-8583-11D1-B16A-00C0F0283628} 
               EndProperty
            EndProperty
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Stop"
            Object.ToolTipText     =   "Stop"
            ImageKey        =   "Stop"
         EndProperty
         BeginProperty Button8 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button9 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "New"
            Object.ToolTipText     =   "New"
            ImageIndex      =   1
         EndProperty
         BeginProperty Button10 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Open"
            Object.ToolTipText     =   "Open"
            ImageKey        =   "Open"
         EndProperty
         BeginProperty Button11 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Save"
            Object.ToolTipText     =   "Save"
            ImageKey        =   "Save"
         EndProperty
         BeginProperty Button12 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button13 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Cut"
            Object.ToolTipText     =   "Cut"
            ImageKey        =   "Cut"
         EndProperty
         BeginProperty Button14 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Copy"
            Object.ToolTipText     =   "Copy"
            ImageKey        =   "Copy"
         EndProperty
         BeginProperty Button15 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Paste"
            Object.ToolTipText     =   "Paste"
            ImageKey        =   "Paste"
            BeginProperty ButtonMenus {66833FEC-8583-11D1-B16A-00C0F0283628} 
               NumButtonMenus  =   1
               BeginProperty ButtonMenu1 {66833FEE-8583-11D1-B16A-00C0F0283628} 
               EndProperty
            EndProperty
         EndProperty
         BeginProperty Button16 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button17 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Properties"
            Object.ToolTipText     =   "Sigma Options"
            ImageKey        =   "Properties"
         EndProperty
      EndProperty
      Begin VB.CheckBox ClosingApp 
         Caption         =   "ClosingApp"
         Height          =   195
         Left            =   10560
         TabIndex        =   4
         Top             =   120
         Visible         =   0   'False
         Width           =   495
      End
      Begin VB.TextBox Port 
         Enabled         =   0   'False
         Height          =   285
         Left            =   6765
         TabIndex        =   3
         Text            =   "5000"
         Top             =   60
         Width           =   735
      End
      Begin VB.TextBox Server 
         Enabled         =   0   'False
         Height          =   285
         Left            =   4755
         TabIndex        =   2
         Text            =   "JORDAN"
         Top             =   60
         Width           =   1935
      End
   End
   Begin MSComctlLib.StatusBar sbStatusBar 
      Align           =   2  'Align Bottom
      Height          =   270
      Left            =   0
      TabIndex        =   0
      Top             =   7005
      Width           =   10890
      _ExtentX        =   19209
      _ExtentY        =   476
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   3
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   13573
            Text            =   "Status"
            TextSave        =   "Status"
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   6
            AutoSize        =   2
            TextSave        =   "3/1/2001"
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   5
            AutoSize        =   2
            TextSave        =   "9:10 PM"
         EndProperty
      EndProperty
   End
   Begin MSComDlg.CommonDialog dlgCommonDialog 
      Left            =   4920
      Top             =   1440
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComctlLib.ImageList imlToolbarIcons 
      Left            =   1740
      Top             =   1350
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   13
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0442
            Key             =   "New"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0554
            Key             =   "Stop"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":09A6
            Key             =   "Open"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0AB8
            Key             =   "Save"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0BCA
            Key             =   "Print"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0CDC
            Key             =   "Cut"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0DEE
            Key             =   "Copy"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0F00
            Key             =   "Paste"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":1012
            Key             =   "Sum"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":1124
            Key             =   "Help"
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":1236
            Key             =   "Camera"
         EndProperty
         BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":1348
            Key             =   "Properties"
         EndProperty
         BeginProperty ListImage13 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":145A
            Key             =   "Telnet"
            Object.Tag             =   "Telnet"
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuServer 
      Caption         =   "&Server"
      Begin VB.Menu mnuConnect 
         Caption         =   "Connect..."
      End
      Begin VB.Menu mnuSSI 
         Caption         =   "Init Logic Engine"
      End
      Begin VB.Menu mnuServerSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuBrowse 
      Caption         =   "&Browse"
      Begin VB.Menu mnuSigmaKB 
         Caption         =   "Sigma KBs"
      End
      Begin VB.Menu mnuBrowse1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuBrosweAssertion 
         Caption         =   "Assertion"
      End
   End
   Begin VB.Menu mnuConsultation 
      Caption         =   "&Consultation"
      Begin VB.Menu mnuTurnOn 
         Caption         =   "Turn On"
      End
      Begin VB.Menu mnuSep1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuNewConsultationPred 
         Caption         =   "New Predicate"
      End
      Begin VB.Menu mnuConsultationList 
         Caption         =   "View List"
      End
   End
   Begin VB.Menu mnuProlog 
      Caption         =   "&Prolog"
      Begin VB.Menu mnuPrologConsult 
         Caption         =   "Consult a File"
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "&Editor"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open..."
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFileMRU 
         Caption         =   ""
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileMRU 
         Caption         =   ""
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileMRU 
         Caption         =   ""
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Close"
      End
      Begin VB.Menu mnuEditBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "&Save"
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "Save &As..."
      End
      Begin VB.Menu mnuFileSaveAll 
         Caption         =   "Save A&ll"
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuSep2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEditUndo 
         Caption         =   "&Undo"
      End
      Begin VB.Menu cmdClear 
         Caption         =   "Clear"
      End
      Begin VB.Menu mnuSep3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEditCut 
         Caption         =   "Cu&t"
         Shortcut        =   ^X
      End
      Begin VB.Menu mnuEditCopy 
         Caption         =   "&Copy"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuEditPaste 
         Caption         =   "&Paste"
         Shortcut        =   ^V
      End
      Begin VB.Menu mnuFileBar4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEditPasteSpecial 
         Caption         =   "Paste &Special..."
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuFileSend 
         Caption         =   "Sen&d..."
         Enabled         =   0   'False
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "&View"
      Begin VB.Menu mnuViewToolbar 
         Caption         =   "&Toolbar"
         Checked         =   -1  'True
      End
      Begin VB.Menu mnuViewStatusBar 
         Caption         =   "Status &Bar"
         Checked         =   -1  'True
      End
      Begin VB.Menu mnuViewBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewRefresh 
         Caption         =   "&Refresh"
      End
      Begin VB.Menu mnuViewOptions 
         Caption         =   "&Options..."
      End
      Begin VB.Menu mnuViewWebBrowser 
         Caption         =   "&Web Browser"
      End
   End
   Begin VB.Menu mnuTools 
      Caption         =   "&Tools"
      Begin VB.Menu mnuFilePrintPreview 
         Caption         =   "Print Pre&view"
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuFilePageSetup 
         Caption         =   "Page Set&up..."
      End
      Begin VB.Menu mnuFileBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFilePrint 
         Caption         =   "&Print..."
      End
      Begin VB.Menu mnuFileBar1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuToolsOptions 
         Caption         =   "&Options..."
      End
   End
   Begin VB.Menu mnuWindow 
      Caption         =   "&Window"
      WindowList      =   -1  'True
      Begin VB.Menu mnuWindowNewWindow 
         Caption         =   "&New Window"
      End
      Begin VB.Menu mnuWindowBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuWindowCascade 
         Caption         =   "&Cascade"
      End
      Begin VB.Menu mnuWindowTileHorizontal 
         Caption         =   "Tile &Horizontal"
      End
      Begin VB.Menu mnuWindowTileVertical 
         Caption         =   "Tile &Vertical"
      End
      Begin VB.Menu mnuWindowArrangeIcons 
         Caption         =   "&Arrange Icons"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpContents 
         Caption         =   "&Contents"
      End
      Begin VB.Menu mnuHelpSearchForHelpOn 
         Caption         =   "&Search For Help On..."
      End
   End
End
Attribute VB_Name = "Client"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Any) As Long
Const EM_UNDO = &HC7
Private Declare Function OSWinHelp% Lib "user32" Alias "WinHelpA" (ByVal hwnd&, ByVal HelpFile$, ByVal wCommand%, dwData As Any)


Const clrSep = &H80000010

Public intActive As Integer, sngLastX As Integer
Public intHover As Integer, bReDraw As Boolean, intLast As Integer

Const buttonShadow = &H80000010
Const buttonHilight = &H80000014

Dim Modes()     As typMode
Public intModes As Integer

Public strDataBuffer As String, minSize As Long
Dim strHoverCap As String





Private Sub cmdClear_Click()
    On Error Resume Next
    ActiveForm.theWindow.Text = ""
End Sub





Private Sub mnuBrosweAssertion_Click()
i$ = InputBox("Assertion Number", "Look up assertion")

If i$ <> "" Then CallProlog "sigma_X_kb(Context," & i$ & ",Assertion)."

End Sub

Private Sub mnuConnect_Click()
Load ConnectForm
ConnectForm.Show

End Sub

Private Sub mnuPredicates_Click()

End Sub

Private Sub mnuNewConsultationPred_Click()
    Load ConsultPreds
    ConsultPreds.Show
End Sub

Public Sub mnuPrologConsult_Click()
    Dim sFile As String
On Error Resume Next

    With dlgCommonDialog
        .DialogTitle = "Consult a prolog file.."
        .CancelError = False
        'ToDo: set the flags and attributes of the common dialog control
        .Filter = "Prolog Source Files (*.P)|*.P"
        .ShowOpen
        If Len(.FileName) = 0 Then
            Exit Sub
        End If
        sFile = .FileName

'    Stop
    FileSystem.FileCopy .FileName, "C:\cygwin\tks\sigma\inference_engine\resources\" & .FileTitle
    CallProlog "['../inference_engine/resources/" & .FileTitle & "']."
    End With
End Sub

Private Sub mnuSigmaKB_Click()
    Dim frmB As New frmBrowser
    frmB.StartingAddress = "http://192.168.200.145:8080/sigma"
    frmB.Show
End Sub

Private Sub mnuSSI_Click()
CallProlog "ssi."

End Sub

Private Sub mnuTurnOn_Click()
If mnuTurnOn.Caption = "Turn On" Then
        'Turn Off
        mnuTurnOn.Caption = "Turn Off"
Else
        'Turn On
        mnuTurnOn.Caption = "Turn On"
End If
End Sub

Private Sub tbToolBar_ButtonClick(ByVal Button As MSComctlLib.Button)
CommandButton Button.Key
Exit Sub

'Stop
    Select Case Button.Key
        Case "New"
            LoadNewDoc
        Case "Open"
            mnuFileOpen_Click
        Case "Save"
            mnuFileSave_Click
        Case "Print"
            mnuFilePrint_Click
        Case "Cut"
            mnuEditCut_Click
        Case "Copy"
            mnuEditCopy_Click
        Case "Paste"
            mnuEditPaste_Click
        Case "sum"
            fAskTell.Show
            fAskTell.SetFocus
        Case "Stop"
            telnet.Close
            SendTelnetLogLn "// Closed Socket"
            ParseXML
        Case "Help"
            ResponseForm.Show
            ResponseForm.SetFocus
        Case "Telnet"
            fTelnetSpy.Show
            fTelnetSpy.SetFocus

            
            
        Case "Properties"
        Options.Show vbModal, Me
    End Select
End Sub

Private Sub mnuHelpAbout_Click()
    MsgBox "Version " & App.Major & "." & App.Minor & "." & App.Revision
End Sub

Private Sub mnuHelpSearchForHelpOn_Click()
    Dim nRet As Integer


    'if there is no helpfile for this project display a message to the user
    'you can set the HelpFile for your application in the
    'Project Properties dialog
    If Len(App.HelpFile) = 0 Then
        MsgBox "Unable to display Help Contents. There is no Help associated with this project.", vbInformation, Me.Caption
    Else
        On Error Resume Next
        nRet = OSWinHelp(Me.hwnd, App.HelpFile, 261, 0)
        If Err Then
            MsgBox Err.Description
        End If
    End If

End Sub

Private Sub mnuHelpContents_Click()
    Dim frmB As New frmBrowser
    frmB.StartingAddress = "http://192.168.200.145"
    frmB.Show

End Sub


Private Sub mnuWindowArrangeIcons_Click()
    Me.Arrange vbArrangeIcons
End Sub

Private Sub mnuWindowTileVertical_Click()
    Me.Arrange vbTileVertical
End Sub

Private Sub mnuWindowTileHorizontal_Click()
    Me.Arrange vbTileHorizontal
End Sub

Private Sub mnuWindowCascade_Click()
    Me.Arrange vbCascade
End Sub

Private Sub mnuWindowNewWindow_Click()
    LoadNewDoc
End Sub

Private Sub mnuToolsOptions_Click()
    frmOptions.Show vbModal, Me
End Sub

Private Sub mnuViewWebBrowser_Click()
    Dim frmB As New frmBrowser
    frmB.StartingAddress = "http://192.168.200.145/"
    frmB.Show
End Sub

Private Sub mnuViewOptions_Click()
    frmOptions.Show vbModal, Me
End Sub

Private Sub mnuViewRefresh_Click()
    MsgBox "ActiveX Version " & App.Major & "." & App.Minor & "." & App.Revision & vbCrLf & vbCrLf & "Program Version 0.6 " & vbCrLf & vbCrLf & "by Douglas Miles 2000"


    'ToDo: Add 'mnuViewRefresh_Click' code.
'    MsgBox "Add 'mnuViewRefresh_Click' code."
    load_default_dlls
End Sub

Private Sub mnuViewStatusBar_Click()
    mnuViewStatusBar.Checked = Not mnuViewStatusBar.Checked
    sbStatusBar.Visible = mnuViewStatusBar.Checked
End Sub

Private Sub mnuViewToolbar_Click()
    mnuViewToolbar.Checked = Not mnuViewToolbar.Checked
    tbToolBar.Visible = mnuViewToolbar.Checked
End Sub

Private Sub mnuEditPasteSpecial_Click()
    'ToDo: Add 'mnuEditPasteSpecial_Click' code.
    MsgBox "Add 'mnuEditPasteSpecial_Click' code."
End Sub

Private Sub mnuEditPaste_Click()
    On Error Resume Next
    ActiveForm.theWindow.SelText = Clipboard.GetText

End Sub

Private Sub mnuEditCopy_Click()
    On Error Resume Next
    Clipboard.SetText ActiveForm.theWindow.SelText

End Sub

Private Sub mnuEditCut_Click()
    On Error Resume Next
    Clipboard.SetText ActiveForm.theWindow.SelText
    ActiveForm.theWindow.SelText = vbNullString

End Sub

Private Sub mnuEditUndo_Click()
    'ToDo: Add 'mnuEditUndo_Click' code.
    MsgBox "Add 'mnuEditUndo_Click' code."
End Sub


Private Sub mnuFileSend_Click()
    'ToDo: Add 'mnuFileSend_Click' code.
'    MsgBox "Add 'mnuFileSend_Click' code."
End Sub

Private Sub mnuFilePrint_Click()
    On Error Resume Next
    If ActiveForm Is Nothing Then Exit Sub
    

    With dlgCommonDialog
        .DialogTitle = "Print"
        .CancelError = True
        .Flags = cdlPDReturnDC + cdlPDNoPageNums
        If ActiveForm.theWindow.SelLength = 0 Then
            .Flags = .Flags + cdlPDAllPages
        Else
            .Flags = .Flags + cdlPDSelection
        End If
        .ShowPrinter
        If Err <> MSComDlg.cdlCancel Then
            ActiveForm.theWindow.SelPrint .hDC
        End If
    End With

End Sub

Private Sub mnuFilePrintPreview_Click()
    'ToDo: Add 'mnuFilePrintPreview_Click' code.
    MsgBox "Add 'mnuFilePrintPreview_Click' code."
End Sub

Private Sub mnuFilePageSetup_Click()
    On Error Resume Next
    With dlgCommonDialog
        .DialogTitle = "Page Setup"
        .CancelError = True
        .ShowPrinter
    End With

End Sub

Private Sub mnuFileProperties_Click()
    'ToDo: Add 'mnuFileProperties_Click' code.
    MsgBox "Add 'mnuFileProperties_Click' code."
End Sub

Private Sub mnuFileSaveAll_Click()
    'ToDo: Add 'mnuFileSaveAll_Click' code.
    MsgBox "Add 'mnuFileSaveAll_Click' code."
End Sub

Private Sub mnuFileSaveAs_Click()
    Dim sFile As String
    

    If ActiveForm Is Nothing Then Exit Sub
    

    With dlgCommonDialog
        .DialogTitle = "Save As"
        .CancelError = False
        'ToDo: set the flags and attributes of the common dialog control
        .Filter = "All Files (*.*)|*.*"
        .ShowSave
        If Len(.FileName) = 0 Then
            Exit Sub
        End If
        sFile = .FileName
    End With
    ActiveForm.Caption = sFile
    ActiveForm.theWindow.SaveFile sFile

End Sub

Private Sub mnuFileSave_Click()
    Dim sFile As String
 On Error Resume Next
    If InStr(ActiveForm.Caption, "\") = 0 Then
        With dlgCommonDialog
            .DialogTitle = "Save"
            .CancelError = False
            'ToDo: set the flags and attributes of the common dialog control
            .Filter = "All Files (*.*)|*.*"
            If InStr(ActiveForm.Caption, ".") = 0 Then .FileName = ActiveForm.Caption & ".log" Else .FileName = ActiveForm.Caption
            .ShowSave
            If Len(.FileName) = 0 Then
                Exit Sub
            End If
            sFile = .FileName
        End With
        ActiveForm.theWindow.SaveFile sFile, rtfText
        ActiveForm.Caption = sFile
    Else
'        Stop
        sFile = ActiveForm.Caption
        ActiveForm.theWindow.SaveFile sFile, rtfText
    End If
'Stop
End Sub

Private Sub mnuFileClose_Click()
    If Not ActiveForm Is Nothing Then Unload ActiveForm
End Sub

Private Sub mnuFileOpen_Click()
    Dim sFile As String
    Dim NewDoc As Object
'On Error Resume Next

    If ActiveForm Is Nothing Then LoadNewDoc
    

    With dlgCommonDialog
        .DialogTitle = "Open"
        .CancelError = False
        'ToDo: set the flags and attributes of the common dialog control
        .Filter = "All Files (*.*)|*.*"
        .ShowOpen
        If Len(.FileName) = 0 Then
            Exit Sub
        End If
        sFile = .FileName
    End With
    
    If ActiveForm Is fMainForm Then
            Set NewDoc = New frmDocument
    Else
            Set NewDoc = ActiveForm
    End If
    NewDoc.theWindow.LoadFile sFile
    NewDoc.Caption = sFile

End Sub

Private Sub mnuFileNew_Click()
    LoadNewDoc
End Sub

Private Sub MDIForm_Unload(Cancel As Integer)
    thePrologTracer.OutputWindow.Visible = False
    thePrologTracer.Enabled = False
    hide_ide
    
    If Me.WindowState <> vbMinimized And Me.WindowState <> vbMaximized Then
        SaveSetting App.Title, "Settings", "MainLeft", Me.Left
        SaveSetting App.Title, "Settings", "MainTop", Me.Top
        SaveSetting App.Title, "Settings", "MainWidth", Me.Width
        SaveSetting App.Title, "Settings", "MainHeight", Me.Height
    End If
    shutdownApp
End Sub

Private Sub mnuFileExit_Click()
shutdownApp
End Sub
Private Sub MDIForm_Terminate()
shutdownApp
End Sub



Private Sub tbToolBar_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)

   If Button = 2 Then   ' Check if right mouse button
                        ' was clicked.
      PopupMenu fMainForm.mnuWindow   ' Display the File menu as a
                        ' pop-up menu.
   End If
End Sub

Private Sub telnet_Connect()
'fMainForm.telnet.SendData theText
End Sub

Private Sub telnet_DataArrival(ByVal bytesTotal As Long)
Dim NewText As String
'Stop
While fMainForm.telnet.State <> 7: DoEvents: Wend
telnet.GetData NewText, vbString
NewText = Replace(NewText, vbLf, vbCrLf)

SendTelnetLog NewText
XMLBuffer = XMLBuffer & NewText

If Right$(NewText, 10) = "</Consult>" Then
    StartConsult = InStrRev(XMLBuffer, "<Consult>")
    If StartConsult <> 0 Then
        XMLD.loadXML (Mid(XMLBuffer, StartConsult))
        Set ConsultXML = XMLD.documentElement
        Stop
    End If
End If


End Sub

Private Sub telnet_Error(ByVal Number As Integer, Description As String, ByVal Scode As Long, ByVal Source As String, ByVal HelpFile As String, ByVal HelpContext As Long, CancelDisplay As Boolean)
    On Error Resume Next
End Sub
Private Sub telnet_Close()
    SendTelnetLogLn "// Closed Socket"
    If CallingXML = True Then ParseXML
    CallingXML = False
End Sub

Private Sub telnet_SendProgress(ByVal bytesSent As Long, ByVal bytesRemaining As Long)
'
End Sub
Private Sub MDIForm_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)

   If Button = 2 Then   ' Check if right mouse button
                        ' was clicked.
      PopupMenu fMainForm.mnuWindow   ' Display the File menu as a
                        ' pop-up menu.
   End If
End Sub

Sub ClearModes()
    For i = 1 To intModes
        Modes(i).bPos = False
        Modes(i).mode = ""
    Next i
    intModes = 0
End Sub


Public Function ModeString() As String
    If intModes = 0 Then Exit Function
    Dim strFinal As String, bWhich As Boolean, i As Integer
    If Modes(1).bPos = True Then bWhich = True
    
    If bWhich Then strFinal = strFinal & "+" Else strFinal = strFinal & "-"
    
    For i = 1 To intModes
        If Modes(i).bPos <> bWhich Then
            bWhich = Not bWhich
            If bWhich Then strFinal = strFinal & "+" Else strFinal = strFinal & "-"
        End If
        strFinal = strFinal & Modes(i).mode
    Next i
    ModeString = strFinal
End Function

Public Sub AddMode(strMode As String, bPlus As Boolean)
    Dim i As Integer
    For i = 1 To intModes
        If Modes(i).mode = strMode Then Exit Sub
    Next i
    
    intModes = intModes + 1
    ReDim Preserve Modes(1 To intModes) As typMode
    
    With Modes(intModes)
        .bPos = True
        .mode = strMode
    End With
    Status.Update
End Sub

Sub PutCTCPReply(strNick As String, strKind As String, strReply As String)
    Dim strFinal As String, lngTemp As Long
    
    If LCase(strKind) = "ping" Then
        If IsNumeric(strReply) Then
            lngTemp = lngPingReply - CLng(strReply)
            strReply = Duration(lngTemp / 1000) & " " & lngTemp Mod 1000 & " milliseconds"
        End If
    End If
    
    strFinal = strColor & "04CTCPReply " & strBold & "[" & strBold & strNick & strUnderline & "!" & strUnderline & strKind & strBold & "]" & strBold & " " & strReply
    
    On Error Resume Next
    PutData client.ActiveForm.DataIn, strFinal
    If Err Then
        PutData Status.DataIn, strFinal
    End If
End Sub

Public Sub RemoveMode(strMode As String)
    Dim i As Integer, j As Integer
    
    For i = 1 To intModes
        If Modes(i).mode = strMode Then
            Modes(i).mode = ""
            For j = i To intModes - 1
                Modes(j) = Modes(j + 1)
            Next j
            intModes = intModes - 1
            ReDim Preserve Modes(1 To intModes) As typMode
            Status.Update
            Exit Sub
        End If
    Next i
    Status.Update
End Sub

Sub CTCPReply(strNick As String, strReply As String)
    client.SendData "NOTICE " & strNick & " :" & strAction & strReply & strAction
End Sub

Sub DrawToolbar()
    Dim intSeps As Integer, CenX As Integer, j As Integer, realWidth As Long
    Dim strTitle As String, intWidth As Integer, i As Integer, intBegin As Integer
    Dim intEnd As Integer, strDrawText As String, lngRet As Long, iconY As Integer, iconX As Integer
    bReDraw = True
    
    intSeps = WindowCount - 1
    
    If bStretchButtons Then
        realWidth = client.ScaleWidth / 15
        minSize = (intSeps + 1) * (ICON_SIZE + 40)
        If realWidth < minSize Then realWidth = minSize Else minSize = realWidth
    Else
        realWidth = (intSeps + 1) * intButtonWidth
        minSize = realWidth
    End If
    
    picTaskBuffer.Width = realWidth
    
    intWidth = realWidth / (intSeps + 1)
    picTaskBuffer.Cls
    
    picTaskBuffer.CurrentY = 2
    '* If one window open, draw only single thing..
    If intSeps = 0 Then
        strTitle = GetWindowTitle(1)
        strDrawText = TaskText(intWidth, strTitle)
        CenX = (realWidth - picTaskBuffer.TextWidth(strDrawText)) / 2
        picTaskBuffer.CurrentX = CenX
        picTaskBuffer.ForeColor = vbRed
        picTaskBuffer.Print strDrawText;
        Exit Sub
    End If
    
    picTaskBuffer.ForeColor = clrSep
    For i = 1 To intSeps + 5
        picTaskBuffer.CurrentX = intWidth * i
        For j = 3 To picTaskBuffer.ScaleHeight - 4 Step 2
            picTaskBuffer.PSet (picTaskBuffer.CurrentX, j)
        Next j
    Next i
    
    picTaskBuffer.ForeColor = vbBlack
    For i = 1 To intSeps + 1
        picTaskBuffer.CurrentY = 2
        picTaskBuffer.CurrentX = intWidth * (i - 1)
        intBegin = picTaskBuffer.CurrentX + 1
        intEnd = intBegin + intWidth - 2
        
        strTitle = GetWindowTitle(Int(i))
        'MsgBox strTitle & "~" & i
        If strTitle Like "Status" Then
            picIcon.Picture = ImageList1.ListImages.Item(6).Picture
        ElseIf strTitle Like "Friend Tracker" Then
            picIcon.Picture = ImageList1.ListImages.Item(7).Picture
        ElseIf strTitle Like "[#]*" Then
            picIcon.Picture = ImageList1.ListImages.Item(4).Picture
        Else
            picIcon.Picture = ImageList1.ListImages.Item(5).Picture
        End If
        picIcon.Picture = picIcon.Image
        
        intBegin = intBegin + 1
        intEnd = intEnd - 1
        
        If intActive = i Then iconY = 4 Else iconY = 3
        If intActive = i Then iconX = intBegin + 3 Else iconX = intBegin + 2
        
        If intHover = i Then strHoverCap = " " & strTitle & " "
        If intActive = i Then
            picTaskBuffer.ForeColor = vbRed
            '* Active window, let's draw a inset bevel
            picTaskBuffer.Line (intBegin, 2)-(intEnd, picTaskBuffer.ScaleWidth - 2), &H80000000, BF
            picTaskBuffer.Line (intBegin, 2)-(intEnd, 2), buttonShadow
            picTaskBuffer.Line (intBegin, 2)-(intBegin, picTaskBuffer.ScaleHeight - 2), buttonShadow
            picTaskBuffer.Line (intEnd, 2)-(intEnd, picTaskBuffer.ScaleHeight - 2), buttonHilight
            picTaskBuffer.Line (intBegin, picTaskBuffer.ScaleHeight - 2)-(intEnd, picTaskBuffer.ScaleHeight - 2), buttonHilight
            picTaskBuffer.CurrentY = 3
            If intHover = i Then picTask.Tag = intHover
        ElseIf intHover = i Then
            
            picTask.Tag = intHover
            picTaskBuffer.ForeColor = vbBlack
            picTaskBuffer.Line (intBegin, 2)-(intEnd, 2), buttonHilight
            picTaskBuffer.Line (intBegin, 2)-(intBegin, picTaskBuffer.ScaleHeight - 2), buttonHilight
            picTaskBuffer.Line (intEnd, 2)-(intEnd, picTaskBuffer.ScaleHeight - 2), buttonShadow
            picTaskBuffer.Line (intBegin, picTaskBuffer.ScaleHeight - 2)-(intEnd, picTaskBuffer.ScaleHeight - 2), buttonShadow
            picTaskBuffer.CurrentY = 2
        ElseIf WindowNewBuffer(i) Then
            Dim clr As Long
            clr = vbRed
            picTaskBuffer.Line (intBegin, 2)-(intEnd, 2), clr
            picTaskBuffer.Line (intBegin, 2)-(intBegin, picTaskBuffer.ScaleHeight - 2), clr
            picTaskBuffer.Line (intEnd, 2)-(intEnd, picTaskBuffer.ScaleHeight - 2), clr
            picTaskBuffer.Line (intBegin, picTaskBuffer.ScaleHeight - 2)-(intEnd, picTaskBuffer.ScaleHeight - 2), clr
            picTaskBuffer.ForeColor = vbBlack
            picTaskBuffer.CurrentY = 2
        Else
            If picTaskBuffer.ForeColor <> vbBlack Then picTaskBuffer.ForeColor = vbBlack
            picTaskBuffer.CurrentY = 2
        End If
        
        '* Let's put the icon
        lngRet = BitBlt(picTaskBuffer.hDC, iconX, iconY, ICON_SIZE, ICON_SIZE, picIcon.hDC, 0, 0, SRCCOPY)
        
        picTaskBuffer.CurrentX = intWidth * (i - 1)
        intBegin = picTaskBuffer.CurrentX
        intEnd = intBegin + intWidth
        strDrawText = TaskText(intWidth, strTitle)
        picTaskBuffer.CurrentX = TaskCenter(intWidth, strDrawText) + picTaskBuffer.CurrentX
        
        picTaskBuffer.CurrentY = picTaskBuffer.CurrentY + 2
        If intActive = i Then
            picTaskBuffer.CurrentX = picTaskBuffer.CurrentX + 1
        End If
        
        picTaskBuffer.Print strDrawText;
    Next i
    picTask.Picture = picTaskBuffer.Image
    'picTask.Refresh
        
End Sub

Public Sub HandleCTCP(strNick As String, strData As String)
    strData = RightR(strData, 1)
    strData = LeftR(strData, 1)
    
    Dim strCom As String, strParam As String, inttemp As Integer, strTemp As String, strArgs() As String
    Dim dccinfo As DCC_INFO
    
    Seperate strData, " ", strCom, strParam
    
    Select Case LCase(strCom)
        Case "version"
            PutData Status.DataIn, strColor & "05" & strBold & strNick & strBold & " has just requested your client version"
            CTCPReply strNick, "VERSION projectIRC for Windows"
        Case "ping"
            strTemp = RightOf(strData, " ")
            PutData Status.DataIn, strColor & "05" & strBold & strNick & strBold & " has just pinged you"
            CTCPReply strNick, "PING " & strTemp
        Case "action"
            inttemp = GetQueryIndex(strNick)
            If inttemp = -1 Then Exit Sub
            PutText Queries(inttemp).DataIn, strColor & "06" & strNick & " " & strParam
        Case "dcc"
            Seperate strData, " ", strTemp, strData
            strArgs = Split(strData, " ")
            
            Select Case LCase(strArgs(0))
                Case "send"
                    dccinfo.File = strArgs(1)
                    dccinfo.IP = strArgs(2)
                    
                    dccinfo.Port = strArgs(3)
                    dccinfo.Size = CLng(strArgs(4))
                    dccinfo.Nick = strNick
                    dccinfo.type = 2
                    
                    Dim bAccept As Boolean
                    bAccept = ShowAcceptDCC(dccinfo.Nick, dccinfo.File, dccinfo.Size)
                    
                    If bAccept = True Then
                        TimeOut 0.3
                        inttemp = NewDCCSend(dccinfo)
                        TimeOut 0.3
                        
                        If lngDCCStart = -1 Then 'DO NOT resume :P
                            DCCSends(inttemp).lngSentRcvd = 0
                            DCCSends(inttemp).sock.Connect
                        Else                        'RESUME!!!
                            lngAccept = CLng(inttemp)
                            client.SendData "PRIVMSG " & strNick & " :" & strAction & "DCC RESUME " & dccinfo.File & " " & dccinfo.Port & " " & lngDCCStart & strAction
                            DCCSends(inttemp).lngSentRcvd = lngDCCStart
                            DCCSends(inttemp).sock.Close
                            DCCSends(inttemp).sock.LocalPort = dccinfo.Port
                            DCCSends(inttemp).sock.RemotePort = dccinfo.Port
                            DCCSends(inttemp).sock.Close
                        End If
                    Else
                        client.SendData "NOTICE " & strNick & " :Your DCC request has been declined for " & dccinfo.File & " (" & Format(dccinfo.Size, "###,###,###,###") & " bytes)"
                    End If
                Case "accept"
                    DCCSends(lngAccept).StartResume DCCSends(lngAccept).sock.RemotePort
                    lngAccept = -1
            End Select
            
    End Select
End Sub


Sub interpret(strData As String)
    Dim parsed As ParsedData, inttemp As Integer
    Dim i As Integer, strChan As String, strTemp As String
    
    strData = Replace(strData, Chr(10), "")
    ParseData strData, parsed
    'DoEvents
    If Len(parsed.strCommand) = 0 Then Exit Sub
    
    '* I will put these in alphanumeric order one day, dont worry
    
    Select Case LCase(parsed.strCommand)
        Case "001"
            strMyNick = params(parsed, 1, 1)
            PutData Status.DataIn, "* " & params(parsed, 2, -1)
            Status.Update
            Exit Sub
        Case IRC_LOCALHOSTIRCD, IRC_SERVERCREATED, IRC_AVAILABLE, "004"
            PutData Status.DataIn, "* " & params(parsed, 2, -1)
            Exit Sub
        Case "ping"
            SendData "PONG :" & params(parsed, 1, 1)
            
            '* Show Ping? Pong! ??
            If Not bHidePing Then
                PutData Status.DataIn, strColor & "03Ping? Pong! [ " & params(parsed, 1, -1) & " ]"
            End If
            
            Exit Sub
        Case "join"
            If LCase(parsed.strNick) = LCase(strMyNick) Then
                inttemp = NewChannel(params(parsed, 1, -1))
                client.SendData "MODE " & params(parsed, 1, -1)
            Else
                inttemp = GetChanIndex(params(parsed, 1, 1))
                If inttemp = -1 Then Exit Sub
                Channels(inttemp).AddNick parsed.strNick
                PutData Channels(inttemp).DataIn, strColor & "03" & strBold & parsed.strNick & strBold & " has joined " & strBold & Channels(inttemp).strName
            End If
            Exit Sub
        Case "privmsg"
            'Stop
            EventBuffer = EventBuffer & vbCrLf & "irc_event(say('" & strNick & "','" & parsed.strParams(1) & "'," & Chr(34) & parsed.strParams(2) & Chr(34) & "))."
'            Stop
            
            
            strChan = params(parsed, 1, 1)
            If Left(strChan, 1) = "#" Or Left(strChan, 1) = "&" Then  'privmsg to channel
                inttemp = GetChanIndex(strChan)
                If inttemp <> -1 Then Channels(inttemp).PutText parsed.strNick, params(parsed, 2, -1)                                  '
            ElseIf parsed.strNick = strMyNick Then
                If params(parsed, 2, 2) = strAction & "VERSION" & strAction Then    'version
                    'Client.SendData "CTCP REPLY " & strChan & " VERSION :jIRC for Windows9x"
                    client.SendData "NOTICE " & parsed.strNick & " :VERSION projectIRC for Win32"
                End If
                GoTo msg
            
            Else    'send to query window
msg:
                strTemp = params(parsed, 2, -1)
                If Left(strTemp, 1) = strAction Then
                    HandleCTCP parsed.strNick, strTemp
                    Exit Sub
                End If
                
                'msgbox parsed.strNick
                
                If QueryExists(parsed.strNick) Then
                    'MsgBox "exists"
                    inttemp = GetQueryIndex(parsed.strNick)
                    If inttemp = -1 Then Exit Sub
                    
                    If Queries(inttemp).strHost <> parsed.strFullHost Then
                        Queries(inttemp).strHost = RightOf(parsed.strFullHost, "!")
                        Queries(inttemp).lblHost = RightOf(parsed.strFullHost, "!")
                        
                    End If
                    Queries(inttemp).Caption = parsed.strNick
                    Queries(inttemp).strNick = parsed.strNick
                    Queries(inttemp).lblNick = parsed.strNick
                    Queries(inttemp).PutText parsed.strNick, strTemp
                Else
                    'MsgBox "doesnt"
                    NewQuery parsed.strNick, parsed.strFullHost
                    inttemp = GetQueryIndex(parsed.strNick)
                    If inttemp = -1 Then Exit Sub
                    Queries(inttemp).Caption = parsed.strNick
                    Queries(inttemp).strNick = parsed.strNick
                    Queries(inttemp).lblNick = parsed.strNick
                    Queries(inttemp).PutText parsed.strNick, strTemp
                End If
            End If
            Exit Sub
        Case "nick"
            If parsed.strNick = strMyNick Then
                strMyNick = params(parsed, 1, 1)
                PutData Status.DataIn, strColor & "03Your nick is now " & strBold & strMyNick
                ChangeNick parsed.strNick, params(parsed, 1, -1)
                Status.Update
            Else
                ChangeNick parsed.strNick, params(parsed, 1, 1)
            End If
            Exit Sub
        Case "part"
            If parsed.strNick = strMyNick Then Exit Sub
            inttemp = GetChanIndex(parsed.strParams(1))
            If inttemp = -1 Then Exit Sub
            Channels(inttemp).RemoveNick parsed.strNick
            PutData Channels(inttemp).DataIn, strColor & "03" & strBold & parsed.strNick & strBold & " has left " & strBold & Channels(inttemp).strName
            If parsed.strNick = strMyNick Then Unload Channels(inttemp)
            Exit Sub
        Case "353" 'nick list!
            'MsgBox parsed.strParams(3)
            inttemp = GetChanIndex(parsed.strParams(3))
            If inttemp = -1 Then Exit Sub
            Dim strNicks() As String
            strNicks = Split(params(parsed, 4, -1), " ")
            For i = LBound(strNicks) To UBound(strNicks)
                Channels(inttemp).AddNick strNicks(i)
            Next i
            Exit Sub
        Case "mode"     'set mode
            inttemp = GetChanIndex(params(parsed, 1, 1))
            If inttemp = -1 And params(parsed, 1, 1) <> strMyNick Then Exit Sub
            strTemp = parsed.strNick
            If strTemp = "" Then strTemp = parsed.strFullHost
            
            If inttemp <> -1 Then PutData Channels(inttemp).DataIn, strColor & "03" & strBold & strTemp & strBold & " sets mode: " & params(parsed, 2, -1)
            If params(parsed, 1, 1) = strMyNick Then PutData Status.DataIn, strColor & "03" & strBold & strTemp & strBold & " sets mode: " & params(parsed, 2, -1)
            
            ParseMode params(parsed, 1, 1), params(parsed, 2, -1)
            Exit Sub
        Case "quit"     'quit
            NickQuit parsed.strNick, params(parsed, 1, -1)
            Exit Sub
        Case "kick"     'kick
            inttemp = GetChanIndex(params(parsed, 1, 1))
            If inttemp = -1 Then Exit Sub
            PutData Channels(inttemp).DataIn, strColor & "03" & strBold & params(parsed, 2, 2) & strBold & " was kicked from " & strBold & params(parsed, 1, 1) & strBold & " by " & strBold & parsed.strNick & strBold & " [ " & params(parsed, 3, -1) & " ]"
            Channels(inttemp).RemoveNick params(parsed, 2, 2)
            
            '* If user, close channel
            If params(parsed, 2, 2) = strMyNick Then
                '* close channel w/o sending PART command
                Channels(inttemp).Tag = "NOPART"
                '* unload it
                Unload Channels(inttemp)
                PutData Status.DataIn, strColor & "03" & "You were kicked from " & strBold & params(parsed, 1, 1) & strBold & " by " & strBold & parsed.strNick & strBold & " [ " & params(parsed, 3, -1) & " ]"
                
                '* Rejoin when kicked?
                If bRejoinOnKick Then
                    TimeOut 0.1
                    client.SendData "JOIN " & params(parsed, 1, 1)
                End If
            End If
            
            Exit Sub
        Case "332"  'topic!
            inttemp = GetChanIndex(params(parsed, 2, 2))
            If inttemp = -1 Then Exit Sub
            Channels(inttemp).rtbTopic.Text = ""
            PutData Channels(inttemp).rtbTopic, params(parsed, 3, -1)
            Channels(inttemp).rtbTopic.SelStart = 0
            Channels(inttemp).rtbTopic.SelLength = 1
            Channels(inttemp).rtbTopic.SelText = ""
            PutData Channels(inttemp).DataIn, strColor & "03Topic is """ & strColor & params(parsed, 3, -1) & strColor & "03"""
            Channels(inttemp).rtbTopic.SelStart = 0
            Channels(inttemp).rtbTopic.Tag = "locked"
            Exit Sub
        Case "topic"    'change in topic!
            inttemp = GetChanIndex(params(parsed, 1, 1))
            If inttemp = -1 Then Exit Sub
            Channels(inttemp).rtbTopic.Text = ""
            PutData Channels(inttemp).rtbTopic, params(parsed, 2, -1)
            Channels(inttemp).rtbTopic.SelStart = 0
            Channels(inttemp).rtbTopic.SelLength = 1
            Channels(inttemp).rtbTopic.SelText = ""
            PutData Channels(inttemp).DataIn, strColor & "03Topic changed by " & strBold & parsed.strNick & strBold & " : " & params(parsed, 2, -1)
            Exit Sub
        Case "333"  'topic on param2 set by param3, on param4
            inttemp = GetChanIndex(params(parsed, 2, 2))
            If inttemp = -1 Then Exit Sub
            PutData Channels(inttemp).DataIn, strColor & "03Topic set by " & strBold & params(parsed, 3, 3) & strBold & " on " & strBold & AscTime(params(parsed, 4, 4)) & strBold
            '* gotta add when it was SET!!
            Exit Sub
        Case "366"  'end of names list
            Exit Sub
        Case "324"  'set channel modes
            ParseMode params(parsed, 2, 2), params(parsed, 3, -1)
            Exit Sub
        Case "notice"
            'On Error Resume Next
            
            Seperate params(parsed, 2, -1), " ", strChan, strTemp 'strChan is actual the type
            
            '* CTCP Replies
            If Left(params(parsed, 2, 2), 1) = strAction Then
                PutCTCPReply parsed.strNick, RightR(strChan, 1), LeftR(strTemp, 1)
                Exit Sub
            End If
            
            '* Actual notices
            If parsed.strNick = "" Then
                PutData Status.DataIn, strColor & "05" & params(parsed, 2, -1)
            Else
                If client.ActiveForm.hwnd = BuddyList.hwnd Then
                    PutData Status.DataIn, strColor & "05" & strBold & "NOTICE" & strBold & strColor & " " & strBold & parsed.strNick & strBold & ":" & Chr(9) & params(parsed, 2, -1)
                Else
                    PutData client.ActiveForm.DataIn, strColor & "05" & strBold & "NOTICE" & strBold & strColor & " " & strBold & parsed.strNick & strBold & ":" & Chr(9) & params(parsed, 2, -1)
                End If
            End If
            
            Exit Sub
        Case "433"  'nick name already in use
            If params(parsed, 2, 2) = strMyNick Then
                strTemp = strMyNick
                strMyNick = strOtherNick
                strOtherNick = strMyNick
                client.SendData "NICK " & strMyNick
            End If
            PutData Status.DataIn, strColor & "04* " & params(parsed, 2, -1)
            Exit Sub
        Case "372"  'MOTD
            PutData Status.DataIn, params(parsed, 2, -1)
            Exit Sub
        Case "375"  'start of MOTD
            PutData Status.DataIn, strColor & "02" & params(parsed, 2, -1)
            Exit Sub
        Case "376"  'end of MOTD
            If bInvisible Then client.SendData "MODE " & strMyNick & " +i"
            Exit Sub
        Case "251", "252", "253", "254", "255", _
            "265", "266" 'server info, users, ops, channels, clients
            PutData Status.DataIn, strColor & "06" & params(parsed, 2, -1)
            Exit Sub
        Case "303"  'users on!
            BuddyList.AddUsers params(parsed, 2, -1)
            Exit Sub
        Case "329"  'date created for channel, $2 = channel, 3 = when
            inttemp = GetChanIndex(params(parsed, 2, 2))
            If inttemp = -1 Then Exit Sub
            PutData Channels(inttemp).DataIn, strColor & "03Channel created on " & strBold & AscTime(params(parsed, 3, 3))
            Exit Sub
        Case "484", "482", "461", "412", "403", "421", "401", "481", "402", "451", "404" 'error msgs
        
            EchoActive "* " & params(parsed, 2, -1), 4
            Exit Sub
        Case "321"  'blah channel list
            'ChannelsList.lvChannels.ListItems.Clear
            ChannelsList.Visible = True
            Exit Sub
        Case "322"          'channel list! ACK!!
            ChannelsList.AddChannel params(parsed, 2, 2), params(parsed, 3, 3), params(parsed, 4, -1)
            ChannelsList.Caption = "Channels List : " & ChannelsList.lvChannels.ListItems.Count & " channels"
            Exit Sub
        Case "323"  'end of channe list!
            'ChannelsList.bNeedClear = True
            'ChannelsList.lvChannels.ListItems.Clear
            Exit Sub
        Case "328"
            inttemp = GetChanIndex(params(parsed, 2, 2))
            If inttemp = -1 Then Exit Sub
            PutData Channels(inttemp).DataIn, strColor & "03Channel URL" & strColor & strUnderline & ":" & strUnderline & " " & strColor & "12" & params(parsed, 3, -1) & strColor
            Exit Sub
        Case "301"      'WHOIS! away
            strTemp = params(parsed, 2, 2)
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strTemp)
                If inttemp = -1 Then
                    PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " is away: " & params(parsed, 3, -1)
                Else
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strTemp & strBold & " is away: " & params(parsed, 3, -1)
                End If
            Else
                PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " is away: " & params(parsed, 3, -1)
            End If
            Exit Sub
        Case "307"  'nick ident
            strTemp = params(parsed, 2, 2)
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strTemp)
                If inttemp = -1 Then
                    PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, -1)
                Else
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, -1)
                End If
            Else
                PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, -1)
            End If
            Exit Sub
        Case "311"  'whois, ident, host, etc
            strTemp = params(parsed, 2, 2)
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strTemp)
                If inttemp = -1 Then
                    PutData Status.DataIn, " "
                    PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " is " & params(parsed, 3, 3) & "@" & params(parsed, 4, -1)
                Else
                    PutData Queries(inttemp).DataIn, " "
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strTemp & strBold & " is " & params(parsed, 3, 3) & "@" & params(parsed, 4, -1)
                End If
            Else
                PutData Status.DataIn, " "
                PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " is " & params(parsed, 3, 3) & "@" & params(parsed, 4, -1)
            End If
            Exit Sub
        Case "312"  'server
            strTemp = params(parsed, 2, 2)
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strTemp)
                If inttemp = -1 Then
                    PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " using: " & params(parsed, 3, 3) & " ( " & params(parsed, 4, -1) & " )"
                Else
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strTemp & strBold & " using: " & params(parsed, 3, 3) & " ( " & params(parsed, 4, -1) & " )"
                End If
            Else
                PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " is using: " & params(parsed, 3, 3) & " ( " & params(parsed, 4, -1) & " )"
            End If
            Exit Sub
        Case "313"  'is an irc operator
            strTemp = params(parsed, 2, 2)
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strTemp)
                If inttemp = -1 Then
                    PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, -1)
                Else
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, -1)
                End If
            Else
                PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, -1)
            End If
            Exit Sub
        Case "314"  'whowas info, ident, host, etc
            strTemp = params(parsed, 2, 2)
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strTemp)
                If inttemp = -1 Then
                    PutData Status.DataIn, " "
                    PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " was " & params(parsed, 3, 3) & "@" & params(parsed, 4, -1)
                Else
                    PutData Queries(inttemp).DataIn, " "
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strTemp & strBold & " was " & params(parsed, 3, 3) & "@" & params(parsed, 4, -1)
                End If
            Else
                PutData Status.DataIn, " "
                PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " was " & params(parsed, 3, 3) & "@" & params(parsed, 4, -1)
            End If
            Exit Sub
        Case "317"
            strTemp = params(parsed, 2, 2)
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strTemp)
                If inttemp = -1 Then
                    PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, 3) & " seconds idle / " & AscTime(CLng(params(parsed, 4, 4))) & " sign-on time"
                Else
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, 3) & " seconds idle / " & AscTime(CLng(params(parsed, 4, 4))) & " sign-on time"
                End If
            Else
                PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " " & params(parsed, 3, 3) & " seconds idle / " & AscTime(CLng(params(parsed, 4, 4))) & " sign-on time"
            End If
            Exit Sub
        Case "318"
            Exit Sub
        Case "319"
            strTemp = params(parsed, 2, 2)
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strTemp)
                If inttemp = -1 Then
                    PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " is on: " & params(parsed, 3, -1)
                Else
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strTemp & strBold & " is on: " & params(parsed, 3, -1)
                End If
            Else
                PutData Status.DataIn, strColor & "05" & strBold & strTemp & strBold & " is on: " & params(parsed, 3, -1)
            End If
            Exit Sub
        Case "320"
            strTemp = params(parsed, 2, 2)
            Seperate strTemp, " ", strChan, strTemp
            If bWhoisInQuery Then
                inttemp = GetQueryIndex(strChan)
                'MsgBox bWhoisInQuery & "~" & inttemp & "~" & strTemp & "~"
                If inttemp = -1 Then
                    PutData Status.DataIn, strColor & "05" & strBold & strChan & strBold & " " & strTemp
                Else
                    PutData Queries(inttemp).DataIn, strColor & "05" & strBold & strChan & strBold & " " & strTemp
                End If
            Else
                PutData Status.DataIn, strColor & "05" & strBold & strChan & strBold & " " & strTemp
            End If
            Exit Sub
        Case "305", "306"   '305=no longer away, 306=marked as away
            PutData Status.DataIn, strColor & "05" & params(parsed, 2, -1)
            Exit Sub
        Case "369"  'end of whowas
            Exit Sub
        Case "391"  'time
            PutData Status.DataIn, "Time: " & params(parsed, 5, -1)
            Exit Sub
        Case "462"
            PutData Status.DataIn, strColor & "04" & params(parsed, 2, -1)
            Exit Sub
        Case "501"      'unknown mode flag
            PutData Status.DataIn, strColor & "04" & params(parsed, 2, -1)
            Exit Sub
        Case "472"      'param1 is unknown mode char to me
            PutData Status.DataIn, strColor & "04" & strBold & params(parsed, 2, 2) & strBold & " " & params(parsed, 3, -1)
            Exit Sub
        Case "406"  'there was no such nick
            EchoActive strBold & params(parsed, 2, 2) & strBold & " " & params(parsed, 3, -1), 4
            Exit Sub
        Case "315"  'end of who list
            Exit Sub
        Case "438"
            PutData Status.DataIn, strColor & "04" & params(parsed, 1, 1) & " -> " & params(parsed, 2, 2) & ": " & params(parsed, 3, -1)
            Exit Sub
        Case "405"  'cannot join param2, param3
            PutData Status.DataIn, strColor & "04Cannot join " & params(parsed, 2, 2) & " ( " & params(parsed, 3, -1) & " )"
            Exit Sub
        Case "471", "473", "474", "475"   'cannot join channel, 471=+l, 473=+i, 475=+k, 474=+b
            PutData Status.DataIn, strColor & "04Cannot join " & params(parsed, 2, 2) & ": " & params(parsed, 3, -1)
            Exit Sub
        Case "263"
            PutData Status.DataIn, "* " & params(parsed, 2, -1)
            Exit Sub
        Case "617"
            PutData Status.DataIn, strColor & "05* " & params(parsed, 2, -1)
            Exit Sub
        Case "error"
            EchoActive "* " & params(parsed, 1, -1), 4
            Exit Sub
    End Select
    PutData Status.DataIn, "*** " & strBold & parsed.strCommand & strBold & " " & params(parsed, 1, -1) ' & " [" & parsed.strFullHost & "]"
End Sub


Sub SendData(strData As String)
    On Error Resume Next
    If sock.State = 0 Then Exit Sub
    sock.SendData strData & Chr(10)
    If DebugWin.Visible Then
        DebugWin.txtDataIn = DebugWin.txtDataIn & "<< " & strData & vbCrLf
        DebugWin.txtDataIn.SelStart = Len(DebugWin.txtDataIn)
    End If
    
End Sub


Private Sub Command1_Click()
    MsgBox Status.DataIn.SelRTF
End Sub

Sub SetClientPos()
    Dim strPos As String, strCPos As String, strLst() As String
    strCPos = "-1,-1,-1,-1"
    strPos = GetINI(winINI, "pos", "!client", strCPos)
    strLst = Split(strPos, ",")
    If UBound(strLst) <> 3 Then Exit Sub
    
    
    If CInt(strLst(0)) = -1 Then Exit Sub
    If CInt(strLst(1)) = -1 Then Exit Sub
    If CInt(strLst(2)) = -1 Then Exit Sub
    If CInt(strLst(3)) = -1 Then Exit Sub
    
    client.Move CInt(strLst(0)), CInt(strLst(1)), CInt(strLst(2)), CInt(strLst(3))
    
End Sub

Private Sub IDENT_ConnectionRequest(ByVal requestID As Long)
    IDENT.Close
    IDENT.Accept requestID
    IDENT.SendData IDENT.LocalPort & ", " & IDENT.RemotePort & " : USERID : UNIX : " & strMyIdent & vbCrLf
    
    Dim i As Integer
    For i = 1 To 500
        DoEvents
    Next i
    
    IDENT.Close
End Sub

Private Sub IDENT_DataArrival(ByVal bytesTotal As Long)
    Dim dat As String
    IDENT.GetData dat, vbString
    
    If dat Like "*, *" Then
        dat = LeftR(dat, 2)
        PutData Status.DataIn, "*** IDENT : " & dat
        dat = dat & " : USERID : UNIX : " & strMyIdent
        On Error Resume Next
        IDENT.SendData dat
        PutData Status.DataIn, "*** IDENT reply : " & dat
        'MsgBox "~" & dat & "~"
        Dim i As Integer
        IDENT.Close
    End If
End Sub

Private Sub IDENT_Error(ByVal Number As Integer, Description As String, ByVal Scode As Long, ByVal Source As String, ByVal HelpFile As String, ByVal HelpContext As Long, CancelDisplay As Boolean)
    PutData Status.DataIn, Chr(Color) & "04IDENT Error " & strColor & Description
End Sub

Private Sub MDIForm_Activate()
    DrawToolbar
    
    Dim lngRet As Long
    'lngRet = HideCaret(DataIn.hwnd)

End Sub

Private Sub MDIForm_Load()
    Me.Left = GetSetting(App.Title, "Settings", "MainLeft", 1000)
    Me.Top = GetSetting(App.Title, "Settings", "MainTop", 1000)
    Me.Width = GetSetting(App.Title, "Settings", "MainWidth", 6500)
    Me.Height = GetSetting(App.Title, "Settings", "MainHeight", 6500)
   ' LoadNewDoc
    intIface = IFT_FANCY
    intButtonWidth = 100
    sngLastX = 1
    DoEvents
    Me.Visible = True
    
    Load LoadSettings
    LoadSettings.Visible = True
    DoEvents
    StayOnTop LoadSettings, True
        
    mnu_Help.Caption = Chr$(8) & mnu_Help.Caption
    DrawToolbar
    
    '* INI stuff
    path = App.path
    If Right(App.path, 1) <> "\" Then path = path & "\"
    INI = path & "settings.ini"
    winINI = path & "windows.ini"
    SetClientPos
    '/if doesnt exist, create
    If Not FileExists(path & "settings.ini") Then
        Open INI For Output As #1
            Print #1, ""
        Close #1
    End If
    
    '/* Connection settings
    strServer = ReadINI("connect", "server", "irc.dal.net")
    strMyNick = ReadINI("connect", "nick", "pIRCu")
    strOtherNick = ReadINI("connect", "altnick", "OtherNick")
    strFullName = ReadINI("connect", "fullname", "projectIRC user")
    strMyIdent = ReadINI("connect", "ident", "projectIRC")
    lngPort = CLng(ReadINI("connect", "port", "6667"))
    bConOnLoad = CBool(ReadINI("connect", "connonload", "true"))
    bReconnect = CBool(ReadINI("connect", "reconnect", "true"))
    bInvisible = CBool(ReadINI("connect", "invisible", "true"))
    bRetry = CBool(ReadINI("connect", "retry", "true"))
    intRetry = CInt(ReadINI("connect", "retrynum", "99"))
    
    '/* Display settings
    lngBackColor = CLng(ReadINI("display", "backcolor", CStr(RGB(255, 255, 255))))
    lngForeColor = CLng(ReadINI("display", "forecolor", CStr(RGB(0, 0, 0))))
    lngLeftColor = CLng(ReadINI("display", "leftcolor", CStr(&H800000)))
    lngRightColor = CLng(ReadINI("display", "rightcolor", CStr(&H8000000F)))
    strFontName = ReadINI("display", "fontname", "ibmpc")
    intFontSize = CInt(ReadINI("display", "fontsize", CStr(8)))
    
    '* Window/Interface Settings
    bStretchButtons = CBool(ReadINI("windows", "stretch", "true"))
    
    '* IRC Settings
    bWhoisInQuery = CBool(ReadINI("irc", "whoisquery", "false"))
    MAX_TEXT_HISTORY = CInt(ReadINI("irc", "maxtexthistory", "30"))
    bAnnounce = CBool(ReadINI("irc", "announceaway", "true"))
    bRejoinOnKick = CBool(ReadINI("irc", "rejoinonkick", "true"))
    bHidePing = CBool(ReadINI("irc", "hidepong", "true"))
    
    TimeOut 0.1
    Unload LoadSettings
    
    
End Sub

 
Private Sub MDIForm_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
    intHover = 0
End Sub

Private Sub MDIForm_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    Cancel = 1
    Unload BuddyList
    Unload Status
    Unload DebugWin
    client.SendData "QUIT :using projectIRC, closed"
    
    Dim i As Integer
    
    For i = 1 To intChannels
        If Channels(i).strName <> "" Then Unload Channels(i)
    Next
    For i = 1 To intQueries
        If Queries(i).strNick <> "" Then Unload Queries(i)
    Next i
    
    For i = 0 To 15
        DoEvents
    Next i
    Unload Me
    'Cancel = 0
    End
End Sub

Private Sub MDIForm_Resize()
    CoolBar1.Width = (Me.ScaleWidth / 15) + 1
    shpTask.Width = (Me.Width / 15) - 8
    SetWinFocus intActive
    DrawToolbar
    
    If bSBL = True Then
        DoEvents
        If BuddyList.Visible = True Then BuddyList.Move client.Width - BuddyList.Width - 180, 0
    End If
    
End Sub

Sub mnu_File_Connect_Click()
    Select Case mnu_File_Connect.Caption
        Case "&Connect"
            '* Connect
            sock.Close
            mnu_File_Connect.Caption = "&Cancel"
            sock.RemoteHost = strServer
            sock.RemotePort = lngPort
            IDENT.Close
            On Error Resume Next
            IDENT.Listen
            sock.Connect
            PutData Status.DataIn, strColor & "02Connecting to " & strBold & strServer & strBold & " port " & strBold & lngPort
        Case "&Cancel"
            '* Cancel
            IDENT.Close
            sock.Close
            mnu_File_Connect.Caption = "&Connect"
            PutData Status.DataIn, strColor & "05Connection attempt cancelled"
    End Select
End Sub


Sub mnu_File_Disconnect_Click()
    mnu_File_Connect.Enabled = True
    mnu_File_Connect.Caption = "&Connect"
    mnu_File_Disconnect.Enabled = False
    PutData Status.DataIn, strColor & "05Disconnected from " & sock.RemoteHost
    sock.Close
    ClearModes
    IDENT.Close
    Status.lblServer = "not connected"
    Status.Update
    BuddyList.lstNicks.Clear
End Sub


Private Sub mnu_File_Options_Click()
    Options.Show 1
End Sub

Private Sub mnu_File_Quit_Click()
    client.SendData "QUIT :Using projectIRC, closed"
    IDENT.Close
    sock.Close
    Dim i As Integer
    For i = 1 To 1000
        DoEvents
    Next i
    Unload Me
End Sub

Private Sub mnu_Help_About_Click()
    About.Show vbModal
End Sub

Sub mnu_nicks_halfop_Click()
    Dim strChan As String, strNick As String
    strChan = client.ActiveForm.strName
    strNick = RealNick(client.ActiveForm.lstNicks.List(client.ActiveForm.lstNicks.ListIndex))
    
    If mnu_nicks_halfop.Caption = "&HalfOp" Then
        client.SendData "MODE " & strChan & " +h " & strNick
    Else
        client.SendData "MODE " & strChan & " -h " & strNick
    End If
        
End Sub

 Sub mnu_nicks_op_Click()
    Dim strChan As String, strNick As String
    strChan = client.ActiveForm.strName
    strNick = RealNick(client.ActiveForm.lstNicks.List(client.ActiveForm.lstNicks.ListIndex))
    
    If mnu_nicks_op.Caption = "&Op" Then
        client.SendData "MODE " & strChan & " +o " & strNick
    Else
        client.SendData "MODE " & strChan & " -o " & strNick
    End If
End Sub

Sub mnu_nicks_voice_Click()
    Dim strChan As String, strNick As String
    strChan = client.ActiveForm.strName
    strNick = RealNick(client.ActiveForm.lstNicks.List(client.ActiveForm.lstNicks.ListIndex))
    
    If mnu_nicks_halfop.Caption = "&Voice" Then
        client.SendData "MODE " & strChan & " +v " & strNick
    Else
        client.SendData "MODE " & strChan & " -v " & strNick
    End If
End Sub

Private Sub mnu_Tile_Vertically_Click()
    client.Arrange vbTileVertical
End Sub

Private Sub mnu_View_BuddyList_Click()
    With mnu_View_BuddyList
        .Checked = Not .Checked
        BuddyList.Visible = .Checked
    End With
End Sub

Private Sub mnu_View_Debug_Click()
    With mnu_View_Debug
        .Checked = Not .Checked
        DebugWin.Visible = .Checked
    End With

End Sub


Private Sub mnu_view_ForAWPos_Click()
    If intActive = -1 Then Exit Sub
    
    Dim strTemp As String, strPos As String, strCPos As String, strLst() As String
    strTemp = GetWindowTitle(intActive)
    
    PutINI winINI, "pos", "*" & strTemp, "-1,-1,-1,-1"
End Sub

Private Sub mnu_view_ForClientPos_Click()
    PutINI winINI, "pos", "!client", "-1,-1,-1,-1"
End Sub

Private Sub mnu_view_RemAWPos_Click()
    If intActive = -1 Then Exit Sub
    
    Dim strTemp As String, strPos As String
    strTemp = GetWindowTitle(intActive)
    
    With client.ActiveForm
        strPos = .Left & "," & _
                 .Top & "," & _
                 .Width & "," & _
                 .Height
    End With
    
    PutINI winINI, "pos", "*" & strTemp, strPos
End Sub

Private Sub mnu_view_RemClientPos_Click()
    PutINI winINI, "pos", "!client", client.Left & "," & client.Top & "," & client.Width & "," & client.Height
End Sub

Private Sub mnu_view_ResClientPos_Click()
    SetClientPos
End Sub


Public Sub mnu_view_ResetAWPos_Click()
    If intActive = -1 Then Exit Sub
    
    Dim strTemp As String, strPos As String, strCPos As String, strLst() As String
    strTemp = GetWindowTitle(intActive)
    
    With client.ActiveForm
        strCPos = .Left & "," & _
                 .Top & "," & _
                 .Width & "," & _
                 .Height
    End With

        
    strPos = GetINI(winINI, "pos", "*" & strTemp, "-1,-1,-1,-1")
    
    If strPos = "-1,-1,-1,-1" Then
    '    MsgBox "The active window(" & strTemp & ")'s position has not yet been remembered, please remember it first", vbInformation
        Exit Sub
    End If
    
    strLst = Split(strPos, ",")
    
    On Error Resume Next
    client.ActiveForm.Move CInt(strLst(0)), CInt(strLst(1)), CInt(strLst(2)), CInt(strLst(3))

End Sub

Private Sub mnu_View_Status_Click()
    mnu_View_Status.Checked = Not mnu_View_Status.Checked
    Status.Visible = mnu_View_Status.Checked
End Sub

Private Sub mnu_viewTBTop_Click()
    mnu_view_TBTop.Checked = True
End Sub

Private Sub mnu_View_TBBot_Click()
    mnu_view_TBTop.Checked = False
    mnu_View_TBBot.Checked = True
    picTask.Align = 2
    If picToolMain.Align = 2 Then
        picToolMain.Align = 1
        picToolMain.Align = 2
    End If
End Sub

Private Sub mnu_View_TBrBot_Click()
    mnu_View_TBrTop.Checked = False
    mnu_View_TBrBot.Checked = True
    
    picToolMain.Align = 2
    If picTask.Align = 2 Then
        picTask.Align = 1
        picTask.Align = 2
    End If
End Sub

Private Sub mnu_View_TBrTop_Click()
    mnu_View_TBrTop.Checked = True
    mnu_View_TBrBot.Checked = False
    picToolMain.Align = 1
    If picTask.Align = 1 Then
        picTask.Align = 2
        picTask.Align = 1
    End If
End Sub


Private Sub mnu_view_TBTop_Click()
    mnu_view_TBTop.Checked = True
    mnu_View_TBBot.Checked = False
    picTask.Align = 1
    'If picToolMain.Align = 1 Then
    '    picToolMain.Align = 2
    '    picToolMain.Align = 1
    'End If
End Sub


Private Sub mnu_Window_Cascade_Click()
    client.Arrange vbCascade
End Sub

Private Sub mnu_Window_TileH_Click()
    client.Arrange vbTileHorizontal
End Sub


Private Sub picTask_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
    If x > minSize Then Exit Sub
    Dim which As Integer, wincnt As Integer, wid As Integer
    wincnt = WindowCount
    
    wid = minSize \ wincnt
    which = Int((x \ wid) + 0.5) + 1
    sngLastX = x
    
    If intActive = which Then
        intActive = 0
        HideWin which
'        DoEvents
        picTask.Tag = ""
        intHover = which
        DrawToolbar
        Exit Sub
    End If
    '* Which now contains which button was clicked
    intActive = which
    'DrawToolbar
    SetWinFocus which
    DrawToolbar
End Sub


Private Sub picTask_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
    If x > minSize Then
        If intHover > 0 Then
            intHover = 0
            client.DrawToolbar
            picTask.Tag = 0
            picTask.ToolTipText = ""
        End If
        Exit Sub
    End If
    
    Dim which As Integer, wincnt As Integer, wid As Integer
    wincnt = WindowCount
    
    wid = minSize \ wincnt
    which = Int((x \ wid) + 0.5) + 1
    'sngLastX = X
    intHover = which
    tmrTask.Enabled = True
    If Val(picTask.Tag) = intHover Then Exit Sub
    client.DrawToolbar
    picTask.ToolTipText = strHoverCap
    
        
    'bReDraw = False
End Sub


Private Sub sock_Close()
    PutData Status.DataIn, strColor & "02Disconnected by SERVER from " & strServer
    sock.Close
    ClearModes
    IDENT.Close
    mnu_File_Connect.Enabled = True
    mnu_File_Connect.Caption = "&Connect"
    mnu_File_Disconnect.Enabled = False
    Status.lblServer = "not connected"
    Status.Update
    BuddyList.lstNicks.Clear

End Sub

Private Sub sock_Connect()
    '* Let's close all open windows
    Dim i As Integer
    For i = 1 To intChannels
        Channels(i).Tag = "NOPART"
        Unload Channels(i)
    Next i
    intChannels = 0
    
    For i = 1 To intQueries
        Unload Queries(i)
    Next i
    intQueries = 0
    mnu_File_Connect.Enabled = False
    mnu_File_Disconnect.Enabled = True
    mnu_File_Connect.Caption = "&Connect"
    PutData Status.DataIn, strColor & "03Connected to " & strServer
    
    SendData "PASS password"
    TimeOut 0.01
    SendData "NICK " & strMyNick
    TimeOut 0.01
    SendData "USER " & strMyNick & " local irc :" & strFullName & vbCrLf
    TimeOut 0.01
    
    Status.lblServer = sock.RemoteHost
    Status.Update
    
    '* Buddy List
    If mnu_View_BuddyList.Checked Then
        Dim strGet As String
        For i = 1 To BuddyList.lstSetup.ListCount
            strGet = strGet & BuddyList.lstSetup.List(i - 1) & " "
        Next i
        TimeOut 5
        If Trim(strGet) <> "" Then client.SendData "ISON " & strGet

    End If
    
    
End Sub

Private Sub sock_DataArrival(ByVal bytesTotal As Long)
    Dim dat As String, AllParams As String
    Dim strData() As String, i As Integer
    
    lngPingReply = GetTickCount
    
    On Error Resume Next
    If sock.State <> 0 Then sock.GetData dat, vbString
    If DebugWin.Visible Then
        If Len(DebugWin.txtDataIn.Text) + Len(dat) > 60000 Then DebugWin.txtDataIn.Text = ""
        On Error Resume Next
        DebugWin.txtDataIn = DebugWin.txtDataIn & ">> " & dat & vbCrLf
        DebugWin.txtDataIn.SelStart = Len(DebugWin.txtDataIn)
    End If
    
    strDataBuffer = strDataBuffer & dat
    
    If Right(strDataBuffer, 1) <> Chr(13) And Right(strDataBuffer, 1) <> Chr(10) Then
        Exit Sub
    Else
        dat = strDataBuffer
        strDataBuffer = ""
    End If
    
    strData = Split(dat, Chr(10))
    
    For i = LBound(strData) To UBound(strData)
        interpret strData(i)
    Next i
    
End Sub


Private Sub sock_Error(ByVal Number As Integer, Description As String, ByVal Scode As Long, ByVal Source As String, ByVal HelpFile As String, ByVal HelpContext As Long, CancelDisplay As Boolean)
    PutData Status.DataIn, strColor & "04ERROR : " & Description

End Sub


Private Sub tmrTask_Timer()
    Dim pt As POINTAPI, lngRet As Long, hwnd As Long
    lngRet = GetCursorPos(pt)
    
    hwnd = WindowFromPoint(pt.x, pt.y)
    If hwnd <> picTask.hwnd Then
        intHover = -1
        client.DrawToolbar
        picTask.Tag = -1
        tmrTask.Enabled = False
    End If
End Sub

Private Sub Toolbar1_ButtonClick(ByVal Button As MSComctlLib.Button)
    Select Case Button.Index
        Case 1
            mnu_File_Connect_Click
        Case 2
            mnu_File_Disconnect_Click
        Case 3
            mnu_File_Options_Click
        Case 5
            Dim strInput As String
            strInput = InputBox("Enter channel(s) to join, seperated each by a comma (EX: ""#irc"" or ""#irc, #mIRC"").  Also if you don't prefix # or &, I will for you (EX: ""irc,help"")", "Join channels")
            strInput = GetAlias("", "JOIN " & strInput)
    End Select
End Sub

Private Sub mnuConsultActiveWindow_Click()
    CommandButton "ConsultActiveWindow"
End Sub

Private Sub mnuNewSourceFile_Click()
    CommandButton "new"
End Sub

Private Sub mnuOpenProject_Click()
    CommandButton "openproject"
End Sub

Private Sub mnuResetProquery_Click()
    CommandButton "resetproquery"
End Sub

Private Sub mnuSwiActX_Click()
    Dim frmB As New fWebBrowser
    frmB.StartingAddress = "http://worlds.dev.aliensystems.com/swiactx"
    frmB.Show

End Sub

Private Sub mnuSWIPrologHelp_Click()
    Dim frmB As New fWebBrowser
    frmB.StartingAddress = "http://www.swi.psy.uva.nl/projects/SWI-Prolog/Manual/Contents.html"
    frmB.Show

End Sub


Public Function CommonDialog(Caption, CancelError, Filter, TypeDLg)
                With dlgCommonDialog
                    .DialogTitle = Caption
                    .CancelError = CancelError
                    .Filter = Filter
                    Select Case LCase(TypeDLg)
                        Case "save"
                            .ShowSave
                        Case "open"
                            .ShowOpen
                    End Select
                    CommonDialog = .FileName
                End With
End Function



Sub EditFile(sFilename)
Dim sForm As Form
On Error Resume Next
For Each sForm In Forms
    If sForm.Caption = sFilename Then
        sForm.Show
        sForm.Visible = True
        Set ActiveForm = sForm
        Exit Sub
    End If
Next
        Dim frmD As frmDocument
        Set frmD = New frmDocument
        frmD.Caption = sFilename
        frmD.Show
        'frmD.outputWindow.OpenFile sFilename
        frmD.OutputWindow.LoadFile sFilename

End Sub

Sub SaveFile(sFilename)
Dim sForm As Form
On Error Resume Next
For Each sForm In Forms
    If sForm.Caption = sFilename Then
        sForm.Show
        sForm.Visible = True
        Set ActiveForm = sForm
        sForm.OutputWindow.SaveFile sFilename
        Exit Sub
    End If
Next

End Sub
Sub OpenProject(sFile)
Dim sfileLine() As String, sfileConfigOption, sFileContents As String, i As Long
        sFileContents = Replace(FileContents(sFile), vbCr, vbLf)
        If Left$(sFileContents, Len("PrologVirtualWorldsProject 1.0")) <> "PrologVirtualWorldsProject 1.0" Then MsgBox "Something is wrong with this file: " & sFile & vbCrLf, vbCritical, "Error opening file ": Exit Sub
        sFileContents = Replace(sFileContents, vbLf & vbLf, vbLf)
        sfileLine = Split(sFileContents, vbLf)
        'Stop
        For i = 1 To UBound(sfileLine)
            sfileConfigOption = Split(sfileLine(i), "=")
            Select Case UBound(sfileConfigOption)
                Case 1
                    Select Case Trim(LCase(sfileConfigOption(0)))
                        Case "prologfile"
                             EditFile Trim(sfileConfigOption(1))
                        Case "projectname"
                             Me.Caption = "PrologVirualWorlds (" & Trim(sfileConfigOption(1)) & ")"
                        Case Else
                    End Select
            End Select
        Next i
End Sub

Public Function FileContents(fName)
    Dim ff As Long
    On Error Resume Next
    ff = FreeFile
    Open fName For Binary As ff
    FileContents = Input$(LOF(ff), ff)
    Close ff
End Function
Sub CommandButton(Key)
        Static lDocumentCount As Long
        Dim frmD As frmDocument, sForm
        Dim sFile As String, sfileLine() As String, sfileConfigOption, sFileContents As String, i As Long
        
    Select Case LCase(Key)
        
    Case "openuiclient"
        loginToTheWorld
    
    Case "openpl"
        If thePrologTracer Is Nothing Then Set thePrologTracer = New fPrologTracer
        If thePrologTracer.theAvatarInstance Is Nothing Then Set thePrologTracer.theAvatarInstance = New AvatarInstance
        thePrologTracer.Show
        
    Case "resetproquery"
    Case "exit"
        Unload Me
    Case "open"
        sFile = CommonDialog("Open", False, "All Files (*.*)|*.*", "open")
        If sFile <> "" Then If Right$(sFile, 4) = "plvw" Then OpenProject sFile Else EditFile sFile
    
    Case "openproject"
        sFile = CommonDialog("Open", False, "PrologVirtualWorlds Files (*.plvw)|*.plvw", "open")
        If sFile = "" Then Exit Sub
        OpenProject sFile
        
    
    Case "save"
        If Left$(ActiveForm.Caption, 10) = "New Source" Then CommandButton "saveas": Exit Sub
        SaveFile ActiveForm.Caption
    
    Case "saveas"
        dlgCommonDialog.FileName = ActiveForm.Caption
        sFile = CommonDialog("Save " & ActiveForm.Caption & " As...", False, "All Files (*.*)|*.*", "save")
        If sFile <> "" Then ActiveForm.OutputWindow.SaveFile sFile: ActiveForm.Caption = sFile
    
    Case "saveall"
        For Each sForm In Forms
            If VBA.TypeName(sForm) = "frmDocument" Then SaveFile ActiveForm.Caption
        Next
        
    Case "help"
        Me.PopupMenu Me.mnuHelp
    
    Case "pagesetup"
        On Error Resume Next
        With dlgCommonDialog
            .DialogTitle = "Page Setup"
            .CancelError = True
            .ShowPrinter
        End With


    Case "new"
        lDocumentCount = lDocumentCount + 1
        Set frmD = New frmDocument
        frmD.Caption = "New Source Document " & lDocumentCount
        frmD.Show
    Case "close"
        Unload ActiveForm
    
        Case "Print"
            mnuFilePrint_Click
        Case "Cut"
            mnuEditCut_Click
        Case "Copy"
            mnuEditCopy_Click
        Case "Paste"
            mnuEditPaste_Click
        Case "Bold"
            ActiveForm.OutputWindow.SelBold = Not ActiveForm.OutputWindow.SelBold
        '    Button.Value = IIf(ActiveForm.OutputWindow.SelBold, tbrPressed, tbrUnpressed)
        Case "Italic"
            ActiveForm.OutputWindow.SelItalic = Not ActiveForm.OutputWindow.SelItalic
         '   Button.Value = IIf(ActiveForm.OutputWindow.SelItalic, tbrPressed, tbrUnpressed)
        Case "Underline"
            ActiveForm.OutputWindow.SelUnderline = Not ActiveForm.OutputWindow.SelUnderline
          '  Button.Value = IIf(ActiveForm.OutputWindow.SelUnderline, tbrPressed, tbrUnpressed)
        Case "Evaluate"
            'ToDo: Add 'Evaluate' button code.
            MsgBox "Add 'Evaluate' button code."
        Case "Spy"
            'ToDo: Add 'Spy' button code.
            MsgBox "Add 'Spy' button code."
        Case "Align Left"
            ActiveForm.OutputWindow.SelAlignment = rtfLeft
        Case "Center"
            ActiveForm.OutputWindow.SelAlignment = rtfCenter
        Case "Align Right"
            ActiveForm.OutputWindow.SelAlignment = rtfRight
        Case Else
            MsgBox Key & " will be implimented soon!"
    End Select
End Sub

Sub NavigateComplete()
If readylogon = False Then Exit Sub
 
 readylogon = False
 
 
End Sub

Sub UnHookAvatar()
    Set ti.theIWorld = Nothing
    Set ti.theIClientOcx = Nothing
    Set theIAvatar = Nothing

End Sub
Sub HookAvatar()
 Dim theDocument As HTMLDocument, i, theclientOCXObj As VWClientOCX, theClientObj As VWClient
 Dim myLE As HTMLElementCollection
 
' Stop
 Set theDocument = theUIClientBrowser.brwWebBrowser.Document
 Set theclientOCXObj = theDocument.All.Item("theclient").Object
' Set theClientObj = theclientOCXObj.VWClient
 Set ti.theIClientOcx = theclientOCXObj
 Set ti.theIWorld = theclientOCXObj.VWClient.World
 Set theIAvatar = theclientOCXObj.VWClient.World.User
 
' Stop
 Set theIAvatar = theDocument
End Sub
Sub loginToTheWorld()
    Dim theDocument As HTMLDocument
   ' Dim LI As fLogin
  '  Set LI = New fLogin
    'LI.Show vbModal, Me
    'If LoginSucceeded = False Then Exit Sub
    
    
    If theUIClientBrowser Is Nothing Then Set theUIClientBrowser = New fWebBrowser
    
   ' theUIClientBrowser.brwWebBrowser.Offline = True
    'theUIClientBrowser.brwWebBrowser
   ' theUIClientBrowser.brwWebBrowser.Navigate ("about:blank")
   
   ' theDocument.All("UserAvatarName").Value = HumanAvatarName
 '   theDocument.All("UserAvatarPass").Value = HumanAvatarPass
  '  theDocument.All("theWorldName").Value = theWorldName
   ' theDocument.parentWindow.execScript "window_onload_call", "VBSCRIPT"
    
    
' Set theDocument = theUIClientBrowser.brwWebBrowser.Document
 '   theDocument.cookie = "sUser=" & HumanAvatarName & ";path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
  '  theDocument.cookie = "sPassword=" & HumanAvatarPass & ";path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
  '  theDocument.cookie = "sWorld=" & theWorldName & ";path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
 'Set theDocument = Nothing
 
    
  '  theUIClientBrowser.brwWebBrowser.GoSearch
    
    theUIClientBrowser.brwWebBrowser.Navigate VWContentPath & "client\html\vbStart.htm"
  Set theDocument = theUIClientBrowser.brwWebBrowser.Document
   'MsgBox theDocument.cookie
    readylogon = True
    
'''Dim Browser As WebBrowser


'Stop
    

    '    Document.cookie = "sUser=" & AvatarList.Value & ";path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
     '   Document.cookie ="sWorld=" & curWorldname & ";path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
      '  If Document.body.ClientHeight < 475 Then
        '    Document.cookie = "sLogoffURL=vwStartClient.htm;path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
       ' Else
         '   Document.cookie = "sLogoffURL=vwStartSDK.htm;path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
       ' End If

End Sub



Private Sub mnuAvatarConnection_Click()
    ti.test_connect
    
End Sub

Private Sub mnuHookAvatar_Click()
    If Left(mnuHookAvatar.Caption, 1) = "H" Then
        HookAvatar
        mnuHookAvatar.Caption = "UnHook Avatar"
    Else
        UnHookAvatar
        mnuHookAvatar.Caption = "Hook Avatar"
    End If

End Sub

Private Sub mnuReloadObjects_Click()
    preptheobjects
    MsgBox "Reloaded Objects and Settings"
End Sub

Private Sub mnuRunLocalServer_Click()
    Shell LocalVWInstall & "\vws.exe"
End Sub

Private Sub mnuUILogon_Click()
    CommandButton "openuiclient"
End Sub

Private Sub mnuViewPortClear_Click()
ActiveForm.OutputWindow.NewFile

End Sub

Private Sub mnuWorldPropertiesEditor_Click()
     '   If theWorldBrowser Is Nothing Then
      '      Set theWorldBrowser = New fWorldBrowser
            
       ' End If
        'theWorldBrowser.Show
End Sub








'    Dim frmB As New fWebBrowser
'    frmB.StartingAddress = "http://worlds.dev.aliensystems.com"
'    frmB.Show
'Private Sub mnuFileSaveAll_Click()
'    CommandButton "saveall"
'End Sub'

'Private Sub mnuFileSaveAs_Click()
'    CommandButton "saveas"
'End Sub

'Private Sub mnuFileSave_Click()
'    CommandButton "save"
'End Sub

'Private Sub mnuFileClose_Click()
'    CommandButton "close"
'End Sub

'Private Sub mnuFileOpen_Click()
 '   CommandButton "open"
'End Sub

'Private Sub tbToolBar_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
'If Button = 2 Then Me.PopupMenu Me.mnuWindow
'End Sub






