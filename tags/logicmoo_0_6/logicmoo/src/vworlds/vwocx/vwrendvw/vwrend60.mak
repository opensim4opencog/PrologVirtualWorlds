# Microsoft Developer Studio Generated NMAKE File, Based on vwrend60.dsp
!IF "$(CFG)" == ""
CFG=VWRendVw - Win32 Debug
!MESSAGE No configuration specified. Defaulting to VWRendVw - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "VWRendVw - Win32 Release" && "$(CFG)" != "VWRendVw - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\vwrendvw.ocx" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\Boundary.obj"
	-@erase "$(INTDIR)\cmddel.obj"
	-@erase "$(INTDIR)\CmdHandl.obj"
	-@erase "$(INTDIR)\edit.obj"
	-@erase "$(INTDIR)\geomsel.obj"
	-@erase "$(INTDIR)\OptionDg.obj"
	-@erase "$(INTDIR)\rlutil.obj"
	-@erase "$(INTDIR)\SelctDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\ToolDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\viewctl.obj"
	-@erase "$(INTDIR)\viewppg.obj"
	-@erase "$(INTDIR)\vwmenu.obj"
	-@erase "$(INTDIR)\vwrend60.pch"
	-@erase "$(INTDIR)\VWRendVw.obj"
	-@erase "$(INTDIR)\VWRendVw.res"
	-@erase "$(INTDIR)\VWRendVw.tlb"
	-@erase "$(INTDIR)\VWRnVwCt.obj"
	-@erase "$(INTDIR)\VWRnVwPp.obj"
	-@erase "$(INTDIR)\vwsgfxut.obj"
	-@erase "$(INTDIR)\vwuiview.obj"
	-@erase "$(OUTDIR)\vwrendvw.exp"
	-@erase "$(OUTDIR)\vwrendvw.lib"
	-@erase "$(OUTDIR)\vwrendvw.ocx"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\vwrend60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VWRendVw.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwrend60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=d3drm.lib ddraw.lib dxguid.lib uuid.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\vwrendvw.pdb" /machine:I386 /def:".\VWRendVw.def" /out:"$(OUTDIR)\vwrendvw.ocx" /implib:"$(OUTDIR)\vwrendvw.lib" /libpath:"\mssdk\lib" 
DEF_FILE= \
	".\VWRendVw.def"
LINK32_OBJS= \
	"$(INTDIR)\Boundary.obj" \
	"$(INTDIR)\cmddel.obj" \
	"$(INTDIR)\CmdHandl.obj" \
	"$(INTDIR)\edit.obj" \
	"$(INTDIR)\geomsel.obj" \
	"$(INTDIR)\OptionDg.obj" \
	"$(INTDIR)\rlutil.obj" \
	"$(INTDIR)\SelctDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\ToolDlg.obj" \
	"$(INTDIR)\viewctl.obj" \
	"$(INTDIR)\viewppg.obj" \
	"$(INTDIR)\vwmenu.obj" \
	"$(INTDIR)\VWRendVw.obj" \
	"$(INTDIR)\VWRnVwCt.obj" \
	"$(INTDIR)\VWRnVwPp.obj" \
	"$(INTDIR)\vwsgfxut.obj" \
	"$(INTDIR)\vwuiview.obj" \
	"$(INTDIR)\VWRendVw.res"

"$(OUTDIR)\vwrendvw.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\vwrendvw.ocx
InputPath=.\Release\vwrendvw.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\vwrendvw.ocx" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\Boundary.obj"
	-@erase "$(INTDIR)\cmddel.obj"
	-@erase "$(INTDIR)\CmdHandl.obj"
	-@erase "$(INTDIR)\edit.obj"
	-@erase "$(INTDIR)\geomsel.obj"
	-@erase "$(INTDIR)\OptionDg.obj"
	-@erase "$(INTDIR)\rlutil.obj"
	-@erase "$(INTDIR)\SelctDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\ToolDlg.obj"
	-@erase "$(INTDIR)\viewctl.obj"
	-@erase "$(INTDIR)\viewppg.obj"
	-@erase "$(INTDIR)\vwmenu.obj"
	-@erase "$(INTDIR)\vwrend60.pch"
	-@erase "$(INTDIR)\vwrendvw.idb"
	-@erase "$(INTDIR)\VWRendVw.obj"
	-@erase "$(INTDIR)\VWRendVw.res"
	-@erase "$(INTDIR)\VWRendVw.tlb"
	-@erase "$(INTDIR)\VWRnVwCt.obj"
	-@erase "$(INTDIR)\VWRnVwPp.obj"
	-@erase "$(INTDIR)\vwsgfxut.obj"
	-@erase "$(INTDIR)\vwuiview.obj"
	-@erase "$(OUTDIR)\vwrendvw.exp"
	-@erase "$(OUTDIR)\vwrendvw.ilk"
	-@erase "$(OUTDIR)\vwrendvw.lib"
	-@erase "$(OUTDIR)\vwrendvw.ocx"
	-@erase "$(OUTDIR)\vwrendvw.pdb"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\vwrend60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwrendvw.pdb" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VWRendVw.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwrend60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=d3drm.lib ddraw.lib dxguid.lib uuid.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\vwrendvw.pdb" /debug /machine:I386 /def:".\VWRendVw.def" /out:"$(OUTDIR)\vwrendvw.ocx" /implib:"$(OUTDIR)\vwrendvw.lib" /libpath:"\mssdk\lib" 
