# Microsoft Developer Studio Generated NMAKE File, Based on vwsyst60.dsp
!IF "$(CFG)" == ""
CFG=vwsystem - Win32 Debug
!MESSAGE No configuration specified. Defaulting to vwsystem - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "vwsystem - Win32 Debug" && "$(CFG)" != "vwsystem - Win32 Release MinSize"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vwsyst60.mak" CFG="vwsystem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vwsystem - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vwsystem - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\vwsystem_i.c" ".\vwsystem.tlb" ".\vwsystem.h" "..\vwcommon\vwsysex_i.c" "..\vwcommon\vwsysex.h" "..\vwcommon\vwserver_i.c" "..\vwcommon\vwserver.h" "..\vwcommon\vwprop_p.c" "..\vwcommon\vwprop_i.c" "..\vwcommon\vwprop.h" "..\vwcommon\vwodb_i.c" "..\vwcommon\vwodb.h" "..\vwcommon\vwobject_p.c" "..\vwcommon\vwobject_i.c" "..\vwcommon\vwobject.h" "..\vwcommon\vwhtml_i.c" "..\vwcommon\vwhtml.h" "..\vwcommon\vwcomm_i.c" "..\vwcommon\vwcomm.h" "..\vwcommon\vwclient_p.c" "..\vwcommon\vwclient_i.c" "..\vwcommon\vwclient.h" "..\vwcommon\vwbuffer_i.c" "..\vwcommon\vwbuffer.h" "..\vwcommon\vwanim_i.c" "..\vwcommon\vwanim.h" "..\vwcommon\vwadmin_i.c" "..\vwcommon\vwadmin.h" "..\vwcommon\ocxproxy_i.c" "..\vwcommon\ocxproxy.h" "..\vwcommon\menuitem_i.c" "..\vwcommon\menuitem.h" "..\vwcommon\inetfile_i.c" "..\vwcommon\inetfile.h" "..\vwcommon\dirserv_i.c" "..\vwcommon\dirserv.h" "..\vwcommon\avprof_i.c" "..\vwcommon\avprof.h" "..\vwcommon\authntic_i.c" "..\vwcommon\authntic.h" "$(OUTDIR)\vwsystem.dll" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\aobj.obj"
	-@erase "$(INTDIR)\artifact.obj"
	-@erase "$(INTDIR)\attrib.obj"
	-@erase "$(INTDIR)\authobj.obj"
	-@erase "$(INTDIR)\avatar.obj"
	-@erase "$(INTDIR)\Cathelp.obj"
	-@erase "$(INTDIR)\cmenuitm.obj"
	-@erase "$(INTDIR)\collenum.obj"
	-@erase "$(INTDIR)\commcono.obj"
	-@erase "$(INTDIR)\commfunc.obj"
	-@erase "$(INTDIR)\commlayr.obj"
	-@erase "$(INTDIR)\commmsgo.obj"
	-@erase "$(INTDIR)\commobj.obj"
	-@erase "$(INTDIR)\comobj.obj"
	-@erase "$(INTDIR)\connlist.obj"
	-@erase "$(INTDIR)\connserv.obj"
	-@erase "$(INTDIR)\dlldatax.obj"
	-@erase "$(INTDIR)\dlthread.obj"
	-@erase "$(INTDIR)\dqueue.obj"
	-@erase "$(INTDIR)\dserv.obj"
	-@erase "$(INTDIR)\fastcono.obj"
	-@erase "$(INTDIR)\fastmsgo.obj"
	-@erase "$(INTDIR)\fastobj.obj"
	-@erase "$(INTDIR)\host.obj"
	-@erase "$(INTDIR)\htmlwnd.obj"
	-@erase "$(INTDIR)\inetfile_p.obj"
	-@erase "$(INTDIR)\inetmgro.obj"
	-@erase "$(INTDIR)\IWportal.obj"
	-@erase "$(INTDIR)\logon.obj"
	-@erase "$(INTDIR)\marshbuf.obj"
	-@erase "$(INTDIR)\methdobj.obj"
	-@erase "$(INTDIR)\plisthlp.obj"
	-@erase "$(INTDIR)\plistobj.obj"
	-@erase "$(INTDIR)\pmaphlp.obj"
	-@erase "$(INTDIR)\pmapobj.obj"
	-@erase "$(INTDIR)\portal.obj"
	-@erase "$(INTDIR)\profhelp.obj"
	-@erase "$(INTDIR)\profobj.obj"
	-@erase "$(INTDIR)\propsecu.obj"
	-@erase "$(INTDIR)\queue.obj"
	-@erase "$(INTDIR)\reghelp.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\room.obj"
	-@erase "$(INTDIR)\scripts.obj"
	-@erase "$(INTDIR)\scrptobj.obj"
	-@erase "$(INTDIR)\statemch.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\thinghlp.obj"
	-@erase "$(INTDIR)\thingobj.obj"
	-@erase "$(INTDIR)\tinfo.obj"
	-@erase "$(INTDIR)\tools.obj"
	-@erase "$(INTDIR)\urlhelp.obj"
	-@erase "$(INTDIR)\vwadmobj.obj"
	-@erase "$(INTDIR)\vwanimob.obj"
	-@erase "$(INTDIR)\vwanmtob.obj"
	-@erase "$(INTDIR)\vwclient_p.obj"
	-@erase "$(INTDIR)\vwcliobj.obj"
	-@erase "$(INTDIR)\vwcliocx.obj"
	-@erase "$(INTDIR)\vwhelper.obj"
	-@erase "$(INTDIR)\vwhtmlob.obj"
	-@erase "$(INTDIR)\vwobject_p.obj"
	-@erase "$(INTDIR)\vwocx.obj"
	-@erase "$(INTDIR)\vwodbobj.obj"
	-@erase "$(INTDIR)\vwprop_p.obj"
	-@erase "$(INTDIR)\vwserve.obj"
	-@erase "$(INTDIR)\vwsysexo.obj"
	-@erase "$(INTDIR)\vwsyst60.pch"
	-@erase "$(INTDIR)\vwsystem.idb"
	-@erase "$(INTDIR)\vwsystem.obj"
	-@erase "$(INTDIR)\vwsystem.res"
	-@erase "$(INTDIR)\vwthing.obj"
	-@erase "$(INTDIR)\wbbrowsr.obj"
	-@erase "$(INTDIR)\WebPage.obj"
	-@erase "$(INTDIR)\wldinfo.obj"
	-@erase "$(INTDIR)\wldlist.obj"
	-@erase "$(INTDIR)\worldevt.obj"
	-@erase "$(INTDIR)\worldfmt.obj"
	-@erase "$(INTDIR)\worldhlp.obj"
	-@erase "$(INTDIR)\worldmar.obj"
	-@erase "$(INTDIR)\worldobj.obj"
	-@erase "$(OUTDIR)\vwsystem.dll"
	-@erase "$(OUTDIR)\vwsystem.exp"
	-@erase "$(OUTDIR)\vwsystem.ilk"
	-@erase "$(OUTDIR)\vwsystem.lib"
	-@erase "$(OUTDIR)\vwsystem.pdb"
	-@erase "..\vwcommon\authntic.h"
	-@erase "..\vwcommon\authntic_i.c"
	-@erase "..\vwcommon\avprof.h"
	-@erase "..\vwcommon\avprof_i.c"
	-@erase "..\vwcommon\dirserv.h"
	-@erase "..\vwcommon\dirserv_i.c"
	-@erase "..\vwcommon\inetfile.h"
	-@erase "..\vwcommon\inetfile_i.c"
	-@erase "..\vwcommon\menuitem.h"
	-@erase "..\vwcommon\menuitem_i.c"
	-@erase "..\vwcommon\ocxproxy.h"
	-@erase "..\vwcommon\ocxproxy_i.c"
	-@erase "..\vwcommon\vwadmin.h"
	-@erase "..\vwcommon\vwadmin_i.c"
	-@erase "..\vwcommon\vwanim.h"
	-@erase "..\vwcommon\vwanim_i.c"
	-@erase "..\vwcommon\vwbuffer.h"
	-@erase "..\vwcommon\vwbuffer_i.c"
	-@erase "..\vwcommon\vwclient.h"
	-@erase "..\vwcommon\vwclient_i.c"
	-@erase "..\vwcommon\vwclient_p.c"
	-@erase "..\vwcommon\vwcomm.h"
	-@erase "..\vwcommon\vwcomm_i.c"
	-@erase "..\vwcommon\vwhtml.h"
	-@erase "..\vwcommon\vwhtml_i.c"
	-@erase "..\vwcommon\vwobject.h"
	-@erase "..\vwcommon\vwobject_i.c"
	-@erase "..\vwcommon\vwobject_p.c"
	-@erase "..\vwcommon\vwodb.h"
	-@erase "..\vwcommon\vwodb_i.c"
	-@erase "..\vwcommon\vwprop.h"
	-@erase "..\vwcommon\vwprop_i.c"
	-@erase "..\vwcommon\vwprop_p.c"
	-@erase "..\vwcommon\vwserver.h"
	-@erase "..\vwcommon\vwserver_i.c"
	-@erase "..\vwcommon\vwsysex.h"
	-@erase "..\vwcommon\vwsysex_i.c"
	-@erase ".\Debug\regsvr32.trg"
	-@erase ".\vwsystem.h"
	-@erase ".\vwsystem.tlb"
	-@erase ".\vwsystem_i.c"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 /Os 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\vwsystem.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwsyst60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=wsock32.lib wininet.lib urlmon.lib uuid.lib netapi32.lib winmm.lib rpcndr.lib rpcns4.lib rpcrt4.lib mswsock.lib ws2_32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\vwsystem.pdb" /debug /machine:I386 /def:".\vwsystem.def" /out:"$(OUTDIR)\vwsystem.dll" /implib:"$(OUTDIR)\vwsystem.lib" /debug:full /dbgimplib 
