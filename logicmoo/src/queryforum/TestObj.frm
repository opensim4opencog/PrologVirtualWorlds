VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "richtx32.ocx"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "Comctl32.ocx"
Object = "{2EA3FE5D-A93B-44C3-BF75-ADC753401AC0}#16.0#0"; "vbobjvw.ocx"
Begin VB.Form QueryObjectOld 
   Caption         =   "QueryObject"
   ClientHeight    =   3810
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6510
   Icon            =   "TestObj.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   3810
   ScaleWidth      =   6510
   Begin ComctlLib.Toolbar Toolbar1 
      Align           =   1  'Align Top
      Height          =   420
      Left            =   0
      TabIndex        =   16
      Top             =   0
      Width           =   6510
      _ExtentX        =   11483
      _ExtentY        =   741
      ButtonWidth     =   609
      ButtonHeight    =   582
      Appearance      =   1
      ImageList       =   "ImageList1"
      _Version        =   327682
      BeginProperty Buttons {0713E452-850A-101B-AFC0-4210102A8DA7} 
         NumButtons      =   15
         BeginProperty Button1 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "listen"
            Object.ToolTipText     =   "Listen"
            Object.Tag             =   ""
            ImageIndex      =   1
         EndProperty
         BeginProperty Button2 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "showhide"
            Object.ToolTipText     =   "Show/Hide the correction window"
            Object.Tag             =   ""
            ImageIndex      =   2
         EndProperty
         BeginProperty Button3 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "capitalizeword"
            Object.ToolTipText     =   "Capitalize/Uncapitalize selected word"
            Object.Tag             =   ""
            ImageIndex      =   3
         EndProperty
         BeginProperty Button4 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "addword"
            Object.ToolTipText     =   "Add a new word"
            Object.Tag             =   ""
            ImageIndex      =   4
         EndProperty
         BeginProperty Button5 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
            Style           =   3
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button6 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "new"
            Object.ToolTipText     =   "Create new dictation file"
            Object.Tag             =   ""
            ImageIndex      =   5
         EndProperty
         BeginProperty Button7 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "open"
            Object.ToolTipText     =   "Open a dictation file"
            Object.Tag             =   ""
            ImageIndex      =   6
         EndProperty
         BeginProperty Button8 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "save"
            Object.ToolTipText     =   "Save dictation file..."
            Object.Tag             =   ""
            ImageIndex      =   7
         EndProperty
         BeginProperty Button9 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
            Style           =   3
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button10 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "cut"
            Object.ToolTipText     =   "cut"
            Object.Tag             =   ""
            ImageIndex      =   8
         EndProperty
         BeginProperty Button11 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "copy"
            Object.ToolTipText     =   "copy"
            Object.Tag             =   ""
            ImageIndex      =   9
         EndProperty
         BeginProperty Button12 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "paste"
            Object.ToolTipText     =   "paste"
            Object.Tag             =   ""
            ImageIndex      =   10
         EndProperty
         BeginProperty Button13 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "cutall"
            Object.ToolTipText     =   "Cut all text"
            Object.Tag             =   ""
            ImageIndex      =   11
         EndProperty
         BeginProperty Button14 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
            Style           =   3
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button15 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "read"
            Object.ToolTipText     =   "Read from Insertion Point"
            Object.Tag             =   ""
            ImageIndex      =   14
         EndProperty
      EndProperty
   End
   Begin VB.CommandButton CmdXMIT 
      Caption         =   "XMIT"
      Height          =   315
      Left            =   3360
      TabIndex        =   15
      Top             =   2760
      Width           =   870
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   345
      Left            =   2160
      TabIndex        =   12
      Top             =   120
      Width           =   870
   End
   Begin VB.CommandButton CancelButton 
      Caption         =   "Cancel"
      Height          =   330
      Left            =   2160
      TabIndex        =   11
      Top             =   555
      Width           =   870
   End
   Begin VB.CommandButton cmdSelectResource 
      Caption         =   "Resource"
      Height          =   330
      Left            =   2160
      TabIndex        =   10
      Top             =   960
      Width           =   870
   End
   Begin VB.ComboBox txtMessage 
      Height          =   315
      Left            =   3360
      TabIndex        =   6
      Text            =   "txtMessage"
      Top             =   2400
      Width           =   3015
   End
   Begin VB.ComboBox TargetObject 
      Height          =   315
      Left            =   0
      TabIndex        =   5
      Text            =   "TargetObject"
      Top             =   2400
      Width           =   1695
   End
   Begin VB.ComboBox txtSubscriptions 
      Height          =   315
      Left            =   1800
      TabIndex        =   4
      Text            =   "Subscriptions"
      Top             =   2760
      Width           =   1455
   End
   Begin VB.ComboBox txtNativeLanguage 
      Height          =   315
      Left            =   0
      TabIndex        =   3
      Text            =   "NativeLanguage"
      Top             =   960
      Width           =   2055
   End
   Begin VB.TextBox txtFriendlyName 
      Height          =   285
      Left            =   0
      TabIndex        =   2
      Text            =   "FriendlyName"
      Top             =   360
      Width           =   2055
   End
   Begin VB.ComboBox txtDistributions 
      Height          =   315
      Left            =   0
      TabIndex        =   1
      Text            =   "Distributions"
      Top             =   2760
      Width           =   1695
   End
   Begin RichTextLib.RichTextBox OutputWindow 
      Height          =   435
      Left            =   0
      TabIndex        =   0
      Top             =   1560
      Width           =   6360
      _ExtentX        =   11218
      _ExtentY        =   767
      _Version        =   393217
      Enabled         =   -1  'True
      ScrollBars      =   3
      TextRTF         =   $"TestObj.frx":0442
   End
   Begin objvw.QueryActorVW TheQueryObject 
      Left            =   3840
      Top             =   360
      _ExtentX        =   1720
      _ExtentY        =   1085
   End
   Begin VB.Label Label1 
      Caption         =   "Name"
      Height          =   375
      Index           =   4
      Left            =   0
      TabIndex        =   14
      Top             =   120
      Width           =   1815
   End
   Begin VB.Label Label1 
      Caption         =   "Language"
      Height          =   375
      Index           =   3
      Left            =   0
      TabIndex        =   13
      Top             =   720
      Width           =   1815
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "To:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   9
      Top             =   2040
      Width           =   1335
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "Message:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   2
      Left            =   3360
      TabIndex        =   8
      Top             =   2040
      Width           =   1335
   End
   Begin VB.Label Label1 
      BackColor       =   &H80000000&
      Caption         =   "From:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   1
      Left            =   1800
      TabIndex        =   7
      Top             =   2040
      Width           =   855
   End
   Begin VB.Menu File 
      Caption         =   "&File"
      Begin VB.Menu New 
         Caption         =   "&New"
         Shortcut        =   ^N
      End
      Begin VB.Menu open 
         Caption         =   "&Open..."
         Shortcut        =   ^O
      End
      Begin VB.Menu save 
         Caption         =   "&Save"
         Shortcut        =   ^S
      End
      Begin VB.Menu saveas 
         Caption         =   "Save &As..."
      End
      Begin VB.Menu qw 
         Caption         =   "-"
      End
      Begin VB.Menu Exit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu Edit 
      Caption         =   "&Edit"
      Begin VB.Menu Cut 
         Caption         =   "Cu&t"
      End
      Begin VB.Menu Copy 
         Caption         =   "&Copy"
      End
      Begin VB.Menu Paste 
         Caption         =   "&Paste"
      End
      Begin VB.Menu Delete 
         Caption         =   "De&lete"
      End
      Begin VB.Menu qwe 
         Caption         =   "-"
      End
      Begin VB.Menu SelectAll 
         Caption         =   "Select &All"
         Shortcut        =   ^A
      End
   End
   Begin VB.Menu Dictation 
      Caption         =   "&Dictation"
      Begin VB.Menu listening 
         Caption         =   "&Listening for dictation"
      End
      Begin VB.Menu notlistening 
         Caption         =   "&Not listening for dictation"
      End
      Begin VB.Menu sdf 
         Caption         =   "-"
      End
      Begin VB.Menu showcorrection 
         Caption         =   "Show &Correction Window"
      End
      Begin VB.Menu capitalizeword 
         Caption         =   "&Capitalize selected word"
      End
      Begin VB.Menu Addword 
         Caption         =   "A&dd Word..."
      End
      Begin VB.Menu fsa 
         Caption         =   "-"
      End
      Begin VB.Menu Alwaysselect 
         Caption         =   "&Always select entire word"
      End
      Begin VB.Menu Options 
         Caption         =   "Dictation Options"
      End
   End
   Begin VB.Menu showmenu 
      Caption         =   "&Show"
      Begin VB.Menu alwaysontop 
         Caption         =   "&Always on top"
      End
      Begin VB.Menu setfont 
         Caption         =   "Set &Font"
      End
   End
   Begin VB.Menu Read 
      Caption         =   "&Read"
      Begin VB.Menu ReadDocument 
         Caption         =   "&Read from Insertion Point"
      End
      Begin VB.Menu sssss 
         Caption         =   "-"
      End
      Begin VB.Menu Stopreading 
         Caption         =   "&Stop Reading"
      End
   End
