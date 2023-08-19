#!/bin/bash
if [ $# != 1 ]; then
    echo "Usage: measurepage [urltofile]"
    exit 1
fi

url="$1"
url_regex='https\:\/\/courses\.cs\.washington.edu\/courses\/cse374\/22au\/lectures\/ccode\/[a-zA-Z]*\.c'
if [[ ! $url =~ $url_regex ]]; then
    echo "Error: Please provide a valid url"
    exit 1
fi

filename=$(basename "$url")

wget -q "$url"
if [ $? -ne 0 ]; then
    echo "0"
    exit 0
fi

gcc -Wall -std=c11 -o wordcount wordcount.c > /dev/null
./wordcount -w "$filename"

exit 0