DEF_FILE= \
	".\vwsystem.def"
LINK32_OBJS= \
	"$(INTDIR)\Cathelp.obj" \
	"$(INTDIR)\dlldatax.obj" \
	"$(INTDIR)\inetfile_p.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\vwclient_p.obj" \
	"$(INTDIR)\vwobject_p.obj" \
	"$(INTDIR)\vwprop_p.obj" \
	"$(INTDIR)\vwsystem.obj" \
	"$(INTDIR)\collenum.obj" \
	"$(INTDIR)\comobj.obj" \
	"$(INTDIR)\connlist.obj" \
	"$(INTDIR)\marshbuf.obj" \
	"$(INTDIR)\methdobj.obj" \
	"$(INTDIR)\plisthlp.obj" \
	"$(INTDIR)\plistobj.obj" \
	"$(INTDIR)\pmaphlp.obj" \
	"$(INTDIR)\pmapobj.obj" \
	"$(INTDIR)\propsecu.obj" \
	"$(INTDIR)\reghelp.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\scripts.obj" \
	"$(INTDIR)\scrptobj.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\thinghlp.obj" \
	"$(INTDIR)\thingobj.obj" \
	"$(INTDIR)\urlhelp.obj" \
	"$(INTDIR)\worldevt.obj" \
	"$(INTDIR)\worldfmt.obj" \
	"$(INTDIR)\worldhlp.obj" \
	"$(INTDIR)\worldmar.obj" \
	"$(INTDIR)\worldobj.obj" \
	"$(INTDIR)\vwcliobj.obj" \
	"$(INTDIR)\vwcliocx.obj" \
	"$(INTDIR)\connserv.obj" \
	"$(INTDIR)\queue.obj" \
	"$(INTDIR)\vwserve.obj" \
	"$(INTDIR)\wldinfo.obj" \
	"$(INTDIR)\wldlist.obj" \
	"$(INTDIR)\commcono.obj" \
	"$(INTDIR)\commfunc.obj" \
	"$(INTDIR)\commmsgo.obj" \
	"$(INTDIR)\commobj.obj" \
	"$(INTDIR)\tinfo.obj" \
	"$(INTDIR)\vwodbobj.obj" \
	"$(INTDIR)\vwadmobj.obj" \
	"$(INTDIR)\tools.obj" \
	"$(INTDIR)\commlayr.obj" \
	"$(INTDIR)\fastcono.obj" \
	"$(INTDIR)\fastmsgo.obj" \
	"$(INTDIR)\fastobj.obj" \
	"$(INTDIR)\vwanimob.obj" \
	"$(INTDIR)\vwanmtob.obj" \
	"$(INTDIR)\dlthread.obj" \
	"$(INTDIR)\inetmgro.obj" \
	"$(INTDIR)\htmlwnd.obj" \
	"$(INTDIR)\vwhelper.obj" \
	"$(INTDIR)\vwhtmlob.obj" \
	"$(INTDIR)\wbbrowsr.obj" \
	"$(INTDIR)\attrib.obj" \
	"$(INTDIR)\dqueue.obj" \
	"$(INTDIR)\dserv.obj" \
	"$(INTDIR)\aobj.obj" \
	"$(INTDIR)\authobj.obj" \
	"$(INTDIR)\logon.obj" \
	"$(INTDIR)\profhelp.obj" \
	"$(INTDIR)\profobj.obj" \
	"$(INTDIR)\cmenuitm.obj" \
	"$(INTDIR)\vwocx.obj" \
	"$(INTDIR)\artifact.obj" \
	"$(INTDIR)\avatar.obj" \
	"$(INTDIR)\host.obj" \
	"$(INTDIR)\IWportal.obj" \
	"$(INTDIR)\portal.obj" \
	"$(INTDIR)\room.obj" \
	"$(INTDIR)\statemch.obj" \
	"$(INTDIR)\vwsysexo.obj" \
	"$(INTDIR)\vwthing.obj" \
	"$(INTDIR)\WebPage.obj" \
	"$(INTDIR)\vwsystem.res"

