#include "student_ops.h"

struct param{
	int csd; 
	char buf[1024];
	struct sockaddr_in cli;
	int len;
	int cnt;
	int client_sockets[30];
	int new_fd;
	int ix;
};

void *update(void *parameters){

	struct param *params = (struct param *) parameters;
	//read(params->csd, params->buf, sizeof(params->buf));
	//printf("%s\n", buf);
	//Check end of connection
	if(strcmp(params->buf, "exit") == 0){
		getpeername(params->csd, (struct sockaddr*)&params->cli,&params->len);
		printf("\nClient disconnected. Socket: %d.\n",params->client_sockets[params->ix]);
		close(params->csd);
		params->client_sockets[params->ix] = 0;
		params->cnt--;
	}
	else{
		int role[2];
		read(params->new_fd, role, sizeof(role));
		printf("Server: %d %d\n", role[0], role[1]);
		
		if(role[0] == -2){
			printf("Invalid Credentials!!!\n");
			strcpy(params->buf, "Invalid Credentials!!!\n");
			write(params->new_fd, params->buf, sizeof(params->buf));
			//return;
		}
		else if(role[0] == -1){
			printf("That login ID does not exist for that role. \n");
			strcpy(params->buf, "That login ID does not exist for that role. \n");
			write(params->new_fd, params->buf, sizeof(params->buf));
			//return;
		}
		else if(role[0] == 0){
			strcpy(params->buf, "exit");
			write(params->new_fd, params->buf, sizeof(params->buf));
		}
		else if(role[0] == 1){
			strcpy(params->buf, "Logged in as Admin \n");
			write(params->new_fd, params->buf, sizeof(params->buf));
			Admin a = getAdmin();
			write(params->new_fd, (void*) &a, sizeof(a));
			
			// while(1){
				int opt = -1;
				read(params->new_fd, &opt, sizeof(int));
				if(opt == 0) ;
				else if(opt == 1){
					Student s;
					read(params->new_fd, (void*) &s, sizeof(s));
					int ID = updateStudentDB(s, 1);
					write(params->new_fd, &ID, sizeof(int));
				}
				else if(opt == 2){
					int ID;
					read(params->new_fd, &ID, sizeof(ID));
					Student s = getStudent(ID);
					write(params->new_fd, (void*) &s, sizeof(s));
				}
				else if(opt == 3){
					Faculty f;
					read(params->new_fd, (void*) &f, sizeof(f));
					int ID = updateFacultyDB(f, 1);
					write(params->new_fd, &ID, sizeof(int));
				}
				else if(opt == 4){
					int ID;
					read(params->new_fd, &ID, sizeof(ID));
					Faculty f = getFaculty(ID);
					write(params->new_fd, (void*) &f, sizeof(f));
				}
				else if(opt == 5){
					int ID;
					read(params->new_fd, &ID, sizeof(ID));
					Student s = getStudent(ID);
					write(params->new_fd, (void*) &s, sizeof(s));
					read(params->new_fd, (void*) &s, sizeof(s));
					updateStudentDB(s, 4);
				}
				else if(opt == 6){
					int ID;
					read(params->new_fd, &ID, sizeof(ID));
					Student s = getStudent(ID);
					write(params->new_fd, (void*) &s, sizeof(s));
					if(s.active == true){
						Student st;
						read(params->new_fd, (void*) &st, sizeof(st));
						updateStudentDB(st, 2);
					}
				}
				else if(opt == 7){
					int ID;
					read(params->new_fd, &ID, sizeof(ID));
					Faculty f = getFaculty(ID);
					write(params->new_fd, (void*) &f, sizeof(f));
					if(f.active == true){
						Faculty fa;
						read(params->new_fd, (void*) &fa, sizeof(fa));
						printFaculty(fa);
						updateFacultyDB(fa, 2);
					}
				}
				else if(opt == 8){
					int ID;
					read(params->new_fd, &ID, sizeof(ID));
					Student s = getStudent(ID);
					write(params->new_fd, (void*) &s, sizeof(s));
					if(s.active == true){
						updateStudentDB(s, 3);
					}
				}
				else if(opt == 9){
					int ID;
					read(params->new_fd, &ID, sizeof(ID));
					Faculty f = getFaculty(ID);
					write(params->new_fd, (void*) &f, sizeof(f));
					if(f.active == true){
						updateFacultyDB(f, 3);
					}
				}
			// }
		}
		else if(role[0] == 2){
			strcpy(params->buf, "Logged in as Faculty \n");
			write(params->new_fd, params->buf, sizeof(params->buf));
			Faculty f = getFaculty(role[1]);
			write(params->new_fd, (void*) &f, sizeof(f));
			if(f.activated == false){
				char newpass[1024];bool first_time;
				read(params->new_fd, newpass, sizeof(newpass));
				read(params->new_fd, &first_time, sizeof(first_time));
				strcpy(f.password, newpass);
				f.activated = true;
				updateFacultyDB(f, 4);
			}
			else{
				// while(1){
					int opt = -1;
					read(params->new_fd, &opt, sizeof(int));
					if(opt == 0);
					else if(opt == 1){
						Course c;char faculty_name[1024];
						read(params->new_fd, (void*) &c, sizeof(c));
						read(params->new_fd, faculty_name, sizeof(faculty_name));
						strcpy(c.faculty_name, faculty_name);
						int code = updateCourseDB(c, 1);
						write(params->new_fd, &code, sizeof(int));
					}
					else if(opt == 2){
						char faculty_name[1024];
						read(params->new_fd, faculty_name, sizeof(faculty_name));
						int count = numberEnrollments(faculty_name);
						Course *ans = viewEnrollments(faculty_name, count);
						write(params->new_fd, ans, sizeof(ans));
						write(params->new_fd, &count, sizeof(count));
					}
					else if(opt == 3){
						int code;
						read(params->new_fd, &code, sizeof(int));
						Course c = getCourse(code);
						write(params->new_fd, (void*) &c, sizeof(c));
						if(c.active == true){
							updateCourseDB(c, 2);
						}
					}
					else if(opt == 4){
						char newpass[1024];
						read(params->new_fd, newpass, sizeof(newpass));
						strcpy(f.password, newpass);
						updateFacultyDB(f, 4);
					}
				// }
			}
		}
		else if(role[0] == 3){
			strcpy(params->buf, "Logged in as Student \n");
			write(params->new_fd, params->buf, sizeof(params->buf));
			Student s = getStudent(role[1]);
			write(params->new_fd, (void*) &s, sizeof(s));
			//studentDriver(s);
		}
		
		printf("\nMessage from Client %d: %s\n", params->client_sockets[params->ix], params->buf);
		
		bzero(params->buf, sizeof(params->buf));
		/*//Write message in buffer
		printf("\nEnter message: ");scanf(" %[^\n]", params->buf);
		write(params->new_fd, params->buf, sizeof(params->buf));*/
	}
}

