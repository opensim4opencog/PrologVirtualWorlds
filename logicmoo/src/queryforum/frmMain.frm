VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.MDIForm fMainMDI 
   BackColor       =   &H8000000C&
   Caption         =   "PrologVirtualWorlds"
   ClientHeight    =   5205
   ClientLeft      =   4590
   ClientTop       =   4080
   ClientWidth     =   11160
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "MDIForm1"
   Visible         =   0   'False
   Begin MSComctlLib.Toolbar tbToolBar 
      Align           =   1  'Align Top
      Height          =   420
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   11160
      _ExtentX        =   19685
      _ExtentY        =   741
      ButtonWidth     =   609
      ButtonHeight    =   582
      Appearance      =   1
      ImageList       =   "imlToolbarIcons(1)"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   10
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Open"
            Object.ToolTipText     =   "Open"
            ImageKey        =   "Open"
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Save"
            Object.ToolTipText     =   "Save"
            ImageKey        =   "Save"
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "OpenUIClient"
            ImageKey        =   "OpenUIClient"
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "OpenPL"
            ImageKey        =   "OpenPL"
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "hktest"
            ImageKey        =   "hktest"
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "TSMsTsC"
            ImageKey        =   "TSMsTsC"
         EndProperty
         BeginProperty Button8 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button9 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "help"
            ImageKey        =   "help"
         EndProperty
         BeginProperty Button10 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Compile"
            ImageKey        =   "Compile"
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.StatusBar sbStatusBar 
      Align           =   2  'Align Bottom
      Height          =   270
      Left            =   0
      TabIndex        =   0
      Top             =   4935
      Width           =   11160
      _ExtentX        =   19685
      _ExtentY        =   476
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   3
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   14049
            Text            =   "Status"
            TextSave        =   "Status"
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   6
            AutoSize        =   2
            TextSave        =   "10/6/2000"
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   5
            AutoSize        =   2
            TextSave        =   "2:10 AM"
         EndProperty
      EndProperty
   End
   Begin MSComDlg.CommonDialog dlgCommonDialog 
      Left            =   2880
      Top             =   1305
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComctlLib.ImageList imlToolbarIcons 
      Index           =   1
      Left            =   4980
      Top             =   2355
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   8
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0442
            Key             =   "Open"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0554
            Key             =   "Save"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0666
            Key             =   "OpenUIClient"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0980
            Key             =   "OpenPL"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0C9A
            Key             =   "hktest"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0FB4
            Key             =   "TSMsTsC"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":12CE
            Key             =   "help"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":15E8
            Key             =   "Compile"
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuOpenProject 
         Caption         =   "Open Project"
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFileNew 
         Caption         =   "&New"
         Begin VB.Menu mnuNewSourceFile 
            Caption         =   "Source File"
         End
         Begin VB.Menu mnuNewProject 
            Caption         =   "Project"
         End
      End
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Add File"
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Close"
      End
      Begin VB.Menu mnuFileBar0 
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
      End
      Begin VB.Menu mnuFileBar1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileProperties 
         Caption         =   "Propert&ies"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileBar2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFilePageSetup 
         Caption         =   "Page Set&up..."
      End
      Begin VB.Menu mnuFilePrintPreview 
         Caption         =   "Print Pre&view"
      End
      Begin VB.Menu mnuFilePrint 
         Caption         =   "&Print..."
      End
      Begin VB.Menu mnuFileBar3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileSend 
         Caption         =   "Sen&d..."
      End
      Begin VB.Menu mnuFileBar4 
         Caption         =   "-"
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
      Begin VB.Menu mnuFileBar5 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuProlog0 
      Caption         =   "Prolog"
      Begin VB.Menu mnuConsultActiveWindow 
         Caption         =   "Consult Active Window"
      End
      Begin VB.Menu mnuConsultAllWindows 
         Caption         =   "Consult All Windows"
      End
      Begin VB.Menu mnuResetProquery 
         Caption         =   "Reset PROQUERY"
      End
   End
   Begin VB.Menu mnuVirtualWorlds 
      Caption         =   "Avatar"
      Begin VB.Menu mnuAvatarConnection 
         Caption         =   "Connect"
      End
      Begin VB.Menu mnuUILogon 
         Caption         =   "UI Logon"
      End
      Begin VB.Menu mnuHookAvatar 
         Caption         =   "Hook to UI"
      End
      Begin VB.Menu mnuRunScript 
         Caption         =   "Run Script"
         Begin VB.Menu mnuAvatarRunScriptLoadNew 
            Caption         =   "Load New..."
         End
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "&Edit"
      Begin VB.Menu mnuEditUndo 
         Caption         =   "&Undo"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuSelectALL 
         Caption         =   "Select All"
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
      Begin VB.Menu mnuEditBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFind 
         Caption         =   "Find.."
      End
      Begin VB.Menu mnuCopyAll 
         Caption         =   "Copy All"
      End
      Begin VB.Menu mnuEditPasteSpecial 
         Caption         =   "Paste &Special..."
      End
      Begin VB.Menu bart 
         Caption         =   "-"
      End
      Begin VB.Menu mnuClear 
         Caption         =   "Clear"
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
   End
   Begin VB.Menu mnuTools 
      Caption         =   "&Tools"
      Begin VB.Menu mnuRunLocalServer 
         Caption         =   "Run Local Server"
      End
      Begin VB.Menu mnuWorldPropertiesEditor 
         Caption         =   "World Properties Editor"
      End
      Begin VB.Menu mnuReloadObjects 
         Caption         =   "Reload Objects and Settings"
      End
      Begin VB.Menu mnuToolsOptions 
         Caption         =   "&Options..."
      End
   End
   Begin VB.Menu mnuWindow 
      Caption         =   "&Window"
      WindowList      =   -1  'True
      Begin VB.Menu mnuViewPort 
         Caption         =   "Viewport"
         Begin VB.Menu mnuViewPortClear 
            Caption         =   "Clear"
         End
         Begin VB.Menu mnuViewPortSelectALL 
            Caption         =   "SelectAll"
         End
      End
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
      Begin VB.Menu mnuSWIPrologHelp 
         Caption         =   "SWI-Prolog"
      End
      Begin VB.Menu mnuSwiActX 
         Caption         =   "SwiActX"
      End
      Begin VB.Menu mnuHelpSearchForHelpOn 
         Caption         =   "&Search For Help On..."
      End
      Begin VB.Menu mnuHelpBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About "
      End
   End
   Begin VB.Menu mnuComBrowserObj 
      Caption         =   "COM Browser Object"
      Begin VB.Menu mnuComBrowserCopyObject 
         Caption         =   "Copy Object"
      End
   End
   Begin VB.Menu mnuComBrowserCopyProp 
      Caption         =   "Com Browser Properties"
      Begin VB.Menu mnuComBrowserCopyProperty 
         Caption         =   "Copy Property"
      End
      Begin VB.Menu mnuComBrowserCopyDecl 
         Caption         =   "Copy Declaration"
      End
   End