"$(OUTDIR)\vwsystem.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\vwsystem.dll
InputPath=.\Debug\vwsystem.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

OUTDIR=.\ReleaseMinSize
INTDIR=.\ReleaseMinSize
# Begin Custom Macros
OutDir=.\ReleaseMinSize
# End Custom Macros

ALL : ".\vwsystem_i.c" ".\vwsystem.tlb" ".\vwsystem.h" "..\vwcommon\vwsysex_i.c" "..\vwcommon\vwsysex.h" "..\vwcommon\vwserver_i.c" "..\vwcommon\vwserver.h" "..\vwcommon\vwprop_p.c" "..\vwcommon\vwprop_i.c" "..\vwcommon\vwprop.h" "..\vwcommon\vwodb_i.c" "..\vwcommon\vwodb.h" "..\vwcommon\vwobject_p.c" "..\vwcommon\vwobject_i.c" "..\vwcommon\vwobject.h" "..\vwcommon\vwhtml_i.c" "..\vwcommon\vwhtml.h" "..\vwcommon\vwcomm_i.c" "..\vwcommon\vwcomm.h" "..\vwcommon\vwclient_p.c" "..\vwcommon\vwclient_i.c" "..\vwcommon\vwclient.h" "..\vwcommon\vwbuffer_i.c" "..\vwcommon\vwbuffer.h" "..\vwcommon\vwanim_i.c" "..\vwcommon\vwanim.h" "..\vwcommon\vwadmin_i.c" "..\vwcommon\vwadmin.h" "..\vwcommon\ocxproxy_i.c" "..\vwcommon\ocxproxy.h" "..\vwcommon\menuitem_i.c" "..\vwcommon\menuitem.h" "..\vwcommon\inetfile_i.c" "..\vwcommon\inetfile.h" "..\vwcommon\dirserv_i.c" "..\vwcommon\dirserv.h" "..\vwcommon\avprof_i.c" "..\vwcommon\avprof.h" "..\vwcommon\authntic_i.c" "..\vwcommon\authntic.h" "$(OUTDIR)\vwsystem.dll" ".\ReleaseMinSize\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\aobj.obj"
	-@erase "$(INTDIR)\artifact.obj"
	-@erase "$(INTDIR)\attrib.obj"
	-@erase "$(INTDIR)\authobj.obj"
	-@erase "$(INTDIR)\avatar.obj"
	-@erase "$(INTDIR)\Cathelp.obj"
	-@erase "$(INTDIR)\cmenuitm.obj"
	-@erase "$(INTDIR)\collenum.obj"
	-@erase "$(INTDIR)\commcono.obj"
	-@erase "$(INTDIR)\commfunc.obj"
	-@erase "$(INTDIR)\commlayr.obj"
	-@erase "$(INTDIR)\commmsgo.obj"
	-@erase "$(INTDIR)\commobj.obj"
	-@erase "$(INTDIR)\comobj.obj"
	-@erase "$(INTDIR)\connlist.obj"
	-@erase "$(INTDIR)\connserv.obj"
	-@erase "$(INTDIR)\dlldatax.obj"
	-@erase "$(INTDIR)\dlthread.obj"
	-@erase "$(INTDIR)\dqueue.obj"
	-@erase "$(INTDIR)\dserv.obj"
	-@erase "$(INTDIR)\fastcono.obj"
	-@erase "$(INTDIR)\fastmsgo.obj"
	-@erase "$(INTDIR)\fastobj.obj"
	-@erase "$(INTDIR)\host.obj"
	-@erase "$(INTDIR)\htmlwnd.obj"
	-@erase "$(INTDIR)\inetfile_p.obj"
	-@erase "$(INTDIR)\inetmgro.obj"
	-@erase "$(INTDIR)\IWportal.obj"
	-@erase "$(INTDIR)\logon.obj"
	-@erase "$(INTDIR)\marshbuf.obj"
	-@erase "$(INTDIR)\methdobj.obj"
	-@erase "$(INTDIR)\plisthlp.obj"
	-@erase "$(INTDIR)\plistobj.obj"
	-@erase "$(INTDIR)\pmaphlp.obj"
	-@erase "$(INTDIR)\pmapobj.obj"
	-@erase "$(INTDIR)\portal.obj"
	-@erase "$(INTDIR)\profhelp.obj"
	-@erase "$(INTDIR)\profobj.obj"
	-@erase "$(INTDIR)\propsecu.obj"
	-@erase "$(INTDIR)\queue.obj"
	-@erase "$(INTDIR)\reghelp.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\room.obj"
	-@erase "$(INTDIR)\scripts.obj"
	-@erase "$(INTDIR)\scrptobj.obj"
	-@erase "$(INTDIR)\statemch.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\syshelp.obj"
	-@erase "$(INTDIR)\thinghlp.obj"
	-@erase "$(INTDIR)\thingobj.obj"
	-@erase "$(INTDIR)\tinfo.obj"
	-@erase "$(INTDIR)\tools.obj"
	-@erase "$(INTDIR)\urlhelp.obj"
	-@erase "$(INTDIR)\vwadmobj.obj"
	-@erase "$(INTDIR)\vwanimob.obj"
	-@erase "$(INTDIR)\vwanmtob.obj"
	-@erase "$(INTDIR)\vwclient_p.obj"
	-@erase "$(INTDIR)\vwcliobj.obj"
	-@erase "$(INTDIR)\vwcliocx.obj"
	-@erase "$(INTDIR)\vwhelper.obj"
	-@erase "$(INTDIR)\vwhtmlob.obj"
	-@erase "$(INTDIR)\vwobject_p.obj"
	-@erase "$(INTDIR)\vwocx.obj"
	-@erase "$(INTDIR)\vwodbobj.obj"
	-@erase "$(INTDIR)\vwprop_p.obj"
	-@erase "$(INTDIR)\vwserve.obj"
	-@erase "$(INTDIR)\vwsysexo.obj"
	-@erase "$(INTDIR)\vwsyst60.pch"
	-@erase "$(INTDIR)\vwsystem.idb"
	-@erase "$(INTDIR)\vwsystem.obj"
	-@erase "$(INTDIR)\vwsystem.res"
	-@erase "$(INTDIR)\vwthing.obj"
	-@erase "$(INTDIR)\wbbrowsr.obj"
	-@erase "$(INTDIR)\WebPage.obj"
	-@erase "$(INTDIR)\wldinfo.obj"
	-@erase "$(INTDIR)\wldlist.obj"
	-@erase "$(INTDIR)\worldevt.obj"
	-@erase "$(INTDIR)\worldfmt.obj"
	-@erase "$(INTDIR)\worldhlp.obj"
	-@erase "$(INTDIR)\worldmar.obj"
	-@erase "$(INTDIR)\worldobj.obj"
	-@erase "$(OUTDIR)\vwsystem.dll"
	-@erase "$(OUTDIR)\vwsystem.exp"
	-@erase "$(OUTDIR)\vwsystem.lib"
	-@erase "$(OUTDIR)\vwsystem.pdb"
	-@erase "..\vwcommon\authntic.h"
	-@erase "..\vwcommon\authntic_i.c"
	-@erase "..\vwcommon\avprof.h"
	-@erase "..\vwcommon\avprof_i.c"
	-@erase "..\vwcommon\dirserv.h"
	-@erase "..\vwcommon\dirserv_i.c"
	-@erase "..\vwcommon\inetfile.h"
	-@erase "..\vwcommon\inetfile_i.c"
	-@erase "..\vwcommon\menuitem.h"
	-@erase "..\vwcommon\menuitem_i.c"
	-@erase "..\vwcommon\ocxproxy.h"
	-@erase "..\vwcommon\ocxproxy_i.c"
	-@erase "..\vwcommon\vwadmin.h"
	-@erase "..\vwcommon\vwadmin_i.c"
	-@erase "..\vwcommon\vwanim.h"
	-@erase "..\vwcommon\vwanim_i.c"
	-@erase "..\vwcommon\vwbuffer.h"
	-@erase "..\vwcommon\vwbuffer_i.c"
	-@erase "..\vwcommon\vwclient.h"
	-@erase "..\vwcommon\vwclient_i.c"
	-@erase "..\vwcommon\vwclient_p.c"
	-@erase "..\vwcommon\vwcomm.h"
	-@erase "..\vwcommon\vwcomm_i.c"
	-@erase "..\vwcommon\vwhtml.h"
	-@erase "..\vwcommon\vwhtml_i.c"
	-@erase "..\vwcommon\vwobject.h"
	-@erase "..\vwcommon\vwobject_i.c"
	-@erase "..\vwcommon\vwobject_p.c"
	-@erase "..\vwcommon\vwodb.h"
	-@erase "..\vwcommon\vwodb_i.c"
	-@erase "..\vwcommon\vwprop.h"
	-@erase "..\vwcommon\vwprop_i.c"
	-@erase "..\vwcommon\vwprop_p.c"
	-@erase "..\vwcommon\vwserver.h"
	-@erase "..\vwcommon\vwserver_i.c"
	-@erase "..\vwcommon\vwsysex.h"
	-@erase "..\vwcommon\vwsysex_i.c"
	-@erase ".\ReleaseMinSize\regsvr32.trg"
	-@erase ".\vwsystem.h"
	-@erase ".\vwsystem.tlb"
	-@erase ".\vwsystem_i.c"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 /Os 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\vwsystem.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vwsyst60.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=activeds.lib wsock32.lib wininet.lib urlmon.lib uuid.lib netapi32.lib winmm.lib rpcndr.lib rpcns4.lib rpcrt4.lib mswsock.lib ws2_32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\vwsystem.pdb" /debug /machine:I386 /def:".\vwsystem.def" /out:"$(OUTDIR)\vwsystem.dll" /implib:"$(OUTDIR)\vwsystem.lib" 
