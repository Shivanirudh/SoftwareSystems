/*
 ============================================================================
Name : Q28.c
Author : Shivanirudh S G
Description : Write a program to get maximum and minimum real time priority.
Date: 1st Sep 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>
#include<sched.h>

int main(int argc, char* argv[]){
	printf("\nSCHED_FIFO: ");
	printf("Max real time priority: %d\n", sched_get_priority_max(SCHED_FIFO));
	printf("Min real time priority: %d\n", sched_get_priority_min(SCHED_FIFO));
	
	printf("\n");
	
	printf("\nSCHED_RR: ");
	printf("Max real time priority: %d\n", sched_get_priority_max(SCHED_RR));
	printf("Min real time priority: %d\n", sched_get_priority_min(SCHED_RR));
	return 0;
}

