################################################################
# Makefile for SWI-Prolog 5.x
# $Id: Makefile,v 1.1.1.1 2002-03-07 06:07:38 dmiles Exp $
#
# Author:	Jan Wielemaker
#		jan@swi.psy.uva.nl
#		SWI
#		University of Amsterdam
#    		Roetersstraat 15
#		1018 WB  Amsterdam
#		The Netherlands
#
# Public targets:
# 
#	* make			Simply makes all programs in the current tree
#	* make install		Installs the libraries and public executables
#	* make install-version  Installs pl as pl-x.y.z in public directory
#	* make install-bins	Link executables from $(bindir)
#	* make install-arch	Install machine dependent files
#	* make install-libs	Install machine independent files
#	* make install-lman	Install man files in Prolog lib hierarchy
#	* make install-mt	Install multi-threaded version next to normal
#	* make check		Run some tests
#
# Copyright (C) University of Amsterdam, all rights reserved
# 
# Copyright policy:
#	
#	* GPL-2 (see file COPYING or http://www.gnu.org/)
#
#	* Licenses for use with proprietary software are available.
#	  See http://www.swi.pay.uva.nl/pub/SWI-Prolog/
#
################################################################

PLVERSION=5.0.1

PL=pl
PLV=pl-5.0.1
prefix=/opt/tomcat/webapps/inference/WEB-INF/swi
exec_prefix=${prefix}
srcdir=/opt/pl/src
vpath %.c /opt/pl/src
bindir=${exec_prefix}/bin
PLBASE=$(prefix)/lib/pl-$(PLVERSION)
# NOTE: Also defined in config.h!
BOOTFILE=boot32.prc
mandir=${prefix}/man
mansec=1
lmansec=3
smandir=$(mandir)/man$(mansec)
lmandir=$(mandir)/man$(lmansec)
plmandir=$(PLBASE)/man

CC=gcc
LDEXE=gcc
LIBS=-ldl -lreadline -lncurses -lm -lrt -lpthread 
AWK=mawk
SED=sed
ETAGS=etags
AR=ar
RANLIB=ranlib
ARCH=i686-linux
INSTALL=/usr/bin/install -c
INSTALL_PROGRAM=${INSTALL}
INSTALL_DATA=${INSTALL} -m 644
MKDIR=mkdir -p
REL_LN=$(srcdir)/rel-ln

COFLAGS=-O2
CWFLAGS=-Wall
CIFLAGS=
CMFLAGS=
CFLAGS=	$(CWFLAGS) $(COFLAGS) $(CIFLAGS) $(CMFLAGS)
LDFLAGS=-export-dynamic 

PLFOREIGN=@PLFOREIGN@

DEFSTARTUP=.plrc

PB=/opt/pl/boot
INCLUDEDIR=/opt/pl/include
EXPORTS=$(INCLUDEDIR)/SWI-Exports
DEF=$(PL).def
CINCLUDE=$(INCLUDEDIR)/SWI-Prolog.h
STREAMH=$(INCLUDEDIR)/SWI-Stream.h
STUBC=$(INCLUDEDIR)/stub.c
STARTUPPATH=$(PL).prc
RUNTIMEDIR=/opt/pl/runtime/$(ARCH)
PLLIB=libplmt.a
RUNTIME=$(RUNTIMEDIR)/$(PLLIB)
LIBRARYDIR=${PLBASE}/library

OBJ=	pl-atom.o pl-wam.o pl-stream.o pl-error.o pl-arith.o pl-bag.o \
	pl-comp.o pl-rc.o pl-dwim.o pl-ext.o pl-file.o pl-flag.o \
	pl-fmt.o pl-funct.o pl-gc.o pl-glob.o pl-itf.o pl-list.o \
	pl-load.o pl-modul.o pl-op.o pl-os.o pl-prims.o pl-pro.o \
	pl-proc.o pl-prof.o pl-read.o pl-rec.o pl-rl.o pl-setup.o \
	pl-sys.o pl-table.o pl-trace.o pl-util.o pl-wic.o pl-write.o \
	pl-term.o pl-buffer.o pl-thread.o pl-xterm.o \
	pl-feature.o pl-ctype.o
