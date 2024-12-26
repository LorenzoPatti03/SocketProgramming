#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void error(char *masg){
    perror(masg);
    exit(1);
}

int rigth_tamplate(char* msg) {

    if( strncmp("Hello! My name is ",msg,18) == 0 )
        if(msg[ strlen(msg)-1] == '.')
            return 1;
    
    return 0;
}

int main(int argc,char *argv[]) {
    int sockfd, newsockfd;
    int portno, clilen, servlen;
    char buffer[256];
    struct sockaddr_in serv_addr,cli_addr;
    pid_t pid;

    if(argc < 2)
        error("ERROR opening socket");

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd < 0)
        error("Errore nella socket");

    
    servlen = sizeof(serv_addr);

    bzero( (char*) &serv_addr, servlen);
    portno = atoi( argv[1] );
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(sockfd, (struct sockaddr *) &serv_addr, (socklen_t) servlen) < 0)
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

            char nome_client[200];

            while(1){
                bzero( (char *) &buffer,256);
                bzero( (char *) &nome_client,200);

                if( read(newsockfd,buffer,256) < 0)
                    error("Errore nella lettura");

                if( rigth_tamplate(buffer) == 1 ){
                    char risposta[256];
                    strcpy(nome_client, &buffer[18]);
                    nome_client[strcspn(nome_client, ".")] = '\0';

                    sprintf(risposta,"Hello %s! My name is %s.", nome_client, argv[0]);
                    strcpy(buffer, risposta);
                }else{
                    char* risposta = "Wrong Message";
                    strcpy(buffer,risposta);
                }
                
                if( write(newsockfd,buffer,256) < 0)
                    error("errore scrittura");
            
            }
            
            return 0;
        }


        close(newsockfd);
        return 0;
    }





}