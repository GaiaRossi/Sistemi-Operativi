#!/bin/sh
cd "$1"

counter=0
stringa=$2

shift
shift

for i in $*
do
    if test -f "$i"
    then
        #esiste il file
        volte=`grep -c $stringa $i`
        if test "$volte" -ne 0
        then
            cp "$i" /tmp
            counter=`expr $counter + 1`
        fi
    fi
done

#aggiornamento di counter
value=`cat /tmp/.counter.tmp`
value=`expr $value + $counter`
echo "$value" > /tmp/.counter.tmp

#chiamata ricorsiva
for dir
do
    if test -d "$dir" -a -x "$dir"
    then
        copylines_ric.sh "`pwd`"/"$dir" "$stringa" "$*"
    fi
done