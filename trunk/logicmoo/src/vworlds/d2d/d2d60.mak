# Microsoft Developer Studio Generated NMAKE File, Based on d2d60.dsp
!IF "$(CFG)" == ""
CFG=d2d - Win32 Release
!MESSAGE No configuration specified. Defaulting to d2d - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "d2d - Win32 Release" && "$(CFG)" != "d2d - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "d2d60.mak" CFG="d2d - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "d2d - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "d2d - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "d2d - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\vwcommon\d2d_i.c" "..\vwcommon\d2d.tlb" "..\vwcommon\d2d.h" "$(OUTDIR)\d2d.dll" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\blt.obj"
	-@erase "$(INTDIR)\bmpimage.obj"
	-@erase "$(INTDIR)\d2d.obj"
	-@erase "$(INTDIR)\d2d.res"
	-@erase "$(INTDIR)\d2d60.pch"
	-@erase "$(INTDIR)\D2DRM.obj"
	-@erase "$(INTDIR)\D2DRMAnm.obj"
	-@erase "$(INTDIR)\D2DRMArr.obj"
	-@erase "$(INTDIR)\D2DRMDev.obj"
	-@erase "$(INTDIR)\D2DRMFrm.obj"
	-@erase "$(INTDIR)\D2DRMHSp.obj"
	-@erase "$(INTDIR)\D2DRMImg.obj"
	-@erase "$(INTDIR)\D2DRMPkA.obj"
	-@erase "$(INTDIR)\D2DRMVP.obj"
	-@erase "$(INTDIR)\D2DTrans.obj"
	-@erase "$(INTDIR)\darray.obj"
	-@erase "$(INTDIR)\ddhelper.obj"
	-@erase "$(INTDIR)\defpal.obj"
	-@erase "$(INTDIR)\DispList.obj"
	-@erase "$(INTDIR)\errmgr.obj"
	-@erase "$(INTDIR)\gifimage.obj"
	-@erase "$(INTDIR)\imgcache.obj"
	-@erase "$(INTDIR)\Jcomapi.obj"
	-@erase "$(INTDIR)\Jdapimin.obj"
	-@erase "$(INTDIR)\Jdapistd.obj"
	-@erase "$(INTDIR)\Jdcoefct.obj"
	-@erase "$(INTDIR)\Jdcolor.obj"
	-@erase "$(INTDIR)\Jddctmgr.obj"
	-@erase "$(INTDIR)\Jdhuff.obj"
	-@erase "$(INTDIR)\Jdinput.obj"
	-@erase "$(INTDIR)\Jdmainct.obj"
	-@erase "$(INTDIR)\Jdmarker.obj"
	-@erase "$(INTDIR)\Jdmaster.obj"
	-@erase "$(INTDIR)\Jdmerge.obj"
	-@erase "$(INTDIR)\Jdphuff.obj"
	-@erase "$(INTDIR)\Jdpostct.obj"
	-@erase "$(INTDIR)\Jdsample.obj"
	-@erase "$(INTDIR)\Jdtrans.obj"
	-@erase "$(INTDIR)\Jidctflt.obj"
	-@erase "$(INTDIR)\Jidctfst.obj"
	-@erase "$(INTDIR)\Jidctint.obj"
	-@erase "$(INTDIR)\Jidctred.obj"
	-@erase "$(INTDIR)\Jmemmgr.obj"
	-@erase "$(INTDIR)\Jmemnobs.obj"
	-@erase "$(INTDIR)\jpegconv.obj"
	-@erase "$(INTDIR)\jpgimage.obj"
	-@erase "$(INTDIR)\Jquant1.obj"
	-@erase "$(INTDIR)\Jquant2.obj"
	-@erase "$(INTDIR)\Jutils.obj"
	-@erase "$(INTDIR)\mfib.obj"
	-@erase "$(INTDIR)\mfimage.obj"
	-@erase "$(INTDIR)\mmutils.obj"
	-@erase "$(INTDIR)\palmap.obj"
	-@erase "$(INTDIR)\palmapb.obj"
	-@erase "$(INTDIR)\palmgr.obj"
	-@erase "$(INTDIR)\pixinfo.obj"
	-@erase "$(INTDIR)\ppmimage.obj"
	-@erase "$(INTDIR)\rlesurf.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\d2d.dll"
	-@erase "$(OUTDIR)\d2d.exp"
	-@erase "$(OUTDIR)\d2d.lib"
	-@erase "$(OUTDIR)\d2d.pdb"
	-@erase "..\vwcommon\d2d.h"
	-@erase "..\vwcommon\d2d.tlb"
	-@erase "..\vwcommon\d2d_i.c"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\d2d.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\d2d60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\d2d.pdb" /debug /machine:I386 /def:".\d2d.def" /out:"$(OUTDIR)\d2d.dll" /implib:"$(OUTDIR)\d2d.lib" /libpath:"\mssdk\lib" 
