# Microsoft Developer Studio Generated NMAKE File, Based on vwstud60.dsp
!IF "$(CFG)" == ""
CFG=vwstudio - Win32 Debug
!MESSAGE No configuration specified. Defaulting to vwstudio - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "vwstudio - Win32 Debug" && "$(CFG)" != "vwstudio - Win32 Release MinSize"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\vwstudio_i.c" ".\vwstudio.tlb" ".\vwstudio.h" "..\vwcommon\webtools_i.c" "..\vwcommon\webtools.h" "..\vwcommon\vwstuex_i.c" "..\vwcommon\vwstuex.h" "..\vwcommon\stdtools_i.c" "..\vwcommon\stdtools.h" "..\vwcommon\geomtool_i.c" "..\vwcommon\geomtool.h" "..\vwcommon\bndytool_i.c" "..\vwcommon\bndytool.h" "$(OUTDIR)\vwstudio.dll" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\cathelp.obj"
	-@erase "$(INTDIR)\ctrlmgr.obj"
	-@erase "$(INTDIR)\dlldatax.obj"
	-@erase "$(INTDIR)\FileAcTl.obj"
	-@erase "$(INTDIR)\geomsel.obj"
	-@erase "$(INTDIR)\geomundo.obj"
	-@erase "$(INTDIR)\polytool.obj"
	-@erase "$(INTDIR)\reghelp.obj"
	-@erase "$(INTDIR)\rotat2d.obj"
	-@erase "$(INTDIR)\rottool.obj"
	-@erase "$(INTDIR)\rwhelper.obj"
	-@erase "$(INTDIR)\scale2d.obj"
	-@erase "$(INTDIR)\scaletl.obj"
	-@erase "$(INTDIR)\select.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\trans2d.obj"
	-@erase "$(INTDIR)\trans3d.obj"
	-@erase "$(INTDIR)\undoitem.obj"
	-@erase "$(INTDIR)\undostak.obj"
	-@erase "$(INTDIR)\undoutil.obj"
	-@erase "$(INTDIR)\vwerrtl.obj"
	-@erase "$(INTDIR)\vwsgfxut.obj"
	-@erase "$(INTDIR)\vwstud60.pch"
	-@erase "$(INTDIR)\vwstudio.idb"
	-@erase "$(INTDIR)\vwstudio.obj"
	-@erase "$(INTDIR)\vwstudio.res"
	-@erase "$(INTDIR)\vwstuexo.obj"
	-@erase "$(INTDIR)\vwsutils.obj"
	-@erase "$(INTDIR)\vwuiobjs_p.obj"
	-@erase "$(INTDIR)\wwhelper.obj"
	-@erase "$(INTDIR)\ZoomTool.obj"
	-@erase "$(OUTDIR)\vwstudio.dll"
	-@erase "$(OUTDIR)\vwstudio.exp"
	-@erase "$(OUTDIR)\vwstudio.ilk"
	-@erase "$(OUTDIR)\vwstudio.lib"
	-@erase "$(OUTDIR)\vwstudio.pdb"
	-@erase "..\vwcommon\bndytool.h"
	-@erase "..\vwcommon\bndytool_i.c"
	-@erase "..\vwcommon\geomtool.h"
	-@erase "..\vwcommon\geomtool_i.c"
	-@erase "..\vwcommon\stdtools.h"
	-@erase "..\vwcommon\stdtools_i.c"
	-@erase "..\vwcommon\vwstuex.h"
	-@erase "..\vwcommon\vwstuex_i.c"
	-@erase "..\vwcommon\webtools.h"
	-@erase "..\vwcommon\webtools_i.c"
	-@erase ".\Debug\regsvr32.trg"
	-@erase ".\vwstudio.h"
	-@erase ".\vwstudio.tlb"
	-@erase ".\vwstudio_i.c"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\vwstudio.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwstud60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=d3drm.lib dxguid.lib uuid.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\vwstudio.pdb" /debug /machine:I386 /def:".\vwstudio.def" /out:"$(OUTDIR)\vwstudio.dll" /implib:"$(OUTDIR)\vwstudio.lib" /libpath:"\mssdk\lib" 
DEF_FILE= \
	".\vwstudio.def"
