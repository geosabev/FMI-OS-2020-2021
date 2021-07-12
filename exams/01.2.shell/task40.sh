#!/bin/bash

[ $# -eq 3 ] || { echo "Three arguments needed!"; exit 1; }

[ -f $1 ] || { echo "First argument should be a file!"; exit 2; }

cat $1 | egrep -v "^#" | if ! egrep -q "$2[ ]*="; then

            echo $2 = $3 '#' added at $(date) by $(whoami) >> $1

         elif ! egrep -q "$2[ ]*=[ ]*$3"; then
                                                 
            old_row=$(cat $1 | grep ".*$2[ ]*=.*")

            edited="$(echo '#' $old_row '#' edited at $(date) by $(whoami))"
            
            newline="$(echo $2 = $3 '#' added at $(date) by $(whoami))"
            
            sed -i "s|$old_row|$edited\n$newline|" $1 
                
        fi
