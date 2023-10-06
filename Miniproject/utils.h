#include "structures.h"

struct flock getLock(short l_type, short l_whence, off_t l_start, off_t l_len){
	struct flock mylock;
	mylock.l_type = l_type;
	mylock.l_whence = l_whence;
	mylock.l_start = l_start;
	mylock.l_len = l_len;
	mylock.l_pid = getpid();
	return mylock;
}

ssize_t my_getpass (char **lineptr, size_t *n, FILE *stream)
{
  struct termios old, new;
  int nread;

  /* Turn echoing off and fail if we can’t. */
  if (tcgetattr (fileno (stream), &old) != 0)
    return -1;
  new = old;
  new.c_lflag &= ~ECHO;
  if (tcsetattr (fileno (stream), TCSAFLUSH, &new) != 0)
    return -1;

  /* Read the passphrase */
  nread = getline (lineptr, n, stream);

  /* Restore terminal. */
  (void) tcsetattr (fileno (stream), TCSAFLUSH, &old);

  return nread;
}

Admin getAdmin(){
	int admin_fd = open(admin_file, O_RDWR|O_CREAT, S_IRWXU);
	Admin a;
	read(admin_fd, &a, sizeof(a));
	close(admin_fd);
	return a;
}


Student getStudent(int ID){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student tmp;
	
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, (ID-1)*sizeof(Student), sizeof(Student)); //Acquire read lock
	
	printf("Fetching Student details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &read_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Student with this ID not found\n");
		return tmp;
	}
	read_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &read_lock);
	printf("Changes saved successfully\n");
	close(student_fd);
	return tmp;
}

Faculty getFaculty(int ID){
	int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Faculty tmp;
	
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, (ID-1)*sizeof(Faculty), sizeof(Faculty)); //Acquire read lock
	
	printf("Fetching Faculty details...\n"); //Critical section
	fcntl(faculty_fd, F_SETLKW, &read_lock);
	
	lseek(faculty_fd, (ID-1)*sizeof(Faculty), SEEK_SET);
	read(faculty_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Faculty with this ID not found\n");
		return tmp;
	}
	read_lock.l_type = F_UNLCK;
	fcntl(faculty_fd, F_SETLK, &read_lock);
	printf("Changes saved successfully\n");
	close(faculty_fd);
	return tmp;
}

Course getCourse(int code){
	int course_fd = open(course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Course tmp;
	
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, (code-1)*sizeof(Course), sizeof(Course)); //Acquire read lock
	
	printf("Fetching Course details...\n"); //Critical section
	fcntl(course_fd, F_SETLKW, &read_lock);
	
	lseek(course_fd, (code-1)*sizeof(Course), SEEK_SET);
	read(course_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Course with this code not found\n");
		return tmp;
	}
	read_lock.l_type = F_UNLCK;
	fcntl(course_fd, F_SETLK, &read_lock);
	printf("Changes saved successfully\n");
	close(course_fd);
	return tmp;
}

char* mystrncpy(char login[20], int offset){
	char *s = calloc(20, sizeof(char));
	for(int i=offset;login[i];i++)
		s[i-offset] = login[i];
	return s;
}

int checkUser(int role, char login[20]){
	printf("Check: %d %s\n", role, login);
	if(role == 1){
		int admin_fd = open(admin_file, O_RDWR|O_CREAT, S_IRWXU);
		Admin a;
		read(admin_fd, &a, sizeof(a));
		if(strcmp(login, a.ID) == 0){
			close(admin_fd);
			return role;
		}
		close(admin_fd);
	}
	else if(role == 2){
		char *s = mystrncpy(login, 4); 
		int loginID = atoi(s);
		printf("LoginID: %d\n", loginID);
		int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		Faculty tmp;
		while(read(faculty_fd, &tmp, sizeof(tmp)))
			if(loginID == tmp.ID && tmp.active){
				close(faculty_fd);
				return role;
			}
		close(faculty_fd);
	}
	else{
		char *s = mystrncpy(login, 2); 
		int loginID = atoi(s);
		printf("LoginID: %d\n", loginID);
		int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		Student tmp;
		while(read(student_fd, &tmp, sizeof(tmp)))
			if(loginID == tmp.ID && tmp.active){
				close(student_fd);
				return role;
			}
		close(student_fd);
	}
	return -1;
}

