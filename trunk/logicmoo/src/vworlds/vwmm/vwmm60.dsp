# Microsoft Developer Studio Project File - Name="vwmm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VWMM - WIN32 RELEASE MINSIZE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vwmm60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vwmm60.mak" CFG="VWMM - WIN32 RELEASE MINSIZE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vwmm - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vwmm - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vwmm - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Yu"stdafx.h" /Fd"Debug/vwmm.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 /Os
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 winmm.lib ddraw.lib d3drm.lib uuid.lib ftkvc40.lib rpcndr.lib rpcns4.lib rpcrt4.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCMT" /nodefaultlib:"LIBC" /out:"Debug/vwmm.dll" /libpath:"\mssdk\lib" /libpath:"..\tools\3dstod3d\3dsftk\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\Debug\vwmm.dll
InputPath=.\Debug\vwmm.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Yu"stdafx.h" /Fd"ReleaseMinSize/vwmm.pdb" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 /Os
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib ddraw.lib d3drm.lib uuid.lib ftkvc40.lib rpcndr.lib rpcns4.lib rpcrt4.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCMT" /nodefaultlib:"LIBC" /out:"ReleaseMinSize/vwmm.dll" /libpath:"\mssdk\lib" /libpath:"..\tools\3dstod3d\3dsftk\lib"
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\vwmm.dll
InputPath=.\ReleaseMinSize\vwmm.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "vwmm - Win32 Debug"
# Name "vwmm - Win32 Release MinSize"
# Begin Group "VWMM"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\vwcommon\cathelp.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# ADD CPP /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound"
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# ADD CPP /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound"
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dlldatax.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\vwcommon\mmhelp.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# ADD CPP /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vector" /I "..\property\vwadata" /I "..\property\sound" /I "..\property\cellprop" /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\vwidata" /I "..\property\vwgeomar" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData"

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# ADD CPP /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vector" /I "..\property\vwadata" /I "..\property\sound" /I "..\property\cellprop" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\vwidata" /I "..\property\vwgeomar" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# ADD CPP /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# ADD CPP /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwcommon\syshelp.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# ADD CPP /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound"

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# ADD CPP /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vwmm.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# ADD CPP /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound"

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# ADD CPP /I "..\property\Sound" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vwmm.def
# End Source File
# Begin Source File

SOURCE=.\vwmm.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWMM_="..\vwcommon\cellprop.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwgeom.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwframe.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwnvtool.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwgeomar.idl"	"..\vwcommon\vwadata.idl"	"..\vwcommon\pickdata.idl"	"..\vwcommon\vwgeomsp.idl"	"..\vwcommon\vwmmex.idl"	"..\vwcommon\vwtrans.idl"	
# Begin Custom Build
InputPath=.\vwmm.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwmm.h" /iid "vwmm_i.c" /I "..\vwcommon" "vwmm.idl"

".\vwmm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwmm_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwmm.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWMM_="..\vwcommon\cellprop.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwgeom.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwframe.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwnvtool.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwgeomar.idl"	"..\vwcommon\vwadata.idl"	"..\vwcommon\pickdata.idl"	"..\vwcommon\vwgeomsp.idl"	"..\vwcommon\vwmmex.idl"	"..\vwcommon\vwtrans.idl"	
# Begin Custom Build
InputPath=.\vwmm.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwmm.h" /iid "vwmm_i.c" /I "..\vwcommon" "vwmm.idl"

".\vwmm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwmm_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwmm.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vwmm.rc
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwuiobjs_p.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "UI Tools"

# PROP Default_Filter "*.cpp;*.idl"
# Begin Group "Pick Tool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\vwmenu.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\uitools\vwpktool\vwpktl.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwpktool.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWPKT="..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwrender.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwpktool.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwpktool.h" /iid "vwpktool_i.c" /I "..\vwcommon" /out                                                                                    "..\vwcommon" "vwpktool.idl"

"..\vwcommon\vwpktool.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwpktool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWPKT="..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwrender.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwpktool.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwpktool.h" /iid "vwpktool_i.c" /I "..\vwcommon" /out                                                                                    "..\vwcommon" "vwpktool.idl"

