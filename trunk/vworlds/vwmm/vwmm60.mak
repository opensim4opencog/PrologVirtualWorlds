# Microsoft Developer Studio Generated NMAKE File, Based on vwmm60.dsp
!IF "$(CFG)" == ""
CFG=VWMM - WIN32 RELEASE MINSIZE
!MESSAGE No configuration specified. Defaulting to VWMM - WIN32 RELEASE MINSIZE.
!ENDIF 

!IF "$(CFG)" != "vwmm - Win32 Debug" && "$(CFG)" != "vwmm - Win32 Release MinSize"
!MESSAGE Invalid configuration "$(CFG)" specified.
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

!IF  "$(CFG)" == "vwmm - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\vwmm.dll" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\BBox.obj"
	-@erase "$(INTDIR)\cathelp.obj"
	-@erase "$(INTDIR)\charobj.obj"
	-@erase "$(INTDIR)\conv3ds.obj"
	-@erase "$(INTDIR)\d3dmath.obj"
	-@erase "$(INTDIR)\dlldatax.obj"
	-@erase "$(INTDIR)\edgecyc.obj"
	-@erase "$(INTDIR)\jointobj.obj"
	-@erase "$(INTDIR)\mmartfct.obj"
	-@erase "$(INTDIR)\mmavatar.obj"
	-@erase "$(INTDIR)\mmhelp.obj"
	-@erase "$(INTDIR)\mmhost.obj"
	-@erase "$(INTDIR)\mmlight.obj"
	-@erase "$(INTDIR)\mmportal.obj"
	-@erase "$(INTDIR)\mmroom.obj"
	-@erase "$(INTDIR)\mmthing.obj"
	-@erase "$(INTDIR)\PickData.obj"
	-@erase "$(INTDIR)\Profhelp.obj"
	-@erase "$(INTDIR)\PuppetOb.obj"
	-@erase "$(INTDIR)\readwdb.obj"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\spritemo.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\vecmath.obj"
	-@erase "$(INTDIR)\vectorob.obj"
	-@erase "$(INTDIR)\vwadatao.obj"
	-@erase "$(INTDIR)\vwframeo.obj"
	-@erase "$(INTDIR)\vwgcacho.obj"
	-@erase "$(INTDIR)\vwgeoma.obj"
	-@erase "$(INTDIR)\vwgeomob.obj"
	-@erase "$(INTDIR)\vwgeoms.obj"
	-@erase "$(INTDIR)\vwidatao.obj"
	-@erase "$(INTDIR)\vwmenu.obj"
	-@erase "$(INTDIR)\vwmm.idb"
	-@erase "$(INTDIR)\vwmm.obj"
	-@erase "$(INTDIR)\vwmm.res"
	-@erase "$(INTDIR)\vwmm60.pch"
	-@erase "$(INTDIR)\vwmmexo.obj"
	-@erase "$(INTDIR)\vwnavt3d.obj"
	-@erase "$(INTDIR)\vwpktl.obj"
	-@erase "$(INTDIR)\vwrend2d.obj"
	-@erase "$(INTDIR)\vwrend3d.obj"
	-@erase "$(INTDIR)\vwrendrt.obj"
	-@erase "$(INTDIR)\vwuiobjs_p.obj"
	-@erase "$(OUTDIR)\vwmm.dll"
	-@erase "$(OUTDIR)\vwmm.exp"
	-@erase "$(OUTDIR)\vwmm.ilk"
	-@erase "$(OUTDIR)\vwmm.lib"
	-@erase "$(OUTDIR)\vwmm.pdb"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 /Os 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\vwmm.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwmm60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ddraw.lib d3drm.lib uuid.lib ftkvc40.lib rpcndr.lib rpcns4.lib rpcrt4.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\vwmm.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT" /nodefaultlib:"LIBC" /def:".\vwmm.def" /out:"$(OUTDIR)\vwmm.dll" /implib:"$(OUTDIR)\vwmm.lib" /libpath:"\mssdk\lib" /libpath:"..\tools\3dstod3d\3dsftk\lib" 
DEF_FILE= \
	".\vwmm.def"
