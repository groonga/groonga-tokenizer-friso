AC_PREREQ(2.59)
AC_INIT([groonga-tokenizer-friso], 1.0.0, [hayashi@clear-code.com])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([tokenizers/friso.c])
AM_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC

AC_PROG_LIBTOOL

if test "$GCC" = "yes"; then
  CFLAGS="$CFLAGS -Wall -Wextra"
  CFLAGS="$CFLAGS -Wmissing-declarations -Wmissing-prototypes"
fi

AC_ARG_ENABLE(debug,
  [AS_HELP_STRING([--enable-debug],
                  [use debug flags (default=no)])],
  [friso_debug="$enableval"],
  [friso_debug="no"])
if test "x$friso_debug" != "xno"; then
  if test "$CLANG" = "yes"; then
    CFLAGS="$CFLAGS -O0 -g"
  elif test "$GCC" = "yes"; then
    CFLAGS="$CFLAGS -O0 -g3"
  fi
fi

AC_ARG_WITH(friso,
            [AS_HELP_STRING([--with-friso=PATH],
            [specify path to friso directory. (default=/usr)])],
            [friso_dir="$withval"],
            [friso_dir="/usr"])
FRISO_CFLAGS="-I$friso_dir/include"
FRISO_LIBS="-lfriso"
FRISO_LDFLAGS="-L$friso_dir/lib ${FRISO_LIBS}"

AC_SUBST(FRISO_CFLAGS)
AC_SUBST(FRISO_LIBS)
AC_SUBST(FRISO_LDFLAGS)

AC_ARG_WITH(friso-ini,
            [AS_HELP_STRING([--with-friso-ini=PATH],
            [specify path to friso configuration file.])],
            [friso_ini="$withval"],
            [friso_ini=""])
if test "x$friso_ini" != "x"; then
   FRISO_INI_PATH="$friso_ini"
else
   AC_ERROR("")
fi
AC_DEFINE_UNQUOTED([FRISO_INI_PATH], ["$friso_ini"], ["Friso ini file"])

GROONGA_REQUIRED_VERSION=3.0.5
PKG_CHECK_MODULES([GROONGA], [groonga >= ${GROONGA_REQUIRED_VERSION}])

_PKG_CONFIG(GROONGA_PLUGINS_DIR, [variable=pluginsdir],    [groonga])
_PKG_CONFIG(GROONGA,             [variable=groonga],       [groonga])

GROONGA_PLUGINS_DIR="${pkg_cv_GROONGA_PLUGINS_DIR}"
GROONGA="${pkg_cv_GROONGA}"

AC_SUBST(GROONGA_PLUGINS_DIR)
AC_SUBST(GROONGA)

tokenizers_pluginsdir="\${GROONGA_PLUGINS_DIR}/tokenizers"
AC_SUBST(tokenizers_pluginsdir)

AC_CONFIG_FILES([
  Makefile
  tokenizers/Makefile
  test/Makefile
])

AC_OUTPUT

echo "$PACKAGE_NAME $PACKAGE_VERSION configuration:"
echo "-----------------------"
echo "  Compiler:              ${CC}"
echo "  CFLAGS:                ${CFLAGS}"
echo "  CXXFLAGS:              ${CXXFLAGS}"
echo "  Libraries:             ${LIBS}"
echo
echo "Groonga"
echo "  CFLAGS:                ${GROONGA_CFLAGS}"
echo "  Libraries:             ${GROONGA_LIBS}"
echo "  install directory:     ${tokenizers_pluginsdir}"
echo
echo "Friso"
echo "  CFLAGS:                ${FRISO_CFLAGS}"
echo "  LDFLAGS:               ${FRISO_LDFLAGS}"
echo "  Libraries:             ${FRISO_LIBS}"
echo "  Configuration:         ${FRISO_INI_PATH}"
