# Microsoft Developer Studio Generated NMAKE File, Based on vwfoun60.dsp
!IF "$(CFG)" == ""
CFG=vwfound - Win32 Release
!MESSAGE No configuration specified. Defaulting to vwfound - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "vwfound - Win32 Release" && "$(CFG)" != "vwfound - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vwfoun60.mak" CFG="vwfound - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vwfound - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vwfound - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "vwfound - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\vwfound.dll" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\favatar.obj"
	-@erase "$(INTDIR)\fglobal.obj"
	-@erase "$(INTDIR)\fhost.obj"
	-@erase "$(INTDIR)\foundatn.obj"
	-@erase "$(INTDIR)\fthing.obj"
	-@erase "$(INTDIR)\mmhelp.obj"
	-@erase "$(INTDIR)\painting.obj"
	-@erase "$(INTDIR)\profhelp.obj"
	-@erase "$(INTDIR)\sign.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\tranito.obj"
	-@erase "$(INTDIR)\TransMch.obj"
	-@erase "$(INTDIR)\vwfoun60.pch"
	-@erase "$(INTDIR)\vwfound.idb"
	-@erase "$(INTDIR)\vwfound.obj"
	-@erase "$(INTDIR)\vwfound.res"
	-@erase "$(OUTDIR)\vwfound.dll"
	-@erase "$(OUTDIR)\vwfound.exp"
	-@erase "$(OUTDIR)\vwfound.lib"
	-@erase "$(OUTDIR)\vwfound.pdb"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O1 /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\vwfound.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwfoun60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=webpost.lib uuid.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\vwfound.pdb" /debug /machine:I386 /def:".\vwfound.def" /out:"$(OUTDIR)\vwfound.dll" /implib:"$(OUTDIR)\vwfound.lib" 
DEF_FILE= \
	".\vwfound.def"
LINK32_OBJS= \
	"$(INTDIR)\favatar.obj" \
	"$(INTDIR)\fglobal.obj" \
	"$(INTDIR)\fhost.obj" \
	"$(INTDIR)\foundatn.obj" \
	"$(INTDIR)\fthing.obj" \
	"$(INTDIR)\mmhelp.obj" \
	"$(INTDIR)\painting.obj" \
	"$(INTDIR)\profhelp.obj" \
	"$(INTDIR)\sign.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\TransMch.obj" \
	"$(INTDIR)\vwfound.obj" \
	"$(INTDIR)\tranito.obj" \
	"$(INTDIR)\vwfound.res"

"$(OUTDIR)\vwfound.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\vwfound.dll
InputPath=.\Release\vwfound.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\vwcommon\vwfound_i.c" "..\..\vwcommon\vwfound.tlb" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem_i.c" "..\..\vwcommon\tranitem.h" "$(OUTDIR)\vwfound.dll" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\favatar.obj"
	-@erase "$(INTDIR)\fglobal.obj"
	-@erase "$(INTDIR)\fhost.obj"
	-@erase "$(INTDIR)\foundatn.obj"
	-@erase "$(INTDIR)\fthing.obj"
	-@erase "$(INTDIR)\mmhelp.obj"
	-@erase "$(INTDIR)\painting.obj"
	-@erase "$(INTDIR)\profhelp.obj"
	-@erase "$(INTDIR)\sign.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\tranito.obj"
	-@erase "$(INTDIR)\TransMch.obj"
	-@erase "$(INTDIR)\vwfoun60.pch"
	-@erase "$(INTDIR)\vwfound.idb"
	-@erase "$(INTDIR)\vwfound.obj"
	-@erase "$(INTDIR)\vwfound.res"
	-@erase "$(OUTDIR)\vwfound.dll"
	-@erase "$(OUTDIR)\vwfound.exp"
	-@erase "$(OUTDIR)\vwfound.ilk"
	-@erase "$(OUTDIR)\vwfound.lib"
	-@erase "$(OUTDIR)\vwfound.pdb"
	-@erase "..\..\vwcommon\tranitem.h"
	-@erase "..\..\vwcommon\tranitem_i.c"
	-@erase "..\..\vwcommon\vwfound.h"
	-@erase "..\..\vwcommon\vwfound.tlb"
	-@erase "..\..\vwcommon\vwfound_i.c"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\vwfound.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwfoun60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=uuid.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\vwfound.pdb" /debug /machine:I386 /def:".\vwfound.def" /out:"$(OUTDIR)\vwfound.dll" /implib:"$(OUTDIR)\vwfound.lib" 
DEF_FILE= \
	".\vwfound.def"