DEF_FILE= \
	".\vwsystem.def"
LINK32_OBJS= \
	"$(INTDIR)\Cathelp.obj" \
	"$(INTDIR)\dlldatax.obj" \
	"$(INTDIR)\inetfile_p.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\vwclient_p.obj" \
	"$(INTDIR)\vwobject_p.obj" \
	"$(INTDIR)\vwprop_p.obj" \
	"$(INTDIR)\vwsystem.obj" \
	"$(INTDIR)\collenum.obj" \
	"$(INTDIR)\comobj.obj" \
	"$(INTDIR)\connlist.obj" \
	"$(INTDIR)\marshbuf.obj" \
	"$(INTDIR)\methdobj.obj" \
	"$(INTDIR)\plisthlp.obj" \
	"$(INTDIR)\plistobj.obj" \
	"$(INTDIR)\pmaphlp.obj" \
	"$(INTDIR)\pmapobj.obj" \
	"$(INTDIR)\propsecu.obj" \
	"$(INTDIR)\reghelp.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\scripts.obj" \
	"$(INTDIR)\scrptobj.obj" \
	"$(INTDIR)\syshelp.obj" \
	"$(INTDIR)\thinghlp.obj" \
	"$(INTDIR)\thingobj.obj" \
	"$(INTDIR)\urlhelp.obj" \
	"$(INTDIR)\worldevt.obj" \
	"$(INTDIR)\worldfmt.obj" \
	"$(INTDIR)\worldhlp.obj" \
	"$(INTDIR)\worldmar.obj" \
	"$(INTDIR)\worldobj.obj" \
	"$(INTDIR)\vwcliobj.obj" \
	"$(INTDIR)\vwcliocx.obj" \
	"$(INTDIR)\connserv.obj" \
	"$(INTDIR)\queue.obj" \
	"$(INTDIR)\vwserve.obj" \
	"$(INTDIR)\wldinfo.obj" \
	"$(INTDIR)\wldlist.obj" \
	"$(INTDIR)\commcono.obj" \
	"$(INTDIR)\commfunc.obj" \
	"$(INTDIR)\commmsgo.obj" \
	"$(INTDIR)\commobj.obj" \
	"$(INTDIR)\tinfo.obj" \
	"$(INTDIR)\vwodbobj.obj" \
	"$(INTDIR)\vwadmobj.obj" \
	"$(INTDIR)\tools.obj" \
	"$(INTDIR)\commlayr.obj" \
	"$(INTDIR)\fastcono.obj" \
	"$(INTDIR)\fastmsgo.obj" \
	"$(INTDIR)\fastobj.obj" \
	"$(INTDIR)\vwanimob.obj" \
	"$(INTDIR)\vwanmtob.obj" \
	"$(INTDIR)\dlthread.obj" \
	"$(INTDIR)\inetmgro.obj" \
	"$(INTDIR)\htmlwnd.obj" \
	"$(INTDIR)\vwhelper.obj" \
	"$(INTDIR)\vwhtmlob.obj" \
	"$(INTDIR)\wbbrowsr.obj" \
	"$(INTDIR)\attrib.obj" \
	"$(INTDIR)\dqueue.obj" \
	"$(INTDIR)\dserv.obj" \
	"$(INTDIR)\aobj.obj" \
	"$(INTDIR)\authobj.obj" \
	"$(INTDIR)\logon.obj" \
	"$(INTDIR)\profhelp.obj" \
	"$(INTDIR)\profobj.obj" \
	"$(INTDIR)\cmenuitm.obj" \
	"$(INTDIR)\vwocx.obj" \
	"$(INTDIR)\artifact.obj" \
	"$(INTDIR)\avatar.obj" \
	"$(INTDIR)\host.obj" \
	"$(INTDIR)\IWportal.obj" \
	"$(INTDIR)\portal.obj" \
	"$(INTDIR)\room.obj" \
	"$(INTDIR)\statemch.obj" \
	"$(INTDIR)\vwsysexo.obj" \
	"$(INTDIR)\vwthing.obj" \
	"$(INTDIR)\WebPage.obj" \
	"$(INTDIR)\vwsystem.res"

