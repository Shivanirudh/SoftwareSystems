/*
 ============================================================================
Name : Q18_read.c
Author : Shivanirudh S G
Description : Write a program to perform Record locking.
			b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access to avoid race condition.
Date: 23rd Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>

struct Record{
	int num1;
	int num2;
} record;

int main(){
	
	int fd;
	fd = open("records.txt", O_RDWR);
	if(fd<0){
		printf("\nError opening records file");
		return -1;
	}
	
	int inp;
	printf("\nEnter record to read (1, 2, 3): ");scanf("%d", &inp);
	
	struct Record rec;
	
	struct flock mylock;
	mylock.l_type = F_RDLCK;
	mylock.l_whence = SEEK_SET;
	mylock.l_start = (inp-1)*sizeof(rec);
	mylock.l_len = sizeof(rec);
	mylock.l_pid = getpid();
	
	lseek(fd, (inp-1)*sizeof(rec), SEEK_SET);
	
	printf("Before critical section\n");
	fcntl(fd, F_SETLKW, &mylock);
	
	printf("Inside critical section\n");
	read(fd, &rec, sizeof(rec));
	printf("Current record: %d %d\n", rec.num1, rec.num2);
	
	printf("Press ENTER to exit critical section\n");
	getchar();getchar();
	
	mylock.l_type=F_UNLCK;
	fcntl(fd, F_SETLK, &mylock);
	printf("Done with critical section. Exiting. \n");
	close(fd);
	
	return 0;
}
