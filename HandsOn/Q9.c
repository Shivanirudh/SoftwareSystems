/*
 ============================================================================
Name : Q9.c
Author : Shivanirudh S G
Description : Write a program to print the following information about a given file.
			a. inode
			b. number of hard links
			c. uid
			d. gid
			e. size
			f. block size
			g. number of blocks
			h. time of last access
			i. time of last modification
			j. time of last change
Date: 18th Aug 2023.
============================================================================	
*/

#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<time.h>

int main(){
	
	char buf[1024];
	
	printf("Enter file name: ");
	scanf(" %[^\n]", buf);
	
	struct stat file_details;
	
	if(stat(buf, &file_details) == -1){
		printf("Error opening file\n");
	}
	else{
		printf("File details: \n");
		printf("Inode number: %ld\n", file_details.st_ino);
		printf("Number of hard links: %ld\n", file_details.st_nlink);
		printf("Owner User ID: %d\n", file_details.st_uid);
		printf("Owner Group ID: %d\n", file_details.st_gid);
		printf("Size: %ld\n", file_details.st_size);
		printf("Block size: %ld\n", file_details.st_blksize);
		printf("Number of blocks: %ld\n", file_details.st_blocks);
		
		/*long int h, m, s;
		long int t = file_details.st_atim.tv_sec;
		h = t/(60*60);
		t = t % (60*60);
		m = t/60;
		t = t % 60;
		printf("Time of last access: %ld hours, %ld minutes, %ld seconds\n", h, m, t);
		*/
		printf("Time of last access: %ld\n", file_details.st_atim.tv_sec);
		
		/*t = file_details.st_mtim.tv_sec;
		h = t/(60*60);
		t = t % (60*60);
		m = t/60;
		t = t % 60;
		printf("Time of last modification: %ld hours, %ld minutes, %ld seconds\n", h, m, t);*/
		printf("Time of last modification: %ld\n", file_details.st_mtim.tv_sec);
		
		/*t = file_details.st_ctim.tv_sec;
		h = t/(60*60);
		t = t % (60*60);
		m = t/60;
		t = t % 60;
		printf("Time of last change: %ld hours, %ld minutes, %ld seconds\n", h, m, t);*/
		printf("Time of last change: %ld\n", file_details.st_ctim.tv_sec);
	}
	
	return 0;
}
