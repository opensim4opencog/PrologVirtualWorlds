# Microsoft Developer Studio Generated NMAKE File, Based on mmimage.dsp
!IF "$(CFG)" == ""
CFG=MMImage - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MMImage - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MMImage - Win32 Release" && "$(CFG)" != "MMImage - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mmimage.mak" CFG="MMImage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MMImage - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MMImage - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MMImage - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\lib\MMImage.lib"


CLEAN :
	-@erase "$(INTDIR)\blt.obj"
	-@erase "$(INTDIR)\bmpimage.obj"
	-@erase "$(INTDIR)\ddhelper.obj"
	-@erase "$(INTDIR)\defpal.obj"
	-@erase "$(INTDIR)\gifimage.obj"
	-@erase "$(INTDIR)\imgcache.obj"
	-@erase "$(INTDIR)\JPGImage.obj"
	-@erase "$(INTDIR)\mfib.obj"
	-@erase "$(INTDIR)\mfimage.obj"
	-@erase "$(INTDIR)\palmap.obj"
	-@erase "$(INTDIR)\palmapb.obj"
	-@erase "$(INTDIR)\palmgr.obj"
	-@erase "$(INTDIR)\PixInfo.obj"
	-@erase "$(INTDIR)\ppmimage.obj"
	-@erase "$(INTDIR)\rlesurf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\lib\MMImage.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /Z7 /O2 /I ".\include" /I "..\D2DUtils\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\mmimage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mmimage.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\lib\MMImage.lib" 
LIB32_OBJS= \
	"$(INTDIR)\blt.obj" \
	"$(INTDIR)\bmpimage.obj" \
	"$(INTDIR)\ddhelper.obj" \
	"$(INTDIR)\defpal.obj" \
	"$(INTDIR)\gifimage.obj" \
	"$(INTDIR)\imgcache.obj" \
	"$(INTDIR)\JPGImage.obj" \
	"$(INTDIR)\mfib.obj" \
	"$(INTDIR)\mfimage.obj" \
	"$(INTDIR)\palmap.obj" \
	"$(INTDIR)\palmapb.obj" \
	"$(INTDIR)\palmgr.obj" \
	"$(INTDIR)\PixInfo.obj" \
	"$(INTDIR)\ppmimage.obj" \
	"$(INTDIR)\rlesurf.obj"

".\lib\MMImage.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MMImage - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\lib\MMImageD.lib"


CLEAN :
	-@erase "$(INTDIR)\blt.obj"
	-@erase "$(INTDIR)\bmpimage.obj"
	-@erase "$(INTDIR)\ddhelper.obj"
	-@erase "$(INTDIR)\defpal.obj"
	-@erase "$(INTDIR)\gifimage.obj"
	-@erase "$(INTDIR)\imgcache.obj"
	-@erase "$(INTDIR)\JPGImage.obj"
	-@erase "$(INTDIR)\mfib.obj"
	-@erase "$(INTDIR)\mfimage.obj"
	-@erase "$(INTDIR)\palmap.obj"
	-@erase "$(INTDIR)\palmapb.obj"
	-@erase "$(INTDIR)\palmgr.obj"
	-@erase "$(INTDIR)\PixInfo.obj"
	-@erase "$(INTDIR)\ppmimage.obj"
	-@erase "$(INTDIR)\rlesurf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\lib\MMImageD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MDd /W3 /GX /ZI /Od /I ".\include" /I "..\d2dutils\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\mmimage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mmimage.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\lib\MMImageD.lib" 
LIB32_OBJS= \
	"$(INTDIR)\blt.obj" \
	"$(INTDIR)\bmpimage.obj" \
	"$(INTDIR)\ddhelper.obj" \
	"$(INTDIR)\defpal.obj" \
	"$(INTDIR)\gifimage.obj" \
	"$(INTDIR)\imgcache.obj" \
	"$(INTDIR)\JPGImage.obj" \
	"$(INTDIR)\mfib.obj" \
	"$(INTDIR)\mfimage.obj" \
	"$(INTDIR)\palmap.obj" \
	"$(INTDIR)\palmapb.obj" \
	"$(INTDIR)\palmgr.obj" \
	"$(INTDIR)\PixInfo.obj" \
	"$(INTDIR)\ppmimage.obj" \
	"$(INTDIR)\rlesurf.obj"

".\lib\MMImageD.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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
!IF EXISTS("mmimage.dep")
!INCLUDE "mmimage.dep"
!ELSE 
!MESSAGE Warning: cannot find "mmimage.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MMImage - Win32 Release" || "$(CFG)" == "MMImage - Win32 Debug"
SOURCE=.\src\blt.cpp

"$(INTDIR)\blt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\bmpimage.cpp

"$(INTDIR)\bmpimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\ddhelper.cpp

"$(INTDIR)\ddhelper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\defpal.cpp

"$(INTDIR)\defpal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\gifimage.cpp

"$(INTDIR)\gifimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\imgcache.cpp

"$(INTDIR)\imgcache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\JPGImage.cpp

"$(INTDIR)\JPGImage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\mfib.cpp

"$(INTDIR)\mfib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\mfimage.cpp

"$(INTDIR)\mfimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\palmap.cpp

"$(INTDIR)\palmap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\palmapb.cpp

"$(INTDIR)\palmapb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\palmgr.cpp

"$(INTDIR)\palmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\PixInfo.cpp

"$(INTDIR)\PixInfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\ppmimage.cpp

"$(INTDIR)\ppmimage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\src\rlesurf.cpp

"$(INTDIR)\rlesurf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