End
Attribute VB_Name = "fMainMDI"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Any) As Long
Const EM_UNDO = &HC7
Private Declare Function OSWinHelp% Lib "user32" Alias "WinHelpA" (ByVal hWnd&, ByVal HelpFile$, ByVal wCommand%, dwData As Any)


Private Sub MDIForm_Terminate()
    thePrologTracer.OutputWindow.Visible = False
End Sub

Private Sub mnuClear_Click()
 CommandButton "clear"
End Sub
Private Sub mnuComBrowserObj_click()
    mnuComBrowserCopyObject.Enabled = (theComClassBrowser.List1.ListIndex > -1)
End Sub
Private Sub mnuComBrowserCopyProp_click()
    mnuComBrowserCopyProperty.Enabled = (theComClassBrowser.List2.ListIndex > -1)
    mnuComBrowserCopyDecl.Enabled = (theComClassBrowser.List2.ListIndex > -1)
End Sub

Private Sub mnuComBrowserCopyObject_Click()
    CommandButton "combrowsercopyobject"
End Sub

Private Sub mnuConsultActiveWindow_Click()
    CommandButton "ConsultActiveWindow"
End Sub

Private Sub mnuCopyAll_Click()
    CommandButton "copyall"
End Sub

Private Sub mnuFind_Click()
    CommandButton "find"
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

