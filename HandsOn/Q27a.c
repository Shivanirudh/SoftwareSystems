/*
 ============================================================================
Name : Q27a.c
Author : Shivanirudh S G
Description : Write a program to execute ls -Rl by the following system calls
a. execl
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
	execl("/usr/bin/ls", "ls", "-R", "-l", (char*) NULL);		
	return 0;
}

