Attribute VB_Name = "Initalization"
Option Explicit


Public Sub preptheobjects()

        cm.TheItem = 1
        VectoredObjects = "ivector,imodule"
        textevents_flag = True
        filterex_flag = True
        theHiddenMembers = "World,Container,KeyAccess,GestureText,NamedGesture,Keys,Password,KeyValue,Exemplar,Terminate,Security,IPropertySecurity,type,PropertySecurity,CopyPermission,Initialize,Instance,PropertyType,Enable,IPropertySecurity,Cleanup,Initalize,IID,Parent,propertyName,BitState,PropertyIIDExt,CLSID,KeyOwner,KeyGraphic,addref,GetTypeInfos,GetTypeInfoCount,GetTypeInfo,GetIDsOfNames,world,container,idispatch,queryinterface,_newenum,release,item,dispatch,invoke,iunknown,refcount,getrefcount,iworld,propertysecurity,propertysecurityext,objectpropertyext,methodsecurityext,methodext,propertyext,remoteproperty"
        theRefMembers = "world,container"
   '     Set david_hovel = New MSBN3Lib.MSBN
'        Stop
        If theDatums Is Nothing Then
            Set theDatums = New PropertyMap
            
        End If
        
        stringlist_opener = "'"
        string_terminator = "'"
        stringlist_opener = "["
        stringlist_terminator = "]"
        stringlist_seperator = ","
        comment_opener = "/*"
        comment_terminator = "*/"
        comment_delimiter = "%"
        object_opener = "('"
        object_terminator = "')"
        typeobject_flag = True
        objectlcase_flag = True
        stringlcase_flag = True
        string_liststrs_flag = True
        list_opener = "["
        list_seperator = ","
        list_terminator = "]"
        argument_opener = "("
        argument_seperator = ","
        argument_terminator = ")"
        set_opener = "["
        set_seperator = ","
        set_terminator = "]"
        command_seperator = ". "
        
        Set escapable_chars = New PropertyList
        escapable_chars.RemoveAll
        escapable_chars.Add (".")
        escapable_chars.Add (",")
        escapable_chars.Add ("'")
        escapable_chars.Add ("/")
        escapable_chars.Add ("-")
        escapable_chars.Add ("*")
        escapable_chars.Add ("+")
        escapable_chars.Add ("%")
        escapable_chars.Add (Chr$(34))
        escapable_chars.Add ("{")
        escapable_chars.Add ("}")
        escapable_chars.Add (":")
        escapable_chars.Add ("(")
        escapable_chars.Add (")")

        expand_ithings_flag = False
        nl_proc_flag = True
        If theAddRefs Is Nothing Then
            Set theAddRefs = New Collection
        End If
        If theKnownMorphs Is Nothing Then
            Set theKnownMorphs = New PropertyMap
         End If
        
        If theTypeNameInterfaces Is Nothing Then
            Set theTypeNameInterfaces = New PropertyMap
         End If
        
        If theLoadedVBSModules Is Nothing Then
            Set theLoadedVBSModules = New PropertyList
         End If
        
        If java_vbscript_engine Is Nothing Then
                Set java_vbscript_engine = New msscriptcontrol.ScriptControl
                java_vbscript_engine.Language = "vbscript"
                java_vbscript_engine.AllowUI = True
                java_vbscript_engine.UseSafeSubset = False
                java_vbscript_engine.ExecuteStatement "set regex = new regexp"
                java_vbscript_engine.ExecuteStatement "regex.ignorecase = true"
                java_vbscript_engine.ExecuteStatement "regex.global = true"
                java_vbscript_engine.AddObject "theKnownMorphs", theKnownMorphs, True
        End If
        
        If ClassMembers Is Nothing Then
            Set ClassMembers = New PropertyMap
        

            
        End If
        
        theWorldName = "worlds.dev.aliensystems.com/sample"
        theAvatarName = "PrologBot"
        
        LocalVWInstall = "c:\Program Files\Microsoft Virtual Worlds"
        
            ClassMembers.Property("IAvatarProfile") = "Avatar,Keys"
            ClassMembers.Property("AvatarProfile") = "Avatar,KeyAccess,KeyGraphic,KeyOwner,Keys,KeyValue"
            ClassMembers.Property("Boundry") = "HeightLower,HeightUpper,IsAutoSolidBoundary,IsClosed,IsInverse,IsPassable,IsProximity,Vertex,VertexCount"
            ClassMembers.Property("Sound") = "is3d,islooping,soundurl"
            ClassMembers.Property("Frame") = "Thing,Type"
            ClassMembers.Property("MenuItem") = "Arguments,Type,Flags,MenuText,Method,OnUpdateMethod,type"
            ClassMembers.Property("Method") = "Arguments,Code,Flags,IsInline,MethodName,Module,type"
            ClassMembers.Property("IMethod") = "Code,Flags,IsInline,MethodName,Module,type"
            ClassMembers.Property("Module") = "ModuleName,Type,FLAGS,ScriptPath"
                ClassMembers.Property("ComModule") = "type,moduleName,ModuleType,FLAGS"
                ClassMembers.Property("ScriptModule") = "moduleName,ModuleType,IsShutdown,Type"
            ClassMembers.Property("PropertyList") = "ForEach"
            ClassMembers.Property("IPropertyList") = "ForEach"
            ClassMembers.Property("Collection") = "ForEach"
            ClassMembers.Property("PropertyMap") = "ForEachName"
            ClassMembers.Property("IPropertyMap") = "ForEachName"
            ClassMembers.Property("IThing") = "Type,ID,Name"
            ClassMembers.Property("Thing") = "Type,ID,Name"
          '  ClassMembers.Property("IThing") = "Name,ID,Exemplar,isExemplar,Owner,Container,Type,Owner"
