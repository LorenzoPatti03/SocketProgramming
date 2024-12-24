#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg,int socketBenvenuto, int socketOperativa){
	perror(msg);
	close(socketBenvenuto);
	if(socketOperativa >= 0)
		close(socketOperativa);

	printf("lalal");
	exit(1);
}

int main(int argc, char *argv[]){
	int socketBenvenuto,socketOperativa;
	int port_n_Benvenuto, client_len;
	char buffer[256];
	struct sockaddr_in serv_addr,client_address;
	int n, counter, pid;
	pid_t process_identifier;
	
	counter = 0;
	
	if(argc < 2){ 
		fprintf(stderr,"Error,no port provieded"); 
		exit(1);
	}
	
	socketBenvenuto = socket(AF_INET,SOCK_STREAM,0);
	
	if(socketBenvenuto < 0){ 
		error("Error opening socket",socketBenvenuto,-1);
    }
	
	bzero( (char*) &serv_addr, sizeof(serv_addr) );
	port_n_Benvenuto = atoi(argv[1]) ;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port_n_Benvenuto);

	if( bind(socketBenvenuto, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ 
            error("Error on binding",socketBenvenuto,-1);
    
        }
	
	//scanf("%d", &counter);
	
	listen(socketBenvenuto,10);
	//scanf("%d", &counter);

	do{
		client_len = sizeof(client_address);
		socketOperativa = accept(socketBenvenuto, (struct sockaddr *) &client_address, (socklen_t *) &client_len );


		if(socketOperativa < 0){
			error("Error an accept",socketBenvenuto,-1);
		}
		pid = fork();

		if(pid == 0){

			close(socketBenvenuto);
			bzero(buffer,256);
			n = read(socketOperativa,buffer,255);

			if( n < 0){
				error("Error,reading to socket",socketBenvenuto, socketOperativa);
			}

			if(!strcmp(buffer, "T\n")){
				counter++;
				time_t t = time(NULL);

				char* timestr = ctime(&t);

				n = write(socketOperativa,timestr,strlen(timestr)+1);
				
				if(n<0){
					error("Error,writing socket",socketBenvenuto, socketOperativa);
				}
			}else if(!strcmp(buffer,"N\n")) {
				char cnt[5];
				sprintf(cnt,"%d",counter);
				n = write(socketOperativa,cnt,strlen(cnt));

				if(n<0){
					error("Error,writing socket",socketBenvenuto, socketOperativa);
				}
			}else {

				printf("Please press a key: ");
				char c;
				scanf("%c",&c);
				n = write(socketOperativa,"messagge received",20);
				if(n<0){
					error("Error,writing socket",socketBenvenuto, socketOperativa);
				}
			}
			close(socketOperativa);
          	return 0;
		}else { 

			close(socketOperativa);

		}
	}while(1);
	close(socketBenvenuto);
	return 0;
	
}
