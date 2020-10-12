#!/bin/sh
#entro nella cartella
cd $1

#counter
counter=0

#cerco i file txt
for i in "`ls *.txt 2>/dev/null`"
do
    if test -f "$i" -a -w "$i"
    then
        #posso cercare preliminare
        versione=`head -n 1 $i | cut -d',' -f3`
        if test "$versione" = preliminare
        then
            #rm -f $i
            counter=`expr $counter + 1`
        fi
    fi
done

#controllo se cartella massima
max=`cat /tmp/.maxcounter.tmp`
if test "$counter" -gt "$max"
then
    echo "$counter" > /tmp/.maxcounter.tmp
    echo "`pwd`" > /tmp/.maxdirname.tmp
fi

#cerco altre directory
for d
do
    if test -d "$d"
    then
        cancella_versioni_preliminari_ric.sh "$d"
    fi
done