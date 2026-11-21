/**
 * Q1 - BookRecords.cpp
 * Constructors, Constructor Overloading, and Member Initializers (Library Book Records)
 * Student: G SAI TEJA (192472137)
 */

#include <iostream>
#include <string>
#include <stdexcept>

class Book {
private:
    std::string isbn;
    std::string title;
    int copies;

public:
    // 1. Delegating Default Constructor
    Book() : Book("000-000", "Untitled", 0) {}

    // 2. Overloaded Constructor for donated book (title only)
    explicit Book(const std::string& t) : Book("DONATED", t, 1) {}

    // 3. Primary Parameterized Constructor with Member Initializer List and Validation
    Book(const std::string& i, const std::string& t, int c)
        : isbn(i), title(t), copies(c < 0 ? 0 : c) {
        if (c < 0) {
            std::cout << "[Warning] Negative copies passed for \"" << t << "\". Reset to 0.\n";
        }
    }

    void print() const {
        std::cout << "Book [ISBN: " << isbn << ", Title: \"" << title 
                  << "\", Copies: " << copies << "]\n";
    }
};

void showExplicitRequirement(const Book& b) {
    b.print();
}

int main() {
    std::cout << "CO3 - Q1: CONSTRUCTORS & CONSTRUCTOR OVERLOADING\n";
    std::cout << "Student: G SAI TEJA (192472137)\n";
    std::cout << "----------------------------------------------\n\n";

    std::cout << "1. Creating Book with Default Constructor:\n";
    Book b1;
    b1.print();

    std::cout << "\n2. Creating Donated Book (Delegated Constructor):\n";
    Book b2("Data Structures in C++");
    b2.print();

    std::cout << "\n3. Creating Fully Specified Book:\n";
    Book b3("978-0131103627", "The C++ Programming Language", 15);
    b3.print();

    std::cout << "\n4. Invariant Validation (Testing Negative Copies):\n";
    Book b4("978-0000000000", "Faulty Record", -5);
    b4.print();

    std::cout << "\n5. Explicit Constructor Verification:\n";
    // showExplicitRequirement("A Title"); // Error if explicit! Requires explicit Book construction:
    showExplicitRequirement(Book("Explicit Title Demonstration"));

    return 0;
}
