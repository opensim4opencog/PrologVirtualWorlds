# Microsoft Developer Studio Generated NMAKE File, Based on uictrl60.dsp
!IF "$(CFG)" == ""
CFG=uictrl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to uictrl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "uictrl - Win32 Release" && "$(CFG)" != "uictrl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "uictrl60.mak" CFG="uictrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "uictrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "uictrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "uictrl - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\vwuictrl.ocx" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\AvListCt.obj"
	-@erase "$(INTDIR)\cutil.obj"
	-@erase "$(INTDIR)\ddutil.obj"
	-@erase "$(INTDIR)\Errmgr.obj"
	-@erase "$(INTDIR)\Mmutils.obj"
	-@erase "$(INTDIR)\reghelp.obj"
	-@erase "$(INTDIR)\scripts.obj"
	-@erase "$(INTDIR)\socxctl.obj"
	-@erase "$(INTDIR)\soundCtl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\uictrl.obj"
	-@erase "$(INTDIR)\uictrl.res"
	-@erase "$(INTDIR)\uictrl.tlb"
	-@erase "$(INTDIR)\uictrl60.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\viewctl.obj"
	-@erase "$(INTDIR)\vwmenu.obj"
	-@erase "$(INTDIR)\vwsfvwct.obj"
	-@erase "$(INTDIR)\whelpct.obj"
	-@erase "$(OUTDIR)\vwuictrl.exp"
	-@erase "$(OUTDIR)\vwuictrl.lib"
	-@erase "$(OUTDIR)\vwuictrl.ocx"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\uictrl60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\uictrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\uictrl60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=uuid.lib ddraw.lib dsound.lib d3drm.lib wininet.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\vwuictrl.pdb" /machine:I386 /def:".\uictrl.def" /out:"$(OUTDIR)\vwuictrl.ocx" /implib:"$(OUTDIR)\vwuictrl.lib" /libpath:"\dxsdk\sdk\lib" /libpath:"..\..\inetsdk\lib" /libpath:"\mssdk\lib" 
DEF_FILE= \
	".\uictrl.def"
LINK32_OBJS= \
	"$(INTDIR)\AvListCt.obj" \
	"$(INTDIR)\cutil.obj" \
	"$(INTDIR)\ddutil.obj" \
	"$(INTDIR)\reghelp.obj" \
	"$(INTDIR)\scripts.obj" \
	"$(INTDIR)\socxctl.obj" \
	"$(INTDIR)\soundCtl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\uictrl.obj" \
	"$(INTDIR)\viewctl.obj" \
	"$(INTDIR)\vwmenu.obj" \
	"$(INTDIR)\vwsfvwct.obj" \
	"$(INTDIR)\whelpct.obj" \
	"$(INTDIR)\Errmgr.obj" \
	"$(INTDIR)\Mmutils.obj" \
	"$(INTDIR)\uictrl.res"

"$(OUTDIR)\vwuictrl.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\vwuictrl.ocx
InputPath=.\Release\vwuictrl.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\vwuictrl.ocx" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\AvListCt.obj"
	-@erase "$(INTDIR)\cutil.obj"
	-@erase "$(INTDIR)\ddutil.obj"
	-@erase "$(INTDIR)\Errmgr.obj"
	-@erase "$(INTDIR)\Mmutils.obj"
	-@erase "$(INTDIR)\reghelp.obj"
	-@erase "$(INTDIR)\scripts.obj"
	-@erase "$(INTDIR)\socxctl.obj"
	-@erase "$(INTDIR)\soundCtl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\uictrl.obj"
	-@erase "$(INTDIR)\uictrl.res"
	-@erase "$(INTDIR)\uictrl.tlb"
	-@erase "$(INTDIR)\uictrl60.pch"
	-@erase "$(INTDIR)\viewctl.obj"
	-@erase "$(INTDIR)\vwmenu.obj"
	-@erase "$(INTDIR)\vwsfvwct.obj"
	-@erase "$(INTDIR)\vwuictrl.idb"
	-@erase "$(INTDIR)\whelpct.obj"
	-@erase "$(OUTDIR)\vwuictrl.exp"
	-@erase "$(OUTDIR)\vwuictrl.ilk"
	-@erase "$(OUTDIR)\vwuictrl.lib"
	-@erase "$(OUTDIR)\vwuictrl.ocx"
	-@erase "$(OUTDIR)\vwuictrl.pdb"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\uictrl60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwuictrl.pdb" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\uictrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\uictrl60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=uuid.lib ddraw.lib dsound.lib d3drm.lib wininet.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\vwuictrl.pdb" /debug /machine:I386 /def:".\uictrl.def" /out:"$(OUTDIR)\vwuictrl.ocx" /implib:"$(OUTDIR)\vwuictrl.lib" /libpath:"\mssdk\lib" 
