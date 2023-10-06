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

Student addStudent(){
	Student s = inputStudent();
	return s;
}

Faculty addFaculty(){
	Faculty f = inputFaculty();
	return f;
}

Student updateStudent(int ID){
	Student s = inputStudent();
	printf("Saved successfully\n");
	return s;
}

Faculty updateFaculty(int ID){
	Faculty f = inputFaculty();
	printf("Saved successfully\n");
	return f;
}

int updateStudentDB(Student s, int action){
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
		close(student_fd);
		return s.ID;
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
		close(student_fd);
		return 0;
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
		close(student_fd);
		return 0;
	}
	else if(action == 4){
		printf("Block student\n");
		Student tmp;
		int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, (s.ID-1)*sizeof(Student), sizeof(Student)); //Acquire write lock
		printf("Fetching details...\n"); //Critical section
		fcntl(student_fd, F_SETLKW, &write_lock);
		
		lseek(student_fd, (s.ID-1)*sizeof(Student), SEEK_SET);
		read(student_fd, &tmp, sizeof(tmp));
		strcpy(s.password, tmp.password);
		s.activated = tmp.activated;
		s.active = tmp.active;
		lseek(student_fd, (-1)*sizeof(Student), SEEK_CUR);
		
		write(student_fd, &s, sizeof(s));
		write_lock.l_type = F_UNLCK;
		fcntl(student_fd, F_SETLK, &write_lock);
		printf("Saved successfully\n");
		close(student_fd);
		return 0;
	}
}

int updateFacultyDB(Faculty f, int action){
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
		close(faculty_fd);
		return f.ID;
	}
	// Update
	else if(action == 2){
		printf("Update Faculty\n");
		Faculty tmp;
		int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		printFaculty(f);
		
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
		
		printf("Fetching details...\n"); //Critical section
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
		close(faculty_fd);
		return 0;
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
		write(faculty_fd, &tmp, sizeof(tmp));
		
		write_lock.l_type = F_UNLCK;
		fcntl(faculty_fd, F_SETLK, &write_lock);
		printf("Saved successfully\n");
		close(faculty_fd);
		return 0;
	}
	else if(action == 4){
		printf("Password change Faculty\n");
		Faculty tmp;
		int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
		printFaculty(f);
		
		struct flock write_lock = getLock(F_WRLCK, SEEK_SET, 0, 0); //Acquire write lock
		
		printf("Fetching details...\n"); //Critical section
		fcntl(faculty_fd, F_SETLKW, &write_lock);
		
		lseek(faculty_fd, (f.ID-1)*sizeof(Faculty), SEEK_SET);
		read(faculty_fd, &tmp, sizeof(tmp));
		
		f.active = tmp.active;
		
		lseek(faculty_fd, (-1)*sizeof(Faculty), SEEK_CUR);
		write(faculty_fd, &f, sizeof(f));
		
		write_lock.l_type = F_UNLCK;
		fcntl(faculty_fd, F_SETLK, &write_lock);
		printf("Saved successfully\n");
		close(faculty_fd);
		return 0;
	}
}

