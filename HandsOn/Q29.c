/*
 ============================================================================
Name : Q29.c
Author : Shivanirudh S G
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,
SCHED_RR).
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
	struct sched_param p;
	int status;
	printf("Current scheduling policy:\n");
	switch(sched_getscheduler(getpid())){
		case SCHED_FIFO: 
			printf("SCHED_FIFO\n");
			p.sched_priority = sched_get_priority_max(SCHED_RR);
			status = sched_setscheduler(getpid(), SCHED_RR, &p);
			break;
		case SCHED_RR:
			printf("SCHED_RR\n");
			p.sched_priority = sched_get_priority_max(SCHED_FIFO);
			status = sched_setscheduler(getpid(), SCHED_FIFO, &p);
			break;
		default:
			printf("SCHED_OTHER\n");
			p.sched_priority = sched_get_priority_max(SCHED_FIFO);
			status = sched_setscheduler(getpid(), SCHED_FIFO, &p);
			break;
	}
	if(status != 0){
		printf("Issue changing policy");
	}
	else{
		printf("Modified scheduling policy:\n");
		switch(sched_getscheduler(getpid())){
			case SCHED_FIFO: 
				printf("SCHED_FIFO\n");
				break;
			case SCHED_RR:
				printf("SCHED_RR\n");
				break;
			default:
				printf("SCHED_OTHER\n");
				break;
		}
	}
	return 0;
}

