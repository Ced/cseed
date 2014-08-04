#
#   /*------------------------------------------------------------------+
#    |                             Spot                                |
#    |------------------------------------------------------------------|
#    |                          makefile.am                             |
#    |------------------------------------------------------------------|
#    |                   First version: 14/01/2014                      |
#    +------------------------------------------------------------------+
#
# +------------------------------------------------------------------------+
# |  / __)/ __)( ___)( ___)(  _ \                                          |
# | ( (__ \__ \ )__)  )__)  )(_) )     Chunky High-Level Compiler Seed     |
# |  \___)(___/(____)(____)(____/                                          |
# +------------------------------------------------------------------------+
# | Copyright (C) 2014 University of Strasbourg                            |
# |                                                                        |
# | This library is free software; you can redistribute it and/or modify   |
# | it under the terms of the GNU Lesser General Public License as         |
# | published by the Free Software Foundation; either version 2.1 of the   |
# | License, or (at your option) any later version.                        |
# |                                                                        |
# | This library is distributed in the hope that it will be useful but     |
# | WITHOUT ANY WARRANTY; without even the implied warranty of             |
# | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       |
# | Lesser General Public License for more details.                        |
# |                                                                        |
# | You should have received a copy of the GNU Lesser General Public       |
# | License along with this software; if not, write to the Free Software   |
# | Foundation, Inc., 51 Franklin Street, Fifth Floor,                     |
# | Boston, MA  02110-1301  USA                                            |
# |                                                                        |
# | Spot, the Chunky High-Level Compiler Seed                             |
# | Written by Cedric Bastoul, Cedric.Bastoul@unistra.fr                   |
# +------------------------------------------------------------------------*/
#
# Makefile.am (or makefile if generated) of Spot.
# Makefile.am is not a makefile, you must run the 'autogen.sh' THEN the
# configure shellscript to generate the Makefile thanks to this file.
#
#############################################################################

AUTOMAKE_OPTIONS = subdir-objects

if BUNDLED_ISL
    MAYBE_ISL = usr/src/isl
    ISL_LA = $(top_builddir)/usr/src/isl/libisl.la
endif
if BUNDLED_OSL
    MAYBE_OSL = usr/src/osl
    OSL_LA = $(top_builddir)/usr/src/osl/libosl.la
endif
if BUNDLED_CLAN
    MAYBE_CLAN = usr/src/clan
    CLAN_LA = $(top_builddir)/usr/src/clan/libclan.la
endif
if BUNDLED_CLOOG
    MAYBE_CLOOG = usr/src/cloog
    CLOOG_LA = $(top_builddir)/usr/src/cloog/libcloog-isl.la
endif

SUBDIRS         = $(MAYBE_ISL) $(MAYBE_OSL) $(MAYBE_CLAN) $(MAYBE_CLOOG) doc
DIST_SUBDIRS    = $(MAYBE_ISL) $(MAYBE_OSL) $(MAYBE_CLAN) $(MAYBE_CLOOG) doc
ACLOCAL_AMFLAGS = -I m4

#############################################################################

bin_PROGRAMS    = spot
lib_LTLIBRARIES = libspot.la

#############################################################################

pkginclude_HEADERS = \
	include/spot/spot.h \
	include/spot/macros.h

DEFAULT_INCLUDES = -I.
INCLUDES = -I$(top_builddir)/include -I$(top_srcdir)/include
AM_CFLAGS = $(CFLAGS_WARN)

#############################################################################

libspot_la_LIBADD   = @ISL_LIBS@ @OSL_LIBS@ @CLAN_LIBS@ @CLOOG_LIBS@ $(ISL_LA) $(OSL_LA) $(CLAN_LA) $(CLOOG_LA)
libspot_la_CPPFLAGS = @ISL_CPPFLAGS@ @OSL_CPPFLAGS@ @CLAN_CPPFLAGS@ @CLOOG_CPPFLAGS@
libspot_la_SOURCES  = source/spot.c

#############################################################################

LDADD             = @ISL_LIBS@ @OSL_LIBS@ @CLAN_LIBS@ @CLOOG_LIBS@ libspot.la
spot_CPPFLAGS     = @ISL_CPPFLAGS@ @OSL_CPPFLAGS@ @CLAN_CPPFLAGS@ @CLOOG_CPPFLAGS@
spot_DEPENDENCIES = libspot.la
spot_SOURCES      = source/main.c

#############################################################################

MAINTAINERCLEANFILES = \
	Makefile.in \
	aclocal.m4 \
	configure \
	$(AUX_DIST)

#############################################################################

dist-hook:
	(cd $(distdir) && mkdir -p $(ac_aux_dir))
	for file in $(AUX_DIST); do \
	  cp $$file $(distdir)/$$file; \
	done

#############################################################################
