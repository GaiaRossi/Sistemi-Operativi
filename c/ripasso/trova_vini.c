#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    //installazione segnale
    signal(SIGINT, SIG_IGN);

    //controllo parametri
    if(argc < 2){
        fprintf(stderr, "Uso: ./trova_vini <cantina1> [cantina2...cantinaN]\n");
        exit(1);
    }

    int i, fd, stato;
    typedef int pipe_t[2];
    pipe_t* pipepf;
    pipe_t* pipefp;

    for(i = 1; i < argc; i++){
        if(argv[i][0] == '/'){
            fprintf(stderr, "Il nome dei file delle cantine deve essere relativo\n");
            exit(1);
        }
        if((fd = open(argv[i], O_RDONLY)) < 0){
            fprintf(stderr, "Il file %s non esiste\n", argv[i]);
            exit(2);
        }
        close(fd);
    }

    //generazione figli
    int numero_figli = argc - 1, pid;
    pipepf = malloc(sizeof(pipe_t) * numero_figli);
    pipefp = malloc(sizeof(pipe_t) * numero_figli);
    for(i = 0; i < numero_figli; i++){
        if(pipe(pipepf[i]) < 0){
            perror("Errore creazione pipe p -> f");
            exit(3);
        }
        if(pipe(pipefp[i]) < 0){
            perror("Errore creazione pipe f -> p");
            exit(3);
        }
        pid = fork();
        if(pid < 0){
            perror("Errore creazione figlio");
            exit(4);
        }
        else if(pid == 0){
            //codice figlio
            signal(SIGINT, SIG_IGN);

            //chiusura pipe figli prima
            int j;
            for(j = 0; j < i; j++){
                close(pipepf[j][0]);
                close(pipepf[j][1]);
                close(pipefp[j][0]);
                close(pipefp[j][1]);
            }

            //chiusura pipe non usate
            close(pipepf[i][1]);
            close(pipefp[i][0]);

            //pipe figlio -> nipote e pipe nipote -> figlio
            pipe_t pipen;
            if(pipe(pipen) < 0){
                perror("Errore creazione pipe f -> n");
                exit(3);
            }
            int pidn1, pidn2;

            //lettura vino
            char vino[100];
            while(read(pipepf[i][0], vino, 100) > 0){
                pidn1 = fork();
                if(pidn1 < 0){
                    perror("Errore creazione nipote");
                    exit(4);
                }
                else if(pidn1 == 0){
                    //codice nipote 1
                    //redirezione output
                    close(1);
                    dup(pipen[1]);
                    close(pipen[1]);
                    //execlp
                    execlp("grep", "grep", "disponibile", argv[i], (char *)0);
                }
                //codice figlio
                pidn2 = fork();
                if(pidn2 < 0){
                    perror("Errore creazione nipote");
                    exit(4);
                }
                else if(pidn2 == 0){
                    //codice nipote 2
                    //redirezione input
                    close(0);
                    dup(pipen[0]);
                    close(pipen[0]);

                    //redirezione output
                    close(1);
                    dup(pipefp[i][1]);
                    close(pipefp[i][1]);
                    //execlp
                    execlp("grep", "grep", vino, (char *)0);
                }
            }
            //close
            close(pipen[0]);
            close(pipen[1]);
            close(pipepf[i][1]);
            close(pipefp[i][0]);

        }
        //codice padre nel for
        //chiusura pipe
        close(pipepf[i][0]);
        close(pipefp[i][1]);
    }

    int k, file_vini, pidn;
    char vino[100], risposta[100];

    file_vini = open("vini.tmp", O_RDWR | O_CREAT, 6777);

    while(strcmp(vino, "fine") != 0){
        printf("Inserire il vino di cui si vuole verificare la disponibilita'\n");
        scanf("%s", vino);

        //invio del vino ai figli
        for(k = 0; k < numero_figli; k++){
            write(pipepf[k][1], vino, sizeof(vino));
        }

        //lettura della risposta
        for(k = 0; k < numero_figli; k++){
            read(pipefp[k][0], risposta, 100);
            write(file_vini, risposta, sizeof(risposta));
        }
        //ordinamento in ordine crescente
        pidn = fork();
        if(pidn < 0){
            perror("Errore creazione figlio");
            exit(4);
        }
        else if (pidn == 0){
            //sort
            signal(SIGINT, SIG_IGN);
            execlp("sort", "sort", "-n", file_vini, (char *)0);
            perror("errore sort");
        }
        wait(&stato);
    }

    free(pipepf);
    free(pipefp);
    
    return 0;
}