"..\vwcommon\vwpktool.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwpktool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Navigation Tool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\uitools\vwnvtool\vwnavt2d.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD CPP /Yu"vwobject.h"

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# ADD CPP /Yu"vwobject.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\uitools\vwnvtool\vwnavt3d.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\uitools\vwnvtool\vwnt2d3d.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD CPP /Yu"vwobject.h"

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# ADD CPP /Yu"vwobject.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwnvtool.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWNVT="..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwrender.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwnvtool.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwnvtool.h" /iid "vwnvtool_i.c" /I "..\vwcommon" /out                                                                                    "..\vwcommon" "vwnvtool.idl"

"..\vwcommon\vwnvtool.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwnvtool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWNVT="..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwrender.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwnvtool.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwnvtool.h" /iid "vwnvtool_i.c" /I "..\vwcommon" /out                                                                                    "..\vwcommon" "vwnvtool.idl"

"..\vwcommon\vwnvtool.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwnvtool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\vwcommon\vwuiobjs.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWUIO="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwuiobjs.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwuiobjs.h" /iid "vwuiobjs_i.c" /I "..\vwcommon" /out                                                                             "..\vwcommon" "vwuiobjs.idl"

"..\vwcommon\vwuiobjs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwuiobjs_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwuiobjs_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWUIO="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwuiobjs.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwuiobjs.h" /iid "vwuiobjs_i.c" /I "..\vwcommon" /out                                                                             "..\vwcommon" "vwuiobjs.idl"

"..\vwcommon\vwuiobjs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwuiobjs_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwuiobjs_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Properties"

# PROP Default_Filter ""
# Begin Group "Cell"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\cellprop.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__CELLP="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwtrans.idl"	
# Begin Custom Build
InputPath=..\vwcommon\cellprop.idl

BuildCmds= \
	midl /Oicf /no_warn /h "cellprop.h" /iid "cellprop_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "cellprop.idl"

"..\vwcommon\cellprop.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\cellprop_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__CELLP="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwtrans.idl"	
# Begin Custom Build
InputPath=..\vwcommon\cellprop.idl

BuildCmds= \
	midl /Oicf /no_warn /h "cellprop.h" /iid "cellprop_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "cellprop.idl"

"..\vwcommon\cellprop.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\cellprop_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\property\cellprop\edgecyc.cpp
# ADD CPP /I "..\property\vwidata" /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vecmath.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwtrans.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWTRA="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwtrans.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwtrans.h" /iid "vwtrans_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwtrans.idl"

"..\vwcommon\vwtrans.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwtrans_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWTRA="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwtrans.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwtrans.h" /iid "vwtrans_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwtrans.idl"

"..\vwcommon\vwtrans.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwtrans_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Vector"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\vector.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VECTO="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vector.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vector.h" /iid "vector_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vector.idl"

"..\vwcommon\vector.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vector_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VECTO="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vector.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vector.h" /iid "vector_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vector.idl"

"..\vwcommon\vector.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vector_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\property\vector\vectorob.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "Geometry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\vwgeom.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWGEO="..\vwcommon\vwobject.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwframe.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwgeom.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwgeom.h" /iid "vwgeom_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeom.idl"

"..\vwcommon\vwgeom.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwgeom_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWGEO="..\vwcommon\vwobject.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwframe.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwgeom.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwgeom.h" /iid "vwgeom_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeom.idl"

"..\vwcommon\vwgeom.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwgeom_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\property\vwgeom\vwgeomob.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "Sprited Geometry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\property\vwgeomsp\d3dmath.cpp
# End Source File
# Begin Source File

SOURCE=..\property\vwgeomsp\vwgeoms.cpp
# ADD CPP /I "..\d2d" /I "..\spritemn" /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwgeomsp.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWGEOM="..\vwcommon\vwgeom.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwgeomsp.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwgeomsp.h" /iid "vwgeomsp_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeomsp.idl"

"..\vwcommon\vwgeomsp.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwgeomsp_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWGEOM="..\vwcommon\vwgeom.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwgeomsp.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwgeomsp.h" /iid "vwgeomsp_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeomsp.idl"

