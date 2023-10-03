/*
 ============================================================================
Name : Q21_write.c
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
	int fif_id = open("fifo_file_1", O_CREAT|O_RDWR);
	
	char buf[1024];
	printf("Enter msg for FIFO: ");
	scanf(" %[^\n]", buf);
	write(fif_id, buf, sizeof(buf));
	close(fif_id);
	
	fif_id = open("fifo_file_2", O_CREAT|O_RDWR);
	read(fif_id, buf, sizeof(buf));
	printf("Message from FIFO: %s\n", buf);
	
	return 0;
}
