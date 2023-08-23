/*Write a program to perform Record locking.
a. Implement write lock
b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access to avoid race condition.*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>

struct Record{
	int num1;
	int num2;
};

int main(){
	
	int fd;
	fd = open("records.txt", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	if(fd<0){
		printf("\nError opening records file");
		return -1;
	}
	
	struct Record r1;
	for(int i = 0;i<3;i++){
		printf("Record %d\n", i+1);
		printf("Enter number 1: ");scanf("%d", &r1.num1);
		printf("Enter number 2: ");scanf("%d", &r1.num2);
		write(fd, &r1, sizeof(r1));
	}
	
	close(fd);
	
	return 0;
}
