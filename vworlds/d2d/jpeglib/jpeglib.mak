# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=jpeglib - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to jpeglib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "jpeglib - Win32 Release" && "$(CFG)" !=\
 "jpeglib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "jpeglib.mak" CFG="jpeglib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jpeglib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeglib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "jpeglib - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "jpeglib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\jpeglib.lib"

CLEAN : 
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
	-@erase "$(INTDIR)\Jquant1.obj"
	-@erase "$(INTDIR)\Jquant2.obj"
	-@erase "$(INTDIR)\Jutils.obj"
	-@erase ".\jpeglib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/jpeglib.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/jpeglib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"jpeglib.lib"
LIB32_FLAGS=/nologo /out:"jpeglib.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\Jutils.obj"

".\jpeglib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\jpeglib.lib" "$(OUTDIR)\jpeglib.bsc"

CLEAN : 
	-@erase "$(INTDIR)\Jcomapi.obj"
	-@erase "$(INTDIR)\Jcomapi.sbr"
	-@erase "$(INTDIR)\Jdapimin.obj"
	-@erase "$(INTDIR)\Jdapimin.sbr"
	-@erase "$(INTDIR)\Jdapistd.obj"
	-@erase "$(INTDIR)\Jdapistd.sbr"
	-@erase "$(INTDIR)\Jdcoefct.obj"
	-@erase "$(INTDIR)\Jdcoefct.sbr"
	-@erase "$(INTDIR)\Jdcolor.obj"
	-@erase "$(INTDIR)\Jdcolor.sbr"
	-@erase "$(INTDIR)\Jddctmgr.obj"
	-@erase "$(INTDIR)\Jddctmgr.sbr"
	-@erase "$(INTDIR)\Jdhuff.obj"
	-@erase "$(INTDIR)\Jdhuff.sbr"
	-@erase "$(INTDIR)\Jdinput.obj"
	-@erase "$(INTDIR)\Jdinput.sbr"
	-@erase "$(INTDIR)\Jdmainct.obj"
	-@erase "$(INTDIR)\Jdmainct.sbr"
	-@erase "$(INTDIR)\Jdmarker.obj"
	-@erase "$(INTDIR)\Jdmarker.sbr"
	-@erase "$(INTDIR)\Jdmaster.obj"
	-@erase "$(INTDIR)\Jdmaster.sbr"
	-@erase "$(INTDIR)\Jdmerge.obj"
	-@erase "$(INTDIR)\Jdmerge.sbr"
	-@erase "$(INTDIR)\Jdphuff.obj"
	-@erase "$(INTDIR)\Jdphuff.sbr"
	-@erase "$(INTDIR)\Jdpostct.obj"
	-@erase "$(INTDIR)\Jdpostct.sbr"
	-@erase "$(INTDIR)\Jdsample.obj"
	-@erase "$(INTDIR)\Jdsample.sbr"
	-@erase "$(INTDIR)\Jdtrans.obj"
	-@erase "$(INTDIR)\Jdtrans.sbr"
	-@erase "$(INTDIR)\Jidctflt.obj"
	-@erase "$(INTDIR)\Jidctflt.sbr"
	-@erase "$(INTDIR)\Jidctfst.obj"
	-@erase "$(INTDIR)\Jidctfst.sbr"
	-@erase "$(INTDIR)\Jidctint.obj"
	-@erase "$(INTDIR)\Jidctint.sbr"
	-@erase "$(INTDIR)\Jidctred.obj"
	-@erase "$(INTDIR)\Jidctred.sbr"
	-@erase "$(INTDIR)\Jmemmgr.obj"
	-@erase "$(INTDIR)\Jmemmgr.sbr"
	-@erase "$(INTDIR)\Jmemnobs.obj"
	-@erase "$(INTDIR)\Jmemnobs.sbr"
	-@erase "$(INTDIR)\jpegconv.obj"
	-@erase "$(INTDIR)\jpegconv.sbr"
	-@erase "$(INTDIR)\Jquant1.obj"
	-@erase "$(INTDIR)\Jquant1.sbr"
	-@erase "$(INTDIR)\Jquant2.obj"
	-@erase "$(INTDIR)\Jquant2.sbr"
	-@erase "$(INTDIR)\Jutils.obj"
	-@erase "$(INTDIR)\Jutils.sbr"
	-@erase "$(OUTDIR)\jpeglib.bsc"
	-@erase ".\jpeglib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
