#!/bin/bash

if [ ! -f $1 ]; then

	echo "First argument should be a file!"

	exit 1

fi

if [ $2 = "top_places" ] && [ $# -eq 5 ] && ( cat $1 | egrep -q "$3" ) && ( echo "$4 $5" | egrep -q "[[:digit:]]{1,} [[:digit:]]{1,}" ); then

	cat $1 | egrep "$3:[[:digit:]]{1,}" | while read person; do

						position=$( echo $person | cut -d ':' -f 3)

						if [ $position -le $4 ]; then

							echo $person

						fi

					      done | cut -d ':' -f 4 | sort -d -k 1 | uniq -c | sort -n -r -k 1 | head -n $5

elif [ $2 = "parts" ] && [ $# -eq 3 ] && ( echo $3 | egrep -q "[[:alpha:]]{1,} [[:alpha:]]{1,}" ); then

	cat $1 | grep "$3" | sort -t ':' -d -k 2 | cut -d ':' -f 2 | uniq | while read category; do

										echo $category "$(cat $1 | grep "$3" | grep "$category" | cut -d ":" -f 1 | sort -t "." -n -k 3 -k 2 -k 1 | tr '\n' ',' | sed "s/,/, /g" | head -c -2 )"

									    done

else
	echo "Invalid arguments! Please try again!"

	exit 2

fi

