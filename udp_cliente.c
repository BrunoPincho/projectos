#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(){

	int sockfd;
	struct sockaddr_in front_addr;
	int n;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0))<0)
		perror("socket\n");
	
	memset((void*)&front_addr,(int)'\0',sizeof(front_addr));
	front_addr.sin_family = AF_INET;
	front_addr.sin_addr.s_addr= inet_addr("127.0.1.1");
	front_addr.sin_port=htons(8000);
	
	n=sendto(sockfd,"hello\n",7,0,(struct sockaddr*)&front_addr,sizeof(front_addr));
					
	

		exit(0);
}
