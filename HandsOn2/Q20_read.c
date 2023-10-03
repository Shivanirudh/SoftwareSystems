/*
 ============================================================================
Name : Q20_read.c
Author : Shivanirudh S G
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
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
	
	int fif = open("fifo_file", O_CREAT|O_RDONLY);
	
	char buf[1024];
	read(fif, buf, sizeof(buf));
	
	printf("Message from FIFO: %s\n", buf);
	
	return 0;
}