DEF_FILE= \
	".\d2d.def"
LINK32_OBJS= \
	"$(INTDIR)\d2d.obj" \
	"$(INTDIR)\D2DRM.obj" \
	"$(INTDIR)\D2DRMAnm.obj" \
	"$(INTDIR)\D2DRMArr.obj" \
	"$(INTDIR)\D2DRMDev.obj" \
	"$(INTDIR)\D2DRMFrm.obj" \
	"$(INTDIR)\D2DRMHSp.obj" \
	"$(INTDIR)\D2DRMImg.obj" \
	"$(INTDIR)\D2DRMPkA.obj" \
	"$(INTDIR)\D2DRMVP.obj" \
	"$(INTDIR)\D2DTrans.obj" \
	"$(INTDIR)\DispList.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\blt.obj" \
	"$(INTDIR)\bmpimage.obj" \
	"$(INTDIR)\ddhelper.obj" \
	"$(INTDIR)\defpal.obj" \
	"$(INTDIR)\gifimage.obj" \
	"$(INTDIR)\imgcache.obj" \
	"$(INTDIR)\jpgimage.obj" \
	"$(INTDIR)\mfib.obj" \
	"$(INTDIR)\mfimage.obj" \
	"$(INTDIR)\palmap.obj" \
	"$(INTDIR)\palmapb.obj" \
	"$(INTDIR)\palmgr.obj" \
	"$(INTDIR)\pixinfo.obj" \
	"$(INTDIR)\ppmimage.obj" \
	"$(INTDIR)\rlesurf.obj" \
	"$(INTDIR)\darray.obj" \
	"$(INTDIR)\errmgr.obj" \
	"$(INTDIR)\mmutils.obj" \
	"$(INTDIR)\Jcomapi.obj" \
	"$(INTDIR)\Jdapimin.obj" \
	"$(INTDIR)\Jdapistd.obj" \
	"$(INTDIR)\Jdcoefct.obj" \
	"$(INTDIR)\Jdcolor.obj" \
	"$(INTDIR)\Jddctmgr.obj" \
	"$(INTDIR)\Jdhuff.obj" \
	"$(INTDIR)\Jdinput.obj" \
	"$(INTDIR)\Jdmainct.obj" \
	"$(INTDIR)\Jdmarker.obj" \
	"$(INTDIR)\Jdmaster.obj" \
	"$(INTDIR)\Jdmerge.obj" \
	"$(INTDIR)\Jdphuff.obj" \
	"$(INTDIR)\Jdpostct.obj" \
	"$(INTDIR)\Jdsample.obj" \
	"$(INTDIR)\Jdtrans.obj" \
	"$(INTDIR)\Jidctflt.obj" \
	"$(INTDIR)\Jidctfst.obj" \
	"$(INTDIR)\Jidctint.obj" \
	"$(INTDIR)\Jidctred.obj" \
	"$(INTDIR)\Jmemmgr.obj" \
	"$(INTDIR)\Jmemnobs.obj" \
	"$(INTDIR)\jpegconv.obj" \
	"$(INTDIR)\Jquant1.obj" \
	"$(INTDIR)\Jquant2.obj" \
	"$(INTDIR)\Jutils.obj" \
	"$(INTDIR)\d2d.res"

