/*
Author: Shivanirudh S G
File description: The client program. All driver programs run here
*/

#include "student_ops.h"

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Unable to create socket. \n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Unable to connect to server\n");
        exit(1);
    }
	printf("Connected to server. \n");

    char message[1024];
	int *role = calloc(2, sizeof(int));
    while (1) {
        // printf("Enter a message: ");
        // fgets(message, sizeof(message), stdin);
        // if (strlen(message) <= 1) {
        //     continue; // Skip empty messages
        // }
		role = mainMenu();

        // Send the message to the server
        // write(client_socket, message, strlen(message));
		write(client_socket, role, sizeof(role));

        // Receive and print the response from the server
        // int bytes_received = read(client_socket, message, sizeof(message));
        // if (bytes_received <= 0) {
        //     perror("Connection closed by server");
        //     break;
        // }
        // message[bytes_received] = '\0';
        // printf("Server response: %s", message);
		read(client_socket, message, sizeof(message));
		printf("Server response: %s", message);
		if(strcmp(message, "Invalid Credentials!!!\n") == 0 || strcmp(message, "That login ID does not exist for that role. \n") == 0)
			continue;

		Combined comb;
		if(role[0] == 0)
			break;
		else if(role[0] == 1)
			printf("Welcome Mr. %s\n\n", "Vishnu Raj");
		else if(role[0] == 2){
			read(client_socket, &comb.f, sizeof(comb.f));
			printf("Welcome Prof. %s\n\n", comb.f.name);
		}
		else if(role[0] == 3){
			read(client_socket, &comb.s, sizeof(comb.s));
			printf("Welcome %s\n\n", comb.s.name);
		}

		while(1){
		
			Combined combine;

			if(role[0] == 1){
				// printf("Welcome Mr. %s\n\n", "Vishnu Raj");
				combine.role = 1;
				int opt;
				adminMenuDisplay();
				printf("Enter choice ");scanf("%d", &opt);
				combine.action = opt;
				
				if(opt == 0){
					combine.action = -1;
					write(client_socket, (void*)&combine, sizeof(combine));
					break;
				}
				else if(opt == 1){
					printf("\n\n");
					combine.s = inputStudent();
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.ID, sizeof(combine.ID));

					printf("Saved successfully\n");
					printf("Student's Login ID: MT%d\n", combine.ID);
					printf("One time Password: MT%d\n", combine.ID);
					printf("Note: Student will be forced to change password on the first login. \n");
					printf("\n\n");
				}
				else if(opt == 2){
					printf("Enter ID: ");scanf("%d", &combine.ID);
					printf("\n\n");
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.s, sizeof(combine.s));
					if(combine.s.active == true)
						printStudent(combine.s);
					else
						printf("Student with this ID not found\n");
					printf("\n\n");
				}
				else if(opt == 3){
					printf("\n\n");
					combine.f = inputFaculty();
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.ID, sizeof(combine.ID));

					printf("Saved successfully\n");
					printf("Faculty Login ID: PROF%d\n", combine.ID);
					printf("One time Password: PROF%d\n", combine.ID);
					printf("Note: Faculty will be forced to change password on the first login. \n");
					printf("\n\n");
				}
				else if(opt == 4){
					printf("Enter ID: ");scanf("%d", &combine.ID);
					printf("\n\n");
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.f, sizeof(combine.f));
					if(combine.f.active == true)
						printFaculty(combine.f);
					else
						printf("Faculty with this ID not found\n");
					printf("\n\n");
				}
				else if(opt == 5){
					printf("Enter ID: ");scanf("%d", &combine.ID);
					printf("\n\n");
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.s, sizeof(combine.s));
					if(combine.s.active == true){
						printStudent(combine.s);
						if(combine.s.blocked == false)
							printf("Student is UNBLOCKED. Do you wish to BLOCK? (y/n)");
						else
							printf("Student is BLOCKED. Do you wish to UNBLOCK? (y/n)");
						scanf(" %c", &combine.choice);

						write(client_socket, (void*)&combine, sizeof(combine));
						printf("Updated successfully\n");
					}
					else
						printf("Student with this ID not found\n");
					printf("\n\n");
				}
				else if(opt == 6){
					printf("Enter ID: ");scanf("%d", &combine.ID);
					printf("\n\n");
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.s, sizeof(combine.s));
					if(combine.s.active == true){
						printStudent(combine.s);
						printf("Enter updated details: \n");
						combine.s = inputStudent();

						write(client_socket, (void*)&combine, sizeof(combine));
						printf("Updated successfully\n");
					}
					else
						printf("Student with this ID not found\n");
					printf("\n\n");
				}
				else if(opt == 7){
					printf("Enter ID: ");scanf("%d", &combine.ID);
					printf("\n\n");
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.f, sizeof(combine.f));
					if(combine.f.active == true){
						printFaculty(combine.f);
						printf("Enter updated details: \n");
						combine.f = inputFaculty();

						write(client_socket, (void*)&combine, sizeof(combine));
						printf("Updated successfully\n");
					}
					else
						printf("Faculty with this ID not found\n");
					printf("\n\n");
				}
				else if(opt == 8){
					printf("Enter ID: ");scanf("%d", &combine.ID);
					printf("\n\n");
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.s, sizeof(combine.s));
					if(combine.s.active == true){
						printStudent(combine.s);
						printf("Do you wish to delete this student? (y/n)");
						scanf(" %c", &combine.choice);

						write(client_socket, (void*)&combine, sizeof(combine));
						printf("Deleted successfully\n");
					}
					else
						printf("Student with this ID not found\n");
					printf("\n\n");
				}
				else if(opt == 9){
					printf("Enter ID: ");scanf("%d", &combine.ID);
					printf("\n\n");
					write(client_socket, (void*)&combine, sizeof(combine));

					read(client_socket, &combine.f, sizeof(combine.f));
					if(combine.f.active == true){
						printFaculty(combine.f);
						printf("Do you wish to delete this faculty? (y/n)");
						scanf(" %c", &combine.choice);

						write(client_socket, (void*)&combine, sizeof(combine));
						printf("Deleted successfully\n");
					}
					else
						printf("Faculty with this ID not found\n");
					printf("\n\n");
				}
				else{
					printf("Invalid option\n");
				}
			}
			else if(role[0] == 2){
				// read(client_socket, &combine.f, sizeof(combine.f));
				combine.role = 2;
				combine.f = comb.f;
				printf("Welcome Prof. %s\n\n", combine.f.name);
				if(combine.f.activated == false){
					printf("You are logging in for the first time. Please change password. \n");
					printf("\n\n");
					char buf[1024], rebuf[1024];
					do{
						printf("Enter new password: ");scanf(" %[^\n]", buf);
						printf("Re-enter new password: ");scanf(" %[^\n]", rebuf);
						if(strcmp(buf, rebuf) != 0){
							printf("Passwords are not matching. Please re-enter: \n\n");
						}
					}while(strcmp(buf, rebuf) != 0);
					strcpy(combine.password, buf);
					write(client_socket, (void*)&combine, sizeof(combine));
					combine.f.activated = true; comb.f.activated = true;
					printf("Password changed successfully\n");
					printf("\n\n");
				}
				else{
					int opt;
					facultyMenuDisplay();
					printf("Enter choice ");scanf("%d", &opt);
					combine.action = opt;
					
					if(opt == 0){
						combine.action = -1;
						write(client_socket, (void*)&combine, sizeof(combine));
						// read(client_socket, (void*)&combine.f, sizeof(f));
						break;
					}
					else if(opt == 1){
						printf("\n\n");
						combine.c = inputCourse();
						write(client_socket, (void*)&combine, sizeof(combine));
						printf("Course added successfully\n");
						printf("\n\n");
					}
					else if(opt == 2){
						printf("\n\n");
						write(client_socket, (void*)&combine, sizeof(combine));
						int count = 0;

						read(client_socket, &count, sizeof(count));
						printf("Number of courses: %d\n", count);

						if(count == 0){
							printf("You are not offering any course yet\n");
						}
						else{
							Course temp;
							// for(int i =0;i<count;i++){
							// 	read(client_socket, (void*)&temp, sizeof(temp));
							// 	printCourse(temp);
							// }
							while(count--){
								read(client_socket, (void*)&temp, sizeof(temp));
								printCourse(temp);
							}		
						}
						printf("\n\n");
					}
					else if(opt == 3){
						printf("Enter course code: ");scanf("%d", &combine.code);
						printf("\n\n");
						write(client_socket, (void*)&combine, sizeof(combine));

						read(client_socket, &combine.c, sizeof(combine.c));
						if(combine.c.active == true){
							printCourse(combine.c);
							printf("Enter updated details: \n");
							combine.c = inputCourse();

							write(client_socket, (void*)&combine, sizeof(combine));
							printf("Course updated successfully\n");
						}
					else
						printf("Course with this code not found\n");
					printf("\n\n");
					}
					else if(opt == 4){
						printf("Enter code: ");scanf("%d", &combine.code);
						printf("\n\n");
						write(client_socket, (void*)&combine, sizeof(combine));

						read(client_socket, &combine.c, sizeof(combine.c));
						if(combine.c.active == true){
							printCourse(combine.c);
							printf("Do you wish to delete this course? (y/n)");scanf(" %c", &combine.choice);
							write(client_socket, (void*)&combine, sizeof(combine));
							printf("Course deleted successfully\n");
						}
						else
							printf("Course with this code not found\n");
						printf("\n\n");
					}
					else if(opt == 5){
						printf("\n\n");
						char buf[1024], rebuf[1024];
						do{
							printf("Enter new password: ");scanf(" %[^\n]", buf);
							printf("Re-enter new password: ");scanf(" %[^\n]", rebuf);
							if(strcmp(buf, rebuf) != 0){
								printf("Passwords are not matching. Please re-enter: \n\n");
							}
						}while(strcmp(buf, rebuf) != 0);
						strcpy(combine.password, buf);
						write(client_socket, (void*)&combine, sizeof(combine));
						printf("Password changed successfully\n");
						printf("\n\n");
					}
					else{
						printf("Invalid option\n");
					}
				}
			}
			else if(role[0] == 3){
				combine.role = 3;
				combine.s = comb.s;
				printf("Welcome %s\n\n", combine.s.name);
				if(combine.s.activated == false){
					printf("You are logging in for the first time. Please change password. \n");
					printf("\n\n");
					char buf[1024], rebuf[1024];
					do{
						printf("Enter new password: ");scanf(" %[^\n]", buf);
						printf("Re-enter new password: ");scanf(" %[^\n]", rebuf);
						if(strcmp(buf, rebuf) != 0){
							printf("Passwords are not matching. Please re-enter: \n\n");
						}
					}while(strcmp(buf, rebuf) != 0);
					strcpy(combine.password, buf);
					write(client_socket, (void*)&combine, sizeof(combine));
					combine.s.activated = true; comb.s.activated = true;
					printf("Password changed successfully\n");
					printf("\n\n");
				}
				else{
					int opt;
					studentMenuDisplay();
					printf("Enter choice ");scanf("%d", &opt);
					combine.action = opt;
					if(opt == 0){
						combine.action = -1;
						write(client_socket, (void*)&combine, sizeof(combine));
						break;
					}
					else if(opt == 1){
						printf("\n\n");
						
						viewAllCourses();

						printf("Enter course code you want to enroll in: "); scanf("%d", &combine.code);
						write(client_socket, (void*)&combine, sizeof(combine));
						read(client_socket, (void*)&combine.c, sizeof(combine.c));
						printCourse(combine.c);
						printf("Do you want to enroll in the above course? (y/n)");scanf(" %c", &combine.choice);
						write(client_socket, (void*)&combine, sizeof(combine));
						write(client_socket, message, sizeof(message));
						printf("Message from server: %s", message);

						printf("\n\n");
					}
					else if(opt == 2){
						printf("\n\n");
						write(client_socket, (void*)&combine, sizeof(combine));
						int count = 0;

						read(client_socket, &count, sizeof(count));
						printf("Number of courses: %d\n", count);

						if(count == 0){
							printf("You have not enrolled in any course yet\n");
						}
						else{
							Course temp;
							while(count--){
								read(client_socket, (void*)&temp, sizeof(temp));
								printCourse(temp);
							}		
						}
						printf("\n\n");
					}
					else if(opt == 3){
						write(client_socket, (void*)&combine, sizeof(combine));
						int count = 0;

						read(client_socket, &count, sizeof(count));
						printf("Number of courses: %d\n", count);

						if(count == 0){
							printf("You have not enrolled in any course yet\n");
						}
						else{
							Course temp;
							while(count--){
								read(client_socket, (void*)&temp, sizeof(temp));
								printCourse(temp);
							}
							printf("Enter course code you want to unenroll from: ");scanf("%d", &combine.code);
							write(client_socket, (void*)&combine, sizeof(combine));		
							read(client_socket, (void*)&combine.c, sizeof(combine.c));
							printCourse(combine.c);
							printf("Do you want to unenroll from the above course? (y/n)");scanf(" %c", &combine.choice);
							write(client_socket, (void*)&combine, sizeof(combine));
							write(client_socket, message, sizeof(message));
							printf("Message from server: %s", message);
						}

					}
					else if(opt == 4){
						printf("\n\n");
						char buf[1024], rebuf[1024];
						do{
							printf("Enter new password: ");scanf(" %[^\n]", buf);
							printf("Re-enter new password: ");scanf(" %[^\n]", rebuf);
							if(strcmp(buf, rebuf) != 0){
								printf("Passwords are not matching. Please re-enter: \n\n");
							}
						}while(strcmp(buf, rebuf) != 0);
						strcpy(combine.password, buf);
						write(client_socket, (void*)&combine, sizeof(combine));
						printf("Password changed successfully\n");
						printf("\n\n");
					}
				}
			}
		}
		
    }

    close(client_socket);
    return 0;
}

