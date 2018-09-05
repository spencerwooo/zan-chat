#!/bin/bash

rm -r build
cp -R zanChat build
cd build
autoscan
aclocal
autoconf
autoheader
automake --add-missing
automake --add-missing
./configure
make
./test

