# Microsoft Developer Studio Project File - Name="d2d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=d2d - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "d2d60.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "d2d - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\Release/d2d.dll" /libpath:"\mssdk\lib"
# Begin Custom Build
OutDir=.\Release
TargetPath=.\Release\d2d.dll
InputPath=.\Release\d2d.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "\mssdk\include" /I "..\vwcommon" /I ".\MMImage\include" /I ".\D2DUtils\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_ATL_NO_UUIDOF" /Yu"stdafx.h" /Fd".\Debug\d2d.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\Debug/d2d.dll" /libpath:"\mssdk\lib"
# SUBTRACT LINK32 /profile /incremental:no /map /nodefaultlib
# Begin Custom Build
OutDir=.\Debug
TargetPath=.\Debug\d2d.dll
InputPath=.\Debug\d2d.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "d2d - Win32 Release"
# Name "d2d - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\d2d.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\d2d.def
# End Source File
# Begin Source File

SOURCE=..\vwcommon\d2d.idl

!IF  "$(CFG)" == "d2d - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\d2d.idl

BuildCmds= \
	midl /no_warn /I ..\vwcommon /out ..\vwcommon /DD2DLIB ..\vwcommon\d2d.idl

"..\vwcommon\d2d.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\d2d.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\d2d_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\vwcommon\d2d.idl

BuildCmds= \
	midl /no_warn /I ..\vwcommon /out ..\vwcommon /DD2DLIB ..\vwcommon\d2d.idl

"..\vwcommon\d2d.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\d2d.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"..\vwcommon\d2d_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\d2d.rc
# End Source File
# Begin Source File

SOURCE=.\D2DRM.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DRMAnm.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DRMArr.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DRMDev.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DRMFrm.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DRMHSp.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DRMImg.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DRMPkA.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DRMVP.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\D2DTrans.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\DispList.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\d2dbase.h
# End Source File
# Begin Source File

SOURCE=.\d2dprv.h
# End Source File
# Begin Source File

SOURCE=.\d2drm.h
# End Source File
# Begin Source File

SOURCE=.\d2drmanm.h
# End Source File
# Begin Source File

SOURCE=.\D2DRMArr.h
# End Source File
# Begin Source File

SOURCE=.\d2drmdev.h
# End Source File
# Begin Source File

SOURCE=.\d2drmfrm.h
# End Source File
# Begin Source File

SOURCE=.\d2drmhsp.h
# End Source File
# Begin Source File

SOURCE=.\d2drmimg.h
# End Source File
# Begin Source File

SOURCE=.\d2drmpka.h
# End Source File
# Begin Source File

SOURCE=.\d2drmvp.h
# End Source File
# Begin Source File

SOURCE=.\D2DTrans.h
# End Source File
# Begin Source File

SOURCE=.\DispList.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\d2d.rc2
# End Source File
# End Group
# Begin Group "mmimage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mmimage\src\blt.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\bmpimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\ddhelper.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\defpal.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\gifimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\imgcache.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\jpgimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\mfib.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\mfimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\palmap.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\palmapb.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\palmgr.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\pixinfo.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\ppmimage.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mmimage\src\rlesurf.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "D2DUtils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\d2dutils\src\darray.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\d2dutils\src\errmgr.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\d2dutils\src\mmutils.cpp

!IF  "$(CFG)" == "d2d - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "d2d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "jpeglib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\jpeglib\Jcomapi.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdapimin.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdapistd.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdcoefct.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdcolor.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jddctmgr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdhuff.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdinput.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdmainct.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdmarker.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdmaster.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdmerge.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdphuff.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdpostct.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdsample.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jdtrans.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jidctflt.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jidctfst.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jidctint.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jidctred.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jmemmgr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jmemnobs.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jpegconv.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jquant1.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jquant2.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\jpeglib\Jutils.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Source File

SOURCE=.\d2dprv_i.c
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
