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

int main(int argc, char* argv[]){
	
	int key = ftok(".", 'a');
	int semID = atoi(argv[1]);

	if(semID<0){
		printf("Creation of semaphore failed. \n");
	}
	else{
		printf("Semaphore ID: %d\n", semID);
	
		int fd;
		fd = open("tickets.txt", O_RDWR);
		if(fd<0){
			printf("\nError opening ticket file");
			return -1;
		}
		printf("Before critical section\n");
		struct sembuf buf = {0, -1, SEM_UNDO};
		
		semop(semID, &buf, 1);
		
		printf("Inside critical section\n");
		int num;
		read(fd, &num, sizeof(num));
		
		num++;
		printf("Current ticket number: %d\n", num);
		
		lseek(fd, 0L, SEEK_SET);
		write(fd, &num, sizeof(num));
		
		printf("Press ENTER to exit critical section\n");
		getchar();
		
		buf.sem_op=1;
		semop(semID, &buf, 1);
		printf("Done with critical section. Exiting. \n");
		close(fd);
	}
	
	return 0;
}
