#!/bin/bash
#controllo parametri
if test ! "$#" -eq 1
then
	echo "uso: cancella_parametri_preliminari.sh <dir>"
	exit 1
fi
#controllo esistenza directory
if test ! -d $1
then
	echo  "inserire una diretory che esiste"
	exit 2
fi

#cambio variabile path
PATH=$PATH:`pwd`
export PATH

#salvataggio argomenti
dir=$1

#creazione file support
echo "0" > max_counter.tmp
echo "" > max_dirname.tmp

#chiamata a cancella_versioni_preliminari_aux.sh
cancella_versioni_preliminari_aux.sh "$dir"

#stampa risultato
echo -n "cartella con numero maggiore di file eliminati: "
cat max_dirname.tmp

#eliminazione file temporanei
rm max_counter.tmp
rm max_dirname.tmp
