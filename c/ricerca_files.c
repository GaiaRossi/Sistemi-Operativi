#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define DIMSTRINGA 50

typedef int pipe_t[2];

//saranno le due pipe, uno da padre a figlio
//l'altra da nipote a padre
pipe_t *pipeIn, *pipeOut;

//variabili globali
int stato, i, timeout, numFigli;

//quando arriva ctrl+c (che equivale a sigint)
void handler_padre(int signo);

//quando il padre ha ricevuto sigint
void handler_figli(int signo);

int main(int argc, char** argv){
    //controllo argomenti
    if(argc < 3){
        fprintf(stderr, "Uso: ./ricerca_files <timeout> <s1> [s2...sN]\n");
        exit(1);
    }

    int pid;

    if((timeout = atoi(argv[1])) <= 0){
        fprintf(stderr, "Il timeout deve essere un numero > 0\n");
        exit(1);
    }

    numFigli = argc - 2;

    //allocazione memoria per le pipe
    pipeIn = (pipe_t *)malloc(numFigli * sizeof(pipe_t));
    pipeOut = (pipe_t *)malloc(numFigli * sizeof(pipe_t));

    //genero figli e apertura pipe
    for(i = 0; i < numFigli; i++){
        if(pipe(pipeIn[i]) < 0){
            perror("Errore apertura pipe p -> f\n");
            exit(2);
        }
        if(pipe(pipeOut[i]) < 0){
            perror("Errore apertura pipe n -> p\n");
            exit(2);
        }

        pid = fork();

        if(pid < 0){
            perror("Errore generazione figlio\n");
            exit(3);
        }
        else if(pid == 0){
            //codice figlio
            //installazione segnale
            signal(SIGINT, SIG_IGN);
            signal(SIGUSR1, handler_figli);
            
            //chiusura pipe non utlizzate
            int j;
            for(j = 0; j < i; j++){
                close(pipeIn[j][0]);
                close(pipeIn[j][1]);
                close(pipeOut[j][0]);
                close(pipeOut[j][1]);
            }

            //per quanto riguarda questo figlio
            //deve solo leggere da pipein
            close(pipeIn[i][1]);
            //chiudo la lettura da pipeot perche'
            //il nipote dovrà solo scriverci
            close(pipeOut[i][0]);
            
            char file[DIMSTRINGA];
            int pidN;
            while(1){
                read(pipeIn[i][0], file, DIMSTRINGA);
                //genero nipote ce fa execlp
                pidN = fork();
                if(pidN < 0){
                    perror("Errore generazione nipote\n");
                    exit(4);
                }
                else if(pidN == 0){
                    //codice nipote
                    //installazione segnali
                    signal(SIGINT, SIG_IGN);
                    signal(SIGUSR1, SIG_IGN);
                    close(pipeIn[i][0]);
                    //redirezione dell'output
                    close(1);
                    dup(pipeOut[i][1]);
                    close(pipeOut[i][1]);

                    //execlp
                    execlp("grep", "grep", "-c", argv[i + 2], file, (char *)0);
                    perror("Errore execlp\n");
                    exit(5);
                }
                //codice figlio nel while
                //attendo la fine di execlp
                wait(&stato);
            }
            //codice figlio fuori dal while
            //non devo chiudere qui le pipe perche'
            //non ci arrivero' mai, devo farlo
            //nell'handler
        }
        //codice padre nel for
        //chiusura pipe che non servono
        close(pipeIn[i][0]);
        close(pipeOut[i][1]);
    }
    //codice padre fuori dal for
    //installo segnali
    signal(SIGINT, handler_padre);
    signal(SIGUSR1, SIG_IGN);

    char stringa[DIMSTRINGA], risposta[DIMSTRINGA];
    int fd, k;

    while(1){
        printf("Inserisci il nome del file dove cercare le stringhe: ");
        scanf("%s", stringa);

        //controllo file inserito
        if((fd = open(stringa, O_RDONLY)) < 0){
            printf("Il file non esiste o non posso leggerlo\n");
            continue;
        }

        //se sono qui il file e' stato aperto correttamente
        //scrivo ai figli dove cercare
        for(k = 0; k < numFigli; k++){
            write(pipeIn[k][1], stringa, strlen(stringa) + 1);
        }

        //leggo le risposte
        for(k = 0; k < numFigli; k++){
            read(pipeOut[k][0], risposta, DIMSTRINGA);
            printf("Nel file %s ci sono %d occorrenze di %s\n", stringa, atoi(risposta), argv[k + 2]);
        }
    }

    //quello che farei qui devo scriverlo nell'handler perche'
    //non ci arrivo mai "naturalmente"
    return 6;
}

void handler_figli(int signo){
    //chiudere le pipe e terminare
    //aspettare il nipote se necessario
    wait(&stato);

    //chiusura pipe
    close(pipeIn[i][0]);
    //devo chiuderla qui perche', se la chiudevo prima,
    //il nipote non poteva utilizzarla
    close(pipeOut[i][1]);

    //termino
    exit(0);
}

void handler_padre(int signo){
    
    //invio segnale ai figli che devono terminare
    kill(0, SIGUSR1);

    //attendo timeout secondi
    sleep(timeout);

    //chiudo tutte le pipe
    int k;
    for(k = 0; k < numFigli; k++){
        close(pipeIn[k][1]);
        close(pipeOut[k][0]);
    }

    //rilevo lo stato di terminazione dei figli
    for(k = 0; k < numFigli; k++){
        wait(&stato);
    }

    //libero memoria
    //devo farlo qui perche' non posso farlo
    //nel padre
    free(pipeIn);
    free(pipeOut);

    //termino
    exit(0);

}