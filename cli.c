#include "psiskv.h"

int main(){
	int socket;
	int port;
	char valor[30];
	char msg[128];
	//char *valu = valor;
	
	//uint32_t length;
	printf("porta?\n");
	scanf("%d",&port);
	
	socket=kv_connect("127.0.1.1",port);
	
	
	printf("socket : %d\n",socket);
	
	//write(socket,"hello\n",7);
	
	bzero(valor,30);
	printf("mensagem?\n");
	scanf("%s",msg);
	int u=strlen(msg);
	printf("vai enviar isto %s de tamanho %d",msg,u);
	//length=6;
	
	if((kv_write(socket,16,msg,strlen(msg),0))==0)
		puts("against all odds it worked\n");
	
	
	//printf("lido: %s\n",valu);
	
	exit(0);
}
