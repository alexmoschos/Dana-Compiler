#!/bin/sh

# if [ "$1" != "" ]; then
#     echo "Compiling $1"
#     ./simple < $1 > 2.ll || exit 1
#     opt-3.8 -O3 2.ll -S -o 1.ll
#     rm 2.ll
#     llc-3.8 -O3 1.ll -o 1.s
#     clang 1.s ./edsger_lib-master/lib.a -o a.out
# fi


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
        -b)   blockflag=true;;
        -f)   asmout=true;;
        -*)   ;;
        *)    file="$1"
    esac
    shift
done

if ["$file" = ""] || ["$irout" = true] || ["$asmout"=true];
then
    if [ "$layoutflag" = true ]; then
        echo "Compiling from stdin with layout"
        ./simple > 2.ll || exit 1
        opt-3.8 -O$Oflag 2.ll -S -o 1.ll
        rm 2.ll
        if [ "$irout" = true ]; then
            cat 1.ll
            exit 0
        fi
        llc-3.8 -O$Oflag 1.ll -o 1.asm
        if [ "$asmout" = true ]; then
            llc-3.8 -O$Oflag 1.ll -o 1.asm
            cat 1.asm
            exit 0
        fi
        clang 1.asm ./edsger_lib-master/lib.a -o a.out
        exit 0
    fi

    if [ "$blockflag" = true ]; then
        echo "Compiling from stdin with block"
        ./block > 2.ll || exit 1
        opt-3.8 -O$Oflag 2.ll -S -o 1.ll
        rm 2.ll
        if [ "$irout" = true]; then
            cat 1.ll
            exit 0
        fi
        llc-3.8 -O$Oflag 1.ll -o 1.asm
        if ["$asmout"= true]; then
            llc-3.8 -O$Oflag 1.ll -o 1.asm
            cat 1.asm
            exit 0
        fi
        clang 1.asm ./edsger_lib-master/lib.a -o a.out
        exit 0
    fi
fi

if [ layoutflag ]; then
    echo "Compiling $file with layout"
    ./simple < $file > 2.ll || exit 1
    opt-3.8 -O$Oflag 2.ll -S -o 1.ll
    rm 2.ll
    exit 0
fi

if [ "$blockflag" = true ]; then
    echo "Compiling $file with block"
    ./block < $file > 2.ll || exit 1
    opt-3.8 -O($Oflag) 2.ll -S -o 1.ll
    rm 2.ll
    exit 0
fi

if [ "$"]