#!/bin/bash
if test ! -x $2
then 
	echo "Non posso entrare nella cartella"
	exit 1 
fi

#entrata nella cartella
dir_sorg=$2
cd $dir_sorg

#salvataggio parametri
num=$1
dir_dest=$3
shift
shift
shift

#controllo e ricerca
for j
do
for i in *$j
do
	if test -f "$i" -a -r "$i"
	then
		righe=`wc -l "$i" | cut -d' ' -f1` 
		if test "$righe" -gt "$num"
		then
			echo "trovato un file"
			echo trovato file >> /home/Gaia/Universita/SisOp/counter.tmp
		fi
	fi
done
done
for i in *
do
	if test -d $i -a -x $i
	then
		dir_sorg=$dir_sorg/$i
		ricerca.sh $num $dir_sorg $dir_dest $*
	fi
done	
