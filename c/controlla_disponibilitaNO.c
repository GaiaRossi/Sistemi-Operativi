#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define MAXDIM 15

int main(int argc, char** argv){

    typedef int pipe_t[2];

    pipe_t *pipeIn, *pipeOut;

    int i, fd, numFigli, stato;

    //controllo argomenti
    if(argc < 2){
        fprintf(stderr, "Uso: controlla_disponibilita <magazzino1> [magazzino2...magazzinoN]\n");
        exit(1);
    }

    //controllo se il nome del file e' relativo
    for(i = 1; i < argc; i++){
        if(argv[i][0] == '/'){
            fprintf(stderr, "Il nome del file deve essere relativo, non assoluto\n");
            exit(2);
        }
    }

    //controllo esistenza file magazzino
    for(i = 1; i < argc; i++){
        if((fd = open(argv[i], O_RDONLY)) < 0){
            fprintf(stderr, "Il file %s non esiste\n", argv[i]);
            exit(2);
        }
        else{
            close(fd);
        }
    }

    numFigli = argc - 1;
    int PID;
    pipeIn = (pipe_t *) malloc(numFigli * sizeof(pipe_t));
    pipeOut = (pipe_t *) malloc(numFigli * sizeof(pipe_t));

    //apertura pipe e generazione figli
    for(i = 0; i < numFigli; i++){
        if(pipe(pipeIn[i]) < 0){
            perror("Errore creazione pipein\n");
            exit(3);
        }
        if(pipe(pipeOut[i]) < 0){
            perror("Errore creazione pipeout\n");
            exit(3);
        }
        PID = fork();
        if(PID < 0){
            perror("Errore generazione figlio\n");
            exit(4);
        }
        else if(PID == 0){
            //codice figlio
            //printf("Generato figlio\n");
            //chiusura pipe che non vengono usate
            //dall'i-esimo figlio
            int j;
            for(j = 0; j < i; j++){
                //chiusura pipe degli altri figli
                close(pipeIn[j][0]);
                close(pipeIn[j][1]);
                close(pipeOut[j][0]);
                close(pipeOut[j][1]);
            }

            //chiudo perche' ci scrivera' il padre
            close(pipeIn[i][1]);

            //chiudo perche' il nipote dovra' solo scriverci
            close(pipeOut[i][0]);

            //finche' leggo qualcosa dal padre creo un nipote e
            //gli passo cio' che ho letto
            char stringa[MAXDIM];
            int pidNipote;
            while(read(pipeIn[i][0], stringa, sizeof(stringa)) > 0){
                pidNipote = fork();
                if(pidNipote < 0){
                    perror("Errore generazione nipote\n");
                    exit(3);
                }
                else if(pidNipote == 0){
                    //codice nipote
                    //chiusura pipe non utilizzate
                    close(pipeIn[i][0]);
                    //redirezione dell'output
                    close(1);
                    dup(pipeOut[i][1]);
                    close(pipeOut[i][1]);
                    //execlp
                    execlp("grep", "grep", "-c", stringa, argv[i + 1], (char*)0);
                    perror("Errore execlp\n");
                    exit(5);
                }
                //codice figlio
                //attesa del nipote
                wait(&stato);
            }
            //chiusura pipe del figlio
            //nessuno sa piu' niente delle pipe
            //e' tutto chiuso perche' il figlio ha finito
            close(pipeIn[i][0]);
            close(pipeOut[i][1]);
        }
        //codice padre
        close(pipeIn[i][0]);
        close(pipeOut[i][1]);
    }

    //codice padre
    //richiesta materie prime
    char materiaPrima[MAXDIM], risposta[MAXDIM];
    int k;
    
    printf("Inserisci la materia prima che vuoi cercare ('fine' per uscire): ");
    scanf("%s", materiaPrima);

    printf("Il pid del processo che e' nel while e' %d\n", getpid());
    
    //finche' non viene inserita la parola fine
    while(strcmp(materiaPrima, "fine") != 0){

        //invio la materia prima richiesta a tutti i figli
        for(k = 0; k < numFigli; k++){
            write(pipeIn[k][1], materiaPrima, strlen(materiaPrima) + 1);
        }

        //leggo le risposte dei figli
        for(k = 0; k < numFigli; k++){
            read(pipeOut[k][0], risposta, MAXDIM);
            printf("Sono disponibili %d unita' di %s nel file %s\n", atoi(risposta), materiaPrima, argv[k + 1]);
        }

        printf("Inserisci la materia prima che vuoi cercare ('fine' per uscire): ");
        scanf("%s", materiaPrima);
    }

    //chiusura tutte pipe
    for(k = 0; k < numFigli; k++){
        close(pipeIn[k][0]);
        close(pipeIn[k][1]);
        close(pipeOut[k][0]);
        close(pipeOut[k][1]);
    }

    //attendo i figli
    for(k = 0; k < numFigli; k++){
        wait(&stato);
    }

    free(pipeIn);
    free(pipeOut);

    return 0;
}