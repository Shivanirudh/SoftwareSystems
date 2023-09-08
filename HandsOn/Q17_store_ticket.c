/*
 ============================================================================
Name : Q17_store_ticket.c
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
	fd = open("tickets.txt", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	if(fd<0){
		printf("\nError opening ticket file");
		return -1;
	}
	
	int num;
	printf("Enter ticket number: ");scanf("%d", &num);
	write(fd, &num, sizeof(num));
	
	close(fd);
	
	return 0;
}
