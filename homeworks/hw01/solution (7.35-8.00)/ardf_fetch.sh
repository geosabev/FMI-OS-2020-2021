#!/bin/bash

if [ $# -ne 1 ]; then

	echo "Pass only one argument please!"
	
	exit 1

elif [ ! -f $1 ]; then

	echo "Argument should be a filename!" 

	exit 2

fi

cat $1 | while read curent_url; do
	 	
		if (echo $curent_url | egrep -q '^[^#]'); then

			date=$(curl -s $curent_url | egrep -o '[0-9]{1,2}.[0-9]{2}.[0-9]{4}' | head -n 1)

			if (echo $date | grep -q "^[^0]\."); then

				date="0$date" 
			fi

			curl -s $curent_url | while read curent_line; do

					      	if (echo $curent_line | egrep -q '<th>[[:alpha:]]{1,}[[:digit:]]{1,}'); then

							curent_category=$(echo $curent_line | egrep -o '<th>[[:alpha:]]{1,}[[:digit:]]{1,}' | cut -d '>' -f 2 | head -n 1)

						fi

						if (echo $curent_line | egrep -q 'ResLine C'); then

							if (echo $curent_line | egrep -q 'href'); then

								i=1

							else

								i=0

							fi

							position=$(echo $curent_line | egrep -o 'class="pl".{1,}' | cut -d '<' -f 1 | cut -d '>' -f 2 | cut -d '.' -f 1 | cut -d ' ' -f 1 | sed "s/&nbsp;//g")

							name=$(echo $curent_line | cut -d '>' "-f$(echo $(( i+4 )))" | cut -d '<' -f 1 | sed "s/\([[:alpha:]]\+\), \([[:alpha:]]\+\)/\2 \1/g")

							nationality=$(echo $curent_line | egrep -o 'class="cl[u]{0,1}b".{1,}' | cut -d '<' -f 1 | cut -d '>' -f 2 | sed "s/&nbsp;//g")

							call=$(echo $curent_line | egrep -o 'class="call".{1,}' | cut -d '<' -f 1 | cut -d '>' -f 2 | sed "s/&nbsp;//g")
							
							runtime=$(echo $curent_line | egrep -o 'class="(Mark1 ){0,1}runT".{1,}' | cut -d '<' -f 1 | cut -d '>' -f 2 | sed "s/&nbsp;//g")

							fox=$(echo $curent_line | egrep -o 'class="fox".{1,}' | cut -d '<' -f 1 | cut -d '>' -f 2 | sed "s/&nbsp;//g")

							stnr=$(echo $curent_line | egrep -o 'class="stno".{1,}' | cut -d '<' -f 1 | cut -d '>' -f 2 | sed "s/&nbsp;//g")

							echo "$date:$curent_category:$position:$name:$nationality:$call:$runtime:$fox:$stnr"

						fi

						if (echo $curent_line | egrep -q 'ComLine C'); then

							if (echo $curent_line | egrep -q 'href'); then

								name=$(echo $curent_line | cut -d '>' -f 5 | cut -d '<' -f 1 | sed "s/\([[:alpha:]]\+\), \([[:alpha:]]\+\)/\2 \1/g" )

								i=2

							else

								name=$(echo $curent_line | cut -d '>' -f 4 | cut -d '<' -f 1 | sed "s/\([[:alpha:]]\+\), \([[:alpha:]]\+\)/\2 \1/g" )

								i=0

							fi

							position=$(echo $curent_line | cut -d '>' -f 3 | cut -d '<' -f 1 | cut -d '.' -f 1 | cut -d ' ' -f 1 | sed "s/&nbsp;//g")

							nationality=$(echo $curent_line | cut -d '>' "-f$(echo $(( i+5 )))" | cut -d '<' -f 1 | sed "s/&nbsp;//g")

							call=$(echo $curent_line | cut -d '>' "-f$(echo $(( i+6 )))" | cut -d '<' -f 1 | sed "s/&nbsp;//g")

							runtime=$(echo $curent_line | cut -d '>' "-f$(echo $(( i+7 )))" | cut -d '<' -f 1 | sed "s/&nbsp;//g")

							fox=$(echo $curent_line | cut -d '>' "-f$(echo $(( i+8 )))" | cut -d '<' -f 1 | sed "s/&nbsp;//g")

							stnr=$(echo $curent_line | cut -d '>' "-f$(echo $(( i+9 )))" | cut -d '<' -f 1 | sed "s/&nbsp;//g")

							echo "$date:$curent_category:$position:$name:$nationality:$call:$runtime:$fox:$stnr"

						fi

					      done

		fi

	 done

