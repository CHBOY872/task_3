#!/bin/sh

if [[ -z $1 ]]
then
    make main
else
    make main DEFINES=MAXTHREADS=$1
fi
rm *.o