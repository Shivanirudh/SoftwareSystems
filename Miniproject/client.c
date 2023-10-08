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
			// Admin a;
			// read(sd, (void*) &a, sizeof(a));
			// adminDriver(a);	
			int opt = -1;char buf[1024];int ID = -1;
			bzero(&buf, sizeof(buf));read(sd, buf, sizeof(buf));printf("%s", buf);usleep(100000);
			while(1){
				adminMenuDisplay();
				printf("Enter choice: ");scanf("%d", &opt);
				if(opt == 0) break;
				else write(sd, &opt, sizeof(opt));
				if(opt == 1){
					printf("\n\n");
					Student s = inputStudent();
					write(sd, (void*) &s, sizeof(s));
					read(sd, &ID, sizeof(ID));
					printf("Saved successfully\n");
					printf("Student's Login ID: MT%d\n", ID);
					printf("One time Password: MT%d\n", ID);
					printf("Note: Student will be forced to change password on the first login. \n");
					printf("\n\n");
				}
				else if(opt == 2){
					printf("\n\n");
					Student s;
					printf("Enter ID: ");scanf("%d", &ID);
					write(sd, &ID, sizeof(ID));
					read(sd, (void*) &s, sizeof(s));
					if(s.active == false) printf("Student with this ID not found\n");
					else printStudent(s);
					printf("\n\n");
				}
				else if(opt == 3){
					printf("\n\n");
					Faculty f = inputFaculty();
					write(sd, (void*) &f, sizeof(f));
					read(sd, &ID, sizeof(ID));
					printf("Saved successfully\n");
					printf("Faculty Login ID: PROF%d\n", ID);
					printf("One time Password: PROF%d\n", ID);
					printf("Note: Faculty will be forced to change password on the first login. \n");
					printf("\n\n");
				}
				else if(opt == 4){
					printf("\n\n");
					Faculty f;
					printf("Enter ID: ");scanf("%d", &ID);
					write(sd, &ID, sizeof(ID));
					read(sd, (void*) &f, sizeof(f));
					if(f.active == false) printf("Faculty with this ID not found\n");
					else printFaculty(f);
					printf("\n\n");
				}
				else if(opt == 5){
					printf("\n\n");
					int block;
					printf("Enter ID: ");scanf("%d", &ID);
					printf("Block or unblock? (1/0)");scanf("%d", &block);
					write(sd, &ID, sizeof(ID));
					write(sd, &block, sizeof(block));
					char buf[1024];
					read(sd, buf, sizeof(buf));
					printf("%s\n", buf);
					printf("\n\n");
				}
				else if(opt == 6){
					printf("\n\n");
					Student s;
					printf("Enter ID: ");scanf("%d", &ID);
					write(sd, &ID, sizeof(ID));
					read(sd, (void*) &s, sizeof(s));
					if(s.active == false) printf("Student with this ID not found\n");
					else{
						printStudent(s);
						printf("Enter updated details: \n");
						Student st = inputStudent();
						write(sd, (void*) &st, sizeof(st));
						char buf[1024];
						read(sd, buf, sizeof(buf));
						printf("%s\n", buf);
					}
					printf("\n\n");
				}
				else if(opt == 7){
					printf("\n\n");
					Faculty f;
					printf("Enter ID: ");scanf("%d", &ID);
					write(sd, &ID, sizeof(ID));
					read(sd, (void*) &f, sizeof(f));
					if(f.active == false) printf("Faculty with this ID not found\n");
					else{
						printFaculty(f);
						printf("Enter updated details: \n");
						Faculty fa = inputFaculty();
						write(sd, (void*) &fa, sizeof(fa));
						char buf[1024];
						read(sd, buf, sizeof(buf));
						printf("%s\n", buf);
					}
					printf("\n\n");
				}
				else if(opt == 8){
					printf("Enter ID: ");scanf("%d", &ID);
					write(sd, &ID, sizeof(ID));
					Student s;
					read(sd, (void*) &s, sizeof(s));
					if(s.active == false) printf("Student with this ID not found\n");
					else{
						printStudent(s);char choice;
						printf("Do you want to delete above entry? (y/n)");scanf(" %c", &choice);
						write(sd, &choice, sizeof(choice));
						if(choice == 'y'||choice=='Y'){
							char buf[1024];
							read(sd, buf, sizeof(buf));
							printf("%s\n", buf);
						}
					}
					printf("\n\n");
				}
				else if(opt == 9){
					printf("Enter ID: ");scanf("%d", &ID);
					write(sd, &ID, sizeof(ID));
					Faculty f;
					read(sd, (void*) &f, sizeof(f));
					if(f.active == false) printf("Faculty with this ID not found\n");
					else{
						printFaculty(f);char choice;
						printf("Do you want to delete above entry? (y/n)");scanf(" %c", &choice);
						write(sd, &choice, sizeof(choice));
						if(choice == 'y'||choice=='Y'){
							char buf[1024];
							read(sd, buf, sizeof(buf));
							printf("%s\n", buf);
						}
					}
					printf("\n\n");
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
