#!/bin/bash

[ $# -eq 1 ] || { echo "One argument needed!"; exit 1; }

[ -f $1 -a -r $1 ] || { echo "First argument should be a readable file!"; exit 2; }

line_nr=1

while read line; do

    echo ""${line_nr}".$(echo "${line}" | cut -d '-' -f 2-)"

    line_nr=$(( line_nr + 1 ))

done < <(cat $1) | sort -t ' ' -k 2
