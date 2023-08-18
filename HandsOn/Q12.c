/*Write a program to find out the opening mode of a file. Use fcntl.*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	int fd = 0;
	
	char buf[1024];
	
	printf("Enter file name: ");
	scanf(" %[^\n]", buf);
	
	fd = open(buf, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(fd<0){
		printf("\n Error opening infile");
	}
	
	int op = fcntl(fd, F_GETFL, 0);
	printf("Opening mode: %d\n", op);
	return 0;
}
