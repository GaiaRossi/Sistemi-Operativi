#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>

int main(int argc, char** argv){
    
    //controllo numero argomenti
    if(argc != 2){
        fprintf(stderr, "Uso: ./controlla_esami <file_dati>\n");
        exit(1);
    }

    //controllo se il file esiste
    int fileDescriptor;
    if((fileDescriptor = open(argv[1], O_RDONLY)) < 0){
        fprintf(stderr, "Il file non esiste o non può essere aperto\n");
        exit(2);
    }

    close(fileDescriptor);

    char matricola_richiesta[10];

    while(strcmp(matricola_richiesta, "fine") != 0){
        printf("Inserisci la matricola di cui si vogliono controllare gli esami ('fine' per terminare): ");
        scanf("%s", matricola_richiesta);

        if(strcmp(matricola_richiesta, "fine") != 0){
            int pipeP1P2[2];

            if(pipe(pipeP1P2) < 0){
                perror("Errore creazione pipe");
                exit(3);
            }

            int pid1, pid2;
            pid1 = fork();
            if(pid1 < 0){
                perror("Errore generazione primo figlio");
                exit(4);
            }
            else if(pid1 == 0){
                //codice p1
                close(pipeP1P2[0]);

                //redirezione dell'output
                close(1);
                dup(pipeP1P2[1]);
                close(pipeP1P2[1]);

                //execlp
                execlp("grep", "grep", matricola_richiesta, argv[1], (char*)0);
                perror("Errore exec");
                exit(5);
            }
            //codice padre
            pid2 = fork();
            if(pid2 < 0){
                perror("Errore generazione secondo figlio");
                exit(4);
            }
            else if(pid2 == 0){
                //codice p2
                close(pipeP1P2[1]);

                //redirezione dell'input
                close(0);
                dup(pipeP1P2[0]);
                close(pipeP1P2[0]);

                //execlp
                execlp("wc", "wc", "-l", (char*)0);
                perror("Errore exec");
                exit(5);
            }
            //codice padre
            close(pipeP1P2[0]);
            close(pipeP1P2[1]);
            int stato;
            wait(&stato);
            wait(&stato);
        }
    }
}