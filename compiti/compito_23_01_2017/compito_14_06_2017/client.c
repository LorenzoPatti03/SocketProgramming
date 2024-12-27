#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* per strlen() */
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>  /* tipi di dati di sistema */
#include <sys/socket.h> /* definizioni utili per le socket() */

#define MAXSIZE 4096

void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, servlen;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[MAXSIZE];

    if(argc < 3)
        error("Argomenti insufficienti");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        error("Errore nell'apertura della socket");

    server = gethostbyname(argv[1]);
    if (server == NULL)
        error("Errore nel recupero dell'indirizzo del server");

    servlen = sizeof(serv_addr);
    memset((char *) &serv_addr, 0, servlen);  // usando memset invece di bzero
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    memcpy((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);  // usando memcpy invece di bcopy

    while(1){
        printf("Inserisci nome simbolico: ");
        scanf("%s", buffer);  // correzione nell'uso di scanf

        if(sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*) &serv_addr, (socklen_t) servlen) < 0)
            error("Errore in invio");

        if(recvfrom(sockfd, buffer, MAXSIZE, 0, (struct sockaddr*) &serv_addr, (socklen_t *) &servlen) < 0)
            error("Errore in ricezione");

        printf("Indirizzo IP: %s\n", buffer);
    }

    return 0;
}