# Microsoft Developer Studio Project File - Name="VWRendVw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VWRendVw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vwrend60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vwrend60.mak" CFG="VWRendVw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VWRendVw - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VWRendVw - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /X /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 d3drm.lib ddraw.lib dxguid.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\Release/vwrendvw.ocx" /libpath:"\mssdk\lib"
# Begin Custom Build - Registering OLE control...
OutDir=.\Release
TargetPath=.\Release\vwrendvw.ocx
InputPath=.\Release\vwrendvw.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Yu"stdafx.h" /Fd".\Debug/vwrendvw.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 d3drm.lib ddraw.lib dxguid.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\Debug/vwrendvw.ocx" /libpath:"\mssdk\lib"
# Begin Custom Build - Registering OLE control...
OutDir=.\Debug
TargetPath=.\Debug\vwrendvw.ocx
InputPath=.\Debug\vwrendvw.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "VWRendVw - Win32 Release"
# Name "VWRendVw - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Boundary.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\cmddel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\CmdHandl.cpp
# End Source File
# Begin Source File

SOURCE=.\edit.cpp
# End Source File
# Begin Source File

SOURCE=.\geomsel.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDg.cpp
# End Source File
# Begin Source File

SOURCE=.\rlutil.cpp
# End Source File
# Begin Source File

SOURCE=.\SelctDlg.cpp
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

SOURCE=.\ToolDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\vwview\viewctl.cpp
# ADD CPP /I "..\..\vwsystem"
# End Source File
# Begin Source File

SOURCE=..\vwview\viewppg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\vwmenu.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\VWRendVw.cpp
# End Source File
# Begin Source File

SOURCE=.\VWRendVw.def
# End Source File
# Begin Source File

SOURCE=.\VWRendVw.odl

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

# ADD MTL /D "VWSYSTEM_BUILD"

!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

# ADD MTL /I "..\..\vwcommon" /D "VWSYSTEM_BUILD"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VWRendVw.rc
# End Source File
# Begin Source File

SOURCE=.\vwrnvwct.bmp
# End Source File
# Begin Source File

SOURCE=.\VWRnVwCt.cpp
# End Source File
# Begin Source File

SOURCE=.\VWRnVwPp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\vwsgfxut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\vwuiview.cpp
# ADD CPP /I "..\..\vwsystem"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Boundary.h
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\cathelp.h
# End Source File
# Begin Source File

SOURCE=.\geomsel.h
# End Source File
# Begin Source File

SOURCE=.\OptionDg.h
# End Source File
# Begin Source File

SOURCE=.\rlutil.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolSite.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=..\vwview\viewctl.h
# End Source File
# Begin Source File

SOURCE=..\vwview\ViewPpg.h
# End Source File
# Begin Source File

SOURCE=..\..\vwcommon\vwmenu.h
# End Source File
# Begin Source File

SOURCE=.\vwrendvw.h
# End Source File
# Begin Source File

SOURCE=.\vwrnvwct.h
# End Source File
# Begin Source File

SOURCE=.\vwrnvwpp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\addvert.ico
# End Source File
# Begin Source File

SOURCE=.\autobnd.ico
# End Source File
# Begin Source File

SOURCE=.\autoecgr.ico
# End Source File
# Begin Source File

SOURCE=.\avcntr.ico
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bpass.ico
# End Source File
# Begin Source File

SOURCE=.\bxpass.ico
# End Source File
# Begin Source File

SOURCE=.\collisio.bmp
# End Source File
# Begin Source File

SOURCE=.\CollTogd.bmp
# End Source File
# Begin Source File

SOURCE=.\CollTogl.bmp
# End Source File
# Begin Source File

SOURCE=.\CollTogu.bmp
# End Source File
# Begin Source File

SOURCE=.\delvert.ico
# End Source File
# Begin Source File

SOURCE=.\delvertg.ico
# End Source File
# Begin Source File

SOURCE=.\drawbnd.ico
# End Source File
# Begin Source File

SOURCE=.\drawvert.ico
# End Source File
# Begin Source File

SOURCE=.\ECVwCtl.ico
# End Source File
# Begin Source File

SOURCE=.\geombar.bmp
# End Source File
# Begin Source File

SOURCE=.\gravityt.bmp
# End Source File
# Begin Source File

SOURCE=.\gravoff.ico
# End Source File
# Begin Source File

SOURCE=.\gravon.ico
# End Source File
# Begin Source File

SOURCE=.\GravTogd.bmp
# End Source File
# Begin Source File

