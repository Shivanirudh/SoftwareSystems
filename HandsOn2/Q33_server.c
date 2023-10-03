/* 
 ============================================================================
Name : Q33_server.c
Author : Shivanirudh S G
Description : Write a program to communicate between two machines using socket.
Date: 1st Oct 2023.
============================================================================
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
	struct sockaddr_in serv, cli;
	
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		printf("Unable to create socket. \n");
		exit(0);
	}
	
	bzero(&serv, sizeof(serv));
	
	serv.sin_family = AF_INET;
	//serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(8000);
	
	if(bind(sd, (struct sockaddr*)&serv, sizeof(serv)) < 0)
		printf("Bind error\n");
	listen(sd, 5);
	
	int len = sizeof(cli);
	int nsd = accept(sd, (struct sockaddr*) &cli, &len);
	if(nsd<0)printf("Connection issue\n");
	
	char buf[1024];
	int rd = read(nsd, buf, sizeof(buf));
	printf("Message from client: %s\n", buf);
	
	printf("Enter msg for client: ");scanf(" %[^\n]", buf);
	write(nsd, buf, sizeof(buf));
	
	close(sd);close(nsd);
	return 0;
}
