# Microsoft Developer Studio Project File - Name="vwstudio" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vwstudio - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vwstud60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vwstud60.mak" CFG="vwstudio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vwstudio - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vwstudio - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /Fd"Debug/vwstudio.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 d3drm.lib dxguid.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/vwstudio.dll" /libpath:"\mssdk\lib"
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\Debug\vwstudio.dll
InputPath=.\Debug\vwstudio.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Yu"stdafx.h" /Fd"ReleaseMinSize/vwstudio.pdb" /FD /c
# SUBTRACT CPP /X /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 d3drm.lib dxguid.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseMinSize/vwstudio.dll" /libpath:"\mssdk\lib"
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\vwstudio.dll
InputPath=.\ReleaseMinSize\vwstudio.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "vwstudio - Win32 Debug"
# Name "vwstudio - Win32 Release MinSize"
# Begin Group "VWStudio"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\vwcommon\cathelp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\dlldatax.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\syshelp.cpp
# End Source File
# Begin Source File

SOURCE=.\vwstudio.cpp
# ADD CPP /I "..\tools\stdtools" /I "..\tools\geomtool" /I "..\tools\celltool" /I "..\tools\popupmgr" /I "..\tools\webtools" /I "..\tools\ecpop" /I "..\tools\menupop" /I "..\tools\thingpop" /I "..\tools\verbpop" /I "..\module\studio"
# End Source File
# Begin Source File

SOURCE=.\vwstudio.def
# End Source File
# Begin Source File

SOURCE=.\vwstudio.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWSTU="..\vwcommon\cellprop.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwgeom.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwframe.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwnvtool.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwgeomar.idl"	
# Begin Custom Build
InputPath=.\vwstudio.idl

BuildCmds= \
	midl /no_warn /Oicf /h "vwstudio.h" /iid "vwstudio_i.c" /I "..\vwcommon" "vwstudio.idl"

".\vwstudio.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwstudio_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwstudio.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWSTU="..\vwcommon\cellprop.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwgeom.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwframe.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwnvtool.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwgeomar.idl"	
# Begin Custom Build
InputPath=.\vwstudio.idl

BuildCmds= \
	midl /no_warn /Oicf /h "vwstudio.h" /iid "vwstudio_i.c" /I "..\vwcommon" "vwstudio.idl"

".\vwstudio.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwstudio_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwstudio.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vwstudio.rc
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwuiobjs_p.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Tools"

# PROP Default_Filter ""
# Begin Group "Standard"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tools\stdtools\ctrlmgr.cpp
# ADD CPP /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\select.cpp
# ADD CPP /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\stdtools.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\stdtools.idl

BuildCmds= \
	midl /no_warn /Oicf /h "stdtools.h" /iid "stdtools_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "stdtools.idl"

"..\vwcommon\stdtools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\stdtools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\stdtools.idl

BuildCmds= \
	midl /no_warn /Oicf /h "stdtools.h" /iid "stdtools_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "stdtools.idl"

"..\vwcommon\stdtools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\stdtools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\undoitem.cpp
# ADD CPP /I "..\tools\geomtool" /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\undostak.cpp
# ADD CPP /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\vwerrtl.cpp
# ADD CPP /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwsgfxut.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

# ADD CPP /Yu"stdtools.h"

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwsutils.cpp
# ADD CPP /Yu"stdtools.h"
# End Source File
# End Group
# Begin Group "Geometry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tools\geomtool\geomsel.cpp
# ADD CPP /Yu"geomtool.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\geomtool.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\geomtool.idl

BuildCmds= \
	midl /no_warn /Oicf /h "geomtool.h" /iid "geomtool_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "geomtool.idl"

"..\vwcommon\geomtool.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\geomtool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\geomtool.idl

BuildCmds= \
	midl /no_warn /Oicf /h "geomtool.h" /iid "geomtool_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "geomtool.idl"

"..\vwcommon\geomtool.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\geomtool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\geomtool\geomundo.cpp
# ADD CPP /Yu"geomtool.h"
# End Source File
# Begin Source File

SOURCE=..\tools\geomtool\rottool.cpp
# ADD CPP /Yu"geomtool.h"
# End Source File
# Begin Source File

SOURCE=..\tools\geomtool\scaletl.cpp
# ADD CPP /Yu"geomtool.h"
# End Source File
# Begin Source File

SOURCE=..\tools\geomtool\trans3d.cpp
# ADD CPP /Yu"geomtool.h"
# End Source File
# Begin Source File

SOURCE=..\tools\geomtool\undoutil.cpp
# ADD CPP /Yu"geomtool.h"
# End Source File
# End Group
# Begin Group "Web"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tools\webtools\FileAcTl.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\reghelp.cpp
# End Source File
# Begin Source File

