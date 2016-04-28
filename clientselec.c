#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc,char *argv[]){
	int sd;
	struct sockaddr_in serv_soc;
	char mensagem[128],msgrec[128];
	int port;
	if((sd= socket(AF_INET,SOCK_STREAM,0))<0)
		exit(0);
		
	bzero ((char *)&serv_soc,sizeof(serv_soc));
	serv_soc.sin_family=AF_INET;
	serv_soc.sin_addr.s_addr=inet_addr("127.0.1.1");
	printf("porta?\n");
	scanf("%d",&port);
	serv_soc.sin_port = htons(port);
	
	if(connect (sd, (struct sockaddr *) &serv_soc,sizeof(struct sockaddr))<0)exit(1);
	printf("cliente tcp >estabeleceu ligação\n");
	fd_set all_set, r_set;
	int maxfd= sd +1;
	FD_ZERO(&all_set);
	FD_SET(STDIN_FILENO,&all_set);FD_SET(sd,&all_set);
	r_set = all_set;
	//struct timeval tv; tv.tv_sec; tv.tv_usec = 0;
	puts("enter message: ");
	
for(;;){	
		r_set = all_set;
		
		select(maxfd,&r_set,NULL,NULL,(struct timeval *)NULL);
		
		if(FD_ISSET(STDIN_FILENO,&r_set)){
			if(fgets(mensagem,128,stdin)!=NULL){
				
				if(write(sd,mensagem,strlen(mensagem)-1)<0)
				{
					puts("write fail\n");
					return 1;
				}
				bzero(mensagem,128);
			
			}
		}
	
	if(FD_ISSET(sd,&r_set)){
		
		if(read(sd,msgrec,128)<0)
		{
			puts("read fail\n");
			break;
		}
		if(strlen(msgrec)>0)
		printf("server: %s\n",msgrec);
		bzero(msgrec,128);
		}
	}
	
	close(sd);
	
	
	
	exit(0);
}
