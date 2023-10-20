#!/bin/bash


#1) grep -E "^[A-Z][a-zA-Z0-9, ]+[\.]$"


#2) grep -E -v "([si][\ ]*[,])|([si][\ ][\ ]+)"
#sau
#grep -v "[si][\ ]*[,]"

#3) grep -v "[n][pb]"

while read text
do
    t=(echo "$text" | grep -E "^[A-Z][a-zA-Z0-9, ]+[\.]$" | grep -v "[si][\ ]*[,]" |  grep -v "[n][pb]")

    echo "$t"

done
