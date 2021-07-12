#!/bin/bash

[ $# -eq 1 ] || { echo "One argument needed!"; exit 1; }

[ -f $1 ] || { echo "Logfile needed!"; exit 2; }

while read site; do

    count_http2=$(cat $1 | grep "$site" | grep "HTTP/2.0" | wc -l)
    count_other=$(cat $1 | grep "$site" | grep -v "HTTP/2.0" | wc -l)

    echo $site HTTP/2.0: $count_http2 non-HTTP/2.0: $count_other
    
    cat $1 | grep "$site" | cut -d " " -f 1,9 | sort -r -k 1 | awk '{if ($2 > 302) print $1, $2}' | cut -d ' ' -f 1 | sort -r -k 1 | uniq -c | head -n 5
    
done < <(cat $1 | cut -d ' ' -f 2 | sort | uniq -c | awk '{print $1,$2}' | sort -n -r -t ' ' -k 1 | cut -d ' ' -f 2 | head -n 3)
