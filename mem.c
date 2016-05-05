#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHMSZ     27

void teste(key_t key,char* porta){
    char c;
    int shmid;
    char *shm, *s;
   
    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now read what the server put in the memory.
     */
    /*for (s = shm; *s != '\0'; s++)
        putchar(*s);
    putchar('\n');*/

        char v[10];
        int i=0;
        
        for (s = shm; *s != '\0'; s++){
                porta[i]=*s;
                putchar(*s);
                i++; 
            }
            
            printf("o final:%s",v);

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*';    
}



int main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    pid_t child;
    char porta[10];
    /*
     * We'll name our shared memory segment
     * "5678".
     */
     child=fork();
     if(child==0){

         key = 69;

         teste(key,porta);

         printf("\napos tudo:%s\n",porta);
    

    exit(0);
     }else{
    key = 69;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    s = shm;
    int i;
    char v[10];
    strcpy(v,"12345678910");
    for (i=0; i <= 10; i++)
        *s++ = v[i];
    
    *s = '\0';

    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    while (*shm != '*')
        puts("a espera\n");
        sleep(1);

    exit(0);
    }
}