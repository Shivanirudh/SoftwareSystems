/*
 ============================================================================
Name : Q30_check.c
Author : Shivanirudh S G
Description : Write a program to run a script at a specific time using a Daemon process.
Date: 1st Sep 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	int in_fd = 0, out_fd = 1;
	
	char ch;
	int ix = 0;
	in_fd = open("/logfile.txt", O_RDONLY);
	if(in_fd<0){
		printf("\n Error opening infile");
	}

	while(read(in_fd, &ch, sizeof(ch)) > 0){
		write(out_fd, &ch, sizeof(ch));
	}
	
	return 0;
}
