/*
 ============================================================================
Name : Q32b_create.c
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

int main(){	
	int key = ftok(".", 'a');
	int semID = semget(key, 1, IPC_CREAT|0744);

	if(semID<0){
		printf("Creation of semaphore failed. \n");
	}
	else{
		printf("Creation success. Semaphore ID: %d\n", semID);
		union semun arg;
		arg.val = 1;
		semctl(semID, 0, SETVAL, arg);
	}
	
	int shmID = shmget(key, 1024,IPC_CREAT|0744);
	if(shmID<0){
		printf("Creation of shared memory failed. \n");
	}
	else{
		printf("Creation success. Shared Memory ID: %d\n", shmID);
	}
	
	return 0;
}
