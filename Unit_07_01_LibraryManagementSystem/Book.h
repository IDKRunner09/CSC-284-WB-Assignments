#ifndef BOOK_H
#define BOOK_H
#include <string>
class Book
{
	private:
		std::string title;
		std::string	author;
		std::string genre;
		int year;

	public:
		Book(const std::string t, const std::string a, int y, const std::string g);
		std::string getTitle() const;
		std::string getAuthor() const;
		int getYear() const;
		std::string getGenre() const;
		void display() const;
};

#endif

