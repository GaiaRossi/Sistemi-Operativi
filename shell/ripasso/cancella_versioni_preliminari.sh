#!/bin/sh
#controllo parametro
if test "$#" -ne 1
then
    echo "Uso: ./cancella_versioni_preliminari.sh <dir_articoli>"
    exit 1
fi

case $1 in
    /*)
        echo "Il percorso della cartella deve essere relativo"
        exit 2
        ;;
    *);;
esac

#export di path
PATH=$PATH:`pwd`
export PATH

#creazione file per conteggio versioni preliminari in cartella
echo "0" > /tmp/.maxcounter.tmp
> /tmp/.maxdirname.tmp

#chiamata ricorsiva
cancella_versioni_preliminari_ric.sh "`pwd`"/"$1"

#stampa risultato
echo "La cartella che conteneva piu' versioni preliminari e' `cat /tmp/.maxdirname.tmp` con `cat /tmp/.maxcounter.tmp` versioni preliminari"

#cancello i file tmp
rm -f /tmp/.maxcounter.tmp
rm -f /tmp/.maxdirname.tmp