LINK32_OBJS= \
	"$(INTDIR)\cathelp.obj" \
	"$(INTDIR)\dlldatax.obj" \
	"$(INTDIR)\mmhelp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\vwmm.obj" \
	"$(INTDIR)\vwuiobjs_p.obj" \
	"$(INTDIR)\vwmenu.obj" \
	"$(INTDIR)\vwpktl.obj" \
	"$(INTDIR)\vwnavt3d.obj" \
	"$(INTDIR)\edgecyc.obj" \
	"$(INTDIR)\vecmath.obj" \
	"$(INTDIR)\vectorob.obj" \
	"$(INTDIR)\vwgeomob.obj" \
	"$(INTDIR)\d3dmath.obj" \
	"$(INTDIR)\vwgeoms.obj" \
	"$(INTDIR)\vwidatao.obj" \
	"$(INTDIR)\charobj.obj" \
	"$(INTDIR)\jointobj.obj" \
	"$(INTDIR)\PuppetOb.obj" \
	"$(INTDIR)\vwgeoma.obj" \
	"$(INTDIR)\BBox.obj" \
	"$(INTDIR)\vwframeo.obj" \
	"$(INTDIR)\vwadatao.obj" \
	"$(INTDIR)\PickData.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\readwdb.obj" \
	"$(INTDIR)\vwgcacho.obj" \
	"$(INTDIR)\vwrend2d.obj" \
	"$(INTDIR)\vwrend3d.obj" \
	"$(INTDIR)\vwrendrt.obj" \
	"$(INTDIR)\spritemo.obj" \
	"$(INTDIR)\conv3ds.obj" \
	"$(INTDIR)\mmartfct.obj" \
	"$(INTDIR)\mmavatar.obj" \
	"$(INTDIR)\mmhost.obj" \
	"$(INTDIR)\mmlight.obj" \
	"$(INTDIR)\mmportal.obj" \
	"$(INTDIR)\mmroom.obj" \
	"$(INTDIR)\mmthing.obj" \
	"$(INTDIR)\Profhelp.obj" \
	"$(INTDIR)\vwmmexo.obj" \
	"$(INTDIR)\vwmm.res"

"$(OUTDIR)\vwmm.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\vwmm.dll
InputPath=.\Debug\vwmm.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

OUTDIR=.\ReleaseMinSize
INTDIR=.\ReleaseMinSize
# Begin Custom Macros
OutDir=.\ReleaseMinSize
# End Custom Macros

ALL : "$(OUTDIR)\vwmm.dll" ".\ReleaseMinSize\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\BBox.obj"
	-@erase "$(INTDIR)\cathelp.obj"
	-@erase "$(INTDIR)\charobj.obj"
	-@erase "$(INTDIR)\conv3ds.obj"
	-@erase "$(INTDIR)\d3dmath.obj"
	-@erase "$(INTDIR)\dlldatax.obj"
	-@erase "$(INTDIR)\edgecyc.obj"
	-@erase "$(INTDIR)\jointobj.obj"
	-@erase "$(INTDIR)\mmartfct.obj"
	-@erase "$(INTDIR)\mmavatar.obj"
	-@erase "$(INTDIR)\mmhelp.obj"
	-@erase "$(INTDIR)\mmhost.obj"
	-@erase "$(INTDIR)\mmlight.obj"
	-@erase "$(INTDIR)\mmportal.obj"
	-@erase "$(INTDIR)\mmroom.obj"
	-@erase "$(INTDIR)\mmthing.obj"
	-@erase "$(INTDIR)\PickData.obj"
	-@erase "$(INTDIR)\Profhelp.obj"
	-@erase "$(INTDIR)\PuppetOb.obj"
	-@erase "$(INTDIR)\readwdb.obj"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\spritemo.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\vecmath.obj"
	-@erase "$(INTDIR)\vectorob.obj"
	-@erase "$(INTDIR)\vwadatao.obj"
	-@erase "$(INTDIR)\vwframeo.obj"
	-@erase "$(INTDIR)\vwgcacho.obj"
	-@erase "$(INTDIR)\vwgeoma.obj"
	-@erase "$(INTDIR)\vwgeomob.obj"
	-@erase "$(INTDIR)\vwgeoms.obj"
	-@erase "$(INTDIR)\vwidatao.obj"
	-@erase "$(INTDIR)\vwmenu.obj"
	-@erase "$(INTDIR)\vwmm.idb"
	-@erase "$(INTDIR)\vwmm.obj"
	-@erase "$(INTDIR)\vwmm.res"
	-@erase "$(INTDIR)\vwmm60.pch"
	-@erase "$(INTDIR)\vwmmexo.obj"
	-@erase "$(INTDIR)\vwnavt2d.obj"
	-@erase "$(INTDIR)\vwnavt3d.obj"
	-@erase "$(INTDIR)\vwnt2d3d.obj"
	-@erase "$(INTDIR)\vwpktl.obj"
	-@erase "$(INTDIR)\vwrend2d.obj"
	-@erase "$(INTDIR)\vwrend3d.obj"
	-@erase "$(INTDIR)\vwrendrt.obj"
	-@erase "$(INTDIR)\vwuiobjs_p.obj"
	-@erase "$(OUTDIR)\vwmm.dll"
	-@erase "$(OUTDIR)\vwmm.exp"
	-@erase "$(OUTDIR)\vwmm.lib"
	-@erase "$(OUTDIR)\vwmm.pdb"
	-@erase ".\ReleaseMinSize\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 /Os 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\vwmm.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwmm60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib ddraw.lib d3drm.lib uuid.lib ftkvc40.lib rpcndr.lib rpcns4.lib rpcrt4.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\vwmm.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT" /nodefaultlib:"LIBC" /def:".\vwmm.def" /out:"$(OUTDIR)\vwmm.dll" /implib:"$(OUTDIR)\vwmm.lib" /libpath:"\mssdk\lib" /libpath:"..\tools\3dstod3d\3dsftk\lib" 