LINK32_OBJS= \
	"$(INTDIR)\cathelp.obj" \
	"$(INTDIR)\dlldatax.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\vwstudio.obj" \
	"$(INTDIR)\vwuiobjs_p.obj" \
	"$(INTDIR)\ctrlmgr.obj" \
	"$(INTDIR)\select.obj" \
	"$(INTDIR)\undoitem.obj" \
	"$(INTDIR)\undostak.obj" \
	"$(INTDIR)\vwerrtl.obj" \
	"$(INTDIR)\vwsgfxut.obj" \
	"$(INTDIR)\vwsutils.obj" \
	"$(INTDIR)\geomsel.obj" \
	"$(INTDIR)\geomundo.obj" \
	"$(INTDIR)\rottool.obj" \
	"$(INTDIR)\scaletl.obj" \
	"$(INTDIR)\trans3d.obj" \
	"$(INTDIR)\undoutil.obj" \
	"$(INTDIR)\FileAcTl.obj" \
	"$(INTDIR)\reghelp.obj" \
	"$(INTDIR)\rwhelper.obj" \
	"$(INTDIR)\wwhelper.obj" \
	"$(INTDIR)\polytool.obj" \
	"$(INTDIR)\rotat2d.obj" \
	"$(INTDIR)\scale2d.obj" \
	"$(INTDIR)\trans2d.obj" \
	"$(INTDIR)\ZoomTool.obj" \
	"$(INTDIR)\vwstuexo.obj" \
	"$(INTDIR)\vwstudio.res"

"$(OUTDIR)\vwstudio.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\vwstudio.dll
InputPath=.\Debug\vwstudio.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

OUTDIR=.\ReleaseMinSize
INTDIR=.\ReleaseMinSize
# Begin Custom Macros
OutDir=.\ReleaseMinSize
# End Custom Macros

ALL : ".\vwstudio_i.c" ".\vwstudio.tlb" ".\vwstudio.h" "..\vwcommon\webtools_i.c" "..\vwcommon\webtools.h" "..\vwcommon\vwstuex_i.c" "..\vwcommon\vwstuex.h" "..\vwcommon\stdtools_i.c" "..\vwcommon\stdtools.h" "..\vwcommon\geomtool_i.c" "..\vwcommon\geomtool.h" "..\vwcommon\bndytool_i.c" "..\vwcommon\bndytool.h" "$(OUTDIR)\vwstudio.dll" ".\ReleaseMinSize\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\cathelp.obj"
	-@erase "$(INTDIR)\ctrlmgr.obj"
	-@erase "$(INTDIR)\dlldatax.obj"
	-@erase "$(INTDIR)\FileAcTl.obj"
	-@erase "$(INTDIR)\geomsel.obj"
	-@erase "$(INTDIR)\geomundo.obj"
	-@erase "$(INTDIR)\polytool.obj"
	-@erase "$(INTDIR)\reghelp.obj"
	-@erase "$(INTDIR)\rotat2d.obj"
	-@erase "$(INTDIR)\rottool.obj"
	-@erase "$(INTDIR)\rwhelper.obj"
	-@erase "$(INTDIR)\scale2d.obj"
	-@erase "$(INTDIR)\scaletl.obj"
	-@erase "$(INTDIR)\select.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\trans2d.obj"
	-@erase "$(INTDIR)\trans3d.obj"
	-@erase "$(INTDIR)\undoitem.obj"
	-@erase "$(INTDIR)\undostak.obj"
	-@erase "$(INTDIR)\undoutil.obj"
	-@erase "$(INTDIR)\vwerrtl.obj"
	-@erase "$(INTDIR)\vwsgfxut.obj"
	-@erase "$(INTDIR)\vwstud60.pch"
	-@erase "$(INTDIR)\vwstudio.idb"
	-@erase "$(INTDIR)\vwstudio.obj"
	-@erase "$(INTDIR)\vwstudio.res"
	-@erase "$(INTDIR)\vwstuexo.obj"
	-@erase "$(INTDIR)\vwsutils.obj"
	-@erase "$(INTDIR)\vwuiobjs_p.obj"
	-@erase "$(INTDIR)\wwhelper.obj"
	-@erase "$(INTDIR)\ZoomTool.obj"
	-@erase "$(OUTDIR)\vwstudio.dll"
	-@erase "$(OUTDIR)\vwstudio.exp"
	-@erase "$(OUTDIR)\vwstudio.lib"
	-@erase "..\vwcommon\bndytool.h"
	-@erase "..\vwcommon\bndytool_i.c"
	-@erase "..\vwcommon\geomtool.h"
	-@erase "..\vwcommon\geomtool_i.c"
	-@erase "..\vwcommon\stdtools.h"
	-@erase "..\vwcommon\stdtools_i.c"
	-@erase "..\vwcommon\vwstuex.h"
	-@erase "..\vwcommon\vwstuex_i.c"
	-@erase "..\vwcommon\webtools.h"
	-@erase "..\vwcommon\webtools_i.c"
	-@erase ".\ReleaseMinSize\regsvr32.trg"
	-@erase ".\vwstudio.h"
	-@erase ".\vwstudio.tlb"
	-@erase ".\vwstudio_i.c"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\vwstudio.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwstud60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=d3drm.lib dxguid.lib uuid.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\vwstudio.pdb" /machine:I386 /def:".\vwstudio.def" /out:"$(OUTDIR)\vwstudio.dll" /implib:"$(OUTDIR)\vwstudio.lib" /libpath:"\mssdk\lib" 
