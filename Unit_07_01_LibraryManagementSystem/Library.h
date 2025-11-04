#ifndef LIBRARY_H
#define LIBRARY_H
#include "Book.h"
#include <vector>
#include <string>
class Library {
	private:
		std::vector<Book> library;
	
	public:
		Library();
		void add_book(const Book& book);
		void remove_book(const std::string& title);
		Book* find_book(const std::string& title);
		std::vector<Book> get_books_by_genre(const std::string& genre);
		std::vector<Book> get_books_by_author(const std::string& author);
		std::vector<Book> ascendingBooks();
		std::vector<Book> descendingBooks();
		void displayGenreSorted(const std::string& genre);
		void display();
};
#endif 

