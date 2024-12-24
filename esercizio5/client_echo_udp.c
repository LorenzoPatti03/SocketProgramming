#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h> // per la funzione toupper()
#include <netdb.h> //per ottenere l'ip dato l'hostname

#define MAXLINE 4096


void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char * argv[]){

    int socketfd,client_len,server_len, byteread;
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent* ip_server;

    char buffer[MAXLINE];

    if( argc < 3)
        error("dimmi la porta");

    socketfd = socket(AF_INET,SOCK_DGRAM,0);

    if(socketfd < 0)
        error("Errore apertura socket");

    ip_server = gethostbyname( argv[1] );

    server_len = sizeof(serv_addr);
    bzero( (char*) &serv_addr, server_len);

    serv_addr.sin_family = AF_INET;
    bcopy( (char *) ip_server -> h_addr_list[0], (char *) &serv_addr.sin_addr.s_addr, ip_server -> h_length  );
    serv_addr.sin_port = htons( atoi( argv[2]) );

    while(1){

        printf("Scrivi qualocasa: ");
        scanf("%s",buffer);
        //snprintf(buffer,sizeof(buffer),"ciao");

        //char prova[] ="Ciao\n";

        server_len = sizeof(serv_addr);
        if( sendto(socketfd,buffer,MAXLINE,0, (struct sockaddr*) &serv_addr, (socklen_t) server_len) < 0)
            error("errore in scrittura");

        
        if( recvfrom(socketfd,buffer,MAXLINE,0, NULL, NULL) < 0)
            error("errore in lettura");

        printf("%s\n",buffer);

    }
    return 0;
}