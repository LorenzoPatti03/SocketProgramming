
#include <stdio.h>
#include <stdlib.h>     /* exit() */
#include <string.h>    /* bzero(), bcopy() */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}


int main(int argc, char * argv[]){


    int socket_client, server_len, port_number_serv;
    struct sockaddr_in server_addr;
    struct hostent *ip_server;
    char buffer[1000]; 

    
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    port_number_serv = atoi(argv[2]);
    socket_client = socket(AF_INET,SOCK_STREAM,0);

    if(socket_client < 0)
        error("socket non creata");

    ip_server = gethostbyname( argv[1]);
    if (ip_server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)ip_server->h_addr_list[0],
         (char *)&server_addr.sin_addr.s_addr,
         ip_server->h_length);
    server_addr.sin_port = htons(port_number_serv);
    
    server_len = sizeof(server_addr);

    if( connect(socket_client, (struct sockaddr*) &server_addr, (socklen_t) server_len) < 0)
        error("errore nella connessione al server");

    while(1){

        printf("scrivi una frase: ");
        scanf("%s",buffer);

        if( write(socket_client,buffer,999) < 0)
            error("scrittura non riuscita");

        if( read(socket_client,buffer,999) < 0)
            error("scrittura non riuscita");

        printf("%s\n",buffer);

    }
    close(socket_client);
    return 0;
    
}