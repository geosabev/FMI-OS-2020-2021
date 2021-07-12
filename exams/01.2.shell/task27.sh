#!/bin/bash

[ $# -eq 2 ] || { echo "Two arguments!"; exit 1; }

[ -d $1 ] || { echo "First argument MUST be a directory!"; exit 2; }

[ -n $2 ] || { echo "Second argument MUST be a non-zero string!"; exit 3; }

find $1 -maxdepth 1 -type f -printf "%f\n" | egrep "^vmlinuz-[[:digit:]]+.[[:digit:]]+.[[:digit:]]+-$2$" | sort -t '.' -nrk 2 | head -n 1
