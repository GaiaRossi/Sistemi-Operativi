#!/bin/sh
if test $# -ne 1
then
    echo "Uso: ./trova_errori.sh <directory>"
    exit 1
fi

case $1 in
    /*);;
    *)
        echo "Il percorso della directory deve essere assoluto"
        exit 2
    ;;
esac

if test ! -d $1
then
    echo "La directory inserita non esiste"
    exit 3
fi

> /tmp/.maxdirname.tmp
echo "0" > /tmp/.max.tmp

PATH=$PATH:`pwd`
export PATH

> /home/Gaia/errori.txt
> /home/Gaia/warning.txt

trova_errori_ric.sh $1

echo "La cartella che conteneva piu' errori e' `cat /tmp/.maxdirname.tmp`"
echo "con `cat /tmp/.max.tmp` errori"

rm -f /tmp/.maxdirname.tmp
rm -f /tmp/.max.tmp
