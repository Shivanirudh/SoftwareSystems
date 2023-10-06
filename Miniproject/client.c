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
			Admin a;
			read(sd, (void*) &a, sizeof(a));

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
					read(sd, &ID, sizeof(int));
					
					printf("Saved successfully\n");
					printf("Student's Login ID: MT%d\n", ID);
					printf("One time Password: MT%d\n", ID);
					printf("Note: Student will be forced to change password on the first login. \n");
					
					printf("\n\n");
				}
				else if(opt == 2){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");

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
					read(sd, &ID, sizeof(int));
					
					printf("Saved successfully\n");
					printf("Faculty Login ID: PROF%d\n", ID);
					printf("One time Password: PROF%d\n", ID);
					printf("Note: Faculty will be forced to change password on the first login. \n");
					
					printf("\n\n");
				}
				else if(opt == 4){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");

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
					
					write(sd, &ID, sizeof(ID));
					Student s;
					read(sd, (void*) &s, sizeof(s));
					if(s.active == false){
						printf("Student with this ID not found\n");
					}
					else{
						if(s.blocked == false){
							char option;
							printf("Student status is UNBLOCKED. Do you wish to BLOCK? (y/n): ");scanf(" %c", &option);
							if(option == 'y' || option == 'Y'){
								s.blocked = true;
								printf("Student is BLOCKED.\n");
							}
							else{
								printf("Student is UNBLOCKED.\n");
							}
						}
						else{
							char option;
							printf("Student status is BLOCKED. Do you wish to UNBLOCK? (y/n): ");scanf(" %c", &option);
							if(option == 'y' || option == 'Y'){
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
						Student st = updateStudent(ID);
						st.ID = ID;
						write(sd, (void*) &st, sizeof(st));
					}
					printf("\n\n");
				}
				else if(opt == 7){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
					
					write(sd, &ID, sizeof(ID));
					Faculty f;
					read(sd, (void*) &f, sizeof(f));
					if(f.active == false){
						printf("Faculty with this ID not found\n");
					}
					else{
						Faculty fa = updateFaculty(ID);
						fa.ID = ID;
						write(sd, (void*) &fa, sizeof(fa));
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
					printf("\n\n");
				}
				else if(opt == 9){
					printf("Enter ID: ");scanf("%d", &ID);
					printf("\n\n");
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
			Faculty f;
			read(sd, (void*) &f, sizeof(f));

			printf("Welcome Prof. %s\n\n", f.name);
			if(f.activated == false){
				printf("You are logging in for the first time. Please change password. \n");
				printf("\n\n");
				char *buf = passwordChange(f.ID);
				write(sd, buf, sizeof(buf));
				bool first_time = true;
				write(sd, &first_time, sizeof(first_time));
				printf("\n\n");
			}
			else{
				int opt = -1;
				while(1){
					int code;
					facultyMenuDisplay();
					printf("Enter choice: ");scanf("%d", &opt);
					write(sd, &opt, sizeof(opt));

					if(opt == 0) break;
					else if(opt == 1){
						printf("\n\n");
						Course c = addCourse();
						write(sd, (void*) &c, sizeof(c));
						write(sd, f.name, sizeof(f.name));

						read(sd, &code, sizeof(code));
						printf("Saved successfully\n");
						printf("Course code: %d\n", code);

						printf("\n\n");
					}
					else if(opt == 2){
						printf("\n\n");
						write(sd, f.name, sizeof(f.name));
						Course *ans;
						int count = 0;
						read(sd, ans, sizeof(ans));
						read(sd, &count, sizeof(count));
						for(int i =0;i<count;i++){
							printCourse(ans[i]);
						}
						printf("\n\n");
					}
					else if(opt == 3){
						printf("Enter code: ");scanf("%d", &code);
						printf("\n\n");
						write(sd, &code, sizeof(code));
						Course c;
						read(sd, (void*) &c, sizeof(c));
						if(c.active == false){
							printf("Course with this code not found\n");
						}
						else{
							printf("Removed successfully\n");
						}
						//removeCourse(code);
						printf("\n\n");
					}
					else if(opt == 4){
						printf("\n\n");
						char *buf = passwordChange(f.ID);
						write(sd, buf, sizeof(buf));
						printf("\n\n");
					}
					else{
						printf("Invalid option\n");
					}
				}
			}	


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
