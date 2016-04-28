#ifndef THREADSERV_H
#define THREADSERV_H

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


int fs;//socket do front_server
int ds;//socket do data_server
int cria_server(int servidor);//servidor faz a diferença entre front e data server
struct sockaddr_in servsoc;

#define nthread 128
void* thread_accept(void* sd);
int sum_trd;

pthread_mutex_t mux;
int global;
void *ler_teclado(void* fd);
void *Master_thread();
//pipeline
int udp_server();
	
int udp_cliente();
struct sockaddr_in front_addr;
int fd;
/////////
int quit;

struct clausa{
	int valor;
}sincro;

#endif
