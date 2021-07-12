#!/bin/bash

recent_files=""

while read user home; do

    recent_file="$(find $home -type f -printf "%T@ %f\n" 2>/dev/null | sort -n -r -t ' ' -k 1 | head -n 1)"
    
    [ -n "$recent_file" ] || continue

    echo $recent_file $user

done < <(cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ') | sort -n -r -t ' ' -k 1 | head -n 1 | cut -d ' ' -f 2-
