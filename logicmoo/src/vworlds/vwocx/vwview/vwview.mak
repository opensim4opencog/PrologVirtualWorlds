# Microsoft Developer Studio Generated NMAKE File, Based on vwview.dsp
!IF "$(CFG)" == ""
CFG=VWView - Win32 Release
!MESSAGE No configuration specified. Defaulting to VWView - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "VWView - Win32 Release" && "$(CFG)" != "VWView - Win32 Debug"\
 && "$(CFG)" != "VWView - Win32 Unicode Debug" && "$(CFG)" !=\
 "VWView - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vwview.mak" CFG="VWView - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VWView - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VWView - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VWView - Win32 Unicode Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VWView - Win32 Unicode Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VWView - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vwview.ocx" "$(OutDir)\regsvr32.trg"

!ELSE 

ALL : "$(OUTDIR)\vwview.ocx" "$(OutDir)\regsvr32.trg"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Cathelp.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ViewCtl.obj"
	-@erase "$(INTDIR)\ViewPpg.obj"
	-@erase "$(INTDIR)\VWView.obj"
	-@erase "$(INTDIR)\VWView.res"
	-@erase "$(INTDIR)\VWView.tlb"
	-@erase "$(OUTDIR)\vwview.exp"
	-@erase "$(OUTDIR)\vwview.lib"
	-@erase "$(OUTDIR)\vwview.ocx"
	-@erase "$(OutDir)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "." /I "..\..\vwcommon" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D\
 "_VWSTANDALONE_CLASS_" /Fp"$(INTDIR)\vwview.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VWView.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwview.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\vwview.pdb" /machine:I386 /def:".\VWView.def"\
 /out:"$(OUTDIR)\vwview.ocx" /implib:"$(OUTDIR)\vwview.lib" 
DEF_FILE= \
	".\VWView.def"
LINK32_OBJS= \
	"$(INTDIR)\Cathelp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ViewCtl.obj" \
	"$(INTDIR)\ViewPpg.obj" \
	"$(INTDIR)\VWView.obj" \
	"$(INTDIR)\VWView.res"

"$(OUTDIR)\vwview.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\.\Release
TargetPath=.\Release\vwview.ocx
InputPath=.\Release\vwview.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	

!ELSEIF  "$(CFG)" == "VWView - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vwview.ocx" "$(OUTDIR)\vwview.bsc" "$(OutDir)\regsvr32.trg"

!ELSE 

ALL : "$(OUTDIR)\vwview.ocx" "$(OUTDIR)\vwview.bsc" "$(OutDir)\regsvr32.trg"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Cathelp.obj"
	-@erase "$(INTDIR)\Cathelp.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\ViewCtl.obj"
	-@erase "$(INTDIR)\ViewCtl.sbr"
	-@erase "$(INTDIR)\ViewPpg.obj"
	-@erase "$(INTDIR)\ViewPpg.sbr"
	-@erase "$(INTDIR)\VWView.obj"
	-@erase "$(INTDIR)\VWView.res"
	-@erase "$(INTDIR)\VWView.sbr"
	-@erase "$(INTDIR)\VWView.tlb"
	-@erase "$(OUTDIR)\vwview.bsc"
	-@erase "$(OUTDIR)\vwview.exp"
	-@erase "$(OUTDIR)\vwview.ilk"
	-@erase "$(OUTDIR)\vwview.lib"
	-@erase "$(OUTDIR)\vwview.ocx"
	-@erase "$(OUTDIR)\vwview.pdb"
	-@erase "$(OutDir)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "." /I "..\..\vwcommon" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D\
 "_VWSTANDALONE_CLASS_" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\vwview.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VWView.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwview.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Cathelp.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\ViewCtl.sbr" \
	"$(INTDIR)\ViewPpg.sbr" \
	"$(INTDIR)\VWView.sbr"

"$(OUTDIR)\vwview.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\vwview.pdb" /debug /machine:I386 /def:".\VWView.def"\
 /out:"$(OUTDIR)\vwview.ocx" /implib:"$(OUTDIR)\vwview.lib" 
DEF_FILE= \
	".\VWView.def"
LINK32_OBJS= \
	"$(INTDIR)\Cathelp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ViewCtl.obj" \
	"$(INTDIR)\ViewPpg.obj" \
	"$(INTDIR)\VWView.obj" \
	"$(INTDIR)\VWView.res"

