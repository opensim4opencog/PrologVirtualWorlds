# Microsoft Developer Studio Generated NMAKE File, Based on srvv260.dsp
!IF "$(CFG)" == ""
CFG=ServerV2 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ServerV2 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ServerV2 - Win32 Release" && "$(CFG)" != "ServerV2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "srvv260.mak" CFG="ServerV2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServerV2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ServerV2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ServerV2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\vws.exe"


CLEAN :
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\DlgMaxLimit.obj"
	-@erase "$(INTDIR)\DSURL.obj"
	-@erase "$(INTDIR)\globdoc.obj"
	-@erase "$(INTDIR)\globedvw.obj"
	-@erase "$(INTDIR)\globfrm.obj"
	-@erase "$(INTDIR)\InfoURL.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\OptionsDlg.obj"
	-@erase "$(INTDIR)\PrgDlg.obj"
	-@erase "$(INTDIR)\reghelp.obj"
	-@erase "$(INTDIR)\Servdoc.obj"
	-@erase "$(INTDIR)\ServerV2.obj"
	-@erase "$(INTDIR)\ServerV2.res"
	-@erase "$(INTDIR)\ServSink.obj"
	-@erase "$(INTDIR)\servView.obj"
	-@erase "$(INTDIR)\srvv260.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\vws.exe"
	-@erase "$(OUTDIR)\vws.map"
	-@erase "$(OUTDIR)\vws.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /I "." /I "..\vwcommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\srvv260.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ServerV2.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\srvv260.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=htmlhelp.lib wsock32.lib wininet.lib urlmon.lib uuid.lib activeds.lib netapi32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\vws.pdb" /map:"$(INTDIR)\vws.map" /debug /machine:I386 /nodefaultlib:"LIBCMT" /out:"$(OUTDIR)\vws.exe" /libpath:"..\vwcommon" 
LINK32_OBJS= \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\DlgMaxLimit.obj" \
	"$(INTDIR)\DSURL.obj" \
	"$(INTDIR)\globdoc.obj" \
	"$(INTDIR)\globedvw.obj" \
	"$(INTDIR)\globfrm.obj" \
	"$(INTDIR)\InfoURL.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OptionsDlg.obj" \
	"$(INTDIR)\PrgDlg.obj" \
	"$(INTDIR)\reghelp.obj" \
	"$(INTDIR)\Servdoc.obj" \
	"$(INTDIR)\ServerV2.obj" \
	"$(INTDIR)\ServSink.obj" \
	"$(INTDIR)\servView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ServerV2.res"

"$(OUTDIR)\vws.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\vws.exe" "$(OUTDIR)\srvv260.bsc"


CLEAN :
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\DlgMaxLimit.obj"
	-@erase "$(INTDIR)\DlgMaxLimit.sbr"
	-@erase "$(INTDIR)\DSURL.obj"
	-@erase "$(INTDIR)\DSURL.sbr"
	-@erase "$(INTDIR)\globdoc.obj"
	-@erase "$(INTDIR)\globdoc.sbr"
	-@erase "$(INTDIR)\globedvw.obj"
	-@erase "$(INTDIR)\globedvw.sbr"
	-@erase "$(INTDIR)\globfrm.obj"
	-@erase "$(INTDIR)\globfrm.sbr"
	-@erase "$(INTDIR)\InfoURL.obj"
	-@erase "$(INTDIR)\InfoURL.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\OptionsDlg.obj"
	-@erase "$(INTDIR)\OptionsDlg.sbr"
	-@erase "$(INTDIR)\PrgDlg.obj"
	-@erase "$(INTDIR)\PrgDlg.sbr"
	-@erase "$(INTDIR)\reghelp.obj"
	-@erase "$(INTDIR)\reghelp.sbr"
	-@erase "$(INTDIR)\Servdoc.obj"
	-@erase "$(INTDIR)\Servdoc.sbr"
	-@erase "$(INTDIR)\ServerV2.obj"
	-@erase "$(INTDIR)\ServerV2.res"
	-@erase "$(INTDIR)\ServerV2.sbr"
	-@erase "$(INTDIR)\ServSink.obj"
	-@erase "$(INTDIR)\ServSink.sbr"
	-@erase "$(INTDIR)\servView.obj"
	-@erase "$(INTDIR)\servView.sbr"
	-@erase "$(INTDIR)\srvv260.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vws.idb"
	-@erase "$(OUTDIR)\srvv260.bsc"
	-@erase "$(OUTDIR)\vws.exe"
	-@erase "$(OUTDIR)\vws.ilk"
	-@erase "$(OUTDIR)\vws.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\srvv260.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vws.pdb" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ServerV2.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\srvv260.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\DlgMaxLimit.sbr" \
	"$(INTDIR)\DSURL.sbr" \
	"$(INTDIR)\globdoc.sbr" \
	"$(INTDIR)\globedvw.sbr" \
	"$(INTDIR)\globfrm.sbr" \
	"$(INTDIR)\InfoURL.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\OptionsDlg.sbr" \
	"$(INTDIR)\PrgDlg.sbr" \
	"$(INTDIR)\reghelp.sbr" \
	"$(INTDIR)\Servdoc.sbr" \
	"$(INTDIR)\ServerV2.sbr" \
	"$(INTDIR)\ServSink.sbr" \
	"$(INTDIR)\servView.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\srvv260.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=htmlhelp.lib wsock32.lib wininet.lib urlmon.lib uuid.lib activeds.lib netapi32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\vws.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT" /out:"$(OUTDIR)\vws.exe" /libpath:"..\vwcommon" 
