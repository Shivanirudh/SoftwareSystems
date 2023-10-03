/*
 ============================================================================
Name : Q18.c
Author : Shivanirudh S G
Description : Write a program to find out total number of directories on the pwd.
execute ls -l | grep ^d | wc ? Use only dup2.
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
	int fd1[2], fd2[2];
	char buf[1024];
	pipe(fd1);pipe(fd2);
	int cid = fork();
	if(!cid){
		close(fd2[0]);close(fd2[1]);
		close(fd1[0]);
		close(1);
		dup2(fd1[1], 1);
		execl("/usr/bin/ls", "ls", "-l", "../",(char*) NULL);
	}
	else{
		int cid2 = fork();
		if(!cid2){
			close(fd1[1]);close(fd2[0]);
			close(0);
			dup2(fd1[0], 0);
			close(1);
			dup2(fd2[1], 1);
			execl("/usr/bin/grep", "grep", "^d", (char*) NULL);
		}
		else{	
			close(fd1[0]);close(fd1[1]);
			close(fd2[1]);
			close(0);
			dup2(fd2[0], 0);
			execl("/usr/bin/wc", "wc", (char*) NULL);
		}
	}
	return 0;
}
