#!/bin/bash

input=$(mktemp)

cat | egrep "^-?[0-9]+$" | sort -n | uniq > "${input}"

[ "$(cat "${input}" | wc -l )" -ne 0 ] || { echo "no valid numbers"; exit 1; } 

max=$(cat "${input}" | tail -n 1)
min=$(cat "${input}" | head -n 1)

echo max = $max
echo min = $min

if [ $max -eq $min ]; then
    
    echo $max

else
    
    a_min=$(echo $min | sed -E "s/-//")    
    a_max=$(echo $max | sed -E "s/-//")
    
    if [ $a_min -eq $a_max ]; then
    
        echo $min
        echo $max
    
    elif [ $a_min -lt $a_max ]; then
    
        echo $max
    
    else
    
        echo $min
    
    fi

fi

rm -- $input
