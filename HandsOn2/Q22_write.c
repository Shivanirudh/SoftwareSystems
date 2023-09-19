/*Write a program to wait for data to be written into FIFO within 10 seconds, use select
system call with FIFO.*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(){

	int fif_id = open("fifo_file", O_CREAT|O_RDWR|O_NONBLOCK);
	
	char buf[1024];
	printf("Enter msg for FIFO: ");
	scanf(" %[^\n]", buf);
	write(fif_id, buf, sizeof(buf));
	
	return 0;
}
