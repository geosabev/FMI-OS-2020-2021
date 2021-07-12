#!/bin/bash

[ $# -eq 3 ] || { echo "Three arguments needed!"; exit 1; }

[ -f $1 ] || { echo "First argument should be a valid file!"; exit 2; }

[ ! -f $2 ] || { echo "Second argument should not be an existing file!"; exit 3; }

[ -d $3 ] || { echo "Third argument should be a directory!"; exit 4; }

touch $2

while read file; do

    if [ $(cat $file | awk '{ print "Line " NR ":" $s}' | egrep -v "^Line [[:digit:]]+:#" | egrep -v "^Line [[:digit:]]+:{ .* };$" | egrep -v "^Line [[:digit:]]+:$" | wc -l) -ne 0 ]; then

        echo Error in $(basename $file):

        while read line; do
            
            echo $line

        done < <(cat $file | awk '{ print "Line " NR ":" $s}' | egrep -v "^Line [[:digit:]]+:#" | egrep -v "^Line [[:digit:]]+:{ .* };$" | egrep -v "^Line [[:digit:]]+:$")
    
    else

        cat $file >> $2

    fi

done < <(find $3 | egrep ".cfg$")

cat $1 | if ! egrep -q "^$(basename $2 | sed 's/.cfg//'):"; then

            pass="$(pwgen 16 1)"

            md5="$(echo -n $pass | md5sum | awk '{print $1}')"

            echo $(basename $2 | sed 's/.cfg//'):$md5 >> $1
            echo $(basename $2 | sed 's/.cfg//') $pass

        fi