int* validateCreds(int role, char login[20], char password[30]){
	printf("Validate: %d %s %s\n", role, login, password);
	if(role == 1){
		printf("admin\n");
		int admin_fd = open(admin_file, O_RDWR|O_CREAT, S_IRWXU);
		Admin a;
		read(admin_fd, &a, sizeof(a));
		if(strcmp(login, a.ID) == 0 && strcmp(password, a.password) == 0){
			close(admin_fd);
			int *arr = calloc(2, sizeof(int));
			arr[0] = role; arr[1] = 0;
			return arr;
		}
		close(admin_fd);
	}
	else if(role == 2){
		printf("faculty\n");
		char *s = mystrncpy(login, 4); 
		int loginID = atoi(s);
		
		int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		Faculty tmp;
		while(read(faculty_fd, &tmp, sizeof(tmp))){
			printf("Faculty password: %s %s \n", password, tmp.password);
			if(loginID == tmp.ID && strcmp(password, tmp.password) == 0 && tmp.active){
				close(faculty_fd);
				int *arr = calloc(2, sizeof(int));
				arr[0] = role; arr[1] = loginID;
				return arr;	
			}
		}
		close(faculty_fd);
	}
	else{
		printf("student\n");
		char *s = mystrncpy(login, 2); 
		int loginID = atoi(s);
		
		int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		Student tmp;
		while(read(student_fd, &tmp, sizeof(tmp)))
			if(loginID == tmp.ID && strcmp(password, tmp.password) == 0 && tmp.active){
				close(student_fd);
				int *arr = calloc(2, sizeof(int));
				arr[0] = role; arr[1] = loginID;
				return arr;	
			}
		close(student_fd);
	}
	int *arr = calloc(2, sizeof(int));
	arr[0] = -2; arr[1] = 0;
	return arr;
}

int* mainMenu(){
	//clrscr();
	int role;
	printf("%s\n", delimiter2);
	printf("%s\n", delimiter2);
	printf("%22s%-48s\n", " ", "Welcome to Academia Portal");
	printf("%s\n", delimiter2);
	printf("%s\n", delimiter2);
	printf("Choose role to login as: \n");
	printf("1. Admin \n");
	printf("2. Faculty \n");
	printf("3. Student \n");
	printf("0. Exit \n");
	do{
		printf("Enter choice: "); scanf("%d", &role);
		if(role != 0 && role != 1 && role != 2 && role != 3) printf("Invalid choice. \n");
	}while(role != 0 && role != 1 && role != 2 && role != 3);
	
	if(role == 0){ 
		int *arr = calloc(2, sizeof(int));
		arr[0] = 0; arr[1] = 0;
		return arr;
	}
	
	char login[20], password[30];
	printf("Enter login ID: "); scanf(" %[^\n]", login);
	int flag = checkUser(role, login);
	printf("Flag %d\n", flag);
	if(flag == -1){
		int *arr = calloc(2, sizeof(int));
		arr[0] = flag; arr[1] = 0;
		return arr;
	}
	/*
	printf("Enter password: "); 
	char ch = 'a';int ix = 0;
	while(1){
		ch = getch();
		if(ch == '\n') break;
		else if(ch == '\b'){
			ix--;
			if(ix<0)ix = 0;
		}
		else{
			password[ix++] = ch;
		}
	}
	password[ix] = '\0';*/
	
	/*
	noecho();
	scanf(" %[^\n]", password);
	echo();*/
	char *passwod = getpass("Enter password: ");
	
	return validateCreds(role, login, passwod);
}
/*
int checkStudentDB(int ID){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, (ID-1)*sizeof(Student), sizeof(Student)); //Acquire read lock
	printf("Fetching details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &read_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	int flag = 1;
	if(tmp.active == false){
		printf("Student with this ID not found\n");
		close(student_fd);
		flag = 0;
	}
	read_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &read_lock);
	return flag;
}*/

