#!/bin/bash

N=10

files=$(mktemp)

if [ "$1" == "-n" ]; then

    if [[ "$2" =~ ^[0-9]*$ ]]; then
	
        N=$2
        
        printf "%s\n" "$@" | tail -n +3 > $files

	else

        echo "Not a valid N given!"

        exit 1

    fi

else

    printf "%s\n" "$@" > $files

fi

while read file; do

    FID="$(echo $file | sed "s/.log//")"
    
    while read line; do

        timestamp="$(echo $line | cut -d ' ' -f 1,2)"
        data="$(echo $line | cut -d ' ' -f 3-)"

        echo $timestamp $FID $data

    done < <(cat $file)

done < <(cat $files) | sort

rm -- $files
