#!/bin/bash
#inizializzazione della variabile file a stringa vuota
file=''

#controllo input
while test "$file" != "fine"
do	
	#richiesta input
	echo -n "Inserisci una stringa: "
	read file
	#se fine non devo controllare
	if test "$file" != "fine"
	then
		#se è un file
		if test -f "$file"
		then
			echo "$file e' un file"
		#altrimenti se è una cartella
		elif test -d "$file"
		then
			echo "$file e' un direttorio"
		#altrimenti
		else
			echo "$file non e' file o directory"
		fi
	fi	 
done
