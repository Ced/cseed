#!/bin/sh
make maintainer-clean
./get_submodules.sh
./autogen.sh
./configure --prefix=$PWD/usr \
	    --with-isl=bundled \
	    --with-osl=bundled \
	    --with-clan=bundled \
	    --with-cloog=bundled
#./configure --prefix=$HOME/usr --with-osl=bundled
make

