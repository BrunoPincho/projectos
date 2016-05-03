#include "psiskv.h"

int* serializacao(char *msg){
	int *z;
	z=(int*)malloc(strlen(msg)*sizeof(int));
	int i;
	for(i=0;i<strlen(msg);i++){
		z[i]=msg[i];
	}
	return z;	
}

int kv_connect(char* kv_server_ip,int kv_server_port){
	int sd,data_server;
	char porta[5];
	int port;
	struct sockaddr_in serv_soc,data_soc;
	////cria socket////
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0)
		return -1;
	
	if((data_server=socket(AF_INET,SOCK_STREAM,0))<0)
		return -1;	
		
	/////	
	
	////estruturar o sockaddr
	
	bzero((char*)&serv_soc,sizeof(serv_soc));
	
	serv_soc.sin_family=AF_INET;
	serv_soc.sin_addr.s_addr=inet_addr(kv_server_ip);
	serv_soc.sin_port=htons(kv_server_port);
	////
	
	if(connect(sd,(struct sockaddr*)&serv_soc,sizeof(struct sockaddr))<0){
		return -1;
		}
		
	read(sd,porta,5);
	sscanf(porta,"%d",&port);
	
	close(sd);
	
	bzero((char*)&data_soc,sizeof(data_soc));
	
	data_soc.sin_family=AF_INET;
	data_soc.sin_addr.s_addr=inet_addr(kv_server_ip);
	data_soc.sin_port=htons(port);
	
	if(connect(data_server,(struct sockaddr*)&data_soc,sizeof(struct sockaddr))<0){
		return -1;
		}
	
	return data_server;
}



void kv_close(int kv_descriptor){
	
	if((write(kv_descriptor,"\0",1))<0){
		puts("erro a desconnectar\n");
		return;
	}
	close(kv_descriptor);
	return;
		
}

int kv_write(int kv_descriptor,uint32_t key,char* value,uint32_t value_length,int kv_overwrite){
	//char *modo;
	char ACK[3];
	/*char chave[10];
	char length[10];
	char *mensagem;*/
	struct timeval tv;
	int nsele;
	
	
	fd_set tcpsock;	
	FD_ZERO(&tcpsock);
	tv.tv_sec=3;
	tv.tv_usec=0;
	FD_SET(kv_descriptor,&tcpsock);
	
	if(kv_overwrite==1){
		pacote.modo='O'; //o maiusculo010
		}else{
			pacote.modo='W';
		}
		
		pacote.key=16;
		pacote.value_length=value_length;
		pacote.value=(char*)malloc(pacote.value_length*sizeof(char));
		strcpy(pacote.value,value);
		//strcpy(pacote.value,"olaa");
		
	/*sprintf(chave,"%u",key);
	sprintf(length,"%u",value_length);
	
	mensagem=(char *)malloc((value_length+strlen(chave)+4)*sizeof(char));*/
	
	/*
/////cosntruir a mensagem//
	strcat(mensagem,modo);
	strcat(mensagem,"-");
	strcat(mensagem,chave);
	strcat(mensagem,"-");
	strcat(mensagem,value);
	strcat(mensagem,"-");
	strcat(mensagem,length);
	strcat(mensagem,"\0");
///////////////////////////
	*/
	if((write(kv_descriptor,&pacote,sizeof(pacote)))<0){
		puts("erro a enviar\n");
		return -1;
	}
	
	nsele=select(kv_descriptor+1,&tcpsock,0,0,&tv);
	
	if(nsele==0){
		puts("key value storage offline,socket fechada\n");
		close(kv_descriptor);
		//free(mensagem);
		return -1;
	}
	
	if(nsele<0){
		puts("erro no selec de timeout\n");
		//free(mensagem);
		return -1;
	}
	
	if((read(kv_descriptor,ACK,3))<0){
		puts("falhou o read");
		//free(mensagem);
		return -1;	
	}
	if(strncmp(ACK,"Ack",3)==0){
		puts("operação realizada com sucesso");
		//free(mensagem);
		if((write(kv_descriptor,pacote.value,value_length))<0){
		puts("erro a enviar\n");
		return -1;
		}
		return 0;
	}
	
	return -1;
}

int kv_read(int kv_descriptor, uint32_t key,char* value, uint32_t value_length){
	char *mensagem;
	char *resposta;
	char chave[10];
	struct timeval tv;
	int nsele;
	fd_set tcpsock;	
	FD_ZERO(&tcpsock);
	tv.tv_sec=3;
	tv.tv_usec=0;
	FD_SET(kv_descriptor,&tcpsock);
		
	sprintf(chave,"%u",key);
	resposta=(char *)malloc((strlen(chave)+value_length+2)*sizeof(char));
	mensagem=(char *)malloc((strlen(chave)+3)*sizeof(char));
	strcat(mensagem,"R");
	strcat(mensagem,"-");
	strcat(mensagem,chave);
	strcat(mensagem,"\0");
	
	if((write(kv_descriptor,mensagem,strlen(mensagem)))<0){
		puts("erro a enviar\n");
		return -1;
	}
	
	nsele=select(kv_descriptor+1,&tcpsock,0,0,&tv);
	
	if(nsele==0){
		puts("key value storage offline,socket fechada\n");
		close(kv_descriptor);
		free(mensagem);
		free(resposta);
		return -1;
	}
	
	if(nsele<0){
		puts("erro no selec de timeout\n");
		free(mensagem);
		free(resposta);
		return -1;
	}
	
	if((read(kv_descriptor,resposta,(strlen(chave)+value_length+2)))<0){
		puts("falhou o read");
		free(mensagem);
		free(resposta);
		return -1;	
	}
	
	strtok(resposta,"-");
	strcpy(value,resposta);
	if(strcmp(value,"\0")==0){
		puts("valor não existente");
		return -2;
	}
	
	free(mensagem);
	free(resposta);
	return strlen(value);
}


int kv_delete(int kv_descriptor,uint32_t key){
	char *mensagem;
	char chave[10];
	sprintf(chave,"%u",key);
	mensagem=(char *)malloc((strlen(chave)+3)*sizeof(char));
	strcat(mensagem,"K");
	strcat(mensagem,"-");
	strcat(mensagem,chave);
	strcat(mensagem,"\0");
	
	if((write(kv_descriptor,mensagem,strlen(mensagem)))<0){
		puts("erro a enviar\n");
		return -1;
	}
	
	free(mensagem);
	return 1;
}



