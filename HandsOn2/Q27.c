/*Write a program to receive messages from the message queue.
a. with 0 as a flag
b. with IPC_NOWAIT as a flag*/

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
#include<sys/msg.h>

struct msgQ{
	long mtype;
	char mtxt[1024];
};

int main(int argc, char* argv[]){
	struct msgQ mq;
	int key = ftok(".", 'a');
	int msgQID = msgget(key, IPC_CREAT|0666);
	
	if(msgQID<0){
		printf("Creation of message queue failed. \n");
	}
	else{
		printf("Creation success. MSG Q ID: %d\n", msgQID);
		long mt = 1;
		int flag = 0;
		if(strcmp(argv[1],"0")!=0)
			flag = IPC_NOWAIT;
		msgrcv(msgQID, &mq, sizeof(mq), 1, flag);
		if(flag == 0)
			printf("Message with 0 as flag: %s\n", mq.mtxt);
		else
			printf("Message with IPC_NOWAIT as flag: %s\n", mq.mtxt);
		
	}
	
	return 0;
}
