dnl
dnl   /*------------------------------------------------------------------+
dnl    |                             Spot                                 |
dnl    |------------------------------------------------------------------|
dnl    |                          autoconf.ac                             |
dnl    |------------------------------------------------------------------|
dnl    |                   First version: 14/01/2014                      |
dnl    +------------------------------------------------------------------+
dnl
dnl +------------------------------------------------------------------------+
dnl |  / __)/ __)( ___)( ___)(  _ \                                          |
dnl | ( (__ \__ \ )__)  )__)  )(_) )     Chunky High-Level Compiler Seed     |
dnl |  \___)(___/(____)(____)(____/                                          |
dnl +------------------------------------------------------------------------+
dnl | Copyright (C) 2014 University of Strasbourg                            |
dnl |                                                                        |
dnl | This library is free software; you can redistribute it and/or modify   |
dnl | it under the terms of the GNU Lesser General Public License as         |
dnl | published by the Free Software Foundation; either version 2.1 of the   |
dnl | License, or (at your option) any later version.                        |
dnl |                                                                        |
dnl | This library is distributed in the hope that it will be useful but     |
dnl | WITHOUT ANY WARRANTY; without even the implied warranty of             |
dnl | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       |
dnl | Lesser General Public License for more details.                        |
dnl |                                                                        |
dnl | You should have received a copy of the GNU Lesser General Public       |
dnl | License along with this software; if not, write to the Free Software   |
dnl | Foundation, Inc., 51 Franklin Street, Fifth Floor,                     |
dnl | Boston, MA  02110-1301  USA                                            |
dnl |                                                                        |
dnl | Spot, the Chunky High-Level Compiler Seed                              |
dnl | Written by Cedric Bastoul, Cedric.Bastoul@unistra.fr                   |
dnl +------------------------------------------------------------------------*/
dnl
dnl Input file for autoconf to build a configuration shellscript.

m4_define([version_major], [0])
m4_define([version_minor], [0])
m4_define([version_revision], [1])

AC_PREREQ(2.53)
dnl Fill here the @bug email adress.
AC_INIT([spot], [version_major.version_minor.version_revision],
        [cedric.bastoul@unistra.fr])
dnl A common file, which serve as a test.
AC_CONFIG_SRCDIR([include/spot/spot.h])
dnl Put as most as possible configuration files to an auxialiry
dnl directory.
AC_CONFIG_AUX_DIR(autoconf)
AC_CONFIG_MACRO_DIR([m4])

dnl Initialize automake.
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])


dnl /**************************************************************************
dnl  *                              Checking                                  *
dnl  **************************************************************************/


dnl Checks for programs.
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_CHECK_PROG(CD, cd)
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LEX
AC_PROG_YACC
AC_CHECK_PROGS(DOXYGEN,doxygen,doxygen)
  
AX_CC_MAXOPT
AC_SUBST(CFLAGS_WARN)
AX_CFLAGS_WARN_ALL(CFLAGS_WARN)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([errno.h stddef.h stdlib.h string.h strings.h unistd.h])

dnl Checks for library functions.
AC_CHECK_FUNCS(strtol)


dnl /**************************************************************************
dnl  *                            Option setting                              *
dnl  **************************************************************************/

dnl /**************************************************************************
dnl  *                            Where is GMP?                               *
dnl  **************************************************************************/

AX_SUBMODULE(gmp,system|build,system)

need_get_memory_functions=false
case "$with_gmp" in
build)
    CPPFLAGS="-I$with_gmp_builddir $CPPFLAGS"
    LDFLAGS="-L$with_gmp_builddir/$lt_cv_objdir $LDFLAGS"
    ;;
system)
    if test "x$with_gmp_prefix" != "x"; then
	CPPFLAGS="-I$with_gmp_prefix/include $CPPFLAGS"
    fi
    
    if test "$with_gmp_exec_prefix" != "yes" ; then
	LDFLAGS="-L$with_gmp_exec_prefix/lib $LDFLAGS"
    fi
    ;;
