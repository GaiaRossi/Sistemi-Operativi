#!/bin/sh

cd $1

for i in `ls -l *.jpg 2>/dev/null`
do
    if test -r "$i"
    then
        #copia il file in dest
        if test ! -e "$2/$i"
        then
            mv $i $2/$i
            echo "$i" >> /tmp/.counter.tmp
        else
            # e' duplicato
            count=1
            while test -e "$2/duplicati/$count-$i"
            do
                count=`expr $count + 1`
            done
            mv $i "$2/duplicati/$count-$i"
        fi
    fi
done

for i in *
do
    if test -d "$i"
    then
        riordina_foto_ric.sh $1/$i $2
    fi
done