End
Attribute VB_Name = "QueryObjectOld"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Public Resources As String


'Default Property Values:
Const m_def_BackColor = 0
Const m_def_Enabled = 0
Const m_def_BackStyle = 0
Const m_def_BorderStyle = 0
Const m_def_propFoo = 0
'Property Variables:
Dim m_BackColor As Long
Dim m_Enabled As Boolean
Dim m_BackStyle As Integer
Dim m_BorderStyle As Integer
Dim m_Image As Picture
Dim m_propFoo As Variant
'Event Declarations:
Event AnswerReady()
Event IamLookingatCode()
'''Dim TheQueryObject As QueryActors.QueryInterface


#Const DebugMode = 0


Private Sub Form_Load()
   Dim i As Long, TypeArray As Variant
    txtNativeLanguage.Clear
    TypeArray = TheQueryObject.Languages
    For i = 0 To UBound(TypeArray)
    txtNativeLanguage.AddItem TypeArray(i)
    Next i
Me.Height = 1770
Me.Width = 3150
End Sub

Private Sub CancelButton_Click()
    Unload Me
End Sub

Private Sub OKButton_Click()
''''   NewRegistrant dlgNewQueryObj.txtName, dlgNewQueryObj.txtLanguage, dlgNewQueryObj.Resources

        If Me.txtNativeLanguage = "" Then Exit Sub

        Me.Caption = txtFriendlyName & ":" & txtNativeLanguage '& ":" & Resources
        Me.NativeLanguage = Me.txtNativeLanguage
        Me.FriendlyName = Me.txtFriendlyName
        Me.OKButton.Visible = False
        Me.CancelButton.Visible = False
        
