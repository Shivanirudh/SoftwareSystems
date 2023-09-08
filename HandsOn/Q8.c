/*
 ============================================================================
Name : Q8.c
Author : Shivanirudh S G
Description : Write a program to open a file in read only mode, read line by line and display each line as it is read. Close the file when end of file is reached.
Date: 18th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	int in_fd = 0, out_fd = 1;
	
	char ch, buf[1024];
	int ix = 0;
	in_fd = open("file", O_RDONLY);
	if(in_fd<0){
		printf("\n Error opening infile");
	}

	while(read(in_fd, &ch, 1)>0){
		if(ch == '\n'){
			buf[ix++] = '\n';
			buf[ix++] = '\0';
			write(out_fd, buf, ix);
			buf[0] = '\0';ix = 0;
		}
		else if(ch == EOF){
			buf[ix++] = '\n';
			buf[ix++] = '\0';
			write(out_fd, buf, ix);
			buf[0] = '\0';ix = 0;
			break;
		}
		else{
			buf[ix++] = ch;
		}
	}
	
	return 0;
}
