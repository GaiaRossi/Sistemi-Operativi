#!/bin/bash

#salvataggio parametri
estensione=$1
dir_sorg=$2
dir_dest=$3
#verifico se posso operare con la cartella
if test -x "$dir_sorg"
then 
	cd $dir_sorg
else
	echo "Non posso entrare in $dir_sorg"
	exit 1
fi

#se posso cerco i file da copiare
for i in *
do
	#se hanno il pattern che cerco, li copio
	case $i in
		*$estensione)
			if test `ls -l "$dir_dest" | grep "$i" | wc -l | cut -d' ' -f1` -eq 0
			then
				echo "copiato file $i in $dir_dest"
			fi;;
		#altrimenti niente
		*);;
	esac
done

#mi richiamo nelle cartelle
for i in *
do
	if test -d "$i" -a -x "$i"
	then
		esame_rec.sh "$estensione" "`pwd`"/"$i" "$dir_dest"
	fi
done
