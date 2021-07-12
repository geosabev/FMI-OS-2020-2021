#!/bin/bash

[ $# -eq 2 ] || { echo "Two arguments needed!"; exit 1; }

[ -f $1 ] || { echo "First argument should be a file!"; exit 2; }

[ ! -e $2 ] || { echo "Second argument should be a non existing file!"; exit 3; }

touch "$2"

while read line; do

    data="$(echo $line | cut -d ',' -f 2-)"
    
    cat $2 | if ! egrep -q ",$data"; then

                echo $line >> $2

             fi

done< <(cat $1 | sort -n -t ',' -k 1)
