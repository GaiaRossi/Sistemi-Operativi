#!/bin/bash
#controllo argomenti
if test ! "$#" -eq 3
then
	echo "uso: esame.sh <estensione> <dir_sorg> <dir_dest>"
	exit 1
fi
#controllo come sono scritti gli argomenti
# - estensione
case $1 in
	.*);;
	*)
	echo "l'estensione deve iniziare con il punto"
	exit 2;;
esac

# - dir_sorg
case $2 in
	/*);;
	*)
	echo "il percorso di dir_sorg deve essere assoluto"
	exit 3;;
esac

# - dir_dest
case $3 in
	/*);;
	*)
	echo "il percorso di dir_dest deve essere assoluto"
	exit 4;;
esac

#controllo se le cartelle inserite esistono davvero
# - dir_sorg
if test ! -d $2
then
	echo "Inserire il percorso di una cartella esistente dir_sorg"
	exit 5
fi

# - dir_dest
if test ! -d $3
then
	echo "Inserire il percorso di una cartella esistente dir_dest"
	exit 6
fi

#salvataggio parametri
estensione=$1
dir_sorg=$2
dir_dest=$3

#modifica variabile path
PATH=$PATH:`pwd`
export PATH

#creazione file dove mettere i file copiati
#touch fileCopiati.txt

#chiamata a esame_rec.sh
esame_rec.sh "$estensione" "$dir_sorg" "$dir_dest"
