VERSION 5.00
Object = "{EF7D655A-0161-11D2-88BE-00C04FC32EF3}#1.0#0"; "vwobjexp.ocx"
Begin VB.Form fWorldBrowser 
   Caption         =   "Form1"
   ClientHeight    =   5835
   ClientLeft      =   8685
   ClientTop       =   3735
   ClientWidth     =   6795
   LinkTopic       =   "Form1"
   ScaleHeight     =   5835
   ScaleWidth      =   6795
   Begin OBJEXPLRLib.TPList TPList1 
      Height          =   1725
      Left            =   2700
      TabIndex        =   2
      Top             =   675
      Width           =   3570
      _Version        =   65536
      _Version        =   65536
      _Version        =   65536
      _ExtentX        =   6297
      _ExtentY        =   3043
      _StockProps     =   4
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin OBJEXPLRLib.TVList TVList1 
      Height          =   1905
      Left            =   2610
      TabIndex        =   1
      Top             =   2655
      Width           =   3615
      _Version        =   65536
      _ExtentX        =   6376
      _ExtentY        =   3360
      _StockProps     =   0
   End
   Begin OBJEXPLRLib.ThingTree ThingTree1 
      Height          =   4110
      Left            =   450
      TabIndex        =   0
      Top             =   585
      Width           =   1860
      _Version        =   65536
      _Version        =   65536
      _ExtentX        =   3281
      _ExtentY        =   7250
      _StockProps     =   0
   End
End
Attribute VB_Name = "fWorldBrowser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit

Public goTop, goClient, goWorld



Public editThing
Public editPropName
Private fShiftKey

Public Window As Form
Public Document As Form
Public thing


Sub OnConnect()
    Dim curWorldname
    curWorldname = Trim(LastWorld.Value)

    'First, check for valid avatar and world name
    If CheckAvatarListValue = False Then
        MsgBox "Select an avatar nickname from the Avatar name list box.", vbInformation, "Microsoft Virtual Worlds"
        AvatarList.focus
    'Otherwise, set cookie and connect
    Else
        AvatarAccessControl.DisconnectAvatar
        Document.cookie = "sUser=" & AvatarList.Value & ";path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
        Document.cookie = "sWorld=" & curWorldname & ";path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
        If Document.body.clientHeight < 475 Then
            Document.cookie = "sLogoffURL=vwStartClient.htm;path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
        Else
            Document.cookie = "sLogoffURL=vwStartSDK.htm;path=/;expires=Fri, 1 Jan 2038 00:00:00 GMT"
        End If
        Window.Open "Client.htm", "_blank", "toolbar=no,location=no,menubar=no,status=yes,resizable=yes"
        Window.Close
    End If
End Sub


Private Sub Form_Load()
    fShiftKey = False
    Set Window = Me
    'Set goTop = helper.arg
    Set Document = Me
    Set goClient = ti.theIClientOcx.VWClient
    Set goWorld = goClient.world
    Set thing = theIAvatar

    Dim sellist, pSelected
    
    'Decide what the initialize selected object should be, default to user
    Set pSelected = Nothing ' goTop.objOEItem
    If pSelected Is Nothing Then Set pSelected = goWorld.User
    
    ' check to see if the Control Manager exists and if it does, use it
    If goWorld.IsValidTool("ControlManager") Then
        Set sellist = goWorld.Tool("ControlManager").SelectionList
    Else
        Set sellist = Nothing
    End If

    If (Not sellist Is Nothing) Then
        If (sellist.Count > 0) Then
            For Each Obj In sellist
                If (Obj.IsStub = False) Then
                    If (Obj.IsOfType("Thing")) Then
                        Set pSelected = Obj
                        Exit For
                    End If
                End If
            Next
        End If
    End If
    
    Set thing.VWClient = goClient
    Set thing.TargetObjectProperty = pSelected
    thing.SelectThing pSelected

    Set ListProp.VWClient = goClient
    Set ListProp.TargetObjectProperty = pSelected
    ListProp.RespondToSelectionEvents = True
    
    Set MethodList.VWClient = goClient
    Set MethodList.TargetObjectProperty = pSelected
    MethodList.RespondToSelectionEvents = True
    
    thing.DoFocus