"$(OUTDIR)\vwview.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\.\Debug
TargetPath=.\Debug\vwview.ocx
InputPath=.\Debug\vwview.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	

!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Debug"

OUTDIR=.\DebugU
INTDIR=.\DebugU
# Begin Custom Macros
OutDir=.\.\DebugU
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vwview.ocx" "$(OutDir)\regsvr32.trg"

!ELSE 

ALL : "$(OUTDIR)\vwview.ocx" "$(OutDir)\regsvr32.trg"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Cathelp.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\ViewCtl.obj"
	-@erase "$(INTDIR)\ViewPpg.obj"
	-@erase "$(INTDIR)\VWView.obj"
	-@erase "$(INTDIR)\VWView.res"
	-@erase "$(INTDIR)\VWView.tlb"
	-@erase "$(OUTDIR)\vwview.exp"
	-@erase "$(OUTDIR)\vwview.ilk"
	-@erase "$(OUTDIR)\vwview.lib"
	-@erase "$(OUTDIR)\vwview.ocx"
	-@erase "$(OUTDIR)\vwview.pdb"
	-@erase "$(OutDir)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "." /I "..\..\vwcommon" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D\
 "_VWSTANDALONE_CLASS_" /Fp"$(INTDIR)\vwview.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\DebugU/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VWView.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwview.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\vwview.pdb" /debug /machine:I386 /def:".\VWView.def"\
 /out:"$(OUTDIR)\vwview.ocx" /implib:"$(OUTDIR)\vwview.lib" 
DEF_FILE= \
	".\VWView.def"
LINK32_OBJS= \
	"$(INTDIR)\Cathelp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ViewCtl.obj" \
	"$(INTDIR)\ViewPpg.obj" \
	"$(INTDIR)\VWView.obj" \
	"$(INTDIR)\VWView.res"

"$(OUTDIR)\vwview.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\.\DebugU
TargetPath=.\DebugU\vwview.ocx
InputPath=.\DebugU\vwview.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	

!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Release"

OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU
# Begin Custom Macros
OutDir=.\.\ReleaseU
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vwview.ocx" "$(OutDir)\regsvr32.trg"

!ELSE 

ALL : "$(OUTDIR)\vwview.ocx" "$(OutDir)\regsvr32.trg"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Cathelp.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ViewCtl.obj"
	-@erase "$(INTDIR)\ViewPpg.obj"
	-@erase "$(INTDIR)\VWView.obj"
	-@erase "$(INTDIR)\VWView.res"
	-@erase "$(INTDIR)\VWView.tlb"
	-@erase "$(OUTDIR)\vwview.exp"
	-@erase "$(OUTDIR)\vwview.lib"
	-@erase "$(OUTDIR)\vwview.ocx"
	-@erase "$(OutDir)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "." /I "..\..\vwcommon" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D\
 "_VWSTANDALONE_CLASS_" /Fp"$(INTDIR)\vwview.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\ReleaseU/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VWView.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwview.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\vwview.pdb" /machine:I386 /def:".\VWView.def"\
 /out:"$(OUTDIR)\vwview.ocx" /implib:"$(OUTDIR)\vwview.lib" 
DEF_FILE= \
	".\VWView.def"
LINK32_OBJS= \
	"$(INTDIR)\Cathelp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ViewCtl.obj" \
	"$(INTDIR)\ViewPpg.obj" \
	"$(INTDIR)\VWView.obj" \
	"$(INTDIR)\VWView.res"

"$(OUTDIR)\vwview.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\.\ReleaseU
TargetPath=.\ReleaseU\vwview.ocx
InputPath=.\ReleaseU\vwview.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "VWView - Win32 Release" || "$(CFG)" == "VWView - Win32 Debug"\
 || "$(CFG)" == "VWView - Win32 Unicode Debug" || "$(CFG)" ==\
 "VWView - Win32 Unicode Release"
SOURCE=..\..\vwcommon\Cathelp.cpp

