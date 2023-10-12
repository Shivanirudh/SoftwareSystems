/*
Author: Shivanirudh S G
File description: The server program. All DB accesses occur from here
*/

#include "student_ops.h"

void handle_client(int client_socket) {
	Faculty f; Student s;
    char buffer[1024];
	int bytes_received;
    int* role = calloc(2, sizeof(int));

    while (1) {
		bzero(role, sizeof(role));
		bzero(buffer, sizeof(buffer));
        // bytes_received = read(client_socket, buffer, sizeof(buffer));
        // if (bytes_received <= 0) {
        //     close(client_socket);
        //     exit(0); // Exit the child process
        // }
		bytes_received = read(client_socket, role, sizeof(role));
        if (bytes_received <= 0 || role[0] == 0) {
			printf("Client exit\n");
			strcpy(buffer, "Client exit\n");
			write(client_socket, buffer, sizeof(buffer));
            close(client_socket);
            exit(0); // Exit the child process
        }
        // buffer[bytes_received] = '\0';
        // printf("Received: %s", buffer);

        // // Echo the received message back to the client
        // write(client_socket, buffer, strlen(buffer));
		if(role[0] == -2){
			printf("Invalid Credentials!!!\n");
			strcpy(buffer, "Invalid Credentials!!!\n");
			write(client_socket, buffer, sizeof(buffer));
			continue;
		}
		else if(role[0] == -1){
			printf("That login ID does not exist for that role. \n");
			strcpy(buffer, "That login ID does not exist for that role. \n");
			write(client_socket, buffer, sizeof(buffer));
			continue;
		}
		else if(role[0] == 1){
			strcpy(buffer, "Logged in as Admin \n");
			write(client_socket, buffer, sizeof(buffer));
		}
		else if(role[0] == 2){
			strcpy(buffer, "Logged in as Faculty \n");
			write(client_socket, buffer, sizeof(buffer));
			f = getFaculty(role[1]);
			write(client_socket, (void*)&f, sizeof(f));
		}
		else if(role[0] == 3){
			strcpy(buffer, "Logged in as Student \n");
			write(client_socket, buffer, sizeof(buffer));
			s = getStudent(role[1]);
			write(client_socket, (void*)&s, sizeof(s));
		}

		// if(role[0] == 1 || role[0] == 2 ||  role[0] == 3)
		while(1){
			Combined combine;
			read(client_socket, (void*)&combine, sizeof(combine));
			if(combine.role == 1){
				printf("Admin operation: ");
				if(combine.action == 1){
					printf("Add student\n");
					int ID = addStudent(combine.s);
					write(client_socket, &ID, sizeof(ID));
				}
				else if(combine.action == 2){
					printf("View student\n");
					Student s = getStudent(combine.ID);
					printStudent(s);
					write(client_socket, (void*)&s, sizeof(s));
				}
				else if(combine.action == 3){
					printf("Add faculty\n");
					int ID = addFaculty(combine.f);
					write(client_socket, &ID, sizeof(ID));
				}
				else if(combine.action == 4){
					printf("View faculty\n");
					Faculty f = getFaculty(combine.ID);
					printFaculty(f);
					write(client_socket, (void*)&f, sizeof(f));
				}
				else if(combine.action == 5){
					printf("Block/Unblock student\n");
					Student s = getStudent(combine.ID);
					printStudent(s);
					write(client_socket, (void*)&s, sizeof(s));
					read(client_socket, (void*)&combine, sizeof(combine));
					if(combine.choice == 'y' || combine.choice == 'Y')
						blockStudent(combine.ID);
				}
				else if(combine.action == 6){
					printf("Update student\n");
					Student s = getStudent(combine.ID);
					printStudent(s);
					write(client_socket, (void*)&s, sizeof(s));
					read(client_socket, (void*)&combine, sizeof(combine));
					
					updateStudent(combine.ID, combine.s);
				}
				else if(combine.action == 7){
					printf("Update faculty\n");
					Faculty f = getFaculty(combine.ID);
					printFaculty(f);
					write(client_socket, (void*)&f, sizeof(f));
					read(client_socket, (void*)&combine, sizeof(combine));
					
					updateFaculty(combine.ID, combine.f);
				}
				else if(combine.action == 8){
					printf("Delete student\n");
					Student s = getStudent(combine.ID);
					printStudent(s);
					write(client_socket, (void*)&s, sizeof(s));
					read(client_socket, (void*)&combine, sizeof(combine));
					if(combine.choice == 'y' || combine.choice == 'Y')
						deleteStudent(combine.ID);
				}
				else if(combine.action == 9){
					printf("Delete faculty\n");
					Faculty f = getFaculty(combine.ID);
					printFaculty(f);
					write(client_socket, (void*)&f, sizeof(f));
					read(client_socket, (void*)&combine, sizeof(combine));
					if(combine.choice == 'y' || combine.choice == 'Y')
						deleteFaculty(combine.ID);
				}
				else if(combine.action == -1){
					printf("Exit\n");
					break;
				}
			}
			else if(combine.role == 2){
				// Faculty f = getFaculty(role[1]);
				// write(client_socket, (void*)&f, sizeof(f));
				if(combine.f.activated == false){
					passwordChange(combine.f.ID, true, combine.password);
				}
				else{
					if(combine.action == 1){
						printf("Add course\n");
						addCourse(combine.c, combine.f.name, combine.f.ID);
					}
					else if(combine.action == 2){
						printf("View my offered courses\n");
						int count = countOfferedCourses(combine.f.ID);
						printf("Number of courses: %d\n", count);
						int *courses = calloc(COURSE_LIMIT, sizeof(int));
						courses = viewOfferedCourses(combine.f.ID);
						write(client_socket, &count, sizeof(count));
						for(int i = 0;i<count;i++){
							printf("Course code: %d\n", courses[i]);
							Course cour = getCourse(courses[i]);
							printCourse(cour);
							write(client_socket, (void*)&cour, sizeof(cour));
						}
					}
					else if(combine.action == 3){
						printf("Modify offered course\n");
						Course c = getCourse(combine.code);
						printCourse(c);
						write(client_socket, (void*)&c, sizeof(c));
						read(client_socket, (void*)&combine, sizeof(combine));
						
						modifyCourse(combine.code, combine.c);

						checkCourseStatus();
					}
					else if(combine.action == 4){
						printf("Remove course\n");
						Course c = getCourse(combine.code);
						printCourse(c);
						write(client_socket, (void*)&c, sizeof(c));
						read(client_socket, (void*)&combine, sizeof(combine));
						if(combine.choice == 'y' || combine.choice == 'Y')
							removeCourse(combine.code);
						courseRemoval(combine.code);
					}
					else if(combine.action == 5){
						printf("Password change\n");
						passwordChange(combine.f.ID, false, combine.password);
					}
					else if(combine.action == -1){
						printf("Exit\n");
						break;
					}
				}
			}
			else if(combine.role == 3){
				// strcpy(buffer, "Student operation: \n");
				// printf("Student operation: ");
				if(combine.s.activated == false){
					passwordChangeStudent(combine.s.ID, true, combine.password);
				}
				else{
					if(combine.action == 1){
						int ret_val = 0;
						printf("Enroll in a course\n");
						Course c = getCourse(combine.code);
						write(client_socket, (void*)&c, sizeof(c));
						read(client_socket, (void*)&combine, sizeof(combine));
						if(combine.choice == 'y' || combine.choice=='Y'){
							ret_val = enrollCourse(combine.s, c);
							if(ret_val == 0)
								strcpy(buffer, "Enrolled successfully \n");
							else if(ret_val == -1)
								strcpy(buffer, "No seats available for this course. \n");
							else if(ret_val == -2)
								strcpy(buffer, "You have already enrolled in this course. \n");
							else if(ret_val == -3)
								strcpy(buffer, "You are blocked currently. Please contact the admin. \n");
						}
						else{
							strcpy(buffer, "Operation cancelled \n");
						}
						write(client_socket, buffer, sizeof(buffer));
					}
					else if(combine.action == 2){
						printf("View my enrolled courses\n");
						int count = countEnrolledCourses(combine.s);
						printf("Number of courses: %d\n", count);
						int *courses = calloc(COURSE_LIMIT, sizeof(int));
						courses = viewEnrolledCourses(combine.s);
						write(client_socket, &count, sizeof(count));
						for(int i = 0;i<count;i++){
							printf("Course code: %d\n", courses[i]);
							Course cour = getCourse(courses[i]);
							printCourse(cour);
							write(client_socket, (void*)&cour, sizeof(cour));
						}
					}
					else if(combine.action == 3){
						int ret_val = 0;
						printf("Unenroll from a course\n");
						int count = countEnrolledCourses(combine.s);
						int *courses = calloc(COURSE_LIMIT, sizeof(int));
						courses = viewEnrolledCourses(combine.s);
						write(client_socket, &count, sizeof(count));
						for(int i = 0;i<count;i++){
							printf("Course code: %d\n", courses[i]);
							Course cour = getCourse(courses[i]);
							printCourse(cour);
							write(client_socket, (void*)&cour, sizeof(cour));
						}
						if(count > 0){
							read(client_socket, (void*)&combine, sizeof(combine));
							Course c = getCourse(combine.code);
							write(client_socket, (void*)&c, sizeof(c));
							read(client_socket, (void*)&combine, sizeof(combine));
							if(combine.choice == 'y' || combine.choice=='Y'){
								ret_val = unenrollCourse(combine.s, c);
								if(ret_val == 0)
									strcpy(buffer, "Unenrolled successfully \n");
								else if(ret_val == -1)
									strcpy(buffer, "You have not enrolled in this course. \n");
								else if(ret_val == -2)
									strcpy(buffer, "You are blocked currently. Please contact the admin. \n");
							}
							else{
								strcpy(buffer, "Operation cancelled \n");
							}
						}
						
					}
					else if(combine.action == 4){
						printf("Password change\n");
						passwordChangeStudent(combine.s.ID, false, combine.password);
					}
					else if(combine.action == -1){
						printf("Exit\n");
						break;
					}
				}
			}
		}
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket binding failed");
        exit(1);
    }

    // Listen for incoming client connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Listening failed");
        exit(1);
    }

    while (1) {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Client connection failed");
            continue;
        }

        // Create a new child process to handle the client
        pid_t pid = fork();
        if (pid == 0) {
            // In the child process
            close(server_socket); // Close the server socket in the child process
            handle_client(client_socket);
        } else if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else {
            // In the parent process
            close(client_socket); // Close the client socket in the parent process
        }
    }

    close(server_socket);
    return 0;
}


