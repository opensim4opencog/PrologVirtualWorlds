VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "comctl32.ocx"
Begin VB.Form fCOMBroswer 
   Caption         =   "TypeLib Info"
   ClientHeight    =   5325
   ClientLeft      =   2025
   ClientTop       =   2670
   ClientWidth     =   6585
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   355
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   439
   WindowState     =   2  'Maximized
   Begin ComctlLib.TreeView tvwMembers 
      Height          =   5265
      Left            =   45
      TabIndex        =   0
      Top             =   0
      Width           =   2580
      _ExtentX        =   4551
      _ExtentY        =   9287
      _Version        =   327682
      HideSelection   =   0   'False
      Indentation     =   0
      LineStyle       =   1
      Sorted          =   -1  'True
      Style           =   7
      Appearance      =   1
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   4950
      Top             =   1980
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuOpen 
         Caption         =   "&Open..."
      End
      Begin VB.Menu mnuExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuTI 
      Caption         =   "TypeInfo for"
      Begin VB.Menu mnuTITW 
         Caption         =   "TreeView"
      End
      Begin VB.Menu mnuTIRichEdit 
         Caption         =   "RichEdit Document"
      End
   End
End
Attribute VB_Name = "fCOMBroswer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim oTLib As ITypeLib

Private RichEdit As CRichEdit
Private Doc As ITextDocument
Private Keywords As Variant

Private Const WM_SETREDRAW = 11
Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Private Declare Function MoveWindow Lib "user32" (ByVal hWnd As Long, ByVal X As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal bRepaint As Long) As Long


Private Sub EnumTIImplTypes(ByVal oTI As ITypeInfo, ByVal TKind As TypeKind)
Dim hRef As Long, oImpTI As ITypeInfo, sImpName As String
Dim TA As TYPEATTR, Idx As Long
   
   ' Get the attributes struct
   TA = GetTypeAttr(oTI)
         
   ' Check if the object implements
   ' other interfaces
   If TA.cImplTypes Then

      If TKind <> TKIND_COCLASS Then InsertText "' Inherited interfaces: "
      
      For Idx = 0 To TA.cImplTypes - 1
      
         ' Get the ITypeInfo of the
         ' implemented interface
         hRef = oTI.GetRefTypeOfImplType(Idx)
         Set oImpTI = oTI.GetRefTypeInfo(hRef)
      
         ' Get the interface name
         oImpTI.GetDocumentation -1, sImpName, "", 0, ""
      
         If TKind = TKIND_COCLASS Then
            Dim lFlags As IMPLTYPE
            
            lFlags = oTI.GetImplTypeFlags(Idx)
            
            If (lFlags And IMPLTYPEFLAG_FSOURCE) <> 0 Then
               InsertText "' Events interface: "
            Else
               InsertText "Implements "
            End If
            
         End If
         
         ' Write the name
         InsertText sImpName
 
         If TKind = TKIND_COCLASS Then
            
            If (lFlags And IMPLTYPEFLAG_FDEFAULT) <> 0 Then InsertText " ' Default"
            
            InsertText vbCr
            
         Else
            If Idx < TA.cImplTypes - 1 Then InsertText ", "
         End If
            
         
      Next
      
      InsertText vbCr
      
   End If

End Sub


Private Sub EnumTIFunctions(ByVal oTI As ITypeInfo, ByVal TypeK As TypeKind)
Dim Funcs() As FunctionInfo, Idx As Long
   
   Idx = EnumFunctions(oTI, Funcs) - 1
   
    ' Enumerate functions
   If Idx > -1 Then
      
      InsertText vbCr & "Functions: " & vbCr
      InsertText "========== " & vbCr & vbCr
      
      ' Write function prototypes to
      ' the textbox
      For Idx = 0 To Idx
         
         InsertText FunctionPrototype(Funcs(Idx)) & vbCr & vbCr
                        
      Next
      
   End If
   
End Sub


