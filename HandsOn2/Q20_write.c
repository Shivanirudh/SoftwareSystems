/*Write two programs so that both can communicate by FIFO -Use one way communication.*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>

int main(){
	//int fif = mkfifo("fifo_file", 0644);
	//if(fif<0){
	//	printf("\nError creating fifo file");
	//}
	int fif_id = open("fifo_file", O_CREAT|O_WRONLY);
	
	
	char buf[1024];
	printf("Enter msg for FIFO: ");
	scanf(" %[^\n]", buf);
	write(fif_id, buf, sizeof(buf));
	
	return 0;
}