DEF_FILE= \
	".\vwmm.def"
LINK32_OBJS= \
	"$(INTDIR)\cathelp.obj" \
	"$(INTDIR)\dlldatax.obj" \
	"$(INTDIR)\mmhelp.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\vwmm.obj" \
	"$(INTDIR)\vwuiobjs_p.obj" \
	"$(INTDIR)\vwmenu.obj" \
	"$(INTDIR)\vwpktl.obj" \
	"$(INTDIR)\vwnavt2d.obj" \
	"$(INTDIR)\vwnavt3d.obj" \
	"$(INTDIR)\vwnt2d3d.obj" \
	"$(INTDIR)\edgecyc.obj" \
	"$(INTDIR)\vecmath.obj" \
	"$(INTDIR)\vectorob.obj" \
	"$(INTDIR)\vwgeomob.obj" \
	"$(INTDIR)\d3dmath.obj" \
	"$(INTDIR)\vwgeoms.obj" \
	"$(INTDIR)\vwidatao.obj" \
	"$(INTDIR)\charobj.obj" \
	"$(INTDIR)\jointobj.obj" \
	"$(INTDIR)\PuppetOb.obj" \
	"$(INTDIR)\vwgeoma.obj" \
	"$(INTDIR)\BBox.obj" \
	"$(INTDIR)\vwframeo.obj" \
	"$(INTDIR)\vwadatao.obj" \
	"$(INTDIR)\PickData.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\readwdb.obj" \
	"$(INTDIR)\vwgcacho.obj" \
	"$(INTDIR)\vwrend2d.obj" \
	"$(INTDIR)\vwrend3d.obj" \
	"$(INTDIR)\vwrendrt.obj" \
	"$(INTDIR)\spritemo.obj" \
	"$(INTDIR)\conv3ds.obj" \
	"$(INTDIR)\mmartfct.obj" \
	"$(INTDIR)\mmavatar.obj" \
	"$(INTDIR)\mmhost.obj" \
	"$(INTDIR)\mmlight.obj" \
	"$(INTDIR)\mmportal.obj" \
	"$(INTDIR)\mmroom.obj" \
	"$(INTDIR)\mmthing.obj" \
	"$(INTDIR)\Profhelp.obj" \
	"$(INTDIR)\vwmmexo.obj" \
	"$(INTDIR)\vwmm.res"

