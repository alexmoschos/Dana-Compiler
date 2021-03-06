#!/bin/bash

Oflag=0
asmout=false
irout=false
file=""
while [ $# -gt 0 ]
do
    case "$1" in
        -O0)  Oflag=0;;
        -O1)  Oflag=1;;
        -O2)  Oflag=2;;
        -O3)  Oflag=3;;
        -i)   irout=true;;
        -f)   asmout=true;;
        -*)   ;;
        *)    file="$1"
    esac
    shift
done

optf=""
if [ $Oflag -ne 0 ]; then
    optf=-O$Oflag
fi

if [ "$file" == "" ] || [ "$irout" == true ] || [ "$asmout" == true ] ; then
    echo "Compiling from stdin"
    ./simple > 2.ll || exit 1
    opt-3.8 $optf 2.ll -S -o 1.ll
    rm 2.ll
    if [ "$irout" = true ]
    then
        cat 1.ll
        exit 0
    fi
    llc-3.8 $optf 1.ll -o 1.s
    if [ "$asmout" = true ]
    then
        cat 1.s
        exit 0
    fi
    clang 1.s ./edsger_lib/lib.a -o a.out
    exit 0
fi

# Compile from file
echo "Compiling $file"
./simple < $file > 2.ll || exit 1
opt-3.8 $optf 2.ll -S -o 1.ll
llc-3.8 $optf 2.ll -o 1.s
clang 1.s ./edsger_lib/lib.a -o a.out
rm 2.ll
exit 0

