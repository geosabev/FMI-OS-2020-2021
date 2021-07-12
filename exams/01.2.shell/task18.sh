#!/bin/bash

if [ $# -ne 2 ]; then

    echo "Two arguments needed!"
    exit 1

fi

if [[ $1 =~ ^[0-9]*$ ]] && [[ $2 =~ ^[0-9]*$ ]]; then

    if [[ ! -d "./a" ]]; then

        mkdir "a"

    fi

    if [[ ! -d "./b" ]]; then

        mkdir "b"

    fi

    if [[ ! -d "./c" ]]; then

        mkdir "c"

    fi

    find . -type f 2>/dev/null | while read line; do

                                    line_count=$(cat $line | wc -l)

                                    if [ $line_count -lt $1 ]; then

                                        echo $line "to ./a"
                                        #mv $line ./a

                                    elif [ $line_count -lt $2 ]; then

                                        echo $line "to ./b"
                                        #mv $line ./b

                                    else

                                        echo $line "to ./c"
                                        #mv $line ./c

                                    fi  

                                done
else

    echo "Arguments should be numbers!"
    exit 2

fi
