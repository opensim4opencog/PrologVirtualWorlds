!IF "$(OS)" == "Windows_NT"
NT=1
!ELSE
NT=0
!ENDIF

!IF "$(NT)" == "1"
DLLDIR=%windir%\system32
COPYPARM=
DELCMD=rd /s /q
DELCMD2=del /q
!ELSE
DLLDIR=%windir%\system
COPYPARM=/y
DELCMD=deltree /y
DELCMD2=del
!ENDIF

!IF "$(ALL)" == "1"
NOPTS=-a
!ENDIF

!IF "$(DEBUG)" == "1"
TARGET=Win32 Debug
ATLTARGET=Win32 Debug
ATLOUTDIR=Debug
NMAKESUF=DEBUG=1
!ELSE
TARGET=Win32 Release
ATLTARGET=Win32 Release MinSize
ATLOUTDIR=ReleaseMinSize
NMAKESUF=RELEASE=1
!ENDIF


all: studio
studio: small studui
small: core extensions exemplars ui
core: system apps

system: t_vwsystem 
extensions: t_d2d t_vwmm t_vwstudio
exemplars: t_vwfound
ui: t_uictrl t_vwrendvw
apps: t_sengine t_vwsV2
studui: t_objexplr

clean:
	cd vwsystem
	-$(DELCMD) debug
	-$(DELCMD) releaseminsize
	-$(DELCMD2) *_i.c
	-$(DELCMD2) *.tlb
	cd ..

	cd vwmm
	-$(DELCMD) debug
	-$(DELCMD) releaseminsize
	-$(DELCMD2) *_i.c
	-$(DELCMD2) *.tlb
	cd ..

	cd vwstudio
	-$(DELCMD) debug
	-$(DELCMD) releaseminsize
	-$(DELCMD2) *_i.c
	-$(DELCMD2) *.tlb
	cd ..

	cd serverV2
	-$(DELCMD) debug
	-$(DELCMD) release
	cd ..

	cd d2d
	-$(DELCMD) debug
	-$(DELCMD) release
	-$(DELCMD2) d2d.opt
	cd mmimage
	-$(DELCMD) debug
	-$(DELCMD) release
	-$(DELCMD2) mmimage.opt
	-$(DELCMD2) lib\mmimage.lib
	-$(DELCMD2) lib\mmimageD.lib
	cd ..\d2dutils
	-$(DELCMD) debug
	-$(DELCMD) release
	-$(DELCMD2) lib\d2dutil.lib
	-$(DELCMD2) lib\d2dutilD.lib
	cd ..\..

	cd vwocx\vwrendvw
	-$(DELCMD) debug
	-$(DELCMD) release
	cd ..\..

	cd vwocx\uictrl
	-$(DELCMD) debug
	-$(DELCMD) release
	cd ..\..

	cd vwocx\objexplr
	-$(DELCMD) debug
	-$(DELCMD) release
	cd ..\..

	cd sengine
	-$(DELCMD) debug
	-$(DELCMD) release
	cd ..

	cd module\foundatn
	-$(DELCMD) debug
	-$(DELCMD) release
	cd ..\..

	cd vwcommon
	-$(DELCMD2) *_i.c
	-$(DELCMD2) *_p.c
	-$(DELCMD2) *.tlb
	cd ..


t_vwsystem:
	cd vwsystem
	-nmake $(NOPTS) CFG="vwsystem - $(ATLTARGET)" -f vwsyst60.mak
	cd ..

t_d2d:
	cd d2d
	-nmake $(NOPTS) CFG="d2d - $(TARGET)" -f d2d60.mak
	cd ..

t_vwsV2:
	cd serverV2
	-nmake $(NOPTS) CFG="ServerV2 - $(TARGET)" -f srvV260.mak
	cd ..

t_vwmm:
	cd vwmm
	-nmake $(NOPTS) CFG="vwmm - $(ATLTARGET)" -f vwmm60.mak
	cd ..

t_vwstudio:
	cd vwstudio
	-nmake $(NOPTS) CFG="vwstudio - $(ATLTARGET)" -f vwstud60.mak
	cd ..

t_vwfound:
	cd module\foundatn
	-nmake $(NOPTS) CFG="vwfound - $(TARGET)" -f vwfoun60.mak
	cd ..\..
	
t_sengine:
	cd sengine
	-nmake $(NOPTS) CFG="SEngine - $(TARGET)" -f sengin60.mak
	cd ..

t_vwrendvw:
	cd vwocx\vwrendvw
	-nmake $(NOPTS) CFG="VWRendVw - $(TARGET)" -f vwrend60.mak
	cd ..\..

t_uictrl:
	cd vwocx\uictrl
	-nmake $(NOPTS) CFG="uictrl - $(TARGET)" -f uictrl60.mak
	cd ..\..

t_objexplr:
	cd vwocx\objexplr
	-nmake $(NOPTS) CFG="objexplr - $(TARGET)" -f objexp60.mak
	cd ..\..
