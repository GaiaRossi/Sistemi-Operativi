#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define DIMENSIONESTRIGHE 100

typedef int pipe_t[2];

int main(int argc, char** argv){
    if(argc < 2){
        fprintf(stderr, "Uso: trovaVini.c <cantina1> [cantina2...cantinaN]\n");
        exit(1);
    }

    int i, fd, stato;
    for(i = 1; i < argc; i++){
        if((fd = open(argv[i], O_RDONLY)) < 0){
            fprintf(stderr, "Il file non esiste o non posso aprirlo\n");
            exit(2);
        }
        close(fd);
    }

    int numeroCantine = argc - 1;
    char nomeVino[DIMENSIONESTRIGHE], risposta[DIMENSIONESTRIGHE];
    printf("Inserisci il nome del vino da cercare ('fine' per terminare):\n");
    scanf("%s", nomeVino);

    while(strcmp(nomeVino, "fine") != 0){
        
        pipe_t *pipeFiglioToPadre = (pipe_t*)malloc(numeroCantine * sizeof(pipe_t));
        
        for(i = 0; i < numeroCantine; i++){
            
            if(pipe(pipeFiglioToPadre[i]) < 0){
                perror("Errore creazione della pipe");
                exit(2);
            }

            int pid;
            pid = fork();
            if(pid < 0){
                perror("Errore generazione figlio");
                exit(3);
            } else if(pid == 0){
                //codice del figlio
                int j;
                for(j = 0; j < i; j++){
                    close(pipeFiglioToPadre[j][0]);
                    close(pipeFiglioToPadre[j][1]);
                }
                close(pipeFiglioToPadre[i][0]);

                pipe_t *pipeGrepVinoToGrepDisponibile = (pipe_t*)malloc(sizeof(pipe_t));
                pipe_t *pipeGrepToSort = (pipe_t*)malloc(sizeof(pipe_t));
                
                if(pipe(pipeGrepVinoToGrepDisponibile[0]) < 0){
                    perror("Errore creazione pipe tra le grep");
                    exit(5);
                }

                if(pipe(pipeGrepToSort[0]) < 0){
                    perror("Errore creazione pipe tra grep e sort");
                    exit(5);
                }

                int pidGrepVino, pidGrepDisponibile, pidSort;
                pidGrepVino = fork();
                if(pidGrepVino < 0){
                    perror("Errore grep vino");
                    exit(4);
                } else if(pidGrepVino == 0){
                    close(1);
                    dup(pipeGrepVinoToGrepDisponibile[0][1]);
                    close(pipeGrepVinoToGrepDisponibile[0][1]);

                    execlp("grep", "grep", nomeVino, argv[i], (char *)0);
                    perror("Errore execlp");
                    exit(4);
                }

                pidGrepDisponibile = fork();
                if(pidGrepDisponibile < 0){
                    perror("Errore grep disponibile");
                    exit(5);
                } else if(pidGrepDisponibile == 0){
                    close(0);
                    dup(pipeGrepVinoToGrepDisponibile[0][0]);
                    close(pipeGrepVinoToGrepDisponibile[0][0]);

                    close(1);
                    dup(pipeGrepToSort[0][1]);
                    close(pipeGrepToSort[0][1]);

                    execlp("grep", "grep", "disponibile", (char *)0);
                    perror("Errore execlp");
                    exit(4);
                }

                pidSort = fork();
                if(pidSort < 0){
                    perror("Errore sort");
                    exit(4);
                } else if(pidSort == 0){
                    close(0);
                    dup(pipeGrepToSort[0][0]);
                    close(pipeGrepToSort[0][0]);

                    close(1);
                    dup(pipeFiglioToPadre[i][1]);
                    close(pipeFiglioToPadre[i][1]);

                    execlp("sort", "sort", "-n", (char *)0);
                    perror("Errore execlp sort");
                    exit(5);
                }
                wait(&stato);
                wait(&stato);
                wait(&stato);

                free(pipeGrepVinoToGrepDisponibile);
                free(pipeGrepToSort);
                exit(0);
            }
            //codice del padre nel for
            close(pipeFiglioToPadre[i][1]);
            int k;
            for(k = 0; k < numeroCantine; k++){
                read(pipeFiglioToPadre[k][0], risposta, sizeof(risposta));
                printf("%s:\n", argv[k + 1]);
                printf("%s\n", risposta);
            }
        }
        int a;
        for (a = 0; a < numeroCantine; a++){
            wait(&stato);
            close(pipeFiglioToPadre[a][0]);
        }

        free(pipeFiglioToPadre);

        printf("Inserisci il nome del vino da cercare ('fine' per terminare):\n");
        scanf("%s", nomeVino);
    }
}