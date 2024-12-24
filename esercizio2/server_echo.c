#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h> // per la funzione toupper()

void error(char *msg,int socketBenvenuto, int socketOperativa){
	perror(msg);
	close(socketBenvenuto);
	if(socketOperativa >= 0)
		close(socketOperativa);
	exit(1);
}

void convertiMaiuscolo(char str[]) {
    int i = 0;
    // Cicla su ogni carattere della stringa fino al termine (null terminator '\0')
    while (str[i] != '\0') {
        str[i] = toupper(str[i]); // Converte il carattere in maiuscolo
        i++;
    }
}

int main(int argc,char* argv[]){
    int socketBenvenutofd, socketOperativafd;
    struct sockaddr_in serv_addr, client_addr;
    int client_len,serv_len;
    int pid;
    char buffer[1000];

    if(argc < 2)
        exit(1);

    socketBenvenutofd = socket(AF_INET,SOCK_STREAM,0);

    if(socketBenvenutofd < 0)
        error("socket non creata",socketBenvenutofd,-1);
    
    bzero( (char*) &serv_addr, sizeof(serv_addr) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( atoi(argv[1]) );
    
    serv_len = sizeof(serv_addr);
    if( bind(socketBenvenutofd,(struct sockaddr*) &serv_addr, (socklen_t) serv_len) < 0)
        error("bind non effettuato",socketBenvenutofd,-1);

    listen(socketBenvenutofd,200);
    
    while(1){

        client_len = sizeof(client_addr);
        socketOperativafd = accept(socketBenvenutofd,(struct sockaddr *) &client_addr, (socklen_t*) &client_len);
        
        if(socketOperativafd < 0)
            error("error nella socket operativa",socketBenvenutofd,socketOperativafd);
        
        pid = fork();

       

        if(pid == 0){
            close(socketBenvenutofd);
            int i = 0;
            while(i < 5){
                bzero( buffer, 1000);

                if( read(socketOperativafd,buffer,999) < 0)
                    error("errore in lettura",socketBenvenutofd,socketOperativafd);
                    
                printf("%s\n",buffer);
                convertiMaiuscolo(buffer);
                printf("%s\n",buffer);

                if( write(socketOperativafd,buffer,999) < 0)
                    error("errore in lettura",socketBenvenutofd,socketOperativafd);
                i++;
            }
            close(socketOperativafd);
            return 0; //serrve per chiudere il processo figlio da mettere assolutamente
        }

        close(socketOperativafd);        

    }
    return 0;
}