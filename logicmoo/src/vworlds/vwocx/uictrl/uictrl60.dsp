# Microsoft Developer Studio Project File - Name="uictrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=uictrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "uictrl60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "uictrl60.mak" CFG="uictrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "uictrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "uictrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "uictrl - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 uuid.lib ddraw.lib dsound.lib d3drm.lib wininet.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/vwuictrl.ocx" /libpath:"\dxsdk\sdk\lib" /libpath:"..\..\inetsdk\lib" /libpath:"\mssdk\lib"
# SUBTRACT LINK32 /debug
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=.\Release\vwuictrl.ocx
InputPath=.\Release\vwuictrl.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /Fd"Debug/vwuictrl.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 uuid.lib ddraw.lib dsound.lib d3drm.lib wininet.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/vwuictrl.ocx" /libpath:"\mssdk\lib"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\Debug\vwuictrl.ocx
InputPath=.\Debug\vwuictrl.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "uictrl - Win32 Release"
# Name "uictrl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AvListCt.cpp
# End Source File
# Begin Source File

SOURCE=.\cutil.cpp
# End Source File
# Begin Source File

SOURCE=.\ddutil.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\reghelp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\scripts.cpp
# End Source File
# Begin Source File

SOURCE=.\socxctl.cpp
# End Source File
# Begin Source File

SOURCE=.\soundCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\syshelp.cpp
# ADD CPP /I "..\..\vwsystem"
# End Source File
# Begin Source File

SOURCE=.\uictrl.cpp
# End Source File
# Begin Source File

SOURCE=.\uictrl.def
# End Source File
# Begin Source File

SOURCE=.\uictrl.odl
# End Source File
# Begin Source File

SOURCE=.\uictrl.rc
# End Source File
# Begin Source File

SOURCE=..\vwview\viewctl.cpp
# ADD CPP /I "..\..\vwsystem"
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\vwmenu.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\vwsfvwct.cpp
# End Source File
# Begin Source File

SOURCE=.\whelpct.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ahistctl.h
# End Source File
# Begin Source File

SOURCE=.\AvListCt.h
# End Source File
# Begin Source File

SOURCE=.\AvListPp.h
# End Source File
# Begin Source File

SOURCE=.\cutils.h
# End Source File
# Begin Source File

SOURCE=.\gestbutt.h
# End Source File
# Begin Source File

SOURCE=.\GstrCtl.h
# End Source File
# Begin Source File

SOURCE=.\GstrPpg.h
# End Source File
# Begin Source File

SOURCE=.\HistCtl.h
# End Source File
# Begin Source File

SOURCE=.\HistPpg.h
# End Source File
# Begin Source File

SOURCE=..\..\manager\reghelp.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoomCtl.h
# End Source File
# Begin Source File

SOURCE=.\RoomPpg.h
# End Source File
# Begin Source File

SOURCE=.\SelfCtl.h
# End Source File
# Begin Source File

SOURCE=.\SelfPpg.h
# End Source File
# Begin Source File

SOURCE=.\socxctl.h
# End Source File
# Begin Source File

SOURCE=.\SpkrCtl.h
# End Source File
# Begin Source File

SOURCE=.\SpkrPpg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\uictrl.h
# End Source File
# Begin Source File

SOURCE=.\UserCtl.h
# End Source File
# Begin Source File

SOURCE=.\UserPpg.h
# End Source File
# Begin Source File

SOURCE=..\vwview\viewctl.h
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\vwmenu.h
# End Source File
# Begin Source File

SOURCE=.\whelpct.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\AvListCt.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap16.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\cliocCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\execa.bmp
# End Source File
# Begin Source File

SOURCE=.\exech.bmp
# End Source File
# Begin Source File

SOURCE=.\execi.bmp
# End Source File
# Begin Source File

SOURCE=.\GstrCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\HistCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\ignore.bmp
# End Source File
# Begin Source File

SOURCE=.\RoomCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\saya.bmp
# End Source File
# Begin Source File

SOURCE=.\sayh.bmp
# End Source File
# Begin Source File

SOURCE=.\sayi.bmp
# End Source File
# Begin Source File

SOURCE=.\SelfCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\shouta.bmp
# End Source File
# Begin Source File

SOURCE=.\shouth.bmp
# End Source File
# Begin Source File

SOURCE=.\shouti.bmp
# End Source File
# Begin Source File

SOURCE=.\socxctl.bmp
# End Source File
# Begin Source File

SOURCE=.\SpkrCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\treeicon.bmp
# End Source File
# Begin Source File

SOURCE=.\uictrl.ico
# End Source File
# Begin Source File

SOURCE=.\UserCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\VWSfVwCt.bmp
# End Source File
# Begin Source File

SOURCE=.\WHelpCt.bmp
# End Source File
# End Group
# Begin Group "D2DUtils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\D2d\D2dutils\Src\Errmgr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\D2d\D2dutils\Src\Mmutils.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# End Target
# End Project