"$(OUTDIR)\vwmm.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\vwmm.dll
InputPath=.\ReleaseMinSize\vwmm.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("vwmm60.dep")
!INCLUDE "vwmm60.dep"
!ELSE 
!MESSAGE Warning: cannot find "vwmm60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "vwmm - Win32 Debug" || "$(CFG)" == "vwmm - Win32 Release MinSize"
SOURCE=..\vwcommon\cathelp.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\dlldatax.c

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\dlldatax.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\dlldatax.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\mmhelp.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vector" /I "..\property\vwadata" /I "..\property\sound" /I "..\property\cellprop" /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\vwidata" /I "..\property\vwgeomar" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\mmhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwidata.h" "..\vwcommon\cellprop.h" "..\vwcommon\vector.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\vwgcache.h" "..\vwcommon\spritemn.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vector" /I "..\property\vwadata" /I "..\property\sound" /I "..\property\cellprop" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\vwidata" /I "..\property\vwgeomar" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\mmhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwidata.h" "..\vwcommon\cellprop.h" "..\vwcommon\vector.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\vwgcache.h" "..\vwcommon\spritemn.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwmm60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwmm60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\syshelp.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\vwmm.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\avprof" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwmm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" ".\vwmm.h" ".\vwmm_i.c" "..\vwcommon\cellprop_i.c" "..\vwcommon\vector_i.c" "..\vwcommon\vwgeom_i.c" "..\vwcommon\vwgeomsp_i.c" "..\vwcommon\vwidata_i.c" "..\vwcommon\vwtrans_i.c" "..\vwcommon\vwgeomar_i.c" "..\vwcommon\vwadata_i.c" "..\vwcommon\Pickdata_i.c" "..\vwcommon\isound_i.c" "..\vwcommon\vwframe_i.c" "..\vwcommon\vwgcache_i.c" "..\vwcommon\vwrender_i.c" "..\vwcommon\convgeom_i.c" "..\vwcommon\3dstod3d_i.c" "..\vwcommon\vwnvtool_i.c" "..\vwcommon\vwpktool_i.c" "..\vwcommon\spritemn_i.c" "..\vwcommon\vwmmex_i.c" "..\vwcommon\vector.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwadata.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwgeomar.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\convgeom.h" "..\vwcommon\3dstod3d.h" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwmmex.h" "..\vwcommon\vwrender.h" "..\vwcommon\spritemn.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwpktool.h" "..\vwcommon\vwframe.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\Sound" /I "..\property\vwframe" /I "..\vwgcache" /I "..\vwrender" /I "..\uitools\vwpktool" /I "..\property\cellprop" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vwidata" /I "..\property\vwadata" /I "..\property\vwgeomar" /I "..\property\vector" /I "..\uitools\vwnvtool" /I "..\vwsystem" /I "..\spritemn" /I "..\tools\3dstod3d" /I "..\module\mm" /I "..\property\PickData" /I "..\property\sound" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwmm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" ".\vwmm.h" ".\vwmm_i.c" "..\vwcommon\cellprop_i.c" "..\vwcommon\vector_i.c" "..\vwcommon\vwgeom_i.c" "..\vwcommon\vwgeomsp_i.c" "..\vwcommon\vwidata_i.c" "..\vwcommon\vwtrans_i.c" "..\vwcommon\vwgeomar_i.c" "..\vwcommon\vwadata_i.c" "..\vwcommon\Pickdata_i.c" "..\vwcommon\isound_i.c" "..\vwcommon\vwframe_i.c" "..\vwcommon\vwgcache_i.c" "..\vwcommon\vwrender_i.c" "..\vwcommon\convgeom_i.c" "..\vwcommon\3dstod3d_i.c" "..\vwcommon\vwnvtool_i.c" "..\vwcommon\vwpktool_i.c" "..\vwcommon\spritemn_i.c" "..\vwcommon\vwmmex_i.c" "..\vwcommon\vector.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwadata.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwgeomar.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\convgeom.h" "..\vwcommon\3dstod3d.h" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwmmex.h" "..\vwcommon\vwrender.h" "..\vwcommon\spritemn.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwpktool.h" "..\vwcommon\vwframe.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\vwmm.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=.\vwmm.idl
USERDEP__VWMM_="..\vwcommon\cellprop.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwgeom.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwframe.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwnvtool.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwgeomar.idl"	"..\vwcommon\vwadata.idl"	"..\vwcommon\pickdata.idl"	"..\vwcommon\vwgeomsp.idl"	"..\vwcommon\vwmmex.idl"	"..\vwcommon\vwtrans.idl"	

".\vwmm.h"	".\vwmm_i.c"	".\vwmm.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWMM_)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwmm.h" /iid "vwmm_i.c" /I "..\vwcommon" "vwmm.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=.\vwmm.idl
USERDEP__VWMM_="..\vwcommon\cellprop.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwgeom.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwframe.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwnvtool.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwgeomar.idl"	"..\vwcommon\vwadata.idl"	"..\vwcommon\pickdata.idl"	"..\vwcommon\vwgeomsp.idl"	"..\vwcommon\vwmmex.idl"	"..\vwcommon\vwtrans.idl"	

".\vwmm.h"	".\vwmm_i.c"	".\vwmm.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWMM_)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwmm.h" /iid "vwmm_i.c" /I "..\vwcommon" "vwmm.idl"
<< 
	

!ENDIF 

SOURCE=.\vwmm.rc

