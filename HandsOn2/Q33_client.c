/*
Write a program to communicate between two machines using socket.
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
	struct sockaddr_in serv;
	
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		printf("Unable to create socket. \n");
		exit(0);
	}
	
	bzero(&serv, sizeof(serv));
	
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	//serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(8000);
	
	connect(sd, (struct sockaddr*) &serv, sizeof(serv));
	
	char buf[1024];
	printf("Enter msg for server: ");scanf(" %[^\n]", buf);
	write(sd, buf, sizeof(buf));
	
	int rd = read(sd, buf, sizeof(buf));
	buf[rd] = '\0';
	printf("Message from server: %s\n", buf);
	
	close(sd);
	return 0;
}
