#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>

#define delimiter1 "-----------------------------------------"
#define delimiter2 "_____________________________________________________________________"

#define PORT_NO 8008

#define admin_file "admin_creds.bin"
#define student_file "students.bin"
#define faculty_file "faculty.bin"
#define course_file "course.bin"

#define student_course_file "student_course.bin"


typedef struct Admin{
	char ID[20];
	char password[30];
} Admin;

typedef struct Course{
	char code[4];
	char name[50];
	char department[50];
	char faculty_name[30];
} Course;

typedef struct Student{
	char ID[4];
	char name[50];
	char email[50];
	int age;
	char password[30];
	bool activated;
	bool blocked;
} Student;

typedef struct Faculty{
	char ID[4];
	char name[50];
	char email[50];
	char password[30];
	bool activated;
} Faculty;

Student inputStudent(){
	Student s;
	printf("Enter name: ");scanf(" %[^\n]", &s.name);
	printf("Enter email ID: ");scanf(" %[^\n]", &s.email);
	printf("Enter age: ");scanf("%d", &s.age);
	s.activated = false;
	s.blocked = false;
	
	return s;
}

Faculty inputFaculty(){
	Faculty f;
	printf("Enter name: ");scanf(" %[^\n]", &f.name);
	printf("Enter email ID: ");scanf(" %[^\n]", &f.email);
	f.activated = false;
	
	return f;
}

Course inputCourse(){
	Course c;
	printf("Enter course name: ");scanf(" %[^\n]", &c.name);
	printf("Enter department: ");scanf(" %[^\n]", &c.department);
	
	return c;
}

void printStudent(Student s){
	printf("Name: %s\n", s.name);
	printf("Email ID: %s\n", s.email);
	printf("Age: %d\n", s.age);
	printf("%s\n", delimiter1);
}

void printFaculty(Faculty f){
	printf("Name: %s\n", f.name);
	printf("Email ID: %s\n", f.email);
	printf("%s\n", delimiter1);
}

void printCourse(Course c){
	printf("Course code: %s\n", c.code);
	printf("Course name: %s\n", c.name);
	printf("Offered by: %s\n", c.department);
	printf("Handled by: Prof. %s\n", c.faculty_name);
	printf("%s\n", delimiter1);
}

int admin_fd = open(admin_file, O_RDWR|O_CREAT, S_IRWXU);
int student_fd = open(student_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
int faculty_fd = open(faculty_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
int course_fd = open(course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
int student_course_fd = open(student_course_file, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);


