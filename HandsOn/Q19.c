/*
 ============================================================================
Name : Q19.c
Author : Shivanirudh S G
Description : Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 25th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(){
	struct timeval start, end;
	
	double elapsed = 0.0;
	
	gettimeofday(&start, NULL);
	int pid = getpid();
	gettimeofday(&end, NULL);
	
	printf("\nProcess ID: %d", pid);
	elapsed += (end.tv_sec - start.tv_sec)*1000000;
	elapsed += (end.tv_usec -start.tv_usec);
	printf("\nTime taken for getpid system call: %lf us. \n", elapsed);
	
	return 0;
}

