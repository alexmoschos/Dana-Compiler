#!/bin/sh
VAR=$(grep -m 1 'def' $1 | tr  ' ' ' ')
readonly VAR

Entry_point="$(echo $VAR | awk '{print $NF}')"
readonly Entry_point
echo ${Entry_point}