"$(OUTDIR)\vwsystem.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\vwsystem.dll
InputPath=.\ReleaseMinSize\vwsystem.dll
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
!IF EXISTS("vwsyst60.dep")
!INCLUDE "vwsyst60.dep"
!ELSE 
!MESSAGE Warning: cannot find "vwsyst60.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "vwsystem - Win32 Debug" || "$(CFG)" == "vwsystem - Win32 Release MinSize"
SOURCE=..\Vwcommon\Cathelp.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\Cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\Cathelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\dlldatax.c

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\dlldatax.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\dlldatax.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\inetfile_p.c

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\inetfile_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\inetfile_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwsyst60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\vwsyst60.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwclient_p.c

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwclient_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwclient_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwobject_p.c

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwobject_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwobject_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwprop_p.c

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwprop_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwprop_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\vwsystem.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /I "..\vwadmin" /I "..\vwclient" /I "..\vwserver" /I "..\vwcomm" /I "..\fastcomm" /I "..\vwodb" /I "..\tools\vwanim" /I "..\tools\vwhtml" /I "..\tools\inetfile" /I "..\tools\vwstats" /I "..\tools\dirserv" /I "\dxsdk\sdk\inc" /I "..\module\system" /I "..\property\menuitem" /I "..\property\ocxproxy" /I "..\property\avprof" /I "..\tools\authntic" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwsystem.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /I "..\vwadmin" /I "..\vwclient" /I "..\vwserver" /I "..\vwcomm" /I "..\fastcomm" /I "..\vwodb" /I "..\tools\vwanim" /I "..\tools\vwhtml" /I "..\tools\inetfile" /I "..\tools\vwstats" /I "..\tools\dirserv" /I "\dxsdk\sdk\inc" /I "..\module\system" /I "..\property\menuitem" /I "..\property\ocxproxy" /I "..\property\avprof" /I "..\tools\authntic" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwsystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\vwsystem.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=.\vwsystem.idl
USERDEP__VWSYS="..\vwcommon\vwobject.idl"	"..\vwcommon\vwclient.idl"	"..\vwcommon\vwserver.idl"	"..\vwcommon\vwodb.idl"	"..\vwcommon\vwcomm.idl"	"..\vwcommon\menuitem.idl"	"..\vwcommon\vwanim.idl"	"..\vwcommon\vwstats.idl"	"..\vwcommon\vwhtml.idl"	"..\vwcommon\inetfile.idl"	"..\vwcommon\dirserv.idl"	"..\vwcommon\authntic.idl"	"..\vwcommon\vwsysex.idl"	"..\vwcommon\vwprop.idl"	"..\vwcommon\avprof.idl"	"..\vwcommon\ocxproxy.idl"	"..\vwcommon\vwadmin.idl"	

