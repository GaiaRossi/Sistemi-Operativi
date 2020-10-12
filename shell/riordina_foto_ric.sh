#!/bin/sh
#$1 e' dir sorg
#$2 e' dir dest

cd $1

#spostamento dei file
for i in *.JPG
do
    if test -r $i -a -e "$2/$i"
    then
        #cerca che numero mettergli nella cartella duplicati
        count=1
        while test -e "$2/duplicati/$count-$i"
        do
            expr $count + 1
        done
        mv "$i" "$2/duplicati/$count-$i"
    else
        #mettilo nella cartella dest
        mv "$i" "$2"
        if test "$?" -eq 0
        then
            echo "$i" >> /tmp/spostati
        fi
    fi
done

#chiamata ricorsiva
for j in *
do
    if test -d "$j" -a "$j" != "duplicati"
    then
        riordina_foto_ric.sh "$j" "$2"
    fi
done