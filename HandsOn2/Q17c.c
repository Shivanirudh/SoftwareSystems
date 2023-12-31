/*
 ============================================================================
Name : Q17c.c
Author : Shivanirudh S G
Description : Write a program to execute ls -l | wc.
			c. use fcntl
Date: 5th Sep 2023.
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
		close(fd[0]);
		close(1);
		fcntl(fd[1], F_DUPFD, 0);
		execl("/usr/bin/ls", "ls", "-l", (char*) NULL);
	}
	else{
		close(fd[1]);
		close(0);
		fcntl(fd[0], F_DUPFD, 0);
		execl("/usr/bin/wc", "wc", (char*) NULL);
	}
	
	return 0;
}