Me.Width = 9060
Me.Height = 3180
form_Resize

End Sub



Private Sub OutputWindow_KeyPress(KeyAscii As Integer)
Dim t$, i%, LineNum%, textright As String
    t$ = Left(OutputWindow.Text, OutputWindow.SelStart)
    If KeyAscii <> 13 Then Exit Sub
    LineNum% = OutputWindow.GetLineFromChar(OutputWindow.SelStart)
    t$ = Replace(t$, vbCrLf, vbLf)
    t$ = Replace(t$, vbCr, vbLf)
    For i% = 1 To LineNum
       t$ = Mid(t$, InStr(t$, vbLf) + 1)
    Next i%
    
    Me.NativeQuery = t$
    'Me.wait 2000
    textright = " //--" & Me.NativeLanguage & "--> " & Me.AnswerList
    Me.OutputWindow.Text = VBA.Left(Me.OutputWindow.Text, Me.OutputWindow.SelStart + 1) & textright & VBA.Mid(Me.OutputWindow.Text, Me.OutputWindow.SelStart + 2)
    Me.OutputWindow.SelStart = Me.OutputWindow.SelStart & Len(txtMessage) & Len(textright)
    Me.OutputWindow.SelStart = Len(Me.OutputWindow.Text)
End Sub

Private Sub OutputWindow_SelChange()
    fMainForm.tbToolBar.Buttons("Bold").Value = IIf(OutputWindow.SelBold, tbrPressed, tbrUnpressed)
    fMainForm.tbToolBar.Buttons("Italic").Value = IIf(OutputWindow.SelItalic, tbrPressed, tbrUnpressed)
    fMainForm.tbToolBar.Buttons("Underline").Value = IIf(OutputWindow.SelUnderline, tbrPressed, tbrUnpressed)
    fMainForm.tbToolBar.Buttons("Align Left").Value = IIf(OutputWindow.SelAlignment = rtfLeft, tbrPressed, tbrUnpressed)
    fMainForm.tbToolBar.Buttons("Center").Value = IIf(OutputWindow.SelAlignment = rtfCenter, tbrPressed, tbrUnpressed)
    fMainForm.tbToolBar.Buttons("Align Right").Value = IIf(OutputWindow.SelAlignment = rtfRight, tbrPressed, tbrUnpressed)
