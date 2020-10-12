#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define DIMENSIONESTRINGHE 100

typedef int pipe_t[2];

int numeroFigli, timeout, stato;
pipe_t *pipePadreToFiglio;
pipe_t *pipeFiglioToPadre;


void handler(int signo);

int main(int argc, char** argv){
    signal(SIGINT, handler);
    //controllo parametri
    if(argc < 3){
        fprintf(stderr, "Uso: ricercaFile <timeout> <s1> [s2...sN]\n");
        exit(1);
    }

    if((timeout = atoi(argv[1])) <= 0){
        fprintf(stderr, "Il timeout deve essere un numero maggiore di 0\n");
    }

    int i, pid;
    numeroFigli = argc - 2;
    pipePadreToFiglio = (pipe_t*)malloc(numeroFigli * sizeof(pipe_t));
    pipeFiglioToPadre = (pipe_t*)malloc(numeroFigli * sizeof(pipe_t));

    for(i = 0; i < numeroFigli; i++){
        if(pipe(pipePadreToFiglio[i]) < 0){
            perror("Errore creazione della pipe p -> f");
        }

        if(pipe(pipeFiglioToPadre[i]) < 0){
            perror("Errore creazione della pipe f -> p");
        }

        pid = fork();
        if(pid < 0){
            perror("Errore generazione figlio");
        }
        else if(pid == 0){
            //codice figlio
            signal(SIGINT, SIG_IGN);
            int j;
            for(j = 0; j < i; j++){
                close(pipePadreToFiglio[j][0]);
                close(pipePadreToFiglio[j][1]);
                close(pipeFiglioToPadre[j][0]);
                close(pipeFiglioToPadre[j][1]);
            }
            close(pipePadreToFiglio[i][1]);
            close(pipeFiglioToPadre[i][0]);

            char nomeFile[DIMENSIONESTRINGHE];
            while(read(pipePadreToFiglio[i][0], nomeFile, sizeof(nomeFile)) > 0){
                int pidNipote;
                pidNipote = fork();
                if(pidNipote < 0){
                    perror("Errore nella generazione del nipote");
                }
                else if(pidNipote == 0){
                    //codice nipote
                    close(1);
                    dup(pipeFiglioToPadre[i][1]);
                    close(pipeFiglioToPadre[i][1]);

                    execlp("grep", "grep", "-c", argv[i + 2], nomeFile, (char *)0);
                    perror("Errore execlp");
                    exit(3);
                }
                wait(&stato);
            }
            close(pipePadreToFiglio[i][0]);
            close(pipeFiglioToPadre[i][1]);
            exit(0);
        }
        //codice padre nel for
        //chiusura pipe
        close(pipePadreToFiglio[i][0]);
        close(pipeFiglioToPadre[i][1]);
    }

    char nomeFile[DIMENSIONESTRINGHE], risposta[DIMENSIONESTRINGHE];
    printf("Inserisci il nome del file in cui cercare le stringhe:\n");
    scanf("%s", nomeFile);

    while(1){
        int k;
        for(k = 0; k < numeroFigli; k++){
            write(pipePadreToFiglio[k][1], nomeFile, strlen(nomeFile) + 1);
        }

        for(k = 0; k < numeroFigli; k++){
            read(pipeFiglioToPadre[k][0], risposta, sizeof(risposta));
            printf("Nel file %s ci sono %soccorrenze della stringa %s\n", nomeFile, risposta, argv[k + 2]);
        }

        printf("Inserisci il nome del file in cui cercare le stringhe:\n");
        scanf("%s", nomeFile);
    }
}

void handler(int signo){
    int k;
    for(k = 0; k < numeroFigli; k++){
        close(pipePadreToFiglio[k][1]);
    }

    sleep(timeout);

    for(k = 0; k < numeroFigli; k++){
        close(pipeFiglioToPadre[k][0]);
        wait(&stato);
    }

    free(pipePadreToFiglio);
    free(pipeFiglioToPadre);
    exit(0);
}