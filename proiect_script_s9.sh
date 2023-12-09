#!/bin/bash

if [ $# -ne 1 ]; 
then
   echo "Numar nepotrivit de argumente"
   exit 1
fi

caracter="$1"
contor=0

while IFS= read -r line;
do
if [[ $line =~ "$caracter" ]];
    then
    if [[ ! $line =~ ",si" ]];
        then
        if [[ $line =~ ^[A-Z][A-Za-z0-9\ \,\.\!\?]+[?\!\.]$ ]]; 
        then 
        ((contor++))
        fi
    fi
fi
done
echo "Sunt $contor propozitii ce respecta regulile."