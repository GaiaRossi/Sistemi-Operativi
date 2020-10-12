#!/bin/bash
#controllo numero parametri
if test ! "$#" -eq 3
then
	echo "uso: cercaFile.sh <stringa> <dir> <num>"
	exit 1
fi

#controllo scrittura argomenti
case $2 in
	/*);;
	*)
	echo "il percorso della directory <dir> deve essere assoluto"
	exit 2;;
esac

#salvataggio variabili
stringa=$1
dir=$2
num=$3

#cambio variabile path
PATH=$PATH:`pwd`
export PATH

#creazione di max_counter.tmp e max_dirname.tmp
echo "-1" > max_counter.tmp
touch max_dirname.tmp

#chiamata a cercaStringa
cercaStringa.sh "$stringa" "$dir" "$num"

#stampa risultati
echo -n "la directory che contiene il maggior numero di file che soddisfano le condizioni date e'"
cat max_dirname.tmp

#cancello file tmp
rm max_counter.tmp
rm max_dirname.tmp
