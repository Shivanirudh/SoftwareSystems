/*
 ============================================================================
Name : Q20.c
Author : Shivanirudh S G
Description : Find out the priority of your running program. Modify the priority with nice command.
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
#include<sys/resource.h>

int main(){
	int pid = getpid();
	int niceVal = getpriority(PRIO_PROCESS, 0);
	
	printf("Priority of process with PID %d is %d\n", pid, niceVal);
	
	nice(10);
	niceVal = getpriority(PRIO_PROCESS, 0);
	
	printf("After modification, priority of process with PID %d is %d\n", pid, niceVal);
	return 0;
}

