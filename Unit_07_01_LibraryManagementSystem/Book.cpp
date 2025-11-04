#include "Book.h"
#include <iostream>

Book::Book::Book(const std::string t, const std::string a, int y, const std::string g) :
title(t), author(a), year(y), genre(g) {}

std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
int Book::getYear() const { return year; }
std::string Book::getGenre() const { return genre; }

void Book::display() const {
	std::cout << title << " by " << author << " (" << year << ") [" << genre << "]\n";
}