/*
 ============================================================================
Name : Q4.c
Author : Shivanirudh S G
Description : Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 11th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	int fd = open("created_file", O_RDWR);
	
	if (fd>=0){
		printf("File opened with descriptor: %d\n", fd);
	}
	else{
		printf("File opening failed\n");
	}
	
	
	int new_fd = open("new_file", O_RDWR, O_CREAT|O_EXCL);
	if(new_fd>=0){
		printf("New file descriptor: %d\n", new_fd);
	}
	else{
		printf("File opening failed with O_EXCL\n");
	}
	
	return 0;
}


