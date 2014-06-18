#!/bin/sh
make maintainer-clean
./get_submodules.sh
./autogen.sh
./configure --prefix=$HOME/usr \
	    --with-isl=system --with-isl-prefix=$HOME/usr \
	    --with-osl=system --with-osl-prefix=$HOME/usr \
	    --with-clan=system --with-clan-prefix=$HOME/usr \
	    --with-cloog=system --with-cloog-prefix=$HOME/usr
#./configure --prefix=$HOME/usr --with-osl=bundled
make

