# Microsoft Developer Studio Project File - Name="vwsystem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vwsystem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vwsyst60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vwsyst60.mak" CFG="vwsystem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vwsystem - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vwsystem - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Yu"stdafx.h" /Fd"Debug/vwsystem.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 /Os
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 wsock32.lib wininet.lib urlmon.lib uuid.lib netapi32.lib winmm.lib rpcndr.lib rpcns4.lib rpcrt4.lib mswsock.lib ws2_32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/vwsystem.dll" /debug:full /dbgimplib
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\Debug\vwsystem.dll
InputPath=.\Debug\vwsystem.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Yu"stdafx.h" /Fd"ReleaseMinSize/vwsystem.pdb" /FD /c
# SUBTRACT CPP /X /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 /Os
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 activeds.lib wsock32.lib wininet.lib urlmon.lib uuid.lib netapi32.lib winmm.lib rpcndr.lib rpcns4.lib rpcrt4.lib mswsock.lib ws2_32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"ReleaseMinSize/vwsystem.dll"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\vwsystem.dll
InputPath=.\ReleaseMinSize\vwsystem.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "vwsystem - Win32 Debug"
# Name "vwsystem - Win32 Release MinSize"
# Begin Group "VWSystem"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Vwcommon\Cathelp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\dlldatax.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\vwcommon\inetfile_p.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwclient_p.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwobject_p.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwprop_p.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\vwsystem.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# ADD CPP /I "..\vwobject" /I "..\vwadmin" /I "..\vwclient" /I "..\vwserver" /I "..\vwcomm" /I "..\fastcomm" /I "..\vwodb" /I "..\tools\vwanim" /I "..\tools\vwhtml" /I "..\tools\inetfile" /I "..\tools\vwstats" /I "..\tools\dirserv" /I "\dxsdk\sdk\inc" /I "..\module\system" /I "..\property\menuitem" /I "..\property\ocxproxy" /I "..\property\avprof" /I "..\tools\authntic"
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# ADD CPP /I "..\vwobject" /I "..\vwadmin" /I "..\vwclient" /I "..\vwserver" /I "..\vwcomm" /I "..\fastcomm" /I "..\vwodb" /I "..\tools\vwanim" /I "..\tools\vwhtml" /I "..\tools\inetfile" /I "..\tools\vwstats" /I "..\tools\dirserv" /I "\dxsdk\sdk\inc" /I "..\module\system" /I "..\property\menuitem" /I "..\property\ocxproxy" /I "..\property\avprof" /I "..\tools\authntic"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vwsystem.def
# End Source File
# Begin Source File

SOURCE=.\vwsystem.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWSYS="..\vwcommon\vwobject.idl"	"..\vwcommon\vwclient.idl"	"..\vwcommon\vwserver.idl"	"..\vwcommon\vwodb.idl"	"..\vwcommon\vwcomm.idl"	"..\vwcommon\menuitem.idl"	"..\vwcommon\vwanim.idl"	"..\vwcommon\vwstats.idl"	"..\vwcommon\vwhtml.idl"	"..\vwcommon\inetfile.idl"	"..\vwcommon\dirserv.idl"	"..\vwcommon\authntic.idl"	"..\vwcommon\vwsysex.idl"	"..\vwcommon\vwprop.idl"	"..\vwcommon\avprof.idl"	"..\vwcommon\ocxproxy.idl"	"..\vwcommon\vwadmin.idl"	
# Begin Custom Build
InputPath=.\vwsystem.idl

BuildCmds= \
	midl /Oicf  /no_warn /h "vwsystem.h" /iid "vwsystem_i.c" /I "..\vwcommon" $(InputPath)

".\vwsystem.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwsystem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwsystem.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWSYS="..\vwcommon\vwobject.idl"	"..\vwcommon\vwclient.idl"	"..\vwcommon\vwserver.idl"	"..\vwcommon\vwodb.idl"	"..\vwcommon\vwcomm.idl"	"..\vwcommon\menuitem.idl"	"..\vwcommon\vwanim.idl"	"..\vwcommon\vwstats.idl"	"..\vwcommon\vwhtml.idl"	"..\vwcommon\inetfile.idl"	"..\vwcommon\dirserv.idl"	"..\vwcommon\authntic.idl"	"..\vwcommon\vwsysex.idl"	"..\vwcommon\vwprop.idl"	"..\vwcommon\avprof.idl"	"..\vwcommon\ocxproxy.idl"	"..\vwcommon\vwadmin.idl"	
# Begin Custom Build
InputPath=.\vwsystem.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwsystem.h" /iid "vwsystem_i.c" /I "..\vwcommon" "vwsystem.idl"

