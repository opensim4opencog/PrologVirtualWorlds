# Microsoft Developer Studio Generated NMAKE File, Based on objexp60.dsp
!IF "$(CFG)" == ""
CFG=objexplr - Win32 Debug
!MESSAGE No configuration specified. Defaulting to objexplr - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "objexplr - Win32 Release" && "$(CFG)" != "objexplr - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "objexp60.mak" CFG="objexplr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "objexplr - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "objexplr - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "objexplr - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\vwobjexp.ocx" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\3clmlist.obj"
	-@erase "$(INTDIR)\4ClmList.obj"
	-@erase "$(INTDIR)\booldrop.obj"
	-@erase "$(INTDIR)\cmddel.obj"
	-@erase "$(INTDIR)\EdLstCtl.obj"
	-@erase "$(INTDIR)\LEButton.obj"
	-@erase "$(INTDIR)\ListList.obj"
	-@erase "$(INTDIR)\lpedit.obj"
	-@erase "$(INTDIR)\MenuList.obj"
	-@erase "$(INTDIR)\methdrop.obj"
	-@erase "$(INTDIR)\methlist.obj"
	-@erase "$(INTDIR)\MExistDl.obj"
	-@erase "$(INTDIR)\MListCtl.obj"
	-@erase "$(INTDIR)\moddrop.obj"
	-@erase "$(INTDIR)\objexp60.pch"
	-@erase "$(INTDIR)\objexplr.obj"
	-@erase "$(INTDIR)\objexplr.res"
	-@erase "$(INTDIR)\objexplr.tlb"
	-@erase "$(INTDIR)\PExistDl.obj"
	-@erase "$(INTDIR)\PListCtl.obj"
	-@erase "$(INTDIR)\PMapCtl.obj"
	-@erase "$(INTDIR)\PrpEdCtl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\ThingCtl.obj"
	-@erase "$(INTDIR)\tplstctl.obj"
	-@erase "$(INTDIR)\Tree.obj"
	-@erase "$(INTDIR)\tvlstctl.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\viewctl.obj"
	-@erase "$(INTDIR)\vwstudio_i.obj"
	-@erase "$(INTDIR)\vwuiview.obj"
	-@erase "$(OUTDIR)\vwobjexp.exp"
	-@erase "$(OUTDIR)\vwobjexp.lib"
	-@erase "$(OUTDIR)\vwobjexp.ocx"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /I "..\..\vwstudio" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\objexp60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\objexplr.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\objexp60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=uuid.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\vwobjexp.pdb" /machine:I386 /def:".\objexplr.def" /out:"$(OUTDIR)\vwobjexp.ocx" /implib:"$(OUTDIR)\vwobjexp.lib" 
DEF_FILE= \
	".\objexplr.def"
LINK32_OBJS= \
	"$(INTDIR)\3clmlist.obj" \
	"$(INTDIR)\4ClmList.obj" \
	"$(INTDIR)\booldrop.obj" \
	"$(INTDIR)\cmddel.obj" \
	"$(INTDIR)\EdLstCtl.obj" \
	"$(INTDIR)\LEButton.obj" \
	"$(INTDIR)\ListList.obj" \
	"$(INTDIR)\lpedit.obj" \
	"$(INTDIR)\MenuList.obj" \
	"$(INTDIR)\methdrop.obj" \
	"$(INTDIR)\methlist.obj" \
	"$(INTDIR)\MExistDl.obj" \
	"$(INTDIR)\MListCtl.obj" \
	"$(INTDIR)\moddrop.obj" \
	"$(INTDIR)\objexplr.obj" \
	"$(INTDIR)\PExistDl.obj" \
	"$(INTDIR)\PListCtl.obj" \
	"$(INTDIR)\PMapCtl.obj" \
	"$(INTDIR)\PrpEdCtl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\ThingCtl.obj" \
	"$(INTDIR)\tplstctl.obj" \
	"$(INTDIR)\Tree.obj" \
	"$(INTDIR)\tvlstctl.obj" \
	"$(INTDIR)\viewctl.obj" \
	"$(INTDIR)\vwstudio_i.obj" \
	"$(INTDIR)\vwuiview.obj" \
	"$(INTDIR)\objexplr.res"

