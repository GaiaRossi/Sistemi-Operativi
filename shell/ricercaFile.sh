#!/bin/bash

dir=$1
file=$2

cd $dir
#se il file e' qui a posto
if test -f "$file"
then
	echo "il file si trova in `pwd`"
	exit 0
fi

#altrimenti cerco
for i in *
do
	if test -d "$i" -a -x "$i"
	then 
		ricercaFile.sh "`pwd`"/"$i" "$file"
	fi
done