"$(INTDIR)\vwmm.res" : $(SOURCE) "$(INTDIR)" ".\vwmm.tlb"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwuiobjs_p.c

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwuiobjs_p.obj" : $(SOURCE) "$(INTDIR)" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwuiobjs_p.obj" : $(SOURCE) "$(INTDIR)" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwmenu.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwmenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwmenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\uitools\vwpktool\vwpktl.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwpktl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwpktool.h" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwpktl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwpktool.h" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwpktool.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwpktool.idl
USERDEP__VWPKT="..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwrender.idl"	

"..\vwcommon\vwpktool.h"	"..\vwcommon\vwpktool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWPKT)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwpktool.h" /iid "vwpktool_i.c" /I "..\vwcommon" /out                                                                                    "..\vwcommon" "vwpktool.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwpktool.idl
USERDEP__VWPKT="..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwrender.idl"	

"..\vwcommon\vwpktool.h"	"..\vwcommon\vwpktool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWPKT)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwpktool.h" /iid "vwpktool_i.c" /I "..\vwcommon" /out                                                                                    "..\vwcommon" "vwpktool.idl"
<< 
	

!ENDIF 

SOURCE=..\uitools\vwnvtool\vwnavt2d.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwnavt2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwpktool.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\uitools\vwnvtool\vwnavt3d.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwnavt3d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwpktool.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwuiobjs_i.c" "..\vwcommon\vwuiobjs.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwnavt3d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwpktool.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwuiobjs_i.c" "..\vwcommon\vwuiobjs.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\uitools\vwnvtool\vwnt2d3d.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwnt2d3d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwpktool.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwnvtool.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwnvtool.idl
USERDEP__VWNVT="..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwrender.idl"	

"..\vwcommon\vwnvtool.h"	"..\vwcommon\vwnvtool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWNVT)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwnvtool.h" /iid "vwnvtool_i.c" /I "..\vwcommon" /out                                                                                    "..\vwcommon" "vwnvtool.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwnvtool.idl
USERDEP__VWNVT="..\vwcommon\vwuiobjs.idl"	"..\vwcommon\vwpktool.idl"	"..\vwcommon\vwrender.idl"	

"..\vwcommon\vwnvtool.h"	"..\vwcommon\vwnvtool_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWNVT)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwnvtool.h" /iid "vwnvtool_i.c" /I "..\vwcommon" /out                                                                                    "..\vwcommon" "vwnvtool.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\vwuiobjs.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwuiobjs.idl
USERDEP__VWUIO="..\vwcommon\vwobject.idl"	

"..\vwcommon\vwuiobjs.h"	"..\vwcommon\vwuiobjs_i.c"	"..\vwcommon\vwuiobjs_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWUIO)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwuiobjs.h" /iid "vwuiobjs_i.c" /I "..\vwcommon" /out                                                                             "..\vwcommon" "vwuiobjs.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwuiobjs.idl
USERDEP__VWUIO="..\vwcommon\vwobject.idl"	

"..\vwcommon\vwuiobjs.h"	"..\vwcommon\vwuiobjs_i.c"	"..\vwcommon\vwuiobjs_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWUIO)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwuiobjs.h" /iid "vwuiobjs_i.c" /I "..\vwcommon" /out                                                                             "..\vwcommon" "vwuiobjs.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\cellprop.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\cellprop.idl
USERDEP__CELLP="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwtrans.idl"	

"..\vwcommon\cellprop.h"	"..\vwcommon\cellprop_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__CELLP)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "cellprop.h" /iid "cellprop_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "cellprop.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\cellprop.idl
USERDEP__CELLP="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\vwidata.idl"	"..\vwcommon\vwtrans.idl"	

"..\vwcommon\cellprop.h"	"..\vwcommon\cellprop_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__CELLP)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "cellprop.h" /iid "cellprop_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "cellprop.idl"
<< 
	

!ENDIF 

SOURCE=..\property\cellprop\edgecyc.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwidata" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\edgecyc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwidata.h" "..\vwcommon\vwmmex.h" "..\vwcommon\cellprop.h" "..\vwcommon\vector.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwidata" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\edgecyc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwidata.h" "..\vwcommon\vwmmex.h" "..\vwcommon\cellprop.h" "..\vwcommon\vector.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vecmath.cpp

"$(INTDIR)\vecmath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwtrans.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwtrans.idl
USERDEP__VWTRA="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	

"..\vwcommon\vwtrans.h"	"..\vwcommon\vwtrans_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWTRA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwtrans.h" /iid "vwtrans_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwtrans.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwtrans.idl
USERDEP__VWTRA="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	

