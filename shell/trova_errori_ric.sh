#!/bin/sh
file=$2
count=0

cd $1

#ricerca dei file di log
for i in *.log
do
    if test -r $i
    then
        stringa=`grep error $i`
        if test ! -z $stringa
        then
            #la stringa error non e'vuota
            cat $i | cut -d',' -f 1,2,6
            count=`expr "$count" + 1`
        else
            stringa=`grep warning $i`
            if test ! -z $stringa
            then
                #la stringa warning non e' vuota
                cat $i | cut -d',' -f 1,2,6
            fi
        fi
    fi
done

#controllo numero errori totali
max=`head -c 1 "$2"`
if test "$count" -gt "$max"
then
    echo "$count `pwd`" > "$2"
fi

#chiamata ricorsiva
for j in *
do
    if test -d $j
    then 
        trova_errori_ric.sh $j $file $3
    fi
done