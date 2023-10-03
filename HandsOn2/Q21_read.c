/*
 ============================================================================
Name : Q21_read.c
Author : Shivanirudh S G
Description : Write two programs so that both can communicate by FIFO -Use two way communications.
Date: 19th Sep 2023.
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
	
	int fif = open("fifo_file_1", O_CREAT|O_RDWR);
	
	char buf[1024];
	read(fif, buf, sizeof(buf));
	
	printf("Message from FIFO: %s\n", buf);
	close(fif);
	
	fif = open("fifo_file_2", O_CREAT|O_RDWR);
	printf("Enter new msg for FIFO: ");
	scanf(" %[^\n]", buf);
	write(fif, buf, sizeof(buf));
	
	
	return 0;
}
