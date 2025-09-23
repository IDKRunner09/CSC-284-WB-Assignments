#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class Student {
private: 
	static int id;
	int studentID, age;
	string name, major, email;

public:
	Student(string name, int age, string major, string mail);
	int getStudentId() const;
	const string& getName() const;
	string getEmail() const;
	string getmajor() const;
	int getAge() const;
	string details() const;
};
#endif 
