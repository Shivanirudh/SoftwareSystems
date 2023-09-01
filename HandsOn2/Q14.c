/*Write a simple program to create a pipe, write to the pipe, read from pipe and display on
the monitor.*/

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
	pipe(fd);
	char buf[1024];
	printf("Enter msg for pipe: "); scanf(" %[^\n]", buf);
	write(fd[1], buf, sizeof(buf));
	
	
	read(fd[0], buf, sizeof(buf));
	printf("\nMessage from pipe: %s\n", buf);
	
	return 0;
}

