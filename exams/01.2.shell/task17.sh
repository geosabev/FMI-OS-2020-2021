#!/bin/bash

if [ $(whoami) == "root" ]; then

    cat /etc/passwd | awk -F : '{print $1,$6}' | while read name home; do

                                                    if [ ! -d $home ] || sudo -u $name [ ! -w $home ]; then

                                                        echo $name

                                                    fi

                                                done

fi