XOBJ=	 
RCOBJ=	rc/access.o rc/build.o rc/html.o rc/rc.o rc/util.o

DEPOBJ=	pl-main.o
EXT=	pl-extend.o
ALLOBJ= $(OBJ) $(DEPOBJ) $(EXT) $(XOBJ)

PLINIT=	$(PB)/init.pl

INCSRC=	pl-index.c pl-alloc.c pl-fli.c
SRC=	$(OBJ:.o=.c) $(DEPOBJ:.o=.c) $(EXT:.o=.c) $(INCSRC)
HDR=	config.h parms.h pl-buffer.h pl-ctype.h pl-incl.h pl-itf.h pl-main.h \
	pl-os.h pl-data.h

PLSRC=	/opt/pl/boot/syspred.pl /opt/pl/boot/toplevel.pl /opt/pl/boot/license.pl \
	/opt/pl/boot/make.pl /opt/pl/boot/sort.pl /opt/pl/boot/bags.pl /opt/pl/boot/apply.pl \
	/opt/pl/boot/list.pl /opt/pl/boot/writef.pl /opt/pl/boot/history.pl \
	/opt/pl/boot/profile.pl /opt/pl/boot/dwim.pl /opt/pl/boot/rc.pl \
	/opt/pl/boot/parms.pl /opt/pl/boot/autoload.pl /opt/pl/boot/qlf.pl \
	/opt/pl/boot/topvars.pl /opt/pl/boot/messages.pl /opt/pl/boot/load.pl
PLLIBS= MANUAL helpidx.pl help.pl explain.pl \
	qsave.pl shlib.pl statistics.pl system.pl \
	backcomp.pl gensym.pl listing.pl \
	bim.pl quintus.pl edinburgh.pl ctypes.pl files.pl \
	edit.pl emacs_interface.pl shell.pl check.pl \
	tty.pl readln.pl readutil.pl \
	am_match.pl oset.pl ordsets.pl occurs.pl \
	netscape.pl url.pl main.pl \
	threadutil.pl qpforeign.pl
PLWIN=	wise.pl dde.pl progman.pl registry.pl

all:	banner \
	headers rc-lib system startup index runtime \
	plld

.c.o:
	$(CC) -c -I. -I$(srcdir) -I$(srcdir)/rc $(CFLAGS) $< -o $@

system:		$(PL)
startup:	$(STARTUPPATH)
headers:	parms.h $(CINCLUDE) $(STREAMH) $(STUBC) 
runtime:	$(RUNTIME)


banner::
		@echo "****************"
		@echo "Making SWI-Prolog $(PLVERSION) for $(ARCH)"
		@echo "To be installed in $(bindir)/$(PL)"
		@echo "Home (libraries) in $(PLBASE)"
		@echo "****************"


$(PL):		$(ALLOBJ) rc/librc.a
		$(LDEXE) $(LDFLAGS) -o $(PL) $(ALLOBJ) -Lrc -lrc $(LIBS)


$(STARTUPPATH):	$(PLINIT) $(PLSRC) $(PL)
		./$(PL) -O -o $(STARTUPPATH) -b $(PLINIT)


$(RUNTIME):	$(OBJ) $(RCOBJ) $(DEPOBJ) $(XOBJ)
		if [ ! -d /opt/pl/runtime ]; then $(MKDIR) /opt/pl/runtime; fi
		if [ ! -d $(RUNTIMEDIR) ]; then $(MKDIR) $(RUNTIMEDIR); fi
		rm -f $@
		$(AR) r $@ $(OBJ) $(RCOBJ) $(DEPOBJ) $(XOBJ)
		$(RANLIB) $@

rc-lib:
		@(cd rc && $(MAKE) COFLAGS="$(COFLAGS)")

