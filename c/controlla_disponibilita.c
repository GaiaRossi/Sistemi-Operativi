#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>

#define DIMMAXSTRINGA 20

typedef int pipe_t[2];

int main(int argc, char** argv){
    //controllo argomenti
    //controllo numero argomenti
    if(argc < 2){
        fprintf(stderr, "Uso: ./controlla_disponibilita <magazzino1> [magazzino2...magazzinoN]\n");
        exit(1);
    }

    int i, fd, nArg, pid, stato;
    for(i = 1; i < argc; i++){
        //controllo se nomi file relativi
        if(argv[i][0] == '/'){
            fprintf(stderr, "Il nome dei file deve essere relativo\n");
            exit(1);
        }
        //controllo se i file esistono
        if((fd = open(argv[i], O_RDONLY)) < 0){
            fprintf(stderr, "Non riesco ad aprire il file %s\n", argv[i]);
            exit(1);
        }
        close(fd);
    }

    //mi salvo il numero dei figli e delle pipe che devo creare
    nArg = argc - 1;

    //creo lo spazio che conterra' i riferimenti alle pipe
    pipe_t *pipeIn = (pipe_t *) malloc(nArg * sizeof(pipe_t));
    pipe_t *pipeOut = (pipe_t *) malloc(nArg * sizeof(pipe_t));

    //genero figli e apro le pipe
    for(i = 0; i < nArg; i++){
        if(pipe(pipeIn[i]) < 0){
            perror("Errore creazione pipe p -> f\n");
            exit(2);
        }
        if(pipe(pipeOut[i]) < 0){
            perror("Errore creazione pipe n -> p\n");
            exit(2);
        }

        pid = fork();

        if(pid < 0){
            perror("Errore generazione figlio\n");
            exit(3);
        }
        else if(pid == 0){
            //codice figlio
            int j;
            //chiusura pipe dei figli precedentemente creati
            for(j = 0; j < i; j++){
                close(pipeIn[j][0]);
                close(pipeIn[j][1]);
                close(pipeOut[j][0]);
                close(pipeOut[j][1]);
            }
            //chiudo pipein in scrittura perche' il figlio
            //ci deve solo leggere
            close(pipeIn[i][1]);

            //chiudo pipeout in lettura perche' il nipote dovra' solo scriverci
            close(pipeOut[i][0]);

            //finche' leggo qualcosa da padre
            int pidN;
            char materia[DIMMAXSTRINGA];
            while(read(pipeIn[i][0], materia, DIMMAXSTRINGA) > 0){
                pidN = fork();
                if(pidN < 0){
                    perror("Errore generazione nipote\n");
                    exit(4);
                }
                else if(pidN == 0){
                    //codice nipote
                    //chiudo pipein perche' ha gia' letto il figlio
                    close(pipeIn[i][0]);
                    //redirezione dell'output
                    close(1);
                    dup(pipeOut[i][1]);
                    close(pipeOut[i][1]);
                    //execlp
                    execlp("grep", "grep", "-c", materia, argv[i+ 1], (char *)0);
                    perror("Errore execlp\n");
                    exit(5);
                }
                //codice figlio nel while
                //attendo il nipote
                wait(&stato);
            }
            //codice figlio fuori dal while
            //finito di leggere, chiudo pipeout e termino
            close(pipeOut[i][1]);
            exit(0);
        }
        //codice padre nel for
        //chiusura lati pipe non utilizzati
        close(pipeIn[i][0]);
        close(pipeOut[i][1]);
    }
    //codice padre fuori dal for
    char nomeMateria[DIMMAXSTRINGA], risposta[DIMMAXSTRINGA];
    int k;
    do{
        printf("Inserisci il nome della materia prima ('fine' per terminare): ");
        scanf("%s", nomeMateria);

        if(strcmp(nomeMateria, "fine") != 0){
            //scrivo ai figli in che file cercare
            for(k = 0; k < nArg; k++){
                write(pipeIn[k][1], nomeMateria, strlen(nomeMateria) + 1);
            }
            //leggo le risposte dai figli
            for(k = 0; k < nArg; k++){
                read(pipeOut[k][0], risposta, sizeof(risposta));
                printf("Nel file %s ci sono %d unita' disponibili di %s\n", argv[k + 1], atoi(risposta),nomeMateria);
            }
        }
        else{
            //termino il while dei figli
            for(k = 0; k < nArg; k++){
                write(pipeIn[k][1], NULL, 0);
            }
        }
    }while(strcmp(nomeMateria, "fine") != 0);

    //chiudo tutte le pipe
    for(k = 0; k < nArg; k++){
        close(pipeIn[k][1]);
        close(pipeOut[k][0]);
    }

    //attendo la terminazione di tutti i figli
    for(k = 0; k < nArg; k++){
        wait(&stato);
    }

    free(pipeIn);
    free(pipeOut);

    return 0;
}