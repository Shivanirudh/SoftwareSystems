#include "student_ops.h"

int main(){
	struct sockaddr_in serv, cli;
	
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("Unable to create socket. \n");
		exit(0);
	}
	
	bzero(&serv, sizeof(serv));
	
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(PORT_NO);
	
	if(connect(sd, (struct sockaddr*) &serv, sizeof(serv)) < 0)
		printf("Unable to connect to server\n");
	else
		printf("Connected to server. \n");
	
	char buf[1024];
	//int len = sizeof(cli);
	while(1){
		
		int *role = mainMenu();
		printf("Client: %d %d\n", role[0], role[1]);
		write(sd, role, sizeof(role));
		
		
		bzero(&buf, sizeof(buf));
		read(sd, buf, sizeof(buf));
		if(strcmp(buf, "exit") == 0)
			break;
		printf("%s\n", buf);
		if(role[0] == -2){
			printf("Invalid Credentials!!!\n");
			continue;
		}
		else if(role[0] == -1){
			printf("That login ID does not exist for that role. \n");
			continue;
		}
		else if(role[0] == 0){
			break;
		}
		else if(role[0] == 1){
			//Admin a = getAdmin();
			Admin a;
			read(sd, (void*) &a, sizeof(a));
			// adminDriver(a);	
			int opt = -1;char buf[1024];int ID = -1;
			bzero(&buf, sizeof(buf));read(sd, buf, sizeof(buf));printf("%s", buf);usleep(100000);
			while(1){
				adminMenuDisplay();
				printf("Enter choice: ");scanf("%d", &opt);
				if(opt == 0) break;
				else write(sd, &opt, sizeof(opt));
				if(opt == 1){
					Student s = inputStudent();
					write(sd, (void*) &s, sizeof(s));
					read(sd, &ID, sizeof(ID));
					printf("Saved successfully\n");
					printf("Student's Login ID: MT%d\n", ID);
					printf("One time Password: MT%d\n", ID);
					printf("Note: Student will be forced to change password on the first login. \n");
				}
			}
		}
		else if(role[0] == 2){
			//Faculty f = getFaculty(role[1]);
			Faculty f;
			read(sd, (void*) &f, sizeof(f));
			facultyDriver(f);
		}
		else if(role[0] == 3){
			//Student s = getStudent(role[1]);
			Student s;
			read(sd, (void*) &s, sizeof(s));
			studentDriver(s);
		}
		/*
		bzero(&buf, sizeof(buf));
		read(sd, buf, sizeof(buf));
		printf("Message from server: %s\n", buf);
		bzero(buf, sizeof(buf));
		printf("\nEnter the message: ");scanf(" %[^\n]", buf);
		write(sd, buf, sizeof(buf));*/
	}
	
	close(sd);
	return 0;
}
