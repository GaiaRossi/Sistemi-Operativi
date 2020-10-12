#!/bin/sh

num_righe=$1
dir_sorg=$2
dir_dest=$3

shift
shift
shift

#cambio cartella
cd "$dir_sorg"

#controllo tutti i file
for i in $*
do
    for j in "`ls *$i 2>/dev/null`"
    do
            #estensione giusta
            #controllo numero righe
            righe_file=`wc -l < "$j"`
            if test $righe_file -gt $num_righe
            then
                cp "$i" "$dir_dest/$i"
                echo "$j" >> /tmp/.conto_righe.tmp
            fi
    done
done

#esploro sottocartelle
for d
do
    if test -d "$d"
    then
        muoviFile_ric.sh "$num_righe" "$d" "$dir_dest" "$*"
    fi
done