Attribute VB_Name = "Initialization"



Public Sub preptheobjects()

        cm.TheItem = 1
        VectoredObjects = "ivector,imodule"
        textevents_flag = True
        filterex_flag = True
        theHiddenMembers = "World,Container,KeyAccess,GestureText,NamedGesture,Keys,Password,KeyValue,Exemplar,Terminate,Security,IPropertySecurity,type,PropertySecurity,CopyPermission,Initialize,Instance,PropertyType,Enable,IPropertySecurity,Cleanup,Initalize,IID,Parent,propertyname,BitState,PropertyIIDExt,CLSID,KeyOwner,KeyGraphic,addref,GetTypeInfos,GetTypeInfoCount,GetTypeInfo,GetIDsOfNames,world,container,idispatch,queryinterface,_newenum,release,item,dispatch,invoke,iunknown,refcount,getrefcount,iworld,propertysecurity,propertysecurityext,objectpropertyext,methodsecurityext,methodext,propertyext,remoteproperty"
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
        If theInstances Is Nothing Then
            Set theInstances = New PropertyMap
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
                java_vbscript_engine.AddObject "theinstances", theInstances, True
        End If
        
        If ClassMembers Is Nothing Then
            Set ClassMembers = New PropertyMap
        

            
        End If
        
        theWorldName = "pop.opengamer.com/opengamer"
        theAvatarName = "PrologBot"
        
        LocalVWInstall = "c:\Program Files\Microsoft Virtual Worlds"
        
        LoadClassMemberSettings
        
        m_ready_flag = True
        
        m_fSafeForScripting = True              ' set to true if object is SafeForScripting
        m_fSafeForInitializing = True           ' set to true if object is SafeForInitializing
        
        If theMorphism Is Nothing Then
            Set theMorphism = New morphism
        End If
            

        

        If AvatarForm Is Nothing Then
            Set AvatarForm = New fAvatarForm
        End If
        
        If AvatarForm.theAvatarInstance Is Nothing Then
            Set AvatarForm.theAvatarInstance = New AvatarInstance
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
            
        
        If ti Is Nothing Then
            Set ti = New AvatarInstance
           'ti.test_connect
        End If
        
        If theRootDatum Is Nothing Then
            Set theRootDatum = New Datum
            theRootDatum.MemberName = "theRootDatum"
            theRootDatum.SiteKind = SITEKIND_MORPHISM
            theRootDatum.MODELKIND = MODELKIND_COLLECTION
            Set theRootDatum.Value = Nothing
            Set theRootDatum.Models = New PropertyMap
        End If

   
End Sub

Sub PrepDiscourse()

    AllOptions = ""
    
    AllOptions = AllOptions & "Interpreted (CycL/KIF):[opt_kb=prolog,opt_precompiled=false,opt_language=cycl,opt_notation=lisp,disp_cputime]" & vbCrLf
    AllOptions = AllOptions & "Compiled (CycL/KIF):[opt_kb=prolog,opt_precompiled,opt_language=hylog,opt_notation=lisp,disp_cputime]" & vbCrLf
    AllOptions = AllOptions & "Compiled (Prolog):[opt_kb=prolog,opt_precompiled,opt_language=hylog,opt_notation=prolog,disp_cputime]" & vbCrLf
    AllOptions = AllOptions & "Prolog:[]" & vbCrLf
    AllOptions = AllOptions & "Prolog Defaults:[]" & vbCrLf
    AllOptions = AllOptions & "Debugging:[disp_notes_nonuser,disp_debug]" & vbCrLf
    
    
    Set XMLD = New DOMDocument
    'ClosingApp = False
    Client.Show
    Set fDiscourse = New Discourse
    fDiscourse.Show
    Set ResponseForm = New fDocument
    ResponseForm.Caption = "Results"
    ResponseForm.Show
    Set theAvatarForm = New fDocument
    theAvatarForm.Hide
    theAvatarForm.Caption = "Telnet Log"

    CallProlog "ssi."
    
    

End Sub

Sub LoadClassMemberSettings()
            ClassMembers.Property("AvatarProfile") = "Avatar,KeyAccess,KeyGraphic,KeyOwner,Keys,KeyValue"
            ClassMembers.Property("Boundry") = "HeightLower,HeightUpper,IsAutoSolidBoundary,IsClosed,IsInverse,IsPassable,IsProximity,Vertex,VertexCount"
            ClassMembers.Property("Sound") = "is3d,islooping,soundurl"
            ClassMembers.Property("Frame") = "Thing,Type"
            ClassMembers.Property("MenuItem") = "MenuText,Method,OnUpdateMethod,submenu"
            ClassMembers.Property("Method") = "Arguments,Code,Flags,IsInline,MethodName,Module,type"
            ClassMembers.Property("Module") = "Modulename,Type,FLAGS,ScriptPath"
            ClassMembers.Property("ComModule") = "type,modulename,ModuleType,FLAGS"
            ClassMembers.Property("ScriptModule") = "modulename,ModuleType,IsShutdown,Type"
            ClassMembers.Property("PropertyMap") = "ForEachName"
            ClassMembers.Property("PropertyList") = "ForEach"
            ClassMembers.Property("Collection") = "ForEach"
            ClassMembers.Property("Thing") = "Type,Name,ID"
            ClassMembers.Property("Vector") = "Rotation,X,Y,Z"
            ClassMembers.Property("vwgeometrysprite") = "GeometryName,Frame,GeometryFrame,CurrentGesture,ImageFileName,ThumbnailName,RotationAxis,AccessoryScene"
:
End Sub
