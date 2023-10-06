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
			//adminDriver(a);
			
			printf("Welcome Mr. %s\n\n", "Vishnu Raj");
			int opt = -1;
			while(1){
				int ID;
				adminMenuDisplay();
				printf("Enter choice: ");scanf("%d", &opt);
				write(sd, &opt, sizeof(opt));
				
				if(opt == 0) break;
				else if(opt == 1){
					printf("\n\n");
					Student s = addStudent();
					write(sd, (void*) &s, sizeof(s));
					printf("\n\n");
				}
				else if(opt == 2){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
					//viewStudent(ID);
					write(sd, &ID, sizeof(ID));
					Student s;
					read(sd, (void*) &s, sizeof(s));
					if(s.active == false){
						printf("Student with this ID not found\n");
					}
					else{
						printStudent(s);
					}
					printf("\n\n");
				}
				else if(opt == 3){
					printf("\n\n");
					Faculty f = addFaculty();
					write(sd, (void*) &f, sizeof(f));
					printf("\n\n");
				}
				else if(opt == 4){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
					//viewFaculty(ID);
					write(sd, &ID, sizeof(ID));
					Faculty f;
					read(sd, (void*) &f, sizeof(f));
					if(f.active == false){
						printf("Faculty with this ID not found\n");
					}
					else{
						printFaculty(f);
					}
					printf("\n\n");
				}
				else if(opt == 5){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
					//blockStudent(ID);
					write(sd, &ID, sizeof(ID));
					Student s;
					read(sd, (void*) &s, sizeof(s));
					if(s.active == false){
						printf("Student with this ID not found\n");
					}
					else{
						if(s.blocked == false){
							char opt;
							printf("Student status is UNBLOCKED. Do you wish to BLOCK? (y/n): ");scanf(" %c", &opt);
							if(opt == 'y' || opt == 'Y'){
								s.blocked = true;
								printf("Student is BLOCKED.\n");
							}
							else{
								printf("Student is UNBLOCKED.\n");
							}
						}
						else{
							char opt;
							printf("Student status is BLOCKED. Do you wish to UNBLOCK? (y/n): ");scanf(" %c", &opt);
							if(opt == 'y' || opt == 'Y'){
								s.blocked = false;
								printf("Student is UNBLOCKED.\n");
							}
							else{
								printf("Student is BLOCKED.\n");
							}
						}
					}
					write(sd, (void*) &s, sizeof(s));
					
					printf("\n\n");
				}
				else if(opt == 6){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
					write(sd, &ID, sizeof(ID));
					Student s;
					read(sd, (void*) &s, sizeof(s));
					if(s.active == false){
						printf("Student with this ID not found\n");
					}
					else{
						Student s = updateStudent(ID);
						write(sd, (void*) &s, sizeof(s));
					}
					printf("\n\n");
				}
				else if(opt == 7){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
					//updateFaculty(ID);
					write(sd, &ID, sizeof(ID));
					Faculty f;
					read(sd, (void*) &f, sizeof(f));
					if(f.active == false){
						printf("Faculty with this ID not found\n");
					}
					else{
						Faculty f = updateFaculty(ID);
						write(sd, (void*) &f, sizeof(f));
					}
					printf("\n\n");
				}
				else if(opt == 8){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
					write(sd, &ID, sizeof(ID));
					Student s;
					read(sd, (void*) &s, sizeof(s));
					if(s.active == false){
						printf("Student with this ID not found\n");
					}
					else{
						printf("Deleted successfully");
					}
					//deleteStudent(ID);
					printf("\n\n");
				}
				else if(opt == 9){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
					//deleteFaculty(ID);
					write(sd, &ID, sizeof(ID));
					Faculty f;
					read(sd, (void*) &f, sizeof(f));
					if(f.active == false){
						printf("Faculty with this ID not found\n");
					}
					else{
						printf("Deleted successfully");
					}
					printf("\n\n");
				}
				else{
					printf("Invalid option\n");
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
