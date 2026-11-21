/*
 * Q1 - Alternative Concept: Single constructor with default arguments
 * Student: G SAI TEJA | Register Number: 192472137
 *
 * Instead of multiple overloaded constructors + delegation,
 * use ONE constructor with default arguments.
 *
 * Time Complexity of object creation: O(1) in both designs
 * Space Complexity: O(1) in both designs — same member storage
 *
 * Comparison:
 *   Overloaded + delegation: validation in primary ctor only (1 place)
 *   Default args: validation in the single ctor (1 place) — same
 *   Default args is simpler but less flexible for different init logic.
 */

#include <iostream>
#include <string>

class Book {
    std::string title;
    std::string author;
    int copies;

public:
    // Single constructor with default arguments
    Book(const std::string& t = "Untitled",
         const std::string& a = "Unknown",
         int c = 0)
        : title(t), author(a), copies(c < 0 ? 0 : c)
    {
        if (c < 0)
            std::cout << "  [Warning] Negative copies corrected to 0 for \""
                      << title << "\"\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Book& b)
    {
        os << "[\"" << b.title << "\" by " << b.author
           << ", copies=" << b.copies << "]";
        return os;
    }
};

int main()
{
    std::cout << "=== Alternative: Single Constructor with Default Args ===\n\n";

    Book b1;                                           // all defaults
    Book b2("Donated Book");                           // title only
    Book b3("C++ Primer", "Lippman", 3);               // all specified
    Book b4("Bad Record", "Staff", -5);                 // negative copies

    std::cout << "b1 = " << b1 << "\n";
    std::cout << "b2 = " << b2 << "\n";
    std::cout << "b3 = " << b3 << "\n";
    std::cout << "b4 = " << b4 << "\n\n";

    std::cout << "=== Complexity Comparison ===\n";
    std::cout << "Overloaded ctors + delegation:  O(1) time, O(1) space per object\n";
    std::cout << "Single ctor with default args:  O(1) time, O(1) space per object\n";
    std::cout << "Both validate copies in exactly 1 place.\n";
    std::cout << "Default-args version is simpler; overloaded version allows\n";
    std::cout << "different initialisation logic per constructor.\n";

    return 0;
}
