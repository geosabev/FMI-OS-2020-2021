#!/bin/bash

[ $# -eq 2 ] || { echo "Two arguments needed!"; exit 1; }

[ -f $1 ] || { echo "First argument should be a file!"; exit 2; }

[ -d $2 ] || { echo "Second argument should be a directory!"; exit 3; }

[ $(find $2 -mindepth 1 | wc -l) -eq 0 ] || { echo "Directory should be empty!"; exit 4; }

num=1

touch "$2/dict.txt"

while read line; do

	name="$(echo $line | cut -d ':' -f 1 | sed -E "s/\(.*\)//" | awk '$1=$1')"
	
    number="$(cat "$2/dict.txt" | grep "$name" | cut -d ';' -f 2 )"

    if [ -z "$number" ]; then
	
        echo "$name;$num" >> "$2/dict.txt"
	
        number=$num
	
        num=$( expr $num + 1 )
	
    fi
	
    echo $line | cut -d ':' -f 2 >> "$2/$number.txt"

done < <(cat $1)
