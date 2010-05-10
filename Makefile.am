ACLCAL_AMFLAGS = -I m4
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess config.h.in \
config.sub configure depcomp install-sh ltmain.sh \
missing config.rpath mkinstalldirs

SUBDIRS = src

filesdir = $(datadir)
files_DATA = module.desktop e-module-egauche.edj

EXTRA_DIST = \
module.desktop.in \
e-module-egauche.edc \
icon-egauche.png

EDJE_FLAGS = -v -id $(top_srcdir)

%.edj: %.edc
	$(EDJE_CC) $(EDJE_FLAGS) $< $@

clean-local:
	rm -rf *.edj module.desktop *~
