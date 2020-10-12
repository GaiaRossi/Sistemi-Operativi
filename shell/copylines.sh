#!/bin/bash
#controllo numero argomenti
if test "$#" -lt 2
then
	echo "uso: copylines.sh <dir_assoluta> <stringa> [file1...fileN]"
	exit 1
fi

#controllo se primo arg e' direcory assoluta
case $1 in
	/*);;
	*)
		echo "il percorso della directory deve essere assoluto"
		exit 2;;
esac
#controllo se directory esiste
if test ! -d "$1"
then
	echo "inserire una directory che esiste"
	exit 3
fi

#controllo nomi file
dir=$1
stringa=$2

shift
shift

for i
do
	case $i in
		*[0-9]*)
			echo "i file non posso contenere cifre"
			exit 4;;
		*.*)
			echo "i file non possono contenere caratteri speciali"
			exit 4;;
		*-*)
			echo "i file non possono contenere caratteri speciali"
			exit 4;;
		*+*)
			echo "i file non possono contenere caratteri speciali"
			exit 4;;
		*);;
	esac
done

#cambio path
PATH=$PATH:`pwd`
export PATH

#creazione file per contare file spostati
echo "0" > counter.tmp
touch righeTrovate.tmp

#chiamata ricorsiva
copylines_aux.sh "$dir" "$stringa" "`pwd`" "$*"

#controllo numero file spostati
file=`cat counter.tmp`
#echo "$file"
if test "$file" -ge 1
then
	echo -n "il numero di file spostati e' "
	cat counter.tmp
	echo -n "le righe che hanno la stringa richiesta sono "
	cat righeTrovate.tmp
else
	#il vero comando sarebbe "sleep -h 1"
	sleep 1
fi

#cancello counter
rm counter.tmp
rm righeTrovate.tmp







