".\vwsystem.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwsystem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\vwsystem.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vwsystem.rc
# End Source File
# End Group
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Group "VWObject"

# PROP Default_Filter "*.cpp;*.idl"
# Begin Source File

SOURCE=..\vwobject\collenum.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\comobj.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\connlist.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\marshbuf.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\methdobj.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\plisthlp.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\plistobj.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\pmaphlp.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\pmapobj.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\propsecu.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\reghelp.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\registry.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\scripts.cpp
# End Source File
# Begin Source File

SOURCE=..\vwobject\scrptobj.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\syshelp.cpp
# ADD CPP /I "..\vwobject" /I "..\property\menuitem" /I "..\property\ocxproxy"
# End Source File
# Begin Source File

SOURCE=..\vwobject\thinghlp.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\thingobj.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\urlhelp.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwbuffer.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwbuffer.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwbuffer.h" /iid "vwbuffer_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwbuffer.idl"

"..\vwcommon\vwbuffer.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwbuffer_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwbuffer.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwbuffer.h" /iid "vwbuffer_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwbuffer.idl"

"..\vwcommon\vwbuffer.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwbuffer_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwobject.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWOBJ="..\vwcommon\vwcomm.idl"	"..\vwcommon\vwprop.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwobject.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwobject.h" /iid "vwobject_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwobject.idl"

"..\vwcommon\vwobject.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwobject_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwobject_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWOBJ="..\vwcommon\vwcomm.idl"	"..\vwcommon\vwprop.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwobject.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwobject.h" /iid "vwobject_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwobject.idl"

"..\vwcommon\vwobject.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwobject_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwobject_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\vwcommon\vwprop.idl"

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWPRO="..\vwcommon\vwbuffer.idl"	
# Begin Custom Build
InputPath="..\vwcommon\vwprop.idl"

BuildCmds= \
	midl /Oicf /no_warn /h "vwprop.h" /iid "vwprop_i.c" /I "..\vwcommon" /out "..\vwcommon"                                                         "vwprop.idl"

"..\vwcommon\vwprop.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwprop_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwprop_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWPRO="..\vwcommon\vwbuffer.idl"	
# Begin Custom Build
InputPath="..\vwcommon\vwprop.idl"

BuildCmds= \
	midl /Oicf /no_warn /h "vwprop.h" /iid "vwprop_i.c" /I "..\vwcommon" /out "..\vwcommon"                                                         "vwprop.idl"

"..\vwcommon\vwprop.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwprop_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwprop_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwobject\worldevt.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\worldfmt.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\worldhlp.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# ADD CPP /I "..\manager" /Yu"vwobject.h"

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# ADD CPP /Yu"vwobject.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwobject\worldmar.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwobject\worldobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# ADD CPP /I "..\manager" /Yu"vwobject.h"

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# ADD CPP /Yu"vwobject.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "VWClient"

# PROP Default_Filter "*.cpp;*.idl"
# Begin Source File

SOURCE=..\vwcommon\vwclient.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWCLI="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwclient.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwclient.h" /iid "vwclient_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwclient.idl"

"..\vwcommon\vwclient.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwclient_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwclient_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWCLI="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwclient.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwclient.h" /iid "vwclient_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwclient.idl"

"..\vwcommon\vwclient.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwclient_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwclient_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwclient\vwcliobj.cpp
# ADD CPP /I "..\vwobject" /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\Vwclient\vwcliocx.cpp
# ADD CPP /I "..\vwclient"
# End Source File
# End Group
# Begin Group "VWServer"

# PROP Default_Filter "*.cpp;*.idl"
# Begin Source File

SOURCE=..\vwserver\connserv.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwserver\queue.cpp
# End Source File
# Begin Source File

