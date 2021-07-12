#!/bin/bash

[ $(whoami) == "root" ] || exit 0

ps_list=$(mktemp)
u_list=$(mktemp)

ps -e -o user=,pid=,rss= | awk '{print $1,$2,$3}' | sort -t ' ' -k 1 > $ps_list
cat $ps_list | cut -d ' ' -f 1 | uniq > $u_list

while read user; do

    ps_count=$(cat $ps_list | grep "$user" | wc -l)

    total_rss=0
    max_rss=0
    max_rss_pid=0

    while read uname pid rss; do

        total_rss=$(( total_rss + rss ))
        
        if [ $max_rss -lt $rss ]; then
            
            max_rss=$rss

            max_rss_pid=$pid

        fi     

    done < <(cat $ps_list | grep "$user" )

    avg_rss=$(expr ${total_rss} '/' ${ps_count})

    echo User $user has $ps_count process with total RSS of $total_rss and average RSS of $avg_rss.

    if [ ${max_rss} -gt $(expr ${avg_rss} '*' 2) ]; then

        echo Process $max_rss_pid will be killed!

        #kill -s SIGTERM ${MAX_RSS_PID}
        #sleep 2
        #kill -s SIGKILL ${MAX_RSS_PID}

    fi

done < <(cat $u_list)

rm -- $ps_list
rm -- $u_list