DEF_FILE= \
	".\vwstudio.def"
LINK32_OBJS= \
	"$(INTDIR)\cathelp.obj" \
	"$(INTDIR)\dlldatax.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\vwstudio.obj" \
	"$(INTDIR)\vwuiobjs_p.obj" \
	"$(INTDIR)\ctrlmgr.obj" \
	"$(INTDIR)\select.obj" \
	"$(INTDIR)\undoitem.obj" \
	"$(INTDIR)\undostak.obj" \
	"$(INTDIR)\vwerrtl.obj" \
	"$(INTDIR)\vwsgfxut.obj" \
	"$(INTDIR)\vwsutils.obj" \
	"$(INTDIR)\geomsel.obj" \
	"$(INTDIR)\geomundo.obj" \
	"$(INTDIR)\rottool.obj" \
	"$(INTDIR)\scaletl.obj" \
	"$(INTDIR)\trans3d.obj" \
	"$(INTDIR)\undoutil.obj" \
	"$(INTDIR)\FileAcTl.obj" \
	"$(INTDIR)\reghelp.obj" \
	"$(INTDIR)\rwhelper.obj" \
	"$(INTDIR)\wwhelper.obj" \
	"$(INTDIR)\polytool.obj" \
	"$(INTDIR)\rotat2d.obj" \
	"$(INTDIR)\scale2d.obj" \
	"$(INTDIR)\trans2d.obj" \
	"$(INTDIR)\ZoomTool.obj" \
	"$(INTDIR)\vwstuexo.obj" \
	"$(INTDIR)\vwstudio.res"

"$(OUTDIR)\vwstudio.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\vwstudio.dll
InputPath=.\ReleaseMinSize\vwstudio.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("vwstud60.dep")
!INCLUDE "vwstud60.dep"
!ELSE 
!MESSAGE Warning: cannot find "vwstud60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "vwstudio - Win32 Debug" || "$(CFG)" == "vwstudio - Win32 Release MinSize"
SOURCE=..\vwcommon\cathelp.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\dlldatax.c

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\dlldatax.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\dlldatax.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwstud60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwstud60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\syshelp.cpp

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\vwstudio.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\stdtools" /I "..\tools\geomtool" /I "..\tools\celltool" /I "..\tools\popupmgr" /I "..\tools\webtools" /I "..\tools\ecpop" /I "..\tools\menupop" /I "..\tools\thingpop" /I "..\tools\verbpop" /I "..\module\studio" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwstudio.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\stdtools" /I "..\tools\geomtool" /I "..\tools\celltool" /I "..\tools\popupmgr" /I "..\tools\webtools" /I "..\tools\ecpop" /I "..\tools\menupop" /I "..\tools\thingpop" /I "..\tools\verbpop" /I "..\module\studio" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwstudio.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\vwstudio.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

InputPath=.\vwstudio.idl
USERDEP__VWSTU="..\vwcommon\cellprop.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwgeom.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwframe.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwnvtool.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwgeomar.idl"	

".\vwstudio.h"	".\vwstudio_i.c"	".\vwstudio.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWSTU)
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "vwstudio.h" /iid "vwstudio_i.c" /I "..\vwcommon" "vwstudio.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

InputPath=.\vwstudio.idl
USERDEP__VWSTU="..\vwcommon\cellprop.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwgeom.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwframe.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwnvtool.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwgeomar.idl"	

