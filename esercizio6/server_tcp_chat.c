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

#define MAXLINE 4096


int error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char * argv[]){
    int socketOperativafd, socketBenvenutofd, server_len, client_len;
    char buffer[ MAXLINE ];
    struct sockaddr_in server_addr,client_addr;
    int pid;


    if(argc < 2)
        error("non hai inserito il numero di porta");

    socketBenvenutofd = socket(AF_INET,SOCK_STREAM,0);

    if( socketBenvenutofd < 0 )
        error("errore nella creazione della socket");


    server_len = sizeof(server_addr);
    bzero( (char*) &server_addr, server_len);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons( atoi( argv[1] ) );

    if( bind(socketBenvenutofd, (struct sockaddr *) &server_addr, (socklen_t) server_len) < 0)
        error("errore nel binding");

    listen(socketBenvenutofd,1000);
    
    while(1){

        client_len = sizeof(client_addr);
        socketOperativafd = accept(socketBenvenutofd, (struct sockaddr *) &client_addr, (socklen_t*) &client_len);


        pid = fork();
        

        if(pid == 0){

            close(socketBenvenutofd);
            while(1){


                bzero(buffer, MAXLINE);

                if(socketOperativafd < 0)
                error("errore nella socket");

                if( read(socketOperativafd,buffer,MAXLINE) < 0)
                    error("errore in lettura");

                printf("%s\n",buffer);
                bzero(buffer, MAXLINE);

                printf("Inserisci una stringa: ");
                scanf(" %s[^\n]",buffer);

                if( write(socketOperativafd,buffer,MAXLINE) < 0)
                    error("errore in lettura");

                printf(buffer);
                printf(" ");

            }
           
            return 0;
        }

        close(socketOperativafd);


    }


    return 0;

}
