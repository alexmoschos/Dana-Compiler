#!/bin/sh
VAR=$(grep 'def' $1 | tr  ' ' ' ')
readonly VAR
#VAR2="$(echo  "${VAR}" | cut -f2 -d ' ')" not so good... 

Entry_point="$(echo $VAR | awk '{print $NF}')"
readonly Entry_point
echo ${Entry_point}
