#!/usr/bin/env bash

if [ $# -lt 2 ]
then
    echo "Please run script like this"
    echo "./finder.sh <filesdir> <searchstr>"
    echo "<filesdir>  is a directory"
    echo "<searchstr> is a pattern to match in files of directory <filesdir>"
    exit 1
elif ! [ -d $1 ]
then
    echo "filesdir is not a directory"
    exit 1
fi

X=$(find $1 -type f | wc -l)
Y=$(find $1 -not -path '*/.*' -type f -exec grep -H "$2" {} \; | wc -l)

echo "The number of files are $X and the number of matching lines are $Y" 

exit 0