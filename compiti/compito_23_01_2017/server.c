#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void error(char *masg){
    perror(msg);
    exit(1);
}

int sameTamplate(char msg){
    char* right_tamplete = "Hello! My name is ";
    int len_tam = sizeof( right_tamplete);
    int len_msg = sizeof(msg);

    for(int i = 0; i < len_tam-1 ;i++)
        if(right_tamplete[i] != msg[i])
            return 0;
    
    if(msg[ len-1 ] != '.')
        return 0;
    
    return 1;
    
}



int main(int argc,char *argv[]) {
    int sockfd, newsockfd;
    int portno, clilen, servlen;
    char buffer[256];
    struct sockaddr_in serv_addr,cli_addr;
    pit_t pid;

    if(argc < 2)
        error("ERROR opening socket");

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd < 0)
        error("Errore nella socket");

    
    servlen = sizeof(serv_addr);

    bzero( (char*) &serv_addr, servlen);
    portno = atoi( argc[1] );
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(sockfd, (struct sockaddr *) &serv_addr, socklen_t servlen) < 0)
        error("Errore nel binding");

    listen(sockfd,400);

    while(1){

        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen );

        if(newsockfd < 0)
            error("errore nell'accept");

        pid = fork();

        if(pid == 0){

            close(sockfd);
            
            bzero( (char *) &buffer,256);

            if( sameTamplate(buffer) == 1 )
                char* risposta = "Hello";


            return 0;
        }


        close(newsockfd);
        return 0;
    }





}