#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h> // per la funzione toupper()

#define MAXLINE 4096


void error(char *msg) {
    perror(msg);
    exit(1);
}

void convertiMaiuscolo(char str[]) {
    int i = 0;
    // Cicla su ogni carattere della stringa fino al termine (null terminator '\0')
    while (str[i] != '\0') {
        str[i] = toupper(str[i]); // Converte il carattere in maiuscolo
        i++;
    }
}

int main(int argc, char * argv[]){

    int socketfd,client_len,server_len, byteread;
    struct sockaddr_in serv_addr, cli_addr;

    char buffer[MAXLINE];

    if( argc < 2)
        error("dimmi la porta");

    socketfd = socket(AF_INET,SOCK_DGRAM,0);

    if(socketfd < 0)
        error("Errore apertura socket");

    server_len = sizeof(serv_addr);
    bzero( (char*) &serv_addr, server_len);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( atoi(argv[1]) );

    if ( bind(socketfd, (struct sockaddr*) &serv_addr, (socklen_t) server_len) )
        error("errore nel bind");

    while(1){

        client_len = sizeof(client_len);
        byteread = recvfrom(socketfd,buffer,MAXLINE,0, (struct sockaddr*) &cli_addr, (socklen_t*) &client_len);


        if(byteread < 0)
            error("errore in lettura");


        convertiMaiuscolo(buffer);



        if(sendto(socketfd,buffer,MAXLINE,0, (struct sockaddr *) &cli_addr, (socklen_t) client_len ) < 0)
            error("errore scrittura");
        
        printf("Risposta al client\n");
    }

   return 0;
}