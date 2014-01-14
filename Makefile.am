#
#   /*------------------------------------------------------------------+
#    |                             CSeed                                |
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
# | CSeed, the Chunky High-Level Compiler Seed                             |
# | Written by Cedric Bastoul, Cedric.Bastoul@unistra.fr                   |
# +------------------------------------------------------------------------*/
#
# Makefile.am (or makefile if generated) of CSeed.
# Makefile.am is not a makefile, you must run the 'autogen.sh' THEN the
# configure shellscript to generate the Makefile thanks to this file.
#
#############################################################################

if BUNDLED_OSL
    MAYBE_OSL = osl
    OSL_LA = $(top_builddir)/osl/source/libosl.la
endif

SUBDIRS         = $(MAYBE_OSL) doc
DIST_SUBDIRS    = $(MAYBE_OSL) doc
ACLOCAL_AMFLAGS = -I m4

#############################################################################

bin_PROGRAMS    = cseed
lib_LTLIBRARIES = libcseed.la

#############################################################################

pkginclude_HEADERS = \
	include/cseed/cseed.h

DEFAULT_INCLUDES = -I.
INCLUDES = -I$(top_builddir)/include -I$(top_srcdir)/include
AM_CFLAGS = $(CFLAGS_WARN)
YFLAGS = -y -d

#############################################################################

libcseed_la_LIBADD   = @OSL_LIBS@ $(OSL_LA)
libcseed_la_CPPFLAGS = @OSL_CPPFLAGS@ -g
libcseed_la_LDFLAGS  = @OSL_LDFLAGS@
libcseed_la_SOURCES  =

#############################################################################

LDADD              = @OSL_LIBS@ libcseed.la
cseed_CPPFLAGS     = @OSL_CPPFLAGS@ -g
cseed_LDFLAGS      = @OSL_LDFLAGS@ # TO BE REMOVED
cseed_DEPENDENCIES = libcseed.la
cseed_SOURCES      = source/cseed.c

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
