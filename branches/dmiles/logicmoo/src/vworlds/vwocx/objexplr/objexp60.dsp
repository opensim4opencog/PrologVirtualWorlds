# Microsoft Developer Studio Project File - Name="objexplr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=objexplr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "objexp60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "objexp60.mak" CFG="objexplr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "objexplr - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "objexplr - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "objexplr - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O1 /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /I "..\..\vwstudio" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/vwobjexp.ocx"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=.\Release\vwobjexp.ocx
InputPath=.\Release\vwobjexp.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\vwstudio" /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /D "_DEBUG" /D "USE_CONTROLMANAGER" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FR /Yu"stdafx.h" /Fd"Debug/vwobjexp.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/vwobjexp.ocx"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\Debug\vwobjexp.ocx
InputPath=.\Debug\vwobjexp.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "objexplr - Win32 Release"
# Name "objexplr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3clmlist.cpp
# End Source File
# Begin Source File

SOURCE=.\4ClmList.cpp
# End Source File
# Begin Source File

SOURCE=.\booldrop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\cmddel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\EdLstCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\LEButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ListList.cpp
# End Source File
# Begin Source File

SOURCE=.\lpedit.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuList.cpp
# End Source File
# Begin Source File

SOURCE=.\methdrop.cpp
# End Source File
# Begin Source File

SOURCE=.\methlist.cpp
# End Source File
# Begin Source File

SOURCE=.\MExistDl.cpp
# End Source File
# Begin Source File

SOURCE=.\MListCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\moddrop.cpp
# End Source File
# Begin Source File

SOURCE=.\objexplr.cpp
# End Source File
# Begin Source File

SOURCE=.\objexplr.def
# End Source File
# Begin Source File

SOURCE=.\objexplr.odl
# End Source File
# Begin Source File

SOURCE=.\objexplr.rc
# End Source File
# Begin Source File

SOURCE=.\PExistDl.cpp
# End Source File
# Begin Source File

SOURCE=.\PListCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\PMapCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\PrpEdCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\syshelp.cpp
# End Source File
# Begin Source File

SOURCE=.\ThingCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\tplstctl.cpp
# End Source File
# Begin Source File

SOURCE=.\Tree.cpp
# End Source File
# Begin Source File

SOURCE=.\tvlstctl.cpp
# End Source File
# Begin Source File

SOURCE=..\vwview\viewctl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vwstudio\vwstudio_i.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\vwuiview.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3clmlist.h
# End Source File
# Begin Source File

SOURCE=.\4ClmList.h
# End Source File
# Begin Source File

SOURCE=.\booldrop.h
# End Source File
# Begin Source File

SOURCE=.\EdLstCtl.h
# End Source File
# Begin Source File

SOURCE=.\LEButton.h
# End Source File
# Begin Source File

SOURCE=.\listdata.h
# End Source File
# Begin Source File

SOURCE=.\ListList.h
# End Source File
# Begin Source File

SOURCE=.\lpedit.h
# End Source File
# Begin Source File

SOURCE=.\MenuList.h
# End Source File
# Begin Source File

SOURCE=.\methdrop.h
# End Source File
# Begin Source File

SOURCE=.\methlist.h
# End Source File
# Begin Source File

SOURCE=.\MExistDl.h
# End Source File
# Begin Source File

SOURCE=.\MListCtl.h
# End Source File
# Begin Source File

SOURCE=.\moddrop.h
# End Source File
# Begin Source File

SOURCE=.\objexplr.h
# End Source File
# Begin Source File

SOURCE=.\PExistDl.h
# End Source File
# Begin Source File

SOURCE=.\PListCtl.h
# End Source File
# Begin Source File

SOURCE=.\PMapCtl.h
# End Source File
# Begin Source File

SOURCE=.\PrpEdCtl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ThingCtl.h
# End Source File
# Begin Source File

SOURCE=.\tplstctl.h
# End Source File
# Begin Source File

SOURCE=.\Tree.h
# End Source File
# Begin Source File

SOURCE=.\tvlstctl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\meths.bmp
# End Source File
# Begin Source File

SOURCE=.\MListCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\objexplr.rc2
# End Source File
# Begin Source File

SOURCE=.\PListCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\PMapCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\props.bmp
# End Source File
# Begin Source File

SOURCE=.\PrpEdCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\ThingCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\tplstctl.bmp
# End Source File
# Begin Source File

SOURCE=.\treeicon.bmp
# End Source File
# Begin Source File

SOURCE=.\tvlstctl.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