''            ClassMembers.Property("Thing") = "Name,ID,Exemplar,isExemplar,Owner,Container,MorphismProperties,MorphismMethods,Contents,Type,Owner"
            ClassMembers.Property("ivector") = "Rotation,X,Y,Z"
            ClassMembers.Property("Vector") = "Rotation,X,Y,Z"
        
        m_ready_flag = True
        
        m_fSafeForScripting = True              ' set to true if object is SafeForScripting
        m_fSafeForInitializing = True           ' set to true if object is SafeForInitializing
        
        If theMorphism Is Nothing Then
            Set theMorphism = New Morphism
        End If
        
        If theCOMMorphism Is Nothing Then
            Set theCOMMorphism = New COMMorphism
        End If
            
        If theMainForm Is Nothing Then
            Set theMainForm = New fMainMDI
        End If
        
        If thePrologTracer Is Nothing Then
            Set thePrologTracer = New fPrologTracer
        End If
        
        If thePrologTracer.theAvatarMorphism Is Nothing Then
            Set thePrologTracer.theAvatarMorphism = New AvatarMorphism
        End If
        
        If theSWIPrologShell Is Nothing Then
            Set theSWIPrologShell = New SWIProlog
        End If
        
        
        
        If theComClassBrowser Is Nothing Then
            Set theComClassBrowser = New fComClassBrowser
        End If
        
        'If theComBrowser Is Nothing Then Set theComBrowser = New fCOMBroswer
        
        If DLLFiles Is Nothing Then
            Set DLLFiles = New PropertyMap
            load_default_dlls
        End If
            
        
        If theAvatarMorphism Is Nothing Then
            Set theAvatarMorphism = thePrologTracer.theAvatarMorphism
           'ti.test_connect
        End If
        
        
        If theRootDatum Is Nothing Then
            Set theRootDatum = New a_Datum
            theRootDatum.MemberName = "theRootDatum"
            theRootDatum.SiteKind = SITEKIND_instance
            theRootDatum.MODELKIND = MODELKIND_COLLECTION
            Set theRootDatum.Value = Nothing
            Set theRootDatum.Models = New PropertyMap
        End If

   
End Sub


Sub load_default_dlls()
        theCOMMorphism.add_com_dll ("c:\program files\microsoft virtual worlds\vwmm.dll")
        theCOMMorphism.add_com_dll ("c:\msr\VWorlds\vwsystem\ReleaseMinSize\vwsystem.dll")
        'theCOMMorphism.add_com_dll ("C:\Msr\output\PrologVirtualWorlds.exe")
       ' theCOMMorphism.add_com_dll ("C:\Msr\output\msvbvm60.dll")
        'theCOMMorphism.add_com_dll ("C:\Msr\output\OLEPRO32.DLL")
     '   theCOMMorphism.add_com_dll ("C:\Msr\output\OLEAUT32.DLL")
End Sub

Public Sub installCheck()
On Error Resume Next

Set theSoftwareRegKey = RegObj.Registry.RegKeyFromHKey(HKEY_LOCAL_MACHINE).SubKeys("SOFTWARE")
SWIInstallPath = theSoftwareRegKey.SubKeys("SWI").SubKeys("PROLOG").Values("HOME").Value
SWILibPath = SWIInstallPath & "\Library"
SWIWorkPath = theSoftwareRegKey.SubKeys("SWI").SubKeys("PROLOG").Values("CWD").Value
SWIExtension = theSoftwareRegKey.SubKeys("SWI").SubKeys("PROLOG").Values("fileExtension").Value
VWInstallPath = theSoftwareRegKey.SubKeys("Microsoft").SubKeys("V-Worlds").Values("InstallPath").Value
VWContentPath = theSoftwareRegKey.SubKeys("Microsoft").SubKeys("V-Worlds").SubKeys("Paths").Values("ContentPath").Value
PVWExePath = App.Path
If LCase(Right$(PVWExePath, 6)) = "System" Then PVWInstallPathNow = Left$(PVWExePath, Len(PVWExePath) - 7)
If LCase(Right$(PVWExePath, 3)) = "src" Then PVWInstallPathNow = Left$(PVWExePath, Len(PVWExePath) - 4)
PVWInstallPath = theSoftwareRegKey.SubKeys("Douglas Miles").SubKeys("PrologVirtualWorlds").SubKeys("Paths").Values("InstallPath").Value
PVWProjectPath = theSoftwareRegKey.SubKeys("Douglas Miles").SubKeys("PrologVirtualWorlds").SubKeys("Paths").Values("ProjectPath").Value
PVWLibraryPath = theSoftwareRegKey.SubKeys("Douglas Miles").SubKeys("PrologVirtualWorlds").SubKeys("Paths").Values("LibraryPath").Value
PVWUIClientPath = theSoftwareRegKey.SubKeys("Douglas Miles").SubKeys("PrologVirtualWorlds").SubKeys("Paths").Values("UIClientPath").Value