End Sub

Sub Window_OnUnLoad()
    Set thing.VWClient = Nothing
    Set ListProp.VWClient = Nothing
    Set MethodList.VWClient = Nothing
End Sub

Sub ListProp_UserMessage(msg)
    MsgBox msg, vbExclamation, "Microsoft Virtual Worlds"
End Sub

Sub ListProp_EditValue(thing, name)
    Dim prop
    Dim world
    Dim editorMap
    Dim editorHTML

    On Error Resume Next
    prop = thing.Property(name)
    Set prop = thing.Property(name)
    Set world = goWorld
    Set editorMap = world.Global.Property("PropertyEditors")
    editorHTML = ""
    Dim proptype

    If IsObject(prop) Then
        If Not prop Is Nothing Then
            If IsObject(editorMap) Then
                If editorMap.IsValid(prop.Type) Then
                    editorHTML = editorMap.Property(prop.Type)
                Else
                    proptype = world.Global.GetTypeFromProperty(thing, name)
                    If editorMap.IsValid(proptype) Then
                        editorHTML = editorMap.Property(proptype)
                    Else
                        MsgBox "Can not edit a property of that type.", vbExclamation, "Microsoft Virtual Worlds"
                    End If
                End If
            End If
        Else
            proptype = world.Global.GetTypeFromProperty(thing, name)
            If editorMap.IsValid(proptype) Then
                editorHTML = editorMap.Property(proptype)
            Else
                MsgBox "Can not edit a property of that type", vbExclamation, "Microsoft Virtual Worlds"
            End If
        End If
    Else
        'must be a string
        'well, I can assume that for now
        editorHTML = editorMap.Property("string")
    End If

    If Len(editorHTML) > 0 Then
        Set editThing = thing
        editPropName = name
        iWidth = 200
        iHeight = 200
        sTitle = "Unknown"
        Select Case LCase(Mid(editorHTML, InStrRev(editorHTML, "/") + 1))
        Case "thing.htm"
            iWidth = 235
            iHeight = 291
            sTitle = "Thing Selector"
            sResize = "no"
        Case "string.htm"
            iWidth = 368
            iHeight = 265
            sTitle = "Advanced Text Editor"
            sResize = "no"
        Case "vector.htm"
            iWidth = 330
            iHeight = 155
            sTitle = "Vector Editor"
            sResize = "no"
        Case "menu.htm"
            iWidth = 375
            iHeight = 286
            sTitle = "Menu Editor"
        Case "proplist.htm"
            iWidth = 375
            iHeight = 286
            sTitle = "Property List Editor"
        Case "propmap.htm"
            iWidth = 375
            iHeight = 286
            sTitle = "Property Map Editor"
        Case Else
        End Select
        Select Case LCase(Mid(editorHTML, InStrRev(editorHTML, "/") + 1))
        Case "thing.htm", "string.htm", "vector.htm"
            Window.showModelessDialog goWorld.ContentPath & editorHTML, Array(sTitle, Window), "dialogHeight:" & iHeight & "px;dialogWidth:" & iWidth - 6 & "px;status:no;help:no;resizable:" & sResize & ";"
        Case "menu.htm", "proplist.htm", "propmap.htm"
            Popup.ShowURLEx sTitle, 0, editorHTML, sTitle, False, Window, 30, 30, iWidth, iHeight
        Case Else
        End Select
    End If
End Sub

Sub ListProp_NewProperty(thing)
    Set editThing = thing
    Window.showModelessDialog "newprop.htm", Window, "dialogHeight:217px;dialogWidth:254px;status:no;help:no;resizable:no;"
End Sub

'sub ListProp_DeleteProperty(Thing, list)
'   msgbox "delete properties"
'end sub

