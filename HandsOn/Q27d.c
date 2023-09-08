/*
 ============================================================================
Name : Q27d.c
Author : Shivanirudh S G
Description : Write a program to execute ls -Rl by the following system calls
d. execv
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

int main(int argc, char* argv[]){
	char* args[] = {"ls", "-R", "-l", (char*) NULL};
	execv("/usr/bin/ls", args);	
	return 0;
}

