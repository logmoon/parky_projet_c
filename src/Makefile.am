## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = parky

parky_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	windows.c windows.h \
	gerer_park.c gerer_park.h \
	service.c service.h \
	citoyen.c citoyen.h \
	reservation.c reservation.h \
	avis.c avis.h
	agent.c agent.h

parky_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)
