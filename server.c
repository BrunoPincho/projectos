#include "threadserv.h"


int main(){
	int fd[2];
	pipe(fd);
	pid_t front_server;
	
	sum_trd=0;
	pthread_t teclas;
	pthread_t Master;
	
	int socket;
	//inicializar o mutexe
	sincro.valor=0;
	
	pthread_mutex_init(&mux,NULL);

	/////
	if((front_server = fork())==-1){
		perror("fork");
		exit(-1);
	}
	
	if(front_server == 0){
		puts("entrou front_server");
		
		int fd;
		fd = udp_cliente();
		char resposta[5]="\0";
		
		//close(fd[0]);
		
		pthread_create(&teclas,NULL,ler_teclado,&fd);
		sum_trd++;
		int n;
		cria_server(1);
		int addrlen1;
		
			while(1){
					addrlen1=sizeof(servsoc);
					if((socket=accept(fs,(struct sockaddr*)&servsoc,(socklen_t *)&addrlen1))==-1){
								puts("sai no ACCEPT");				
								exit(1);
							}
							puts("ligou-se\n");	
					
					if((sendto(fd,"P",2,0,(struct sockaddr*)&front_addr,sizeof(front_addr))<0))
						printf("erro send\t");
					
					n=recvfrom(fd,resposta,5,0,(struct sockaddr*)&front_addr,((socklen_t *)&addrlen1));
					
					write(socket,resposta,5);
					
					if(n==-1)exit(0);
				
					
				}
		
	}else{
		int porta=0;
		int socket_udp;
		puts("entrou data server");
		struct sockaddr_in cli_addr;
		int addrlen;
		int n;
		porta=cria_server(0);
		socket_udp = udp_server();
		char leitura[10];
		char responder[5];
		bzero(responder,5);
		pthread_create(&Master,NULL,Master_thread,NULL);
		
		addrlen=sizeof(cli_addr);
		
		while(1){
			
			
			
			n=recvfrom(socket_udp,leitura,10,0,(struct sockaddr*)&cli_addr,((socklen_t *)&addrlen));
			if(n==-1)exit(1);
			
			printf("%s",leitura);
			if(strcmp(leitura,"\0")==0){
				exit(0);
			}
			if(strcmp(leitura,"P")==0){
				sprintf(responder,"%d",porta);
				if((sendto(socket_udp,responder,strlen(responder)+1,0,(struct sockaddr*)&cli_addr,sizeof(cli_addr))<0))
					printf("erro send\t");
				
			}
			bzero(leitura,10);
			
			
			
		}

			
		

		close(ds);
	}
	exit(1);
}
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		
		
		
		
		
		