DEF_FILE= \
	".\uictrl.def"
LINK32_OBJS= \
	"$(INTDIR)\AvListCt.obj" \
	"$(INTDIR)\cutil.obj" \
	"$(INTDIR)\ddutil.obj" \
	"$(INTDIR)\reghelp.obj" \
	"$(INTDIR)\scripts.obj" \
	"$(INTDIR)\socxctl.obj" \
	"$(INTDIR)\soundCtl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\uictrl.obj" \
	"$(INTDIR)\viewctl.obj" \
	"$(INTDIR)\vwmenu.obj" \
	"$(INTDIR)\vwsfvwct.obj" \
	"$(INTDIR)\whelpct.obj" \
	"$(INTDIR)\Errmgr.obj" \
	"$(INTDIR)\Mmutils.obj" \
	"$(INTDIR)\uictrl.res"

"$(OUTDIR)\vwuictrl.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\vwuictrl.ocx
InputPath=.\Debug\vwuictrl.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("uictrl60.dep")
!INCLUDE "uictrl60.dep"
!ELSE 
!MESSAGE Warning: cannot find "uictrl60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "uictrl - Win32 Release" || "$(CFG)" == "uictrl - Win32 Debug"
SOURCE=.\AvListCt.cpp

"$(INTDIR)\AvListCt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"


SOURCE=.\cutil.cpp

"$(INTDIR)\cutil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"


SOURCE=.\ddutil.cpp

!IF  "$(CFG)" == "uictrl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ddutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwuictrl.pdb" /FD /c 

"$(INTDIR)\ddutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\vwcommon\reghelp.cpp

"$(INTDIR)\reghelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\vwcommon\scripts.cpp

"$(INTDIR)\scripts.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\socxctl.cpp

"$(INTDIR)\socxctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"


SOURCE=.\soundCtl.cpp

"$(INTDIR)\soundCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "uictrl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\uictrl60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\uictrl60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\uictrl60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwuictrl.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\uictrl60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\vwcommon\syshelp.cpp

!IF  "$(CFG)" == "uictrl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\uictrl60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\uictrl60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwuictrl.pdb" /FD /c 

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\uictrl.cpp

"$(INTDIR)\uictrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"


SOURCE=.\uictrl.odl

!IF  "$(CFG)" == "uictrl - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\uictrl.tlb" /mktyplib203 /o "NUL" /win32 

"$(OUTDIR)\uictrl.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\uictrl.tlb" /mktyplib203 /o "NUL" /win32 

"$(OUTDIR)\uictrl.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\uictrl.rc

!IF  "$(CFG)" == "uictrl - Win32 Release"


"$(INTDIR)\uictrl.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\uictrl.res" /i "Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"


"$(INTDIR)\uictrl.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\uictrl.res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=..\vwview\viewctl.cpp

!IF  "$(CFG)" == "uictrl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\uictrl60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\viewctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fp"$(INTDIR)\uictrl60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwuictrl.pdb" /FD /c 

"$(INTDIR)\viewctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\vwcommon\vwmenu.cpp

!IF  "$(CFG)" == "uictrl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\vwmenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwuictrl.pdb" /FD /c 

"$(INTDIR)\vwmenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\vwsfvwct.cpp

"$(INTDIR)\vwsfvwct.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"


SOURCE=.\whelpct.cpp

"$(INTDIR)\whelpct.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\uictrl60.pch"


SOURCE=..\..\D2d\D2dutils\Src\Errmgr.cpp

!IF  "$(CFG)" == "uictrl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Errmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwuictrl.pdb" /FD /c 

"$(INTDIR)\Errmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\D2d\D2dutils\Src\Mmutils.cpp

!IF  "$(CFG)" == "uictrl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Mmutils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uictrl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\mssdk\include" /I "..\..\vwcommon" /I "..\..\d2d\d2dutils\include" /I "..\..\d2d\mmimage\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwuictrl.pdb" /FD /c 

"$(INTDIR)\Mmutils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