CPP_PROJ=/nologo /MLd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/jpeglib.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/jpeglib.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Jcomapi.sbr" \
	"$(INTDIR)\Jdapimin.sbr" \
	"$(INTDIR)\Jdapistd.sbr" \
	"$(INTDIR)\Jdcoefct.sbr" \
	"$(INTDIR)\Jdcolor.sbr" \
	"$(INTDIR)\Jddctmgr.sbr" \
	"$(INTDIR)\Jdhuff.sbr" \
	"$(INTDIR)\Jdinput.sbr" \
	"$(INTDIR)\Jdmainct.sbr" \
	"$(INTDIR)\Jdmarker.sbr" \
	"$(INTDIR)\Jdmaster.sbr" \
	"$(INTDIR)\Jdmerge.sbr" \
	"$(INTDIR)\Jdphuff.sbr" \
	"$(INTDIR)\Jdpostct.sbr" \
	"$(INTDIR)\Jdsample.sbr" \
	"$(INTDIR)\Jdtrans.sbr" \
	"$(INTDIR)\Jidctflt.sbr" \
	"$(INTDIR)\Jidctfst.sbr" \
	"$(INTDIR)\Jidctint.sbr" \
	"$(INTDIR)\Jidctred.sbr" \
	"$(INTDIR)\Jmemmgr.sbr" \
	"$(INTDIR)\Jmemnobs.sbr" \
	"$(INTDIR)\jpegconv.sbr" \
	"$(INTDIR)\Jquant1.sbr" \
	"$(INTDIR)\Jquant2.sbr" \
	"$(INTDIR)\Jutils.sbr"

"$(OUTDIR)\jpeglib.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"jpeglib.lib"
LIB32_FLAGS=/nologo /out:"jpeglib.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\Jutils.obj"

".\jpeglib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "jpeglib - Win32 Release"
# Name "jpeglib - Win32 Debug"

!IF  "$(CFG)" == "jpeglib - Win32 Release"

!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Jquant1.cpp
DEP_CPP_JQUAN=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jquant1.obj" : $(SOURCE) $(DEP_CPP_JQUAN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jquant1.obj" : $(SOURCE) $(DEP_CPP_JQUAN) "$(INTDIR)"

