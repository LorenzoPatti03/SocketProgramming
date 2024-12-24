#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(char *msg,int socketBenvenuto, int socketOperativa){
	perror(msg);
	close(socketBenvenuto);
	if(socketOperativa >= 0)
		close(socketOperativa);

	printf("lalal");
	exit(1);
}


int main(int argc,char* argv[]){

    int socketBenvenuto, socketOperativa,n;
    struct sockaddr_in serv_addr, client_address;
    int client_len,port_number;
    int pid;
    char buffer[256];


    socketBenvenuto = socket(AF_INET,SOCK_STREAM,0);

    if(socketBenvenuto < 0)
        error("socket non creata",socketBenvenuto,-1);
    
    bzero( (char*) &serv_addr, sizeof(serv_addr) );
    port_number = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);
    

    if( bind(socketBenvenuto,(struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("bind non effettuato",socketBenvenuto,-1);
    
    listen(socketBenvenuto, 200);

    do{
        client_len = sizeof(client_address);

        socketOperativa = accept(socketBenvenuto, (struct sockaddr*) &client_address,  (socklen_t*) &client_len);

        if(socketOperativa < 0)
            error("creazione socket operativa non riuscita",socketBenvenuto,-1);
        
        pid = fork();

        if(pid == 0){

            close(socketBenvenuto);
            bzero( buffer, 256);

            n = read(socketOperativa,buffer, 255);
            printf("%s\n",buffer);

            if( n < 0)
                error("Errore in lettura",socketBenvenuto,socketOperativa);

            //if(!strcmp(buffer,"T\n")){
                

            struct sockaddr_in local_addr;
            int local_addr_len = sizeof(local_addr);

            if(getsockname(socketOperativa, (struct sockaddr*) &local_addr, (socklen_t*) &local_addr_len) < 0){
                error("non sono riuscito ad ottenere la porta",socketBenvenuto,socketOperativa);
            }
                char ip[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &local_addr.sin_addr, ip, sizeof(ip)) == NULL) {
                error("Errore nella conversione dell'indirizzo IP", 0, socketOperativa);
            }

            // Ottieni la porta (usando ntohs per convertirla da network byte order a host byte order)
            int porta = ntohs(local_addr.sin_port);

            // Prepara il messaggio da inviare
            char mess_to_write[INET_ADDRSTRLEN + 10 + 60];  // IP + Porta + spazio per messaggio extra
            snprintf(mess_to_write, sizeof(mess_to_write), "ip: %s, port: %d", ip, porta);
            

            n = write(socketOperativa,(char*) &mess_to_write, sizeof(mess_to_write));
            // Scrivi il messaggio nel socket
            if( n < 0)
                error("Errore in scrittura",socketBenvenuto,socketOperativa);

            /*}else{
                char mess_to_write[] = "comando non corretto!!";

                n = write(socketOperativa,(char*) &mess_to_write, sizeof(mess_to_write));

                if( n < 0)
                    error("Errore in scrittura",socketBenvenuto,socketOperativa);
            }*/
            close(socketOperativa);
          	return 0;
        }else{

            close(socketOperativa);
        }


    }while(1);
    close(socketBenvenuto);
	return 0;





}