Sub ListProp_Help(helpId)
  '  Top.DoHelp helpId
End Sub

Sub ListProp_KeyPressed(KeyCode)
    Select Case KeyCode
    Case 9
        If fShiftKey Then
            thing.DoFocus
        Else
            MethodList.DoFocus
        End If
    Case 16
        fShiftKey = True
    Case 27
        ' Hack to make sure windows has time to process the message before killing this one
        Window.setTimeout "helper.closewindow", 100
    Case Else
    End Select
End Sub

Sub ListProp_vKeyUp(KeyCode)
    Select Case KeyCode
    Case 16
        fShiftKey = False
    Case Else
    End Select
End Sub

Sub MethodList_KeyPressed(KeyCode)
    Select Case KeyCode
    Case 9
        If fShiftKey Then
            ListProp.DoFocus
        Else
            btnClose.focus
        End If
    Case 16
        fShiftKey = True
    Case 27
        ' Hack to make sure windows has time to process the message before killing this one
        Window.setTimeout "helper.closewindow", 100
    Case Else
    End Select
End Sub

Sub MethodList_vKeyUp(KeyCode)
    Select Case KeyCode
    Case 16
        fShiftKey = False
    Case Else
    End Select
End Sub

Sub Thing_KeyPressed(KeyCode)
    Select Case KeyCode
    Case 9
        If fShiftKey Then
            lnkHelp.focus
        Else
            ListProp.DoFocus
        End If
    Case 16
        fShiftKey = True
    Case 27
        ' Hack to make sure windows has time to process the message before killing this one
        Window.setTimeout "helper.closewindow", 100
    Case Else
    End Select
End Sub

Sub Thing_vKeyUp(KeyCode)
    Select Case KeyCode
    Case 16
        fShiftKey = False
    Case Else
    End Select
End Sub

Sub MethodList_UserMessage(msg)
    MsgBox msg, vbExclamation, "Microsoft Virtual Worlds"
End Sub

Sub MethodList_EditValue(thing, name)
    Dim prop
    Dim editorMap
    Dim editorHTML
    Set prop = thing.Method(name)
    If IsObject(prop) Then
        Set editorMap = goWorld.Global.Property("PropertyEditors")
        If IsObject(editorMap) Then
            editorHTML = editorMap.Property(prop.Type)
            Set editThing = thing
            editPropName = name
            Window.showModelessDialog goWorld.ContentPath & editorHTML, Window, "dialogHeight:400px;dialogWidth:494px;status:no;help:no;resizable:yes;"
        End If
    End If
End Sub

Sub MethodList_NewMethod(thing)
    Set editThing = thing
    Window.showModelessDialog "newmeth.htm", Window, "dialogHeight:200px;dialogWidth:269px;status:no;help:no;resizable:no;"
End Sub

Sub Thing_NewThing()
    Window.showModalDialog goWorld.ContentPath & "sdk\wizards\object\main.htm", goTop, "dialogHeight:337px;dialogWidth:468px;help:no;status:no;"
    
End Sub

Sub Thing_LoadModule()
    Window.showModelessDialog "newmod.htm", Window, "dialogHeight:169px;dialogWidth:292px;status:no;help:no;resizable:no;"
End Sub

Sub document_OnMouseOver()
    Select Case Window.Event.srcElement.classname
    Case "bEnabled"
        Window.Event.srcElement.classname = "bHighlight"
    Case "vHelp"
        imgHelp.src = "../../../client/shared/helpH.gif"
    Case Else
    End Select
End Sub

Sub document_OnMouseOut()
    Select Case Window.Event.srcElement.classname
    Case "bHighlight"
        Window.Event.srcElement.classname = "bEnabled"
    Case "vHelp"
        imgHelp.src = "../../../client/shared/help.gif"
    Case Else
    End Select
End Sub

