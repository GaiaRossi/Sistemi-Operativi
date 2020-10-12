#!/bin/sh

errori=0

cd $1

for i in `ls *.log 2> /dev/null`
do
    #if test -r "$i"
    #then
        isErrore=`grep -c "error" $i`
        echo "isErrore: $isErrore"
        errori=`expr "$errori" + "$isErrore"`

        grep "error" $i | cut -d',' -f1,2,6 >> /home/Gaia/errori.txt
        grep "warning" $i | cut -d',' -f1,2,6 >> /home/Gaia/warning.txt
    #fi
done

max=`cat /tmp/.max.tmp`
echo "max: $max"

if test "$errori" -gt "$max"
then
    echo "$errori" > /tmp/.max.tmp
    echo "`pwd`" > /tmp/.maxdirname.tmp
fi

for dir in *
do
    if test -d $dir
    then
        trova_errori_ric.sh "$1/$dir"
    fi
done