# Microsoft Developer Studio Project File - Name="vwfound" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vwfound - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vwfoun60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vwfoun60.mak" CFG="vwfound - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vwfound - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vwfound - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vwfound - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O1 /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /Fd".\Release\vwfound.pdb" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 webpost.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\Release/vwfound.dll"
# Begin Custom Build
OutDir=.\Release
TargetPath=.\Release\vwfound.dll
InputPath=.\Release\vwfound.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /Fd".\Debug\vwfound.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\Debug/vwfound.dll"
# Begin Custom Build
OutDir=.\Debug
TargetPath=.\Debug\vwfound.dll
InputPath=.\Debug\vwfound.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "vwfound - Win32 Release"
# Name "vwfound - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\favatar.cpp
# End Source File
# Begin Source File

SOURCE=.\fglobal.cpp
# End Source File
# Begin Source File

SOURCE=.\fhost.cpp
# End Source File
# Begin Source File

SOURCE=.\foundatn.cpp
# ADD CPP /Yu"vwfound.h"
# End Source File
# Begin Source File

SOURCE=.\fthing.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\mmhelp.cpp
# SUBTRACT CPP /Fr
# End Source File
# Begin Source File

SOURCE=.\painting.cpp
# ADD CPP /Yu"vwfound.h"
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\profhelp.cpp
# End Source File
# Begin Source File

SOURCE=.\sign.cpp
# ADD CPP /Yu"vwfound.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\syshelp.cpp
# End Source File
# Begin Source File

SOURCE=.\TransMch.cpp
# End Source File
# Begin Source File

SOURCE=.\vwfound.cpp
# End Source File
# Begin Source File

SOURCE=.\vwfound.def
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\vwfound.idl

!IF  "$(CFG)" == "vwfound - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__VWFOU="..\..\vwcommon\tranitem.idl"	
# Begin Custom Build
InputPath=..\..\vwcommon\vwfound.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwfound.h" /iid "vwfound_i.c" /I "..\..\vwcommon" /out                             "..\..\vwcommon" "vwfound.idl"

"..\..\vwcommon\vwfound.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\vwcommon\vwfound_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\vwcommon\vwfound.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWFOU="..\..\vwcommon\tranitem.idl"	
# Begin Custom Build
InputPath=..\..\vwcommon\vwfound.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwfound.h" /iid "vwfound_i.c" /I "..\..\vwcommon" /out                             "..\..\vwcommon" "vwfound.idl"

"..\..\vwcommon\vwfound.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\vwcommon\vwfound_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\vwcommon\vwfound.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vwfound.rc
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\vwfound.rc2
# End Source File
# End Group
# Begin Group "Properties"

# PROP Default_Filter ""
# Begin Group "Transaction Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\vwcommon\tranitem.idl

!IF  "$(CFG)" == "vwfound - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\vwcommon\tranitem.idl

BuildCmds= \
	midl /Oicf /no_warn /h "tranitem.h" /iid "tranitem_i.c" /I "..\..\vwcommon" /out                             "..\..\vwcommon" "tranitem.idl"

"..\..\vwcommon\tranitem.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\vwcommon\tranitem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\vwcommon\tranitem.idl

BuildCmds= \
	midl /Oicf /no_warn /h "tranitem.h" /iid "tranitem_i.c" /I "..\..\vwcommon" /out                             "..\..\vwcommon" "tranitem.idl"

"..\..\vwcommon\tranitem.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\..\vwcommon\tranitem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\property\tranitem\tranito.cpp
# End Source File
# End Group
# End Group
# End Target
# End Project
