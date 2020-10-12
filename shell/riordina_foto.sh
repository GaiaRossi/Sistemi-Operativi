#!/bin/sh
#controllo parametri
if test ! $# -eq 2
then
    echo "Uso: riordina_foto <dir_sorg> <dir_dest>"
    exit 1
fi

case $1 in
    /*)
        echo "Il nome della <dir_sorg> deve essere relativo"
        exit 2;;
    *);;
esac

case $2 in
    /*)
        echo "Il nome della <dir_dest> deve essere relativo"
        exit 2;;
    *);;
esac

#controllo esistenza directory
if test ! -d $1
then
    echo "Il direttorio <dir_sorg> deve esistere"
fi

if test ! -d $2
then
    echo "Il direttorio <dir_dest> deve esistere"
fi

#creare la directory per i duplicati
if test ! -d "$dir_dest"/duplicati
then    
    mkdir "$dir_dest"/duplicati
fi

#creazione file temporaneo
> /tmp/spostati

#aggiorno la variabile PATH per chiamata ricorsiva
PATH=$PATH:`pwd`
export PATH

#chiamata ricorsiva
riordina_foto_ric.sh "$1" `pwd`/"$2" 

#stampa numero file spostati
echo "Sono stati spostati `wc -l /tmp/spostati| cut -d' ' -f1` file"

#rimozione file temporaneo
rm /tmp/spostati