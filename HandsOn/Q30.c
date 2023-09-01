/*Write a program to run a script at a specific time using a Daemon process.*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(){
	int pid = fork();
	if(pid<0){
		printf("\nFork issue");
		exit(1);
	}
	else if(pid>0){
		printf("\nChild process ID: %d", pid);
		exit(0);
	}
	if(setsid() < 0){
		printf("\nSession issue");
		exit(1);
	}
	chdir("/");
	umask(0);
	
	int fd = open("logfile.txt", O_CREAT|O_RDWR, S_IRWXU);
	write(fd, "HI\n", sizeof("HI\n"));
	if(fd<0){
		printf("\nError opening logfile");
		exit(1);
	}
	while(1){
		sleep(10);
		write(fd, "HI\n", sizeof("HI\n"));
	}
	close(fd);
	
	return 0;
}

