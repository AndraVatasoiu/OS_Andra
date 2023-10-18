#!/bin/bash

fis=$2
sum=0

if [ "$#" -lt 2 ]
then echo "Use min 3 arg"
fi

if [ ! -d "$1" ]
then echo "Use a directory"
fi

if [ ! -f "$2" ]
then echo "Use a file"
fi

for fisier in "$1"/*.txt
do
    nr_caract=$(wc -c < "$fisier")
    sum=$(( sum + $nr_caract ))
    echo "$(basename "$fisier" ) : $nr_caract">>"$fis"
done

echo "Total: $sum">>"$fis"
