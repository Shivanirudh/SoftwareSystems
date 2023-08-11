/*
Create the following types of a files using (i) shell command (ii) system call
a. soft link (symlink system call)
b. hard link (link system call)
c. FIFO (mkfifo Library Function or mknod system call)
*/

char* oldpath = "linktest";
char* newpath_soft = "softlinktest";
char* newpath_hard = "hardlinktest";
char* fifopath = "fifo1";

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	// Soft link
	// ln -s oldpath newpath_soft
	int error = symlink(oldpath, newpath_soft);

	if(!error){
		printf("Soft link creation success\n");
	}
	else{
		printf("Error with code: %d\n", error);
	}
	
	// Hard link
	// ln oldpath newpath_hard
	error = link(oldpath, newpath_hard);
	
	if(!error){
		printf("Hard link creation success\n");
	}
	else{
		printf("Error with code: %d\n", error);
	}
	
	// FIFO
	// mkfifo fifopath
	error = mkfifo(fifopath, 0640);
	
	if(!error){
		printf("FIFO link creation success\n");
	}
	else{
		printf("Error with code: %d\n", error);
	}
}

