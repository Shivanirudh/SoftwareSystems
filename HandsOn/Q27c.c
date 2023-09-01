/*Write a program to execute ls -Rl by the following system calls
a. execl
b. execlp
c. execle
d. execv
e. execvp*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(int argc, char* argv[]){
	char* const env[]={"DIR=/home/shivanirudh/Documents/iiitb/SoftwareSystems", NULL};
	execle("/usr/bin/ls", "ls", "-R", "-l", "/home/shivanirudh/Documents/iiitb/SoftwareSystems", (char*) NULL, env);	
	return 0;
}