SOURCE=.\GravTogl.bmp
# End Source File
# Begin Source File

SOURCE=.\GravTogu.bmp
# End Source File
# Begin Source File

SOURCE=.\help.ico
# End Source File
# Begin Source File

SOURCE=.\helpd.bmp
# End Source File
# Begin Source File

SOURCE=.\helpu.bmp
# End Source File
# Begin Source File

SOURCE=.\insvertg.ico
# End Source File
# Begin Source File

SOURCE=.\Link.cur
# End Source File
# Begin Source File

SOURCE=.\move.ico
# End Source File
# Begin Source File

SOURCE=.\nav.ico
# End Source File
# Begin Source File

SOURCE=.\navigatd.bmp
# End Source File
# Begin Source File

SOURCE=.\navigate.bmp
# End Source File
# Begin Source File

SOURCE=.\navigate.cur
# End Source File
# Begin Source File

SOURCE=.\navigatu.bmp
# End Source File
# Begin Source File

SOURCE=.\redo.bmp
# End Source File
# Begin Source File

SOURCE=.\redo.ico
# End Source File
# Begin Source File

SOURCE=.\redod.bmp
# End Source File
# Begin Source File

SOURCE=.\redogr.ico
# End Source File
# Begin Source File

SOURCE=.\redogry.bmp
# End Source File
# Begin Source File

SOURCE=.\redou.bmp
# End Source File
# Begin Source File

SOURCE=.\redox.bmp
# End Source File
# Begin Source File

SOURCE=.\rotate.bmp
# End Source File
# Begin Source File

SOURCE=.\rotate.ico
# End Source File
# Begin Source File

SOURCE=.\rotated.bmp
# End Source File
# Begin Source File

SOURCE=.\rotateu.bmp
# End Source File
# Begin Source File

SOURCE=.\scale.bmp
# End Source File
# Begin Source File

SOURCE=.\scale.ico
# End Source File
# Begin Source File

SOURCE=.\scaled.bmp
# End Source File
# Begin Source File

SOURCE=.\scaleu.bmp
# End Source File
# Begin Source File

SOURCE=.\seldlg.bmp
# End Source File
# Begin Source File

SOURCE=.\select.bmp
# End Source File
# Begin Source File

SOURCE=.\select.ico
# End Source File
# Begin Source File

SOURCE=.\select1.bmp
# End Source File
# Begin Source File

SOURCE=.\selectd.bmp
# End Source File
# Begin Source File

SOURCE=.\selectdi.bmp
# End Source File
# Begin Source File

SOURCE=.\selectu.bmp
# End Source File
# Begin Source File

SOURCE=.\slctdlgd.bmp
# End Source File
# Begin Source File

SOURCE=.\slctdlgu.bmp
# End Source File
# Begin Source File

SOURCE=.\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\trans.bmp
# End Source File
# Begin Source File

SOURCE=.\transd.bmp
# End Source File
# Begin Source File

SOURCE=.\transu.bmp
# End Source File
# Begin Source File

SOURCE=.\undo.bmp
# End Source File
# Begin Source File

SOURCE=.\undo.ico
# End Source File
# Begin Source File

SOURCE=.\undod.bmp
# End Source File
# Begin Source File

SOURCE=.\undogr.ico
# End Source File
# Begin Source File

SOURCE=.\undogry.bmp
# End Source File
# Begin Source File

SOURCE=.\undou.bmp
# End Source File
# Begin Source File

SOURCE=.\undox.bmp
# End Source File
# Begin Source File

SOURCE=.\vwrendvw.rc2
# End Source File
# Begin Source File

SOURCE=.\zoomin.bmp
# End Source File
# Begin Source File

SOURCE=.\zoomin.ico
# End Source File
# Begin Source File

SOURCE=.\zoomind.bmp
# End Source File
# Begin Source File

SOURCE=.\zooming.bmp
# End Source File
# Begin Source File

SOURCE=.\zoominu.bmp
# End Source File
# Begin Source File

SOURCE=.\zoominx.bmp
# End Source File
# Begin Source File

SOURCE=.\zoomout.bmp
# End Source File
# Begin Source File

SOURCE=.\zoomout.ico
# End Source File
# Begin Source File

SOURCE=.\zoomoutd.bmp
# End Source File
# Begin Source File

SOURCE=.\zoomoutg.bmp
# End Source File
# Begin Source File

SOURCE=.\zoomoutu.bmp
# End Source File
# Begin Source File

SOURCE=.\zoomoutx.bmp
# End Source File
# End Group
# End Target
# End Project
