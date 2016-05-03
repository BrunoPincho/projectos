#include "threadserv.h"






typedef struct _ListValues_{
	uint32_t key;
	char* value;
	struct ListValues* next;
	struct ListValues* ant;
}LIST;

/*************************************************************************************************************************/
//Cria um novo elemento na lista
LIST** novalor(LIST** a, uint32_t key,char* value){

LIST* novop;
LIST* aux=NULL;

aux=*a;
	//Caso seja o primeiro
	if(aux==NULL){
		novop = (LIST *)malloc(sizeof(LIST));
			if(novop == NULL) exit(-2);

		strcpy(novop->value,value);
		novop->key=key;
		novop->next=NULL;
		novop->ant=NULL;

		aux=novop;
		*a=aux;
		return a;
	}
	
aux=*a;

	//Caso nao seja o primeiro
		
	novop = (LIST *)malloc(sizeof(LIST));
		if(novop == NULL) exit(-2);

	strcpy(novop->value,value);
	novop->key=key;
	novop->next=NULL;
	novop->ant=NULL;
		if(aux->next!= NULL){
			while(aux->next!=NULL){
				aux=aux->next;
			}
			aux->next = novop;
			novop->ant=aux;
		}else{
	
			aux->next = novop;
			novop->ant= aux;

		}
	return a;
}

/*************************************************************************************************************************/
//Elimina toda a lista
void matalista(LIST** a){

LIST* aux;
LIST* prox;

	aux=*a;

	while(aux!=NULL){
		prox=aux->next;
		free(aux);
		aux=prox;
	}
	*a=aux;
}
/*************************************************************************************************************************/
LIST* procura(LIST* a,uint32_t key){

	LIST* aux;

	aux=a;
	int o=0;
		while(aux!=NULL){
			if(aux->key==key){
				o=1;
				return aux;
			}
			aux=aux->next;
		}
		if(o==0){
			printf("Nao existe nenhum valor para a chave: %u\n",key);
			aux=NULL;
		}
		return aux;
}
/*************************************************************************************************************************/
void imprimeList(LIST** a){
LIST* aux;

	aux=*a;
	if(aux==NULL){
		printf("LISTA VAZIA\n");
	}

	while(aux!=NULL){
	printf("Chave: %u Valor: %s\n",aux->key,aux->value);
	
	aux=aux->next;
	}

return;
}
/*************************************************************************************************************************/
void eliminar(LIST** a,uint32_t key){

	LIST* aux=NULL;
	LIST* ant=NULL;
	char* buffer;
	LIST* prox=NULL;

	if(a==NULL){
		printf("Valor Removido");
		return;
	}
	
	aux=*a;
	while(aux!=NULL){
		if(aux->key==key){
				if(aux->ant==NULL){ //VER SE E O 1o
					prox=aux->next;
						if(prox==NULL){ //SE FOR O UNICO ELEMENTO DA LISTA
							free(aux);
							*a=prox;
							printf("Valor Removido");
							return;
						}
					prox->ant=NULL;
					free(aux);
					*a=prox;
					printf("Valor Removido");
					return;
				}
				if(aux->next==NULL){ //VER SE E O ULTIMO
					ant=aux->ant;
					ant->next=NULL;
					free(aux);
					printf("Valor Removido");
					return;
				}
				else{ //E UM PARA O MEIO
				
					ant=aux->ant;
					prox=aux->next;
					ant->next=prox;
					prox->ant=ant;
					free(aux);
					printf("Valor Removido");
					return;
				}
		}
		aux=aux->next;
	}
	printf("Nao existe valor para a chave: %u\n",key);
return;
}



void ler_socket(int* n,int* sd,LIST** lista){

	char msgrec[20];
	char* op;
	uint32_t chave;
	int comprimento=-1;
	char* dados;
	LIST *aux=NULL;
	char* data;
	int comm;

	bzero(msgrec,20);	

	if(read(*sd,msgrec,20)<0){
		puts("read fail\n");
		return;
	}
	
	comm = analysemess(msgrec);
	
	switch(comm){	

		case 1:
			if((sscanf(msgrec,"%s %u %d",op, &chave,&comprimento))!=3){
				if(write(*sd,"Erro\0",5)<0){
					puts("write fail\n");
				}
				break;
			}
			
			dados = (char *)malloc(comprimento*(sizeof(char)));
				if(dados == NULL){
					printf("Erro no malloc dos dados\n");
					exit(-2);
				}

			if(read(*sd,dados,comprimento)<0){
				puts("read fail\n");
				return;
			}
			
			novalor(lista,chave,dados);
			free(dados);
			break;
		case 2: 
			close(*socket);
			break;
		case 3: 
			if((sscanf(msgrec,"%u",&chave))!=1){
				if(write(*sd,"Erro\0",5)<0){
					puts("write fail\n");
				}
			}
			eliminar(lista,chave);

				if(write(*sd,"OK\0",3)<0){
						puts("write fail\n");
				}
			break;
		case 4: 

			if((sscanf(msgrec,"%u",&chave))!=1){
				if(write(*sd,"Erro\0",5)<0){
					puts("write fail\n");
				}
			}
			aux=procura(*lista,chave);
				if(aux==NULL){
					if(write(*sd,"Nao encontrado\0",15)<0){
						puts("write fail\n");
					}
				break;
			}else{
			data=aux->value;
			chave=aux->key;

			comprimento=strlen(data);

			sprintf(msgrec,"%u %d",chave,comprimento);

				if(write(*sd,msgrec,strlen(msgrec))<0){
					puts("write fail\n");
				}

				if(write(*sd,data,comprimento)<0){
					puts("write fail\n");
				}
			}
		break;

	}
}

/*************************************************************************************************************************/

int analysemess(char* comando){

char* op;
char* cont;


sscanf(comando,"%s %s",op,cont);

	if(strstr(op, "write")){
        	return 1;
    	
	}else if(strstr(op, "read")){
        	return 4;
	
	}else if(strstr(op, "delete")){			
        	return 3;

	}else if(strstr(op, "exit")){			
        	return 2;

	}else{
		return -1;

	}
}



