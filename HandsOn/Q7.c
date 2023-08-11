/*Write a program to copy file1 into file2 ($cp file1 file2).*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>

int main(){
	int in_fd = 0, out_fd = 1;
	
	char ch;
	in_fd = open("infile", O_RDONLY);
	if(in_fd<0){
		printf("\n Error opening infile");
	}
	out_fd = open("outfile", O_WRONLY);
	if(out_fd<0){
		printf("\n Error opening outfile");
	}
	while(read(in_fd, &ch, 1)>0){
		write(out_fd, &ch, 1);
	}
	
	return 0;
}
