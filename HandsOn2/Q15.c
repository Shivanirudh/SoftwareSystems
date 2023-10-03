/*
 ============================================================================
Name : Q15.c
Author : Shivanirudh S G
Description : Write a simple program to send some data from parent to the child process.
Date: 1st Sep 2023.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(){
	int fd[2];
	char buf[1024];
	pipe(fd);
	int cid = fork();
	if(!cid){
		close(fd[1]);
		read(fd[0], buf, sizeof(buf));
		printf("\nMessage from parent: %s\n", buf);
	}
	else{
		close(fd[0]);
		printf("Enter msg for child: "); scanf(" %[^\n]", buf);
		write(fd[1], buf, sizeof(buf));
	}
	
	return 0;
}

