#!/bin/bash

if [ $# -ne 3 ]; then

    echo "Invalid arguments!"
    exit 1

fi

if [[ ! -d $1 ]]; then

    echo "First argument should be a directory!"
    exit 2

fi

if [[ ! -d $2 ]]; then

    echo "Second argument should be a directory!"
    exit 3

fi

while read result; do
    
    no_prefix_path="$(echo $result | sed "s|$1||g")"

    mkdir -p "$2$(dirname $no_prefix_path)"
   
    mv $result "$2$no_prefix_path"
    #cp $result "$2$no_prefix_path"

done < <(find $1 -type f -name "*$3*" 2>/dev/null)
