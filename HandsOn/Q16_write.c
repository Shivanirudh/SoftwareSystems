/*Write a program to perform mandatory locking.
a. Implement write lock
b. Implement read lock*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(){
	
	int fd;
	fd = open("infile", O_RDWR);
	if(fd<0){
		printf("\nError opening infile");
	}
	
	struct flock mylock;
	mylock.l_type = F_WRLCK;
	mylock.l_whence = SEEK_SET;
	mylock.l_start = 0;
	mylock.l_len = 0;
	mylock.l_pid = getpid();
	
	printf("Writer\n");
	printf("Before critical section\n");
	fcntl(fd, F_SETLKW, &mylock);
	
	printf("Inside critical section\n");
	printf("Press ENTER to exit critical section\n");
	getchar();
	
	mylock.l_type=F_UNLCK;
	fcntl(fd, F_SETLK, &mylock);
	printf("Done with critical section. Exiting. \n");
	
	return 0;
}
