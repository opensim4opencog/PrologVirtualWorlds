# Microsoft Developer Studio Project File - Name="MMImage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MMImage - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mmimage.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MMImage - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Z7 /O2 /I ".\include" /I "..\D2DUtils\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\lib\MMImage.lib"

!ELSEIF  "$(CFG)" == "MMImage - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MDd /W3 /GX /ZI /Od /I ".\include" /I "..\d2dutils\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\lib\MMImageD.lib"

!ENDIF 

# Begin Target

# Name "MMImage - Win32 Release"
# Name "MMImage - Win32 Debug"
# Begin Source File

SOURCE=.\src\blt.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bmpimage.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ddhelper.cpp
# End Source File
# Begin Source File

SOURCE=.\src\defpal.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gifimage.cpp
# End Source File
# Begin Source File

SOURCE=.\src\imgcache.cpp
# End Source File
# Begin Source File

SOURCE=.\src\JPGImage.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mfib.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mfimage.cpp
# End Source File
# Begin Source File

SOURCE=.\src\palmap.cpp
# End Source File
# Begin Source File

SOURCE=.\src\palmapb.cpp
# End Source File
# Begin Source File

SOURCE=.\src\palmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PixInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ppmimage.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rlesurf.cpp
# End Source File
# End Target
# End Project
