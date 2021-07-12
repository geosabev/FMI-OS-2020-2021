#!/bin/bash

[ $# -eq 1 ] || { echo "One argument!"; exit 1; }

username=$1

#root validation
[ $(whoami) == "root" ] || exit 0

ps_list=$(mktemp)
u_list=$(mktemp)

ps -e -o user=,pid=,etimes= | awk '{print $1, $2, $3}' | sort -t ' ' -k 1 > $ps_list
cat $ps_list | cut -d ' ' -f 1 | uniq > $u_list

user_ps_count=$(cat $ps_list | grep "$username" | wc -l)

while read user; do

    ps_count=$(cat $ps_list | grep "$user" | wc -l)

    if [ $ps_count -gt $user_ps_count ]; then
        
        echo $user

    fi

done < <(cat $u_list)

avg_time=$(awk '{total += $3} END {print int(total/NR)}' "${ps_list}")

echo average time in seconds: $avg_time

while read user process times; do

    if [ "${times}" -gt $( echo "${avg_time} * 2" | bc ) ]; then

        echo $process by $user should be terminated

        #kill -15 PID
        #sleep 2
        #kill -9 PID

    fi

done < <(cat $ps_list | grep "$username")

rm -- $ps_list
rm -- $u_list