index:
		./$(PL) -x $(STARTUPPATH) \
			-f none -F none \
			-g make_library_index\(\'/opt/pl/library\'\) \
			-t halt

$(INCLUDEDIR):
		if [ ! -d $@ ]; then $(MKDIR) $@; fi

# Create SWI-Exports, used for importing and exporting symbols on behalf of
# the AIX version.

exports:	$(EXPORTS)
def:		$(DEF)

$(EXPORTS):	Makefile $(srcdir)/pl-itf.h $(INCLUDEDIR)
		echo \#\!$(bindir)/$(PL) > $@
		grep '^__pl_export' $(srcdir)/pl-itf.h $(srcdir)/pl-stream.h | \
		    $(SED) -e 's/.*[ 	*]\(_*PL_[a-zA-Z0-9_]*\).*/\1/' \
			   -e 's/.*[ 	*]\(S[a-zA-Z_]*\).*/\1/' | \
		    grep -v '_t$$' | grep -v '^PL_extension$$' | \
		    sort -u >> $@

# Create pl.def, required for exporting symbols from pl.exe for the CygWin
# platform

$(DEF):		$(srcdir)/pl-itf.h
		echo EXPORTS > $@
		grep '^__pl_export' $(srcdir)/pl-itf.h $(srcdir)/pl-stream.h | \
		    $(SED) -e 's/.*[ 	*]\(_*PL_[a-zA-Z0-9_]*\).*/\1/' \
			   -e 's/.*[ 	*]\(S[a-zA-Z_]*\).*/\1/' | \
		    grep -v '_t$$' | grep -v '^PL_extension$$' | \
		    sort -u >> $@

$(CINCLUDE):	$(srcdir)/pl-itf.h $(INCLUDEDIR)
		cp $(srcdir)/pl-itf.h $(CINCLUDE)

$(STREAMH):	$(srcdir)/pl-stream.h $(INCLUDEDIR)
		cp $(srcdir)/pl-stream.h $@

$(STUBC):	$(srcdir)/pl-extend.c $(INCLUDEDIR)
		$(SED) 's/"pl-itf.h"/<SWI-Prolog.h>/' $(srcdir)/pl-extend.c > $@

parms.h:	Makefile
		@echo "#define PLHOME       \"$(PLBASE)\""      > $@%
		@echo "#define DEFSTARTUP   \"$(DEFSTARTUP)\"" >> $@%
		@echo $(PLVERSION) | \
		$(AWK) -F. '{ printf("#define PLVERSION %d\n", $$1 * 10000 + $$2 * 100 + $$3); }' >> $@%
		@echo "#define ARCH	    \"$(ARCH)\""       >> $@%
		@echo "#define C_LIBS	    \"$(LIBS)\""       >> $@%
		@echo "#define C_CC	    \"$(CC)\""	       >> $@%
		@echo "#define C_LDFLAGS    \"$(LDFLAGS)\""    >> $@%
		@if cmp -s $@% $@; then rm $@%; else mv $@% $@; fi

pl-main.o:	parms.h
pl-setup.o:	parms.h
pl-wic.o:	parms.h
pl-funct.o:	$(srcdir)/pl-funct.ih
pl-atom.o:	$(srcdir)/pl-funct.ih
pl-wam.o:	pl-alloc.c pl-index.c pl-fli.c

$(srcdir)/pl-funct.ih:	$(srcdir)/ATOMS
		cd $(srcdir) ; $(AWK) -f defatom.awk < ATOMS

$(srcdir)/pl-atom.ih:	$(srcdir)/ATOMS
		cd $(srcdir) ; $(AWK) -f defatom.awk < ATOMS

# Hack to deal with __register_frame problems in different glibc 2 versions
# (Linux, compiled on SuSE 6.0, glibc 2.0.7, egcs 1.1.1)

frame.o:
		ar x `$(CC) -print-libgcc-file-name` $@

plld:		$(srcdir)/plld.c
		$(CC) -I. -I$(srcdir) $(CFLAGS) -o $@ $(srcdir)/plld.c

tags:		$(srcdir)/TAGS

$(srcdir)/TAGS:	$(SRC)
		(cd $(srcdir) && $(ETAGS) $(SRC) $(HDR))

check:
		./$(PL) -f $(srcdir)/test.pl -F none -g test,halt -t 'halt(1)'

install:	dv-install
install-version: install-arch install-libs install-vbins

dv-install:	install-arch install-libs install-bins


varibales:  
		@echo $(CC) varinfodb -c -I. -I$(srcdir) -I$(srcdir)/rc $(CFLAGS) -Lrc -lrc $(LIBS)


install-mt:	
		$(INSTALL_PROGRAM) $(PL) $(PLBASE)/bin/$(ARCH)/pl-mt
		$(REL_LN) $(PLBASE)/bin/$(ARCH)/pl-mt $(bindir)

install-bins:	install-arch
		-if [ ! -d $(bindir)  ]; then $(MKDIR) $(bindir); fi
		for f in $(PL) plld plrc; do \
		    $(REL_LN) $(PLBASE)/bin/$(ARCH)/$$f $(bindir); \
		done

install-vbins:	install-arch
		-if [ ! -d $(bindir)  ]; then $(MKDIR) $(bindir); fi
		$(REL_LN) $(PLBASE)/bin/$(ARCH)/$(PL) $(bindir)/$(PLV)
		for f in plld plrc; do \
		    $(REL_LN) $(PLBASE)/bin/$(ARCH)/$$f $(bindir); \
		done

install-arch:	idirs iruntime
		for f in $(PL) plld rc/plrc; do \
		    $(INSTALL_PROGRAM) $$f $(PLBASE)/bin/$(ARCH); \
		done

install-libs:	idirs iboot ilib iruntime iinclude iman idot
		$(INSTALL_DATA) $(STARTUPPATH) $(PLBASE)/$(BOOTFILE)
		echo . > $(PLBASE)/swipl
		echo .. > $(PLBASE)/bin/swipl
		cd $(PLBASE)/library; \
		   /opt/pl/bin/$(ARCH)/$(PL) \
			-f none \
			-g make_library_index\('.'\) \
			-t halt

installdirs=	bin bin/$(ARCH) lib lib/$(ARCH) man boot library \
		include runtime runtime/$(ARCH) dotfiles

iruntime::
		$(INSTALL_DATA) $(RUNTIME) $(PLBASE)/runtime/$(ARCH)
		$(RANLIB) $(PLBASE)/runtime/$(ARCH)/$(PLLIB)

install-implib::
		$(INSTALL_DATA) libplimp.a $(PLBASE)/runtime/$(ARCH)

idirs::
		if [ ! -d $(prefix)/lib  ]; then $(MKDIR) $(prefix)/lib; fi
		if [ ! -d $(PLBASE) ]; then $(MKDIR) $(PLBASE); fi
		for d in $(installdirs); do \
			if [ ! -d $(PLBASE)/$$d ]; then \
				$(MKDIR) $(PLBASE)/$$d; \
			fi; \
		done


iboot::		
		cd /opt/pl/boot; \
		for f in *.pl; do $(INSTALL_DATA) $$f $(PLBASE)/boot; done
ilib::		
		cd /opt/pl/library; \
		for f in $(PLLIBS); do \
			$(INSTALL_DATA) $$f $(PLBASE)/library; \
		done

idot::
		$(INSTALL_DATA) /opt/pl/dotfiles/dotplrc $(PLBASE)/dotfiles
		$(INSTALL_DATA) /opt/pl/dotfiles/dotxpcerc $(PLBASE)/dotfiles
		$(INSTALL_DATA) /opt/pl/dotfiles/README $(PLBASE)/dotfiles
		$(INSTALL_PROGRAM) /opt/pl/dotfiles/edit $(PLBASE)/dotfiles

iinclude::
		cd /opt/pl/include; \
		for f in SWI-Prolog.h SWI-Exports SWI-Stream.h stub.c; do \
		    if [ -f $$f ]; then \
			$(INSTALL_DATA) $$f $(PLBASE)/include; \
		    fi; \
		done
		if [ -d $(prefix)/include -a -w $(prefix)/include ]; then \
		    $(INSTALL_DATA) /opt/pl/include/SWI-Prolog.h $(prefix)/include; \
		    $(INSTALL_DATA) /opt/pl/include/SWI-Stream.h $(prefix)/include; \
		fi

iman:		install_rlman
		-if [ ! -d $(mandir) ]; then $(MKDIR) $(mandir); fi
		-if [ ! -d $(smandir)  ];    then $(MKDIR) $(smandir);     fi
		-$(INSTALL_DATA) $(srcdir)/pl.1 $(smandir)/$(PL).$(mansec)
		-$(INSTALL_DATA) $(srcdir)/plrc.1 $(smandir)/plrc.$(mansec)
		-$(INSTALL_DATA) $(srcdir)/plld.1 $(smandir)/plld.$(mansec)

install_rlman:
		-if [ ! -d $(lmandir) ]; then $(MKDIR) $(lmandir); fi
		-if [ ! -r $(lmandir)/readline.$(lmansec) ]; then \
		  $(INSTALL_DATA) $(srcdir)/readline.3 $(lmandir)/readline.$(lmansec); \
		fi

install-lman::
		$(INSTALL_DATA) pl.1 $(plmandir)/$(PL).$(mansec)
		$(INSTALL_DATA) $(srcdir)/plrc.1 $(plmandir)/plrc.$(mansec)
		$(INSTALL_DATA) $(srcdir)/plld.1 $(plmandir)/plld.$(mansec)
		$(INSTALL_DATA) $(srcdir)/readline.3 $(plmandir)/readline.$(lmansec)

#
# runtime-only environment installation
#

RTHOME=		$(prefix)/lib/$(PLV)
RTDIRS=		$(RTHOME) $(RTHOME)/bin $(RTHOME)/lib $(RTHOME)/lib/$(ARCH) \
		$(RTHOME)/man $(RTHOME)/bin/$(ARCH) $(RTHOME)/include

rt-install:
		@echo "****************"
		@echo "Creating runtime environment in $(RTHOME)"
		@echo "****************"
		@for d in $(RTDIRS); do \
		    if [ ! -d $$d ]; then \
			echo $(MKDIR) $$d; \
			$(MKDIR) $$d; \
		    fi; \
		done
		$(INSTALL_PROGRAM) $(PL) $(RTHOME)/bin/$(ARCH)/$(PL)
		$(INSTALL_DATA) pl.1 $(RTHOME)/man/$(PL).1
		echo . > $(RTHOME)/swipl
		echo .. > $(RTHOME)/bin/swipl
		$(INSTALL_DATA) /opt/pl/README.RT $(RTHOME)
		$(INSTALL_DATA) /opt/pl/include/SWI-Prolog.h $(RTHOME)/include
		$(INSTALL_DATA) /opt/pl/include/SWI-Stream.h $(RTHOME)/include

#
# Cleanup
#

clean:
		@(cd rc && $(MAKE) clean)
		rm -f *.o a.out core *~ #* parms.h

distclean:	clean
		@(cd rc && $(MAKE) distclean)
		rm -rf $(INCLUDEDIR) $(RUNTIMEDIR)
		rm -f /opt/pl/library/INDEX.pl
		rm -f Makefile config.h config.status parms.h
		rm -f swipl swiplbin pl.prc
		rm -f $(PL) plld pl.1 $(PL).exe plld.exe libplimp.a $(DEF)
		rm -f config.log config.cache

realclean:	clean
		rm -f $(STARTUPPATH)
		rm -f config.log config.cache
		rm -rf $(PL) /opt/pl/startup /opt/pl/include /opt/pl/runtime

