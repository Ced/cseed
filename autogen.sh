#!/bin/sh
autoreconf -i
if test -f usr/src/isl/autogen.sh; then
	(cd usr/src/isl; ./autogen.sh)
fi
if test -f usr/src/osl/autogen.sh; then
	(cd usr/src/osl; ./autogen.sh)
fi
if test -f usr/src/clan/autogen.sh; then
	(cd usr/src/clan; ./autogen.sh)
fi
if test -f usr/src/cloog/autogen.sh; then
	(cd usr/src/cloog; ./autogen.sh)
fi
