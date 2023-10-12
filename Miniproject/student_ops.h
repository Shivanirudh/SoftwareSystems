/*
Author: Shivanirudh S G
File description: All operations that can be carried out by a student
*/
#include "faculty_ops.h"

void studentMenuDisplay(){
	printf("%s\n", delimiter2);
	printf("%27s%-43s\n", " ", "STUDENT DASHBOARD");
	printf("%s\n", delimiter2);
	printf("Choose option:\n");
	printf("1. Enroll in a course\n");
	printf("2. View enrolled courses\n");            // View all courses offered by this faculty 
	printf("3. Unenroll from a course\n");
	printf("4. Change Password\n");
	printf("0. Logout\n");
}

/*
	
*/

void viewAllCourses(){
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
	read_lock.l_type = F_UNLCK;
	fcntl(course_fd, F_SETLK, &read_lock);
	close(course_fd);
}

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
	
	lseek(student_course_fd, 0L, SEEK_END);
	write(student_course_fd, &e, sizeof(e));
	
	write_lock.l_type = F_UNLCK;
	fcntl(student_course_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_course_fd);
}

int enrollCourse(Student s, Course c){
	int flag = checkEnrollment(s, c);
	int ret_val = 0;
	if(s.blocked == true){
		printf("You are blocked currently. Please contact the admin. \n");
		ret_val = -3;
	}
	else if(flag == 1){
		printf("You have already enrolled in this course. \n");
		ret_val = -2;
	}
	else if(c.available_seat_count <= 0){
		printf("No seats available for this course. \n");
		ret_val = -1;
	}
	else{
		c.available_seat_count--;
		updateCourse(c.code, -1);
		studentCourseCount(s.ID, 1);
		Enrollment e = createEnrollment(s, c);
		makeEnrollmentEntry(e);
	}
	return ret_val;
}

int countEnrolledCourses(Student s){
	int student_course_fd = open(student_course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Enrollment tmp;
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, 0, 0); //Acquire read lock
	int count = 0;
	
	printf("Fetching details...\n"); //Critical section
	fcntl(student_course_fd, F_SETLKW, &read_lock);
	while(read(student_course_fd, &tmp, sizeof(tmp))){
		if(tmp.active && tmp.s.ID == s.ID){
			count++;
		}
	}
	read_lock.l_type = F_UNLCK;
	fcntl(student_course_fd, F_SETLK, &read_lock);
	close(student_course_fd);
	return count;
}

int* viewEnrolledCourses(Student s){
	int student_course_fd = open(student_course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Enrollment tmp;
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, 0, 0); //Acquire read lock
	int *courses = calloc(COURSE_LIMIT, sizeof(int));int ix = 0;

	printf("Fetching details...\n"); //Critical section
	fcntl(student_course_fd, F_SETLKW, &read_lock);
	while(read(student_course_fd, &tmp, sizeof(tmp))){
		if(tmp.active && tmp.s.ID == s.ID){
			printCourse(tmp.c);
			courses[ix++] = tmp.c.code;
		}
	}
	read_lock.l_type = F_UNLCK;
	fcntl(student_course_fd, F_SETLK, &read_lock);
	close(student_course_fd);
	return courses;
}

int unenrollCourse(Student s, Course c){
	int ret_val = 0;
	int student_course_fd = open(student_course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Enrollment tmp;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
		int flag = checkEnrollment(s, c);
	if(s.blocked == true){
		printf("You are blocked currently. Please contact the admin. \n");
		ret_val = -2;
	}
	else if(flag == 0){
		printf("You have not enrolled in this course. \n");
		ret_val = -1;
	}
	else{
		printf("Fetching details...\n"); //Critical section
		fcntl(student_course_fd, F_SETLKW, &write_lock);
		while(read(student_course_fd, &tmp, sizeof(tmp))){
			if(tmp.s.ID == s.ID && tmp.c.code == c.code) break;
		}
		
		lseek(student_course_fd, (-1)*sizeof(Enrollment), SEEK_CUR);
		tmp.active = false;
		write(student_course_fd, &tmp, sizeof(tmp));
		updateCourse(tmp.c.code, 1);
		studentCourseCount(s.ID, -1);
	}
	
	write_lock.l_type = F_UNLCK;
	fcntl(student_course_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_course_fd);
	return ret_val;
}

void passwordChangeStudent(int ID, bool first_time, char password[1024]){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student tmp;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &write_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	
	strcpy(tmp.password, password);
	if(first_time) tmp.activated = true;
	
	lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
	write(student_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_fd);
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