esac
case "$with_gmp" in
build|system)
    AC_CHECK_HEADER(gmp.h,
                    [],
                    [AC_MSG_ERROR(Can't find gmp headers.)])
    AC_CHECK_LIB(gmp,
                 __gmpz_init,
                 [LIBS="$LIBS -lgmp"],
                 [AC_MSG_ERROR(Can't find gmp library.)])
    AC_CHECK_DECLS(mp_get_memory_functions,[],[
	    need_get_memory_functions=true
    ],[#include <gmp.h>])
    ;;
esac
AM_CONDITIONAL(NEED_GET_MEMORY_FUNCTIONS,
		test x$need_get_memory_functions = xtrue)

dnl /**************************************************************************
dnl  *                       Where is the isl Library?                        *
dnl  **************************************************************************/

AX_SUBMODULE(isl,system|build|bundled,bundled)

AC_SUBST(ISL_CPPFLAGS)
AC_SUBST(ISL_LDFLAGS)
AC_SUBST(ISL_LIBS)
case "$with_isl" in
bundled)
	ISL_CPPFLAGS="-I$srcdir/usr/src/isl/include -Iusr/src/isl/include"
	ISL_LIBS="$srcdir/usr/src/isl/libisl.la"
	;;
build)
	ISL_CPPFLAGS="-I$isl_srcdir/include -I$with_isl_builddir/include"
	ISL_LIBS="$with_isl_builddir/libisl.la"
	;;
system)
	ISL_CPPFLAGS="-I$with_isl_prefix/include"
	ISL_LIBS="$with_isl_prefix/lib/libisl.la -lisl"
esac
AM_CONDITIONAL(BUNDLED_ISL, test $with_isl = bundled)

dnl /**************************************************************************
dnl  *                      Where is the OpenScop Library?                    *
dnl  **************************************************************************/

AX_SUBMODULE(osl,system|build|bundled,bundled)

AC_SUBST(OSL_CPPFLAGS)
AC_SUBST(OSL_LDFLAGS)
AC_SUBST(OSL_LIBS)
case "$with_osl" in
bundled)
	OSL_CPPFLAGS="-I$srcdir/usr/src/osl/include -Iusr/src/osl/include"
	OSL_LIBS="$srcdir/usr/src/osl/libosl.la"
	;;
build)
	OSL_CPPFLAGS="-I$osl_srcdir/include -I$with_osl_builddir/include"
	OSL_LIBS="$with_osl_builddir/libosl.la"
	;;
system)
	OSL_CPPFLAGS="-I$with_osl_prefix/include"
	OSL_LIBS="$with_osl_prefix/lib/libosl.la -losl"
esac
AM_CONDITIONAL(BUNDLED_OSL, test $with_osl = bundled)

dnl /**************************************************************************
dnl  *                             Where is Clan ?                            *
dnl  **************************************************************************/

AX_SUBMODULE(clan,system|build|bundled,bundled)

AC_SUBST(CLAN_CPPFLAGS)
AC_SUBST(CLAN_LDFLAGS)
AC_SUBST(CLAN_LIBS)
case "$with_clan" in
bundled)
	CLAN_CPPFLAGS="-I$srcdir/usr/src/clan/include -Iusr/src/clan/include -DCLAN_LINKED"
	CLAN_LIBS="$srcdir/usr/src/clan/libclan.la"
	;;
build)
	CLAN_CPPFLAGS="-I$clan_srcdir/include -I$with_clan_builddir/include -DCLAN_LINKED"
	CLAN_LIBS="$with_clan_builddir/libclan.la"
	;;
system)
	CLAN_CPPFLAGS="-I$with_clan_prefix/include -DCLAN_LINKED"
	CLAN_LIBS="$with_clan_prefix/lib/libclan.la -lclan"
esac
AM_CONDITIONAL(BUNDLED_CLAN, test $with_clan = bundled)

dnl /**************************************************************************
dnl  *                             Where is Cloog ?                           *
dnl  **************************************************************************/

AX_SUBMODULE(cloog,system|build|bundled,bundled)

AC_SUBST(CLOOG_CPPFLAGS)
AC_SUBST(CLOOG_LDFLAGS)
AC_SUBST(CLOOG_LIBS)
case "$with_cloog" in
bundled)
	CLOOG_CPPFLAGS="-I$srcdir/usr/src/cloog/include -Iusr/src/cloog/include -DCLOOG_LINKED"
  	CLOOG_LIBS="$srcdir/usr/src/cloog/libcloog-isl.la"
	;;
build)
	CLOOG_CPPFLAGS="-I$cloog_srcdir/include -I$with_cloog_builddir/include -DCLOOG_LINKED"
	CLOOG_LIBS="$with_cloog_builddir/libcloog-isl.la"
	;;
system)
	CLOOG_CPPFLAGS="-I$with_cloog_prefix/include -DCLOOG_LINKED"
	CLOOG_LIBS="$with_cloog_prefix/lib/libcloog-isl.la -lcloog-isl"
esac
AM_CONDITIONAL(BUNDLED_CLOOG, test $with_cloog = bundled)


dnl /**************************************************************************
dnl  *                            Substitutions                               *
dnl  **************************************************************************/


dnl Substitutions to do.
AC_SUBST(ac_aux_dir)
AC_SUBST(abs_top_srcdir)

AC_SUBST(VERSION_MAJOR)
AC_SUBST(VERSION_MINOR)
AC_SUBST(VERSION_REVISION)

dnl Configure Makefiles.
AC_CONFIG_FILES([
	Makefile
	doc/Makefile
	doc/Doxyfile
	include/spot/macros.h
	],
	[test -z "$CONFIG_HEADERS" || echo timestamp > source/stamp-h.in])

if test $with_isl = bundled; then
	AC_CONFIG_SUBDIRS(usr/src/isl)
fi
if test $with_osl = bundled; then
	AC_CONFIG_SUBDIRS(usr/src/osl)
fi
if test $with_clan = bundled; then
	AC_CONFIG_SUBDIRS(usr/src/clan)
fi
if test $with_cloog = bundled; then
	AC_CONFIG_SUBDIRS(usr/src/cloog)
fi
AC_CONFIG_COMMANDS_POST([
	ac_configure_args="$ac_configure_args $spot_configure_args"
])

AC_OUTPUT

echo "             /*-----------------------------------------------+"
echo "              |           Spot configuration is OK           |"
echo "              +-----------------------------------------------*/"
echo "It appears that your system is OK to start Spot compilation. You need"
echo "now to type \"make\". Lastly type \"make install\" to install Spot on"
echo "your system (log as root if necessary)."