FileSystem.FileCopy PVWInstallPathNow & "\support\3dview.htm", VWContentPath & "client\html\3dview.htm"
'FileSystem.FileCopy PVWInstallPathNow & "\support\vbClient.htm", VWContentPath & "client\html\vbClient.htm"

If PVWInstallPath = "" Then
    theSoftwareRegKey.SubKeys.Add "Douglas Miles"
    theSoftwareRegKey.SubKeys("Douglas Miles").SubKeys.Add "PrologVirtualWorlds"
    Set PVWRegKey = theSoftwareRegKey.SubKeys("Douglas Miles").SubKeys("PrologVirtualWorlds")
    PVWRegKey.SubKeys.Add "Paths"
    PVWRegKey.SubKeys("Paths").Values.Add "InstallPath", PVWInstallPathNow, RegValueType.rvString
    PVWRegKey.SubKeys("Paths").Values.Add "ProjectPath", PVWInstallPathNow & "\Projects", RegValueType.rvString
    PVWRegKey.SubKeys("Paths").Values.Add "LibraryPath", PVWInstallPathNow & "\PrologLibrary", RegValueType.rvString
    PVWRegKey.SubKeys("Paths").Values.Add "UIClientPath", VWContentPath & "client\html\vbClient.htm", RegValueType.rvString
    installCheck
Else
    Set PVWRegKey = theSoftwareRegKey.SubKeys("Douglas Miles").SubKeys("PrologVirtualWorlds")
End If
'Stop

Shell "regsvr32 /c /s " & PVWInstallPathNow & "\system\vwsystem.dll"
Shell "regsvr32 /c /s " & PVWInstallPathNow & "\system\vwmm.dll"
Shell "regsvr32 /c /s " & PVWInstallPathNow & "\system\Editeur.ocx"
Shell "regsvr32 /c /s " & VWInstallPath & "\vwobjexp.ocx"
Shell "regsvr32 /c /s " & VWInstallPath & "\vwrendvw.ocx"
Shell "regsvr32 /c /s " & VWInstallPath & "\vwuictrl.ocx"

'Stop
If SWIInstallPath = "" Then
    If vbYes = MsgBox("SWI-Prolog was not detected in your system registry." & vbCrLf & vbCrLf & "Would you like to install it now?", vbYesNo) Then
        MsgBox "After SWI-Prolog has been installed, Please restart PrologVirualWorlds to continue", vbOKOnly
        Shell PVWInstallPathNow & "\Support\w32pl3310.exe"
        End
    Else
        MsgBox "Installation of SWI-Prolog must be complete before running this application", vbOKOnly
        End
    End If
End If

If FileSystem.FileLen(SWIInstallPath & "\bin\proquery.dll") < 100 Then
    MsgBox "SWIActX and ProQuery will now be installed onto your system.", vbOK
    FileSystem.FileCopy PVWInstallPathNow & "\support\proquery.dll", SWIInstallPath & "\bin\ProQuery.Dll"
    FileSystem.FileCopy PVWInstallPathNow & "\support\swiactx.dll", SWIInstallPath & "\bin\SwiActX.Dll"
    FileSystem.FileCopy PVWInstallPathNow & "\support\swiactx.pl", SWIInstallPath & "\library\SwiActX.pl"
End If

Shell "regsvr32 /c /s " & SWIInstallPath & "\bin\proquery.dll"


If VWContentPath = "" Then
    MsgBox "It seems that you have not installed Microsoft Virtual Worlds Samples" & vbCrLf & vbCrLf & "This is alright, this program will be functional", vbOKOnly
    
End If

End Sub


Public Sub LoadResStrings(frm As Form)
   On Error Resume Next

    Dim ctl As Control
    Dim Obj As Object
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
        sCtlType = typeName(ctl)
        If sCtlType = "Label" Then
            ctl.Caption = LoadResString(CInt(ctl.Tag))
        ElseIf sCtlType = "Menu" Then
            ctl.Caption = LoadResString(CInt(ctl.Caption))
        ElseIf sCtlType = "TabStrip" Then
            For Each Obj In ctl.Tabs
                Obj.Caption = LoadResString(CInt(Obj.Tag))
                Obj.ToolTipText = LoadResString(CInt(Obj.ToolTipText))
            Next
        ElseIf sCtlType = "Toolbar" Then
            For Each Obj In ctl.Buttons
                Obj.ToolTipText = LoadResString(CInt(Obj.ToolTipText))
            Next
        ElseIf sCtlType = "ListView" Then
            For Each Obj In ctl.ColumnHeaders
                Obj.Text = LoadResString(CInt(Obj.Tag))
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


