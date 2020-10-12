#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DIMENSIONESTRINGHE 100

typedef int pipe_t[2];

int main(int argc, char ** argv){
    
    if(argc != 2){
        fprintf(stderr, "Uso: conta_VM <directory_con_vm>\n");
        exit(1);
    }

    if(argv[1][0] != '/'){
        fprintf(stderr, "Il percorso della directory deve essere assoluto\n");
        exit(2);
    }

    char nomeFile[DIMENSIONESTRINGHE], risposta[DIMENSIONESTRINGHE], tipoApplicazione[DIMENSIONESTRINGHE];
    int stato;

    printf("Inserisci il nome del fornitore da cercare ('fine' per terminare):\n");
    scanf("%s", nomeFile);

    while(strcmp(nomeFile, "fine") != 0){
        int fileDescriptor;
        char percorsoCompletoFile[DIMENSIONESTRINGHE];
        strcpy(percorsoCompletoFile, argv[1]);
        strcat(percorsoCompletoFile, nomeFile);
        strcat(percorsoCompletoFile, ".txt");
        if((fileDescriptor = open(percorsoCompletoFile, O_RDONLY)) < 0){
            fprintf(stderr, "Non riesco ad aprire il file o non esiste\n");
            exit(3);
        }
        close(fileDescriptor);

        printf("Inserisci il tipo di applicazione da cercare\n");
        scanf("%s", tipoApplicazione);

        pipe_t pipeGrepToGrep, pipeFiglioToPadre;
        
        if(pipe(pipeGrepToGrep) < 0){
            perror("Errore creazione pipe");
            exit(4);
        }

        int pidGrep1, pidGrep2;

        pidGrep1 = fork();
        if(pidGrep1 < 0){
            perror("Errore fork primo grep");
            exit(5);
        }
        else if(pidGrep1 == 0){
            //codice figlio
            close(pipeGrepToGrep[0]);
            close(1);
            dup(pipeGrepToGrep[1]);
            close(pipeGrepToGrep[1]);

            execlp("grep", "grep", tipoApplicazione, percorsoCompletoFile, (char *)0);
            perror("Errore grep 1");
            exit(6);
        }

        if(pipe(pipeFiglioToPadre) < 0){
            perror("Errore creazione pipe p -> f");
            exit(5);
        }
        
        pidGrep2 = fork();
        if(pidGrep2 < 0){
            perror("Errore creazione secondo figlio");
            exit(7);
        }
        else if(pidGrep2 == 0){
            //codice figlio
            close(pipeGrepToGrep[1]);
            close(pipeFiglioToPadre[0]);

            close(0);
            dup(pipeGrepToGrep[0]);
            close(pipeGrepToGrep[0]);

            close(1);
            dup(pipeFiglioToPadre[1]);
            close(pipeFiglioToPadre[1]);
    
            execlp("grep", "grep", "operativa", (char *)0);
            perror("Errore grep 2");
            exit(6);
        }
        
        close(pipeGrepToGrep[0]);
        close(pipeGrepToGrep[1]);
        
        close(pipeFiglioToPadre[1]);

        wait(&stato);
        wait(&stato);

        read(pipeFiglioToPadre[0], risposta, sizeof(risposta));
        printf("I risultati sono %s\n", risposta);

        close(pipeFiglioToPadre[0]);
        
        printf("Inserisci il nome del fornitore da cercare ('fine' per terminare):\n");
        scanf("%s", nomeFile);
    }
    return 0;
}