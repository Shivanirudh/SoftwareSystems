/*
 ============================================================================
Name : Q15_v2.c
Author : Shivanirudh S G
Description : Write a program to display the environmental variable of the user (use environ).
Date: 18th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>

extern char** environ;

int main(int argc, char* argv[], char* envp[]){
	
	char **env = environ;
	while(*env){
		char s[4];
		strncpy(s, *env, 4);
		if(strcmp("USER", s) == 0){
			printf("%s\n", *env);
		}
		env++;
	}
	return 0;
}
