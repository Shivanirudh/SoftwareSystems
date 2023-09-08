/*
 ============================================================================
Name : Q27e.c
Author : Shivanirudh S G
Description : Write a program to execute ls -Rl by the following system calls
e. execvp
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
	execvp("ls", args);	
	return 0;
}

