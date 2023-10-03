/* 
 ============================================================================
Name : Q32d.c
Author : Shivanirudh S G
Description : Write a program to implement semaphore to protect any critical section.
			d. remove the created semaphore
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
		sleep(10);
		
		semctl(semID, 0, IPC_RMID, NULL);
		printf("Semaphore deleted\n");
	}
	
	return 0;
}