"..\vwcommon\vwtrans.h"	"..\vwcommon\vwtrans_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWTRA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwtrans.h" /iid "vwtrans_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwtrans.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\vector.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vector.idl
USERDEP__VECTO="..\vwcommon\vwobject.idl"	

"..\vwcommon\vector.h"	"..\vwcommon\vector_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VECTO)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vector.h" /iid "vector_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vector.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vector.idl
USERDEP__VECTO="..\vwcommon\vwobject.idl"	

"..\vwcommon\vector.h"	"..\vwcommon\vector_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VECTO)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vector.h" /iid "vector_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vector.idl"
<< 
	

!ENDIF 

SOURCE=..\property\vector\vectorob.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vectorob.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vectorob.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwgeom.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwgeom.idl
USERDEP__VWGEO="..\vwcommon\vwobject.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwframe.idl"	

"..\vwcommon\vwgeom.h"	"..\vwcommon\vwgeom_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWGEO)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwgeom.h" /iid "vwgeom_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeom.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwgeom.idl
USERDEP__VWGEO="..\vwcommon\vwobject.idl"	"..\vwcommon\vwgcache.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vwframe.idl"	

"..\vwcommon\vwgeom.h"	"..\vwcommon\vwgeom_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWGEO)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwgeom.h" /iid "vwgeom_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeom.idl"
<< 
	

!ENDIF 

SOURCE=..\property\vwgeom\vwgeomob.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwgeomob.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwgeomob.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\property\vwgeomsp\d3dmath.cpp

"$(INTDIR)\d3dmath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\property\vwgeomsp\vwgeoms.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\d2d" /I "..\spritemn" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwgeoms.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwadata.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwrender.h" "..\vwcommon\spritemn.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\d2d" /I "..\spritemn" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwgeoms.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwadata.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwrender.h" "..\vwcommon\spritemn.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwgeomsp.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwgeomsp.idl
USERDEP__VWGEOM="..\vwcommon\vwgeom.idl"	

"..\vwcommon\vwgeomsp.h"	"..\vwcommon\vwgeomsp_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWGEOM)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwgeomsp.h" /iid "vwgeomsp_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeomsp.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwgeomsp.idl
USERDEP__VWGEOM="..\vwcommon\vwgeom.idl"	

"..\vwcommon\vwgeomsp.h"	"..\vwcommon\vwgeomsp_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWGEOM)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwgeomsp.h" /iid "vwgeomsp_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeomsp.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\vwidata.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwidata.idl
USERDEP__VWIDA="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\cellprop.idl"	

"..\vwcommon\vwidata.h"	"..\vwcommon\vwidata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWIDA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn  /h "vwidata.h" /iid "vwidata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwidata.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwidata.idl
USERDEP__VWIDA="..\vwcommon\vwobject.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\cellprop.idl"	

"..\vwcommon\vwidata.h"	"..\vwcommon\vwidata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWIDA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn  /h "vwidata.h" /iid "vwidata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwidata.idl"
<< 
	

!ENDIF 

SOURCE=..\property\vwidata\vwidatao.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwidatao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwidata.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwidatao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwidata.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\property\vwgeomar\charobj.cpp

"$(INTDIR)\charobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwgeomar.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\property\vwgeomar\jointobj.cpp

"$(INTDIR)\jointobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwgeomar.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\property\vwgeomar\PuppetOb.cpp

"$(INTDIR)\PuppetOb.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwgeomar.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\property\vwgeomar\vwgeoma.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwgeoma.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwgeomar.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwgeoma.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwgeomar.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwrender.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE="..\vwcommon\vwgeomar.idl"

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath="..\vwcommon\vwgeomar.idl"
USERDEP__VWGEOMA="..\vwcommon\vwgeom.idl"	

"..\vwcommon\vwgeomar.h"	"..\vwcommon\vwgeomar_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWGEOMA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwgeomar.h" /iid "vwgeomar_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeomar.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath="..\vwcommon\vwgeomar.idl"
USERDEP__VWGEOMA="..\vwcommon\vwgeom.idl"	

"..\vwcommon\vwgeomar.h"	"..\vwcommon\vwgeomar_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWGEOMA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwgeomar.h" /iid "vwgeomar_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgeomar.idl"
<< 
	

!ENDIF 

SOURCE=..\property\vwframe\BBox.cpp

"$(INTDIR)\BBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwframe.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwframe.idl
USERDEP__VWFRA="..\vwcommon\vwtrans.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\cellprop.idl"	

