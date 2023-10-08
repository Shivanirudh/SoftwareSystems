#include "faculty_ops.h"

void studentMenuDisplay(){
	printf("%s\n", delimiter2);
	printf("%27s%-43s\n", " ", "STUDENT DASHBOARD");
	printf("%s\n", delimiter2);
	printf("Choose option:\n");
	printf("1. Enroll in a course\n");
	printf("2. Unenroll from a course\n");            // View all courses offered by this faculty 
	printf("3. View enrolled courses\n");
	printf("4. Change Password\n");
	printf("0. Logout\n");
}

/*
	
*/

int checkEnrollment(Student s, Course c){
	int student_course_fd = open(student_course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Enrollment tmp;
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, 0, 0); //Acquire write lock
	int flag = 0;
	
	printf("Fetching details...\n"); //Critical section
	fcntl(student_course_fd, F_SETLKW, &read_lock);
	while(read(student_course_fd, &tmp, sizeof(tmp))){
		if(tmp.s.ID == s.ID && tmp.c.code == c.code && tmp.active){
			flag = 1; return flag;
		}
	}
	read_lock.l_type = F_UNLCK;
	fcntl(student_course_fd, F_SETLK, &read_lock);
	close(student_course_fd);
	return flag;
}

void makeEnrollmentEntry(Enrollment e){
	int student_course_fd = open(student_course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
	int flag = 0;
	
	Enrollment tmp;
	
	printf("Updating details...\n"); //Critical section
	fcntl(student_course_fd, F_SETLKW, &write_lock);
	//while(read(student_course_fd, &tmp, sizeof(tmp))){
	//	if(tmp.s.ID == e.s.ID && tmp.c.code == e.c.code) break;
	//}
	
	//lseek(student_course_fd, -1*sizeof(Enrollment), SEEK_CUR);
	lseek(student_course_fd, 0L, SEEK_END);
	write(student_course_fd, &e, sizeof(e));
	
	write_lock.l_type = F_UNLCK;
	fcntl(student_course_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_course_fd);
}

void updateCourse(int code, int value){
	int course_fd = open(course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Course tmp; 
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (code-1)*sizeof(Course), sizeof(Course)); //Acquire write lock
	
	printf("Updating course details...\n"); //Critical section
	fcntl(course_fd, F_SETLKW, &write_lock);
	
	lseek(course_fd, (code-1)*sizeof(Course), SEEK_SET);
	read(course_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Course with this code not found\n");
		return;
	}
	
	tmp.available_seat_count += value;
	
	lseek(course_fd, (-1)*sizeof(Course), SEEK_CUR);
	write(course_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(course_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(course_fd);
}

void enrollCourse(Student s){
	int course_fd = open(course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Course tmp;
	int num = 1;
	
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, 0, 0); //Acquire read lock
	
	printf("Fetching details...\n"); //Critical section
	fcntl(course_fd, F_SETLKW, &read_lock);
	while(read(course_fd, &tmp, sizeof(tmp))){
		if(tmp.active)
			printCourse(tmp);
	}
	
	int choice = -1; char opt = 'o';
	printf("Enter course code you want to enroll in: ");
	scanf("%d", &choice);
	
	lseek(course_fd, (choice-1)*sizeof(Course), SEEK_SET);
	read(course_fd, &tmp, sizeof(tmp));
	printCourse(tmp);
	printf("Do you want to enroll in the above course? (y/n)");scanf(" %c", &opt);
	if(opt == 'y' || opt == 'Y'){
		int flag = checkEnrollment(s, tmp);
		if(s.blocked == true){
			printf("You are blocked currently. Please contact the admin. \n");
		}
		else if(flag == 1){
			printf("You have already enrolled in this course. \n");
		}
		else if(tmp.available_seat_count <= 0){
			printf("No seats available for this course. \n");
		}
		else{
			Enrollment e = createEnrollment(s, tmp);
			tmp.available_seat_count -= 1;
			makeEnrollmentEntry(e);
			updateCourse(tmp.code, -1);
		}
	}
	
	read_lock.l_type = F_UNLCK;
	fcntl(course_fd, F_SETLK, &read_lock);
	close(course_fd);
	
}

void viewEnrolledCourses(Student s){
	int student_course_fd = open(student_course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Enrollment tmp;
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, 0, 0); //Acquire read lock
	
	printf("Fetching details...\n"); //Critical section
	fcntl(student_course_fd, F_SETLKW, &read_lock);
	while(read(student_course_fd, &tmp, sizeof(tmp))){
		if(tmp.active && tmp.s.ID == s.ID){
			printCourse(tmp.c);
		}
	}
	read_lock.l_type = F_UNLCK;
	fcntl(student_course_fd, F_SETLK, &read_lock);
	close(student_course_fd);
}

void unenrollCourse(Student s){
	viewEnrolledCourses(s);
	int student_course_fd = open(student_course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Enrollment tmp;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
	
	int choice = -1; char opt = 'o';
	printf("Enter course code you want to unenroll from: ");
	scanf("%d", &choice);
	
	Course co = getCourse(choice);
	if(co.active == false)
		printf("Course with this code not found\n");
	else
		printCourse(co);
	printf("Do you want to unenroll from the above course? (y/n)");scanf(" %c", &opt);
	if(opt == 'y' || opt == 'Y'){
		Course dummy; dummy.code = choice;
		int flag = checkEnrollment(s, dummy);
		if(s.blocked == true){
			printf("You are blocked currently. Please contact the admin. \n");
		}
		else if(flag == 0){
			printf("You have not enrolled in this course. \n");
		}
		else{
			printf("Fetching details...\n"); //Critical section
			fcntl(student_course_fd, F_SETLKW, &write_lock);
			while(read(student_course_fd, &tmp, sizeof(tmp))){
				if(tmp.s.ID == s.ID && tmp.c.code == choice) break;
			}
			
			lseek(student_course_fd, (-1)*sizeof(Enrollment), SEEK_CUR);
			tmp.active = false;
			write(student_course_fd, &tmp, sizeof(tmp));
			updateCourse(tmp.c.code, 1);
		}
	}
	write_lock.l_type = F_UNLCK;
	fcntl(student_course_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_course_fd);
}

void passwordChangeStudent(int ID, bool first_time){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student tmp;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &write_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Student with this ID not found\n");
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
	if(first_time) tmp.activated = true;
	
	lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
	write(student_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_fd);
}

void studentDriver(Student s){
	printf("Welcome %s\n\n", s.name);
	if(s.activated == false){
		printf("You are logging in for the first time. Please change password. \n");
		printf("\n\n");
		passwordChangeStudent(s.ID, true);
		printf("\n\n");
	}
	else{
		int opt = -1;
		while(1){
			int code;
			studentMenuDisplay();
			printf("Enter choice: ");scanf("%d", &opt);
			
			if(opt == 0) break;
			else if(opt == 1){
				printf("\n\n");
				enrollCourse(s);
				printf("\n\n");
			}
			else if(opt == 2){
				printf("\n\n");
				unenrollCourse(s);
				printf("\n\n");
			}
			else if(opt == 3){
				printf("\n\n");
				viewEnrolledCourses(s);
				printf("\n\n");
			}
			else if(opt == 4){
				printf("\n\n");
				passwordChangeStudent(s.ID, false);
				printf("\n\n");
			}
			else{
				printf("Invalid option\n");
			}
		}
	}
}

//Testing driver code
/*
int main(){
	int opt = -1;
	while(1){
		int code;
		studentMenuDisplay();
		printf("Enter choice: ");scanf("%d", &opt);
		
		Student s; s.ID = 1;
		
		if(opt == 0) break;
		else if(opt == 1){
			enrollCourse(s);
			printf("\n\n");
		}
		else if(opt == 2){
			unenrollCourse(s);
			printf("\n\n");
		}
		else if(opt == 3){
			viewEnrolledCourses(s);
			printf("\n\n");
		}
		else if(opt == 4){
			passwordChange(s.ID);
			printf("\n\n");
		}
		else{
			printf("Invalid option\n");
		}
	}
}
*/