// int main(){
// 	struct sockaddr_in serv, cli;
// 	int client_sockets[30];
// 	fd_set clientfds;
// 	int new_fd;
// 	char buf[1024];
	
// 	int sd = socket(AF_INET, SOCK_STREAM, 0);
// 	if(sd < 0){
// 		perror("Unable to create socket. \n");
// 		exit(0);
// 	}
	
// 	for(int i = 0; i < 30; i++)
// 		client_sockets[i] = 0;
	
// 	bzero(&serv, sizeof(serv));
	
// 	serv.sin_family = AF_INET;
// 	serv.sin_addr.s_addr = INADDR_ANY;
// 	serv.sin_port = htons(7229);
	
// 	if(bind(sd, (struct sockaddr*)&serv, sizeof(serv)) < 0)
// 		perror("Bind error\n");
// 	listen(sd, 5);
	
// 	int len = sizeof(cli);
// 	int cnt = 0;

// 	while(1){
// 		FD_ZERO(&clientfds);

// 		FD_SET(sd, &clientfds);
// 		int max_sd = sd;

// 		for(int i = 0;i < 30;i++){
// 			int scd = client_sockets[i];
// 			if(scd > 0)
// 				FD_SET(scd, &clientfds);
			
// 			if(scd > max_sd)
// 				max_sd = scd;
// 		}