LINK32_OBJS= \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\DlgMaxLimit.obj" \
	"$(INTDIR)\DSURL.obj" \
	"$(INTDIR)\globdoc.obj" \
	"$(INTDIR)\globedvw.obj" \
	"$(INTDIR)\globfrm.obj" \
	"$(INTDIR)\InfoURL.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OptionsDlg.obj" \
	"$(INTDIR)\PrgDlg.obj" \
	"$(INTDIR)\reghelp.obj" \
	"$(INTDIR)\Servdoc.obj" \
	"$(INTDIR)\ServerV2.obj" \
	"$(INTDIR)\ServSink.obj" \
	"$(INTDIR)\servView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ServerV2.res"

"$(OUTDIR)\vws.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("srvv260.dep")
!INCLUDE "srvv260.dep"
!ELSE 
!MESSAGE Warning: cannot find "srvv260.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ServerV2 - Win32 Release" || "$(CFG)" == "ServerV2 - Win32 Debug"
SOURCE=.\ChildFrm.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\ChildFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\ChildFrm.obj"	"$(INTDIR)\ChildFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\DlgMaxLimit.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\DlgMaxLimit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\DlgMaxLimit.obj"	"$(INTDIR)\DlgMaxLimit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\DSURL.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\DSURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\DSURL.obj"	"$(INTDIR)\DSURL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\globdoc.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\globdoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\globdoc.obj"	"$(INTDIR)\globdoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\globedvw.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\globedvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\globedvw.obj"	"$(INTDIR)\globedvw.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\globfrm.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\globfrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\globfrm.obj"	"$(INTDIR)\globfrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\InfoURL.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\InfoURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\InfoURL.obj"	"$(INTDIR)\InfoURL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\OptionsDlg.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\OptionsDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\OptionsDlg.obj"	"$(INTDIR)\OptionsDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\PrgDlg.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\PrgDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\PrgDlg.obj"	"$(INTDIR)\PrgDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=..\vwcommon\reghelp.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\reghelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\reghelp.obj"	"$(INTDIR)\reghelp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Servdoc.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\Servdoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\Servdoc.obj"	"$(INTDIR)\Servdoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\ServerV2.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /I "." /I "..\vwcommon" /I "..\vwsystem" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\srvv260.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ServerV2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwsystem" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\srvv260.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vws.pdb" /FD /c 

"$(INTDIR)\ServerV2.obj"	"$(INTDIR)\ServerV2.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ServerV2.rc

"$(INTDIR)\ServerV2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ServSink.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\ServSink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\ServSink.obj"	"$(INTDIR)\ServSink.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\servView.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"


"$(INTDIR)\servView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"


"$(INTDIR)\servView.obj"	"$(INTDIR)\servView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\srvv260.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ServerV2 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /I "." /I "..\vwcommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\srvv260.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\srvv260.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ServerV2 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\srvv260.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vws.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\srvv260.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

