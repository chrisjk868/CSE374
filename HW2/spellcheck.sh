#!/bin/bash

if [ $# -eq "0" ]
then
    echo "Error: Please provide input files" >&2
    exit 1
fi

for file in "$@"
do
    if [ ! -f "$file" ]
    then
	echo "error: $@ does not exist - skipping." >&2
    else
	if [ -e "$file.spelling" ]
	then
	    echo "replacing $file.spelling file"
	    rm "$file.spelling"
	else
	    echo "creating $file.spelling file"
	fi
	touch "$file.spelling"

	e_cnt=0
	u_cnt=0
	lines=$(cat "$file")
	for word in $lines
	do
	    if [ ! -z "$word" ]
	    then
		matches=$(grep -ioc "\<$word\>" /usr/share/dict/words)
		if [[ $matches -eq "0" ]]
		then
		    e_cnt=$(($e_cnt + 1))
		    u_matches=$(grep -ioc "\<$word\>" $file.spelling)
		    if [[ $u_matches -eq "0" ]]
		    then
			u_cnt=$(($u_cnt + 1))
		    fi
		    echo "$word" >> "$file.spelling"
		fi
	    fi
	done
      	echo "processed $@ and found $e_cnt spelling errors $u_cnt of which are unique"
    fi
done	
	
    


