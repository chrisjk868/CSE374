#!/bin/bash
if [ $# -lt 2 ]; then
    echo "Usage: urllist outputfilename [inputfilename ...]"
    exit 1
fi

urls=$1
if [ -f $1 ]; then
    echo "Warning: $urls file already exists overriding..."
    echo -n > "$urls"
fi

# Iterate through files and write matched url strings to output
shift
while (( "$#" )); do
    if [ ! -f $1 ]; then
        echo "Error: Provided html file doesn't exist"
        exit 1
    else
        grep -o "\/lectures\/ccode\/[a-zA-Z]*\.c" "$1" >> "$urls"
    fi
    shift
done
sed -i "s/\/lectures/https:\/\/courses.cs.washington.edu\/courses\/cse374\/22au\/lectures/g"  "$urls"
exit 0