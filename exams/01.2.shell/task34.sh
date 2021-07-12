#!/bin/bash

[ $# -eq 1 ] || { echo "Wrong number of arguments!"; echo "Usage "${0}" DIR"; exit 1; }

[ -d $1 ] || { echo "Argument should be a directory!"; exit 2; }

touch temp 2>/dev/null
mkdir -p extracted

while read line; do

    if [[ $(egrep $(sha256sum $line) temp) ]]; then

        continue

    fi

    if [[ ! $(egrep "^[[:alnum:]]{64}  $line" temp) ]]; then

        echo "here!"

        echo "$(sha256sum $line)" >> temp

    else

        sed -i -r "s|^[[:alnum:]]{64}  ${line}$|$(sha256sum $line)|g" temp

    fi

    if [[ $(tar -tf $line | grep "^meow.txt$") ]]; then

        tar -xvf $line meow.txt

        name=$(basename $line | cut -d '_' -f 1)
        timestamp=$(basename $line | cut -d '-' -f 2 | sed 's/.tgz//')

        mv meow.txt "extracted/${name}_${timestamp}.txt"

    fi

done < <(find $1 -type f | egrep "^[^_]*_report-[[:digit:]]*.tgz$")