LINK32_OBJS= \
	"$(INTDIR)\favatar.obj" \
	"$(INTDIR)\fglobal.obj" \
	"$(INTDIR)\fhost.obj" \
	"$(INTDIR)\foundatn.obj" \
	"$(INTDIR)\fthing.obj" \
	"$(INTDIR)\mmhelp.obj" \
	"$(INTDIR)\painting.obj" \
	"$(INTDIR)\profhelp.obj" \
	"$(INTDIR)\sign.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\TransMch.obj" \
	"$(INTDIR)\vwfound.obj" \
	"$(INTDIR)\tranito.obj" \
	"$(INTDIR)\vwfound.res"

"$(OUTDIR)\vwfound.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\vwfound.dll
InputPath=.\Debug\vwfound.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("vwfoun60.dep")
!INCLUDE "vwfoun60.dep"
!ELSE 
!MESSAGE Warning: cannot find "vwfoun60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "vwfound - Win32 Release" || "$(CFG)" == "vwfound - Win32 Debug"
SOURCE=.\favatar.cpp

"$(INTDIR)\favatar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"


SOURCE=.\fglobal.cpp

"$(INTDIR)\fglobal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"


SOURCE=.\fhost.cpp

"$(INTDIR)\fhost.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"


SOURCE=.\foundatn.cpp

!IF  "$(CFG)" == "vwfound - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"vwfound.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\foundatn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"vwfound.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\foundatn.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\fthing.cpp

"$(INTDIR)\fthing.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"


SOURCE=..\..\vwcommon\mmhelp.cpp

!IF  "$(CFG)" == "vwfound - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\mmhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\mmhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\painting.cpp

!IF  "$(CFG)" == "vwfound - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"vwfound.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\painting.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"vwfound.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\painting.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\vwcommon\profhelp.cpp

"$(INTDIR)\profhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\sign.cpp

!IF  "$(CFG)" == "vwfound - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"vwfound.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\sign.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yu"vwfound.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\sign.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "vwfound - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwfoun60.pch" : $(SOURCE) "$(INTDIR)" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\property\tranitem" /I "." /I "..\..\vwcommon" /I "..\..\d2d" /I "\mssdk\include" /I "..\..\vwmm" /I "..\..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\vwfoun60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwfound.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwfoun60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\vwcommon\syshelp.cpp

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\TransMch.cpp

"$(INTDIR)\TransMch.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\tranitem.h"


SOURCE=.\vwfound.cpp

"$(INTDIR)\vwfound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\vwfound.h" "..\..\vwcommon\vwfound_i.c" "..\..\vwcommon\tranitem_i.c" "..\..\vwcommon\tranitem.h"


SOURCE=..\..\vwcommon\vwfound.idl

!IF  "$(CFG)" == "vwfound - Win32 Release"

InputPath=..\..\vwcommon\vwfound.idl
USERDEP__VWFOU="..\..\vwcommon\tranitem.idl"	

"..\..\vwcommon\vwfound.h"	"..\..\vwcommon\vwfound_i.c"	"..\..\vwcommon\vwfound.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWFOU)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwfound.h" /iid "vwfound_i.c" /I "..\..\vwcommon" /out                             "..\..\vwcommon" "vwfound.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

InputPath=..\..\vwcommon\vwfound.idl
USERDEP__VWFOU="..\..\vwcommon\tranitem.idl"	

"..\..\vwcommon\vwfound.h"	"..\..\vwcommon\vwfound_i.c"	"..\..\vwcommon\vwfound.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWFOU)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwfound.h" /iid "vwfound_i.c" /I "..\..\vwcommon" /out                             "..\..\vwcommon" "vwfound.idl"
<< 
	

!ENDIF 

SOURCE=.\vwfound.rc

"$(INTDIR)\vwfound.res" : $(SOURCE) "$(INTDIR)" "..\..\vwcommon\vwfound.tlb"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\..\vwcommon\tranitem.idl

!IF  "$(CFG)" == "vwfound - Win32 Release"

InputPath=..\..\vwcommon\tranitem.idl

"..\..\vwcommon\tranitem.h"	"..\..\vwcommon\tranitem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "tranitem.h" /iid "tranitem_i.c" /I "..\..\vwcommon" /out                             "..\..\vwcommon" "tranitem.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwfound - Win32 Debug"

InputPath=..\..\vwcommon\tranitem.idl

"..\..\vwcommon\tranitem.h"	"..\..\vwcommon\tranitem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "tranitem.h" /iid "tranitem_i.c" /I "..\..\vwcommon" /out                             "..\..\vwcommon" "tranitem.idl"
<< 
	

!ENDIF 

SOURCE=..\..\property\tranitem\tranito.cpp

"$(INTDIR)\tranito.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwfoun60.pch" "..\..\vwcommon\tranitem.h" "..\..\vwcommon\vwfound.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

