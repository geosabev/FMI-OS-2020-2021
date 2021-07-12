#!/bin/bash

[ $# -eq 1 ] || [ $# -eq 2 ] || { echo "One or two arguments only!"; exit 1; }

[ -d $1 ] || { echo "First argument should be a directory!"; exit 2; }

while read symlink; do

    echo $symlink | if ! grep -q "broken sym"; then
		
                        if [ -n "$2" ]; then
		
                            stat -c "%N" "$(echo "${symlink}" | cut -d':' -f1)" | tr -d "\'" >> "${2}"
		
                        else
			
                            stat -c "%N" "$(echo "${symlink}" | cut -d':' -f1)" | tr -d "\'"
		
                        fi
	
                    fi

done < <(find $1 -type l 2>/dev/null -exec file {} \;)

cnt=$(find $1 -type l 2>/dev/null -exec file {} \; | grep "broken symbolic link to" | wc -l)

if [ -n "$2" ]; then

    echo Broken symlinks: $cnt >> "${2}"

else

    echo Broken symlinks: $cnt

fi
