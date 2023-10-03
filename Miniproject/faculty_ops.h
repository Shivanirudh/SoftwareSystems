#include "admin_ops.h"

void facultyMenuDisplay(){
	printf("%s\n", delimiter2);
	printf("%27s%-43s\n", " ", "FACULTY DASHBOARD");
	printf("%s\n", delimiter2);
	printf("Choose option:\n");
	printf("1. Add new Course\n");
	printf("2. View Enrollments\n");            // View all courses offered by this faculty 
	printf("3. Remove Course\n");
	printf("4. Change Password\n");
	printf("0. Logout\n");
}

/*
	
*/

void addCourse(char name[50]){
	int course_fd = open(course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Course c = inputCourse();
	Course tmp;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
	
	printf("Saving details...\n"); //Critical section
	fcntl(course_fd, F_SETLKW, &write_lock);
	
	struct stat file_details;
	stat(course_file, &file_details);
	if(file_details.st_size > 1){
		lseek(course_fd, -1*sizeof(c), SEEK_END);
		read(course_fd, &tmp, sizeof(tmp));
		num = tmp.code;
		num++;
	}
	else
		num = 1;
	c.code = num;
	strcpy(c.faculty_name, name); 
	
	write(course_fd, &c, sizeof(c));
	write_lock.l_type = F_UNLCK;
	fcntl(course_fd, F_SETLK, &write_lock);
	printf("Saved successfully\n");
	close(course_fd);
}

void viewEnrollments(char name[50]){
	int course_fd = open(course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Course tmp;
	int num = 1;
	
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, 0, 0); //Acquire write lock
	
	printf("Fetching details...\n"); //Critical section
	fcntl(course_fd, F_SETLKW, &read_lock);
	while(read(course_fd, &tmp, sizeof(tmp))){
		if(tmp.active && strcmp(name, tmp.faculty_name) == 0)
			printCourse(tmp);
	}
	read_lock.l_type = F_UNLCK;
	fcntl(course_fd, F_SETLK, &read_lock);
	close(course_fd);
}

void removeCourse(int code){
	int course_fd = open(course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Course tmp; 
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (code-1)*sizeof(Course), sizeof(Course)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(course_fd, F_SETLKW, &write_lock);
	
	lseek(course_fd, (code-1)*sizeof(Course), SEEK_SET);
	read(course_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Course with this code not found\n");
		return;
	}
	
	tmp.active = false;
	
	lseek(course_fd, (-1)*sizeof(Course), SEEK_CUR);
	write(course_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(course_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(course_fd);
}

void passwordChange(int ID){
	int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Faculty tmp;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (ID-1)*sizeof(Faculty), sizeof(Faculty)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(faculty_fd, F_SETLKW, &write_lock);
	
	lseek(faculty_fd, (ID-1)*sizeof(Faculty), SEEK_SET);
	read(faculty_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Faculty with this ID not found\n");
		return;
	}
	
	char buf[1024], rebuf[1024];
	do{
		printf("Enter new password: ");scanf(" %[^\n]", buf);
		printf("Re-enter new password: ");scanf(" %[^\n]", rebuf);
		if(strcmp(buf, rebuf) != 0){
			printf("Passwords are not matching. Please re-enter: \n\n");
		}
	}while(strcmp(buf, rebuf) != 0);
	
	strcpy(tmp.password, buf);
	
	lseek(faculty_fd, (-1)*sizeof(Faculty), SEEK_CUR);
	write(faculty_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(faculty_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(faculty_fd);
}

void facultyDriver(Faculty f){
	int opt = -1;
	while(1){
		int code;
		facultyMenuDisplay();
		printf("Enter choice: ");scanf("%d", &opt);
		
		if(opt == 0) break;
		else if(opt == 1){
			addCourse(f.name);
			printf("\n\n");
		}
		else if(opt == 2){
			viewEnrollments(f.name);
			printf("\n\n");
		}
		else if(opt == 3){
			printf("Enter code: ");scanf("%d", &code);
			removeCourse(code);
			printf("\n\n");
		}
		else if(opt == 4){
			passwordChange(1);
			printf("\n\n");
		}
		else{
			printf("Invalid option\n");
		}
	}
}

//Testing driver code

int main(){
	int opt = -1;
	while(1){
		int code;
		facultyMenuDisplay();
		printf("Enter choice: ");scanf("%d", &opt);
		
		char name[50];strcpy(name, "BTR");
		
		if(opt == 0) break;
		else if(opt == 1){
			addCourse(name);
			printf("\n\n");
		}
		else if(opt == 2){
			viewEnrollments(name);
			printf("\n\n");
		}
		else if(opt == 3){
			printf("Enter code: ");scanf("%d", &code);
			removeCourse(code);
			printf("\n\n");
		}
		else if(opt == 4){
			passwordChange(1);
			printf("\n\n");
		}
		else{
			printf("Invalid option\n");
		}
	}
}

