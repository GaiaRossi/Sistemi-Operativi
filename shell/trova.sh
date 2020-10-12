#!/bin/bash

#controllo parametri
if test ! "$#" -eq 2
then
	echo "uso: trova.sh <nome_assoluto_dir> <nome_file>"
	exit 1
fi

#controllo directory
case $1 in
	/*);;
	*)
		echo "il percorso della directory deve essere assoluto"
		exit 2;;
esac

#controllo se esiste la directory
if test ! -d $1
then
	echo "Inserire una directory che esiste"
	exit 3
fi

#salvo parametri
dir_sorg=$1
file=$2

#cambio path
PATH=$PATH:`pwd`
export PATH

ricercaFile.sh "$dir_sorg" "$file"
