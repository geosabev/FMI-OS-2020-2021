#!/bin/bash

if [ $# -gt 2 ]; then

    echo "One or two arguments only!"
    exit 1

fi

if [[ ! -d $1 ]]; then

    echo "First argument should be a directory!"
    exit 2

fi

if [ $# -eq 1 ]; then

    find -L $1 -type l -printf "%p\n" 2>/dev/null

else

    minimum_hl=$2

    find $1 -type f -printf "%p %n\n" 2>/dev/null | awk -v min=$minimum_hl '$2 >= min {print $1}'
fi
