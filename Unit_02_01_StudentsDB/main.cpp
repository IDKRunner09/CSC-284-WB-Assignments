#include "Student.h"
#include <iostream>
#include <vector>
#include <string>
#include <format>
#include <map>
#include <string_view>

using namespace std;

int IntroCommand() {
	int command;
	std::cout << "Acceptable Commands:\n"
		<< "1. Add student\n"
		<< "2. Display all students\n"
		<< "3. Search by id\n"
		<< "4. Search by name\n"
		<< "5. Count by major\n"
		<< "6. quit\n"
		<< "Command: ";
	cin >> command;
	cout << endl;
	return command;
}

void AddStudent(vector<Student>& database) {
	string name, major, email;
	int age;

	std::cout << "Enter Name: ";
	cin.ignore();
	getline(cin, name);
	std::cout << "Enter age: ";
	cin >> age;
	std::cout << "Enter major: ";
	cin.ignore();
	getline(cin, major);
	std::cout << "Enter email: ";
	cin.ignore();
	getline(cin, email);
	database.push_back(Student(name, age, major, email));
}

void PrintData(vector<Student>& database) {
	if (database.empty()) {
		cout << "No student in data\n";
		return;
	}
	else {
		std::cout << std::format("{:<6} {:<20} {:<5} {:<12} {:<25}\n",
			"ID", "Name", "Age", "Major", "Email");
		std::cout << format("---------------------------------------------------------------------\n");
		for (Student& student : database) {
			std::cout << student.details();
		}
		std::cout << endl;
	}
}

void SearchByID(vector<Student>& database) {
	if (database.empty()) {
		std::cout << "No students in database.\n";
		return;
	}
	else {
		bool found = false;
		int id;
		std::cout << "Enter the id of the student you want to find: ";
		cin >> id;
		for (Student& student : database) {
			if (student.getStudentId() == id) {
				std::cout << "\nStudent found:\n";
				std::cout << student.details() << endl << endl;
				found = true;
			}
		}
		if (found == false) {
			std::cout << "Student not found\n\n";
		}
	}
}

void SearchByName(vector<Student>& database) {
	if (database.empty()) {
		std::cout << "No students in database.\n";
		return;
	}
	else {
		string name;
		std::cout << "Enter the full name: ";
		cin.ignore();
		getline(cin, name);
		bool found = false;
		std::string_view searchView(name);

		for (const Student& student : database) {
			std::string_view searchNameView(student.getName());
			if (searchNameView == searchView) {
				found = true;
				cout << "\nStudent found: \n";
				cout << student.details();
			}
		}
		if (!found) {
			cout << "Student not found\n";
		}
		cout << endl;
	}
}

void GroupByMajor(vector<Student>& database) {
	if (database.empty()) {
		std::cout << "No students in database.\n";
		return;
	}
	else {
		string major = "";
		int count = 0;
		map<string, int> countMajor;

		for (const Student& s : database) {
			countMajor[s.getmajor()]++; 
		}

		printf("Major Count :\n");
		for (const auto& [major, count] : countMajor) {
			cout << major << '\t' << count << endl;
		}
	}
}

int main() {
	int command;
	vector<Student> database;

	do {
		command = IntroCommand();
		switch (command) {
		case 1: {
			AddStudent(database);
			printf("student added\n");
			cout << endl;
			break;
		}
		case 2: {
			PrintData(database);
			break;
		}
		case 3: {
			SearchByID(database);
			break;
		}
		case 4: {
			SearchByName(database);
			break;
		}
		case 5: 
		{
			GroupByMajor(database);
			break;
		}
		}

	} while(command != 6);
	cout << "Exited the program!" << endl;
}