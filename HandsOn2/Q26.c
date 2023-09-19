/*Write a program to send messages to the message queue. Check $ipcs -q*/

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

int main(){
	struct msgQ mq;
	int key = ftok(".", 'a');
	int msgQID = msgget(key, IPC_CREAT|0666);
	
	if(msgQID<0){
		printf("Creation of message queue failed. \n");
	}
	else{
		printf("Creation success. MSG Q ID: %d\n", msgQID);
		mq.mtype = 1;
		strcpy(mq.mtxt,"MQ msg");
		msgsnd(msgQID, &mq, sizeof(mq), 0);
	}
	
	return 0;
}
