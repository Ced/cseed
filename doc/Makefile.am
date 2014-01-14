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
SUBDIRS =

#############################################################################

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = \
	htmldoc.tar.gz \
	cseed.pdf

TEXINFO_TEX = cseed.texi
#info_TEXINFOS = cseed.texi

DOCDIR = htmldoc
DOXYFILE = Doxyfile
DOXYGEN = @DOXYGEN@

# The following requires a fixed version of the Emacs 19.30 etags.
ETAGS_ARGS = --lang=none \
 --regex='/^@node[ \t]+\([^,]+\)/\1/' $(srcdir)/doc/cseed.texi
TAGS_DEPENDENCIES = cseed.texi $(DOXYFILE).in

htmldoc.tar.gz:
	mkdir -p $(top_srcdir)/doc/htmldoc
	$(DOXYGEN) $(DOXYFILE)
	tar czf htmldoc.tar.gz htmldoc
