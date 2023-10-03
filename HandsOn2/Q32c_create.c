/*
 ============================================================================
Name : Q32c_create.c
Author : Shivanirudh S G
Description : Write a program to implement semaphore to protect any critical section.
			c. protect multiple pseudo resources ( may be two) using counting semaphore
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

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo  *__buf;
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
		arg.val = 3;
		if(semctl(semID, 0, SETVAL, arg)<0)
			printf("Error setting value\n");	
	}
	
	return 0;
}