"$(OUTDIR)\vwobjexp.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\vwobjexp.ocx
InputPath=.\Release\vwobjexp.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\vwobjexp.ocx" "$(OUTDIR)\objexp60.bsc" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\3clmlist.obj"
	-@erase "$(INTDIR)\3clmlist.sbr"
	-@erase "$(INTDIR)\4ClmList.obj"
	-@erase "$(INTDIR)\4ClmList.sbr"
	-@erase "$(INTDIR)\booldrop.obj"
	-@erase "$(INTDIR)\booldrop.sbr"
	-@erase "$(INTDIR)\cmddel.obj"
	-@erase "$(INTDIR)\cmddel.sbr"
	-@erase "$(INTDIR)\EdLstCtl.obj"
	-@erase "$(INTDIR)\EdLstCtl.sbr"
	-@erase "$(INTDIR)\LEButton.obj"
	-@erase "$(INTDIR)\LEButton.sbr"
	-@erase "$(INTDIR)\ListList.obj"
	-@erase "$(INTDIR)\ListList.sbr"
	-@erase "$(INTDIR)\lpedit.obj"
	-@erase "$(INTDIR)\lpedit.sbr"
	-@erase "$(INTDIR)\MenuList.obj"
	-@erase "$(INTDIR)\MenuList.sbr"
	-@erase "$(INTDIR)\methdrop.obj"
	-@erase "$(INTDIR)\methdrop.sbr"
	-@erase "$(INTDIR)\methlist.obj"
	-@erase "$(INTDIR)\methlist.sbr"
	-@erase "$(INTDIR)\MExistDl.obj"
	-@erase "$(INTDIR)\MExistDl.sbr"
	-@erase "$(INTDIR)\MListCtl.obj"
	-@erase "$(INTDIR)\MListCtl.sbr"
	-@erase "$(INTDIR)\moddrop.obj"
	-@erase "$(INTDIR)\moddrop.sbr"
	-@erase "$(INTDIR)\objexp60.pch"
	-@erase "$(INTDIR)\objexplr.obj"
	-@erase "$(INTDIR)\objexplr.res"
	-@erase "$(INTDIR)\objexplr.sbr"
	-@erase "$(INTDIR)\objexplr.tlb"
	-@erase "$(INTDIR)\PExistDl.obj"
	-@erase "$(INTDIR)\PExistDl.sbr"
	-@erase "$(INTDIR)\PListCtl.obj"
	-@erase "$(INTDIR)\PListCtl.sbr"
	-@erase "$(INTDIR)\PMapCtl.obj"
	-@erase "$(INTDIR)\PMapCtl.sbr"
	-@erase "$(INTDIR)\PrpEdCtl.obj"
	-@erase "$(INTDIR)\PrpEdCtl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\syshelp.sbr"
	-@erase "$(INTDIR)\ThingCtl.obj"
	-@erase "$(INTDIR)\ThingCtl.sbr"
	-@erase "$(INTDIR)\tplstctl.obj"
	-@erase "$(INTDIR)\tplstctl.sbr"
	-@erase "$(INTDIR)\Tree.obj"
	-@erase "$(INTDIR)\Tree.sbr"
	-@erase "$(INTDIR)\tvlstctl.obj"
	-@erase "$(INTDIR)\tvlstctl.sbr"
	-@erase "$(INTDIR)\viewctl.obj"
	-@erase "$(INTDIR)\viewctl.sbr"
	-@erase "$(INTDIR)\vwobjexp.idb"
	-@erase "$(INTDIR)\vwstudio_i.obj"
	-@erase "$(INTDIR)\vwstudio_i.sbr"
	-@erase "$(INTDIR)\vwuiview.obj"
	-@erase "$(INTDIR)\vwuiview.sbr"
	-@erase "$(OUTDIR)\objexp60.bsc"
	-@erase "$(OUTDIR)\vwobjexp.exp"
	-@erase "$(OUTDIR)\vwobjexp.ilk"
	-@erase "$(OUTDIR)\vwobjexp.lib"
	-@erase "$(OUTDIR)\vwobjexp.ocx"
	-@erase "$(OUTDIR)\vwobjexp.pdb"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\vwstudio" /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /D "_DEBUG" /D "USE_CONTROLMANAGER" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\objexp60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwobjexp.pdb" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\objexplr.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\objexp60.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\3clmlist.sbr" \
	"$(INTDIR)\4ClmList.sbr" \
	"$(INTDIR)\booldrop.sbr" \
	"$(INTDIR)\cmddel.sbr" \
	"$(INTDIR)\EdLstCtl.sbr" \
	"$(INTDIR)\LEButton.sbr" \
	"$(INTDIR)\ListList.sbr" \
	"$(INTDIR)\lpedit.sbr" \
	"$(INTDIR)\MenuList.sbr" \
	"$(INTDIR)\methdrop.sbr" \
	"$(INTDIR)\methlist.sbr" \
	"$(INTDIR)\MExistDl.sbr" \
	"$(INTDIR)\MListCtl.sbr" \
	"$(INTDIR)\moddrop.sbr" \
	"$(INTDIR)\objexplr.sbr" \
	"$(INTDIR)\PExistDl.sbr" \
	"$(INTDIR)\PListCtl.sbr" \
	"$(INTDIR)\PMapCtl.sbr" \
	"$(INTDIR)\PrpEdCtl.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\syshelp.sbr" \
	"$(INTDIR)\ThingCtl.sbr" \
	"$(INTDIR)\tplstctl.sbr" \
	"$(INTDIR)\Tree.sbr" \
	"$(INTDIR)\tvlstctl.sbr" \
	"$(INTDIR)\viewctl.sbr" \
	"$(INTDIR)\vwstudio_i.sbr" \
	"$(INTDIR)\vwuiview.sbr"

