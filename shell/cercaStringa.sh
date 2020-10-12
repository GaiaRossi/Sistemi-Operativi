#!/bin/bash
#salvataggio argomenti
stringa=$1
dir=$2
num=$3

#conta i file dentro la cartella
counter=0

#entrata nella directory
if test ! -x "$dir"
then
	echo "Non posso accedere alla directory"
	exit 1
fi

cd $dir

for i in *
do
	case $i in
		*.sh)
		if test -r "$i"
		then
			numeroRighe=`grep "$stringa" "$i" | wc -l | cut -d' ' -f1`
			if test "$numeroRighe" -ge "$num"
			then
				counter=`expr $counter + 1`
			fi
		fi;;
		*);;
	esac
done

#controllo se ho trovato la directory con piu'Ã file richiesti
max_righe=`cat /home/Gaia/Universita/SisOp/max_counter.tmp`
if test "$counter" -gt "$max_righe"
then
	echo "$counter" > /home/Gaia/Universita/SisOp/max_counter.tmp
	echo "`pwd`" > /home/Gaia/Universita/SisOp/max_dirname.tmp
fi

#chiamata ricorsiva su sottodirectory
for i in *
do
	if test -d $i
	then 
		cercaStringa.sh "$stringa" "`pwd`"/"$i" "$num"
	fi
done
