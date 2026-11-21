/*
 * Q2 - Copy Constructors: Shallow and Deep Copies
 * Problem Context: Hospital Prescription Records
 * Student: G SAI TEJA | Register Number: 192472137
 *
 * Complete the Code: TODO 1, TODO 2, TODO 3 filled in.
 * Demonstrates deep copy constructor, copy assignment, destructor (Rule of Three).
 */

#include <iostream>
#include <string>

class Prescription {
    int n;
    std::string* medicines;   // dynamically allocated array

public:
    explicit Prescription(int count) : n(count), medicines(new std::string[count]) {}

    // TODO 1: Copy constructor — deep copy
    Prescription(const Prescription& other)
        : n(other.n), medicines(new std::string[other.n])
    {
        for (int i = 0; i < n; ++i)
            medicines[i] = other.medicines[i];
        std::cout << "  [Copy constructor called, copied " << n << " medicines]\n";
    }

    // TODO 2: Copy assignment operator — guard self-assignment, deep copy
    Prescription& operator=(const Prescription& other)
    {
        if (this != &other) {                       // guard against self-assignment
            std::string* newArr = new std::string[other.n];  // allocate new
            for (int i = 0; i < other.n; ++i)
                newArr[i] = other.medicines[i];     // copy elements
            delete[] medicines;                     // release old
            medicines = newArr;
            n = other.n;
        }
        return *this;
    }

    ~Prescription()
    {
        // TODO 3: Release the dynamically allocated array
        delete[] medicines;
    }

    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
    int size() const { return n; }
};

int main()
{
    std::cout << "=== Q2: Hospital Prescription Records ===\n\n";

    Prescription p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");

    std::cout << "Creating q via copy constructor (q = p):\n";
    Prescription q = p;        // copy constructor
    q.set(1, "Cetirizine");

    Prescription r(1);
    std::cout << "Assigning r = p (copy assignment):\n";
    r = p;                     // copy assignment

    p.set(0, "Ibuprofen");

    std::cout << "\nResult:\n";
    std::cout << p.get(0) << ' ' << p.get(1) << " | "
              << q.get(0) << ' ' << q.get(1) << " | "
              << r.get(0) << ' ' << r.get(1) << '\n';

    // Expected output:
    // Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS

    std::cout << "\n=== Self-assignment test (p = p) ===\n";
    p = p;
    std::cout << "After self-assignment: " << p.get(0) << ' ' << p.get(1) << "\n";

    std::cout << "\n=== Pass by value vs const reference ===\n";
    // Pass by value triggers copy constructor
    auto byValue = [](Prescription pr) {
        std::cout << "  Inside byValue: " << pr.get(0) << "\n";
    };
    // Pass by const reference — no copy
    auto byRef = [](const Prescription& pr) {
        std::cout << "  Inside byRef:   " << pr.get(0) << "\n";
    };

    std::cout << "Passing p by value:\n";
    byValue(p);
    std::cout << "Passing p by const reference:\n";
    byRef(p);

    // ---------- Memory Diagram ----------
    std::cout << "\n=== Memory Diagrams (ASCII) ===\n";
    std::cout << R"(
SHALLOW COPY (before fix — both point to same array):
  p: [n=2] [medicines*]---+
                           \
                            +--> ["Paracetamol", "ORS"]
                           /
  q: [n=2] [medicines*]---+     <-- DANGER: double delete!

DEEP COPY (after fix — independent arrays):
  p: [n=2] [medicines*]-------> ["Paracetamol", "ORS"]

  q: [n=2] [medicines*]-------> ["Paracetamol", "ORS"]  (independent copy)
)";

    std::cout << "\n=== Rule of Three ===\n";
    std::cout << "If a class needs a custom destructor, copy constructor, or\n";
    std::cout << "copy assignment operator, it likely needs ALL THREE.\n";
    std::cout << "Reason: if you manage a resource (dynamic memory), all three\n";
    std::cout << "must handle it correctly to prevent leaks/double-deletes.\n";

    std::cout << "\n=== Three situations where copy constructor is called ===\n";
    std::cout << "1. Initialising a new object from an existing one (Prescription q = p;)\n";
    std::cout << "2. Passing an object by value to a function\n";
    std::cout << "3. Returning an object by value from a function (unless RVO/NRVO)\n";

    return 0;
}