// 		int action = select(max_sd + 1, &clientfds, NULL, NULL, NULL);
// 		if(action<0){
// 			perror("\nSelect error!\n");
// 		}
// 		if(FD_ISSET(sd, &clientfds)){
// 			new_fd = accept(sd, (struct sockaddr*) &cli, &len);
// 			if(new_fd<0)perror("Connection issue\n");
// 			cnt++;
// 			printf("Client no. %d --- %s : %d connected\n", cnt, inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
			
// 			printf("Number of active connections: %d\n", cnt);
// 			strcpy(buf, "Connection established");
// 			write(new_fd, buf, sizeof(buf));
			
// 			for(int i =0;i<30;i++){
// 				if(client_sockets[i] == 0){
// 					client_sockets[i] = new_fd;
// 					printf("\nConnection at socket %d\n", i);
// 					break;
// 				}
// 			}
// 		}
// 		for(int i = 0;i<30;i++){
// 			int csd = client_sockets[i];
// 			//Check for change in descriptors
// 			if(FD_ISSET(csd, &clientfds)){
// 				if(!fork()){
// 					// read(csd, buf, sizeof(buf));
// 					int role[2];
// 					bzero(role, sizeof(role));
// 					read(csd, role, sizeof(role));
// 					//Check end of connection
// 					if(role[0] == 0){
// 						strcpy(buf, "end");
// 						write(new_fd, buf, sizeof(buf));
// 						getpeername(csd, (struct sockaddr*)&cli,&len);
// 						printf("\nHost disconnected. Socket: %d.\n",client_sockets[i]);
// 						close(csd);
// 						client_sockets[i] = 0;
// 						cnt--;
// 					}
// 					else{
// 						bzero(buf, sizeof(buf));
// 						if(role[0] == -2){
// 							printf("Invalid Credentials!!!\n");
// 							strcpy(buf, "Invalid Credentials!!!\n");
// 							write(new_fd, buf, sizeof(buf));
// 						}
// 						else if(role[0] == -1){
// 							printf("That login ID does not exist for that role. \n");
// 							strcpy(buf, "That login ID does not exist for that role. \n");
// 							write(new_fd, buf, sizeof(buf));
// 						}
// 						else if(role[0] == 1){
// 							printf("Admin operation: ");
// 							if(role[1] == 1){
// 								printf("Add student\n");
// 							}
// 							else if(role[1] == 2){
// 								printf("View student\n");
// 							}
// 							else if(role[1] == 3){
// 								printf("Add faculty\n");
// 							}
// 							else if(role[1] == 4){
// 								printf("View faculty\n");
// 							}
// 							else if(role[1] == 5){
// 								printf("Block/Unblock student\n");
// 							}
// 							else if(role[1] == 6){
// 								printf("Update student\n");
// 							}
// 							else if(role[1] == 7){
// 								printf("Update faculty\n");
// 							}
// 							else if(role[1] == 8){
// 								printf("Delete student\n");
// 							}
// 							else if(role[1] == 9){
// 								printf("Delete faculty\n");
// 							}
// 							else if(role[1] == 0){
// 								printf("Exit\n");
// 							}
// 						}
// 						else if(role[0] == 2){
// 							printf("Faculty operation: ");
// 							if(role[1] == 1){
// 								printf("Add course\n");
// 							}
// 							else if(role[1] == 2){
// 								printf("View my offered courses\n");
// 							}
// 							else if(role[1] == 3){
// 								printf("View enrollments\n");
// 							}
// 							else if(role[1] == 4){
// 								printf("Modify offered course\n");
// 							}
// 							else if(role[1] == 5){
// 								printf("Remove course\n");
// 							}
// 							else if(role[1] == 6){
// 								printf("Password change\n");
// 							}
// 							else if(role[1] == 0){
// 								printf("Exit\n");
// 							}
// 						}
// 						else if(role[0] == 3){
// 							printf("Student operation: ");
// 							if(role[1] == 1){
// 								printf("Enroll in a course\n");
// 							}
// 							else if(role[1] == 2){
// 								printf("View my enrolled courses\n");
// 							}
// 							else if(role[1] == 3){
// 								printf("Unenroll from a course\n");
// 							}
// 							else if(role[1] == 4){
// 								printf("Password change\n");
// 							}
// 							else if(role[1] == 0){
// 								printf("Exit\n");
// 							}
// 						}
						

// 						// printf("\nMessage from Client %d: %s\n", client_sockets[i], buf);
						
// 						// bzero(buf, sizeof(buf));
// 						// //Write message in buffer
// 						// printf("\nEnter message: ");scanf(" %[^\n]", buf);
// 						// write(new_fd, buf, sizeof(buf));
// 					}
// 				}
// 			}
// 		}
// 	}
// 	return 0;
// }