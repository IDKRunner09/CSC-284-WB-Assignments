#include "Student.h"
#include <string>
#include <format>
using namespace std;
int Student::id = 100;

Student::Student(string name, int age, string major, string mail) {
	this->studentID = ++id;
	this->name = name;
	this->age = age;
	this->major = major;
	this->email = mail;
}

int Student::getStudentId() const {
	return studentID;
}

const string& Student::getName() const {
	return name;
}

string Student::getEmail() const {
	return email;
}

int Student::getAge() const {
	return age;
}

string Student::getmajor() const {
	return major;
}

string Student::details() const {
	return  std::format("{:<6} {:<20} {:<5} {:<12} {:<25}\n", studentID, name, age, major, email);
}