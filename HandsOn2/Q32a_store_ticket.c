/*
Write a program to implement semaphore to protect any critical section.
a. rewrite the ticket number creation program using semaphore
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
	
	int fd;
	fd = open("tickets.txt", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	if(fd<0){
		printf("\nError opening ticket file");
		return -1;
	}
	
	int num;
	printf("Enter ticket number: ");scanf("%d", &num);
	write(fd, &num, sizeof(num));
	
	close(fd);
	
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
	
	return 0;
}
