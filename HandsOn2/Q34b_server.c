/* 
 ============================================================================
Name : Q34b_server.c
Author : Shivanirudh S G
Description : Write a program to communicate between two machines using socket.
			b. use pthread_create
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
#include<pthread.h>

struct param{
	int csd; 
	char buf[1024];
	struct sockaddr_in cli;
	int len;
	int cnt;
	int client_sockets[30];
	int new_fd;
	int ix;
};

void *update(void *parameters){

	struct param *params = (struct param *) parameters;
	read(params->csd, params->buf, sizeof(params->buf));
	//Check end of connection
	if(strcmp(params->buf, "end") == 0){
		getpeername(params->csd, (struct sockaddr*)&params->cli,&params->len);
		printf("\nHost disconnected. Socket: %d.\n",params->client_sockets[params->ix]);
		close(params->csd);
		params->client_sockets[params->ix] = 0;
		params->cnt--;
	}
	else{
		printf("\nMessage from Client %d: %s\n", params->client_sockets[params->ix], params->buf);
		
		bzero(params->buf, sizeof(params->buf));
		//Write message in buffer
		printf("\nEnter message: ");scanf(" %[^\n]", params->buf);
		write(params->new_fd, params->buf, sizeof(params->buf));
	}
}

struct param param_array[100];
int t = 0;

int main(){
	struct sockaddr_in serv, cli;
	int client_sockets[30];
	fd_set clientfds;
	int new_fd;
	char buf[1024];
	
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("Unable to create socket. \n");
		exit(0);
	}
	
	for(int i = 0; i < 30; i++)
		client_sockets[i] = 0;
	
	bzero(&serv, sizeof(serv));
	
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(7229);
	
	if(bind(sd, (struct sockaddr*)&serv, sizeof(serv)) < 0)
		perror("Bind error\n");
	listen(sd, 5);
	
	int len = sizeof(cli);
	int cnt = 0;

	while(1){
		FD_ZERO(&clientfds);

		FD_SET(sd, &clientfds);
		int max_sd = sd;

		for(int i = 0;i < 30;i++){
			int scd = client_sockets[i];
			if(scd > 0)
				FD_SET(scd, &clientfds);
			
			if(scd > max_sd)
				max_sd = scd;
		}

		int action = select(max_sd + 1, &clientfds, NULL, NULL, NULL);
		if(action<0){
			perror("\nSelect error!\n");
		}
		if(FD_ISSET(sd, &clientfds)){
			new_fd = accept(sd, (struct sockaddr*) &cli, &len);
			if(new_fd<0)perror("Connection issue\n");
			cnt++;
			printf("Client no. %d --- %s : %d connected\n", cnt, inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
			
			printf("Number of active connections: %d\n", cnt);
			strcpy(buf, "Connection established");
			write(new_fd, buf, sizeof(buf));
			
			for(int i =0;i<30;i++){
				if(client_sockets[i] == 0){
					client_sockets[i] = new_fd;
					printf("\nConnection at socket %d\n", i);
					break;
				}
			}
		}
		for(int i = 0;i<30;i++){
			int csd = client_sockets[i];
			//Check for change in descriptors
			if(FD_ISSET(csd, &clientfds)){
				t++;
				
				param_array[t].csd = csd;
				strcpy(param_array[t].buf, buf);
				param_array[t].cli = cli;
				param_array[t].len = len;
				for(int i=0;i<30;i++)
					param_array[t].client_sockets[i] = client_sockets[i];
				param_array[t].cnt = cnt;
				param_array[t].new_fd = new_fd;
				param_array[t].ix = i;
				
				pthread_t process;
				pthread_create(&process, NULL, update, &param_array[t]);
				
				/*if(!fork()){
					read(csd, buf, sizeof(buf));
					//Check end of connection
					if(strcmp(buf, "end") == 0){
						getpeername(csd, (struct sockaddr*)&cli,&len);
						printf("\nHost disconnected. Socket: %d.\n",client_sockets[i]);
						close(csd);
						client_sockets[i] = 0;
						cnt--;
					}
					else{
						printf("\nMessage from Client %d: %s\n", client_sockets[i], buf);
						
						bzero(buf, sizeof(buf));
						//Write message in buffer
						printf("\nEnter message: ");scanf(" %[^\n]", buf);
						write(new_fd, buf, sizeof(buf));
					}
				}*/
				pthread_join(process, NULL);
			}
		}
	}
	return 0;
}
