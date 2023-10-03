/*
 ============================================================================
Name : Q31.c
Author : Shivanirudh S G
Description : Write a program to create a semaphore and initialize value to the semaphore.
			a. create a binary semaphore
			b. create a counting semaphore
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
	struct seminfo  *__buf;
};

int main(){
	int key = ftok(".", 'a');
	int semID = semget(key, 2, IPC_CREAT|0744);

	if(semID<0){
		printf("Creation of semaphore failed. \n");
	}
	else{
		union semun arg;
		arg.val = 1;
		semctl(semID, 0, SETVAL, arg);
		printf("Initial value of binary semaphone: %d\n", arg.val);
		arg.val = 2;
		semctl(semID, 1, SETVAL, arg);
		printf("Initial value of counting semaphone: %d\n", arg.val);
	}
	
	return 0;
}
