#!/bin/sh

if test $# -ne 2
then
    echo "Uso: ./riordina_foto.sh <dir_sorgente> <dir_destinazione>"
    exit 1
fi

case $1 in
    /*)
        echo "<dir_sorgente> deve essere relativa"
        exit 2
        ;;
    *);;
esac

case $2 in
    /*)
        echo "<dir_destinazione> deve essere relativa"
        exit 2
        ;;
    *);;
esac

if test ! -d $1
then
    echo "<dir_sorgente> non esiste"
    exit 3
fi

if test ! -d $2
then
    echo "<dir_destinazione> non esiste"
    exit 3
fi

if test ! -d $2/duplicati
then
    mkdir $2/duplicati
fi

PATH=$PATH:`pwd`
export PATH

> /tmp/.counter.tmp

riordina_foto_ric.sh "`pwd`/$1" "`pwd`/$2"

echo "Sono stati copiati `wc -l < /tmp/.counter.tmp` file"

rm -f /tmp/.counter.tmp