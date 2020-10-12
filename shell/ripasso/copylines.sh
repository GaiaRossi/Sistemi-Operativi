#!/bin/sh
#controllo argomenti
if test "$#" -lt 3
then
    echo "Uso: ./copylines.sh <dir_assoluta> <stringa> <nome_file1> [nome_file2...nome_fileN]"
    exit 1
fi

case $1 in
    /*);;
    *)
        echo "Il percorso della directory deve essere assoluto"
        exit 2;;
esac

if test ! -d $1
then
    echo "La directory non esiste"
    exit 3
fi

case $2 in
    *[0-9]*)
        echo "La stringa non deve contenere numeri"
        exit 2;;
    *[.-/]*)
        echo "La stringa non deve contenere caratteri speciali"
        exit 2;;
        *);;
esac

#export path
PATH=$PATH:`pwd`
export PATH

echo "0" > /tmp/.counter.tmp

dir=$1
stringa=$2
shift
shift

#chiamata ricorsiva
copylines_ric.sh "$dir" "$stringa" "$*"

#stampa risultati
echo "Sono stati spostati `cat /tmp/.counter.tmp` file"

if test `cat /tmp/.counter.tmp` -eq 0
then
    echo "Sleep"
    sleep 1
    copylines.sh "$dir" "$stringa" "$*"
fi

#cancello file
rm -f /tmp/.counter.tmp