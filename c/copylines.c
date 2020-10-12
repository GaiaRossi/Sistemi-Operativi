#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char ** argv){

    int fd;
    char testo[100];

    fd = open("fcl.txt", O_WRONLY | O_CREAT, 0664);

    do{
        write(1, "Inserisci una stringa ('fine' per terminare): ", 47);
        //scanf prende le stringhe senza spazi, meglio utilizzare fgets o
        //read, però fgets ritorna anche il carattere di nuova linea 
        fgets(testo, 100, stdin);
        if(strcmp(testo, "fine\n") != 0){
            write(fd, testo, strlen(testo));
        }
    }while(strcmp(testo, "fine\n") != 0);

    close(fd);

    return 0;

}