!IF  "$(CFG)" == "VWView - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "." /I "..\..\vwcommon" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D\
 "_VWSTANDALONE_CLASS_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWView - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "." /I "..\..\vwcommon" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D\
 "_VWSTANDALONE_CLASS_" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\Cathelp.obj"	"$(INTDIR)\Cathelp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "." /I "..\..\vwcommon" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D\
 "_VWSTANDALONE_CLASS_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "." /I "..\..\vwcommon" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D\
 "_VWSTANDALONE_CLASS_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	"..\..\vwcommon\cellprop.h"\
	"..\..\vwcommon\d2d.h"\
	"..\..\vwcommon\default.h"\
	"..\..\vwcommon\inetfile.h"\
	"..\..\vwcommon\menuitem.h"\
	"..\..\vwcommon\propbase.h"\
	"..\..\vwcommon\vector.h"\
	"..\..\vwcommon\vwanim.h"\
	"..\..\vwcommon\vwbuffer.h"\
	"..\..\vwcommon\vwclient.h"\
	"..\..\vwcommon\vwcomm.h"\
	"..\..\vwcommon\VWFrame.h"\
	"..\..\vwcommon\VWGCache.h"\
	"..\..\vwcommon\vwgeom.h"\
	"..\..\vwcommon\vwgeomsp.h"\
	"..\..\vwcommon\vwhtml.h"\
	"..\..\vwcommon\vwidata.h"\
	"..\..\vwcommon\vwimpl.cpp"\
	"..\..\vwcommon\VWNvTarg.h"\
	"..\..\vwcommon\vwnvtgth.h"\
	"..\..\vwcommon\vwnvtool.h"\
	"..\..\vwcommon\vwobject.h"\
	"..\..\vwcommon\vwodb.h"\
	"..\..\vwcommon\vwpktool.h"\
	"..\..\vwcommon\vwrender.h"\
	"..\..\vwcommon\vwserver.h"\
	"..\..\vwcommon\vwsound.h"\
	"..\..\vwcommon\vwstats.h"\
	"..\..\vwcommon\vwsystem.h"\
	"..\..\vwcommon\vwtool.h"\
	"..\..\vwcommon\vwtrans.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "VWView - Win32 Release"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Debug"


"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Debug"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Release"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ENDIF 

SOURCE=.\ViewCtl.cpp
DEP_CPP_VIEWC=\
	"..\..\vwcommon\cathelp.h"\
	"..\..\vwcommon\cellprop.h"\
	"..\..\vwcommon\d2d.h"\
	"..\..\vwcommon\default.h"\
	"..\..\vwcommon\inetfile.h"\
	"..\..\vwcommon\menuitem.h"\
	"..\..\vwcommon\propbase.h"\
	"..\..\vwcommon\vector.h"\
	"..\..\vwcommon\vwanim.h"\
	"..\..\vwcommon\vwbuffer.h"\
	"..\..\vwcommon\vwclient.h"\
	"..\..\vwcommon\vwclient_i.c"\
	"..\..\vwcommon\vwcomm.h"\
	"..\..\vwcommon\VWFrame.h"\
	"..\..\vwcommon\VWGCache.h"\
	"..\..\vwcommon\vwgeom.h"\
	"..\..\vwcommon\vwgeomsp.h"\
	"..\..\vwcommon\vwhtml.h"\
	"..\..\vwcommon\vwidata.h"\
	"..\..\vwcommon\VWNvTarg.h"\
	"..\..\vwcommon\vwnvtgth.h"\
	"..\..\vwcommon\vwnvtool.h"\
	"..\..\vwcommon\vwobject.h"\
	"..\..\vwcommon\vwobject_i.c"\
	"..\..\vwcommon\vwodb.h"\
	"..\..\vwcommon\vwpktool.h"\
	"..\..\vwcommon\vwrender.h"\
	"..\..\vwcommon\vwserver.h"\
	"..\..\vwcommon\vwsound.h"\
	"..\..\vwcommon\vwstats.h"\
	"..\..\vwcommon\vwsystem.h"\
	"..\..\vwcommon\vwtool.h"\
	"..\..\vwcommon\vwtrans.h"\
	".\stdafx.h"\
	".\viewctl.h"\
	".\viewppg.h"\
	".\VWView.h"\
	

!IF  "$(CFG)" == "VWView - Win32 Release"


