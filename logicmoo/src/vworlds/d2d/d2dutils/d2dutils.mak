# Microsoft Developer Studio Generated NMAKE File, Based on D2DUtils.dsp
!IF "$(CFG)" == ""
CFG=D2DUtils - Win32 Debug
!MESSAGE No configuration specified. Defaulting to D2DUtils - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "D2DUtils - Win32 Release" && "$(CFG)" !=\
 "D2DUtils - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "D2DUtils.mak" CFG="D2DUtils - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "D2DUtils - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "D2DUtils - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe

!IF  "$(CFG)" == "D2DUtils - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\lib\D2DUtil.lib"

!ELSE 

ALL : ".\lib\D2DUtil.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\darray.obj"
	-@erase "$(INTDIR)\errmgr.obj"
	-@erase "$(INTDIR)\mmutils.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase ".\lib\D2DUtil.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I ".\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\D2DUtils.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\D2DUtils.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\lib\D2DUtil.lib" 
LIB32_OBJS= \
	"$(INTDIR)\darray.obj" \
	"$(INTDIR)\errmgr.obj" \
	"$(INTDIR)\mmutils.obj"

".\lib\D2DUtil.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "D2DUtils - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : ".\lib\D2DUtilD.lib"

!ELSE 

ALL : ".\lib\D2DUtilD.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\darray.obj"
	-@erase "$(INTDIR)\errmgr.obj"
	-@erase "$(INTDIR)\mmutils.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase ".\lib\D2DUtilD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /I ".\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\D2DUtils.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\D2DUtils.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:".\lib\D2DUtilD.lib" 
LIB32_OBJS= \
	"$(INTDIR)\darray.obj" \
	"$(INTDIR)\errmgr.obj" \
	"$(INTDIR)\mmutils.obj"

".\lib\D2DUtilD.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

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


!IF "$(CFG)" == "D2DUtils - Win32 Release" || "$(CFG)" ==\
 "D2DUtils - Win32 Debug"
SOURCE=.\src\darray.cpp

!IF  "$(CFG)" == "D2DUtils - Win32 Release"

DEP_CPP_DARRA=\
	".\include\d2dutils.h"\
	".\include\darray.h"\
	".\include\dset.h"\
	".\include\errmgr.h"\
	".\include\mmutils.h"\
	

"$(INTDIR)\darray.obj" : $(SOURCE) $(DEP_CPP_DARRA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "D2DUtils - Win32 Debug"

DEP_CPP_DARRA=\
	".\include\d2dutils.h"\
	".\include\darray.h"\
	".\include\dset.h"\
	".\include\errmgr.h"\
	".\include\mmutils.h"\
	

"$(INTDIR)\darray.obj" : $(SOURCE) $(DEP_CPP_DARRA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\errmgr.cpp

!IF  "$(CFG)" == "D2DUtils - Win32 Release"

DEP_CPP_ERRMG=\
	".\include\d2dutils.h"\
	".\include\darray.h"\
	".\include\dset.h"\
	".\include\errmgr.h"\
	".\include\mmutils.h"\
	

"$(INTDIR)\errmgr.obj" : $(SOURCE) $(DEP_CPP_ERRMG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "D2DUtils - Win32 Debug"

DEP_CPP_ERRMG=\
	".\include\d2dutils.h"\
	".\include\darray.h"\
	".\include\dset.h"\
	".\include\errmgr.h"\
	".\include\mmutils.h"\
	

"$(INTDIR)\errmgr.obj" : $(SOURCE) $(DEP_CPP_ERRMG) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\src\mmutils.cpp

!IF  "$(CFG)" == "D2DUtils - Win32 Release"

DEP_CPP_MMUTI=\
	".\include\d2dutils.h"\
	".\include\darray.h"\
	".\include\dset.h"\
	".\include\errmgr.h"\
	".\include\mmutils.h"\
	

"$(INTDIR)\mmutils.obj" : $(SOURCE) $(DEP_CPP_MMUTI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "D2DUtils - Win32 Debug"

DEP_CPP_MMUTI=\
	".\include\d2dutils.h"\
	".\include\darray.h"\
	".\include\dset.h"\
	".\include\errmgr.h"\
	".\include\mmutils.h"\
	

"$(INTDIR)\mmutils.obj" : $(SOURCE) $(DEP_CPP_MMUTI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

