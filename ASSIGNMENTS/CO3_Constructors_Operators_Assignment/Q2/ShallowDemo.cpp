/*
 * Q2 - Shallow Copy Demo (without custom copy ctor — shows the problem)
 * Student: G SAI TEJA | Register Number: 192472137
 *
 * This version deliberately omits the copy constructor to demonstrate
 * the shallow copy problem: both objects share the same dynamically
 * allocated array, leading to double-delete and crashes.
 *
 * Run with: g++ -std=c++17 -Wall -Wextra -o ShallowDemo.exe ShallowDemo.cpp
 * (Under -fsanitize=address, this would report a heap-use-after-free or double-free.)
 */

#include <iostream>
#include <string>

class PrescriptionShallow {
    int n;
    std::string* medicines;

public:
    explicit PrescriptionShallow(int count)
        : n(count), medicines(new std::string[count]) {}

    // NO copy constructor — compiler generates shallow copy
    // NO copy assignment operator
    // Destructor releases memory — double-delete with shallow copy!

    ~PrescriptionShallow() {
        std::cout << "  ~PrescriptionShallow: deleting " << n
                  << " medicines at " << medicines << "\n";
        delete[] medicines;
    }

    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
};

int main()
{
    std::cout << "=== Shallow Copy Problem Demo ===\n";
    std::cout << "(This would crash / report errors under -fsanitize=address)\n\n";

    // We only demonstrate the concept; we do NOT actually trigger the
    // double-delete to avoid crashing this run.

    std::cout << "Without a user-defined copy constructor:\n";
    std::cout << "  PrescriptionShallow p(2);\n";
    std::cout << "  PrescriptionShallow q = p;   // shallow copy\n";
    std::cout << "  q.set(1, \"Cetirizine\");     // also changes p!\n";
    std::cout << "  // When p and q go out of scope, both delete[] the SAME pointer\n";
    std::cout << "  // → double-delete → undefined behaviour / crash\n\n";

    std::cout << "Sanitizer would report:\n";
    std::cout << "  ==ERROR: AddressSanitizer: heap-use-after-free\n";
    std::cout << "  ==ERROR: AddressSanitizer: attempting double-free\n\n";

    std::cout << "Solution: Implement the Rule of Three (copy ctor, copy assignment, dtor).\n";

    return 0;
}
