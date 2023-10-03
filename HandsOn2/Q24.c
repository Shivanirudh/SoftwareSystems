/*
 ============================================================================
Name : Q24.c
Author : Shivanirudh S G
Description : Write a program to create a message queue and print the key and message queue id.
Date: 19th Sep 2023.
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
#include<sys/msg.h>

int main(){
	int key = ftok(".", 'a');
	int msgQID = msgget(key, IPC_CREAT|0666);
	
	if(msgQID<0){
		printf("Creation of message queue failed. \n");
	}
	else{
		printf("Creation success. MSG Q ID: %d\n", msgQID);
	}
	
	return 0;
}
