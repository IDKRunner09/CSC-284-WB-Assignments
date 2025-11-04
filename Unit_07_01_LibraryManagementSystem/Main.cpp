#include <iostream>
#include "Book.h";
#include "Library.h"

int main() {
	Library library;
	library.add_book(Book("Chanakya Niti", "Chanakya", -275, "Non-Fiction"));
	library.add_book(Book("Rangila Rasul", "M. A. Chamupati", 1924, "Non-Fiction"));
	library.add_book(Book("Percy Jackson: The Lightning Theif", "Rick Riordan", 2011, "Fantasy"));
	library.add_book(Book("Steve Jobs", "Walter Isaacson", 2011, "Biography"));
	library.add_book(Book("India, That Is Bharat", "Sai Deepak", 2021, "Non-Fiction"));
	library.add_book(Book("The Great Gatsby", "F. Scott Fitzgeral", 1925, "Classic"));
	library.add_book(Book("Harry Potter and the Sorcerer's Stone", "J.K. Rowling", 1997, "Fantasy"));
	library.add_book(Book("Harry Potter and the Order of the Phoenix", "J.K. Rowling", 2003, "Fantasy"));
	library.add_book(Book("Harry Potter and the Half-Blood Prince", "J.K. Rowling", 2005, "Fantasy"));
	library.add_book(Book("Harry Potter and the Deathly Hallows", "J.K. Rowling", 2007, "Fantasy"));
	library.add_book(Book("Harry Potter and the Chamber of Secrets", "J.K. Rowling", 1998, "Fantasy"));
	library.add_book(Book("Harry Potter and the Prisoner of Azkaban", "J.K. Rowling", 1999, "Fantasy"));
	library.add_book(Book("Harry Potter and the Goblet of Fire", "J.K. Rowling", 2000, "Fantasy"));
	
	//removing a book
	library.remove_book("Steve Jobs");
	//finding a book
	library.find_book("Rangila Rasul");
	library.find_book("Steve Jobs");
	//testing find by author 
	std::cout << "\n\nBooks by J.K. Rowling\n";
	std::vector<Book> rowlingBooks = library.get_books_by_author("J.K. Rowling");
	for (auto& b : rowlingBooks) { b.display(); }
	//testing find by genre
	std::cout << "\n\nNon-Fiction Books\n";
	std::vector<Book> nonfictionBooks = library.get_books_by_genre("Non-Fiction");
	for (auto& b : nonfictionBooks) { b.display(); }
	//ascending book order
	std::vector<Book> ascendingSortedBooksByYear = library.ascendingBooks();
	std::cout << "\n\nAscending Books: \n";
	for (auto& b : ascendingSortedBooksByYear) { b.display(); }
	//descending books order
	std::vector<Book> descendingSortedBooksByYear = library.descendingBooks();
	std::cout << "\n\nDescending Books: \n";
	for (auto& b : descendingSortedBooksByYear) { b.display(); }
	//sorted fantasy books
	std::cout << "\n\nFantasy sorted books\n";
	library.displayGenreSorted("Fantasy");
}
