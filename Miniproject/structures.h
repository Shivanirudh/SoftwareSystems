/*
Author: Shivanirudh S G
File description: Constants, Base structures for storing into files (Faculty, Student, Course, Enrollment) and their correspodning input/output functions
*/
#include<stdio.h>
#include<curses.h>
#include<ncurses.h>
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
#include<stdbool.h>
#include<termios.h>
#include<limits.h>

#define delimiter1 "-----------------------------------------"
#define delimiter2 "_____________________________________________________________________"

#define MAX_CLIENTS 5
#define PORT 8008
#define COURSE_LIMIT 10
#define SERVER_IP "127.0.0.1"

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
	int code;
	char name[50];
	char department[50];
	char faculty_name[30];
	int faculty_id;
	int credits;
	int total_seat_count;
	int available_seat_count;
	bool active;
} Course;

typedef struct Student{
	int ID;
	char name[50];
	char email[50];
	int age;
	char password[30];
	bool activated;
	bool blocked;
	bool active;
	int courseCount;
	int courses[COURSE_LIMIT];
} Student;

typedef struct Faculty{
	int ID;
	char name[50];
	char email[50];
	char password[30];
	bool activated;
	bool active;
} Faculty;

typedef struct Enrollment{
	Student s;
	Course c;
	bool active;
} Enrollment;

typedef struct Combined{
	int role;
	int action;
	int ID;
	int code;
	char choice;
	char password[1024];
	Student s;
	Faculty f;
	Course c;
} Combined;


Student inputStudent(){
	Student s;
	printf("Enter name: ");scanf(" %[^\n]", s.name);
	printf("Enter email ID: ");scanf(" %[^\n]", s.email);
	printf("Enter age: ");scanf("%d", &s.age);
	s.activated = false;
	s.blocked = false;
	s.active = true;
	s.courseCount = 0;
	
	return s;
}

Faculty inputFaculty(){
	Faculty f;
	printf("Enter name: ");scanf(" %[^\n]", f.name);
	printf("Enter email ID: ");scanf(" %[^\n]", f.email);
	f.activated = false;
	f.active = true;
	
	return f;
}

Course inputCourse(){
	Course c;
	printf("Enter course name: ");scanf(" %[^\n]", c.name);
	printf("Enter department: ");scanf(" %[^\n]", c.department);
	printf("Enter number of credits: ");scanf(" %d", &c.credits);
	printf("Enter number of seats: ");scanf(" %d", &c.total_seat_count);
	c.available_seat_count = c.total_seat_count;
	c.active = true;
	
	return c;
}

Enrollment createEnrollment(Student s, Course c){
	Enrollment e;
	e.s = s;
	e.c = c;
	e.active = true;
	
	return e;
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
	printf("Course code: %d\n", c.code);
	printf("Course name: %s\n", c.name);
	printf("Offered by: %s\n", c.department);
	printf("Handled by: Prof. %s\n", c.faculty_name);
	printf("Number of credits: %d\n", c.credits);
	printf("Total seats: %d\n", c.total_seat_count);
	printf("Available seats: %d\n", c.available_seat_count);
	printf("%s\n", delimiter1);
}


