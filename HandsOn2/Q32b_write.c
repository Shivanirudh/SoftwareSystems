/*
 ============================================================================
Name : Q32b_write.c
Author : Shivanirudh S G
Description : Write a program to implement semaphore to protect any critical section.
			b. protect shared memory from concurrent write access
Date: 19th Sep 2023.
============================================================================
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
#include<sys/shm.h>

union semun{
	int val;
	struct semid_ds buf;
	unsigned short int *array;
};

int main(int argc, char* argv[]){
	
	int semID = atoi(argv[1]);
	int shmID = atoi(argv[2]);
	void* addr;

	if(shmID<0){
		printf("Creation of shared memory failed. \n");
	}
	else{
		printf("Creation success. Shared Memory ID: %d\n", shmID);
		addr = shmat(shmID, (void*)0, 0);
		
		printf("Before critical section\n");
		struct sembuf buf = {0, -1, SEM_UNDO};
		
		semop(semID, &buf, 1);
		
		printf("Inside critical section\n");
		
		printf("SHM contents: %s\n", (char*)addr);
		printf("Enter content into shared memory: ");scanf(" %[^\n]", (char*)addr);
		printf("New SHM contents: %s\n", (char*)addr);
		
		printf("Press ENTER to exit critical section\n");
		getchar();
		
		buf.sem_op=1;
		semop(semID, &buf, 1);
		printf("Done with critical section. Exiting. \n");
	}
	
	return 0;
}
