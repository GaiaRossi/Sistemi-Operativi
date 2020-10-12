//ctrl + c equivale a SIGINT

#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define MAXNFIGLI 10
#define MAXDIMSTRINGHE 20

int timeout, loopPadre = 1, loopFigli = 1, numeroFigli;

//padre, quando riceve sigint deve dire ai figli
//di terminare e aspettare timeout
//pdre deve ignorare sigusr1
void handler_padre(int signo){
    kill(0, SIGUSR1);
    sleep(timeout);
    loopPadre = 0;
    //non serve aspettare qui i figli, lo si fa nel main
}

//figli, quando ricevono sigusr1 devono chiudere tutte le ricerce,
//chiudere tutte le pipe e terminare
//figli devono ignorare sigint
void handler_figli(int signo){
    loopFigli = 0;
}

int main(int argc, char** argv){
    //variabili
    int i, PID, stato;

    //controllo invocazione
    //controllo numero parametri
    if(argc < 3){
        fprintf(stderr, "Uso: ./ricerca_files <timeout> <s1> [s2...sN]\n");
        exit(1);
    }

    //controllo timeout
    if(atoi(argv[1]) <= 0){
        fprintf(stderr, "Timeout deve essere un numero positivo\n");
        exit(1);
    }

    //controllo numero figli
    if(argc > MAXNFIGLI + 2){
        fprintf(stderr, "Il numero massimo di stringhe che possono essere cercare e' %d\n", MAXNFIGLI);
        exit(1);
    }

    //generazione figli
    numeroFigli = argc - 2;
    int pipeIn[numeroFigli][2];
    int pipeOut[numeroFigli][2];

    for(i = 0; i < numeroFigli; i++){
        //creazione pipeIn
        if(pipe(pipeIn[i]) < 0){
            perror("Errore creazione pipeIn\n");
            exit(2);
        }
        //creazione pipeOut
        if(pipe(pipeOut[i]) < 0){
            perror("Errore creazione pipeOut\n");
            exit(2);
        }

        //creazione figlio
        PID = fork();
        if(PID < 0){
            perror("Errore creazione figlio\n");
            exit(3);
        } else if(PID == 0){
            //codice figlio
            //installazione segnale
            signal(SIGINT, SIG_IGN);
            signal(SIGUSR1, handler_figli);
            //chiusura pipe altri figli
            int k;
            for(k = 0; k < i; k++){
                close(pipeIn[k][0]);
                close(pipeIn[k][1]);
                
                close(pipeOut[k][0]);
                close(pipeOut[k][1]);
            }

            //chiusura lati pipe che non servono
            close(pipeIn[i][1]);
            close(pipeOut[i][0]);

            //lettura da padre
            char file[MAXDIMSTRINGHE];
            while(loopFigli){
                read(pipeIn[i][0], file, sizeof(file));
                //genero nipote
                int pidNipote;
                pidNipote = fork();
                if(pidNipote < 0){
                    perror("Errore generazione nipote\n");
                    exit(4);
                } else if(pidNipote == 0){
                    //codice nipote
                    close(pipeIn[i][0]);

                    //redirezione output
                    close(1);
                    dup(pipeOut[i][1]);
                    close(pipeOut[i][1]);

                    //execlp
                    execlp("grep", "grep", "-c", argv[i + 1], file, (char*) 0);
                    perror("Errore execlp\n");
                    exit(5);
                }
                //codice figlio nel while
            }
            //codice figlio fuori while
            //ci esce solo quando arriva sigusr1
            //attesa nipote
            wait(&stato);
            //chiusura pipe perche' figlio ha finito
            close(pipeIn[i][0]);
            close(pipeOut[i][1]);
            exit(0);
        }
        //codice padre mentre creo figli
        //chiusura lati pipe non utilizzati dal padre
        close(pipeIn[i][0]);
        close(pipeOut[i][1]);
    }
    //codice padre dopo aver creato figli
    //installazione segnali
    signal(SIGUSR1, SIG_IGN);
    signal(SIGINT, handler_padre);

    char nomeFile[MAXDIMSTRINGHE], risposta[MAXDIMSTRINGHE];
    int fd, k;

    //ciclo per chiedere input all'utente
    while(loopPadre){
        printf("Inserisci il nome del file: ");
        scanf("%s", nomeFile);
        //controllo nome file appena inserito
        if(nomeFile[0] == '/'){
            fprintf(stderr, "Il nome del file deve essere relativo\n");
            continue;
        }
        //controllo esistenza file
        if((fd = open(nomeFile, O_RDONLY)) < 0){
            fprintf(stderr, "Il file %s non esiste\n", nomeFile);
            continue;
        }

        //file aperto correttamente
        //dico ai figli il nome del file
        for(k = 0; k < numeroFigli; k++){
            write(pipeIn[k][1], nomeFile, strlen(nomeFile) + 1);
        }

        //leggo le risposte dei figli
        for(k = 0; k < numeroFigli; k++){
            read(pipeOut[k][0], risposta, sizeof(risposta));
            printf("Nel file %s ci sono %d occorrenze della stringa %s\n", nomeFile, atoi(risposta), argv[k + 2]);
        }
    }
    //esco solo dopo aver ricevuto sigint
    //chiudo tutte le pipe
    for(k = 0; k < numeroFigli; k++){
        close(pipeIn[k][1]);
        close(pipeOut[k][0]);
    }

    //rilevo lo stato di terminazione dei figli
    //se no diventano zombie
    for(k = 0; k < numeroFigli; k++){
        wait(&stato);
    }

    return 0;
}