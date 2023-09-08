/*
 ============================================================================
Name : Q17_book_ticket.c
Author : Shivanirudh S G
Description : Write a program to simulate online ticket reservation. Implement write lock
Write a program to open a file, store a ticket number and exit. Write a separate program, to
open the file, implement write lock, read the ticket number, increment the number and print
the new ticket number then close the file.
Date: 23rd Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(){
	
	int fd;
	fd = open("tickets.txt", O_RDWR);
	if(fd<0){
		printf("\nError opening ticket file");
		return -1;
	}
	
	struct flock mylock;
	mylock.l_type = F_WRLCK;
	mylock.l_whence = SEEK_SET;
	mylock.l_start = 0;
	mylock.l_len = 0;
	mylock.l_pid = getpid();
	
	printf("Before critical section\n");
	fcntl(fd, F_SETLKW, &mylock);
	
	printf("Inside critical section\n");
	int num;
	read(fd, &num, sizeof(num));
	
	num++;
	printf("Current ticket number: %d\n", num);
	
	lseek(fd, 0L, SEEK_SET);
	write(fd, &num, sizeof(num));
	
	printf("Press ENTER to exit critical section\n");
	getchar();
	
	mylock.l_type=F_UNLCK;
	fcntl(fd, F_SETLK, &mylock);
	printf("Done with critical section. Exiting. \n");
	close(fd);
	
	return 0;
}
