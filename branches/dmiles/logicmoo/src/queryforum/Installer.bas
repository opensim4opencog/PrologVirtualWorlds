Attribute VB_Name = "Installer"

Public Sub installCheck()
On Error Resume Next

Set theSoftwareRegKey = RegObj.Registry.RegKeyFromHKey(HKEY_LOCAL_MACHINE).SubKeys("SOFTWARE")
SWIInstallPath = theSoftwareRegKey.SubKeys("SWI").SubKeys("PROLOG").Values("HOME").Value
SWILibPath = SWIInstallPath & "\Library"
SWIWorkPath = theSoftwareRegKey.SubKeys("SWI").SubKeys("PROLOG").Values("CWD").Value
SWIExtension = theSoftwareRegKey.SubKeys("SWI").SubKeys("PROLOG").Values("fileExtension").Value
VWInstallPath = theSoftwareRegKey.SubKeys("Microsoft").SubKeys("V-Worlds").Values("InstallPath").Value
VWContentPath = theSoftwareRegKey.SubKeys("Microsoft").SubKeys("V-Worlds").SubKeys("Paths").Values("ContentPath").Value
PVWExePath = App.path
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
  '  installCheck
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
