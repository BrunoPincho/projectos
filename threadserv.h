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
#include <signal.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int fs;//socket do front_server
int ds;//socket do data_server
int socket_udp;//socket udp do data server
int cria_server(int servidor);//servidor faz a diferença entre front e data server
struct sockaddr_in servsoc;

#define nthread 128
void* thread_accept(void* sd);
int sum_trd;
void morte_geral();
pthread_mutex_t mux;
int global;
void *ler_teclado(void* fd);
void *Master_thread();
//pipeline
int udp_server();


////servidores///////////////////////////
void Frontserver();
void DataServer();
	
////////////////////////////////////////	
int udp_cliente();
struct sockaddr_in front_addr;
int fd;
/////////
int quit;

void dead_child(int sig_num);//detecta se o filho terminou subitamente
void dead_parent(int sig_num);//detecta se o processo pai terminou subitamente sem terminar o processo filho
////variaveis do mutexe
struct clausa{
	int valor;
}sincro;

//////reboots
void Reboot();

/////////estrutura com os valores///////

struct Pacote{
	uint32_t key;
	uint32_t value_length;
	char* value;	
	char modo;
};

///shared memory
int cria_shmem(int porta);
void acede_shmem(char* porta);
void terminu_shmem();

#endif
