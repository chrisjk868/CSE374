#!/bin/bash
if [ $# -lt 2 ]; then
    echo "Usage: urllist outputfilename [inputfilename ...]"
    exit 1
fi

output="$1"
urls="$2"
if [ -f $1 ]; then
    echo "Replacing $output" 1>&2
    echo -n > "$output"
fi

# Iterate through contents of URLs file
index=1
while read line; do
    echo "Performing word count measurement on $line"
    words=$(./measurepage.sh "$line")
    if [ $? -eq 0 ]; then
        echo "$index $words" >> "$output"
        index=$((index + 1))
        echo "...successful"
    else
        echo "...failure"
    fi
done < "$urls"
exit 0