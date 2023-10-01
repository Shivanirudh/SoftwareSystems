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
	int key = ftok(".", 'a');
	int semID = semget(key, 1, 0);

	if(semID<0){
		printf("Creation of semaphore failed. \n");
	}
	else{
		printf("Sem ID: %d\n", semID);
		printf("Before critical section\n");
		
		struct sembuf buf = {0, -1, SEM_UNDO};
		
		if(semop(semID, &buf, 1) == -1)
			printf("Error locking\n");
		
		printf("Inside critical section\n");
		
		printf("Press ENTER to exit critical section\n");
		getchar();
		
		buf.sem_op = 1;
		if(semop(semID, &buf, 1) == -1)
			printf("Error unlocking\n");
		printf("Done with critical section. Exiting. \n");
		
		
	}
	
	return 0;
}