"$(OUTDIR)\d2d.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\d2d.dll
InputPath=.\Release\d2d.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\vwcommon\d2d_i.c" "..\vwcommon\d2d.tlb" "..\vwcommon\d2d.h" "$(OUTDIR)\d2d.dll" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\blt.obj"
	-@erase "$(INTDIR)\bmpimage.obj"
	-@erase "$(INTDIR)\d2d.idb"
	-@erase "$(INTDIR)\d2d.obj"
	-@erase "$(INTDIR)\d2d.pdb"
	-@erase "$(INTDIR)\d2d.res"
	-@erase "$(INTDIR)\d2d60.pch"
	-@erase "$(INTDIR)\D2DRM.obj"
	-@erase "$(INTDIR)\D2DRMAnm.obj"
	-@erase "$(INTDIR)\D2DRMArr.obj"
	-@erase "$(INTDIR)\D2DRMDev.obj"
	-@erase "$(INTDIR)\D2DRMFrm.obj"
	-@erase "$(INTDIR)\D2DRMHSp.obj"
	-@erase "$(INTDIR)\D2DRMImg.obj"
	-@erase "$(INTDIR)\D2DRMPkA.obj"
	-@erase "$(INTDIR)\D2DRMVP.obj"
	-@erase "$(INTDIR)\D2DTrans.obj"
	-@erase "$(INTDIR)\darray.obj"
	-@erase "$(INTDIR)\ddhelper.obj"
	-@erase "$(INTDIR)\defpal.obj"
	-@erase "$(INTDIR)\DispList.obj"
	-@erase "$(INTDIR)\errmgr.obj"
	-@erase "$(INTDIR)\gifimage.obj"
	-@erase "$(INTDIR)\imgcache.obj"
	-@erase "$(INTDIR)\Jcomapi.obj"
	-@erase "$(INTDIR)\Jdapimin.obj"
	-@erase "$(INTDIR)\Jdapistd.obj"
	-@erase "$(INTDIR)\Jdcoefct.obj"
	-@erase "$(INTDIR)\Jdcolor.obj"
	-@erase "$(INTDIR)\Jddctmgr.obj"
	-@erase "$(INTDIR)\Jdhuff.obj"
	-@erase "$(INTDIR)\Jdinput.obj"
	-@erase "$(INTDIR)\Jdmainct.obj"
	-@erase "$(INTDIR)\Jdmarker.obj"
	-@erase "$(INTDIR)\Jdmaster.obj"
	-@erase "$(INTDIR)\Jdmerge.obj"
	-@erase "$(INTDIR)\Jdphuff.obj"
	-@erase "$(INTDIR)\Jdpostct.obj"
	-@erase "$(INTDIR)\Jdsample.obj"
	-@erase "$(INTDIR)\Jdtrans.obj"
	-@erase "$(INTDIR)\Jidctflt.obj"
	-@erase "$(INTDIR)\Jidctfst.obj"
	-@erase "$(INTDIR)\Jidctint.obj"
	-@erase "$(INTDIR)\Jidctred.obj"
	-@erase "$(INTDIR)\Jmemmgr.obj"
	-@erase "$(INTDIR)\Jmemnobs.obj"
	-@erase "$(INTDIR)\jpegconv.obj"
	-@erase "$(INTDIR)\jpgimage.obj"
	-@erase "$(INTDIR)\Jquant1.obj"
	-@erase "$(INTDIR)\Jquant2.obj"
	-@erase "$(INTDIR)\Jutils.obj"
	-@erase "$(INTDIR)\mfib.obj"
	-@erase "$(INTDIR)\mfimage.obj"
	-@erase "$(INTDIR)\mmutils.obj"
	-@erase "$(INTDIR)\palmap.obj"
	-@erase "$(INTDIR)\palmapb.obj"
	-@erase "$(INTDIR)\palmgr.obj"
	-@erase "$(INTDIR)\pixinfo.obj"
	-@erase "$(INTDIR)\ppmimage.obj"
	-@erase "$(INTDIR)\rlesurf.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\d2d.dll"
	-@erase "$(OUTDIR)\d2d.exp"
	-@erase "$(OUTDIR)\d2d.ilk"
	-@erase "$(OUTDIR)\d2d.lib"
	-@erase "..\vwcommon\d2d.h"
	-@erase "..\vwcommon\d2d.tlb"
	-@erase "..\vwcommon\d2d_i.c"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\d2d.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\d2d60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\d2d.pdb" /debug /machine:I386 /def:".\d2d.def" /out:"$(OUTDIR)\d2d.dll" /implib:"$(OUTDIR)\d2d.lib" /libpath:"\mssdk\lib" 
