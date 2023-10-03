/*
 ============================================================================
Name : Q23.c
Author : Shivanirudh S G
Description : Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).
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
#include<sys/resource.h>

int main(){
	struct rlimit rlim;
	struct stat file_details;
	
	getrlimit(RLIMIT_NOFILE, &rlim);
	printf("Max number of files: %ld\n", rlim.rlim_max);
	
	lstat("named_pipe", &file_details);
	printf("Size: %ld\n", file_details.st_size);
	
	return 0;
}