SOURCE=..\vwserver\vwserve.cpp
# ADD CPP /I "..\vwobject" /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwserver.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWSER="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwserver.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwserver.h" /iid "vwserver_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwserver.idl"

"..\vwcommon\vwserver.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwserver_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWSER="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwserver.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwserver.h" /iid "vwserver_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwserver.idl"

"..\vwcommon\vwserver.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwserver_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwserver\wldinfo.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwserver\wldlist.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "VWComm"

# PROP Default_Filter "*.cpp;*.idl"
# Begin Source File

SOURCE=..\vwcomm\commcono.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcomm\commfunc.cpp
# ADD CPP /I "..\vwobject"
# End Source File
# Begin Source File

SOURCE=..\vwcomm\commmsgo.cpp
# ADD CPP /I "..\vwobject"
# End Source File
# Begin Source File

SOURCE=..\vwcomm\commobj.cpp
# ADD CPP /I "..\vwobject" /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\vwcomm\tinfo.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwcomm.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWCOM="..\vwcommon\vwbuffer.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwcomm.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwcomm.h" /iid "vwcomm_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwcomm.idl"

"..\vwcommon\vwcomm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwcomm_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWCOM="..\vwcommon\vwbuffer.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwcomm.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwcomm.h" /iid "vwcomm_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwcomm.idl"

"..\vwcommon\vwcomm.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwcomm_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "VWODB"

# PROP Default_Filter "*.cpp;*.idl"
# Begin Source File

SOURCE=..\vwcommon\vwodb.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwodb.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwodb.h" /iid "vwodb_i.c" /I "..\vwcommon" /out  "..\vwcommon"                                                                                                   "vwodb.idl"

"..\vwcommon\vwodb.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwodb_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwodb.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwodb.h" /iid "vwodb_i.c" /I "..\vwcommon" /out  "..\vwcommon"                                                                                                   "vwodb.idl"

"..\vwcommon\vwodb.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwodb_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwodb\vwodbobj.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# End Group
# Begin Group "VWAdmin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Vwcommon\vwadmin.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWADM="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\Vwcommon\vwadmin.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwadmin.h" /iid "vwadmin_i.c" /I "..\vwcommon" /out     "..\vwcommon" "vwadmin.idl"

"..\vwcommon\vwadmin_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwadmin.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWADM="..\vwcommon\vwobject.idl"	
# Begin Custom Build
InputPath=..\Vwcommon\vwadmin.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwadmin.h" /iid "vwadmin_i.c" /I "..\vwcommon" /out     "..\vwcommon" "vwadmin.idl"

"..\vwcommon\vwadmin_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwadmin.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwadmin\vwadmobj.cpp
# End Source File
# End Group
# Begin Group "FastComm"

# PROP Default_Filter ""
# Begin Group "Toolkit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\fastcomm\Toolkit\tools.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\fastcomm\commlayr.cpp
# End Source File
# Begin Source File

SOURCE=..\fastcomm\fastcono.cpp
# ADD CPP /I "..\vwobject"
# End Source File
# Begin Source File

SOURCE=..\fastcomm\fastmsgo.cpp
# ADD CPP /I "..\vwobject"
# End Source File
# Begin Source File

SOURCE=..\fastcomm\fastobj.cpp
# ADD CPP /I "..\vwobject"
# End Source File
# End Group
# End Group
# Begin Group "Tools"

# PROP Default_Filter ""
# Begin Group "Animator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\vwanim.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwanim.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwanim.h" /iid "vwanim_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwanim.idl"

"..\vwcommon\vwanim.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwanim_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwanim.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwanim.h" /iid "vwanim_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwanim.idl"

"..\vwcommon\vwanim.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwanim_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\vwanim\vwanimob.cpp
# End Source File
# Begin Source File

SOURCE=..\tools\vwanim\vwanmtob.cpp
# End Source File
# End Group
# Begin Group "Internet File Manager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tools\inetfile\dlthread.cpp
# ADD CPP /I "..\vwserver"
# End Source File
# Begin Source File

SOURCE=..\vwcommon\inetfile.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\inetfile.idl