DEF_FILE= \
	".\d2d.def"
LINK32_OBJS= \
	"$(INTDIR)\d2d.obj" \
	"$(INTDIR)\D2DRM.obj" \
	"$(INTDIR)\D2DRMAnm.obj" \
	"$(INTDIR)\D2DRMArr.obj" \
	"$(INTDIR)\D2DRMDev.obj" \
	"$(INTDIR)\D2DRMFrm.obj" \
	"$(INTDIR)\D2DRMHSp.obj" \
	"$(INTDIR)\D2DRMImg.obj" \
	"$(INTDIR)\D2DRMPkA.obj" \
	"$(INTDIR)\D2DRMVP.obj" \
	"$(INTDIR)\D2DTrans.obj" \
	"$(INTDIR)\DispList.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\blt.obj" \
	"$(INTDIR)\bmpimage.obj" \
	"$(INTDIR)\ddhelper.obj" \
	"$(INTDIR)\defpal.obj" \
	"$(INTDIR)\gifimage.obj" \
	"$(INTDIR)\imgcache.obj" \
	"$(INTDIR)\jpgimage.obj" \
	"$(INTDIR)\mfib.obj" \
	"$(INTDIR)\mfimage.obj" \
	"$(INTDIR)\palmap.obj" \
	"$(INTDIR)\palmapb.obj" \
	"$(INTDIR)\palmgr.obj" \
	"$(INTDIR)\pixinfo.obj" \
	"$(INTDIR)\ppmimage.obj" \
	"$(INTDIR)\rlesurf.obj" \
	"$(INTDIR)\darray.obj" \
	"$(INTDIR)\errmgr.obj" \
	"$(INTDIR)\mmutils.obj" \
	"$(INTDIR)\Jcomapi.obj" \
	"$(INTDIR)\Jdapimin.obj" \
	"$(INTDIR)\Jdapistd.obj" \
	"$(INTDIR)\Jdcoefct.obj" \
	"$(INTDIR)\Jdcolor.obj" \
	"$(INTDIR)\Jddctmgr.obj" \
	"$(INTDIR)\Jdhuff.obj" \
	"$(INTDIR)\Jdinput.obj" \
	"$(INTDIR)\Jdmainct.obj" \
	"$(INTDIR)\Jdmarker.obj" \
	"$(INTDIR)\Jdmaster.obj" \
	"$(INTDIR)\Jdmerge.obj" \
	"$(INTDIR)\Jdphuff.obj" \
	"$(INTDIR)\Jdpostct.obj" \
	"$(INTDIR)\Jdsample.obj" \
	"$(INTDIR)\Jdtrans.obj" \
	"$(INTDIR)\Jidctflt.obj" \
	"$(INTDIR)\Jidctfst.obj" \
	"$(INTDIR)\Jidctint.obj" \
	"$(INTDIR)\Jidctred.obj" \
	"$(INTDIR)\Jmemmgr.obj" \
	"$(INTDIR)\Jmemnobs.obj" \
	"$(INTDIR)\jpegconv.obj" \
	"$(INTDIR)\Jquant1.obj" \
	"$(INTDIR)\Jquant2.obj" \
	"$(INTDIR)\Jutils.obj" \
	"$(INTDIR)\d2d.res"

