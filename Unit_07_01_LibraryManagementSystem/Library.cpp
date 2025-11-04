#include "Library.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <ranges>


Library::Library() {}

void Library::add_book(const Book& book) {
	library.push_back(book);
	std:: cout << std::endl;
}

void Library::remove_book(const std::string& title) {
	std::vector<Book>::iterator book = std::remove_if(library.begin(), library.end(), [&](Book& b) { return b.getTitle() == title; });

	if (book != library.end()) {
		library.erase(book, library.end());
		std::cout << "Book Erased: " << title << std::endl;
	} else {
		std::cout << title << " not found\n";
	}
}

Book* Library::find_book(const std::string& title) {
	std::vector<Book>::iterator book = std::find_if(library.begin(), library.end(), [&](Book& b) { return b.getTitle() == title; });
	if (book == library.end()) {
		std::cout << "Error: " << title << " not found\n";
		return nullptr;
	} else {
		std::cout << "Book: " << title << " found\n";
		return &(*book);
	}
}

std::vector<Book> Library::get_books_by_author(const std::string& author) {
	std::vector<Book> new_library;
	for (std::vector<Book>::iterator it = library.begin(); it < library.end(); ++it) {
		if (it->getAuthor() == author) {
			new_library.push_back(*it);
		}
	}
	return new_library;
}

std::vector<Book> Library::get_books_by_genre(const std::string& genre) {
	auto filter = library | std::views::filter([&](const Book& book) {return book.getGenre() == genre; });
	std::vector<Book> new_library(filter.begin(), filter.end());
	return new_library;

}

std::vector<Book> Library::ascendingBooks() {
	std::vector<Book> sortedBook = library;
	std::sort(sortedBook.begin(), sortedBook.end(), 
		[](const Book& a, const Book& b) { return a.getYear() < b.getYear(); });
	return sortedBook;
}

std::vector<Book> Library::descendingBooks() {
	std::vector<Book> sortedBook = library;
	std::sort(sortedBook.begin(), sortedBook.end(), 
		[](const Book& a, const Book& b) {return a.getYear() > b.getYear(); });
	return sortedBook;
}

void Library::display() {
	for (std::vector<Book>::iterator book = library.begin(); book != library.end(); ++book) {
		book->display();
	}
} 

void Library::displayGenreSorted(const std::string& genre) {
	auto filtered = library | std::views::filter([&](const Book& b) { return b.getGenre() == genre; });
	std::vector<Book> sorted(filtered.begin(), filtered.end());
	std::ranges::sort(sorted, {}, &Book::getYear);

	auto displayStrings = sorted | std::views::transform([](const Book& b) {
		return b.getTitle() + " (" + std::to_string(b.getYear()) + ")" ;});
	for (const auto& b : displayStrings) {
		std::cout << b << std::endl;
	}
	
}