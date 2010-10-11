ACLOCAL_AMFLAGS = -I m4
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I. -I$(top_srcdir) -I$(includedir) @E_CFLAGS@ @gauche_cflags@

pkgdir = $(datadir)/$(MODULE_ARCH)
pkg_LTLIBRARIES = module.la
module_la_SOURCES = \
e_mod_main.h e_mod_main.c \
e_mod_gauche.h e_mod_gauche.c \
ew.h ew.c

module_la_LIBADD = @E_LIBS@ @gauche_libs@
module_la_LDFLAGS = -module -avoid-version
module_la_DEPENDENCIES = $(top_builddir)/config.h
