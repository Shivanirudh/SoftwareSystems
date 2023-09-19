/*Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
a. access permission
b. uid, gid
c. time of last message sent and received
d. time of last change in the message queue
d. size of the queue
f. number of messages in the queue
g. maximum number of bytes allowed
h. pid of the msgsnd and msgrcv*/

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
	struct msqid_ds mq;
	
	int key = ftok(".", 'a');
	int msgQID = msgget(key, IPC_CREAT|0666);
	
	if(msgQID<0){
		printf("Creation of message queue failed. \n");
	}
	else{
		printf("Creation success. MSG Q ID: %d\n", msgQID);
		msgctl(msgQID, IPC_STAT, &mq);
		printf("Permissions: %d\n", mq.msg_perm.mode);
		printf("UID: %d\n", mq.msg_perm.uid);
		printf("GID: %d\n", mq.msg_perm.gid);
		printf("Time of last msg sent: %ld\n", mq.msg_stime);
		printf("Time of last msg received: %ld\n", mq.msg_rtime);
		printf("Time of last msg modification: %ld\n", mq.msg_ctime);
		printf("Size of queue in bytes: %ld\n", mq.msg_cbytes);
		printf("Number of msgs in queue: %ld\n", mq.msg_qnum);
		printf("Maximum number of bytes allowed: %ld\n", mq.msg_qbytes);
		printf("PID of last msgsnd: %d\n", mq.msg_lspid);
		printf("PID of last msgrcv: %d\n", mq.msg_lrpid);
		
	}
	
	return 0;
}
