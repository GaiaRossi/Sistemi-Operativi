#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv){

    char messaggio[24] = "inserisci una stringa: ";
    char risposta[100];
    int caratteri_letti;

    write(1, messaggio, strlen(messaggio));
    caratteri_letti = read(1, risposta, 100);
    write(1, risposta, caratteri_letti);

    return 0;
}