void updateStudentDB(Student s, int action){
	//Add
	if(action == 1){
		printf("Add student\n");
		int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		Student tmp;
		int num = 1;
		
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
		
		printf("Saving details...\n"); //Critical section
		fcntl(student_fd, F_SETLKW, &write_lock);
		
		struct stat file_details;
		stat(student_file, &file_details);
		if(file_details.st_size > 1){
			lseek(student_fd, -1*sizeof(s), SEEK_END);
			read(student_fd, &tmp, sizeof(tmp));
			num = tmp.ID;
			num++;
		}
		else
			num = 1;
		s.ID = num;
		sprintf(s.password, "MT%d", s.ID);
		
		write(student_fd, &s, sizeof(s));
		write_lock.l_type = F_UNLCK;
		fcntl(student_fd, F_SETLK, &write_lock);
		printf("Saved successfully\n");
	}
	// Update
	else if(action == 2){
		printf("Update student\n");
		Student tmp;
		int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (s.ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
		printf("Fetching details...\n"); //Critical section
		fcntl(student_fd, F_SETLKW, &write_lock);
		
		lseek(student_fd, (s.ID-1)*sizeof(Student), SEEK_SET);
		read(student_fd, &tmp, sizeof(tmp));
		strcpy(s.password, tmp.password);
		s.activated = tmp.activated;
		s.blocked = tmp.blocked; 
		s.active = tmp.active;
		lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
		
		write(student_fd, &s, sizeof(s));
		write_lock.l_type = F_UNLCK;
		fcntl(student_fd, F_SETLK, &write_lock);
		printf("Saved successfully\n");
	}
	//Delete
	else if(action == 3){
		printf("Delete student\n");
		Student tmp;
		int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (s.ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
		printf("Fetching details...\n"); //Critical section
		fcntl(student_fd, F_SETLKW, &write_lock);
		
		lseek(student_fd, (s.ID-1)*sizeof(Student), SEEK_SET);
		read(student_fd, &tmp, sizeof(tmp));
		tmp.active = false;
		lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
		
		write(student_fd, &tmp, sizeof(tmp));
		write_lock.l_type = F_UNLCK;
		fcntl(student_fd, F_SETLK, &write_lock);
		printf("Saved successfully\n");
	}
}

void updateFacultyDB(Faculty f, int action){
	//Add
	if(action == 1){
		printf("Add faculty\n");
		
		int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		Faculty tmp;
		int num = 1;
		
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
		
		printf("Saving details...\n"); //Critical section
		fcntl(faculty_fd, F_SETLKW, &write_lock);
		
		struct stat file_details;
		stat(faculty_file, &file_details);
		if(file_details.st_size > 1){
			lseek(faculty_fd, -1*sizeof(f), SEEK_END);
			read(faculty_fd, &tmp, sizeof(tmp));
			num = tmp.ID;
			num++;
		}
		else
			num = 1;
		f.ID = num;
		sprintf(f.password, "PROF%d", f.ID);
		
		write(faculty_fd, &f, sizeof(f));
		write_lock.l_type = F_UNLCK;
		fcntl(faculty_fd, F_SETLK, &write_lock);
		
		printf("Saved successfully\n");
	}
	// Update
	else if(action == 2){
		printf("Update Faculty\n");
		Faculty tmp;
		int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
		
		printf("Saving details...\n"); //Critical section
		fcntl(faculty_fd, F_SETLKW, &write_lock);
		
		lseek(faculty_fd, (f.ID-1)*sizeof(Faculty), SEEK_SET);
		read(faculty_fd, &tmp, sizeof(tmp));
		
		strcpy(f.password, tmp.password);
		f.activated = tmp.activated;
		f.active = tmp.active;
		
		lseek(faculty_fd, (-1)*sizeof(Faculty), SEEK_CUR);
		write(faculty_fd, &f, sizeof(f));
		
		write_lock.l_type = F_UNLCK;
		fcntl(faculty_fd, F_SETLK, &write_lock);
		printf("Saved successfully\n");
	}
	//Delete
	else if(action == 3){
		printf("Delete faculty\n");
		
		Faculty tmp;
		int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
		
		printf("Saving details...\n"); //Critical section
		fcntl(faculty_fd, F_SETLKW, &write_lock);
		
		lseek(faculty_fd, (f.ID-1)*sizeof(Faculty), SEEK_SET);
		read(faculty_fd, &tmp, sizeof(tmp));
		
		tmp.active = false;
		
		lseek(faculty_fd, (-1)*sizeof(Faculty), SEEK_CUR);
		write(faculty_fd, &f, sizeof(f));
		
		write_lock.l_type = F_UNLCK;
		fcntl(faculty_fd, F_SETLK, &write_lock);
		printf("Saved successfully\n");
	}
}
