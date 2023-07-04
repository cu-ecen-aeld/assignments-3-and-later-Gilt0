#!/usr/bin/env bash

if [ $# -lt 2 ]
then
    echo "Please run script using a command like: ./finder.sh <filesdir> <searchstr>"
    exit 1
fi

echo "Running with command: $0 $1 $2"
exit 0