"$(OUTDIR)\objexp60.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=uuid.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\vwobjexp.pdb" /debug /machine:I386 /def:".\objexplr.def" /out:"$(OUTDIR)\vwobjexp.ocx" /implib:"$(OUTDIR)\vwobjexp.lib" 
DEF_FILE= \
	".\objexplr.def"
LINK32_OBJS= \
	"$(INTDIR)\3clmlist.obj" \
	"$(INTDIR)\4ClmList.obj" \
	"$(INTDIR)\booldrop.obj" \
	"$(INTDIR)\cmddel.obj" \
	"$(INTDIR)\EdLstCtl.obj" \
	"$(INTDIR)\LEButton.obj" \
	"$(INTDIR)\ListList.obj" \
	"$(INTDIR)\lpedit.obj" \
	"$(INTDIR)\MenuList.obj" \
	"$(INTDIR)\methdrop.obj" \
	"$(INTDIR)\methlist.obj" \
	"$(INTDIR)\MExistDl.obj" \
	"$(INTDIR)\MListCtl.obj" \
	"$(INTDIR)\moddrop.obj" \
	"$(INTDIR)\objexplr.obj" \
	"$(INTDIR)\PExistDl.obj" \
	"$(INTDIR)\PListCtl.obj" \
	"$(INTDIR)\PMapCtl.obj" \
	"$(INTDIR)\PrpEdCtl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\ThingCtl.obj" \
	"$(INTDIR)\tplstctl.obj" \
	"$(INTDIR)\Tree.obj" \
	"$(INTDIR)\tvlstctl.obj" \
	"$(INTDIR)\viewctl.obj" \
	"$(INTDIR)\vwstudio_i.obj" \
	"$(INTDIR)\vwuiview.obj" \
	"$(INTDIR)\objexplr.res"

"$(OUTDIR)\vwobjexp.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\vwobjexp.ocx
InputPath=.\Debug\vwobjexp.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("objexp60.dep")
!INCLUDE "objexp60.dep"
!ELSE 
!MESSAGE Warning: cannot find "objexp60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "objexplr - Win32 Release" || "$(CFG)" == "objexplr - Win32 Debug"
SOURCE=.\3clmlist.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\3clmlist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\3clmlist.obj"	"$(INTDIR)\3clmlist.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\4ClmList.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\4ClmList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\4ClmList.obj"	"$(INTDIR)\4ClmList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\booldrop.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\booldrop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\booldrop.obj"	"$(INTDIR)\booldrop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=..\..\vwcommon\cmddel.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /I "..\..\vwstudio" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\cmddel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\vwstudio" /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /D "_DEBUG" /D "USE_CONTROLMANAGER" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwobjexp.pdb" /FD /c 

