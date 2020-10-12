#!/bin/bash

#controllo parametri
if test "$#" -lt 2
then
	echo "uso: contaRicorrenze.sh <nome_file> <stringa1...stringaN> con N>=1"
	exit 1
fi

#controllo se il file passato e' un file
if test ! -f $1 
then
	echo "Inserire un file che esiste"
	exit 2
fi

#salvataggio file
file=$1

#shift che facilita il for
shift

#grep
for i
do
	count=`grep "$i" "$file" | wc -l`
	echo "stringa: $1 - righe: $count"
done
	
