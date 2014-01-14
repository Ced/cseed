#!/bin/sh
git submodule init
git submodule update
cd usr/src/clan && ./get_submodules.sh
cd -
cd usr/src/cloog && ./get_submodules.sh
