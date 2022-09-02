#!/bin/sh

FILES="*.sh *.cpp *.hpp makefile"

if [[ -z $1 ]]; then
    echo "Type a commit"
else
    git add $FILES
    git commit -m "$1"
fi