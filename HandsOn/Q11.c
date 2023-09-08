/*
 ============================================================================
Name : Q11.c
Author : Shivanirudh S G
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the descriptors and check whether the file is updated properly or not.
			a. use dup
			b. use dup2
			c. use fcntl
Date: 18th Aug 2023.
============================================================================	
*/



#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	int fd = 0;
	
	char buf[1024], content[10];
	
	printf("Enter file name: ");
	scanf(" %[^\n]", buf);
	
	fd = open(buf, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(fd<0){
		printf("\n Error opening infile");
	}
	
	printf("Enter initial content: ");
	scanf(" %[^\n]", content);
	
	write(fd, content, 10);
	
	int dup_fd = dup(fd);
	
	printf("Enter content for fd: ");
	scanf(" %[^\n]", content);
	write(fd, content, 10);
	
	printf("Enter content for dup fd: ");
	scanf(" %[^\n]", content);
	write(dup_fd, content, 10);
	
	int dup2_fd = dup2(fd, 10);
	printf("Enter content for dup2 fd: ");
	scanf(" %[^\n]", content);
	write(dup2_fd, content, 10);
	
	int fdclose = close(fd);
	if(fdclose<0){
		printf("\n Error closing infile");
	}
	fdclose = close(dup_fd);
	if(fdclose<0){
		printf("\n Error closing infile");
	}
	fdclose = close(dup2_fd);
	if(fdclose<0){
		printf("\n Error closing infile");
	}
	return 0;
}

