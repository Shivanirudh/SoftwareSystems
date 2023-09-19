/*Create a FIFO file by
a. mknod command
b. mkfifo command
c. use strace command to find out, which command (mknod or mkfifo) is better.
c. mknod system call
d. mkfifo library function*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(){
	int nod = mknod("call_node_fifo", 0644, 0);
	if(nod<0){
		printf("\nError creating node file");
	}
	else{
		printf("\nmknod works");
	}
	
	int fif = mkfifo("call_fifo_fifo", 0644);
	if(fif<0){
		printf("\nError creating fifo file");
	}
	else{
		printf("\nmkfifo works");
	}
	
	return 0;
}
