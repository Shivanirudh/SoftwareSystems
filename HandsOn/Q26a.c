/*Write a program to execute an executable program.
a. use some executable program
b. pass some input to an executable program. (for example execute an executable of $./a.out name)*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(int argc, char* argv[]){
	execl("/home/shivanirudh/Documents/iiitb/SoftwareSystems/HandsOn/25", "25", "/home/shivanirudh/Documents/iiitb/SoftwareSystems/HandsOn/25", (char*) NULL);		
	return 0;
}

