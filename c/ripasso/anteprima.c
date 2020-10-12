#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char** argv){
    if(argc != 2){
        fprintf(stderr, "Uso: ./anteprima <numero_righe>\n");
        exit(1);
    }

    int numero_righe = atoi(argv[1]), stato;
    if(numero_righe <= 0){
        fprintf(stderr, "Il numero di righe deve essere maggiore di 0\n");
        exit(2);
    }

    char nome_file[100];

    while(strcmp(nome_file, "fine") != 0){
        printf("Inserisci il nome del file di cui vuoi vedere l'anteprima\n");
        scanf("%s", nome_file);
        if(strcmp(nome_file, "fine") != 0){
            int fd;
            if((fd = open(nome_file, O_RDONLY)) < 0){
                printf("Il file non esiste o non posso aprirlo\n");
            }
            else{
                close(fd);
                //generazione figlio
                int pid;
                pid = fork();
                if(pid < 0){
                    perror("Errore generazione figlio");
                    exit(5);
                }
                else if(pid == 0){
                    //codice figlio
                    execlp("head", "head", "-n", argv[1], nome_file, (char*)0);
                    perror("Errore exec");
                    exit(5);
                }
                //codice padre nel while
                wait(&stato);
            }
        }
    }
}