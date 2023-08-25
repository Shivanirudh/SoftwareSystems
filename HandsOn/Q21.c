/*Write a program, call fork and print the parent and child process id.*/

#include<stdio.h>
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
		return 0;
	}
	printf("Current process ID: %d and child process ID: %d\n", getpid(), cpid);
	
	return 0;
}

