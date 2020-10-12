#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>

#define DIMSTRINGA 100

typedef int pipe_t[2];

int numero_figli, timeout, stato, i;

pipe_t* pipePF;
pipe_t* pipeFP;


void handler_sigint(int signo);

void handler_figli(int signo);

int main(int argc, char** argv){
    //installazione segnali
    signal(SIGUSR1, SIG_IGN);
    signal(SIGINT, handler_sigint);
    
    //controllo argomenti
    if(argc < 3){
        fprintf(stderr, "Uso: ricerca_file <timeout> <s1> [s2...sN]");
        exit(1);
    }

    if((timeout = atoi(argv[1])) < 0){
        fprintf(stderr, "Il timeout deve essere maggiore di 0\n");
        exit(1);
    }

    //il numero dei figli e' dato da argc meno
    //il nome del programma e il timeout
    numero_figli = argc - 2;
    //genero figli
    int PID[numero_figli];
    pipePF = malloc(sizeof(pipe_t) * numero_figli);
    pipeFP = malloc(sizeof(pipe_t) * numero_figli);


    for(i = 0; i < numero_figli; i++){
        //creazione pipe
        if(pipe(pipePF[i]) < 0){
            perror("Errore creazione pipe p -> f");
            exit(2);
        }
        if(pipe(pipeFP[i]) < 0){
            perror("Errore creazione pipe f -> p");
            exit(2);
        }
        //fork del figlio
        PID[i] = fork();

        if(PID[i] < 0){
            perror("Errore creazione figlio");
            exit(3);
        }
        else if(PID[i] == 0){
            //codice figlio
            //installazione segnali
            signal(SIGINT, SIG_IGN);
            signal(SIGUSR1, handler_figli);
            int k;
            //chiusura pipe altri figli
            for(k = 0; k < i; k++){
                close(pipePF[k][0]);
                close(pipePF[k][1]);
                close(pipeFP[k][0]);
                close(pipeFP[k][1]);
            }
            //chiusura sue pipe
            close(pipePF[i][1]);
            close(pipeFP[i][0]);

            //leggere dal padre il file in cui cercare la stringa
            char nome_file[DIMSTRINGA];
            int letti;
            while((letti = read(pipePF[i][0], nome_file, DIMSTRINGA)) > 0){
                //execl
                int pid_nipote;
                pid_nipote = fork();
                if(pid_nipote < 0){
                    perror("Errore generazione nipote");
                    exit(4);
                }
                else if(pid_nipote == 0){
                    signal(SIGINT, SIG_IGN);
                    signal(SIGUSR1, SIG_IGN);
                    //execl
                    close(pipePF[i][0]);
                    //redirezione output
                    close(1);
                    dup(pipeFP[i][1]);
                    close(pipeFP[i][1]);
                    //execl
                    execlp("grep", "grep", "-c", argv[i + 2], nome_file, (char *)0);
                }
                //codice figlio nel while
            }
            //codice figlio fuori dal while
        }
        //codice padre nel for
        //chiusura pipe non usate dal padre
        close(pipePF[i][0]);
        close(pipeFP[i][1]);
    }
    //codice padre fuori dal for
    //chiedere il nome del file in cui cercare le stringhe
    char nome_file[DIMSTRINGA], risposta[DIMSTRINGA];
    int j;
    //guarda fgets per file con spazi nel nome
    while(1){
        printf("Inserisci il nome del file in cui cercare le stringhe\n");
        scanf("%s", nome_file);

        //inviare il nome del file al figlio
        for(j = 0; j < numero_figli; j++){
            write(pipePF[j][1], nome_file, sizeof(nome_file));
        }

        //ricevo dati
        for(j = 0; j < numero_figli; j++){
            read(pipeFP[j][0], risposta, DIMSTRINGA);
            printf("Nel file %s ci sono %d occorenze di %s\n", nome_file, atoi(risposta), argv[j + 2]);
        }
    }
}

void handler_sigint(int signo){
    //aspettare timeout per far terminare i figli
    int a;

    /*for(a = 0; a < numero_figli; a++){
        write(pipePF[a][1], "", 0);
    }*/

    kill(0, SIGUSR1);

    sleep(timeout);

    for(a = 0; a < numero_figli; a++){
        wait(&stato);
    }

    for(a = 0; a < numero_figli; a++){
        close(pipePF[a][1]);
        close(pipeFP[a][0]);
    }

    free(pipePF);
    free(pipeFP);  

    exit(0);
}

void handler_figli(int signo){
    close(pipePF[i][0]);
    close(pipeFP[i][1]);
    exit(0);
}