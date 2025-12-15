## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = projet_gym

projet_gym_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	equipement.c equipement.h \
	callbacks.c callbacks.h

projet_gym_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