End Sub

Private Sub form_Resize()
    On Error Resume Next
    If Me.txtNativeLanguage = "" Then Exit Sub
    
    Dim i As Long
    
    Debug.Print "Resize: " & Width & ", " & Height
    OutputWindow.Width = Me.Width
    OutputWindow.Left = 0
    OutputWindow.Top = 0
    
    OutputWindow.Height = Int(Me.ScaleHeight - txtNativeLanguage.Height - txtMessage.Height - Label1(0).Height)
    Label1(0).Top = OutputWindow.Height + 20
    Label1(1).Top = OutputWindow.Height + 20
    Label1(2).Top = OutputWindow.Height + 20
    Label1(0).Left = 0
    Label1(1).Left = Int(Me.Width * 0.25)
    Label1(2).Left = Int(Me.Width * 0.5)
    Label1(0).Width = Int(Me.Width * 0.25)
    Label1(1).Width = Int(Me.Width * 0.25)
    Label1(2).Width = Int(Me.Width * 0.25)
    TargetObject.Left = Label1(0).Left
    txtFriendlyName.Left = Label1(1).Left
    txtMessage.Left = Label1(2).Left
    TargetObject.Width = Label1(0).Width
    txtFriendlyName.Width = Label1(1).Width
    txtMessage.Width = Label1(2).Width * 2 - 90
    i = Label1(0).Height + OutputWindow.Height
    TargetObject.Top = i
    txtFriendlyName.Top = i
    txtMessage.Top = i
    
    i = i + TargetObject.Height
    
    txtNativeLanguage.Top = i
    txtDistributions.Top = i
    txtSubscriptions.Top = i
    
    txtNativeLanguage.Width = Int(Me.Width / 4)
    txtDistributions.Width = txtNativeLanguage.Width
    txtSubscriptions.Width = txtNativeLanguage.Width
    
    txtDistributions.Left = Label1(0).Left
    txtSubscriptions.Left = Label1(1).Left
    txtNativeLanguage.Left = Label1(2).Left
    CmdXMIT.Left = txtNativeLanguage.Left + txtNativeLanguage.Width + 20
    CmdXMIT.Top = txtNativeLanguage.Top
    
    cmdSelectResource.Top = Me.CmdXMIT.Top
    cmdSelectResource.Left = CmdXMIT.Left + CmdXMIT.Width + 20
    

End Sub


#If DebugMode Then
    'local variable to hold the serialized class ID that was created in Class_Initialize
    Private mlClassDebugID As Long
#End If

'RaiseEvent AnswerReady[(arg1, arg2, ... , argn)]
'Default Property Values:

'Public WithEvents TheQueryObject As QueryActors.QueryInterface

#If DebugMode Then
    Public Property Get ClassDebugID()
        'if we are in debug mode, surface this property that consumers can query
        ClassDebugID = mlClassDebugID
    End Property
#End If



Private Sub cmdCancel_Click()
    Unload Me
