#!/bin/bash

[ $# -eq 0 ] || { echo "No arguments needed!"; exit 1; }

#[ $(whoami) == "root" ] || { echo "Not root!"; exit 0; }

root_rss=$(ps -u "root" -o rss= | awk 'BEGIN{count=0} {count+=$1} END{print count}')

while read user homedir; do

	[ "$user" != "root" ] || continue

    [ ! -d $homedir ] || [ "$(stat -c "%U" $homedir)" != "$user" ] || [ "$(stat -c "%A" $homedir | cut -c 3)" != "w" ] || continue

	user_rss=$(ps -u $user -o rss= | awk 'BEGIN{count=0} {count+=$1} END{print count}')

	if [ $root_rss -lt $user_rss ]; then
		
        #killall -u "${USER_UID}" -m .
		#sleep 2
		#killall -u "${USER_UID}" -KILL -m .
        
        echo Terminated

    fi

done < <(cut -d ':' -f 1,6 /etc/passwd | tr ':' ' ')
