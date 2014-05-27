## Process this file with automake to produce Makefile.in

AM_CFLAGS = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DTWF_DATA=\""$(twf_data_dir)"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = twf

twf_SOURCES = \
	main.c \
	themes.c themes.h

twf_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

twf_datadir = $(twf_data_dir)
twf_data_DATA = twf.glade

EXTRA_DIST = $(twf_data_DATA)
