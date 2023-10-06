#include "utils.h"

void adminMenuDisplay(){
	printf("%s\n", delimiter2);
	printf("%28s%-42s\n", " ", "ADMIN DASHBOARD");
	printf("%s\n", delimiter2);
	printf("Choose option:\n");
	printf("1. Add Student\n");
	printf("2. View Student\n");            // View student with ID
	printf("3. Add Faculty\n");
	printf("4. View Faculty\n");            // View faculty with ID
	printf("5. Block/Unblock Student\n");   //Blocking can be reverted.
	printf("6. Update Student details\n");
	printf("7. Update Faculty details\n");
	printf("8. Delete Student\n");          //Deletion is permanent.
	printf("9. Delete Faculty\n");          //Deletion is permanent.
	printf("0. Logout\n");
}

/*
	Most functions in this file work on a full DB basis, including addition and deletion of records. For that reason all the locks acquired for this perform mandatory locking. 
	The exception to this are the updation and view functionalities, which perform advisory locking on just the record that is to be updated/viewed. 
*/
/*
void addStudent(){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student s = inputStudent();
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
	printf("Student's Login ID: MT%d\n", s.ID);
	printf("One time Password: MT%d\n", s.ID);
	printf("Note: Student will be forced to change password on the first login. \n");
	close(student_fd);
}*/

Student addStudent(){
	Student s = inputStudent();
	printf("Saved successfully\n");
	printf("Student's Login ID: MT%d\n", s.ID);
	printf("One time Password: MT%d\n", s.ID);
	printf("Note: Student will be forced to change password on the first login. \n");
	return s;
}
/*
void addFaculty(){
	int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Faculty f = inputFaculty();
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
	printf("Faculty Login ID: PROF%d\n", f.ID);
	printf("One time Password: PROF%d\n", f.ID);
	printf("Note: Faculty will be forced to change password on the first login. \n");
	close(faculty_fd);
}*/
Faculty addFaculty(){
	Faculty f = inputFaculty();
	printf("Saved successfully\n");
	printf("Faculty Login ID: PROF%d\n", f.ID);
	printf("One time Password: PROF%d\n", f.ID);
	printf("Note: Faculty will be forced to change password on the first login. \n");
	return f;
}

/*void updateStudent(int ID){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student tmp; tmp.ID = 0;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &write_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Student with this ID not found\n");
		return;
	}
	
	Student s = inputStudent();
	
	strcpy(s.password, tmp.password);
	s.activated = tmp.activated;
	s.blocked = tmp.blocked; 
	s.active = tmp.active;
	
	lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
	write(student_fd, &s, sizeof(s));
	write_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_fd);
}*/

Student updateStudent(int ID){
	Student s = inputStudent();
	printf("Saved successfully\n");
	return s;
}
/*
void updateFaculty(int ID){
	int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Faculty tmp;tmp.ID = 0;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (ID-1)*sizeof(Faculty), sizeof(Faculty)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(faculty_fd, F_SETLKW, &write_lock);
	
	lseek(faculty_fd, (ID-1)*sizeof(Faculty), SEEK_SET);
	read(faculty_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Faculty with this ID not found\n");
		return;
	}
	
	Faculty f = inputFaculty();
	
	strcpy(f.password, tmp.password);
	f.activated = tmp.activated;
	f.active = tmp.active;
	
	lseek(faculty_fd, (-1)*sizeof(Faculty), SEEK_CUR);
	write(faculty_fd, &f, sizeof(f));
	write_lock.l_type = F_UNLCK;
	fcntl(faculty_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(faculty_fd);
}*/
Faculty updateFaculty(int ID){
	Faculty f = inputFaculty();
	printf("Saved successfully\n");
	return f;
}

/*
void viewStudent(int ID){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student tmp; tmp.ID = 0;
	int num = 1;
	
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, (ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
	
	printf("Fetching details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &read_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Student with this ID not found\n");
		return;
	}
	printStudent(tmp);
	read_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &read_lock);
	close(student_fd);
}*/

void viewFaculty(int ID){
	int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Faculty tmp;tmp.ID = 0;
	int num = 1;
	
	struct flock read_lock = getLock(F_RDLCK, SEEK_SET, (ID-1)*sizeof(Faculty), sizeof(Faculty)); //Acquire write lock
	
	printf("Fetching details...\n"); //Critical section
	fcntl(faculty_fd, F_SETLKW, &read_lock);
	
	lseek(faculty_fd, (ID-1)*sizeof(Faculty), SEEK_SET);
	read(faculty_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Faculty with this ID not found\n");
		return;
	}
	printFaculty(tmp);
	read_lock.l_type = F_UNLCK;
	fcntl(faculty_fd, F_SETLK, &read_lock);
	close(faculty_fd);
}
/*
void blockStudent(int ID){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student tmp; tmp.ID = 0;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
	
	printf("Fetching details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &write_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Student with this ID not found\n");
		return;
	}
	
	if(tmp.blocked == false){
		char opt;
		printf("Student status is UNBLOCKED. Do you wish to BLOCK? (y/n): ");scanf(" %c", &opt);
		if(opt == 'y' || opt == 'Y'){
			tmp.blocked = true;
			printf("Student is BLOCKED.\n");
		}
		else{
			printf("Student is UNBLOCKED.\n");
		}
	}
	else{
		char opt;
		printf("Student status is BLOCKED. Do you wish to UNBLOCK? (y/n): ");scanf(" %c", &opt);
		if(opt == 'y' || opt == 'Y'){
			tmp.blocked = false;
			printf("Student is UNBLOCKED.\n");
		}
		else{
			printf("Student is BLOCKED.\n");
		}
	}
	printf("Updating details...\n");
	lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
	write(student_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_fd);
}*/
/*
void deleteStudent(int ID){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student tmp; tmp.ID = 0;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &write_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Student with this ID not found\n");
		return;
	}
	
	tmp.active = false;
	
	lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
	write(student_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_fd);
}*/

