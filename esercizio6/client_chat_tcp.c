#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h> // tipi di dati di sistema
#include <sys/socket.h> // definizioni utili per le socket
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 4096


int error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char * argv[]){
    int socketfd, server_len;
    char buffer[ MAXLINE ];
    struct sockaddr_in server_addr;
    struct hostent *ip_server;


    if(argc < 3)
        error("mettere hostname e porta");

    socketfd = socket(AF_INET,SOCK_STREAM,0);

    if(socketfd < 0)
        error("errore nella creazione della socket ");

    ip_server = gethostbyname( argv[1] );


    server_len = sizeof(server_addr);
    bzero( (char*) &server_addr,server_len);

    
    server_addr.sin_family = AF_INET;
    bcopy( (char*) ip_server -> h_addr_list[0],
            (char*) &server_addr.sin_addr.s_addr, ip_server->h_length );
    
    server_addr.sin_port = htons( atoi(argv[2]));

    if(connect(socketfd, (struct sockaddr *) &server_addr, server_len) < 0 )
        error("errore nella connessione al server");

    while(1){

        printf("Iserisci una stringa: ");
        scanf(" %s[^\n]",buffer);

        if( write(socketfd,buffer,MAXLINE) < 0 )
            error("Errore in scrittura");

        if( read(socketfd,buffer,MAXLINE) < 0)
            error("Errore in lettura");

        printf(buffer);
        printf(" ");

    }
    
    return 0;
    

}