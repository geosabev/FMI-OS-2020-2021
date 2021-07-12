#!/bin/bash

[ $# -eq 1 ] || { echo "One argument needed!"; exit 1; }

#user validation
[ $(id -u) -eq 0 ] || exit 0

while read user; do

    sum=0

    while read pid rss; do

        sum=$(expr $sum + $rss)	

        latest_p="${pid}"

    done < <(ps -u $user -o pid=,rss= | sort -n -k 2)
		
	echo "$user $sum"
	
	if [ $sum -gt $1 ]; then

        echo "$user's processes should be terminated!"

        #kill -s TERM "${latest_p}"

        #sleep 1

        #kill -s KILL "${latest_p}"	

    fi

done < <(ps -e -o user= | sort | uniq)
