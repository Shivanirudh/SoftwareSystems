/*
 ============================================================================
Name : Q6.c
Author : Shivanirudh S G
Description : Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date: 11th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	int stdin = 0, stdout = 1;
	
	char buffer[20];
	int l = read(stdin, buffer, 20);
	buffer[l] = '\0';
	write(stdout, buffer, 20);
	
	return 0;
}


