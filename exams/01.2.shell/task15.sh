#!/bin/bash

if [ $# -ne 1 ]; then

    echo "Script takes one argument!"
    exit 1

fi

if [[ ! -d $1 ]]; then

    echo "Argument should be a directory!"
    exit 2

fi

find $1 -type l -printf "%Y %p\n" 2>/dev/null | grep -e "^[NL]" | cut -d ' ' -f 2
