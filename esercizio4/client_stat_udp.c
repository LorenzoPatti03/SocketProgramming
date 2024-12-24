#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h> // per la funzione toupper()
#include <netdb.h> //per ottenere l'ip dato l'hostname
#include <arpa/inet.h>

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
    server_len = sizeof(serv_addr);

    //
    struct sockaddr_in local_addr;
    int local_addr_len = sizeof(local_addr);

    if(getsockname(socketfd, (struct sockaddr*) &local_addr, (socklen_t*) &local_addr_len) < 0){
        error("non sono riuscito ad ottenere la porta");
    }
        char ip[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &local_addr.sin_addr, ip, sizeof(ip)) == NULL) {
        error("Errore nella conversione dell'indirizzo IP");
    }

    // Ottieni la porta (usando ntohs per convertirla da network byte order a host byte order)
    int porta = ntohs(local_addr.sin_port);

    // Prepara il messaggio da inviare
    char mess_to_write[INET_ADDRSTRLEN + 10 + 60];  // IP + Porta + spazio per messaggio extra
    snprintf(mess_to_write, sizeof(mess_to_write), "ip: %s, port: %d", ip, porta);
    //


    if( sendto(socketfd,mess_to_write,strlen(mess_to_write)+1,0, (struct sockaddr*) &serv_addr, (socklen_t) server_len) < 0)
        error("errore in scrittura");

    
    if( recvfrom(socketfd,mess_to_write,strlen(mess_to_write)+1,0, NULL, NULL) < 0)
        error("errore in lettura");

    printf("%s\n",mess_to_write);

    return 0;
}