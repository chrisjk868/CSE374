#!/bin/bash

if [ $# -lt 2 ]
then
    echo "Usage: combine outputfilename [inputfilename ...]" >&2
    exit 1
fi

if [ -e $1 ]
then
    echo "Error: Output file should not exist" >&2
    exit 1
fi

file1=$1
shift 1
cat "$@" >> "$file1" 2>>"$file1"
exit 0
done
