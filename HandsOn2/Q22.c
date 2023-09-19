/*Write a program to wait for data to be written into FIFO within 10 seconds, use select
system call with FIFO.*/
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	fd_set rfds;
	struct timeval tim;
	int fif = open("fifo_file", O_CREAT|O_RDWR|O_NONBLOCK);
	
	FD_ZERO(&rfds);
	FD_SET(fif, &rfds);
	
	tim.tv_sec = 10;
	tim.tv_usec = 0;
	
	char buf[1024];
	//read(fif, buf, sizeof(buf));
	
	int ret = select(fif+1, &rfds, NULL, NULL, &tim);
	
	if(ret == -1)
		printf("Select failed. \n");
	else if(ret)
		printf("FIFO data available now\n");
	else
		printf("No data within 10 seconds.\n");
	return 0;
}