End Sub


Private Sub form_AccessKeyPress(KeyAscii As Integer)
    Debug.Print Me.Caption & " " & "AccessKeyPress: " & KeyAscii
End Sub

Private Sub form_AmbientChanged(PropertyName As String)
    If PropertyName = Null Or PropertyName = "" Then
        'Debug.Print me.caption & " " & "AmbientChanged: all"
    Else
        'Debug.Print me.caption & " " & "AmbientChanged: " & PropertyName
    End If
End Sub

Private Sub form_AsyncReadComplete(PropertyValue As AsyncProperty)
    Debug.Print Me.Caption & " " & "AsyncReadComplete: " & PropertyValue.AsyncType & ", " & PropertyValue.PropertyName
End Sub

Private Sub form_Initialize()
'Set TheQueryObject = New QueryActors.QueryInterface

Debug.Print Me.Caption & " " & "Initialize"
End Sub

Private Sub form_InitProperties()
    Debug.Print Me.Caption & " " & "InitProperties"
    m_BackColor = m_def_BackColor
    m_Enabled = m_def_Enabled
    m_BackStyle = m_def_BackStyle
    m_BorderStyle = m_def_BorderStyle
    Set m_Image = LoadPicture("")
    m_propFoo = m_def_propFoo
End Sub

Private Sub txtFriendlyName_KeyPress(KeyAscii As Integer)
If KeyAscii <> 13 Then Exit Sub
TheQueryObject.FriendlyName = txtFriendlyName.Text

End Sub

Private Sub txtNativeLanguge_KeyPress(KeyAscii As Integer)
If KeyAscii <> 13 Then Exit Sub
TheQueryObject.NativeLanguage = txtNativeLanguage.Text

End Sub

Private Sub txtDistributions_KeyPress(KeyAscii As Integer)
If KeyAscii <> 13 Then Exit Sub
Me.txtDistributions.AddItem txtDistributions.Text
End Sub

Private Sub txtNativeLanguage_Click()
Me.NativeLanguage = Me.txtNativeLanguage
End Sub

Private Sub txtSubscriptions_KeyPress(KeyAscii As Integer)
If KeyAscii <> 13 Then Exit Sub
Me.txtSubscriptions.AddItem txtSubscriptions.Text
End Sub

Private Sub form_OLECompleteDrag(Effect As Long)
    Debug.Print Me.Caption & " " & "OLECompleteDrag: " & Effect
End Sub

Private Sub form_OLEDragDrop(Data As DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single)
    Debug.Print Me.Caption & " " & "OLEDragDrop: " & Effect & ", " & Button & ", " & X & ", " & Y
End Sub

Private Sub form_OLEDragOver(Data As DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single, State As Integer)
    Debug.Print Me.Caption & " " & "OLEDragOver: " & Effect & ", " & Button & ", " & X & ", " & Y
End Sub

Private Sub form_OLEGiveFeedback(Effect As Long, DefaultCursors As Boolean)
    Debug.Print Me.Caption & " " & "OLEGiveFeedback: " & Effect & ", " & DefaultCursors
End Sub

Private Sub form_OLESetData(Data As DataObject, DataFormat As Integer)
    Debug.Print Me.Caption & " " & "OLESetData: " & DataFormat
End Sub

Private Sub form_OLEStartDrag(Data As DataObject, AllowedEffects As Long)
    Debug.Print Me.Caption & " " & "OLEStartDrag: " & AllowedEffects
End Sub

Private Sub form_ReadProperties(PropBag As PropertyBag)
    Debug.Print Me.Caption & " " & "ReadProperties"
    m_Enabled = PropBag.ReadProperty("Enabled", m_def_Enabled)
End Sub



Private Sub form_WriteProperties(PropBag As PropertyBag)
    Debug.Print Me.Caption & " " & "WriteProperties"
    Call PropBag.WriteProperty("BackColor", m_BackColor, m_def_BackColor)
    Call PropBag.WriteProperty("Enabled", m_Enabled, m_def_Enabled)
    Call PropBag.WriteProperty("BackStyle", m_BackStyle, m_def_BackStyle)
    Call PropBag.WriteProperty("BorderStyle", m_BorderStyle, m_def_BorderStyle)
    Call PropBag.WriteProperty("Image", m_Image, Nothing)
    Call PropBag.WriteProperty("propFoo", m_propFoo, m_def_propFoo)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14,0,0,0