"$(INTDIR)\ViewCtl.obj" : $(SOURCE) $(DEP_CPP_VIEWC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Debug"


"$(INTDIR)\ViewCtl.obj"	"$(INTDIR)\ViewCtl.sbr" : $(SOURCE) $(DEP_CPP_VIEWC)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Debug"


"$(INTDIR)\ViewCtl.obj" : $(SOURCE) $(DEP_CPP_VIEWC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Release"


"$(INTDIR)\ViewCtl.obj" : $(SOURCE) $(DEP_CPP_VIEWC) "$(INTDIR)"


!ENDIF 

SOURCE=.\ViewPpg.cpp
DEP_CPP_VIEWP=\
	".\stdafx.h"\
	".\viewppg.h"\
	".\VWView.h"\
	

!IF  "$(CFG)" == "VWView - Win32 Release"


"$(INTDIR)\ViewPpg.obj" : $(SOURCE) $(DEP_CPP_VIEWP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Debug"


"$(INTDIR)\ViewPpg.obj"	"$(INTDIR)\ViewPpg.sbr" : $(SOURCE) $(DEP_CPP_VIEWP)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Debug"


"$(INTDIR)\ViewPpg.obj" : $(SOURCE) $(DEP_CPP_VIEWP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Release"


"$(INTDIR)\ViewPpg.obj" : $(SOURCE) $(DEP_CPP_VIEWP) "$(INTDIR)"


!ENDIF 

SOURCE=.\VWView.cpp
DEP_CPP_VWVIE=\
	".\stdafx.h"\
	".\VWView.h"\
	

!IF  "$(CFG)" == "VWView - Win32 Release"


"$(INTDIR)\VWView.obj" : $(SOURCE) $(DEP_CPP_VWVIE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Debug"


"$(INTDIR)\VWView.obj"	"$(INTDIR)\VWView.sbr" : $(SOURCE) $(DEP_CPP_VWVIE)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Debug"


"$(INTDIR)\VWView.obj" : $(SOURCE) $(DEP_CPP_VWVIE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Release"


"$(INTDIR)\VWView.obj" : $(SOURCE) $(DEP_CPP_VWVIE) "$(INTDIR)"


!ENDIF 

SOURCE=.\VWView.odl

!IF  "$(CFG)" == "VWView - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\VWView.tlb" /mktyplib203\
 /win32 

"$(OUTDIR)\VWView.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWView - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\VWView.tlb" /mktyplib203\
 /win32 

"$(OUTDIR)\VWView.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\VWView.tlb" /mktyplib203\
 /win32 

"$(OUTDIR)\VWView.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\VWView.tlb" /mktyplib203\
 /win32 

"$(OUTDIR)\VWView.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\VWView.rc

!IF  "$(CFG)" == "VWView - Win32 Release"

DEP_RSC_VWVIEW=\
	".\version.h"\
	".\version.rc"\
	".\viewctl.bmp"\
	".\vwview.ico"\
	
NODEP_RSC_VWVIEW=\
	".\Release\VWView.tlb"\
	

"$(INTDIR)\VWView.res" : $(SOURCE) $(DEP_RSC_VWVIEW) "$(INTDIR)"\
 "$(INTDIR)\VWView.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\VWView.res" /i ".\Release" /d "NDEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "VWView - Win32 Debug"

DEP_RSC_VWVIEW=\
	".\Debug\VWView.tlb"\
	".\version.h"\
	".\version.rc"\
	".\viewctl.bmp"\
	".\vwview.ico"\
	

"$(INTDIR)\VWView.res" : $(SOURCE) $(DEP_RSC_VWVIEW) "$(INTDIR)"\
 "$(INTDIR)\VWView.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\VWView.res" /i ".\Debug" /d "_DEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Debug"

DEP_RSC_VWVIEW=\
	".\version.h"\
	".\version.rc"\
	".\viewctl.bmp"\
	".\vwview.ico"\
	
NODEP_RSC_VWVIEW=\
	".\DebugU\VWView.tlb"\
	

"$(INTDIR)\VWView.res" : $(SOURCE) $(DEP_RSC_VWVIEW) "$(INTDIR)"\
 "$(INTDIR)\VWView.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\VWView.res" /i ".\DebugU" /d "_DEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "VWView - Win32 Unicode Release"

DEP_RSC_VWVIEW=\
	".\version.h"\
	".\version.rc"\
	".\viewctl.bmp"\
	".\vwview.ico"\
	
NODEP_RSC_VWVIEW=\
	".\ReleaseU\VWView.tlb"\
	

"$(INTDIR)\VWView.res" : $(SOURCE) $(DEP_RSC_VWVIEW) "$(INTDIR)"\
 "$(INTDIR)\VWView.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\VWView.res" /i ".\ReleaseU" /d "NDEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ENDIF 


!ENDIF 

