#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char** argv){

    char* nomeFile = argv[1];
    int fd;
    
    //controllo argomenti
    if(argc < 3){
        write(2, "uso: cerca <nome_file> <stringa1...stringaN>\n", 45);
        return 1;
    }

    //controllo se riesco ad aprire il file
    if((fd = open(nomeFile, O_RDONLY, 0644)) == -1){
        write(2, "non riesco ad aprire il file", 29);
        return 2;
    }

    //creazione file conteggio
    int conteggio = open("conteggio.txt", O_CREAT | O_TRUNC | O_WRONLY);
    close(conteggio);

    //creazione processi figli per ogni stringa richiesta
    int PID;
    int stato, i, a;
    for(i = 2; i < argc; i++){
        PID = fork();
        if(PID == 0){
            char stringa[256];
            sprintf(stringa, "%s %s\n", nomeFile, argv[i]);
            conteggio = open("conteggio.txt", O_WRONLY | O_APPEND);
            write(conteggio, stringa, strlen(stringa));
            close(conteggio);
            execl("/bin/grep", "grep", "-c", argv[i], nomeFile, (char*) 0);
        }
        else{
            for(a = 0; a < argc - 2; a++){
                wait(&stato);
            }
        }
    }

    close(fd);

    return 0;
}