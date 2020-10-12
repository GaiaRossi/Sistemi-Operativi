#!/bin/sh
#controllo argomenti
if test "$#" -lt 4
then
    echo "Uso: ./muovifile.sh <num_righe> <dir_sorg> <dir_dest> <ext1> [ext2...extN]"
    exit 1
fi

if test "$1" -lt 0
then
    echo "Il numero di righe deve essere maggiore di 0"
    exit 1
fi

case "$2" in
    /*);;
    *)
        echo "Il percorso della directory sorgente deve essere assoluto"
        exit 1
        ;;
esac

case "$3" in
    /*);;
    *)
        echo "Il percorso della directory destinazione deve essere assoluto"
        exit 1
        ;;
esac

num_righe=$1
dir_sorg=$2
dir_dest=$3

shift
shift
shift

for i in "$*"
do
    case $i in
        .*);;
        *)
            echo "Le estesioni devono iniziare con il punto"
            exit 1
    esac
done

#controllo validita argomenti
if test ! -d "$dir_sorg"
then
    echo "La directory sorgente non esiste"
    exit 2
fi

if test ! -d "$dir_dest"
then
    echo "La directory destinazione non esiste"
    exit 2
fi

#export path
PATH=`pwd`:$PATH
export PATH

#creazione file per conto righe
> /tmp/.conto_righe.tmp

#chiamata ricorsiva
muoviFile_ric.sh "$num_righe" "$dir_sorg" "$dir_dest" "$*"

#lettura file per conteggio righe
echo "I file spostati sono stati `wc -l < /tmp/.conto_righe.tmp`"

#cancello file
rm -f /tmp/.conto_righe.tmp

