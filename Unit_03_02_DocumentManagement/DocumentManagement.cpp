#include <iostream>
#include <memory>

class Document {
private:
	std::string title;
	std::unique_ptr <std::string> content;

public:

	Document(const std::string& t, const std::string& c) : title(t), content(std::make_unique<std::string>(c)) {	
		std::cout <<"Document " << '\"' << title << '\"' <<" created\n";
	}

	~Document() {
		std::cout << "Document \"" << title << "\" destroyed" << std::endl;
	}

	void show() {
		std::cout << "Title: " << title << '\n' << "Content: " << *content << std::endl;
	}

	void edit(const std::string& c) {
		*content = c;
	}
	
};

std::unique_ptr<Document> createDocument(const std::string& t, const std::string& c) {
	return std::make_unique<Document>(t, c);
}

std::shared_ptr<Document> createSharedDocument(const std::string& t, const std::string& c) {
	return std::make_shared<Document>(t, c);
}


int main() {

	std::cout << "===Unique_ptr demonstration===\n";
	std::unique_ptr<Document> doc1 = createDocument("My Unique Doc", "Initial content");
	doc1->show();

	std::unique_ptr<Document> doc2 = std::move(doc1);
	if (!doc1) {
		std::cout << "doc1 no longer owns the documentation\n";
	}
	doc2->edit("Updated content");
	doc2->show();
	std::cout << "\n=== Shared_ptr demonstration ===\n";
	std::shared_ptr<Document> sharedDoc = createSharedDocument("Shared Doc", "Shared content");
	{
		std::shared_ptr<Document>viewer = sharedDoc;
		viewer->show();
		viewer->edit("Edited by viewer");
	}
	sharedDoc->show();
	std::cout << "End of main()\n";
	doc1.reset();
	doc2.reset();
	
	return 0;
}