struct param param_array[100];
int t = 0;

int main(){
	struct sockaddr_in serv, cli;
	int client_sockets[30];
	fd_set clientfds;
	int new_fd;
	char buf[1024];
	
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("Unable to create socket. \n");
		exit(0);
	}
	
	for(int i = 0; i < 30; i++)
		client_sockets[i] = 0;
	
	bzero(&serv, sizeof(serv));
	
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(PORT_NO);
	
	
	
	if(bind(sd, (struct sockaddr*)&serv, sizeof(serv)) < 0){
		perror("Bind error\n");
		exit(1);
	}
	listen(sd, 10);
	printf("Server: Socket created. Listening....\n");
	
	int len = sizeof(cli);
	int cnt = 0;

	while(1){
		FD_ZERO(&clientfds);

		FD_SET(sd, &clientfds);
		int max_sd = sd;

		for(int i = 0;i < 30;i++){
			int scd = client_sockets[i];
			if(scd > 0)
				FD_SET(scd, &clientfds);
			
			if(scd > max_sd)
				max_sd = scd;
		}

		int action = select(max_sd + 1, &clientfds, NULL, NULL, NULL);
		if(action<0){
			perror("\nSelect error!\n");
		}
		if(FD_ISSET(sd, &clientfds)){
			new_fd = accept(sd, (struct sockaddr*) &cli, &len);
			if(new_fd<0)perror("Connection issue\n");
			cnt++;
			printf("Client no. %d --- %s : %d connected\n", cnt, inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
			
			//printf("Number of active connections: %d\n", cnt);
			//strcpy(buf, "Connection established");
			//write(new_fd, buf, sizeof(buf));
			
			for(int i =0;i<30;i++){
				if(client_sockets[i] == 0){
					client_sockets[i] = new_fd;
					printf("\nConnection at socket %d\n", i);
					break;
				}
			}
		}
		for(int i = 0;i<30;i++){
			int csd = client_sockets[i];
			//Check for change in descriptors
			if(FD_ISSET(csd, &clientfds)){
				t++;
				
				param_array[t].csd = csd;
				strcpy(param_array[t].buf, buf);
				param_array[t].cli = cli;
				param_array[t].len = len;
				for(int i=0;i<30;i++)
					param_array[t].client_sockets[i] = client_sockets[i];
				param_array[t].cnt = cnt;
				param_array[t].new_fd = new_fd;
				param_array[t].ix = i;
				
				pthread_t process;
				pthread_create(&process, NULL, update, &param_array[t]);
				
				pthread_join(process, NULL);
			}
		}
	}
	return 0;
}
