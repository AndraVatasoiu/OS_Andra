#!/bin/bash



mod=$2


if [ "$#" -lt 2 ]
then echo "Use min 3 arg"
fi


if [ ! -d "$1" ]
then echo "Use a directory"
fi

#nerecursiv

for fisier in "$1"/*.txt
do
    chmod "+$mod" "$fisier"
done


#recursiv

set_mod () {

    for fisier in "$1"/*.txt
    do
	if [-f "$fisier" ]
	then
	    chmod "${mod}" "$fisier"
	fi
    done

    for subdir in "$1"/*/
    do
	if [ -d "$subdir" ]
	then
	    set_mod "$subdir" "$mod"
	fi
    done
}

set_mod "$1" "$mod"



