#!/bin/sh
VAR=$(grep -m 1 'def' $1 | tr  ' ' ' ')
readonly VAR

VAR_NO_TRAIL_SPACE="$(echo -e "${VAR}" | sed -e 's/[[:space:]]*$//')"

Entry_point="$(echo $VAR_NO_TRAIL_SPACE | awk '{print $NF}')"
readonly Entry_point
echo ${Entry_point}