Public Property Get propFoo() As Variant
    propFoo = m_propFoo
End Property

Public Property Let propFoo(ByVal New_propFoo As Variant)
    m_propFoo = New_propFoo
  '  PropertyChanged "propFoo"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14
Public Function methFoo() As Variant

End Function

Public Function ClearError() As Boolean
    ClearError = TheQueryObject.ClearError
End Function

Public Function Reset() As Boolean
    Reset = TheQueryObject.Reset
End Function

Public Function Reboot() As Boolean
    Reboot = TheQueryObject.Reboot
End Function

Public Function wait(Milliseconds)
    TheQueryObject.wait (Milliseconds)
End Function

Public Function NativeCommand(Optional ByVal TheCommand As String = "") As String
     NativeCommand = TheQueryObject.NativeCommand(TheCommand)
     Debug.Print Me.Caption & " " & "NativeCommand " & TheCommand
End Function
    
Public Function Analyze(Optional ByVal TheQuery As String = "") As String
     Analyze = TheQueryObject.Analyze(TheQuery)
     Debug.Print Me.Caption & " " & "Analyze " & TheQuery
End Function

Public Function Consult(ByVal TheResource As String) As Boolean
     Consult = TheQueryObject.Consult(TheResource)
End Function

Public Function Clone(Optional ByVal TheNewName As String = "") As String
     Clone = TheQueryObject.Clone(TheNewName)
End Function

Public Function DimObject(Optional AsType As String = "vbscript") As Boolean
        Let Me.NativeLanguage = AsType
        DimObject = True
End Function

Public Property Let NativeLanguage(ByVal vData As String)
    TheQueryObject.NativeLanguage = LCase(vData)
End Property

Public Property Let AnswerCode(ByVal vData As String)
    TheQueryObject.AnswerCode = vData
End Property

Public Property Get AnswerCode() As String
    AnswerCode = TheQueryObject.AnswerCode
End Property

Public Property Let BusyFree(ByVal vData As Boolean)
    TheQueryObject.BusyFree = vData
End Property

Public Property Get BusyFree() As Boolean
    BusyFree = TheQueryObject.BusyFree
End Property


Public Property Get AnswerList() As String
    AnswerList = TheQueryObject.AnswerList
End Property

Public Property Let AnswerCursor(ByVal vData As Integer)
    TheQueryObject.AnswerCursor = vData
End Property

Public Property Get AnswerCursor() As Integer
    AnswerCursor = TheQueryObject.AnswerCursor
End Property

Public Property Let AnswerCount(ByVal vData As Integer)
    TheQueryObject.AnswerCount = vData
End Property

Public Property Get AnswerCount() As Integer
    AnswerCount = TheQueryObject.AnswerCount
End Property

Public Property Get AnswerAtom() As String
    AnswerAtom = TheQueryObject.AnswerAtom
End Property

Public Property Set Errors(ByVal vData As ErrObject)
    TheQueryObject.Errors = vData
End Property

Public Property Get Errors() As ErrObject
    Errors = TheQueryObject.Errors
End Property

Public Property Let NativeQuery(ByVal vData As String)
    TheQueryObject.NativeQuery = vData
End Property

Public Property Get NativeQuery() As String
    NativeQuery = TheQueryObject.NativeQuery
End Property

Public Property Get NativeLanguage() As String
    NativeLanguage = TheQueryObject.NativeLanguage
End Property

Public Property Let FriendlyName(ByVal vData As String)
    TheQueryObject.FriendlyName = vData
End Property


Public Property Get FriendlyName() As String
    FriendlyName = TheQueryObject.FriendlyName
End Property

Public Property Let AgentQuery(ByVal vData As String)
    TheQueryObject.AgentQuery = vData
