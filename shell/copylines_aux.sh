#!/bin/bash
#salvataggio parametri
dir=$1
stringa=$2
dir_partenza=$3

shift 
shift
shift

#inizializzazione variabili
counter=0

#verifico se posso entrare in dir
if test ! -x $dir
then
	exit 1
fi

cd $dir

#cercato i file con il nome richiesto e verificato se contiene stringa
for i in *
do
	for j
	do
	if test -f "$i"
	then
		nomeFile=`echo "$i" | cut -d'.' -f1`
		if test "$nomeFile" == "$j"
		then
			trovataStringa=`grep "$stringa" "$i" | wc -l | cut -d' ' -f1`
			if test "$trovataStringa" -ge 1
			then
				#echo "dentro ciclo"
				counter=` expr $counter + 1 `
				echo `grep "$stringa" "$i"`>> "$dir_partenza"/righeTrovate.tmp
			fi
		fi
	fi
	done
done

#aggiornare file temporanei
valoreCounter=`cat "$dir_partenza"/counter.tmp`
valoreCounter=` expr "$valoreCounter" + "$counter"`
echo "$valoreCounter" > "$dir_partenza"/counter.tmp

#chiamata ricorsiva nelle sotto directory
for i in *
do
	if test -d "$i"
	then
		copylines_aux.sh "`pwd`"/"$i" "$stringa" "$dir_partenza" "$*"
	fi
done
