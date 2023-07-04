#!/usr/bin/env bash

if [ $# -lt 2 ]
then
    echo "Error: Invalid number of arguments (received $# - expected 2)"
    echo ""
    echo "Usage"
    echo "./writer.sh <writefile> <writestr>"
    echo "<writefile> is a file name"
    echo "<writestr>  is string to write to <writefile>"
    exit 1
fi

if ! [ -f $1 ]
then
    mkdir -p "$(dirname "$1")"
fi

echo "$2" > $1 

exit 0