Private Sub mnuSelectALL_Click()
    CommandButton "selectall"
End Sub


Private Sub mnuViewPortSelectALL_Click()
    CommandButton "selectall"
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


Private Sub tbToolBar_ButtonClick(ByVal Button As MSComCtlLib.Button)
    CommandButton Button.Key
End Sub

Sub EditFile(sFileName)
Dim sForm As Form
    On Error Resume Next
    For Each sForm In Forms
        If sForm.Caption = sFileName Then
            sForm.Show
            sForm.Visible = True
            Set ActiveForm = sForm
            Exit Sub
        End If
    Next
        Dim frmD As fDocument
        Set frmD = New fDocument
        frmD.Caption = sFileName
        frmD.Show
        'frmD.outputwindow.OpenFile sFileName
        frmD.OutputWindow.LoadFile sFileName, rtfText
End Sub

Sub SaveFile(sFileName)
Dim sForm As Form
On Error Resume Next
    For Each sForm In Forms
        If sForm.Caption = sFileName Then
            sForm.Show
            sForm.Visible = True
            Set ActiveForm = sForm
            sForm.OutputWindow.SaveFile sFileName, rtfText
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
                        Case "projectName"
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
Sub mnuConsultAllWindows_Click()
    CommandButton "ConsultAllWindows"
End Sub
Sub mnuProlog0_Click()
    mnuConsultActiveWindow.Enabled = (typeName(ActiveForm) = "fDocument")
