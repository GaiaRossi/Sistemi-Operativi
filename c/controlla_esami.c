#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){

    //controllo argomenti
    if(argc != 2){
        fprintf(stderr, "Uso: controlla_esami <file esami>\n");
        exit(1);
    }

    if(argv[1][0] != '/'){
        fprintf(stderr, "Il nome del file deve essere assoluto\n");
        exit(2);
    }

    //controllo se il file esiste
    if(open(argv[1], O_RDONLY) < 0){
        fprintf(stderr, "Il file deve esistere\n");
        exit(2);
    }


    //varibili esercizio
    char stringa[1024];
    int pipefd[2], PID, i, stato;

    //uso una scanf perche' sicuramente non ci devono essere spazi
    //nell'input, in qualsiasi caso
    do{
        printf("Inserisci il numero di matricola ('fine' per terminare): ");
        scanf("%s", stringa);
        if(strcmp(stringa, "fine") != 0){
            if(pipe(pipefd) < 0){
                fprintf(stderr, "Errore creazione pipe\n");
                exit(5);
            }
            PID = fork();
            if(PID < 0){
                perror("fork\n");
                exit(3);
            }
            else if(PID == 0){
                //codice figlio P1
                //deve eseguire solo write quindi
                //chiudo la lettura della pipe
                close(pipefd[0]);
                //redirezione dell'output
                close(1);
                dup(pipefd[1]);
                close(pipefd[1]);
                //ricavo informazioni sugli esami della matricola richiesta
                execlp("grep", "grep", stringa, argv[1], (char*) 0);
                perror("execlp\n");
                exit(4);
                //terminazione di p1
            }
            //codice padre
            //creazione di p2
            PID = fork();
            if(PID < 0){
                perror("fork\n");
                exit(3);
            }
            else if(PID == 0){
                //codice figlio p2
                //deve solo leggere quindi chiudo 
                //la parte write della pipe
                close(pipefd[1]);
                //redirezione dell'input
                close(0);
                dup(pipefd[0]);
                close(pipefd[0]);
                //stampa numero esami sostenuti
                execlp("wc", "wc", "-l", (char*) 0);
                perror("execlp 2\n");
                exit(4);
                //terminazione di p2
            }

            //codice padre
            
            close(pipefd[0]);
            close(pipefd[1]);
            
            for(i = 0; i < 2; i++){
                wait(&stato);
            }
        } 
    }while(strcmp(stringa, "fine") != 0);






    return 0;
}