"..\vwcommon\vwframe.h"	"..\vwcommon\vwframe_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWFRA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwframe.h" /iid "vwframe_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwframe.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwframe.idl
USERDEP__VWFRA="..\vwcommon\vwtrans.idl"	"..\vwcommon\vwrender.idl"	"..\vwcommon\vector.idl"	"..\vwcommon\cellprop.idl"	

"..\vwcommon\vwframe.h"	"..\vwcommon\vwframe_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWFRA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwframe.h" /iid "vwframe_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwframe.idl"
<< 
	

!ENDIF 

SOURCE=..\property\vwframe\vwframeo.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwframeo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwframe.h" "..\vwcommon\vwidata.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vector.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwrender.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwframeo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwframe.h" "..\vwcommon\vwidata.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vector.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwrender.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwadata.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwadata.idl

"..\vwcommon\vwadata.h"	"..\vwcommon\vwadata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwadata.h" /iid "vwadata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwadata.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwadata.idl

"..\vwcommon\vwadata.h"	"..\vwcommon\vwadata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwadata.h" /iid "vwadata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwadata.idl"
<< 
	

!ENDIF 

SOURCE=..\property\vwadata\vwadatao.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwadatao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwadata.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwadatao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwadata.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\property\PickData\PickData.cpp

"$(INTDIR)\PickData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\Pickdata.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\PickData.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\PickData.idl

"..\vwcommon\Pickdata.h"	"..\vwcommon\Pickdata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "PickData.h" /iid "Pickdata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "Pickdata.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\PickData.idl

"..\vwcommon\Pickdata.h"	"..\vwcommon\Pickdata_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "PickData.h" /iid "Pickdata_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "Pickdata.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\isound.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\isound.idl

"..\vwcommon\isound.h"	"..\vwcommon\isound_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "isound.h" /iid "isound_i.c" /I "..\vwcommon" /out         "..\vwcommon" "isound.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\isound.idl

"..\vwcommon\isound.h"	"..\vwcommon\isound_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "isound.h" /iid "isound_i.c" /I "..\vwcommon" /out         "..\vwcommon" "isound.idl"
<< 
	

!ENDIF 

SOURCE=..\property\Sound\Sound.cpp

"$(INTDIR)\Sound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\isound.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwgcache\readwdb.cpp

"$(INTDIR)\readwdb.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h" "..\vwcommon\3dstod3d.h" "..\vwcommon\vwframe.h" "..\vwcommon\convgeom.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwgcache.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwgcache.idl
USERDEP__VWGCA="..\vwcommon\vwframe.idl"	

"..\vwcommon\vwgcache.h"	"..\vwcommon\vwgcache_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWGCA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwgcache.h" /iid "vwgcache_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgcache.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwgcache.idl
USERDEP__VWGCA="..\vwcommon\vwframe.idl"	

"..\vwcommon\vwgcache.h"	"..\vwcommon\vwgcache_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWGCA)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwgcache.h" /iid "vwgcache_i.c" /I "..\vwcommon" /out                                                                                               "..\vwcommon" "vwgcache.idl"
<< 
	

!ENDIF 

SOURCE=..\vwgcache\vwgcacho.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\d2d" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwgcacho.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h" "..\vwcommon\3dstod3d.h" "..\vwcommon\vwframe.h" "..\vwcommon\convgeom.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\d2d" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwgcacho.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h" "..\vwcommon\3dstod3d.h" "..\vwcommon\vwframe.h" "..\vwcommon\convgeom.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwrender\vwrend2d.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\d2d\d2dutils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwrend2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\d2d\d2dutils\include" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwrend2d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwrender\vwrend3d.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwrend3d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwrend3d.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwrender.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwrender.idl
USERDEP__VWREN="..\vwcommon\vwframe.idl"	"..\vwcommon\d2d.idl"	

"..\vwcommon\vwrender.h"	"..\vwcommon\vwrender_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWREN)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwrender.h" /iid "vwrender_i.c" /I "..\vwcommon" /out                                                                                     "..\vwcommon" "vwrender.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwrender.idl
USERDEP__VWREN="..\vwcommon\vwframe.idl"	"..\vwcommon\d2d.idl"	

"..\vwcommon\vwrender.h"	"..\vwcommon\vwrender_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWREN)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwrender.h" /iid "vwrender_i.c" /I "..\vwcommon" /out                                                                                     "..\vwcommon" "vwrender.idl"
<< 
	

!ENDIF 