Sub document_onkeypress()
    Select Case Window.Event.KeyCode
    Case 27
        helper.CloseWindow
    Case 9
        fShiftKey = Window.Event.shiftKey
        Select Case Window.Event.srcElement.ID
        Case "btnClose"
            If Window.Event.shiftKey = True Then
                MethodList.DoFocus
            Else
                lnkHelp.focus
            End If
        Case "lnkHelp"
            If Window.Event.shiftKey = True Then
                btnClose.focus
            Else
                thing.DoFocus
            End If
        Case Else
            If Window.Event.shiftKey = True Then
                btnClose.focus
            Else
                thing.DoFocus
            End If
        End Select
    Case Else
    End Select
End Sub

Sub window_onresize()
    bVal = 2    'this value is needed since adding borders to the ActiveX controls gives a different offset value
    If Document.body.offsetWidth - btnClose.offsetWidth - 18 > 550 Then
        newWidth = Round((Document.body.offsetWidth - thing.offsetLeft * 3) * 0.3)
        thing.Style.Width = newWidth - bVal
        lblListPropLeft.Style.Left = thing.offsetLeft * 2 + newWidth
        lblListPropRight.Style.Left = Document.body.offsetWidth - thing.offsetLeft - 253
        ListProp.Style.Left = thing.offsetLeft * 2 + newWidth
        ListProp.Style.Width = Document.body.offsetWidth - thing.offsetLeft * 3 - newWidth - bVal
        lblMethodListLeft.Style.Left = thing.offsetLeft * 2 + newWidth
        lblMethodListRight.Style.Left = Document.body.offsetWidth - thing.offsetLeft - 253
        MethodList.Style.Left = thing.offsetLeft * 2 + newWidth
        MethodList.Style.Width = Document.body.clientWidth - thing.offsetLeft * 3 - newWidth - bVal
        btnClose.Style.Left = Document.body.offsetWidth - btnClose.offsetWidth - 18
    Else
        thing.Style.Width = 170
        lblListPropLeft.Style.Left = 208
        lblListPropRight.Style.Left = 357
        ListProp.Style.Left = 208
        ListProp.Style.Width = 400
        lblMethodListLeft.Style.Left = 208
        lblMethodListRight.Style.Left = 357
        MethodList.Style.Left = 208
        MethodList.Style.Width = 400
        btnClose.Style.Left = 550
    End If
    If Document.body.offsetHeight - btnClose.offsetHeight - 7 > 426 Then
        newHeight = Document.body.offsetHeight - thing.offsetTop - bVal - 10 - btnClose.offsetHeight - 7
        thing.Style.Height = newHeight
        newHeight = (newHeight + bVal - ListProp.offsetTop) / 2 - bVal
        ListProp.Style.Height = newHeight
        lblMethodListLeft.Style.Top = ListProp.offsetTop + newHeight + bVal + lblListPropLeft.offsetTop
        lblMethodListRight.Style.Top = ListProp.offsetTop + newHeight + bVal + lblListPropLeft.offsetTop
        MethodList.Style.Top = ListProp.offsetTop * 2 + newHeight + bVal
        MethodList.Style.Height = newHeight
        btnClose.Style.Top = Document.body.offsetHeight - btnClose.offsetHeight - 7
        imgHelp.Style.Top = Document.body.offsetHeight - imgHelp.offsetHeight - 7
    Else
        thing.Style.Height = 392
        ListProp.Style.Height = 184
        lblMethodListLeft.Style.Top = 215
        lblMethodListRight.Style.Top = 215
        MethodList.Style.Top = 230
        MethodList.Style.Height = 184
        btnClose.Style.Top = 426
        imgHelp.Style.Top = 426
    End If
End Sub

Sub DoHelp()
    Dim client, helpstringsList
    Window.Event.returnValue = False
    On Error Resume Next
    Set client = goClient
    Set helpstringsList = client.world.Global.CreatePropertyList
    helpstringsList.Add "Authoring"
    helpstringsList.Add "Object Explorer"
    client.world.FireUIEvent client.world.User, "Help", helpstringsList
End Sub

