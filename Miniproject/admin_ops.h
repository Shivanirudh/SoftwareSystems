#include "utils.h"

void adminMenuDisplay(){
	printf("%s\n", delimiter2);
	printf("%27s%-43s\n", " ", "ADMIN DASHBOARD");
	printf("%s\n", delimiter2);
	printf("Choose option:\n");
	printf("1. Add Student\n");
	printf("2. Add Faculty\n");
	printf("3. Activate/Deactivate/Block Student\n");
	printf("4. Activate/Deactivate Faculty\n");
	printf("5. Update Student details\n");
	printf("6. Update Faculty details\n");
	printf("0. Logout\n");
}

void addStudent(){
	Student s = inputStudent();
	Student tmp = NULL;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
	
	printf("Saving details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &write_lock);
	
	read(student_fd, &tmp, sizeof(tmp));
	if(tmp != NULL){
		lseek(student_fd, -1*sizeof(s), SEEK_END);
		read(student_fd, &tmp, sizeof(tmp));
		num = atoi(tmp.ID);
	}
	else
		num = 1;
	s.ID = num;
	
	write(student_fd, s, sizeof(s));
	write_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &write_lock);
	printf("Saved successfully\n");
	printf("Student's Login ID: MT%d\n", s.ID);
}

void addFaculty(){
	Faculty f = inputFaculty();
	Faculty tmp = NULL;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
	
	printf("Saving details...\n"); //Critical section
	fcntl(faculty_fd, F_SETLKW, &write_lock);
	
	read(faculty_fd, &tmp, sizeof(tmp));
	if(tmp != NULL){
		lseek(faculty_fd, -1*sizeof(f), SEEK_END);
		read(faculty_fd, &tmp, sizeof(tmp));
		num = atoi(tmp.ID);
		num++;
	}
	else
		num = 1;
	f.ID = num;
	
	write(faculty_fd, f, sizeof(f));
	write_lock.l_type = F_UNLCK;
	fcntl(faculty_fd, F_SETLK, &write_lock);
	printf("Saved successfully\n");
	printf("Faculty Login ID: PROF%d\n", f.ID);
}

void updateStudent(int ID){
	Student tmp = NULL; tmp.ID = 0;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &write_lock);
	
	while(tmp.ID != ID){
		read(student_fd, &tmp, sizeof(tmp));
	}
	if(tmp.ID != ID){
		printf("Student with this ID not found");
		return;
	}
	Student s = inputStudent();
	lseek(student_fd, -1*sizeof(s), SEEK_CUR);
	s.password = tmp.password;
	s.activated = tmp.activated;
	s.blocked = tmp.blocked; 
	
	write(student_fd, s, sizeof(s));
	write_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	printf("Student's Login ID: MT%d\n", s.ID);
}

void addFaculty(int ID){
	Faculty tmp = NULL;tmp.ID = 0;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(faculty_fd, F_SETLKW, &write_lock);
	
	while(tmp.ID != ID){
		read(faculty_id, &tmp, sizeof(tmp));
	}
	if(tmp.ID != ID){
		printf("Faculty with this ID not found");
		return;
	}
	Faculty f = inputFaculty();
	lseek(faculty_fd, -1*sizeof(f), SEEK_CUR);
	f.password = tmp.password;
	f.activated = tmp.activated;
	
	write(faculty_fd, f, sizeof(f));
	write_lock.l_type = F_UNLCK;
	fcntl(faculty_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	printf("Faculty Login ID: PROF%d\n", f.ID);
}
