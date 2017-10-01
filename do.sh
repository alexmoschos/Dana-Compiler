#!/bin/sh
if [ "$1" != "" ]; then
    echo "Compiling $1"
    ./simple < $1 > 2.ll || exit 1
    opt -O3 2.ll -S -o 1.ll
    rm 2.ll
    llc -O3 1.ll -o 1.s 
    clang 1.s ./edsger_lib-master/lib.a -o a.out
fi
