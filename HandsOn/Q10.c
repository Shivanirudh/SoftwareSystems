/*Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
a. check the return value of lseek
b. open the file with od and check the empty spaces in between the data.*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	int fd = 0;
	
	char buf[1024], content[10];
	
	printf("Enter file name: ");
	scanf(" %[^\n]", buf);
	
	fd = open(buf, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(fd<0){
		printf("\n Error opening infile");
	}
	
	printf("Enter 10 byte content: ");
	scanf(" %[^\n]", content);
	
	write(fd, content, 10);
	int lseek_op = lseek(fd, 10L, SEEK_CUR);
	printf("Lseek Output: %d\n", lseek_op);
	
	printf("Enter 10 byte content: ");
	scanf(" %[^\n]", content);
	
	write(fd, content, 10);
	
	int fdclose = close(fd);
	if(fdclose<0){
		printf("\n Error closing infile");
	}
	return 0;
}