DEF_FILE= \
	".\VWRendVw.def"
LINK32_OBJS= \
	"$(INTDIR)\Boundary.obj" \
	"$(INTDIR)\cmddel.obj" \
	"$(INTDIR)\CmdHandl.obj" \
	"$(INTDIR)\edit.obj" \
	"$(INTDIR)\geomsel.obj" \
	"$(INTDIR)\OptionDg.obj" \
	"$(INTDIR)\rlutil.obj" \
	"$(INTDIR)\SelctDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\ToolDlg.obj" \
	"$(INTDIR)\viewctl.obj" \
	"$(INTDIR)\viewppg.obj" \
	"$(INTDIR)\vwmenu.obj" \
	"$(INTDIR)\VWRendVw.obj" \
	"$(INTDIR)\VWRnVwCt.obj" \
	"$(INTDIR)\VWRnVwPp.obj" \
	"$(INTDIR)\vwsgfxut.obj" \
	"$(INTDIR)\vwuiview.obj" \
	"$(INTDIR)\VWRendVw.res"

"$(OUTDIR)\vwrendvw.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\vwrendvw.ocx
InputPath=.\Debug\vwrendvw.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("vwrend60.dep")
!INCLUDE "vwrend60.dep"
!ELSE 
!MESSAGE Warning: cannot find "vwrend60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "VWRendVw - Win32 Release" || "$(CFG)" == "VWRendVw - Win32 Debug"
SOURCE=.\Boundary.cpp

"$(INTDIR)\Boundary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=..\..\vwcommon\cmddel.cpp

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\cmddel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwrendvw.pdb" /FD /c 

"$(INTDIR)\cmddel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\CmdHandl.cpp

"$(INTDIR)\CmdHandl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=.\edit.cpp

"$(INTDIR)\edit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=.\geomsel.cpp

"$(INTDIR)\geomsel.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=.\OptionDg.cpp

"$(INTDIR)\OptionDg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=.\rlutil.cpp

"$(INTDIR)\rlutil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=.\SelctDlg.cpp

"$(INTDIR)\SelctDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\vwrend60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwrend60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\vwrend60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwrendvw.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwrend60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\vwcommon\syshelp.cpp

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /I "..\..\vwsystem" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\vwrend60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /I "..\..\vwsystem" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\vwrend60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwrendvw.pdb" /FD /c 

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ToolDlg.cpp

"$(INTDIR)\ToolDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=..\vwview\viewctl.cpp

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /I "..\..\vwsystem" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\vwrend60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\viewctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /I "..\..\vwsystem" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\vwrend60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwrendvw.pdb" /FD /c 

"$(INTDIR)\viewctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwview\viewppg.cpp

"$(INTDIR)\viewppg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\vwcommon\vwmenu.cpp

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\vwmenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwrendvw.pdb" /FD /c 

"$(INTDIR)\vwmenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\VWRendVw.cpp

"$(INTDIR)\VWRendVw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=.\VWRendVw.odl

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /D "VWSYSTEM_BUILD" /tlb "$(OUTDIR)\VWRendVw.tlb" /mktyplib203 /win32 

"$(OUTDIR)\VWRendVw.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

MTL_SWITCHES=/nologo /I "..\..\vwcommon" /D "_DEBUG" /D "VWSYSTEM_BUILD" /tlb "$(OUTDIR)\VWRendVw.tlb" /mktyplib203 /win32 

"$(OUTDIR)\VWRendVw.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\VWRendVw.rc

!IF  "$(CFG)" == "VWRendVw - Win32 Release"


"$(INTDIR)\VWRendVw.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VWRendVw.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\VWRendVw.res" /i ".\Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"


"$(INTDIR)\VWRendVw.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VWRendVw.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\VWRendVw.res" /i ".\Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\VWRnVwCt.cpp

"$(INTDIR)\VWRnVwCt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=.\VWRnVwPp.cpp

"$(INTDIR)\VWRnVwPp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"


SOURCE=..\..\vwcommon\vwsgfxut.cpp

"$(INTDIR)\vwsgfxut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwrend60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\vwcommon\vwuiview.cpp

!IF  "$(CFG)" == "VWRendVw - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /I "..\..\vwsystem" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\vwuiview.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWRendVw - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\vwstudio" /I "..\vwview" /I "..\..\d2d" /I "..\..\vwmm" /I "..\..\vwsystem" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwrendvw.pdb" /FD /c 

"$(INTDIR)\vwuiview.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

