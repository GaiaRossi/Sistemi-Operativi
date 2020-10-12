#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv){
    if(argc < 3){
        fprintf(stderr, "Uso: cerca <nome_file> <stringa1> [stringa2...stringaN]\n");
        exit(1);
    }

    int fd, numero_figli, i, pid, conteggi;

    if((fd = open(argv[1], O_RDONLY) < 0)){
        fprintf(stderr, "Il file non esiste o non posso leggerlo\n");
        exit(2);
    }

    conteggi = open("conteggi.txt", O_CREAT, 0644);
    close(conteggi);

    numero_figli = argc - 2;
    for(i = 0; i < numero_figli; i++){
        pid = fork();
        if(pid < 0){
            perror("Errore creazione figlio");
            exit(3);
        }
        else if(pid == 0){
            //codice del figlio
            char log[100];
            sprintf(log, "%s %s\n", argv[1], argv[i + 2]);
            conteggi = open("conteggi.txt", O_WRONLY | O_APPEND);
            write(conteggi, log, strlen(log));
            close(conteggi);

            execlp("grep", "grep", "-c", argv[i + 2], argv[1], (char *)0);
            perror("Errore exec\n");
            exit(4);
        }
    }

    int j, stato;

    for(j = 0; j < numero_figli; j++){
        wait(&stato);
    }
}