End Property

Public Property Get AgentQuery() As String
    AgentQuery = TheQueryObject.AgentQuery
End Property


Private Sub TheQueryObject_AnswerReady()
    Debug.Print Me.Caption & " " & "RaiseEvent: ANSWER READY!"
    RaiseEvent AnswerReady
End Sub





Private Sub DirectSS1_WordPosition(ByVal hi As Long, ByVal low As Long, ByVal byteoffset As Long)
Dim firstspace As Integer

'VB strings are unicode, so we divide the byteoffset by 2 to get the character offset
OutputWindow.SelStart = byteoffset / 2 + frmMain.gReadStart
firstspace = FindTextBreak(1, OutputWindow.SelStart, " " + vbNewLine + vbLf + vbCr + vbCrLf)
OutputWindow.SelLength = firstspace - OutputWindow.SelStart

On Error GoTo done
'frmMain.Vdict1.Lock
'Vdict1.TextSelSet OutputWindow.SelStart, OutputWindow.SelLength
'Vdict1.Unlock
done:
End Sub

Private Sub Addword_Click()
On Error GoTo addend
'Vdict1.LexiconDlg Form5.hwnd, "Add Word"
addend:
End Sub

Private Sub alwaysontop_Click()
alwaysontop.Checked = Not (alwaysontop.Checked)
If (alwaysontop.Checked) Then
 '   FloatPad (True)
Else
  '  FloatPad (False)
End If
End Sub

Private Sub ResetText()
OutputWindow.Font.Name = "Times New Roman"
OutputWindow.Font.Size = 14
'SelectAll_Click
'Delete_Click

OutputWindow.Font.Name = "Times New Roman"
OutputWindow.Font.Size = 14

End Sub

Private Sub Alwaysselect_Click()
Alwaysselect.Checked = Not (Alwaysselect.Checked)
End Sub

Private Sub capitalizeword_Click()
Dim e, s
s = OutputWindow.SelStart
e = OutputWindow.SelLength
'Vdict1.Lock
'Vdict1.FX (3)   'toggle word case
'Vdict1.Unlock
OutputWindow.SelStart = s
OutputWindow.SelLength = e

End Sub

Private Sub listening_Click()
'Listen (1)
End Sub

Private Sub notlistening_Click()
'Listen (0)
End Sub


Private Sub ReadDocument_Click()
Dim ReadText As String

On Error Resume Next  'if TTS cannot read because some other thing is making sound, dont except

'If (DirectSS1.Speaking) Then
 '   Stopreading_Click
'Else
  '  Listen (0)
 '   frmMain.Toolbar1.Buttons(15).Value = tbrPressed
  '  frmMain.Toolbar1.Buttons(15).Image = 13
   ' frmMain.gReadStart = OutputWindow.SelStart
    'ReadText = Right$(OutputWindow.Text, Len(OutputWindow.Text) - OutputWindow.SelStart)
   ' DirectSS1.Speak ReadText
'End If
End Sub

Private Function isTextBreak(c As String, breakchars As String)
Dim i
For i = 1 To Len(breakchars)
    If c = Mid$(breakchars, i, 1) Then
      isTextBreak = True
        GoTo done
    End If
Next i
isTextBreak = False
done:
End Function

Rem This is used instead of the rich text box scan function to reduce screen flicker
Private Function FindTextBreak(direction As Integer, start As Integer, breakchars As String)
Dim i
If (OutputWindow.Text = "") Then
    FindTextBreak = 1
ElseIf (direction = 0) Then
    For i = start To 1 Step -1
        If (isTextBreak(Mid$(OutputWindow.Text, i, 1), breakchars)) Then
            FindTextBreak = i
            GoTo done
        End If
        
    Next i
    FindTextBreak = i
    GoTo done
Else
    For i = start To Len(OutputWindow.Text)
        If (isTextBreak(Mid$(OutputWindow.Text, i + 1, 1), breakchars)) Then
            FindTextBreak = i
            GoTo done
        End If
    Next i
    FindTextBreak = i
End If
done:
End Function



