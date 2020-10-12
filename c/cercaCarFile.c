#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <fcntl.h>

//caratteri che leggono i figli
//mi serve qui per poi chiamarlo nell'handler
int numeroCaratteri = 0, i, numeroFigli, stato;
int* PID;

void inizio_gara(int signo){
    fprintf(stdout, "Segnale ricevuto da %d\n", getpid());
}

void finito_figlio(int signo){
    fprintf(stdout, "Un figlio ha finito prima dello scadere del tempo\n");
    
    for(i = 0; i < numeroFigli; i++){
        kill(PID[i], SIGUSR2);
    }

    free(PID);

    //termina figli
    for(i = 0; i < numeroFigli; i++){
        wait(&stato);
    }
}

void timer_scaduto_padre(int signo){
    fprintf(stdout, "Il timer e' scaduto e nessun figlio ha ancora finito\n");
    for(i = 0; i < numeroFigli; i++){
        kill(PID[i], SIGUSR2);
    }

    free(PID);

    //termina figli
    for(i = 0; i < numeroFigli; i++){
        wait(&stato);
    }

}

void timer_scaduto_figlio(int signo){
    fprintf(stdout, "Figlio con PID %d e' stato interrotto con %d caratteri letti.\n", getpid(), numeroCaratteri);
    exit(0);
}

int main(int argc, char** argv){

    //controllo argomenti
    if(argc < 4){
        write(2, "Uso: cercaCarFile <c1> [c2...cn] <nomeFile> <numSec>\n", 54);
        exit(1);
    }

    numeroFigli = argc - 3;
    int numeroSecondi = atoi(argv[argc - 1]);

    fprintf(stdout, "Numero figli: %d, numero secondi: %d\n", numeroFigli, numeroSecondi);

    PID = malloc(sizeof(int) * numeroFigli);

    int j;

    for(j = 0; j < numeroFigli; j++){
        PID[j] = fork();
        if(PID[j] < 0){
            write(2, "Errore creazione figli\n", 24);
            exit(2);
        }
        else if(PID[j] == 0){
            //codice figlio
            //inizio gara
            struct sigaction sa;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = inizio_gara;
            if(sigaction(SIGUSR1, &sa, NULL) < 0){
                write(2, "Errore sigaction\n", 18);
                exit(3);
            }
            //timer padre
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = timer_scaduto_figlio;
            if(sigaction(SIGUSR2, &sa, NULL) < 0){
                write(2, "Errore sigaction\n", 18);
                exit(3);
            }            
            fprintf(stdout, "Figlio generato con PID %d\n", getpid());
            pause();
            int caratteriLetti, fd;
            char carattere;
            fd = open(argv[argc - 2], O_RDONLY);
            while((caratteriLetti = read(fd, &carattere, 1)) == 1){
                if(strcmp(&carattere, argv[j + 1]) == 0){
                    numeroCaratteri++;
                }
            }
            close(fd);
            //per vedere chi ha letto piu' caratteri
            //ma mi sembra che non serva
            /*
            fd = open("fileCercaCF.txt", O_RDWR | O_APPEND, 0644);
            char* stringa = sprintf("%d\t%d\n", getpid(), numeroCaratteri);
            write(fd, stringa, strlen(stringa));
            close(fd);
            */
            //avviso il padre che ho finito
            fprintf(stdout, "Ho finito prima degli altri e ho PID %d\n", getpid());
            kill(getppid(), SIGUSR1);
        }
    }
    //codice padre
    //se un figlio finisce prima del tempo
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = finito_figlio;
    if(sigaction(SIGUSR1, &sa, NULL) < 0){
        write(2, "Errore sigaction padre\n", 24);
        exit(4);
    }

    //se il timer scade
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = timer_scaduto_padre;
    if(sigaction(SIGALRM, &sa, NULL) < 0){
        write(2, "Errore sigaction padre\n", 24);
        exit(4);
    }

    sleep(1);
    //per far partire i figli
    for(i = 0; i < numeroFigli; i++){
        kill(PID[i], SIGUSR1);
    }

    //attendo
    alarm(numeroSecondi);
    pause();

    return 0;
}
