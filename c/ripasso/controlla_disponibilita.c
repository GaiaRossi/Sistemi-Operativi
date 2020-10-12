#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>

typedef int pipe_t[2];

#define DIMENSIONE_STRINGHE 100

int main(int argc, char** argv){
    //controllo argomenti
    if(argc < 2){
        fprintf(stderr, "Uso: ./controlla_disponiblita <magazzino1> [magazzino2...magazzinoN]\n");
        exit(1);
    }

    //controllo nomi file
    int i, numero_magazzini = argc - 1, stato;
    for(i = 1; i <= numero_magazzini; i++){
        if(argv[i][0] == '/'){
            fprintf(stderr, "Il percorso dei file dei magazzini devono essere relativi\n");
            exit(2);
        }
    }

    //controllo esistenza file
    for(i = 1; i <= numero_magazzini; i++){
        int fileDescriptor;
        if((fileDescriptor = open(argv[i], O_RDONLY)) < 0){
            fprintf(stderr, "Uno dei file non esiste o non puo' essere aperto\n");
            exit(3);
        }
        close(fileDescriptor);
    }

    pipe_t *pipePF = malloc(sizeof(pipe_t) * numero_magazzini);
    pipe_t *pipeFP = malloc(sizeof(pipe_t) * numero_magazzini);

    //generare n figli
    for(i = 0; i < numero_magazzini; i++){
        if(pipe(pipePF[i]) < 0){
            perror("Errore creazione pipe padre -> figlio");
            exit(4);
        }
        if(pipe(pipeFP[i]) < 0){
            perror("Errore creazione pipe figlio -> padre");
            exit(4);
        }
        
        int pid;
        pid = fork();
        if(pid < 0){
            perror("Errore generazione figlio");
            exit(5);
        }
        else if(pid == 0){
            //codice figlio
            int j;
            //chiusura pipe altri figli
            for(j = 0; j < i; j++){
                close(pipePF[j][0]);
                close(pipePF[j][1]);
                close(pipeFP[j][0]);
                close(pipeFP[j][1]);
            }
            //chiusura proprie pipe
            close(pipePF[i][1]);
            close(pipeFP[i][0]);

            //lettura materia prima
            char materia_prima[DIMENSIONE_STRINGHE];
            while(read(pipePF[i][0], materia_prima, DIMENSIONE_STRINGHE) > 0){
                int pid_nipote;
                pid_nipote = fork();
                if(pid_nipote < 0){
                    perror("Errore creazione nipote");
                    exit(5);
                }
                else if(pid_nipote == 0){
                    //codice del nipote
                    printf("Sono il nipote in exec\n");
                    close(pipePF[i][0]);

                    //redirezione output
                    close(1);
                    dup(pipeFP[i][1]);
                    close(pipeFP[i][1]);

                    //execlp
                    execlp("grep", "grep", "-c", materia_prima, argv[i + 1], (char*) 0);
                    perror("Errore execlp");
                    exit(6);
                }
                //codice del figlio nel while
                wait(&stato);
            }
            //codice figlio fuori while
            close(pipePF[i][0]);
            close(pipeFP[i][1]);
            exit(0);
        }
        //codice padre nel for
        close(pipePF[i][0]);
        close(pipeFP[i][1]);
    }
    //codice padre
    //richiesta input
    char materia_richiesta[DIMENSIONE_STRINGHE], risposta[DIMENSIONE_STRINGHE];
    int k;

    printf("Inserisci il nome della materia prima di cui vuoi verifica la disponibilita ('fine per terminare'): ");
    scanf("%s", materia_richiesta);

    while(strcmp(materia_richiesta, "fine") != 0){
        //invio la materia prima richiesta
        for(k = 0; k < numero_magazzini; k++){
            write(pipePF[k][1], materia_richiesta, strlen(materia_richiesta) + 1);
        }

        //leggo le risposte
        for(k = 0; k < numero_magazzini; k++){
            read(pipeFP[k][0], risposta, DIMENSIONE_STRINGHE);
            printf("Nel magazzino %d ci sono %d unita' di %s\n", k + 1, atoi(risposta), materia_richiesta);
        }

        printf("Inserisci il nome della materia prima di cui vuoi verifica la disponibilita ('fine per terminare'): ");
        scanf("%s", materia_richiesta);       
    }

    //invio carattere eof per terminare figli
    for(k = 0; k < numero_magazzini; k++){
        close(pipePF[k][1]);
        close(pipeFP[k][0]);
    }

    //aspetto che i figli terminino
    for(k = 0; k < numero_magazzini; k++){
        wait(&stato);
    }

    free(pipePF);
    free(pipeFP);
}