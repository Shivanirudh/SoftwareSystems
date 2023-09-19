/*Write a program to change the exiting message queue permission. (use msqid_ds structure)*/

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
		printf("Permissions before changing: %d\n", check.msg_perm.mode);
		mq.msg_perm.mode = 0644;
		int ret = msgctl(msgQID, IPC_SET, &mq);
		if(ret != 0)
			printf("Error changing permissions.\n");
		else{
			msgctl(msgQID, IPC_STAT, &check);
			printf("Permissions after changing: %d\n", check.msg_perm.mode);
		}
	}
	
	return 0;
}
