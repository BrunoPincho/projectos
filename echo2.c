#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

extern int errno;


int main(){
	int fd;
	struct sockaddr_in cliente;
	char mensagem[128];
	fd=socket(AF_INET,SOCK_STREAM,0);
		
	bzero((char *)&cliente,sizeof(cliente));
	cliente.sin_family=AF_INET;
	cliente.sin_addr.s_addr= inet_addr("127.0.1.1");
	cliente.sin_port= htons(9005);

	connect(fd, (struct sockaddr*) &cliente,sizeof(cliente));
	
	/*cliente.sin_port= htons(9005);
	connect(fd, (struct sockaddr*) &cliente,sizeof(cliente));*/
	//while(fgets(mensagem,128,stdin)!=NULL){		


		/*write(fd,mensagem,strlen(mensagem));*/
		write(fd,"hlo",3);
		/*if(strncmp(mensagem,"exit",4)==0){
				puts("vai sair do ciclo");
				write(fd,"\0",1);				
				close(fd);
				break;
			}*/
		
		read(fd,mensagem,strlen(mensagem));
		printf("%s\n",mensagem);	
		bzero(mensagem,128);
	//}
	
	close(fd);
	
	
	exit(0);
}
	
	
	
	
	
	
	
	
	