Private Sub EnumTIVars(ByVal oTI As ITypeInfo, ByVal TKind As TypeKind)
Dim Vars() As VarInfo, Idx As Long
Dim sName As String, sHelpStr As String
 
   Idx = EnumVariables(oTI, Vars()) - 1
   
   Select Case TKind
   
      Case TKIND_ENUM
         
         oTI.GetDocumentation -1, sName, sHelpStr, 0, ""
         
         If sHelpStr <> "" Then InsertText "' " & sHelpStr & vbCr
         InsertText "Enum " & sName & vbCr
         
         For Idx = 0 To Idx
            InsertText vbTab & Vars(Idx).Name & " = " & Vars(Idx).Value & " ' &H" & Hex$(Vars(Idx).Value) & vbCr
         Next
      
         InsertText "End Enum" & vbCr
   
      Case TKIND_RECORD
      
         oTI.GetDocumentation -1, sName, sHelpStr, 0, ""
         
         If sHelpStr <> "" Then InsertText "' " & sHelpStr & vbCr
         
         InsertText "Type " & sName & vbCr
         
         For Idx = 0 To Idx
            
            With Vars(Idx)
            
            InsertText vbTab & .Name
            
            If .Type.Array Then
               Dim ElemIdx As Long
                  
               InsertText "("
               
               For ElemIdx = 0 To .Type.DimsCount - 1
                  
                  With .Type.Dims(ElemIdx)
                     InsertText .LBound & " To " & .Elements - .LBound - 1
                  End With
                  
                  If ElemIdx < .Type.DimsCount - 1 Then InsertText ", "
                  
               Next
               
               InsertText ")"

            End If

            InsertText " As " & .Type.Name & vbCr
            
            End With
         Next
      
         InsertText "End Type" & vbCr
      
      Case Else
   
         InsertText "Constants:" & vbCr
         InsertText "==========" & vbCr & vbCr
      
         For Idx = 0 To Idx
            
            With Vars(Idx)

            InsertText "Public Const " & .Name & " As " & .Type.Name & " = "
            
            If .Type.Name = "String" Then
               InsertText """" & .Value & """" & vbCr
            Else
               InsertText .Value & vbCr
            End If
            
            End With
            
         Next
   
   End Select
   
      
End Sub