"..\vwcommon\vwgeomsp.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwgeomsp_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Intersect Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\vwidata.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWIDA="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\cellprop.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwidata.idl

BuildCmds= \
	midl /Oicf /no_warn  /h "vwidata.h" /iid "vwidata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwidata.idl"

"..\vwcommon\vwidata.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwidata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWIDA="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\cellprop.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwidata.idl

BuildCmds= \
	midl /Oicf /no_warn  /h "vwidata.h" /iid "vwidata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwidata.idl"

"..\vwcommon\vwidata.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwidata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\property\vwidata\vwidatao.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "Articulated Geometry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\property\vwgeomar\charobj.cpp
# End Source File
# Begin Source File

SOURCE=..\property\vwgeomar\jointobj.cpp
# End Source File
# Begin Source File

SOURCE=..\property\vwgeomar\PuppetOb.cpp
# End Source File
# Begin Source File

SOURCE=..\property\vwgeomar\vwgeoma.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE="..\vwcommon\vwgeomar.idl"

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWGEOMA="..\vwcommon\vwgeom.idl"	
# Begin Custom Build
InputPath="..\vwcommon\vwgeomar.idl"

BuildCmds= \
	midl /Oicf /no_warn /h "vwgeomar.h" /iid "vwgeomar_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeomar.idl"

"..\vwcommon\vwgeomar.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwgeomar_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWGEOMA="..\vwcommon\vwgeom.idl"	
# Begin Custom Build
InputPath="..\vwcommon\vwgeomar.idl"

BuildCmds= \
	midl /Oicf /no_warn /h "vwgeomar.h" /iid "vwgeomar_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeomar.idl"

"..\vwcommon\vwgeomar.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwgeomar_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Frame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\property\vwframe\BBox.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwframe.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWFRA="..\vwcommon\vwtrans.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\cellprop.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwframe.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwframe.h" /iid "vwframe_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwframe.idl"

"..\vwcommon\vwframe.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwframe_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWFRA="..\vwcommon\vwtrans.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\cellprop.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwframe.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwframe.h" /iid "vwframe_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwframe.idl"

"..\vwcommon\vwframe.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwframe_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\property\vwframe\vwframeo.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "Accessory Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\vwadata.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwadata.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwadata.h" /iid "vwadata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwadata.idl"

"..\vwcommon\vwadata.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwadata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwadata.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwadata.h" /iid "vwadata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwadata.idl"

"..\vwcommon\vwadata.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwadata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\property\vwadata\vwadatao.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "Studio Pick Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\property\PickData\PickData.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\PickData.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\PickData.idl

BuildCmds= \
	midl /Oicf /no_warn /h "PickData.h" /iid "Pickdata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "Pickdata.idl"

"..\vwcommon\Pickdata.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\Pickdata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\PickData.idl

BuildCmds= \
	midl /Oicf /no_warn /h "PickData.h" /iid "Pickdata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "Pickdata.idl"

"..\vwcommon\Pickdata.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\Pickdata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\isound.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing Custom Build Step
InputPath=..\vwcommon\isound.idl

BuildCmds= \
	midl /Oicf /no_warn /h "isound.h" /iid "isound_i.c" /I "..\vwcommon" /out         "..\vwcommon" "isound.idl"

"..\vwcommon\isound.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\isound_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing Custom Build Step
InputPath=..\vwcommon\isound.idl

BuildCmds= \
	midl /Oicf /no_warn /h "isound.h" /iid "isound_i.c" /I "..\vwcommon" /out         "..\vwcommon" "isound.idl"

"..\vwcommon\isound.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\isound_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\property\Sound\Sound.cpp
# End Source File
# End Group
# End Group
# Begin Group "Graphics"

# PROP Default_Filter ""
# Begin Group "Geometry Cache"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwgcache\readwdb.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwgcache.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWGCA="..\vwcommon\vwframe.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwgcache.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwgcache.h" /iid "vwgcache_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgcache.idl"

"..\vwcommon\vwgcache.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwgcache_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWGCA="..\vwcommon\vwframe.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwgcache.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwgcache.h" /iid "vwgcache_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgcache.idl"

