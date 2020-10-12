#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char** argv){
    //controllo argomenti
    if(argc != 2){
        fprintf(stderr, "Uso: copy_input <nome_file>\n");
        exit(1);
    }

    int fd = open(argv[1], O_CREAT | O_WRONLY, 0666);
    
    char stringa[100];

    while(strcmp(stringa, "fine") != 0){
        printf("Inserisci una stringa ('fine per terminare'):\n");
        scanf("%s", stringa);
        if(strcmp(stringa, "fine") != 0){
            write(fd, stringa, strlen(stringa));
        }
    }

    close(fd);
    return 0;
}