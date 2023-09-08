/*
 ============================================================================
Name : Q22.c
Author : Shivanirudh S G
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the parent processes. Check output of the file.
Date: 25th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(){
	
	int fd = open("oyako.txt", O_RDWR);
	if(fd < 0){
		printf("Error opening file\n");
		return 0;
	}
	int cpid = fork();
	if(cpid == 0){
		write(fd, "child content", sizeof("child content"));
		return 0;
	}
	write(fd, "PARENT CONTENT", sizeof("PARENT CONTENT"));
	
	return 0;
}

