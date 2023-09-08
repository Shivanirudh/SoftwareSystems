/*
 ============================================================================
Name : Q13.c
Author : Shivanirudh S G
Description : Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date: 18th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	fd_set rfds;
	struct timeval tim;
	
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	
	tim.tv_sec = 10;
	tim.tv_usec = 0;
	
	int ret = select(1, &rfds, NULL, NULL, &tim);
	
	if(ret == -1)
		printf("Select failed. \n");
	else if(ret)
		printf("STDIN data available now\n");
	else
		printf("No data within 10 seconds.\n");
	return 0;
}