".\vwsystem.h"	".\vwsystem_i.c"	".\vwsystem.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWSYS)
	<<tempfile.bat 
	@echo off 
	midl /Oicf  /no_warn /h "vwsystem.h" /iid "vwsystem_i.c" /I "..\vwcommon" $(InputPath)
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=.\vwsystem.idl
USERDEP__VWSYS="..\vwcommon\vwobject.idl"	"..\vwcommon\vwclient.idl"	"..\vwcommon\vwserver.idl"	"..\vwcommon\vwodb.idl"	"..\vwcommon\vwcomm.idl"	"..\vwcommon\menuitem.idl"	"..\vwcommon\vwanim.idl"	"..\vwcommon\vwstats.idl"	"..\vwcommon\vwhtml.idl"	"..\vwcommon\inetfile.idl"	"..\vwcommon\dirserv.idl"	"..\vwcommon\authntic.idl"	"..\vwcommon\vwsysex.idl"	"..\vwcommon\vwprop.idl"	"..\vwcommon\avprof.idl"	"..\vwcommon\ocxproxy.idl"	"..\vwcommon\vwadmin.idl"	

".\vwsystem.h"	".\vwsystem_i.c"	".\vwsystem.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWSYS)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwsystem.h" /iid "vwsystem_i.c" /I "..\vwcommon" "vwsystem.idl"
<< 
	

!ENDIF 

SOURCE=.\vwsystem.rc

"$(INTDIR)\vwsystem.res" : $(SOURCE) "$(INTDIR)" ".\vwsystem.tlb"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\vwobject\collenum.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\collenum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\collenum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\comobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\comobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\comobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\connlist.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\connlist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\connlist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\marshbuf.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\marshbuf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\marshbuf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\methdobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\methdobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\methdobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\plisthlp.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\plisthlp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\plisthlp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\plistobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\plistobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\plistobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\pmaphlp.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\pmaphlp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\pmaphlp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\pmapobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\pmapobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\pmapobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\propsecu.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\propsecu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\propsecu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\reghelp.cpp

"$(INTDIR)\reghelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\registry.cpp

"$(INTDIR)\registry.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\scripts.cpp

"$(INTDIR)\scripts.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwobject\scrptobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\scrptobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\scrptobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\syshelp.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /I "..\property\menuitem" /I "..\property\ocxproxy" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /I "..\property\menuitem" /I "..\property\ocxproxy" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\syshelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\thinghlp.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\thinghlp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\thinghlp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\thingobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\thingobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\thingobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\urlhelp.cpp

"$(INTDIR)\urlhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwbuffer.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwbuffer.idl

"..\vwcommon\vwbuffer.h"	"..\vwcommon\vwbuffer_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwbuffer.h" /iid "vwbuffer_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwbuffer.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwbuffer.idl

"..\vwcommon\vwbuffer.h"	"..\vwcommon\vwbuffer_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwbuffer.h" /iid "vwbuffer_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwbuffer.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\vwobject.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwobject.idl
USERDEP__VWOBJ="..\vwcommon\vwcomm.idl"	"..\vwcommon\vwprop.idl"	

"..\vwcommon\vwobject.h"	"..\vwcommon\vwobject_i.c"	"..\vwcommon\vwobject_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWOBJ)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwobject.h" /iid "vwobject_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwobject.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwobject.idl
USERDEP__VWOBJ="..\vwcommon\vwcomm.idl"	"..\vwcommon\vwprop.idl"	

"..\vwcommon\vwobject.h"	"..\vwcommon\vwobject_i.c"	"..\vwcommon\vwobject_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWOBJ)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwobject.h" /iid "vwobject_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwobject.idl"
<< 
	

!ENDIF 

SOURCE="..\vwcommon\vwprop.idl"

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath="..\vwcommon\vwprop.idl"
USERDEP__VWPRO="..\vwcommon\vwbuffer.idl"	

"..\vwcommon\vwprop.h"	"..\vwcommon\vwprop_i.c"	"..\vwcommon\vwprop_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWPRO)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwprop.h" /iid "vwprop_i.c" /I "..\vwcommon" /out "..\vwcommon"                                                         "vwprop.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath="..\vwcommon\vwprop.idl"
USERDEP__VWPRO="..\vwcommon\vwbuffer.idl"	

"..\vwcommon\vwprop.h"	"..\vwcommon\vwprop_i.c"	"..\vwcommon\vwprop_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWPRO)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwprop.h" /iid "vwprop_i.c" /I "..\vwcommon" /out "..\vwcommon"                                                         "vwprop.idl"
<< 
	

!ENDIF 

SOURCE=..\vwobject\worldevt.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldevt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldevt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\worldfmt.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldfmt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldfmt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\worldhlp.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\manager" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldhlp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldhlp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\worldmar.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldmar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldmar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwobject\worldobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\manager" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\worldobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwclient.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwclient.idl
USERDEP__VWCLI="..\vwcommon\vwobject.idl"	