void deleteStudent(int ID){
	int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Student tmp; tmp.ID = 0;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(student_fd, F_SETLKW, &write_lock);
	
	lseek(student_fd, (ID-1)*sizeof(Student), SEEK_SET);
	read(student_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Student with this ID not found\n");
		return;
	}
	
	tmp.active = false;
	
	lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
	write(student_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(student_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(student_fd);
}

void deleteFaculty(int ID){
	int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	Faculty tmp;tmp.ID = 0;
	int num = 1;
	
	struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (ID-1)*sizeof(Faculty), sizeof(Faculty)); //Acquire write lock
	
	printf("Fetching updated details...\n"); //Critical section
	fcntl(faculty_fd, F_SETLKW, &write_lock);
	
	lseek(faculty_fd, (ID-1)*sizeof(Faculty), SEEK_SET);
	read(faculty_fd, &tmp, sizeof(tmp));
	if(tmp.active == false){
		printf("Faculty with this ID not found\n");
		return;
	}
	
	tmp.active = false;
	
	lseek(faculty_fd, (-1)*sizeof(Faculty), SEEK_CUR);
	write(faculty_fd, &tmp, sizeof(tmp));
	write_lock.l_type = F_UNLCK;
	fcntl(faculty_fd, F_SETLK, &write_lock);
	printf("Changes saved successfully\n");
	close(faculty_fd);
}
/*
void adminDriver(Admin a){
	printf("Welcome Mr. %s\n\n", "Vishnu Raj");
	int opt = -1;
	while(1){
		int ID;
		adminMenuDisplay();
		printf("Enter choice: ");scanf("%d", &opt);
		
		if(opt == 0) break;
		else if(opt == 1){
			printf("\n\n");
			Student s = addStudent();
			printf("\n\n");
		}
		else if(opt == 2){
			printf("Enter ID: ");scanf("%d", &ID);
			printf("\n\n");
			viewStudent(ID);
			printf("\n\n");
		}
		else if(opt == 3){
			addFaculty();
			printf("\n\n");
		}
		else if(opt == 4){
			printf("Enter ID: ");scanf("%d", &ID);
			printf("\n\n");
			viewFaculty(ID);
			printf("\n\n");
		}
		else if(opt == 5){
			printf("Enter ID: ");scanf("%d", &ID);
			printf("\n\n");
			blockStudent(ID);
			printf("\n\n");
		}
		else if(opt == 6){
			printf("Enter ID: ");scanf("%d", &ID);
			printf("\n\n");
			updateStudent(ID);
			printf("\n\n");
		}
		else if(opt == 7){
			printf("Enter ID: ");scanf("%d", &ID);
			printf("\n\n");
			updateFaculty(ID);
			printf("\n\n");
		}
		else if(opt == 8){
			printf("Enter ID: ");scanf("%d", &ID);
			printf("\n\n");
			deleteStudent(ID);
			printf("\n\n");
		}
		else if(opt == 9){
			printf("Enter ID: ");scanf("%d", &ID);
			printf("\n\n");
			deleteFaculty(ID);
			printf("\n\n");
		}
		else{
			printf("Invalid option\n");
		}
	}
}*/

//Testing driver code
/*
int main(){
	int opt = -1;
	while(1){
		int ID;
		adminMenuDisplay();
		printf("Enter choice: ");scanf("%d", &opt);
		
		if(opt == 0) break;
		else if(opt == 1){
			addStudent();
			printf("\n\n");
		}
		else if(opt == 2){
			printf("Enter ID: ");scanf("%d", &ID);
			viewStudent(ID);
			printf("\n\n");
		}
		else if(opt == 3){
			addFaculty();
			printf("\n\n");
		}
		else if(opt == 4){
			printf("Enter ID: ");scanf("%d", &ID);
			viewFaculty(ID);
			printf("\n\n");
		}
		else if(opt == 5){
			printf("Enter ID: ");scanf("%d", &ID);
			blockStudent(ID);
			printf("\n\n");
		}
		else if(opt == 6){
			printf("Enter ID: ");scanf("%d", &ID);
			updateStudent(ID);
			printf("\n\n");
		}
		else if(opt == 7){
			printf("Enter ID: ");scanf("%d", &ID);
			updateFaculty(ID);
			printf("\n\n");
		}
		else if(opt == 8){
			printf("Enter ID: ");scanf("%d", &ID);
			deleteStudent(ID);
			printf("\n\n");
		}
		else if(opt == 9){
			printf("Enter ID: ");scanf("%d", &ID);
			deleteFaculty(ID);
			printf("\n\n");
		}
		else{
			printf("Invalid option\n");
		}
	}
}*/

