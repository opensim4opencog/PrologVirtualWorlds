# Microsoft Developer Studio Generated NMAKE File, Based on sengin60.dsp
!IF "$(CFG)" == ""
CFG=SEngine - Win32 Release
!MESSAGE No configuration specified. Defaulting to SEngine - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "SEngine - Win32 Release" && "$(CFG)" != "SEngine - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sengin60.mak" CFG="SEngine - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SEngine - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SEngine - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SEngine - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\sengine.exe" "$(OUTDIR)\SEngine.tlb"


CLEAN :
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\DBGView.obj"
	-@erase "$(INTDIR)\getmedlg.obj"
	-@erase "$(INTDIR)\gotodlg.obj"
	-@erase "$(INTDIR)\guids.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\OCXView.obj"
	-@erase "$(INTDIR)\OleID.obj"
	-@erase "$(INTDIR)\scripts.obj"
	-@erase "$(INTDIR)\sengdoc.obj"
	-@erase "$(INTDIR)\SEngine.obj"
	-@erase "$(INTDIR)\SEngine.res"
	-@erase "$(INTDIR)\SEngine.tlb"
	-@erase "$(INTDIR)\sengview.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\sengine.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "." /I "..\vwcommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\sengin60.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SEngine.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sengin60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=uuid.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\sengine.pdb" /machine:I386 /out:"$(OUTDIR)\sengine.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\DBGView.obj" \
	"$(INTDIR)\getmedlg.obj" \
	"$(INTDIR)\gotodlg.obj" \
	"$(INTDIR)\guids.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OCXView.obj" \
	"$(INTDIR)\OleID.obj" \
	"$(INTDIR)\scripts.obj" \
	"$(INTDIR)\sengdoc.obj" \
	"$(INTDIR)\SEngine.obj" \
	"$(INTDIR)\sengview.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SEngine.res"

"$(OUTDIR)\sengine.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SEngine - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\sengine.exe" "$(OUTDIR)\SEngine.tlb"


CLEAN :
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\DBGView.obj"
	-@erase "$(INTDIR)\getmedlg.obj"
	-@erase "$(INTDIR)\gotodlg.obj"
	-@erase "$(INTDIR)\guids.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\OCXView.obj"
	-@erase "$(INTDIR)\OleID.obj"
	-@erase "$(INTDIR)\scripts.obj"
	-@erase "$(INTDIR)\sengdoc.obj"
	-@erase "$(INTDIR)\sengine.idb"
	-@erase "$(INTDIR)\SEngine.obj"
	-@erase "$(INTDIR)\SEngine.res"
	-@erase "$(INTDIR)\SEngine.tlb"
	-@erase "$(INTDIR)\sengview.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\sengine.exe"
	-@erase "$(OUTDIR)\sengine.ilk"
	-@erase "$(OUTDIR)\sengine.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\sengin60.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\sengine.pdb" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SEngine.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sengin60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=uuid.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\sengine.pdb" /debug /machine:I386 /out:"$(OUTDIR)\sengine.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\DBGView.obj" \
	"$(INTDIR)\getmedlg.obj" \
	"$(INTDIR)\gotodlg.obj" \
	"$(INTDIR)\guids.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OCXView.obj" \
	"$(INTDIR)\OleID.obj" \
	"$(INTDIR)\scripts.obj" \
	"$(INTDIR)\sengdoc.obj" \
	"$(INTDIR)\SEngine.obj" \
	"$(INTDIR)\sengview.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SEngine.res"

"$(OUTDIR)\sengine.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("sengin60.dep")
!INCLUDE "sengin60.dep"
!ELSE 
!MESSAGE Warning: cannot find "sengin60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SEngine - Win32 Release" || "$(CFG)" == "SEngine - Win32 Debug"
SOURCE=.\ChildFrm.cpp

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DBGView.cpp

"$(INTDIR)\DBGView.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getmedlg.cpp

"$(INTDIR)\getmedlg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gotodlg.cpp

"$(INTDIR)\gotodlg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\guids.cpp

"$(INTDIR)\guids.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OCXView.cpp

"$(INTDIR)\OCXView.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OleID.cpp

"$(INTDIR)\OleID.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\vwcommon\scripts.cpp

"$(INTDIR)\scripts.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\sengdoc.cpp

"$(INTDIR)\sengdoc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SEngine.cpp

"$(INTDIR)\SEngine.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SEngine.odl

!IF  "$(CFG)" == "SEngine - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\SEngine.tlb" /mktyplib203 /win32 

"$(OUTDIR)\SEngine.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SEngine - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\SEngine.tlb" /mktyplib203 /win32 

"$(OUTDIR)\SEngine.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SEngine.rc

!IF  "$(CFG)" == "SEngine - Win32 Release"


"$(INTDIR)\SEngine.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\SEngine.res" /i ".\Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "SEngine - Win32 Debug"


"$(INTDIR)\SEngine.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\SEngine.res" /i ".\Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\sengview.cpp

"$(INTDIR)\sengview.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SEngine - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "." /I "..\vwcommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\sengin60.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SEngine - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\sengin60.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\sengine.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

