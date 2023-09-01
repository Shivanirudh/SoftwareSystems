/*Write a program to send and receive data from parent to child vice versa. Use two way
communication.*/

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
		close(fd1[1]);
		read(fd1[0], buf, sizeof(buf));
		printf("\nMessage from parent: %s\n", buf);
		close(fd2[0]);
		printf("Enter msg for parent: "); scanf(" %[^\n]", buf);
		write(fd2[1], buf, sizeof(buf));
	}
	else{
		close(fd1[0]);
		printf("Enter msg for child: "); scanf(" %[^\n]", buf);
		write(fd1[1], buf, sizeof(buf));
		close(fd2[1]);
		read(fd2[0], buf, sizeof(buf));
		printf("\nMessage from child: %s\n", buf);
	}
	
	return 0;
}