"..\vwcommon\vwclient.h"	"..\vwcommon\vwclient_i.c"	"..\vwcommon\vwclient_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWCLI)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwclient.h" /iid "vwclient_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwclient.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwclient.idl
USERDEP__VWCLI="..\vwcommon\vwobject.idl"	

"..\vwcommon\vwclient.h"	"..\vwcommon\vwclient_i.c"	"..\vwcommon\vwclient_p.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWCLI)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwclient.h" /iid "vwclient_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwclient.idl"
<< 
	

!ENDIF 

SOURCE=..\vwclient\vwcliobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwcliobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwcliobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Vwclient\vwcliocx.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwclient" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwcliocx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwclient" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwcliocx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwserver\connserv.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\connserv.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\connserv.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwserver\queue.cpp

"$(INTDIR)\queue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwserver\vwserve.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwserve.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwserve.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwserver.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwserver.idl
USERDEP__VWSER="..\vwcommon\vwobject.idl"	

"..\vwcommon\vwserver.h"	"..\vwcommon\vwserver_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWSER)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwserver.h" /iid "vwserver_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwserver.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwserver.idl
USERDEP__VWSER="..\vwcommon\vwobject.idl"	

"..\vwcommon\vwserver.h"	"..\vwcommon\vwserver_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWSER)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwserver.h" /iid "vwserver_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwserver.idl"
<< 
	

!ENDIF 

SOURCE=..\vwserver\wldinfo.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\wldinfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\wldinfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwserver\wldlist.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\wldlist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\wldlist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcomm\commcono.cpp

"$(INTDIR)\commcono.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcomm\commfunc.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\commfunc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\commfunc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcomm\commmsgo.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\commmsgo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\commmsgo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcomm\commobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\commobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\commobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcomm\tinfo.cpp

"$(INTDIR)\tinfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwcomm.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwcomm.idl
USERDEP__VWCOM="..\vwcommon\vwbuffer.idl"	

"..\vwcommon\vwcomm.h"	"..\vwcommon\vwcomm_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWCOM)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwcomm.h" /iid "vwcomm_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwcomm.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwcomm.idl
USERDEP__VWCOM="..\vwcommon\vwbuffer.idl"	

"..\vwcommon\vwcomm.h"	"..\vwcommon\vwcomm_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWCOM)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwcomm.h" /iid "vwcomm_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwcomm.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\vwodb.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwodb.idl

"..\vwcommon\vwodb.h"	"..\vwcommon\vwodb_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwodb.h" /iid "vwodb_i.c" /I "..\vwcommon" /out  "..\vwcommon"                                                                                                   "vwodb.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwodb.idl

"..\vwcommon\vwodb.h"	"..\vwcommon\vwodb_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwodb.h" /iid "vwodb_i.c" /I "..\vwcommon" /out  "..\vwcommon"                                                                                                   "vwodb.idl"
<< 
	

!ENDIF 

SOURCE=..\vwodb\vwodbobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwodbobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwodbobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Vwcommon\vwadmin.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\Vwcommon\vwadmin.idl
USERDEP__VWADM="..\vwcommon\vwobject.idl"	

"..\vwcommon\vwadmin_i.c"	"..\vwcommon\vwadmin.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWADM)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwadmin.h" /iid "vwadmin_i.c" /I "..\vwcommon" /out     "..\vwcommon" "vwadmin.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\Vwcommon\vwadmin.idl
USERDEP__VWADM="..\vwcommon\vwobject.idl"	

"..\vwcommon\vwadmin_i.c"	"..\vwcommon\vwadmin.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWADM)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwadmin.h" /iid "vwadmin_i.c" /I "..\vwcommon" /out     "..\vwcommon" "vwadmin.idl"
<< 
	

!ENDIF 

SOURCE=..\vwadmin\vwadmobj.cpp

"$(INTDIR)\vwadmobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\fastcomm\Toolkit\tools.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\tools.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\tools.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\fastcomm\commlayr.cpp

"$(INTDIR)\commlayr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\fastcomm\fastcono.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\fastcono.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\fastcono.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\fastcomm\fastmsgo.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\fastmsgo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\fastmsgo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\fastcomm\fastobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\fastobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\fastobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\vwanim.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwanim.idl

"..\vwcommon\vwanim.h"	"..\vwcommon\vwanim_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwanim.h" /iid "vwanim_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwanim.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwanim.idl

"..\vwcommon\vwanim.h"	"..\vwcommon\vwanim_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwanim.h" /iid "vwanim_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwanim.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\vwanim\vwanimob.cpp

"$(INTDIR)\vwanimob.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\vwanim\vwanmtob.cpp

"$(INTDIR)\vwanmtob.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\inetfile\dlthread.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwserver" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\dlthread.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwserver" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\dlthread.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\vwcommon\inetfile.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\inetfile.idl

"..\vwcommon\inetfile.h"	"..\vwcommon\inetfile_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "inetfile.h" /iid "inetfile_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "inetfile.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\inetfile.idl

"..\vwcommon\inetfile.h"	"..\vwcommon\inetfile_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "inetfile.h" /iid "inetfile_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "inetfile.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\inetfile\inetmgro.cpp

"$(INTDIR)\inetmgro.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\vwhtml\htmlwnd.cpp

"$(INTDIR)\htmlwnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\vwhtml\vwhelper.cpp

"$(INTDIR)\vwhelper.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwhtml.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwhtml.idl
USERDEP__VWHTM="..\vwcommon\vwclient.idl"	

"..\vwcommon\vwhtml.h"	"..\vwcommon\vwhtml_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWHTM)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwhtml.h" /iid "vwhtml_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwhtml.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwhtml.idl
USERDEP__VWHTM="..\vwcommon\vwclient.idl"	

"..\vwcommon\vwhtml.h"	"..\vwcommon\vwhtml_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)" $(USERDEP__VWHTM)
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwhtml.h" /iid "vwhtml_i.c" /I "..\vwcommon" /out                                                                                                    "..\vwcommon" "vwhtml.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\vwhtml\vwhtmlob.cpp

