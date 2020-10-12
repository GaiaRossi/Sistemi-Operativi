#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>

int numero_figli;
int counter = 0;

void handler_figli(int errno){
    printf("counter vale: %d\n", counter);
    exit(0);
}

int main(int argc, char** argv){

    signal(SIGUSR1, SIG_IGN);

    if(argc != 3){
        fprintf(stderr, "Uso: ./itercounter <numero_figli> <secondi_sleep>\n");
        exit(1);
    }
    int secondi_sleep;
    if((numero_figli = atoi(argv[1])) < 0){
        fprintf(stderr, "Il numero di figli deve essere maggiore di 0\n");
        exit(2);
    }
    if((secondi_sleep = atoi(argv[2])) < 0){
        fprintf(stderr, "Il numero di secondi deve essere maggiore di 0\n");
        exit(2);
    }
    
    int i, pid;
    for(i = 0; i < numero_figli; i++){
        pid = fork();
        if(pid < 0){
            perror("Errore generazione figlio");
            exit(3);
        }
        else if(pid == 0){
            //codice figlio
            signal(SIGUSR1, handler_figli);
            while(1){
                sleep(1);
                counter++;
            }
        }
        //codice padre nel for
    }

    sleep(secondi_sleep);

    kill(0, SIGUSR1);

    int j, stato;
    for(j = 0; j < numero_figli; j++){
        wait(&stato);
    }
}