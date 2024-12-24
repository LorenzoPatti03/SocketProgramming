#include <stdio.h>
#include <stdlib.h>     /* exit() */
#include <string.h>    /* bzero(), bcopy() */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    //prendo i requisiti

    struct sockaddr_in local_addr;
    int local_addr_len = sizeof(local_addr);

    if(getsockname(sockfd, (struct sockaddr*) &local_addr, (socklen_t*) &local_addr_len) < 0){
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

    int c;
    scanf("%d",&c);

    n = write(sockfd,(char*) &mess_to_write, sizeof(mess_to_write));
    if (n < 0)
         error("ERROR writing to socket");

	bzero(buffer,256);
	n = read(sockfd,buffer,255);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);

	close(sockfd);

    return 0;
}