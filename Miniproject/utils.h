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
	if(role == 1){
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
		char *s = mystrncpy(login, 4); 
		int loginID = atoi(s);
		
		int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		Faculty tmp;
		while(read(faculty_fd, &tmp, sizeof(tmp)))
			if(loginID == tmp.ID && strcmp(password, tmp.password) == 0 && tmp.active){
				close(faculty_fd);
				int *arr = calloc(2, sizeof(int));
				arr[0] = role; arr[1] = loginID;
				return arr;	
			}
		close(faculty_fd);
	}
	else{
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
		if(role != 0 || role != 1 || role != 2 || role != 3) printf("Invalid choice. \n");
	}while(role != 0 || role != 1 || role != 2 || role != 3);
	
	if(role == 0){ 
		int *arr = calloc(2, sizeof(int));
		arr[0] = 0; arr[1] = 0;
		return arr;
	}
	
	char login[20], password[30];
	printf("Enter login ID: "); scanf(" %[^\n]", login);
	int flag = checkUser(role, login);
	if(flag == -1){
		int *arr = calloc(2, sizeof(int));
		arr[0] = flag; arr[1] = 0;
		return arr;
	}
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
	password[ix] = '\0';
	
	return validateCreds(role, login, password);
}
