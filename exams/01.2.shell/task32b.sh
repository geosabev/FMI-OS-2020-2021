#!/bin/bash

input=$(mktemp)
data=$(mktemp)

cat | egrep "^-?[0-9]+$" | sort -n | uniq > "${input}"

[ "$(cat $input | wc -l)" -ne 0 ] || { echo "No numbers given!"; exit 1; }

while read number; do

    sum=0
    
    original=$number
    
    number=$( echo $number | sed "s/-//" )
    
    while [ $number -ne 0 ]; do
    
        add=$(( $number % 10 ))
    
        sum=$(( $sum + $add ))
    
        number=$(( $number / 10 ))
    
    done
	
    echo $original $sum >> "${data}"

done < <(cat $input)

largest_sum=$(cat $data | sort -n -r -t ' ' -k 2 | head -n 1 | cut -d ' ' -f 2)

result=$(cat $data | grep " $largest_sum$" | sort -n -t ' ' -k 1 | head -n 1 | cut -d ' ' -f 1)

echo $result

rm -- "${input}"
rm -- "${data}"