"$(INTDIR)\cmddel.obj"	"$(INTDIR)\cmddel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\EdLstCtl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\EdLstCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\EdLstCtl.obj"	"$(INTDIR)\EdLstCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\LEButton.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\LEButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\LEButton.obj"	"$(INTDIR)\LEButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\ListList.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\ListList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\ListList.obj"	"$(INTDIR)\ListList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\lpedit.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\lpedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\lpedit.obj"	"$(INTDIR)\lpedit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\MenuList.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\MenuList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\MenuList.obj"	"$(INTDIR)\MenuList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\methdrop.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\methdrop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\methdrop.obj"	"$(INTDIR)\methdrop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\methlist.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\methlist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\methlist.obj"	"$(INTDIR)\methlist.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\MExistDl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\MExistDl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\MExistDl.obj"	"$(INTDIR)\MExistDl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\MListCtl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\MListCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\MListCtl.obj"	"$(INTDIR)\MListCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\moddrop.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\moddrop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\moddrop.obj"	"$(INTDIR)\moddrop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\objexplr.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\objexplr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\objexplr.obj"	"$(INTDIR)\objexplr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\objexplr.odl

!IF  "$(CFG)" == "objexplr - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\objexplr.tlb" /mktyplib203 /o "NUL" /win32 

"$(OUTDIR)\objexplr.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\objexplr.tlb" /mktyplib203 /o "NUL" /win32 

"$(OUTDIR)\objexplr.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\objexplr.rc

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\objexplr.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexplr.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\objexplr.res" /i "Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\objexplr.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexplr.tlb"
	$(RSC) /l 0x409 /fo"$(INTDIR)\objexplr.res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\PExistDl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\PExistDl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\PExistDl.obj"	"$(INTDIR)\PExistDl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\PListCtl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\PListCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\PListCtl.obj"	"$(INTDIR)\PListCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\PMapCtl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\PMapCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\PMapCtl.obj"	"$(INTDIR)\PMapCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\PrpEdCtl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\PrpEdCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\PrpEdCtl.obj"	"$(INTDIR)\PrpEdCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /I "..\..\vwstudio" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fp"$(INTDIR)\objexp60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\objexp60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\vwstudio" /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /D "_DEBUG" /D "USE_CONTROLMANAGER" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\objexp60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwobjexp.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\objexp60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\vwcommon\syshelp.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\syshelp.obj"	"$(INTDIR)\syshelp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ThingCtl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\ThingCtl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\ThingCtl.obj"	"$(INTDIR)\ThingCtl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\tplstctl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\tplstctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\tplstctl.obj"	"$(INTDIR)\tplstctl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\Tree.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\Tree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\Tree.obj"	"$(INTDIR)\Tree.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=.\tvlstctl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\tvlstctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\tvlstctl.obj"	"$(INTDIR)\tvlstctl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"


!ENDIF 

SOURCE=..\vwview\viewctl.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"


"$(INTDIR)\viewctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"


"$(INTDIR)\viewctl.obj"	"$(INTDIR)\viewctl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\objexp60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\vwstudio\vwstudio_i.c

!IF  "$(CFG)" == "objexplr - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /I "..\..\vwstudio" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\vwstudio_i.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\vwstudio" /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /D "_DEBUG" /D "USE_CONTROLMANAGER" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwobjexp.pdb" /FD /c 

"$(INTDIR)\vwstudio_i.obj"	"$(INTDIR)\vwstudio_i.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\vwcommon\vwuiview.cpp

!IF  "$(CFG)" == "objexplr - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /I "..\..\vwstudio" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "USE_CONTROLMANAGER" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\vwuiview.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "objexplr - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\vwstudio" /I "..\vwview" /I "..\..\vwcommon" /I "..\..\vwsystem" /D "_DEBUG" /D "USE_CONTROLMANAGER" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwobjexp.pdb" /FD /c 

"$(INTDIR)\vwuiview.obj"	"$(INTDIR)\vwuiview.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