".\vwstudio.h"	".\vwstudio_i.c"	".\vwstudio.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWSTU)
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "vwstudio.h" /iid "vwstudio_i.c" /I "..\vwcommon" "vwstudio.idl"
<< 
	

!ENDIF 

SOURCE=.\vwstudio.rc

"$(INTDIR)\vwstudio.res" : $(SOURCE) "$(INTDIR)" ".\vwstudio.tlb"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwuiobjs_p.c

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwuiobjs_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwuiobjs_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\stdtools\ctrlmgr.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\ctrlmgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\ctrlmgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\stdtools\select.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\select.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\select.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\stdtools.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

InputPath=..\vwcommon\stdtools.idl

"..\vwcommon\stdtools.h"	"..\vwcommon\stdtools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "stdtools.h" /iid "stdtools_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "stdtools.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

InputPath=..\vwcommon\stdtools.idl

"..\vwcommon\stdtools.h"	"..\vwcommon\stdtools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "stdtools.h" /iid "stdtools_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "stdtools.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\stdtools\undoitem.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\undoitem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\undoitem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\stdtools\undostak.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\undostak.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\undostak.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\stdtools\vwerrtl.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwerrtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwerrtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwsgfxut.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwsgfxut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwsgfxut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwsutils.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwsutils.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\vwsutils.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\geomtool\geomsel.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\geomsel.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\geomsel.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\geomtool.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

InputPath=..\vwcommon\geomtool.idl

"..\vwcommon\geomtool.h"	"..\vwcommon\geomtool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "geomtool.h" /iid "geomtool_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "geomtool.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

InputPath=..\vwcommon\geomtool.idl

"..\vwcommon\geomtool.h"	"..\vwcommon\geomtool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "geomtool.h" /iid "geomtool_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "geomtool.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\geomtool\geomundo.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\geomundo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\geomundo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\geomtool\rottool.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\rottool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\rottool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\geomtool\scaletl.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\scaletl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\scaletl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\geomtool\trans3d.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\trans3d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\trans3d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\geomtool\undoutil.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\undoutil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"geomtool.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\undoutil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\webtools\FileAcTl.cpp

"$(INTDIR)\FileAcTl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\reghelp.cpp

"$(INTDIR)\reghelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\webtools\rwhelper.cpp

"$(INTDIR)\rwhelper.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\webtools.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

InputPath=..\vwcommon\webtools.idl

"..\vwcommon\webtools.h"	"..\vwcommon\webtools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "webtools.h" /iid "webtools_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "webtools.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

InputPath=..\vwcommon\webtools.idl

"..\vwcommon\webtools.h"	"..\vwcommon\webtools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "webtools.h" /iid "webtools_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "webtools.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\webtools\wwhelper.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"webtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\wwhelper.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"webtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\wwhelper.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\bndytool.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

InputPath=..\vwcommon\bndytool.idl

"..\vwcommon\bndytool.h"	"..\vwcommon\bndytool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "bndytool.h" /iid "bndytool_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "bndytool.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

InputPath=..\vwcommon\bndytool.idl

"..\vwcommon\bndytool.h"	"..\vwcommon\bndytool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "bndytool.h" /iid "bndytool_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "bndytool.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\stdtools\polytool.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\polytool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\polytool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\stdtools\rotat2d.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\rotat2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\rotat2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\stdtools\scale2d.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\scale2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\scale2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\stdtools\trans2d.cpp

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\trans2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwsystem" /I "..\vwmm" /I "..\tools\geomtool" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwstud60.pch" /Yu"stdtools.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwstudio.pdb" /FD /c 

"$(INTDIR)\trans2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tools\stdtools\ZoomTool.cpp

"$(INTDIR)\ZoomTool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwstuex.idl

!IF  "$(CFG)" == "vwstudio - Win32 Debug"

InputPath=..\vwcommon\vwstuex.idl

"..\vwcommon\vwstuex.h"	"..\vwcommon\vwstuex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "vwstuex.h" /iid "vwstuex_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "vwstuex.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwstudio - Win32 Release MinSize"

InputPath=..\vwcommon\vwstuex.idl

"..\vwcommon\vwstuex.h"	"..\vwcommon\vwstuex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /Oicf /h "vwstuex.h" /iid "vwstuex_i.c" /I "..\vwcommon"  /out                     "..\vwcommon" "vwstuex.idl"
<< 
	

!ENDIF 

SOURCE=..\module\studio\vwstuexo.cpp

"$(INTDIR)\vwstuexo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwstud60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

