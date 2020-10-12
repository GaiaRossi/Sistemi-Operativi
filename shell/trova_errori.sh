#!/bin/sh
#controllo argomenti
if test "$#" -ne 1
then
    echo "Errore di uso: trova_errori.sh <dir>"
    exit 1
fi

case $1 in
    /*);;
    *)
        echo "Il percorso di <dir> deve essere assoluto"
        exit 2
    ;;
esac

if test ! -d $1
then
    echo "<dir> non e' valido"
    exit 3
fi

#creazione dei file errori e warning
if test ! -e prova_dest/errori.txt
then    
    > prova_dest/errori.txt
fi

if test ! -e prova_dest/warning.txt
then    
    > prova_dest/warning.txt
fi

#export path
PATH=`pwd`:$PATH
export PATH

#creazione file temporaneo che mi dira' la cartella che ha piu' errori
echo "0" > conto.tmp

#chiamata
trova_errori_ric.sh $1 `pwd`/conto.tmp `pwd`/$1

#lettura del file tmp
echo "La cartella che contiene file con piu' errori e' `cat conto.tmp | cut -d' ' -f2`"

#eliminazione file tmp
rm conto.tmp