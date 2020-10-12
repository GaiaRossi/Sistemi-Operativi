#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void ricevuto_segnale(int signo){
    printf("Ricevuto segnale numero %d\n", signo);
    exit(0);
}

int main(int argc, char** argv){

    signal(SIGINT, ricevuto_segnale);

    while(1);

    
    return 0;
}