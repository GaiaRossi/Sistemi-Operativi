//senza sincronizzazione tra processi
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

//variabile globale che conta le iterazioni
//dovrebbe essere gestita in mutua esclusione
//con i semafori
//BUGIA
//non ha bisogno di semafori perche' in UNX tutti i processi sono
//pesanti, quindi con memoria separata
int iterazioni = 0;

//handler per SIGUSR1
void handler(int signo){
    printf("Le iterazioni sono state %d\n", iterazioni);
    exit(0);
}

//main
int main(int argc, char** argv){
    
    //controllo argomenti
    if(argc != 3){
        write(2, "Uso: ./itercounter <num_figli> <num_secondi>\n", 46);
        exit(1);
    }

    int nf, ns;
    if((nf = atoi(argv[1])) < 0 || (ns = atoi(argv[2])) < 0){
        write(2, "Gli argomenti passati devono essere numeri positivi\n", 53);
        exit(2);
    }
    
    //preparazione gestione segnali
    //CHE NON VA QUI
    //non si vuole che anche il padre gestisca sigusr1 come i figli
    /*
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    */
   //essendoci piu' figli devo ricordare tutti i pid
   //per fare questo va allocato dinamicamente un array di interi
    int i, stato;
    int* PID = malloc(nf * sizeof(int));
    for(i = 0; i < nf; i++){
        PID[i] = fork();
        if(PID[i] < 0){
            write(2, "Errore generazione figlio\n", 27);
            exit(3);
        }
        else if(PID[i] == 0){
            //figlio
            //installazione segnale
            struct sigaction sa;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if(sigaction(SIGUSR1, &sa, NULL) < 0){
                perror("Errore sigaction\n");
                exit(4);
            }

            //stampa figlio
            printf("figlio con pid: %d\n", getpid());

            //dormo per un secondo
            //poi aumento il contatore
            while(1){
                sleep(1);
                iterazioni++;
            }
        }
        //else{
            //padre
            //NOPE, il padre non e' necessariamente qui
            //e' ovunque il filgio non possa arrivare
            //come il codice che viene dopo
            //perche' i figli sono in un ciclo infinito
            //che termina con il segnale, ma quando arriva il segnale
            //terminano anche i figli
            //dormo per ns secondi
            /*
            sleep(ns);
            //invio sigusr1 a tutti i figli e attendo la morte dei figli
            for(j = 0; j < nf; j++){
                kill(PID, SIGUSR1);
                wait(&stato);
            }
        }*/
    }

    //vero codice del padre
    sleep(ns);

    //invio segnale a tutti i figli
    for(i = 0; i < nf; i++){
        kill(PID[i], SIGUSR1);
    }

    //libero lo spazio di memoria associato a PID
    free(PID);

    //attendo la terminazione di tutti i figli
    for(i = 0; i < nf; i++){
        wait(&stato);
    }
    
    return 0;
}