SOURCE=..\vwrender\vwrendrt.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\d2d\d2dutils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwrendrt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwgcache.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\d2d\d2dutils\include" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwrendrt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwrender.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwgcache.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\spritemn.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\spritemn.idl

"..\vwcommon\spritemn.h"	"..\vwcommon\spritemn_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "spritemn.h" /iid "spritemn_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "spritemn.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\spritemn.idl

"..\vwcommon\spritemn.h"	"..\vwcommon\spritemn_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "spritemn.h" /iid "spritemn_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "spritemn.idl"
<< 
	

!ENDIF 

SOURCE=..\spritemn\spritemo.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\spritemo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\spritemn.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\spritemo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\spritemn.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\3dstod3d.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\3dstod3d.idl

"..\vwcommon\3dstod3d.h"	"..\vwcommon\3dstod3d_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "3dstod3d.h" /iid "3dstod3d_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "3dstod3d.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\3dstod3d.idl

"..\vwcommon\3dstod3d.h"	"..\vwcommon\3dstod3d_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "3dstod3d.h" /iid "3dstod3d_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "3dstod3d.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\3dstod3d\conv3ds.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\vwsystem" /I "..\tools\3dstod3d\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\conv3ds.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\3dstod3d.h" "..\vwcommon\convgeom.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\vwsystem" /I "..\tools\3dstod3d\include" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\conv3ds.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\3dstod3d.h" "..\vwcommon\convgeom.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\convgeom.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\convgeom.idl

"..\vwcommon\convgeom.h"	"..\vwcommon\convgeom_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "convgeom.h" /iid "convgeom_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "convgeom.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\convgeom.idl

"..\vwcommon\convgeom.h"	"..\vwcommon\convgeom_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "convgeom.h" /iid "convgeom_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "convgeom.idl"
<< 
	

!ENDIF 

SOURCE=..\module\mm\mmartfct.cpp

"$(INTDIR)\mmartfct.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\mm\mmavatar.cpp

"$(INTDIR)\mmavatar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwidata.h" "..\vwcommon\spritemn.h" "..\vwcommon\vector.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\mm\mmhost.cpp

"$(INTDIR)\mmhost.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\mm\mmlight.cpp

"$(INTDIR)\mmlight.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\mm\mmportal.cpp

"$(INTDIR)\mmportal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\mm\mmroom.cpp

"$(INTDIR)\mmroom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwrender.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\mm\mmthing.cpp

"$(INTDIR)\mmthing.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwidata.h" "..\vwcommon\vwnvtool.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\cellprop.h" "..\vwcommon\vwuiobjs.h" "..\vwcommon\vwpktool.h" "..\vwcommon\vwframe.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vector.h" "..\vwcommon\vwrender.h" "..\vwcommon\vwgcache.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Vwcommon\Profhelp.cpp

"$(INTDIR)\Profhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwmmex.idl

!IF  "$(CFG)" == "vwmm - Win32 Debug"

InputPath=..\vwcommon\vwmmex.idl

"..\vwcommon\vwmmex.h"	"..\vwcommon\vwmmex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwmmex.h" /iid "vwmmex_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "vwmmex.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

InputPath=..\vwcommon\vwmmex.idl

"..\vwcommon\vwmmex.h"	"..\vwcommon\vwmmex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwmmex.h" /iid "vwmmex_i.c" /I "..\vwcommon" /out                            "..\vwcommon" "vwmmex.idl"
<< 
	

!ENDIF 

SOURCE=..\module\mm\vwmmexo.cpp

!IF  "$(CFG)" == "vwmm - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vector" /I "..\property\vwadata" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwmmexo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vector.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwrender.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwmm - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "\mssdk\include" /I "..\vwobject" /I "..\property\vwgeom" /I "..\property\vwgeomsp" /I "..\property\vector" /I "..\property\vwadata" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWMM_BUILD" /D "NO_VRML_CONVERTER" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwmm60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwmm.pdb" /FD /c 

"$(INTDIR)\vwmmexo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwmm60.pch" "..\vwcommon\vwmmex.h" "..\vwcommon\vwgeom.h" "..\vwcommon\vwgeomsp.h" "..\vwcommon\vwadata.h" "..\vwcommon\Pickdata.h" "..\vwcommon\isound.h" "..\vwcommon\vwframe.h" "..\vwcommon\cellprop.h" "..\vwcommon\vector.h" "..\vwcommon\vwgcache.h" "..\vwcommon\vwtrans.h" "..\vwcommon\vwrender.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