"$(INTDIR)\Jquant1.sbr" : $(SOURCE) $(DEP_CPP_JQUAN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdapimin.cpp
DEP_CPP_JDAPI=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdapimin.obj" : $(SOURCE) $(DEP_CPP_JDAPI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdapimin.obj" : $(SOURCE) $(DEP_CPP_JDAPI) "$(INTDIR)"

"$(INTDIR)\Jdapimin.sbr" : $(SOURCE) $(DEP_CPP_JDAPI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdapistd.cpp
DEP_CPP_JDAPIS=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdapistd.obj" : $(SOURCE) $(DEP_CPP_JDAPIS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdapistd.obj" : $(SOURCE) $(DEP_CPP_JDAPIS) "$(INTDIR)"

"$(INTDIR)\Jdapistd.sbr" : $(SOURCE) $(DEP_CPP_JDAPIS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdcoefct.cpp
DEP_CPP_JDCOE=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdcoefct.obj" : $(SOURCE) $(DEP_CPP_JDCOE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdcoefct.obj" : $(SOURCE) $(DEP_CPP_JDCOE) "$(INTDIR)"

"$(INTDIR)\Jdcoefct.sbr" : $(SOURCE) $(DEP_CPP_JDCOE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdcolor.cpp
DEP_CPP_JDCOL=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdcolor.obj" : $(SOURCE) $(DEP_CPP_JDCOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdcolor.obj" : $(SOURCE) $(DEP_CPP_JDCOL) "$(INTDIR)"

"$(INTDIR)\Jdcolor.sbr" : $(SOURCE) $(DEP_CPP_JDCOL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jddctmgr.cpp
DEP_CPP_JDDCT=\
	".\all.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jddctmgr.obj" : $(SOURCE) $(DEP_CPP_JDDCT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jddctmgr.obj" : $(SOURCE) $(DEP_CPP_JDDCT) "$(INTDIR)"

"$(INTDIR)\Jddctmgr.sbr" : $(SOURCE) $(DEP_CPP_JDDCT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdhuff.cpp
DEP_CPP_JDHUF=\
	".\all.h"\
	".\jconfig.h"\
	".\jdhuff.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdhuff.obj" : $(SOURCE) $(DEP_CPP_JDHUF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdhuff.obj" : $(SOURCE) $(DEP_CPP_JDHUF) "$(INTDIR)"

"$(INTDIR)\Jdhuff.sbr" : $(SOURCE) $(DEP_CPP_JDHUF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdinput.cpp
DEP_CPP_JDINP=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdinput.obj" : $(SOURCE) $(DEP_CPP_JDINP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdinput.obj" : $(SOURCE) $(DEP_CPP_JDINP) "$(INTDIR)"

"$(INTDIR)\Jdinput.sbr" : $(SOURCE) $(DEP_CPP_JDINP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdmainct.cpp
DEP_CPP_JDMAI=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdmainct.obj" : $(SOURCE) $(DEP_CPP_JDMAI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdmainct.obj" : $(SOURCE) $(DEP_CPP_JDMAI) "$(INTDIR)"

"$(INTDIR)\Jdmainct.sbr" : $(SOURCE) $(DEP_CPP_JDMAI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdmarker.cpp
DEP_CPP_JDMAR=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdmarker.obj" : $(SOURCE) $(DEP_CPP_JDMAR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdmarker.obj" : $(SOURCE) $(DEP_CPP_JDMAR) "$(INTDIR)"

"$(INTDIR)\Jdmarker.sbr" : $(SOURCE) $(DEP_CPP_JDMAR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdmaster.cpp
DEP_CPP_JDMAS=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdmaster.obj" : $(SOURCE) $(DEP_CPP_JDMAS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdmaster.obj" : $(SOURCE) $(DEP_CPP_JDMAS) "$(INTDIR)"

"$(INTDIR)\Jdmaster.sbr" : $(SOURCE) $(DEP_CPP_JDMAS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdmerge.cpp
DEP_CPP_JDMER=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdmerge.obj" : $(SOURCE) $(DEP_CPP_JDMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdmerge.obj" : $(SOURCE) $(DEP_CPP_JDMER) "$(INTDIR)"

"$(INTDIR)\Jdmerge.sbr" : $(SOURCE) $(DEP_CPP_JDMER) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdphuff.cpp
DEP_CPP_JDPHU=\
	".\all.h"\
	".\jconfig.h"\
	".\jdhuff.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdphuff.obj" : $(SOURCE) $(DEP_CPP_JDPHU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdphuff.obj" : $(SOURCE) $(DEP_CPP_JDPHU) "$(INTDIR)"

"$(INTDIR)\Jdphuff.sbr" : $(SOURCE) $(DEP_CPP_JDPHU) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdpostct.cpp
DEP_CPP_JDPOS=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdpostct.obj" : $(SOURCE) $(DEP_CPP_JDPOS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdpostct.obj" : $(SOURCE) $(DEP_CPP_JDPOS) "$(INTDIR)"

"$(INTDIR)\Jdpostct.sbr" : $(SOURCE) $(DEP_CPP_JDPOS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdsample.cpp
DEP_CPP_JDSAM=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdsample.obj" : $(SOURCE) $(DEP_CPP_JDSAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdsample.obj" : $(SOURCE) $(DEP_CPP_JDSAM) "$(INTDIR)"

"$(INTDIR)\Jdsample.sbr" : $(SOURCE) $(DEP_CPP_JDSAM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jdtrans.cpp
DEP_CPP_JDTRA=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jdtrans.obj" : $(SOURCE) $(DEP_CPP_JDTRA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jdtrans.obj" : $(SOURCE) $(DEP_CPP_JDTRA) "$(INTDIR)"

"$(INTDIR)\Jdtrans.sbr" : $(SOURCE) $(DEP_CPP_JDTRA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jidctflt.cpp
DEP_CPP_JIDCT=\
	".\all.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jidctflt.obj" : $(SOURCE) $(DEP_CPP_JIDCT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jidctflt.obj" : $(SOURCE) $(DEP_CPP_JIDCT) "$(INTDIR)"

"$(INTDIR)\Jidctflt.sbr" : $(SOURCE) $(DEP_CPP_JIDCT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jidctfst.cpp
DEP_CPP_JIDCTF=\
	".\all.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jidctfst.obj" : $(SOURCE) $(DEP_CPP_JIDCTF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jidctfst.obj" : $(SOURCE) $(DEP_CPP_JIDCTF) "$(INTDIR)"

"$(INTDIR)\Jidctfst.sbr" : $(SOURCE) $(DEP_CPP_JIDCTF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jidctint.cpp
DEP_CPP_JIDCTI=\
	".\all.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jidctint.obj" : $(SOURCE) $(DEP_CPP_JIDCTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jidctint.obj" : $(SOURCE) $(DEP_CPP_JIDCTI) "$(INTDIR)"

"$(INTDIR)\Jidctint.sbr" : $(SOURCE) $(DEP_CPP_JIDCTI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jidctred.cpp
DEP_CPP_JIDCTR=\
	".\all.h"\
	".\jconfig.h"\
	".\jdct.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jidctred.obj" : $(SOURCE) $(DEP_CPP_JIDCTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jidctred.obj" : $(SOURCE) $(DEP_CPP_JIDCTR) "$(INTDIR)"

"$(INTDIR)\Jidctred.sbr" : $(SOURCE) $(DEP_CPP_JIDCTR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jmemmgr.cpp
DEP_CPP_JMEMM=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmemsys.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jmemmgr.obj" : $(SOURCE) $(DEP_CPP_JMEMM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jmemmgr.obj" : $(SOURCE) $(DEP_CPP_JMEMM) "$(INTDIR)"

"$(INTDIR)\Jmemmgr.sbr" : $(SOURCE) $(DEP_CPP_JMEMM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jmemnobs.cpp
DEP_CPP_JMEMN=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmemsys.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jmemnobs.obj" : $(SOURCE) $(DEP_CPP_JMEMN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jmemnobs.obj" : $(SOURCE) $(DEP_CPP_JMEMN) "$(INTDIR)"

"$(INTDIR)\Jmemnobs.sbr" : $(SOURCE) $(DEP_CPP_JMEMN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jpegconv.cpp
DEP_CPP_JPEGC=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\jpegconv.obj" : $(SOURCE) $(DEP_CPP_JPEGC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\jpegconv.obj" : $(SOURCE) $(DEP_CPP_JPEGC) "$(INTDIR)"

"$(INTDIR)\jpegconv.sbr" : $(SOURCE) $(DEP_CPP_JPEGC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jcomapi.cpp
DEP_CPP_JCOMA=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jcomapi.obj" : $(SOURCE) $(DEP_CPP_JCOMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jcomapi.obj" : $(SOURCE) $(DEP_CPP_JCOMA) "$(INTDIR)"

"$(INTDIR)\Jcomapi.sbr" : $(SOURCE) $(DEP_CPP_JCOMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jquant2.cpp
DEP_CPP_JQUANT=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jquant2.obj" : $(SOURCE) $(DEP_CPP_JQUANT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jquant2.obj" : $(SOURCE) $(DEP_CPP_JQUANT) "$(INTDIR)"

"$(INTDIR)\Jquant2.sbr" : $(SOURCE) $(DEP_CPP_JQUANT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Jutils.cpp
DEP_CPP_JUTIL=\
	".\all.h"\
	".\jconfig.h"\
	".\jerror.h"\
	".\jinclude.h"\
	".\jmorecfg.h"\
	".\jpegint.h"\
	".\jpeglib.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "jpeglib - Win32 Release"


"$(INTDIR)\Jutils.obj" : $(SOURCE) $(DEP_CPP_JUTIL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"


"$(INTDIR)\Jutils.obj" : $(SOURCE) $(DEP_CPP_JUTIL) "$(INTDIR)"

"$(INTDIR)\Jutils.sbr" : $(SOURCE) $(DEP_CPP_JUTIL) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
