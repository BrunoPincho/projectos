#include "threadserv.h"
	
void dead_child(int sig_num){
	
	int status;
	
	wait(&status);
	printf("child exited \n");
}


void Frontserver(){
		int socket;
		pthread_t teclas;
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
	
}


void DataServer(){
		pthread_t Master;
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


int cria_server(int servidor){
	if(servidor==1){
	//meter noutro ficheiro 
			if((fs = socket(AF_INET,SOCK_STREAM,0))<0){
			puts("erro no socket");
			exit(-1);
			}		

			printf("socket tcp nº %d criada\n",fs);

			memset((void*)&servsoc,(int)'\0',sizeof(servsoc));
			servsoc.sin_family = AF_INET;
			servsoc.sin_addr.s_addr = htonl(INADDR_ANY);
			int port=9999;
			servsoc.sin_port = htons(port);

			while(bind (fs,(struct sockaddr *) &servsoc, sizeof(servsoc))==-1){
				printf("erro bind\t mudar a porta\n");	
				port--;
				servsoc.sin_port = htons(port);
			}
			printf("porta %d \n",port);
			////
			if(listen(fs,5)==-1)exit(1);
			return port;
	}
	
	if(servidor==0){
		int porta=9005;
		
			if((ds = socket(AF_INET,SOCK_STREAM,0))<0){
		puts("erro no socket");
		exit(-1);
		}		

		printf("socket tcp nº %d criada\n",ds);

		memset((void*)&servsoc,(int)'\0',sizeof(servsoc));
		servsoc.sin_family = AF_INET;
		servsoc.sin_addr.s_addr = htonl(INADDR_ANY);
		servsoc.sin_port = htons(porta);

		if(bind (ds,(struct sockaddr *) &servsoc, sizeof(servsoc))<0){
			printf("erro bind DS n");	
			exit(-1);
		}
		
		if(listen(ds,5)==-1)exit(1);
		
		return porta;
	}
	return -1;
}

void* thread_accept(void *sd){
		
	char buffer[128];
	int socket;
	socket=*((int *) sd);
	while(1){
				if(read(socket,buffer,128)<0)
						exit(0);
							
				/*pthread_mutex_lock(&mux);
					printf("%d\n",sincro.valor);
					sincro.valor++;
				pthread_mutex_unlock(&mux);	*/	
	
				if(strcmp(buffer,"\0")!=0){					
					printf("%s \n",buffer);	
					}
				if(strncmp(buffer,"\0",1)==0){
					puts("thread a sair");
					close(socket);
					pthread_exit(NULL);
					return NULL;
				}
				
				bzero(buffer,128);		
			}

		return NULL;
}

void *Master_thread(){
	pthread_t thread[nthread];
	
	int sock,addrlen,i;
		
	while(1){
			addrlen=sizeof(servsoc);
			if((sock=accept(ds,(struct sockaddr*)&servsoc,(socklen_t *)&addrlen))==-1){
								puts("sai no ACCEPT");				
								exit(1);
				}
					
					//sscanf(leitura,"%d",&sock);		
					//
					
				printf("connectada a socket:%d",sock);			
				printf("criada thread #%d\n",sum_trd);
				pthread_create(&thread[sum_trd],NULL,thread_accept,&sock);
				sum_trd++;	
			
		}
	
	for(i=0;i<sum_trd;i++){
			pthread_join(thread[i],NULL);
		}
	
}



void *ler_teclado(void *fd){
char mensagem[128];
	int dataserver;
	dataserver=*((int *) fd);

	puts("a ler do teclado\n");
	while(fgets(mensagem,128,stdin)!=NULL){

		printf("\nfoi escrito: %s \n",mensagem);
		if(strncmp(mensagem,"exit",4)==0){
			/*if((sendto(dataserver,"\0",2,0,(struct sockaddr*)&front_addr,sizeof(front_addr))<0))
					printf("erro send\t");
			*/
			close(fs);
			exit(0);
			
			}
	}	
	return NULL;
}

int udp_server(){
	
	int sockfd;
	struct sockaddr_in serv_addr;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0))<0)
		perror("socket\n");
	
	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr= htonl(INADDR_ANY);
	serv_addr.sin_port=htons(8000);
	
	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
		printf("error bind udp\n");
	}
	
	return sockfd;
}



int udp_cliente(){
	int sockfd;
	
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0))<0)
		perror("socket\n");
	
	bzero((char*)&front_addr,sizeof(front_addr));
	front_addr.sin_family = AF_INET;
	front_addr.sin_addr.s_addr= inet_addr("127.0.1.1");
	front_addr.sin_port=htons(8000);
	
	
	return sockfd;
}
