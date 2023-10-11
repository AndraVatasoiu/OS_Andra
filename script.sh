#!/bin/bash

file=$1
dir=$2

shift 2

var=0
sum=0
lg_sum=0

#1
if [ "$#" -lt 3 ]
then echo " Usage $0 file dir n1 n2 n3 ..."
fi


#2
for arg in "$@"
do
    if [ "$arg" -gt 10 ]
    then
       	var=`expr $var + 1`
    fi
done

echo " Numere mai mari ca 10 = $var"

#3
for arg in "$@"
do
    sum=$(( sum + $arg))
done

#4

lg_sum=$(echo -n $sum | wc -c)

echo "Suma $sum are $lg_sum cifre"


#5

if [ -f "$file" ]
then echo "Suma $sum are $lg_sum cifre" > "$file"
     echo "Fisierul este unul obisnuit"
fi


#6

for fisier in "$dir"/*.txt
do
    if [ -f "$fisier" ]
       then echo "Continul fisier $fisier"
       	    echo "$fisier"
	    echo"....."
    fi
done
