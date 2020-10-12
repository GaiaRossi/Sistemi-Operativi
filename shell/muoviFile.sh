#!/bin/bash
#CONTROLLO PARAMETRI
#test se primo argomento e' numero maggiore di 0
if test ! "$1" -ge 0
then
	echo "uso: muoviFile <numero_intero -gt 0> <dir_assoluta_sorg> <dir_assoluta_dest> <ext1...extN>"
	exit 1 
fi

#test dir sorgente
case $2 in
	/*);;
	*)
		echo "dir_sorg deve essere un percorso assoluto"
		exit 2;;
esac

#controllo dir destinazione
case $3 in 
	/*);;
	*)
		echo "dir_dest deve essere un percorso assoluto"
		exit 3;
esac

#salvataggio argomenti in vista di tre shift
num=$1
dir_sorg=$2
dir_dest=$3

#shift
shift
shift
shift

#controllo se gli argomenti rimasti sono tutte delle estensioni
for i
do
	case $i in
		.*);;
		*)
			echo "Le estensioni <ext1...extN> devono iniziare con il punto"
			exit 4;;
	esac
done

#CAMBIO VARIABILE PATH
PATH=$PATH:`pwd`
export PATH

#creazione di counter.tmp
#il tutor ha fatto anche >counter.tmp
touch counter.tmp

#chiamata a ricerca
ricerca.sh $num $dir_sorg $dir_dest $*

#stampa numero di righe di counter.tmp
count=`wc -l counter.tmp | cut -d' ' -f1`
echo "I file copiati sono $count"

#rimozione di counter.tmp
rm counter.tmp
