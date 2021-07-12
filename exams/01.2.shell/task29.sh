#!/bin/bash

[ $# -eq 1 ] || { echo "One argument only!"; exit 1; }

[ -d $1 ] || { echo "Argument should be a directory!"; exit 2; }

[ -r $1 ] || { echo "Directory should be readable!"; exit 3; }

while read friend; do

    lines=$(find $1 -mindepth 3 -maxdepth 3 -type f | grep "$friend" | xargs wc -l | head -n -1 | awk '{print $1}' | awk '{sum+=$1} END{print sum}')
    
    if [ -z "$lines" ]; then

        lines=0

    fi

    echo $friend $lines

done < <(find $1 -mindepth 2 -maxdepth 2 -printf "%f\n" | sort | uniq) | sort -nr -k 2 | head
