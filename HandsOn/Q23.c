/*
 ============================================================================
Name : Q23.c
Author : Shivanirudh S G
Description : Write a program to create a Zombie state of the running program.
Date: 25th Aug 2023.
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
	int cpid = fork();
	if(cpid == 0){
		printf("Child process ID: %d and parent process ID: %d\n", getpid(), getppid());
		exit(0);
	}
	printf("Current process ID: %d and child process ID: %d\n", getpid(), cpid);
	sleep(20);
	
	return 0;
}