End Sub
Sub CommandButton(Key)
        On Error Resume Next
        Static lDocumentCount As Long
        Static LastFind As String
        Dim frmD As fDocument, sForm
        Static LastForm As Form
        Dim sFile As String, sfileLine() As String, sfileConfigOption, sFileContents As String, i As Long
        Dim loc As Long
        Dim SearchOn As Boolean
    Select Case LCase(Key)
        
    Case "find"
        LastFind = InputBox("Enter Text", "Find...", LastFind)
        If LastFind = "" Then Exit Sub
        SearchOn = True
        Set LastForm = ActiveForm
        For Each sForm In Forms
            If typeName(sForm) = "fDocument" Then
                Set frmD = sForm
                If frmD Is LastForm Then SearchOn = True
                
                loc = InStr(1, frmD.OutputWindow.Text, LastFind, vbTextCompare)
                If loc > 0 And SearchOn Then
                    Stop
                    frmD.OutputWindow.SelStart = loc
                    frmD.OutputWindow.SelLength = Len(LastFind)
                  '  Set ActiveForm = frmD
                    frmD.Show
                    Exit Sub
                End If
            End If
        Next
    Case "saveall"
        For Each sForm In Forms
            If typeName(sForm) = "fDocument" Then
                sForm.DoSave
            End If
        Next
    Case "consultallwindows"
        For Each sForm In Forms
            If typeName(sForm) = "fDocument" Then
                sForm.ConfirmSave
                If InStr(sForm.Caption, "\") = 0 Then sForm.Caption = PVWLibraryPath & "\" & sForm.Caption
                theSWIPrologShell.Consult sForm.Caption
            End If
        Next
    Case "consultactivewindow"
        'Stop
        If typeName(ActiveForm) = "fDocument" Then
            ActiveForm.ConfirmSave
            If InStr(ActiveForm.Caption, "\") = 0 Then ActiveForm.Caption = PVWLibraryPath & "\" & ActiveForm.Caption
            theSWIPrologShell.Consult ActiveForm.Caption
        Else
            
        End If
    Case "selectall"
         ActiveForm.OutputWindow.SelStart = 0
         ActiveForm.OutputWindow.SelLength = Len(ActiveForm.OutputWindow.Text) + 1
    Case "clear"
            
         If InStr(ActiveForm.Caption, ".") > 0 Then If vbYes <> MsgBox("Are you sure you want to clear this file?", vbYesNo, "Confirm Clear") Then Exit Sub
         ActiveForm.OutputWindow.Text = ""
    
    Case "copyall"
        CommandButton "selectall"
        CommandButton "copy"
        
    Case "openuiclient"
        loginToTheWorld
    
    Case "openpl"
        'If thePrologTracer Is Nothing Then Set thePrologTracer = New fPrologTracer
        'If thePrologTracer.theAvatarMorphism Is Nothing Then Set thePrologTracer.theAvatarMorphism = New AvatarMorphism
        
        thePrologTracer.Show
        thePrologTracer.SetFocus
    
    Case "copy"
        On Error Resume Next
        Clipboard.SetText ActiveForm.OutputWindow.SelText
    
    Case "paiste"
        On Error Resume Next
        ActiveForm.OutputWindow.SelText = Clipboard.GetText
        
    Case "cut"
        On Error Resume Next
        Clipboard.SetText ActiveForm.OutputWindow.SelText
        ActiveForm.OutputWindow.SelText = vbNullString
    
    Case "resetproquery"
    
    Case "exit"
        Unload Me
    Case "open"
        sFile = CommonDialog("Open", False, "All Files (*.*)|*.*", "open")
        If sFile <> "" Then If Right$(sFile, 4) = "plvw" Then OpenProject sFile Else EditFile sFile
    
    Case "openproject"
        dlgCommonDialog.InitDir = PVWLibraryPath
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
    
    Case "hktest"
        theComClassBrowser.Show
        theComClassBrowser.SetFocus
        
    Case "saveall"
        For Each sForm In Forms
            If VBA.typeName(sForm) = "fDocument" Then SaveFile ActiveForm.Caption
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
        Set frmD = New fDocument
        frmD.Caption = "New Source Document " & lDocumentCount
        frmD.Show
    Case "close"
        Unload ActiveForm
    
        Case "Print"
            mnuFilePrint_Click
        Case "Cut"
            mnuEditCut_Click

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
    Set theAvatarMorphism.theIWorld = Nothing
    Set theAvatarMorphism.theIClientOcx = Nothing
    Set theIAvatar = Nothing

End Sub
Sub HookAvatar()
 Dim theDocument As HTMLDocument, i, theclientOCXObj As VWClientOCX, theClientObj As VWClient
 Dim myLE As HTMLElementCollection
 
' Stop
 Set theDocument = theUIClientBrowser.brwWebBrowser.Document
 Set theclientOCXObj = theDocument.All.Item("theclient").Object
' Set theClientObj = theclientOCXObj.VWClient
 Set theAvatarMorphism.theIClientOcx = theclientOCXObj
 Set theAvatarMorphism.theIWorld = theclientOCXObj.VWClient.World
 Set theIAvatar = theclientOCXObj.VWClient.World.User
 
' Stop
 Set theIAvatar = theDocument
End Sub
Private Sub MDIForm_Load()
  '  Me.Left = GetSetting(App.Title, "Settings", "MainLeft", 1000)
 '   Me.Top = GetSetting(App.Title, "Settings", "MainTop", 1000)
    Me.Width = GetSetting(App.Title, "Settings", "MainWidth", 6500)
    Me.Height = GetSetting(App.Title, "Settings", "MainHeight", 6500)
    'commandbutton "new"
    Me.mnuComBrowserObj.Visible = False
    Me.mnuComBrowserCopyProp.Visible = False
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
     '   Document.cookie ="sWorld=" & curWorldName & ";path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
      '  If Document.body.ClientHeight < 475 Then
        '    Document.cookie = "sLogoffURL=vwStartClient.htm;path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
       ' Else
         '   Document.cookie = "sLogoffURL=vwStartSDK.htm;path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
       ' End If

End Sub

Private Sub MDIForm_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
If Button = 2 Then Me.PopupMenu Me.mnuWindow
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
End Sub

Private Sub mnuAvatarConnection_Click()
    theAvatarMorphism.test_connect
    
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
    CommandButton "clear"

End Sub

Private Sub mnuWorldPropertiesEditor_Click()
     '   If theWorldBrowser Is Nothing Then
      '      Set theWorldBrowser = New fWorldBrowser
            
       ' End If
        'theWorldBrowser.Show
End Sub

Private Sub mnuHelpAbout_Click()
    MsgBox "ActiveX Version " & App.Major & "." & App.Minor & "." & App.Revision & vbCrLf & vbCrLf & "Program Version 0.6 " & vbCrLf & vbCrLf & "by Douglas Miles 2000"
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
        nRet = OSWinHelp(Me.hWnd, App.HelpFile, 261, 0)
        If Err Then
            MsgBox Err.Description
        End If
    End If

End Sub

Private Sub mnuHelpContents_Click()
    Dim nRet As Integer

    Call mnuViewWebBrowser_Click
    
    Exit Sub
    'if there is no helpfile for this project display a message to the user
    'you can set the HelpFile for your application in the
    'Project Properties dialog
    If Len(App.HelpFile) = 0 Then
        MsgBox "Unable to display Help Contents. There is no Help associated with this project.", vbInformation, Me.Caption
    Else
        On Error Resume Next
        nRet = OSWinHelp(Me.hWnd, App.HelpFile, 3, 0)
        If Err Then
            MsgBox Err.Description
        End If
    End If

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

Private Sub mnuToolsOptions_Click()
    fOptions.Show vbModal, Me
End Sub

Private Sub mnuViewWebBrowser_Click()
    Dim frmB As New fWebBrowser
    frmB.StartingAddress = "http://worlds.dev.aliensystems.com"
    frmB.Show
End Sub

Private Sub mnuViewOptions_Click()
    fOptions.Show vbModal, Me
End Sub

Private Sub mnuViewRefresh_Click()
    'ToDo: Add 'mnuViewRefresh_Click' code.
    preptheobjects
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
    CommandButton "paiste"
End Sub

Private Sub mnuEditCopy_Click()
    CommandButton "copy"

End Sub

Private Sub mnuEditCut_Click()
    CommandButton "cut"

End Sub

Private Sub mnuEditUndo_Click()
    'ToDo: Add 'mnuEditUndo_Click' code.
    MsgBox "Add 'mnuEditUndo_Click' code."
End Sub


Private Sub mnuFileExit_Click()
    CommandButton "exit"
End Sub

Private Sub mnuFileSend_Click()
        CommandButton "sendto"
End Sub

Private Sub mnuFilePrint_Click()
    On Error Resume Next
    If ActiveForm Is Nothing Then Exit Sub
    

    With dlgCommonDialog
        .DialogTitle = "Print"
        .CancelError = True
        .Flags = cdlPDReturnDC + cdlPDNoPageNums
        If ActiveForm.OutputWindow.SelLength = 0 Then
            .Flags = .Flags + cdlPDAllPages
        Else
            .Flags = .Flags + cdlPDSelection
        End If
        .ShowPrinter
        If Err <> MSComDlg.cdlCancel Then
            ActiveForm.OutputWindow.SelPrint .hDC
        End If
    End With

End Sub

Private Sub mnuFilePrintPreview_Click()
    CommandButton "printpreview"
End Sub

Private Sub mnuFilePageSetup_Click()
    CommandButton "pagesetup"
End Sub

Private Sub mnuFileProperties_Click()
    'ToDo: Add 'mnuFileProperties_Click' code.
    MsgBox "Add 'mnuFileProperties_Click' code."
End Sub

Private Sub mnuFileSaveAll_Click()
    CommandButton "saveall"
End Sub

Private Sub mnuFileSaveAs_Click()
    CommandButton "saveas"
End Sub

Private Sub mnuFileSave_Click()
    CommandButton "save"
End Sub

Private Sub mnuFileClose_Click()
    CommandButton "close"
End Sub

Private Sub mnuFileOpen_Click()
    CommandButton "open"
End Sub

Private Sub tbToolBar_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
If Button = 2 Then Me.PopupMenu Me.mnuWindow
End Sub