"..\vwcommon\vwgcache.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwgcache_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwgcache\vwgcacho.cpp
# ADD CPP /I "..\d2d" /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "Renderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwrender\vwrend2d.cpp
# ADD CPP /I "..\d2d\d2dutils\include" /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwrender\vwrend3d.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwrender.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWREN="..\vwcommon\vwframe.idl"	"..\vwcommon\d2d.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwrender.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwrender.h" /iid "vwrender_i.c" /I "..\vwcommon" /out                                                                                     "..\vwcommon" "vwrender.idl"

"..\vwcommon\vwrender.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwrender_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWREN="..\vwcommon\vwframe.idl"	"..\vwcommon\d2d.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwrender.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwrender.h" /iid "vwrender_i.c" /I "..\vwcommon" /out                                                                                     "..\vwcommon" "vwrender.idl"

"..\vwcommon\vwrender.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwrender_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwrender\vwrendrt.cpp
# ADD CPP /I "..\d2d\d2dutils\include" /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "Sprite Manager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\spritemn.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\spritemn.idl

BuildCmds= \
	midl /Oicf /no_warn /h "spritemn.h" /iid "spritemn_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "spritemn.idl"

"..\vwcommon\spritemn.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\spritemn_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\spritemn.idl

BuildCmds= \
	midl /Oicf /no_warn /h "spritemn.h" /iid "spritemn_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "spritemn.idl"

"..\vwcommon\spritemn.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\spritemn_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\spritemn\spritemo.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "Geometry Converters"

# PROP Default_Filter ""
# Begin Group "VRML"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\vrml2d3d.idl
# PROP Exclude_From_Build 1
# PROP Ignore_Default_Tool 1
# End Source File
# End Group
# Begin Group "3DStudio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\3dstod3d.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\3dstod3d.idl

BuildCmds= \
	midl /Oicf /no_warn /h "3dstod3d.h" /iid "3dstod3d_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "3dstod3d.idl"

"..\vwcommon\3dstod3d.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\3dstod3d_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\3dstod3d.idl

BuildCmds= \
	midl /Oicf /no_warn /h "3dstod3d.h" /iid "3dstod3d_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "3dstod3d.idl"

"..\vwcommon\3dstod3d.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\3dstod3d_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\3dstod3d\conv3ds.cpp
# ADD CPP /I "..\vwsystem" /I "..\tools\3dstod3d\include"
# End Source File
# End Group
# Begin Source File

SOURCE=..\vwcommon\convgeom.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\convgeom.idl

BuildCmds= \
	midl /Oicf /no_warn /h "convgeom.h" /iid "convgeom_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "convgeom.idl"

"..\vwcommon\convgeom.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\convgeom_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\convgeom.idl

BuildCmds= \
	midl /Oicf /no_warn /h "convgeom.h" /iid "convgeom_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "convgeom.idl"

"..\vwcommon\convgeom.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\convgeom_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "Modules"

# PROP Default_Filter ""
# Begin Group "Multimedia"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\module\mm\mmartfct.cpp
# End Source File
# Begin Source File

SOURCE=..\module\mm\mmavatar.cpp
# End Source File
# Begin Source File

SOURCE=..\module\mm\mmhost.cpp
# End Source File
# Begin Source File

SOURCE=..\module\mm\mmlight.cpp
# End Source File
# Begin Source File

SOURCE=..\module\mm\mmportal.cpp
# End Source File
# Begin Source File

SOURCE=..\module\mm\mmroom.cpp
# End Source File
# Begin Source File

SOURCE=..\module\mm\mmthing.cpp
# End Source File
# Begin Source File

SOURCE=..\Vwcommon\Profhelp.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwmmex.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwmmex.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwmmex.h" /iid "vwmmex_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "vwmmex.idl"

"..\vwcommon\vwmmex.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwmmex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwmmex.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwmmex.h" /iid "vwmmex_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "vwmmex.idl"

"..\vwcommon\vwmmex.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwmmex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\module\mm\vwmmexo.cpp
# ADD CPP /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vector" /I "..\property\vwadata"
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\Conv3DS.rgs
# End Source File
# Begin Source File

SOURCE=.\vwmmex.rgs
# End Source File
# End Target
# End Project