Private Function FunctionPrototype(FuncInfo As FunctionInfo) As String
Dim Idx As Long

   With FuncInfo
            
      If .Doc.HelpString <> "" Then
         FunctionPrototype = FunctionPrototype & "' Description: " & .Doc.HelpString & vbCr
      End If
         
      FunctionPrototype = FunctionPrototype & "' Function Kind: "
      
      Select Case .FunctionKind
         Case FUNC_DISPATCH
            FunctionPrototype = FunctionPrototype & "Dispatch"
         Case FUNC_NONVIRTUAL
            FunctionPrototype = FunctionPrototype & "Non Virtual"
         Case FUNC_PUREVIRTUAL
            FunctionPrototype = FunctionPrototype & "Pure Virtual"
         Case FUNC_STATIC
            FunctionPrototype = FunctionPrototype & "Static"
         Case FUNC_VIRTUAL
            FunctionPrototype = FunctionPrototype & "Virtual"
      End Select
      
      FunctionPrototype = FunctionPrototype & vbCr
               
      If .Flags <> 0 Then
         
         FunctionPrototype = FunctionPrototype & "' Flags: "
         
         If (.Flags And FUNCFLAG_FHIDDEN) <> 0 Then FunctionPrototype = FunctionPrototype & "Hidden, "
         If (.Flags And FUNCFLAG_FRESTRICTED) <> 0 Then FunctionPrototype = FunctionPrototype & "Restricted, "
         If (.Flags And FUNCFLAG_FUIDEFAULT) <> 0 Then FunctionPrototype = FunctionPrototype & "UI Default, "
         If (.Flags And FUNCFLAG_FUSESGETLASTERROR) <> 0 Then FunctionPrototype = FunctionPrototype & "Uses GetLastError, "
         
         FunctionPrototype = Left$(FunctionPrototype, Len(FunctionPrototype) - 2) & vbCr
      
      End If
      
      FunctionPrototype = FunctionPrototype & "' MemID: &H" & Hex$(.DISPID) & vbCr
      
      If .DllEntry.DllName <> "" Then FunctionPrototype = FunctionPrototype & "Declare "
      
      Select Case .InvokeKind
         
         Case INVOKE_PROPERTYGET
            
            FunctionPrototype = FunctionPrototype & "Property Get "
            
         Case INVOKE_PROPERTYPUT
            
            FunctionPrototype = FunctionPrototype & "Property Let "
         
         Case INVOKE_PROPERTYPUTREF
            
            FunctionPrototype = FunctionPrototype & "Property Set "
         
         Case INVOKE_FUNC
   
            Select Case .ReturnType.VariantType
               Case VT_HRESULT, VT_VOID
                  FunctionPrototype = FunctionPrototype & "Sub "
               Case Else
                  FunctionPrototype = FunctionPrototype & "Function "
            End Select
            
      End Select
       
      FunctionPrototype = FunctionPrototype & .Name
      
      If .DllEntry.DllName <> "" Then
         
         FunctionPrototype = FunctionPrototype & " lib """ & .DllEntry.DllName & """ "
         
         If .DllEntry.Name <> "" Then
            If .DllEntry.Name <> .Name Then FunctionPrototype = FunctionPrototype & vbTab & "Alias """ & .DllEntry.Name & """ "
         Else
            FunctionPrototype = FunctionPrototype & " Alias ""#" & .DllEntry.Ordinal & """ "
         End If
         
      End If

      If .ParamCount > 0 Then
      
         FunctionPrototype = FunctionPrototype & " ("
         
         If .ParamCount > 0 Then
            
            For Idx = 0 To .ParamCount - 1
            
               With .Parameters(Idx)
                  
                  If .IsOptional Then FunctionPrototype = FunctionPrototype & "Optional "
                  If Not .Type.ByRef Then FunctionPrototype = FunctionPrototype & "ByVal "
                  If .Type.ParamArray Then FunctionPrototype = FunctionPrototype & "ParamArray "
                  
                  FunctionPrototype = FunctionPrototype & .Name
                  
                  If .Type.Array Then FunctionPrototype = FunctionPrototype & "()"
                  
                  FunctionPrototype = FunctionPrototype & " As " & .Type.Name
                  
                  If .HasDefaultValue Then
                     
                     If VarType(.DefaultValue) = vbString Then
                        FunctionPrototype = FunctionPrototype & " = """ & .DefaultValue & """"
                     Else
                        If Not IsEmpty(.DefaultValue) Then
                           FunctionPrototype = FunctionPrototype & " = " & CStr(.DefaultValue)
                        End If
                     End If
                     
                  End If
                  
                  FunctionPrototype = FunctionPrototype & ", "
                  
               End With
               
            Next
            
            FunctionPrototype = Left$(FunctionPrototype, Len(FunctionPrototype) - 2)
            
         End If
         
         FunctionPrototype = FunctionPrototype & ")"
      
      End If
         
      On Error Resume Next
      
      Select Case .ReturnType.VariantType
         Case VT_HRESULT, VT_VOID
         Case Else
            FunctionPrototype = FunctionPrototype & " As " & .ReturnType.Name
      End Select
     
   End With

End Function

Sub HiliteText()
Dim lIdx As Long

   With Doc.Range(0, 0)
      Do While .FindTextStart("'", tomForward, 0) And .FindTextEnd(vbCr, tomForward, 0)
         .Font.ForeColor = &H8000&
         .MoveStart tomCharacter, 1
      Loop
   End With

   With Doc.Range(0, 0)
      
      For lIdx = 0 To UBound(Keywords)
         .start = 0
         Do While .FindText(Keywords(lIdx), tomForward, tomMatchWord)
            If .Font.ForeColor <> &H8000& Then .Font.ForeColor = &H800000
         Loop
      Next
   End With

End Sub

Private Sub InsertText(ByVal Text As String, Optional ByVal Color As Long = vbBlack)

   With Doc.Selection
   
      With .Font
         .Name = "Courier New"
         .Size = "10"
         .Bold = False
      End With
      
      .TypeText Text
      
   End With

End Sub

Sub LoadTL(ByVal FileName As String)
Dim oTI As ITypeInfo, Idx As Long
Dim TKind As TypeKind, sName As String, TA As TYPEATTR
Dim nCoclass As ComctlLib.Node, nInterface As ComctlLib.Node, nDisp As ComctlLib.Node, nModule As ComctlLib.Node
Dim nEnums As ComctlLib.Node, nUDTs As ComctlLib.Node, nTypedefs As ComctlLib.Node, nParent As ComctlLib.Node
Dim nControls As ComctlLib.Node, nClasses As ComctlLib.Node
   
   ' Clear the TextBox and TreeView
   tvwMembers.Nodes.Clear
   Doc.New
   
   ' Load the typelib
   Set oTLib = LoadTypeLibEx(FileName, REGKIND_NONE)
      
   ' Enumerate all typelib members
   For Idx = 0 To oTLib.GetTypeInfoCount - 1
   
      ' Get member type
      TKind = oTLib.GetTypeInfoType(Idx)
      
      ' Get member name
      Set oTI = oTLib.GetTypeInfo(Idx)
      oTI.GetDocumentation DISPID_UNKNOWN, sName, "", 0, ""
      TA = GetTypeAttr(oTI)
      Set oTI = Nothing
      
      Select Case TKind
      
         Case TKIND_COCLASS
         
            If nCoclass Is Nothing Then Set nCoclass = tvwMembers.Nodes.Add(, , , "Coclasses")
            
            If (TA.wTypeFlags And TYPEFLAG_FCONTROL) <> 0 Then
               If nControls Is Nothing Then Set nControls = tvwMembers.Nodes.Add(nCoclass, tvwChild, , "Controls")
               Set nParent = nControls
            Else
               If nClasses Is Nothing Then Set nClasses = tvwMembers.Nodes.Add(nCoclass, tvwChild, , "Classes")
               Set nParent = nClasses
            End If
         
         Case TKIND_INTERFACE
            If nInterface Is Nothing Then Set nInterface = tvwMembers.Nodes.Add(, , , "Interfaces")
                     
            Set nParent = nInterface
         
         Case TKIND_DISPATCH
         
            If nDisp Is Nothing Then Set nDisp = tvwMembers.Nodes.Add(, , , "Dispinterfaces")
            
            Set nParent = nDisp
         
         Case TKIND_MODULE
         
            If nModule Is Nothing Then Set nModule = tvwMembers.Nodes.Add(, , , "Modules")
            
            Set nParent = nModule
                        
         Case TKIND_ENUM
            If nEnums Is Nothing Then Set nEnums = tvwMembers.Nodes.Add(, , , "Enums")
         
            Set nParent = nEnums

         Case TKIND_ALIAS
            If nTypedefs Is Nothing Then Set nTypedefs = tvwMembers.Nodes.Add(, , , "Typedefs")
         
            Set nParent = nTypedefs
         
         Case TKIND_RECORD, TKIND_UNION
            If nUDTs Is Nothing Then Set nUDTs = tvwMembers.Nodes.Add(, , , "Structs & Unions")
   
            Set nParent = nUDTs
      End Select
      
      ' Add member name to treeview
      tvwMembers.Nodes.Add nParent, tvwChild, "#" & Idx, sName
      
   Next
   
End Sub




Private Sub Form_Load()

   ' Create the RichEdit control
   Set RichEdit = New CRichEdit
   RichEdit.Init hWnd
   
   ' Get the document
   Set Doc = RichEdit.Doc
   
   ' Initialize the keywords array
   Keywords = Array( _
      "sub", "function", "interger", "long", _
      "string", "byte", "double", "single", _
      "currency", "date", "boolean", "alias", _
      "lib", "implements", "byval", "as", _
      "variant", "any", "paramarray", "optional", _
      "property get", "property let", "property set")
   
End Sub

Private Sub Form_Resize()
   
   tvwMembers.Move 0, 0, ScaleWidth / 3, ScaleHeight
    
   MoveWindow RichEdit.hWnd, ScaleWidth / 3, 0, ScaleWidth / 3 * 2, ScaleHeight, 1
   
End Sub


Private Sub Form_Unload(Cancel As Integer)

   Set Doc = Nothing
   Set RichEdit = Nothing
   
End Sub

Private Sub mnuExit_Click()
   Unload Me
End Sub

Private Sub mnuOpen_Click()

   On Error Resume Next
   
   With CommonDialog1
      .Filter = "Type Libraries|*.tlb;*.olb|OCXs|*.ocx|DLLs|*.dll|EXEs|*.exe"
      .DefaultExt = "tlb"
      .Flags = cdlOFNFileMustExist Or cdlOFNHideReadOnly
      .ShowOpen
      
      If Err.Number = 0 Then
            
         LoadTL .FileName
          
      End If
      
   End With
   
End Sub

Private Sub mnuTIRichEdit_Click()
Dim oTI As ITypeInfo

   Set oTLib = Nothing
   tvwMembers.Nodes.Clear
   
   Set oTI = TypeInfoFromObject(Doc)
   
   Doc.Freeze
   Doc.New
   
   EnumTIFunctions oTI, TKIND_DISPATCH
   
   HiliteText

   Doc.Unfreeze

End Sub

Private Sub mnuTITW_Click()
Dim oTI As ITypeInfo

   Set oTLib = Nothing
   tvwMembers.Nodes.Clear
   
   Set oTI = TypeInfoFromObject(tvwMembers)
   
   Doc.Freeze
   Doc.New
   
   EnumTIFunctions oTI, TKIND_DISPATCH

   Doc.Unfreeze
   
End Sub


Private Sub tvwMembers_NodeClick(ByVal Node As ComctlLib.Node)
Dim oTI As ITypeInfo, TKind As TypeKind, Idx As Long
Dim sHelpStr As String, TA As TYPEATTR, sCLSID As String

   If Node.Key <> "" Then
      Idx = Val(Mid$(Node.Key, 2))
      
      Set oTI = oTLib.GetTypeInfo(Idx)
      TKind = oTLib.GetTypeInfoType(Idx)
      
      ' Get object help string
      oTI.GetDocumentation DISPID_UNKNOWN, "", sHelpStr, 0, ""
      
      TA = GetTypeAttr(oTI)
      sCLSID = Space$(38)
      StringFromGUID2 TA.IID, StrPtr(sCLSID), 39
      
      Doc.Freeze
      Doc.New
      
      Select Case TKind
      
         Case TKIND_DISPATCH
         
            InsertText "' Dispinterface " & Node.Text & vbCr
            InsertText "' IID " & sCLSID & vbCr
            
            If sHelpStr <> "" Then InsertText "' " & sHelpStr & vbCr & vbCr
         
            EnumTIImplTypes oTI, TKind
            EnumTIFunctions oTI, TKind
         
         Case TKIND_INTERFACE
            
            InsertText "' Interface " & Node.Text & vbCr
            InsertText "' IID " & sCLSID & vbCr
            If sHelpStr <> "" Then InsertText "' " & sHelpStr & vbCr & vbCr
         
            EnumTIImplTypes oTI, TKind
            EnumTIFunctions oTI, TKind
            
         Case TKIND_MODULE
            
            InsertText "' Module " & Node.Text & vbCr
            If sHelpStr <> "" Then InsertText "' " & sHelpStr & vbCr '& vbCr
            
            EnumTIVars oTI, TKind
            
            EnumTIFunctions oTI, TKind
      
         Case TKIND_ENUM, TKIND_RECORD
            
            If sHelpStr <> "" Then InsertText "' " & sHelpStr & vbCr & vbCr
            EnumTIVars oTI, TKind
            
         Case TKIND_COCLASS
         
            InsertText "' CLSID " & sCLSID & vbCr
            If sHelpStr <> "" Then InsertText "' " & sHelpStr & vbCr & vbCr
            EnumTIImplTypes oTI, TKind
            
      End Select
      
      Set oTI = Nothing
      
      HiliteText
      
      Doc.Unfreeze
      
   Else
   
      Doc.New
      
   End If
   
End Sub






