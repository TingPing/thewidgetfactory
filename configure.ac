dnl Process this file with autoconf to produce a configure script.

AC_INIT([twf], [0.5])
AM_INIT_AUTOMAKE([1.11 dist-bzip2 subdir-objects no-define foreign])
AM_SILENT_RULES([yes])

AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

pkg_modules="gtk+-2.0 >= 2.0.0 libglade-2.0"
PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

GETTEXT_PACKAGE=twf
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])

dnl Add the languages which your application supports here.
ALL_LINGUAS=""
AM_GLIB_GNU_GETTEXT

AM_PROG_LIBTOOL

dnl Define data dir
twf_data_dir='$(datadir)/twf'
AC_SUBST(twf_data_dir)

AC_CONFIG_FILES([
Makefile
src/Makefile
po/Makefile.in
])

AC_OUTPUT
