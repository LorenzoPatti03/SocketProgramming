#include <stdio.h>
#include <stdlib.h>     /* exit() */
#include <string.h>    /* bzero(), bcopy() */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


void error(char *masg){
    perror(masg);
    exit(1);
}

int main(int argc,char* argv[]){
    int sockfd, len_cli,len_serv;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256],pwd[256];

    if(argc < 3)
        error("Troppi pochi argomenti");

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd < 0)
        error("Errore nell'apertura della socket");

    
    server = gethostbyname( argv[1] );
    
    len_serv = sizeof(serv_addr);
    bzero( (char*) &serv_addr,len_serv);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( atoi(argv[2]) );
    bcopy( (char*) server->h_addr, (char*) &serv_addr.sin_addr, server->h_length);


    if( connect(sockfd,(struct sockaddr*) &serv_addr, (socklen_t) len_serv) < 0)
        error("Errore nella connect");


    for(int i = 0; i<3;i++){

        printf("Inserisci la parola segreta: ");
        scanf(" %[^\n]",&pwd);

        strcpy(buffer, pwd);


        if( write(sockfd,buffer,256) < 0)
            error("Errore in scrittura");

        if( read(sockfd,buffer,256) < 0)
            error("Errore in lettura");

        printf("%s\n",buffer);


    }

    


}