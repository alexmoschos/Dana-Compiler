#!/bin/bash

# if [ "$1" != "" ]; then
#     echo "Compiling $1"
#     ./simple < $1 > 2.ll || exit 1
#     opt-3.8 -O3 2.ll -S -o 1.ll
#     rm 2.ll
#     llc-3.8 -O3 1.ll -o 1.s
#     clang 1.s ./edsger_lib-master/lib.a -o a.out
# fi
# VAR=$(grep -m 1 'def' $1 | tr  ' ' ' ')
# readonly VAR

# Entry_point="$(echo $VAR | awk '{print $NF}')"
# readonly Entry_point
# echo ${Entry_point}

#set -x

Oflag=0
layoutflag=true
blockflag=false
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
        -l)   layoutflag=true;;
        -b)   blockflag=true
              layoutflag=false;;
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
    if [ "$layoutflag" = true ] ; then
        echo "Compiling from stdin with layout"
        dd of=input.dana
        ./simple < input.dana > 2.ll || exit 1
        var=$(grep -m 1 'def' "input.dana" )
        a=( $var )
        entry=${a[1]}
        sed -i 's/main/_main/g' 2.ll
        sed -i 's/'$entry'/main/g' 2.ll
        opt-3.8 $optf 2.ll -S -o 1.ll
        rm 2.ll
        rm input.dana
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
        clang 1.s ./edsger_lib-master/lib.a -o a.out
        exit 0
    fi

    if [ "$blockflag" = true ] ; then
        echo "Compiling from stdin with block"
        dd of=input.dana
        ./block < input.dana > 2.ll || exit 1
        var=$(grep -m 1 'def' "input.dana" )
        a=( $var )
        entry=${a[1]}
        sed -i 's/main/_main/g' 2.ll
        sed -i 's/'$entry'/main/g' 2.ll
        opt-3.8 $optf 2.ll -S -o 1.ll
        rm 2.ll
        rm input.dana
        if [ "$irout" = true ]; then
            cat 1.ll
            exit 0
        fi
        llc-3.8 $optf 1.ll -o 1.s
        if [ "$asmout" = true ]; then
            cat 1.s
            exit 0
        fi
        clang 1.s ./edsger_lib-master/lib.a -o a.out
        exit 0
    fi

fi

var=$(grep -m 1 'def' "$file" )
a=( $var )
entry=${a[1]}
#echo $var
if [ "$layoutflag" = true ]; then
    echo "Compiling $file with layout"
    ./simple < $file > 2.ll || exit 1
    sed -i 's/main/_main/g' 2.ll
    sed -i 's/'$entry'/main/g' 2.ll
    opt-3.8 $optf 2.ll -S -o 1.ll
    llc-3.8 $optf 2.ll -o 1.s
    clang 1.s ./edsger_lib-master/lib.a -o a.out
    exit 0
fi

if [ "$blockflag" = true ]; then
    echo "Compiling $file with block"
    ./block < $file > 2.ll || exit 1
    sed -i 's/main/_main/g' 2.ll
    sed -i 's/'$entry'/main/g' 2.ll
    opt-3.8 $optf 2.ll -S -o 1.ll
    llc-3.8 $optf 1.ll -o 1.s

    clang 1.s ./edsger_lib-master/lib.a -o a.out
    rm 2.ll
    exit 0
fi

