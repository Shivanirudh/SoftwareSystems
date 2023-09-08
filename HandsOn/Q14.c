/*
 ============================================================================
Name : Q14.c
Author : Shivanirudh S G
Description : Write a program to find the type of a file.
			a. Input should be taken from command line.
			b. program should be able to identify any type of a file.
Date: 18th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(int argc, char* argv[]){
	
	struct stat file_details;
	
	if(stat(argv[1], &file_details) == -1){
		printf("Error opening file\n");
	}
	else{
		printf("File type: %d\n", file_details.st_mode);
		if(S_ISREG(file_details.st_mode)){
			printf("Regular file\n");
		}
		else if(S_ISDIR(file_details.st_mode)){
			printf("Directory\n");
		}
		else if(S_ISCHR(file_details.st_mode)){
			printf("Character device\n");
		}
		else if(S_ISBLK(file_details.st_mode)){
			printf("Block device\n");
		}
		else if(S_ISFIFO(file_details.st_mode)){
			printf("Named pipe\n");
		}
		else if(S_ISLNK(file_details.st_mode)){
			printf("Symbolic link\n");
		}
		else if(S_ISSOCK(file_details.st_mode)){
			printf("Socket\n");
		}
		else{
			printf("Unknown\n");
		}
	}
	return 0;
}
