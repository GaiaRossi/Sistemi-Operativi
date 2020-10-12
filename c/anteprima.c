#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv){
    
    //controllo numero argomenti
    if(argc != 2){
        fprintf(stderr, "uso: anteprima <num>\n");
        exit(1);
    }

    //conversione dell'argomento in intero
    //int num = atoi(argv[1]);


    char* numero = argv[1];
    char testo[1024];
    int fd, PID, stato;
    do{
        //leggo da input
        scanf("%s", testo);
        //se non ha scritto fine
        if(strcmp("fine", testo) != 0){
            //controllo se esiste il file
            if((fd = open(testo, O_RDONLY)) < 0){
                fprintf(stderr, "il file non esiste\n");
                exit(3);
            }

            close(fd);

            //genero il figlio
            PID = fork();
            //controllo se la creazione e' andata a buon fine
            if(PID < 0){
                exit(5);
            }
            else if(PID == 0){
                //codice figlio
                execlp("head", "head", "-n", numero, testo, (char*) 0);
                
                printf("errore");
                exit(4);
            }
            else{
                //codice padre
                wait(&stato);
            }
        }
    }while(strcmp("fine", testo) != 0);
    
    return 0;
}