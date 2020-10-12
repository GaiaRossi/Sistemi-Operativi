#!/bin/bash
#dichiarazione variabili
counter=0
#ingresso directory
if test ! -x $1
then
	exit 1
fi

dir=$1

cd $dir
 
#controllo se versione preliminare
for i in *
do
	if test -f $i -a -r $i
	then
		case $i in
			*.txt)
				testo=`head -n 1 $i | cut -d',' -f3`
				if test "$testo" = "preliminare"
				then
					counter=` expr $counter + 1 `
					echo "file eliminato: $i"
				fi;;
			*);;
		esac
	fi
done
#controllo se cartella con piu' file eliminati
max_file_eliminati=`cat /home/Gaia/Universita/SisOp/max_counter.tmp`
if test "$counter" -gt "$max_file_eliminati"
then
	echo "$counter" > /home/Gaia/Universita/SisOp/max_counter.tmp
	echo "`pwd`" > /home/Gaia/Universita/SisOp/max_dirname.tmp
fi

for i in *
do
	if test -d $i
	then
		cancella_versioni_preliminari_aux.sh "`pwd`"/"$i"
	fi
done
