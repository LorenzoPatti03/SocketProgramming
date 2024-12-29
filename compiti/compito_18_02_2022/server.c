#include <stdio.h>
#include <ctype.h>
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

int isNumber(char *str) {
    int i = 0;
    
    if (str[0] == '\0') 
        return 0; // Non è un numero

    while(str[i] != '\0') {

        if (!isdigit(str[i])) 
            return 0; 

        i++;
    }

    return 1; // La stringa rappresenta un numero
}



int main(int argc,char *argv[]){
    int sockfd, servlen,clilen;
    struct sockaddr_in serv_addr,cli_addr;
    char buffer[MAXSIZE];
    

    if(argc < 2)
        error("Troppi pochi parametri");
    
    sockfd = socket(AF_INET, SOCK_DGRAM,0);

    if(sockfd < 0)
        error("errore nell'apertura della socket");


    servlen = sizeof(serv_addr);
    bzero( (char*) &serv_addr,servlen);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( atoi( argv[1] ) );
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(sockfd,(struct sockaddr*) &serv_addr,(socklen_t) servlen) < 0)
        error("Errore nel binding");
    
    while(1){

        int numero = rand()%101; 

        printf("%d\n",numero);
       
    
            
        for(int i = 0; i < 3 ; i++){

            bzero((char*) buffer,MAXSIZE);
            clilen = sizeof(cli_addr);
            bzero( (char*) &cli_addr,clilen);
            char riposta[MAXSIZE];
            bzero( (char*) &riposta,MAXSIZE);

            printf("%d\n",i);

            if( recvfrom(sockfd,buffer,MAXSIZE,0, (struct sockaddr*) &cli_addr,(socklen_t *) &clilen) < 0)
                error("errore in lettura");

            if(!isNumber(buffer))
                sprintf(riposta,"Is not a number!!\n");
                
            else
                if(numero != atoi(buffer) )
                    sprintf(riposta,"Wrong Number\n");
                    
                else{
                    sprintf(riposta,"Correct Number\n");
                    bcopy(riposta,buffer,MAXSIZE);
                    if( sendto(sockfd,buffer,MAXSIZE,0, (struct sockaddr*) &cli_addr,(socklen_t) clilen) < 0)
                        error("errore in scrittura");
                    break;
                }                

            bcopy(riposta,buffer,MAXSIZE);

            if( sendto(sockfd,buffer,MAXSIZE,0, (struct sockaddr*) &cli_addr,(socklen_t) clilen) < 0)
                error("errore in scrittura");

                
        }
            
        
    }
   

    return 0;

}