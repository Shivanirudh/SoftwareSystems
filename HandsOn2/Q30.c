/*Write a program to create a shared memory.
a. write some data to the shared memory
b. attach with O_RDONLY and check whether you are able to overwrite.
c. detach the shared memory
d. remove the shared memory*/

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
#include<sys/shm.h>

int main(){
	int key = ftok(".", 'a');
	int shmID = shmget(key, 1024,IPC_CREAT|0744);
	void* addr;
	void* addr_ro;
	
	if(shmID<0){
		printf("Creation of shared memory failed. \n");
	}
	else{
		printf("Creation success. Shared Memory ID: %d\n", shmID);
		addr = shmat(shmID, (void*)0, 0);
		printf("Enter content into shared memory: ");scanf(" %[^\n]", (char*)addr);
		printf("SHM contents: %s\n", (char*)addr);
		
		addr_ro = shmat(shmID, (void*)0, SHM_RDONLY);
		//printf("Enter content into shared memory in read mode: ");scanf(" %[^\n]", (char*)addr_ro);
		//printf("SHM contents in read mode: %s\n", (char*)addr_ro);
		
		shmdt(addr);
		shmdt(addr_ro);
		printf("Detached\n");
		
		shmctl(shmID, IPC_RMID, NULL);
		printf("Shared memory deleted\n");
		
		
	}
	
	return 0;
}