BuildCmds= \
	midl /Oicf /no_warn /h "inetfile.h" /iid "inetfile_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "inetfile.idl"

"..\vwcommon\inetfile.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\inetfile_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\inetfile.idl

BuildCmds= \
	midl /Oicf /no_warn /h "inetfile.h" /iid "inetfile_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "inetfile.idl"

"..\vwcommon\inetfile.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\inetfile_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\inetfile\inetmgro.cpp
# End Source File
# End Group
# Begin Group "HTML Popup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tools\vwhtml\htmlwnd.cpp
# End Source File
# Begin Source File

SOURCE=..\tools\vwhtml\vwhelper.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwhtml.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VWHTM="..\vwcommon\vwclient.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwhtml.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwhtml.h" /iid "vwhtml_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwhtml.idl"

"..\vwcommon\vwhtml.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwhtml_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
USERDEP__VWHTM="..\vwcommon\vwclient.idl"	
# Begin Custom Build
InputPath=..\vwcommon\vwhtml.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwhtml.h" /iid "vwhtml_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwhtml.idl"

"..\vwcommon\vwhtml.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwhtml_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\vwhtml\vwhtmlob.cpp
# End Source File
# Begin Source File

SOURCE=..\tools\vwhtml\wbbrowsr.cpp
# End Source File
# End Group
# Begin Group "Statistics Manager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tools\vwstats\statmgr.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwstats.idl
# PROP Exclude_From_Build 1
# PROP Ignore_Default_Tool 1
# End Source File
# End Group
# Begin Group "Directory Services"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tools\dirserv\attrib.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\dirserv.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\dirserv.idl

BuildCmds= \
	midl /Oicf /no_warn  /h "dirserv.h" /iid "dirserv_i.c" /I "..\vwcommon" /out                                                                        "..\vwcommon" "dirserv.idl"

"..\vwcommon\dirserv.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\dirserv_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\dirserv.idl

BuildCmds= \
	midl /Oicf /no_warn  /h "dirserv.h" /iid "dirserv_i.c" /I "..\vwcommon" /out                                                                        "..\vwcommon" "dirserv.idl"

"..\vwcommon\dirserv.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\dirserv_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\dirserv\dqueue.cpp
# End Source File
# Begin Source File

SOURCE=..\tools\dirserv\dserv.cpp
# End Source File
# End Group
# Begin Group "Authentication"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\tools\authntic\aobj.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\authntic.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\authntic.idl

BuildCmds= \
	midl /Oicf /no_warn  /h "authntic.h" /iid "authntic_i.c" /I "..\vwcommon" /out                          "..\vwcommon"               "authntic.idl"

"..\vwcommon\authntic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\authntic_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\authntic.idl

BuildCmds= \
	midl /Oicf /no_warn  /h "authntic.h" /iid "authntic_i.c" /I "..\vwcommon" /out                          "..\vwcommon"               "authntic.idl"

"..\vwcommon\authntic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\authntic_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\tools\authntic\authobj.cpp
# End Source File
# Begin Source File

SOURCE=..\tools\authntic\logon.cpp
# End Source File
# End Group
# End Group
# Begin Group "Properties"

# PROP Default_Filter ""
# Begin Group "Avatar Profile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\avprof.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\avprof.idl

BuildCmds= \
	midl /Oicf /no_warn /h "avprof.h" /iid "avprof_i.c" /I "..\vwcommon" /out "..\vwcommon"                                        "avprof.idl"

"..\vwcommon\avprof.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\avprof_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\avprof.idl

BuildCmds= \
	midl /Oicf /no_warn /h "avprof.h" /iid "avprof_i.c" /I "..\vwcommon" /out "..\vwcommon"                                        "avprof.idl"

"..\vwcommon\avprof.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\avprof_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\vwcommon\profhelp.cpp
# ADD CPP /Yu"vwobject.h"
# End Source File
# Begin Source File

SOURCE=..\property\avprof\profobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# ADD CPP /I "..\vwsystem" /Yu"vwobject.h"

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# ADD CPP /Yu"vwobject.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "MenuItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\property\menuitem\cmenuitm.cpp
# End Source File
# Begin Source File

