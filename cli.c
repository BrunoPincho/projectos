#include "psiskv.h"

int main(){
	int socket;
	int port;
	
	printf("porta?\n");
	scanf("%d",&port);
	
	socket=kv_connect("127.0.1.1",port);
	
	
	printf("socket : %d\n",socket);
	
	write(socket,"hello\n",7);
	
	exit(0);
}
