/*
 ============================================================================
Name : Q25.c
Author : Shivanirudh S G
Description : Write a program to create three child processes. The parent should wait for a particular child (use waitpid system call).
Date: 1st Sep 2023.
============================================================================	
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>
#include<sys/wait.h>

int main(){
	
	int cid1, cid2, cid3;
	int status;
	printf("Main parent process ID: %d\n", getpid());
	cid1 = fork();
	if(!cid1){
		printf("Child process ID: %d and parent process ID: %d\n", getpid(), getppid());
		sleep(2);
	}
	else{
		cid2 = fork();
		if(!cid2){
			printf("Child process ID: %d and parent process ID: %d\n", getpid(), getppid());
			sleep(5);
		}
		else{
			cid3 = getpid();
			printf("Child process ID: %d and parent process ID: %d\n", getpid(), getppid());
			sleep(2);
		}
	}
	
	printf("Waiting for child....\n");
	waitpid(cid2, &status, 0);
	
	if(WIFSIGNALED(status)){
		printf("\nSignal interrupt");
	}
	else if(WEXITSTATUS(status)){
		printf("\nNormal exit");
	}
	printf("\nExiting");
	
	return 0;
}

