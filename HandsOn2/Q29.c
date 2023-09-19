/*Write a program to remove the message queue.*/

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

int main(){
	struct msqid_ds mq, check;
	
	int key = ftok(".", 'a');
	int msgQID = msgget(key, IPC_CREAT|0666);
	
	if(msgQID<0){
		printf("Creation of message queue failed. \n");
	}
	else{
		printf("Creation success. MSG Q ID: %d\n", msgQID);
		msgctl(msgQID, IPC_STAT, &check);
		printf("Permissions before deleting: %d\n", check.msg_perm.mode);
		int ret = msgctl(msgQID, IPC_RMID, NULL);
		printf("Message Queue deleted\n");
	}
	
	return 0;
}
