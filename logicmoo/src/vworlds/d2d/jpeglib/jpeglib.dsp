# Microsoft Developer Studio Project File - Name="jpeglib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=jpeglib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jpeglib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jpeglib.mak" CFG="jpeglib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jpeglib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeglib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/federation/WinTrek", PJAAAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe

!IF  "$(CFG)" == "jpeglib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /FR /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "jpeglib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /FR /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "jpeglib - Win32 Release"
# Name "jpeglib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Jcomapi.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdapimin.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdapistd.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdcoefct.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdcolor.cpp
# End Source File
# Begin Source File

SOURCE=.\Jddctmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdhuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdinput.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdmainct.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdmaster.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdmerge.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdphuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdpostct.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdsample.cpp
# End Source File
# Begin Source File

SOURCE=.\Jdtrans.cpp
# End Source File
# Begin Source File

SOURCE=.\Jidctflt.cpp
# End Source File
# Begin Source File

SOURCE=.\Jidctfst.cpp
# End Source File
# Begin Source File

SOURCE=.\Jidctint.cpp
# End Source File
# Begin Source File

SOURCE=.\Jidctred.cpp
# End Source File
# Begin Source File

SOURCE=.\Jmemmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Jmemnobs.cpp
# End Source File
# Begin Source File

SOURCE=.\jpegconv.cpp
# End Source File
# Begin Source File

SOURCE=.\Jquant1.cpp
# End Source File
# Begin Source File

SOURCE=.\Jquant2.cpp
# End Source File
# Begin Source File

SOURCE=.\Jutils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\all.h
# End Source File
# Begin Source File

SOURCE=.\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\jdct.h
# End Source File
# Begin Source File

SOURCE=.\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\jerror.h
# End Source File
# Begin Source File

SOURCE=.\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
