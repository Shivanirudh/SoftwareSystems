/*
 ============================================================================
Name : Q18_write.c
Author : Shivanirudh S G
Description : Write a program to perform Record locking.
			a. Implement write lock
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
	printf("\nEnter record to write (1, 2, 3): ");scanf("%d", &inp);
	
	struct Record rec;
	
	struct flock mylock;
	mylock.l_type = F_WRLCK;
	mylock.l_whence = SEEK_SET;
	mylock.l_start = (inp-1)*sizeof(rec);
	mylock.l_len = sizeof(rec);
	mylock.l_pid = getpid();
	
	lseek(fd, (inp-1)*sizeof(rec), SEEK_SET);
	
	printf("Before critical section\n");
	fcntl(fd, F_SETLKW, &mylock);
	
	printf("Inside critical section\n");
	read(fd, &rec, sizeof(rec));
	printf("Old record: %d %d\n", rec.num1, rec.num2);
	
	printf("Press ENTER to update values\n");
	getchar();
	int num;
	printf("Enter new number 1: ");scanf("%d", &num);rec.num1 = num;
	printf("Enter new number 2: ");scanf("%d", &num);rec.num2 = num;
	lseek(fd, (inp-1)*sizeof(rec), SEEK_SET);
	write(fd, &rec, sizeof(rec));
	
	mylock.l_type=F_UNLCK;
	fcntl(fd, F_SETLK, &mylock);
	printf("Done with critical section. Exiting. \n");
	close(fd);
	
	return 0;
}
