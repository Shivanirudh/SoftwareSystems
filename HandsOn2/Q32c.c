/*
Write a program to implement semaphore to protect any critical section.
c. protect multiple pseudo resources ( may be two) using counting semaphore
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/ipc.h>
#include<sys/sem.h>

union semun{
	int val;
	struct semid_ds buf;
	unsigned short int *array;
};

int main(int argc, char* argv[]){
	int semID = atoi(argv[1]);

	if(semID<0){
		printf("Creation of semaphore failed. \n");
	}
	else{		
		printf("Before critical section\n");
		struct sembuf buf = {0, -2, SEM_UNDO};
		
		semop(semID, &buf, 1);
		
		printf("Inside critical section\n");
		
		printf("Press ENTER to exit critical section\n");
		getchar();
		
		buf.sem_op=1;
		semop(semID, &buf, 1);
		printf("Done with critical section. Exiting. \n");
		
		
	}
	
	return 0;
}