SOURCE="..\vwcommon\menuitem.idl"

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath="..\vwcommon\menuitem.idl"

BuildCmds= \
	midl /Oicf /no_warn  /h "menuitem.h" /iid "menuitem_i.c" /I "..\vwcommon" /out                             "..\vwcommon"            "menuitem.idl"

"..\vwcommon\menuitem.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\menuitem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath="..\vwcommon\menuitem.idl"

BuildCmds= \
	midl /Oicf /no_warn  /h "menuitem.h" /iid "menuitem_i.c" /I "..\vwcommon" /out                             "..\vwcommon"            "menuitem.idl"

"..\vwcommon\menuitem.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\menuitem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "OCXProxy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\vwcommon\ocxproxy.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\ocxproxy.idl

BuildCmds= \
	midl /Oicf /no_warn /h "ocxproxy.h" /iid "ocxproxy_i.c" /I "..\vwcommon" /out                             "..\vwcommon"            "ocxproxy.idl"

"..\vwcommon\ocxproxy.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\ocxproxy_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\ocxproxy.idl

BuildCmds= \
	midl /Oicf /no_warn /h "ocxproxy.h" /iid "ocxproxy_i.c" /I "..\vwcommon" /out                             "..\vwcommon"            "ocxproxy.idl"

"..\vwcommon\ocxproxy.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\ocxproxy_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\property\ocxproxy\vwocx.cpp
# End Source File
# End Group
# End Group
# Begin Group "Modules"

# PROP Default_Filter ""
# Begin Group "System"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\module\system\artifact.cpp
# End Source File
# Begin Source File

SOURCE=..\module\system\avatar.cpp
# End Source File
# Begin Source File

SOURCE=..\module\system\host.cpp
# End Source File
# Begin Source File

SOURCE=..\module\system\IWportal.cpp
# End Source File
# Begin Source File

SOURCE=..\module\system\portal.cpp
# End Source File
# Begin Source File

SOURCE=..\module\system\room.cpp
# End Source File
# Begin Source File

SOURCE=..\module\system\statemch.cpp
# End Source File
# Begin Source File

SOURCE=..\vwcommon\vwsysex.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwsysex.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwsysex.h" /iid "vwsysex_i.c" /I "..\vwcommon" /out                          "..\vwcommon"               "vwsysex.idl"

"..\vwcommon\vwsysex.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwsysex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\vwsysex.idl

BuildCmds= \
	midl /Oicf /no_warn /h "vwsysex.h" /iid "vwsysex_i.c" /I "..\vwcommon" /out                          "..\vwcommon"               "vwsysex.idl"

"..\vwcommon\vwsysex.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\vwsysex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\module\system\vwsysexo.cpp
# ADD CPP /I "..\vwobject" /I "..\property\menuitem" /I "..\property\ocxproxy"
# End Source File
# Begin Source File

SOURCE=..\module\system\vwthing.cpp
# End Source File
# Begin Source File

SOURCE=..\module\system\WebPage.cpp
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\artifact.rgs
# End Source File
# Begin Source File

SOURCE=.\authcate.rgs
# End Source File
# Begin Source File

SOURCE=.\authshun.rgs
# End Source File
# Begin Source File

SOURCE=.\avatar.rgs
# End Source File
# Begin Source File

SOURCE=.\avprof.rgs
# End Source File
# Begin Source File

SOURCE=.\dlthread.rgs
# End Source File
# Begin Source File

SOURCE=.\DServ.rgs
# End Source File
# Begin Source File

SOURCE=.\host.rgs
# End Source File
# Begin Source File

SOURCE=.\IWportal.rgs
# End Source File
# Begin Source File

SOURCE=.\portal.rgs
# End Source File
# Begin Source File

SOURCE=.\room.rgs
# End Source File
# Begin Source File

SOURCE=.\statemch.rgs
# End Source File
# Begin Source File

SOURCE=.\vwcliocx.rgs
# End Source File
# Begin Source File

SOURCE=.\vwsysex.rgs
# End Source File
# Begin Source File

SOURCE=.\vwthing.rgs
# End Source File
# Begin Source File

SOURCE=.\WebPage.rgs
# End Source File
# End Target
# End Project
