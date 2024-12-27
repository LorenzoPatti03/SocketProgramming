#include <stdio.h>

#include <stdlib.h>     /* exit() */
#include <string.h>    /* bzero(), bcopy() */

#include <unistd.h>
#include <netinet/in.h>
#include <time.h>       /* time_t e ctime() */
#include <sys/types.h>  /* tipi di dati di sistema */
#include <sys/socket.h> /* definizioni utili per le socket() */

#define MAXSIZE 4096

int error(char *msg){
    perror(msg);
    exit(1);    
}

void getAddress1(char * addr,char *name){
    bcopy(addr,"192.168.1.1",MAXSIZE);
}

int main(int argc,char argv[]){
    int sockfd, servlen,clilen;
    struct sockaddr_in serv_addr,cli_addr;
    char buffer1[MAXSIZE],buffer2[MAXSIZE];

    if(argc < 2)
        error("Troppi pochi parametri");
    
    sockfd = socket(AF_INET, SOCK_DGRAM,0);

    if(sockfd < 0)
        error("errore nell'apertura della socket");


    servlen = sizeof(serv_addr);
    bzero( (char*) &serv_addr,servlen);
    serv_addr.sin_addr = AF_INET;
    serv_addr.sin_port = htons( atoi( argv[1] ) );
    serv_addr.sin_addr = INADDR_ANY;

    if( bind(sockfd,(struct sockaddr*) &serv_addr,(socklen_t) servlen) < 0)
        error("Errore nel binding");
    
    while(1){

        bzero((char*) buffer1,MAXSIZE);
        bzero((char*) buffer2,MAXSIZE);
        clilen = sizeof(cli_addr);
        bzero( (char*) &cli_addr,clilen);

        if( recvfrom(sockfd,buffer1,MAXSIZE,0, (struct sockaddr*) &cli_addr,(socklen_t *) &clilen) < 0)
            error("errore in lettura");

        getAddress1(buffer2,buffer1);

        if( sendto(sockfd,buffer2,MAXSIZE,0, (struct sockaddr*) &cli_addr,(socklen_t *) &clilen) < 0)
            error("errore in lettura");
    }
   



}