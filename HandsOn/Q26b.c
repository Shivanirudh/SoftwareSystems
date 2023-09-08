/*
 ============================================================================
Name : Q26b.c
Author : Shivanirudh S G
Description : Write a program to execute an executable program.
b. pass some input to an executable program. (for example execute an executable of $./a.out name)
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
	char* args[] = {"14", "/home/shivanirudh/Documents/iiitb/SoftwareSystems/HandsOn/14", argv[1], (char*) NULL};
	execv("/home/shivanirudh/Documents/iiitb/SoftwareSystems/HandsOn/14", args);	
	return 0;
}

