dnl Process this file with autoconf to produce a configure script.

# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT(egauche, [0.0.0], [takebi@laafc.net])
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([1.8 foreign])
AM_CONFIG_HEADER([config.h])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])
define([AC_LIBTOOL_LANG_GCJ_CONFIG], [:])
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(E, [enlightenment])
release=$(pkg-config --variable=release enlightenment)
MODULE_ARCH="$host_os-$host_cpu-$release"
AC_SUBST(MODULE_ARCH)
AC_DEFINE_UNQUOTED(MODULE_ARCH, "$MODULE_ARCH", "Module architecture")

# Find edje_cc
PKG_CHECK_MODULES(EDJE, [edje])
AC_ARG_WITH(edje-cc,
  AC_HELP_STRING([--with-edje-cc=PATH], [specify a specific path to edje_cc]),
  [
    v=$withval;
    EDJE_CC=$v
  ],[
    EDJE_CC=$(pkg-config --variable=prefix edje)/bin/edje_cc
  ])
AC_SUBST(EDJE_CC)
AC_MSG_CHECKING([Which edje_cc to use])
AC_MSG_RESULT(${EDJE_CC})

datadir=$(pkg-config --variable=modules enlightenment)/${PACKAGE}
AC_ARG_ENABLE(homedir-install,
  AS_HELP_STRING([--enable-homedir-install], [Install module in homedir]),
  [ datadir="${HOME}/.e/e/modules/${PACKAGE}" ]
)

AC_ARG_WITH(gauche-config,
  AC_HELP_STRING([--with-gauche-config=PATH], [use gauche-config specified]),
  [
    GAUCHE_CONFIG=$withval;
    echo "using "$GAUCHE_CONFIG" for gauche-config";
  ],[
    PROG="gauche-config";
    AC_PATH_PROG(GAUCHE_CONFIG, $PROG, "", $PATH)
  ])
gauche_cflags=`$GAUCHE_CONFIG -I`
gauche_libs="`$GAUCHE_CONFIG -L` `$GAUCHE_CONFIG -l`"
AC_SUBST(gauche_cflags)
AC_SUBST(gauche_libs)

AC_OUTPUT([
Makefile
module.desktop
src/Makefile
], [])