SOURCE=..\tools\webtools\rwhelper.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\webtools.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\webtools.idl

BuildCmds= \
	midl /no_warn /Oicf /h "webtools.h" /iid "webtools_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "webtools.idl"

"..\vwcommon\webtools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\webtools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\webtools.idl

BuildCmds= \
	midl /no_warn /Oicf /h "webtools.h" /iid "webtools_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "webtools.idl"

"..\vwcommon\webtools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\webtools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\webtools\wwhelper.cpp
# ADD CPP /Yu"webtools.h"
# End Source File
# End Group
# Begin Group "Boundary"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\bndytool.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\bndytool.idl

BuildCmds= \
	midl /no_warn /Oicf /h "bndytool.h" /iid "bndytool_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "bndytool.idl"

"..\vwcommon\bndytool.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\bndytool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\bndytool.idl

BuildCmds= \
	midl /no_warn /Oicf /h "bndytool.h" /iid "bndytool_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "bndytool.idl"

"..\vwcommon\bndytool.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\bndytool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\polytool.cpp
# ADD CPP /I "..\tools\geomtool" /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\rotat2d.cpp
# ADD CPP /I "..\tools\geomtool" /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\scale2d.cpp
# ADD CPP /I "..\tools\geomtool" /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\trans2d.cpp
# ADD CPP /I "..\tools\geomtool" /Yu"stdtools.h"
# End Source File
# Begin Source File

SOURCE=..\tools\stdtools\ZoomTool.cpp
# End Source File
# End Group
# End Group
# Begin Group "Modules"

# PROP Default_Filter ""
# Begin Group "Studio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\vwstuex.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwstuex.idl

BuildCmds= \
	midl /no_warn /Oicf /h "vwstuex.h" /iid "vwstuex_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "vwstuex.idl"

"..\vwcommon\vwstuex.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwstuex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwstuex.idl

BuildCmds= \
	midl /no_warn /Oicf /h "vwstuex.h" /iid "vwstuex_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "vwstuex.idl"

"..\vwcommon\vwstuex.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwstuex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\module\studio\vwstuexo.cpp
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\CopyTool.rgs
# End Source File
# Begin Source File

SOURCE=.\ctrlmgr.rgs
# End Source File
# Begin Source File

SOURCE=.\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\CutTool.rgs
# End Source File
# Begin Source File

SOURCE=.\delvert.cur
# End Source File
# Begin Source File

SOURCE=.\DelVert.rgs
# End Source File
# Begin Source File

SOURCE=.\delxslct.rgs
# End Source File
# Begin Source File

SOURCE=.\draw.cur
# End Source File
# Begin Source File

SOURCE=.\FileAcTl.rgs
# End Source File
# Begin Source File

SOURCE=.\GeomUndo.rgs
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\InsVert.rgs
# End Source File
# Begin Source File

SOURCE=.\nav.cur
# End Source File
# Begin Source File

SOURCE=.\NavBound.rgs
# End Source File
# Begin Source File

SOURCE=.\navigate.cur
# End Source File
# Begin Source File

SOURCE=.\PolyTool.rgs
# End Source File
# Begin Source File

SOURCE=.\Rotat2D.rgs
# End Source File
# Begin Source File

SOURCE=.\rotate.cur
# End Source File
# Begin Source File

SOURCE=.\rottool.rgs
# End Source File
# Begin Source File

SOURCE=.\RWHelper.rgs
# End Source File
# Begin Source File

SOURCE=.\scale.cur
# End Source File
# Begin Source File

SOURCE=.\scale2d.rgs
# End Source File
# Begin Source File

SOURCE=.\ScaleTl.rgs
# End Source File
# Begin Source File

SOURCE=.\select.cur
# End Source File
# Begin Source File

SOURCE=.\select.rgs
# End Source File
# Begin Source File

SOURCE=.\Trans2d.rgs
# End Source File
# Begin Source File

SOURCE=.\translat.cur
# End Source File
# Begin Source File

SOURCE=.\UndoItem.rgs
# End Source File
# Begin Source File

SOURCE=.\undostak.rgs
# End Source File
# Begin Source File

SOURCE=.\vwerrtl.rgs
# End Source File
# Begin Source File

SOURCE=.\vwstuex.rgs
# End Source File
# Begin Source File

SOURCE=.\VWTrans.rgs
# End Source File
# Begin Source File

SOURCE=.\vxinsert.cur
# End Source File
# Begin Source File

SOURCE=.\WWHelper.rgs
# End Source File
# Begin Source File

SOURCE=.\zoomin.cur
# End Source File
# Begin Source File

SOURCE=.\zoomout.cur
# End Source File
# Begin Source File

SOURCE=.\ZoomTool.rgs
# End Source File
# End Target
# End Project
