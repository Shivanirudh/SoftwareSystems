/*
Author: Shivanirudh S G
File description: Creation of admin credentials
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<stdbool.h>

#define admin_file "admin_creds.bin"

typedef struct Admin{
	char ID[20];
	char password[30];
} Admin;

int main(){
	int admin_fd = open(admin_file, O_RDWR|O_CREAT, S_IRWXU);
	Admin a;
	printf("Enter login ID: ");scanf(" %[^\n]", a.ID);
	printf("Enter password: ");scanf(" %[^\n]", a.password);
	write(admin_fd, &a, sizeof(a));
	printf("Admin created successfully\n");
	close(admin_fd);
	return 0;
}