"$(INTDIR)\vwhtmlob.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\vwhtml\wbbrowsr.cpp

"$(INTDIR)\wbbrowsr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\vwstats\statmgr.cpp
SOURCE=..\tools\dirserv\attrib.cpp

"$(INTDIR)\attrib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\dirserv.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\dirserv.idl

"..\vwcommon\dirserv.h"	"..\vwcommon\dirserv_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn  /h "dirserv.h" /iid "dirserv_i.c" /I "..\vwcommon" /out                                                                        "..\vwcommon" "dirserv.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\dirserv.idl

"..\vwcommon\dirserv.h"	"..\vwcommon\dirserv_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn  /h "dirserv.h" /iid "dirserv_i.c" /I "..\vwcommon" /out                                                                        "..\vwcommon" "dirserv.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\dirserv\dqueue.cpp

"$(INTDIR)\dqueue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\dirserv\dserv.cpp

"$(INTDIR)\dserv.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\authntic\aobj.cpp

"$(INTDIR)\aobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\authntic.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\authntic.idl

"..\vwcommon\authntic.h"	"..\vwcommon\authntic_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn  /h "authntic.h" /iid "authntic_i.c" /I "..\vwcommon" /out                          "..\vwcommon"               "authntic.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\authntic.idl

"..\vwcommon\authntic.h"	"..\vwcommon\authntic_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn  /h "authntic.h" /iid "authntic_i.c" /I "..\vwcommon" /out                          "..\vwcommon"               "authntic.idl"
<< 
	

!ENDIF 

SOURCE=..\tools\authntic\authobj.cpp

"$(INTDIR)\authobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\tools\authntic\logon.cpp

"$(INTDIR)\logon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\avprof.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\avprof.idl

"..\vwcommon\avprof.h"	"..\vwcommon\avprof_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "avprof.h" /iid "avprof_i.c" /I "..\vwcommon" /out "..\vwcommon"                                        "avprof.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\avprof.idl

"..\vwcommon\avprof.h"	"..\vwcommon\avprof_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "avprof.h" /iid "avprof_i.c" /I "..\vwcommon" /out "..\vwcommon"                                        "avprof.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\profhelp.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\profhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\profhelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\property\avprof\profobj.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwsystem" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\profobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"vwobject.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\profobj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\property\menuitem\cmenuitm.cpp

"$(INTDIR)\cmenuitm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE="..\vwcommon\menuitem.idl"

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath="..\vwcommon\menuitem.idl"

"..\vwcommon\menuitem.h"	"..\vwcommon\menuitem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn  /h "menuitem.h" /iid "menuitem_i.c" /I "..\vwcommon" /out                             "..\vwcommon"            "menuitem.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath="..\vwcommon\menuitem.idl"

"..\vwcommon\menuitem.h"	"..\vwcommon\menuitem_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn  /h "menuitem.h" /iid "menuitem_i.c" /I "..\vwcommon" /out                             "..\vwcommon"            "menuitem.idl"
<< 
	

!ENDIF 

SOURCE=..\vwcommon\ocxproxy.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\ocxproxy.idl

"..\vwcommon\ocxproxy.h"	"..\vwcommon\ocxproxy_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "ocxproxy.h" /iid "ocxproxy_i.c" /I "..\vwcommon" /out                             "..\vwcommon"            "ocxproxy.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\ocxproxy.idl

"..\vwcommon\ocxproxy.h"	"..\vwcommon\ocxproxy_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "ocxproxy.h" /iid "ocxproxy_i.c" /I "..\vwcommon" /out                             "..\vwcommon"            "ocxproxy.idl"
<< 
	

!ENDIF 

SOURCE=..\property\ocxproxy\vwocx.cpp

"$(INTDIR)\vwocx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\system\artifact.cpp

"$(INTDIR)\artifact.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\system\avatar.cpp

"$(INTDIR)\avatar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\system\host.cpp

"$(INTDIR)\host.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\system\IWportal.cpp

"$(INTDIR)\IWportal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\system\portal.cpp

"$(INTDIR)\portal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\system\room.cpp

"$(INTDIR)\room.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\system\statemch.cpp

"$(INTDIR)\statemch.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\vwcommon\vwsysex.idl

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

InputPath=..\vwcommon\vwsysex.idl

"..\vwcommon\vwsysex.h"	"..\vwcommon\vwsysex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwsysex.h" /iid "vwsysex_i.c" /I "..\vwcommon" /out                          "..\vwcommon"               "vwsysex.idl"
<< 
	

!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

InputPath=..\vwcommon\vwsysex.idl

"..\vwcommon\vwsysex.h"	"..\vwcommon\vwsysex_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	midl /Oicf /no_warn /h "vwsysex.h" /iid "vwsysex_i.c" /I "..\vwcommon" /out                          "..\vwcommon"               "vwsysex.idl"
<< 
	

!ENDIF 

SOURCE=..\module\system\vwsysexo.cpp

!IF  "$(CFG)" == "vwsystem - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\vwcommon" /I "..\vwobject" /I "..\property\menuitem" /I "..\property\ocxproxy" /D "_DEBUG" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwsysexo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "vwsystem - Win32 Release MinSize"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O1 /I "." /I "..\vwcommon" /I "..\vwobject" /I "..\property\menuitem" /I "..\property\ocxproxy" /D "NDEBUG" /D "_ATL_DLL" /D "_WIN32_WINNT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "VWSYSTEM_BUILD" /D _WIN32_WINNT=0x0400 /D "_MERGE_PROXYSTUB" /Fp"$(INTDIR)\vwsyst60.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\vwsystem.pdb" /FD /c 

"$(INTDIR)\vwsysexo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\module\system\vwthing.cpp

"$(INTDIR)\vwthing.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\module\system\WebPage.cpp

"$(INTDIR)\WebPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\vwsyst60.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