"$(OUTDIR)\d2d.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\d2d.dll
InputPath=.\Debug\d2d.dll
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
!IF EXISTS("d2d60.dep")
!INCLUDE "d2d60.dep"
!ELSE 
!MESSAGE Warning: cannot find "d2d60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "d2d - Win32 Release" || "$(CFG)" == "d2d - Win32 Debug"
SOURCE=.\d2d.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\d2d.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\d2d.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\d2d.idl

!IF  "$(CFG)" == "d2d - Win32 Release"

InputPath=..\vwcommon\d2d.idl

"..\vwcommon\d2d.h"	"..\vwcommon\d2d.tlb"	"..\vwcommon\d2d_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /I ..\vwcommon /out ..\vwcommon /DD2DLIB ..\vwcommon\d2d.idl
<< 
	

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

InputPath=..\vwcommon\d2d.idl

"..\vwcommon\d2d.h"	"..\vwcommon\d2d.tlb"	"..\vwcommon\d2d_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /no_warn /I ..\vwcommon /out ..\vwcommon /DD2DLIB ..\vwcommon\d2d.idl
<< 
	

!ENDIF 

SOURCE=.\d2d.rc

"$(INTDIR)\d2d.res" : $(SOURCE) "$(INTDIR)" "..\vwcommon\d2d.tlb"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\D2DRM.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DRMAnm.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRMAnm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRMAnm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DRMArr.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRMArr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRMArr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DRMDev.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRMDev.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRMDev.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DRMFrm.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRMFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRMFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DRMHSp.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRMHSp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRMHSp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DRMImg.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRMImg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRMImg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DRMPkA.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRMPkA.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRMPkA.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DRMVP.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DRMVP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DRMVP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\D2DTrans.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\D2DTrans.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\D2DTrans.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\DispList.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\DispList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\DispList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\d2d60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\d2d60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fp"$(INTDIR)\d2d60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\d2d60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\blt.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\blt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\blt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\bmpimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\bmpimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\bmpimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\ddhelper.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ddhelper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\ddhelper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\defpal.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\defpal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\defpal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\gifimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\gifimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\gifimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\imgcache.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\imgcache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\imgcache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\jpgimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\jpgimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\jpgimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\mfib.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\mfib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\mfib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\mfimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\mfimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\mfimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\palmap.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\palmap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\palmap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\palmapb.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\palmapb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\palmapb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\palmgr.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\palmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\palmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\pixinfo.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\pixinfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\pixinfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\ppmimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ppmimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\ppmimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\mmimage\src\rlesurf.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\rlesurf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\rlesurf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\d2dutils\src\darray.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\darray.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\darray.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\d2dutils\src\errmgr.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\errmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\errmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\d2dutils\src\mmutils.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\mmutils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\mmutils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jcomapi.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jcomapi.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jcomapi.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdapimin.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdapimin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdapimin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdapistd.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdapistd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdapistd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdcoefct.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdcoefct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdcoefct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdcolor.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdcolor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdcolor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jddctmgr.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jddctmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jddctmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdhuff.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdhuff.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdhuff.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdinput.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdinput.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdinput.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdmainct.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdmainct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdmainct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdmarker.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdmarker.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdmarker.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdmaster.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdmaster.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdmaster.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdmerge.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdmerge.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdmerge.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdphuff.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdphuff.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdphuff.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdpostct.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdpostct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdpostct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdsample.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdsample.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdsample.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jdtrans.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jdtrans.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jdtrans.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jidctflt.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jidctflt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jidctflt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jidctfst.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jidctfst.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jidctfst.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jidctint.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jidctint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jidctint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jidctred.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jidctred.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jidctred.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jmemmgr.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jmemmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jmemmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jmemnobs.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jmemnobs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jmemnobs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\jpegconv.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\jpegconv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\jpegconv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jquant1.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jquant1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jquant1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jquant2.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jquant2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jquant2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\jpeglib\Jutils.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Jutils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\d2d.pdb" /FD /c 

"$(INTDIR)\Jutils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\d2dprv_i.c

!ENDIF 

